#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include <string.h>
	#include "MemMan.h"
	#include "FileMan.h"
	#include "imgfmt.h"
	#include "himage.h"
	#include "Types.h"
	#include "Debug.h"
	#include "WCheck.h"
#endif

BOOLEAN STCILoadRGB( HIMAGE hImage, UINT16 fContents, HWFILE hFile, STCIHeader * pHeader );
BOOLEAN STCILoadIndexed( HIMAGE hImage, UINT16 fContents, HWFILE hFile, STCIHeader * pHeader );
BOOLEAN STCISetPalette( PTR pSTCIPalette, HIMAGE hImage );

BOOLEAN LoadSTCIFileToImage( HIMAGE hImage, UINT16 fContents )
{
	HWFILE			hFile;
	STCIHeader	Header;
	UINT32			uiBytesRead;
	image_type	TempImage;

	// Check that hImage is valid, and that the file in question exists
	Assert( hImage != NULL );

	TempImage = *hImage;

	CHECKF( FileExists( TempImage.ImageFile ) );

	// Open the file and read the header
	hFile = FileOpen( TempImage.ImageFile, FILE_ACCESS_READ, FALSE );
	CHECKF( hFile );

	if (!FileRead( hFile, &Header, STCI_HEADER_SIZE, &uiBytesRead ) || uiBytesRead != STCI_HEADER_SIZE || memcmp( Header.cID, STCI_ID_STRING, STCI_ID_LEN ) != 0 )
	{
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem reading STCI header." );
		FileClose( hFile );
		return( FALSE );
	}

	// Determine from the header the data stored in the file. and run the appropriate loader
	if (Header.fFlags & STCI_RGB)
	{
		if( !STCILoadRGB( &TempImage, fContents, hFile, &Header ) )
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem loading RGB image." );
			FileClose( hFile );
			return( FALSE );
		}
	}
	else if (Header.fFlags & STCI_INDEXED)
	{
		if( !STCILoadIndexed( &TempImage, fContents, hFile, &Header ) )
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem loading palettized image." );
			FileClose( hFile );
			return( FALSE );
		}
	}
	else
	{	// unsupported type of data, or the right flags weren't set!
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Unknown data organization in STCI file." );
		FileClose( hFile );
		return( FALSE );
	}

	// Requested data loaded successfully.
	FileClose( hFile );

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

