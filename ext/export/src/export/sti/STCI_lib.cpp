#include <vfs/Core/vfs_types.h>

#include "STCI_lib.h"

#include "imgfmt.h"
#include "himage.h"
#include "Types.h"
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/vfs_debug.h>
#include <string>

#include <cassert>
#include <iostream>

namespace ja2xp
{
	BOOLEAN STCILoadRGB(vfs::tReadableFile *pFile, HIMAGE hImage, UINT16 fContents,  STCIHeader * pHeader );
	BOOLEAN STCILoadIndexed(vfs::tReadableFile *pFile, HIMAGE hImage, UINT16 fContents, STCIHeader * pHeader );
	BOOLEAN STCISetPalette(vfs::tReadableFile *pFile, PTR pSTCIPalette, HIMAGE hImage );
};

BOOLEAN ja2xp::LoadSTCIFileToImage(vfs::tReadableFile *pFile, HIMAGE hImage, UINT16 fContents )
{
	STCIHeader	Header;
	image_type	TempImage;

	// Check that hImage is valid, and that the file in question exists
	assert( hImage != NULL );

	TempImage = *hImage;

	if(pFile == NULL)
	{
		return FALSE;
	}

	// Open the file and read the header
	
	vfs::COpenReadFile file(pFile);
	VFS_THROW_IFF( STCI_HEADER_SIZE == pFile->read((vfs::Byte*)&Header,STCI_HEADER_SIZE), L"" );
	VFS_THROW_IFF( memcmp( Header.cID, STCI_ID_STRING, STCI_ID_LEN ) == 0, L"" );

	// Determine from the header the data stored in the file. and run the appropriate loader
	if (Header.fFlags & STCI_RGB)
	{
		if( !STCILoadRGB(pFile, &TempImage, fContents, &Header ) )
		{
			pFile->close();
			std::cout << ".. loading bitmap image failed .." << std::endl;
			return( FALSE );
		}
	}
	else if (Header.fFlags & STCI_INDEXED)
	{
		if( !STCILoadIndexed(pFile, &TempImage, fContents, &Header ) )
		{
			pFile->close();
			std::cout << ".. loading compressed image failed .." << std::endl;
			return( FALSE );
		}
	}
	else
	{	// unsupported type of data, or the right flags weren't set!
		pFile->close();
		return( FALSE );
	}

	// Requested data loaded successfully.
	pFile->close();

	// Set some more flags in the temporary image structure, copy it so that hImage points
	// to it, and return.
	if (Header.fFlags & STCI_ZLIB_COMPRESSED)
	{
		TempImage.fFlags |= IMAGE_COMPRESSED;
	}
	TempImage.usWidth = Header.usWidth;
	TempImage.usHeight = Header.usHeight;
	TempImage.ubBitDepth = Header.ubDepth;
	*hImage = TempImage;

	return( TRUE );		
}

BOOLEAN ja2xp::STCILoadRGB(vfs::tReadableFile *pFile, HIMAGE hImage, UINT16 fContents, STCIHeader * pHeader )
{
	if (fContents & IMAGE_PALETTE && !(fContents & IMAGE_ALLIMAGEDATA))
	{ 
		// RGB doesn't have a palette!
		std::cout << "RGB doesn't have a palette!" << std::endl;
		return( FALSE );
	}
	
	if (fContents & IMAGE_BITMAPDATA)
	{ 
		// Allocate memory for the image data and read it in
		hImage->pImageData = new char[ pHeader->uiStoredSize ];
		if (hImage->pImageData == NULL)
		{
			return( FALSE );
		}
		try
		{
			vfs::size_t size = pHeader->uiStoredSize;
			VFS_THROW_IFF( size == pFile->read( (vfs::Byte*)hImage->pImageData, size ), L"" ); 
		}
		catch(vfs::Exception& ex)
		{
			delete[] hImage->pImageData;
			return( FALSE );
		}

		hImage->fFlags |= IMAGE_BITMAPDATA;

		if( pHeader->ubDepth == 16)
		{
			// ASSUMPTION: file data is 565 R,G,B

			if (gusRedMask != (UINT16) pHeader->RGB.uiRedMask || gusGreenMask != (UINT16) pHeader->RGB.uiGreenMask || gusBlueMask != (UINT16) pHeader->RGB.uiBlueMask )
			{
				// colour distribution of the file is different from hardware!  We have to change it!
				// Convert the image to the current hardware's specifications
				if (gusRedMask > gusGreenMask && gusGreenMask > gusBlueMask)
				{
					// hardware wants RGB!
					if (gusRedMask == 0x7C00 && gusGreenMask == 0x03E0 && gusBlueMask == 0x001F)
					{	// hardware is 555
						ConvertRGBDistribution565To555( hImage->p16BPPData, pHeader->usWidth * pHeader->usHeight );
						return( TRUE );
					}
					else if (gusRedMask == 0xFC00 && gusGreenMask == 0x03E0 && gusBlueMask == 0x001F)
					{
						ConvertRGBDistribution565To655( hImage->p16BPPData, pHeader->usWidth * pHeader->usHeight );
						return( TRUE );
					}
					else if (gusRedMask == 0xF800 && gusGreenMask == 0x07C0 && gusBlueMask == 0x003F)
					{
						ConvertRGBDistribution565To556( hImage->p16BPPData, pHeader->usWidth * pHeader->usHeight );
						return( TRUE );
					}
					else 
					{
						// take the long route
						ConvertRGBDistribution565ToAny( hImage->p16BPPData, pHeader->usWidth * pHeader->usHeight );
						return( TRUE );
					}
				}
				else
				{
					// hardware distribution is not R-G-B so we have to take the long route!
					ConvertRGBDistribution565ToAny( hImage->p16BPPData, pHeader->usWidth * pHeader->usHeight );
					return( TRUE );
				}
			}
		}
	}
	return( TRUE );
}


