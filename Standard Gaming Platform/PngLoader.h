#ifndef _PNGLOADER_H_
#define _PNGLOADER_H_

#include "types.h"
#include "himage.h"

bool LoadPNGFileToImage(HIMAGE hImage, UINT16 fContents);

bool LoadJPCFileToImage(HIMAGE hImage, UINT16 fContents);


#endif // _PNGLOADER_H_

