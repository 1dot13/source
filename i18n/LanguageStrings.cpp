// Scaffolding for the language axis migration (docs/plans/language-design.md, chunk
// C2): pulls all eight base + eight Ja2.5-carryover text files into this one
// translation unit, each inside its own per-language namespace, to prove they compile
// together. The global table pointers and BindLanguageStrings body are filled in by
// later chunks (Recipe R1); for now BindLanguageStrings is an empty stub.

#include "Text.h"
#include "FileMan.h"
#include "Scheduling.h"
#include "EditorMercs.h"
#include "Item Statistics.h"

#include <language.hpp>

// Recipe R1 (docs/plans/language-design.md): the loose _<LANG>Text.cpp files are still
// compiled standalone until Cn+2, and #include the headers declaring the now-pointer
// externs themselves; this guard keeps their array definitions out of those standalone
// TUs (which would collide with the pointer extern) while still compiling them here.
#define JA2_UNIFIED_TEXT 1

// The compile-time default language for this exe, captured before the namespace blocks
// below because each block #undefs its language macro on exit.
#if defined(GERMAN)
#  define LANG_DEFAULT_NS lang_de
#elif defined(RUSSIAN)
#  define LANG_DEFAULT_NS lang_ru
#elif defined(DUTCH)
#  define LANG_DEFAULT_NS lang_nl
#elif defined(POLISH)
#  define LANG_DEFAULT_NS lang_pl
#elif defined(FRENCH)
#  define LANG_DEFAULT_NS lang_fr
#elif defined(ITALIAN)
#  define LANG_DEFAULT_NS lang_it
#elif defined(CHINESE)
#  define LANG_DEFAULT_NS lang_zh
#else
#  define LANG_DEFAULT_NS lang_en
#endif

namespace lang_en {
#undef ENGLISH
#define ENGLISH
#include "_EnglishText.cpp"
#include "_Ja25EnglishText.cpp"
#undef ENGLISH
}

namespace lang_de {
#undef GERMAN
#define GERMAN
#include "_GermanText.cpp"
#include "_Ja25GermanText.cpp"
#undef GERMAN
}

namespace lang_ru {
#undef RUSSIAN
#define RUSSIAN
#include "_RussianText.cpp"
#include "_Ja25RussianText.cpp"
#undef RUSSIAN
}

namespace lang_nl {
#undef DUTCH
#define DUTCH
#include "_DutchText.cpp"
#include "_Ja25DutchText.cpp"
#undef DUTCH
}

namespace lang_pl {
#undef POLISH
#define POLISH
#include "_PolishText.cpp"
#include "_Ja25PolishText.cpp"
#undef POLISH
}

namespace lang_fr {
#undef FRENCH
#define FRENCH
#include "_FrenchText.cpp"
#include "_Ja25FrenchText.cpp"
#undef FRENCH
}

namespace lang_it {
#undef ITALIAN
#define ITALIAN
#include "_ItalianText.cpp"
#include "_Ja25ItalianText.cpp"
#undef ITALIAN
}

namespace lang_zh {
#undef CHINESE
#define CHINESE
#include "_ChineseText.cpp"
#include "_Ja25ChineseText.cpp"
#undef CHINESE
}

namespace lang_default = LANG_DEFAULT_NS;

// Global table pointers, statically bound to lang_default so every exe stays functional
// before BindLanguageStrings is wired in at Cn+2. Filled in one subsystem group per R1
// chunk.
STR16* pMenuStrings = lang_default::pMenuStrings;
STR16* pLocationPageText = lang_default::pLocationPageText;
STR16* pSectorPageText = lang_default::pSectorPageText;
STR16* pEncyclopediaHelpText = lang_default::pEncyclopediaHelpText;
STR16* pEncyclopediaTypeText = lang_default::pEncyclopediaTypeText;
STR16* pEncyclopediaSkrotyText = lang_default::pEncyclopediaSkrotyText;
STR16* pEncyclopediaFilterLocationText = lang_default::pEncyclopediaFilterLocationText;
STR16* pEncyclopediaSubFilterLocationText = lang_default::pEncyclopediaSubFilterLocationText;
STR16* pEncyclopediaFilterCharText = lang_default::pEncyclopediaFilterCharText;
STR16* pEncyclopediaSubFilterCharText = lang_default::pEncyclopediaSubFilterCharText;
STR16* pEncyclopediaFilterItemText = lang_default::pEncyclopediaFilterItemText;
STR16* pEncyclopediaSubFilterItemText = lang_default::pEncyclopediaSubFilterItemText;
STR16* pEncyclopediaFilterQuestText = lang_default::pEncyclopediaFilterQuestText;
STR16* pEncyclopediaSubFilterQuestText = lang_default::pEncyclopediaSubFilterQuestText;
STR16* pEncyclopediaShortCharacterText = lang_default::pEncyclopediaShortCharacterText;
STR16* pEncyclopediaHelpCharacterText = lang_default::pEncyclopediaHelpCharacterText;
STR16* pEncyclopediaShortInventoryText = lang_default::pEncyclopediaShortInventoryText;
STR16* BoxFilter = lang_default::BoxFilter;
STR16* pOtherButtonsText = lang_default::pOtherButtonsText;
STR16* pOtherButtonsHelpText = lang_default::pOtherButtonsHelpText;
STR16* QuestDescText = lang_default::QuestDescText;
STR16* FactDescText = lang_default::FactDescText;

