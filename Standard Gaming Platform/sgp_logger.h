#ifndef SGP_LOGGER_H
#define SGP_LOGGER_H

#include <vector>
#if defined(USE_VFS)
#	include <vfs/Core/vfs_string.h>
#	include <vfs/Tools/vfs_log.h>
#	include <vfs/Tools/vfs_hp_timer.h>
#else
#	include <iostream>
#endif

namespace sgp
{
	typedef unsigned int Logger_ID;

	enum _endl{endl};
	enum _lock{lock};
	enum _flush{flush};
	
	class Logger
	{
		struct LogData
		{
#if defined(USE_VFS)
			LogData() : file(NULL), stream(NULL), screen(NULL) {};
			vfs::HPTimer		timer;
			vfs::Log*			file;
#else
			LogData() : stream(NULL), screen(NULL) {};
#endif
			std::wostream*		stream;
			std::wstringstream*	screen; 
		};
	public:

		enum FlushMode
		{
			FLUSH_ON_DELETE,
			FLUSH_ON_ENDL,
			FLUSH_ON_FLUSH,
			FLUSH_FULL_BUFFER,
			FLUSH_IMMIDIATELY,
		};

		// need to lock/unlock on every logging operation, as they can be called from multiple threads
		class LogInstance
		{
			friend class Logger;
			LogInstance(LogData* ld) : _log(*ld) {};
		public:
			template <typename T>
			LogInstance& operator<<(T const& t){
				if(_log.stream) (*_log.stream) << t;
				if(_log.screen) (*_log.screen) << t;
#if defined(USE_VFS)
				if(_log.file  ) (*_log.file)   << t;
#endif
				return *this;
			}
		private:
			LogData&	_log;
		};


		static Logger& instance();

		bool init();
		bool shutdown();

		LogInstance	logger(Logger_ID id);

		Logger_ID	createLogger();

#if defined(USE_VFS)
		void		connectFile(Logger_ID id, vfs::Path const& log_file, bool append, FlushMode fp);
		void		disconnectFile(Logger_ID id);
#endif
		void		connectStream(Logger_ID id, std::ostream& stream);
		void		disconnectStream(Logger_ID id);

		void		connectScreen(Logger_ID id);
		void		disconnectScreen(Logger_ID id);

		void		flushMode(Logger_ID id, FlushMode fmode);
		FlushMode	flushMode(Logger_ID id);
	private:
		Logger();
		~Logger();

	private:
		static int					_next_log_id;

		static std::vector<LogData> _logs;
	};

	template <>
	Logger::LogInstance& Logger::LogInstance::operator<< <_endl>(_endl const& e);
	template <>
	Logger::LogInstance& Logger::LogInstance::operator<< <_lock>(_lock const& l);
	template <>
	Logger::LogInstance& Logger::LogInstance::operator<< <_flush>(_flush const& f);
}

#if defined(USE_VFS)
inline sgp::Logger::LogInstance SGP_LOG(sgp::Logger_ID id, vfs::String const& str){
#else
inline sgp::Logger::LogInstance SGP_LOG(int id, const char* str){
#endif
	return sgp::Logger::instance().logger(id) << (str) << sgp::endl;
}
inline sgp::Logger::LogInstance SGP_LOG(sgp::Logger_ID id){
	return sgp::Logger::instance().logger(id);
}

#if defined(USE_VFS)
inline sgp::Logger::LogInstance SGP_INFO(vfs::String const& str){
#else
inline sgp::Logger::LogInstance SGP_INFO(const char* str){
#endif
	return sgp::Logger::instance().logger(0) << str << sgp::endl;
}
inline sgp::Logger::LogInstance SGP_INFO(){
	return sgp::Logger::instance().logger(0);
}

#if defined(USE_VFS)
inline sgp::Logger::LogInstance SGP_WARNING(vfs::String const& str){
#else
inline sgp::Logger::LogInstance SGP_WARNING(const char* str){
#endif
	return sgp::Logger::instance().logger(0) << "WARNING : " << str << sgp::endl;
}
inline sgp::Logger::LogInstance SGP_WARNING(){
	return sgp::Logger::instance().logger(0) << "WARNING : ";
}

#if defined(USE_VFS)
inline sgp::Logger::LogInstance SGP_ERROR(vfs::String const& str){
#else
inline sgp::Logger::LogInstance SGP_ERROR(const char* str){
#endif
	return sgp::Logger::instance().logger(0) << "ERROR : " << str << sgp::endl;
}
inline sgp::Logger::LogInstance SGP_ERROR(){
	return sgp::Logger::instance().logger(0) << "ERROR : ";
}

#endif // SGP_LOGGER_H
