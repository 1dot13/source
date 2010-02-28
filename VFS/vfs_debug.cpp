#include "vfs_debug.h"

#include "utf8string.h"
#include "vfs_file_raii.h"
#include "File/vfs_file.h"

#include "Tools/Log.h"

#include <sstream>
#include <ctime>

CBasicException::CBasicException(const wchar_t* text, const char* function, int line, const char* file, CBasicException* ex)
: std::exception() //(utf8string::as_utf8(text).c_str())
{
	if(ex)
	{
		m_CallStack.insert(m_CallStack.end(), ex->m_CallStack.begin(), ex->m_CallStack.end());
	}
	_LINE = line;
	_FILE = file;
	_FUNCTION = function;

	time_t rawtime;
	time ( &rawtime );
	std::string datetime(ctime(&rawtime));
	IGNOREEXCEPTION(_time = utf8string(datetime.substr(0,datetime.length()-1)));

	SEntry en;
	en.message = text;
	en.line = line;
	en.file = file;
	en.function = function;
	en.time = _time;
	m_CallStack.push_back(en);
};

CBasicException::CBasicException(utf8string const& text, utf8string const& function, int line, const char* file, CBasicException* ex)
: std::exception() //(text.utf8().c_str())
{
	if(ex)
	{
		m_CallStack.insert(m_CallStack.end(), ex->m_CallStack.begin(), ex->m_CallStack.end());
	}
	_LINE = line;
	_FILE = file;
	_FUNCTION = function;

	time_t rawtime;
	time ( &rawtime );
	std::string datetime(ctime(&rawtime));
	IGNOREEXCEPTION(_time = utf8string(datetime.substr(0,datetime.length()-1)));

	SEntry en;
	en.message = text;
	en.line = line;
	en.file = file;
	en.function = function;
	en.time = _time;
	m_CallStack.push_back(en);
};

CBasicException::~CBasicException() throw()
{
}

utf8string CBasicException::getLastEntryString() const
{
	if(!m_CallStack.empty())
	{
		CALLSTACK::const_reverse_iterator rit = m_CallStack.rbegin();
		std::wstringstream wss;
		wss << rit->file.c_wcs()
			<< L" (l. "
			<< rit->line
			<< ") : ["
			<< rit->function.c_wcs()
			<< L"] - "
			<< rit->message.c_wcs();
		return wss.str();
	}
	return "";
}

const char* CBasicException::what() const throw()
{
	static std::string msg;
	if(!m_CallStack.empty())
	{
		//msg = m_CallStack.front().message.utf8();
		msg = this->getExceptionString().utf8();
	}
	return msg.c_str();
}


utf8string CBasicException::getExceptionString() const
{
	if(!m_CallStack.empty())
	{
		std::wstringstream wss;
		CALLSTACK::const_reverse_iterator rit = m_CallStack.rbegin();
		for(; rit != m_CallStack.rend(); ++rit)
		{
			wss << L"========== "	<< rit->time		<< L" ==========\r\n";
			wss << L"File     :  "	<< rit->file		<< L"\r\n";
			wss << L"Line     :  "	<< rit->line		<< L"\r\n";
			wss << L"Location :  "	<< rit->function	<< L"\r\n\r\n";
			wss << L"   "			<< rit->message		<< L"\r\n";
		}
		return wss.str();
	}
	return L"";
}

void CBasicException::writeFile(vfs::Path const& sPath)
{
	try
	{
		vfs::COpenWriteFile oFile(sPath,true,true);
		utf8string s = this->getExceptionString();
		oFile.file().write(s.utf8().c_str(), s.length());
		oFile.file().close();
	}
	catch(CBasicException &ex)
	{
		CBasicException ex2(L"Could not write exception file into VFS",
			_FUNCTION_FORMAT_,__LINE__,__FILE__, &ex);
		CBasicException out("Writing exception to disc failed : is there no writable profile?",
			_FUNCTION_FORMAT_,__LINE__,__FILE__, this);

		out.m_CallStack.insert(out.m_CallStack.begin(),ex.m_CallStack.begin(),ex.m_CallStack.end());

		vfs::Path sDir,sFile;
		sPath.splitLast(sDir,sFile);
		vfs::CFile oFile(sFile);

		try
		{
			// can also fail, but there is only so much we can do
			vfs::COpenWriteFile file( vfs::tWritableFile::cast(&oFile) );
			utf8string s = out.getExceptionString();
			file.file().write(s.utf8().c_str(), s.length());
		}
		catch(CBasicException &fex)
		{
			RETHROWEXCEPTION(L"Could write exception file at all",&fex);
		}
	}
}

void logException(CBasicException const& ex)
{
	static bool is_logging = false;
	if(is_logging)
	{
		// drop message
		return;
	}
	//////////////////
	is_logging = true;
	//////////////////

	static CLog& exlog = *CLog::create(L"game_exceptions.log", false, CLog::FLUSH_IMMEDIATELY);
	try
	{
		exlog << ">>>>>>>>>>>>>>>>>>>>>" << CLog::ENDL;
		exlog << ex.getExceptionString();
		exlog << "<<<<<<<<<<<<<<<<<<<<<" << CLog::ENDL << CLog::ENDL;
	}
	catch(...)
	{
		// don't throw at all
	}

	//////////////////
	is_logging = false;
	//////////////////
}

