#include "vfs_config.h"
#include "PropertyContainer.h"

#include "vfs.h"
#include "File/vfs_file.h"
#include "File/vfs_memory_file.h"
#include "vfs_file_raii.h"
#include "os_functions.h"

#include "Tools/Tools.h"
#include "Tools/ParserTools.h"

#include <sstream>
#include <vector>

/*************************************************************************************/
/*************************************************************************************/

bool CPropertyContainer::CSection::has(utf8string const& key)
{
	return mapProps.find(key) != mapProps.end();
}

bool CPropertyContainer::CSection::add(utf8string const& key, utf8string const& value)
{
	if(!mapProps[key].empty())
	{
		mapProps[key] += L", ";
	}
	mapProps[key] += value;
	return true;
}

utf8string& CPropertyContainer::CSection::value(utf8string const& key)
{
	return mapProps[key];
}
bool CPropertyContainer::CSection::value(utf8string const& key, utf8string& value)
{
	tProps::iterator sit = mapProps.find(key);
	if(sit != mapProps.end())
	{
		value.r_wcs().assign(sit->second.c_wcs());
		return true;
	}
	return false;
}
void CPropertyContainer::CSection::print(std::ostream& out, utf8string::str_t sPrefix)
{
	tProps::iterator sit = mapProps.begin();
	for(; sit != mapProps.end(); ++sit)
	{
		out << utf8string::as_utf8(sPrefix) << sit->first.utf8() << " = " << sit->second.utf8() << "\r\n";
	}
}

void CPropertyContainer::CSection::clear()
{
	mapProps.clear();
}

/*************************************************************************************/
/*************************************************************************************/
void CPropertyContainer::clearContainer()
{
	tSections::iterator it = m_mapProps.begin();
	for(;it != m_mapProps.end(); ++it)
	{
		it->second.clear();
	}
	m_mapProps.clear();
}

bool CPropertyContainer::extractSection(utf8string::str_t const& readStr, vfs::size_t startPos, utf8string::str_t& sSection)
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

CPropertyContainer::EOperation CPropertyContainer::extractKeyValue(utf8string::str_t const &readStr, vfs::size_t startPos, utf8string::str_t& sKey, utf8string::str_t& sValue)
{
	vfs::size_t iEqual = readStr.find_first_of(L"+=", startPos);
	if(iEqual == vfs::npos)
	{
		//std::cout << "WARNING : could not extract key-value pair : " << readStr << std::endl;
		return CPropertyContainer::Error;
	}
	// extract key
	sKey = vfs::trimString(readStr,0,iEqual-1);
	// extract value
	EOperation op = CPropertyContainer::Set;
	if( readStr.at(iEqual) == L'+' )
	{
		if( (iEqual+1) < readStr.size() && (readStr.at(iEqual+1) == L'=') )
		{
			iEqual += 1;
			op = CPropertyContainer::Add;
		}
	}
	sValue = vfs::trimString(readStr,iEqual+1,readStr.size());
	return op;
}


bool CPropertyContainer::initFromIniFile(vfs::Path const& sFileName)
{
	// try to open via VirtualFileSystem
	if(getVFS()->fileExists(sFileName))
	{
		return initFromIniFile(getVFS()->getReadFile(sFileName));
	}
	else
	{
		vfs::CFile file(sFileName);
		return initFromIniFile(vfs::tReadableFile::cast(&file));
	}
}

bool CPropertyContainer::initFromIniFile(vfs::tReadableFile *pFile)
{
	if(!pFile)
	{
		return false;
	}

	std::string	sBuffer;
	utf8string::str_t sCurrentSection;
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
					utf8string u8s;
					try
					{
						utf8string::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s.r_wcs());
					}
					catch(CBasicException& ex)
					{
						std::wstringstream wss;
						wss << L"Conversion error in file \"" << pFile->getPath().c_wcs()
							<< L"\", line " << line_counter;
						RETHROWEXCEPTION(wss.str().c_str(),&ex);
					}
					if(this->extractSection(u8s.c_wcs(), 0, sCurrentSection))
					{
						m_mapProps[sCurrentSection];
					}
					else
					{
						//std::cout << "WARNING : could not extract section name : " << sBuffer << std::endl;
					}
				}
				break;
			default:
				{
					// probably key-value pair
					utf8string::str_t u8s;
					try
					{
						utf8string::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s);
					}
					catch(CBasicException& ex)
					{
						std::wstringstream wss;
						wss << L"Conversion error in file \"" << pFile->getPath().c_wcs()
							<< L"\", line " << line_counter;
						RETHROWEXCEPTION(wss.str().c_str(),&ex);
					}
					utf8string::str_t sKey, sValue;
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
							//std::cout << "ERROR : could not find section ["<<sCurrentSection << "]in container" << std::endl;
						}
					}
				}
				break;
			}; // end switch
		} // end if (empty)
	} // end while(!eof)
	return true;
}

static vfs::UByte utf8bom[3] = {0xef,0xbb,0xbf};

