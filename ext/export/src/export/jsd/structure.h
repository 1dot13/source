#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <vfs/Core/Interface/vfs_file_interface.h>

namespace ja2xp
{
	bool ConvertStructure(vfs::tReadableFile* pStructureFile, vfs::tWritableFile* pOutputFile);
};

#endif // _STRUCTURE_H_
