#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Types.h"
#include "himage.h"

#include <vfs/Core/vfs_types.h>
#include <vfs/Core/File/vfs_file.h>

#include <vector>

namespace ja2xp
{
	class CImage
	{
	public:
		enum EImageType
		{
			IT_NONE,
			IT_BITMAP,
			IT_COMPRESSED,
		};
	public:

		CImage(vfs::Path const& sFileName); 
		CImage(vfs::tReadableFile *pFile); 
		~CImage();
		
		EImageType GetImageType();

		bool	GetSize(UINT32 uiSubImage, UINT32 &uiWidth, UINT32 &uiHeight);
		bool	GetPosition(UINT32 uiSubImage, INT32 &uiX, INT32 &uiY);
				
		SGPPaletteEntry* GetPalette();

		// use when image type is 'BITMAP'
		void*	GetDataPointer(UINT32 &uiSize);

		bool	GetRawDataOfSubImage(UINT32 uiSubImage, void* pData, UINT32 uiMaxSize, UINT32 &uiFilledSize);
		// use when image type is 'COMPRESSED' unless you know how to 'decompress'
		UINT32	FillData(UINT32 uiSubImage, UINT8* pData, UINT32 uiMaxSize, bool rgba = false);
		bool	Unpack16BppImageToRGBBuffer(UINT16* pSrc, UINT8* pDst, UINT32 uiWidth, UINT32 uiHeight);

		UINT32	GetNumberOfSubImages();
		
		bool	LoadData();
		bool	DeleteData();

		void	LoadBMP();
		//bool	WriteAsBMPs(vfs_string sOutPathName);
		bool	WriteAsPNGs(vfs::tWritableFile* outFile, bool bWriteOffsets=false, bool rgba=false );
		bool	WriteAsPNGs(vfs::Path outpath, bool bWriteOffsets=false, bool rgba=false);
		bool	WriteFirstPNG(vfs::tWritableFile* outFile, bool rgba=false);
		//bool	WriteAsSTIs(vfs_string sOutPathName);

	protected:
		bool	writeSubImageToPNGFile(int i, vfs::tWritableFile* file, bool rgba = false);
		bool	writeImageToPNGFile(vfs::tWritableFile* file, bool rgba = false);
		bool	writeSTIAppData(std::vector<bool> const& write_image, bool bWriteOffsets, vfs::tWritableFile* file);
	private:
		vfs::tReadableFile*	m_pImageFile;
		EImageType			m_eImageType;
		image_type*			m_pRawImage;
		bool				m_bImageLoaded;
	};
};

#endif // _IMAGE_H_