bool CPropertyContainer::writeToIniFile(vfs::Path const& sFilename, bool bCreateNew)
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
		catch(CBasicException& ex)
		{
			logException(ex);
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
			CSection& section = sit->second;
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
		vfs::tReadableFile *pFile = NULL;
		vfs::CMemoryFile rfile;
		if(getVFS()->fileExists(sFilename))
		{
			pFile = getVFS()->getReadFile(sFilename);
			rfile.copyToBuffer(*pFile);
		}
		else
		{
			// file doesn't exist or VFS not initialized yet
			vfs::IBaseFile* pFile = new vfs::CFile(sFilename);
			if(pFile)
			{
				rfile.copyToBuffer(*vfs::tReadableFile::cast(pFile));
				delete pFile;				
			}
		}
		std::stringstream outbuffer;

		std::string sBuffer;
		utf8string::str_t sCurrentSection;

		std::set<utf8string> setKeys;
		std::set<utf8string> setSections;
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
						utf8string u8s;
						try
						{
							utf8string::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s.r_wcs());
						}
						catch(CBasicException& ex)
						{
							std::wstringstream wss;
							wss << L"Conversion error in file \"" << pFile->getPath().c_wcs()
								<< L"\", line " << line_counter;
							RETHROWEXCEPTION(wss.str().c_str(),&ex);
						}
						utf8string::str_t oldSection = sCurrentSection;
						if(this->extractSection(u8s.c_wcs(), 0, sCurrentSection))
						{
							if(setSections.find(sCurrentSection) == setSections.end())
							{
								// section already handled ?!?!?!
								// just print duplicate version
								outbuffer << utf8string::as_utf8(sBuffer) << ENDL;
								break;
							}
							if(!setKeys.empty())
							{
								// there are new keys in the previous section
								CSection& oldsec = m_mapProps[oldSection];

								std::set<utf8string>::iterator kit = setKeys.begin();
								for(; kit != setKeys.end(); ++kit)
								{
									outbuffer << utf8string::as_utf8(*kit) << " = " << utf8string::as_utf8(oldsec.value(*kit)) << ENDL;
								}
								// all remaining keys were written, clear set
								setKeys.clear();
								outbuffer << ENDL;
							}
							CSection& sec = m_mapProps[sCurrentSection];
							CSection::tProps::iterator it = sec.mapProps.begin();
							for(; it != sec.mapProps.end(); ++it)
							{
								setKeys.insert(it->first);
							}
						}
						outbuffer << utf8string::as_utf8(sBuffer) << ENDL;
					}
					break;
				default:
					{
						// probably key-value pair
						utf8string u8s;
						try
						{
							utf8string::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s.r_wcs());
						}
						catch(CBasicException& ex)
						{
							std::wstringstream wss;
							wss << L"Conversion error in file \"" << pFile->getPath().c_wcs()
								<< L"\", line " << line_counter;
							RETHROWEXCEPTION(wss.str().c_str(),&ex);
						}
						utf8string::str_t sKey, sValue;
						if(this->extractKeyValue(u8s.c_wcs(), 0, sKey, sValue))
						{
							if(setKeys.find(sKey) != setKeys.end())
							{
								outbuffer << utf8string::as_utf8(sKey) << " = " << utf8string::as_utf8(m_mapProps[sCurrentSection].value(sKey)) << "\r\n";
								setKeys.erase(sKey);
							}
							else
							{
								outbuffer << utf8string::as_utf8(sBuffer) << ENDL;
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
			CSection& sec = m_mapProps[sCurrentSection];
			std::set<utf8string>::iterator kit = setKeys.begin();
			for(; kit != setKeys.end(); ++kit)
			{
				outbuffer << utf8string::as_utf8(*kit) << " = " << utf8string::as_utf8(sec.value(*kit)) << ENDL;
			}
			setKeys.clear();
			if(setKeys.empty())
			{
				setSections.erase(sCurrentSection);
			}
		}
		std::set<utf8string>::iterator it = setSections.begin();
		for(; it != setSections.end(); ++it)
		{
			outbuffer << ENDL << "[" << utf8string::as_utf8(*it) << "]" << ENDL;
			std::stringstream ss;
			m_mapProps[*it].print(outbuffer);
		}
		
		try
		{
			vfs::COpenWriteFile wfile(sFilename,true,true);
			wfile.file().write(outbuffer.str().c_str(),(vfs::size_t)outbuffer.str().length());
		}
		catch(CBasicException& ex)
		{
			logException(ex);
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


void CPropertyContainer::printProperties()
{
	tSections::iterator pit = m_mapProps.begin();
	for(;pit != m_mapProps.end(); ++pit)
	{
		std::cout << "[" << pit->first.utf8() << "]\n";
		pit->second.print(std::cout, L"  ");
		std::cout << std::endl;
	}
}


CPropertyContainer::CSection& CPropertyContainer::section(utf8string const& sSection)
{
	return m_mapProps[sSection];
}

bool CPropertyContainer::getValueForKey(utf8string const& sSection, utf8string const& sKey, utf8string &sValue)
{
	tSections::iterator pit = m_mapProps.find(vfs::trimString(sSection,0,(vfs::size_t)sSection.length()));
	if( pit != m_mapProps.end() )
	{
		return pit->second.value( vfs::trimString(sKey,0,(vfs::size_t)sKey.length()), sValue );
	}
	return false;
}

bool CPropertyContainer::hasProperty(utf8string const& sSection, utf8string const& sKey)
{
	tSections::iterator pit = m_mapProps.find(vfs::trimString(sSection,0,(vfs::size_t)sSection.length()));
	if( pit != m_mapProps.end() )
	{
		return pit->second.has(vfs::trimString(sKey,0,sKey.length()));
	}
	return false;
}

utf8string const& CPropertyContainer::getStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const& sDefaultValue)
{
	CPropertyContainer::tSections::iterator sit = m_mapProps.find(sSection);
	if(sit != m_mapProps.end())
	{
		CPropertyContainer::CSection::tProps::iterator pit = sit->second.mapProps.find(sKey);
		if(pit != sit->second.mapProps.end())
		{
			return pit->second;
		}
	}
	return sDefaultValue;
}

bool CPropertyContainer::getStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string& sValue, utf8string const& sDefaultValue)
{
	if(getValueForKey(sSection,sKey,sValue))
	{
		return true;
	}
	sValue = sDefaultValue;
	return false;
}

