/* 
 * bfVFS : vfs/Tools/vfs_property_container.cpp
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

#include <vfs/vfs_config.h>

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Core/File/vfs_file.h>
#include <vfs/Core/File/vfs_buffer_file.h>

#include <vfs/Tools/vfs_tools.h>
#include <vfs/Tools/vfs_parser_tools.h>
#include <vfs/Tools/vfs_property_container.h>

#include <sstream>
#include <vector>

/*************************************************************************************/
/*************************************************************************************/

bool vfs::PropertyContainer::Section::has(vfs::String const& key)
{
	return mapProps.find(key) != mapProps.end();
}

bool vfs::PropertyContainer::Section::add(vfs::String const& key, vfs::String const& value)
{
	if(!mapProps[key].empty())
	{
		mapProps[key] += L", ";
	}
	mapProps[key] += value;
	return true;
}

vfs::String& vfs::PropertyContainer::Section::value(vfs::String const& key)
{
	return mapProps[key];
}
bool vfs::PropertyContainer::Section::value(vfs::String const& key, vfs::String& value)
{
	tProps::iterator sit = mapProps.find(key);
	if(sit != mapProps.end())
	{
		value.r_wcs().assign(sit->second.c_wcs());
		return true;
	}
	return false;
}
void vfs::PropertyContainer::Section::print(std::ostream& out, vfs::String::str_t sPrefix)
{
	tProps::iterator sit = mapProps.begin();
	for(; sit != mapProps.end(); ++sit)
	{
		out << vfs::String::as_utf8(sPrefix) << sit->first.utf8() << " = " << sit->second.utf8() << "\r\n";
	}
}

void vfs::PropertyContainer::Section::clear()
{
	mapProps.clear();
}

/*************************************************************************************/
/*************************************************************************************/
void vfs::PropertyContainer::clearContainer()
{
	tSections::iterator it = m_mapProps.begin();
	for(;it != m_mapProps.end(); ++it)
	{
		it->second.clear();
	}
	m_mapProps.clear();
}

bool vfs::PropertyContainer::extractSection(vfs::String::str_t const& readStr, vfs::size_t startPos, vfs::String::str_t& sSection)
{
	// extract section name
	vfs::size_t close = readStr.find_first_of(L"]", startPos);
	if( close != vfs::npos && close > startPos)
	{
		startPos += 1;
		sSection = vfs::trimString(readStr,startPos,(vfs::size_t)(close-1));
		return true;
	}
	return false;
}

vfs::PropertyContainer::EOperation vfs::PropertyContainer::extractKeyValue(vfs::String::str_t const &readStr, vfs::size_t startPos, vfs::String::str_t& sKey, vfs::String::str_t& sValue)
{
	vfs::size_t iEqual = readStr.find_first_of(L"+=", startPos);
	if(iEqual == vfs::npos)
	{
		VFS_LOG_WARNING(_BS("WARNING : could not extract key-value pair : ") << readStr << _BS::wget);
		return vfs::PropertyContainer::Error;
	}
	// extract key
	sKey = vfs::trimString(readStr,0,iEqual-1);
	// extract value
	EOperation op = vfs::PropertyContainer::Set;
	if( readStr.at(iEqual) == L'+' )
	{
		if( (iEqual+1) < readStr.size() && (readStr.at(iEqual+1) == L'=') )
		{
			iEqual += 1;
			op = vfs::PropertyContainer::Add;
		}
	}
	sValue = vfs::trimString(readStr,iEqual+1,readStr.size());
	return op;
}


bool vfs::PropertyContainer::initFromIniFile(vfs::Path const& sFileName)
{
	// try to open via VirtualFileSystem
	if(getVFS()->fileExists(sFileName))
	{
		return initFromIniFile(getVFS()->getReadFile(sFileName));
	}
	else
	{
		vfs::CFile file(sFileName);
		if(file.openRead())
		{
			return initFromIniFile(vfs::tReadableFile::cast(&file));
		}
		return false;
	}
}

