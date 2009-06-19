#include "ParserTools.h"
#include "Tools.h"
#include "../vfs.h"
#include "../File/vfs_file.h"
#include "../vfs_file_raii.h"

/*************************************************************************************/
/*************************************************************************************/

CReadLine::CReadLine(vfs::tReadableFile& rFile)
: _file(rFile), _buffer_pos(0), _eof(false)
{
	memset(_buffer,0,sizeof(_buffer));
	FillBuffer();
	vfs::UByte utf8bom[3] = {0xef,0xbb,0xbf};
	if(memcmp(utf8bom, &_buffer[0],3) == 0)
	{
		_buffer_pos += 3;
	}
};

bool CReadLine::FillBuffer()
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

bool CReadLine::FromBuffer(std::string& line)
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

bool CReadLine::GetLine(std::string& line)
{
	line.clear();
	return FromBuffer(line);
}

/*************************************************************************************/
/*************************************************************************************/

CSplitStringList::CSplitStringList(utf8string const& sList)
: m_sList(sList), iCurrent(0),iNext(0)
{};

CSplitStringList::~CSplitStringList()
{};

bool CSplitStringList::NextListEntry(utf8string &sEntry)
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

/*************************************************************************************/
/*************************************************************************************/

CTransferRules::CTransferRules()
: m_eDefaultAction(ACCEPT)
{};


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

/*************************************************************************************/
/*************************************************************************************/

#ifdef USE_CODE_PAGE

typedef vfs::UInt16 UINT16;
extern UINT16 gsKeyTranslationTable[1024];

CodePageReader::EMode CodePageReader::ExtractMode(std::string const &readStr, size_t startPos)
{
	vfs::Int32 iEqual = readStr.find_first_of("=",startPos);
	if(iEqual < 0)
	{
		return Error;
	}
	// extract keyword
	std::string key = vfs::TrimString(readStr,0,iEqual-1);
	if(!StrCmp::Equal(key, "mode"))
	{
		return Error;
	}
	// extract mode
	EMode mode = Error;
	std::string sMode = vfs::TrimString(readStr,iEqual+1,readStr.size());
	if(StrCmp::Equal(sMode, "normal"))
	{
		mode = Normal;
	}
	else if(StrCmp::Equal(sMode, "shift"))
	{
		mode = Shift;
	}
	else if(StrCmp::Equal(sMode, "ctrl"))
	{
		mode = Ctrl;
	}
	else if(StrCmp::Equal(sMode, "alt"))
	{
		mode = Alt;
	}
	return mode;
}

bool CodePageReader::ExtractCode(std::string const& str, int iStart, vfs::UInt32& rInsertPoint, utf8string::str_t& u8s)
{
	vfs::Int32 iEqual = str.find_first_of("=",iStart);
	if(iEqual < 0)
	{
		return false;
	}
	// extract insert point
	std::string sip = vfs::TrimString(str,0,iEqual-1);
	if(!ConvertTo<vfs::UInt32>(sip, rInsertPoint))
	{
		return false;
	}
	utf8string::str_t u8temp;
	IGNOREEXCEPTION(utf8string::as_utf16(str.substr(iEqual, str.length()-iEqual), u8temp));

	vfs::Int32 iCodeStart, iCodeEnd;
	iCodeStart = u8temp.find_first_of(L"{", 0);
	if(iCodeStart < 0)
	{
		return false;
	}
	iCodeEnd = u8temp.find_last_of(L"}");
	if(iCodeEnd < 0)
	{
		return false;
	}
	u8s = u8temp.substr(iCodeStart+1, iCodeEnd-iCodeStart-1);
	return true;
}

