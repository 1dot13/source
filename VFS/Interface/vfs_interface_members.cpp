#include "vfs_file_interface.h"
#include "vfs_library_interface.h"

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
