/* 
 * bfVFS : vfs/Ext/7z/vfs_create_7z_library.cpp
 *  - writes uncompressed 7z archive file
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

#ifdef VFS_WITH_7ZIP

#include <cstring>

#include <vfs/Core/vfs_types.h>

#include <vfs/Ext/7z/vfs_create_7z_library.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/vfs_debug.h>

namespace sz
{
extern "C"
{
#include <7zCrc.h>
#include <7z.h>
//#include "Archive/7z/7zIn.h"
}
};

#include <vector>
#include <sstream>

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
namespace szExt
{
	inline ::size_t WRITEBYTE(std::ostream& out, sz::Byte const& value)
	{
		out.write((char*)&value,sizeof(sz::Byte));
		return 1;
	}
	template<typename T>
	inline ::size_t WRITEALL(std::ostream& out, T const& value)
	{
		out.write((char*)&value,sizeof(T));
		return sizeof(T);
	}
	template<typename T>
	inline ::size_t WRITEBUFFER(std::ostream& out, T* value, ::size_t num_elements)
	{
		out.write((char*)value, num_elements*sizeof(T));
		return num_elements*sizeof(T);
	}

	/**
	 *  "compress" numbers by removing heading zero-bytes
	 *  - add additional byte that represents a bit-vector of bytes within a 64-bit/8-byte number
	 *  - if the number is smaller than 128, use the extra byte to store the value
	 */
	template<typename T>
	inline ::size_t WRITE(std::ostream& out, T const& value)
	{
		::size_t count = 0;
		sz::Byte data[8];
		sz::Byte firstByte = 0;
		sz::Byte* b = (sz::Byte*)&value;
		::size_t SIZE = sizeof(T);
		b+= SIZE-1;
		vfs::Int32 i;
		for(i = (vfs::Int32)(SIZE-1); i>=0; --i)
		{
			if( (*b & 0xFF) != 0)
			{
				break;
			}
			b--;
		}
		if(i < 0)
		{
			count += WRITEBYTE(out,0);
			return count;
		}
		if(i == 0)
		{
			if(*b >= 0x80)
			{
				count += WRITEBYTE(out,0x80);
			}
			count += WRITEBYTE(out,*b);
			return count;
		}
		vfs::Int32 num = 0;
		for(;i >= 0; --i)
		{
			firstByte |= 1 << (8 - i - 1);
			data[i] = *b--;
			num++;
		}
		count += WRITEBYTE(out,firstByte);
		count += WRITEBUFFER(out,data,num);
		return count;
	}
}
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

vfs::CCreateUncompressed7zLibrary::CCreateUncompressed7zLibrary()
: m_pLibFile(NULL)
{
	sz::CrcGenerateTable();
}

vfs::CCreateUncompressed7zLibrary::~CCreateUncompressed7zLibrary()
{
	m_pLibFile = NULL;
	
	m_lFileInfo.clear();
	m_mapDirInfo.clear();
}

bool vfs::CCreateUncompressed7zLibrary::addFile(vfs::tReadableFile* pFile)
{
	if(!pFile)
	{
		// at least nothing bad happened
		return true;
	}
	try
	{
		vfs::COpenReadFile infile(pFile);
	}
	catch(std::exception &ex)
	{
		std::wstringstream wss;
		wss << L"Could not open File \"" << pFile->getPath()() << L"\"";
		VFS_RETHROW(wss.str().c_str(), ex);
	}
	SFileInfo fi;
	vfs::Path filename = pFile->getPath();
	fi.name = filename.c_wcs();
	fi.size = pFile->getSize();
	if(m_lFileInfo.empty())
	{
		fi.offset = 0;
	}
	else
	{
		SFileInfo const& fic = m_lFileInfo.back();
		fi.offset = fic.offset + fic.size;
	}

	typedef std::vector<vfs::Byte> tByteVector;
	tByteVector data( (tByteVector::size_type)fi.size );
	VFS_THROW_IFF(fi.size == pFile->read(&data[0], (vfs::size_t)fi.size), L"");
	fi.CRC = sz::CrcCalc(&data[0],(::size_t)fi.size);
	m_ssFileStream.write(&data[0],(std::streamsize)fi.size);

	m_lFileInfo.push_back(fi);

	vfs::Path path,dummy;
	filename.splitLast(path,dummy);
	if(!path.empty())
	{
		tDirInfo::iterator it_find = m_mapDirInfo.find(path.c_wcs());
		if(it_find == m_mapDirInfo.end())
		{
			SFileInfo dir;
			dir.name = path.c_wcs();
			dir.offset = 0;
			dir.size = 0;
			dir.time_creation = 0;
			dir.time_last_access = 0;
			dir.time_write = 0;
			m_mapDirInfo.insert(std::make_pair(dir.name,dir));
		}
	}
	return true;
}