void CodePageReader::ReadCodePage(vfs::Path const& codepagefile)
{
	if(!GetVFS()->FileExists(codepagefile))
	{
		return;
	}
	vfs::COpenReadFile rfile(codepagefile);
	CReadLine rline(rfile.file());
	std::string sBuffer;
	vfs::UInt32 line_counter = 0;
	EMode mode = Normal;
	while(rline.GetLine(sBuffer))
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
			case 'm':
				mode = ExtractMode(sBuffer,iStart);
				if(mode == Error)
				{
					utf8string::str_t u8s;
					IGNOREEXCEPTION(utf8string::as_utf16(sBuffer, u8s));
					std::wstringstream wss;
					wss << L"Could not determine mode from line [" << line_counter << L"] : " << u8s;
					THROWEXCEPTION(wss.str().c_str());
				}
				break;
			default:
				utf8string::str_t u8s;
				vfs::UInt32 uiInsertPoint;
				if(!ExtractCode(sBuffer, iStart,uiInsertPoint,u8s))
				{
					continue;
				}
				if(mode == Shift) uiInsertPoint += 256;
				else if(mode == Ctrl) uiInsertPoint += 512;
				else if(mode == Alt) uiInsertPoint += 768;

				if(uiInsertPoint >= 1023)
				{
					continue;
				}
				for(unsigned int i=0; i < u8s.length(); ++i)
				{
					if(uiInsertPoint+i < 1024)
					{
						gsKeyTranslationTable[uiInsertPoint+i] = u8s.at(i);
					}
				}
			}
		}
	}
}

/*************************************************************************************/
/*************************************************************************************/

#include <set>

namespace charSet
{
	static std::map<ECharSet,std::set<wchar_t> > _sets;
};

inline bool TestSet(int char_set, charSet::ECharSet es, wchar_t wc)
{
	if(char_set & es)
	{
		std::set<wchar_t>& wcset = charSet::_sets[es];
		return wcset.find(wc) !=  wcset.end();
	}
	return false;
}

bool charSet::IsFromSet(char wc, unsigned int char_set)
{
	return charSet::IsFromSet((wchar_t)wc, char_set);
}
bool charSet::IsFromSet(int wc, unsigned int char_set)
{
	return charSet::IsFromSet((wchar_t)wc, char_set);
}
bool charSet::IsFromSet(unsigned int wc, unsigned int char_set)
{
	return charSet::IsFromSet((wchar_t)wc, char_set);
}

bool charSet::IsFromSet(wchar_t wc, unsigned int char_set)
{
	bool inSet = false;
	if( inSet |= TestSet(char_set, charSet::CS_SPACE, wc)) return true; 
	if( inSet |= TestSet(char_set, charSet::CS_ALPHA_LC, wc)) return true; 
	if( inSet |= TestSet(char_set, charSet::CS_ALPHA_UC, wc)) return true; 
	if( inSet |= TestSet(char_set, charSet::CS_NUMERIC, wc)) return true; 
	if( inSet |= TestSet(char_set, charSet::CS_SPECIAL_ALPHA_LC, wc)) return true; 
	if( inSet |= TestSet(char_set, charSet::CS_SPECIAL_ALPHA_UC, wc)) return true; 
	if( inSet |= TestSet(char_set, charSet::CS_SPECIAL_NON_CHAR, wc)) return true; 
	return false;
}

void charSet::AddToCharSet(ECharSet eset, std::wstring cset)
{
	std::set<wchar_t>& wcset = charSet::_sets[eset];
	for(unsigned int i = 0; i < cset.length(); ++i)
	{
		wcset.insert(cset.at(i));
	}
}

void charSet::InitializeCharSets()
{
	charSet::AddToCharSet(charSet::CS_SPACE,			L" ");
	charSet::AddToCharSet(charSet::CS_ALPHA_LC,			L"abcdefghijklmnopqrstuvwxyz");
	charSet::AddToCharSet(charSet::CS_ALPHA_UC,			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	charSet::AddToCharSet(charSet::CS_NUMERIC,			L"0123456789");
	charSet::AddToCharSet(charSet::CS_SPECIAL_ALPHA_LC,	L"äöüßáàâéèêóòôúùô");
	charSet::AddToCharSet(charSet::CS_SPECIAL_ALPHA_UC,	L"ÄÖÜÁÀÂÉÈÊÓÒÔÚÙÛ");
	charSet::AddToCharSet(charSet::CS_SPECIAL_NON_CHAR,	L"~*+-_.,:;'´`#°^!\"§$%&/()=?\\{}[]");
}

#endif // USE_CODE_PAGE

/*************************************************************************************/
/*************************************************************************************/
