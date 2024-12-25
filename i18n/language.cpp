#include <language.hpp>

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
