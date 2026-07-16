// Scaffolding for the language axis migration (docs/plans/language-design.md, chunk
// C2): pulls all eight base + eight Ja2.5-carryover text files into this one
// translation unit, each inside its own per-language namespace, to prove they compile
// together. The global table pointers and BindLanguageStrings body are filled in by
// later chunks (Recipe R1); for now BindLanguageStrings is an empty stub.

#include "Text.h"
#include "FileMan.h"
#include "Scheduling.h"
#include "EditorMercs.h"
#include "Item Statistics.h"

#include <language.hpp>

namespace lang_en {
#undef ENGLISH
#define ENGLISH
#include "_EnglishText.cpp"
#include "_Ja25EnglishText.cpp"
#undef ENGLISH
}

namespace lang_de {
#undef GERMAN
#define GERMAN
#include "_GermanText.cpp"
#include "_Ja25GermanText.cpp"
#undef GERMAN
}

namespace lang_ru {
#undef RUSSIAN
#define RUSSIAN
#include "_RussianText.cpp"
#include "_Ja25RussianText.cpp"
#undef RUSSIAN
}

namespace lang_nl {
#undef DUTCH
#define DUTCH
#include "_DutchText.cpp"
#include "_Ja25DutchText.cpp"
#undef DUTCH
}

namespace lang_pl {
#undef POLISH
#define POLISH
#include "_PolishText.cpp"
#include "_Ja25PolishText.cpp"
#undef POLISH
}

namespace lang_fr {
#undef FRENCH
#define FRENCH
#include "_FrenchText.cpp"
#include "_Ja25FrenchText.cpp"
#undef FRENCH
}

namespace lang_it {
#undef ITALIAN
#define ITALIAN
#include "_ItalianText.cpp"
#include "_Ja25ItalianText.cpp"
#undef ITALIAN
}

namespace lang_zh {
#undef CHINESE
#define CHINESE
#include "_ChineseText.cpp"
#include "_Ja25ChineseText.cpp"
#undef CHINESE
}

auto BindLanguageStrings(i18n::Lang lang) -> void {
  switch (lang) {
    case i18n::Lang::en: break;
    case i18n::Lang::de: break;
    case i18n::Lang::ru: break;
    case i18n::Lang::nl: break;
    case i18n::Lang::pl: break;
    case i18n::Lang::fr: break;
    case i18n::Lang::it: break;
    case i18n::Lang::zh: break;
  }
}