STR16* iEditorItemStatsButtonsText = lang_default::iEditorItemStatsButtonsText;
STR16* FaceDirs = lang_default::FaceDirs;
STR16* iEditorMercsToolbarText = lang_default::iEditorMercsToolbarText;
STR16* iEditorBuildingsToolbarText = lang_default::iEditorBuildingsToolbarText;
STR16* iEditorItemsToolbarText = lang_default::iEditorItemsToolbarText;
STR16* iEditorMapInfoToolbarText = lang_default::iEditorMapInfoToolbarText;
STR16* iEditorOptionsToolbarText = lang_default::iEditorOptionsToolbarText;
STR16* iEditorTerrainToolbarText = lang_default::iEditorTerrainToolbarText;
STR16* iEditorTaskbarInternalText = lang_default::iEditorTaskbarInternalText;
STR16* iRenderMapEntryPointsAndLightsText = lang_default::iRenderMapEntryPointsAndLightsText;
STR16* iBuildTriggerNameText = lang_default::iBuildTriggerNameText;
STR16* iRenderDoorLockInfoText = lang_default::iRenderDoorLockInfoText;
STR16* iRenderEditorInfoText = lang_default::iRenderEditorInfoText;
STR16* iUpdateBuildingsInfoText = lang_default::iUpdateBuildingsInfoText;
STR16* iRenderDoorEditingWindowText = lang_default::iRenderDoorEditingWindowText;
STR16* pInitEditorItemsInfoText = lang_default::pInitEditorItemsInfoText;
STR16* pDisplayItemStatisticsTex = lang_default::pDisplayItemStatisticsTex;
STR16* pUpdateMapInfoText = lang_default::pUpdateMapInfoText;
CHAR16 (*gszScheduleActions)[20] = lang_default::gszScheduleActions;
STR16* zDiffNames = lang_default::zDiffNames;
STR16* EditMercStat = lang_default::EditMercStat;
STR16* EditMercOrders = lang_default::EditMercOrders;
STR16* EditMercAttitudes = lang_default::EditMercAttitudes;
STR16* pDisplayEditMercWindowText = lang_default::pDisplayEditMercWindowText;
STR16* pCreateEditMercWindowText = lang_default::pCreateEditMercWindowText;
STR16* pDisplayBodyTypeInfoText = lang_default::pDisplayBodyTypeInfoText;
STR16* pUpdateMercsInfoText = lang_default::pUpdateMercsInfoText;
CHAR16 (*pRenderMercStringsText)[100] = lang_default::pRenderMercStringsText;
STR16* pClearCurrentScheduleText = lang_default::pClearCurrentScheduleText;
STR16* pCopyMercPlacementText = lang_default::pCopyMercPlacementText;
STR16* pPasteMercPlacementText = lang_default::pPasteMercPlacementText;
STR16* pEditModeShutdownText = lang_default::pEditModeShutdownText;
STR16* pHandleKeyboardShortcutsText = lang_default::pHandleKeyboardShortcutsText;
STR16* pPerformSelectedActionText = lang_default::pPerformSelectedActionText;
STR16* pWaitForHelpScreenResponseText = lang_default::pWaitForHelpScreenResponseText;
STR16* pAutoLoadMapText = lang_default::pAutoLoadMapText;
STR16* pShowHighGroundText = lang_default::pShowHighGroundText;
STR16* pUpdateItemStatsPanelText = lang_default::pUpdateItemStatsPanelText;
STR16* pSetupGameTypeFlagsText = lang_default::pSetupGameTypeFlagsText;
STR16* pSetupGunGUIText = lang_default::pSetupGunGUIText;
STR16* pSetupArmourGUIText = lang_default::pSetupArmourGUIText;
STR16* pSetupExplosivesGUIText = lang_default::pSetupExplosivesGUIText;
STR16* pSetupTriggersGUIText = lang_default::pSetupTriggersGUIText;
STR16* pCreateSummaryWindowText = lang_default::pCreateSummaryWindowText;
STR16* pRenderSectorInformationText = lang_default::pRenderSectorInformationText;
STR16* pRenderItemDetailsText = lang_default::pRenderItemDetailsText;
STR16* pRenderSummaryWindowText = lang_default::pRenderSummaryWindowText;
STR16* pUpdateSectorSummaryText = lang_default::pUpdateSectorSummaryText;
STR16* pSummaryLoadMapCallbackText = lang_default::pSummaryLoadMapCallbackText;
STR16* pReportErrorText = lang_default::pReportErrorText;
STR16* pRegenerateSummaryInfoForAllOutdatedMapsText = lang_default::pRegenerateSummaryInfoForAllOutdatedMapsText;
STR16* pSummaryUpdateCallbackText = lang_default::pSummaryUpdateCallbackText;
STR16* pApologizeOverrideAndForceUpdateEverythingText = lang_default::pApologizeOverrideAndForceUpdateEverythingText;
STR16* pDisplaySelectionWindowGraphicalInformationText = lang_default::pDisplaySelectionWindowGraphicalInformationText;
STR16* pDisplaySelectionWindowButtonText = lang_default::pDisplaySelectionWindowButtonText;
STR16* wszSelType = lang_default::wszSelType;
STR16* gzNewLaptopMessages = lang_default::gzNewLaptopMessages;
STR16* zNewTacticalMessages = lang_default::zNewTacticalMessages;
CHAR16 (*gszAimPages)[20] = lang_default::gszAimPages;
CHAR16 (*zGrod)[500] = lang_default::zGrod;
STR16* pCreditsJA2113 = lang_default::pCreditsJA2113;
CHAR16 (*ShortItemNames)[80] = lang_default::ShortItemNames;
CHAR16 (*ItemNames)[80] = lang_default::ItemNames;
CHAR16 (*AmmoCaliber)[20] = lang_default::AmmoCaliber;
CHAR16 (*BobbyRayAmmoCaliber)[20] = lang_default::BobbyRayAmmoCaliber;
CHAR16 (*WeaponType)[30] = lang_default::WeaponType;
CHAR16 (*Message)[STRING_LENGTH] = lang_default::Message;
CHAR16 (*TeamTurnString)[STRING_LENGTH] = lang_default::TeamTurnString;
STR16* pMilitiaControlMenuStrings = lang_default::pMilitiaControlMenuStrings;
STR16* pTraitSkillsMenuStrings = lang_default::pTraitSkillsMenuStrings;
STR16* pTraitSkillsMenuDescStrings = lang_default::pTraitSkillsMenuDescStrings;
STR16* pTraitSkillsDenialStrings = lang_default::pTraitSkillsDenialStrings;
STR16* pSkillMenuStrings = lang_default::pSkillMenuStrings;
STR16* pSnitchMenuStrings = lang_default::pSnitchMenuStrings;
STR16* pSnitchMenuDescStrings = lang_default::pSnitchMenuDescStrings;
STR16* pSnitchToggleMenuStrings = lang_default::pSnitchToggleMenuStrings;
STR16* pSnitchToggleMenuDescStrings = lang_default::pSnitchToggleMenuDescStrings;
STR16* pSnitchSectorMenuStrings = lang_default::pSnitchSectorMenuStrings;
STR16* pSnitchSectorMenuDescStrings = lang_default::pSnitchSectorMenuDescStrings;
STR16* pPrisonerMenuStrings = lang_default::pPrisonerMenuStrings;
STR16* pPrisonerMenuDescStrings = lang_default::pPrisonerMenuDescStrings;
STR16* pSnitchPrisonExposedStrings = lang_default::pSnitchPrisonExposedStrings;
STR16* pSnitchGatheringRumoursResultStrings = lang_default::pSnitchGatheringRumoursResultStrings;
STR16* pAssignMenuStrings = lang_default::pAssignMenuStrings;
STR16* pTrainingStrings = lang_default::pTrainingStrings;
STR16* pTrainingMenuStrings = lang_default::pTrainingMenuStrings;
STR16* pAttributeMenuStrings = lang_default::pAttributeMenuStrings;
STR16* pVehicleStrings = lang_default::pVehicleStrings;
STR16* pShortAttributeStrings = lang_default::pShortAttributeStrings;
STR16* pLongAttributeStrings = lang_default::pLongAttributeStrings;
STR16* pContractStrings = lang_default::pContractStrings;
STR16* pAssignmentStrings = lang_default::pAssignmentStrings;
STR16* pConditionStrings = lang_default::pConditionStrings;
CHAR16 (*pCountryNames)[MAX_TOWN_NAME_LENGHT] = lang_default::pCountryNames;
CHAR16 (*pTownNames)[MAX_TOWN_NAME_LENGHT] = lang_default::pTownNames;
STR16* pPersonnelScreenStrings = lang_default::pPersonnelScreenStrings;
STR16* pPersonnelRecordsHelpTexts = lang_default::pPersonnelRecordsHelpTexts;
STR16* pPersonnelTitle = lang_default::pPersonnelTitle;
STR16* pUpperLeftMapScreenStrings = lang_default::pUpperLeftMapScreenStrings;
STR16* pTacticalPopupButtonStrings = lang_default::pTacticalPopupButtonStrings;
STR16* pSquadMenuStrings = lang_default::pSquadMenuStrings;
STR16* pDoorTrapStrings = lang_default::pDoorTrapStrings;
STR16* pLongAssignmentStrings = lang_default::pLongAssignmentStrings;
STR16* pContractExtendStrings = lang_default::pContractExtendStrings;
STR16* pMapScreenMouseRegionHelpText = lang_default::pMapScreenMouseRegionHelpText;
STR16* pPersonnelAssignmentStrings = lang_default::pPersonnelAssignmentStrings;
STR16* pNoiseVolStr = lang_default::pNoiseVolStr;
STR16* pNoiseTypeStr = lang_default::pNoiseTypeStr;
STR16* pDirectionStr = lang_default::pDirectionStr;
STR16* pRemoveMercStrings = lang_default::pRemoveMercStrings;
STR16* sTimeStrings = lang_default::sTimeStrings;
STR16* pLandTypeStrings = lang_default::pLandTypeStrings;
STR16* pGuardMenuStrings = lang_default::pGuardMenuStrings;
STR16* pOtherGuardMenuStrings = lang_default::pOtherGuardMenuStrings;
STR16* pInvPanelTitleStrings = lang_default::pInvPanelTitleStrings;
STR16* pPOWStrings = lang_default::pPOWStrings;
STR16* pMilitiaString = lang_default::pMilitiaString;
STR16* pMilitiaButtonString = lang_default::pMilitiaButtonString;
STR16* pEpcMenuStrings = lang_default::pEpcMenuStrings;