bool vfs::CCreateUncompressed7zLibrary::writeLibrary(vfs::Path const& sLibName)
{
	vfs::COpenWriteFile outfile(sLibName,true);
	return writeLibrary(&outfile.file());
}
bool vfs::CCreateUncompressed7zLibrary::writeLibrary(vfs::tWritableFile* pFile)
{
	if(!pFile)
	{
		return false;
	}
	if(m_lFileInfo.empty())
	{
		return false;
	}
	m_pLibFile = pFile;
	if(!m_pLibFile->isOpenWrite() && !m_pLibFile->openWrite(true,true))
	{
		return false;
	}
	//
	writeNextHeader(m_ssInfoStream);
	//
	std::stringstream ssSigHeader;
	writeSignatureHeader(ssSigHeader);
	//
	m_pLibFile->write(ssSigHeader.str().c_str()		, (vfs::size_t)ssSigHeader.str().length());
	//
	m_pLibFile->write(m_ssFileStream.str().c_str()	, (vfs::size_t)m_ssFileStream.str().length());
	//
	m_pLibFile->write(m_ssInfoStream.str().c_str()	, (vfs::size_t)m_ssInfoStream.str().length());

	return true;
}

/**************************************************************************************/

bool vfs::CCreateUncompressed7zLibrary::writeSignatureHeader(std::ostream& out)
{
	::size_t count=0;

	// #define k7zSignatureSize -> not in namespace sz
	count += szExt::WRITEBUFFER(out, sz::k7zSignature, k7zSignatureSize);

	sz::Byte Major = 0, Minor = 2;
	count += szExt::WRITEALL(out, (sz::Byte)Major );
	count += szExt::WRITEALL(out, (sz::Byte)Minor );

	sz::UInt32 StartHeaderCRC, NextHeaderCRC;
	
	SFileInfo const& fi = m_lFileInfo.back();
	sz::UInt64 NextHeaderOffset = fi.offset + fi.size;
	sz::UInt64 NextHeaderSize = m_ssInfoStream.str().length()*sizeof(char);
	NextHeaderCRC = sz::CrcCalc(m_ssInfoStream.str().c_str(),(::size_t)NextHeaderSize);

	std::stringstream sstemp;
	count += szExt::WRITEALL(sstemp, (sz::UInt64)NextHeaderOffset );
	count += szExt::WRITEALL(sstemp, (sz::UInt64)NextHeaderSize );
	count += szExt::WRITEALL(sstemp, (sz::UInt32)NextHeaderCRC );
	StartHeaderCRC = sz::CrcCalc(sstemp.str().c_str(), sstemp.str().length()*sizeof(char));

	count += szExt::WRITEALL(out, (sz::UInt32)StartHeaderCRC );
	out << sstemp.str();

	return true;
}
bool vfs::CCreateUncompressed7zLibrary::writeNextHeader(std::ostream& out)
{
	szExt::WRITE(out, (sz::Byte)sz::k7zIdHeader);

	// this->WriteArchiveProperties(out);

	// this->WriteAdditionalStreamsInfo(out)

	//
	this->writeMainStreamsInfo(out);

	//
	this->writeFilesInfo(out);

	szExt::WRITE(out, (sz::Byte)sz::k7zIdEnd );

	return true;
}

bool vfs::CCreateUncompressed7zLibrary::writeMainStreamsInfo(std::ostream& out)
{
	szExt::WRITE(out, (sz::Byte)sz::k7zIdMainStreamsInfo );

	this->writePackInfo(out);

	this->writeUnPackInfo(out);

	this->writeSubStreamsInfo(out);

	szExt::WRITE(out, (sz::Byte)sz::k7zIdEnd );

	return true;
}


