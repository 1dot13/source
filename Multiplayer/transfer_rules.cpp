#include "transfer_rules.h"
#include "Debug.h"
#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/File/vfs_file.h>
#include <vfs/Tools/vfs_parser_tools.h>
#include <vfs/Tools/vfs_tools.h>

CTransferRules::CTransferRules()
: m_eDefaultAction(ACCEPT)
{};


bool CTransferRules::initFromTxtFile(vfs::Path const& sPath)
{
	// try to open via VirtualFileSystem
	if(getVFS()->fileExists(sPath))
	{
		return initFromTxtFile(getVFS()->getReadFile(sPath));
	}
	else
	{
		// file doesn't exist or VFS not initialized yet
		vfs::IBaseFile* pFile = new vfs::CFile(sPath);
		if(pFile)
		{
			bool success = initFromTxtFile(vfs::tReadableFile::cast(pFile));
			delete pFile;
			return success;
		}
	}
	return false;
}

bool CTransferRules::initFromTxtFile(vfs::tReadableFile* pFile)
{
	if(pFile && pFile->openRead())
	{
		vfs::COpenReadFile rfile(pFile);
		std::string sBuffer;
		vfs::UInt32 line_counter = 0;
		vfs::CReadLine rl(*pFile);
		while(rl.getLine(sBuffer))
		{
			line_counter++;
			// very simple parsing : key = value
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
					// comment -> do nothing
					break;
				default:
					::size_t iEnd = sBuffer.find_first_of(" \t", iStart);
					if(iEnd != std::string::npos)
					{
						SRule rule;
						std::string action = sBuffer.substr(iStart, iEnd - iStart);
						if( vfs::StrCmp::Equal(action, "deny") )
						{
							rule.action = CTransferRules::DENY;
						}
						else if( vfs::StrCmp::Equal(action, "accept") )
						{
							rule.action = CTransferRules::ACCEPT;
						}
						else
						{
							std::wstring trybuffer = L"Invalid UTF-8 character in string";
							VFS_IGNOREEXCEPTION( trybuffer = vfs::String(sBuffer).c_wcs(), "" ); /* just make sure we don't break off when string conversion fails */
							std::wstringstream wss;
							wss << L"Unknown action in file \"" << pFile->getPath().c_wcs()
								<< L", line " << line_counter << " : " << vfs::String(sBuffer).c_wcs();
							SGP_THROW(wss.str().c_str());
						}
						try
						{
							rule.pattern = vfs::Path(vfs::trimString(sBuffer, iEnd, sBuffer.length()));
						}
						catch(vfs::Exception& ex)
						{
							std::wstringstream wss;
							wss << L"Could not convert string, invalid utf8 encoding in file \"" << pFile->getPath().c_wcs()
								<< L"\", line "  << line_counter;
							SGP_RETHROW(wss.str().c_str(), ex);
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

void CTransferRules::setDefaultAction(CTransferRules::EAction act)
{
	m_eDefaultAction = act;
}

CTransferRules::EAction	CTransferRules::getDefaultAction()
{
	return m_eDefaultAction;
}

CTransferRules::EAction CTransferRules::applyRule(vfs::String const& sStr)
{
	tPatternList::iterator sit = m_listRules.begin();
	for(; sit != m_listRules.end(); ++sit)
	{
		if(matchPattern(sit->pattern(), sStr))
		{
			return sit->action;
		}
	}
	return m_eDefaultAction;
}

