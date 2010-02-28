#ifndef _LOG_H_
#define _LOG_H_

#include "Tools.h"
#include "../vfs_types.h"
#include "../Interface/vfs_file_interface.h"
#include "../File/vfs_file.h"
#include "../vfs_file_raii.h"

class VFS_API CLog
{
public:
	enum EFlushMode
	{
		FLUSH_ON_DELETE,
		FLUSH_BUFFER,
		FLUSH_IMMEDIATELY,
	};
public:

	CLog(vfs::Path const& fileName, bool append = false, EFlushMode flushMode = FLUSH_ON_DELETE);
	~CLog();

	static CLog*	create(vfs::Path const& fileName, bool append = false, EFlushMode flushMode = FLUSH_ON_DELETE);
	static void		flushAll();
	static void		flushFinally();

	static utf8string const&	getSharedString();
	static void					setSharedString(utf8string const& str);

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

	CLog& endl();
	static const char ENDL[];

	void setAppend(bool append = true);
	void setBufferSize(vfs::UInt32 bufferSize);

	void flush();
private:
	void _test_flush(bool force=false);

	template<typename T_>
	CLog& pushNumber(T_ const& t)
	{
		_buffer << toString<char>(t);
		_buffer_size += sizeof(T_);
		_test_flush();
		return *this;
	}
private:
	vfs::Path				_filename;
	vfs::tWritableFile*		_file;
	bool					_own_file;
	bool					_first_write;
	EFlushMode				_flush_mode;
	bool					_append;
	::size_t				_buffer_size, _buffer_test_size;
	std::stringstream		_buffer;
private:
	static std::list<CLog*>& _logs();
	static utf8string		_shared_id_str;
};


#endif // _LOG_H_
