#include "Tools.h"

template<>
utf8string ToStringList<utf8string>(std::list<utf8string> const& rValList)
{
	std::wstringstream ss;
	std::list<utf8string>::const_iterator cit = rValList.begin();
	if(cit != rValList.end())
	{
		ss << (*cit);
		cit++;
		for(;cit != rValList.end(); ++cit)
		{
			ss << L" , " << (*cit);
		}
	}
	if(!ss)
	{
		return L"";
	}
	return ss.str();
}
