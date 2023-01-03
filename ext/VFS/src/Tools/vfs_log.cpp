/* 
 * bfVFS : vfs/Tools/vfs_log.cpp
 *  - simple file logger
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

#include <vfs/Tools/vfs_log.h>
#include <cstring>

#ifdef WIN32
static const char ENDL[] = "\r\n";
#else
static const char ENDL[] = "\n";
#endif

typedef std::list<vfs::Log*> LogList_t;
static LogList_t* __logs;
std::list<vfs::Log*>& vfs::Log::_logs()
{
	if(!__logs)
	{
		__logs = new LogList_t;
	}
	return *__logs;
}

vfs::Log* vfs::Log::create(vfs::Path const& fileName, bool append, EFlushMode flushMode)
{
	_logs().push_back(new vfs::Log(fileName, true, append, flushMode));
	return _logs().back();
}
vfs::Log* vfs::Log::create(vfs::tWritableFile* file, bool append, EFlushMode flushMode)
{
	_logs().push_back(new vfs::Log(file, append, flushMode));
	return _logs().back();
}

void vfs::Log::flushDeleteAll()
{
	LogList_t::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		(*it)->flush();
		(*it)->Release();
	}
	_logs().clear();
}

void vfs::Log::flushReleaseAll()
{
	LogList_t::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		(*it)->releaseFile();
	}
}

vfs::String vfs::Log::_shared_id_str;

vfs::String const& vfs::Log::getSharedString()
{
	return _shared_id_str;
}

void vfs::Log::setSharedString(vfs::String const& str)
{
	_shared_id_str = str;
}


vfs::Log::Log(vfs::Path const& fileName, bool use_vfs_file, bool append, EFlushMode flushMode)
:	IRefCountable(),
	_filename(fileName), _file(NULL), _own_file(false),
	_first_write(true), _flush_mode(flushMode), _append(append),
	_buffer_size(0), _buffer_test_size(512)
{};

vfs::Log::Log(vfs::tWritableFile* file, bool append, EFlushMode flushMode)
:	IRefCountable(),
	_file(file), _own_file(false),
	_first_write(true), _flush_mode(flushMode), _append(append),
	_buffer_size(0), _buffer_test_size(512)
{
}


vfs::Log::~Log()
{
	// the final flush
	flush();

	// also delete file pointer if we created it
	if(_file && _own_file)
	{
		delete _file;
		_file = NULL;
	}
	
	// one extra unlock wouldn't hurt
	_mutex.unlock();
}

int vfs::Log::Reserve()
{
	VFS_LOCK(_mutex);
	return this->Register();
}
int vfs::Log::Release()
{
	_mutex.lock();
	int tmp_count = this->UnRegister();
	if(tmp_count > 0)
	{
		_mutex.unlock();
	}
	// otherwise the mutex object is destroyed at that time
	return tmp_count;
}
int vfs::Log::RefCount()
{
	VFS_LOCK(_mutex);
	return this->GetRefCount();
}


void vfs::Log::destroy()
{
	// no need to lock here as 'flush' and 'Release' do it themselves
	this->flush();
	if(this->Release() <= 0)
	{
		// object is deleted, so remove it now from the static list
		_logs().remove(this);
	}
}

void vfs::Log::releaseFile()
{
	VFS_LOCK(_mutex);
	this->flush();
	this->_file = NULL;
}

vfs::Log& vfs::Log::operator<<(vfs::UInt64 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
vfs::Log& vfs::Log::operator<<(vfs::UInt32 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
vfs::Log& vfs::Log::operator<<(vfs::UInt16 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
vfs::Log& vfs::Log::operator<<(vfs::UInt8 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}

vfs::Log& vfs::Log::operator<<(vfs::Int64 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
vfs::Log& vfs::Log::operator<<(vfs::Int32 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
vfs::Log& vfs::Log::operator<<(vfs::Int16 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
vfs::Log& vfs::Log::operator<<(vfs::Int8 const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
#ifdef _MSC_VER
	vfs::Log& vfs::Log::operator<<(DWORD const& t)
	{
		VFS_LOCK(_mutex);
		return pushNumber(t);
	}
#endif
vfs::Log& vfs::Log::operator<<(float const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}
vfs::Log& vfs::Log::operator<<(double const& t)
{
	VFS_LOCK(_mutex);
	return pushNumber(t);
}

vfs::Log& vfs::Log::operator<<(const char* t)
{
	VFS_LOCK(_mutex);
	_buffer << t;
	_buffer_size += strlen(t);
	_test_flush();
	return *this;
}
vfs::Log& vfs::Log::operator<<(const wchar_t* t)
{
	VFS_LOCK(_mutex);
	std::string s = vfs::String::as_utf8(t);
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}
vfs::Log& vfs::Log::operator<<(std::string const& t)
{
	VFS_LOCK(_mutex);
	_buffer << t;
	_buffer_size += t.length();
	_test_flush();
	return *this;
}
vfs::Log& vfs::Log::operator<<(std::wstring const& t)
{
	VFS_LOCK(_mutex);
	std::string s = vfs::String::as_utf8(t);
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}
vfs::Log& vfs::Log::operator<<(vfs::String const& t)
{
	VFS_LOCK(_mutex);
	std::string s = t.utf8();
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}

vfs::Log& vfs::Log::operator<<(void* const& t)
{
	VFS_LOCK(_mutex);
	_buffer << t;
	return *this;
}

vfs::Log& vfs::Log::operator<<(vfs::Log::_endl const& endl)
{
	VFS_LOCK(_mutex);
	_buffer << ENDL;
	_buffer_size += sizeof(ENDL)-1;
	if(_flush_mode == vfs::Log::FLUSH_ON_ENDL) flush();
	return *this;
}

/*
vfs::Log& vfs::Log::endl()
{
	_buffer << ENDL;
	_buffer_size += sizeof(ENDL)-1;
	_test_flush();
	return *this;
}
*/
void vfs::Log::setAppend(bool append)
{
	VFS_LOCK(_mutex);
	_append = append;
}