BOOLEAN STCILoadRGB( HIMAGE hImage, UINT16 fContents, HWFILE hFile, STCIHeader * pHeader )
{
	UINT32			uiBytesRead;

	if (fContents & IMAGE_PALETTE && !(fContents & IMAGE_ALLIMAGEDATA))
	{ // RGB doesn't have a palette!
		return( FALSE );
	}

	if (fContents & IMAGE_BITMAPDATA)
	{
		// Allocate memory for the image data and read it in
		hImage->pImageData =	MemAlloc( pHeader->uiStoredSize );
		if (hImage->pImageData == NULL)
		{
			return( FALSE );
		}
		else if (!FileRead( hFile, hImage->pImageData, pHeader->uiStoredSize, &uiBytesRead ) || uiBytesRead != pHeader->uiStoredSize)
		{
			MemFree( hImage->pImageData );
			return( FALSE );
		}

		hImage->fFlags |= IMAGE_BITMAPDATA;

		if( pHeader->ubDepth == 16)
		{
			// ASSUMPTION: file data is 565 R,G,B

			if (gusRedMask != (UINT16) pHeader->RGB.uiRedMask || gusGreenMask != (UINT16) pHeader->RGB.uiGreenMask || gusBlueMask != (UINT16) pHeader->RGB.uiBlueMask )
			{
				// colour distribution of the file is different from hardware!	We have to change it!
				DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Converting to current RGB distribution!" );
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
#ifdef JA2
	return( TRUE );
#else
// Anything else is an ERROR! --DB
	return(FALSE);
#endif
}


BOOLEAN STCILoadIndexed( HIMAGE hImage, UINT16 fContents, HWFILE hFile, STCIHeader * pHeader )
{
	UINT32			uiFileSectionSize;
	UINT32			uiBytesRead;
	PTR					pSTCIPalette;

	if (fContents & IMAGE_PALETTE)
	{ // Allocate memory for reading in the palette
		if (pHeader->Indexed.uiNumberOfColours != 256)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Palettized image has bad palette size." );
			return( FALSE );
		}
		uiFileSectionSize = pHeader->Indexed.uiNumberOfColours * STCI_PALETTE_ELEMENT_SIZE;
		pSTCIPalette = MemAlloc( uiFileSectionSize );
		if (pSTCIPalette == NULL)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Out of memory!" );
			FileClose( hFile );
			return( FALSE );
		}

		// ATE: Memset: Jan 16/99
		memset( pSTCIPalette, 0, uiFileSectionSize );

		// Read in the palette
		if (!FileRead( hFile, pSTCIPalette, uiFileSectionSize, &uiBytesRead ) || uiBytesRead != uiFileSectionSize)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem loading palette!" );
			FileClose( hFile );
			MemFree( pSTCIPalette );
			return( FALSE );
		}
		else if (!STCISetPalette( pSTCIPalette, hImage ))
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem setting hImage-format palette!" );
			FileClose( hFile );
			MemFree( pSTCIPalette );
			return( FALSE );
		}
		hImage->fFlags |= IMAGE_PALETTE;
		// Free the temporary buffer
		MemFree( pSTCIPalette );
	}
	else if (fContents & (IMAGE_BITMAPDATA | IMAGE_APPDATA))
	{ // seek past the palette
		uiFileSectionSize = pHeader->Indexed.uiNumberOfColours * STCI_PALETTE_ELEMENT_SIZE;
		if (FileSeek( hFile, uiFileSectionSize, FILE_SEEK_FROM_CURRENT) == FALSE)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem seeking past palette!" );
			FileClose( hFile );
			return( FALSE );
		}
	}
	if (fContents & IMAGE_BITMAPDATA)
	{
		if (pHeader->fFlags & STCI_ETRLE_COMPRESSED)
		{
			// load data for the subimage (object) structures
			Assert( sizeof( ETRLEObject ) == STCI_SUBIMAGE_SIZE );
			hImage->usNumberOfObjects = pHeader->Indexed.usNumberOfSubImages;
			uiFileSectionSize = hImage->usNumberOfObjects * STCI_SUBIMAGE_SIZE;
			hImage->pETRLEObject = (ETRLEObject *) MemAlloc( uiFileSectionSize );
			if (hImage->pETRLEObject == NULL)
			{
				DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Out of memory!" );
				FileClose( hFile );
				if (fContents & IMAGE_PALETTE)
				{
					MemFree( hImage->pPalette );
				}
				return( FALSE );
			}
			if (!FileRead( hFile, hImage->pETRLEObject, uiFileSectionSize, &uiBytesRead ) || uiBytesRead != uiFileSectionSize)
			{
				DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Error loading subimage structures!" );
				FileClose( hFile );
				if (fContents & IMAGE_PALETTE)
				{
					MemFree( hImage->pPalette );
				}
				MemFree( hImage->pETRLEObject );
				return( FALSE );
			}
			hImage->uiSizePixData = pHeader->uiStoredSize;
			hImage->fFlags |= IMAGE_TRLECOMPRESSED;
		}
		// allocate memory for and read in the image data
		hImage->pImageData = MemAlloc( pHeader->uiStoredSize );
		if (hImage->pImageData == NULL)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Out of memory!" );
			FileClose( hFile );
			if (fContents & IMAGE_PALETTE)
			{
				MemFree( hImage->pPalette );
			}
			if (hImage->usNumberOfObjects > 0)
			{
				MemFree( hImage->pETRLEObject );
			}
			return( FALSE );
		}
		else if (!FileRead( hFile, hImage->pImageData, pHeader->uiStoredSize, &uiBytesRead ) || uiBytesRead != pHeader->uiStoredSize)
		{ // Problem reading in the image data!
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Error loading image data!" );
			FileClose( hFile );
			MemFree( hImage->pImageData );
			if (fContents & IMAGE_PALETTE)
			{
				MemFree( hImage->pPalette );
			}
			if (hImage->usNumberOfObjects > 0)
			{
				MemFree( hImage->pETRLEObject );
			}
			return( FALSE );
		}
		hImage->fFlags |= IMAGE_BITMAPDATA;
	}
	else if (fContents & IMAGE_APPDATA) // then there's a point in seeking ahead
	{
		if (FileSeek( hFile, pHeader->uiStoredSize, FILE_SEEK_FROM_CURRENT) == FALSE)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem seeking past image data!" );
			FileClose( hFile );
			return( FALSE );
		}
	}

	if (fContents & IMAGE_APPDATA && pHeader->uiAppDataSize > 0)
	{
		// load application-specific data
		hImage->pAppData = (UINT8 *) MemAlloc( pHeader->uiAppDataSize );
		if (hImage->pAppData == NULL)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Out of memory!" );
			FileClose( hFile );
			MemFree( hImage->pAppData );
			if (fContents & IMAGE_PALETTE)
			{
				MemFree( hImage->pPalette );
			}
			if (fContents & IMAGE_BITMAPDATA)
			{
				MemFree( hImage->pImageData );
			}
			if (hImage->usNumberOfObjects > 0)
			{
				MemFree( hImage->pETRLEObject );
			}
			return( FALSE );
		}
		if (!FileRead( hFile, hImage->pAppData, pHeader->uiAppDataSize, &uiBytesRead ) || uiBytesRead != pHeader->uiAppDataSize)
		{
			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Error loading application-specific data!" );
			FileClose( hFile );
			MemFree( hImage->pAppData );
			if (fContents & IMAGE_PALETTE)
			{
				MemFree( hImage->pPalette );
			}
			if (fContents & IMAGE_BITMAPDATA)
			{
				MemFree( hImage->pImageData );
			}
			if (hImage->usNumberOfObjects > 0)
			{
				MemFree( hImage->pETRLEObject );
			}
			return( FALSE );
		}
		hImage->uiAppDataSize = pHeader->uiAppDataSize;;
		hImage->fFlags |= IMAGE_APPDATA;
	}
	else
	{
		hImage->pAppData = NULL;
		hImage->uiAppDataSize = 0;
	}
	return( TRUE );
}


