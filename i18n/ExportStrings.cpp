#include "ExportStrings.h"
#include "LocalizedStrings.h"
#include "Map Screen Interface.h"
#include "personnel.h"
#include "soldier profile type.h"
#include "Interface.h"
#include "Keys.h"
#include "Merc Contract.h"
#include "Campaign Types.h"
#include "finances.h"
#include "laptop.h"
#include <language.hpp>

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_string.h>
#include <vfs/Tools/vfs_tools.h>
#include <vfs/Tools/vfs_parser_tools.h>
#include <vfs/Tools/vfs_property_container.h>

namespace
{
	void Translate(vfs::String::char_t* str, int len, i18n::Lang lang);

	void ExportMercBio();
	void ExportAIMHistory();
	void ExportAIMPolicy();
	void ExportAlumniName();
	void ExportDialogues();
	void ExportNPCDialogues();

	template<typename T>
	void ExportSection(vfs::PropertyContainer& props, const vfs::String::char_t* section_name, T* strings, int min, int max)
	{
		for(int i = min; i < max; ++i)
		{
			vfs::String str(strings[i]);
			if(!str.empty())
			{
				props.setStringProperty(section_name, vfs::toString<wchar_t>(i), str);
			}
		}
	}

	template<>
	void ExportSection<wchar_t>(vfs::PropertyContainer& props, const vfs::String::char_t* section_name, wchar_t* strings, int min, int max)
	{
		ExportSection(props,section_name, &strings, min, max);
	}
}

//////////////////////////////////////////////////////////

#include "Text.h"


// Table symbols below (Loc::pTownNames etc.) are unqualified inside Loc::ExportStrings's
// body, so they resolve to the pointer globals in Text.h/LanguageStrings.cpp -- rebound to
// the runtime-selected g_lang by BindLanguageStrings before this ever runs (sgp.cpp,
// GetRuntimeSettings then EXPORT_STRINGS ini check). No private per-language copy needed.

#include "Assignments.h"
#include "history.h"

