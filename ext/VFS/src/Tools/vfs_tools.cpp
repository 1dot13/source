/* 
 * bfVFS : vfs/Tools/vfs_tools.cpp
 *  - simple from/to string (list) conversion functions, 
 *  - remove leading/trailing whitspace characters in a string
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

#include <vfs/Tools/vfs_tools.h>

template<>
vfs::String vfs::toStringList<vfs::String>(std::list<vfs::String> const& rValList)
{
	std::wstringstream ss;
	std::list<vfs::String>::const_iterator cit = rValList.begin();
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

/*************************************************************************************/
/*************************************************************************************/

template<>
std::string vfs::trimString<std::string>(std::string const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos)
{
	if(iMinPos > iMaxPos || iMaxPos == vfs::npos)
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
	if(iMinPos > iMaxPos || iMaxPos == vfs::npos)
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
vfs::String vfs::trimString<vfs::String>(vfs::String const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos)
{
	return vfs::trimString(sStr.c_wcs(), iMinPos, iMaxPos);
}
