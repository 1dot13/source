#include "ExportStrings.h"
#include "LocalizedStrings.h"
#include "Map Screen Interface.h"
#include "personnel.h"
#include "soldier profile type.h"
#include "interface.h"
#include "Keys.h"
#include "Merc Contract.h"
#include "Campaign Types.h"
#include "Finances.h"
#include "Laptop.h"

#include <vfs/Core/vfs_string.h>
#include <vfs/Tools/vfs_tools.h>
#include <vfs/Tools/vfs_parser_tools.h>
#include <vfs/Tools/vfs_property_container.h>

namespace Loc
{
	bool Translate(vfs::String::char_t* str, int len, Language lang);

	void ExportMercBio();
	void ExportAIMHistory();
	void ExportAIMPolicy();
	void ExportAlumniName();
	void ExportDialogues();
	void ExportNPCDialogues();
};

//////////////////////////////////////////////////////////

//#define GERMAN
#include "Text.h"
namespace Loc
{
#ifdef CHINESE
#	include "_ChineseText.cpp"
	static Loc::Language gs_Lang = Loc::Chinese;
#endif
#ifdef DUTCH
#	include "_DutchText.cpp"
	static Loc::Language gs_Lang = Loc::Dutch;
#endif
#ifdef ENGLISH
#	include "_EnglishText.cpp"
	static Loc::Language gs_Lang = Loc::English;
#endif
#ifdef FRENCH
#	include "_FrenchText.cpp"
	static Loc::Language gs_Lang = Loc::French;
#endif
#ifdef GERMAN
#	include "_GermanText.cpp"
	static Loc::Language gs_Lang = Loc::German;
#endif
#ifdef ITALIAN
#	include "_ItalianText.cpp"
	static Loc::Language gs_Lang = Loc::Italian;
#endif
#ifdef POLISH
#	include "_PolishText.cpp"
	static Loc::Language gs_Lang = Loc::Polish;
#endif
#ifdef RUSSIAN
#	include "_RussianText.cpp"
	static Loc::Language gs_Lang = Loc::Russian;
#endif
#ifdef TAIWANESE
#	include "_TaiwaneseText.cpp"
	static Loc::Language gs_Lang = Loc::Taiwanese;
#endif
}

#include "Assignments.h"
#include "History.h"