bool vfs::PropertyContainer::initFromIniFile(vfs::tReadableFile *pFile)
{
	if(!pFile)
	{
		return false;
	}

	std::string	sBuffer;
	vfs::String::str_t sCurrentSection;
	int line_counter = 0;
	CReadLine rl(*pFile);
	while(rl.getLine(sBuffer))
	{
		line_counter++;
		// very simple parsing : key = value
		if(!sBuffer.empty())
		{
			// remove leading white spaces
			::size_t iStart = sBuffer.find_first_not_of(" \t",0);
			if(iStart == std::string::npos)
			{
				// only white space characters
				continue;
			}
			char first = sBuffer.at(iStart);
			switch(first)
			{
			case '!':
			case ';':
			case '#':
				// comment -> do nothing
				break;
			case '[':
				{
					vfs::String u8s;
					try
					{
						vfs::String::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s.r_wcs());
					}
					catch(std::exception& ex)
					{
						VFS_RETHROW( _BS(L"Conversion error in file \"") << pFile->getPath()
							<< L"\", line " << line_counter << _BS::wget, ex);
					}
					if(this->extractSection(u8s.c_wcs(), 0, sCurrentSection))
					{
						m_mapProps[sCurrentSection];
					}
					else
					{
						VFS_LOG_WARNING(_BS("WARNING : could not extract section name : ") << sBuffer << _BS::wget);
					}
				}
				break;
			default:
				{
					// probably key-value pair
					vfs::String::str_t u8s;
					try
					{
						vfs::String::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s);
					}
					catch(std::exception& ex)
					{
						VFS_RETHROW( _BS(L"Conversion error in file \"") << pFile->getPath()
							<< L"\", line " << line_counter << _BS::wget, ex);
					}
					vfs::String::str_t sKey, sValue;
					EOperation op = this->extractKeyValue(u8s, 0, sKey, sValue);
					if(op != Error)
					{
						// add key-value pair to map
						if(m_mapProps.find(sCurrentSection) != m_mapProps.end())
						{
							if(op == Set)
							{
								this->section(sCurrentSection).value(sKey) = sValue;
							}
							else if(op == Add)
							{
								this->section(sCurrentSection).add(sKey, sValue);
							}
						}
						else
						{
							VFS_LOG_WARNING(_BS(L"ERROR : could not find section [") << sCurrentSection 
								<< L"] in container" << _BS::wget);
						}
					}
				}
				break;
			}; // end switch
		} // end if (empty)
	} // end while(!eof)
	return true;
}

static vfs::UByte utf8bom[4] = {0xef,0xbb,0xbf,0x0};

