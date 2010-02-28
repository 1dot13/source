#include "vfs_lib_file.h"
#include "../vfs.h"


#define ERROR_FILE(msg)		BuildString().add(L"[").add(this->getPath()).add(L"] - ").add(msg).get()

ObjBlockAllocator<vfs::CLibFile>* vfs::CLibFile::_lfile_pool = NULL;

vfs::CLibFile* vfs::CLibFile::create(vfs::Path const& filename, 
									 tLocation *location, 
									 ILibrary *library,
									 ObjBlockAllocator<vfs::CLibFile>* allocator)
{
#if 0
	vfs::CLibFile* pFile = new vfs::CLibFile();
#else
	vfs::CLibFile* pFile;
	if(allocator)
	{
		pFile = allocator->New();
	}
	else
	{
		if(!_lfile_pool)
		{
			_lfile_pool = new ObjBlockAllocator<vfs::CLibFile>();
			CFileAllocator::registerAllocator(_lfile_pool);
		}
		pFile = _lfile_pool->New();
	}
#endif
	pFile->m_filename = filename;
	pFile->m_location = location;
	pFile->m_library = library;
	return pFile;
}

vfs::CLibFile::CLibFile()
: tBaseClass(L""), 
	m_isOpen_read(false),
	m_library(NULL),
	m_location(NULL)
{
};

vfs::CLibFile::~CLibFile()
{
}

void vfs::CLibFile::close()
{
	if(m_isOpen_read)
	{
		m_library->close(this);
		m_isOpen_read = false;
	}
}

vfs::FileAttributes vfs::CLibFile::getAttributes()
{
	return vfs::FileAttributes(vfs::FileAttributes::ATTRIB_NORMAL | vfs::FileAttributes::ATTRIB_READONLY,
		vfs::FileAttributes::LT_LIBRARY);
}

vfs::Path vfs::CLibFile::getPath()
{
	if(m_location)
	{
		return m_location->getPath() + m_filename;
	}
	else
	{
		return m_filename;
	}
}
bool vfs::CLibFile::isOpenRead()
{
	return m_isOpen_read;
}

bool vfs::CLibFile::openRead()
{
	if(!m_isOpen_read)
	{
		TRYCATCH_RETHROW(m_isOpen_read = m_library->openRead(this), ERROR_FILE(L"read open error"));
	}
	return m_isOpen_read;
}

vfs::size_t vfs::CLibFile::read(vfs::Byte* data, vfs::size_t bytesToRead)
{
	THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );
	try
	{
		return m_library->read(this, data, bytesToRead);
	}
	catch(CBasicException& ex)
	{
		RETHROWEXCEPTION(ERROR_FILE(L"read error"), &ex);
	}
}	

vfs::size_t vfs::CLibFile::getReadPosition()
{
	THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );
	try
	{
		return m_library->getReadPosition(this);
	}
	catch(CBasicException& ex)
	{
		RETHROWEXCEPTION(ERROR_FILE(L"library error"), &ex);
	}
}


void vfs::CLibFile::setReadPosition(vfs::size_t uiPositionInBytes)
{
	THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );
	TRYCATCH_RETHROW(m_library->setReadPosition(this,uiPositionInBytes), ERROR_FILE(L"library error") );
}

void vfs::CLibFile::setReadPosition(vfs::offset_t offsetInBytes, IBaseFile::ESeekDir seekDir)
{
	THROWIFFALSE( m_isOpen_read, ERROR_FILE(L"file not opened") );
	TRYCATCH_RETHROW(m_library->setReadPosition(this, offsetInBytes, seekDir), ERROR_FILE(L"library error"));
}

vfs::size_t vfs::CLibFile::getSize()
{
	THROWIFFALSE( m_isOpen_read || this->openRead(), ERROR_FILE(L"could not open file") );
	try
	{
		return m_library->getSize(this);
	}
	catch(CBasicException& ex)
	{
		RETHROWEXCEPTION(ERROR_FILE(L"library error"), &ex);
	}
}
