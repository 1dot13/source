
#include <export/jsd/export_jsd.h>

#include <init_vfs.h>
#include <progress_bar.h>

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/File/vfs_buffer_file.h>
#include <vfs/Ext/slf/vfs_slf_library.h>
#include <vfs/Ext/7z/vfs_create_7z_library.h>

#include <iostream>

#include <export/jsd/structure.h>

bool ja2xp::convertJSDtoXML(vfs::Path const& sSrc, vfs::Path const& sDst)
{
	vfs::CVirtualFileSystem::Iterator it = getVFS()->begin(sSrc);
	int file_counter = 0;
	for(; !it.end(); it.next())
	{
		file_counter++;
		vfs::String::str_t fname = it.value()->getPath().c_wcs();
		if( !vfs::StrCmp::Equal( fname.substr(fname.length()-4,4), L".jsd") )
		{
			std::wcout << L"\"" << fname << L"\" " << L"is not an .jsd file" << std::endl;
			continue;
		}
		if(!it.value()->openRead())
		{
			std::wcout << L"Could not open file \"" << sSrc() << L"\""  << std::endl;
			continue;
		}
		vfs::String::str_t new_fname = fname.substr(0,fname.length()-4) + L".jsd.xml";
		vfs::Path out_name;
		if(vfs::StrCmp::Equal(sDst(), L"."))
		{
			out_name = vfs::Path(new_fname);
		}
		else
		{
			out_name = sDst;
		}
		 
		std::wcout << L"Converting file \"" << it.value()->getPath().c_wcs() << L"\"" << std::endl;

		vfs::COpenWriteFile wfile(out_name,true,true);
		if(!ConvertStructure(it.value(), &wfile.file()))
		{
			printf("Error converting file : %s", vfs::String::as_utf8(it.value()->getPath()()));
			continue;
		}
	}
	if(file_counter == 0)
	{
		std::wcout << L"Nothing to convert!" << std::endl;
	}
	return true;
}

/**********************************************************************************************/

const wchar_t* ja2xp::CExportJSD::commandString = L"jsd";

void ja2xp::CExportJSD::handleCommand(param_list_t const& params)
{
	//param_list_t::const_iterator cit = params.begin();
	//for(; cit != params.end(); ++cit)
	//{
	//}

	ja2xp::InitVFS& vfs = ja2xp::InitVFS::instance();
	vfs.checkParameters(params);
	if(!vfs.init())
	{
		return;
	}
	try
	{
		convertJSDtoXML(vfs.srcPattern(), vfs.dstPattern());
	}
	catch(vfs::Exception& msg)
	{
		std::wcout << msg.getLastEntryString().c_str() << std::endl;
	}
}

void ja2xp::CExportJSD::printHelp()
{
	std::wcout
		<< L" convert JSD to XML file(s)" << std::endl
		<< L" * if \"destination\" is a directory, then the filename's extension will be" << std::endl
		<< L"   replaced with \".jsd.xml\"" << std::endl
		<< std::endl;
}
