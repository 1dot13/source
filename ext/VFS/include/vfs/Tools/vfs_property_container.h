/* 
 * bfVFS : vfs/Tools/vfs_property_container.h 
 *  - <string,string> key-value map with capability to convert values to other types
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

#ifndef _VFS_PROPERTY_CONTAINER_H_
#define _VFS_PROPERTY_CONTAINER_H_

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/Interface/vfs_file_interface.h>

#include <map>
#include <string>
#include <list>
#include <set>
#include <ostream>

namespace vfs
{
	class PropertyContainer
	{
	public:
		class TagMap
		{
			typedef std::map<vfs::String,vfs::String> tTagMap;
		public:
			TagMap();
			vfs::String const& container(vfs::String::char_t* container = NULL);
			vfs::String const& section(vfs::String::char_t* section = NULL);
			vfs::String const& sectionID(vfs::String::char_t* section_id = NULL);
			vfs::String const& key(vfs::String::char_t* key = NULL);
			vfs::String const& keyID(vfs::String::char_t* key_id = NULL);
		private:
			tTagMap _map;
		};
	public:
		VFS_API PropertyContainer(){};
		VFS_API ~PropertyContainer(){};
		
		VFS_API void			clearContainer();

		VFS_API bool			initFromIniFile(vfs::Path const& sFileName);
		VFS_API bool			initFromIniFile(vfs::tReadableFile *pFile);
		VFS_API bool			writeToIniFile(vfs::Path const& sFileName, bool bCreateNew = false);

		// these functions are not implemented
		bool					initFromXMLFile(vfs::Path const& sFileName, TagMap& tagmap);
		bool					writeToXMLFile(vfs::Path const& sFileName, TagMap& tagmap);

		VFS_API void			printProperties(std::ostream &out);

		VFS_API bool			hasProperty(vfs::String const& sSection, vfs::String const& sKey);
		//
		VFS_API vfs::String const& getStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String const& sDefaultValue=L"");
		VFS_API bool			getStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String& sValue, vfs::String const& sDefaultValue=L"");
		VFS_API bool			getStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String::char_t* sValue, vfs::size_t len, vfs::String const& sDefaultValue=L"");
		//
		VFS_API vfs::Int64		getIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::Int64 iDefaultValue);
		VFS_API vfs::Int64		getIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::Int64 iDefaultValue, vfs::Int64 iMinValue, vfs::Int64 iMaxValue);
		//
		VFS_API vfs::UInt64		getUIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::UInt64 iDefaultValue);
		VFS_API vfs::UInt64		getUIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::UInt64 iDefaultValue, vfs::UInt64 iMinValue, vfs::UInt64 iMaxValue);
		//
		VFS_API double			getFloatProperty(vfs::String const& sSection, vfs::String const& sKey, double fDefaultValue);
		VFS_API double			getFloatProperty(vfs::String const& sSection, vfs::String const& sKey, double fDefaultValue, double fMinValue, double fMaxValue);
		//
		VFS_API bool			getBoolProperty(vfs::String const& sSection, vfs::String const& sKey, bool bDefaultValue);
		//
		VFS_API bool			getStringListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::String> &lValueList, vfs::String sDefaultValue);
		VFS_API bool			getIntListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::Int64> &lValueList, vfs::Int64 iDefaultValue);
		VFS_API bool			getUIntListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::UInt64> &lValueList, vfs::UInt64 iDefaultValue);
		VFS_API bool			getFloatListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<double> &lValueList, double fDefaultValue);
		VFS_API bool			getBoolListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<bool> &lValueList, bool bDefaultValue);
		//
		VFS_API void			setStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String const&  sValue);
		//
		VFS_API void			setIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::Int64 const&  iValue);
		VFS_API void			setUIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::UInt64 const&  iValue);
		VFS_API void			setFloatProperty(vfs::String const& sSection, vfs::String const& sKey, double const& fValue);
		VFS_API void			setBoolProperty(vfs::String const& sSection, vfs::String const& sKey, bool const& bValue);
		//
		VFS_API void			setStringListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::String> const&  slValue);
		VFS_API void			setIntListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::Int64> const& ilValue);
		VFS_API void			setUIntListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::UInt64> const& ilValue);
		VFS_API void			setFloatListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<double> const& flValue);
		VFS_API void			setBoolListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<bool> const& blValue);
	private:
		enum EOperation
		{
			Error, Set, Add,
		};
		bool			extractSection(vfs::String::str_t const& readStr, vfs::size_t startPos, vfs::String::str_t& sSection);
		EOperation		extractKeyValue(vfs::String::str_t const &readStr, vfs::size_t startPos, vfs::String::str_t& sKey, vfs::String::str_t& sValue);
	private:
		class Section
		{
			friend class PropertyContainer;
			typedef std::map<vfs::String,vfs::String, vfs::String::Less> tProps;
		public:
			bool			has(vfs::String const& key);
			bool			add(vfs::String const& key, vfs::String const& value);
			bool			value(vfs::String const& key, vfs::String& value);
			vfs::String&	value(vfs::String const& key);
			void			print(std::ostream& out, vfs::String::str_t sPrefix = L"");
			void			clear();
		private:
			tProps			mapProps;
		};
		typedef std::map<vfs::String, Section, vfs::String::Less> tSections;

		bool			getValueForKey(vfs::String const& sSection, vfs::String const& sKey, vfs::String &sValue);

		Section&		section(vfs::String const& sSection);

		tSections		m_mapProps;
	};

} // namespace vfs

#endif // _VFS_PROPERTY_CONTAINER_H_
