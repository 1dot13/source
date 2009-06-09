#include "PropertyContainer.h"

#include "vfs.h"
#include "File/vfs_file.h"
#include "File/vfs_memory_file.h"
#include "vfs_file_raii.h"
#include "iteratedir.h"

#include "Tools/Tools.h"
#include "Tools/ParserTools.h"

#include <sstream>
#include <vector>


/*************************************************************************************/
/*************************************************************************************/

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
void CPropertyContainer::CSection::Print(std::ostream& out, utf8string::str_t sPrefix)
{
	tProps::iterator sit = mapProps.begin();
	for(; sit != mapProps.end(); ++sit)
	{
		out << utf8string::as_utf8(sPrefix) << sit->first.utf8() << " = " << sit->second.utf8() << "\r\n";
	}
}

void CPropertyContainer::CSection::Clear()
{
	mapProps.clear();
}

/*************************************************************************************/
/*************************************************************************************/
void CPropertyContainer::ClearContainer()
{
	tSections::iterator it = m_mapProps.begin();
	for(;it != m_mapProps.end(); ++it)
	{
		it->second.Clear();
	}
	m_mapProps.clear();
}

bool CPropertyContainer::ExtractSection(utf8string::str_t const& readStr, size_t startPos, utf8string::str_t& sSection)
{
	// extract section name
	unsigned int close = readStr.find_first_of(L"]",startPos);
	if( close > 0 && close > startPos)
	{
		startPos += 1;
		sSection = vfs::TrimString(readStr,startPos,close-1);
		return true;
	}
	return false;
}

CPropertyContainer::EOperation CPropertyContainer::ExtractKeyValue(utf8string::str_t const &readStr, size_t startPos, utf8string::str_t& sKey, utf8string::str_t& sValue)
{
	vfs::Int32 iEqual = readStr.find_first_of(L"+=",startPos);
	if(iEqual < 0)
	{
		//std::cout << "WARNING : could not extract key-value pair : " << readStr << std::endl;
		return CPropertyContainer::Error;
	}
	// extract key
	sKey = vfs::TrimString(readStr,0,iEqual-1);
	// extract value
	EOperation op = CPropertyContainer::Set;
	if( readStr.at(iEqual) == L'+' )
	{
		if( (vfs::UInt32)(iEqual+1) < readStr.size() && (readStr.at((vfs::UInt32)(iEqual+1)) == L'=') )
		{
			iEqual += 1;
			op = CPropertyContainer::Add;
		}
	}
	sValue = vfs::TrimString(readStr,iEqual+1,readStr.size());
	return op;
}


bool CPropertyContainer::InitFromIniFile(vfs::Path const& sFileName)
{
	// try to open via VirtualFileSystem
	if(GetVFS()->FileExists(sFileName))
	{
		return InitFromIniFile(GetVFS()->GetRFile(sFileName));
	}
	else
	{
		// file doesn't exist or VFS not initialized yet
		vfs::IBaseFile* pFile = new vfs::CTextFile(sFileName);
		if(pFile)
		{
			bool success = InitFromIniFile(vfs::tReadableFile::Cast(pFile));
			delete pFile;
			return success;
		}
	}
	return false;
}

