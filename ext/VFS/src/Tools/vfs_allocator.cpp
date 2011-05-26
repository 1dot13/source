/* 
 * bfVFS : vfs/Tools/vfs_allocator.cpp
 *  - allocator class to reserve memory blockwise for a larger amount of objects (that have constant size)
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

#include <vfs/Tools/vfs_allocator.h>

std::vector<vfs::IAllocator*> vfs::ObjectAllocator::_valloc;

void vfs::ObjectAllocator::registerAllocator(vfs::IAllocator* allocator)
{
	_valloc.push_back(allocator);
}
void vfs::ObjectAllocator::clear()
{
	std::vector<IAllocator*>::iterator it = _valloc.begin();
	for(; it != _valloc.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	_valloc.clear();
}
