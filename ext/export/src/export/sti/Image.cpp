
#include "Image.h"
#include <sgp_auto_memory.h>

#include "STCI_lib.h"
#include "stci_image_utils.h"
#include "XMLWriter.h"

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_debug.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/File/vfs_buffer_file.h>
#include <vfs/Ext/7z/vfs_create_7z_library.h>

#include <sstream>

namespace png
{
#include <png.h>
}
#include <cassert>
#include <iostream>

//#include "load_png.h"

#include <windows.h>


template<typename FlagType>
inline bool _FLAG(FlagType &flags, FlagType const& flag)
{
	if(flags & flag)
	{
		flags -= flag;
		return true;
	}
	return false;
}

//template<typename CharType, typename FlagType>
//inline void WriteFlags(XMLWriter<CharType> &xmlw, std::string const& sNodeName, FlagType flags)
template<typename FlagType>
inline void WriteFlags(XMLWriter &xmlw, std::string const& sNodeName, FlagType flags)
{
	xmlw.addAttributeToNextValue("value",(int)flags);
	xmlw.openNode(sNodeName);

	/*0x01*/ if(_FLAG(flags, (FlagType)AUX_FULL_TILE))			xmlw.addValue("AUX_FULL_TILE");				
	/*0x02*/ if(_FLAG(flags, (FlagType)AUX_ANIMATED_TILE))		xmlw.addValue("AUX_ANIMATED_TILE");
	/*0x04*/ if(_FLAG(flags, (FlagType)AUX_DYNAMIC_TILE))		xmlw.addValue("AUX_DYNAMIC_TILE");
	/*0x08*/ if(_FLAG(flags, (FlagType)AUX_INTERACTIVE_TILE))	xmlw.addValue("AUX_INTERACTIVE_TILE");
	/*0x10*/ if(_FLAG(flags, (FlagType)AUX_IGNORES_HEIGHT))		xmlw.addValue("AUX_IGNORES_HEIGHT");
	/*0x20*/ if(_FLAG(flags, (FlagType)AUX_USES_LAND_Z))		xmlw.addValue("AUX_USES_LAND_Z");

	if(flags != 0)
	{
		xmlw.addValue("UNKNOWN_FLAG","UNKNOWN_FLAG");
	}
	xmlw.closeNode();
}


/***********************
 *  from : http://www.gamedev.net/reference/articles/article1966.asp
 *
//File information header
//provides general information about the file
typedef struct tagBITMAPFILEHEADER { 
	unsigned short		bfType; 
	unsigned int		bfSize; 
	unsigned short		bfReserved1; 
	unsigned short		bfReserved2; 
	unsigned int		bfOffBits; 
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
/*typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;*
//Bitmap information header
//provides information specific to the image data
typedef struct tagBITMAPINFOHEADER{
	unsigned int	biSize; 
	long			biWidth; 
	long			biHeight; 
	unsigned short  biPlanes; 
	unsigned short  biBitCount; 
	unsigned int	biCompression; 
	unsigned int	biSizeImage; 
	long			biXPelsPerMeter; 
	long			biYPelsPerMeter; 
	unsigned int	biClrUsed; 
	unsigned int	biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
/*typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;*

//Colour palette
typedef struct tagRGBQUAD {
	unsigned char    rgbBlue; 
	unsigned char    rgbGreen; 
	unsigned char    rgbRed; 
	unsigned char    rgbReserved; 
} RGBQUAD; */

/******************************************************************************************/



ja2xp::CImage::CImage(vfs::Path const& sFileName) 
: m_eImageType(IT_NONE), m_pRawImage(NULL), m_bImageLoaded(false) 
{
	m_pImageFile = getVFS()->getReadFile(sFileName);
	if(!m_pImageFile )
	{
		int throw_exception = 0;
	}
};

ja2xp::CImage::CImage(vfs::tReadableFile *pFile) 
: m_pImageFile(pFile), m_eImageType(IT_NONE), m_pRawImage(NULL), m_bImageLoaded(false) 
{
};


ja2xp::CImage::~CImage()
{
	if(m_pRawImage)
	{
		if(m_pRawImage->pPalette)
		{
			delete[] m_pRawImage->pPalette;
		}
		m_pRawImage->pPalette = NULL;
		if(m_pRawImage->pETRLEObject)
		{
			delete[] m_pRawImage->pETRLEObject;
		}
		m_pRawImage->pETRLEObject = NULL;
		if(m_pRawImage->pImageData)
		{
			delete[] m_pRawImage->pImageData;
		}
		m_pRawImage->pImageData = NULL;
		if(m_pRawImage->pAppData)
		{
			delete[] m_pRawImage->pAppData;
		}
		m_pRawImage->pAppData = NULL;

		delete m_pRawImage;
	}
	m_pRawImage = NULL;
}

UINT32 ja2xp::CImage::GetNumberOfSubImages()
{
	if(m_pRawImage)
	{
		if(m_pRawImage->pETRLEObject)
		{
			return m_pRawImage->usNumberOfObjects;
		}
		return 1;
	}
	return 0;
}
	
