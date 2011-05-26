#include "Types.h"
#include "himage.h"

#include <vfs/Core/File/vfs_file.h>

namespace ja2xp
{
	BOOLEAN LoadSTCIFileToImage(vfs::tReadableFile *pFile, HIMAGE hImage, UINT16 fContents );

	BOOLEAN IsSTCIETRLEFile(vfs::tReadableFile *pFile, CHAR8 * ImageFile );
};
