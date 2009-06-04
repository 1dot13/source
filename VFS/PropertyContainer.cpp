#include "PropertyContainer.h"

#include "vfs.h"
#include "File/vfs_file.h"
#include "File/vfs_memory_file.h"
#include "vfs_file_raii.h"
#include "iteratedir.h"

#include <sstream>
#include <vector>

static const vfs::UInt32 BUFFER_SIZE = 1024;

class CReadLine
{
public:
	CReadLine(vfs::tReadableFile& rFile) : _file(rFile), _buffer_pos(0), _eof(false)
	{
		memset(_buffer,0,sizeof(_buffer));
		FillBuffer();
		vfs::UByte utf8bom[3] = {0xef,0xbb,0xbf};
		if(memcmp(utf8bom, &_buffer[0],3) == 0)
		{
			_buffer_pos += 3;
		}
	};
	bool FillBuffer()
	{
		if(!_eof)
		{
			// reading can be done byte-wise or line-wise.
			// we have no control over this here, so we have to account for both cases.

			vfs::UInt32 read = 0;
			// fill the buffer from the start, BUFFER_SIZE charactes at max (_buffer has BUFFER_SIZE+1 elements)
			_eof = !_file.Read(&_buffer[0],BUFFER_SIZE,read);
			// if read() fails, it means that we are at the end of the file,
			// but we probably have read a couple of valid bytes, so we need to do one last 'correct' run

			// bite-wise read files usually terminate a line with \n (or \r\n on WIN32)
			// line-wise read files just returns 0-terminated string
			
			// always terminate the string with 0
			_buffer[read] = 0;

			_buffer_pos = 0;
			_buffer_last = read;
			return true;
		}
		return false;
	}
	bool FromBuffer(std::string& line)
	{
		bool done = false;
		while(!done)
		{
			if(_buffer_pos < _buffer_last)
			{
				// start where we left last time
				vfs::Byte *temp = &_buffer[_buffer_pos];
				vfs::UInt32 start_pos = _buffer_pos;
				// go until we hit 0. since our buffer is always 0 terminated, the second test should be redundant.
				while(*temp && (_buffer_pos < _buffer_last))
				{
					// stop when we find a line terminator
					if(*temp == '\n' || *temp == '\r' /* || *temp == '\0' */)
					{
						break;
					}
					temp++;
					_buffer_pos++;
				}
				// need to append substring, as we might have refilles the buffer (because there was no \n or \r\n terminator)
				line.append( (char*)&_buffer[start_pos], _buffer_pos - start_pos );

				// if we reach the (real) end of the buffer (that always terminate with 0), this means
				// that there was no line terminator and that we have to refill the buffer.
				if( _buffer_pos < BUFFER_SIZE && (*temp == '\n' || *temp == '\r' || *temp == 0) )
				{
					// found the line terminator
					if(*temp == '\r')
					{
						// the \r is most probably followed by \n. 'swallow' both characters
						*temp++;
						_buffer_pos++;
						if( (_buffer_pos < BUFFER_SIZE) && (*temp == '\n' || *temp == 0) )
						{
							// increase buffer position, so that we can start with a valid character in the next run
							_buffer_pos++;
							return true;
						}
						else
						{
							done = !FillBuffer();
						}
					}
					else if(*temp == '\n' || *temp == 0)
					{
						// increase buffer position, so that we can start with a valid character in the next run
						_buffer_pos++;
						return true;
					}
				}
				else
				{
					done = !FillBuffer();
				}
			}
			else
			{
				done = !FillBuffer();
			}
		}
		return false;
	}
	bool GetLine(std::string& line)
	{
		line.clear();
		return FromBuffer(line);
	}
private:
	vfs::Byte			_buffer[BUFFER_SIZE+1];
	vfs::tReadableFile& _file;
	vfs::UInt32			_buffer_pos;
	vfs::UInt32			_buffer_last;
	bool				_eof;
};

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

class CSplitStringList
{
public:
	CSplitStringList(utf8string const& sList)
		: m_sList(sList), iCurrent(0),iNext(0)
	{};
	~CSplitStringList() {};