bool ja2xp::CImage::LoadData()
{
	if(m_bImageLoaded && m_pRawImage == NULL)
	{
		return false;
	}
	if(m_pRawImage == NULL) // m_bImageLoaded == false
	{
		if(m_pImageFile == NULL)
		{
			return false;
		}
		m_pRawImage = new image_type;
		memset(m_pRawImage,0,sizeof(image_type));
		UINT32 uiImageContent = IMAGE_ALLIMAGEDATA; // use it when loading images from disc

		vfs::Path const& sFileName = m_pImageFile->getName();

		vfs::String::str_t const& filename = sFileName();
		vfs::String::str_t sExt = filename.substr(filename.length()-4,4);
		if(vfs::StrCmp::Equal(sExt,L".sti"))
		{
			if(LoadSTCIFileToImage(m_pImageFile, m_pRawImage, uiImageContent))
			{
				if(m_pRawImage->pPalette)
				{
					m_eImageType = IT_COMPRESSED;
//					std::cout << "compressed ETRLE image" << std::endl;
				}
				else
				{
					m_eImageType = IT_BITMAP;
//					std::cout << "bitmap image [" << m_pRawImage->usWidth  << "," << m_pRawImage->usHeight << "]" << std::endl;
				}
			}
			else
			{
				std::cout << "error : could not load image \"" << vfs::String::as_utf8(sFileName()) << "\"" << std::endl;
			}
		}
		else if (vfs::StrCmp::Equal(sExt, L".bmp"))
		{
			LoadBMP();
		}
		m_bImageLoaded = true;
	}
	return true;
}

bool ja2xp::CImage::DeleteData()
{
	if( m_bImageLoaded &&  !m_pRawImage )
	{
		return false;
	}
	if(m_pRawImage)
	{
		delete m_pRawImage;
		m_eImageType = IT_NONE;
	}
	return true;
}

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

bool ja2xp::CImage::GetSize(UINT32 uiSubImage, UINT32 &uiWidth, UINT32 &uiHeight)
{
	if(m_eImageType == IT_BITMAP)
	{
		uiWidth = m_pRawImage->usWidth;
		uiHeight = m_pRawImage->usHeight;
	}
	else if(m_eImageType == IT_COMPRESSED)
	{
		if(uiSubImage >= m_pRawImage->usNumberOfObjects)
		{
			return false;
		}
		uiWidth = m_pRawImage->pETRLEObject[uiSubImage].usWidth;
		uiHeight = m_pRawImage->pETRLEObject[uiSubImage].usHeight;
	}
	else
	{
		return false;
	}
	return true;
}

bool ja2xp::CImage::GetPosition(UINT32 uiSubImage, INT32 &iX, INT32 &iY)
{
	if(m_eImageType == IT_BITMAP)
	{
		iX = 0;
		iY = 0;
	}
	else if(m_eImageType == IT_COMPRESSED)
	{
		if(uiSubImage >= m_pRawImage->usNumberOfObjects)
		{
			return false;
		}
		iX = m_pRawImage->pETRLEObject[uiSubImage].sOffsetX;
		iY = m_pRawImage->pETRLEObject[uiSubImage].sOffsetY;
	}
	else
	{
		return false;
	}
	return true;
}
		
void* ja2xp::CImage::GetDataPointer(UINT32 &uiSize)
{
	if(m_eImageType == IT_BITMAP)
	{
		uiSize = m_pRawImage->uiSizePixData;
		return m_pRawImage->p8BPPData;
	}
	uiSize = 0;
	return NULL;
}

bool ja2xp::CImage::GetRawDataOfSubImage(UINT32 uiSubImage, void* pData, UINT32 uiMaxSize, UINT32 &uiFilledSize)
{
	int size = min(uiMaxSize,m_pRawImage->pETRLEObject[uiSubImage].uiDataLength);
	memcpy(pData, &(m_pRawImage->p8BPPData[m_pRawImage->pETRLEObject[uiSubImage].uiDataOffset]),size);
	return true;
}

UINT32 ja2xp::CImage::FillData(UINT32 uiSubImage, UINT8* pData, UINT32 uiMaxSize, bool rgba)
{
	if(m_eImageType == IT_BITMAP)
	{
		if( (uiSubImage == 0) && (m_pRawImage->uiSizePixData < uiMaxSize) )
		{
			memcpy(pData,m_pRawImage->p8BPPData,m_pRawImage->uiSizePixData);
			return m_pRawImage->uiSizePixData;
		}
	}
	else if(m_eImageType == IT_COMPRESSED)
	{
		if( (uiSubImage < m_pRawImage->usNumberOfObjects) )//&& 
			//(uiMaxSize > m_pRawImage->pETRLEObject[uiSubImage].uiDataLength) )
		{
			UINT32 written = 0;
			if(rgba)
			{
				written = UnpackETRLEImageToRGBABuffer(pData, m_pRawImage, uiSubImage);
			}
			else
			{
				written = UnpackETRLEImageToBuffer((UINT8*)pData,m_pRawImage,uiSubImage);
			}
			// TODO : 
			return written;
		}
	}
	return 0;
}
bool ja2xp::CImage::Unpack16BppImageToRGBBuffer(UINT16* pSrc, UINT8* pDst, UINT32 uiWidth, UINT32 uiHeight)
{
	if(!pSrc || !pDst )
	{
		return false;
	}
	UINT32 uiSize = uiWidth * uiHeight;
	for(UINT32 i=0; i < uiSize; ++i)
	{
		UINT16 value = *pSrc;
		UINT8 red = (value & gusRedMask) >> gusRedShift;
		UINT8 green = (value & gusGreenMask) >> gusGreenShift;
		UINT8 blue = (value & gusBlueMask) << (-gusBlueShift);
		*pDst++ = red;
		*pDst++ = green;
		*pDst++ = blue;
		pSrc++;
	}
	return true;
}


ja2xp::CImage::EImageType ja2xp::CImage::GetImageType()
{
	return m_eImageType;
}

