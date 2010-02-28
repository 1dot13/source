#include "vfs_file.h"
#include "../vfs_debug.h"
#include "../vfs_settings.h"
#include "../os_functions.h"

#include <sys/stat.h>

#define ERROR_FILE(msg)		BuildString().add(L"[").add(this->getPath()).add(L"] - ").add(msg).get()

#ifdef _DEBUG
	#define IS_FILE_VALID()	THROWIFFALSE( m_file.good(), ERROR_FILE(BuildString().add(L"invalid file object").add(m_file.eof() ? L" [EOF]" : L"").get()) );
#else
	#define IS_FILE_VALID()	THROWIFFALSE( m_file.good(), ERROR_FILE(BuildString().add(L"invalid file object").add(m_file.eof() ? L" [EOF]" : L"").get()) );
#endif

static inline bool hasAttrib(vfs::UInt32 const& attrib, vfs::UInt32 Attribs)
{
	return attrib == (attrib & Attribs);
}

static inline void copyAttributes(vfs::UInt32 osFileAttributes, vfs::UInt32& vfsFileAttributes)
{
	if(os::FileAttributes::ATTRIB_ARCHIVE == (os::FileAttributes::ATTRIB_ARCHIVE & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_ARCHIVE;
	}
	if(os::FileAttributes::ATTRIB_COMPRESSED == (os::FileAttributes::ATTRIB_COMPRESSED & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_COMPRESSED;
	}
	if(os::FileAttributes::ATTRIB_DIRECTORY == (os::FileAttributes::ATTRIB_DIRECTORY & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_DIRECTORY;
	}
	if(os::FileAttributes::ATTRIB_HIDDEN == (os::FileAttributes::ATTRIB_HIDDEN & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_HIDDEN;
	}
	if(os::FileAttributes::ATTRIB_NORMAL == (os::FileAttributes::ATTRIB_NORMAL & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_NORMAL;
	}
	if(os::FileAttributes::ATTRIB_OFFLINE == (os::FileAttributes::ATTRIB_OFFLINE & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_OFFLINE;
	}
	if(os::FileAttributes::ATTRIB_READONLY == (os::FileAttributes::ATTRIB_READONLY & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_READONLY;
	}
	if(os::FileAttributes::ATTRIB_SYSTEM == (os::FileAttributes::ATTRIB_SYSTEM & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_SYSTEM;
	}
	if(os::FileAttributes::ATTRIB_TEMPORARY == (os::FileAttributes::ATTRIB_TEMPORARY & osFileAttributes))
	{
		vfsFileAttributes |= vfs::FileAttributes::ATTRIB_TEMPORARY;
	}
}

static inline void _openFile(std::fstream& file, vfs::Path const& filename, std::ios::openmode mode)
{
#ifdef _MSC_VER
	utf8string::str_t const& fname = filename.c_wcs();
	vfs::Settings::getUseUnicode() ? 
		file.open( fname.c_str(), mode ) :
		file.open( utf8string::narrow(fname.c_str(),fname.length()).c_str(), mode );
#else
	file.open(utf8string::as_utf8(filename()).c_str(), mode);
#endif
}

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

template<typename WriteType>
vfs::TFile<WriteType>::TFile(vfs::Path const& filename) 
: tBaseClass(filename), m_isOpen_read(false)
{
}

template<typename WriteType>
vfs::TFile<WriteType>::~TFile()
{
	m_file.clear();
	if(m_isOpen_read)
	{
		this->close();
	}
}

template<typename WriteType>
void vfs::TFile<WriteType>::close()
{
	m_file.clear();
	if( m_file.is_open() )
	{
		m_file.close();
	}
	m_isOpen_read = false;
	IS_FILE_VALID();
}

template<typename WriteType>
vfs::FileAttributes vfs::TFile<WriteType>::getAttributes()
{
	vfs::Path fullpath;
	THROWIFFALSE(this->_getRealPath(fullpath), ERROR_FILE(L""));

	vfs::UInt32 osFileAttributes = 0;
	os::FileAttributes fa;
	THROWIFFALSE( fa.getFileAttributes(fullpath, osFileAttributes), ERROR_FILE(L"Could not read file Attributes") );

	vfs::UInt32 _attribs = vfs::FileAttributes::ATTRIB_INVALID;
	copyAttributes(osFileAttributes, _attribs);

	if(!this->implementsWritable())
	{
		_attribs &= ~vfs::FileAttributes::ATTRIB_NORMAL;
		_attribs |= vfs::FileAttributes::ATTRIB_READONLY;
	}

	return vfs::FileAttributes(_attribs, vfs::FileAttributes::LT_NONE);
}

template<typename WriteType>
bool vfs::TFile<WriteType>::isOpenRead()
{
	return m_isOpen_read;
}

template<typename WriteType>
bool vfs::TFile<WriteType>::_internalOpenRead(vfs::Path const& path)
{
	if( m_isOpen_read )
		return true;
	if( !m_file.good() )
		return false;

	std::ios::openmode Mode;
	Mode = std::ios::in|std::ios::binary;
	// try to open 
	_openFile(m_file, path, Mode);
	m_isOpen_read = m_file.is_open() && m_file.good();
	return m_isOpen_read;
}

template<typename WriteType>
bool vfs::TFile<WriteType>::openRead()
{
	return _internalOpenRead(this->m_filename);
}

template<typename WriteType>
vfs::size_t vfs::TFile<WriteType>::read(vfs::Byte* pData, vfs::size_t bytesToRead)
{
	if(!m_file.eof())
	{
		IS_FILE_VALID();
		THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );

		m_file.read(static_cast<char*>(pData), bytesToRead);
		std::streamsize bytesRead = m_file.gcount();

		// don't treat EOF as error
		THROWIFFALSE( m_file.good() || m_file.eof(), ERROR_FILE(L"read error") );
		return (vfs::size_t)bytesRead;
	}
	return 0;
}

template<typename WriteType>
vfs::size_t vfs::TFile<WriteType>::getReadPosition()
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );
	return (vfs::size_t)m_file.tellg();
}

template<typename WriteType>
void vfs::TFile<WriteType>::setReadPosition(vfs::size_t positionInBytes)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );

	m_file.seekg( (std::streamoff)positionInBytes );

	IS_FILE_VALID();
}

template<typename WriteType>
void vfs::TFile<WriteType>::setReadPosition(vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );

	std::ios::seekdir ioSeekDir;
	TRYCATCH_RETHROW( ioSeekDir = _seekDir(seekDir), ERROR_FILE(L"seek error"));
	m_file.seekg( offsetInBytes, ioSeekDir );

	IS_FILE_VALID();
}

/********************************************************************************/
/********************************************************************************/


vfs::CFile::CFile(vfs::Path const& filename) 
: tBaseClass(filename), m_isOpen_write(false)
{
}

vfs::CFile::~CFile()
{
	m_file.clear();
	if(m_isOpen_read || m_isOpen_write)
	{
		this->close();
	}
}

void vfs::CFile::close()
{
	tBaseClass::close();
	m_isOpen_write = false;
}

bool vfs::CFile::deleteFile()
{
	this->close();
	return os::deleteRealFile(m_filename);
}

bool vfs::CFile::isOpenWrite()
{
	return m_isOpen_write;
}

bool vfs::CFile::_internalOpenWrite(vfs::Path const& path, bool createWhenNotExist, bool truncate)
{
	if( m_isOpen_write )
		return true;
	if( !m_file.good() )
		return false;

	std::ios::openmode Mode;
	Mode = std::ios::in|std::ios::out|std::ios::binary;
	if(truncate)
	{
		Mode |= std::ios::trunc;
	}
	_openFile(m_file, path, Mode);
	if( !(m_isOpen_write = m_file.is_open() && m_file.good()) )
	{
		m_file.clear();
		if(createWhenNotExist)
		{
			// create file 
			Mode = std::ios::out|std::ios::binary;
			if(truncate)
			{
				Mode |= std::ios::trunc;
			}
			_openFile(m_file, path, Mode);
			if( !(m_isOpen_write = m_file.is_open() && m_file.good()) )
			{
				return false;
			}
			m_file.close();
			Mode |= std::ios::in;
			_openFile(m_file, path, Mode);
			if( !(m_isOpen_write = m_file.is_open() && m_file.good()) )
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return m_isOpen_write;
}

bool vfs::CFile::openWrite(bool createWhenNotExist, bool truncate)
{
	return _internalOpenWrite(m_filename, createWhenNotExist, truncate);
}

vfs::size_t vfs::CFile::write(const vfs::Byte* data, vfs::size_t bytesToWrite)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_write, ERROR_FILE(L"file not opened") );

	std::streampos start = m_file.tellp();
	THROWIFFALSE( m_file.write(data, bytesToWrite), ERROR_FILE(L"write error") );
	std::streampos bytesWritten = m_file.tellp() - start;

	IS_FILE_VALID();

	return (vfs::size_t)bytesWritten;
}	

vfs::size_t vfs::CFile::getWritePosition()
{
	IS_FILE_VALID();
	return (vfs::size_t)m_file.tellp();
}

void vfs::CFile::setWritePosition(vfs::size_t positionInBytes)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_write, ERROR_FILE(L"file not opened") );

	m_file.seekp( (std::streamoff)positionInBytes );

	IS_FILE_VALID();
}

