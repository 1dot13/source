/* 
 * bfVFS : vfs/Core/vfs_path.cpp
 *  - Path class, stores and validates a file/directory path string, offers meaningful path operations
 *  - path comparison functions (operator overloading)
 *
 * Copyright (C) 2008 - 2010 (BF) john.bf.smith@googlemail.com
 * 
 * This file is part of the bfVFS library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <vfs/Core/vfs_path.h>
#include <vfs/Core/vfs_debug.h>
#include <vfs/Core/vfs_os_functions.h>

#include <vfs/Aspects/vfs_settings.h>

#include <stack>
#include <vector>

typedef struct{ vfs::String::size_t start, end; } t_env;

bool vfs::Path::expandEnv()
{
	vfs::String::ptr_t raw_ptr = &_path[0];
	std::stack<t_env> pos;
	vfs::Int32 pos_start = -1, pos_end = -1, pos_current = 0;
	while(*raw_ptr != 0)
	{
		if(*raw_ptr == L'$')
		{
			pos_start = pos_current;
		}
		else if(pos_start >= 0)
		{
			if(*raw_ptr == '(' && pos_current == (pos_start+1))
			{
				// ensure the syntax $(VARNAME) is followed
				pos_end = pos_start;
			}
			//else
			//{
			//	pos_start = pos_end = -1;
			//}
			else if(pos_start == pos_end && *raw_ptr == ')')
			{
				pos_end = pos_current;
			}
			//
			if(pos_end >= 0 && pos_start != pos_end)
			{
				t_env e;
				e.start = pos_start;
				e.end = pos_end;
				pos.push(e);
				pos_start = pos_end = -1;
			}
		}
		pos_current++;
		raw_ptr++;
	}
	if(!pos.empty())
	{
		while(!pos.empty())
		{
			t_env v = pos.top();
			vfs::String var_name = _path.substr(v.start+2, v.end-v.start-2);
			vfs::String var_value;
			if(!vfs::OS::getEnv(var_name, var_value))
			{
				VFS_LOG_WARNING(_BS(L"Could not expand environment variable : ") << var_name << _BS::wget);
				return false;
			}
			_path.replace(v.start,v.end-v.start+1,var_value.c_wcs());
			pos.pop();
		}
		doCheck();
	}
	return true;
}

static void unifySeparators(vfs::String::str_t &sPath)
{
	vfs::String::char_t &raw = sPath[0];
	vfs::String::ptr_t raw_ptr = &raw;
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
static vfs::UInt32 removeSeparators(vfs::String::str_t &str)
{
	vfs::UInt32 sepcount = 0;
	vfs::Int32 numsep = 0; 
	::size_t put_pos = 0;
	::size_t len = str.length();
	vfs::String::char_t&	raw = str[0];
	vfs::String::ptr_t	old_ptr = &raw;
	vfs::String::ptr_t	new_ptr = &raw;
	vfs::String::ptr_t	last_ptr = &raw;
	vfs::String::char_t	sep = vfs::Const::SEPARATOR_CHAR();
	while(*old_ptr != 0)
	{
		if(*old_ptr == sep)
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
	if(*last_ptr == sep)
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

static void removeLastSeparator(vfs::String::str_t &str)
{
	if( *str.rbegin() == vfs::Const::SEPARATOR_CHAR() )
	{
		str.erase( str.length()-1);
	}
}

static void removeDots(vfs::String::str_t &str, vfs::UInt32 number_of_separators)
{
	vfs::String::char_t&	raw = str[0];
	vfs::String::ptr_t	old_ptr = &raw;
	vfs::String::ptr_t	new_ptr = &raw;
	vfs::String::size_t	LENGTH = str.length();
	bool dirty = false;

	// see if we start with ./ (not . as this might be a hidden files on unix systems)
	if( (LENGTH > 1) && (*old_ptr == vfs::Const::DOT_CHAR()) && (*(old_ptr+1) == vfs::Const::SEPARATOR_CHAR()) )
	{
		old_ptr += 2;
		dirty = true;
	}

	std::vector<vfs::String::ptr_t> sub_strings;
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

static void getFirstLastSeparator(const vfs::String::char_t* sPath, vfs::size_t &iFirst, vfs::size_t &iLast)
{
	iFirst = vfs::npos;
	iLast = vfs::npos;
	const vfs::String::char_t*	ptr = sPath;
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
	return vfs::String::less(s1._path.c_str(), s2._path.c_str());
}
bool vfs::Path::Equal::operator ()(vfs::Path const& s1, vfs::Path const& s2) const
{
	return vfs::String::equal(s1._path.c_str(), s2._path.c_str());
}
//////////////////////////////////////////////////////////////////////

vfs::Path::Path()
{
}

vfs::Path::Path(vfs::String const& sPath)
: _path(sPath.c_wcs())
{
	doCheck();
}

vfs::Path::Path(const char* sPath)
{
	if(vfs::Settings::getUseUnicode())
	{
		vfs::String::as_utf16(sPath,_path); 
	}
	else
	{
		vfs::String::widen(std::string(sPath),_path);
	}
	doCheck();
}

vfs::Path::Path(std::string const& sPath)
{
	if(vfs::Settings::getUseUnicode())
	{
		vfs::String::as_utf16(sPath,_path); 
	}
	else
	{
		vfs::String::widen(sPath,_path);
	}
	doCheck();
}

vfs::Path::Path(const wchar_t* sPath)
: _path(sPath)
{
	doCheck();
}

const vfs::String::char_t* vfs::Path::c_str() const
{
	return _path.c_str();
}

const vfs::String::str_t& vfs::Path::c_wcs() const
{
	return _path;
}

const vfs::String::str_t& vfs::Path::operator()() const
{
	return _path;
}

std::string	vfs::Path::to_string() const
{
	if(vfs::Settings::getUseUnicode())
	{
		return vfs::String::as_utf8(_path);
	}
	else
	{
		std::string s;
		vfs::String::narrow(_path,s);
		return s;
	}
}

bool vfs::Path::empty() const
{
	// is there a case where a non-empty string can become empty after a check?
	// if it is so then i don't care
	return _path.empty();
}
vfs::String::size_t vfs::Path::length() const
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

static void pathSplitLast(vfs::String::str_t const& path,
						  vfs::String::str_t &head,
						  vfs::String::str_t &last,
						  vfs::size_t const& sep_first,
						  vfs::size_t const& sep_last)
{
	if(path.empty())
	{
		return;
	}
	if(&head == &path || &last == &path)
	{
		VFS_THROW(L"cannot use output parameters that are equal to 'this'");
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
	vfs::String::size_t position = path.length();
	while(--position != vfs::npos)
	{
		vfs::String::char_t const& c = path.at(position);
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

static void pathSplitFirst(vfs::String::str_t const& path,
						   vfs::String::str_t &first,
						   vfs::String::str_t &tail,
						   vfs::size_t const& sep_first,
						   vfs::size_t const& sep_last)
{
	if(path.empty())
	{
		return;
	}
	if(&first == &path || &tail == &path)
	{
		VFS_THROW(L"cannot use output parameters that are equal to 'this'");
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
	vfs::String::size_t position = 0;
	while(position < path.length())
	{
		vfs::String::char_t const& c = path.at(position);
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

bool vfs::Path::extension(vfs::String &sExt) const
{
	if(_path.empty())
	{
		return false;
	}
	vfs::String::size_t SIZE = _path.length();
	if(_path.at(SIZE-1) == L'.')
	{
		// not an extension
		return false;
	}
	for(vfs::String::size_t i=SIZE-2; i > 0; i--)
	{
		if(_path.at(i) == L'.')
		{
			sExt.r_wcs().assign(&_path.at(i+1),SIZE-i-1);
			return true;
		}
	}
	return false;
}

vfs::Path& vfs::Path::operator+=(vfs::String const& p)
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
	return vfs::String::equal(_path.c_str(), p2._path.c_str());
}

bool operator==(vfs::Path const& p1, vfs::Path const& p2)
{
	return vfs::StrCmp::Equal(p1.c_str(), p2.c_str());
}
bool operator==(vfs::Path const& p1, vfs::String const& p2)
{
	return vfs::StrCmp::Equal(p1.c_str(), p2); 
}
bool operator==(vfs::Path const& p1, vfs::String::str_t const& p2)
{
	return vfs::StrCmp::Equal(p1.c_str(), p2);
}
bool operator==(vfs::Path const& p1, const vfs::String::char_t* p2)
{
	return vfs::StrCmp::Equal(p1.c_str(), p2);
}

template<>
BuildString& BuildString::add<vfs::Path>(vfs::Path const& value)
{
	this->add(value.c_str());
	return *this;
}

template<>
BuildString& BuildString::operator<< <vfs::Path>(vfs::Path const& value)
{
	this->add(value.c_str());
	return *this;
}
