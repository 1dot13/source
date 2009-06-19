#include "vfs_memory_file.h"
#include "../vfs_file_raii.h"

#include <vector>

vfs::CMemoryFile::CMemoryFile()
: vfs::IFileTemplate<vfs::IReadable,vfs::IWriteable>(vfs::Path()), m_bIsOpen_read(false), m_bIsOpen_write(false)
{
	m_ssBuffer.str("");
}

vfs::CMemoryFile::CMemoryFile(vfs::Path const& sFileName)
: vfs::IFileTemplate<vfs::IReadable,vfs::IWriteable>(sFileName), m_bIsOpen_read(false), m_bIsOpen_write(false)
{
	m_ssBuffer.str("");
}

vfs::CMemoryFile::~CMemoryFile()
{
	m_ssBuffer.str("");
	m_ssBuffer.clear();
}

bool vfs::CMemoryFile::Close()
{
	m_ssBuffer.clear();
	m_bIsOpen_read = false;
	m_bIsOpen_write = false;
	return m_ssBuffer.good();
}
bool vfs::CMemoryFile::GetFileSize(UInt32& uiFileSize)
{
	uiFileSize = 0;
	if(!m_ssBuffer.good())
	{
		return false;
	}
	std::ios::pos_type current_position = 0;
	if(!m_ssBuffer.str().empty())
	{
		current_position = m_ssBuffer.tellg();
		m_ssBuffer.seekg(0,std::ios::end);
		uiFileSize = m_ssBuffer.tellg();
		m_ssBuffer.seekg(current_position,std::ios::beg);
	}
	return m_ssBuffer.good();
}

bool vfs::CMemoryFile::IsOpenRead()
{
	return m_bIsOpen_read;
}

bool vfs::CMemoryFile::OpenRead()
{
	m_bIsOpen_read = m_ssBuffer.good();
	return m_bIsOpen_read;
}
bool vfs::CMemoryFile::Read(Byte* pData, UInt32 uiBytesToRead, UInt32& uiBytesRead)
{
	if(!m_bIsOpen_read && !this->OpenRead())
	{
		return false;
	}
	m_ssBuffer.read(static_cast<Byte*>(pData),uiBytesToRead);
	uiBytesRead = m_ssBuffer.gcount();
	return m_ssBuffer.good();
}

vfs::UInt32 vfs::CMemoryFile::GetReadLocation()
{
	if(!m_bIsOpen_read && this->OpenRead())
	{
		return false;
	}
	return m_ssBuffer.tellg();
}

bool vfs::CMemoryFile::SetReadLocation(UInt32 uiPositionInBytes)
{
	if(!m_ssBuffer.good())
	{
		return false;
	}
	m_ssBuffer.seekg(uiPositionInBytes);
	return m_ssBuffer.good();
}
bool vfs::CMemoryFile::SetReadLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir)
{
	if(!m_bIsOpen_read && !this->OpenRead())
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
	m_ssBuffer.seekg(uiOffsetInBytes,ioSeekDir);
	return m_ssBuffer.good();
}

bool vfs::CMemoryFile::IsOpenWrite()
{
	return m_bIsOpen_write;
}

bool vfs::CMemoryFile::OpenWrite(bool bCreateWhenNotExist, bool bTruncate)
{
	if(bTruncate)
	{
		m_ssBuffer.str("");
		m_ssBuffer.clear();
	}
	m_bIsOpen_write = m_ssBuffer.good();
	return m_bIsOpen_write;
}

bool vfs::CMemoryFile::Write(const Byte* pData, UInt32 uiBytesToWrite, UInt32& uiBytesWritten)
{
	if(!m_bIsOpen_write && !this->OpenWrite())
	{
		return false;
	}
	vfs::UInt64 start = 0;
	if(!m_ssBuffer.str().empty())
	{
		start = m_ssBuffer.tellp();
	}
	m_ssBuffer.write(pData,uiBytesToWrite);
	vfs::UInt64 end = m_ssBuffer.tellp();
	uiBytesWritten = end-start;
	if(uiBytesWritten != uiBytesToWrite)
	{
		return false;
	}
	return m_ssBuffer.good();
}

vfs::UInt32 vfs::CMemoryFile::GetWriteLocation()
{
	return m_ssBuffer.tellp();
}

bool vfs::CMemoryFile::SetWriteLocation(Int32 uiPositionInBytes)
{
	if(!m_bIsOpen_write && !this->OpenWrite())
	{
		return false;
	}
	m_ssBuffer.seekp(uiPositionInBytes);
	return m_ssBuffer.good();
}

bool vfs::CMemoryFile::SetWriteLocation(Int32 uiOffsetInBytes, IBaseFile::ESeekDir eSeekDir)
{
	if(!m_bIsOpen_write && !this->OpenWrite())
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
	m_ssBuffer.seekp(uiOffsetInBytes,ioSeekDir);
	return m_ssBuffer.good();
}

bool vfs::CMemoryFile::CopyToBuffer(vfs::tReadableFile& rFile)
{
	bool needToClose = !rFile.IsOpenRead();
	vfs::COpenReadFile readfile(&rFile);
	if(!needToClose)
	{
		readfile.release();
	}
	vfs::UInt32 uiSize,uiIO;

	uiSize = rFile.GetFileSize();
	std::vector<Byte> vBuffer(uiSize);
	if( rFile.Read(&vBuffer[0],uiSize,uiIO) && (uiSize == uiIO) )
	{
		if( this->Write(&vBuffer[0],uiSize,uiIO) && (uiSize == uiIO) )
		{
			return true;
		}
	}
	return false;
}


bool vfs::CMemoryFile::Delete()
{
	m_ssBuffer.clear();
	m_ssBuffer.str("");
	return m_ssBuffer.good();
}
