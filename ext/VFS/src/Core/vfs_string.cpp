/* 
 * bfVFS : vfs/Core/vfs_string.cpp
 *  - string class that allows conversions to/from Unicode representation (uses wchar_t internally)
 *  - comparison, concatenation, stream output class/functions
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

#include <vfs/Core/vfs_string.h>
#include <utf8.h>

#include <vector>
#include <cstdlib>
#include <cstring>

#include <vfs/Core/vfs_debug.h>
////////////////////////////////////////////////////////////////////

namespace _StrCmp
{
	////////////////////////////////////////////////////////////////
	static inline void Advance( const char*& s1, const char*& s2 )
	{
		while (*s1 && *s2 && toupper(*s1) == toupper(*s2))
		{
			++s1;
			++s2;
		}
	}
	static inline void Advance( const wchar_t*& s1, const wchar_t*& s2 )
	{
		// should be 'towupper'
		while (*s1 && *s2 && toupper(*s1) == toupper(*s2))
		{
			++s1;
			++s2;
		}	
	}
	template<typename CharType>
	static inline void AdvanceCase( const CharType*& s1, const CharType*& s2 )
	{
		while (*s1 && *s2 && (*s1 == *s2))
		{
			++s1;
			++s2;
		}	
	}
	////////////////////////////////////////////////////////////////
	template<typename CharType>
	static inline bool Equal( const CharType* s1, const CharType* s2 )
	{
		return !(*s1 || *s2);
	}
	////////////////////////////////////////////////////////////////
	static inline bool Less( const wchar_t* s1, const wchar_t* s2 )
	{
		if (!*s1) return *s2 != 0;
		if (!*s2) return false;
		// should be 'towupper'
		return toupper(*s1) < toupper(*s2);
	}
	static inline bool Less( const char* s1, const char* s2 )
	{
		if (!*s1) return *s2 != 0;
		if (!*s2) return false;
		return toupper(*s1) < toupper(*s2);
	}
	template<typename CharType>
	static inline bool LessCase( const CharType* s1, const CharType* s2 )
	{
		if (!*s1) return *s2 != 0;
		if (!*s2) return false;
		return (*s1) < (*s2);
	}
}

////////////////////////////////////////////////////////////////////

bool vfs::String::lessCase(const vfs::String::char_t* s1, const vfs::String::char_t* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::LessCase(s1,s2);

}

bool vfs::String::less(const vfs::String::char_t* s1, const vfs::String::char_t* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Less(s1,s2);
}

bool vfs::String::equalCase(const vfs::String::char_t* s1, const vfs::String::char_t* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::Equal(s1,s2);
}

bool vfs::String::equal(const vfs::String::char_t* s1, const vfs::String::char_t* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Equal(s1,s2);
}

////////////////////////////////////////////////////////////////////
///	                    class constructors
////////////////////////////////////////////////////////////////////
vfs::String::String()
{
}
vfs::String::String(const char* str)
{
	vfs::String::as_utf16(str,_str);
}
vfs::String::String(std::string const& str)
{
	vfs::String::as_utf16(str,_str);
}
vfs::String::String(const wchar_t* str)
{
	_str.assign(str);
}
vfs::String::String(std::wstring const& str)
{
	_str.assign(str);
}

////////////////////////////////////////////////////////////////////
///	                  static class methods
////////////////////////////////////////////////////////////////////
vfs::String::str_t vfs::String::as_utf16(std::string const& str)
{
	vfs::String::str_t s;
	vfs::String::as_utf16(str, s);
	return s;
}
void vfs::String::as_utf16(std::string const& str, vfs::String::str_t &str16)
{
	try
	{
		::size_t d = utf8::distance(str.begin(), str.end());
		if(d > 0)
		{
			str16.resize(d);
			utf8::utf8to16(str.begin(), str.end(), &str16[0]);
		}
	}
	catch(utf8::invalid_utf8& ex)
	{
		utf8::uint8_t c = ex.utf8_octet();
		VFS_THROW( _BS(L"Invalid UTF8 character '") << (wchar_t)c << L"'=" << (unsigned char)c <<_BS::wget );
	}
	catch(utf8::not_enough_room &ex)
	{
		std::wstring err;
		VFS_IGNOREEXCEPTION(vfs::String::as_utf16(ex.what(),err), true);
		VFS_THROW( _BS(L"Incomplete UTF8 string [") << err << L"]" << _BS::wget );
	}
	catch(...)
	{
		VFS_THROW(L"Unicode error");
	}
}

vfs::String::str_t vfs::String::as_utf16(const char* str)
{
	vfs::String::str_t s;
	vfs::String::as_utf16(str, s);
	return s;
}
void vfs::String::as_utf16(const char* str, vfs::String::str_t &str16)
{
	wchar_t buf16[1024];

	if(str == NULL)
	{
		return;
	}
	try
	{
		memset(buf16, 0, 1024 * sizeof(wchar_t));
		mbstowcs(buf16, str, 1024 - 1);

		::size_t len = strlen(str);
		::size_t d = utf8::distance(str,str+len);
		if(d > 0)
		{
			str16.resize(d);
			utf8::utf8to16(str, str+len, &str16[0]);
		}
	}
	catch(utf8::invalid_utf8& ex)
	{
		utf8::uint8_t c = ex.utf8_octet();
		//VFS_THROW( _BS(L"Invalid UTF8 character '") << (wchar_t)c << L"'=" << (unsigned char)c << _BS::wget );
		VFS_THROW(_BS(L"Invalid UTF8 character '") << (wchar_t)c << L"'=" << (unsigned char)c << L" str16:" << buf16 << _BS::wget);
	}
	catch(utf8::not_enough_room &ex)
	{
		std::wstring err;
		VFS_IGNOREEXCEPTION(vfs::String::as_utf16(ex.what(), err), true);
		VFS_THROW( _BS(L"Incomplete UTF8 string [") << err << L"]" << _BS::wget );
	}
}

std::string vfs::String::as_utf8(vfs::String const& str)
{
	return str.utf8();
}
std::string vfs::String::as_utf8(std::wstring const& str)
{
	if(str.empty())
	{
		return "";
	}
#if 0
	std::string s;
	utf8::utf16to8(str.begin(), str.end(), std::back_inserter(s));
#else
	const int UTF8_MAX_CHARS = 4;
	std::vector<char> buffer(str.length()+UTF8_MAX_CHARS);
	::size_t pos_in=0, pos_out=0;
	while(pos_in < str.length())
	{
		if( (pos_out+UTF8_MAX_CHARS) >= buffer.size() )
		{
			buffer.resize(buffer.size()+str.length());
		}
		pos_out = utf8::append(str.at(pos_in++), &buffer[pos_out]) - &buffer[0];
	}
	std::string s(&buffer[0],pos_out);
#endif
	return s;
}

// if 'strlength' is 0, length is determined automatically
std::string vfs::String::as_utf8(const wchar_t* str, vfs::String::size_t strlength)
{
	std::string s;
	if(str != NULL)
	{
		::size_t len = strlength;
		if(len == 0)
		{
			len = wcslen(str);
		}
		if(len != 0)
		{
			utf8::utf16to8(str,str+len,std::back_inserter(s));
		}
	}
	return s;
}

// convenience method, for the case it should be used in generic code (overloading)
std::string vfs::String::as_utf8(std::string const& str)
{
	return str;
}

vfs::String::size_t vfs::String::narrow(std::wstring const& src, std::string& dst)
{
	vfs::String::size_t len = vfs::String::narrow(src.c_str(),src.length(),NULL,0);
	if(len > 0)
	{
		dst.resize(len);
		return vfs::String::narrow(src.c_str(),src.length(),&dst[0],len);
	}
	return 0;
}
std::string vfs::String::narrow(wchar_t const* str, vfs::String::size_t length)
{
	std::string s;
	vfs::String::size_t len = vfs::String::narrow(str,length,NULL,0);
	if(len > 0)
	{
		s.resize(len);
		vfs::String::narrow(str,length,&s[0],len);
	}
	return s;
}
vfs::String::size_t vfs::String::narrow(wchar_t const* src_str, vfs::String::size_t src_len, char* dst_str, vfs::String::size_t dst_len)
{
	if(src_str && src_len>0)
	{
		if(!dst_str || dst_len==0)
		{
			return wcstombs(NULL, src_str, src_len);
		}
		return wcstombs(dst_str, src_str, std::min<vfs::String::size_t>(src_len,dst_len));
	}
	return 0;
}
// 
std::wstring vfs::String::widen(char const* str, vfs::String::size_t length)
{
	std::wstring ws;
	vfs::String::size_t len = vfs::String::widen(str,length,NULL,0);
	if(len > 0)
	{
		ws.resize(len);
		vfs::String::widen(str,length,&ws[0],len);
	}
	return ws;
}
vfs::String::size_t vfs::String::widen(std::string const& src, std::wstring& dst)
{
	vfs::String::size_t len = vfs::String::widen(src.c_str(),src.length(),NULL,0);
	if(len > 0)
	{
		dst.resize(len);
		return vfs::String::widen(src.c_str(),src.length(),&dst[0],len);
	}
	return 0;
}
vfs::String::size_t vfs::String::widen(char const* src_str, size_t src_len, wchar_t* dst_str, size_t dst_len)
{
	if(src_str && src_len>0)
	{
		if(!dst_str || dst_len==0)
		{
			return mbstowcs(NULL, src_str, src_len);
		}
		return mbstowcs(dst_str, src_str, std::min<size_t>(src_len,dst_len));
	}
	return 0;
}


////////////////////////////////////////////////////////////////////
bool vfs::String::empty() const
{
	return _str.empty();
}
vfs::String::size_t vfs::String::length() const
{
	return _str.length();
}

vfs::String vfs::String::operator+(vfs::String const& str)
{
	return vfs::String(_str + str._str);
}
vfs::String vfs::String::operator+=(vfs::String const& str)
{
	_str += str._str;
	return vfs::String(_str);
}

////////////////////////////////////////////////////////////////

bool vfs::operator<(vfs::String const& s1, vfs::String const& s2)
{
	return s1._str < s2._str;
}

std::wostream& operator<<(std::wostream& out, vfs::String const& str)
{
	out.write(str.c_str(), (std::streamsize)str.length());
	return out;
}

std::wostream& operator<<(std::wostream& out, vfs::String::str_t const& str)
{
	out.write(str.c_str(), (std::streamsize)str.length());
	return out;
}

std::wostream& operator<<(std::wostream& out, const vfs::String::char_t* str)
{
	out.write(str, (std::streamsize)wcslen(str));
	return out;
}

/*****************************************************************************************/
/*****************************************************************************************/