bool vfs::PropertyContainer::writeToIniFile(vfs::Path const& sFilename, bool bCreateNew)
{
#ifdef WIN32
	const char ENDL[] = "\r\n";
#else
	const char ENDL[] = "\n";
#endif
	if(bCreateNew)
	{
		vfs::tWritableFile* file;
		bool delete_file = false;
		try
		{
			vfs::COpenWriteFile wfile(sFilename,true,true);
			file = &wfile.file();
			wfile.release();
		}
		catch(std::exception& ex)
		{
			VFS_LOG_WARNING(ex.what());
			// vfs not initialized?
			vfs::CFile* cfile = new vfs::CFile(sFilename);
			cfile->openWrite(true,true);
			file = vfs::tWritableFile::cast(cfile);
			delete_file = true;
		}
		tSections::iterator sit = m_mapProps.begin();
		std::stringstream ss;
		std::string str;
		ss << (char*)utf8bom;
		for(; sit != m_mapProps.end(); ++sit)
		{
			ss.str("");
			ss << "[" << sit->first.utf8() << "]" << ENDL;
			str = ss.str();
			file->write(str.c_str(), str.length());

			ss.clear();
			ss.str("");
			Section& section = sit->second;
			section.print(ss);
			ss << ENDL;
			str = ss.str();
			file->write(str.c_str(),str.length());
		}
		file->close();
		if(delete_file)
		{
			delete file;
		}
		return true;
	}
	else
	{
		// try to open via VirtualFileSystem
		vfs::CBufferFile rfile;
		if(getVFS()->fileExists(sFilename))
		{
			vfs::COpenReadFile rf(sFilename);
			rfile.copyToBuffer(rf.file());
		}
		else if(getVFS()->createNewFile(sFilename))
		{
			vfs::tReadableFile* pFile = getVFS()->getReadFile(sFilename);
			if(pFile && pFile->openRead())
			{
				rfile.copyToBuffer(*pFile);
				pFile->close();
			}
		}
		else
		{
			// file doesn't exist or VFS not initialized yet
			vfs::CFile file(sFilename);
			rfile.copyToBuffer(*vfs::tReadableFile::cast(&file));
		}
		std::stringstream outbuffer;

		std::string sBuffer;
		vfs::String::str_t sCurrentSection;

		std::set<vfs::String> setKeys;
		std::set<vfs::String> setSections;
		tSections::iterator sit = m_mapProps.begin();
		for(; sit != m_mapProps.end(); ++sit)
		{
			setSections.insert(sit->first);
		}

		CReadLine rl(*vfs::tReadableFile::cast(&rfile));
		outbuffer << (char*)(utf8bom);
		vfs::UInt32 line_counter = 0;
		while(rl.getLine(sBuffer))
		{
			line_counter++;
			if(!sBuffer.empty())
			{
				// remove leading white spaces
				::size_t iStart = sBuffer.find_first_not_of(" \t",0);
				char first = sBuffer.at(iStart);
				switch(first)
				{
				case '!':
				case ';':
				case '#':
					outbuffer << sBuffer << ENDL;
					break;
				case '[':
					{
						vfs::String u8s;
						try
						{
							vfs::String::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s.r_wcs());
						}
						catch(std::exception& ex)
						{
							VFS_RETHROW(_BS(L"Conversion error in file \"") << sFilename
								<< L"\", line " << line_counter << _BS::wget, ex);
						}
						vfs::String::str_t oldSection = sCurrentSection;
						if(this->extractSection(u8s.c_wcs(), 0, sCurrentSection))
						{
							if(setSections.find(sCurrentSection) == setSections.end())
							{
								// section already handled ?!?!?!
								// just print duplicate version
								outbuffer << vfs::String::as_utf8(sBuffer) << ENDL;
								break;
							}
							if(!setKeys.empty())
							{
								// there are new keys in the previous section
								Section& oldsec = m_mapProps[oldSection];

								std::set<vfs::String>::iterator kit = setKeys.begin();
								for(; kit != setKeys.end(); ++kit)
								{
									outbuffer << vfs::String::as_utf8(*kit) << " = " << vfs::String::as_utf8(oldsec.value(*kit)) << ENDL;
								}
								// all remaining keys were written, clear set
								setKeys.clear();
								outbuffer << ENDL;
							}
							Section& sec = m_mapProps[sCurrentSection];
							Section::tProps::iterator it = sec.mapProps.begin();
							for(; it != sec.mapProps.end(); ++it)
							{
								setKeys.insert(it->first);
							}
						}
						outbuffer << vfs::String::as_utf8(sBuffer) << ENDL;
					}
					break;
				default:
					{
						// probably key-value pair
						vfs::String u8s;
						try
						{
							vfs::String::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s.r_wcs());
						}
						catch(std::exception& ex)
						{
							VFS_RETHROW(_BS(L"Conversion error in file \"") << sFilename
								<< L"\", line " << line_counter << _BS::wget, ex);
						}
						vfs::String::str_t sKey, sValue;
						if(this->extractKeyValue(u8s.c_wcs(), 0, sKey, sValue))
						{
							if(setKeys.find(sKey) != setKeys.end())
							{
								outbuffer << vfs::String::as_utf8(sKey) << " = " << vfs::String::as_utf8(m_mapProps[sCurrentSection].value(sKey)) << "\r\n";
								setKeys.erase(sKey);
							}
							else
							{
								outbuffer << vfs::String::as_utf8(sBuffer) << ENDL;
							}
							if(setKeys.empty())
							{
								setSections.erase(sCurrentSection);
							}
						}
					}
					break;
				}; // end switch
			}
			else
			{
				outbuffer << ENDL;
			}
		}
		if(!setKeys.empty())
		{
			Section& sec = m_mapProps[sCurrentSection];
			std::set<vfs::String>::iterator kit = setKeys.begin();
			for(; kit != setKeys.end(); ++kit)
			{
				outbuffer << vfs::String::as_utf8(*kit) << " = " << vfs::String::as_utf8(sec.value(*kit)) << ENDL;
			}
			setKeys.clear();
			if(setKeys.empty())
			{
				setSections.erase(sCurrentSection);
			}
		}
		std::set<vfs::String>::iterator it = setSections.begin();
		for(; it != setSections.end(); ++it)
		{
			outbuffer << ENDL << "[" << vfs::String::as_utf8(*it) << "]" << ENDL;
			std::stringstream ss;
			m_mapProps[*it].print(outbuffer);
		}
		
		try
		{
			vfs::COpenWriteFile wfile(sFilename,true,true);
			wfile->write(outbuffer.str().c_str(),(vfs::size_t)outbuffer.str().length());
		}
		catch(std::exception& ex)
		{
			VFS_LOG_WARNING(ex.what());
			vfs::CFile file(sFilename);
			if(file.openWrite(true,true))
			{
				file.write(outbuffer.str().c_str(),(vfs::size_t)outbuffer.str().length());
				file.close();
			}
		}
		return true;
	}
}


