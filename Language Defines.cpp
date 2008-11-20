#include "Language Defines.h"

#if defined(ENGLISH)
#	pragma message(" (Language set to ENGLISH, You'll need english CDs)")
#elif defined(GERMAN)
#	pragma message(" (Language set to GERMAN, You'll need Topware/german CDs)")
#elif defined(RUSSIAN)
#	pragma message(" (Language set to RUSSIAN, You'll need russian CDs)")
#elif defined(DUTCH)
#	pragma message(" (Language set to DUTCH, You'll need dutch CDs)")
#elif defined(POLISH)
#	pragma message(" (Language set to POLISH, You'll need polish CDs)")
#elif defined(FRENCH)
#	pragma message(" (Language set to FRENCH, You'll need french CDs)")
#elif defined(ITALIAN)
#	pragma message(" (Language set to ITALIAN, You'll need italian CDs)")
#elif defined(TAIWANESE)
#	pragma message(" (Language set to TAIWANESE, You'll need taiwanese CDs)")
#elif defined(CHINESE)
#	pragma message(" (Language set to CHINESE, You'll need chinese CDs)")
#else
#	error "At least You have to specify a Language somewhere. See comments above."
#endif
