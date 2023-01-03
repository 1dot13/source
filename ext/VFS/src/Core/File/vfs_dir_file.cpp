/* 
 * bfVFS : vfs/Core/File/vfs_dir_file.cpp
 *  - read/read-write files for usage in vfs locations (directories)
 *
 * Copyright (C) 2008 - 2010 (BF) john.bf.smith@googlemail.com
 * 
 * This file is part of the bfVFS library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <vfs/Core/File/vfs_dir_file.h>
#include <vfs/Core/Interface/vfs_directory_interface.h>
#include <vfs/Core/vfs_os_functions.h>

#include <vfs/Aspects/vfs_settings.h>

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
		return vfs::OS::deleteRealFile(fname);
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