bool CPropertyContainer::InitFromIniFile(vfs::tReadableFile *pFile)
{
	if(pFile && pFile->OpenRead())
	{
		vfs::COpenReadFile rfile(pFile);
		std::string	sBuffer;
		utf8string::str_t sCurrentSection;
		int line_counter = 0;
		CReadLine rl(*pFile);
		while(rl.GetLine(sBuffer))
		{
			line_counter++;
			// very simple parsing : key = value
			if(!sBuffer.empty())
			{
				// remove leading white spaces
				unsigned int iStart = sBuffer.find_first_not_of(" \t",0);
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
							wss << L"Conversion error in file \"" << pFile->GetFullPath()().c_wcs()
								<< L"\", line " << line_counter;
							RETHROWEXCEPTION(wss.str().c_str(),&ex);
						}
						if(this->ExtractSection(u8s.c_wcs(), 0, sCurrentSection))
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
						utf8string u8s;
						try
						{
							utf8string::as_utf16(sBuffer.substr(iStart, sBuffer.length()-iStart), u8s.r_wcs());
						}
						catch(CBasicException& ex)
						{
							std::wstringstream wss;
							wss << L"Conversion error in file \"" << pFile->GetFullPath()().c_wcs()
								<< L"\", line " << line_counter;
							RETHROWEXCEPTION(wss.str().c_str(),&ex);
						}
						utf8string::str_t sKey, sValue;
						EOperation op = this->ExtractKeyValue(u8s.c_wcs(), 0, sKey, sValue);
						if(op != Error)
						{
							// add key-value pair to map
							if(m_mapProps.find(sCurrentSection) != m_mapProps.end())
							{
								if(op == Set)
								{
									Section(sCurrentSection).value(sKey) = sValue;
								}
								else if(op == Add)
								{
									Section(sCurrentSection).add(sKey, sValue);
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
	
	return false;
}

bool CPropertyContainer::WriteToIniFile(vfs::Path const& sFileName, bool bCreateNew)
{
#ifdef WIN32
	const char ENDL[] = "\r\n";
#else
	const char ENDL[] = "\n";
#endif
	if(bCreateNew)
	{
		vfs::tWriteableFile* file;
		bool delete_file = false;
		try
		{
			vfs::COpenWriteFile wfile(sFileName,true,true);
			file = &wfile.file();
			wfile.release();
		}
		catch(CBasicException& ex)
		{
			LogException(ex);
			// vfs not initialized?
			vfs::CFile* cfile = new vfs::CFile(sFileName);
			cfile->OpenWrite(true,true);
			file = vfs::tWriteableFile::Cast(cfile);
			delete_file = true;
		}
		tSections::iterator sit = m_mapProps.begin();
		std::stringstream ss;
		std::string str;
		vfs::UInt32 written;
		for(; sit != m_mapProps.end(); ++sit)
		{
			ss.str("");
			ss << "[" << sit->first.utf8() << "]" << ENDL;
			str = ss.str();
			file->Write(str.c_str(),str.length(),written);

			ss.clear();
			ss.str("");
			CSection& section = sit->second;
			section.Print(ss);
			ss << ENDL;
			str = ss.str();
			file->Write(str.c_str(),str.length(),written);
		}
		file->Close();
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
		if(GetVFS()->FileExists(sFileName))
		{
			pFile = GetVFS()->GetRFile(sFileName);
			rfile.CopyToBuffer(*pFile);
		}
		else
		{
			// file doesn't exist or VFS not initialized yet
			vfs::IBaseFile* pFile = new vfs::CFile(sFileName);
			if(pFile)
			{
				rfile.CopyToBuffer(*vfs::tReadableFile::Cast(pFile));
				delete pFile;				
			}
		}
		std::stringstream outbuffer;

		std::string sBuffer;
		utf8string::str_t sCurrentSection;
		vfs::UInt32 written;

		std::set<utf8string> setKeys;
		std::set<utf8string> setSections;
		tSections::iterator sit = m_mapProps.begin();
		for(; sit != m_mapProps.end(); ++sit)
		{
			setSections.insert(sit->first);
		}

		CReadLine rl(*vfs::tReadableFile::Cast(&rfile));
		vfs::UInt32 line_counter = 0;
		while(rl.GetLine(sBuffer))
		{
			line_counter++;
			if(!sBuffer.empty())
			{
				// remove leading white spaces
				int iStart = sBuffer.find_first_not_of(" \t",0);
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
							wss << L"Conversion error in file \"" << pFile->GetFullPath()().c_wcs()
								<< L"\", line " << line_counter;
							RETHROWEXCEPTION(wss.str().c_str(),&ex);
						}
						utf8string::str_t oldSection = sCurrentSection;
						if(this->ExtractSection(u8s.c_wcs(), 0, sCurrentSection))
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
							wss << L"Conversion error in file \"" << pFile->GetFullPath()().c_wcs()
								<< L"\", line " << line_counter;
							RETHROWEXCEPTION(wss.str().c_str(),&ex);
						}
						utf8string::str_t sKey, sValue;
						if(this->ExtractKeyValue(u8s.c_wcs(), 0, sKey, sValue))
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
			m_mapProps[*it].Print(outbuffer);
		}
		
		try
		{
			vfs::COpenWriteFile wfile(sFileName,true,true);
			wfile.file().Write(outbuffer.str().c_str(),outbuffer.str().length(),written);
		}
		catch(CBasicException& ex)
		{
			LogException(ex);
			vfs::CFile file(sFileName);
			if(file.OpenWrite(true,true))
			{
				file.Write(outbuffer.str().c_str(),outbuffer.str().length(),written);
				file.Close();
			}
		}
		return true;
	}
	return false;
}


void CPropertyContainer::PrintProperties()
{
	tSections::iterator pit = m_mapProps.begin();
	for(;pit != m_mapProps.end(); ++pit)
	{
		std::cout << "[" << pit->first.utf8() << "]\n";
		pit->second.Print(std::cout, L"  ");
		std::cout << std::endl;
	}
}

CPropertyContainer::CSection& CPropertyContainer::Section(utf8string const& sSection)
{
	return m_mapProps[sSection];
}

bool CPropertyContainer::GetValueForKey(utf8string const& sSection, utf8string const& sKey, utf8string &sValue)
{
	tSections::iterator pit = m_mapProps.find(vfs::TrimString(sSection,0,sSection.length()));
	if( pit != m_mapProps.end() )
	{
		return pit->second.value( vfs::TrimString(sKey,0,sKey.length()), sValue );
	}
	return false;
}

utf8string const& CPropertyContainer::GetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const& sDefaultValue)
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

bool CPropertyContainer::GetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string& sValue, utf8string const& sDefaultValue)
{
	if(GetValueForKey(sSection,sKey,sValue))
	{
		return true;
	}
	sValue = sDefaultValue;
	return false;
}

bool CPropertyContainer::GetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string::char_t* sValue, vfs::UInt32 len, utf8string const& sDefaultValue)
{
	utf8string s;
	if(GetValueForKey(sSection,sKey,s))
	{
		vfs::UInt32 l = std::min<vfs::UInt32>(s.length(), len-1);
		wcsncpy(sValue,s.c_wcs().c_str(), l);
		sValue[l] = 0;
		return true;
	}
	vfs::UInt32 l = std::min<vfs::UInt32>(sDefaultValue.length(), len-1);
	wcsncpy(sValue,sDefaultValue.c_wcs().c_str(), l);
	sValue[l] = 0;
	return false;
}

vfs::Int64 CPropertyContainer::GetIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue, vfs::Int64 iMinValue, vfs::Int64 iMaxValue)
{
	return std::min<vfs::Int64>(iMaxValue, std::max<vfs::Int64>(iMinValue, GetIntProperty(sSection, sKey, iDefaultValue)));
}

