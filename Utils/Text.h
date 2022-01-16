#ifndef __TEXT_H
#define __TEXT_H

#include "items.h"
#include "types.h"
#include "mapscreen.h"
#include "XML_Language.h"

#define STRING_LENGTH 255

enum
{
	//TCTL_MSG__RANGE_TO_TARGET,
	TCTL_MSG__ATTACH_TRANSMITTER_TO_LAPTOP,
	TACT_MSG__CANNOT_AFFORD_MERC,
	TACT_MSG__AIMMEMBER_FEE_TEXT,
	TACT_MSG__AIMMEMBER_ONE_TIME_FEE,
	TACT_MSG__FEE,
	TACT_MSG__SOMEONE_ELSE_IN_SECTOR,
	//TCTL_MSG__GUN_RANGE_AND_CTH,
	TCTL_MSG__DISPLAY_COVER,
	TCTL_MSG__LOS,
	TCTL_MSG__INVALID_DROPOFF_SECTOR,
	TCTL_MSG__PLAYER_LOST_SHOULD_RESTART,
	TCTL_MSG__JERRY_BREAKIN_LAPTOP_ANTENA,
	TCTL_MSG__END_GAME_POPUP_TXT_1,
	TCTL_MSG__END_GAME_POPUP_TXT_2,
	TCTL_MSG__IRON_MAN_CANT_SAVE_NOW,
	TCTL_MSG__CANNOT_SAVE_DURING_COMBAT,
	TCTL_MSG__CAMPAIGN_NAME_TOO_LARGE,
	TCTL_MSG__CAMPAIGN_DOESN_T_EXIST,
	TCTL_MSG__DEFAULT_CAMPAIGN_LABEL,
	TCTL_MSG__CAMPAIGN_LABEL,
	TCTL_MSG__NEW_CAMPAIGN_CONFIRM,
	TCTL_MSG__CANT_EDIT_DEFAULT,
	TCTL_MSG__SOFT_IRON_MAN_CANT_SAVE_NOW,
	TCTL_MSG__EXTREME_IRON_MAN_CANT_SAVE_NOW,

};
//Ja25 UB
//enums used for zNewLaptopMessages
enum
{
	LPTP_MSG__MERC_SPECIAL_OFFER,
	LPTP_MSG__TEMP_UNAVAILABLE,
	LPTP_MSG__PREVIEW_TEXT,
};

extern	CHAR16	XMLTacticalMessages[1000][MAX_MESSAGE_NAMES_CHARS];

//Encyclopedia
extern STR16 	pMenuStrings[];
extern STR16	pLocationPageText[];
extern STR16	pSectorPageText[];
extern STR16	pEncyclopediaHelpText[];
extern STR16	pEncyclopediaTypeText[];
extern STR16	pEncyclopediaSkrotyText[];
extern STR16	pEncyclopediaFilterLocationText[];
extern STR16	pEncyclopediaSubFilterLocationText[];
extern STR16	pEncyclopediaFilterCharText[];
extern STR16	pEncyclopediaSubFilterCharText[];
extern STR16	pEncyclopediaFilterItemText[];
extern STR16	pEncyclopediaSubFilterItemText[];
extern STR16	pEncyclopediaFilterQuestText[];
extern STR16	pEncyclopediaSubFilterQuestText[];
extern STR16	pEncyclopediaShortCharacterText[];
extern STR16	pEncyclopediaHelpCharacterText[];
extern STR16	pEncyclopediaShortInventoryText[];
extern STR16	BoxFilter[];
extern STR16	pOtherButtonsText[];
extern STR16	pOtherButtonsHelpText[];
extern STR16	QuestDescText[];
extern STR16	FactDescText[];

//Editor
//Editor Taskbar Creation.cpp
extern STR16 iEditorItemStatsButtonsText[];
extern STR16 FaceDirs[8];
extern STR16 iEditorMercsToolbarText[];
extern STR16 iEditorBuildingsToolbarText[];
extern STR16 iEditorItemsToolbarText[];
extern STR16 iEditorMapInfoToolbarText[];
extern STR16 iEditorOptionsToolbarText[];
extern STR16 iEditorTerrainToolbarText[];
extern STR16 iEditorTaskbarInternalText[];
//Editor Taskbar Utils.cpp
extern STR16 iRenderMapEntryPointsAndLightsText[];
extern STR16 iBuildTriggerNameText[];
extern STR16 iRenderDoorLockInfoText[];
extern STR16 iRenderEditorInfoText[];
//EditorBuildings.cpp
extern STR16 iUpdateBuildingsInfoText[];
extern STR16 iRenderDoorEditingWindowText[];
//EditorItems.cpp
extern STR16 pInitEditorItemsInfoText[];
extern STR16 pDisplayItemStatisticsTex[];
extern STR16 pUpdateMapInfoText[];
//EditorMercs.cpp
extern CHAR16 gszScheduleActions[ 11 ][20];	// NUM_SCHEDULE_ACTIONS = 11
extern STR16 zDiffNames[5];	// NUM_DIFF_LVLS = 5
extern STR16 EditMercStat[12];
extern STR16 EditMercOrders[8];
extern STR16 EditMercAttitudes[6];
extern STR16 pDisplayEditMercWindowText[];
extern STR16 pCreateEditMercWindowText[];
extern STR16 pDisplayBodyTypeInfoText[];
extern STR16 pUpdateMercsInfoText[];
extern CHAR16 pRenderMercStringsText[][100];
extern STR16 pClearCurrentScheduleText[];
extern STR16 pCopyMercPlacementText[];
extern STR16 pPasteMercPlacementText[];
//editscreen.cpp
extern STR16 pEditModeShutdownText[];
extern STR16 pHandleKeyboardShortcutsText[];
extern STR16 pPerformSelectedActionText[];
extern STR16 pWaitForHelpScreenResponseText[];
extern STR16 pAutoLoadMapText[];
extern STR16 pShowHighGroundText[];
//Item Statistics.cpp
//extern CHAR16 gszActionItemDesc[ 34 ][ 30 ];	// NUM_ACTIONITEMS = 34
extern STR16 pUpdateItemStatsPanelText[];
extern STR16 pSetupGameTypeFlagsText[];
extern STR16 pSetupGunGUIText[];
extern STR16 pSetupArmourGUIText[];
extern STR16 pSetupExplosivesGUIText[];
extern STR16 pSetupTriggersGUIText[];
//Sector Summary.cpp
extern STR16 pCreateSummaryWindowText[];
extern STR16 pRenderSectorInformationText[];
extern STR16 pRenderItemDetailsText[];
extern STR16 pRenderSummaryWindowText[];
extern STR16 pUpdateSectorSummaryText[];
extern STR16 pSummaryLoadMapCallbackText[];
extern STR16 pReportErrorText[];
extern STR16 pRegenerateSummaryInfoForAllOutdatedMapsText[];
extern STR16 pSummaryUpdateCallbackText[];
extern STR16 pApologizeOverrideAndForceUpdateEverythingText[];
//selectwin.cpp
extern STR16 pDisplaySelectionWindowGraphicalInformationText[];
extern STR16 pDisplaySelectionWindowButtonText[];
//Cursor Modes.cpp
extern STR16 wszSelType[6];
//--

extern	STR16	gzNewLaptopMessages[];
extern	STR16	zNewTacticalMessages[];
extern CHAR16  gszAimPages[ 6 ][ 20 ];
extern CHAR16  zGrod[][500];
extern STR16 pCreditsJA2113[];
extern CHAR16 ShortItemNames[MAXITEMS][80];
extern CHAR16 ItemNames[MAXITEMS][80];
extern CHAR16 AmmoCaliber[MAXITEMS][20];
extern CHAR16 BobbyRayAmmoCaliber[MAXITEMS][20];
extern CHAR16 WeaponType[MAXITEMS][30];

extern CHAR16 Message[][STRING_LENGTH];
extern CHAR16 TeamTurnString[][STRING_LENGTH];
extern STR16 pMilitiaControlMenuStrings[]; //lal
extern STR16 pTraitSkillsMenuStrings[];		//Flugente
extern STR16 pTraitSkillsMenuDescStrings[]; //Flugente
extern STR16 pTraitSkillsDenialStrings[]; //Flugente

enum
{
	SKILLMENU_MILITIA,
	SKILLMENU_OTHERSQUADS,
	SKILLMENU_CANCEL,
	SKILLMENU_X_MILITIA,
	SKILLMENU_ALL_MILITIA,
	SKILLMENU_MORE,
	SKILLMENU_CORPSES,
};

extern STR16 pSkillMenuStrings[];			//Flugente
//extern STR16 pTalkToAllMenuStrings[];
extern STR16 pSnitchMenuStrings[];
extern STR16 pSnitchMenuDescStrings[];
extern STR16 pSnitchToggleMenuStrings[];
extern STR16 pSnitchToggleMenuDescStrings[];
extern STR16 pSnitchSectorMenuStrings[];
extern STR16 pSnitchSectorMenuDescStrings[];
extern STR16 pPrisonerMenuStrings[];
extern STR16 pPrisonerMenuDescStrings[];
extern STR16 pSnitchPrisonExposedStrings[];
extern STR16 pSnitchGatheringRumoursResultStrings[];
extern STR16 pAssignMenuStrings[];
extern STR16 pTrainingStrings[];
extern STR16 pTrainingMenuStrings[];
extern STR16 pAttributeMenuStrings[];
extern STR16 pVehicleStrings[];
extern STR16 pShortAttributeStrings[];
extern STR16 pLongAttributeStrings[];
extern STR16 pContractStrings[];
extern STR16 pAssignmentStrings[];
extern STR16 pConditionStrings[];
extern CHAR16 pCountryNames[][MAX_TOWN_NAME_LENGHT];
extern CHAR16 pTownNames[MAX_TOWNS][MAX_TOWN_NAME_LENGHT];	// Lesh: look mapscreen.h for definitions
extern STR16 pPersonnelScreenStrings[];
extern STR16 pPersonnelRecordsHelpTexts[]; // added by SANDRO
extern STR16 pPersonnelTitle[];
extern STR16 pUpperLeftMapScreenStrings[];
extern STR16 pTacticalPopupButtonStrings[];
extern STR16 pSquadMenuStrings[];
extern STR16 pDoorTrapStrings[];
extern STR16 pLongAssignmentStrings[];
extern STR16 pContractExtendStrings[];
extern STR16 pMapScreenMouseRegionHelpText[];
extern STR16 pPersonnelAssignmentStrings[];
extern STR16 pNoiseVolStr[];
extern STR16 pNoiseTypeStr[];
extern STR16 pDirectionStr[];
extern STR16 pRemoveMercStrings[];
extern STR16 sTimeStrings[];
extern STR16 pLandTypeStrings[];
extern STR16 pGuardMenuStrings[];
extern STR16 pOtherGuardMenuStrings[];
extern STR16 pInvPanelTitleStrings[];
extern STR16 pPOWStrings[];
extern STR16 pMilitiaString[];
extern STR16 pMilitiaButtonString[];
extern STR16 pEpcMenuStrings[];

extern STR16 pRepairStrings[];
extern STR16 sPreStatBuildString[];
extern STR16 sStatGainStrings[];
extern STR16 pHelicopterEtaStrings[];
extern STR16 pHelicopterRepairRefuelStrings[];
extern STR16 sMapLevelString[];
extern STR16 gsLoyalString[];
extern STR16 pMapHeliErrorString[];
extern STR16 gsUndergroundString[];
extern STR16 gsTimeStrings[];
extern STR16 sFacilitiesStrings[];
extern STR16 pMapPopUpInventoryText[];
extern STR16 pwTownInfoStrings[];
extern STR16 pwMineStrings[];
extern STR16 pwMiscSectorStrings[];
extern STR16 pMapInventoryErrorString[];
extern STR16 pMapInventoryStrings[];
extern STR16 pMapScreenFastHelpTextList[];
extern STR16 pMovementMenuStrings[];
extern STR16 pUpdateMercStrings[];
extern STR16 pMapScreenBorderButtonHelpText[];
extern STR16 pMapScreenInvenButtonHelpText[];
extern STR16 pMapScreenBottomFastHelp[];
extern STR16 pMapScreenBottomText[];
extern STR16 pMercDeadString[];
extern CHAR16 pSenderNameList[500][128];
extern STR16 pTraverseStrings[];
extern STR16 pNewMailStrings[];
extern STR16 pDeleteMailStrings[];
extern STR16 pEmailHeaders[];
extern STR16 pEmailTitleText[];
extern STR16 pFinanceTitle[];
extern STR16 pFinanceSummary[];
extern STR16 pFinanceHeaders[];
extern STR16 pTransactionText[];
extern STR16 pTransactionAlternateText[];
extern STR16 pMoralStrings[];
extern STR16 pSkyriderText[];
extern STR16 pMercFellAsleepString[];
extern STR16 pLeftEquipmentString[]; 
extern STR16 pMapScreenStatusStrings[];
extern STR16 pMapScreenPrevNextCharButtonHelpText[];
extern STR16 pEtaString[];
extern STR16 pShortVehicleStrings[];
extern STR16 pTrashItemText[];
extern STR16 pMapErrorString[];
extern STR16 pMapPlotStrings[];
extern STR16 pMiscMapScreenMouseRegionHelpText[];
extern STR16 pMercHeLeaveString[];
extern STR16 pMercSheLeaveString[];
extern STR16 pImpPopUpStrings[];
extern STR16 pImpButtonText[];
extern STR16 pExtraIMPStrings[];
extern STR16 pFilesTitle[];
extern STR16 pFilesSenderList[];
extern STR16 pHistoryLocations[];
//extern STR16 pHistoryAlternateStrings[];
//extern STR16 pHistoryStrings[];		// Externalized to "TableData\History.xml"
extern STR16 pHistoryHeaders[];
extern STR16 pHistoryTitle[];
extern STR16 pShowBookmarkString[];
extern STR16 pWebPagesTitles[];
extern STR16 pWebTitle[ ];
extern STR16 pPersonnelString[];
extern STR16 pErrorStrings[];
extern STR16 pDownloadString[];
extern STR16 pBookmarkTitle[];
extern STR16 pBookMarkStrings[];
extern STR16 pLaptopIcons[];
extern STR16 sATMText[ ];
extern STR16 gsAtmStartButtonText[];
extern STR16 gsAtmSideButtonText[];
extern STR16 pDownloadString[];
extern STR16 pPersonnelTeamStatsStrings[];
extern STR16 pPersonnelCurrentTeamStatsStrings[];
extern STR16 pPersonelTeamStrings[];
extern STR16 pPersonnelDepartedStateStrings[];
extern STR16 pMapHortIndex[];
extern STR16 pMapVertIndex[];
extern STR16 pMapDepthIndex[];
//extern STR16 sCritLocationStrings[];
//extern STR16 sVehicleHit[ ];
extern STR16 pLaptopTitles[];
extern STR16 pDayStrings[];
extern STR16 pMercContractOverStrings[];
extern STR16 pMilitiaConfirmStrings[];
extern STR16 pDeliveryLocationStrings[];
extern STR16 pSkillAtZeroWarning[];
extern STR16 pIMPBeginScreenStrings[];
extern STR16 pIMPFinishButtonText[1];
extern STR16 pIMPFinishStrings[];
extern STR16 pIMPVoicesStrings[];
extern STR16 pDepartedMercPortraitStrings[];
extern STR16 pPersTitleText[];
extern STR16 pPausedGameText[];
extern STR16	zOptionsToggleText[];
extern STR16	zOptionsScreenHelpText[];
extern STR16 pDoctorWarningString[];
extern STR16 pMilitiaButtonsHelpText[];
extern STR16 pMapScreenJustStartedHelpText[];
extern STR16 pLandMarkInSectorString[];
extern STR16 gzMercSkillText[];
extern STR16 gzMercSkillTextNew[]; // added by SANDRO
extern STR16 gzNonPersistantPBIText[];
extern STR16 gzMiscString[];

extern STR16 wMapScreenSortButtonHelpText[];
extern STR16 pNewNoiseStr[];
extern STR16 pTauntUnknownVoice[]; // anv: for enemy taunts
extern STR16 gzLateLocalizedString[];

extern STR16 gzCWStrings[];

extern STR16 gzTooltipStrings[];