bool vfs::CCreateUncompressed7zLibrary::writePackInfo(std::ostream& out)
{
	szExt::WRITE(out, (sz::Byte)sz::k7zIdPackInfo );
	szExt::WRITE(out, (sz::UInt64)0 ); // data offset
	szExt::WRITE(out, (sz::UInt32)m_lFileInfo.size() );

	szExt::WRITE(out, (sz::Byte)sz::k7zIdSize );
	std::list<SFileInfo>::iterator it = m_lFileInfo.begin();
	for(;it != m_lFileInfo.end(); ++it)
	{
		szExt::WRITE(out, (sz::UInt64)it->size );
	}

	szExt::WRITE(out, (sz::Byte)sz::k7zIdEnd );
	return true;
}
bool vfs::CCreateUncompressed7zLibrary::writeUnPackInfo(std::ostream& out)
{
	szExt::WRITE(out, (sz::Byte)sz::k7zIdUnpackInfo );

	szExt::WRITE(out, (sz::Byte)sz::k7zIdFolder );
	szExt::WRITE(out, (sz::UInt64)m_lFileInfo.size() );
	szExt::WRITE(out, (sz::Byte)0 ); // External
	
	std::list<SFileInfo>::iterator fit = m_lFileInfo.begin();
	for(;fit != m_lFileInfo.end(); ++fit)
	{
		this->writeFolder(out);
	}

	szExt::WRITE(out, (sz::Byte)sz::k7zIdCodersUnpackSize );
	fit = m_lFileInfo.begin();
	for(;fit != m_lFileInfo.end(); ++fit)
	{
		szExt::WRITE(out, (sz::UInt64)fit->size );
	}

	szExt::WRITE(out, (sz::Byte)sz::k7zIdEnd );

	return true;
}
bool vfs::CCreateUncompressed7zLibrary::writeSubStreamsInfo(std::ostream& out)
{
	szExt::WRITE(out, (sz::Byte)sz::k7zIdSubStreamsInfo );
	szExt::WRITE(out, (sz::Byte)sz::k7zIdCRC );
	
	szExt::WRITE(out, (sz::Byte)1 ); // early out - all CRCs defined
	std::list<SFileInfo>::iterator fit = m_lFileInfo.begin();
	for(;fit != m_lFileInfo.end(); ++fit )
	{
		szExt::WRITEALL(out, (sz::UInt32)fit->CRC);
	}
	
	szExt::WRITE(out, (sz::Byte)sz::k7zIdEnd );
	return true;
}

bool vfs::CCreateUncompressed7zLibrary::writeFolder(std::ostream& out)
{
	szExt::WRITE(out, (sz::UInt32)1 ); // NumCoders
	
	szExt::WRITE(out, (sz::Byte)1 ); // MainByte

	szExt::WRITE(out, (sz::Byte)0 ); // Methods

	return true;
}
bool vfs::CCreateUncompressed7zLibrary::writeFilesInfo(std::ostream& out)
{
	szExt::WRITE(out, (sz::Byte)sz::k7zIdFilesInfo );

	vfs::UInt64 num_files = (m_lFileInfo.size()+m_mapDirInfo.size());
	szExt::WRITE(out, (sz::UInt64)num_files );

	// empty stream -> pack info in bit-vector
	szExt::WRITE(out, (sz::Byte)sz::k7zIdEmptyStream );
	sz::UInt64 num_empty64 = num_files/8 + (num_files%8 == 0 ? 0 : 1);
	::size_t num_empty = (::size_t)num_empty64;
	VFS_THROW_IFF(num_empty == num_empty64, L"WTF");

	sz::Byte *empty_vector = new sz::Byte[num_empty];
	memset(empty_vector,0,(::size_t)num_empty);
	for(::size_t e=m_lFileInfo.size(); e < num_files; ++e)
	{
		::size_t index = e / 8;
		empty_vector[index] |= 1 << (7 - e%8);
	}
	szExt::WRITE(out, (sz::UInt64)num_empty ); // size
	szExt::WRITEBUFFER(out, empty_vector, (::size_t)num_empty);
	delete[] empty_vector;

	// names
	szExt::WRITE(out, (sz::Byte)sz::k7zIdName );
	::size_t count = 0;
	std::stringstream name_stream;
	count += szExt::WRITE(name_stream, (sz::Byte)0 ); // switch
	std::list<SFileInfo>::iterator fit = m_lFileInfo.begin();
	for(;fit != m_lFileInfo.end(); ++fit)
	{
		count += (::size_t)this->writeFileName(name_stream, fit->name);
	}
	std::map<vfs::String::str_t,SFileInfo>::iterator dit = m_mapDirInfo.begin();
	for(;dit != m_mapDirInfo.end(); ++dit)
	{
		count += (::size_t)this->writeFileName(name_stream, dit->second.name);
	}
	szExt::WRITE(out, (sz::UInt64)count ); // size
	szExt::WRITEBUFFER(out, name_stream.str().c_str(), name_stream.str().length() );

	//szExt::WRITE(out, (sz::Byte)sz::k7zIdEmptyFile );
	//szExt::WRITE(out, (sz::Byte)sz::k7zIdCTime ); // create
	//szExt::WRITE(out, (sz::Byte)sz::k7zIdATime ); // last access
	//szExt::WRITE(out, (sz::Byte)sz::k7zIdMTime ); // write

	//szExt::WRITE(out, (sz::Byte)sz::k7zIdWinAttributes );

	szExt::WRITE(out, (sz::Byte)sz::k7zIdEnd );

	return true;
}

vfs::size_t vfs::CCreateUncompressed7zLibrary::writeFileName(std::ostream& out, vfs::String const& filename)
{
	::size_t count = 0;
	VFS_THROW_IFF(filename.length(), L"zero length name");
	count += szExt::WRITEBUFFER(out, filename.c_str(), filename.length());
	count += szExt::WRITE(out, (sz::Byte)0);
	count += szExt::WRITE(out, (sz::Byte)0);
	return (vfs::size_t)count;
}

#endif // VFS_WITH_7ZIP
