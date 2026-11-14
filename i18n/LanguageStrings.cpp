#include "Text.h"
#include "Item Statistics.h"

#include <language.hpp>

#include <iterator>
#include <type_traits>

namespace lang_en {
#include "_EnglishText.cpp"
#include "_Ja25EnglishText.cpp"
}

namespace lang_de {
#include "_GermanText.cpp"
#include "_Ja25GermanText.cpp"
}

namespace lang_ru {
#include "_RussianText.cpp"
#include "_Ja25RussianText.cpp"
}

namespace lang_nl {
#include "_DutchText.cpp"
#include "_Ja25DutchText.cpp"
}

namespace lang_pl {
#include "_PolishText.cpp"
#include "_Ja25PolishText.cpp"
}

namespace lang_fr {
#include "_FrenchText.cpp"
#include "_Ja25FrenchText.cpp"
}

namespace lang_it {
#include "_ItalianText.cpp"
#include "_Ja25ItalianText.cpp"
}

namespace lang_zh {
#include "_ChineseText.cpp"
#include "_Ja25ChineseText.cpp"
}

namespace lang_default = lang_en;

// definitions (default language)
#define X(NAME)                                                                \
  std::decay_t<decltype(lang_default::NAME)> NAME = lang_default::NAME;
#include "text.def"
#undef X

