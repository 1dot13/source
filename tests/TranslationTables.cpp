#include "Text.h"
#include "Item Statistics.h"

#include <language.hpp>

#include <gtest/gtest.h>

#include <cwchar>
#include <cwctype>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

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

namespace {

struct Entry
{
  std::string name;
  const CHAR16* text;
};

using Entries = std::vector<Entry>;

template <typename T>
auto collect(Entries& entries, const std::string& name, const T& table) -> void
{
  if constexpr (std::is_array_v<T>) {
    if constexpr (std::is_same_v<std::remove_cv_t<std::remove_extent_t<T>>, CHAR16>) {
      entries.push_back({name, table});
    } else {
      for (size_t i = 0; i < std::extent_v<T>; ++i) {
        collect(entries, name + "[" + std::to_string(i) + "]", table[i]);
      }
    }
  } else if constexpr (std::is_convertible_v<T, const CHAR16*>) {
    entries.push_back({name, table});
  }
}

auto en_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_en::NAME);
#include "text.def"
#undef X
  return entries;
}

auto de_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_de::NAME);
#include "text.def"
#undef X
  return entries;
}

auto ru_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_ru::NAME);
#include "text.def"
#undef X
  return entries;
}

auto nl_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_nl::NAME);
#include "text.def"
#undef X
  return entries;
}

auto pl_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_pl::NAME);
#include "text.def"
#undef X
  return entries;
}

auto fr_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_fr::NAME);
#include "text.def"
#undef X
  return entries;
}

auto it_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_it::NAME);
#include "text.def"
#undef X
  return entries;
}

auto zh_entries() -> Entries
{
  Entries entries;
#define X(NAME) collect(entries, #NAME, lang_zh::NAME);
#include "text.def"
#undef X
  return entries;
}

auto formatArguments(const CHAR16* text) -> std::wstring
{
  std::wstring arguments;
  auto skip = [&text](auto&& matches) {
    while (*text && matches(*text)) {
      ++text;
    }
  };
  auto skipCounts = [&] {
    skip([&](CHAR16 c) {
      if (c == L'*') {
        arguments += L'*';
      }
      return std::iswdigit(c) || c == L'*';
    });
  };

  while (*text) {
    if (*text++ != L'%') {
      continue;
    }
    if (*text == L'%') {
      ++text;
      continue;
    }
    skip([](CHAR16 c) { return std::wcschr(L"-+ #0", c) != nullptr; });
    skipCounts();
    if (*text == L'.') {
      ++text;
      skipCounts();
    }
    skip([](CHAR16 c) { return std::wcschr(L"hlLjzt", c) != nullptr; });
    if (*text == L'I') {
      ++text;
      skip([](CHAR16 c) { return std::iswdigit(c) != 0; });
    }
    if (*text) {
      arguments += *text == L'i' ? L'd' : *text;
      ++text;
    }
  }
  return arguments;
}

const std::set<std::string> plainTextTables = {
    "FactDescText",
    "gzMiscItemStatsFasthelp[32]",
    "szUDBAdvStatsExplanationsTooltipText",
    "szUDBGenArmorStatsExplanationsTooltipText",
};

auto isPlainText(const std::string& name) -> bool
{
  return plainTextTables.count(name) || plainTextTables.count(name.substr(0, name.find('[')));
}

struct Language
{
  const char* code;
  Entries (*entries)();
};

auto languageName(const testing::TestParamInfo<Language>& info) -> std::string
{
  return info.param.code;
}

auto PrintTo(const Language& language, std::ostream* os) -> void
{
  *os << language.code;
}

class TranslationTable : public testing::TestWithParam<Language>
{
};

TEST_P(TranslationTable, HasNoNullEntries)
{
  for (const auto& entry : GetParam().entries()) {
    EXPECT_NE(entry.text, nullptr) << GetParam().code << "::" << entry.name;
  }
}

TEST_P(TranslationTable, FormatArgumentsMatchEnglish)
{
  const auto english = en_entries();
  const auto translation = GetParam().entries();
  ASSERT_EQ(translation.size(), english.size());
  for (size_t i = 0; i < english.size(); ++i) {
    if (!english[i].text || !translation[i].text || isPlainText(english[i].name)) {
      continue;
    }
    EXPECT_EQ(formatArguments(translation[i].text), formatArguments(english[i].text))
        << GetParam().code << "::" << translation[i].name << "\n  "
        << testing::PrintToString(std::wstring(translation[i].text)) << "\n  English: "
        << testing::PrintToString(std::wstring(english[i].text));
  }
}

INSTANTIATE_TEST_SUITE_P(English, TranslationTable,
                         testing::Values(Language{"en", en_entries}), languageName);

INSTANTIATE_TEST_SUITE_P(Translations, TranslationTable,
                         testing::Values(Language{"de", de_entries},
                                         Language{"ru", ru_entries},
                                         Language{"nl", nl_entries},
                                         Language{"pl", pl_entries},
                                         Language{"fr", fr_entries},
                                         Language{"it", it_entries},
                                         Language{"zh", zh_entries}),
                         languageName);

}
