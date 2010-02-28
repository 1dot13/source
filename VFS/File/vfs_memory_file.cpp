#include "vfs_memory_file.h"
#include "../vfs_file_raii.h"

#include <vector>

#define ERROR_FILE(msg)			BuildString().add(L"[").add(this->getPath()).add(L"] - ").add(msg).get()

#ifdef _DEBUG
	#define IS_FILE_VALID()		THROWIFFALSE( m_buffer.good(), ERROR_FILE(L"invalid file object") );
#else
	#define IS_FILE_VALID()		THROWIFFALSE( m_buffer.good(), ERROR_FILE(L"invalid file object") );
#endif

static inline std::ios::seekdir _seekDir(vfs::IBaseFile::ESeekDir seekDir)
{
	if(seekDir == vfs::IBaseFile::SD_BEGIN)
	{
		return std::ios::beg;
	}
	else if(seekDir == vfs::IBaseFile::SD_CURRENT)
	{
		return std::ios::cur;
	}
	else if(seekDir == vfs::IBaseFile::SD_END)
	{
		return std::ios::end;
	}
	THROWEXCEPTION(BuildString().add(L"Unknown seek direction [").add(seekDir).add(L"]").get());
}

///////////////////////////////////////////////////

vfs::CMemoryFile::CMemoryFile()
: tBaseClass(vfs::Path()), m_isOpen_read(false), m_isOpen_write(false)
{
	m_buffer.str("");
}

vfs::CMemoryFile::CMemoryFile(vfs::Path const& fileName)
: tBaseClass(fileName), m_isOpen_read(false), m_isOpen_write(false)
{
	m_buffer.str("");
}

vfs::CMemoryFile::~CMemoryFile()
{
	m_buffer.str("");
	m_buffer.clear();
}

vfs::FileAttributes vfs::CMemoryFile::getAttributes()
{
	return vfs::FileAttributes(vfs::FileAttributes::ATTRIB_NORMAL, vfs::FileAttributes::LT_NONE);
}

void vfs::CMemoryFile::close()
{
	m_buffer.clear();
	m_isOpen_read = false;
	m_isOpen_write = false;
	IS_FILE_VALID();
}
vfs::size_t vfs::CMemoryFile::getSize()
{
	IS_FILE_VALID();

	std::streampos size = 0;
	if(!m_buffer.str().empty())
	{
		std::streampos current_position = m_buffer.tellg();
		m_buffer.seekg(0,std::ios::end);
		size = m_buffer.tellg();
		m_buffer.seekg(current_position,std::ios::beg);
	}

	IS_FILE_VALID();
	return (vfs::size_t)size;
}

bool vfs::CMemoryFile::isOpenRead()
{
	return m_isOpen_read;
}

bool vfs::CMemoryFile::openRead()
{
	if(!m_buffer.good())
	{
		return false;
	}
	return m_isOpen_read = true;
}
vfs::size_t vfs::CMemoryFile::read(vfs::Byte* data, vfs::size_t bytesToRead)
{
	if(!m_buffer.eof())
	{
		IS_FILE_VALID();
		THROWIFFALSE(m_isOpen_read || this->openRead(), ERROR_FILE(L"open error"));

		m_buffer.read(static_cast<Byte*>(data), bytesToRead);
		std::streamsize bytesRead = m_buffer.gcount();

		THROWIFFALSE( m_buffer.good() || m_buffer.eof(), ERROR_FILE(L"read error") );
		return (vfs::size_t)bytesRead;
	}
	return 0;
}

vfs::size_t vfs::CMemoryFile::getReadPosition()
{
	IS_FILE_VALID();
	THROWIFFALSE(m_isOpen_read || this->openRead(), ERROR_FILE(L"open error"));

	return (vfs::size_t)m_buffer.tellg();
}

void vfs::CMemoryFile::setReadPosition(vfs::size_t positionInBytes)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_read || this->openRead(), ERROR_FILE(L"open error") );

	m_buffer.seekg((std::streamoff)positionInBytes);

	IS_FILE_VALID();
}
void vfs::CMemoryFile::setReadPosition(vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_read || this->openRead(), ERROR_FILE(L"open error") );

	std::ios::seekdir ioSeekDir;
	TRYCATCH_RETHROW(ioSeekDir = _seekDir(seekDir), ERROR_FILE(L"seek error"));
	m_buffer.seekg(offsetInBytes, ioSeekDir);

	IS_FILE_VALID();
}

bool vfs::CMemoryFile::isOpenWrite()
{
	return m_isOpen_write;
}

bool vfs::CMemoryFile::openWrite(bool bCreateWhenNotExist, bool bTruncate)
{
	if( !m_buffer.good() )
		return false;
	if( m_isOpen_write )
		return true;

	if(bTruncate)
	{
		m_buffer.str("");
		m_buffer.clear();
	}
	m_isOpen_write = m_buffer.good();
	return m_isOpen_write;
}

vfs::size_t vfs::CMemoryFile::write(const vfs::Byte* data, vfs::size_t bytesToWrite)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	std::streampos start = 0;
	if(!m_buffer.str().empty())
	{
		start = m_buffer.tellp();
	}
	THROWIFFALSE( m_buffer.write(data, bytesToWrite), ERROR_FILE(L"write error") );
	std::streampos bytesWritten = m_buffer.tellp() - start;

	IS_FILE_VALID();

	return (vfs::size_t)bytesWritten;
}

vfs::size_t vfs::CMemoryFile::getWritePosition()
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	return (vfs::size_t)m_buffer.tellp();
}

void vfs::CMemoryFile::setWritePosition(vfs::size_t positionInBytes)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	m_buffer.seekp((std::streamoff)positionInBytes);

	IS_FILE_VALID();
}

void vfs::CMemoryFile::setWritePosition(vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_write || this->openWrite(), ERROR_FILE(L"open error") );

	std::ios::seekdir ioSeekDir;
	TRYCATCH_RETHROW( ioSeekDir = _seekDir(seekDir), ERROR_FILE(L"seek error") );
	m_buffer.seekp(offsetInBytes, ioSeekDir);

	IS_FILE_VALID();
}

void vfs::CMemoryFile::copyToBuffer(vfs::tReadableFile& rFile)
{
	try
	{
		bool needToClose = !rFile.isOpenRead();
		vfs::COpenReadFile readfile(&rFile);
		if(!needToClose)
		{
			readfile.release();
		}

		typedef std::vector<vfs::Byte> tByteVector;

		vfs::size_t size = rFile.getSize();
		tByteVector vBuffer(size);

		rFile.read(&vBuffer[0], size);
		this->write(&vBuffer[0], size);
	}
	catch(CBasicException& ex)
	{
		RETHROWEXCEPTION(ERROR_FILE(L""), &ex);
	}
}


bool vfs::CMemoryFile::deleteFile()
{
	m_buffer.clear();
	m_buffer.str("");
	return m_buffer.good();
}
