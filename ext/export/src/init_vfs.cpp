#include <init_vfs.h>

#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Core/vfs_init.h>
#include <vfs/Tools/vfs_property_container.h>

#include <stack>
#include <iostream>

ja2xp::InitVFS::InitVFS()
{
}

ja2xp::InitVFS& ja2xp::InitVFS::instance()
{
	static ja2xp::InitVFS singleton;
	return singleton;
}

void ja2xp::InitVFS::checkParameters(ja2xp::IExporterBase::param_list_t const& params)
{
	ja2xp::IExporterBase::param_list_t::const_iterator cit = params.begin();
	for(; cit != params.end(); ++cit)
	{
		if( vfs::StrCmp::Equal(*cit, L"-lib") )
		{
			this->src_lib = true;
			cit++;
			if(cit != params.end())
			{
				// get library path
				this->src_path = vfs::Path(*cit);
				this->has_src = true;
				
				cit++;
				if(cit != params.end())
				{
					this->src_pattern = vfs::Path(*cit); 
					continue;
				}
			}
			std::wcout << L"error " << std::endl;
		}
		else if( vfs::StrCmp::Equal(*cit, L"-create_dst") )
		{
			this->dst_create = true;
			continue;
		}
		else if(!cit->empty() && cit->at(0) == L'-')
		{
			//ignore unknown options
			continue;
		}

		if(!this->has_src)
		{
			this->src_path = vfs::Path(*cit);
			this->has_src = true;
		}
		else if(this->has_src && !this->has_dst)
		{
			this->dst_path = vfs::Path(*cit);
			this->has_dst = true;
		}
	}
}

static void SplitPath(vfs::Path const& path, vfs::Path& root, vfs::Path& pattern)
{
	vfs::String::str_t sSrc = path();
	if(sSrc.empty())
	{
		return;
	}
	// remove enclosing ""
	if(sSrc.at(0) == L'\"' && sSrc.at(sSrc.length()-1) == L'\"')
	{
		sSrc = sSrc.substr(1,sSrc.length()-2);
	}
	// has * in path
	size_t pos = sSrc.find_first_of(L"*");
	if(pos != std::wstring::npos)
	{
		vfs::Path temp_root, temp_pattern;
		// get directory 
		vfs::Path temp(sSrc.substr(0,pos+1));
		temp.splitLast(temp_root, temp_pattern);
		// correct pattern
		temp_pattern = vfs::Path(temp_pattern.c_wcs() + sSrc.substr(pos+1,sSrc.length()-pos-1));

		root = vfs::Path(temp_root);
		pattern = vfs::Path(temp_pattern);
	}
	else
	{
		path.splitLast(root,pattern);
	}
}


//bool InitVFS(vfs::Path const& sSrcPath, vfs::Path const& sDstPath,
//			 vfs::Path& src_pattern, vfs::Path& dst_pattern,
//			 bool bSourceAsLib, bool bCreateDstDirectory)
bool ja2xp::InitVFS::init()
{
	if( !this->has_src || !this->has_dst )
	{
		return false;
	}
	vfs::Path src_root, dst_root;

	if( this->src_pattern.empty() && !this->src_lib )
	{
		SplitPath(this->src_path, src_root, this->src_pattern);
		if(src_root.empty()) src_root = L".";
	}
	else
	{
		src_root = this->src_path;
	}

	SplitPath(this->dst_path, dst_root, this->dst_pattern);
	if(dst_root.empty()) dst_root = L".";

	vfs::String ext_test;
	if(!this->dst_pattern.extension(ext_test))
	{
		// output pattern has no extension and is probably not a directory name
		dst_root += this->dst_pattern;
		this->dst_pattern = L".";
	}

	if(!this->src_lib && !vfs::OS::checkRealDirectory(src_root))
	{
		std::wcout << L"Source directory \"" << src_root() << L"\" does not exist" << std::endl;
		return false;
	}
	if(!vfs::OS::checkRealDirectory(dst_root))
	{
		if(!this->dst_create)
		{
			std::wcout << L"Destination directory \"" << dst_root() << L"\" does not exist" << std::endl;
			return false;
		}
		std::stack<vfs::Path> dirs;
		vfs::Path left = dst_root, right, temp;
		while(!left.empty() && !vfs::OS::checkRealDirectory(left))
		{
			dirs.push(left);
			left.splitLast(temp,right);
			left = temp;
		}
		while(!dirs.empty())
		{
			if(!vfs::OS::createRealDirectory(dirs.top()))
			{
				std::wcout << L"Could not create directory \"" << dirs.top().c_wcs() << std::endl;
				return false;
			}
			dirs.pop();
		}
	}

	vfs_init::Location src_loc;
	if(this->src_lib)
	{
		src_loc.m_type = L"library";
	}
	else
	{
		src_loc.m_type = L"directory";
	}
	src_loc.m_path = src_root;

	vfs_init::Profile src_prof;
	src_prof.m_name = L"Source";
	src_prof.addLocation(&src_loc);

	vfs_init::Location dst_loc;
	dst_loc.m_type = L"directory";
	dst_loc.m_path = dst_root;

	vfs_init::Profile dst_prof;
	dst_prof.m_name = L"Destination";
	dst_prof.addLocation(&dst_loc);
	dst_prof.m_writable = true;

	vfs_init::VfsConfig conf;
	conf.addProfile(&src_prof);
	conf.addProfile(&dst_prof);

	try
	{
		vfs_init::initVirtualFileSystem(conf);
	}
	catch(vfs::Exception& ex)
	{
		std::wcout << ex.m_CallStack.front().message.c_wcs() << std::endl;
		return false;
	}
	return true;
}

vfs::Path const& ja2xp::InitVFS::srcPattern()
{
	return this->src_pattern;
}
vfs::Path const& ja2xp::InitVFS::dstPattern()
{
	return this->dst_pattern;
}
