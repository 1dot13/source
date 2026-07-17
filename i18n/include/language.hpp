#pragma once

#include <types.h>

#include <string>

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

extern i18n::Lang g_lang;

/* Parses a LANGUAGE ini value (ENGLISH/GERMAN/...) into i18n::Lang; falls back
 * to the build's compiled-in default (and logs) on unknown input. Called once
 * at startup. */
auto SetLanguageFromName(std::string const& name) -> void;

extern int MAX_MESSAGES_ON_MAP_BOTTOM;

auto GetLanguagePrefix() -> const STR;

/* Rebinds every language table's global pointer to the given language's
 * namespace (see LanguageStrings.cpp). Called once at startup, right after
 * the LANGUAGE ini key resolves g_lang (sgp.cpp GetRuntimeSettings), before
 * any UI reads a string. */
auto BindLanguageStrings(i18n::Lang lang) -> void;
