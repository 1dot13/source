#include "vfs_path.h"
#include "vfs_debug.h"
#include "vfs_settings.h"

static void unifySeparators(utf8string::str_t &sPath)
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
static vfs::UInt32 removeSeparators(utf8string::str_t &str)
{
	vfs::UInt32 sepcount = 0;
	vfs::Int32 numsep = 0; 
	::size_t put_pos = 0;
	::size_t len = str.length();
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

static void removeLastSeparator(utf8string::str_t &str)
{
	if( *str.rbegin() == vfs::Const::SEPARATOR_CHAR() )
	{
		str.erase( str.length()-1);
	}
}

static void removeDots(utf8string::str_t &str, vfs::UInt32 number_of_separators)
{
	utf8string::char_t&	raw = str[0];
	utf8string::ptr_t	old_ptr = &raw;
	utf8string::ptr_t	new_ptr = &raw;
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

	::size_t ttt = (new_ptr - &raw);
	if(ttt < LENGTH)
	{
		str.erase(ttt);
	}
}

static void getFirstLastSeparator(const utf8string::char_t* sPath, vfs::size_t &iFirst, vfs::size_t &iLast)
{
	const utf8string::char_t*	ptr = sPath;
	vfs::size_t					pos = 0;
	while(*ptr != 0)
	{
		if(*ptr == vfs::Const::SEPARATOR_CHAR())
		{
			if(iFirst == vfs::npos)
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
	return utf8string::less(s1._path.c_str(), s2._path.c_str());
}
bool vfs::Path::Equal::operator ()(vfs::Path const& s1, vfs::Path const& s2) const
{
	return utf8string::equal(s1._path.c_str(), s2._path.c_str());
}
//////////////////////////////////////////////////////////////////////

vfs::Path::Path()
{
}

vfs::Path::Path(utf8string const& sPath)
: _path(sPath.c_wcs())
{
	doCheck();
}

vfs::Path::Path(const char* sPath)
{
	if(vfs::Settings::getUseUnicode())
	{
		utf8string::as_utf16(sPath,_path); 
	}
	else
	{
		utf8string::widen(std::string(sPath),_path);
	}
	doCheck();
}

vfs::Path::Path(std::string const& sPath)
{
	if(vfs::Settings::getUseUnicode())
	{
		utf8string::as_utf16(sPath,_path); 
	}
	else
	{
		utf8string::widen(sPath,_path);
	}
	doCheck();
}

vfs::Path::Path(const wchar_t* sPath)
: _path(sPath)
{
	doCheck();
}

const utf8string::char_t* vfs::Path::c_str() const
{
	return _path.c_str();
}

const utf8string::str_t& vfs::Path::c_wcs() const
{
	return _path;
}

const utf8string::str_t& vfs::Path::operator()() const
{
	return _path;
}

std::string	vfs::Path::to_string() const
{
	if(vfs::Settings::getUseUnicode())
	{
		return utf8string::as_utf8(_path);
	}
	else
	{
		std::string s;
		utf8string::narrow(_path,s);
		return s;
	}
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


void vfs::Path::doCheck()
{
	if(!_path.empty())
	{
		unifySeparators(_path);

		vfs::UInt32 number_of_separators = removeSeparators(_path);

		if(number_of_separators>0)
		{
			removeDots(_path,number_of_separators+1);
			getFirstLastSeparator(_path.c_str(),_sep.first,_sep.last);
		}
	}
}

static void pathSplitLast(utf8string::str_t const& path,
						  utf8string::str_t &head,
						  utf8string::str_t &last,
						  vfs::size_t const& sep_first,
						  vfs::size_t const& sep_last)
{
	if(path.empty())
	{
		return;
	}
	if(&head == &path || &last == &path)
	{
		THROWEXCEPTION(L"cannot use output parameters that are equal to 'this'");
	}
#if 1
	// use results from "GetFirstLastSeparator(..)"
	if(sep_last != vfs::npos)
	{
		head.assign(path.substr(0,sep_last));
		last.assign(path.substr(sep_last+1, path.length()-sep_last-1));
		return;
	}
#else
	utf8string::size_t position = path.length();
	while(--position != vfs::npos)
	{
		utf8string::char_t const& c = path.at(position);
		if(c == '\\' || c == '/')
		{
			break;
		}		
	}
	if(position != vfs::npos)
	{
		head.assign(path.substr(0,position++));
		last.assign(path.substr(position,path.length()-position));
		return;
	}
#endif
	head.assign(vfs::Const::EMPTY());
	last.assign(path);
}


void vfs::Path::splitLast(vfs::Path &rsHead, vfs::Path &rsLast) const
{

	pathSplitLast(_path, rsHead._path, rsLast._path, _sep.first, _sep.last);
	getFirstLastSeparator(rsHead.c_str(), rsHead._sep.first, rsHead._sep.last);
	getFirstLastSeparator(rsLast.c_str(), rsLast._sep.first, rsLast._sep.last);
	// no need to check, as the original path is already checked
	//rPath.DoCheck();
	//rFile.DoCheck();
}

static void pathSplitFirst(utf8string::str_t const& path,
						   utf8string::str_t &first,
						   utf8string::str_t &tail,
						   vfs::size_t const& sep_first,
						   vfs::size_t const& sep_last)
{
	if(path.empty())
	{
		return;
	}
	if(&first == &path || &tail == &path)
	{
		THROWEXCEPTION(L"cannot use output parameters that are equal to 'this'");
	}

#if 1
	// use results from "GetFirstLastSeparator(..)"
	if(sep_first != vfs::npos)
	{
		first.assign(path.substr(0,sep_first));
		tail.assign(path.substr(sep_first+1, path.length()-sep_first-1));
		return;
	}
#else
	utf8string::size_t position = 0;
	while(position < path.length())
	{
		utf8string::char_t const& c = path.at(position);
		if(c == '\\' || c == '/')
		{
			break;
		}
		position++;
	}
	if(position < path.length())
	{
		first.assign(path.substr(0,position++));
		tail.assign(path.substr(position,path.length()-position));
		return;
	}
#endif
	first.assign(path);
	tail.assign(vfs::Const::EMPTY());
}

void vfs::Path::splitFirst(vfs::Path &rsFirst, vfs::Path &rsTail) const
{
	pathSplitFirst(_path, rsFirst._path, rsTail._path, _sep.first, _sep.last);
	getFirstLastSeparator(rsFirst.c_str(), rsFirst._sep.first, rsFirst._sep.last);
	getFirstLastSeparator(rsTail.c_str(), rsTail._sep.first, rsTail._sep.last);
	// no need to check, as the original path is already checked
	//rPath.doCheck();
	//rFile.doCheck();
}

bool vfs::Path::extension(utf8string &sExt) const
{
	if(_path.empty())
	{
		return false;
	}
	utf8string::size_t SIZE = _path.length();
	if(_path.at(SIZE-1) == L'.')
	{
		// not an extension
		return false;
	}
	for(utf8string::size_t i=SIZE-2; i > 0; i--)
	{
		if(_path.at(i) == L'.')
		{
			sExt.r_wcs().assign(&_path.at(i+1),SIZE-i-1);
			return true;
		}
	}
	return false;
}

vfs::Path& vfs::Path::operator+=(utf8string const& p)
{
	return *this += vfs::Path(p);
}

vfs::Path& vfs::Path::operator+=(vfs::Path const& p)
{
	if(_path.empty())
	{
		_path = p._path;
	}
	else if(!p.empty())
	{
		_path += vfs::Const::SEPARATOR();
		_path += p._path;
		getFirstLastSeparator(_path.c_str(),_sep.first,_sep.last);
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
	return utf8string::equal(_path.c_str(), p2._path.c_str());
}

bool operator==(vfs::Path const& p1, vfs::Path const& p2)
{
	return StrCmp::Equal(p1.c_str(), p2.c_str());
}
bool operator==(vfs::Path const& p1, utf8string const& p2)
{
	return StrCmp::Equal(p1.c_str(), p2); 
}
bool operator==(vfs::Path const& p1, utf8string::str_t const& p2)
{
	return StrCmp::Equal(p1.c_str(), p2);
}
bool operator==(vfs::Path const& p1, const utf8string::char_t* p2)
{
	return StrCmp::Equal(p1.c_str(), p2);
}

template<>
BuildString& BuildString::add<vfs::Path>(vfs::Path const& value)
{
	this->add(value.c_str());
	return *this;
}

