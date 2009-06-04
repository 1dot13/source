#include "vfs_file.h"
#include <cassert>

#include <sys/stat.h>

vfs::CFile::CFile(vfs::Path const& sFileName) 
: vfs::IFileTemplate<vfs::IReadable,vfs::IWriteable>(sFileName), m_bIsOpen_read(false), m_bIsOpen_write(false)
{
}

vfs::CFile::~CFile()
{
	m_oFile.clear();
	if(m_bIsOpen_read || m_bIsOpen_write)
	{
		this->Close();
	}
}

bool vfs::CFile::Delete()
{
	Close();
	if(remove(m_sFileName().utf8().c_str()) == 0)
	{
		return true;
	}
	return false;
}

bool vfs::CFile::Close()
{
	m_oFile.clear();
	if( (m_bIsOpen_read || m_bIsOpen_write) && m_oFile.good() )
	{
		m_oFile.close();
		m_bIsOpen_read = false;
		m_bIsOpen_write = false;
	}
	return m_oFile.good();
}

bool vfs::CFile::IsOpenRead()
{
	return m_bIsOpen_read;
}

bool vfs::CFile::OpenRead()
{
	if(!m_oFile.good())
	{	
		return false;
	}
	if(m_bIsOpen_read)
	{
		return true;
	}
	std::ios::openmode Mode;
	// try to open 
	Mode = std::ios::in|std::ios::binary;
#ifdef WIN32
	m_oFile.open(m_sFileName().c_wcs().c_str(), Mode);
#else
	m_oFile.open(m_sFileName().utf8().c_str(), Mode);
#endif
	m_bIsOpen_read = m_oFile.is_open() && m_oFile.good();
	return m_bIsOpen_read;
}

bool vfs::CFile::Read(vfs::Byte* pData, vfs::UInt32 uiBytesToRead, vfs::UInt32& uiBytesRead)
{

	uiBytesRead = 0;
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return false;
	}
	m_oFile.read(static_cast<char*>(pData),uiBytesToRead);
	uiBytesRead = m_oFile.gcount();
	return m_oFile.good();
}

vfs::UInt32 vfs::CFile::GetReadLocation()
{
	return m_oFile.tellg();
}
bool vfs::CFile::SetReadLocation(vfs::UInt32 uiPositionInBytes)
{
	if(m_oFile.good())
	{
		if(m_bIsOpen_read || this->OpenRead())
		{
			m_oFile.seekg(uiPositionInBytes);
		}
	}
	return m_oFile.good();
}

bool vfs::CFile::SetReadLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir)
{
	if(!m_bIsOpen_read && !OpenRead())
	{
		return false;
	}
	std::ios::seekdir ioSeekDir;
	if(eSeekDir == IBaseFile::SD_BEGIN)
	{
		ioSeekDir = std::ios::beg;
	}
	else if(eSeekDir == IBaseFile::SD_CURRENT)
	{
		ioSeekDir = std::ios::cur;
	}
	else if(eSeekDir == IBaseFile::SD_END)
	{
		ioSeekDir = std::ios::end;
	}
	else
	{
		return m_oFile.good();
	}
	m_oFile.seekg(uiOffsetInBytes,ioSeekDir);
	return m_oFile.good();
}

bool vfs::CFile::IsOpenWrite()
{
	return m_bIsOpen_write;
}

