#include "vfs_vfile.h"
#include "vfs_vloc.h"
#include "vfs.h"

// static member
ObjBlockAllocator<vfs::CVirtualFile>* vfs::CVirtualFile::_vfile_pool = NULL;

vfs::CVirtualFile* vfs::CVirtualFile::Create(vfs::Path const& sFilePath, vfs::CProfileStack& rPStack)
{
	unsigned int ID=0;
#if 0
	CVirtualFile* file = new CVirtualFile();
#else
	if(!_vfile_pool)
	{
		_vfile_pool = new ObjBlockAllocator<vfs::CVirtualFile>();
		CFileAllocator::RegisterAllocator(_vfile_pool);
	}
	CVirtualFile* file = _vfile_pool->New(&ID);
#endif
	file->_path = sFilePath;
	file->_pstack = &rPStack;
	file->_myID = ID;
	return file;
}

//vfs::CVirtualFile::CVirtualFile(vfs::Path const& sFilePath, CProfileStack& rPStack)
//: _path(sFilePath), _top_pname("_INVALID_"), _top_file(NULL), _rstack(rPStack)
//{
//};
vfs::CVirtualFile::CVirtualFile()
: _path(""), _top_pname("_INVALID_"), _top_file(NULL), _pstack(NULL)
, _myID(-1)
{
};

vfs::CVirtualFile::~CVirtualFile()
{
	int i = 0;
}

vfs::Path const& vfs::CVirtualFile::Path()
{
	return _path;
}

void vfs::CVirtualFile::Add(vfs::IBaseFile *pFile, utf8string sProfileName, bool bReplace)
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
		// file already set, but new file is? exacly the same file
		if(pFile == _top_file)
		{
			THROWIFFALSE( StrCmp::Equal(sProfileName,_top_pname), L"same file, different profile name");
		}
		// OK, not the same file, but these two different files are supposed to have the asme filename
		THROWIFFALSE( _top_file->GetFileName() == pFile->GetFileName(), L"different filenames");
		// set new file only when its profile is on top of the current file's profile
		bool bFoundOld = false, bFoundNew = false;
		vfs::CProfileStack::Iterator it = _pstack->begin();
		for(; !it.end(); it.next())
		{
			if(_top_pname == it.value()->Name)
			{
				bFoundOld = true;
				break;
			}
			else if(sProfileName == it.value()->Name)
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
bool vfs::CVirtualFile::Remove(vfs::IBaseFile *pFile)
{
	if(_top_file == pFile)
	{
		if(_path == pFile->GetFullPath())
		{
			// need to replace '_top_file'
			vfs::CProfileStack::Iterator prof_it = _pstack->begin();
			for(; !prof_it.end(); prof_it.next())
			{
				CVirtualProfile *pProf = prof_it.value();
				if(pProf)
				{
					vfs::IBaseFile *file = pProf->GetFile(_path);
					if(file && (file != pFile))
					{
						_top_file = file;
						_top_pname = pProf->Name;
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
			THROWEXCEPTION(L"Same file object but different file paths? WTH?");
		}
	}
	return true;
}

vfs::IBaseFile* vfs::CVirtualFile::File(ESearchFile eSearch)
{
	if(eSearch == SF_TOP)
	{
		return _top_file;
	}
	else if(eSearch == SF_FIRST_WRITEABLE)
	{
		CVirtualProfile *pVProf = _pstack->GetWriteProfile();
		if(pVProf)
		{
			return pVProf->GetFile(_path);
		}
	}
	else if(eSearch == SF_STOP_ON_WRITEABLE_PROFILE)
	{
		vfs::CProfileStack::Iterator prof_it = _pstack->begin();
		for(; !prof_it.end(); prof_it.next())
		{
			CVirtualProfile *pProf = prof_it.value();
			if(pProf)
			{
				if(pProf->Writeable)
				{
					return pProf->GetFile(_path);
				}
				else
				{
					vfs::IBaseFile *pFile = pProf->GetFile(_path);
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

vfs::IBaseFile* vfs::CVirtualFile::File(utf8string const& sProfileName)
{
	if(sProfileName == _top_pname)
	{
		return vfs::tReadableFile::Cast(_top_file);
	}
	else
	{
		CVirtualProfile* pProf = _pstack->GetProfile(sProfileName);
		if(pProf)
		{
			CVirtualProfile::Iterator loc_it = pProf->begin();
			for(; !loc_it.end(); loc_it.next())
			{
				if(loc_it.value() && loc_it.value()->FileExists(_path))
				{
					if(loc_it.value())
					{
						return loc_it.value()->GetFile(_path);
					}
				}
			}
		}
	}
	return NULL;
}

