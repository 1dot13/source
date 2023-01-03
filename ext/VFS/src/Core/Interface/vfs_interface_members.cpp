/* 
 * bfVFS : vfs/Core/Interface/vfs_interface_members.cpp
 *  - non-generic code from the interface header files
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

#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/Interface/vfs_library_interface.h>

/**********************************************************************
 * vfs::FileAttributes
 */
vfs::FileAttributes::FileAttributes()
: _attribs(ATTRIB_NORMAL), _location(LT_NONE)
{
};

vfs::FileAttributes::FileAttributes(vfs::UInt32 attribs, LocationType location)
: _attribs(attribs), _location(location)
{
};

vfs::UInt32	vfs::FileAttributes::getAttrib() const
{
	return _attribs;
};

vfs::UInt32	vfs::FileAttributes::getLocation() const
{
	return _location;
};

bool vfs::FileAttributes::isAttribSet(vfs::UInt32 attribs) const
{
	return attribs == (attribs & _attribs);
};

bool vfs::FileAttributes::isAttribNotSet(vfs::UInt32 attribs) const
{
	return 0 == (attribs & _attribs);
};

bool vfs::FileAttributes::isLocation(vfs::UInt32 location) const
{
	return location == (location & _location);
};

/**********************************************************************
 * vfs::IBaseFile
 */

vfs::IBaseFile::IBaseFile(vfs::Path const& filename)
: m_filename(filename)
{};

vfs::IBaseFile::~IBaseFile()
{};

vfs::Path const& vfs::IBaseFile::getName()
{
	return m_filename;
};

vfs::Path vfs::IBaseFile::getPath()
{
	return this->getName();
};


bool vfs::IBaseFile::_getRealPath(vfs::Path& path)
{
	return false;
}

/**********************************************************************
 * vfs::ILibrary
 */

vfs::ILibrary::ILibrary(vfs::tReadableFile *libraryFile, vfs::Path const& mountPoint, bool ownFile)
: tBaseClass(mountPoint), m_ownLibFile(ownFile), m_libraryFile(libraryFile)
{
}

vfs::ILibrary::~ILibrary()
{
	if(m_libraryFile && m_ownLibFile)
	{
		m_libraryFile->close();
		delete m_libraryFile;
		m_libraryFile = NULL;
	}
}

vfs::Path const& vfs::ILibrary::getName()
{
	return m_libraryFile->getName();
}

/**********************************************************************/
