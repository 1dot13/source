//
// Snap: Declaration of case-insensitive string comparison classes
//
#pragma warning(disable:4786)
#ifndef STRINGICMP_H
#define STRINGICMP_H

#include <string>

// Function-object that compares strings (s1 < s2) ignoring case
class TStringiLess {
public:
	bool operator() (std::string const& s1, std::string const& s2) const;
};

#endif // STRINGICMP_H