bool Loc::ExportStrings()
{
	vfs::PropertyContainer::TagMap tmap;
	//tmap.Container(L"LocalizedStrings");
	//tmap.Section(L"Topic");
	//tmap.SectionID(L"name");
	//tmap.Key(L"msg");
	//tmap.KeyID(L"index");

	vfs::PropertyContainer props;

	//not_required ExportSection(props, L"Ja2Credits", pCreditsJA2113, 0, 7);
	ExportSection(props, L"WeaponType",					WeaponType,					0,	MAXITEMS);
	ExportSection(props, L"TeamTurn",					TeamTurnString,				0,	10);
	ExportSection(props, L"Message",					Message,						0,	TEXT_NUM_STR_MESSAGE);
	ExportSection(props, L"TownNames",					pTownNames,					0,	MAX_TOWNS);
	ExportSection(props, L"Time",						sTimeStrings,					0,	6);
	ExportSection(props, L"Assignment",					pAssignmentStrings,			0,	NUM_ASSIGNMENTS);
	ExportSection(props, L"PersonnelAssignment",		pPersonnelAssignmentStrings,	0,	NUM_ASSIGNMENTS);
	ExportSection(props, L"LongAssignment",				pLongAssignmentStrings,		0,	NUM_ASSIGNMENTS);
	ExportSection(props, L"Militia",					pMilitiaString,				0,	3);

	ExportSection(props, L"MilitiaButton",				pMilitiaButtonString,			0,	2);
	ExportSection(props, L"Condition",					pConditionStrings,				0,	9);
	ExportSection(props, L"EpcMenu",					pEpcMenuStrings,				0,	MAX_EPC_MENU_STRING_COUNT);
	ExportSection(props, L"Contract",					pContractStrings,				0,	MAX_CONTRACT_MENU_STRING_COUNT);
	ExportSection(props, L"POW",						pPOWStrings,					0,	2);
	ExportSection(props, L"InvPanelTitle",				pInvPanelTitleStrings,			0,	5);
	ExportSection(props, L"LongAttribute",				pLongAttributeStrings,			0,	10);
	ExportSection(props, L"ShortAttribute",				pShortAttributeStrings,		0,	10);
	ExportSection(props, L"UpperLeftMapScreen",			pUpperLeftMapScreenStrings,	0,	6);
	ExportSection(props, L"Training",					pTrainingStrings,				0,	4);

	ExportSection(props, L"GuardMenu",					pGuardMenuStrings,				0,	10);
	ExportSection(props, L"OtherGuardMenu",				pOtherGuardMenuStrings,		0,	10);
	ExportSection(props, L"AssignMenu",					pAssignMenuStrings,			0,	MAX_ASSIGN_STRING_COUNT);
	ExportSection(props, L"MilitiaControlMenu",			pMilitiaControlMenuStrings,	0,	MAX_MILCON_STRING_COUNT);
	ExportSection(props, L"RemoveMerc",					pRemoveMercStrings,			0,	MAX_REMOVE_MERC_COUNT);
	ExportSection(props, L"AttributeMenu",				pAttributeMenuStrings,			0,	MAX_ATTRIBUTE_STRING_COUNT);
	ExportSection(props, L"TrainingMenu",				pTrainingMenuStrings,			0,	MAX_TRAIN_STRING_COUNT);
	ExportSection(props, L"SquadMenu",					pSquadMenuStrings,				0,	MAX_SQUAD_MENU_STRING_COUNT);

	ExportSection(props, L"SnitchMenu",					pSnitchMenuStrings,			0,	MAX_SNITCH_MENU_STRING_COUNT);
	ExportSection(props, L"SnitchMenuDesc",				pSnitchMenuDescStrings,		0,	MAX_SNITCH_MENU_STRING_COUNT-1);
	ExportSection(props, L"SnitchToggleMenu",			pSnitchToggleMenuStrings,		0,	MAX_SNITCH_TOGGLE_MENU_STRING_COUNT);
	ExportSection(props, L"SnitchToggleMenuDesc",		pSnitchToggleMenuDescStrings,	0,	MAX_SNITCH_TOGGLE_MENU_STRING_COUNT-1);
	ExportSection(props, L"SnitchSectorMenu",			pSnitchSectorMenuStrings,		0,	MAX_SNITCH_SECTOR_MENU_STRING_COUNT);
	ExportSection(props, L"SnitchSectorMenuDesc",		pSnitchSectorMenuDescStrings,	0,	MAX_SNITCH_SECTOR_MENU_STRING_COUNT-1);
	ExportSection(props, L"PrisonerMenu",				pPrisonerMenuStrings,			0,  MAX_PRISONER_MENU_STRING_COUNT );
	ExportSection(props, L"PrisonerMenuDesc",			pPrisonerMenuDescStrings,		0,  MAX_PRISONER_MENU_STRING_COUNT - 1 );
	ExportSection(props, L"SnitchPrisonExposed",		pSnitchPrisonExposedStrings,	0,	NUM_SNITCH_PRISON_EXPOSED);
	ExportSection(props, L"SnitchGatheringRumoursResult",	pSnitchGatheringRumoursResultStrings,	0,	NUM_SNITCH_GATHERING_RUMOURS_RESULT);

	ExportSection(props, L"PersonnelTitle",				pPersonnelTitle,				0,	1);
	ExportSection(props, L"PersonnelScreen",			pPersonnelScreenStrings,		0,	TEXT_NUM_PRSNL);

	ExportSection(props, L"MercSkill",					gzMercSkillText,				0,	NUM_SKILLTRAITS_OT);
	ExportSection(props, L"TacticalPopupButton",		pTacticalPopupButtonStrings,	0,	NUM_ICONS);
	ExportSection(props, L"DoorTrap",					pDoorTrapStrings,				0,	NUM_DOOR_TRAPS);
	ExportSection(props, L"ContractExtend",				pContractExtendStrings,		0,	NUM_CONTRACT_EXTEND);
	ExportSection(props, L"MapScreenMouseRegionHelp",	pMapScreenMouseRegionHelpText,	0,	6);
	ExportSection(props, L"NoiseVol",					pNoiseVolStr,					0,	4);
	ExportSection(props, L"NoiseType",					pNoiseTypeStr,					0,	12);
	ExportSection(props, L"Direction",					pDirectionStr,					0,	8);
	ExportSection(props, L"LandType",					pLandTypeStrings,				0,	NUM_TRAVTERRAIN_TYPES);
	ExportSection(props, L"Strategic",					gpStrategicString,				0,	TEXT_NUM_STRATEGIC_TEXT);

	ExportSection(props, L"GameClock",					gpGameClockString,				0,	TEXT_NUM_GAMECLOCK);
	ExportSection(props, L"KeyDescription",				sKeyDescriptionStrings,		0,	2);
	ExportSection(props, L"WeaponStatsDesc",			gWeaponStatsDesc,				0,	17);	
	ExportSection(props, L"WeaponStatsFasthelpTactical",gzWeaponStatsFasthelpTactical, 0,	29);
	ExportSection(props, L"MiscItemStatsFasthelp",		gzMiscItemStatsFasthelp,		0,	34);
	ExportSection(props, L"MoneyStatsDesc",				gMoneyStatsDesc,				0,	TEXT_NUM_MONEY_DESC);

	ExportSection(props, L"Health",						zHealthStr,					0,	7);
	ExportSection(props, L"MoneyAmounts",				gzMoneyAmounts,				0,	6);
	ExportSection(props, L"ProsLabel",					gzProsLabel,					0,	1);
	ExportSection(props, L"ConsLabel",					gzConsLabel,					0,	1);
	ExportSection(props, L"TalkMenu",					zTalkMenuStrings,				0,	6);
	ExportSection(props, L"Dealer",						zDealerStrings,				0,	4);
	ExportSection(props, L"DialogActions",				zDialogActions,				0,	1);
	ExportSection(props, L"Vehicle",					pVehicleStrings,				0,	6);
	ExportSection(props, L"ShortVehicle",				pShortVehicleStrings,			0,	6);
	ExportSection(props, L"VehicleName",				zVehicleName,					0,	6);
	ExportSection(props, L"VehicleSeatsStrings",		pVehicleSeatsStrings,			0,	2);

	ExportSection(props, L"Tactical",					TacticalStr,					0,	TEXT_NUM_TACTICAL_STR);
	ExportSection(props, L"ExitingSectorHelp",			pExitingSectorHelpText,		0,	TEXT_NUM_EXIT_GUI);
	ExportSection(props, L"Repair",						pRepairStrings,				0,	4);
	ExportSection(props, L"PreStatBuild",				sPreStatBuildString,			0,	6);
	ExportSection(props, L"StatGain",					sStatGainStrings,				0,	11);
	ExportSection(props, L"HelicopterEta",				pHelicopterEtaStrings,			0,	TEXT_NUM_STR_HELI_ETA);
	ExportSection(props, L"HelicopterRepair",			pHelicopterRepairRefuelStrings,		0,	TEXT_NUM_STR_HELI_REPAIRS);
	ExportSection(props, L"MapLevel",					sMapLevelString,				0,	1);
	ExportSection(props, L"Loyal",						gsLoyalString,					0,	1);
	ExportSection(props, L"Underground",				gsUndergroundString,			0,	1);
	ExportSection(props, L"TimeStings",					gsTimeStrings,					0,	1);

	ExportSection(props, L"Facilities",					sFacilitiesStrings,			0,	7);
	ExportSection(props, L"MapPopUpInventory",			pMapPopUpInventoryText,		0,	2);
	ExportSection(props, L"TownInfo",					pwTownInfoStrings,				0,	12);
	ExportSection(props, L"Mine",						pwMineStrings,					0,	14);
	ExportSection(props, L"MiscSector",					pwMiscSectorStrings,			0,	7);
	ExportSection(props, L"MapInventoryError",			pMapInventoryErrorString,		0,	7);
	ExportSection(props, L"MapInventory",				pMapInventoryStrings,			0,	2);
	ExportSection(props, L"MapScreenFastHelp",			pMapScreenFastHelpTextList,	0,	10);
	ExportSection(props, L"MovementMenu",				pMovementMenuStrings,			0,	4);
	ExportSection(props, L"UpdateMerc",					pUpdateMercStrings,			0,	6);

	ExportSection(props, L"MapScreenBorderButtonHelp",	pMapScreenBorderButtonHelpText,0,	6);
	ExportSection(props, L"MapScreenBottomFastHelp",	pMapScreenBottomFastHelp,		0,	8);
	ExportSection(props, L"MapScreenBottom",			pMapScreenBottomText,			0,	1);
	ExportSection(props, L"MercDead",					pMercDeadString,				0,	1);
	ExportSection(props, L"Day",						pDayStrings,					0,	1);
	ExportSection(props, L"SenderName",					pSenderNameList,				0,	51);
	ExportSection(props, L"Traverse",					pTraverseStrings,				0,	2);
	ExportSection(props, L"NewMail",					pNewMailStrings,				0,	1);
	ExportSection(props, L"DeleteMail",					pDeleteMailStrings,			0,	2);
	ExportSection(props, L"EmailHeader",				pEmailHeaders,					0,	3);

	ExportSection(props, L"EmailTitle",					pEmailTitleText,				0,	1);
	ExportSection(props, L"FinanceTitle",				pFinanceTitle,					0,	1);
	ExportSection(props, L"FinanceSummary",				pFinanceSummary,				0,	12);
	ExportSection(props, L"FinanceHeader",				pFinanceHeaders,				0,	7);
	ExportSection(props, L"Transaction",				pTransactionText,				0,	TEXT_NUM_FINCANCES);
	ExportSection(props, L"TransactionAlternate",		pTransactionAlternateText,		0,	4);
	ExportSection(props, L"Skyrider",					pSkyriderText,					0,	7);
	ExportSection(props, L"Moral",						pMoralStrings,					0,	6);
	ExportSection(props, L"LeftEquipment",				pLeftEquipmentString,			0,	2);
	ExportSection(props, L"MapScreenStatus",			pMapScreenStatusStrings,		0,	5);

	ExportSection(props, L"MapScreenPrevNextCharButtonHelp",	pMapScreenPrevNextCharButtonHelpText,	0,	2);
	ExportSection(props, L"Eta",								pEtaString,							0,	1);
	ExportSection(props, L"TrashItem",							pTrashItemText,						0,	2);
	ExportSection(props, L"MapError",							pMapErrorString,						0,	50);
	ExportSection(props, L"MapPlot",							pMapPlotStrings,						0,	5);
	ExportSection(props, L"Bullseye",							pBullseyeStrings,						0,	5);
	ExportSection(props, L"MiscMapScreenMouseRegionHelp",		pMiscMapScreenMouseRegionHelpText,		0,	3);
	ExportSection(props, L"MercHeLeave",						pMercHeLeaveString,					0,	5);
	ExportSection(props, L"MercSheLeave",						pMercSheLeaveString,					0,	5);
	ExportSection(props, L"MercContractOver",					pMercContractOverStrings,				0,	5);

	ExportSection(props, L"ImpPopUp",					pImpPopUpStrings,				0,	12);
	ExportSection(props, L"ImpButton",					pImpButtonText,				0,	26);
	ExportSection(props, L"ExtraIMP",					pExtraIMPStrings,				0,	4);
	ExportSection(props, L"FilesTitle",					pFilesTitle,					0,	1);
	ExportSection(props, L"FilesSender",				pFilesSenderList,				0,	7);
	ExportSection(props, L"HistoryTitle",				pHistoryTitle,					0,	1);
	ExportSection(props, L"HistoryHeader",				pHistoryHeaders,				0,	5);
	//ExportSection(props, L"History",					pHistoryStrings,				0,	TEXT_NUM_HISTORY);
	ExportSection(props, L"HistoryLocation",			pHistoryLocations,				0,	1);
	ExportSection(props, L"LaptopIcon",					pLaptopIcons,					0,	8);

	ExportSection(props, L"BookMark",					pBookMarkStrings,				0,	TEXT_NUM_LAPTOP_BOOKMARKS);
	ExportSection(props, L"BookmarkTitle",				pBookmarkTitle,				0,	2);
	ExportSection(props, L"Download",					pDownloadString,				0,	2);
	ExportSection(props, L"AtmStartButton",				gsAtmStartButtonText,			0,	4);
	ExportSection(props, L"Error",						pErrorStrings,					0,	5);
	ExportSection(props, L"Personnel",					pPersonnelString,				0,	1);
	ExportSection(props, L"WebTitle",					pWebTitle,						0,	1);
	ExportSection(props, L"WebPagesTitle",				pWebPagesTitles,				0,	36);

	ExportSection(props, L"ShowBookmark",				pShowBookmarkString,				0,	2);
	ExportSection(props, L"LaptopTitle",				pLaptopTitles,						0,	5);
	ExportSection(props, L"PersonnelDepartedState",		pPersonnelDepartedStateStrings,	0,	TEXT_NUM_DEPARTED);
	ExportSection(props, L"PersonelTeam",				pPersonelTeamStrings,				0,	8);
	ExportSection(props, L"PersonnelCurrentTeamStats",	pPersonnelCurrentTeamStatsStrings, 0,	3);
	ExportSection(props, L"PersonnelTeamStats",			pPersonnelTeamStatsStrings,		0,	11);
	ExportSection(props, L"MapVertIndex",				pMapVertIndex,						0,	17);
	ExportSection(props, L"MapHortIndex",				pMapHortIndex,						0,	17);
	ExportSection(props, L"MapDepthIndex",				pMapDepthIndex,					0,	4);
	ExportSection(props, L"ContractButton",				pContractButtonString,				0,	1);

	ExportSection(props, L"UpdatePanelButton",			pUpdatePanelButtons,			0,	2);
	ExportSection(props, L"LargeTactical",				LargeTacticalStr,				0,	TEXT_NUM_LARGESTR);
	ExportSection(props, L"InsContract",				InsContractText,				0,	TEXT_NUM_INS_CONTRACT);
	ExportSection(props, L"InsInfo",					InsInfoText,					0,	TEXT_NUM_INS_INFO);
	ExportSection(props, L"MercAccount",				MercAccountText,				0,	TEXT_NUM_MERC_ACCOUNT);
	ExportSection(props, L"MercAccountPage",			MercAccountPageText,			0,	2);
	ExportSection(props, L"MercInfo",					MercInfo,						0,	TEXT_NUM_MERC_FILES);
	ExportSection(props, L"MercNoAccount",				MercNoAccountText,				0,	TEXT_NUM_MERC_NO_ACC);
	ExportSection(props, L"MercHomePage",				MercHomePageText,				0,	TEXT_NUM_MERC);
	ExportSection(props, L"Funeral",					sFuneralString,				0,	TEXT_NUM_FUNERAL);

	ExportSection(props, L"Florist",					sFloristText,					0,	TEXT_NUM_FLORIST);
	ExportSection(props, L"OrderForm",					sOrderFormText,				0,	TEXT_NUM_FLORIST_ORDER);
	ExportSection(props, L"FloristGallery",				sFloristGalleryText,			0,	TEXT_NUM_FLORIST_GALLERY);
	ExportSection(props, L"FloristCards",				sFloristCards,					0,	TEXT_NUM_FLORIST_CARDS);
	ExportSection(props, L"BobbyROrderForm",			BobbyROrderFormText,			0,	TEXT_NUM_BOBBYR_MAILORDER);
	ExportSection(props, L"BobbyRFilter",				BobbyRFilter,					0,	TEXT_NUM_BOBBYR_FILTER);
	ExportSection(props, L"BobbyR",						BobbyRText,					0,	TEXT_NUM_BOBBYR_GUNS);
	ExportSection(props, L"BobbyRaysFront",				BobbyRaysFrontText,			0,	TEXT_NUM_BOBBYR);
	ExportSection(props, L"AimSort", AimSortText, 0, TEXT_NUM_AIM_SORT);
	ExportSection(props, L"AimPolicy", AimPolicyText, 0, TEXT_NUM_AIM_POLICIES);

	ExportSection(props, L"AimMember",					AimMemberText,					0,	4);
	ExportSection(props, L"CharacterInfo",				CharacterInfo,					0,	TEXT_NUM_AIM_MEMBER_CHARINFO);
	ExportSection(props, L"VideoConfercing",			VideoConfercingText,			0,	TEXT_NUM_AIM_MEMBER_VCONF);
	ExportSection(props, L"AimPopUp",					AimPopUpText,					0,	TEXT_NUM_AIM_MEMBER_POPUP);
	ExportSection(props, L"AimLink",					AimLinkText,					0,	TEXM_NUM_AIM_LINK);
	ExportSection(props, L"AimHistory",					AimHistoryText,				0,	TEXT_NUM_AIM_HISTORY);
	ExportSection(props, L"AimFi",						AimFiText,						0,	TEXT_NUM_AIM_FI);
	ExportSection(props, L"AimAlumni",					AimAlumniText,					0,	TEXT_NUM_AIM_ALUMNI);
	ExportSection(props, L"AimScreen",					AimScreenText,					0,	TEXT_NUM_AIM_SCREEN);
	ExportSection(props, L"AimBottomMenu",				AimBottomMenuText,				0,	TEXT_NUM_AIM_MENU);

	ExportSection(props, L"SKI",						SKI_Text,						0, TEXT_NUM_SKI_TEXT);
	ExportSection(props, L"SkiAtm",						SkiAtmText,					0, NUM_SKI_ATM_BUTTONS);
	ExportSection(props, L"SkiAtmText",					gzSkiAtmText,					0, TEXT_NUM_SKI_ATM_MODE_TEXT);
	ExportSection(props, L"SkiMessageBox",				SkiMessageBoxText,				0, TEXT_NUM_SKI_MBOX_TEXT);
	ExportSection(props, L"Options",					zOptionsText,					0, TEXT_NUM_OPT_TEXT);
	ExportSection(props, L"SaveLoad",					zSaveLoadText,					0, TEXT_NUM_SLG_TEXT);
	ExportSection(props, L"MarksMapScreen",				zMarksMapScreenText,			0, 25);
	ExportSection(props, L"LandMarkInSector",			pLandMarkInSectorString,		0, 1);
	ExportSection(props, L"MilitiaConfirm",				pMilitiaConfirmStrings,		0, 11);
	ExportSection(props, L"MoneyWithdrawMessage",		gzMoneyWithdrawMessageText,	0, TEXT_NUM_MONEY_WITHDRAW);

	ExportSection(props, L"Copyright",					gzCopyrightText,				0,	1);
	ExportSection(props, L"OptionsToggle",				zOptionsToggleText,			0,	49);
	ExportSection(props, L"OptionsScreenHelp",			zOptionsScreenHelpText,		0,	49);
	ExportSection(props, L"GIOScreen",					gzGIOScreenText,				0,	TEXT_NUM_GIO_TEXT);
	ExportSection(props, L"MPJScreen",					gzMPJScreenText,				0,	TEXT_NUM_MPJ_TEXT);
	ExportSection(props, L"MPJHelpText",				gzMPJHelpText,					0,	10);
	ExportSection(props, L"MPHScreen",					gzMPHScreenText,				0,	TEXT_NUM_MPH_TEXT);	
	ExportSection(props, L"DeliveryLocation",			pDeliveryLocationStrings,		0,	17);
	ExportSection(props, L"SkillAtZeroWarning",			pSkillAtZeroWarning,			0,	1);
	ExportSection(props, L"IMPBeginScreen",				pIMPBeginScreenStrings,		0,	1);
	ExportSection(props, L"IMPFinishButton",			pIMPFinishButtonText,			0,	1);

	ExportSection(props, L"IMPFinish",					pIMPFinishStrings,				0,	1);
	ExportSection(props, L"IMPVoices",					pIMPVoicesStrings,				0,	1);
	ExportSection(props, L"DepartedMercPortrait",		pDepartedMercPortraitStrings,	0,	3);
	ExportSection(props, L"PersTitle",					pPersTitleText,				0,	1);
	ExportSection(props, L"PausedGame",					pPausedGameText,				0,	3);
	ExportSection(props, L"MessageStrings",				pMessageStrings,				0,	TEXT_NUM_MSG);
	ExportSection(props, L"ItemPickupHelpPopup",		ItemPickupHelpPopup,			0,	5);
	ExportSection(props, L"DoctorWarning",				pDoctorWarningString,			0,	2);
	ExportSection(props, L"MilitiaButtonsHelp",			pMilitiaButtonsHelpText,		0,	4);
	ExportSection(props, L"MapScreenJustStartedHelp",	pMapScreenJustStartedHelpText,	0,	2);

	ExportSection(props, L"AntiHacker",					pAntiHackerString,				0,	TEXT_NUM_ANTIHACKERSTR);
	ExportSection(props, L"LaptopHelp",					gzLaptopHelpText,				0,	TEXT_NUM_LAPTOP_BN_BOOKMARK_TEXT);
	ExportSection(props, L"HelpScreen",					gzHelpScreenText,				0,	TEXT_NUM_HLP);
	ExportSection(props, L"NonPersistantPBI",			gzNonPersistantPBIText,		0,	10);
	ExportSection(props, L"MiscString",					gzMiscString,					0,	5);
	ExportSection(props, L"IntroScreen",				gzIntroScreen,					0,	1);
	ExportSection(props, L"NewNoise",					pNewNoiseStr,					0,	11/*MAX_NOISES*/);
	ExportSection(props, L"MapScreenSortButtonHelp",	wMapScreenSortButtonHelpText,	0,	6);
	ExportSection(props, L"BrokenLink",					BrokenLinkText,				0,	TEXT_NUM_BROKEN_LINK);
	ExportSection(props, L"BobbyRShipment",				gzBobbyRShipmentText,			0,	TEXT_NUM_BOBBYR_SHIPMENT);

	ExportSection(props, L"CreditNames",				gzCreditNames,					0,	15);
	ExportSection(props, L"CreditNameTitle",			gzCreditNameTitle,				0,	15);
	ExportSection(props, L"CreditNameFunny",			gzCreditNameFunny,				0,	15);
	ExportSection(props, L"RepairsDone",				sRepairsDoneString,			0,	7);
	ExportSection(props, L"GioDifConfirm",				zGioDifConfirmText,			0,	TEXT_NUM_GIO_CFS);
	ExportSection(props, L"LateLocalized",				gzLateLocalizedString,			0,	64);
	ExportSection(props, L"CWStrings",					gzCWStrings,					0,	1);
	ExportSection(props, L"TooltipStrings",				gzTooltipStrings,				0,	TEXT_NUM_STR_TT);
	ExportSection(props, L"New113Message",				New113Message,					0,	TEXT_NUM_MSG113);

	ExportSection(props, L"New113HAMMessage",			New113HAMMessage,				0,	25);
	ExportSection(props, L"New113MERCMercMail",			New113MERCMercMailTexts,		0,	4);
	ExportSection(props, L"New113AIMMercMail",			New113AIMMercMailTexts,		0,	16);
	ExportSection(props, L"MissingIMPSkills",			MissingIMPSkillsDescriptions,	0,	2);
	ExportSection(props, L"NewInvMessage",				NewInvMessage,					0,	TEXT_NUM_NIV);
	ExportSection(props, L"MPServerMessage",			MPServerMessage,				0,	13);
	ExportSection(props, L"MPClientMessage",			MPClientMessage,				0,	69);	
	ExportSection(props, L"MPEdges",					gszMPEdgesText,				0,	5);
	ExportSection(props, L"MPTeamName",					gszMPTeamNames,				0,	5);
	ExportSection(props, L"MPMapscreen",				gszMPMapscreenText,			0,	9);

	ExportSection(props, L"MPSScreen",					gzMPSScreenText,				0,	TEXT_NUM_MPS_TEXT);
	ExportSection(props, L"MPCScreen",					gzMPCScreenText,				0,	TEXT_NUM_MPC_TEXT);
	ExportSection(props, L"MPChatToggle",				gzMPChatToggleText,			0,	2);
	ExportSection(props, L"MPChatbox",					gzMPChatboxText,				0,	2);

	props.writeToXMLFile(L"Localization/GameStrings.xml",tmap);
	props.writeToIniFile(L"Localization/GameStrings.ini",true);

	ExportMercBio();
	ExportAIMHistory();
	ExportAIMPolicy();
	ExportAlumniName();
	ExportDialogues();
	ExportNPCDialogues();

	return true;
}