	bool NextListEntry(utf8string &sEntry)
	{
		if(iNext >= 0)
		{
			iNext = m_sList.c_wcs().find_first_of(L",",iCurrent+1);
			if(iNext > iCurrent)
			{
				sEntry.r_wcs().assign(vfs::TrimString(m_sList,iCurrent,iNext-1).c_wcs());
				iCurrent = iNext+1;
			}
			else
			{
				// last or only entry
				sEntry.r_wcs().assign(vfs::TrimString(m_sList,iCurrent,m_sList.length()).c_wcs());
			}
			return true;
		}
		return false;
	}
private:
	const utf8string	m_sList;
	int					iCurrent,iNext;
};


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

bool CTransferRules::InitFromTxtFile(vfs::Path const& sPath)
{
	// try to open via VirtualFileSystem
	if(GetVFS()->FileExists(sPath))
	{
		return InitFromTxtFile(GetVFS()->GetRFile(sPath));
	}
	else
	{
		// file doesn't exist or VFS not initialized yet
		vfs::IBaseFile* pFile = new vfs::CTextFile(sPath);
		if(pFile)
		{
			bool success = InitFromTxtFile(vfs::tReadableFile::Cast(pFile));
			delete pFile;
			return success;
		}
	}
	return false;
}

bool CTransferRules::InitFromTxtFile(vfs::tReadableFile* pFile)
{
	if(pFile && pFile->OpenRead())
	{
		vfs::COpenReadFile rfile(pFile);
		std::string sBuffer;
		vfs::UInt32 line_counter = 0;
		CReadLine rl(*pFile);
		while(rl.GetLine(sBuffer))
		{
			line_counter++;
			// very simple parsing : key = value
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
					// comment -> do nothing
					break;
				default:
					int iEnd = sBuffer.find_first_of(" \t", iStart);
					if(iEnd != std::string::npos)
					{
						SRule rule;
						std::string action = sBuffer.substr(iStart, iEnd - iStart);
						if( StrCmp::Equal(action, "deny") )
						{
							rule.action = CTransferRules::DENY;
						}
						else if( StrCmp::Equal(action, "accept") )
						{
							rule.action = CTransferRules::ACCEPT;
						}
						else
						{
							std::wstring trybuffer = L"Invalid UTF-8 character in string";
							IGNOREEXCEPTION( trybuffer = utf8string(sBuffer).c_wcs() ); /* just make sure we don't break off when string conversion fails */
							std::wstringstream wss;
							wss << L"Unknown action in file \"" << pFile->GetFullPath()().c_wcs()
								<< L", line " << line_counter << " : " << utf8string(sBuffer).c_wcs();
							THROWEXCEPTION(wss.str().c_str());
						}
						try
						{
							rule.pattern = vfs::Path(vfs::TrimString(sBuffer, iEnd, sBuffer.length()));
						}
						catch(CBasicException& ex)
						{
							std::wstringstream wss;
							wss << L"Could not convert string, invalid utf8 encoding in file \"" << pFile->GetFullPath()().c_wcs()
								<< L"\", line "  << line_counter;
							RETHROWEXCEPTION(wss.str().c_str(),&ex);
						}
						m_listRules.push_back(rule);
					}
					break;
				}; // end switch
			} // end if (empty)
		} // end while(!eof)
		return true;
	}
	return false;
}

void CTransferRules::SetDefaultAction(CTransferRules::EAction act)
{
	m_eDefaultAction = act;
}

CTransferRules::EAction	CTransferRules::GetDefaultAction()
{
	return m_eDefaultAction;
}

CTransferRules::EAction CTransferRules::ApplyRule(utf8string const& sStr)
{
	tPatternList::iterator sit = m_listRules.begin();
	for(; sit != m_listRules.end(); ++sit)
	{
		if(MatchPattern(sit->pattern(), sStr))
		{
			return sit->action;
		}
	}
	return m_eDefaultAction;
}

