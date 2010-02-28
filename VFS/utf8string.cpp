#include "utf8string.h"
#include "utf8.h"
#include <vector>
#include <cstdlib>
#include <cstring>

#include "vfs_debug.h"
////////////////////////////////////////////////////////////////////

namespace _StrCmp
{
	////////////////////////////////////////////////////////////////
	inline void Advance( const char*& s1, const char*& s2 )
	{
		while (*s1 && *s2 && toupper(*s1) == toupper(*s2))
		{
			++s1;
			++s2;
		}
	}
	inline void Advance( const wchar_t*& s1, const wchar_t*& s2 )
	{
		// should be 'towupper'
		while (*s1 && *s2 && toupper(*s1) == toupper(*s2))
		{
			++s1;
			++s2;
		}	
	}
	template<typename CharType>
	inline void AdvanceCase( const CharType*& s1, const CharType*& s2 )
	{
		while (*s1 && *s2 && (*s1 == *s2))
		{
			++s1;
			++s2;
		}	
	}
	////////////////////////////////////////////////////////////////
	template<typename CharType>
	inline bool Equal( const CharType* s1, const CharType* s2 )
	{
		return !(*s1 || *s2);
	}
	////////////////////////////////////////////////////////////////
	inline bool Less( const wchar_t* s1, const wchar_t* s2 )
	{
		if (!*s1) return *s2 != 0;
		if (!*s2) return false;
		// should be 'towupper'
		return toupper(*s1) < toupper(*s2);
	}
	inline bool Less( const char* s1, const char* s2 )
	{
		if (!*s1) return *s2 != 0;
		if (!*s2) return false;
		return toupper(*s1) < toupper(*s2);
	}
	template<typename CharType>
	inline bool LessCase( const CharType* s1, const CharType* s2 )
	{
		if (!*s1) return *s2 != 0;
		if (!*s2) return false;
		return (*s1) < (*s2);
	}
}

////////////////////////////////////////////////////////////////////

bool utf8string::lessCase(const utf8string::char_t* s1, const utf8string::char_t* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::LessCase(s1,s2);

}

bool utf8string::less(const utf8string::char_t* s1, const utf8string::char_t* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Less(s1,s2);
}

bool utf8string::equalCase(const utf8string::char_t* s1, const utf8string::char_t* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::Equal(s1,s2);
}

bool utf8string::equal(const utf8string::char_t* s1, const utf8string::char_t* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Equal(s1,s2);
}

////////////////////////////////////////////////////////////////////
///	                    class constructors
////////////////////////////////////////////////////////////////////
utf8string::utf8string()
{
}
utf8string::utf8string(const char* str)
{
	utf8string::as_utf16(str,_str);
}
utf8string::utf8string(std::string const& str)
{
	utf8string::as_utf16(str,_str);
}
utf8string::utf8string(const wchar_t* str)
{
	_str.assign(str);
}
utf8string::utf8string(std::wstring const& str)
{
	_str.assign(str);
}

////////////////////////////////////////////////////////////////////
///	                  static class methods
////////////////////////////////////////////////////////////////////
utf8string::str_t utf8string::as_utf16(std::string const& str)
{
	utf8string::str_t s;
	utf8string::as_utf16(str, s);
	return s;
}
void utf8string::as_utf16(std::string const& str, utf8string::str_t &str16)
{
	try
	{
		::size_t d = utf8::distance(str.begin(), str.end());
		str16.resize(d);
		utf8::utf8to16(str.begin(), str.end(), &str16[0]);
	}
	catch(utf8::invalid_utf8& ex)
	{
		utf8::uint8_t c = ex.utf8_octet();
		THROWEXCEPTION(BuildString().add(L"Invalid UTF8 character '").add((wchar_t)c).add(L"'=").add((unsigned char)c).get());
	}
	catch(utf8::not_enough_room &ex)
	{
		std::wstring err;
		IGNOREEXCEPTION(utf8string::as_utf16(ex.what(),err));
		THROWEXCEPTION(BuildString().add(L"Incomplete UTF8 string [").add(err).add(L"]").get());
	}
}

