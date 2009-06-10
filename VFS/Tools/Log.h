#ifndef _LOG_H_
#define _LOG_H_

#include "Tools.h"
#include "../vfs_types.h"
#include "../Interface/vfs_file_interface.h"
#include "../File/vfs_file.h"
#include "../vfs_file_raii.h"

//class CMemoryFile;
class CLog
{
public:
	enum EFlushMode
	{
		FLUSH_ON_DELETE,
		FLUSH_BUFFER,
		FLUSH_IMMEDIATELY,
	};
public:

	CLog(vfs::Path fileName, bool append = false, EFlushMode flushMode = FLUSH_ON_DELETE);
	~CLog();

	static CLog*	Create(vfs::Path fileName, bool append = false, EFlushMode flushMode = FLUSH_ON_DELETE);
	static void		FlushAll();
	static void		FlushFinally();

	CLog& operator<<(unsigned int const& t);
	CLog& operator<<(unsigned short const& t);
	CLog& operator<<(unsigned char const& t);
	CLog& operator<<(int const& t);
	CLog& operator<<(short const& t);
	CLog& operator<<(char const& t);
	CLog& operator<<(float const& t);
	CLog& operator<<(double const& t);

	CLog& operator<<(const char* t);
	CLog& operator<<(const wchar_t* t);
	CLog& operator<<(std::string const& t);
	CLog& operator<<(std::wstring const& t);
	CLog& operator<<(utf8string const& t);

	CLog& Endl();
	static const char endl[];

	void SetAppend(bool append = true);
	void SetBufferSize(vfs::UInt32 bufferSize);

	void Flush();
private:
	void _test_flush(bool force=false);

	template<typename T_>
	CLog& PushNumber(T_ const& t)
	{
		_buffer << ToString<char>(t);
		_buffer_size += sizeof(T_);
		_test_flush();
		return *this;
	}
private:
	vfs::Path				_filename;
	vfs::tWriteableFile*	_file;
	bool					_own_file;
	bool					_first_write;
	EFlushMode				_flush_mode;
	bool					_append;
	vfs::UInt32				_buffer_size, _buffer_test_size;
	std::stringstream		_buffer;
private:
	static std::list<CLog*>& _logs();
};


#endif // _LOG_H_
