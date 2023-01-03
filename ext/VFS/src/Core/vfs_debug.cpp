/* 
 * bfVFS : vfs/Core/vfs_dfebug.cpp
 *  - Exception class and throw macros, used to notify the using program of unexpected situations
 *
 * Copyright (C) 2008 - 2010 (BF) john.bf.smith@googlemail.com
 * 
 * This file is part of the bfVFS library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <vfs/Core/vfs_debug.h>
#include <vfs/Core/vfs_string.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/File/vfs_file.h>
#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Tools/vfs_log.h>

#include <sstream>
#include <ctime>

vfs::Exception::Exception(vfs::String const& text, vfs::String const& function, int line, const char* file)
: std::exception() //(text.utf8().c_str())
{
	VFS_LOG_ERROR( text.c_str() );

	time_t rawtime;
	time ( &rawtime );
	std::string datetime(ctime(&rawtime));

	SEntry en;
	en.message = text;
	en.line = line;
	en.file = file;
	en.function = function;
	VFS_IGNOREEXCEPTION(en.time = vfs::String(datetime.substr(0,datetime.length()-1)), false);
	m_CallStack.push_back(en);
};

vfs::Exception::Exception(vfs::String const& text, vfs::String const& function, int line, const char* file, std::exception& ex)
: std::exception() //(text.utf8().c_str())
{
	VFS_LOG_ERROR( text.c_str() );

	if(dynamic_cast<vfs::Exception*>(&ex))
	{
		vfs::Exception& vfs_ex = *static_cast<vfs::Exception*>(&ex);
		m_CallStack.insert(m_CallStack.end(), vfs_ex.m_CallStack.begin(), vfs_ex.m_CallStack.end());
	}
	else
	{
		SEntry en;
		en.line			= -1;
		en.file			= "";
		en.function		= "";
		en.time			= "";
		VFS_IGNOREEXCEPTION( en.message	= vfs::String(ex.what()), false );

		m_CallStack.push_back(en);
	}

	time_t rawtime;
	time ( &rawtime );
	std::string datetime(ctime(&rawtime));

	SEntry en;
	en.message = text;
	en.line = line;
	en.file = file;
	en.function = function;
	VFS_IGNOREEXCEPTION(en.time = vfs::String(datetime.substr(0,datetime.length()-1)), false);

	m_CallStack.push_back(en);
};




vfs::Exception::~Exception() throw()
{
}

vfs::String vfs::Exception::getLastEntryString() const
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

const char* vfs::Exception::what() const throw()
{
	static std::string msg;
	msg = "";
	if(!m_CallStack.empty())
	{
		//msg = m_CallStack.front().message.utf8();
		msg = this->getExceptionString().utf8();
	}
	return msg.c_str();
}


vfs::String vfs::Exception::getExceptionString() const
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
//
//void vfs::Exception::writeFile(vfs::Path const& sPath)
//{
//	try
//	{
//		vfs::COpenWriteFile oFile(sPath,true,true);
//		vfs::String s = this->getExceptionString();
//		oFile->write(s.utf8().c_str(), s.length());
//		oFile->close();
//	}
//	catch(vfs::Exception &ex)
//	{
//		vfs::Exception ex2(L"Could not write exception file into VFS",
//			_FUNCTION_FORMAT_,__LINE__,__FILE__, ex);
//		vfs::Exception out("Writing exception to disc failed : is there no writable profile?",
//			_FUNCTION_FORMAT_,__LINE__,__FILE__, *this);
//
//		out.m_CallStack.insert(out.m_CallStack.begin(), ex.m_CallStack.begin(), ex.m_CallStack.end());
//
//		vfs::Path sDir,sFile;
//		sPath.splitLast(sDir,sFile);
//		vfs::CFile oFile(sFile);
//
//		try
//		{
//			// can also fail, but there is only so much we can do
//			vfs::COpenWriteFile file( vfs::tWritableFile::cast(&oFile) );
//			vfs::String s = out.getExceptionString();
//			file->write(s.utf8().c_str(), s.length());
//		}
//		catch(std::exception &fex)
//		{
//			VFS_RETHROW(L"Could write exception file at all",fex);
//		}
//	}
//}
//

//
//void logException(vfs::Exception const& ex)
//{
//	static bool is_logging = false;
//	if(is_logging)
//	{
//		// drop message
//		return;
//	}
//	//////////////////
//	is_logging = true;
//	//////////////////
//
//	static vfs::Log& exlog = *vfs::Log::create(L"game_exceptions.log", false, vfs::Log::FLUSH_IMMEDIATELY);
//	try
//	{
//		exlog << ">>>>>>>>>>>>>>>>>>>>>" << vfs::Log::endl;
//		exlog << ex.getExceptionString();
//		exlog << "<<<<<<<<<<<<<<<<<<<<<" << vfs::Log::endl << vfs::Log::endl;
//	}
//	catch(...)
//	{
//		// don't throw at all
//	}
//
//	//////////////////
//	is_logging = false;
//	//////////////////
//}
//
//void logException(std::exception const& ex)
//{
//	logException(ex.what());
//}
//
//void logException(const wchar_t* ex)
//{
//	logException(vfs::String::as_utf8(ex).c_str());
//}
//
//void logException(const char* ex)
//{
//	static bool is_logging = false;
//	if(is_logging)
//	{
//		// drop message
//		return;
//	}
//	//////////////////
//	is_logging = true;
//	//////////////////
//
//	static vfs::Log& exlog = *vfs::Log::create(L"game_exceptions.log", false, vfs::Log::FLUSH_IMMEDIATELY);
//	try
//	{
//		exlog << ">>>>>>>>>>>>>>>>>>>>>" << vfs::Log::endl;
//		exlog << ex;
//		exlog << "<<<<<<<<<<<<<<<<<<<<<" << vfs::Log::endl << vfs::Log::endl;
//	}
//	catch(...)
//	{
//		// don't throw at all
//	}
//
//	//////////////////
//	is_logging = false;
//	//////////////////}
//}