SGPPaletteEntry* ja2xp::CImage::GetPalette()
{
	if(m_pRawImage)
	{
		return m_pRawImage->pPalette;
	}
	return NULL;
}


/***************************************************************************************/

vfs::Path CreateFileName(vfs::Path const& sOutPathName, vfs::Path const& sFileName, int current_image, int num_images, vfs::String::str_t sExt)
{
	// create_file
	vfs::Path outPath = sOutPathName;
	std::wstringstream fullpath;
	int test = num_images;
	int left=0,right=0;
	while(test > 0){
		left++;
		test /= 10;
	}
	test = current_image==0 ? 1 : current_image;
	while(test > 0){
		right++;
		test /= 10;
	}
	test = left-right;
	while(test > 0 )
	{
		fullpath << L"0";
		test--;
	}
	fullpath << current_image << L"." << sExt;
	outPath += vfs::Path(fullpath.str());
	return outPath;
}
#if 0
bool CImage::WriteAsBMPs(vfs_string sOutPathName)
{
	if(!this->LoadData())
	{
		return false;
	}

	vfs_string sFileName,path;

	//SplitPath(m_pImageFile->GetFileName(),path,sFileName);
	SplitPath(m_pImageFile->GetFileName()(),path,sFileName);
	
	for(int i=0; i<m_pRawImage->usNumberOfObjects; ++i)
	{
		ETRLEObject &image = m_pRawImage->pETRLEObject[i];

		BITMAPINFOHEADER bmp_infoheader;
		bmp_infoheader.biSize = 40;//sizeof(BITMAPFILEHEADER);
		bmp_infoheader.biWidth = image.usWidth;
		bmp_infoheader.biHeight = image.usHeight;
		bmp_infoheader.biPlanes = 1;
		bmp_infoheader.biBitCount = 8;
		bmp_infoheader.biCompression = 0;
		bmp_infoheader.biSizeImage = image.usWidth * image.usHeight * sizeof(UINT8);
		bmp_infoheader.biXPelsPerMeter = 0;
		bmp_infoheader.biYPelsPerMeter = 0;
		bmp_infoheader.biClrUsed = 256;
		bmp_infoheader.biClrImportant = 256;

		UINT32 scanline_size = image.usWidth + (4 - image.usWidth%4)%4;

		// create headers for our bmp file
		BITMAPFILEHEADER bmp_header;
		bmp_header.bfType = 19778;
		bmp_header.bfReserved1 = 0;
		bmp_header.bfReserved2 = 0;
		bmp_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD); // headers + pallette
		//bmp_header.bfSize = bmp_header.bfOffBits + (bmp_infoheader.biWidth * bmp_infoheader.biHeight);
		bmp_header.bfSize = bmp_header.bfOffBits + (scanline_size * bmp_infoheader.biHeight);

		UINT8 *data = new UINT8[bmp_infoheader.biWidth * bmp_infoheader.biHeight];
		UINT32 imagesize;
		if(!this->FillData(i,data,bmp_infoheader.biSizeImage*sizeof(UINT8),imagesize))
		{
			return false;
		}
		if(imagesize != bmp_infoheader.biSizeImage)
		{
			return false;
		}

		ChompSlash(sOutPathName);
		vfs_string filename = CreateFileName(sOutPathName, sFileName, i, m_pRawImage->usNumberOfObjects, L"bmp");

		VFS::tReadableFile *pTempFile =  GetVFS()->GetFile(filename);
		int x;
		if(!pTempFile)
		{
			if(!GetVFS()->CreateNewFile(filename))
			{
				return false;
			}
			pTempFile = GetVFS()->GetFile(filename);
		}
		if(pTempFile && pTempFile->IsWriteable())
		{
			VFS::tWriteableFile *pWriteFile = VFS::tWriteableFile::Cast(pTempFile->GetWriteable());
			if(!pWriteFile || !pWriteFile->OpenWrite(true))
			{
				return false;
			}
			UINT32 written_bytes;
			// file header
			if(!pWriteFile->Write(&bmp_header,sizeof(BITMAPFILEHEADER),&written_bytes) || 
				written_bytes != sizeof(BITMAPFILEHEADER) )
			{
				pWriteFile->Close();
				return false;
			}
			// info header
			if(!pWriteFile->Write(&bmp_infoheader,sizeof(BITMAPINFOHEADER),&written_bytes) || 
				written_bytes != sizeof(BITMAPINFOHEADER) )
			{
				pWriteFile->Close();
				return false;
			}
			// palette
			SGPPaletteEntry *pal = GetPalette();
			RGBQUAD palette[256];
			for(int i=0;i<256;++i)
			{
				palette[i].rgbBlue = pal[i].peBlue;
				palette[i].rgbGreen= pal[i].peGreen;
				palette[i].rgbRed  = pal[i].peRed;
				palette[i].rgbReserved = pal[i].peFlags;
			}
			if(!pWriteFile->Write(palette,sizeof(RGBQUAD)*256,&written_bytes) || 
				written_bytes != sizeof(RGBQUAD)*256 )
			{
				pWriteFile->Close();
				return false;
			}
			// write data scanlines
			UINT8 *scanline = new UINT8[scanline_size];
			memset(scanline,0,scanline_size);
			for(int j=bmp_infoheader.biHeight-1; j>=0; --j)
			{
				memcpy(scanline,&data[j*bmp_infoheader.biWidth],bmp_infoheader.biWidth);
				if(!pWriteFile->Write(scanline,scanline_size,&written_bytes) || 
					written_bytes != scanline_size )
				{
					pWriteFile->Close();
					return false;
				}
			}
			delete[] scanline;
			pWriteFile->Close();
		}
		delete[] data; 
	}

	return true;
}
#endif
vfs::tWritableFile* GetWriteFile(vfs::Path const& sFileName)
{
	vfs::tReadableFile *pRFile = getVFS()->getReadFile(sFileName);
	if(!pRFile)
	{
		if(!getVFS()->createNewFile(sFileName))
		{
			return NULL;
		}
		pRFile = getVFS()->getReadFile(sFileName);
	}
	if(pRFile->implementsWritable())
	{
		//return vfs::tWritableFile::cast(pRFile->getWritable());
		return vfs::tWritableFile::cast(pRFile);
	}
	return NULL;
}

