#include "Log.h"

#ifdef WIN32
const char CLog::endl[] = "\r\n";
#else
const char CLog::endl[] = "\n";
#endif

std::list<CLog*>& CLog::_logs()
{
	static std::list<CLog*> logs;
	return logs;
}

CLog* CLog::Create(vfs::Path fileName, bool append, EFlushMode flushMode)
{
	_logs().push_back(new CLog(fileName, append, flushMode));
	return _logs().back();
}
void CLog::FlushFinally()
{
	std::list<CLog*>::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		delete *it;
	}
	_logs().clear();
}

void CLog::FlushAll()
{
	std::list<CLog*>::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		(*it)->Flush();
		(*it)->_file = NULL;
	}
}


CLog::CLog(vfs::Path fileName, bool append, EFlushMode flushMode)
:	_filename(fileName), _file(NULL), _own_file(false),
	_first_write(true), _flush_mode(flushMode), _append(append),
	_buffer_size(0), _buffer_test_size(512)
{};

CLog::~CLog()
{
	_test_flush(true);
	if(_file && _own_file)
	{
		delete _file;
	}
}

CLog& CLog::operator<<(unsigned int const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(unsigned short const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(unsigned char const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(int const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(short const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(char const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(float const& t)
{
	return PushNumber(t);
}
CLog& CLog::operator<<(double const& t)
{
	return PushNumber(t);
}

CLog& CLog::operator<<(const char* t)
{
	_buffer << t;
	_buffer_size += strlen(t);
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(const wchar_t* t)
{
	std::string s = utf8string::as_utf8(t);
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(std::string const& t)
{
	_buffer << t;
	_buffer_size += t.length();
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(std::wstring const& t)
{
	std::string s = utf8string::as_utf8(t);
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}
CLog& CLog::operator<<(utf8string const& t)
{
	std::string s = t.utf8();
	_buffer << s;
	_buffer_size += s.length();
	_test_flush();
	return *this;
}

CLog& CLog::Endl()
{
	_buffer << CLog::endl;
	_buffer_size += sizeof(CLog::endl)-1;
	_test_flush();
	return *this;
}

void CLog::SetAppend(bool append)
{
	_append = append;
}

void CLog::SetBufferSize(vfs::UInt32 bufferSize)
{
	_buffer_test_size = bufferSize;
}

void CLog::_test_flush(bool force)
{
	if( (_flush_mode == FLUSH_IMMEDIATELY) || 
		(_flush_mode == FLUSH_BUFFER && _buffer_size > _buffer_test_size) || 
		(/*_flush_mode == FLUSH_ON_DELETE &&*/ force == true) )
	{
		Flush();
	}
}

#include <ctime>
void CLog::Flush()
{
	vfs::UInt32 buflen = _buffer.str().length();
	if(buflen == 0)
	{
		return;
	}

	if(!_file)
	{
		try
		{
			vfs::COpenWriteFile file_raii(_filename,true,!_append);
			_file = &file_raii.file();
			file_raii.release();
		}
		catch(CBasicException& ex)
		{
			LogException(ex);
			// write file anyway 
			_file = vfs::tWriteableFile::Cast(new vfs::CFile(_filename));
			_file->OpenWrite(true,!_append);
			_own_file = true;
		}
	}

	vfs::COpenWriteFile wfile(_file);
	if(_append)
	{
		wfile.file().SetWriteLocation(0,vfs::IBaseFile::SD_END);
	}

	vfs::UInt32 io;
	if(_first_write)
	{
		time_t rawtime;
		time ( &rawtime );
		std::string datetime(ctime(&rawtime));
		std::string s_out;
	
		vfs::UInt32 wloc = wfile.file().GetWriteLocation();
		if(wloc > 0)
		{
			s_out = CLog::endl;
		}
		s_out += " *** ";
		s_out += datetime.substr(0,datetime.length()-1);
		s_out += " *** ";
		s_out += CLog::endl;
		s_out += CLog::endl;
		wfile.file().Write(s_out.c_str(), s_out.length(), io);
		_first_write = false;
	}

	wfile.file().Write(_buffer.str().c_str(), buflen, io);
	_buffer.str("");
	_buffer.clear();
	_buffer_size = 0;

	_append = true;
}