vfs::Int64 CPropertyContainer::GetIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		vfs::Int64 iRetVal;
		if(ConvertTo<vfs::Int64>(sValue,iRetVal))
		{
			return iRetVal;
		}
	}
	return iDefaultValue;
}

vfs::UInt64 CPropertyContainer::GetUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue, vfs::UInt64 iMinValue, vfs::UInt64 iMaxValue)
{
	return std::min<vfs::UInt64>(iMaxValue, std::max<vfs::UInt64>(iMinValue, GetIntProperty(sSection, sKey, iDefaultValue)));
}

vfs::UInt64 CPropertyContainer::GetUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		vfs::UInt64 iRetVal;
		if(ConvertTo<vfs::UInt64>(sValue,iRetVal))
		{
			return iRetVal;
		}
	}
	return iDefaultValue;
}

double CPropertyContainer::GetFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue, double  fMinValue, double fMaxValue)
{
	return std::min<double>(fMaxValue, std::max<double>(fMinValue, GetFloatProperty(sSection, sKey, fDefaultValue)));
}

double CPropertyContainer::GetFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		double fRetVal;
		if(ConvertTo<double>(sValue,fRetVal))
		{
			return fRetVal;
		}
	}
	return fDefaultValue;
}

bool CPropertyContainer::GetBoolProperty(utf8string const& sSection, utf8string const& sKey, bool bDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		int iRetVal;
		if( StrCmp::Equal(sValue,L"true") || ( ConvertTo<vfs::Int32>(sValue,iRetVal) && (iRetVal != 0) ) )
		{
			return true;
		}
		else if( StrCmp::Equal(sValue,"false") || ( ConvertTo<vfs::Int32>(sValue,iRetVal) && (iRetVal == 0) ) )
		{
			return false;
		}
		// else return bDefaultValue
	}
	return bDefaultValue;
}