/***************************************************************************/
void user_write_data(png::png_structp png_ptr, png::png_bytep data, png::png_size_t length)
{
	vfs::tWritableFile* pFile = (vfs::tWritableFile*)(png_ptr->io_ptr);
	if(pFile)
	{
		pFile->write((vfs::Byte*)data,length);
	}
}
void user_flush_data(png::png_structp png_ptr)
{
	return;
}
/***************************************************************************/

bool ja2xp::CImage::writeSubImageToPNGFile(int i, vfs::tWritableFile* file, bool rgba)
{
	VFS_THROW_IFF(i >= 0 && i < m_pRawImage->usNumberOfObjects, L"invalid index");
	ETRLEObject &image = m_pRawImage->pETRLEObject[i];

	png::png_error_ptr user_error_ptr  = NULL;
	png::png_error_ptr user_error_fn   = NULL;
	png::png_error_ptr user_warning_fn = NULL;

	png::png_structp png_ptr = png_create_write_struct( 
		PNG_LIBPNG_VER_STRING,
		(png::png_voidp)user_error_ptr,
		user_error_fn, 
		user_warning_fn );
	if (!png_ptr)
	{
		return (false);
	}

	png::png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png::png_destroy_write_struct(&png_ptr, (png::png_infopp)NULL);
		return (false);
	}

	/* set the zlib compression level */
	png::png_set_compression_level(png_ptr, Z_DEFAULT_COMPRESSION);

	/* set other zlib parameters */
	png::png_set_compression_mem_level(png_ptr, 8);
	png::png_set_compression_strategy(png_ptr, Z_DEFAULT_STRATEGY);
	png::png_set_compression_window_bits(png_ptr, 15);
	png::png_set_compression_method(png_ptr, 8);
	png::png_set_compression_buffer_size(png_ptr, 8192);