// These have been added - SANDRO 
extern STR16 pSkillTraitBeginIMPStrings[];
extern STR16 sgAttributeSelectionText[];
extern STR16 pCharacterTraitBeginIMPStrings[];
extern STR16 gzIMPCharacterTraitText[];
extern STR16 gzIMPAttitudesText[];
extern STR16 gzIMPColorChoosingText[];
extern STR16 sColorChoiceExplanationTexts[];
extern STR16 gzIMPDisabilityTraitText[];			// added by Flugente
extern STR16 gzIMPDisabilityTraitEmailTextDeaf[];	// added by Flugente
extern STR16 gzIMPDisabilityTraitEmailTextShortSighted[];
extern STR16 gzIMPDisabilityTraitEmailTextHemophiliac[];	// added by Flugente
extern STR16 gzIMPDisabilityTraitEmailTextAfraidOfHeights[];	// added by Flugente
extern STR16 gzIMPDisabilityTraitEmailTextSelfHarm[];	// added by Flugente
extern STR16 sEnemyTauntsFireGun[];
extern STR16 sEnemyTauntsFireLauncher[];
extern STR16 sEnemyTauntsThrow[];
extern STR16 sEnemyTauntsChargeKnife[];
extern STR16 sEnemyTauntsRunAway[];
extern STR16 sEnemyTauntsSeekNoise[];
extern STR16 sEnemyTauntsAlert[];
extern STR16 sEnemyTauntsGotHit[];
extern STR16 sEnemyTauntsNoticedMerc[];
extern STR16 sSpecialCharacters[];
//****

// HEADROCK HAM 3.6: New arrays for facility operation messages
extern STR16 gzFacilityErrorMessage[];
extern STR16 gzFacilityAssignmentStrings[];
extern STR16 gzFacilityRiskResultStrings[];

// HEADROCK HAM 4: Text for the new CTH indicator.
extern STR16 gzNCTHlabels[];

// HEADROCK HAM 5: Messages for automatic sector inventory sorting.
extern STR16 gzMapInventorySortingMessage[];
extern STR16 gzMapInventoryFilterOptions[];

// MeLoDy (Merc Compare)
extern STR16 gzMercCompare[];

enum
{
	ANTIHACKERSTR_EXITGAME,
	TEXT_NUM_ANTIHACKERSTR,
};
extern STR16 pAntiHackerString[];

enum
{
	MSG_EXITGAME,
	MSG_OK,
	MSG_YES,
	MSG_NO,
	MSG_CANCEL,
	MSG_REHIRE,
	MSG_LIE,
	MSG_NODESC,
	MSG_SAVESUCCESS,
	MSG_SAVESLOTSUCCESS,
	MSG_QUICKSAVE_NAME,
	MSG_SAVE_NAME,
	MSG_SAVEEXTENSION,
	MSG_SAVEDIRECTORY,
	MSG_DAY,
	MSG_MERCS,
	MSG_EMPTYSLOT,
	MSG_DEMOWORD,
	MSG_DEBUGWORD,
	MSG_RELEASEWORD,
	MSG_RPM,
	MSG_MINUTE_ABBREVIATION,
	MSG_METER_ABBREVIATION,
	MSG_ROUNDS_ABBREVIATION,
	MSG_KILOGRAM_ABBREVIATION,
	MSG_POUND_ABBREVIATION,
	MSG_HOMEPAGE,
	MSG_USDOLLAR_ABBREVIATION,
	MSG_LOWERCASE_NA,
	MSG_MEANWHILE,
	MSG_ARRIVE,
	MSG_VERSION,
	MSG_EMPTY_QUICK_SAVE_SLOT,
	MSG_QUICK_SAVE_RESERVED_FOR_TACTICAL,
	MSG_OPENED,
	MSG_CLOSED,
	MSG_LOWDISKSPACE_WARNING,
	MSG_HIRED_MERC,
	MSG_MERC_CAUGHT_ITEM,
	MSG_MERC_TOOK_DRUG,
	MSG_MERC_HAS_NO_MEDSKILL,
	MSG_INTEGRITY_WARNING,
	MSG_CDROM_SAVE,
	MSG_CANT_FIRE_HERE,
	MSG_CANT_CHANGE_STANCE,
	MSG_DROP,
	MSG_THROW,
	MSG_PASS,
	MSG_ITEM_PASSED_TO_MERC,
	MSG_NO_ROOM_TO_PASS_ITEM,
	MSG_END_ATTACHMENT_LIST,
	MSG_CHEAT_LEVEL_ONE,
	MSG_CHEAT_LEVEL_TWO,
	MSG_SQUAD_ON_STEALTHMODE,
	MSG_SQUAD_OFF_STEALTHMODE,
	MSG_MERC_ON_STEALTHMODE,
	MSG_MERC_OFF_STEALTHMODE,
	MSG_WIREFRAMES_ADDED,
	MSG_WIREFRAMES_REMOVED,
	MSG_CANT_GO_UP,
	MSG_CANT_GO_DOWN,
	MSG_ENTERING_LEVEL,
	MSG_LEAVING_BASEMENT,
	MSG_DASH_S,				// the old 's
	MSG_TRACKING_MODE_OFF,
	MSG_TRACKING_MODE_ON,
	MSG_3DCURSOR_OFF,
	MSG_3DCURSOR_ON,
	MSG_SQUAD_ACTIVE,
	MSG_CANT_AFFORD_TO_PAY_NPC_DAILY_SALARY_MSG,
	MSG_SKIP,
	MSG_EPC_CANT_TRAVERSE,
	MSG_CDROM_SAVE_GAME,
	MSG_DRANK_SOME,
	MSG_PACKAGE_ARRIVES,
	MSG_JUST_HIRED_MERC_ARRIVAL_LOCATION_POPUP,
	MSG_HISTORY_UPDATED,
	MSG_GL_BURST_CURSOR_ON,
	MSG_GL_BURST_CURSOR_OFF,
	MSG_SOLDIER_TOOLTIPS_ON, // changed by SANDRO
	MSG_SOLDIER_TOOLTIPS_OFF, // changed by SANDRO
	MSG_GL_LOW_ANGLE,
	MSG_GL_HIGH_ANGLE,
	MSG_FORCED_TURN_MODE,
	MSG_NORMAL_TURN_MODE,
	MSG_FTM_EXIT_COMBAT,
	MSG_FTM_ENTER_COMBAT,
	MSG_END_TURN_AUTO_SAVE,
	MSG_MPSAVEDIRECTORY,//88
	MSG_CLIENT,
	MSG_NAS_AND_OIV_INCOMPATIBLE,	// 90

	MSG_SAVE_AUTOSAVE_TEXT,			// 91
	MSG_SAVE_AUTOSAVE_TEXT_INFO,	// 92
	MSG_SAVE_AUTOSAVE_EMPTY_TEXT,	// 93
	MSG_SAVE_AUTOSAVE_FILENAME,		// 94
	MSG_SAVE_END_TURN_SAVE_TEXT,	// 95
	MSG_SAVE_AUTOSAVE_SAVING_TEXT,	// 96
	MSG_SAVE_END_TURN_SAVE_SAVING_TEXT,	// 97
	MSG_SAVE_AUTOSAVE_ENDTURN_EMPTY_TEXT,	//98
	MSG_SAVE_AUTOSAVE_ENDTURN_TEXT_INFO,	//99
	MSG_SAVE_QUICKSAVE_SLOT,				// 100
	MSG_SAVE_AUTOSAVE_SLOT,					// 101
	MSG_SAVE_AUTOSAVE_ENDTURN_SLOT,			// 102
	MSG_SAVE_NORMAL_SLOT,					// 103
	
	MSG_WINDOWED_MODE_LOCK_MOUSE,			// 104
	MSG_WINDOWED_MODE_RELEASE_MOUSE,		// 105
	MSG_FORMATIONS_ON,						// 106
	MSG_FORMATIONS_OFF,						// 107
	MSG_MERC_CASTS_LIGHT_ON,
	MSG_MERC_CASTS_LIGHT_OFF,

	MSG_SQUAD_ACTIVE_STRING,
	MSG_MERC_TOOK_CIGARETTE,

	MSG_PROMPT_CHEATS_ACTIVATE,
	MSG_PROMPT_CHEATS_DEACTIVATE,

	TEXT_NUM_MSG,
};
extern STR16 pMessageStrings[];

extern CHAR16 ItemPickupHelpPopup[][40];

enum
{
	STR_EMPTY,
	STR_LOSES_1_WISDOM,
	STR_LOSES_1_DEX,
	STR_LOSES_1_STRENGTH,
	STR_LOSES_1_AGIL,
	STR_LOSES_WISDOM,
	STR_LOSES_DEX,
	STR_LOSES_STRENGTH,
	STR_LOSES_AGIL,
	STR_INTERRUPT,
	STR_HEARS_NOISE_FROM,
	STR_PLAYER_REINFORCEMENTS,
	STR_PLAYER_RELOADS,
	STR_PLAYER_NOT_ENOUGH_APS,
	STR_IS_APPLYING_FIRST_AID,
	STR_ARE_APPLYING_FIRST_AID,
	STR_RELIABLE,
	STR_UNRELIABLE,
	STR_EASY_TO_REPAIR,
	STR_HARD_TO_REPAIR,
	STR_HIGH_DAMAGE,
	STR_LOW_DAMAGE,
	STR_QUICK_FIRING,
	STR_SLOW_FIRING,
	STR_LONG_RANGE,
	STR_SHORT_RANGE,
	STR_LIGHT,
	STR_HEAVY,
	STR_SMALL,
	STR_FAST_BURST,
	STR_NO_BURST,
	STR_LARGE_AMMO_CAPACITY,
	STR_SMALL_AMMO_CAPACITY,
	STR_CAMMO_WORN_OFF,
	STR_CAMMO_WASHED_OFF,
	STR_2ND_CLIP_DEPLETED,
	STR_STOLE_SOMETHING,
	STR_NOT_BURST_CAPABLE,
	STR_ATTACHMENT_ALREADY,
	STR_MERGE_ITEMS,
	STR_CANT_ATTACH,
	STR_NONE,
	STR_EJECT_AMMO,
	STR_ATTACHMENTS,
	STR_CANT_USE_TWO_ITEMS,
	STR_ATTACHMENT_HELP,
	STR_ATTACHMENT_INVALID_HELP,
	STR_SECTOR_NOT_CLEARED,
	STR_NEED_TO_GIVE_MONEY,
	STR_HEAD_HIT,
	STR_ABANDON_FIGHT,
	STR_PERMANENT_ATTACHMENT,
	STR_ENERGY_BOOST,
	STR_SLIPPED_MARBLES,
	STR_FAILED_TO_STEAL_SOMETHING,
	STR_REPAIRED,
	STR_INTERRUPT_FOR,
	STR_SURRENDER,
	STR_REFUSE_FIRSTAID,
	STR_REFUSE_FIRSTAID_FOR_CREATURE,
	STR_HOW_TO_USE_SKYRIDDER,
	STR_RELOAD_ONLY_ONE_GUN,
	STR_BLOODCATS_TURN,
	STR_AUTOFIRE,
	STR_NO_AUTOFIRE,
	STR_ACCURATE,
	STR_INACCURATE,
	STR_NO_SEMI_AUTO,
	STR_NO_MORE_ITEMS_TO_STEAL,
	STR_NO_MORE_ITEM_IN_HAND,
	
	//add new camo text 
	STR_DESERT_WORN_OFF,
	STR_DESERT_WASHED_OFF,	
	
	STR_JUNGLE_WORN_OFF,
	STR_JUNGLE_WASHED_OFF,
	
	STR_URBAN_WORN_OFF,
	STR_URBAN_WASHED_OFF,

	STR_SNOW_WORN_OFF,
	STR_SNOW_WASHED_OFF,		

	STR_CANNOT_ATTACH_SLOT,
	STR_CANNOT_ATTACH_ANY_SLOT,
	
	STR_NO_SPACE_FOR_POCKET,

	STR_REPAIRED_PARTIAL,
	STR_REPAIRED_PARTIAL_FOR_OWNER,

	STR_CLEANED,
	STR_CLEANED_FOR_OWNER,

	STR_ASSIGNMENT_NOTPOSSIBLE,
	STR_ASSIGNMENT_NOMILITIAPRESENT,

	STR_ASSIGNMENT_EXPLORATION_DONE,

	TEXT_NUM_STR_MESSAGE,
};

// WANNE: Tooltips
enum
{
	STR_TT_CAT_LOCATION,
	STR_TT_CAT_BRIGHTNESS,
	STR_TT_CAT_RANGE_TO_TARGET,
	STR_TT_CAT_ID,
	STR_TT_CAT_ORDERS,
	STR_TT_CAT_ATTITUDE,
	STR_TT_CAT_CURRENT_APS,
	STR_TT_CAT_CURRENT_HEALTH,
	STR_TT_CAT_CURRENT_ENERGY,
	STR_TT_CAT_CURRENT_MORALE,
	STR_TT_CAT_SHOCK,		///< Moa: shows current shock value. Only for debug tooltip.
	STR_TT_CAT_SUPPRESION,	///< Moa: shows current supression value. Only for debug tooltip.
	STR_TT_CAT_HELMET,
	STR_TT_CAT_VEST,
	STR_TT_CAT_LEGGINGS,
	STR_TT_CAT_ARMOR,
	STR_TT_HELMET,
	STR_TT_VEST,
	STR_TT_LEGGINGS,
	STR_TT_WORN,
	STR_TT_NO_ARMOR,
	STR_TT_CAT_NVG,
	STR_TT_NO_NVG,
	STR_TT_CAT_GAS_MASK,
	STR_TT_NO_MASK,
	STR_TT_CAT_HEAD_POS_1,
	STR_TT_CAT_HEAD_POS_2,
	STR_TT_IN_BACKPACK,
	STR_TT_CAT_WEAPON,
	STR_TT_NO_WEAPON,
	STR_TT_HANDGUN,
	STR_TT_SMG,
	STR_TT_RIFLE,
	STR_TT_MG,
	STR_TT_SHOTGUN,
	STR_TT_KNIFE,
	STR_TT_HEAVY_WEAPON,
	STR_TT_NO_HELMET,
	STR_TT_NO_VEST,
	STR_TT_NO_LEGGING,
	STR_TT_CAT_ARMOR_2,
	// Following added - SANDRO
	STR_TT_SKILL_TRAIT_1,
	STR_TT_SKILL_TRAIT_2,
	STR_TT_SKILL_TRAIT_3,
        // Additional suppression effects info - sevenfm
        STR_TT_SUPPRESSION_AP,
        STR_TT_SUPPRESSION_TOLERANCE,
        STR_TT_EFFECTIVE_SHOCK,
        STR_TT_AI_MORALE,

	TEXT_NUM_STR_TT
};

enum
{
	STR_HELI_ETA_TOTAL_DISTANCE,
	STR_HELI_ETA_SAFE,
	STR_HELI_ETA_UNSAFE,
	STR_HELI_ETA_TOTAL_COST,
	STR_HELI_ETA_ETA,

	STR_HELI_ETA_LOW_ON_FUEL_HOSTILE_TERRITORY,
	STR_HELI_ETA_PASSENGERS,
	STR_HELI_ETA_SELECT_SKYRIDER_OR_ARRIVALS,
	STR_HELI_ETA_SKYRIDER,
	STR_HELI_ETA_ARRIVALS,

	STR_HELI_ETA_HELI_DAMAGED_HOSTILE_TERRITORY,
	STR_HELI_ETA_KICK_OUT_PASSENGERS_PROMPT,
	STR_HELI_ETA_REMAINING_FUEL,
	STR_HELI_ETA_DISTANCE_TO_REFUEL_SITE,

	TEXT_NUM_STR_HELI_ETA,
};

// anv: helicopter repairs
enum
{
	STR_HELI_RR_REPAIR_PROMPT,
	STR_HELI_RR_REPAIR_IN_PROGRESS,
	STR_HELI_RR_REPAIR_FINISHED,
	STR_HELI_RR_REFUEL_FINISHED,

	STR_HELI_TOOFAR_ERROR,

	TEXT_NUM_STR_HELI_REPAIRS,
};

#define LARGE_STRING_LENGTH			200
#define MED_STRING_LENGTH				80
#define	SMALL_STRING_LENGTH			20

extern CHAR16 TacticalStr[][MED_STRING_LENGTH];
extern CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ];


extern CHAR16		zDialogActions[][ SMALL_STRING_LENGTH ];
extern CHAR16		zDealerStrings[][ SMALL_STRING_LENGTH ];
extern CHAR16		zTalkMenuStrings[][ SMALL_STRING_LENGTH ];
extern STR16		gzMoneyAmounts[6];
extern CHAR16		gzProsLabel[10];
extern CHAR16		gzConsLabel[10];
// HEADROCK HAM 4: Text for the UDB tabs
extern STR16 gzItemDescTabButtonText[ 3 ];
extern STR16 gzItemDescTabButtonShortText[ 3 ];
extern STR16 gzItemDescGenHeaders[ 4 ];
extern STR16 gzItemDescGenIndexes[ 4 ];
// HEADROCK HAM 4: Added list of condition strings
extern STR16		gConditionDesc[ 9 ];

// Flugente: Added list of temperature descriptions
extern STR16		gTemperatureDesc[ 11 ];

// Flugente: Added list of food condition descriptions
extern STR16		gFoodDesc[ 8 ];

