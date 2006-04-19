//
// Snap: Implementation of case-insensitive string comparison classes
//
#include "stringicmp.h"
//#include <cctype>
#include <ctype.h>



bool TStringiLess::operator() (std::string const& s1, std::string const& s2) const
{
	// An MSVC compliance issue...
	//using std::toupper;

	std::string::const_iterator p1 = s1.begin();
	std::string::const_iterator p2 = s2.begin();

	while (p1 != s1.end() && p2 != s2.end() && toupper(*p1) == toupper(*p2)) {
		++p1;
		++p2;
	}

	if (p1 == s1.end()) return p2 != s2.end();
	if (p2 == s2.end()) return false;

	return toupper(*p1) < toupper(*p2);
}
