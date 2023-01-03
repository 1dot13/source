/* 
 * bfVFS : vfs/Aspects/vfs_settings.h
 *  - library runtime settings,
 *  - e.g., to use Unicode strings or regular for file names
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

#ifndef _VFS_SETTIGNS_H_
#define _VFS_SETTIGNS_H_

#include <vfs/vfs_config.h>

namespace vfs
{
	class VFS_API Settings
	{
	public:
		static void setUseUnicode(bool useUnicode);
		static bool getUseUnicode();
	};
}

#endif // _VFS_SETTIGNS_H_
