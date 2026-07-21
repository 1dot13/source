#include "Text.h"
#include "Item Statistics.h"

#include <language.hpp>

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
