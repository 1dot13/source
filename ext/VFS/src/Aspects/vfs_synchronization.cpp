/* 
 * bfVFS : vfs/Aspects/vfs_synchronization.cpp
 *  - Interface for Mutexes and Locks, implementation can or has to be provided by the using program
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

#include <vfs/Aspects/vfs_synchronization.h>

static vfs::Aspects::IMutexFactory* s_mutex_factory = 0;

void vfs::Aspects::setMutexFactory(IMutexFactory* mutex_factory)
{
	s_mutex_factory = mutex_factory;
}
vfs::Aspects::IMutexFactory* vfs::Aspects::getMutexFactory()
{
	return s_mutex_factory;
}


vfs::Aspects::Mutex::Mutex() : _locked(0)
{
	if(s_mutex_factory)
	{
		_mutex = s_mutex_factory->createMutex();
	}
	else
	{
		_mutex = new IMutex();
	}
}

vfs::Aspects::Mutex::~Mutex()
{
	if(_mutex)
	{
		_mutex->unlock();
		delete _mutex;
	}
}

void vfs::Aspects::Mutex::lock()
{
	_mutex->lock();
	_locked++;
}

void vfs::Aspects::Mutex::unlock()
{
	if(_locked > 0)
	{
		_locked--;
		_mutex->unlock();
	}
}