bool CPropertyContainer::GetStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> &lValueList, utf8string sDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.NextListEntry(entry))
		{
			lValueList.push_back(vfs::TrimString(entry,0,entry.length()));
		}
		return true;
	}
	return false;
}

bool CPropertyContainer::GetIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> &lValueList, vfs::Int64 iDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.NextListEntry(entry))
		{
			vfs::Int64 iRetVal;
			if(ConvertTo<vfs::Int64>(entry,iRetVal))
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

bool CPropertyContainer::GetUIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::UInt64> &lValueList, vfs::UInt64 iDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.NextListEntry(entry))
		{
			vfs::UInt64 iRetVal;
			if(ConvertTo<vfs::UInt64>(entry,iRetVal))
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
bool CPropertyContainer::GetFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> &lValueList, double fDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.NextListEntry(entry))
		{
			double fRetVal;
			if(ConvertTo<double>(entry,fRetVal))
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

bool CPropertyContainer::GetBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> &lValueList, bool bDefaultValue)
{
	utf8string sValue;
	if(GetValueForKey(sSection,sKey,sValue))
	{
		utf8string entry;
		CSplitStringList splitter(sValue);
		while(splitter.NextListEntry(entry))
		{
			int iRetVal;
			if( StrCmp::Equal(entry,L"true") || ( ConvertTo<vfs::Int32>(entry,iRetVal) && (iRetVal != 0) ) )
			{
				lValueList.push_back(true);
			}
			else if( StrCmp::Equal(entry,L"false") || ( ConvertTo<vfs::Int32>(entry,iRetVal) && (iRetVal == 0) ) )
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

void CPropertyContainer::SetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const& sValue)
{
	Section(sSection).value(sKey) = sValue;
}

void CPropertyContainer::SetIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 const& iValue)
{
	Section(sSection).value(sKey) = ToString<wchar_t,vfs::Int64>(iValue);
}
void CPropertyContainer::SetUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 const& iValue)
{
	Section(sSection).value(sKey) = ToString<wchar_t,vfs::UInt64>(iValue);
}
void CPropertyContainer::SetFloatProperty(utf8string const& sSection, utf8string const& sKey, double const& fValue)
{
	Section(sSection).value(sKey) = ToString<wchar_t,double>(fValue);
}
void CPropertyContainer::SetBoolProperty(utf8string const& sSection, utf8string const& sKey, bool const& bValue)
{
	Section(sSection).value(sKey) = ToString<wchar_t,bool>(bValue);
}

void CPropertyContainer::SetStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> const& slValue)
{
	Section(sSection).value(sKey) = ToStringList<utf8string>(slValue);
}

void CPropertyContainer::SetIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> const& ilValue)
{
	Section(sSection).value(sKey) = ToStringList<vfs::Int64>(ilValue);
}

void CPropertyContainer::SetFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> const& flValue)
{
	Section(sSection).value(sKey) = ToStringList<double>(flValue);
}

void CPropertyContainer::SetBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> const& blValue)
{
	Section(sSection).value(sKey) = ToStringList<bool>(blValue);
}

/**************************************************************************************************/
/**************************************************************************************************/

