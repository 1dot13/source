#include "vfs_types.h"
#include "vfs_debug.h"

#include <vector>

//////////////////////////////////////////////////////////////////////

namespace vfs
{
	const vfs::size_t npos = vfs::size_t(-1);
}

//////////////////////////////////////////////////////////////////////

template<>
std::string vfs::trimString<std::string>(std::string const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos)
{
	if(iMinPos >= iMaxPos || iMaxPos == vfs::npos)
	{
		return "";
	}
	::size_t iStart,iEnd;
	iStart = sStr.find_first_not_of(" \t\r\n",(::size_t)iMinPos);
	iEnd   = sStr.find_last_not_of(" \t\r\n",(::size_t)iMaxPos);
	if( (iStart != std::string::npos) && (iEnd != std::string::npos) )
	{
		return sStr.substr(iStart,iEnd-iStart+1);
	}
	return "";
}
template<>
std::wstring vfs::trimString<std::wstring>(std::wstring const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos)
{
	if(iMinPos >= iMaxPos || iMaxPos == vfs::npos)
	{
		return L"";
	}
	::size_t iStart,iEnd;
	iStart = sStr.find_first_not_of(L" \t\r\n",(::size_t)iMinPos);
	iEnd   = sStr.find_last_not_of(L" \t\r\n",(::size_t)iMaxPos);
	if( (iStart != std::wstring::npos) && (iEnd != std::wstring::npos) )
	{
		return sStr.substr(iStart,iEnd-iStart+1);
	}
	return L"";
}
template<>
utf8string vfs::trimString<utf8string>(utf8string const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos)
{
	return vfs::trimString(sStr.c_wcs(), iMinPos, iMaxPos);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template<>
BuildString& BuildString::add<utf8string>(utf8string const& value)
{
	this->add(value.c_str());
	return *this;
}

/*************************************************************************/
/*************************************************************************/

/**
 *  try to recursively match the pattern
 */
bool matchPattern(utf8string const& sPattern, utf8string const& sStr)
{
	return matchPattern(sPattern,sStr.c_wcs());
}

bool matchPattern(utf8string const& sPattern, utf8string::str_t const& sStr)
{
	utf8string::str_t const& pat = sPattern.c_wcs();

	utf8string::size_t star = pat.find_first_of(vfs::Const::STAR());
	if(star == utf8string::str_t::npos)
	{
		return StrCmp::Equal( pat, sStr );
	}
	else if(star == 0)
	{
		if(pat.length() == 1)
		{
			// there is only the '*' -> matches all strings
			return true;
		}
		utf8string::char_t atpos1 = pat.at(1);
		utf8string::size_t match = utf8string::size_t(-1);
		do
		{
			match = sStr.find_first_of(atpos1,match+1);
			if(match == utf8string::str_t::npos)
			{
				return false;
			}
		} while(!matchPattern( pat.substr(1,pat.length()-1), sStr.substr(match,sStr.length()-match) ));
		return true;
	}
	else // if(star > 0)
	{
		// check if characters before * match
		if(!StrCmp::Equal(pat.substr(0,star), sStr.substr(0,star)) )
		{
			return false;
		}
		return matchPattern( pat.substr(star,pat.length()-star), sStr.substr(star,sStr.length()-star) );
	}
}

/*************************************************************************/
/*************************************************************************/

