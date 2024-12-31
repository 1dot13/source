#include <language.hpp>

#include <BobbyR.h>
#include <laptop.h>

/* FIXME: The ugliest of ugly hacks. Getting rid of this and letting language
 * (ideally text and voice separately) be set in the options menu would be
 * ideal. */
const i18n::Lang g_lang{
  #if defined(ENGLISH)
  i18n::Lang::en
#elif defined(CHINESE)
  i18n::Lang::zh
#elif defined(DUTCH)
  i18n::Lang::nl
#elif defined(FRENCH)
  i18n::Lang::fr
#elif defined(GERMAN)
  i18n::Lang::de
#elif defined(ITALIAN)
  i18n::Lang::it
#elif defined(POLISH)
  i18n::Lang::pl
#elif defined(RUSSIAN)
  i18n::Lang::ru
#endif
};

const int MAX_MESSAGES_ON_MAP_BOTTOM{
#if defined(CHINESE) // zwwoooooo: Chinese fonts relatively high , so to reduce the number of rows
    6
#else
    9
#endif
};

const int BOBBYR_ITEMS_BOUGHT_X{BOBBYR_GRIDLOC_X + 105 - BOBBYR_ORDER_NUM_WIDTH
#ifdef CHINESE
  - 10
#endif
};

auto GetLanguagePrefix() -> const STR {
  return
#if defined(ENGLISH)
  ""
#elif defined(CHINESE)
  "Chinese."
#elif defined(DUTCH)
  "Dutch."
#elif defined(FRENCH)
  "French."
#elif defined(GERMAN)
  "German."
#elif defined(ITALIAN)
  "Italian."
#elif defined(POLISH)
  "Polish."
#elif defined(RUSSIAN)
  "Russian."
#endif
  ;
}
