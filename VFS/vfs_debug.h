#ifndef _VFS_DEBUG_H_
#define _VFS_DEBUG_H_

#include "vfs_types.h"
#include <list>

class CBasicException : public std::exception
{
public:
	CBasicException(const wchar_t* text, const char* function, int line, const char* file, CBasicException* ex=NULL);
	CBasicException(utf8string const& text, utf8string const& function, int line, const char* file, CBasicException* ex=NULL);

	utf8string GetLastEntryString() const;
	utf8string GetExceptionString() const;

	void WriteFile(vfs::Path const& sPath);

	struct SEntry
	{
		utf8string		time;
		utf8string		message;
		utf8string		function;
		int				line;
		utf8string		file;
	};
	typedef std::list<SEntry> CALLSTACK;
	CALLSTACK m_CallStack;

	int				_LINE;
	utf8string		_FILE;
	utf8string		_FUNCTION;
	utf8string		_time;
};

void LogException(CBasicException const& ex);

#ifdef WIN32
#define _FUNCTION_FORMAT_		__FUNCTION__
#else
//#define _FUNCTION_FORMAT_		__FUNCTION__
#define _FUNCTION_FORMAT_		__PRETTY_FUNCTION__
#endif

#define THROWEXCEPTION(message) throw CBasicException((message), _FUNCTION_FORMAT_, __LINE__, __FILE__, NULL)
#define RETHROWEXCEPTION(message,ex) throw CBasicException((message), _FUNCTION_FORMAT_, __LINE__, __FILE__, (ex))

#define THROWIFFALSE(boolexpr,message) if(!(boolexpr)){THROWEXCEPTION((message));}

#define TRYCATCH_RETHROW(expr,message) \
{ \
	try { (expr); } \
	catch(CBasicException &ex){ throw CBasicException((message),_FUNCTION_FORMAT_,__LINE__,__FILE__,&ex); } \
}

#define IGNOREEXCEPTION(expr) \
{ \
	try{ (expr); } \
	catch(CBasicException& ex){ LogException(ex); } \
}

#endif // _VFS_DEBUG_H_
