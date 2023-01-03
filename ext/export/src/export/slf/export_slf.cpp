
#include <export/slf/export_slf.h>
#include <export/sti/export_sti.h>

#include <init_vfs.h>
#include <progress_bar.h>

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/File/vfs_buffer_file.h>
#include <vfs/Ext/slf/vfs_slf_library.h>
#include <vfs/Ext/7z/vfs_create_7z_library.h>

#include <iostream>

bool ja2xp::convertSLFto7z(vfs::tReadableFile* pInFile, vfs::tWritableFile *pOutFile, bool bConvertSTIs, bool bPngOffsets)
{
	vfs::CSLFLibrary src_lib(pInFile,"");
	if(!src_lib.init())
	{
		std::wcout << L"Could not init SLF Library \"" << pInFile->getPath().c_wcs() << L"\"" << std::endl;
		return false;
	}
	
	vfs::CCreateUncompressed7zLibrary create7zLib;

	vfs::CSLFLibrary::Iterator it = src_lib.begin();
	int count = 0;
	while(!it.end())
	{
		count++;
		it.next();
	}
	ProgressBar pbar(54,count);
	it = src_lib.begin();
	for(; !it.end(); it.next())
	{
		count++;
		vfs::CBufferFile* temp_file = NULL;
		vfs::tReadableFile *file = vfs::tReadableFile::cast(it.value());
		if(bConvertSTIs)
		{
			vfs::Path filename = it.value()->getPath();
			if(vfs::StrCmp::Equal(filename().substr(filename().length()-4,4), L".sti"))
			{
				vfs::Path path = filename().substr(0,filename.length()-4) + L".jpc.7z";
				temp_file = new vfs::CBufferFile(path);
				if(convertSTItoJPC(vfs::tReadableFile::cast(it.value()), vfs::tWritableFile::cast(temp_file), bPngOffsets, false))
				{
					file = vfs::tReadableFile::cast( temp_file );
				}
			}
		}
		if(create7zLib.addFile(file))
		{
			pbar.Next(it.value()->getPath()());
		}
		else
		{
			std::wcout << L"Adding file '" << it.value()->getPath().c_wcs() << L"' failed!"  << std::endl;
		}
		if(temp_file)
		{
			delete temp_file;
		}
	}
	std::wstringstream wss;
	wss << L"Writing file \"" << pOutFile->getPath().c_wcs() << L"\"";
	pbar.Next(wss.str());
	if(!create7zLib.writeLibrary(pOutFile))
	{
		wss.str(L"");
		wss << L"Could not write file \"" << pOutFile->getPath().c_wcs() << L"\"";
		pbar.Next(wss.str());
		return false;
	}
	return true;
}

bool ja2xp::convertSLFto7z(vfs::Path const& sSrc, vfs::Path const& sDst, bool bConvertSTIs, bool bPngOffsets)
{
	vfs::CVirtualFileSystem::Iterator it = getVFS()->begin(sSrc);
	int file_counter = 0;
	for(; !it.end(); it.next())
	{
		file_counter++;
		vfs::String::str_t fname = it.value()->getPath().c_wcs();
		if( !vfs::StrCmp::Equal( fname.substr(fname.length()-4,4), L".slf") )
		{
			std::wcout << L"\"" << fname << L"\" " << L"is not an .slf file" << std::endl;
			continue;
		}
		if(!it.value()->openRead())
		{
			std::wcout << L"Could not open file \"" << sSrc() << L"\""  << std::endl;
			continue;
		}
		vfs::String::str_t new_fname = fname.substr(0,fname.length()-4) + L".jdc.7z";
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
		if(!convertSLFto7z(it.value(), &wfile.file(), bConvertSTIs, bPngOffsets))
		{
			printf("\n");
			continue;
		}
		printf("\n");
	}
	if(file_counter == 0)
	{
		std::wcout << L"Nothing to convert!" << std::endl;
	}
	return true;
}

/**********************************************************************************************/

const wchar_t* ja2xp::CExportSLF::commandString = L"slf";

void ja2xp::CExportSLF::handleCommand(param_list_t const& params)
{
	bool opt_Sti2Png = false, opt_PngOffsets = false;

	param_list_t vfs_params;
	param_list_t::const_iterator cit = params.begin();
	for(; cit != params.end(); ++cit)
	{
		if( vfs::StrCmp::Equal(*cit, L"-sti2png") )
		{
			opt_Sti2Png = true;
			continue;
		}
		else if( vfs::StrCmp::Equal(*cit, L"-png_offsets") )
		{
			opt_PngOffsets = true;
			continue;
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
		convertSLFto7z(vfs.srcPattern(), vfs.dstPattern(), opt_Sti2Png, opt_PngOffsets);
	}
	catch(vfs::Exception& msg)
	{
		std::wcout << msg.getLastEntryString().c_str() << std::endl;
	}
}

void ja2xp::CExportSLF::printHelp()
{
	std::wcout
		<< L" convert SLF archive(s) to uncompressed 7z archive(s)" << std::endl
		<< L" * if \"destination\" is a directory, then the filename's extension will be" << std::endl
		<< L"   replaced with \".jdc.7z\"" << std::endl
		<< std::endl
		<< L" options :" << std::endl
		<< L"   -sti2png     : all sti files in the archive will be converted to" << std::endl
		<< L"                  \".jpc.7z\" files" << std::endl
		<< L"   -png_offsets : if option \"sti2png\" is enabled, offsets of the png" << std::endl
		<< L"                  subimages will be written int the \"appdata.xml\" file" << std::endl;
}