BOOLEAN ja2xp::STCILoadIndexed(vfs::tReadableFile *pFile, HIMAGE hImage, UINT16 fContents, STCIHeader * pHeader )
{
	UINT32			uiFileSectionSize;
	PTR				pSTCIPalette;

	if (fContents & IMAGE_PALETTE)
	{
		// Allocate memory for reading in the palette
		if (pHeader->Indexed.uiNumberOfColours != 256)
		{ 
			return( FALSE );
		}
		uiFileSectionSize = pHeader->Indexed.uiNumberOfColours * STCI_PALETTE_ELEMENT_SIZE;
		pSTCIPalette = new char[uiFileSectionSize];
		if (pSTCIPalette == NULL)
		{
			pFile->close();
			return( FALSE );
		}

		memset( pSTCIPalette, 0, uiFileSectionSize );

		// Read in the palette
		try
		{
			VFS_THROW_IFF( uiFileSectionSize == pFile->read( (vfs::Byte*)pSTCIPalette, uiFileSectionSize ), L"" );
		}
		catch(vfs::Exception& ex)
		{
			pFile->close();
			delete[] pSTCIPalette;
			return( FALSE );
		}
		if (!STCISetPalette(pFile, pSTCIPalette, hImage ))
		{
			pFile->close();
			delete[] pSTCIPalette;
			return( FALSE );	
		}
		hImage->fFlags |= IMAGE_PALETTE;
		// Free the temporary buffer
		delete[] pSTCIPalette;
	}
	else if (fContents & (IMAGE_BITMAPDATA | IMAGE_APPDATA))
	{ // seek past the palette
		uiFileSectionSize = pHeader->Indexed.uiNumberOfColours * STCI_PALETTE_ELEMENT_SIZE;
		try
		{
			pFile->setReadPosition( uiFileSectionSize, vfs::IBaseFile::SD_CURRENT);
		}
		catch(vfs::Exception& ex)
		{
			pFile->close();
			return( FALSE );	
		}
	}
	if (fContents & IMAGE_BITMAPDATA)
	{ 	
		if (pHeader->fFlags & STCI_ETRLE_COMPRESSED)
		{
			// load data for the subimage (object) structures
			assert( sizeof( ETRLEObject ) == STCI_SUBIMAGE_SIZE );
			hImage->usNumberOfObjects = pHeader->Indexed.usNumberOfSubImages;
			uiFileSectionSize = hImage->usNumberOfObjects * STCI_SUBIMAGE_SIZE;
			hImage->pETRLEObject = (ETRLEObject *) new char[uiFileSectionSize];
			if (hImage->pETRLEObject == NULL)
			{
				pFile->close();
				if (fContents & IMAGE_PALETTE) 
				{
					delete[] hImage->pPalette;
				}
				return( FALSE );
			}
			try
			{
				VFS_THROW_IFF(uiFileSectionSize == pFile->read((vfs::Byte*)hImage->pETRLEObject, uiFileSectionSize), L"")
			}
			catch(vfs::Exception& ex)
			{ 
				//FileClose( hFile );
				pFile->close();
				if (fContents & IMAGE_PALETTE) 
				{
					delete[] hImage->pPalette;
				}
				delete[] hImage->pETRLEObject;
				return( FALSE );
			}
			hImage->uiSizePixData = pHeader->uiStoredSize;
			hImage->fFlags |= IMAGE_TRLECOMPRESSED;
		}
		// allocate memory for and read in the image data
		hImage->pImageData = new char[pHeader->uiStoredSize];
		if (hImage->pImageData == NULL)
		{
			pFile->close();
			if (fContents & IMAGE_PALETTE) 
			{
				delete[] hImage->pPalette;
			}
			if (hImage->usNumberOfObjects > 0)
			{
				delete[] hImage->pETRLEObject;
			}
			return( FALSE );
		}
		try
		{
			VFS_THROW_IFF(pHeader->uiStoredSize == pFile->read( (vfs::Byte*)hImage->pImageData, pHeader->uiStoredSize), L"");
		}
		catch(vfs::Exception& ex)
		{ // Problem reading in the image data!
			pFile->close();
			delete[] hImage->pImageData;
			if (fContents & IMAGE_PALETTE) 
			{
				delete[] hImage->pPalette;
			}
			if (hImage->usNumberOfObjects > 0)
			{
				delete[] hImage->pETRLEObject;
			}
			return( FALSE );
		}
		hImage->fFlags |= IMAGE_BITMAPDATA;
	}
	else if (fContents & IMAGE_APPDATA) // then there's a point in seeking ahead
	{
		try
		{
			pFile->setReadPosition( pHeader->uiStoredSize, vfs::IBaseFile::SD_CURRENT);
		}
		catch(vfs::Exception& ex)
		{
			//FileClose( hFile );
			pFile->close();
			return( FALSE );	
		}
	}

	if (/*fContents & IMAGE_APPDATA &&*/ pHeader->uiAppDataSize > 0)
	{
		// load application-specific data
		hImage->pAppData = new UINT8[pHeader->uiAppDataSize];
		if (hImage->pAppData == NULL)
		{
			pFile->close();
			delete[] hImage->pAppData;
			if (fContents & IMAGE_PALETTE) 
			{
				delete[] hImage->pPalette;
			}
			if (fContents & IMAGE_BITMAPDATA)
			{
				delete[] hImage->pImageData;
			}
			if (hImage->usNumberOfObjects > 0)
			{
				delete[] hImage->pETRLEObject;
			}
			return( FALSE );
		}
		try
		{
			VFS_THROW_IFF(pHeader->uiAppDataSize == pFile->read( (vfs::Byte*)hImage->pAppData, pHeader->uiAppDataSize), L"");
		}
		catch(vfs::Exception& ex)
		{ 
			pFile->close();
			delete[] hImage->pAppData;
			if (fContents & IMAGE_PALETTE) 
			{
				delete[] hImage->pPalette;
			}
			if (fContents & IMAGE_BITMAPDATA)
			{
				delete[] hImage->pImageData;
			}
			if (hImage->usNumberOfObjects > 0)
			{
				delete[] hImage->pETRLEObject;
			}
			return( FALSE );
		}
		hImage->uiAppDataSize = pHeader->uiAppDataSize;
		for(int i=0; i<pHeader->Indexed.usNumberOfSubImages; ++i)
		{
			AuxObjectData *pAOD = (AuxObjectData*)(&(hImage->pAppData[i*16]));
			pAOD->ubUnused[0] = pAOD->ubUnused[0];
		}
		hImage->fFlags |= IMAGE_APPDATA;
	}
	else
	{
		hImage->pAppData = NULL;
		hImage->uiAppDataSize = 0;
	}
	return( TRUE );
}