BOOLEAN STCISetPalette( PTR pSTCIPalette, HIMAGE hImage )
{
	UINT16								usIndex;
	STCIPaletteElement *	pubPalette;

	pubPalette = (STCIPaletteElement *) pSTCIPalette;

	// Allocate memory for palette
	hImage->pPalette = (SGPPaletteEntry *) MemAlloc( sizeof( SGPPaletteEntry ) * 256 );
	memset( hImage->pPalette, 0, ( sizeof( SGPPaletteEntry ) * 256 ) );

	if ( hImage->pPalette == NULL )
	{
		return( FALSE );
	}

	// Initialize the proper palette entries
	for (usIndex = 0; usIndex < 256; usIndex++)
	{
		hImage->pPalette[ usIndex ].peRed	= pubPalette->ubRed;
		hImage->pPalette[ usIndex ].peGreen = pubPalette->ubGreen;
	hImage->pPalette[ usIndex ].peBlue	= pubPalette->ubBlue;
	hImage->pPalette[ usIndex ].peFlags = 0;
		pubPalette ++;
	}
	return TRUE;
}


BOOLEAN IsSTCIETRLEFile( CHAR8 * ImageFile )
{
	HWFILE		hFile;
	STCIHeader	Header;
	UINT32		uiBytesRead;

	CHECKF( FileExists( ImageFile ) );

	// Open the file and read the header
	hFile = FileOpen( ImageFile, FILE_ACCESS_READ, FALSE );
	CHECKF( hFile );

	if (!FileRead( hFile, &Header, STCI_HEADER_SIZE, &uiBytesRead ) || uiBytesRead != STCI_HEADER_SIZE || memcmp( Header.cID, STCI_ID_STRING, STCI_ID_LEN ) != 0 )
	{
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Problem reading STCI header." );
		FileClose( hFile );
		return( FALSE );
	}
	FileClose( hFile );
	if (Header.fFlags & STCI_ETRLE_COMPRESSED)
	{
	 return( TRUE );
	}
	else
	{
	 return( FALSE );
	}
}
