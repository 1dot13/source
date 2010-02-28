#include "vfs_dir_file.h"
#include "../Interface/vfs_directory_interface.h"
#include "../vfs_settings.h"
#include "../os_functions.h"

vfs::CReadOnlyDirFile::CReadOnlyDirFile(vfs::Path const& filename, tLocation *directory)
: vfs::CReadOnlyFile(filename), _location(directory)
{
}

vfs::CReadOnlyDirFile::~CReadOnlyDirFile()
{
}

vfs::Path vfs::CReadOnlyDirFile::getPath()
{
	if(_location)
	{
		return _location->getPath() + m_filename;
	}
	else
	{
		return m_filename;
	}
}

bool vfs::CReadOnlyDirFile::_getRealPath(vfs::Path& path)
{
	if(_location)
	{
		path = _location->getRealPath() + m_filename;
		return true;
	}
	return false;
}


vfs::FileAttributes vfs::CReadOnlyDirFile::getAttributes()
{
	vfs::FileAttributes _attribs = vfs::CReadOnlyFile::getAttributes();

	vfs::UInt32 attr = _attribs.getAttrib();
	attr |= vfs::FileAttributes::ATTRIB_READONLY;

	return vfs::FileAttributes(attr, vfs::FileAttributes::LT_READONLY_DIRECTORY);
}

bool vfs::CReadOnlyDirFile::openRead()
{
	vfs::Path filename;
	if(!_getRealPath(filename))
	{
		return false;
	}
	return _internalOpenRead(filename);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

vfs::CDirFile::CDirFile(vfs::Path const& filename, tLocation *directory)
: CFile(filename), _location(directory)
{
}

vfs::CDirFile::~CDirFile()
{
}

vfs::Path vfs::CDirFile::getPath()
{
	if(_location)
	{
		return _location->getPath() + m_filename;
	}
	else
	{
		return m_filename;
	}
}

bool vfs::CDirFile::deleteFile()
{
	this->close();
	vfs::Path fname;
	if(_getRealPath(fname))
	{
		return os::deleteRealFile(fname);
	}
	return false;
}


bool vfs::CDirFile::_getRealPath(vfs::Path& path)
{
	if(_location)
	{
		path = _location->getRealPath() + m_filename;
		return true;
	}
	return false;
}


vfs::FileAttributes vfs::CDirFile::getAttributes()
{
	vfs::FileAttributes _attribs = vfs::CFile::getAttributes();

	return vfs::FileAttributes(_attribs.getAttrib(), vfs::FileAttributes::LT_DIRECTORY);
}

bool vfs::CDirFile::openRead()
{
	vfs::Path filename;
	if(!_getRealPath(filename))
	{
		return false;
	}
	return _internalOpenRead(filename);
}

bool vfs::CDirFile::openWrite(bool createWhenNotExist, bool truncate)
{
	vfs::Path filename;
	if(!_getRealPath(filename))
	{
		return false;
	}
	return _internalOpenWrite(filename, createWhenNotExist, truncate);
}