utf8string::str_t utf8string::as_utf16(const char* str)
{
	utf8string::str_t s;
	utf8string::as_utf16(str, s);
	return s;
}
void utf8string::as_utf16(const char* str, utf8string::str_t &str16)
{
	if(str == NULL)
	{
		return;
	}
	try
	{
		::size_t len = strlen(str);
		::size_t d = utf8::distance(str,str+len);
		str16.resize(d);
		utf8::utf8to16(str, str+len, &str16[0]);
	}
	catch(utf8::invalid_utf8& ex)
	{
		utf8::uint8_t c = ex.utf8_octet();
		THROWEXCEPTION(BuildString().add(L"Invalid UTF8 character '").add((wchar_t)c).add(L"'=").add((unsigned char)c).get());
	}
	catch(utf8::not_enough_room &ex)
	{
		std::wstring err;
		IGNOREEXCEPTION(utf8string::as_utf16(ex.what(), err));
		THROWEXCEPTION(BuildString().add(L"Incomplete UTF8 string [").add(err).add(L"]").get());
	}
}

std::string utf8string::as_utf8(utf8string const& str)
{
	return str.utf8();
}
std::string utf8string::as_utf8(std::wstring const& str)
{
	if(str.empty())
	{
		return "";
	}
#if 0
	std::string s;
	utf8::utf16to8(str.begin(), str.end(), std::back_inserter(s));
#else
	static std::vector<char> buffer;
	buffer.resize(std::max< ::size_t>(buffer.size(), str.length()*3));
	char* end = utf8::utf16to8(str.begin(), str.end(), &buffer[0]);
	std::string s(&buffer[0],end);
#endif
	return s;
}

// if 'strlength' is 0, length is determined automatically
std::string utf8string::as_utf8(const wchar_t* str, utf8string::size_t strlength)
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
std::string utf8string::as_utf8(std::string const& str)
{
	return str;
}

utf8string::size_t utf8string::narrow(std::wstring const& src, std::string& dst)
{
	utf8string::size_t len = utf8string::narrow(src.c_str(),src.length(),NULL,0);
	dst.resize(len);
	return utf8string::narrow(src.c_str(),src.length(),&dst[0],len);
}
std::string utf8string::narrow(wchar_t const* str, utf8string::size_t length)
{
	utf8string::size_t len = utf8string::narrow(str,length,NULL,0);
	std::string s;
	s.resize(len);
	utf8string::narrow(str,length,&s[0],len);
	return s;
}
utf8string::size_t utf8string::narrow(wchar_t const* src_str, utf8string::size_t src_len, char* dst_str, utf8string::size_t dst_len)
{
	if(src_str && src_len>0)
	{
		if(!dst_str || dst_len==0)
		{
			return wcstombs(NULL, src_str, src_len);
		}
		return wcstombs(dst_str, src_str, std::min<utf8string::size_t>(src_len,dst_len));
	}
	return 0;
}
// 
std::wstring utf8string::widen(char const* str, utf8string::size_t length)
{
	utf8string::size_t len = utf8string::widen(str,length,NULL,0);
	std::wstring ws;
	ws.resize(len);
	utf8string::widen(str,length,&ws[0],len);
	return ws;
}
utf8string::size_t utf8string::widen(std::string const& src, std::wstring& dst)
{
	utf8string::size_t len = utf8string::widen(src.c_str(),src.length(),NULL,0);
	dst.resize(len);
	return utf8string::widen(src.c_str(),src.length(),&dst[0],len);
}
utf8string::size_t utf8string::widen(char const* src_str, size_t src_len, wchar_t* dst_str, size_t dst_len)
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
bool utf8string::empty() const
{
	return _str.empty();
}
utf8string::size_t utf8string::length() const
{
	return _str.length();
}