//			std::cout << "width : " << image.usWidth << ", height : " << image.usHeight << std::endl;
	int bit_depth  = rgba ? 8 : m_pRawImage->ubBitDepth;
	int color_type = rgba ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_PALETTE;
	png_set_IHDR(
		png_ptr,
		info_ptr,
		image.usWidth,				// - holds the width of the image in pixels (up to 2^31).
		image.usHeight,				//   holds the height of the image in pixels (up to 2^31).
		bit_depth,					// - holds the bit depth of one of the image channels.
									//   (valid values are 1, 2, 4, 8, 16 and depend also on the color_type.  
									//   See also significant bits (sBIT) below).
		color_type,					// - describes which color/alpha channels are present.
									//   PNG_COLOR_TYPE_GRAY (bit depths 1, 2, 4, 8, 16)
									//   PNG_COLOR_TYPE_GRAY_ALPHA (bit depths 8, 16)
									//   PNG_COLOR_TYPE_PALETTE (bit depths 1, 2, 4, 8)
									//   PNG_COLOR_TYPE_RGB (bit_depths 8, 16)
									//   PNG_COLOR_TYPE_RGB_ALPHA (bit_depths 8, 16)
									//   PNG_COLOR_MASK_PALETTE
									//   PNG_COLOR_MASK_COLOR
									//   PNG_COLOR_MASK_ALPHA
		PNG_INTERLACE_NONE,			// - PNG_INTERLACE_NONE or PNG_INTERLACE_ADAM7
		PNG_COMPRESSION_TYPE_DEFAULT,//- (must be PNG_COMPRESSION_TYPE_DEFAULT)
		PNG_FILTER_TYPE_DEFAULT 	// - (must be PNG_FILTER_TYPE_DEFAULT or, if you are writing a PNG to
									//   be embedded in a MNG datastream, can also be PNG_INTRAPIXEL_DIFFERENCING)
	);

	if(!rgba)
	{
		const UINT32 PALETTE_SIZE = 256;
		png::png_color pngpal[PALETTE_SIZE];
		SGPPaletteEntry *stipal = m_pRawImage->pPalette;
		for(int p=0; p < PALETTE_SIZE; ++p)
		{
			pngpal[p].red   = stipal[p].peRed;
			pngpal[p].green = stipal[p].peGreen;
			pngpal[p].blue  = stipal[p].peBlue;
		}
		png_set_PLTE(png_ptr, info_ptr, 
			pngpal,						// the palette for the file (array of png_color)
			PALETTE_SIZE				// number of entries in the palette
		);
	}

	//png_set_gAMA(png_ptr, info_ptr, 
	//	gamma						// - the gamma the image was created at (PNG_INFO_gAMA)
	//);

	//png_set_sRGB(png_ptr, info_ptr, 
	//	srgb_intent					// - the rendering intent (PNG_INFO_sRGB) The presence of
	//								//   the sRGB chunk means that the pixel data is in the sRGB 
	//								//   color space. This chunk also implies specific values 
	//								//   of gAMA and cHRM.  Rendering intent is the CSS-1 property 
	//								//   that has been defined by the International Color Consortium
	//								//   (http://www.color.org/).
	//								// 	 It can be one of PNG_sRGB_INTENT_SATURATION,
	//								//   PNG_sRGB_INTENT_PERCEPTUAL, PNG_sRGB_INTENT_ABSOLUTE, or
	//								//   PNG_sRGB_INTENT_RELATIVE.

	//);

	//png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr,
	//   srgb_intent					// - the rendering intent (PNG_INFO_sRGB) The presence of the
	//								//   sRGB chunk means that the pixel data is in the sRGB color 
	//								//   space. This function also causes gAMA and cHRM chunks with 
	//								//   the specific values that are consistent with sRGB to be written.
	//);

	//png_set_iCCP(png_ptr, info_ptr, 
	//	name,						// - The profile name.
	//	compression_type,			// - The compression type; always PNG_COMPRESSION_TYPE_BASE for PNG 1.0.
	//								//   You may give NULL to this argument to gnore it.
	//	profile,					// - International Color Consortium color profile data. May contain NULs.
	//	proflen						// - length of profile data in bytes.
	//);

	//png_set_sBIT(png_ptr, info_ptr, 
	//	sig_bit						// - the number of significant bits for (PNG_INFO_sBIT) each of the 
	//								//   gray, red, green, and blue channels, whichever are appropriate 
	//								//   for the given color type (png_color_16)
	//);

	//png_set_tRNS(png_ptr, info_ptr, 
	//	trans,						// - array of transparent entries for palette (PNG_INFO_tRNS)
	//	num_trans,					// - graylevel or color sample values of the single transparent 
	//								//   color for non-paletted images (PNG_INFO_tRNS)
	//	trans_values				// - number of transparent entries (PNG_INFO_tRNS)
	//);

	//png_set_hIST(png_ptr, info_ptr, 
	//	hist						// - histogram of palette (array of png_uint_16)
	//);

	//png_set_tIME(png_ptr, info_ptr, 
	//	mod_time					// - time image was last modified (PNG_VALID_tIME)
	//);

	//png_set_bKGD(png_ptr, info_ptr, 
	//	background					// - background color (PNG_VALID_bKGD)
	//);

	//png_set_text(png_ptr, info_ptr, 
	//	text_ptr,					// - array of png_text holding image comments
	//	num_text					// - number of comments
	//);
	/*****
		text_ptr[i].compression		// - type of compression used on "text" PNG_TEXT_COMPRESSION_NONE
									//   PNG_TEXT_COMPRESSION_zTXt 
									//   PNG_ITXT_COMPRESSION_NONE
									//	 PNG_ITXT_COMPRESSION_zTXt
		text_ptr[i].key				// - keyword for comment.  Must contain 1-79 characters.
		text_ptr[i].text			// - text comments for current keyword.  Can be NULL or empty.
		text_ptr[i].text_length		// - length of text string, after decompression, 0 for iTXt
		text_ptr[i].itxt_length		// - length of itxt string, after decompression, 0 for tEXt/zTXt
		text_ptr[i].lang			// - language of comment (NULL or empty for unknown).
		text_ptr[i].translated_keyword
									// - keyword in UTF-8 (NULL or empty for unknown).
	*****/

	//png_set_sPLT(png_ptr, info_ptr, 
	//	&palette_ptr,				// - array of png_sPLT_struct structures to be added to the list 
	//								//   of palettes in the info structure.
	//	num_spalettes				// - number of palette structures to be added.
	//);

	png_set_oFFs(png_ptr, info_ptr, 
		image.sOffsetX,				// - positive offset from the left edge of the screen
		image.sOffsetY,				// - positive offset from the top edge of the screen
		PNG_OFFSET_PIXEL			// - PNG_OFFSET_PIXEL, PNG_OFFSET_MICROMETER
	);

	//png_set_pHYs(png_ptr, info_ptr, 
	//	res_x,						// - pixels/unit physical resolution in x direction
	//	res_y,						// - pixels/unit physical resolution in y direction
	//	unit_type					// - PNG_RESOLUTION_UNKNOWN, PNG_RESOLUTION_METER
	//);

	//png_set_sCAL(png_ptr, info_ptr, 
	//	unit,						// - physical scale units (an integer)
	//	width,						// - width of a pixel in physical scale units
	//	height						// - height of a pixel in physical scale units (width and height are doubles)
	//);

	//png_set_sCAL_s(png_ptr, info_ptr, 
	//	unit,						// - physical scale units (an integer)
	//	width,						// - width of a pixel in physical scale units 
	//	height						// - height of a pixel in physical scale units (width and height are strings like "2.54")
	//);

	//png_set_unknown_chunks(png_ptr, info_ptr, 
	//	&unknowns,					// - array of png_unknown_chunk structures holding unknown chunks
	//	num_unknowns
	//);
	/*****
		unknowns[i].name			// - name of unknown chunk
		unknowns[i].data			// - data of unknown chunk
		unknowns[i].size			// - size of unknown chunk's data
		unknowns[i].location		// - position to write chunk in file
									//   0: do not write chunk
									//   PNG_HAVE_IHDR: before PLTE
									//   PNG_HAVE_PLTE: before IDAT
									//   PNG_AFTER_IDAT: after IDAT
	*****/

	/*****
		PNG_TRANSFORM_IDENTITY		   - No transformation
		PNG_TRANSFORM_PACKING		   - Pack 1, 2 and 4-bit samples
		PNG_TRANSFORM_PACKSWAP		   - Change order of packed pixels to LSB first
		PNG_TRANSFORM_INVERT_MONO      - Invert monochrome images
		PNG_TRANSFORM_SHIFT            - Normalize pixels to the sBIT depth
		PNG_TRANSFORM_BGR              - Flip RGB to BGR, RGBA to BGRA
		PNG_TRANSFORM_SWAP_ALPHA       - Flip RGBA to ARGB or GA to AG
		PNG_TRANSFORM_INVERT_ALPHA     - Change alpha from opacity to transparency
		PNG_TRANSFORM_SWAP_ENDIAN      - Byte-swap 16-bit samples PNG_TRANSFORM_STRIP_FILLER  Strip out filler bytes.
	*****/
	UINT32 png_transforms = PNG_TRANSFORM_IDENTITY;
	
	UINT32 num_values = image.usWidth * image.usHeight * (rgba ? 4 : 1);
	sgp::AutoArray<UINT8> image_data(num_values);
	UINT32 fill_size = num_values * sizeof(UINT8);
	memset(image_data(), 0, fill_size);
	if( fill_size != this->FillData(i,image_data(),fill_size, rgba) )
	{
		return false;
	}

	sgp::AutoArray<png::png_bytep> rows(image.usHeight);
	int line_skip = image.usWidth * (rgba ? 4 : 1);
	for(int j=0; j<image.usHeight; ++j)
	{
		rows[j] = &image_data[j * line_skip];
	}
	png::png_set_rows(png_ptr,info_ptr, rows());
	
	if(file)
	{
		vfs::COpenWriteFile wfile(file);
		png::png_set_write_fn(png_ptr,file,user_write_data, user_flush_data);
		png::png_write_png(png_ptr, info_ptr, png_transforms, NULL);

		png_write_end(png_ptr,info_ptr);
	}

	png::png_destroy_info_struct(png_ptr, &info_ptr);
	png::png_destroy_write_struct(&png_ptr, (png::png_infopp)NULL);

	return true;
}