bool CPropertyContainer::getStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string::char_t* sValue, vfs::size_t len, utf8string const& sDefaultValue)
{
	utf8string s;
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

vfs::Int64 CPropertyContainer::getIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue, vfs::Int64 iMinValue, vfs::Int64 iMaxValue)
{
	return std::min<vfs::Int64>(iMaxValue, std::max<vfs::Int64>(iMinValue, this->getIntProperty(sSection, sKey, iDefaultValue)));
}

vfs::Int64 CPropertyContainer::getIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue)
{
	utf8string sValue;
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

vfs::UInt64 CPropertyContainer::getUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue, vfs::UInt64 iMinValue, vfs::UInt64 iMaxValue)
{
	return std::min<vfs::UInt64>(iMaxValue, std::max<vfs::UInt64>(iMinValue, this->getIntProperty(sSection, sKey, iDefaultValue)));
}

vfs::UInt64 CPropertyContainer::getUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue)
{
	utf8string sValue;
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

double CPropertyContainer::getFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue, double  fMinValue, double fMaxValue)
{
	return std::min<double>(fMaxValue, std::max<double>(fMinValue, this->getFloatProperty(sSection, sKey, fDefaultValue)));
}

double CPropertyContainer::getFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue)
{
	utf8string sValue;
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

bool CPropertyContainer::getBoolProperty(utf8string const& sSection, utf8string const& sKey, bool bDefaultValue)
{
	utf8string sValue;
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

bool CPropertyContainer::getStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> &lValueList, utf8string sDefaultValue)
{
	utf8string sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.nextListEntry(entry))
		{
			lValueList.push_back(vfs::trimString(entry,0,entry.length()));
		}
		return true;
	}
	return false;
}

bool CPropertyContainer::getIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> &lValueList, vfs::Int64 iDefaultValue)
{
	utf8string sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.nextListEntry(entry))
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

bool CPropertyContainer::getUIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::UInt64> &lValueList, vfs::UInt64 iDefaultValue)
{
	utf8string sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.nextListEntry(entry))
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
bool CPropertyContainer::getFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> &lValueList, double fDefaultValue)
{
	utf8string sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.nextListEntry(entry))
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

bool CPropertyContainer::getBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> &lValueList, bool bDefaultValue)
{
	utf8string sValue;
	if(getValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.nextListEntry(entry))
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

void CPropertyContainer::setStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const& sValue)
{
	this->section(sSection).value(sKey) = sValue;
}

void CPropertyContainer::setIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 const& iValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,vfs::Int64>(iValue);
}
void CPropertyContainer::setUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 const& iValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,vfs::UInt64>(iValue);
}
void CPropertyContainer::setFloatProperty(utf8string const& sSection, utf8string const& sKey, double const& fValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,double>(fValue);
}
void CPropertyContainer::setBoolProperty(utf8string const& sSection, utf8string const& sKey, bool const& bValue)
{
	this->section(sSection).value(sKey) = toString<wchar_t,bool>(bValue);
}

void CPropertyContainer::setStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> const& slValue)
{
	this->section(sSection).value(sKey) = toStringList<utf8string>(slValue);
}

void CPropertyContainer::setIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> const& ilValue)
{
	this->section(sSection).value(sKey) = toStringList<vfs::Int64>(ilValue);
}

void CPropertyContainer::setFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> const& flValue)
{
	this->section(sSection).value(sKey) = toStringList<double>(flValue);
}

void CPropertyContainer::setBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> const& blValue)
{
	this->section(sSection).value(sKey) = toStringList<bool>(blValue);
}

/**************************************************************************************************/
/**************************************************************************************************/