STR16* pRepairStrings = lang_default::pRepairStrings;
STR16* sPreStatBuildString = lang_default::sPreStatBuildString;
STR16* sStatGainStrings = lang_default::sStatGainStrings;
STR16* pHelicopterEtaStrings = lang_default::pHelicopterEtaStrings;
STR16* pHelicopterRepairRefuelStrings = lang_default::pHelicopterRepairRefuelStrings;
STR16* sMapLevelString = lang_default::sMapLevelString;
STR16* gsLoyalString = lang_default::gsLoyalString;
STR16* gsUndergroundString = lang_default::gsUndergroundString;
STR16* gsTimeStrings = lang_default::gsTimeStrings;
STR16* sFacilitiesStrings = lang_default::sFacilitiesStrings;
STR16* pMapPopUpInventoryText = lang_default::pMapPopUpInventoryText;
STR16* pwTownInfoStrings = lang_default::pwTownInfoStrings;
STR16* pwMineStrings = lang_default::pwMineStrings;
STR16* pwMiscSectorStrings = lang_default::pwMiscSectorStrings;
STR16* pMapInventoryErrorString = lang_default::pMapInventoryErrorString;
STR16* pMapInventoryStrings = lang_default::pMapInventoryStrings;
STR16* pMapScreenFastHelpTextList = lang_default::pMapScreenFastHelpTextList;
STR16* pMovementMenuStrings = lang_default::pMovementMenuStrings;
STR16* pUpdateMercStrings = lang_default::pUpdateMercStrings;
STR16* pMapScreenBorderButtonHelpText = lang_default::pMapScreenBorderButtonHelpText;
STR16* pMapScreenInvenButtonHelpText = lang_default::pMapScreenInvenButtonHelpText;
STR16* pMapScreenBottomFastHelp = lang_default::pMapScreenBottomFastHelp;
STR16* pMapScreenBottomText = lang_default::pMapScreenBottomText;
STR16* pMercDeadString = lang_default::pMercDeadString;
CHAR16 (*pSenderNameList)[128] = lang_default::pSenderNameList;
STR16* pTraverseStrings = lang_default::pTraverseStrings;
STR16* pNewMailStrings = lang_default::pNewMailStrings;
STR16* pDeleteMailStrings = lang_default::pDeleteMailStrings;
STR16* pEmailHeaders = lang_default::pEmailHeaders;
STR16* pEmailTitleText = lang_default::pEmailTitleText;
STR16* pFinanceTitle = lang_default::pFinanceTitle;
STR16* pFinanceSummary = lang_default::pFinanceSummary;
STR16* pFinanceHeaders = lang_default::pFinanceHeaders;
STR16* pTransactionText = lang_default::pTransactionText;
STR16* pTransactionAlternateText = lang_default::pTransactionAlternateText;
STR16* pMoralStrings = lang_default::pMoralStrings;
STR16* pSkyriderText = lang_default::pSkyriderText;
STR16* pLeftEquipmentString = lang_default::pLeftEquipmentString;
STR16* pMapScreenStatusStrings = lang_default::pMapScreenStatusStrings;
STR16* pMapScreenPrevNextCharButtonHelpText = lang_default::pMapScreenPrevNextCharButtonHelpText;
STR16* pEtaString = lang_default::pEtaString;
STR16* pShortVehicleStrings = lang_default::pShortVehicleStrings;
STR16* pTrashItemText = lang_default::pTrashItemText;
STR16* pMapErrorString = lang_default::pMapErrorString;
STR16* pMapPlotStrings = lang_default::pMapPlotStrings;
STR16* pMiscMapScreenMouseRegionHelpText = lang_default::pMiscMapScreenMouseRegionHelpText;
STR16* pMercHeLeaveString = lang_default::pMercHeLeaveString;
STR16* pMercSheLeaveString = lang_default::pMercSheLeaveString;
STR16* pImpPopUpStrings = lang_default::pImpPopUpStrings;
STR16* pImpButtonText = lang_default::pImpButtonText;
STR16* pExtraIMPStrings = lang_default::pExtraIMPStrings;
STR16* pDayStrings = lang_default::pDayStrings;
STR16* pMercContractOverStrings = lang_default::pMercContractOverStrings;
STR16* pFilesTitle = lang_default::pFilesTitle;
STR16* pFilesSenderList = lang_default::pFilesSenderList;
STR16* pHistoryLocations = lang_default::pHistoryLocations;
STR16* pHistoryHeaders = lang_default::pHistoryHeaders;
STR16* pHistoryTitle = lang_default::pHistoryTitle;
STR16* pShowBookmarkString = lang_default::pShowBookmarkString;
STR16* pWebPagesTitles = lang_default::pWebPagesTitles;
STR16* pWebTitle = lang_default::pWebTitle;
STR16* pPersonnelString = lang_default::pPersonnelString;
STR16* pErrorStrings = lang_default::pErrorStrings;
STR16* pDownloadString = lang_default::pDownloadString;
STR16* pBookmarkTitle = lang_default::pBookmarkTitle;
STR16* pBookMarkStrings = lang_default::pBookMarkStrings;
STR16* pLaptopIcons = lang_default::pLaptopIcons;
STR16* gsAtmStartButtonText = lang_default::gsAtmStartButtonText;
STR16* pPersonnelTeamStatsStrings = lang_default::pPersonnelTeamStatsStrings;
STR16* pPersonnelCurrentTeamStatsStrings = lang_default::pPersonnelCurrentTeamStatsStrings;
STR16* pPersonelTeamStrings = lang_default::pPersonelTeamStrings;
STR16* pPersonnelDepartedStateStrings = lang_default::pPersonnelDepartedStateStrings;
STR16* pMapHortIndex = lang_default::pMapHortIndex;
STR16* pMapVertIndex = lang_default::pMapVertIndex;
STR16* pMapDepthIndex = lang_default::pMapDepthIndex;
STR16* pLaptopTitles = lang_default::pLaptopTitles;
STR16* pMilitiaConfirmStrings = lang_default::pMilitiaConfirmStrings;
STR16* pDeliveryLocationStrings = lang_default::pDeliveryLocationStrings;
STR16* pSkillAtZeroWarning = lang_default::pSkillAtZeroWarning;
STR16* pIMPBeginScreenStrings = lang_default::pIMPBeginScreenStrings;
STR16* pIMPFinishButtonText = lang_default::pIMPFinishButtonText;
STR16* pIMPFinishStrings = lang_default::pIMPFinishStrings;
STR16* pIMPVoicesStrings = lang_default::pIMPVoicesStrings;
STR16* pDepartedMercPortraitStrings = lang_default::pDepartedMercPortraitStrings;
STR16* pPersTitleText = lang_default::pPersTitleText;
STR16* pPausedGameText = lang_default::pPausedGameText;
STR16* zOptionsToggleText = lang_default::zOptionsToggleText;
STR16* zOptionsScreenHelpText = lang_default::zOptionsScreenHelpText;
STR16* pDoctorWarningString = lang_default::pDoctorWarningString;
STR16* pMilitiaButtonsHelpText = lang_default::pMilitiaButtonsHelpText;
STR16* pMapScreenJustStartedHelpText = lang_default::pMapScreenJustStartedHelpText;
STR16* pLandMarkInSectorString = lang_default::pLandMarkInSectorString;
STR16* gzMercSkillText = lang_default::gzMercSkillText;
STR16* gzMercSkillTextNew = lang_default::gzMercSkillTextNew;
STR16* gzNonPersistantPBIText = lang_default::gzNonPersistantPBIText;
STR16* gzMiscString = lang_default::gzMiscString;
STR16* pBullseyeStrings = lang_default::pBullseyeStrings;
STR16* wMapScreenSortButtonHelpText = lang_default::wMapScreenSortButtonHelpText;
STR16* pNewNoiseStr = lang_default::pNewNoiseStr;
STR16* pTauntUnknownVoice = lang_default::pTauntUnknownVoice;
STR16* gzLateLocalizedString = lang_default::gzLateLocalizedString;
STR16* gzCWStrings = lang_default::gzCWStrings;
STR16* gzTooltipStrings = lang_default::gzTooltipStrings;
STR16* pSkillTraitBeginIMPStrings = lang_default::pSkillTraitBeginIMPStrings;
STR16* sgAttributeSelectionText = lang_default::sgAttributeSelectionText;
STR16* pCharacterTraitBeginIMPStrings = lang_default::pCharacterTraitBeginIMPStrings;
STR16* gzIMPCharacterTraitText = lang_default::gzIMPCharacterTraitText;
STR16* gzIMPAttitudesText = lang_default::gzIMPAttitudesText;
STR16* gzIMPColorChoosingText = lang_default::gzIMPColorChoosingText;
STR16* sColorChoiceExplanationTexts = lang_default::sColorChoiceExplanationTexts;
STR16* gzIMPDisabilityTraitText = lang_default::gzIMPDisabilityTraitText;
STR16* gzIMPDisabilityTraitEmailTextDeaf = lang_default::gzIMPDisabilityTraitEmailTextDeaf;
STR16* gzIMPDisabilityTraitEmailTextShortSighted = lang_default::gzIMPDisabilityTraitEmailTextShortSighted;
STR16* gzIMPDisabilityTraitEmailTextHemophiliac = lang_default::gzIMPDisabilityTraitEmailTextHemophiliac;
STR16* gzIMPDisabilityTraitEmailTextAfraidOfHeights = lang_default::gzIMPDisabilityTraitEmailTextAfraidOfHeights;
STR16* gzIMPDisabilityTraitEmailTextSelfHarm = lang_default::gzIMPDisabilityTraitEmailTextSelfHarm;
STR16* sEnemyTauntsFireGun = lang_default::sEnemyTauntsFireGun;
STR16* sEnemyTauntsFireLauncher = lang_default::sEnemyTauntsFireLauncher;
STR16* sEnemyTauntsThrow = lang_default::sEnemyTauntsThrow;
STR16* sEnemyTauntsChargeKnife = lang_default::sEnemyTauntsChargeKnife;
STR16* sEnemyTauntsRunAway = lang_default::sEnemyTauntsRunAway;
STR16* sEnemyTauntsSeekNoise = lang_default::sEnemyTauntsSeekNoise;
STR16* sEnemyTauntsAlert = lang_default::sEnemyTauntsAlert;
STR16* sEnemyTauntsGotHit = lang_default::sEnemyTauntsGotHit;
STR16* sEnemyTauntsNoticedMerc = lang_default::sEnemyTauntsNoticedMerc;
STR16* sSpecialCharacters = lang_default::sSpecialCharacters;
STR16* gzFacilityErrorMessage = lang_default::gzFacilityErrorMessage;
STR16* gzFacilityAssignmentStrings = lang_default::gzFacilityAssignmentStrings;
STR16* gzFacilityRiskResultStrings = lang_default::gzFacilityRiskResultStrings;
STR16* gzNCTHlabels = lang_default::gzNCTHlabels;
STR16* gzMapInventorySortingMessage = lang_default::gzMapInventorySortingMessage;
STR16* gzMapInventoryFilterOptions = lang_default::gzMapInventoryFilterOptions;
STR16* gzMercCompare = lang_default::gzMercCompare;
STR16* pAntiHackerString = lang_default::pAntiHackerString;