bool ja2xp::CImage::writeImageToPNGFile(vfs::tWritableFile* file, bool rgba)
{
	png::png_error_ptr	user_error_ptr = NULL;
	png::png_error_ptr	user_error_fn = NULL;
	png::png_error_ptr	user_warning_fn = NULL;

	png::png_structp png_ptr = png::png_create_write_struct( 
		PNG_LIBPNG_VER_STRING,
		(png::png_voidp)user_error_ptr,
		user_error_fn,
		user_warning_fn );
	if(!png_ptr)
	{
	   return (false);
	}

	png::png_infop info_ptr = png::png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png::png_destroy_write_struct(&png_ptr, (png::png_infopp)NULL);
		return (false);
	}

	/* set the zlib compression level */
	png::png_set_compression_level(png_ptr, Z_DEFAULT_COMPRESSION);

	/* set other zlib parameters */
	png::png_set_compression_mem_level(png_ptr, 8);
	png::png_set_compression_strategy(png_ptr, Z_DEFAULT_STRATEGY);
	png::png_set_compression_window_bits(png_ptr, 15);
	png::png_set_compression_method(png_ptr, 8);
	png::png_set_compression_buffer_size(png_ptr, 8192);

//		std::cout << "16 : widht : "  << m_pRawImage->usWidth << ", height : " << m_pRawImage->usHeight << std::endl;

	png::png_set_IHDR(
		png_ptr,
		info_ptr,
		m_pRawImage->usWidth,			// - holds the width of the image in pixels (up to 2^31).
		m_pRawImage->usHeight,			//   holds the height of the image in pixels (up to 2^31).
		8,								// - holds the bit depth of one of the image channels.
										//   (valid values are 1, 2, 4, 8, 16 and depend also on the color_type.  
										//   See also significant bits (sBIT) below).
		PNG_COLOR_TYPE_RGB,				// - describes which color/alpha channels are present.
										//   PNG_COLOR_TYPE_GRAY (bit depths 1, 2, 4, 8, 16)
										//   PNG_COLOR_TYPE_GRAY_ALPHA (bit depths 8, 16)
										//   PNG_COLOR_TYPE_PALETTE (bit depths 1, 2, 4, 8)
										//   PNG_COLOR_TYPE_RGB (bit_depths 8, 16)
										//   PNG_COLOR_TYPE_RGB_ALPHA (bit_depths 8, 16)
										//   PNG_COLOR_MASK_PALETTE
										//   PNG_COLOR_MASK_COLOR
										//   PNG_COLOR_MASK_ALPHA
		PNG_INTERLACE_NONE,				// - PNG_INTERLACE_NONE or PNG_INTERLACE_ADAM7
		PNG_COMPRESSION_TYPE_DEFAULT,	// - (must be PNG_COMPRESSION_TYPE_DEFAULT)
		PNG_FILTER_TYPE_DEFAULT 		// - (must be PNG_FILTER_TYPE_DEFAULT or, if you are writing a PNG to
										//   be embedded in a MNG datastream, can also be PNG_INTRAPIXEL_DIFFERENCING)
	);

	png_set_oFFs(png_ptr, info_ptr, 
		0,							// - positive offset from the left edge of the screen
		0,							// - positive offset from the top edge of the screen
		PNG_OFFSET_PIXEL			// - PNG_OFFSET_PIXEL, PNG_OFFSET_MICROMETER
	);

	UINT32 png_transforms = PNG_TRANSFORM_IDENTITY;
		
	std::vector<UINT8> image_data;
	image_data.resize(3*m_pRawImage->usHeight*m_pRawImage->usWidth);
	sgp::AutoArray<png::png_bytep> rows(m_pRawImage->usHeight);
	if(m_pRawImage->ubBitDepth == 16)
	{
		this->Unpack16BppImageToRGBBuffer((UINT16*)m_pRawImage->pImageData,&image_data[0],m_pRawImage->usWidth, m_pRawImage->usHeight);
		for(int j=0; j<m_pRawImage->usHeight; ++j)
		{
			rows[j] = (UINT8*)&image_data[3*j*m_pRawImage->usWidth];
		}
		png_set_rows(png_ptr,info_ptr, rows());
	}			
	else if(m_pRawImage->ubBitDepth == 8)
	{
		UINT8 *image_data = (UINT8*)m_pRawImage->pImageData;
		for(int j=0; j<m_pRawImage->usHeight; ++j)
		{
			rows[j] = &image_data[j*m_pRawImage->usWidth];
		}
		png_set_rows(png_ptr,info_ptr, rows());
	}			
		
	if(file)
	{
		vfs::COpenWriteFile wfile(file);
		png::png_set_write_fn(png_ptr,file,user_write_data, user_flush_data);
		png::png_write_png(png_ptr, info_ptr, png_transforms, NULL);
		png_write_end(png_ptr,info_ptr);
	}

	png::png_destroy_info_struct(png_ptr, &info_ptr);
	png::png_destroy_write_struct(&png_ptr, (png::png_infopp)NULL);

	return true;
}