extern CHAR16		gMoneyStatsDesc[][ 14 ];
// HEADROCK: Altered value to 16 //WarmSteel - And I need 17.	// Flugente: 17->19
extern CHAR16		gWeaponStatsDesc[][ 20 ];
// HEADROCK: Added externs for Item Description Box icon and stat tooltips
// Note that I've inflated some of these to 20 to avoid issues.
extern STR16		gzWeaponStatsFasthelpTactical[ 33 ];
extern STR16		gzMiscItemStatsFasthelp[];
// HEADROCK HAM 4: New tooltip texts
extern STR16		gzUDBButtonTooltipText[ 3 ];
extern STR16		gzUDBHeaderTooltipText[ 4 ];
extern STR16		gzUDBGenIndexTooltipText[ 4 ];
extern STR16		gzUDBAdvIndexTooltipText[ 5 ];
extern STR16		szUDBGenWeaponsStatsTooltipText[ 23 ];
extern STR16		szUDBGenWeaponsStatsExplanationsTooltipText[ 24 ];
extern STR16		szUDBGenArmorStatsTooltipText[ 4 ];						// silversurfer Repair Ease: 3->5
extern STR16		szUDBGenArmorStatsExplanationsTooltipText[ 5 ];			// silversurfer Repair Ease: 3->5
extern STR16		szUDBGenAmmoStatsTooltipText[ 6 ];						// Flugente Overheating: 3->4	poison: 4->5	dirt: 5->6
extern STR16		szUDBGenAmmoStatsExplanationsTooltipText[ 6 ];			// Flugente Overheating: 3->4	poison: 4->5	dirt: 5->6
extern STR16		szUDBGenExplosiveStatsTooltipText[ 23 ];				// silversurfer Repair Ease: 22->23
extern STR16		szUDBGenExplosiveStatsExplanationsTooltipText[ 23 ];	// silversurfer Repair Ease: 22->23
extern STR16		szUDBGenCommonStatsTooltipText[ 3 ];					// silversurfer new for items that don't fit the other categories
extern STR16		szUDBGenCommonStatsExplanationsTooltipText[ 3 ];		// silversurfer new for items that don't fit the other categories
extern STR16		szUDBGenSecondaryStatsTooltipText[];				// Flugente Food System: 26 -> 28 external feeding: 28->30 JMich_SkillsModifiers: 31 for Defusal kit - covert item: 31->32 silversurfer more tags: 32->37
extern STR16		szUDBGenSecondaryStatsExplanationsTooltipText[];	// Flugente Food System: 26 -> 28 external feeding: 28->30 JMich_SkillsModifiers: 31 for Defusal kit - covert item: 31->32 silversurfer more tags: 32->37
extern STR16		szUDBAdvStatsTooltipText[];
extern STR16		szUDBAdvStatsExplanationsTooltipText[];
extern STR16		szUDBAdvStatsExplanationsTooltipTextForWeapons[];

// Headrock: End Externs
extern STR16		sKeyDescriptionStrings[2];
extern CHAR16		zHealthStr[][13];
extern STR16		gzHiddenHitCountStr[1];
extern STR16		zVehicleName[ 6 ];
extern STR16		pVehicleSeatsStrings[ 2 ] ;

// Flugente: externalised texts for some features
enum
{
	STR_COVERT_CAMOFOUND,
	STR_COVERT_BACKPACKFOUND,
	STR_COVERT_CARRYCORPSEFOUND,
	STR_COVERT_ITEM_SUSPICIOUS,
	STR_COVERT_MILITARYGEARFOUND,
	STR_COVERT_TOOMANYGUNS,
	STR_COVERT_ITEMSTOOGOOD,
	STR_COVERT_TOOMANYATTACHMENTS,
	STR_COVERT_ACTIVITIES,
	STR_COVERT_NO_CIV,
	STR_COVERT_BLEEDING,
	STR_COVERT_DRUNKEN_SOLDIER,
	STR_COVERT_TOO_CLOSE,
	STR_COVERT_CURFEW_BROKEN,
	STR_COVERT_CURFEW_BROKEN_NIGHT,
	STR_COVERT_NEAR_CORPSE,
	STR_COVERT_SUSPICIOUS_EQUIPMENT,
	STR_COVERT_TARGETTING_SOLDIER,
	STR_COVERT_UNCOVERED,
	STR_COVERT_NO_CLOTHES_ITEM,
	STR_COVERT_ERROR_OLDTRAITS,
	STR_COVERT_NOT_ENOUGH_APS,
	STR_COVERT_BAD_PALETTE,
	STR_COVERT_NO_SKILL,
	STR_COVERT_NO_UNIFORM_FOUND,	
	STR_COVERT_DISGUISED_AS_CIVILIAN,
	STR_COVERT_DISGUISED_AS_SOLDIER,
	STR_COVERT_UNIFORM_NOORDER,
	STR_COVERT_SURRENDER_FAILED,
	STR_COVERT_UNCOVER_SINGLE,
	STR_COVERT_TEST_OK,
	STR_COVERT_TEST_FAIL,
	STR_COVERT_STEAL_FAIL,
	STR_COVERT_APPLYITEM_STEAL_FAIL,
	STR_COVERT_TOO_CLOSE_TO_ELITE,
	STR_COVERT_TOO_CLOSE_TO_OFFICER,
	TEXT_NUM_COVERT_STR
};

extern STR16		szCovertTextStr[];

enum
{
	STR_POWERPACK_BEGIN,
	STR_POWERPACK_FULL,
	STR_POWERPACK_GOOD,
	STR_POWERPACK_HALF,
	STR_POWERPACK_LOW,
	STR_POWERPACK_EMPTY,
	STR_POWERPACK_END,

	TEXT_POWERPACK_STR
};

extern STR16 gPowerPackDesc[];

enum
{
	STR_CORPSE_NO_HEAD_ITEM,
	STR_CORPSE_NO_DECAPITATION,
	STR_CORPSE_NO_MEAT_ITEM,
	STR_CORPSE_NO_GUTTING,
	STR_CORPSE_NO_CLOTHESFOUND,
	STR_CORPSE_NO_STRIPPING_POSSIBLE,
	STR_CORPSE_NO_TAKING,
	STR_CORPSE_NO_FREEHAND,
	STR_CORPSE_NO_CORPSE_ITEM,
	STR_CORPSE_INVALID_CORPSE_ID,

	TEXT_NUM_CORPSE_STR
};

extern STR16		szCorpseTextStr[];

enum
{
	STR_FOOD_DONOTWANT_EAT,
	STR_FOOD_DONOTWANT_DRINK,
	STR_FOOD_ATE,
	STR_FOOD_DRANK,
	STR_FOOD_STR_DAMAGE_FOOD_TOO_MUCH,
	STR_FOOD_STR_DAMAGE_FOOD_TOO_LESS,
	STR_FOOD_HEALTH_DAMAGE_FOOD_TOO_MUCH,
	STR_FOOD_HEALTH_DAMAGE_FOOD_TOO_LESS,
	STR_FOOD_STR_DAMAGE_DRINK_TOO_MUCH,
	STR_FOOD_STR_DAMAGE_DRINK_TOO_LESS,
	STR_FOOD_HEALTH_DAMAGE_DRINK_TOO_MUCH,
	STR_FOOD_HEALTH_DAMAGE_DRINK_TOO_LESS,

	STR_FOOD_ERROR_NO_FOOD_SYSTEM,

	TEXT_NUM_FOOD_STR
};

extern STR16		szFoodTextStr[];

enum
{
	STR_PRISONER_PROCESSED,
	STR_PRISONER_RANSOM,
	STR_PRISONER_DETECTION,
	STR_PRISONER_TURN_MILITIA,
	STR_PRISONER_RIOT,
	STR_PRISONER_SENTTOSECTOR,
	STR_PRISONER_RELEASED,
	STR_PRISONER_ARMY_FREED_PRISON,
	STR_PRISONER_REFUSE_SURRENDER,
	STR_PRISONER_REFUSE_TAKE_PRISONERS,
	SRT_PRISONER_INI_SETTING_OFF,
	STR_PRISONER_X_FREES_Y,
	STR_PRISONER_DETECTION_VIP,
	STR_PRISONER_REFUSE_SURRENDER_LEADER,
	STR_PRISONER_TURN_VOLUNTEER,

	TEXT_NUM_PRISONER_STR
};

extern STR16		szPrisonerTextStr[];

enum
{
	STR_MTA_NONE,
	STR_MTA_FORTIFY,
	STR_MTA_REMOVE_FORTIFY,
	STR_MTA_HACK,
	STR_MTA_CANCEL,
	STR_MTA_CANNOT_BUILD,

	TEXT_NUM_MTA_STR
};

extern STR16		szMTATextStr[];

enum
{
	STR_INV_ARM_BLOWUP_AP,
	STR_INV_ARM_BLOWUP,
	STR_INV_ARM_ARM_AP,
	STR_INV_ARM_ARM,
	STR_INV_ARM_DISARM_AP,
	STR_INV_ARM_DISARM,

	TEXT_NUM_INV_ARM_STR
};

extern STR16		szInventoryArmTextStr[];

enum
{
	AIR_RAID_TURN_STR,
	BEGIN_AUTOBANDAGE_PROMPT_STR,
	NOTICING_MISSING_ITEMS_FROM_SHIPMENT_STR,
	DOOR_LOCK_DESCRIPTION_STR,
	DOOR_THERE_IS_NO_LOCK_STR,
	DOOR_LOCK_DESTROYED_STR,
	DOOR_LOCK_NOT_DESTROYED_STR,
	DOOR_LOCK_HAS_BEEN_PICKED_STR,
	DOOR_LOCK_HAS_NOT_BEEN_PICKED_STR,
	DOOR_LOCK_UNTRAPPED_STR,
	DOOR_LOCK_HAS_BEEN_UNLOCKED_STR,
	DOOR_NOT_PROPER_KEY_STR,
	DOOR_LOCK_HAS_BEEN_UNTRAPPED_STR,
	DOOR_LOCK_IS_NOT_TRAPPED_STR,
	DOOR_LOCK_HAS_BEEN_LOCKED_STR,
	DOOR_DOOR_MOUSE_DESCRIPTION,
	DOOR_TRAPPED_MOUSE_DESCRIPTION,
	DOOR_LOCKED_MOUSE_DESCRIPTION,
	DOOR_UNLOCKED_MOUSE_DESCRIPTION,
	DOOR_BROKEN_MOUSE_DESCRIPTION,
	ACTIVATE_SWITCH_PROMPT,
	DISARM_TRAP_PROMPT,
	ITEMPOOL_POPUP_PREV_STR,
	ITEMPOOL_POPUP_NEXT_STR,
	ITEMPOOL_POPUP_MORE_STR,
	ITEM_HAS_BEEN_PLACED_ON_GROUND_STR,
	ITEM_HAS_BEEN_GIVEN_TO_STR,
	GUY_HAS_BEEN_PAID_IN_FULL_STR,
	GUY_STILL_OWED_STR,
	CHOOSE_BOMB_FREQUENCY_STR,
	CHOOSE_TIMER_STR,
	CHOOSE_REMOTE_FREQUENCY_STR,
	DISARM_BOOBYTRAP_PROMPT,
	REMOVE_BLUE_FLAG_PROMPT,
	PLACE_BLUE_FLAG_PROMPT,
	ENDING_TURN,
	ATTACK_OWN_GUY_PROMPT,
	VEHICLES_NO_STANCE_CHANGE_STR,
	ROBOT_NO_STANCE_CHANGE_STR,
	CANNOT_STANCE_CHANGE_STR,
	CANNOT_DO_FIRST_AID_STR,
	CANNOT_NO_NEED_FIRST_AID_STR,
	CANT_MOVE_THERE_STR,
	CANNOT_RECRUIT_TEAM_FULL,
	HAS_BEEN_RECRUITED_STR,
	BALANCE_OWED_STR,
	ESCORT_PROMPT,
	HIRE_PROMPT,
	BOXING_PROMPT,
	BUY_VEST_PROMPT,
	NOW_BING_ESCORTED_STR,
	JAMMED_ITEM_STR,
	ROBOT_NEEDS_GIVEN_CALIBER_STR,
	CANNOT_THROW_TO_DEST_STR,
	TOGGLE_STEALTH_MODE_POPUPTEXT,
	MAPSCREEN_POPUPTEXT,
	END_TURN_POPUPTEXT,
	TALK_CURSOR_POPUPTEXT,
	TOGGLE_MUTE_POPUPTEXT,
	CHANGE_STANCE_UP_POPUPTEXT,
	CURSOR_LEVEL_POPUPTEXT,
	JUMPCLIMB_POPUPTEXT,
	CHANGE_STANCE_DOWN_POPUPTEXT,
	EXAMINE_CURSOR_POPUPTEXT,
	PREV_MERC_POPUPTEXT,
	NEXT_MERC_POPUPTEXT,
	CHANGE_OPTIONS_POPUPTEXT,
	TOGGLE_BURSTMODE_POPUPTEXT,
	LOOK_CURSOR_POPUPTEXT,
	MERC_VITAL_STATS_POPUPTEXT,
	CANNOT_DO_INV_STUFF_STR,
	CONTINUE_OVER_FACE_STR,
	MUTE_OFF_STR,
	MUTE_ON_STR,
	DRIVER_POPUPTEXT,
	EXIT_VEHICLE_POPUPTEXT,
	CHANGE_SQUAD_POPUPTEXT,
	DRIVE_POPUPTEXT,
	NOT_APPLICABLE_POPUPTEXT,
	USE_HANDTOHAND_POPUPTEXT,
	USE_FIREARM_POPUPTEXT,
	USE_BLADE_POPUPTEXT ,
	USE_EXPLOSIVE_POPUPTEXT,
	USE_MEDKIT_POPUPTEXT,
	CATCH_STR,
	RELOAD_STR,
	GIVE_STR,
	LOCK_TRAP_HAS_GONE_OFF_STR,
	MERC_HAS_ARRIVED_STR,
	GUY_HAS_RUN_OUT_OF_APS_STR,
	MERC_IS_UNAVAILABLE_STR,
	MERC_IS_ALL_BANDAGED_STR,
	MERC_IS_OUT_OF_BANDAGES_STR,
	ENEMY_IN_SECTOR_STR,
	NO_ENEMIES_IN_SIGHT_STR,
	NOT_ENOUGH_APS_STR,
	NOBODY_USING_REMOTE_STR,
	BURST_FIRE_DEPLETED_CLIP_STR,
	ENEMY_TEAM_MERC_NAME,
	CREATURE_TEAM_MERC_NAME,
	MILITIA_TEAM_MERC_NAME,
	CIV_TEAM_MERC_NAME,
	ZOMBIE_TEAM_MERC_NAME,
	POW_TEAM_MERC_NAME,
	
	//The text for the 'exiting sector' gui
	EXIT_GUI_TITLE_STR,
	OK_BUTTON_TEXT_STR,
	CANCEL_BUTTON_TEXT_STR,
	EXIT_GUI_SELECTED_MERC_STR,
	EXIT_GUI_ALL_MERCS_IN_SQUAD_STR,
	EXIT_GUI_GOTO_SECTOR_STR,
	EXIT_GUI_GOTO_MAP_STR,
	CANNOT_LEAVE_SECTOR_FROM_SIDE_STR,
	CANNOT_LEAVE_IN_TURN_MODE_STR,
	MERC_IS_TOO_FAR_AWAY_STR,
	REMOVING_TREETOPS_STR,
	SHOWING_TREETOPS_STR,
	CROW_HIT_LOCATION_STR,
	NECK_HIT_LOCATION_STR,
	HEAD_HIT_LOCATION_STR,
	TORSO_HIT_LOCATION_STR,
	LEGS_HIT_LOCATION_STR,
	YESNOLIE_STR,
	GUN_GOT_FINGERPRINT,
	GUN_NOGOOD_FINGERPRINT,
	GUN_GOT_TARGET,
	NO_PATH,
	MONEY_BUTTON_HELP_TEXT,
	AUTOBANDAGE_NOT_NEEDED,
	SHORT_JAMMED_GUN,
	CANT_GET_THERE,
	EXCHANGE_PLACES_REQUESTER,
	REFUSE_EXCHANGE_PLACES,
	PAY_MONEY_PROMPT,
	FREE_MEDICAL_PROMPT,
	MARRY_DARYL_PROMPT,
	KEYRING_HELP_TEXT,
	EPC_CANNOT_DO_THAT,
	SPARE_KROTT_PROMPT,
	OUT_OF_RANGE_STRING,
	CIV_TEAM_MINER_NAME,
	VEHICLE_CANT_MOVE_IN_TACTICAL,
	CANT_AUTOBANDAGE_PROMPT,
	NO_PATH_FOR_MERC,
	POW_MERCS_ARE_HERE,
	LOCK_HAS_BEEN_HIT,
	LOCK_HAS_BEEN_DESTROYED,
	DOOR_IS_BUSY,
	VEHICLE_VITAL_STATS_POPUPTEXT,
	NO_LOS_TO_TALK_TARGET,
	ATTACHMENT_REMOVED,
	VEHICLE_CAN_NOT_BE_ADDED,

