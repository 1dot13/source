#ifndef _VFS_DEBUG_H_
#define _VFS_DEBUG_H_

#include "vfs_types.h"
#include "vfs_path.h"
#include <list>

#ifdef _MSC_VER
class VFS_API std::exception;
#endif

class VFS_API CBasicException : public std::exception
{
public:
	CBasicException(const wchar_t* text, const char* function, int line, const char* file, CBasicException* ex=NULL);
	CBasicException(utf8string const& text, utf8string const& function, int line, const char* file, CBasicException* ex=NULL);

	virtual ~CBasicException() throw();

	virtual const char* what() const throw();

	utf8string getLastEntryString() const;
	utf8string getExceptionString() const;

	void writeFile(vfs::Path const& sPath);

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

void logException(CBasicException const& ex);

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
	catch(CBasicException& ex){ logException(ex); } \
}

#endif // _VFS_DEBUG_H_