bool ja2xp::CImage::writeSTIAppData(std::vector<bool> const& write_image, bool bWriteOffsets, vfs::tWritableFile* file)
{
	XMLWriter xmlw;
	xmlw.openNode("ImageData");
	for(int i=0; i<m_pRawImage->usNumberOfObjects; ++i)
	{
		if(write_image[i] || bWriteOffsets)
		{
			xmlw.addAttributeToNextValue("index",i);
			xmlw.openNode("SubImage");
			if(bWriteOffsets)
			{
				if(m_pRawImage->pETRLEObject[i].sOffsetX != 0)
				{
					xmlw.addAttributeToNextValue("x", m_pRawImage->pETRLEObject[i].sOffsetX);
				}
				if(m_pRawImage->pETRLEObject[i].sOffsetY != 0)
				{
					xmlw.addAttributeToNextValue("y", m_pRawImage->pETRLEObject[i].sOffsetY);
				}
				xmlw.addValue("offset");
			}
			AuxObjectData *pAOD = (AuxObjectData*)(&(m_pRawImage->pAppData[i*16]));
			if(write_image[i])
			{
				xmlw.openNode("AuxObjectData");
				if(pAOD->fFlags)			WriteFlags( xmlw, "flags", pAOD->fFlags );

				if(pAOD->ubCurrentFrame)	xmlw.addValue("ubCurrentFrame",		(int)pAOD->ubCurrentFrame);
				if(pAOD->ubNumberOfFrames)	xmlw.addValue("ubNumberOfFrames",	(int)pAOD->ubNumberOfFrames);
				if(pAOD->ubNumberOfTiles)	xmlw.addValue("ubNumberOfTiles",	(int)pAOD->ubNumberOfTiles);
				if(pAOD->ubWallOrientation) xmlw.addValue("ubWallOrientation",	(int)pAOD->ubWallOrientation);
				if(pAOD->usTileLocIndex)	xmlw.addValue("usTileLocIndex",		(int)pAOD->usTileLocIndex);

				xmlw.closeNode();
			}
			xmlw.closeNode();
		}
	}
	xmlw.closeNode();

	return xmlw.writeToFile(file);
}


bool ja2xp::CImage::WriteAsPNGs(vfs::tWritableFile* outFile, bool bWriteOffsets, bool rgba)
{
	if(!this->LoadData())
	{
		return false;
	}

	vfs::CCreateUncompressed7zLibrary outLib;
	vfs::Path sFileName,path;
	
	m_pImageFile->getName().splitLast(path,sFileName);
	
	if(m_pRawImage->usNumberOfObjects)
	{
		for(int i=0; i<m_pRawImage->usNumberOfObjects; ++i)
		{
			vfs::Path filename = CreateFileName("", sFileName, i, m_pRawImage->usNumberOfObjects, L"png");
			vfs::CBufferFile memFile(filename);
			vfs::tWritableFile *pFile = vfs::tWritableFile::cast(&memFile);

			this->writeSubImageToPNGFile(i, pFile, rgba);
			outLib.addFile(vfs::tReadableFile::cast(pFile));
		}
	}
	else
	{
		vfs::Path filename = CreateFileName("", sFileName, 0, m_pRawImage->usNumberOfObjects, L"png");
		vfs::CBufferFile memFile(filename);
		vfs::tWritableFile *pFile = vfs::tWritableFile::cast(&memFile);

		this->writeImageToPNGFile(pFile, rgba);
		outLib.addFile(vfs::tReadableFile::cast(pFile));
	}
	if(m_pRawImage->usNumberOfObjects > 0)
	{
		std::vector<bool> write_image(m_pRawImage->usNumberOfObjects, false);
		bool write_appdata = false;
		if(m_pRawImage->pAppData)
		{
			for(int i=0; i<m_pRawImage->usNumberOfObjects; ++i)
			{
				AuxObjectData *pAOD = (AuxObjectData*)(&(m_pRawImage->pAppData[i*16]));
				if(pAOD && (pAOD->fFlags || 
							pAOD->ubCurrentFrame || 
							pAOD->ubNumberOfFrames || 
							pAOD->ubNumberOfTiles || 
							pAOD->ubWallOrientation ||
							pAOD->usTileLocIndex) )
				{
					write_image[i] = true;
					write_appdata = true;
				}
			}
		}
		if(write_appdata || bWriteOffsets)
		{
			vfs::CBufferFile appDataFile("appdata.xml");
			this->writeSTIAppData(write_image, bWriteOffsets, vfs::tWritableFile::cast(&appDataFile));

			outLib.addFile(vfs::tReadableFile::cast(&appDataFile));
		}
	}

	return outLib.writeLibrary(outFile);
};

