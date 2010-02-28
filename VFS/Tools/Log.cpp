#include "Log.h"
#include <cstring>

#ifdef WIN32
const char CLog::ENDL[] = "\r\n";
#else
const char CLog::ENDL[] = "\n";
#endif

std::list<CLog*>& CLog::_logs()
{
	static std::list<CLog*> logs;
	return logs;
}

CLog* CLog::create(vfs::Path const& fileName, bool append, EFlushMode flushMode)
{
	_logs().push_back(new CLog(fileName, append, flushMode));
	return _logs().back();
}
void CLog::flushFinally()
{
	std::list<CLog*>::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		delete *it;
	}
	_logs().clear();
}

void CLog::flushAll()
{
	std::list<CLog*>::iterator it = _logs().begin();
	for(; it != _logs().end(); ++it)
	{
		(*it)->flush();
		(*it)->_file = NULL;
	}
}

utf8string CLog::_shared_id_str;

utf8string const& CLog::getSharedString()
{
	return _shared_id_str;
}

void CLog::setSharedString(utf8string const& str)
{
	_shared_id_str = str;
}


CLog::CLog(vfs::Path const& fileName, bool append, EFlushMode flushMode)
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
	return pushNumber(t);
}
CLog& CLog::operator<<(unsigned short const& t)
{
	return pushNumber(t);
}
CLog& CLog::operator<<(unsigned char const& t)
{
	return pushNumber(t);
}
CLog& CLog::operator<<(int const& t)
{
	return pushNumber(t);
}
CLog& CLog::operator<<(short const& t)
{
	return pushNumber(t);
}
CLog& CLog::operator<<(char const& t)
{
	return pushNumber(t);
}
CLog& CLog::operator<<(float const& t)
{
	return pushNumber(t);
}
CLog& CLog::operator<<(double const& t)
{
	return pushNumber(t);
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

CLog& CLog::endl()
{
	_buffer << CLog::ENDL;
	_buffer_size += sizeof(CLog::ENDL)-1;
	_test_flush();
	return *this;
}

void CLog::setAppend(bool append)
{
	_append = append;
}

void CLog::setBufferSize(vfs::UInt32 bufferSize)
{
	_buffer_test_size = bufferSize;
}

void CLog::_test_flush(bool force)
{
	if( (_flush_mode == FLUSH_IMMEDIATELY) || 
		(_flush_mode == FLUSH_BUFFER && _buffer_size > _buffer_test_size) || 
		(/*_flush_mode == FLUSH_ON_DELETE &&*/ force == true) )
	{
		flush();
	}
}

#include <ctime>
void CLog::flush()
{
	::size_t buflen = _buffer.str().length();
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
			try
			{
				logException(ex);
				// write file anyway 
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
		wfile.file().setWritePosition(0,vfs::IBaseFile::SD_END);
	}

	if(_first_write)
	{
		time_t rawtime;
		time ( &rawtime );
		std::string datetime(ctime(&rawtime));
		std::string s_out;
	
		vfs::size_t wloc = wfile.file().getWritePosition();
		if(wloc > 0)
		{
			s_out = CLog::ENDL;
		}
		s_out += " *** ";
		s_out += datetime.substr(0,datetime.length()-1);
		s_out += " *** ";
		s_out += CLog::ENDL;
		s_out += "[ ";
		s_out += _shared_id_str.utf8();
		s_out += " ]";
		s_out += CLog::ENDL;
		s_out += CLog::ENDL;

		wfile.file().write(s_out.c_str(), s_out.length());
		_first_write = false;
	}

	wfile.file().write(_buffer.str().c_str(), buflen);
	_buffer.str("");
	_buffer.clear();
	_buffer_size = 0;

	_append = true;
}