	// added by Flugente for defusing/setting up trap networks
	CHOOSE_BOMB_OR_DEFUSE_FREQUENCY_STR,
	CHOOSE_REMOTE_DEFUSE_FREQUENCY_STR,
	CHOOSE_REMOTE_DETONATE_AND_REMOTE_DEFUSE_FREQUENCY_STR,
	CHOOSE_DETONATE_AND_REMOTE_DEFUSE_FREQUENCY_STR,
	CHOOSE_TRIPWIRE_NETWORK,

	MERC_VITAL_STATS_WITH_FOOD_POPUPTEXT,

	FUNCTION_SELECTION_STR,
	FILL_CANTEEN_STR,
	CLEAN_ONE_GUN_STR,
	CLEAN_ALL_GUNS_STR,
	TAKE_OFF_CLOTHES_STR,
	TAKE_OFF_DISGUISE_STR,
	MILITIA_DROP_EQ_STR,
	MILITIA_PICK_UP_EQ_STR,
	SPY_SELFTEST_STR,
	UNUSED_STR,

	CORPSE_SELECTION_STR,
	DECAPITATE_STR,
	GUT_STR,
	TAKE_CLOTHES_STR,
	TAKE_BODY_STR,

	WEAPON_CLEANING_STR,

	PRISONER_FIELDINTERROGATION_STR,
	PRISONER_FIELDINTERROGATION_SHORT_STR,
	PRISONER_DECIDE_STR,
	PRISONER_LETGO_STR,
	PRISONER_OFFER_SURRENDER,
	PRISONER_DEMAND_SURRENDER_STR,
	PRISONER_OFFER_SURRENDER_STR,
	PRISONER_DISTRACT_STR, 
	PRISONER_TALK_STR,
	PRISONER_RECRUIT_TURNCOAT_STR,
	
    // sevenfm: new disarm trap dialog, new messages for wrong mines when arming
    DISARM_DIALOG_DISARM,
    DISARM_DIALOG_INSPECT,
    DISARM_DIALOG_REMOVE_BLUEFLAG,
    DISARM_DIALOG_BLOWUP,
    DISARM_DIALOG_ACTIVATE_TRIPWIRE,
    DISARM_DIALOG_DEACTIVATE_TRIPWIRE,
    DISARM_DIALOG_REVEAL_TRIPWIRE,
    ARM_MESSAGE_NO_DETONATOR,
    ARM_MESSAGE_ALREADY_ARMED,
    INSPECT_RESULT_SAFE,
    INSPECT_RESULT_MOSTLY_SAFE,
    INSPECT_RESULT_RISKY,
    INSPECT_RESULT_DANGEROUS,
    INSPECT_RESULT_HIGH_DANGER,

    GENERAL_INFO_MASK,
    GENERAL_INFO_NVG,
    GENERAL_INFO_ITEM,

    QUICK_ITEMS_ONLY_NIV,
    QUICK_ITEMS_NO_ITEM_IN_HAND,
    QUICK_ITEMS_NOWHERE_TO_PLACE,
    QUICK_ITEM_NO_DEFINED_ITEM,
    QUICK_ITEM_NO_FREE_HAND,
    QUICK_ITEM_NOT_FOUND,
    QUICK_ITEM_CANNOT_TAKE,

	ATTEMPT_BANDAGE_DURING_TRAVEL,

	IMPROVEGEARBUTTON_STR,
	IMPROVEGEARDESCRIBE_STR,
	IMPROVEGEARPICKUPMAG_STR,

	DISTRACT_STOP_STR,
	DISTRACT_TRY_TO_TURNCOAT,

	TEXT_NUM_TACTICAL_STR
};

enum{
	EXIT_GUI_LOAD_ADJACENT_SECTOR_HELPTEXT,
	EXIT_GUI_GOTO_MAPSCREEN_HELPTEXT,
	EXIT_GUI_CANT_LEAVE_HOSTILE_SECTOR_HELPTEXT,
	EXIT_GUI_MUST_LOAD_ADJACENT_SECTOR_HELPTEXT,
	EXIT_GUI_MUST_GOTO_MAPSCREEN_HELPTEXT,
	EXIT_GUI_ESCORTED_CHARACTERS_MUST_BE_ESCORTED_HELPTEXT,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_MALE_SINGULAR,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_FEMALE_SINGULAR,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_MALE_PLURAL,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_FEMALE_PLURAL,
	EXIT_GUI_ALL_MERCS_MUST_BE_TOGETHER_TO_ALLOW_HELPTEXT,
	EXIT_GUI_EPC_NOT_ALLOWED_TO_LEAVE_ALONE_HELPTEXT,
	EXIT_GUI_SINGLE_TRAVERSAL_WILL_SEPARATE_SQUADS_HELPTEXT,
	EXIT_GUI_ALL_TRAVERSAL_WILL_MOVE_CURRENT_SQUAD_HELPTEXT,
	EXIT_GUI_ESCORTED_CHARACTERS_CANT_LEAVE_SECTOR_ALONE_STR,
	TEXT_NUM_EXIT_GUI
};
extern STR16 pExitingSectorHelpText[];


enum
{
	LARGESTR_NOONE_LEFT_CAPABLE_OF_BATTLE_STR,
	LARGESTR_NOONE_LEFT_CAPABLE_OF_BATTLE_AGAINST_CREATURES_STR,
	LARGESTR_HAVE_BEEN_CAPTURED,
	TEXT_NUM_LARGESTR
};


//Insurance Contract.c
enum
{
	INS_CONTRACT_PREVIOUS,
	INS_CONTRACT_NEXT,
	INS_CONTRACT_ACCEPT,
	INS_CONTRACT_CLEAR,
	TEXT_NUM_INS_CONTRACT
};
extern		STR16			InsContractText[];


//Insurance Info
enum
{
	INS_INFO_PREVIOUS,
	INS_INFO_NEXT,
	TEXT_NUM_INS_INFO,
};
extern		STR16		InsInfoText[];

//Merc Account.c
enum
{
	MERC_ACCOUNT_AUTHORIZE,
	MERC_ACCOUNT_HOME,
	MERC_ACCOUNT_ACCOUNT,
	MERC_ACCOUNT_MERC,
	MERC_ACCOUNT_DAYS,
	MERC_ACCOUNT_RATE,
	MERC_ACCOUNT_CHARGE,
	MERC_ACCOUNT_TOTAL,
	MERC_ACCOUNT_AUTHORIZE_CONFIRMATION,
	MERC_ACCOUNT_NAME_PLUSGEAR,
	TEXT_NUM_MERC_ACCOUNT,
};
extern		STR16			MercAccountText[];

// WANNE: The "Next" and "Prev" button text of the merc account page
extern	STR16 MercAccountPageText[];


//MercFile.c
enum
{
	MERC_FILES_HEALTH,
	MERC_FILES_AGILITY,
	MERC_FILES_DEXTERITY,
	MERC_FILES_STRENGTH,
	MERC_FILES_LEADERSHIP,
	MERC_FILES_WISDOM,
	MERC_FILES_EXPLEVEL,
	MERC_FILES_MARKSMANSHIP,
	MERC_FILES_MECHANICAL,
	MERC_FILES_EXPLOSIVE,
	MERC_FILES_MEDICAL,

	MERC_FILES_PREVIOUS,
	MERC_FILES_HIRE,
	MERC_FILES_NEXT,
	MERC_FILES_ADDITIONAL_INFO,
	MERC_FILES_HOME,
	MERC_FILES_ALREADY_HIRED,			//5
	MERC_FILES_SALARY,
	MERC_FILES_PER_DAY,
	MERC_FILES_GEAR,
	MERC_FILES_TOTAL,
	MERC_FILES_MERC_IS_DEAD,

	MERC_FILES_HIRE_TO_MANY_PEOPLE_WARNING,
	MERC_FILES_BUY_GEAR,
	MERC_FILES_MERC_UNAVAILABLE,
	MERC_FILES_MERC_OUTSTANDING,
	MERC_FILES_BIO,	//JMich_MMG: Adding two new texts for the small button, assuming we manage to add a silhouette with the gear, add it after this.
	MERC_FILES_INVENTORY,
	TEXT_NUM_MERC_FILES,
};
extern		STR16			MercInfo[];


//MercNoAccount.c
enum
{
	MERC_NO_ACC_OPEN_ACCOUNT,
	MERC_NO_ACC_CANCEL,
	MERC_NO_ACC_NO_ACCOUNT_OPEN_ONE,
	TEXT_NUM_MERC_NO_ACC,
};
extern		STR16			MercNoAccountText[];



//Merc HomePage
enum
{
	MERC_SPECK_OWNER,
	MERC_OPEN_ACCOUNT,
	MERC_VIEW_ACCOUNT,
	MERC_VIEW_FILES,
	MERC_SPECK_COM,
	MERC_NO_FUNDS_TRANSFER_FAILED,
	TEXT_NUM_MERC,
};
extern		STR16			MercHomePageText[];


//Funerl.c
enum
{
	FUNERAL_INTRO_1,
	FUNERAL_INTRO_2,
	FUNERAL_INTRO_3,
	FUNERAL_INTRO_4,
	FUNERAL_INTRO_5,
	FUNERAL_SEND_FLOWERS,			//5
	FUNERAL_CASKET_URN,
	FUNERAL_CREMATION,
	FUNERAL_PRE_FUNERAL,
	FUNERAL_FUNERAL_ETTIQUETTE,
	FUNERAL_OUR_CONDOLENCES,	//10
	FUNERAL_OUR_SYMPATHIES,
	TEXT_NUM_FUNERAL,
};
extern		STR16			sFuneralString[];


//Florist.c
enum
{
	FLORIST_GALLERY,
	FLORIST_DROP_ANYWHERE,
	FLORIST_PHONE_NUMBER,
	FLORIST_STREET_ADDRESS,
	FLORIST_WWW_ADDRESS,
	FLORIST_ADVERTISEMENT_1,
	FLORIST_ADVERTISEMENT_2,
	FLORIST_ADVERTISEMENT_3,
	FLORIST_ADVERTISEMENT_4,
	FLORIST_ADVERTISEMENT_5,
	FLORIST_ADVERTISEMENT_6,
	FLORIST_ADVERTISEMENT_7,
	FLORIST_ADVERTISEMENT_8,
	FLORIST_ADVERTISEMENT_9,
	TEXT_NUM_FLORIST,
};
extern		STR16			sFloristText[];


//Florist Order Form
enum
{
	FLORIST_ORDER_BACK,		
	FLORIST_ORDER_SEND,
	FLORIST_ORDER_CLEAR,
	FLORIST_ORDER_GALLERY,
	FLORIST_ORDER_NAME_BOUQUET,
	FLORIST_ORDER_PRICE,					//5
	FLORIST_ORDER_ORDER_NUMBER,
	FLORIST_ORDER_DELIVERY_DATE,
	FLORIST_ORDER_NEXT_DAY,
	FLORIST_ORDER_GETS_THERE,
	FLORIST_ORDER_DELIVERY_LOCATION,			//10
	FLORIST_ORDER_ADDITIONAL_CHARGES,
	FLORIST_ORDER_CRUSHED,
	FLORIST_ORDER_BLACK_ROSES,
	FLORIST_ORDER_WILTED,
	FLORIST_ORDER_FRUIT_CAKE,							//15
	FLORIST_ORDER_PERSONAL_SENTIMENTS,
	FLORIST_ORDER_CARD_LENGTH,
	FLORIST_ORDER_SELECT_FROM_OURS,
	FLORIST_ORDER_STANDARDIZED_CARDS,
	FLORIST_ORDER_BILLING_INFO,							//20
	FLORIST_ORDER_NAME,
	TEXT_NUM_FLORIST_ORDER,
};
extern		STR16			sOrderFormText[];



//Florist Gallery.c
enum
{
	FLORIST_GALLERY_PREV,
	FLORIST_GALLERY_NEXT,
	FLORIST_GALLERY_CLICK_TO_ORDER,
	FLORIST_GALLERY_ADDIFTIONAL_FEE,
	FLORIST_GALLERY_HOME,
	TEXT_NUM_FLORIST_GALLERY,
};
extern		STR16		sFloristGalleryText[];


//Florist Cards
enum
{
	FLORIST_CARDS_CLICK_SELECTION,
	FLORIST_CARDS_BACK,
	TEXT_NUM_FLORIST_CARDS,
};
extern		STR16			sFloristCards[];

// Bobbyr Mail Order.c
enum
{
	BOBBYR_ORDER_FORM,
	BOBBYR_QTY,
	BOBBYR_WEIGHT,
	BOBBYR_NAME,
	BOBBYR_UNIT_PRICE,
	BOBBYR_TOTAL,
	BOBBYR_SUB_TOTAL,
	BOBBYR_S_H,
	BOBBYR_GRAND_TOTAL,
	BOBBYR_SHIPPING_LOCATION,
	BOBBYR_SHIPPING_SPEED,
	BOBBYR_COST,
	BOBBYR_OVERNIGHT_EXPRESS,
	BOBBYR_BUSINESS_DAYS,
	BOBBYR_STANDARD_SERVICE,
	BOBBYR_CLEAR_ORDER,
	BOBBYR_ACCEPT_ORDER,
	BOBBYR_BACK,
	BOBBYR_HOME,
	BOBBYR_USED_TEXT,
	BOBBYR_CANT_AFFORD_PURCHASE,
	BOBBYR_SELECT_DEST,
	BOBBYR_CONFIRM_DEST,
	BOBBYR_PACKAGE_WEIGHT,
	BOBBYR_MINIMUM_WEIGHT,
	BOBBYR_GOTOSHIPMENT_PAGE,
	TEXT_NUM_BOBBYR_MAILORDER,
};
extern		STR16			BobbyROrderFormText[];

enum
{
	// Guns
	BOBBYR_FILTER_GUNS_PISTOL,
	BOBBYR_FILTER_GUNS_M_PISTOL,
	BOBBYR_FILTER_GUNS_SMG,
	BOBBYR_FILTER_GUNS_RIFLE,
	BOBBYR_FILTER_GUNS_SN_RIFLE,
	BOBBYR_FILTER_GUNS_AS_RIFLE,
	BOBBYR_FILTER_GUNS_LMG,
	BOBBYR_FILTER_GUNS_SHOTGUN,
	BOBBYR_FILTER_GUNS_HEAVY,
	// Ammo
	BOBBYR_FILTER_AMMO_PISTOL,
	BOBBYR_FILTER_AMMO_M_PISTOL,
	BOBBYR_FILTER_AMMO_SMG,
	BOBBYR_FILTER_AMMO_RIFLE,
	BOBBYR_FILTER_AMMO_SN_RIFLE,
	BOBBYR_FILTER_AMMO_AS_RIFLE,
	BOBBYR_FILTER_AMMO_LMG,
	BOBBYR_FILTER_AMMO_SHOTGUN,
	//BOBBYR_FILTER_AMMO_HEAVY,
	// Used
	BOBBYR_FILTER_USED_GUNS,
	BOBBYR_FILTER_USED_ARMOR,
	BOBBYR_FILTER_USED_LBEGEAR,
	BOBBYR_FILTER_USED_MISC,
	// Armour
	BOBBYR_FILTER_ARMOUR_HELM,
	BOBBYR_FILTER_ARMOUR_VEST,
	BOBBYR_FILTER_ARMOUR_LEGGING,
	BOBBYR_FILTER_ARMOUR_PLATE,
	// Misc
	BOBBYR_FILTER_MISC_BLADE,
	BOBBYR_FILTER_MISC_THROWING_KNIFE,
	BOBBYR_FILTER_MISC_PUNCH,
	BOBBYR_FILTER_MISC_GRENADE,
	BOBBYR_FILTER_MISC_BOMB,
	BOBBYR_FILTER_MISC_MEDKIT,
	BOBBYR_FILTER_MISC_KIT,
	BOBBYR_FILTER_MISC_FACE,
	BOBBYR_FILTER_MISC_LBEGEAR,
	BOBBYR_FILTER_MISC_OPTICS_ATTACHMENTS, // Madd: New BR filter option
	BOBBYR_FILTER_MISC_SIDE_AND_BOTTOM_ATTACHMENTS, // Madd: New BR filter option
	BOBBYR_FILTER_MISC_MUZZLE_ATTACHMENTS, // Madd: New BR filter option
	BOBBYR_FILTER_MISC_STOCK_ATTACHMENTS, // Madd: New BR filter option
	BOBBYR_FILTER_MISC_INTERNAL_ATTACHMENTS, // Madd: New BR filter option
	BOBBYR_FILTER_MISC_OTHER_ATTACHMENTS, // Madd: New BR filter option
	BOBBYR_FILTER_MISC_MISC,
	TEXT_NUM_BOBBYR_FILTER
};


