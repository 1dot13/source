/* 
 * bfVFS : vfs/Aspects/vfs_synchronization.h
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

#ifndef VFS_SYNCHRONIZATION_H
#define VFS_SYNCHRONIZATION_H

#include <vfs/vfs_config.h>

namespace vfs
{
	namespace Aspects
	{
		class IMutex
		{
		public:
			virtual void lock() {};
			virtual void unlock() {};
		};

		class IMutexFactory
		{
		public:
			virtual IMutex* createMutex() = 0;
		};

		VFS_API void setMutexFactory(IMutexFactory* mutex_factory);
		VFS_API IMutexFactory* getMutexFactory();

		class Mutex
		{
		public:
			Mutex();
			~Mutex();

			void lock();
			void unlock();
		private:
			int		_locked;
			IMutex* _mutex;
		};

		class ScopedLock
		{
		public:
			ScopedLock(Mutex& mutex) : _mutex(mutex) {
				this->_mutex.lock();
			}
			~ScopedLock(){
				this->_mutex.unlock();
			}
		private:
			Mutex&	_mutex;
		};
	}
}

#ifdef VFS_SYNCHRONIZE
#	if !defined VFS_LOCK
#		define VFS_LOCK(mutex) vfs::Aspects::ScopedLock _scoped_lock(mutex)
#	endif
#else
#	if !defined VFS_LOCK
#		define VFS_LOCK(mutex) 
#	endif
#endif

#endif // VFS_SYNCHRONIZATION_H