#include <vfs/Core/vfs_file_raii.h>
#include "Encrypted File.h"

namespace {
wchar_t ToPolish(wchar_t siChar)
{
	switch( siChar )
	{
		case 165:          siChar = 260;          break;
		case 198:          siChar = 262;          break;
		case 202:          siChar = 280;          break;
		case 163:          siChar = 321;          break;
		case 209:          siChar = 323;          break;
		case 211:          siChar = 211;          break;

		case 140:          siChar = 346;          break;
		case 175:          siChar = 379;          break;
		case 143:          siChar = 377;          break;
		case 185:          siChar = 261;          break;
		case 230:          siChar = 263;          break;
		case 234:          siChar = 281;          break;

		case 179:          siChar = 322;          break;
		case 241:          siChar = 324;          break;
		case 243:          siChar = 243;          break;
		case 156:          siChar = 347;          break;
		case 191:          siChar = 380;          break;
		case 159:          siChar = 378;          break;
	}
	return siChar;
}

wchar_t ToRussian(wchar_t siChar)
{
	switch( siChar )
	{
		//capital letters
		case 168:          siChar = 1025;  break;	//U+0401		   d0 81     CYRILLIC CAPITAL LETTER IO
		case 192:          siChar = 1040;  break; //U+0410     A     d0 90     CYRILLIC CAPITAL LETTER A
		case 193:          siChar = 1041;  break;
		case 194:          siChar = 1042;  break;
		case 195:          siChar = 1043;  break;
		case 196:          siChar = 1044;  break;
		case 197:          siChar = 1045;  break;
		case 198:          siChar = 1046;  break;
		case 199:          siChar = 1047;  break;
		case 200:          siChar = 1048;  break;
		case 201:          siChar = 1049;  break;
		case 202:          siChar = 1050;  break;
		case 203:          siChar = 1051;  break;
		case 204:          siChar = 1052;  break;
		case 205:          siChar = 1053;  break;
		case 206:          siChar = 1054;  break;
		case 207:          siChar = 1055;  break;
		case 208:          siChar = 1056;  break;
		case 209:          siChar = 1057;  break;
		case 210:          siChar = 1058;  break;
		case 211:          siChar = 1059;  break;
		case 212:          siChar = 1060;  break;
		case 213:          siChar = 1061;  break;
		case 214:          siChar = 1062;  break;
		case 215:          siChar = 1063;  break;
		case 216:          siChar = 1064;  break;
		case 217:          siChar = 1065;  break;
		case 218:          siChar = 1066;  break;
		case 219:          siChar = 1067;  break;
		case 220:          siChar = 1068;  break;
		case 221:          siChar = 1069;  break;
		case 222:          siChar = 1070;  break;
		case 223:          siChar = 1071;  break; //U+042F           d0 af     CYRILLIC CAPITAL LETTER YA

		//small letters
		case 185:          siChar = 8470;  break;		// ¹
		case 178:          siChar = 1030;  break;		// ²
		case 161:          siChar = 1038;  break;		// ¡
		case 179:          siChar = 1110;  break;		// ³
		case 162:          siChar = 1118;  break;		// ¢
		case 165:          siChar = 1168;  break;		// ¥
		case 170:          siChar = 1028;  break;		// ª
		case 175:          siChar = 1031;  break;		// ¯
		case 180:          siChar = 1169;  break;		// ´
		case 186:          siChar = 1108;  break;		// º
		case 191:          siChar = 1111;  break;		// ¿

		case 184:          siChar = 1105;  break; //U+0451           d1 91     CYRILLIC SMALL LETTER IO
		case 224:          siChar = 1072;  break; //U+0430     a     d0 b0     CYRILLIC SMALL LETTER A
		case 225:          siChar = 1073;  break;
		case 226:          siChar = 1074;  break;
		case 227:          siChar = 1075;  break;
		case 228:          siChar = 1076;  break;
		case 229:          siChar = 1077;  break;
		case 230:          siChar = 1078;  break;
		case 231:          siChar = 1079;  break;
		case 232:          siChar = 1080;  break;
		case 233:          siChar = 1081;  break;
		case 234:          siChar = 1082;  break;
		case 235:          siChar = 1083;  break;
		case 236:          siChar = 1084;  break;
		case 237:          siChar = 1085;  break;
		case 238:          siChar = 1086;  break;
		case 239:          siChar = 1087;  break; //U+043F           d0 bf     CYRILLIC SMALL LETTER PE
		case 240:          siChar = 1088;  break; //U+0440     p     d1 80     CYRILLIC SMALL LETTER ER
		case 241:          siChar = 1089;  break;
		case 242:          siChar = 1090;  break;
		case 243:          siChar = 1091;  break;
		case 244:          siChar = 1092;  break;
		case 245:          siChar = 1093;  break;
		case 246:          siChar = 1094;  break;
		case 247:          siChar = 1095;  break;
		case 248:          siChar = 1096;  break;
		case 249:          siChar = 1097;  break;
		case 250:          siChar = 1098;  break;
		case 251:          siChar = 1099;  break;
		case 252:          siChar = 1100;  break;
		case 253:          siChar = 1101;  break;
		case 254:          siChar = 1102;  break;
		case 255:          siChar = 1103;  break; //U+044F           d1 8f     CYRILLIC SMALL LETTER YA
	}
	return siChar;
}

void Translate(vfs::String::char_t* str, int len, i18n::Lang lang)
{
	switch (lang) {
		case i18n::Lang::en:
		case i18n::Lang::de:
			break; // nothing to do
		case i18n::Lang::ru:
			for (int i = 0; i < len; i++)
			{
				str[i] = ToRussian(str[i]);
			}
			break;
		case i18n::Lang::pl:
			for (int i = 0; i < len; i++)
			{
				str[i] = ToPolish(str[i]);
			}
			break;
		case i18n::Lang::nl:
		case i18n::Lang::fr:
		case i18n::Lang::it:
		case i18n::Lang::zh:
			break; // no table for these yet
	}
}

void ExportMercBio()
{
	#define	SIZE_MERC_BIO_INFO				400	* 2
	#define SIZE_MERC_ADDITIONAL_INFO		160 * 2

	vfs::String::char_t pInfoString[SIZE_MERC_BIO_INFO];
	vfs::String::char_t pAddInfo[SIZE_MERC_ADDITIONAL_INFO];
	vfs::COpenReadFile rfile("BINARYDATA\\aimbios.edt");
	vfs::tReadableFile& file = rfile.file();

	vfs::PropertyContainer props; 
	for(int i=0; i<40; ++i)
	{
		memset(pInfoString,0,SIZE_MERC_BIO_INFO*sizeof(wchar_t));
		memset(pAddInfo,0,SIZE_MERC_ADDITIONAL_INFO*sizeof(wchar_t));
		//
		file.read((vfs::Byte*)pInfoString, SIZE_MERC_BIO_INFO);
		DecodeString(pInfoString,SIZE_MERC_BIO_INFO);
		Translate(pInfoString, SIZE_MERC_BIO_INFO, g_lang);
		props.setStringProperty(L"Bio", vfs::toString<wchar_t>(i), pInfoString);
		
		file.read((vfs::Byte*)pAddInfo, SIZE_MERC_ADDITIONAL_INFO);
		DecodeString(pAddInfo, SIZE_MERC_ADDITIONAL_INFO);
		Translate(pAddInfo, SIZE_MERC_ADDITIONAL_INFO, g_lang);
		props.setStringProperty(L"Add", vfs::toString<wchar_t>(i), pAddInfo);
	}
	props.writeToXMLFile(L"Localization/AimBiographies.xml", vfs::PropertyContainer::TagMap());
}

void ExportAIMHistory()
{
	#define AIM_HISTORY_LINE_SIZE 400 * 2
	vfs::String::char_t pHistLine[AIM_HISTORY_LINE_SIZE];
	vfs::COpenReadFile rfile("BINARYDATA\\AimHist.edt");
	vfs::tReadableFile& file = rfile.file();

	vfs::PropertyContainer props; 
	for(int i=0; i<23; ++i)
	{
		memset(pHistLine,0,AIM_HISTORY_LINE_SIZE*sizeof(wchar_t));
		//
		file.read((vfs::Byte*)pHistLine, AIM_HISTORY_LINE_SIZE);
		DecodeString(pHistLine,AIM_HISTORY_LINE_SIZE);
		Translate(pHistLine, AIM_HISTORY_LINE_SIZE, g_lang);
		props.setStringProperty(L"Line", vfs::toString<wchar_t>(i), pHistLine);
	}
	props.writeToXMLFile(L"Localization/AimHistory.xml", vfs::PropertyContainer::TagMap());
}
	
	
void ExportAIMPolicy()
{
	#define AIM_HISTORY_LINE_SIZE 400 * 2
	vfs::String::char_t pPolLine[AIM_HISTORY_LINE_SIZE];
	vfs::COpenReadFile rfile("BINARYDATA\\AimPol.edt");
	vfs::tReadableFile& file = rfile.file();

	vfs::PropertyContainer props; 
	for(int i=0; i<46; ++i)
	{
		memset(pPolLine,0,400*sizeof(wchar_t));
		//
		file.read((vfs::Byte*)pPolLine, AIM_HISTORY_LINE_SIZE);
		DecodeString(pPolLine,AIM_HISTORY_LINE_SIZE);
		Translate(pPolLine, AIM_HISTORY_LINE_SIZE, g_lang);
		props.setStringProperty(L"Line", vfs::toString<wchar_t>(i), pPolLine);
	}
	props.writeToXMLFile(L"Localization/AimPolicy.xml", vfs::PropertyContainer::TagMap());
}

void ExportAlumniName()
{
	#define AIM_ALUMNI_NAME_SIZE 80 * 2
	vfs::String::char_t pAlumniName[AIM_ALUMNI_NAME_SIZE];
	vfs::COpenReadFile rfile("BINARYDATA\\AlumName.edt");
	vfs::tReadableFile& file = rfile.file();

	vfs::PropertyContainer props; 
	for(int i=0; i<51; ++i)
	{
		memset(pAlumniName,0,AIM_ALUMNI_NAME_SIZE*sizeof(wchar_t));
		//
		file.read((vfs::Byte*)pAlumniName, AIM_ALUMNI_NAME_SIZE);
		DecodeString(pAlumniName,AIM_ALUMNI_NAME_SIZE);
		Translate(pAlumniName, AIM_ALUMNI_NAME_SIZE, g_lang);
		props.setStringProperty(L"Line", vfs::toString<wchar_t>(i), pAlumniName);
	}
	props.writeToXMLFile(L"Localization/AlumniName.xml", vfs::PropertyContainer::TagMap());
}

void ExportDialogues()
{
	#define DIALOGUESIZE		480
	vfs::String::char_t pDiagLine[DIALOGUESIZE];

	vfs::CVirtualFileSystem::Iterator it = getVFS()->begin(L"MercEdt/*.edt");
	for(; !it.end(); it.next())
	{
		vfs::PropertyContainer props;
		vfs::COpenReadFile rfile(it.value());
		vfs::tReadableFile& file = rfile.file();

		std::wstringstream wss;
		wss.str(file.getName().c_str());
		int id=0;
		wss >> id;

		for(int i=0; i<200; ++i)
		{
			memset(pDiagLine,0,DIALOGUESIZE*sizeof(wchar_t));
			//
			if(file.read((vfs::Byte*)pDiagLine, DIALOGUESIZE) > 0)
			{
				DecodeString(pDiagLine,DIALOGUESIZE);
				Translate(pDiagLine, DIALOGUESIZE, g_lang);
				if(wcslen(pDiagLine))
				{
					props.setStringProperty(vfs::toString<wchar_t>(id),vfs::toString<wchar_t>(i), pDiagLine);
				}
			}
		}
		vfs::Path x(L"Localization/Dialogue");
		x += vfs::Path(file.getName().c_wcs() + L".xml");
		props.writeToXMLFile(x, vfs::PropertyContainer::TagMap());
	}
}

void ExportNPCDialogues()
{
	#define DIALOGUESIZE		480
	#define CIVQUOTESIZE		320
	vfs::String::char_t pDiagLine[DIALOGUESIZE];

	vfs::CVirtualFileSystem::Iterator it = getVFS()->begin(L"npcdata/*.edt");
	for(; !it.end(); it.next())
	{
		vfs::PropertyContainer props;
		vfs::COpenReadFile rfile(it.value());
		vfs::tReadableFile& file = rfile.file();

		vfs::String::str_t const& ws = file.getName().c_wcs();
		vfs::String::str_t::size_type pos = ws.find_first_of(L".");
		vfs::String id = ws.substr(0,pos);

		int SIZE;
		if(vfs::matchPattern(L"civ*", id))
		{
			SIZE = CIVQUOTESIZE;
		}
		else
		{
			SIZE = DIALOGUESIZE;
		}

		for(int i=0; i<200; ++i)
		{
			memset(pDiagLine,0,DIALOGUESIZE*sizeof(wchar_t));
			//
			if(file.read((vfs::Byte*)pDiagLine, SIZE) > 0)
			{
				DecodeString(pDiagLine,SIZE);
				Translate(pDiagLine, SIZE, g_lang);
				if(wcslen(pDiagLine))
				{
					props.setStringProperty(id,vfs::toString<wchar_t>(i), pDiagLine);
				}
			}
		}
		vfs::Path x(L"Localization/NpcDialogue");
		x += vfs::Path(file.getName().c_wcs() + L".xml");
		props.writeToXMLFile(x, vfs::PropertyContainer::TagMap());
	}
}
} // namespace
