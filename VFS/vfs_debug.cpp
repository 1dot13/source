#include "vfs_debug.h"

#include "utf8string.h"
#include "PropertyContainer.h"
#include "vfs_file_raii.h"
#include "FILE/vfs_file.h"

#include <sstream>
#include <ctime>

CBasicException::CBasicException(const wchar_t* text, const char* function, int line, const char* file, CBasicException* ex)
: std::exception(utf8string::as_utf8(text).c_str())
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
: std::exception(text.utf8().c_str())
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

utf8string CBasicException::GetLastEntryString() const
{
	if(!m_CallStack.empty())
	{
		CALLSTACK::const_reverse_iterator rit = m_CallStack.rbegin();
		std::wstringstream ss;
		ss << rit->file.c_wcs() << L" (l. " << rit->line<< ") : [" << rit->function.c_wcs() << L"] - " << rit->message.c_wcs();
		return ss.str();
	}
	return "";
}

utf8string CBasicException::GetExceptionString() const
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
			wss << L"   "			<< rit->message		<< L"\r\n\r\n";
		}
		return wss.str();
	}
	return L"";
}

void CBasicException::WriteFile(vfs::Path const& sPath)
{
	try
	{
		vfs::COpenWriteFile oFile(sPath,true,true);
		utf8string s = this->GetExceptionString();
		vfs::UInt32 written;
		oFile.file().Write(s.utf8().c_str(),s.length(),written);
		oFile.file().Close();
	}
	catch(CBasicException &ex)
	{
		CBasicException ex2(L"Could not write exception file into VFS",
			_FUNCTION_FORMAT_,__LINE__,__FILE__, &ex);
		CBasicException out("Writing exception to disc failed : is there no writeable profile?",
			_FUNCTION_FORMAT_,__LINE__,__FILE__, this);

		out.m_CallStack.insert(out.m_CallStack.begin(),ex.m_CallStack.begin(),ex.m_CallStack.end());

		vfs::Path sDir,sFile;
		sPath.SplitLast(sDir,sFile);
		vfs::CFile oFile(sFile);

		try
		{
			// can also fail, but there is only so much we can do
			vfs::COpenWriteFile file( vfs::tWriteableFile::Cast(&oFile) );
			utf8string s = out.GetExceptionString();
			vfs::UInt32 written;
			file.file().Write(s.utf8().c_str(),s.length(),written);
		}
		catch(CBasicException &fex)
		{
			RETHROWEXCEPTION(L"Could write exception file at all",&fex);
		}
	}
}

void LogException(CBasicException const& ex)
{
	static CLog& exlog = *CLog::Create(L"game_exceptions.log");
	try
	{
		exlog << ">>>>>>>>>>>>>>>>>>>>>" << CLog::endl;
		exlog << ex.GetExceptionString();
		exlog << "<<<<<<<<<<<<<<<<<<<<<" << CLog::endl << CLog::endl;
	}
	catch(...)
	{
		// don't throw at all
	}
}