void vfs::Log::setBufferSize(vfs::UInt32 bufferSize)
{
	VFS_LOCK(_mutex);
	_buffer_test_size = bufferSize;
}

void vfs::Log::_test_flush(bool force)
{
	if( (_flush_mode == FLUSH_IMMEDIATELY) || 
		(_flush_mode == FLUSH_BUFFER && _buffer_size > _buffer_test_size) || 
		(/*_flush_mode == FLUSH_ON_DELETE &&*/ force == true) )
	{
		flush();
	}
}


vfs::Log::EFlushMode vfs::Log::flushMode()
{
	return _flush_mode;
}
void vfs::Log::flushMode(vfs::Log::EFlushMode fmode)
{
	_flush_mode = fmode;
}

#include <ctime>
#include <vfs/Core/vfs.h>

void vfs::Log::flush()
{
	VFS_LOCK(_mutex);
	::size_t buflen = _buffer.str().length();
	if(buflen == 0)
	{
		return;
	}

	if(!_file)
	{
		VFS_THROW_IFF(!_filename.empty(), L"_file is NULL and _filename is empty");
		//vfs::CVirtualProfile *prof = getVFS()->getProfileStack()->topProfile();
		//if( prof && prof->cWritable )
		if(vfs::canWrite())
		{
			try
			{
				vfs::COpenWriteFile file_raii(_filename,true,!_append);
				_file = &file_raii.file();
				file_raii.release();
				_own_file = false;
			}
			catch(...)
			{
			}
		}
		else
		{
			try
			{
				_file = vfs::tWritableFile::cast(new vfs::CFile(_filename));
				_file->openWrite(true,!_append);
				_own_file = true;
			}
			catch(...)
			{
			}
		}
	}

	vfs::COpenWriteFile wfile(_file);
	if(_append)
	{
		wfile->setWritePosition(0,vfs::IBaseFile::SD_END);
	}

	if(_first_write)
	{
		time_t rawtime;
		time ( &rawtime );
		std::string datetime(ctime(&rawtime));
		std::string s_out;
	
		vfs::size_t wloc = wfile->getWritePosition();
		if(wloc > 0)
		{
			s_out = ENDL;
		}
		s_out += " *** ";
		s_out += datetime.substr(0,datetime.length()-1);
		s_out += " *** ";
		s_out += ENDL;
		s_out += "[ ";
		s_out += _shared_id_str.utf8();
		s_out += " ]";
		s_out += ENDL;
		s_out += ENDL;

		wfile->write(s_out.c_str(), s_out.length());
		_first_write = false;
	}

	wfile->write(_buffer.str().c_str(), buflen);
	_buffer.str("");
	_buffer.clear();
	_buffer_size = 0;

	_append = true;
}

void vfs::Log::lock()
{
	_mutex.lock();
}
void vfs::Log::unlock()
{
	_mutex.unlock();
}