// case IN-sensitive
bool vfs::StrCmp::Equal(const char* s1, const char* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool vfs::StrCmp::Equal(std::string const& s1, std::string const& s2)
{
	const char* p1 = s1.c_str();
	const char* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::Equal(std::string const& s1, const char* s2)
{
	const char* p1 = s1.c_str();
	_StrCmp::Advance(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool vfs::StrCmp::Equal(const char* s1, std::string const& s2)
{
	const char* p2 = s2.c_str();
	_StrCmp::Advance(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool vfs::StrCmp::Equal(const wchar_t* s1, const wchar_t* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool vfs::StrCmp::Equal(std::wstring const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::Equal(std::wstring const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::Advance(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool vfs::StrCmp::Equal(const wchar_t* s1, std::wstring const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool vfs::StrCmp::Equal(vfs::String const& s1, vfs::String const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::Equal(vfs::String const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::Equal(std::wstring const& s1, vfs::String const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::Equal(vfs::String const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::Advance(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool vfs::StrCmp::Equal(const wchar_t* s1, vfs::String const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
// case Sensitive
bool vfs::StrCmp::EqualCase(const char* s1, const char* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool vfs::StrCmp::EqualCase(std::string const& s1, std::string const& s2)
{
	const char* p1 = s1.c_str();
	const char* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::EqualCase(std::string const& s1, const char* s2)
{
	const char* p1 = s1.c_str();
	_StrCmp::AdvanceCase(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool vfs::StrCmp::EqualCase(const char* s1, std::string const& s2)
{
	const char* p2 = s2.c_str();
	_StrCmp::AdvanceCase(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool vfs::StrCmp::EqualCase(const wchar_t* s1, const wchar_t* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool vfs::StrCmp::EqualCase(std::wstring const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::EqualCase(std::wstring const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::AdvanceCase(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool vfs::StrCmp::EqualCase(const wchar_t* s1, std::wstring const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool vfs::StrCmp::EqualCase(vfs::String const& s1, vfs::String const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::EqualCase(vfs::String const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::EqualCase(std::wstring const& s1, vfs::String const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool vfs::StrCmp::EqualCase(vfs::String const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::AdvanceCase(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool vfs::StrCmp::EqualCase(const wchar_t* s1, vfs::String const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(s1,p2);
	return _StrCmp::Equal(s1,p2);
}

/*****************************************************************************************/
/*****************************************************************************************/

template<>
BuildString& BuildString::add<vfs::String>(vfs::String const& value)
{
	this->add(value.c_str());
	return *this;
}
template<>
BuildString& BuildString::add<std::string>(std::string const& value)
{
	this->add(vfs::String::as_utf16(value));
	return *this;
}
template<>
BuildString& BuildString::add<const char*>(const char* const& value)
{
	this->add(vfs::String::as_utf16(value));
	return *this;
}

template<>
BuildString& BuildString::operator<< <vfs::String>(vfs::String const& value)
{
	this->add(value.c_str());
	return *this;
}
template<>
BuildString& BuildString::operator<< <std::string>(std::string const& value)
{
	this->add(vfs::String::as_utf16(value));
	return *this;
}
template<>
BuildString& BuildString::operator<< <const char*>(const char* const& value)
{
	this->add(vfs::String::as_utf16(value));
	return *this;
}

