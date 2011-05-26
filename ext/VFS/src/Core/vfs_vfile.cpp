/* 
 * bfVFS : vfs/Core/vfs_vfile.cpp
 *  - Virtual File, handles access to files from different VFS profiles
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

#include <vfs/Core/vfs_vfile.h>
#include <vfs/Core/vfs_vloc.h>
#include <vfs/Core/vfs.h>

// static member
vfs::ObjBlockAllocator<vfs::CVirtualFile>* vfs::CVirtualFile::_vfile_pool = NULL;

vfs::CVirtualFile* vfs::CVirtualFile::create(vfs::Path const& sFilePath, vfs::CProfileStack& rPStack)
{
	unsigned int ID=0;
#ifdef VFILE_BLOCK_CREATE
	if(!_vfile_pool)
	{
		_vfile_pool = new ObjBlockAllocator<vfs::CVirtualFile>();
		vfs::ObjectAllocator::registerAllocator(_vfile_pool);
	}
	CVirtualFile* file = _vfile_pool->New(&ID);
#else
	CVirtualFile* file = new CVirtualFile();
#endif
	file->_path = sFilePath;
	file->_pstack = &rPStack;
	file->_myID = ID;
	return file;
}

void vfs::CVirtualFile::destroy()
{
#ifndef VFILE_BLOCK_CREATE
	delete this;
#endif
}


vfs::CVirtualFile::CVirtualFile()
: _path(L""), _top_pname(L"_INVALID_"), _top_file(NULL), _pstack(NULL), _myID(vfs::UInt32(-1))
{
};

vfs::CVirtualFile::~CVirtualFile()
{
}

vfs::Path const& vfs::CVirtualFile::path()
{
	return _path;
}

void vfs::CVirtualFile::add(vfs::IBaseFile *pFile, vfs::String sProfileName, bool bReplace)
{
	if(pFile)
	{
		// if there is no file then just set it
		// if bReplace is set to true then override all other settings and just set the file
		if(!_top_file || bReplace)
		{
			_top_file = pFile;
			_top_pname = sProfileName;
			return;
		}
		// file already set, but new file is? the same object
		if(pFile == _top_file)
		{
			VFS_THROW_IFF( StrCmp::Equal(sProfileName,_top_pname), L"same file, different profile name");
		}
		// OK, not the same file, but these two different files are supposed to have the same filename
		VFS_THROW_IFF( _top_file->getName() == pFile->getName(), L"different filenames");
		// set new file only when its profile is on top of the current file's profile
		bool bFoundOld = false, bFoundNew = false;
		vfs::CProfileStack::Iterator it = _pstack->begin();
		for(; !it.end(); it.next())
		{
			if(_top_pname == it.value()->cName)
			{
				bFoundOld = true;
				break;
			}
			else if(sProfileName == it.value()->cName)
			{
				bFoundNew = true;
				break;
			}
		}
		if(bFoundNew && !bFoundOld)
		{
			_top_file = pFile;
			_top_pname = sProfileName;
		}
	}
}

/**
 * @returns : returns true if pFile is not top file or top file could be replaced with another file
 *            returns false if there is no more files with given name. in this case object should be destroyed
 */
bool vfs::CVirtualFile::remove(vfs::IBaseFile *pFile)
{
	if(_top_file == pFile)
	{
		if(_path == pFile->getPath())
		{
			// need to replace '_top_file'
			vfs::CProfileStack::Iterator prof_it = _pstack->begin();
			for(; !prof_it.end(); prof_it.next())
			{
				CVirtualProfile *pProf = prof_it.value();
				if(pProf)
				{
					vfs::IBaseFile *file = pProf->getFile(_path);
					if(file && (file != pFile))
					{
						_top_file = file;
						_top_pname = pProf->cName;
						return true;
					}
				}
			}
			// no more files
			_top_file = NULL;
			_top_pname = "";
			return false;
		}
		else
		{
			VFS_THROW(L"Same file object but different file paths? WTH?");
		}
	}
	return true;
}

vfs::IBaseFile* vfs::CVirtualFile::file(ESearchFile eSearch)
{
	if(eSearch == SF_TOP)
	{
		return _top_file;
	}
	else if(eSearch == SF_FIRST_WRITABLE)
	{
		CVirtualProfile *pVProf = _pstack->getWriteProfile();
		if(pVProf)
		{
			return pVProf->getFile(_path);
		}
	}
	else if(eSearch == SF_STOP_ON_WRITABLE_PROFILE)
	{
		vfs::CProfileStack::Iterator prof_it = _pstack->begin();
		for(; !prof_it.end(); prof_it.next())
		{
			CVirtualProfile *pProf = prof_it.value();
			if(pProf)
			{
				if(pProf->cWritable)
				{
					return pProf->getFile(_path);
				}
				else
				{
					vfs::IBaseFile *pFile = pProf->getFile(_path);
					if(pFile)
					{
						return pFile;
					}
				}
			}
		}
	}
	return NULL;
}

vfs::IBaseFile* vfs::CVirtualFile::file(vfs::String const& sProfileName)
{
	if(sProfileName == _top_pname)
	{
		return vfs::tReadableFile::cast(_top_file);
	}
	else
	{
		CVirtualProfile* pProf = _pstack->getProfile(sProfileName);
		if(pProf)
		{
			CVirtualProfile::Iterator loc_it = pProf->begin();
			for(; !loc_it.end(); loc_it.next())
			{
				if(loc_it.value() && loc_it.value()->fileExists(_path))
				{
					if(loc_it.value())
					{
						return loc_it.value()->getFile(_path);
					}
				}
			}
		}
	}
	return NULL;
}