/********************************************************************/
/********************************************************************/
#ifdef WIN32
const char CLog::endl[] = "\r\n";
#else
const char CLog::endl[] = "\n";
#endif

std::list<CLog*>& CLog::_logs()
{
	static std::list<CLog*> logs;
	return logs;
}

CLog* CLog::Create(vfs::Path fileName, bool append, EFlushMode flushMode)
{
	_logs().push_back(new CLog(fileName, append, flushMode));
	return _logs().back();
}
void CLog::FlushFinally()
{
	std::list<CLog*>::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		delete *it;
	}
	_logs().clear();
}

void CLog::FlushAll()
{
	std::list<CLog*>::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		(*it)->Flush();
		(*it)->_file = NULL;
	}
}


CLog::CLog(vfs::Path fileName, bool append, EFlushMode flushMode)
:	_filename(fileName), _file(NULL), _own_file(false),
	_first_write(true), _flush_mode(flushMode), _append(append),
	_buffer_size(0), _buffer_test_size(512)
{};

CLog::~CLog()
{
	_test_flush(true);
	if(_file && _own_file)
	{
		delete _file;
	}
}

CLog& CLog::operator<<(unsigned int const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(unsigned short const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(unsigned char const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(int const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(short const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(char const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(float const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(double const& t)
{
	return PushNumber(t);
}

CLog& CLog::operator<<(const char* t)
{
	_buffer << t;
	_buffer_size += strlen(t);
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(const wchar_t* t)
{
	std::string s = utf8string::as_utf8(t);
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(std::string const& t)
{
	_buffer << t;
	_buffer_size += t.length();
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(std::wstring const& t)
{
	std::string s = utf8string::as_utf8(t);
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(utf8string const& t)
{
	std::string s = t.utf8();
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}

CLog& CLog::Endl()
{
	_buffer << CLog::endl;
	_buffer_size += sizeof(CLog::endl)-1;
	_test_flush();
	return *this;
}

void CLog::SetAppend(bool append)
{
	_append = append;
}

void CLog::SetBufferSize(vfs::UInt32 bufferSize)
{
	_buffer_test_size = bufferSize;
}

void CLog::_test_flush(bool force)
{
	if( (_flush_mode == FLUSH_IMMEDIATELY) || 
		(_flush_mode == FLUSH_BUFFER && _buffer_size > _buffer_test_size) || 
		(/*_flush_mode == FLUSH_ON_DELETE &&*/ force == true) )
	{
		Flush();
	}
}

#include <ctime>
void CLog::Flush()
{
	vfs::UInt32 buflen = _buffer.str().length();
	if(buflen == 0)
	{
		return;
	}

	if(!_file)
	{
		try
		{
			vfs::COpenWriteFile file_raii(_filename,true,!_append);
			_file = &file_raii.file();
			file_raii.release();
		}
		catch(CBasicException& ex)
		{
			LogException(ex);
			// write file anyway 
			_file = vfs::tWriteableFile::Cast(new vfs::CFile(_filename));
			_file->OpenWrite(true,!_append);
			_own_file = true;
		}
	}

	vfs::COpenWriteFile wfile(_file);
	if(_append)
	{
		wfile.file().SetWriteLocation(0,vfs::IBaseFile::SD_END);
	}

	vfs::UInt32 io;
	if(_first_write)
	{
		time_t rawtime;
		time ( &rawtime );
		std::string datetime(ctime(&rawtime));
		std::string s_out;
	
		vfs::UInt32 wloc = wfile.file().GetWriteLocation();
		if(wloc > 0)
		{
			s_out = CLog::endl;
		}
		s_out += " *** ";
		s_out += datetime.substr(0,datetime.length()-1);
		s_out += " *** ";
		s_out += CLog::endl;
		s_out += CLog::endl;
		wfile.file().Write(s_out.c_str(), s_out.length(), io);
		_first_write = false;
	}

	wfile.file().Write(_buffer.str().c_str(), buflen, io);
	_buffer.str("");
	_buffer.clear();
	_buffer_size = 0;

	_append = true;
}