//BobbyRGuns.c
enum
{
	BOBBYR_GUNS_TO_ORDER,
	BOBBYR_GUNS_CLICK_ON_ITEMS,
	BOBBYR_GUNS_PREVIOUS_ITEMS,
	BOBBYR_GUNS_GUNS,
	BOBBYR_GUNS_AMMO,
	BOBBYR_GUNS_ARMOR,		//5
	BOBBYR_GUNS_MISC,
	BOBBYR_GUNS_USED,
	BOBBYR_GUNS_MORE_ITEMS,
	BOBBYR_GUNS_ORDER_FORM,
	BOBBYR_GUNS_HOME,					//10
	BOBBYR_GUNS_NUM_GUNS_THAT_USE_AMMO_1,
	BOBBYR_GUNS_NUM_GUNS_THAT_USE_AMMO_2,

	BOBBYR_GUNS_WGHT,
	BOBBYR_GUNS_CALIBRE,
	BOBBYR_GUNS_MAGAZINE,
	BOBBYR_GUNS_RANGE,
	BOBBYR_GUNS_DAMAGE,
	BOBBYR_GUNS_ROF,
	BOBBYR_GUNS_AP,
	BOBBYR_GUNS_STUN,
	BOBBYR_GUNS_PROTECTION,
	BOBBYR_GUNS_CAMO,
	BOBBYR_GUNS_ARMOUR_PIERCING_MODIFIER,
	BOBBYR_GUNS_BULLET_TUMBLE_MODIFIER,
	BOBBYR_GUNS_NUM_PROJECTILES,
	BOBBYR_GUNS_COST,
	BOBBYR_GUNS_IN_STOCK,
	BOBBYR_GUNS_QTY_ON_ORDER,
	BOBBYR_GUNS_DAMAGED,
	BOBBYR_GUNS_WEIGHT,
	BOBBYR_GUNS_SUB_TOTAL,
	BOBBYR_GUNS_PERCENT_FUNCTIONAL,

	BOBBYR_MORE_THEN_10_PURCHASES_A,
	BOBBYR_MORE_THEN_10_PURCHASES_B,
	BOBBYR_MORE_NO_MORE_IN_STOCK,
	BOBBYR_NO_MORE_STOCK,
	TEXT_NUM_BOBBYR_GUNS,
};

extern		STR16			BobbyRText[];
extern		STR16			BobbyRFilter[];


//BobbyR.c
enum
{
	BOBBYR_ADVERTISMENT_1,
	BOBBYR_ADVERTISMENT_2,
	BOBBYR_USED,
	BOBBYR_MISC,
	BOBBYR_GUNS,
	BOBBYR_AMMO,
	BOBBYR_ARMOR,
	BOBBYR_ADVERTISMENT_3,
	BOBBYR_UNDER_CONSTRUCTION,
	TEXT_NUM_BOBBYR
};
extern		STR16			BobbyRaysFrontText[];

//Aim Sort.c
enum
{
	AIM_AIMMEMBERS,
	SORT_BY,
	PRICE,
	EXPERIENCE,
	AIMMARKSMANSHIP,
	AIMMECHANICAL,
	AIMEXPLOSIVES,
	AIMMEDICAL,
	AIMHEALTH,
	AIMAGILITY,
	AIMDEXTERITY,
	AIMSTRENGTH,
	AIMLEADERSHIP,
	AIMWISDOM,
	NAME,
	MUGSHOT_INDEX,
	MERCENARY_FILES,
	ALUMNI_GALLERY,
	ASCENDING,
	DESCENDING,
	TEXT_NUM_AIM_SORT
};
extern		STR16			AimSortText[];

//Aim Policies.c
enum
{
	AIM_POLICIES_PREVIOUS,
	AIM_POLICIES_HOMEPAGE,
	AIM_POLICIES_POLICY,
	AIM_POLICIES_NEXT_PAGE,
	AIM_POLICIES_DISAGREE,
	AIM_POLICIES_AGREE,
	TEXT_NUM_AIM_POLICIES
};
extern		STR16		AimPolicyText[];




//Aim Member.c
enum
{
	AIM_MEMBER_CLICK_INSTRUCTIONS,
	TEXT_NUM_AIM_MEMBER_TEXT
};
extern		STR16			AimMemberText[];



//Aim Member.c
enum
{
	AIM_MEMBER_HEALTH,
	AIM_MEMBER_AGILITY,
	AIM_MEMBER_DEXTERITY,
	AIM_MEMBER_STRENGTH,
	AIM_MEMBER_LEADERSHIP,
	AIM_MEMBER_WISDOM,				//5
	AIM_MEMBER_EXP_LEVEL,
	AIM_MEMBER_MARKSMANSHIP,
	AIM_MEMBER_MECHANICAL,
	AIM_MEMBER_EXPLOSIVE,
	AIM_MEMBER_MEDICAL,				//10
	AIM_MEMBER_FEE,
	AIM_MEMBER_CONTRACT,
	AIM_MEMBER_1_DAY,
	AIM_MEMBER_1_WEEK,
	AIM_MEMBER_2_WEEKS,				//15
	AIM_MEMBER_PREVIOUS,
	AIM_MEMBER_CONTACT,
	AIM_MEMBER_NEXT,
	AIM_MEMBER_ADDTNL_INFO,
	AIM_MEMBER_ACTIVE_MEMBERS,	//20
	AIM_MEMBER_OPTIONAL_GEAR,
	AIM_MEMBER_OPTIONAL_GEAR_NSGI,
	AIM_MEMBER_MEDICAL_DEPOSIT_REQ,
	AIM_MEMBER_GEAR_KIT_ONE,
	AIM_MEMBER_GEAR_KIT_TWO,	//25
	AIM_MEMBER_GEAR_KIT_THREE,
	AIM_MEMBER_GEAR_KIT_FOUR,
	AIM_MEMBER_GEAR_KIT_FIVE,
	TEXT_NUM_AIM_MEMBER_CHARINFO,
};
extern		STR16			CharacterInfo[];



//Aim Member.c
enum
{
	AIM_MEMBER_CONTRACT_CHARGE,
	AIM_MEMBER_ONE_DAY,
	AIM_MEMBER_ONE_WEEK,
	AIM_MEMBER_TWO_WEEKS,
	AIM_MEMBER_NO_EQUIPMENT,
	AIM_MEMBER_BUY_EQUIPMENT,		//5
	AIM_MEMBER_TRANSFER_FUNDS,
	AIM_MEMBER_CANCEL,
	AIM_MEMBER_HIRE,
	AIM_MEMBER_HANG_UP,
	AIM_MEMBER_OK,							//10
	AIM_MEMBER_LEAVE_MESSAGE,
	AIM_MEMBER_VIDEO_CONF_WITH,
	AIM_MEMBER_CONNECTING,
	AIM_MEMBER_WITH_MEDICAL,		//14
	TEXT_NUM_AIM_MEMBER_VCONF
};
extern		STR16			VideoConfercingText[];

//Aim Member.c
enum
{
	AIM_MEMBER_FUNDS_TRANSFER_SUCCESFUL,
	AIM_MEMBER_FUNDS_TRANSFER_FAILED,
	AIM_MEMBER_NOT_ENOUGH_FUNDS,

	AIM_MEMBER_ON_ASSIGNMENT,
	AIM_MEMBER_LEAVE_MSG,
	AIM_MEMBER_DEAD,

	AIM_MEMBER_ALREADY_HAVE_MAX_MERCS,

	AIM_MEMBER_PRERECORDED_MESSAGE,
	AIM_MEMBER_MESSAGE_RECORDED,
	TEXT_NUM_AIM_MEMBER_POPUP
};
extern		STR16			AimPopUpText[];

//AIM Link.c
enum
{
	AIM_LINK_TITLE,
	TEXM_NUM_AIM_LINK,
};
extern		STR16			AimLinkText[];


//Aim History
enum
{
	AIM_HISTORY_TITLE,
	AIM_HISTORY_PREVIOUS,
	AIM_HISTORY_HOME,
	AIM_HISTORY_AIM_ALUMNI,
	AIM_HISTORY_NEXT,
	TEXT_NUM_AIM_HISTORY,
};
extern		STR16			AimHistoryText[];



//Aim Facial Index
enum
{
	AIM_FI_PRICE,
	AIM_FI_EXP,
	AIM_FI_MARKSMANSHIP,
	AIM_FI_MECHANICAL,
	AIM_FI_EXPLOSIVES,
	AIM_FI_MEDICAL,
	AIM_FI_HEALTH,
	AIM_FI_AGILITY,
	AIM_FI_DEXTERITY,
	AIM_FI_STRENGTH,
	AIM_FI_LEADERSHIP,
	AIM_FI_WISDOM,
	AIM_FI_NAME,
	AIM_FI_AIM_MEMBERS_SORTED_ASCENDING,
	AIM_FI_AIM_MEMBERS_SORTED_DESCENDING,
	AIM_FI_LEFT_CLICK,
	AIM_FI_TO_SELECT,			
	AIM_FI_RIGHT_CLICK,
	AIM_FI_TO_ENTER_SORT_PAGE,
	AIM_FI_AWAY,
	AIM_FI_DEAD,
	AIM_FI_ON_ASSIGN,
	TEXT_NUM_AIM_FI,
};
extern		STR16			AimFiText[];


//AimArchives.
enum
{
	AIM_ALUMNI_PAGE_1,
	AIM_ALUMNI_PAGE_2,
	AIM_ALUMNI_PAGE_3,
	AIM_ALUMNI_ALUMNI,
	AIM_ALUMNI_DONE,
	TEXT_NUM_AIM_ALUMNI,
};
extern		STR16			AimAlumniText[];



//Aim Home Page
enum
{
//	AIM_INFO_1,
//	AIM_INFO_2,
//	AIM_POLICIES,
//	AIM_HISTORY,
//	AIM_LINKS,		//5
	AIM_INFO_3,
	AIM_INFO_4,
	AIM_INFO_5,
	AIM_INFO_6,
	AIM_INFO_7,			//9
	AIM_BOBBYR_ADD1,
	AIM_BOBBYR_ADD2,
	AIM_BOBBYR_ADD3,
	TEXT_NUM_AIM_SCREEN
};

extern		STR16			AimScreenText[];

//Aim Home Page
enum
{
	AIM_HOME,
	AIM_MEMBERS,
	AIM_ALUMNI,
	AIM_POLICIES,
	AIM_HISTORY,
	AIM_LINKS,
	TEXT_NUM_AIM_MENU
};

extern STR16			AimBottomMenuText[];



// MapScreen
enum
{
	MAP_SCREEN_MAP_LEVEL,
	MAP_SCREEN_NO_MILITIA_TEXT,
	TEXT_NUM_MAP_SCREEN,
};
extern	STR16		zMarksMapScreenText[];



//Weapon Name and Description size
#define		ITEMSTRINGFILENAME				"BINARYDATA\\ITEMDESC.EDT"
#define	SIZE_ITEM_NAME						160
#define	SIZE_SHORT_ITEM_NAME			160
#define	SIZE_ITEM_INFO						480
#define	SIZE_ITEM_PROS						160
#define	SIZE_ITEM_CONS						160

BOOLEAN LoadItemInfo(UINT16 ubIndex, STR16 pNameString, STR16 pInfoString );
extern void LoadAllExternalText( void );
BOOLEAN LoadShortNameItemInfo(UINT16 ubIndex, STR16 pNameString );
BOOLEAN LoadItemProsAndCons( UINT16 usIndex, STR16 pProsString, STR16 pConsString );
BOOLEAN LoadBRDesc(UINT16 ubIndex, STR16 pDescString );
BOOLEAN LoadBRName(UINT16 ubIndex, STR16 pNameString );

// sevenfm
inline std::string narrow(std::wstring const& text);
// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str);
// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str);

enum
{
	//Coordinating simultaneous arrival dialog strings
	STR_DETECTED_SIMULTANEOUS_ARRIVAL,
	STR_DETECTED_SINGULAR,
	STR_DETECTED_PLURAL,
	STR_COORDINATE,
	//AutoResove Enemy capturing strings
	STR_ENEMY_SURRENDER_OFFER,
	STR_ENEMY_CAPTURED,
	//AutoResolve Text buttons
	STR_AR_RETREAT_BUTTON,
	STR_AR_DONE_BUTTON,
	//AutoResolve header text
	STR_AR_DEFEND_HEADER,
	STR_AR_ATTACK_HEADER,
	STR_AR_ENCOUNTER_HEADER,
	STR_AR_SECTOR_HEADER,
	//String for AutoResolve battle over conditions
	STR_AR_OVER_VICTORY,
	STR_AR_OVER_DEFEAT,
	STR_AR_OVER_SURRENDERED,
	STR_AR_OVER_CAPTURED,
	STR_AR_OVER_RETREATED,
	STR_AR_MILITIA_NAME,
	STR_AR_ELITE_NAME,
	STR_AR_TROOP_NAME,
	STR_AR_ADMINISTRATOR_NAME,
	STR_AR_CREATURE_NAME,
	STR_AR_TIME_ELAPSED,
	STR_AR_MERC_RETREATED,
	STR_AR_MERC_RETREATING,
	STR_AR_MERC_RETREAT,
	//Strings for prebattle interface
	STR_PB_AUTORESOLVE_BTN,
	STR_PB_GOTOSECTOR_BTN,
	STR_PB_RETREATMERCS_BTN,
	STR_PB_ENEMYENCOUNTER_HEADER,
	STR_PB_ENEMYINVASION_HEADER,
	STR_PB_ENEMYAMBUSH_HEADER,
	STR_PB_ENTERINGENEMYSECTOR_HEADER,
	STR_PB_CREATUREATTACK_HEADER,
	STR_PB_BLOODCATAMBUSH_HEADER,
	STR_PB_ENTERINGBLOODCATLAIR_HEADER,
	STR_PB_ENEMYINVASION_AIRDROP_HEADER,
	STR_PB_LOCATION,
	STR_PB_ENEMIES,
	STR_PB_MERCS,
	STR_PB_MILITIA,
	STR_PB_CREATURES,
	STR_PB_BLOODCATS,
	STR_PB_SECTOR,
	STR_PB_NONE,
	STR_PB_NOTAPPLICABLE_ABBREVIATION,
	STR_PB_DAYS_ABBREVIATION,
	STR_PB_HOURS_ABBREVIATION,
	//Strings for the tactical placement gui
	//The four buttons and it's help text.
	STR_TP_CLEAR,
	STR_TP_SPREAD,
	STR_TP_GROUP,
	STR_TP_DONE,
	STR_TP_CLEARHELP,
	STR_TP_SPREADHELP,
	STR_TP_GROUPHELP,
	STR_TP_DONEHELP,
	STR_TP_DISABLED_DONEHELP,
	//various strings.
	STR_TP_SECTOR,
	STR_TP_CHOOSEENTRYPOSITIONS,
	STR_TP_INACCESSIBLE_MESSAGE,
	STR_TP_INVALID_MESSAGE,
	STR_TP_NAME_HASARRIVEDINSECTOR_XX,
	STR_PB_AUTORESOLVE_FASTHELP,
	STR_PB_DISABLED_AUTORESOLVE_FASTHELP,
	STR_PB_GOTOSECTOR_FASTHELP,
	STR_BP_RETREATSINGLE_FASTHELP,
	STR_BP_RETREATPLURAL_FASTHELP,

	//various popup messages for battle,
	STR_DIALOG_ENEMIES_ATTACK_MILITIA,
	STR_DIALOG_CREATURES_ATTACK_MILITIA,
	STR_DIALOG_CREATURES_KILL_CIVILIANS,
	STR_DIALOG_ENEMIES_ATTACK_UNCONCIOUSMERCS,
	STR_DIALOG_CREATURES_ATTACK_UNCONCIOUSMERCS,

	// Flugente: militia movement forbidden due to limited roaming
	STR_MILITIAMOVEMENT_NO_LIMITEDROAMING,
	STR_MILITIAMOVEMENT_NO_STAFF_ABORT,

	STR_AR_ROBOT_NAME,
	STR_AR_TANK_NAME,
	STR_AR_JEEP_NAME,

	STR_BREATH_REGEN_SLEEP,

	STR_PB_ZOMBIES,
	STR_PB_BANDITS,
	STR_PB_BLOODCATRAID_HEADER,
	STR_PB_ZOMBIERAID_HEADER,
	STR_PB_BANDITRAID_HEADER,
	STR_PB_ZOMBIE,
	STR_PB_BANDIT,
	STR_PB_BANDIT_KILLCIVS_IN_SECTOR,

	TEXT_NUM_STRATEGIC_TEXT
};