template<typename T>
void ExportSection(vfs::PropertyContainer& props, const vfs::String::char_t* section_name, T* strings, int min, int max)
{
	for(int i = min; i < max; ++i)
	{
		vfs::String str(strings[i]);
		//Loc::Translate(&str.r_wcs()[0],str.length(), gs_Lang);
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


bool Loc::ExportStrings()
{
	vfs::PropertyContainer::TagMap tmap;
	//tmap.Container(L"LocalizedStrings");
	//tmap.Section(L"Topic");
	//tmap.SectionID(L"name");
	//tmap.Key(L"msg");
	//tmap.KeyID(L"index");

	vfs::PropertyContainer props;

	//not_required ExportSection(props, L"Ja2Credits", Loc::pCreditsJA2113, 0, 7);
	ExportSection(props, L"WeaponType",					Loc::WeaponType,					0,	MAXITEMS);
	ExportSection(props, L"TeamTurn",					Loc::TeamTurnString,				0,	10);
	ExportSection(props, L"Message",					Loc::Message,						0,	TEXT_NUM_STR_MESSAGE);
	ExportSection(props, L"TownNames",					Loc::pTownNames,					0,	MAX_TOWNS);
	ExportSection(props, L"Time",						Loc::sTimeStrings,					0,	6);
	ExportSection(props, L"Assignment",					Loc::pAssignmentStrings,			0,	NUM_ASSIGNMENTS);
	ExportSection(props, L"PersonnelAssignment",		Loc::pPersonnelAssignmentStrings,	0,	NUM_ASSIGNMENTS);
	ExportSection(props, L"LongAssignment",				Loc::pLongAssignmentStrings,		0,	NUM_ASSIGNMENTS);
	ExportSection(props, L"Militia",					Loc::pMilitiaString,				0,	3);

	ExportSection(props, L"MilitiaButton",				Loc::pMilitiaButtonString,			0,	2);
	ExportSection(props, L"Condition",					Loc::pConditionStrings,				0,	9);
	ExportSection(props, L"EpcMenu",					Loc::pEpcMenuStrings,				0,	MAX_EPC_MENU_STRING_COUNT);
	ExportSection(props, L"Contract",					Loc::pContractStrings,				0,	MAX_CONTRACT_MENU_STRING_COUNT);
	ExportSection(props, L"POW",						Loc::pPOWStrings,					0,	2);
	ExportSection(props, L"InvPanelTitle",				Loc::pInvPanelTitleStrings,			0,	5);
	ExportSection(props, L"LongAttribute",				Loc::pLongAttributeStrings,			0,	10);
	ExportSection(props, L"ShortAttribute",				Loc::pShortAttributeStrings,		0,	10);
	ExportSection(props, L"UpperLeftMapScreen",			Loc::pUpperLeftMapScreenStrings,	0,	6);
	ExportSection(props, L"Training",					Loc::pTrainingStrings,				0,	4);

	ExportSection(props, L"GuardMenu",					Loc::pGuardMenuStrings,				0,	10);
	ExportSection(props, L"OtherGuardMenu",				Loc::pOtherGuardMenuStrings,		0,	10);
	ExportSection(props, L"AssignMenu",					Loc::pAssignMenuStrings,			0,	MAX_ASSIGN_STRING_COUNT);
	ExportSection(props, L"MilitiaControlMenu",			Loc::pMilitiaControlMenuStrings,	0,	MAX_MILCON_STRING_COUNT);
	ExportSection(props, L"RemoveMerc",					Loc::pRemoveMercStrings,			0,	MAX_REMOVE_MERC_COUNT);
	ExportSection(props, L"AttributeMenu",				Loc::pAttributeMenuStrings,			0,	MAX_ATTRIBUTE_STRING_COUNT);
	ExportSection(props, L"TrainingMenu",				Loc::pTrainingMenuStrings,			0,	MAX_TRAIN_STRING_COUNT);
	ExportSection(props, L"SquadMenu",					Loc::pSquadMenuStrings,				0,	MAX_SQUAD_MENU_STRING_COUNT);

	ExportSection(props, L"SnitchMenu",					Loc::pSnitchMenuStrings,			0,	MAX_SNITCH_MENU_STRING_COUNT);
	ExportSection(props, L"SnitchMenuDesc",				Loc::pSnitchMenuDescStrings,		0,	MAX_SNITCH_MENU_STRING_COUNT-1);
	ExportSection(props, L"SnitchToggleMenu",			Loc::pSnitchToggleMenuStrings,		0,	MAX_SNITCH_TOGGLE_MENU_STRING_COUNT);
	ExportSection(props, L"SnitchToggleMenuDesc",		Loc::pSnitchToggleMenuDescStrings,	0,	MAX_SNITCH_TOGGLE_MENU_STRING_COUNT-1);
	ExportSection(props, L"SnitchSectorMenu",			Loc::pSnitchSectorMenuStrings,		0,	MAX_SNITCH_SECTOR_MENU_STRING_COUNT);
	ExportSection(props, L"SnitchSectorMenuDesc",		Loc::pSnitchSectorMenuDescStrings,	0,	MAX_SNITCH_SECTOR_MENU_STRING_COUNT-1);
	ExportSection(props, L"SnitchPrisonExposed",		Loc::pSnitchPrisonExposedStrings,	0,	NUM_SNITCH_PRISON_EXPOSED);
	ExportSection(props, L"SnitchGatheringRumoursResult",	Loc::pSnitchGatheringRumoursResultStrings,	0,	NUM_SNITCH_GATHERING_RUMOURS_RESULT);

	ExportSection(props, L"PersonnelTitle",				Loc::pPersonnelTitle,				0,	1);
	ExportSection(props, L"PersonnelScreen",			Loc::pPersonnelScreenStrings,		0,	TEXT_NUM_PRSNL);

	ExportSection(props, L"MercSkill",					Loc::gzMercSkillText,				0,	NUM_SKILLTRAITS_OT);
	ExportSection(props, L"TacticalPopupButton",		Loc::pTacticalPopupButtonStrings,	0,	NUM_ICONS);
	ExportSection(props, L"DoorTrap",					Loc::pDoorTrapStrings,				0,	NUM_DOOR_TRAPS);
	ExportSection(props, L"ContractExtend",				Loc::pContractExtendStrings,		0,	NUM_CONTRACT_EXTEND);
	ExportSection(props, L"MapScreenMouseRegionHelp",	Loc::pMapScreenMouseRegionHelpText,	0,	6);
	ExportSection(props, L"NoiseVol",					Loc::pNoiseVolStr,					0,	4);
	ExportSection(props, L"NoiseType",					Loc::pNoiseTypeStr,					0,	12);
	ExportSection(props, L"Direction",					Loc::pDirectionStr,					0,	8);
	ExportSection(props, L"LandType",					Loc::pLandTypeStrings,				0,	NUM_TRAVTERRAIN_TYPES);
	ExportSection(props, L"Strategic",					Loc::gpStrategicString,				0,	TEXT_NUM_STRATEGIC_TEXT);

	ExportSection(props, L"GameClock",					Loc::gpGameClockString,				0,	TEXT_NUM_GAMECLOCK);
	ExportSection(props, L"KeyDescription",				Loc::sKeyDescriptionStrings,		0,	2);
	ExportSection(props, L"WeaponStatsDesc",			Loc::gWeaponStatsDesc,				0,	17);	
	ExportSection(props, L"WeaponStatsFasthelpTactical",Loc::gzWeaponStatsFasthelpTactical, 0,	29);
	ExportSection(props, L"MiscItemStatsFasthelp",		Loc::gzMiscItemStatsFasthelp,		0,	34);
	ExportSection(props, L"MoneyStatsDesc",				Loc::gMoneyStatsDesc,				0,	TEXT_NUM_MONEY_DESC);

	ExportSection(props, L"Health",						Loc::zHealthStr,					0,	7);
	ExportSection(props, L"MoneyAmounts",				Loc::gzMoneyAmounts,				0,	6);
	ExportSection(props, L"ProsLabel",					Loc::gzProsLabel,					0,	1);
	ExportSection(props, L"ConsLabel",					Loc::gzConsLabel,					0,	1);
	ExportSection(props, L"TalkMenu",					Loc::zTalkMenuStrings,				0,	6);
	ExportSection(props, L"Dealer",						Loc::zDealerStrings,				0,	4);
	ExportSection(props, L"DialogActions",				Loc::zDialogActions,				0,	1);
	ExportSection(props, L"Vehicle",					Loc::pVehicleStrings,				0,	6);
	ExportSection(props, L"ShortVehicle",				Loc::pShortVehicleStrings,			0,	6);
	ExportSection(props, L"VehicleName",				Loc::zVehicleName,					0,	6);
	ExportSection(props, L"VehicleSeatsStrings",		Loc::pVehicleSeatsStrings,			0,	2);

	ExportSection(props, L"Tactical",					Loc::TacticalStr,					0,	TEXT_NUM_TACTICAL_STR);
	ExportSection(props, L"ExitingSectorHelp",			Loc::pExitingSectorHelpText,		0,	TEXT_NUM_EXIT_GUI);
	ExportSection(props, L"Repair",						Loc::pRepairStrings,				0,	4);
	ExportSection(props, L"PreStatBuild",				Loc::sPreStatBuildString,			0,	6);
	ExportSection(props, L"StatGain",					Loc::sStatGainStrings,				0,	11);
	ExportSection(props, L"HelicopterEta",				Loc::pHelicopterEtaStrings,			0,	TEXT_NUM_STR_HELI_ETA);
	ExportSection(props, L"HelicopterRepair",			Loc::pHelicopterRepairRefuelStrings,		0,	TEXT_NUM_STR_HELI_REPAIRS);
	ExportSection(props, L"MapLevel",					Loc::sMapLevelString,				0,	1);
	ExportSection(props, L"Loyal",						Loc::gsLoyalString,					0,	1);
	ExportSection(props, L"Underground",				Loc::gsUndergroundString,			0,	1);
	ExportSection(props, L"TimeStings",					Loc::gsTimeStrings,					0,	1);

	ExportSection(props, L"Facilities",					Loc::sFacilitiesStrings,			0,	7);
	ExportSection(props, L"MapPopUpInventory",			Loc::pMapPopUpInventoryText,		0,	2);
	ExportSection(props, L"TownInfo",					Loc::pwTownInfoStrings,				0,	12);
	ExportSection(props, L"Mine",						Loc::pwMineStrings,					0,	14);
	ExportSection(props, L"MiscSector",					Loc::pwMiscSectorStrings,			0,	7);
	ExportSection(props, L"MapInventoryError",			Loc::pMapInventoryErrorString,		0,	7);
	ExportSection(props, L"MapInventory",				Loc::pMapInventoryStrings,			0,	2);
	ExportSection(props, L"MapScreenFastHelp",			Loc::pMapScreenFastHelpTextList,	0,	10);
	ExportSection(props, L"MovementMenu",				Loc::pMovementMenuStrings,			0,	4);
	ExportSection(props, L"UpdateMerc",					Loc::pUpdateMercStrings,			0,	6);

	ExportSection(props, L"MapScreenBorderButtonHelp",	Loc::pMapScreenBorderButtonHelpText,0,	6);
	ExportSection(props, L"MapScreenBottomFastHelp",	Loc::pMapScreenBottomFastHelp,		0,	8);
	ExportSection(props, L"MapScreenBottom",			Loc::pMapScreenBottomText,			0,	1);
	ExportSection(props, L"MercDead",					Loc::pMercDeadString,				0,	1);
	ExportSection(props, L"Day",						Loc::pDayStrings,					0,	1);
	ExportSection(props, L"SenderName",					Loc::pSenderNameList,				0,	51);
	ExportSection(props, L"Traverse",					Loc::pTraverseStrings,				0,	2);
	ExportSection(props, L"NewMail",					Loc::pNewMailStrings,				0,	1);
	ExportSection(props, L"DeleteMail",					Loc::pDeleteMailStrings,			0,	2);
	ExportSection(props, L"EmailHeader",				Loc::pEmailHeaders,					0,	3);

	ExportSection(props, L"EmailTitle",					Loc::pEmailTitleText,				0,	1);
	ExportSection(props, L"FinanceTitle",				Loc::pFinanceTitle,					0,	1);
	ExportSection(props, L"FinanceSummary",				Loc::pFinanceSummary,				0,	12);
	ExportSection(props, L"FinanceHeader",				Loc::pFinanceHeaders,				0,	7);
	ExportSection(props, L"Transaction",				Loc::pTransactionText,				0,	TEXT_NUM_FINCANCES);
	ExportSection(props, L"TransactionAlternate",		Loc::pTransactionAlternateText,		0,	4);
	ExportSection(props, L"Skyrider",					Loc::pSkyriderText,					0,	7);
	ExportSection(props, L"Moral",						Loc::pMoralStrings,					0,	6);
	ExportSection(props, L"LeftEquipment",				Loc::pLeftEquipmentString,			0,	2);
	ExportSection(props, L"MapScreenStatus",			Loc::pMapScreenStatusStrings,		0,	5);

	ExportSection(props, L"MapScreenPrevNextCharButtonHelp",	Loc::pMapScreenPrevNextCharButtonHelpText,	0,	2);
	ExportSection(props, L"Eta",								Loc::pEtaString,							0,	1);
	ExportSection(props, L"TrashItem",							Loc::pTrashItemText,						0,	2);
	ExportSection(props, L"MapError",							Loc::pMapErrorString,						0,	50);
	ExportSection(props, L"MapPlot",							Loc::pMapPlotStrings,						0,	5);
	ExportSection(props, L"Bullseye",							Loc::pBullseyeStrings,						0,	5);
	ExportSection(props, L"MiscMapScreenMouseRegionHelp",		Loc::pMiscMapScreenMouseRegionHelpText,		0,	3);
	ExportSection(props, L"MercHeLeave",						Loc::pMercHeLeaveString,					0,	5);
	ExportSection(props, L"MercSheLeave",						Loc::pMercSheLeaveString,					0,	5);
	ExportSection(props, L"MercContractOver",					Loc::pMercContractOverStrings,				0,	5);

	ExportSection(props, L"ImpPopUp",					Loc::pImpPopUpStrings,				0,	12);
	ExportSection(props, L"ImpButton",					Loc::pImpButtonText,				0,	26);
	ExportSection(props, L"ExtraIMP",					Loc::pExtraIMPStrings,				0,	4);
	ExportSection(props, L"FilesTitle",					Loc::pFilesTitle,					0,	1);
	ExportSection(props, L"FilesSender",				Loc::pFilesSenderList,				0,	7);
	ExportSection(props, L"HistoryTitle",				Loc::pHistoryTitle,					0,	1);
	ExportSection(props, L"HistoryHeader",				Loc::pHistoryHeaders,				0,	5);
	ExportSection(props, L"History",					Loc::pHistoryStrings,				0,	TEXT_NUM_HISTORY);
	ExportSection(props, L"HistoryLocation",			Loc::pHistoryLocations,				0,	1);
	ExportSection(props, L"LaptopIcon",					Loc::pLaptopIcons,					0,	8);

	ExportSection(props, L"BookMark",					Loc::pBookMarkStrings,				0,	TEXT_NUM_LAPTOP_BOOKMARKS);
	ExportSection(props, L"BookmarkTitle",				Loc::pBookmarkTitle,				0,	2);
	ExportSection(props, L"Download",					Loc::pDownloadString,				0,	2);
	ExportSection(props, L"AtmSideButton",				Loc::gsAtmSideButtonText,			0,	5);
	ExportSection(props, L"AtmStartButton",				Loc::gsAtmStartButtonText,			0,	4);
	ExportSection(props, L"ATM",						Loc::sATMText,						0,	6);
	ExportSection(props, L"Error",						Loc::pErrorStrings,					0,	5);
	ExportSection(props, L"Personnel",					Loc::pPersonnelString,				0,	1);
	ExportSection(props, L"WebTitle",					Loc::pWebTitle,						0,	1);
	ExportSection(props, L"WebPagesTitle",				Loc::pWebPagesTitles,				0,	36);

	ExportSection(props, L"ShowBookmark",				Loc::pShowBookmarkString,				0,	2);
	ExportSection(props, L"LaptopTitle",				Loc::pLaptopTitles,						0,	5);
	ExportSection(props, L"PersonnelDepartedState",		Loc::pPersonnelDepartedStateStrings,	0,	TEXT_NUM_DEPARTED);
	ExportSection(props, L"PersonelTeam",				Loc::pPersonelTeamStrings,				0,	8);
	ExportSection(props, L"PersonnelCurrentTeamStats",	Loc::pPersonnelCurrentTeamStatsStrings, 0,	3);
	ExportSection(props, L"PersonnelTeamStats",			Loc::pPersonnelTeamStatsStrings,		0,	11);
	ExportSection(props, L"MapVertIndex",				Loc::pMapVertIndex,						0,	17);
	ExportSection(props, L"MapHortIndex",				Loc::pMapHortIndex,						0,	17);
	ExportSection(props, L"MapDepthIndex",				Loc::pMapDepthIndex,					0,	4);
	ExportSection(props, L"ContractButton",				Loc::pContractButtonString,				0,	1);

	ExportSection(props, L"UpdatePanelButton",			Loc::pUpdatePanelButtons,			0,	2);
	ExportSection(props, L"LargeTactical",				Loc::LargeTacticalStr,				0,	TEXT_NUM_LARGESTR);
	ExportSection(props, L"InsContract",				Loc::InsContractText,				0,	TEXT_NUM_INS_CONTRACT);
	ExportSection(props, L"InsInfo",					Loc::InsInfoText,					0,	TEXT_NUM_INS_INFO);
	ExportSection(props, L"MercAccount",				Loc::MercAccountText,				0,	TEXT_NUM_MERC_ACCOUNT);
	ExportSection(props, L"MercAccountPage",			Loc::MercAccountPageText,			0,	2);
	ExportSection(props, L"MercInfo",					Loc::MercInfo,						0,	TEXT_NUM_MERC_FILES);
	ExportSection(props, L"MercNoAccount",				Loc::MercNoAccountText,				0,	TEXT_NUM_MERC_NO_ACC);
	ExportSection(props, L"MercHomePage",				Loc::MercHomePageText,				0,	TEXT_NUM_MERC);
	ExportSection(props, L"Funeral",					Loc::sFuneralString,				0,	TEXT_NUM_FUNERAL);

	ExportSection(props, L"Florist",					Loc::sFloristText,					0,	TEXT_NUM_FLORIST);
	ExportSection(props, L"OrderForm",					Loc::sOrderFormText,				0,	TEXT_NUM_FLORIST_ORDER);
	ExportSection(props, L"FloristGallery",				Loc::sFloristGalleryText,			0,	TEXT_NUM_FLORIST_GALLERY);
	ExportSection(props, L"FloristCards",				Loc::sFloristCards,					0,	TEXT_NUM_FLORIST_CARDS);
	ExportSection(props, L"BobbyROrderForm",			Loc::BobbyROrderFormText,			0,	TEXT_NUM_BOBBYR_MAILORDER);
	ExportSection(props, L"BobbyRFilter",				Loc::BobbyRFilter,					0,	TEXT_NUM_BOBBYR_FILTER);
	ExportSection(props, L"BobbyR",						Loc::BobbyRText,					0,	TEXT_NUM_BOBBYR_GUNS);
	ExportSection(props, L"BobbyRaysFront",				Loc::BobbyRaysFrontText,			0,	TEXT_NUM_BOBBYR);
	ExportSection(props, L"AimSort", Loc::AimSortText, 0, TEXT_NUM_AIM_SORT);
	ExportSection(props, L"AimPolicy", Loc::AimPolicyText, 0, TEXT_NUM_AIM_POLICIES);

	ExportSection(props, L"AimMember",					Loc::AimMemberText,					0,	4);
	ExportSection(props, L"CharacterInfo",				Loc::CharacterInfo,					0,	TEXT_NUM_AIM_MEMBER_CHARINFO);
	ExportSection(props, L"VideoConfercing",			Loc::VideoConfercingText,			0,	TEXT_NUM_AIM_MEMBER_VCONF);
	ExportSection(props, L"AimPopUp",					Loc::AimPopUpText,					0,	TEXT_NUM_AIM_MEMBER_POPUP);
	ExportSection(props, L"AimLink",					Loc::AimLinkText,					0,	TEXM_NUM_AIM_LINK);
	ExportSection(props, L"AimHistory",					Loc::AimHistoryText,				0,	TEXT_NUM_AIM_HISTORY);
	ExportSection(props, L"AimFi",						Loc::AimFiText,						0,	TEXT_NUM_AIM_FI);
	ExportSection(props, L"AimAlumni",					Loc::AimAlumniText,					0,	TEXT_NUM_AIM_ALUMNI);
	ExportSection(props, L"AimScreen",					Loc::AimScreenText,					0,	TEXT_NUM_AIM_SCREEN);
	ExportSection(props, L"AimBottomMenu",				Loc::AimBottomMenuText,				0,	TEXT_NUM_AIM_MENU);

	ExportSection(props, L"SKI",						Loc::SKI_Text,						0, TEXT_NUM_SKI_TEXT);
	ExportSection(props, L"SkiAtm",						Loc::SkiAtmText,					0, NUM_SKI_ATM_BUTTONS);
	ExportSection(props, L"SkiAtmText",					Loc::gzSkiAtmText,					0, TEXT_NUM_SKI_ATM_MODE_TEXT);
	ExportSection(props, L"SkiMessageBox",				Loc::SkiMessageBoxText,				0, TEXT_NUM_SKI_MBOX_TEXT);
	ExportSection(props, L"Options",					Loc::zOptionsText,					0, TEXT_NUM_OPT_TEXT);
	ExportSection(props, L"SaveLoad",					Loc::zSaveLoadText,					0, TEXT_NUM_SLG_TEXT);
	ExportSection(props, L"MarksMapScreen",				Loc::zMarksMapScreenText,			0, 25);
	ExportSection(props, L"LandMarkInSector",			Loc::pLandMarkInSectorString,		0, 1);
	ExportSection(props, L"MilitiaConfirm",				Loc::pMilitiaConfirmStrings,		0, 11);
	ExportSection(props, L"MoneyWithdrawMessage",		Loc::gzMoneyWithdrawMessageText,	0, TEXT_NUM_MONEY_WITHDRAW);

	ExportSection(props, L"Copyright",					Loc::gzCopyrightText,				0,	1);
	ExportSection(props, L"OptionsToggle",				Loc::zOptionsToggleText,			0,	49);
	ExportSection(props, L"OptionsScreenHelp",			Loc::zOptionsScreenHelpText,		0,	49);
	ExportSection(props, L"GIOScreen",					Loc::gzGIOScreenText,				0,	TEXT_NUM_GIO_TEXT);
	ExportSection(props, L"MPJScreen",					Loc::gzMPJScreenText,				0,	TEXT_NUM_MPJ_TEXT);
	ExportSection(props, L"MPJHelpText",				Loc::gzMPJHelpText,					0,	10);
	ExportSection(props, L"MPHScreen",					Loc::gzMPHScreenText,				0,	TEXT_NUM_MPH_TEXT);	
	ExportSection(props, L"DeliveryLocation",			Loc::pDeliveryLocationStrings,		0,	17);
	ExportSection(props, L"SkillAtZeroWarning",			Loc::pSkillAtZeroWarning,			0,	1);
	ExportSection(props, L"IMPBeginScreen",				Loc::pIMPBeginScreenStrings,		0,	1);
	ExportSection(props, L"IMPFinishButton",			Loc::pIMPFinishButtonText,			0,	1);

	ExportSection(props, L"IMPFinish",					Loc::pIMPFinishStrings,				0,	1);
	ExportSection(props, L"IMPVoices",					Loc::pIMPVoicesStrings,				0,	1);
	ExportSection(props, L"DepartedMercPortrait",		Loc::pDepartedMercPortraitStrings,	0,	3);
	ExportSection(props, L"PersTitle",					Loc::pPersTitleText,				0,	1);
	ExportSection(props, L"PausedGame",					Loc::pPausedGameText,				0,	3);
	ExportSection(props, L"MessageStrings",				Loc::pMessageStrings,				0,	TEXT_NUM_MSG);
	ExportSection(props, L"ItemPickupHelpPopup",		Loc::ItemPickupHelpPopup,			0,	5);
	ExportSection(props, L"DoctorWarning",				Loc::pDoctorWarningString,			0,	2);
	ExportSection(props, L"MilitiaButtonsHelp",			Loc::pMilitiaButtonsHelpText,		0,	4);
	ExportSection(props, L"MapScreenJustStartedHelp",	Loc::pMapScreenJustStartedHelpText,	0,	2);

	ExportSection(props, L"AntiHacker",					Loc::pAntiHackerString,				0,	TEXT_NUM_ANTIHACKERSTR);
	ExportSection(props, L"LaptopHelp",					Loc::gzLaptopHelpText,				0,	TEXT_NUM_LAPTOP_BN_BOOKMARK_TEXT);
	ExportSection(props, L"HelpScreen",					Loc::gzHelpScreenText,				0,	TEXT_NUM_HLP);
	ExportSection(props, L"NonPersistantPBI",			Loc::gzNonPersistantPBIText,		0,	10);
	ExportSection(props, L"MiscString",					Loc::gzMiscString,					0,	5);
	ExportSection(props, L"IntroScreen",				Loc::gzIntroScreen,					0,	1);
	ExportSection(props, L"NewNoise",					Loc::pNewNoiseStr,					0,	11/*MAX_NOISES*/);
	ExportSection(props, L"MapScreenSortButtonHelp",	Loc::wMapScreenSortButtonHelpText,	0,	6);
	ExportSection(props, L"BrokenLink",					Loc::BrokenLinkText,				0,	TEXT_NUM_BROKEN_LINK);
	ExportSection(props, L"BobbyRShipment",				Loc::gzBobbyRShipmentText,			0,	TEXT_NUM_BOBBYR_SHIPMENT);

	ExportSection(props, L"CreditNames",				Loc::gzCreditNames,					0,	15);
	ExportSection(props, L"CreditNameTitle",			Loc::gzCreditNameTitle,				0,	15);
	ExportSection(props, L"CreditNameFunny",			Loc::gzCreditNameFunny,				0,	15);
	ExportSection(props, L"RepairsDone",				Loc::sRepairsDoneString,			0,	7);
	ExportSection(props, L"GioDifConfirm",				Loc::zGioDifConfirmText,			0,	TEXT_NUM_GIO_CFS);
	ExportSection(props, L"LateLocalized",				Loc::gzLateLocalizedString,			0,	64);
	ExportSection(props, L"CWStrings",					Loc::gzCWStrings,					0,	1);
	ExportSection(props, L"TooltipStrings",				Loc::gzTooltipStrings,				0,	TEXT_NUM_STR_TT);
	ExportSection(props, L"New113Message",				Loc::New113Message,					0,	TEXT_NUM_MSG113);

	ExportSection(props, L"New113HAMMessage",			Loc::New113HAMMessage,				0,	25);
	ExportSection(props, L"New113MERCMercMail",			Loc::New113MERCMercMailTexts,		0,	4);
	ExportSection(props, L"New113AIMMercMail",			Loc::New113AIMMercMailTexts,		0,	16);
	ExportSection(props, L"MissingIMPSkills",			Loc::MissingIMPSkillsDescriptions,	0,	2);
	ExportSection(props, L"NewInvMessage",				Loc::NewInvMessage,					0,	TEXT_NUM_NIV);
	ExportSection(props, L"MPServerMessage",			Loc::MPServerMessage,				0,	13);
	ExportSection(props, L"MPClientMessage",			Loc::MPClientMessage,				0,	69);	
	ExportSection(props, L"MPEdges",					Loc::gszMPEdgesText,				0,	5);
	ExportSection(props, L"MPTeamName",					Loc::gszMPTeamNames,				0,	5);
	ExportSection(props, L"MPMapscreen",				Loc::gszMPMapscreenText,			0,	9);

	ExportSection(props, L"MPSScreen",					Loc::gzMPSScreenText,				0,	TEXT_NUM_MPS_TEXT);
	ExportSection(props, L"MPCScreen",					Loc::gzMPCScreenText,				0,	TEXT_NUM_MPC_TEXT);
	ExportSection(props, L"MPChatToggle",				Loc::gzMPChatToggleText,			0,	2);
	ExportSection(props, L"MPChatbox",					Loc::gzMPChatboxText,				0,	2);

	props.writeToXMLFile(L"Localization/GameStrings.xml",tmap);
	props.writeToIniFile(L"Localization/GameStrings.ini",true);

	Loc::ExportMercBio();
	Loc::ExportAIMHistory();
	Loc::ExportAIMPolicy();
	Loc::ExportAlumniName();
	Loc::ExportDialogues();
	Loc::ExportNPCDialogues();

	return true;
}

#include <vfs/Core/vfs_file_raii.h>
#include "Encrypted File.h"

namespace Loc
{
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
	bool Translate(vfs::String::char_t* str, int len, Language lang)
	{
		if(lang == English || lang == German)
		{
			return true;
		}
		else if(lang == Russian)
		{
			for(int i=0; i<len; i++) str[i] = ToRussian(str[i]);
			return true;
		}
		else if(lang == Polish)
		{
			for(int i=0; i<len; i++) str[i] = ToPolish(str[i]);
			return true;
		}
		return false;
	}
}; // namespace Loc

void Loc::ExportMercBio()
{
	Loc::Language lang = gs_Lang;
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
		Loc::Translate(pInfoString, SIZE_MERC_BIO_INFO, lang);
		props.setStringProperty(L"Bio", vfs::toString<wchar_t>(i), pInfoString);
		
		file.read((vfs::Byte*)pAddInfo, SIZE_MERC_ADDITIONAL_INFO);
		DecodeString(pAddInfo, SIZE_MERC_ADDITIONAL_INFO);
		Loc::Translate(pAddInfo, SIZE_MERC_ADDITIONAL_INFO, lang);
		props.setStringProperty(L"Add", vfs::toString<wchar_t>(i), pAddInfo);
	}
	props.writeToXMLFile(L"Localization/AimBiographies.xml", vfs::PropertyContainer::TagMap());
}

void Loc::ExportAIMHistory()
{
	Loc::Language lang = gs_Lang;
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
		Loc::Translate(pHistLine, AIM_HISTORY_LINE_SIZE, lang);
		props.setStringProperty(L"Line", vfs::toString<wchar_t>(i), pHistLine);
	}
	props.writeToXMLFile(L"Localization/AimHistory.xml", vfs::PropertyContainer::TagMap());
}
	
	
void Loc::ExportAIMPolicy()
{
	Loc::Language lang = gs_Lang;
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
		Loc::Translate(pPolLine, AIM_HISTORY_LINE_SIZE, lang);
		props.setStringProperty(L"Line", vfs::toString<wchar_t>(i), pPolLine);
	}
	props.writeToXMLFile(L"Localization/AimPolicy.xml", vfs::PropertyContainer::TagMap());
}

void Loc::ExportAlumniName()
{
	Loc::Language lang = gs_Lang;
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
		Loc::Translate(pAlumniName, AIM_ALUMNI_NAME_SIZE, lang);
		props.setStringProperty(L"Line", vfs::toString<wchar_t>(i), pAlumniName);
	}
	props.writeToXMLFile(L"Localization/AlumniName.xml", vfs::PropertyContainer::TagMap());
}

#include <vfs/Core/vfs.h>

void Loc::ExportDialogues()
{
	Loc::Language lang = gs_Lang;
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
				Loc::Translate(pDiagLine, DIALOGUESIZE, lang);
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

void Loc::ExportNPCDialogues()
{
	Loc::Language lang = gs_Lang;
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
				Loc::Translate(pDiagLine, SIZE, lang);
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
