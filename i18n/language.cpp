#include <language.hpp>
#include "DEBUG.H"

namespace {
constexpr auto RowsForLang(i18n::Lang lang) -> int {
  return lang == i18n::Lang::zh ? 6 : 9;
}
constexpr i18n::Lang kBuildDefaultLang = i18n::Lang::en;
constexpr int kBuildDefaultMaxMessagesOnMapBottom = RowsForLang(kBuildDefaultLang);
} // namespace

i18n::Lang g_lang = kBuildDefaultLang;
int MAX_MESSAGES_ON_MAP_BOTTOM = kBuildDefaultMaxMessagesOnMapBottom;

auto ApplyLang(i18n::Lang lang = kBuildDefaultLang ) {
  MAX_MESSAGES_ON_MAP_BOTTOM = RowsForLang(lang);
  g_lang = lang;
}

auto SetLanguageFromName(std::string const& name) -> void {
  static const struct { const STR name; i18n::Lang lang; } table[] = {
    { "ENGLISH", i18n::Lang::en },
    { "GERMAN",  i18n::Lang::de },
    { "RUSSIAN", i18n::Lang::ru },
    { "DUTCH",   i18n::Lang::nl },
    { "POLISH",  i18n::Lang::pl },
    { "FRENCH",  i18n::Lang::fr },
    { "ITALIAN", i18n::Lang::it },
    { "CHINESE", i18n::Lang::zh },
  };

  for (auto const& entry : table) {
    if (_stricmp(name.c_str(), entry.name) == 0) {
      ApplyLang(entry.lang);
      return;
    }
  }

  ApplyLang();
  FastDebugMsg(String("SetLanguageFromName: unknown LANGUAGE value '%s', keeping build default", name.c_str()));
}

auto GetLanguagePrefix() -> const STR {
  switch (g_lang) {
    case i18n::Lang::en: return "";
    case i18n::Lang::de: return "German.";
    case i18n::Lang::ru: return "Russian.";
    case i18n::Lang::nl: return "Dutch.";
    case i18n::Lang::pl: return "Polish.";
    case i18n::Lang::fr: return "French.";
    case i18n::Lang::it: return "Italian.";
    case i18n::Lang::zh: return "Chinese.";
  }
  return "";
}
