/* 
 * bfVFS : vfs/Aspects/vfs_settings.cpp
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

#include <vfs/Aspects/vfs_settings.h>

static bool s_UseUnicode = true;

void vfs::Settings::setUseUnicode(bool useUnicode)
{
	s_UseUnicode = useUnicode;
}

bool vfs::Settings::getUseUnicode()
{
	return s_UseUnicode;
}