void vfs::CFile::setWritePosition(vfs::offset_t offsetInBytes, vfs::IBaseFile::ESeekDir seekDir)
{
	IS_FILE_VALID();
	THROWIFFALSE( m_isOpen_write, ERROR_FILE(L"file not opened") );

	std::ios::seekdir ioSeekDir;
	TRYCATCH_RETHROW( ioSeekDir = _seekDir(seekDir), ERROR_FILE(L"seek error") );
	m_file.seekp(offsetInBytes,ioSeekDir);

	IS_FILE_VALID();
}

//vfs::size_t vfs::CFile::getSize()
template<typename T>
vfs::size_t vfs::TFile<T>::getSize()
{
	IS_FILE_VALID();

	// if file was alredy opened, keep it open, otherwise close it
	bool closeAtExit = !m_isOpen_read;

	THROWIFFALSE( m_isOpen_read || this->openRead(), ERROR_FILE(L"could not open file") )

	// save the current position 
	std::streampos current_position =  m_file.tellg();

	// move to end of the file
	m_file.seekg(0,std::ios::end);
	std::streampos file_size = m_file.tellg();

	// move to old position
	m_file.seekg(current_position,std::ios::beg);
	THROWIFFALSE(current_position == m_file.tellg(), ERROR_FILE(L"could not restore seek position"));

	if(closeAtExit)
	{
		this->close();
	}
	IS_FILE_VALID();
	return (vfs::size_t)file_size;
}

/******************************************************************/
/******************************************************************/

template class vfs::TFile<vfs::IWriteType>;

