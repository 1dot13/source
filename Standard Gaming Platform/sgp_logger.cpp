#include "sgp_logger.h"
#include "Debug.h"
#include "Font Control.h"

#include "Message.h"
#include <iostream>

static inline void flush_screen(std::wstringstream& stream)
{
	std::wstring str = stream.str();
	if(!str.empty())
	{
		ScreenMsg(MSG_FONT_YELLOW, 255, (STR16)str.c_str());
	}
}

int sgp::Logger::_next_log_id = -1;

	//std::vector<sgp::Logger::STimeLog> sgp::Logger::_logs;
	std::vector<sgp::Logger::LogData> sgp::Logger::_logs;

sgp::Logger::Logger()
{
	connectFile(this->createLogger(), L"game_log.log", false, sgp::Logger::FLUSH_ON_ENDL);
}

sgp::Logger::~Logger()
{
}

sgp::Logger& sgp::Logger::instance()
{
	static Logger _logger;
	return _logger;
}

bool sgp::Logger::init()
{
	return true;
}

bool sgp::Logger::shutdown()
{
	for(::size_t i=0; i < _logs.size(); ++i)
	{
		disconnectFile(i);
		disconnectStream(i);
		disconnectScreen(i);
	}
	return true;
}

sgp::Logger_ID sgp::Logger::createLogger()
{
	_logs.push_back(sgp::Logger::LogData());
	Logger_ID id = _logs.size() - 1;
	_logs[id].timer.startTimer();
	return id;
}

sgp::Logger::FlushMode sgp::Logger::flushMode(sgp::Logger_ID id)
{
	sgp::Logger::FlushMode fmode = (sgp::Logger::FlushMode)-1;
	if(id >= 0 && id < _logs.size())
	{
		if(_logs[id].file)
		{
			vfs::Log::EFlushMode fm = _logs[id].file->flushMode();

			if     (fm == vfs::Log::FLUSH_ON_DELETE)   fmode = sgp::Logger::FLUSH_ON_DELETE;
			else if(fm == vfs::Log::FLUSH_IMMEDIATELY) fmode = sgp::Logger::FLUSH_IMMIDIATELY;
			else if(fm == vfs::Log::FLUSH_BUFFER)      fmode = sgp::Logger::FLUSH_FULL_BUFFER;
			else if(fm == vfs::Log::FLUSH_ON_ENDL)     fmode = sgp::Logger::FLUSH_ON_ENDL;
		}
	}
	return fmode;
}

void sgp::Logger::flushMode(sgp::Logger_ID id, sgp::Logger::FlushMode fmode)
{
	if(id >= 0 && id < _logs.size())
	{
		vfs::Log::EFlushMode fm;
		if     (fmode == sgp::Logger::FLUSH_ON_DELETE)   fm = vfs::Log::FLUSH_ON_DELETE;
		else if(fmode == sgp::Logger::FLUSH_IMMIDIATELY) fm = vfs::Log::FLUSH_IMMEDIATELY;
		else if(fmode == sgp::Logger::FLUSH_FULL_BUFFER) fm = vfs::Log::FLUSH_BUFFER;
		else if(fmode == sgp::Logger::FLUSH_ON_ENDL)     fm = vfs::Log::FLUSH_ON_ENDL;
		else								             fm = vfs::Log::FLUSH_IMMEDIATELY;

		if(_logs[id].file)
		{
			_logs[id].file->releaseFile();
			_logs[id].file->flushMode(fm);
		}
	}
}


void sgp::Logger::connectFile(sgp::Logger_ID id, vfs::Path const& log_file, bool append, FlushMode fmode)
{
	if(id >= 0 && id < _logs.size())
	{
		vfs::Log::EFlushMode fm;
		if     (fmode == sgp::Logger::FLUSH_ON_DELETE)   fm = vfs::Log::FLUSH_ON_DELETE;
		else if(fmode == sgp::Logger::FLUSH_IMMIDIATELY) fm = vfs::Log::FLUSH_IMMEDIATELY;
		else if(fmode == sgp::Logger::FLUSH_FULL_BUFFER) fm = vfs::Log::FLUSH_BUFFER;
		else if(fmode == sgp::Logger::FLUSH_ON_ENDL)     fm = vfs::Log::FLUSH_ON_ENDL;
		else                                             fm = vfs::Log::FLUSH_IMMEDIATELY;

		_logs[id].file = vfs::Log::create(log_file,append,fm);
	}
}
void sgp::Logger::disconnectFile(sgp::Logger_ID id)
{
	if(id >= 0 && id < _logs.size())
	{
		if(_logs[id].file)
		{
			_logs[id].file->destroy();
			_logs[id].file = NULL;
		}
	}
}

void sgp::Logger::connectStream(sgp::Logger_ID id, std::ostream& stream)
{
	if(id >= 0 && id < _logs.size())
	{
		_logs[id].stream = &std::wcout;
	}
}
void sgp::Logger::disconnectStream(sgp::Logger_ID id)
{
	if(id >= 0 && id < _logs.size())
	{
		_logs[id].stream = NULL;
	}
}

void sgp::Logger::connectScreen(sgp::Logger_ID id)
{
	if(id >= 0 && id < _logs.size())
	{
		if(_logs[id].screen)
		{
			disconnectScreen(id);
		}
		_logs[id].screen = new std::wstringstream;
	}
}
void sgp::Logger::disconnectScreen(sgp::Logger_ID id)
{
	if(id >= 0 && id < _logs.size())
	{
		if(_logs[id].screen)
		{
			flush_screen(*_logs[id].screen);
			delete _logs[id].screen;
			_logs[id].screen = NULL;
		}
	}
}


sgp::Logger::LogInstance sgp::Logger::logger(sgp::Logger_ID id)
{
	if(id < _logs.size())
	{
		std::wstringstream tmp;
		tmp << L"[" << _logs[id].timer.running() << L"] : ";
		std::wstring str(tmp.str());

		LogData& ld = _logs[id];
		if(ld.file) {
			(*ld.file).lock();
			(*ld.file) << str;
		}
		if(ld.stream) {
			(*ld.stream) << str;
		}
		if(ld.screen) {
			(*ld.screen) << str;
		}
		return sgp::Logger::LogInstance(&ld);
	}
	SGP_THROW(L"Unknown log id");
}

template <>
sgp::Logger::LogInstance& sgp::Logger::LogInstance::operator<< <sgp::_flush>(sgp::_flush const& e)
{
	if(_log.file)
	{
		(*_log.file).flush();
		(*_log.file).unlock();
	}
	if(_log.stream)
	{
		(*_log.stream).flush();
	}
	if(_log.screen)
	{
		flush_screen(*_log.screen);
	}
	return *this;
}

template <>
sgp::Logger::LogInstance& sgp::Logger::LogInstance::operator<< <sgp::_endl>(sgp::_endl const& e)
{
	if(_log.file)
	{
		(*_log.file) << vfs::Log::endl;
		if(_log.file->flushMode() == vfs::Log::FLUSH_ON_ENDL)
		{
			(*_log.file).flush();
		}
		(*_log.file).unlock();
	}
	if(_log.stream)
	{
		(*_log.stream) << std::endl;
		(*_log.stream).flush();
	}
	if(_log.screen)
	{
		flush_screen(*_log.screen);
	}
	return *this;
}
template <>
sgp::Logger::LogInstance& sgp::Logger::LogInstance::operator<< <sgp::_lock>(sgp::_lock const& l)
{
	_log.file->lock();
	return *this;
}