//Strings used in conjunction with above enumerations
extern STR16 gpStrategicString[];

enum
{
	STR_GAMECLOCK_DAY_NAME,
	TEXT_NUM_GAMECLOCK,
};
extern STR16 gpGameClockString[];

//enums for the Shopkeeper Interface
enum
{
	SKI_TEXT_MERCHADISE_IN_STOCK,
	SKI_TEXT_PAGE,
	SKI_TEXT_TOTAL_COST,
	SKI_TEXT_TOTAL_VALUE,
	SKI_TEXT_EVALUATE,
	SKI_TEXT_TRANSACTION,
	SKI_TEXT_DONE,
	SKI_TEXT_REPAIR_COST,
	SKI_TEXT_ONE_HOUR,
	SKI_TEXT_PLURAL_HOURS,
	SKI_TEXT_REPAIRED,
	SKI_TEXT_NO_MORE_ROOM_IN_PLAYER_OFFER_AREA,
	SKI_TEXT_MINUTES,
	SKI_TEXT_DROP_ITEM_TO_GROUND,
	SKI_TEXT_BUDGET,
	TEXT_NUM_SKI_TEXT
};
extern	STR16			SKI_Text[];

//ShopKeeper Interface
enum
{
	SKI_ATM_0,
	SKI_ATM_1,
	SKI_ATM_2,
	SKI_ATM_3,
	SKI_ATM_4,
	SKI_ATM_5,
	SKI_ATM_6,
	SKI_ATM_7,
	SKI_ATM_8,
	SKI_ATM_9,
	SKI_ATM_OK,
	SKI_ATM_TAKE,
	SKI_ATM_GIVE,
	SKI_ATM_CANCEL,
	SKI_ATM_CLEAR,

	NUM_SKI_ATM_BUTTONS
};
extern	STR16	SkiAtmText[];

//ShopKeeper Interface
enum
{
	SKI_ATM_MODE_TEXT_SELECT_MODE,
	SKI_ATM_MODE_TEXT_ENTER_AMOUNT,
	SKI_ATM_MODE_TEXT_SELECT_TO_MERC,
	SKI_ATM_MODE_TEXT_SELECT_FROM_MERC,
	SKI_ATM_MODE_TEXT_SELECT_INUSUFFICIENT_FUNDS,
	SKI_ATM_MODE_TEXT_BALANCE,
	TEXT_NUM_SKI_ATM_MODE_TEXT,
};
extern	STR16	gzSkiAtmText[];

//ShopKeeperInterface Message Box defines
enum
{
	SKI_QUESTION_TO_DEDUCT_MONEY_FROM_PLAYERS_ACCOUNT_TO_COVER_DIFFERENCE,
	SKI_SHORT_FUNDS_TEXT,
	SKI_QUESTION_TO_DEDUCT_MONEY_FROM_PLAYERS_ACCOUNT_TO_COVER_COST,

	SKI_TRANSACTION_BUTTON_HELP_TEXT,
	SKI_REPAIR_TRANSACTION_BUTTON_HELP_TEXT,
	SKI_DONE_BUTTON_HELP_TEXT,

	SKI_PLAYERS_CURRENT_BALANCE,

	SKI_QUESTION_TO_DEDUCT_INTEL_FROM_PLAYERS_ACCOUNT_TO_COVER_DIFFERENCE,
	SKI_QUESTION_TO_DEDUCT_INTEL_FROM_PLAYERS_ACCOUNT_TO_COVER_COST,

	TEXT_NUM_SKI_MBOX_TEXT
};

extern	STR16	SkiMessageBoxText[];


//enums for the above text
enum
{
	SLG_SAVE_GAME,
	SLG_LOAD_GAME,
	SLG_CANCEL,
	SLG_SAVE_SELECTED,
	SLG_LOAD_SELECTED,
	SLG_SAVE_GAME_OK,				//5
	SLG_SAVE_GAME_ERROR,
	SLG_LOAD_GAME_OK,
	SLG_LOAD_GAME_ERROR,
	SLG_GAME_VERSION_DIF,
	SLG_DELETE_ALL_SAVE_GAMES,	//10
	SLG_SAVED_GAME_VERSION_DIF,
	SLG_BOTH_GAME_AND_SAVED_GAME_DIF,
	SLG_CONFIRM_SAVE,
	SLG_CONFIRM_LOAD,
	SLG_NOT_ENOUGH_HARD_DRIVE_SPACE,		//15
	SLG_SAVING_GAME_MESSAGE,
	SLG_NORMAL_GUNS,
	SLG_ADDITIONAL_GUNS,
	SLG_REALISTIC,
	SLG_SCIFI,		//20

	SLG_DIFF,
	SLG_PLATINUM,

	SLG_BR_QUALITY_TEXT,
	SLG_BR_GOOD_TEXT,
	SLG_BR_GREAT_TEXT,
	SLG_BR_EXCELLENT_TEXT,
	SLG_BR_AWESOME_TEXT,

	SLG_INV_RES_ERROR,
	SLG_INV_CUSTUM_ERROR,

	SLG_SQUAD_SIZE_RES_ERROR,

	SLG_BR_QUANTITY_TEXT,

	TEXT_NUM_SLG_TEXT,
};
extern	STR16			zSaveLoadText[];



//OptionScreen.h
// defines used for the zOptionsText
enum
{
	OPT_SAVE_GAME,
	OPT_LOAD_GAME,
	OPT_MAIN_MENU,
	OPT_NEXT,
	OPT_PREV,
	OPT_DONE,
	OPT_113_FEATURES,
	OPT_NEW_IN_113,
	OPT_OPTIONS,
	OPT_SOUND_FX,
	OPT_SPEECH,
	OPT_MUSIC,
	OPT_RETURN_TO_MAIN,
	OPT_NEED_AT_LEAST_SPEECH_OR_SUBTITLE_OPTION_ON,
	TEXT_NUM_OPT_TEXT,
};

extern	STR16	zOptionsText[];

extern	STR16	z113FeaturesScreenText[]; // main UI text
extern	STR16	z113FeaturesToggleText[]; // toggle button text
extern	STR16	z113FeaturesHelpText[]; // hover text
extern	STR16	z113FeaturesPanelText[]; // left panel text

//used with the gMoneyStatsDesc[]
enum
{
	MONEY_DESC_AMOUNT,
	MONEY_DESC_REMAINING,
	MONEY_DESC_AMOUNT_2_SPLIT,
	MONEY_DESC_TO_SPLIT,

	MONEY_DESC_PLAYERS,
	MONEY_DESC_BALANCE,
	MONEY_DESC_AMOUNT_2_WITHDRAW,
	MONEY_DESC_TO_WITHDRAW,
	TEXT_NUM_MONEY_DESC,
};


// used with gzMoneyWithdrawMessageText
enum
{
	MONEY_TEXT_WITHDRAW_MORE_THEN_MAXIMUM,
	CONFIRMATION_TO_DEPOSIT_MONEY_TO_ACCOUNT,
	TEXT_NUM_MONEY_WITHDRAW
};



// Game init option screen
enum
{
	GIO_INITIAL_GAME_SETTINGS,

	GIO_GAME_STYLE_TEXT,
	GIO_REALISTIC_TEXT,
	GIO_SCI_FI_TEXT,
	GIO_PLATINUM_TEXT,

	GIO_GUN_OPTIONS_TEXT,
	GIO_GUN_NUT_TEXT,
	GIO_REDUCED_GUNS_TEXT,
	
	GIO_DIF_LEVEL_TEXT,
	GIO_EASY_TEXT,
	GIO_MEDIUM_TEXT,
	GIO_HARD_TEXT,
	GIO_INSANE_TEXT,

	GIO_START_TEXT,
	GIO_CANCEL_TEXT,

	GIO_GAME_SAVE_STYLE_TEXT,
	GIO_SAVE_ANYWHERE_TEXT,
	GIO_IRON_MAN_TEXT,
	GIO_DISABLED_FOR_THE_DEMO_TEXT,

	GIO_BR_QUALITY_TEXT,
	GIO_BR_GOOD_TEXT,
	GIO_BR_GREAT_TEXT,
	GIO_BR_EXCELLENT_TEXT,
	GIO_BR_AWESOME_TEXT,

	GIO_INV_TEXT,
	GIO_INV_OLD_TEXT,
	GIO_INV_NEW_TEXT,
	GIO_LOAD_MP_GAME,
	GIO_INITIAL_GAME_SETTINGS_MP,
	////////////////////////////////////
	// SANDRO - added following
	GIO_TRAITS_TEXT,
	GIO_TRAITS_OLD_TEXT,
	GIO_TRAITS_NEW_TEXT,
	GIO_IMP_NUMBER_TITLE_TEXT,
	GIO_IMP_NUMBER_1,
	GIO_IMP_NUMBER_2,
	GIO_IMP_NUMBER_3,
	GIO_IMP_NUMBER_4,
	GIO_IMP_NUMBER_5,
	GIO_IMP_NUMBER_6,
	GIO_DROPALL_TITLE_TEXT,
	GIO_DROPALL_OFF_TEXT,
	GIO_DROPALL_ON_TEXT,
	GIO_TERRORISTS_TITLE_TEXT,
	GIO_TERRORISTS_RANDOM_TEXT,
	GIO_TERRORISTS_ALL_TEXT,
	GIO_CACHES_TITLE_TEXT,
	GIO_CACHES_RANDOM_TEXT,
	GIO_CACHES_ALL_TEXT,
	GIO_PROGRESS_TITLE_TEXT,
	GIO_PROGRESS_VERY_SLOW_TEXT,
	GIO_PROGRESS_SLOW_TEXT,
	GIO_PROGRESS_NORMAL_TEXT,
	GIO_PROGRESS_FAST_TEXT,
	GIO_PROGRESS_VERY_FAST_TEXT,

	// WANNE: New strings for start new game screen (for NAS)
	GIO_INV_SETTING_OLD_TEXT,
	GIO_INV_SETTING_NEW_TEXT,
	GIO_INV_SETTING_NEW_NAS_TEXT,

	// WANNE: Squad size
	GIO_SQUAD_SIZE_TITLE_TEXT,
	GIO_SQUAD_SIZE_6_TEXT,
	GIO_SQUAD_SIZE_8_TEXT,
	GIO_SQUAD_SIZE_10_TEXT,

	//GIO_FAST_BR_TITLE_TEXT,

	//Inventory AP Cost
	GIO_INVENTORY_AP_TITLE_TEXT,
	
	GIO_NCTH_TITLE_TEXT,
	GIO_IIS_TITLE_TEXT,
	GIO_BACKGROUND_TITLE_TEXT,
	GIO_FOODSYSTEM_TITLE_TEXT,
	GIO_BR_QUANTITY_TEXT,

	// anv: new iron man modes
	GIO_ALMOST_IRON_MAN_TEXT,
	GIO_EXTREME_IRON_MAN_TEXT,
	GIO_ULTIMATE_IRON_MAN_TEXT,

	////////////////////////////////////
	TEXT_NUM_GIO_TEXT
};
extern STR16	gzGIOScreenText[];

// OJW - 20081129
// Multiplayer Join Screen
enum 
{
	MPJ_TITLE_TEXT,
	MPJ_JOIN_TEXT,
	MPJ_HOST_TEXT,
	MPJ_CANCEL_TEXT,
	MPJ_REFRESH_TEXT,
	MPJ_HANDLE_TEXT,
	MPJ_SERVERIP_TEXT,
	MPJ_SERVERPORT_TEXT,
	MPJ_SERVERNAME_TEXT,
	MPJ_NUMPLAYERS_TEXT,
	MPJ_SERVERVER_TEXT,
	MPJ_GAMETYPE_TEXT,
	MPJ_PING_TEXT,
	MPJ_HANDLE_INVALID,
	MPJ_SERVERIP_INVALID,
	MPJ_SERVERPORT_INVALID,	
	TEXT_NUM_MPJ_TEXT
};

extern STR16	gzMPJHelpText[];

extern STR16	gzMPJScreenText[];
//Multiplayer Host Screen
enum
{
	MPH_TITLE_TEXT,
	MPH_START_TEXT,
	MPH_CANCEL_TEXT,
	MPH_SERVERNAME_TEXT,
	MPH_GAMETYPE_TEXT,
	MPH_DEATHMATCH_TEXT,
	MPH_TEAMDM_TEXT,
	MPH_COOP_TEXT,
	MPH_NUMPLAYERS_TEXT,
	MPH_SQUADSIZE_TEXT,
	MPH_MERCSELECT_TEXT,
	MPH_RANDOMMERCS_TEXT,
	MPH_PLAYERMERCS_TEXT,
	MPH_BALANCE_TEXT,
	MPH_SAMEMERC_TEXT,
	MPH_RPTMERC_TEXT,
	MPH_BOBBYRAY_TEXT,
	MPH_RNDMSTART_TEXT,
	MPH_SERVERNAME_INVALID,
	MPH_MAXPLAYERS_INVALID,
	MPH_SQUADSIZE_INVALID,
	MPH_TIME_TEXT,
	MPH_TIME_INVALID,
	MPH_CASH_INVALID,
	MPH_DMG_TEXT,
	MPH_DMG_INVALID,
	MPH_TIMER_TEXT,
	MPH_TIMER_INVALID,
	MPH_ENABLECIV_TEXT,
	MPH_USENIV_TEXT,
	MPH_OVERRIDEMAXAI_TEXT,
	MPH_SYNC_GAME_DIRECTORY,
	MPH_FILE_TRANSFER_DIR_TEXT,
	MPH_FILE_TRANSFER_DIR_INVALID,
	MPH_FILE_TRANSFER_DIR_TEXT_ADDITIONAL,
	MPH_FILE_TRANSFER_DIR_NOT_EXIST,
	MPH_1,
	MPH_2,
	MPH_3,
	MPH_4,
	MPH_5,
	MPH_6,
	MPH_YES,
	MPH_NO,	
	MPH_MORNING,
	MPH_AFTERNOON,
	MPH_NIGHT,
	MPH_CASH_LOW,
	MPH_CASH_MEDIUM,
	MPH_CASH_HIGH,
	MPH_CASH_UNLIMITED,
	MPH_TIME_NEVER,
	MPH_TIME_SLOW,
	MPH_TIME_MEDIUM,
	MPH_TIME_FAST,
	MPH_DAMAGE_VERYLOW,
	MPH_DAMAGE_LOW,
	MPH_DAMAGE_NORMAL,
	MPH_HIRE_RANDOM,
	MPH_HIRE_NORMAL,
	MPH_EDGE_RANDOM,
	MPH_EDGE_SELECTABLE,
	MPH_DISABLE,
	MPH_ALLOW,
	TEXT_NUM_MPH_TEXT,
};
extern STR16	gzMPHScreenText[];
enum
{
	MPS_TITLE_TEXT,
	MPS_CONTINUE_TEXT,
	MPS_CANCEL_TEXT,
	MPS_PLAYER_TEXT,
	MPS_KILLS_TEXT,
	MPS_DEATHS_TEXT,
	MPS_AITEAM_TEXT,
	MPS_HITS_TEXT,
	MPS_MISSES_TEXT,
	MPS_ACCURACY_TEXT,
	MPS_DMGDONE_TEXT,
	MPS_DMGTAKEN_TEXT,
	MPS_WAITSERVER_TEXT,
	TEXT_NUM_MPS_TEXT,
};
extern STR16	gzMPSScreenText[];
enum
{
	MPC_CANCEL_TEXT,
	MPC_CONNECTING_TEXT,
	MPC_GETSETTINGS_TEXT,
	MPC_DOWNLOADING_TEXT,
	MPC_HELP1_TEXT,
	MPC_HELP2_TEXT,
	MPC_READY_TEXT,
	TEXT_NUM_MPC_TEXT,
};
extern STR16	gzMPCScreenText[];
// Multiplayer Starting Edges
enum
{
	MP_EDGE_NORTH,
	MP_EDGE_EAST,
	MP_EDGE_SOUTH,
	MP_EDGE_WEST,
	MP_EDGE_CENTER,
	MAX_EDGES,
};
extern STR16	gszMPEdgesText[];
// MP TEAM NAMES
enum
{
	MP_TEAM_1,
	MP_TEAM_2,
	MP_TEAM_3,
	MP_TEAM_4,
	MAX_MP_TEAMS,
};
extern STR16	gszMPTeamNames[];

extern STR16	gzMPChatToggleText[];

extern STR16	gzMPChatboxText[];

enum
{
	LAPTOP_BN_HLP_TXT_VIEW_EMAIL,
	LAPTOP_BN_HLP_TXT_BROWSE_VARIOUS_WEB_SITES,
	LAPTOP_BN_HLP_TXT_VIEW_FILES_AND_EMAIL_ATTACHMENTS,
	LAPTOP_BN_HLP_TXT_READ_LOG_OF_EVENTS,
	LAPTOP_BN_HLP_TXT_VIEW_TEAM_INFO,
	LAPTOP_BN_HLP_TXT_VIEW_FINANCIAL_SUMMARY_AND_HISTORY,
	LAPTOP_BN_HLP_TXT_CLOSE_LAPTOP,

	LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_MAIL,
	LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_FILE,


	BOOKMARK_TEXT_ASSOCIATION_OF_INTERNATION_MERCENARIES,
	BOOKMARK_TEXT_BOBBY_RAY_ONLINE_WEAPON_MAIL_ORDER,
	BOOKMARK_TEXT_INSTITUTE_OF_MERCENARY_PROFILING,
	BOOKMARK_TEXT_MORE_ECONOMIC_RECRUITING_CENTER,
	BOOKMARK_TEXT_MCGILLICUTTY_MORTUARY,
	BOOKMARK_TEXT_UNITED_FLORAL_SERVICE,
	BOOKMARK_TEXT_INSURANCE_BROKERS_FOR_AIM_CONTRACTS,
	TEXT_NUM_LAPTOP_BN_BOOKMARK_TEXT
};


//enums for the help screen
enum
{
	HLP_SCRN_TXT__EXIT_SCREEN,
	TEXT_NUM_HLP
};
extern	STR16	gzHelpScreenText[];


extern	STR16 gzLaptopHelpText[];


extern	STR16	gzMoneyWithdrawMessageText[];
extern STR16 gzCopyrightText[];



//enums used for the mapscreen inventory messages
enum
{
	MAPINV_MERC_ISNT_CLOSE_ENOUGH,
	MAPINV_CANT_SELECT_MERC,
	MAPINV_NOT_IN_SECTOR_TO_TAKE,
	MAPINV_CANT_PICKUP_IN_COMBAT,
	MAPINV_CANT_DROP_IN_COMBAT,
	MAPINV_NOT_IN_SECTOR_TO_DROP,
	TEXT_NUM_MAPINV
};


//the laptop broken link site
enum
{
	BROKEN_LINK_TXT_ERROR_404,
	BROKEN_LINK_TXT_SITE_NOT_FOUND,
	TEXT_NUM_BROKEN_LINK,
};
extern	STR16		BrokenLinkText[];

//Bobby rays page for recent shipments
enum
{
	BOBBYR_SHIPMENT__TITLE,
	BOBBYR_SHIPMENT__ORDER_NUM,
	BOBBYR_SHIPMENT__NUM_ITEMS,
	BOBBYR_SHIPMENT__ORDERED_ON,
	TEXT_NUM_BOBBYR_SHIPMENT,
};

extern STR16 gzBobbyRShipmentText[];


enum
{
	GIO_CFS_NOVICE,
	GIO_CFS_EXPERIENCED,
	GIO_CFS_EXPERT,
	GIO_CFS_INSANE,
	TEXT_NUM_GIO_CFS,
};
extern STR16 zGioDifConfirmText[];


enum
{
	CRDT_CAMFIELD,
	CRDT_SHAWN,
	CRDT_KRIS,
	CRDT_IAN,
	CRDT_LINDA,
	CRDT_ERIC,
	CRDT_LYNN,
	CRDT_NORM,
	CRDT_GEORGE,
	CRDT_STACEY,
	CRDT_SCOTT,
	CRDT_EMMONS,
	CRDT_DAVE,
	CRDT_ALEX,
	CRDT_JOEY,

	NUM_PEOPLE_IN_CREDITS,
};

STR16	gzCreditNames[];
STR16	gzCreditNameTitle[];
STR16	gzCreditNameFunny[];


extern STR16 GetWeightUnitString( void );
FLOAT GetWeightBasedOnMetricOption( UINT32 uiObjectWeight );


//SB: new 1.13 messages
extern STR16 New113Message[];
extern STR16 New113MERCMercMailTexts[];
extern STR16 MissingIMPSkillsDescriptions[];

extern STR16 New113AIMMercMailTexts[];	// WANNE: new WF Merc text, that does not exist in Email.edt

// HEADROCK: HAM Messages
extern STR16 New113HAMMessage[];
enum
{
	MSG113_STORM_STARTED,
	MSG113_STORM_ENDED,
	MSG113_RAIN_STARTED,
	MSG113_RAIN_ENDED,
	MSG113_WATHCHOUTFORSNIPERS,
	MSG113_SUPPRESSIONFIRE,
	MSG113_BRST,
	MSG113_AUTO,
	MSG113_GL,
	MSG113_GL_BRST,
	MSG113_GL_AUTO,
	MSG113_UB,
	MSG113_UB_BRST,
	MSG113_UB_AUTO,
	MSG113_BAYONET,
	MSG113_SNIPER,
	MSG113_UNABLETOSPLITMONEY,
	MSG113_ARRIVINGREROUTED,
	MSG113_DELETED,
	MSG113_DELETE_ALL,
	MSG113_SOLD,
	MSG113_SOLD_ALL,
	MSG113_CHECK_GOGGLES,
	MSG113_RTM_IN_COMBAT_ALREADY,
	MSG113_RTM_NO_ENEMIES,
	MSG113_RTM_SNEAKING_OFF,
	MSG113_RTM_SNEAKING_ON,
	MSG113_RTM_ENEMIES_SPOOTED,
	// added by SANDRO
	MSG113_THIEF_SUCCESSFUL,
	MSG113_NOT_ENOUGH_APS_TO_STEAL_ALL,
	MSG113_DO_WE_WANT_SURGERY_FIRST,
	MSG113_DO_WE_WANT_SURGERY,
	MSG113_SURGERY_BEFORE_DOCTOR_ASSIGNMENT,
	MSG113_SURGERY_BEFORE_PATIENT_ASSIGNMENT,
	MSG113_SURGERY_ON_TACTICAL_AUTOBANDAGE,
	MSG113_DO_WE_WANT_SURGERY_FIRST_BLOODBAG,
	MSG113_DO_WE_WANT_SURGERY_BLOODBAG,
	MSG113_SURGERY_BEFORE_DOCTOR_ASSIGNMENT_BLOODBAG,
	MSG113_SURGERY_FINISHED,
	MSG113_LOSES_ONE_POINT_MAX_HEALTH,
	MSG113_LOSES_X_POINTS_MAX_HEALTH,
	MSG113_BLINDED_BY_BLAST,
	MSG113_REGAINED_ONE_POINTS_OF_STAT,
	MSG113_REGAINED_X_POINTS_OF_STATS,
	MSG113_ENEMY_AMBUSH_PREVENTED,
	MSG113_BLOODCATS_AMBUSH_PREVENTED,
	MSG113_SOLDIER_HIT_TO_GROIN,
	MSG113_ENEMY_FOUND_DEAD_BODY,
	MSG113_AMMO_SPEC_STRING,
	MSG113_INVENTORY_APS_INSUFFICIENT,
	MSG113_HINT_TEXT,
	MSG113_SURRENDER_VALUES,

	MSG113_CANNOT_USE_SKILL,
	MSG113_CANNOT_BUILD,
	MSG113_CANNOT_SPOT_LOCATION,
	MSG113_INCORRECT_GRIDNO_ARTILLERY,
	MSG113_RADIO_JAMMED_NO_COMMUNICATION,
	MSG113_RADIO_ACTION_FAILED,
	MSG113_NOT_ENOUGH_MORTAR_SHELLS,
	MSG113_NO_SIGNAL_SHELL,
	MSG113_NO_MORTARS,
	MSG113_ALREADY_JAMMING,
	MSG113_ALREADY_LISTENING,
	MSG113_ALREADY_SPOTTING,
	MSG113_ALREADY_SCANNING,
	MSG113_COULD_NOT_APPLY,
	MSG113_ORDERS_REINFORCEMENTS,
	MSG113_RADIO_NO_ENERGY,
	MSG113_WORKING_RADIO_SET,
	MSG113_BINOCULAR,
	MSG113_PATIENCE,
	MSG113_SHIELD_DESTROYED,
	MSG113_FIREMODE_GL_DELAYED,
	MSG113_BLOODBAGOPTIONS_YESSTAR,
	MSG113_BLOODBAGOPTIONS_YES,
	MSG113_BLOODBAGOPTIONS_NO,
	MSG113_X_APPLY_Y_TO_Z,

	TEXT_NUM_MSG113,
};

extern STR16 gzTransformationMessage[];

//CHRISL: NewInv messages
extern STR16 NewInvMessage[];

// WANNE - MP: New multiplayer messages
extern STR16 MPServerMessage[];
extern STR16 MPClientMessage[];

// WANNE: Some Chinese specific strings that needs to be in unicode!
extern STR16 ChineseSpecString1;
extern STR16 ChineseSpecString2;
extern STR16 ChineseSpecString3;
extern STR16 ChineseSpecString4;
extern STR16 ChineseSpecString5;
extern STR16 ChineseSpecString6;
extern STR16 ChineseSpecString7;
extern STR16 ChineseSpecString8;
extern STR16 ChineseSpecString9;
extern STR16 ChineseSpecString10;
extern STR16 ChineseSpecString11;
extern STR16 ChineseSpecString12;

enum
{
	NIV_CAN_NOT_PICKUP,
	NIV_NO_DROP,
	NIV_NO_PACK,
	NIV_ZIPPER_COMBAT,
	NIV_ZIPPER_NO_MOVE,
	NIV_SELL_ALL,
	NIV_DELETE_ALL,
	NIV_NO_CLIMB,
	NIV_ALL_DROP,
	NIV_ALL_PICKUP,
	NIV_SOLDIER_DROP,
	NIV_SOLDIER_PICKUP,
	TEXT_NUM_NIV,
};

// OJW - MP
extern STR16 gszMPMapscreenText[];

//SB
enum
{
	ADDTEXT_16BPP_REQUIRED,
	ADDTEXT_DIFFRES_REQUIRED,
	ADDTEXT_WRONG_TEAM_SIZE,
	ERROR_MAX_MERCSVEHICLES,
	ERROR_MAX_ENEMIES,
	ERROR_MAX_CREATURES,
	ERROR_MAX_MILITIA,
	ERROR_MAX_CIVILIANS,
};
extern STR16 Additional113Text[];
extern STR16 ranks[];
//extern STR16 ranks[];

enum
{
	POCKET_POPUP_GRENADE_LAUNCHERS,
	POCKET_POPUP_ROCKET_LAUNCHERS,
	POCKET_POPUP_MEELE_AND_THROWN,
	POCKET_POPUP_NO_AMMO,
	POCKET_POPUP_NO_GUNS,
	POCKET_POPUP_MOAR
};
extern STR16	gszPocketPopupText[];

// rftr: better LBE tooltips
extern STR16		gLbeStatsDesc[14];

// Flugente: backgrounds
extern STR16		szBackgroundText_Flags[];
extern STR16		szBackgroundText_Value[];
extern STR16		szSoldierClassName[];

// Flugente: personality
enum
{
	PERSONALITYTEXT_YOULOOK,
	PERSONALITYTEXT_ANDAPPEARANCEIS,
	PERSONALITYTEXT_IMPORTANTTOYOU,
	PERSONALITYTEXT_YOUHAVE,
	PERSONALITYTEXT_ANDCARE,
	PERSONALITYTEXT_ABOUTTHAT,
	PERSONALITYTEXT_YOUARE,
	PERSONALITYTEXT_ADHATEEVERYONE,
	PERSONALITYTEXT_DOT,
	PERSONALITYTEXT_RACISTAGAINSTNON,
	PERSONALITYTEXT_PEOPLE,
	PERSONALITYTEXT_MAX,
};

extern STR16		szBackgroundTitleText[];
extern STR16 		szPersonalityTitleText[];
extern STR16		szPersonalityDisplayText[];
extern STR16		szPersonalityHelpText[];
extern STR16		szRaceText[];
extern STR16		szAppearanceText[];
extern STR16		szRefinementText[];
extern STR16		szRefinementTextTypes[];
extern STR16		szNationalityText[];
extern STR16		szNationalityTextAdjective[];
extern STR16		szNationalityText_Special[];
extern STR16		szCareLevelText[];
extern STR16		szRacistText[];
extern STR16		szSexistText[];

enum
{
	TEXT_SKILL_DENIAL_REQ,
	TEXT_SKILL_DENIAL_X_AP,
	TEXT_SKILL_DENIAL_X_TXT,
	TEXT_SKILL_DENIAL_X_TXT_ORHIGHER,
	TEXT_SKILL_DENIAL_X_TXT_ORHIGHER_OR,
	TEXT_SKILL_DENIAL_X_MINUTES,
	TEXT_SKILL_DENIAL_NOMORTAR,
	TEXT_SKILL_DENIAL_ITSCOMPLICATED,
	TEXT_SKILL_DENIAL_NODEMON,
	TEXT_SKILL_DENIAL_GUNTRAIT,
	TEXT_SKILL_DENIAL_AIMEDGUN,
	TEXT_SKILL_DENIAL_PRONEPERSONORCORPSE,
	TEXT_SKILL_DENIAL_CROUCH,
	TEXT_SKILL_DENIAL_FREEHANDS,
	TEXT_SKILL_DENIAL_COVERTTRAIT,
	TEXT_SKILL_DENIAL_ENEMYSECTOR,
	TEXT_SKILL_DENIAL_SINGLEMERC,
	TEXT_SKILL_DENIAL_NOALARM,
	TEXT_SKILL_DENIAL_DISGUISE_CIV_OR_MIL,
	TEXT_SKILL_DENIAL_NOT_DURING_INTERRUPT,
	TEXT_SKILL_DENIAL_TURNED_ENEMY,
	TEXT_SKILL_DENIAL_ENEMY,
	TEXT_SKILL_DENIAL_SURFACELEVEL,
	TEXT_SKILL_DENIAL_STRATEGIC_SUSPICION,

	TEXT_SKILL_DENIAL_MAX,
};

// Flugente: campaign history website
enum
{
	TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION,
	TEXT_CAMPAIGNHISTORY_NAME_MINISTRY,
	TEXT_CAMPAIGNHISTORY_NAME_REBEL,
	TEXT_CAMPAIGNHISTORY_NAME_NEUTRAL_1,
	TEXT_CAMPAIGNHISTORY_NAME_NEUTRAL_2,
	TEXT_CAMPAIGNHISTORY_NAME_RIS,

	TEXT_CAMPAIGNHISTORY_NAME_PRESSORGANISATION_SUBTITLE,
	TEXT_CAMPAIGNHISTORY_DESCRIPTION_1,
	//TEXT_CAMPAIGNHISTORY_DESCRIPTION_2,
	//TEXT_CAMPAIGNHISTORY_DESCRIPTION_3,

	TEXT_CAMPAIGNHISTORY_LINK_CONFLICTSUMMARY,
	TEXT_CAMPAIGNHISTORY_LINK_NEWS_MOSTIMPORTANT,
	TEXT_CAMPAIGNHISTORY_LINK_NEWS_RECENT,
	TEXT_CAMPAIGNHISTORY_LINK_HOME,

	TEXT_CAMPAIGNHISTORY_MAX,
};

extern STR16		szCampaignHistoryWebSite[];

enum
{
	TEXT_CAMPAIGNHISTORY_DETAIL_SETTING,
	TEXT_CAMPAIGNHISTORY_DETAIL_REBELFORCES,
	TEXT_CAMPAIGNHISTORY_DETAIL_ARMY,

	TEXT_CAMPAIGNHISTORY_DETAIL_ATTACKED,
	TEXT_CAMPAIGNHISTORY_DETAIL_AMBUSHED,
	TEXT_CAMPAIGNHISTORY_DETAIL_AIRDROPPED,

	TEXT_CAMPAIGNHISTORY_DETAIL_ATTACKERDIR,
	TEXT_CAMPAIGNHISTORY_DETAIL_DEFENDERDIR,
	TEXT_CAMPAIGNHISTORY_DETAIL_ATTACKERANDDEFENDERDIR,
	TEXT_CAMPAIGNHISTORY_DETAIL_NORTH,
	TEXT_CAMPAIGNHISTORY_DETAIL_EAST,
	TEXT_CAMPAIGNHISTORY_DETAIL_SOUTH,
	TEXT_CAMPAIGNHISTORY_DETAIL_WEST,
	TEXT_CAMPAIGNHISTORY_DETAIL_AND,	// " and " text
	TEXT_CAMPAIGNHISTORY_DETAIL_UNKNOWNLOCATION,

