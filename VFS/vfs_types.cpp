#include "vfs_types.h"
#include "vfs_debug.h"

#include <vector>

//////////////////////////////////////////////////////////////////////

template<>
std::string vfs::TrimString<std::string>(std::string const& sStr, vfs::Int32 iMinPos, vfs::Int32 iMaxPos)
{
	if(iMinPos >= iMaxPos || iMaxPos < 0)
	{
		return "";
	}
	vfs::Int32 iStart,iEnd;
	iStart = sStr.find_first_not_of(" \t\r\n",iMinPos);
	iEnd   = sStr.find_last_not_of(" \t\r\n",iMaxPos);
	if( (iStart >= 0) && (iEnd >= 0) )
	{
		return sStr.substr(iStart,iEnd-iStart+1);
	}
	return "";
}
template<>
std::wstring vfs::TrimString<std::wstring>(std::wstring const& sStr, vfs::Int32 iMinPos, vfs::Int32 iMaxPos)
{
	if(iMinPos >= iMaxPos || iMaxPos < 0)
	{
		return L"";
	}
	vfs::Int32 iStart,iEnd;
	iStart = sStr.find_first_not_of(L" \t\r\n",iMinPos);
	iEnd   = sStr.find_last_not_of(L" \t\r\n",iMaxPos);
	if( (iStart >= 0) && (iEnd >= 0) )
	{
		return sStr.substr(iStart,iEnd-iStart+1);
	}
	return L"";
}
template<>
utf8string vfs::TrimString<utf8string>(utf8string const& sStr, vfs::Int32 iMinPos, vfs::Int32 iMaxPos)
{
	return vfs::TrimString(sStr.c_wcs(), iMinPos, iMaxPos);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline static void UnifySeparators(utf8string::str_t &sPath)
{
	utf8string::char_t &raw = sPath[0];
	utf8string::ptr_t raw_ptr = &raw;
	while(*raw_ptr != 0)
	{
		if(*raw_ptr == '\\' || *raw_ptr == '/')
		{
			*raw_ptr = vfs::Const::SEPARATOR_CHAR();
		}
		raw_ptr++;
	}
}

/**
 *  call after unifying separators
 */
inline unsigned int RemoveSeparators(utf8string::str_t &str)
{
	vfs::UInt32 sepcount = 0;
	vfs::Int32 numsep = 0; 
	size_t put_pos = 0;
	size_t len = str.length();
	utf8string::char_t&	raw = str[0];
	utf8string::ptr_t	old_ptr = &raw;
	utf8string::ptr_t	new_ptr = &raw;
	utf8string::ptr_t	last_ptr = &raw;
	while(*old_ptr != 0)
	{
		if(*old_ptr == vfs::Const::SEPARATOR_CHAR())
		{
			numsep++;
			if(numsep == 1)
			{
				sepcount++;
			}
		}
		else
		{
			// we have normal text (again)
			numsep = 0;
		}
		if(numsep <= 1)
		{
			*new_ptr = *old_ptr;
			last_ptr = new_ptr++;
			put_pos++;
		}
		old_ptr++;
	}
	if(*last_ptr == vfs::Const::SEPARATOR_CHAR())
	{
		put_pos--;
		if(sepcount>0)
		{
			sepcount--;
		}
	}
	if(put_pos < len)
	{
		str.erase(put_pos);
	}
	return sepcount;
}

inline void RemoveLastSeparator(utf8string::str_t &str)
{
	if( *str.rbegin() == vfs::Const::SEPARATOR_CHAR() )
	{
		str.erase( str.length()-1);
	}
}

inline void RemoveDots(utf8string::str_t &str, vfs::UInt32 number_of_separators)
{
	utf8string::char_t&	raw = str[0];
	utf8string::ptr_t	old_ptr = &raw;
	utf8string::ptr_t	new_ptr = &raw;
	utf8string::size_t	new_pos = 0;
	utf8string::size_t	LENGTH = str.length();
	bool dirty = false;

	// see if we start with ./ (not . as this might be a hidden files on unix systems)
	if( (LENGTH > 1) && (*old_ptr == vfs::Const::DOT_CHAR()) && (*(old_ptr+1) == vfs::Const::SEPARATOR_CHAR()) )
	{
		old_ptr += 2;
		dirty = true;
	}

	std::vector<utf8string::ptr_t> sub_strings;
	sub_strings.resize(number_of_separators);
	int pos=0;

	if(*old_ptr != 0 && *old_ptr == vfs::Const::SEPARATOR_CHAR())
	{
		if(!dirty)
		{
			// in-place : just move along
			new_ptr++;
			old_ptr++;
		}
		else
		{
			*new_ptr++ = *old_ptr++;
		}
	}
	else
	{
	}
	sub_strings[pos++] = new_ptr;
	
	size_t current_position = 0;
	while(*old_ptr != 0)
	{
		if(*old_ptr != vfs::Const::SEPARATOR_CHAR())
		{
			if(!dirty)
			{
				new_ptr++;
				old_ptr++;
			}
			else
			{
				*new_ptr++ = *old_ptr++;
			}
			continue;
		}

		*new_ptr++ = *old_ptr++;
		sub_strings[pos++] = new_ptr;

		if((&raw+3) < old_ptr)
		{
			if( (*(old_ptr-4) == vfs::Const::SEPARATOR_CHAR()) && 
				(*(old_ptr-3) == vfs::Const::DOT_CHAR()) && 
				(*(old_ptr-2) == vfs::Const::DOT_CHAR()) && 
				(*(old_ptr-1) == vfs::Const::SEPARATOR_CHAR()) )
			{
				if(pos > 1)
				{
					new_ptr = sub_strings[pos-3];
					pos -= 2;
					dirty = true;
				}
			}
			else if((*(old_ptr-3) == vfs::Const::SEPARATOR_CHAR()) && 
					(*(old_ptr-2) == vfs::Const::DOT_CHAR()) && 
					(*(old_ptr-1) == vfs::Const::SEPARATOR_CHAR()) )
			{
				if(pos > 0)
				{
					new_ptr = sub_strings[pos-2];
					pos -= 1;
					dirty = true;
				}
			}
		}
		else if((&raw+2) < old_ptr)
		{
			if( (*(old_ptr-3) == vfs::Const::SEPARATOR_CHAR()) && 
				(*(old_ptr-2) == vfs::Const::DOT_CHAR()) && 
				(*(old_ptr-1) == vfs::Const::SEPARATOR_CHAR()) )
			{
				if(pos > 0)
				{
					new_ptr = sub_strings[pos-2];
					pos -= 1;
					dirty = true;
				}
			}
		}
	}

	unsigned int ttt = (new_ptr - &raw);
	if(ttt < LENGTH)
	{
		str.erase(ttt);
	}
}

void GetFirstLastSeparator(utf8string::str_t &sPath, vfs::Int32 &iFirst, vfs::Int32 &iLast)
{
	utf8string::char_t&	raw = sPath[0];
	utf8string::ptr_t	ptr = &raw;
	utf8string::size_t	pos = 0;
	while(*ptr != 0)
	{
		if(*ptr == vfs::Const::SEPARATOR_CHAR())
		{
			if(iFirst < 0)
			{
				iFirst = pos;
			}
			iLast = pos;
		}
		pos++;
		ptr++;
	}
}

//////////////////////////////////////////////////////////////////////
bool vfs::Path::Less::operator ()(vfs::Path const& s1, vfs::Path const& s2) const
{
	return utf8string::less(s1._path,s2._path);
}
bool vfs::Path::Equal::operator ()(vfs::Path const& s1, vfs::Path const& s2) const
{
	return utf8string::equal(s1._path,s2._path);
}
//////////////////////////////////////////////////////////////////////

vfs::Path::Path(utf8string const& sPath)
: _path(sPath), _first(-1), _last(-1)
{
	DoCheck();
}

vfs::Path::Path(const char* sPath)
: _path(sPath), _first(-1), _last(-1)
{
	DoCheck();
}

vfs::Path::Path(const wchar_t* sPath)
: _path(sPath), _first(-1), _last(-1)
{
	DoCheck();
}

bool vfs::Path::empty() const
{
	// is there a case where a non-empty string can become empty after a check?
	// if it is so then i don't care
	return _path.empty();
}
utf8string::size_t vfs::Path::length() const
{
	return _path.length();
}


void vfs::Path::DoCheck()
{
	if(!_path.empty())
	{
		UnifySeparators(_path.r_wcs());

		vfs::UInt32 number_of_separators = RemoveSeparators(_path.r_wcs());

		if(number_of_separators>0)
		{
			RemoveDots(_path.r_wcs(),number_of_separators+1);
			GetFirstLastSeparator(_path.r_wcs(),_first,_last);
		}
	}
}

const utf8string& vfs::Path::operator()() const
{
	return _path;
}

bool vfs::Path::SplitLast(vfs::Path &rsHead, vfs::Path &rsLast) const
{
	bool success = SplitLast(rsHead._path, rsLast._path);
	// no need to check, as the original path is already checked
	//rPath.DoCheck();
	//rFile.DoCheck();
	return success;
}

bool vfs::Path::SplitLast(utf8string &rsHead, utf8string &rsLast) const
{
	utf8string::str_t& rHead = rsHead.r_wcs();
	utf8string::str_t& rLast = rsLast.r_wcs();
	if(&rHead == &_path.c_wcs() || &rLast == &_path.c_wcs())
	{
		THROWEXCEPTION(L"cannot use output parameters that are equal to 'this'");
	}
	if(_path.empty())
	{
		return false;
	}

	utf8string::size_t LENGTH = _path.length();

#if 1
	// use results from "GetFirstLastSeparator(..)"
	if(_last >= 0)
	{
		rHead.assign(_path.c_wcs().substr(0,_last));
		rLast.assign(_path.c_wcs().substr(_last+1,LENGTH-_last-1));
		return true;
	}
#endif

	vfs::Int32 position = LENGTH;
	while(--position >= 0)
	{
		utf8string::char_t const& c = _path.c_wcs()[position];
		if(c == '\\' || c == '/')
		{
			break;
		}		
	}
	if(position >= 0)
	{
		rHead.assign(_path.c_wcs().substr(0,position));
		rLast.assign(_path.c_wcs().substr(position+1,LENGTH-position-1));
	}
	else
	{
		rHead.assign(vfs::Const::EMPTY());
		rLast.assign(_path.c_wcs());
	}
	return true;
}

bool vfs::Path::SplitFirst(vfs::Path &rsFirst, vfs::Path &rsTail) const
{
	bool success = SplitFirst(rsFirst._path, rsTail._path);
	// no need to check, as the original path is already checked
	//rPath.DoCheck();
	//rFile.DoCheck();
	return success;
}
bool vfs::Path::SplitFirst(utf8string &rsFirst, utf8string &rsTail) const
{
	utf8string::str_t& rFirst = rsFirst.r_wcs();
	utf8string::str_t& rTail = rsTail.r_wcs();

	if(_path.empty())
	{
		return false;
	}
	if(&rFirst == &_path.c_wcs() || &rTail == &_path.c_wcs())
	{
		THROWEXCEPTION(L"cannot use output parameters that are equal to this");
	}

	utf8string::size_t LENGTH = _path.length();

#if 1
	// use results from "GetFirstLastSeparator(..)"
	if(_first >= 0)
	{
		rFirst.assign(_path.c_wcs().substr(0,_first));
		rTail.assign(_path.c_wcs().substr(_first+1,LENGTH-_first-1));
		return true;
	}
#endif

	vfs::UInt32 position = 0;
	while(position++ < LENGTH)
	{
		utf8string::char_t const& c = _path.c_wcs()[position];
		if(c == '\\' || c == '/')
		{
			break;
		}		
	}
	if(position < LENGTH)
	{
		rFirst.assign(_path.c_wcs().substr(0,position));
		rTail.assign(_path.c_wcs().substr(position+1,LENGTH-position-1));
	}
	else
	{
		rFirst.assign(_path.c_wcs());
		rTail.assign(vfs::Const::EMPTY());
	}
	return true;
}


bool vfs::Path::Extension(utf8string &sExt) const
{
	utf8string::size_t SIZE = _path.length();
	if(_path.c_wcs().at(SIZE-1) == L'.')
	{
		// not an extension
		return false;
	}
	for(vfs::UInt32 i=SIZE-2; i > 0; i--)
	{
		if(_path.c_wcs().at(i) == L'.')
		{
			sExt.r_wcs().assign(&_path.c_wcs().at(i+1),SIZE-i-1);
			return true;
		}
	}
	return false;
}


vfs::Path& vfs::Path::operator+=(vfs::Path const& p)
{
	if(_path.empty())
	{
		_path = p._path;
	}
	else if(!p.empty())
	{
		utf8string::str_t& s =_path.r_wcs();
		s += vfs::Const::SEPARATOR();
		s += p._path.c_wcs();
		GetFirstLastSeparator(s,_first,_last);
	}
	return *this;
}

vfs::Path operator+(vfs::Path const& p1, vfs::Path const& p2)
{
	vfs::Path newpath = p1;
	newpath += p2;
	return newpath;
}

bool vfs::Path::operator==(vfs::Path const& p2)
{
	return utf8string::equal(_path, p2._path);
}


class PathAccess
{
public:
	PathAccess(vfs::Path const& p) : str(p._path) {};
	utf8string const& str;
};

bool operator==(vfs::Path const& p1, vfs::Path const& p2)
{
	return StrCmp::Equal(PathAccess(p1).str, PathAccess(p2).str);
}
bool operator==(vfs::Path const& p1, utf8string const& p2)
{
	return StrCmp::Equal(PathAccess(p1).str, p2); 
}
bool operator==(vfs::Path const& p1, utf8string::str_t const& p2)
{
	return StrCmp::Equal(PathAccess(p1).str, p2);
}
bool operator==(vfs::Path const& p1, const utf8string::char_t* p2)
{
	return StrCmp::Equal(PathAccess(p1).str, p2);
}

/*************************************************************************/
/*************************************************************************/

/**
 *  try to recursively match the pattern
 */
bool MatchPattern(utf8string const& sPattern, utf8string const& sStr)
{
	return MatchPattern(sPattern,sStr.c_wcs());
}

bool MatchPattern(utf8string const& sPattern, utf8string::str_t const& sStr)
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
		utf8string::size_t match = -1;
		do
		{
			match = sStr.find_first_of(atpos1,match+1);
			if(match == utf8string::str_t::npos)
			{
				return false;
			}
		} while(!MatchPattern( pat.substr(1,pat.length()-1), sStr.substr(match,sStr.length()-match) ));
		return true;
	}
	else // if(star > 0)
	{
		// check if characters before * match
		if(!StrCmp::Equal(pat.substr(0,star), sStr.substr(0,star)) )
		{
			return false;
		}
		return MatchPattern( pat.substr(star,pat.length()-star), sStr.substr(star,sStr.length()-star) );
	}
}

/*************************************************************************/
/*************************************************************************/