utf8string utf8string::operator+(utf8string const& str)
{
	return utf8string(_str + str._str);
}
utf8string utf8string::operator+=(utf8string const& str)
{
	_str += str._str;
	return utf8string(_str);
}

////////////////////////////////////////////////////////////////

bool operator<(utf8string const& s1, utf8string const& s2)
{
	return s1._str < s2._str;
}

std::wstringstream& operator<<(std::wstringstream& out, utf8string const& str)
{
	out.write(str.c_str(), (std::streamsize)str.length());
	return out;
}

std::wstringstream& operator<<(std::wstringstream& out, utf8string::str_t const& str)
{
	out.write(str.c_str(), (std::streamsize)str.length());
	return out;
}

std::wstringstream& operator<<(std::wstringstream& out, const utf8string::char_t* str)
{
	out.write(str, (std::streamsize)wcslen(str));
	return out;
}

/*****************************************************************************************/
/*****************************************************************************************/

// case IN-sensitive
bool StrCmp::Equal(const char* s1, const char* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool StrCmp::Equal(std::string const& s1, std::string const& s2)
{
	const char* p1 = s1.c_str();
	const char* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::Equal(std::string const& s1, const char* s2)
{
	const char* p1 = s1.c_str();
	_StrCmp::Advance(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool StrCmp::Equal(const char* s1, std::string const& s2)
{
	const char* p2 = s2.c_str();
	_StrCmp::Advance(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool StrCmp::Equal(const wchar_t* s1, const wchar_t* s2)
{
	_StrCmp::Advance(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool StrCmp::Equal(std::wstring const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::Equal(std::wstring const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::Advance(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool StrCmp::Equal(const wchar_t* s1, std::wstring const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool StrCmp::Equal(utf8string const& s1, utf8string const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::Equal(utf8string const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::Equal(std::wstring const& s1, utf8string const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::Equal(utf8string const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::Advance(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool StrCmp::Equal(const wchar_t* s1, utf8string const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::Advance(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
// case Sensitive
bool StrCmp::EqualCase(const char* s1, const char* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool StrCmp::EqualCase(std::string const& s1, std::string const& s2)
{
	const char* p1 = s1.c_str();
	const char* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::EqualCase(std::string const& s1, const char* s2)
{
	const char* p1 = s1.c_str();
	_StrCmp::AdvanceCase(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool StrCmp::EqualCase(const char* s1, std::string const& s2)
{
	const char* p2 = s2.c_str();
	_StrCmp::AdvanceCase(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool StrCmp::EqualCase(const wchar_t* s1, const wchar_t* s2)
{
	_StrCmp::AdvanceCase(s1,s2);
	return _StrCmp::Equal(s1,s2);
}
bool StrCmp::EqualCase(std::wstring const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::EqualCase(std::wstring const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::AdvanceCase(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool StrCmp::EqualCase(const wchar_t* s1, std::wstring const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(s1,p2);
	return _StrCmp::Equal(s1,p2);
}
//
bool StrCmp::EqualCase(utf8string const& s1, utf8string const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::EqualCase(utf8string const& s1, std::wstring const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::EqualCase(std::wstring const& s1, utf8string const& s2)
{
	const wchar_t* p1 = s1.c_str();
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(p1,p2);
	return _StrCmp::Equal(p1,p2);
}
bool StrCmp::EqualCase(utf8string const& s1, const wchar_t* s2)
{
	const wchar_t* p1 = s1.c_str();
	_StrCmp::AdvanceCase(p1,s2);
	return _StrCmp::Equal(p1,s2);
}
bool StrCmp::EqualCase(const wchar_t* s1, utf8string const& s2)
{
	const wchar_t* p2 = s2.c_str();
	_StrCmp::AdvanceCase(s1,p2);
	return _StrCmp::Equal(s1,p2);
}


