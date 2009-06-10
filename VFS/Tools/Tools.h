#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "../utf8string.h"
#include <list>

template<typename CharType, typename ValueType>
std::basic_string<CharType> ToString(ValueType const& rVal) 
{
	std::basic_stringstream<CharType> tss;
	if( !(tss << rVal))
	{
		return std::basic_string<CharType>();
	}
	return tss.str();
}

template<typename T_>
bool ConvertTo(utf8string const& sStr, T_ &rVal)
{
	std::wstringstream ss;
	ss.str(sStr.c_wcs());
	if(!(ss >> rVal))
	{
		return false;
	}
	return true;
}

template<typename T_>
utf8string ToStringList(std::list<T_> const& rValList)
{
	std::wstringstream ss;
	typename std::list<T_>::const_iterator cit = rValList.begin();
	if(cit != rValList.end())
	{
		ss << *cit;
		cit++;
		for(;cit != rValList.end(); ++cit)
		{
			ss << L" , " << *cit;
		}
	}
	if(!ss)
	{
		return L"";
	}
	return ss.str();
}

template<>
utf8string ToStringList<utf8string>(std::list<utf8string> const& rValList);

#endif // _TOOLS_H_