BOOLEAN ja2xp::STCISetPalette(vfs::tReadableFile *pFile, PTR pSTCIPalette, HIMAGE hImage )
{
	UINT16								usIndex;
	STCIPaletteElement *	pubPalette;

	pubPalette = (STCIPaletteElement *) pSTCIPalette;

	// Allocate memory for palette
	hImage->pPalette = (SGPPaletteEntry *) new char[ sizeof( SGPPaletteEntry ) * 256];
	memset( hImage->pPalette, 0, ( sizeof( SGPPaletteEntry ) * 256 ) );

	if ( hImage->pPalette == NULL )
	{
		return( FALSE );
	}

	// Initialize the proper palette entries
	for (usIndex = 0; usIndex < 256; usIndex++)
	{ 
		hImage->pPalette[ usIndex ].peRed   = pubPalette->ubRed;
		hImage->pPalette[ usIndex ].peGreen = pubPalette->ubGreen;
		hImage->pPalette[ usIndex ].peBlue  = pubPalette->ubBlue;
		hImage->pPalette[ usIndex ].peFlags = 0;
		pubPalette ++;
	}
	return TRUE;
}


BOOLEAN ja2xp::IsSTCIETRLEFile(vfs::tReadableFile *pFile, CHAR8 * ImageFile )
{
//	HWFILE		hFile;
	STCIHeader	Header;

	if(pFile == NULL)
	{
		return FALSE;
	}
	//CHECKF( FileExists( ImageFile ) );
	vfs::Path filename(ImageFile);

	try
	{
		VFS_THROW_IFF(STCI_HEADER_SIZE == pFile->read( (vfs::Byte*)&Header, STCI_HEADER_SIZE), L"");
		VFS_THROW_IFF( memcmp( Header.cID, STCI_ID_STRING, STCI_ID_LEN ) == 0, L"" );
	}
	catch(vfs::Exception& ex)
	{
		pFile->close();
		return( FALSE );
	}
	pFile->close();
	if (Header.fFlags & STCI_ETRLE_COMPRESSED)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
} 