bool ja2xp::CImage::WriteAsPNGs(vfs::Path outpath, bool bWriteOffsets, bool rgba)
{
	if(!this->LoadData())
	{
		return false;
	}

	vfs::Path sFileName,path;
	
	m_pImageFile->getName().splitLast(path,sFileName);
	
	if(m_pRawImage->usNumberOfObjects)
	{
		for(int i=0; i<m_pRawImage->usNumberOfObjects; ++i)
		{
			vfs::Path filename = CreateFileName(outpath, sFileName, i, m_pRawImage->usNumberOfObjects, L"png");
			
			vfs::COpenWriteFile file(filename, true, true);
			this->writeSubImageToPNGFile(i, &file.file(), rgba);
		}
	}
	else
	{
		vfs::Path filename = CreateFileName("", sFileName, 0, m_pRawImage->usNumberOfObjects, L"png");

		vfs::COpenWriteFile file(filename, true, true);
		this->writeImageToPNGFile(&file.file(), rgba);
	}
	if(m_pRawImage->usNumberOfObjects > 0)
	{
		std::vector<bool> write_image(m_pRawImage->usNumberOfObjects, false);
		bool write_appdata = false;
		if(m_pRawImage->pAppData)
		{
			for(int i=0; i<m_pRawImage->usNumberOfObjects; ++i)
			{
				AuxObjectData *pAOD = (AuxObjectData*)(&(m_pRawImage->pAppData[i*16]));
				if(pAOD && (pAOD->fFlags || 
							pAOD->ubCurrentFrame || 
							pAOD->ubNumberOfFrames || 
							pAOD->ubNumberOfTiles || 
							pAOD->ubWallOrientation ||
							pAOD->usTileLocIndex) )
				{
					write_image[i] = true;
					write_appdata = true;
				}
			}
		}
		if(write_appdata || bWriteOffsets)
		{
			vfs::COpenWriteFile file(outpath+vfs::Path(L"appdata.xml"), true, true);
			this->writeSTIAppData(write_image, bWriteOffsets, &file.file());
		}
	}

	return true;
}

bool ja2xp::CImage::WriteFirstPNG(vfs::tWritableFile* outFile, bool rgba)
{
	if(!outFile)
	{
		return false;
	}
	if(!this->LoadData())
	{
		return false;
	}

	if(m_pRawImage->usNumberOfObjects)
	{
		if(m_pRawImage->usNumberOfObjects > 1)
		{
			std::wcout << L"file has multiple subimages - converting only the first one" << std::endl;
		}
		this->writeSubImageToPNGFile(0, outFile, rgba);
	}
	else
	{
		this->writeImageToPNGFile(outFile);
	}
	if(m_pRawImage->pAppData)
	{
		std::wcout << "File has addional information stored - disable the '-onepng' option to convert it too" << std::endl;
	}

	return true;
}


void ja2xp::CImage::LoadBMP()
{
	if(!m_pImageFile->openRead())
	{
		return;
	}
	BITMAPFILEHEADER bmp_header;
	if(!m_pImageFile->read((vfs::Byte*)&bmp_header,sizeof(BITMAPFILEHEADER)))
	{
		return;
	}
	BITMAPINFOHEADER bmp_infoheader;
	if(!m_pImageFile->read((vfs::Byte*)&bmp_infoheader,sizeof(BITMAPINFOHEADER)))
	{
		return;
	}
	UINT8 palette[4*256];
	memset(palette,0,4*256);
	if(!m_pImageFile->read((vfs::Byte*)&palette,256*sizeof(RGBQUAD)))
	{
		return;
	}
}
#if 0
bool CImage::WriteAsSTIs(vfs_string sOutPathName)
{
	if(!this->LoadData())
	{
		return false;
	}

	vfs_string sFileName,path;
	
	//SplitPath(m_pImageFile->GetFileName(),path,sFileName);
	SplitPath(m_pImageFile->GetFileName()(),path,sFileName);
	
	for(int i=0; i<m_pRawImage->usNumberOfObjects; ++i)
	{
		ETRLEObject &image = m_pRawImage->pETRLEObject[i];

		IndexedSTIImage stiimg;
		stiimg.AddCompressedImage(&m_pRawImage->p8BPPData[image.uiDataOffset], image.uiDataLength, image.usWidth, image.usHeight, image.sOffsetX, image.sOffsetY);
		
		stiimg.SetPalette(this->GetPalette(), 256);		
		
		ChompSlash(sOutPathName);
		vfs_string filename = CreateFileName(sOutPathName, sFileName, i, m_pRawImage->usNumberOfObjects, L"sti");
		vfs::tWriteableFile *pFile = GetWriteFile(filename);
		if(pFile && pFile->OpenWrite())
		{
			stiimg.WriteImage(pFile);
			pFile->Close();
		}

	}


	return true;
};
#endif