void vfs::PropertyContainer::printProperties(std::ostream &out)
{
	tSections::iterator pit = m_mapProps.begin();
	for(;pit != m_mapProps.end(); ++pit)
	{
		out << "[" << pit->first.utf8() << "]\n";
		pit->second.print(out, L"  ");
		out << std::endl;
	}
}


vfs::PropertyContainer::Section& vfs::PropertyContainer::section(vfs::String const& sSection)
{
	return m_mapProps[sSection];
}

bool vfs::PropertyContainer::getValueForKey(vfs::String const& sSection, vfs::String const& sKey, vfs::String &sValue)
{
	tSections::iterator pit = m_mapProps.find(vfs::trimString(sSection,0,(vfs::size_t)sSection.length()));
	if( pit != m_mapProps.end() )
	{
		return pit->second.value( vfs::trimString(sKey,0,(vfs::size_t)sKey.length()), sValue );
	}
	return false;
}

bool vfs::PropertyContainer::hasProperty(vfs::String const& sSection, vfs::String const& sKey)
{
	tSections::iterator pit = m_mapProps.find(vfs::trimString(sSection,0,(vfs::size_t)sSection.length()));
	if( pit != m_mapProps.end() )
	{
		return pit->second.has(vfs::trimString(sKey,0,sKey.length()));
	}
	return false;
}

vfs::String const& vfs::PropertyContainer::getStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String const& sDefaultValue)
{
	vfs::PropertyContainer::tSections::iterator sit = m_mapProps.find(sSection);
	if(sit != m_mapProps.end())
	{
		vfs::PropertyContainer::Section::tProps::iterator pit = sit->second.mapProps.find(sKey);
		if(pit != sit->second.mapProps.end())
		{
			return pit->second;
		}
	}
	return sDefaultValue;
}

bool vfs::PropertyContainer::getStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String& sValue, vfs::String const& sDefaultValue)
{
	if(getValueForKey(sSection,sKey,sValue))
	{
		return true;
	}
	sValue = sDefaultValue;
	return false;
}

bool vfs::PropertyContainer::getStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String::char_t* sValue, vfs::size_t len, vfs::String const& sDefaultValue)
{
	vfs::String s;
	if(getValueForKey(sSection,sKey,s))
	{
		vfs::size_t l = std::min<vfs::size_t>(s.length(), len-1);
		wcsncpy(sValue,s.c_str(), l);
		sValue[l] = 0;
		return true;
	}
	vfs::size_t l = std::min<vfs::size_t>(sDefaultValue.length(), len-1);
	wcsncpy(sValue,sDefaultValue.c_str(), l);
	sValue[l] = 0;
	return false;
}

vfs::Int64 vfs::PropertyContainer::getIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::Int64 iDefaultValue, vfs::Int64 iMinValue, vfs::Int64 iMaxValue)
{
	return std::min<vfs::Int64>(iMaxValue, std::max<vfs::Int64>(iMinValue, this->getIntProperty(sSection, sKey, iDefaultValue)));
}

vfs::Int64 vfs::PropertyContainer::getIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::Int64 iDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		vfs::Int64 iRetVal;
		if(convertTo<vfs::Int64>(sValue,iRetVal))
		{
			return iRetVal;
		}
	}
	return iDefaultValue;
}

vfs::UInt64 vfs::PropertyContainer::getUIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::UInt64 iDefaultValue, vfs::UInt64 iMinValue, vfs::UInt64 iMaxValue)
{
	return std::min<vfs::UInt64>(iMaxValue, std::max<vfs::UInt64>(iMinValue, this->getIntProperty(sSection, sKey, iDefaultValue)));
}

vfs::UInt64 vfs::PropertyContainer::getUIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::UInt64 iDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		vfs::UInt64 iRetVal;
		if(convertTo<vfs::UInt64>(sValue,iRetVal))
		{
			return iRetVal;
		}
	}
	return iDefaultValue;
}

double vfs::PropertyContainer::getFloatProperty(vfs::String const& sSection, vfs::String const& sKey, double fDefaultValue, double  fMinValue, double fMaxValue)
{
	return std::min<double>(fMaxValue, std::max<double>(fMinValue, this->getFloatProperty(sSection, sKey, fDefaultValue)));
}

double vfs::PropertyContainer::getFloatProperty(vfs::String const& sSection, vfs::String const& sKey, double fDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		double fRetVal;
		if(convertTo<double>(sValue,fRetVal))
		{
			return fRetVal;
		}
	}
	return fDefaultValue;
}

