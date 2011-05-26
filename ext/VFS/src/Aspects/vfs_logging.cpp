/* 
 * bfVFS : vfs/Aspects/vfs_logging.cpp
 *  - Logging interface and macros that will be used to report errors/warnings to the using program
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

#include <vfs/Aspects/vfs_logging.h>
#include <vfs/Core/vfs_string.h>

static vfs::Aspects::ILogger* s_LogDebug	= 0;
static vfs::Aspects::ILogger* s_LogInfo		= 0;
static vfs::Aspects::ILogger* s_LogWarning	= 0;
static vfs::Aspects::ILogger* s_LogError	= 0;

void vfs::Aspects::setLogger( ILogger* info_logger, ILogger* warning_logger, ILogger* error_logger, ILogger* debug_logger )
{
	setLogger(LOG_INFO,		info_logger);
	setLogger(LOG_WARNING,	warning_logger);
	setLogger(LOG_ERROR,	error_logger);
	setLogger(LOG_DEBUG,	debug_logger);
}


void vfs::Aspects::setLogger(vfs::Aspects::LogType type, vfs::Aspects::ILogger* logger)
{
	switch (type)
	{
	case vfs::Aspects::LOG_INFO :
		s_LogInfo = logger;
		break;
	case vfs::Aspects::LOG_WARNING :
		s_LogWarning = logger;
		break;
	case vfs::Aspects::LOG_ERROR :
		s_LogError = logger;
		break;
	case vfs::Aspects::LOG_DEBUG :
		s_LogDebug = logger;
		break;
	}
}

vfs::Aspects::ILogger* vfs::Aspects::getLogger(vfs::Aspects::LogType type)
{
	switch (type)
	{
	case vfs::Aspects::LOG_DEBUG   : return s_LogDebug;
	case vfs::Aspects::LOG_INFO    : return s_LogInfo;
	case vfs::Aspects::LOG_WARNING : return s_LogWarning;
	case vfs::Aspects::LOG_ERROR   : return s_LogError;
	}
	return 0;
}

void vfs::Aspects::Debug(vfs::String const& msg)
{
	if(s_LogDebug)
	{
		s_LogDebug->Msg(msg.c_str());
	}
}
void vfs::Aspects::Debug(const wchar_t* msg)
{
	if(s_LogDebug)
	{
		s_LogDebug->Msg(msg);
	}
}
void vfs::Aspects::Debug(const char* msg)
{
	if(s_LogDebug)
	{
		s_LogDebug->Msg(msg);
	}
}


void vfs::Aspects::Info(vfs::String const& msg)
{
	if(s_LogInfo)
	{
		s_LogInfo->Msg(msg.c_str());
	}
}
void vfs::Aspects::Info(const wchar_t* msg)
{
	if(s_LogInfo)
	{
		s_LogInfo->Msg(msg);
	}
}
void vfs::Aspects::Info(const char* msg)
{
	if(s_LogInfo)
	{
		s_LogInfo->Msg(msg);
	}
}

void vfs::Aspects::Warning(vfs::String const& msg)
{
	if(s_LogWarning)
	{
		s_LogWarning->Msg(msg.c_str());
	}
}
void vfs::Aspects::Warning(const wchar_t* msg)
{
	if(s_LogWarning)
	{
		s_LogWarning->Msg(msg);
	}
}
void vfs::Aspects::Warning(const char* msg)
{
	if(s_LogWarning)
	{
		s_LogWarning->Msg(msg);
	}
}

void vfs::Aspects::Error(vfs::String const& msg)
{
	if(s_LogError)
	{
		s_LogError->Msg(msg.c_str());
	}
}
void vfs::Aspects::Error(const wchar_t* msg)
{
	if(s_LogError)
	{
		s_LogError->Msg(msg);
	}
}
void vfs::Aspects::Error(const char* msg)
{
	if(s_LogError)
	{
		s_LogError->Msg(msg);
	}
}
