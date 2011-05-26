#ifndef _STCI_IMAGE_UTILS_H_
#define _STCI_IMAGE_UTILS_H_

#include "Types.h"
#include "himage.h"
#include <vfs/Core/vfs_types.h>
#include <vfs/Core/File/vfs_file.h>

#include <vector>

namespace ja2xp
{
	class CIndexedSTIImage
	{
	public:
		CIndexedSTIImage();
		~CIndexedSTIImage();
		
		bool SetPalette(SGPPaletteEntry *pPal, int iSize);
		bool AddImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y, UINT8 *original_compressed=NULL, UINT32 iriginal_compressed_size=0);

		bool AddCompressedImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y);

		bool WriteImage(vfs::tWritableFile* pFile);
		bool WriteToHIMAGE(HIMAGE pImage);
	private:
		STCIHeader _header;
		STCIPaletteElement *_palette;
		int _pal_size;
		std::vector<STCISubImage> _images;
		std::vector<UINT8*> _compressed_images;
	};


	void SplitSTIImage(vfs::String const&, vfs::String &file, vfs::String &ext);

	/**
	 *  BUG: this function gives wrong results
	 */
	BOOLEAN GetETRLEPixelValue( UINT8 * pDest, image_type *hImage, UINT16 usETRLEIndex, UINT16 usX, UINT16 usY );

	UINT32	UnpackETRLEImageToBuffer(UINT8 *pBuffer, image_type *pImage, UINT16 ueETRLEIndex); 
	UINT32	UnpackETRLEImageToRGBABuffer(UINT8 *pBuffer, image_type *pImage, UINT16 ueETRLEIndex); 

	/**
	 *  NOTE: implemented in msvc inline assembler 
	 *        does not work with gcc -> use 'UnpackETRLEImageToBuffer'
	 */
	BOOLEAN Blt8BPPDataTo8BPPBuffer( UINT8 *pBuffer, UINT32 uiDestPitchBYTES, image_type *hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex );
};
#endif // _STCI_IMAGE_UTILS_H_
