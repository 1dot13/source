/* 
 * bfVFS : vfs/Tools/vfs_parser_tools.cpp
 *  - read file line-wise,
 *  - split string into tokens,
 *  - simple pattern matching
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

#include <vfs/Core/vfs.h>
#include <vfs/Core/File/vfs_file.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Tools/vfs_parser_tools.h>
#include <vfs/Tools/vfs_tools.h>

#include <cstring>

/*************************************************************************************/
/*************************************************************************************/

vfs::CReadLine::CReadLine(vfs::tReadableFile& rFile)
: _file(rFile), _buffer_pos(0), _eof(false)
{
	memset(_buffer,0,sizeof(_buffer));

	vfs::COpenReadFile rfile(&_file);
	_bytes_left = rfile->getSize();
	fillBuffer();
	vfs::UByte utf8bom[3] = {0xef,0xbb,0xbf};
	if(memcmp(utf8bom, &_buffer[0],3) == 0)
	{
		_buffer_pos += 3;
	}
	rfile.release();
};

vfs::CReadLine::~CReadLine()
{
	if(_file.isOpenRead())
	{
		_file.close();
	}
}

bool vfs::CReadLine::fillBuffer()
{
	if(_eof)
	{
		return false;
	}
	vfs::size_t bytesRead = BUFFER_SIZE < _bytes_left ? BUFFER_SIZE : _bytes_left;
	try
	{
		vfs::COpenReadFile rfile(&_file);

		// fill the buffer from the start, BUFFER_SIZE charactes at max (_buffer has BUFFER_SIZE+1 elements)
		VFS_THROW_IFF(bytesRead == _file.read(&_buffer[0], bytesRead), L"");
		rfile.release();
	}
	catch(std::exception& ex)
	{
		VFS_RETHROW(L"", ex);
	}

	_bytes_left -= bytesRead;
	_eof = (_bytes_left == 0);


	// bite-wise read files usually terminate a line with \n (or \r\n on WIN32)
	// line-wise read files just returns 0-terminated string

	// always terminate the string with 0
	_buffer[bytesRead] = 0;

	_buffer_pos = 0;
	_buffer_last = bytesRead;

	return true;
}

bool vfs::CReadLine::fromBuffer(std::string& line)
{
	bool done = false;
	while(!done)
	{
		if(_buffer_pos < _buffer_last)
		{
			// start where we left last time
			vfs::Byte *temp = &_buffer[_buffer_pos];
			vfs::size_t start_pos = _buffer_pos;
			// go until we hit 0. since our buffer is always 0 terminated, the second test should be redundant.
			while(*temp && (_buffer_pos < _buffer_last))
			{
				// stop when we find a line terminator
				if(*temp == '\n' || *temp == '\r' /* || *temp == '\0' */)
				{
					break;
				}
				temp++;
				_buffer_pos++;
			}
			// need to append substring, as we might have refilles the buffer (because there was no \n or \r\n terminator)
			line.append( (char*)&_buffer[start_pos], _buffer_pos - start_pos );

			// if we reach the (real) end of the buffer (that always terminate with 0), this means
			// that there was no line terminator and that we have to refill the buffer.
			if( _buffer_pos < BUFFER_SIZE && (*temp == '\n' || *temp == '\r' || *temp == 0) )
			{
				// found the line terminator
				if(*temp == '\r')
				{
					// the \r is most probably followed by \n. 'swallow' both characters
					*temp++;
					_buffer_pos++;
					if( (_buffer_pos < BUFFER_SIZE) && (*temp == '\n' || *temp == 0) )
					{
						// increase buffer position, so that we can start with a valid character in the next run
						_buffer_pos++;
						return true;
					}
					else
					{
						done = !fillBuffer();
					}
				}
				else if(*temp == '\n' || *temp == 0)
				{
					// increase buffer position, so that we can start with a valid character in the next run
					_buffer_pos++;
					return true;
				}
			}
			else
			{
				done = !fillBuffer();
			}
		}
		else
		{
			done = !fillBuffer();
		}
	}
	return false;
}

bool vfs::CReadLine::getLine(std::string& line)
{
	line.clear();
	return fromBuffer(line);
}

/*************************************************************************************/
/*************************************************************************************/

vfs::CTokenizer::CTokenizer(vfs::String const& str)
: m_list(str), m_current(0), m_next(0)
{};

vfs::CTokenizer::~CTokenizer()
{};

bool vfs::CTokenizer::next(vfs::String& token, vfs::String::char_t delimeter)
{
	if(m_next != vfs::String::str_t::npos)
	{
		m_next = m_list.c_wcs().find_first_of(delimeter, m_current);
		if(m_next != vfs::String::str_t::npos)
		{
			token.r_wcs().assign(vfs::trimString(m_list,m_current,m_next > m_current ? m_next-1 : m_current).c_wcs());
			m_current = m_next+1;
		}
		else
		{
			// last or only entry
			token.r_wcs().assign(vfs::trimString(m_list,m_current,m_list.length()).c_wcs());
		}
		return true;
	}
	return false;
}

/*************************************************************************************/
/*************************************************************************************/

/**
 *  try to recursively match the pattern
 */
bool vfs::matchPattern(vfs::String const& sPattern, vfs::String const& sStr)
{
	return matchPattern(sPattern,sStr.c_wcs());
}

bool vfs::matchPattern(vfs::String const& sPattern, vfs::String::str_t const& sStr)
{
	vfs::String::str_t const& pat = sPattern.c_wcs();

	vfs::String::size_t star = pat.find_first_of(vfs::Const::STAR());
	if(star == vfs::String::str_t::npos)
	{
		return vfs::StrCmp::Equal( pat, sStr );
	}
	else if(star == 0)
	{
		if(pat.length() == 1)
		{
			// there is only the '*' -> matches all strings
			return true;
		}
		vfs::String::char_t atpos1 = pat.at(1);
		vfs::String::size_t match = vfs::String::size_t(-1);
		do
		{
			match = sStr.find_first_of(atpos1,match+1);
			if(match == vfs::String::str_t::npos)
			{
				return false;
			}
		} while(!matchPattern( pat.substr(1,pat.length()-1), sStr.substr(match,sStr.length()-match) ));
		return true;
	}
	else // if(star > 0)
	{
		// check if characters before * match
		if(!vfs::StrCmp::Equal(pat.substr(0,star), sStr.substr(0,star)) )
		{
			return false;
		}
		return matchPattern( pat.substr(star,pat.length()-star), sStr.substr(star,sStr.length()-star) );
	}
}

/*************************************************************************************/
/*************************************************************************************/