#define TRANSLATION_TABLE_MATCHES_ENGLISH(LANG, NAME)                         \
  static_assert(std::is_same_v<decltype(lang_en::NAME), decltype(LANG::NAME)>, \
                "Translation table size differs from English table: " #LANG "::" #NAME);

#define X(NAME) TRANSLATION_TABLE_MATCHES_ENGLISH(lang_de, NAME)
#include "text.def"
#undef X
#define X(NAME) TRANSLATION_TABLE_MATCHES_ENGLISH(lang_ru, NAME)
#include "text.def"
#undef X
#define X(NAME) TRANSLATION_TABLE_MATCHES_ENGLISH(lang_nl, NAME)
#include "text.def"
#undef X
#define X(NAME) TRANSLATION_TABLE_MATCHES_ENGLISH(lang_pl, NAME)
#include "text.def"
#undef X
#define X(NAME) TRANSLATION_TABLE_MATCHES_ENGLISH(lang_fr, NAME)
#include "text.def"
#undef X
#define X(NAME) TRANSLATION_TABLE_MATCHES_ENGLISH(lang_it, NAME)
#include "text.def"
#undef X
#define X(NAME) TRANSLATION_TABLE_MATCHES_ENGLISH(lang_zh, NAME)
#include "text.def"
#undef X
#undef TRANSLATION_TABLE_MATCHES_ENGLISH

#define ENGLISH_TABLE_MATCHES_ENUM(NAME, COUNT)                                  \
  static_assert(std::size(lang_en::NAME) == COUNT,                              \
                "English table size differs from its enum: " #NAME " vs " #COUNT);

ENGLISH_TABLE_MATCHES_ENUM(AimAlumniText, TEXT_NUM_AIM_ALUMNI)
ENGLISH_TABLE_MATCHES_ENUM(AimBottomMenuText, TEXT_NUM_AIM_MENU)
ENGLISH_TABLE_MATCHES_ENUM(AimFiText, TEXT_NUM_AIM_FI)
ENGLISH_TABLE_MATCHES_ENUM(AimHistoryText, TEXT_NUM_AIM_HISTORY)
ENGLISH_TABLE_MATCHES_ENUM(AimLinkText, TEXM_NUM_AIM_LINK)
ENGLISH_TABLE_MATCHES_ENUM(AimMemberText, TEXT_NUM_AIM_MEMBER_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(AimPopUpText, TEXT_NUM_AIM_MEMBER_POPUP)
ENGLISH_TABLE_MATCHES_ENUM(AimScreenText, TEXT_NUM_AIM_SCREEN)
ENGLISH_TABLE_MATCHES_ENUM(AimSortText, TEXT_NUM_AIM_SORT)
ENGLISH_TABLE_MATCHES_ENUM(BobbyRFilter, TEXT_NUM_BOBBYR_FILTER)
ENGLISH_TABLE_MATCHES_ENUM(BobbyROrderFormText, TEXT_NUM_BOBBYR_MAILORDER)
ENGLISH_TABLE_MATCHES_ENUM(BobbyRText, TEXT_NUM_BOBBYR_GUNS)
ENGLISH_TABLE_MATCHES_ENUM(BobbyRaysFrontText, TEXT_NUM_BOBBYR)
ENGLISH_TABLE_MATCHES_ENUM(BrokenLinkText, TEXT_NUM_BROKEN_LINK)
ENGLISH_TABLE_MATCHES_ENUM(CharacterInfo, TEXT_NUM_AIM_MEMBER_CHARINFO)
ENGLISH_TABLE_MATCHES_ENUM(InsContractText, TEXT_NUM_INS_CONTRACT)
ENGLISH_TABLE_MATCHES_ENUM(InsInfoText, TEXT_NUM_INS_INFO)
ENGLISH_TABLE_MATCHES_ENUM(LargeTacticalStr, TEXT_NUM_LARGESTR)
ENGLISH_TABLE_MATCHES_ENUM(MercAccountText, TEXT_NUM_MERC_ACCOUNT)
ENGLISH_TABLE_MATCHES_ENUM(MercHomePageText, TEXT_NUM_MERC)
ENGLISH_TABLE_MATCHES_ENUM(MercInfo, TEXT_NUM_MERC_FILES)
ENGLISH_TABLE_MATCHES_ENUM(MercNoAccountText, TEXT_NUM_MERC_NO_ACC)
ENGLISH_TABLE_MATCHES_ENUM(Message, TEXT_NUM_STR_MESSAGE)
ENGLISH_TABLE_MATCHES_ENUM(New113Message, TEXT_NUM_MSG113)
ENGLISH_TABLE_MATCHES_ENUM(NewInvMessage, TEXT_NUM_NIV)
ENGLISH_TABLE_MATCHES_ENUM(SKI_Text, TEXT_NUM_SKI_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(SkiMessageBoxText, TEXT_NUM_SKI_MBOX_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(TacticalStr, TEXT_NUM_TACTICAL_STR)
ENGLISH_TABLE_MATCHES_ENUM(VideoConfercingText, TEXT_NUM_AIM_MEMBER_VCONF)
ENGLISH_TABLE_MATCHES_ENUM(gMoneyStatsDesc, TEXT_NUM_MONEY_DESC)
ENGLISH_TABLE_MATCHES_ENUM(gpGameClockString, TEXT_NUM_GAMECLOCK)
ENGLISH_TABLE_MATCHES_ENUM(gpStrategicString, TEXT_NUM_STRATEGIC_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzBobbyRShipmentText, TEXT_NUM_BOBBYR_SHIPMENT)
ENGLISH_TABLE_MATCHES_ENUM(gzGIOScreenText, TEXT_NUM_GIO_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzHelpScreenText, TEXT_NUM_HLP)
ENGLISH_TABLE_MATCHES_ENUM(gzLaptopHelpText, TEXT_NUM_LAPTOP_BN_BOOKMARK_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzMPCScreenText, TEXT_NUM_MPC_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzMPHScreenText, TEXT_NUM_MPH_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzMPJScreenText, TEXT_NUM_MPJ_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzMPSScreenText, TEXT_NUM_MPS_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzMoneyWithdrawMessageText, TEXT_NUM_MONEY_WITHDRAW)
ENGLISH_TABLE_MATCHES_ENUM(gzSkiAtmText, TEXT_NUM_SKI_ATM_MODE_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(gzTooltipStrings, TEXT_NUM_STR_TT)
ENGLISH_TABLE_MATCHES_ENUM(pAntiHackerString, TEXT_NUM_ANTIHACKERSTR)
ENGLISH_TABLE_MATCHES_ENUM(pExitingSectorHelpText, TEXT_NUM_EXIT_GUI)
ENGLISH_TABLE_MATCHES_ENUM(pHelicopterEtaStrings, TEXT_NUM_STR_HELI_ETA)
ENGLISH_TABLE_MATCHES_ENUM(pHelicopterRepairRefuelStrings, TEXT_NUM_STR_HELI_REPAIRS)
ENGLISH_TABLE_MATCHES_ENUM(pMessageStrings, TEXT_NUM_MSG)
ENGLISH_TABLE_MATCHES_ENUM(pTraitSkillsDenialStrings, TEXT_SKILL_DENIAL_MAX)
ENGLISH_TABLE_MATCHES_ENUM(sFloristCards, TEXT_NUM_FLORIST_CARDS)
ENGLISH_TABLE_MATCHES_ENUM(sFloristGalleryText, TEXT_NUM_FLORIST_GALLERY)
ENGLISH_TABLE_MATCHES_ENUM(sFloristText, TEXT_NUM_FLORIST)
ENGLISH_TABLE_MATCHES_ENUM(sFuneralString, TEXT_NUM_FUNERAL)
ENGLISH_TABLE_MATCHES_ENUM(sOrderFormText, TEXT_NUM_FLORIST_ORDER)
ENGLISH_TABLE_MATCHES_ENUM(szCampaignHistoryDetail, TEXT_CAMPAIGNHISTORY_DETAIL_MAX)
ENGLISH_TABLE_MATCHES_ENUM(szCampaignHistoryWebSite, TEXT_CAMPAIGNHISTORY_MAX)
ENGLISH_TABLE_MATCHES_ENUM(szCorpseTextStr, TEXT_NUM_CORPSE_STR)
ENGLISH_TABLE_MATCHES_ENUM(szCovertTextStr, TEXT_NUM_COVERT_STR)
ENGLISH_TABLE_MATCHES_ENUM(szFoodTextStr, TEXT_NUM_FOOD_STR)
ENGLISH_TABLE_MATCHES_ENUM(szInventoryArmTextStr, TEXT_NUM_INV_ARM_STR)
ENGLISH_TABLE_MATCHES_ENUM(szMTATextStr, TEXT_NUM_MTA_STR)
ENGLISH_TABLE_MATCHES_ENUM(szMercCompareWebSite, TEXT_MERCCOMPARE_MAX)
ENGLISH_TABLE_MATCHES_ENUM(szMilitiaWebSite, TEXT_MILITIAWEBSITE_MAX)
ENGLISH_TABLE_MATCHES_ENUM(szPMCWebSite, TEXT_PMC_MAX)
ENGLISH_TABLE_MATCHES_ENUM(szPersonalityDisplayText, PERSONALITYTEXT_MAX)
ENGLISH_TABLE_MATCHES_ENUM(szPrisonerTextStr, TEXT_NUM_PRISONER_STR)
ENGLISH_TABLE_MATCHES_ENUM(szWHOWebSite, TEXT_WHO_MAX)
ENGLISH_TABLE_MATCHES_ENUM(zOptionsText, TEXT_NUM_OPT_TEXT)
ENGLISH_TABLE_MATCHES_ENUM(zSaveLoadText, TEXT_NUM_SLG_TEXT)
#undef ENGLISH_TABLE_MATCHES_ENUM


namespace {
auto bind_en() -> void {
#define X(NAME) NAME = lang_en::NAME;
#include "text.def"
#undef X
}
auto bind_de() -> void {
#define X(NAME) NAME = lang_de::NAME;
#include "text.def"
#undef X
}
auto bind_ru() -> void {
#define X(NAME) NAME = lang_ru::NAME;
#include "text.def"
#undef X
}
auto bind_nl() -> void {
#define X(NAME) NAME = lang_nl::NAME;
#include "text.def"
#undef X
}
auto bind_pl() -> void {
#define X(NAME) NAME = lang_pl::NAME;
#include "text.def"
#undef X
}
auto bind_fr() -> void {
#define X(NAME) NAME = lang_fr::NAME;
#include "text.def"
#undef X
}
auto bind_it() -> void {
#define X(NAME) NAME = lang_it::NAME;
#include "text.def"
#undef X
}
auto bind_zh() -> void {
#define X(NAME) NAME = lang_zh::NAME;
#include "text.def"
#undef X
}
} // namespace

auto BindLanguageStrings(i18n::Lang lang) -> void {
  switch (lang) {
  case i18n::Lang::en:
    bind_en();
    break;
  case i18n::Lang::de:
    bind_de();
    break;
  case i18n::Lang::ru:
    bind_ru();
    break;
  case i18n::Lang::nl:
    bind_nl();
    break;
  case i18n::Lang::pl:
    bind_pl();
    break;
  case i18n::Lang::fr:
    bind_fr();
    break;
  case i18n::Lang::it:
    bind_it();
    break;
  case i18n::Lang::zh:
    bind_zh();
    break;
  }
}
