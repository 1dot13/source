//
// Snap: Implementation of case-insensitive string comparison classes
//
#include "stringicmp.h"
#include <ctype.h>



bool TStringiLess::operator() (std::string const& s1, std::string const& s2) const
{
	// An MSVC compliance issue...
	//using std::toupper;

	const char *p1 = s1.c_str();
	const char *p2 = s2.c_str();

	while (*p1 && *p2 && toupper(*p1) == toupper(*p2)) {
		++p1;
		++p2;
	}

	if (!*p1) return *p2 != 0;
	if (!*p2) return false;

	return toupper(*p1) < toupper(*p2);
}