bool vfs::PropertyContainer::getBoolProperty(vfs::String const& sSection, vfs::String const& sKey, bool bDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		vfs::Int32 iRetVal;
		if( StrCmp::Equal(sValue,L"true") || ( convertTo<>(sValue,iRetVal) && (iRetVal != 0) ) )
		{
			return true;
		}
		else if( StrCmp::Equal(sValue,"false") || ( convertTo<>(sValue,iRetVal) && (iRetVal == 0) ) )
		{
			return false;
		}
		// else return bDefaultValue
	}
	return bDefaultValue;
}

bool vfs::PropertyContainer::getStringListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::String> &lValueList, vfs::String sDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		CTokenizer splitter(sValue);
		vfs::String entry;
		while( splitter.next(entry, L',') )
		{
			lValueList.push_back(vfs::trimString(entry,0,entry.length()));
		}
		return true;
	}
	return false;
}

bool vfs::PropertyContainer::getIntListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::Int64> &lValueList, vfs::Int64 iDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		vfs::String entry;
		CTokenizer splitter(sValue);
		while( splitter.next(entry, L',') )
		{
			vfs::Int64 iRetVal;
			if(convertTo<vfs::Int64>(entry,iRetVal))
			{
				lValueList.push_back(iRetVal);
			}
			else
			{
				lValueList.push_back(iDefaultValue);
			}
		}
		return true;
	}
	return false;
}

bool vfs::PropertyContainer::getUIntListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::UInt64> &lValueList, vfs::UInt64 iDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		vfs::String entry;
		CTokenizer splitter(sValue);
		while( splitter.next(entry, L',') )
		{
			vfs::UInt64 iRetVal;
			if(convertTo<vfs::UInt64>(entry,iRetVal))
			{
				lValueList.push_back(iRetVal);
			}
			else
			{
				lValueList.push_back(iDefaultValue);
			}
		}
		return true;
	}
	return false;
}
bool vfs::PropertyContainer::getFloatListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<double> &lValueList, double fDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		vfs::String entry;
		CTokenizer splitter(sValue);
		while( splitter.next(entry, L',') )
		{
			double fRetVal;
			if(convertTo<double>(entry,fRetVal))
			{
				lValueList.push_back(fRetVal);
			}
			else
			{
				lValueList.push_back(fDefaultValue);
			}
		}
		return true;
	}
	return false;
}

bool vfs::PropertyContainer::getBoolListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<bool> &lValueList, bool bDefaultValue)
{
	vfs::String sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		vfs::String entry;
		CTokenizer splitter(sValue);
		while( splitter.next(entry, L',') )
		{
			vfs::Int32 iRetVal;
			if( StrCmp::Equal(entry,L"true") || ( convertTo<>(entry,iRetVal) && (iRetVal != 0) ) )
			{
				lValueList.push_back(true);
			}
			else if( StrCmp::Equal(entry,L"false") || ( convertTo<>(entry,iRetVal) && (iRetVal == 0) ) )
			{
				lValueList.push_back(false);
			}
			else
			{
				lValueList.push_back(bDefaultValue);
			}
		}
		return true;
	}
	return false;
}

void vfs::PropertyContainer::setStringProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::String const& sValue)
{
	this->section(sSection).value(sKey) = sValue;
}

void vfs::PropertyContainer::setIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::Int64 const& iValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,vfs::Int64>(iValue);
}
void vfs::PropertyContainer::setUIntProperty(vfs::String const& sSection, vfs::String const& sKey, vfs::UInt64 const& iValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,vfs::UInt64>(iValue);
}
void vfs::PropertyContainer::setFloatProperty(vfs::String const& sSection, vfs::String const& sKey, double const& fValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,double>(fValue);
}
void vfs::PropertyContainer::setBoolProperty(vfs::String const& sSection, vfs::String const& sKey, bool const& bValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,bool>(bValue);
}

void vfs::PropertyContainer::setStringListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::String> const& slValue)
{
	this->section(sSection).value(sKey) = toStringList<vfs::String>(slValue);
}

void vfs::PropertyContainer::setIntListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<vfs::Int64> const& ilValue)
{
	this->section(sSection).value(sKey) = toStringList<vfs::Int64>(ilValue);
}

void vfs::PropertyContainer::setFloatListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<double> const& flValue)
{
	this->section(sSection).value(sKey) = toStringList<double>(flValue);
}

void vfs::PropertyContainer::setBoolListProperty(vfs::String const& sSection, vfs::String const& sKey, std::list<bool> const& blValue)
{
	this->section(sSection).value(sKey) = toStringList<bool>(blValue);
}

/**************************************************************************************************/
/**************************************************************************************************/