	TEXT_CAMPAIGNHISTORY_DETAIL_BUILDINGDAMAGE,
	TEXT_CAMPAIGNHISTORY_DETAIL_BUILDINGANDCIVDAMAGE,
	TEXT_CAMPAIGNHISTORY_DETAIL_REINFORCE_BOTH,
	TEXT_CAMPAIGNHISTORY_DETAIL_REINFORCE,
	TEXT_CAMPAIGNHISTORY_DETAIL_CHEMICAL_BOTH,
	TEXT_CAMPAIGNHISTORY_DETAIL_CHEMICAL,
	TEXT_CAMPAIGNHISTORY_DETAIL_TANKS_BOTH,
	TEXT_CAMPAIGNHISTORY_DETAIL_TANKS,
	TEXT_CAMPAIGNHISTORY_DETAIL_SNIPERS_BOTH,
	TEXT_CAMPAIGNHISTORY_DETAIL_SNIPERS,
	TEXT_CAMPAIGNHISTORY_DETAIL_SAMSITESABOTAGED,
	TEXT_CAMPAIGNHISTORY_DETAIL_SPY_ENEMY,
	TEXT_CAMPAIGNHISTORY_DETAIL_SPY_PLAYER,

	TEXT_CAMPAIGNHISTORY_DETAIL_MAX,
};

extern STR16		szCampaignHistoryDetail[];

enum
{
	TEXT_CAMPAIGNHISTORY_TIME_DEEPNIGHT,
	TEXT_CAMPAIGNHISTORY_TIME_DAWN,
	TEXT_CAMPAIGNHISTORY_TIME_EARLYMORNING,
	TEXT_CAMPAIGNHISTORY_TIME_MORNING,
	TEXT_CAMPAIGNHISTORY_TIME_NOON,
	TEXT_CAMPAIGNHISTORY_TIME_AFTERNOON,
	TEXT_CAMPAIGNHISTORY_TIME_EVENING,
	TEXT_CAMPAIGNHISTORY_TIME_NIGHT,
};

extern STR16		szCampaignHistoryTimeString[];

extern STR16		szCampaignHistoryMoneyTypeString[];
extern STR16		szCampaignHistoryConsumptionTypeString[];

enum
{
	TEXT_CAMPAIGNHISTORY_RESULT_ONESIDED_REBEL,
	TEXT_CAMPAIGNHISTORY_RESULT_EASY_REBEL,
	TEXT_CAMPAIGNHISTORY_RESULT_EASY_REBEL_PRISONER,
	TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_REBEL,
	TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_REBEL_PRISONER,
	TEXT_CAMPAIGNHISTORY_RESULT_HARD_REBEL,

	TEXT_CAMPAIGNHISTORY_RESULT_ONESIDED_ARMY_NUMBERS,
	TEXT_CAMPAIGNHISTORY_RESULT_ONESIDED_ARMY_TRAINING,
	TEXT_CAMPAIGNHISTORY_RESULT_EASY_ARMY_NUMBERS,
	TEXT_CAMPAIGNHISTORY_RESULT_EASY_ARMY_TRAINING,
	TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_ARMY_NUMBERS,
	TEXT_CAMPAIGNHISTORY_RESULT_MEDIUM_ARMY_TRAINING,
	TEXT_CAMPAIGNHISTORY_RESULT_HARD_ARMY,
};

extern STR16		szCampaignHistoryResultString[];

enum
{
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_IRRELEVANT,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_INSIGNIFICANT,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_NOTABLE,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_NOTEWORTHY,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_SIGNIFICANT,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_INTERESTING,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_IMPORTANT,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_VERYIMPORTANT,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_GRAVE,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_MAJOR,
	TEXT_CAMPAIGNHISTORY_IMPORTANCE_MOMENTOUS,
};

extern STR16		szCampaignHistoryImportanceString[];

enum
{
	WEBPAGE_CAMPAIGNHISTORY_KILLED,
	WEBPAGE_CAMPAIGNHISTORY_WOUNDED,
	WEBPAGE_CAMPAIGNHISTORY_PRISONERS,
	WEBPAGE_CAMPAIGNHISTORY_SHOTSFIRED,

	WEBPAGE_CAMPAIGNHISTORY_MONEYEARNED,
	WEBPAGE_CAMPAIGNHISTORY_CONSUMPTION,
	WEBPAGE_CAMPAIGNHISTORY_LOSSES,
	WEBPAGE_CAMPAIGNHISTORY_PARTICIPANTS,

	WEBPAGE_CAMPAIGNHISTORY_PROMOTIONS,
	WEBPAGE_CAMPAIGNHISTORY_SUMMARY,
	WEBPAGE_CAMPAIGNHISTORY_DETAIL,
	WEBPAGE_CAMPAIGNHISTORY_PREVIOUS,

	WEBPAGE_CAMPAIGNHISTORY_NEXT,
	WEBPAGE_CAMPAIGNHISTORY_INCIDENT,
	WEBPAGE_CAMPAIGNHISTORY_DAY,
};

extern STR16		szCampaignHistoryWebpageString[];

// Flugente: wacky operation names for campaign stats
#define		CAMPAIGNSTATS_OPERATION_NUM_PREFIX		140
#define		CAMPAIGNSTATS_OPERATION_NUM_SUFFIX		140

extern STR16		szCampaignStatsOperationPrefix[];
extern STR16		szCampaignStatsOperationSuffix[];


// Flugente: merc compare website
#define TEXT_MERCCOMPARE_CUSTOMERSTATEMENTS		7	// number of customer quotes, each with an additional short character name

enum
{
	// main page
	TEXT_MERCCOMPARE_WEBSITENAME,
	TEXT_MERCCOMPARE_SLOGAN,

	// links to other pages
	TEXT_MERCCOMPARE_SUBSITE1,

	TEXT_MERCCOMPARE_INTRO1 = TEXT_MERCCOMPARE_SUBSITE1 + 5,
	TEXT_MERCCOMPARE_BULLET1,
	TEXT_MERCCOMPARE_BULLET2,
	TEXT_MERCCOMPARE_BULLET3,
	TEXT_MERCCOMPARE_BULLET4,
	TEXT_MERCCOMPARE_INTRO2,

	// customer quotes
	TEXT_MERCCOMPARE_QUOTEINTRO,
	TEXT_MERCCOMPARE_QUOTE1,
	TEXT_MERCCOMPARE_QUOTE1NAME,

	// analyze
	TEXT_MERCCOMPARE_DROPDOWNTEXT = TEXT_MERCCOMPARE_QUOTE1 + 2 * TEXT_MERCCOMPARE_CUSTOMERSTATEMENTS,
	TEXT_MERCCOMPARE_DROPDOWNTEXT_MATRIX,

	// error messages
	TEXT_MERCCOMPARE_ERROR_NOBODYTHERE,

	TEXT_MERCCOMPARE_MAX,
};

extern STR16		szMercCompareWebSite[];
extern STR16		szMercCompareEventText[];

// Flugente: WHO website
enum
{
	// main page
	TEXT_WHO_WEBSITENAME,
	TEXT_WHO_SLOGAN,

	// links to other pages
	TEXT_WHO_SUBSITE1,

	TEXT_WHO_MAIN1 = TEXT_WHO_SUBSITE1 + 3,

	TEXT_WHO_CONTRACT1 = TEXT_WHO_MAIN1 + 3,
	TEXT_WHO_CONTRACT_ACQUIRED_NOT = TEXT_WHO_CONTRACT1 + 4,
	TEXT_WHO_CONTRACT_ACQUIRED,
	TEXT_WHO_CONTRACT_BUTTON_SUBSCRIBE,
	TEXT_WHO_CONTRACT_BUTTON_UNSUBSCRIBE,

	TEXT_WHO_TIPS1,

	TEXT_WHO_MAX = TEXT_WHO_TIPS1 + 8,
};

extern STR16		szWHOWebSite[];

// Flugente: PMC website
enum
{
	// main page
	TEXT_PMC_WEBSITENAME,
	TEXT_PMC_SLOGAN,

	// links to other pages
	TEXT_PMC_SUBSITE1,

	TEXT_PMC_MAIN1 = TEXT_PMC_SUBSITE1 + 3,

	// team contracts
	TEXT_PMC_CONTRACT_TEAM_INTRO = TEXT_PMC_MAIN1 + 7,
	TEXT_PMC_CONTRACT_DROPDOWNTEXT,
	TEXT_PMC_REGULAR,
	TEXT_PMC_VETERAN,

	TEXT_PMC_DETAIL,

	TEXT_PMC_SELECTAREA = TEXT_PMC_DETAIL + 3,
	TEXT_PMC_TOTALCOST,
	TEXT_PMC_ETA,
	TEXT_PMC_CONTRACTBUTTON,

	TEXT_PMC_CONFIRMATION,
	TEXT_PMC_ARRIVAL,
	TEXT_PMC_NEXTDEPLOYMENT,
	TEXT_PMC_NODROPOFF,

	TEXT_PMC_MAX,
};

extern STR16		szPMCWebSite[];

extern STR16   szTacticalInventoryDialogString[];
extern STR16   szTacticalCoverDialogString[];
extern STR16   szTacticalCoverDialogPrintString[];

// OPINIONEVENT_MAX is 39
// DOST_MAX is 17

extern STR16	szDynamicDialogueText[40][17];

// Flugente: dynamic dialogue
extern STR16	szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_DENY[];
extern STR16	szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_AGREE[];
extern STR16	szDynamicDialogueText_DOST_SIDEWITH_VICTIM[];
extern STR16	szDynamicDialogueText_DOST_SIDEWITH_CAUSE[];

extern STR16	szDynamicDialogueText_DOST_INTERJECTOR_DIALOGUESELECTION_SHORTTEXT[];
extern STR16	szDynamicDialogueText_GenderText[];

// Flugente: disease
enum
{
	// effect description
	TEXT_DISEASE_EFFSTAT_AGI,
	TEXT_DISEASE_EFFSTAT_DEX,
	TEXT_DISEASE_EFFSTAT_STR,
	TEXT_DISEASE_EFFSTAT_WIS,
	TEXT_DISEASE_EFFSTAT_EXP,

	TEXT_DISEASE_AP,
	TEXT_DISEASE_MAXBREATH,
	TEXT_DISEASE_CARRYSTRENGTH,
	TEXT_DISEASE_LIFEREGENHUNDREDS,
	TEXT_DISEASE_NEEDTOSLEEP,
	TEXT_DISEASE_DRINK,
	TEXT_DISEASE_FOOD,

	// text when diagnosed
	TEXT_DISEASE_DIAGNOSE_GENERAL,
	TEXT_DISEASE_CURED,

	// menu entries
	TEXT_DISEASE_DIAGNOSIS,
	TEXT_DISEASE_TREATMENT,
	TEXT_DISEASE_BURIAL,
	TEXT_DISEASE_CANCEL,

	// (undiagnosed)
	TEXT_DISEASE_UNDIAGNOSED,

	TEXT_DISEASE_PTSD_BUNS_SPECIAL,
	TEXT_DISEASE_CONTAMINATION_FOUND,
	TEXT_DISEASE_ADD_DISABILITY,

	TEXT_DISEASE_LIMITED_ARMS,
	TEXT_DISEASE_LIMITED_ARMS_SPLINT,
	TEXT_DISEASE_LIMITED_LEGS,
	TEXT_DISEASE_LIMITED_LEGS_SPLINT,
};

enum
{
	TEXT_SPY_CONCEAL,
	TEXT_SPY_GETINTEL,
};

extern STR16	szDiseaseText[];
extern STR16	szSpyText[];
extern STR16	szFoodText[];

extern STR16	szIMPGearWebSiteText[];
extern STR16	szIMPGearDropDownText[];
extern STR16	szIMPGearDropDownNoneText[];

// Flugente: militia movement
extern STR16	szMilitiaStrategicMovementText[];

// Flugente: enemy heli/SAM
extern STR16	szEnemyHeliText[];

// Flugente: fortification
extern STR16	szFortificationText[];

// Flugente: militia website
enum
{
	// main page
	TEXT_MILITIAWEBSITE_WEBSITENAME,
	TEXT_MILITIAWEBSITE_SLOGAN,

	// links to other pages
	TEXT_MILITIAWEBSITE_SUBSITE1,

	TEXT_MILITIAWEBSITE_MAIN1 = TEXT_MILITIAWEBSITE_SUBSITE1 + 3,

	TEXT_MILITIAWEBSITE_MAX,
};

extern STR16	szMilitiaWebSite[];
extern STR16	szIndividualMilitiaBattleReportText[];
extern STR16	szIndividualMilitiaTraitRequirements[];
extern STR16	szIdividualMilitiaWebsiteText[];
extern STR16	szIdividualMilitiaWebsiteFilterText_Dead[];
extern STR16	szIdividualMilitiaWebsiteFilterText_Rank[] ;
extern STR16	szIdividualMilitiaWebsiteFilterText_Origin[];
extern STR16	szIdividualMilitiaWebsiteFilterText_Sector[];

// Flugente: non-profile merchants
extern STR16	szNonProfileMerchantText[];

// Flugente: externalised weather
extern STR16	szWeatherTypeText[];

// Flugente: snakes
extern STR16	szSnakeText[];

// Flugente: militia resources
extern STR16	szSMilitiaResourceText[];

// Flugente: interactive actions
extern STR16	szInteractiveActionText[];

extern STR16	szLaptopStatText[];

// Flugente: gear templates
extern STR16	szGearTemplateText[];

// Flugente: intel
enum
{
	// defaults
	TEXT_INTEL_TITLE,
	TEXT_INTEL_SUBTITLE,
	TEXT_INTEL_LINK_1,
	TEXT_INTEL_LINK_2,

	TEXT_INTEL_LINK_3,

	// buy info
	TEXT_INTEL_BUDGET,
	TEXT_INTEL_TEXT_1,
	TEXT_INTEL_TEXT_2,
	TEXT_INTEL_OFFER_1,
	TEXT_INTEL_AIRREGION,
	TEXT_INTEL_AIRREGION_BUY1,
	TEXT_INTEL_AIRREGION_BUY2,

	TEXT_INTEL_DROPDOWN_HELPTEXT,
	TEXT_INTEL_MAPREGION_1,

	// about us
	TEXT_INTEL_ABOUTUS_1 = TEXT_INTEL_MAPREGION_1 + 16,

	TEXT_INTEL_ABOUTUS_MAX = TEXT_INTEL_ABOUTUS_1 + 6,

	// sell info
	TEXT_INTEL_SELL_1 = TEXT_INTEL_ABOUTUS_MAX,
	TEXT_INTEL_SELL_BUTTON_1,
	TEXT_INTEL_SELL_BUTTON_2,
	TEXT_INTEL_SELL_BUTTON_3,

	TEXT_INTEL_SELL_ALREADYGOT_1,
	TEXT_INTEL_SELL_NOTHING_1,
};

extern STR16	szIntelWebsiteText[];

extern STR16	szIntelText[];

extern STR16	szChatTextSpy[];
extern STR16	szChatTextEnemy[];

extern STR16	szMilitiaText[];

extern STR16	szFactoryText[];

extern STR16	szTurncoatText[];

extern STR16	szRebelCommandText[];
extern STR16	szRebelCommandHelpText[];
extern STR16	szRebelCommandAdminActionsText[];
extern STR16	szRebelCommandDirectivesText[];

#define TACTICAL_INVENTORY_DIALOG_NUM    16
#define TACTICAL_COVER_DIALOG_NUM        16

// Enumeration support
typedef struct Str8EnumLookupType {
	int value;
	const STR8 name;
} Str8EnumLookupType;

typedef struct Str16EnumLookupType {
	int value;
	const STR16 name;
} Str16EnumLookupType;

const STR8 EnumToString(int value, const Str8EnumLookupType *table);
const STR16 EnumToString(int value, const Str16EnumLookupType *table);
int StringToEnum(const STR8 value, const Str8EnumLookupType *table);
int StringToEnum(const STR8 value, const Str16EnumLookupType *table);
int StringToEnum(const STR16 value, const Str16EnumLookupType *table);

void ParseCommandLine(const char *start,char **argv,char *args,int *numargs,int *numchars);
void ParseCommandLine(const wchar_t *start,wchar_t **argv,wchar_t *args,int *numargs,int *numchars);

#endif


//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
//these are dummy functions. Not all of these may be necessary. They are included for future possible useage
void this_is_the_ChineseText_public_symbol(void);
void this_is_the_DutchText_public_symbol(void);
void this_is_the_EnglishText_public_symbol(void);
void this_is_the_FrenchText_public_symbol(void);
void this_is_the_GermanText_public_symbol(void);
void this_is_the_ItalianText_public_symbol(void);
void this_is_the_PolishText_public_symbol(void);
void this_is_the_RussianText_public_symbol(void);

void this_is_the_Ja25ChineseText_public_symbol(void);
void this_is_the_Ja25DutchText_public_symbol(void);
void this_is_the_Ja25EnglishText_public_symbol(void);
void this_is_the_Ja25FrenchText_public_symbol(void);
void this_is_the_Ja25GermanText_public_symbol(void);
void this_is_the_Ja25ItalianText_public_symbol(void);
void this_is_the_Ja25PolishText_public_symbol(void);
void this_is_the_Ja25RussianText_public_symbol(void);
