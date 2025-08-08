#pragma once

#include <Types.h>

namespace i18n {
enum class Lang
{
  en,
  de,
  ru,
  nl,
  pl,
  fr,
  it,
  zh
};
}

extern const i18n::Lang g_lang;

extern const int MAX_MESSAGES_ON_MAP_BOTTOM;

auto GetLanguagePrefix() -> const STR;
