#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_init.h>
#include <vfs/Core/vfs_profile.h>
#include <vfs/Core/Interface/vfs_library_interface.h>

#include <iostream>
#include <list>

static std::string utf8(vfs::Path const& path)
{
	return vfs::String::as_utf8(path.c_wcs());
}

static std::string source(vfs::IBaseLocation& location, vfs::IBaseFile& file, vfs::Path const& path)
{
	vfs::Path real;
	if(file._getRealPath(real))
	{
		return utf8(real);
	}
	if(auto library = dynamic_cast<vfs::ILibrary*>(&location))
	{
		return utf8(library->getName()) + ":" + utf8(path);
	}
	return "?";
}

int wmain(int argc, wchar_t** argv)
{
	if(argc < 2)
	{
		std::cerr << "usage: vfstree <vfs_config.ini> [<vfs_config.ini> ...]\n"
			"run from the game directory: the configs' paths are relative to it\n";
		return 2;
	}
	std::list<vfs::Path> inis(argv + 1, argv + argc);
	try
	{
		if(!vfs_init::initVirtualFileSystem(inis))
		{
			std::cerr << "initializing the VFS failed\n";
			return 1;
		}
	}
	catch(vfs::Exception& ex)
	{
		std::cerr << vfs::String::as_utf8(ex.m_CallStack.front().message) << "\n";
		return 1;
	}

	vfs::CVirtualFileSystem* fs = vfs::CVirtualFileSystem::getVFS();
	for(vfs::CVirtualFileSystem::Iterator it = fs->begin(); !it.end(); it.next())
	{
		vfs::IBaseFile* file = it.value();
		vfs::Path path = file->getPath();
		vfs::CVirtualProfile* owner = nullptr;
		vfs::IBaseLocation* location = nullptr;
		for(vfs::CProfileStack::Iterator prof = fs->getProfileStack()->begin(); !owner && !prof.end(); prof.next())
		{
			for(vfs::CVirtualProfile::Iterator loc = prof.value()->begin(); !loc.end(); loc.next())
			{
				if(loc.value()->getFile(path) == file)
				{
					owner = prof.value();
					location = loc.value();
					break;
				}
			}
		}
		std::cout << utf8(path) << "\t"
			<< (owner ? vfs::String::as_utf8(owner->cName) + "\t" + source(*location, *file, path) : "?\t?") << "\n";
	}
	return 0;
}
