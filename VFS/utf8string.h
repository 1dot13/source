#ifndef _UTF8STRING_H_
#define _UTF8STRING_H_

#include <string>
#include <sstream>

// simple UTF8 wrapper, uses utf8 implementation from http://utfcpp.sourceforge.net/
class utf8string
{
	friend bool operator<(utf8string const& s1, utf8string const& s2);
	friend class StrAccess;
public:
	typedef std::wstring				str_t;
	typedef std::wstring::value_type	char_t;
	typedef std::wstring::value_type*	ptr_t;
	
	typedef std::wstring::size_type		size_t;
	////////////////////////////////////////////////////////////////////
	static bool less(utf8string const& s1, const utf8string& s2);
	static bool equal(utf8string const& s1, const utf8string& s2);
	static bool equal(utf8string const& s1, const utf8string::str_t& s2);
	static bool equal(utf8string const& s1, const utf8string::char_t* s2);
	// case sensitive
	static bool lessCase(utf8string const& s1, utf8string const& s2);
	static bool equalCase(utf8string const& s1, const utf8string& s2);
	static bool equalCase(utf8string const& s1, const utf8string::str_t& s2);
	static bool equalCase(utf8string const& s1, const utf8string::char_t* s2);

	template<bool (*funName)(utf8string const& s1, utf8string const& s2)> 
	class Op{
	public:
		bool operator()(utf8string const& s1, utf8string const& s2) const
		{
			return funName(s1,s2);
		}
	};
	typedef Op<utf8string::less>		Less;
	typedef Op<utf8string::lessCase>	LessCase;
	typedef Op<utf8string::equal>		Equal;
	typedef Op<utf8string::equalCase>	EqualCase;
public:
	utf8string();
	utf8string(const char* str);
	utf8string(std::string const& str);
	utf8string(const wchar_t* str);
	utf8string(std::wstring const& str);

	////////////////////////////////////////////////////////////////////
	static utf8string::str_t	as_utf16(const char* str);
	static void					as_utf16(const char* str, utf8string::str_t &str16);
	static utf8string::str_t	as_utf16(std::string const& str);
	static void					as_utf16(std::string const& str, utf8string::str_t &str16);
	// fast conversion without creating an internal copy
	static std::string			as_utf8(utf8string const& str);
	static std::string			as_utf8(std::wstring const& str);
	// if 'strlen' is 0, length is determined automatically
	static std::string			as_utf8(const wchar_t* str, unsigned int strlength=0);
	// 
	static std::string			narrow(wchar_t const* str, size_t length);
	static size_t				narrow(wchar_t const* src_str, size_t src_len, char* dst_str, size_t dst_len);
	static size_t				narrow(std::wstring const& src, std::string& dst);
	// 
	static std::wstring			widen(char const* str, size_t length);
	static size_t				widen(char const* src_str, size_t src_len, wchar_t* dst_str, size_t dst_len);
	static size_t				widen(std::string const& src, std::wstring& dst);

	// convenience method, for the case it should be used in generic code (overloading)
	static std::string	as_utf8(std::string const& str);

	////////////////////////////////////////////////////////////////////
	// convert string to UTF8 encoding
	inline std::string			utf8() const;

	// returns const reference to copy or compare string
	inline std::wstring const&	c_wcs() const;

	// returns reference to modify string
	std::wstring&				r_wcs();

	////////////////////////////////////////////////////////////////////
	bool						empty() const;
	inline utf8string::size_t	length() const;

	////////////////////////////////////////////////////////////////////
	utf8string operator+(utf8string const& str);
	utf8string operator+=(utf8string const& str);
private:
	std::wstring		_str;
};

bool				operator<(utf8string const& s1, utf8string const& s2);
std::wstringstream& operator<<(std::wstringstream& out, utf8string const& str);
std::wstringstream& operator<<(std::wstringstream& out, utf8string::str_t const& str);
std::wstringstream& operator<<(std::wstringstream& out, const utf8string::char_t* str);

// explicit compare
namespace StrCmp
{
	// case IN-sensitive
	bool Equal(const char* s1, const char* s2);
	bool Equal(std::string const& s1, std::string const& s2);
	bool Equal(std::string const& s1, const char* s2);
	bool Equal(const char* s1, std::string const& s2);
	//
	bool Equal(const wchar_t* s1, const wchar_t* s2);
	bool Equal(std::wstring const& s1, std::wstring const& s2);
	bool Equal(std::wstring const& s1, const wchar_t* s2);
	bool Equal(const wchar_t* s1, std::wstring const& s2);
	//
	bool Equal(utf8string const& s1, utf8string const& s2);
	bool Equal(utf8string const& s1, std::wstring const& s2);
	bool Equal(std::wstring const& s1, utf8string const& s2);
	bool Equal(utf8string const& s1, const wchar_t* s2);
	bool Equal(const wchar_t* s1, utf8string const& s2);
	// case Sensitive
	bool EqualCase(const char* s1, const char* s2);
	bool EqualCase(std::string const& s1, std::string const& s2);
	bool EqualCase(std::string const& s1, const char* s2);
	bool EqualCase(const char* s1, std::string const& s2);
	//
	bool EqualCase(const wchar_t* s1, const wchar_t* s2);
	bool EqualCase(std::wstring const& s1, std::wstring const& s2);
	bool EqualCase(std::wstring const& s1, const wchar_t* s2);
	bool EqualCase(const wchar_t* s1, std::wstring const& s2);
	//
	bool EqualCase(utf8string const& s1, utf8string const& s2);
	bool EqualCase(utf8string const& s1, std::wstring const& s2);
	bool EqualCase(std::wstring const& s1, utf8string const& s2);
	bool EqualCase(utf8string const& s1, const wchar_t* s2);
	bool EqualCase(const wchar_t* s1, utf8string const& s2);
}
#endif // _UTF8STRING_H_