bool vfs::CFile::OpenWrite(bool bCreateWhenNotExist, bool bTruncate)
{
	if(!m_oFile.good())
	{	
		return false;
	}
	if(m_bIsOpen_write)
	{
		return true;
	}
	std::ios::openmode Mode;
	Mode = std::ios::in|std::ios::out|std::ios::binary;
	if(bTruncate)
	{
		Mode |= std::ios::trunc;
	}
#ifdef WIN32
	m_oFile.open(m_sFileName().c_wcs().c_str(),Mode);
#else
	m_oFile.open(m_sFileName().utf8().c_str(),Mode);
#endif
	if( !(m_bIsOpen_write = m_oFile.is_open()) )
	{
		m_oFile.clear();
		if(bCreateWhenNotExist)
		{
			// create file 
			Mode = std::ios::out|std::ios::binary;
			if(bTruncate)
			{
				Mode |= std::ios::trunc;
			}
			//m_oFile.open(VfsString(sFileName).AsString().c_str(),Mode);
#ifdef WIN32
			m_oFile.open(m_sFileName().c_wcs().c_str(),Mode);
#else
			m_oFile.open(m_sFileName().utf8().c_str(),Mode);
#endif
			if( !(m_bIsOpen_write = m_oFile.is_open()) )
			{
				return false;
			}
			m_oFile.close();
			Mode |= std::ios::in;
#ifdef WIN32
			m_oFile.open(m_sFileName().c_wcs().c_str(),Mode);
#else
			m_oFile.open(m_sFileName().utf8().c_str(),Mode);
#endif
			if( !(m_bIsOpen_write = m_oFile.is_open()) )
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return m_oFile.good();
}

bool vfs::CFile::Write(const vfs::Byte* pData, vfs::UInt32 uiBytesToWrite, vfs::UInt32& uiBytesWritten)
{
	uiBytesWritten = 0;
	// expect file to be opened (if not, should i open it? with what parameters?)
	if(!m_bIsOpen_write && !this->OpenWrite(true))
	{
		return false;
	}
	vfs::UInt64 start = m_oFile.tellp();
	if(!m_oFile.write(pData,uiBytesToWrite))
	{
		return false;
	}
	vfs::UInt64 end = m_oFile.tellp();
	uiBytesWritten = end-start;
	if(uiBytesWritten != uiBytesToWrite)
	{
		return false;
	}
	return m_oFile.good();
}	

vfs::UInt32 vfs::CFile::GetWriteLocation()
{
	return m_oFile.tellp();
}
bool vfs::CFile::SetWriteLocation(vfs::Int32 uiPositionInBytes)
{
	if(m_oFile.good())
	{
		if(!m_bIsOpen_write && !this->OpenWrite(true))
		{
			return false;
		}
		m_oFile.seekp(uiPositionInBytes);
	}
	return m_oFile.good();
}

bool vfs::CFile::SetWriteLocation(Int32 uiOffsetInBytes, vfs::IBaseFile::ESeekDir eSeekDir)
{
	if(m_oFile.good())
	{
		if(!m_bIsOpen_write && !this->OpenWrite(true))
		{
			return false;
		}
		std::ios::seekdir ioSeekDir;
		if(eSeekDir == IBaseFile::SD_BEGIN)
		{
			ioSeekDir = std::ios::beg;
		}
		else if(eSeekDir == IBaseFile::SD_CURRENT)
		{
			ioSeekDir = std::ios::cur;
		}
		else if(eSeekDir == IBaseFile::SD_END)
		{
			ioSeekDir = std::ios::end;
		}
		else
		{
			return false;
		}
		m_oFile.seekp(uiOffsetInBytes,ioSeekDir);
	}
	return m_oFile.good();
}

bool vfs::CFile::GetFileSize(UInt32& uiFileSize)
{
	uiFileSize = 0;
	if(!m_oFile.good())
	{
		return false;
	}
	bool closeAtExit = !m_bIsOpen_read;
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return false;
	}
	// save the current position 
	std::ios::pos_type current_position =  m_oFile.tellg();
	// move to end of the file
	m_oFile.seekg(0,std::ios::end);
	std::ios::pos_type file_size = m_oFile.tellg();
	// move to old position
	m_oFile.seekg(current_position,std::ios::beg);
	assert(current_position == m_oFile.tellg());
	uiFileSize = file_size;
	if(closeAtExit)
	{
		this->Close();
	}
	return m_oFile.good();
}


/******************************************************************/
/******************************************************************/

bool vfs::CTextFile::OpenRead()
{
	if(m_bIsOpen_read)
	{
		return true;
	}
	std::ios::openmode Mode;
	Mode = std::ios::in;
#ifdef WIN32
	m_oFile.open(m_sFileName().c_wcs().c_str(),Mode);
#else
	m_oFile.open(m_sFileName().utf8().c_str(),Mode);
#endif
	if(!(m_bIsOpen_read = m_oFile.is_open()))
	{
		return false;
	}
	return m_bIsOpen_read && m_oFile.good();
}


bool vfs::CTextFile::OpenWrite(bool bCreateWhenNotExist, bool bTruncate)
{
	if(m_oFile.good())
	{
		if(m_bIsOpen_write)
		{
			return true;
		}
		std::ios::openmode Mode;
		Mode = std::ios::out|std::ios::app;
#ifdef WIN32
		m_oFile.open(m_sFileName().c_wcs().c_str(),Mode);
#else
		m_oFile.open(m_sFileName().utf8().c_str(),Mode);
#endif
		if(!m_oFile.is_open())
		{
			m_oFile.clear();
			if(bCreateWhenNotExist)
			{
				// create file 
				Mode = std::ios::out;
				Mode |= bTruncate ? std::ios::trunc : std::ios::app;
#ifdef WIN32
				m_oFile.open(m_sFileName().c_wcs().c_str(),Mode);
#else
				m_oFile.open(m_sFileName().utf8().c_str(),Mode);
#endif
				if(!m_oFile.is_open())
				{
					return false;
				}
			}
			else
			{
				m_bIsOpen_write = false;
				return false;
			}
		}
		m_bIsOpen_write = true;
	}
	return m_oFile.good();
}

bool vfs::CTextFile::ReadLine(std::string &sLine, UInt32 uiMaxNumChars)
{
	if(!m_bIsOpen_read && !OpenRead())
	{
		return false;
	}
	if(!m_oFile.good())
	{
		return false;
	}
	if(!m_oFile.eof())
	{
		char *text = new char[uiMaxNumChars+1];
		m_oFile.getline(text,uiMaxNumChars);
		sLine.assign(text);
		delete[] text;
		return m_oFile.good();
	}
	return false;
}

bool vfs::CTextFile::Read(Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead)
{
	if(!m_bIsOpen_read)
	{
		OpenRead();
	}
	if(!m_oFile.good())
	{
		return false;
	}
	if(!m_oFile.eof())
	{
		m_oFile.getline((char*)pData,uiBytesToRead);
		uiBytesRead = m_oFile.gcount();
		return m_oFile.good();
	}
	return false;
}


bool vfs::CTextFile::Write(const Byte* pData, UInt32 uiBytesToWrite, UInt32& uiBytesWritten)
{
	// TODO
	return false;
}



bool vfs::CTextFile::WriteLine(std::string const& sLine)
{
	UInt32 uiWritten, uiToWrite=sLine.size();
	return CFile::Write(sLine.c_str(), uiToWrite ,uiWritten) && (uiWritten == uiToWrite);
}



