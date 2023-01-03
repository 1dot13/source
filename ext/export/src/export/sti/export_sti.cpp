
#include "export_sti.h"

#include "Image.h"
#include <init_vfs.h>

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>

#include <iostream>

bool ja2xp::convertSTItoJPC(vfs::tReadableFile *pFile, vfs::tWritableFile *pOutFile, bool bOffsets, bool rgba)
{
	if(!pFile)
	{
		std::wcout << L"file to read from is NULL" << std::endl;
	}
	if(!pOutFile)
	{
		std::wcout << L"file to write to is NULL" << std::endl;
	}
	CImage image(pFile);
	if(image.LoadData())
	{
		return image.WriteAsPNGs(pOutFile, bOffsets, rgba);
	}
	std::wcout << L"could not load data from STI file" << std::endl;
	return false;
}

bool ja2xp::convertSTItoJPC(vfs::tReadableFile *pFile, vfs::Path const base_out_path, bool bOffsets, bool rgba)
{
	if(!pFile)
	{
		std::wcout << L"file to read from is NULL" << std::endl;
	}
	CImage image(pFile);
	if(image.LoadData())
	{
		return image.WriteAsPNGs(base_out_path, bOffsets, rgba);
	}
	std::wcout << L"could not load data from STI file" << std::endl;
	return false;
}

bool ja2xp::convertSTItoPNG(vfs::tReadableFile *pFile, vfs::tWritableFile *pOutFile, bool rgba)
{
	if(!pFile)
	{
		std::wcout << L"file to read from is NULL" << std::endl;
	}
	CImage image(pFile);
	if(image.LoadData())
	{
		return image.WriteFirstPNG(pOutFile, rgba);
	}
	std::wcout << L"could not load data from STI file" << std::endl;
	return false;
}

bool ja2xp::convertSTItoJPC(vfs::Path const& sSrc, vfs::Path const& sDst, bool bPacked, bool bOffsets, bool bOnePng, bool rgba)
{
	vfs::CVirtualFileSystem::Iterator it = getVFS()->begin(sSrc);
	int file_counter = 0;
	for(; !it.end(); it.next())
	{
		file_counter++;
		vfs::String::str_t fname = it.value()->getPath().c_wcs();
		if( !vfs::StrCmp::Equal( fname.substr(fname.length()-4,4), L".sti") )
		{
			std::wcout << L"\"" << fname << L"\" is not an .sti file" << std::endl;
			continue;
		}
		if(!it.value()->openRead())
		{
			std::wcout << L"Could not open file \"" << sSrc() << L"\""  << std::endl;
			continue;
		}
		vfs::String::str_t fname_base = fname.substr(0,fname.length()-4);
		 
		std::wcout << L"Converting file \"" << fname << L"\"" << std::endl;

		if(bOnePng)
		{
			vfs::Path out_name;
			if(vfs::StrCmp::Equal(sDst(), L"."))
			{
				out_name = vfs::Path(fname_base + L".png");
			}
			else
			{
				out_name = sDst;
			}
			vfs::COpenWriteFile wfile(out_name,true,true);

			if(	!convertSTItoPNG( it.value(), &(wfile.file()), rgba ) )
			{
				std::wcout << L"error : could not convert sti image" << std::endl;
				continue;
			}
		}
		else if(bPacked)
		{
			vfs::Path out_name;
			if(vfs::StrCmp::Equal(sDst(), L"."))
			{
				out_name = vfs::Path(fname_base + L".jpc.7z");
			}
			else
			{
				out_name = sDst;
			}
			vfs::COpenWriteFile wfile(out_name,true,true);

			if(	!convertSTItoJPC( it.value(), &(wfile.file()), bOffsets, rgba ) )
			{
				std::wcout << L"error : could not convert sti image" << std::endl;
				continue;
			}
		}
		else
		{
			if(	!convertSTItoJPC( it.value(), vfs::Path(fname_base), bOffsets, rgba ) )
			{
				std::wcout << L"error : could not convert sti image" << std::endl;
				continue;
			}
		}
	}
	if(file_counter == 0)
	{
		std::wcout << L"Nothing to convert!" << std::endl;
	}
	return true;
}

/**********************************************************************************************/

const wchar_t* ja2xp::CExportSTI::commandString = L"sti";

void ja2xp::CExportSTI::handleCommand(CExportSTI::param_list_t const& params)
{
	bool opt_Packed = true, 
		 opt_ExportOffsets = false,
		 opt_OnePng = false,
		 opt_Rgba = false;

	param_list_t vfs_params;
	param_list_t::const_iterator cit = params.begin();
	for(; cit != params.end(); ++cit)
	{
		if( vfs::StrCmp::Equal(*cit, L"-unpack") )
		{
			opt_Packed = false;
		}
		else if( vfs::StrCmp::Equal(*cit, L"-offsets") )
		{
			opt_ExportOffsets = true;
		}
		else if( vfs::StrCmp::Equal(*cit, L"-onepng") )
		{
			opt_OnePng = true;
		}
		else if( vfs::StrCmp::Equal(*cit, L"-rgba") )
		{
			opt_Rgba = true;
		}
		else
		{
			vfs_params.push_back(*cit);
		}
	}

	ja2xp::InitVFS& vfs = ja2xp::InitVFS::instance();
	vfs.checkParameters(vfs_params);
	if(!vfs.init())
	{
		return;
	}
	try
	{
		convertSTItoJPC(vfs.srcPattern(), vfs.dstPattern(), opt_Packed, opt_ExportOffsets, opt_OnePng, opt_Rgba);
	}
	catch(vfs::Exception& msg)
	{
		std::wcout << msg.getLastEntryString().c_wcs() << std::endl;
	}
}

void ja2xp::CExportSTI::printHelp()
{
	std::wcout
		<< L" convert STI image file(s) to a series of png images (as 7z archive)" << std::endl
		<< L" * if \"destination\" is a directory, then the filename's extension will be" << std::endl
		<< L"   replaced with \".jpc.7z\"" << std::endl
		<< std::endl
		<< L" options :" << std::endl
		<< L"   -unpack   : if set, no 7z archive will be created. Instead," << std::endl
		<< L"               for an image 'path/image.sti',subimages will be" << std::endl
		<< L"               written into the directory \"path/image/.\"" << std::endl
		<< L"   -offsets  : offsets of all subimages will we written in the" << std::endl
		<< L"               \"appdata.xml\" file" << std::endl
		<< L"   -onepng   : creates one png image instead of a jpc.7z archive." << std::endl
		<< L"               If there are multiple subimages, only the first one" << std::endl
		<< L"               will be converted." << std::endl;
}

