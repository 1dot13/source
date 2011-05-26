/* 
 * bfVFS : vfs/Core/vfs_string.h
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

#ifndef _VFS_STRING_H_
#define _VFS_STRING_H_

#include <vfs/vfs_config.h>
#include <string>
#include <sstream>

namespace vfs
{
	// simple UTF8 wrapper, uses utf8 implementation from http://utfcpp.sourceforge.net/
	class VFS_API String
	{
		friend VFS_API bool operator<(vfs::String const& s1, vfs::String const& s2);
	public:
		typedef std::wstring		str_t;
		typedef str_t::value_type	char_t;
		typedef str_t::value_type*	ptr_t;
		
		typedef str_t::size_type	size_t;
		////////////////////////////////////////////////////////////////////
		static bool less(const vfs::String::char_t* s1, const vfs::String::char_t* s2);
		static bool equal(const vfs::String::char_t* s1, const vfs::String::char_t* s2);
		// case sensitive
		static bool lessCase(const vfs::String::char_t*  s1, const vfs::String::char_t* s2);
		static bool equalCase(const vfs::String::char_t* s1, const vfs::String::char_t* s2);

		template<bool (*funName)(const vfs::String::char_t* s1, const vfs::String::char_t* s2)> 
		class Op{
		public:
			bool operator()(const vfs::String& s1, const vfs::String& s2) const
			{
				return funName(s1.c_str(),s2.c_str());
			}
		};
		typedef Op<vfs::String::less>		Less;
		typedef Op<vfs::String::lessCase>	LessCase;
		typedef Op<vfs::String::equal>		Equal;
		typedef Op<vfs::String::equalCase>	EqualCase;
	public:
		String();
		String(const char* str);
		String(std::string const& str);
		String(const wchar_t* str);
		String(std::wstring const& str);

		////////////////////////////////////////////////////////////////////
		static vfs::String::str_t	as_utf16(const char* str);
		static void					as_utf16(const char* str, vfs::String::str_t &str16);
		static vfs::String::str_t	as_utf16(std::string const& str);
		static void					as_utf16(std::string const& str, vfs::String::str_t &str16);
		// fast conversion without creating an internal copy
		static std::string			as_utf8(vfs::String const& str);
		static std::string			as_utf8(std::wstring const& str);
		// if 'strlen' is 0, length is determined automatically
		static std::string			as_utf8(const wchar_t* str, vfs::String::size_t strlength=0);
		// 
		static std::string			narrow(wchar_t const* str, vfs::String::size_t length);
		static vfs::String::size_t	narrow(wchar_t const* src_str, vfs::String::size_t src_len, char* dst_str, vfs::String::size_t dst_len);
		static vfs::String::size_t	narrow(std::wstring const& src, std::string& dst);
		// 
		static std::wstring			widen(char const* str, vfs::String::size_t length);
		static vfs::String::size_t	widen(char const* src_str, vfs::String::size_t src_len, wchar_t* dst_str, vfs::String::size_t dst_len);
		static vfs::String::size_t	widen(std::string const& src, std::wstring& dst);

		// convenience method, for the case it should be used in generic code (overloading)
		static std::string			as_utf8(std::string const& str);

		////////////////////////////////////////////////////////////////////
		// convert string to UTF8 encoding
		inline std::string			utf8() const	{ return as_utf8(_str); }

		// returns const reference to copy or compare string
		inline std::wstring const&	c_wcs() const	{ return _str; }

		inline const wchar_t*		c_str() const	{ return _str.c_str(); }

		// returns reference to modify string
		inline std::wstring&		r_wcs()			{ return _str; }


		////////////////////////////////////////////////////////////////////
		bool						empty() const;
		vfs::String::size_t			length() const;

		////////////////////////////////////////////////////////////////////
		vfs::String operator+(vfs::String const& str);
		vfs::String operator+=(vfs::String const& str);
	private:
		str_t	_str;
	};

} // namespace vfs

namespace vfs
{
	VFS_API bool			operator<(vfs::String const& s1, vfs::String const& s2);
}

VFS_API std::wostream& operator<<(std::wostream& out, vfs::String const& str);
VFS_API std::wostream& operator<<(std::wostream& out, vfs::String::str_t const& str);
VFS_API std::wostream& operator<<(std::wostream& out, const vfs::String::char_t* str);

namespace vfs
{
	// explicit compare
	namespace StrCmp
	{
		// case IN-sensitive
		VFS_API bool Equal(const char* s1, const char* s2);
		VFS_API bool Equal(std::string const& s1, std::string const& s2);
		VFS_API bool Equal(std::string const& s1, const char* s2);
		VFS_API bool Equal(const char* s1, std::string const& s2);
		//
		VFS_API bool Equal(const wchar_t* s1, const wchar_t* s2);
		VFS_API bool Equal(std::wstring const& s1, std::wstring const& s2);
		VFS_API bool Equal(std::wstring const& s1, const wchar_t* s2);
		VFS_API bool Equal(const wchar_t* s1, std::wstring const& s2);
		//
		VFS_API bool Equal(vfs::String const& s1, vfs::String const& s2);
		VFS_API bool Equal(vfs::String const& s1, std::wstring const& s2);
		VFS_API bool Equal(std::wstring const& s1, vfs::String const& s2);
		VFS_API bool Equal(vfs::String const& s1, const wchar_t* s2);
		VFS_API bool Equal(const wchar_t* s1, vfs::String const& s2);
		// case Sensitive
		VFS_API bool EqualCase(const char* s1, const char* s2);
		VFS_API bool EqualCase(std::string const& s1, std::string const& s2);
		VFS_API bool EqualCase(std::string const& s1, const char* s2);
		VFS_API bool EqualCase(const char* s1, std::string const& s2);
		//
		VFS_API bool EqualCase(const wchar_t* s1, const wchar_t* s2);
		VFS_API bool EqualCase(std::wstring const& s1, std::wstring const& s2);
		VFS_API bool EqualCase(std::wstring const& s1, const wchar_t* s2);
		VFS_API bool EqualCase(const wchar_t* s1, std::wstring const& s2);
		//
		VFS_API bool EqualCase(vfs::String const& s1, vfs::String const& s2);
		VFS_API bool EqualCase(vfs::String const& s1, std::wstring const& s2);
		VFS_API bool EqualCase(std::wstring const& s1, vfs::String const& s2);
		VFS_API bool EqualCase(vfs::String const& s1, const wchar_t* s2);
		VFS_API bool EqualCase(const wchar_t* s1, vfs::String const& s2);
	}

} // namespace vfs

class VFS_API BuildString
{
public:
	enum _cget{cget};
	enum _wget{wget};
	BuildString()
	{}

	std::string operator<<(_cget)
	{
		return vfs::String::as_utf8(_strstr.str());
	}
	std::wstring operator<<(_wget)
	{
		return _strstr.str();
	}

	BuildString(BuildString const& bs)
	{
		_strstr.str(bs._strstr.str());
	}

	template<typename T>
	BuildString(T const& t)
	{
		add(t);
	}
	template<typename T>
	BuildString&				add(T const& value)
	{
		_strstr << value;
		return *this;
	}
	template<typename T>
	BuildString&				operator<<(T const& value)
	{
		_strstr << value;
		return *this;
	}

	vfs::String::str_t			get()
	{
		return _strstr.str();
	}
private:
	std::basic_stringstream<vfs::String::char_t> _strstr;
};

typedef BuildString _BS;

template<>
VFS_API BuildString& BuildString::add<vfs::String>(vfs::String const& value);
template<>
VFS_API BuildString& BuildString::add<std::string>(std::string const& value);
template<>
VFS_API BuildString& BuildString::add<const char*>(const char* const& value);

template<>
VFS_API BuildString& BuildString::operator<< <vfs::String>(vfs::String const& value);
template<>
VFS_API BuildString& BuildString::operator<< <std::string>(std::string const& value);
template<>
VFS_API BuildString& BuildString::operator<< <const char*>(const char* const& value);


#endif // _VFS_STRING_H_
