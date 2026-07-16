#include <language.hpp>
#include "DEBUG.H"

namespace {
// Same #ifdef chain as before C1 (kept as the per-exe default until Cn+4
// collapses the CMake language axis). The ini key (below) overrides it.
constexpr i18n::Lang kBuildDefaultLang =
#if defined(ENGLISH)
  i18n::Lang::en;
#elif defined(CHINESE)
  i18n::Lang::zh;
#elif defined(DUTCH)
  i18n::Lang::nl;
#elif defined(FRENCH)
  i18n::Lang::fr;
#elif defined(GERMAN)
  i18n::Lang::de;
#elif defined(ITALIAN)
  i18n::Lang::it;
#elif defined(POLISH)
  i18n::Lang::pl;
#elif defined(RUSSIAN)
  i18n::Lang::ru;
#endif

auto RowsForLang(i18n::Lang lang) -> int {
  return lang == i18n::Lang::zh ? 6 : 9;
}
}

/* g_lang used to be a compile-time const picked by the ENGLISH/GERMAN/...
 * define; it is now a runtime value that starts at the build's default and
 * can be overridden at startup from the [Ja2 Settings] LANGUAGE ini key (see
 * SetLanguageFromName / sgp.cpp GetRuntimeSettings). */
i18n::Lang g_lang = kBuildDefaultLang;

int MAX_MESSAGES_ON_MAP_BOTTOM = RowsForLang(g_lang);

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
      g_lang = entry.lang;
      MAX_MESSAGES_ON_MAP_BOTTOM = RowsForLang(g_lang);
      return;
    }
  }

  g_lang = kBuildDefaultLang;
  MAX_MESSAGES_ON_MAP_BOTTOM = RowsForLang(g_lang);
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