#define BIND_ALL(NS) \
    pMenuStrings = NS::pMenuStrings; \
    pLocationPageText = NS::pLocationPageText; \
    pSectorPageText = NS::pSectorPageText; \
    pEncyclopediaHelpText = NS::pEncyclopediaHelpText; \
    pEncyclopediaTypeText = NS::pEncyclopediaTypeText; \
    pEncyclopediaSkrotyText = NS::pEncyclopediaSkrotyText; \
    pEncyclopediaFilterLocationText = NS::pEncyclopediaFilterLocationText; \
    pEncyclopediaSubFilterLocationText = NS::pEncyclopediaSubFilterLocationText; \
    pEncyclopediaFilterCharText = NS::pEncyclopediaFilterCharText; \
    pEncyclopediaSubFilterCharText = NS::pEncyclopediaSubFilterCharText; \
    pEncyclopediaFilterItemText = NS::pEncyclopediaFilterItemText; \
    pEncyclopediaSubFilterItemText = NS::pEncyclopediaSubFilterItemText; \
    pEncyclopediaFilterQuestText = NS::pEncyclopediaFilterQuestText; \
    pEncyclopediaSubFilterQuestText = NS::pEncyclopediaSubFilterQuestText; \
    pEncyclopediaShortCharacterText = NS::pEncyclopediaShortCharacterText; \
    pEncyclopediaHelpCharacterText = NS::pEncyclopediaHelpCharacterText; \
    pEncyclopediaShortInventoryText = NS::pEncyclopediaShortInventoryText; \
    BoxFilter = NS::BoxFilter; \
    pOtherButtonsText = NS::pOtherButtonsText; \
    pOtherButtonsHelpText = NS::pOtherButtonsHelpText; \
    QuestDescText = NS::QuestDescText; \
    FactDescText = NS::FactDescText; \
    iEditorItemStatsButtonsText = NS::iEditorItemStatsButtonsText; \
    FaceDirs = NS::FaceDirs; \
    iEditorMercsToolbarText = NS::iEditorMercsToolbarText; \
    iEditorBuildingsToolbarText = NS::iEditorBuildingsToolbarText; \
    iEditorItemsToolbarText = NS::iEditorItemsToolbarText; \
    iEditorMapInfoToolbarText = NS::iEditorMapInfoToolbarText; \
    iEditorOptionsToolbarText = NS::iEditorOptionsToolbarText; \
    iEditorTerrainToolbarText = NS::iEditorTerrainToolbarText; \
    iEditorTaskbarInternalText = NS::iEditorTaskbarInternalText; \
    iRenderMapEntryPointsAndLightsText = NS::iRenderMapEntryPointsAndLightsText; \
    iBuildTriggerNameText = NS::iBuildTriggerNameText; \
    iRenderDoorLockInfoText = NS::iRenderDoorLockInfoText; \
    iRenderEditorInfoText = NS::iRenderEditorInfoText; \
    iUpdateBuildingsInfoText = NS::iUpdateBuildingsInfoText; \
    iRenderDoorEditingWindowText = NS::iRenderDoorEditingWindowText; \
    pInitEditorItemsInfoText = NS::pInitEditorItemsInfoText; \
    pDisplayItemStatisticsTex = NS::pDisplayItemStatisticsTex; \
    pUpdateMapInfoText = NS::pUpdateMapInfoText; \
    gszScheduleActions = NS::gszScheduleActions; \
    zDiffNames = NS::zDiffNames; \
    EditMercStat = NS::EditMercStat; \
    EditMercOrders = NS::EditMercOrders; \
    EditMercAttitudes = NS::EditMercAttitudes; \
    pDisplayEditMercWindowText = NS::pDisplayEditMercWindowText; \
    pCreateEditMercWindowText = NS::pCreateEditMercWindowText; \
    pDisplayBodyTypeInfoText = NS::pDisplayBodyTypeInfoText; \
    pUpdateMercsInfoText = NS::pUpdateMercsInfoText; \
    pRenderMercStringsText = NS::pRenderMercStringsText; \
    pClearCurrentScheduleText = NS::pClearCurrentScheduleText; \
    pCopyMercPlacementText = NS::pCopyMercPlacementText; \
    pPasteMercPlacementText = NS::pPasteMercPlacementText; \
    pEditModeShutdownText = NS::pEditModeShutdownText; \
    pHandleKeyboardShortcutsText = NS::pHandleKeyboardShortcutsText; \
    pPerformSelectedActionText = NS::pPerformSelectedActionText; \
    pWaitForHelpScreenResponseText = NS::pWaitForHelpScreenResponseText; \
    pAutoLoadMapText = NS::pAutoLoadMapText; \
    pShowHighGroundText = NS::pShowHighGroundText; \
    pUpdateItemStatsPanelText = NS::pUpdateItemStatsPanelText; \
    pSetupGameTypeFlagsText = NS::pSetupGameTypeFlagsText; \
    pSetupGunGUIText = NS::pSetupGunGUIText; \
    pSetupArmourGUIText = NS::pSetupArmourGUIText; \
    pSetupExplosivesGUIText = NS::pSetupExplosivesGUIText; \
    pSetupTriggersGUIText = NS::pSetupTriggersGUIText; \
    pCreateSummaryWindowText = NS::pCreateSummaryWindowText; \
    pRenderSectorInformationText = NS::pRenderSectorInformationText; \
    pRenderItemDetailsText = NS::pRenderItemDetailsText; \
    pRenderSummaryWindowText = NS::pRenderSummaryWindowText; \
    pUpdateSectorSummaryText = NS::pUpdateSectorSummaryText; \
    pSummaryLoadMapCallbackText = NS::pSummaryLoadMapCallbackText; \
    pReportErrorText = NS::pReportErrorText; \
    pRegenerateSummaryInfoForAllOutdatedMapsText = NS::pRegenerateSummaryInfoForAllOutdatedMapsText; \
    pSummaryUpdateCallbackText = NS::pSummaryUpdateCallbackText; \
    pApologizeOverrideAndForceUpdateEverythingText = NS::pApologizeOverrideAndForceUpdateEverythingText; \
    pDisplaySelectionWindowGraphicalInformationText = NS::pDisplaySelectionWindowGraphicalInformationText; \
    pDisplaySelectionWindowButtonText = NS::pDisplaySelectionWindowButtonText; \
    wszSelType = NS::wszSelType; \
    gzNewLaptopMessages = NS::gzNewLaptopMessages; \
    zNewTacticalMessages = NS::zNewTacticalMessages; \
    gszAimPages = NS::gszAimPages; \
    zGrod = NS::zGrod; \
    pCreditsJA2113 = NS::pCreditsJA2113; \
    ShortItemNames = NS::ShortItemNames; \
    ItemNames = NS::ItemNames; \
    AmmoCaliber = NS::AmmoCaliber; \
    BobbyRayAmmoCaliber = NS::BobbyRayAmmoCaliber; \
    WeaponType = NS::WeaponType; \
    Message = NS::Message; \
    TeamTurnString = NS::TeamTurnString; \
    pMilitiaControlMenuStrings = NS::pMilitiaControlMenuStrings; \
    pTraitSkillsMenuStrings = NS::pTraitSkillsMenuStrings; \
    pTraitSkillsMenuDescStrings = NS::pTraitSkillsMenuDescStrings; \
    pTraitSkillsDenialStrings = NS::pTraitSkillsDenialStrings; \
    pSkillMenuStrings = NS::pSkillMenuStrings; \
    pSnitchMenuStrings = NS::pSnitchMenuStrings; \
    pSnitchMenuDescStrings = NS::pSnitchMenuDescStrings; \
    pSnitchToggleMenuStrings = NS::pSnitchToggleMenuStrings; \
    pSnitchToggleMenuDescStrings = NS::pSnitchToggleMenuDescStrings; \
    pSnitchSectorMenuStrings = NS::pSnitchSectorMenuStrings; \
    pSnitchSectorMenuDescStrings = NS::pSnitchSectorMenuDescStrings; \
    pPrisonerMenuStrings = NS::pPrisonerMenuStrings; \
    pPrisonerMenuDescStrings = NS::pPrisonerMenuDescStrings; \
    pSnitchPrisonExposedStrings = NS::pSnitchPrisonExposedStrings; \
    pSnitchGatheringRumoursResultStrings = NS::pSnitchGatheringRumoursResultStrings; \
    pAssignMenuStrings = NS::pAssignMenuStrings; \
    pTrainingStrings = NS::pTrainingStrings; \
    pTrainingMenuStrings = NS::pTrainingMenuStrings; \
    pAttributeMenuStrings = NS::pAttributeMenuStrings; \
    pVehicleStrings = NS::pVehicleStrings; \
    pShortAttributeStrings = NS::pShortAttributeStrings; \
    pLongAttributeStrings = NS::pLongAttributeStrings; \
    pContractStrings = NS::pContractStrings; \
    pAssignmentStrings = NS::pAssignmentStrings; \
    pConditionStrings = NS::pConditionStrings; \
    pCountryNames = NS::pCountryNames; \
    pTownNames = NS::pTownNames; \
    pPersonnelScreenStrings = NS::pPersonnelScreenStrings; \
    pPersonnelRecordsHelpTexts = NS::pPersonnelRecordsHelpTexts; \
    pPersonnelTitle = NS::pPersonnelTitle; \
    pUpperLeftMapScreenStrings = NS::pUpperLeftMapScreenStrings; \
    pTacticalPopupButtonStrings = NS::pTacticalPopupButtonStrings; \
    pSquadMenuStrings = NS::pSquadMenuStrings; \
    pDoorTrapStrings = NS::pDoorTrapStrings; \
    pLongAssignmentStrings = NS::pLongAssignmentStrings; \
    pContractExtendStrings = NS::pContractExtendStrings; \
    pMapScreenMouseRegionHelpText = NS::pMapScreenMouseRegionHelpText; \
    pPersonnelAssignmentStrings = NS::pPersonnelAssignmentStrings; \
    pNoiseVolStr = NS::pNoiseVolStr; \
    pNoiseTypeStr = NS::pNoiseTypeStr; \
    pDirectionStr = NS::pDirectionStr; \
    pRemoveMercStrings = NS::pRemoveMercStrings; \
    sTimeStrings = NS::sTimeStrings; \
    pLandTypeStrings = NS::pLandTypeStrings; \
    pGuardMenuStrings = NS::pGuardMenuStrings; \
    pOtherGuardMenuStrings = NS::pOtherGuardMenuStrings; \
    pInvPanelTitleStrings = NS::pInvPanelTitleStrings; \
    pPOWStrings = NS::pPOWStrings; \
    pMilitiaString = NS::pMilitiaString; \
    pMilitiaButtonString = NS::pMilitiaButtonString; \
    pEpcMenuStrings = NS::pEpcMenuStrings; \
    pRepairStrings = NS::pRepairStrings; \
    sPreStatBuildString = NS::sPreStatBuildString; \
    sStatGainStrings = NS::sStatGainStrings; \
    pHelicopterEtaStrings = NS::pHelicopterEtaStrings; \
    pHelicopterRepairRefuelStrings = NS::pHelicopterRepairRefuelStrings; \
    sMapLevelString = NS::sMapLevelString; \
    gsLoyalString = NS::gsLoyalString; \
    gsUndergroundString = NS::gsUndergroundString; \
    gsTimeStrings = NS::gsTimeStrings; \
    sFacilitiesStrings = NS::sFacilitiesStrings; \
    pMapPopUpInventoryText = NS::pMapPopUpInventoryText; \
    pwTownInfoStrings = NS::pwTownInfoStrings; \
    pwMineStrings = NS::pwMineStrings; \
    pwMiscSectorStrings = NS::pwMiscSectorStrings; \
    pMapInventoryErrorString = NS::pMapInventoryErrorString; \
    pMapInventoryStrings = NS::pMapInventoryStrings; \
    pMapScreenFastHelpTextList = NS::pMapScreenFastHelpTextList; \
    pMovementMenuStrings = NS::pMovementMenuStrings; \
    pUpdateMercStrings = NS::pUpdateMercStrings; \
    pMapScreenBorderButtonHelpText = NS::pMapScreenBorderButtonHelpText; \
    pMapScreenInvenButtonHelpText = NS::pMapScreenInvenButtonHelpText; \
    pMapScreenBottomFastHelp = NS::pMapScreenBottomFastHelp; \
    pMapScreenBottomText = NS::pMapScreenBottomText; \
    pMercDeadString = NS::pMercDeadString; \
    pSenderNameList = NS::pSenderNameList; \
    pTraverseStrings = NS::pTraverseStrings; \
    pNewMailStrings = NS::pNewMailStrings; \
    pDeleteMailStrings = NS::pDeleteMailStrings; \
    pEmailHeaders = NS::pEmailHeaders; \
    pEmailTitleText = NS::pEmailTitleText; \
    pFinanceTitle = NS::pFinanceTitle; \
    pFinanceSummary = NS::pFinanceSummary; \
    pFinanceHeaders = NS::pFinanceHeaders; \
    pTransactionText = NS::pTransactionText; \
    pTransactionAlternateText = NS::pTransactionAlternateText; \
    pMoralStrings = NS::pMoralStrings; \
    pSkyriderText = NS::pSkyriderText; \
    pLeftEquipmentString = NS::pLeftEquipmentString; \
    pMapScreenStatusStrings = NS::pMapScreenStatusStrings; \
    pMapScreenPrevNextCharButtonHelpText = NS::pMapScreenPrevNextCharButtonHelpText; \
    pEtaString = NS::pEtaString; \
    pShortVehicleStrings = NS::pShortVehicleStrings; \
    pTrashItemText = NS::pTrashItemText; \
    pMapErrorString = NS::pMapErrorString; \
    pMapPlotStrings = NS::pMapPlotStrings; \
    pMiscMapScreenMouseRegionHelpText = NS::pMiscMapScreenMouseRegionHelpText; \
    pMercHeLeaveString = NS::pMercHeLeaveString; \
    pMercSheLeaveString = NS::pMercSheLeaveString; \
    pImpPopUpStrings = NS::pImpPopUpStrings; \
    pImpButtonText = NS::pImpButtonText; \
    pExtraIMPStrings = NS::pExtraIMPStrings; \
    pDayStrings = NS::pDayStrings; \
    pMercContractOverStrings = NS::pMercContractOverStrings; \
    pFilesTitle = NS::pFilesTitle; \
    pFilesSenderList = NS::pFilesSenderList; \
    pHistoryLocations = NS::pHistoryLocations; \
    pHistoryHeaders = NS::pHistoryHeaders; \
    pHistoryTitle = NS::pHistoryTitle; \
    pShowBookmarkString = NS::pShowBookmarkString; \
    pWebPagesTitles = NS::pWebPagesTitles; \
    pWebTitle = NS::pWebTitle; \
    pPersonnelString = NS::pPersonnelString; \
    pErrorStrings = NS::pErrorStrings; \
    pDownloadString = NS::pDownloadString; \
    pBookmarkTitle = NS::pBookmarkTitle; \
    pBookMarkStrings = NS::pBookMarkStrings; \
    pLaptopIcons = NS::pLaptopIcons; \
    gsAtmStartButtonText = NS::gsAtmStartButtonText; \
    pPersonnelTeamStatsStrings = NS::pPersonnelTeamStatsStrings; \
    pPersonnelCurrentTeamStatsStrings = NS::pPersonnelCurrentTeamStatsStrings; \
    pPersonelTeamStrings = NS::pPersonelTeamStrings; \
    pPersonnelDepartedStateStrings = NS::pPersonnelDepartedStateStrings; \
    pMapHortIndex = NS::pMapHortIndex; \
    pMapVertIndex = NS::pMapVertIndex; \
    pMapDepthIndex = NS::pMapDepthIndex; \
    pLaptopTitles = NS::pLaptopTitles; \
    pMilitiaConfirmStrings = NS::pMilitiaConfirmStrings; \
    pDeliveryLocationStrings = NS::pDeliveryLocationStrings; \
    pSkillAtZeroWarning = NS::pSkillAtZeroWarning; \
    pIMPBeginScreenStrings = NS::pIMPBeginScreenStrings; \
    pIMPFinishButtonText = NS::pIMPFinishButtonText; \
    pIMPFinishStrings = NS::pIMPFinishStrings; \
    pIMPVoicesStrings = NS::pIMPVoicesStrings; \
    pDepartedMercPortraitStrings = NS::pDepartedMercPortraitStrings; \
    pPersTitleText = NS::pPersTitleText; \
    pPausedGameText = NS::pPausedGameText; \
    zOptionsToggleText = NS::zOptionsToggleText; \
    zOptionsScreenHelpText = NS::zOptionsScreenHelpText; \
    pDoctorWarningString = NS::pDoctorWarningString; \
    pMilitiaButtonsHelpText = NS::pMilitiaButtonsHelpText; \
    pMapScreenJustStartedHelpText = NS::pMapScreenJustStartedHelpText; \
    pLandMarkInSectorString = NS::pLandMarkInSectorString; \
    gzMercSkillText = NS::gzMercSkillText; \
    gzMercSkillTextNew = NS::gzMercSkillTextNew; \
    gzNonPersistantPBIText = NS::gzNonPersistantPBIText; \
    gzMiscString = NS::gzMiscString; \
    pBullseyeStrings = NS::pBullseyeStrings; \
    wMapScreenSortButtonHelpText = NS::wMapScreenSortButtonHelpText; \
    pNewNoiseStr = NS::pNewNoiseStr; \
    pTauntUnknownVoice = NS::pTauntUnknownVoice; \
    gzLateLocalizedString = NS::gzLateLocalizedString; \
    gzCWStrings = NS::gzCWStrings; \
    gzTooltipStrings = NS::gzTooltipStrings; \
    pSkillTraitBeginIMPStrings = NS::pSkillTraitBeginIMPStrings; \
    sgAttributeSelectionText = NS::sgAttributeSelectionText; \
    pCharacterTraitBeginIMPStrings = NS::pCharacterTraitBeginIMPStrings; \
    gzIMPCharacterTraitText = NS::gzIMPCharacterTraitText; \
    gzIMPAttitudesText = NS::gzIMPAttitudesText; \
    gzIMPColorChoosingText = NS::gzIMPColorChoosingText; \
    sColorChoiceExplanationTexts = NS::sColorChoiceExplanationTexts; \
    gzIMPDisabilityTraitText = NS::gzIMPDisabilityTraitText; \
    gzIMPDisabilityTraitEmailTextDeaf = NS::gzIMPDisabilityTraitEmailTextDeaf; \
    gzIMPDisabilityTraitEmailTextShortSighted = NS::gzIMPDisabilityTraitEmailTextShortSighted; \
    gzIMPDisabilityTraitEmailTextHemophiliac = NS::gzIMPDisabilityTraitEmailTextHemophiliac; \
    gzIMPDisabilityTraitEmailTextAfraidOfHeights = NS::gzIMPDisabilityTraitEmailTextAfraidOfHeights; \
    gzIMPDisabilityTraitEmailTextSelfHarm = NS::gzIMPDisabilityTraitEmailTextSelfHarm; \
    sEnemyTauntsFireGun = NS::sEnemyTauntsFireGun; \
    sEnemyTauntsFireLauncher = NS::sEnemyTauntsFireLauncher; \
    sEnemyTauntsThrow = NS::sEnemyTauntsThrow; \
    sEnemyTauntsChargeKnife = NS::sEnemyTauntsChargeKnife; \
    sEnemyTauntsRunAway = NS::sEnemyTauntsRunAway; \
    sEnemyTauntsSeekNoise = NS::sEnemyTauntsSeekNoise; \
    sEnemyTauntsAlert = NS::sEnemyTauntsAlert; \
    sEnemyTauntsGotHit = NS::sEnemyTauntsGotHit; \
    sEnemyTauntsNoticedMerc = NS::sEnemyTauntsNoticedMerc; \
    sSpecialCharacters = NS::sSpecialCharacters; \
    gzFacilityErrorMessage = NS::gzFacilityErrorMessage; \
    gzFacilityAssignmentStrings = NS::gzFacilityAssignmentStrings; \
    gzFacilityRiskResultStrings = NS::gzFacilityRiskResultStrings; \
    gzNCTHlabels = NS::gzNCTHlabels; \
    gzMapInventorySortingMessage = NS::gzMapInventorySortingMessage; \
    gzMapInventoryFilterOptions = NS::gzMapInventoryFilterOptions; \
    gzMercCompare = NS::gzMercCompare; \
    pAntiHackerString = NS::pAntiHackerString;

auto BindLanguageStrings(i18n::Lang lang) -> void {
  switch (lang) {
    case i18n::Lang::en: BIND_ALL(lang_en); break;
    case i18n::Lang::de: BIND_ALL(lang_de); break;
    case i18n::Lang::ru: BIND_ALL(lang_ru); break;
    case i18n::Lang::nl: BIND_ALL(lang_nl); break;
    case i18n::Lang::pl: BIND_ALL(lang_pl); break;
    case i18n::Lang::fr: BIND_ALL(lang_fr); break;
    case i18n::Lang::it: BIND_ALL(lang_it); break;
    case i18n::Lang::zh: BIND_ALL(lang_zh); break;
  }
}
