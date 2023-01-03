#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "builddefines.h"
	#include <math.h>
	#include <stdlib.h>
	#include "types.h"
	#include "string.h"
	#include "debug.h"
	#include "fileman.h"
	#include "himage.h"
	#include "impTGA.h"
	#include "pcx.h"
	#include "STCI.h"
	#include "PngLoader.h"
	#include "wcheck.h"
	#include "Compression.h"
	#include "vobject.h"
	#include "vobject_blitters.h"
#endif

#include <vfs/Core/vfs.h>

const vfs::String::str_t CONST_DOTJPC(L".jpc.7z");



// This is the color substituted to keep a 24bpp->16bpp color
// from going transparent (0x0000) -- DB

#define BLACK_SUBSTITUTE	0x0001


UINT16 gusAlphaMask = 0;
UINT16 gusRedMask = 0;
UINT16 gusGreenMask = 0;
UINT16 gusBlueMask = 0;
INT16	gusRedShift = 0;
INT16	gusBlueShift = 0;
INT16	gusGreenShift = 0;


// this funky union is used for fast 16-bit pixel format conversions
typedef union
{
	struct
	{
		UINT16	usLower;
		UINT16	usHigher;
	};
	UINT32	uiValue;
} SplitUINT32;

namespace ImageFileType
{
	typedef std::map<vfs::String, int, vfs::String::Less> ExtMap_t;

	static int map(vfs::String const& ext)
	{
		static ExtMap_t _ext_map;
		static bool inited = false;
		if(!inited)
		{
			_ext_map["pcx"]    = PCX_FILE_READER;
			_ext_map["tga"]    = TGA_FILE_READER;
			_ext_map["sti"]    = STCI_FILE_READER;
			_ext_map["png"]    = PNG_FILE_READER;
			_ext_map["jpc.7z"] = JPC_FILE_READER;
			inited = true;
		}
		ExtMap_t::const_iterator cit = _ext_map.find(ext);
		if(cit != _ext_map.end())
		{
			return cit->second;
		}
		return UNKNOWN_FILE_READER;
	}

	static int getFileReaderType(std::string& filename, TestOrder order)
	{
		std::string::size_type pos = filename.find_last_of(".");
		std::string ext = filename.substr(pos+1, std::string::npos);
		if(ext.empty())
		{
			ext = "pcx";
			filename += ".pcx";
		}
		int reader_type = map(ext);

		/*
		 * if DEFAULT, then just check existance of file
		 * if not STI, then there is no different load order, just continue as usual
		 */
		if(order == DEFAULT || reader_type != STCI_FILE_READER)
		{
			return getVFS()->fileExists(filename) ? reader_type : UNKNOWN_FILE_READER;
		}
		/*
		 * file must have originally been an STI file, but should be treated as a JPC or a PNG file
		 */
		else if(order == JPC || order == PNG)
		{
			vfs::String file = filename.substr(0, pos+1).append(order == JPC ? "jpc.7z" : "png");
			if( getVFS()->fileExists(file) )
			{
				filename = file.utf8();
				return order == JPC ? JPC_FILE_READER : PNG_FILE_READER;
			}
			return UNKNOWN_FILE_READER;
		}
		/*
		 * file must have originally been an STI file, but should be treated as a JPC or a PNG file
		 * if the replacement filetypes don't exist, fall back to STI
		 */
		else if(order == JPC_FALLBACK || order == PNG_FALLBACK)
		{
			vfs::String file = filename.substr(0, pos+1).append(order == JPC_FALLBACK ? "jpc.7z" : "png");
			if( getVFS()->fileExists(file) )
			{
				filename = file.utf8();
				return order == JPC_FALLBACK ? JPC_FILE_READER : PNG_FILE_READER;
			}
			// fallback to original type
			return getVFS()->fileExists(filename) ? reader_type : UNKNOWN_FILE_READER;
		}
		return UNKNOWN_FILE_READER;
	}
};

HIMAGE CreateImage( SGPFILENAME ImageFile, UINT16 fContents, ImageFileType::TestOrder order )
{
	HIMAGE			hImage = NULL;	
	CHAR8			ExtensionSep[] = ".";	
	UINT32			iFileLoader;

#if 0
	SGPFILENAME	Extension;
	STR					StrPtr;

	// Depending on extension of filename, use different image readers
	// Get extension
	StrPtr = strstr( ImageFile, ExtensionSep );

	if ( StrPtr == NULL )
	{
		// No extension given, use default internal loader extension
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, "No extension given, using default" );
		strcat( ImageFile, ".PCX" );
		strcpy( Extension, ".PCX" );
	}
	else
	{
		strcpy( Extension, StrPtr+1 );
	}

	// Determine type from Extension
	do
	{
		iFileLoader = UNKNOWN_FILE_READER;

		if ( _stricmp( Extension, "PCX" ) == 0 )
		{
			iFileLoader = PCX_FILE_READER;
			break;
		}
		else if ( _stricmp( Extension, "TGA" ) == 0 )
		{
			iFileLoader = TGA_FILE_READER;
			break;
		}
		else if ( _stricmp( Extension, "STI" ) == 0 )
		{
#ifdef USE_VFS
			// see if there is a .jpc file first and when that fails, try .sti
			vfs::Path str(ImageFile);
			vfs::String::str_t const& findext = str.c_wcs();
			vfs::String::size_t dot = findext.find_last_of(vfs::Const::DOT());
			vfs::String fname = findext.substr(0,dot).append(CONST_DOTJPC);
			if(getVFS()->fileExists(fname))
			{
				iFileLoader = JPC_FILE_READER;
				strncpy(ImageFile, fname.utf8().c_str(), fname.length());
				ImageFile[fname.length()] = 0;
				break;
			}
#endif
			iFileLoader = STCI_FILE_READER;
			break;
		}
		else if ( _stricmp( Extension, "PNG" ) == 0 )
		{
			iFileLoader = PNG_FILE_READER;
			break;
		}
#ifdef USE_VFS
		else if ( vfs::StrCmp::Equal(Extension, L"jpc.7z") )
		{
			iFileLoader = JPC_FILE_READER;
			break;
		}
#endif
	} while ( FALSE );

	// Determine if resource exists before creating image structure
	if ( !FileExists( ImageFile ) )
	{
		//If in debig, make fatal!
#ifdef JA2
#ifdef _DEBUG
		//FatalError( "Resource file %s does not exist.", ImageFile );
#endif
#endif
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, String("Resource file %s does not exist.", ImageFile) );

		return( NULL );
	}
#else
	std::string filename(ImageFile);
	iFileLoader = ImageFileType::getFileReaderType(filename, order);
	if ( iFileLoader == UNKNOWN_FILE_READER )
	{
		//If in debug, make fatal!
#ifdef JA2
#ifdef _DEBUG
		//FatalError( "Resource file %s does not exist.", ImageFile );
#endif
#endif
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, String("Resource file %s does not exist.", ImageFile) );

		return( NULL );
	}

#endif

	// Create memory for image structure
	hImage = (HIMAGE)MemAlloc( sizeof( image_type ) );

	AssertMsg( hImage, "Failed to allocate memory for hImage in CreateImage");
	// Initialize some values
	memset( hImage, 0, sizeof( image_type ) );

	//hImage->fFlags = 0;
	// Set data pointers to NULL
	//hImage->pImageData = NULL;
	//hImage->pPalette	= NULL;
	//hImage->pui16BPPPalette = NULL;

	// Set filename and loader
	strncpy( hImage->ImageFile, /*ImageFile*/filename.c_str(), filename.length() );
	hImage->iFileLoader = iFileLoader;

	if ( !LoadImageData( hImage, fContents ) )
	{
		return( NULL );
	}

	// All is fine, image is loaded and allocated, return pointer
	return( hImage );

}

BOOLEAN DestroyImage( HIMAGE hImage )
{
	Assert( hImage != NULL );

	// First delete contents
	ReleaseImageData( hImage, IMAGE_ALLDATA );//hImage->fFlags );

	// Now free structure
	MemFree( hImage );

	return( TRUE );
}

BOOLEAN ReleaseImageData( HIMAGE hImage, UINT16 fContents )
{

	Assert( hImage != NULL );

	if ( (fContents & IMAGE_PALETTE) && (hImage->fFlags & IMAGE_PALETTE) )
	{
		//Destroy palette
		if( hImage->pPalette != NULL )
		{
			MemFree( hImage->pPalette );
			hImage->pPalette = NULL;
		}

		if ( hImage->pui16BPPPalette != NULL )
		{
			MemFree( hImage->pui16BPPPalette );
			hImage->pui16BPPPalette = NULL;
		}

		// Remove contents flag
		hImage->fFlags = hImage->fFlags ^ IMAGE_PALETTE;
	}

	if ( (fContents & IMAGE_BITMAPDATA) && (hImage->fFlags & IMAGE_BITMAPDATA) )
	{
		//Destroy image data
		Assert( hImage->pImageData != NULL );
		MemFree( hImage->pImageData );
		hImage->pImageData = NULL;
		if (hImage->usNumberOfObjects > 0)
		{
			MemFree( hImage->pETRLEObject );
		}
		// Remove contents flag
		hImage->fFlags = hImage->fFlags ^ IMAGE_BITMAPDATA;
	}

	if ( (fContents & IMAGE_APPDATA) && (hImage->fFlags & IMAGE_APPDATA) )
	{
		// get rid of the APP DATA
		if ( hImage->pAppData != NULL )
		{
			MemFree( hImage->pAppData );
			hImage->fFlags &= (~IMAGE_APPDATA);
		}
	}

	return( TRUE );
}

BOOLEAN LoadImageData( HIMAGE hImage, UINT16 fContents )
{
	BOOLEAN fReturnVal = FALSE;

	Assert( hImage != NULL );

	// Switch on file loader
	switch( hImage->iFileLoader )
	{
		case TGA_FILE_READER:

			fReturnVal = LoadTGAFileToImage( hImage, fContents );
			break;

		case PCX_FILE_READER:

			fReturnVal = LoadPCXFileToImage( hImage, fContents );
			break;

		case STCI_FILE_READER:
			fReturnVal = LoadSTCIFileToImage( hImage, fContents );
			break;

		case PNG_FILE_READER:
			fReturnVal = LoadPNGFileToImage( hImage, fContents );
			break;

		case JPC_FILE_READER:
			fReturnVal = LoadJPCFileToImage( hImage, fContents );
			break;
		
		default:

			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, "Unknown image loader was specified." );

	}

	if ( !fReturnVal )
	{
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, "Error occured while reading image data." );
	}

	return( fReturnVal );

}

BOOLEAN CopyImageToBuffer( HIMAGE hImage, UINT32 fBufferType, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect )
{
	// Use blitter based on type of image
	Assert( hImage != NULL );

	if ( hImage->ubBitDepth == 8 && fBufferType == BUFFER_8BPP )
	{
		#ifndef NO_ZLIB_COMPRESSION
			if ( hImage->fFlags & IMAGE_COMPRESSED )
			{
				DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, "Copying Compressed 8 BPP Imagery." );
				return( Copy8BPPCompressedImageTo8BPPBuffer( hImage, pDestBuf, usDestWidth, usDestHeight, usX, usY, srcRect ) );
			}
		#endif

		// Default do here
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, "Copying 8 BPP Imagery." );
		return ( Copy8BPPImageTo8BPPBuffer( hImage, pDestBuf, usDestWidth, usDestHeight, usX, usY, srcRect ) );

	}

	if ( hImage->ubBitDepth == 8 && fBufferType == BUFFER_16BPP )
	{
		#ifndef NO_ZLIB_COMPRESSION
			if ( hImage->fFlags & IMAGE_COMPRESSED )
			{
				DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Copying Compressed 8 BPP Imagery to 16BPP Buffer." );
				return ( Copy8BPPCompressedImageTo16BPPBuffer( hImage, pDestBuf, usDestWidth, usDestHeight, usX, usY, srcRect ) );
			}
		#endif

		// Default do here
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Copying 8 BPP Imagery to 16BPP Buffer." );
		return ( Copy8BPPImageTo16BPPBuffer( hImage, pDestBuf, usDestWidth, usDestHeight, usX, usY, srcRect ) );

	}


	if ( hImage->ubBitDepth == 16 && fBufferType == BUFFER_16BPP )
	{
		#ifndef NO_ZLIB_COMPRESSION
			if ( hImage->fFlags & IMAGE_COMPRESSED )
			{
				DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Automatically Copying Compressed 16 BPP Imagery." );
				return( Copy16BPPCompressedImageTo16BPPBuffer( hImage, pDestBuf, usDestWidth, usDestHeight, usX, usY, srcRect ) );
			}
		#endif

			DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Automatically Copying 16 BPP Imagery." );
		return( Copy16BPPImageTo16BPPBuffer( hImage, pDestBuf, usDestWidth, usDestHeight, usX, usY, srcRect ) );
	}

	if ( hImage->ubBitDepth == 24 && fBufferType == BUFFER_16BPP )
	{
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Copying 24 BPP Imagery to 16BPP Buffer." );
        AssertMsg(false,"not yet implemented");
		return( FALSE );
	}

	if ( hImage->ubBitDepth == 32 && fBufferType == BUFFER_16BPP )
	{
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Copying 32 BPP Imagery to 16BPP Buffer." );
		return Blt32BPPTo16BPPTrans((UINT16*)pDestBuf, usDestWidth * sizeof(UINT16), hImage->p32BPPData, usDestWidth*sizeof(UINT32), 0,0,0,0,usDestWidth, usDestHeight);
	}

	return( FALSE );

}


#ifndef NO_ZLIB_COMPRESSION

BOOLEAN Copy8BPPCompressedImageTo8BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect )
{
	UINT32	uiNumLines;
	UINT32	uiLineSize;
	UINT32		uiCnt;

	UINT8 *	pDest;
	UINT32	uiDestStart;

	UINT8 *	pScanLine;

	PTR			pDecompPtr;
	UINT32	uiDecompressed;

	// Assertions
	Assert( hImage != NULL );
	Assert( hImage->pCompressedImageData != NULL );

	// Validations
	CHECKF( usX >= 0 );
	CHECKF( usX < usDestWidth );
	CHECKF( usY >= 0 );
	CHECKF( usY < usDestHeight );
	CHECKF( srcRect->iRight > srcRect->iLeft );
	CHECKF( srcRect->iBottom > srcRect->iTop );

	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "8BPP to 8BPP Compressed Blitter Called!" );
	// determine where to start Copying and rectangle size
	uiDestStart = usY * usDestWidth + usX;
	uiNumLines = srcRect->iBottom - srcRect->iTop;
	uiLineSize = srcRect->iRight - srcRect->iLeft;

	Assert( usDestWidth >= uiLineSize );
	Assert( usDestHeight >= uiNumLines );

	pDest = (UINT8 *) pDestBuf + uiDestStart;

	// Copying a portion of a compressed image is rather messy
	// because we have to decompress past all the data we want
	// to skip.

	// To keep memory requirements small and regular, we will
	// decompress one scanline at a time even if none of the data will
	// be blitted (but stop when the bottom line of the rectangle
	// to blit has been done).

	// initialize the decompression routines
	pDecompPtr = DecompressInit( hImage->pCompressedImageData, hImage->usWidth * hImage->usHeight );
	CHECKF( pDecompPtr );

	// Allocate memory for one scanline
	pScanLine = (UINT8*) MemAlloc( hImage->usWidth );
	CHECKF( pScanLine );
	memset( pScanLine, 0, hImage->usWidth );

	// go past all the scanlines we don't need to process
	for (uiCnt = 0; uiCnt < (UINT32) srcRect->iTop; uiCnt++)
	{
		uiDecompressed = Decompress( pDecompPtr, pScanLine, hImage->usWidth );
		Assert( uiDecompressed == hImage->usWidth );
	}

	// now we start Copying
	for (uiCnt = 0; uiCnt < uiNumLines - 1; uiCnt++)
	{
		// decompress a scanline
		uiDecompressed = Decompress( pDecompPtr, pScanLine, hImage->usWidth );
		Assert( uiDecompressed == hImage->usWidth );
		// and blit
//		memcpy( pDest, pScanLine + srcRect->iLeft, uiLineSize );
		pDest += usDestWidth;
	}
	// decompress the last scanline and blit
	uiDecompressed = Decompress( pDecompPtr, pScanLine, hImage->usWidth );
	Assert( uiDecompressed == hImage->usWidth );
//	memcpy( pDest, pScanLine + srcRect->iLeft, uiLineSize );

	DecompressFini( pDecompPtr );
	return( TRUE );
}

BOOLEAN Copy8BPPCompressedImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect )
{
	UINT32		uiNumLines;
	UINT32		uiLineSize;
	UINT32		uiLine;
	UINT32		uiCol;

	UINT16 *	pDest;
	UINT16 *	pDestTemp;
	UINT32		uiDestStart;

	UINT8 *		pScanLine;
	UINT8 *		pScanLineTemp;

	PTR				pDecompPtr;
	UINT32		uiDecompressed;

	UINT16 *	p16BPPPalette;

	// Assertions
	Assert( hImage != NULL );
	Assert( hImage->pCompressedImageData != NULL );
	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Start check" );
	// Validations
	CHECKF( usX >= 0 );
	CHECKF( usX < usDestWidth );
	CHECKF( usY >= 0 );
	CHECKF( usY < usDestHeight );
	CHECKF( srcRect->iRight > srcRect->iLeft );
	CHECKF( srcRect->iBottom > srcRect->iTop );
	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "End check" );
	p16BPPPalette = hImage->pui16BPPPalette;

	// determine where to start Copying and rectangle size
	uiDestStart = usY * usDestWidth + usX;
	uiNumLines = srcRect->iBottom - srcRect->iTop;
	uiLineSize = srcRect->iRight - srcRect->iLeft;

	Assert( usDestWidth >= uiLineSize );
	Assert( usDestHeight >= uiNumLines );

	pDest = (UINT16 *) pDestBuf;
	pDest += uiDestStart;
	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, String( "Start Copying at %p", pDest ) );

	// Copying a portion of a compressed image is rather messy
	// because we have to decompress past all the data we want
	// to skip.

	// To keep memory requirements small and regular, we will
	// decompress one scanline at a time even if none of the data will
	// be blitted (but stop when the bottom line of the rectangle
	// to blit has been done).

	// initialize the decompression routines
	pDecompPtr = DecompressInit( hImage->pCompressedImageData, hImage->usWidth * hImage->usHeight );
	CHECKF( pDecompPtr );

	// Allocate memory for one scanline
	pScanLine = (UINT8*) MemAlloc( hImage->usWidth );
	CHECKF( pScanLine );
	memset( pScanLine, 0, hImage->usWidth );

	// go past all the scanlines we don't need to process
	for (uiLine = 0; uiLine < (UINT32) srcRect->iTop; uiLine++)
	{
		DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Skipping scanline" );
		uiDecompressed = Decompress( pDecompPtr, pScanLine, hImage->usWidth );
		Assert( uiDecompressed == hImage->usWidth );
	}

	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, "Actually Copying" );
	// now we start Copying
	for (uiLine = 0; uiLine < uiNumLines - 1; uiLine++)
	{
		// decompress a scanline
		uiDecompressed = Decompress( pDecompPtr, pScanLine, hImage->usWidth );
		Assert( uiDecompressed == hImage->usWidth );

		// set pointers and blit
		pDestTemp = pDest;
		pScanLineTemp = pScanLine + srcRect->iLeft;
		for (uiCol = 0; uiCol < uiLineSize; uiCol++ )
		{
			*pDestTemp = p16BPPPalette[ *pScanLineTemp ];
			pDestTemp++;
			pScanLineTemp++;
		}
		pDest += usDestWidth;
	}

	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, String( "End Copying at %p", pDest ) );

	DecompressFini( pDecompPtr );
	return( TRUE );
}

BOOLEAN Copy16BPPCompressedImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect )
{
	// 16BPP Compressed image has not been implemented yet
	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_2, "16BPP Compressed imagery blitter has not been implemented yet." );
	return( FALSE );
}
#endif //NO_ZLIB_COMPRESSION


BOOLEAN Copy8BPPImageTo8BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect )
{
	UINT32 uiSrcStart, uiDestStart, uiNumLines, uiLineSize;
	UINT32 cnt;
	UINT8 *pDest, *pSrc;

	// Assertions
	Assert( hImage != NULL );
	Assert( hImage->p16BPPData != NULL );

	// Validations
	CHECKF( usX >= 0 );
	CHECKF( usX < usDestWidth );
	CHECKF( usY >= 0 );
	CHECKF( usY < usDestHeight );
	CHECKF( srcRect->iRight > srcRect->iLeft );
	CHECKF( srcRect->iBottom > srcRect->iTop );

	// Determine memcopy coordinates
	uiSrcStart = srcRect->iTop * hImage->usWidth + srcRect->iLeft;
	uiDestStart = usY * usDestWidth + usX;
	uiNumLines = ( srcRect->iBottom - srcRect->iTop ) + 1;
	uiLineSize = ( srcRect->iRight - srcRect->iLeft ) + 1;

	Assert( usDestWidth >= uiLineSize );
	Assert( usDestHeight >= uiNumLines );

	// Copy line by line
	pDest = ( UINT8*)pDestBuf + uiDestStart;
	pSrc =	hImage->p8BPPData + uiSrcStart;

	for( cnt = 0; cnt < uiNumLines-1; cnt++ )
	{
		memcpy( pDest, pSrc, uiLineSize );
		pDest += usDestWidth;
		pSrc	+= hImage->usWidth;
	}
	// Do last line
	memcpy( pDest, pSrc, uiLineSize );

	return( TRUE );

}

BOOLEAN Copy16BPPImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect )
{
	UINT32 uiSrcStart, uiDestStart, uiNumLines, uiLineSize;
	UINT32 cnt;
	UINT16 *pDest, *pSrc;

	Assert( hImage != NULL );
	Assert( hImage->p16BPPData != NULL );

	// Validations
	CHECKF( usX >= 0 );
	CHECKF( usX < hImage->usWidth );
	CHECKF( usY >= 0 );
	CHECKF( usY < hImage->usHeight );
	CHECKF( srcRect->iRight > srcRect->iLeft );
	CHECKF( srcRect->iBottom > srcRect->iTop );

	// Determine memcopy coordinates
	uiSrcStart = srcRect->iTop * hImage->usWidth + srcRect->iLeft;
	uiDestStart = usY * usDestWidth + usX;
	uiNumLines = ( srcRect->iBottom - srcRect->iTop ) + 1;
	uiLineSize = ( srcRect->iRight - srcRect->iLeft ) + 1;

	CHECKF( usDestWidth >= uiLineSize );
	CHECKF( usDestHeight >= uiNumLines );

	// Copy line by line
	pDest = ( UINT16*)pDestBuf + uiDestStart;
	pSrc =	hImage->p16BPPData + uiSrcStart;

	for( cnt = 0; cnt < uiNumLines-1; cnt++ )
	{
		memcpy( pDest, pSrc, uiLineSize * 2 );
		pDest += usDestWidth;
		pSrc	+= hImage->usWidth;
	}
	// Do last line
	memcpy( pDest, pSrc, uiLineSize * 2 );

	return( TRUE );

}

BOOLEAN Extract8BPPCompressedImageToBuffer( HIMAGE hImage, BYTE *pDestBuf )
{

	return( FALSE );
}

BOOLEAN Extract16BPPCompressedImageToBuffer( HIMAGE hImage, BYTE *pDestBuf )
{

	return( FALSE );
}


BOOLEAN Copy8BPPImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect )
{
	UINT32 uiSrcStart, uiDestStart, uiNumLines, uiLineSize;
	UINT32 rows, cols;
	UINT8	*pSrc, *pSrcTemp;
	UINT16 *pDest, *pDestTemp;
	UINT16 *p16BPPPalette;


	p16BPPPalette = hImage->pui16BPPPalette;

	// Assertions
	Assert( p16BPPPalette != NULL );
	Assert( hImage != NULL );

	// Validations
	CHECKF( hImage->p16BPPData != NULL );
	CHECKF( usX >= 0 );
	CHECKF( usX < usDestWidth );
	CHECKF( usY >= 0 );
	CHECKF( usY < usDestHeight );
	CHECKF( srcRect->iRight > srcRect->iLeft );
	CHECKF( srcRect->iBottom > srcRect->iTop );

	// Determine memcopy coordinates
	uiSrcStart = srcRect->iTop * hImage->usWidth + srcRect->iLeft;
	uiDestStart = usY * usDestWidth + usX;
	uiNumLines = ( srcRect->iBottom - srcRect->iTop );
	uiLineSize = ( srcRect->iRight - srcRect->iLeft );

	CHECKF( usDestWidth >= uiLineSize );
	CHECKF( usDestHeight >= uiNumLines );

	// Convert to Pixel specification
	pDest = ( UINT16*)pDestBuf + uiDestStart;
	pSrc =	hImage->p8BPPData + uiSrcStart;
	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, String( "Start Copying at %p", pDest ) );

	// For every entry, look up into 16BPP palette
	for( rows = 0; rows < uiNumLines-1; rows++ )
	{
		pDestTemp = pDest;
		pSrcTemp = pSrc;

		for ( cols = 0; cols < uiLineSize; cols++ )
		{
			*pDestTemp = p16BPPPalette[ *pSrcTemp ];
			pDestTemp++;
			pSrcTemp++;
		}

		pDest += usDestWidth;
		pSrc	+= hImage->usWidth;
	}
	// Do last line
	DbgMessage( TOPIC_HIMAGE, DBG_LEVEL_3, String( "End Copying at %p", pDest ) );

	return( TRUE );

}

UINT16 *Create16BPPPalette( SGPPaletteEntry *pPalette )
{
	UINT16 *p16BPPPalette, r16, g16, b16, usColor;
	UINT32 cnt;
	UINT8	r,g,b;

	Assert( pPalette != NULL );

	p16BPPPalette = (UINT16 *) MemAlloc( sizeof( UINT16 ) * 256 );
	memset( p16BPPPalette, 0, sizeof( UINT16 ) * 256 );

	for ( cnt = 0; cnt < 256; cnt++ )
	{
		r = pPalette[ cnt ].peRed;
		g = pPalette[ cnt ].peGreen;
		b = pPalette[ cnt ].peBlue;

		if(gusRedShift < 0)
			r16=((UINT16)r>>abs(gusRedShift));
		else
			r16=((UINT16)r<<gusRedShift);

		if(gusGreenShift < 0)
			g16=((UINT16)g>>abs(gusGreenShift));
		else
			g16=((UINT16)g<<gusGreenShift);


		if(gusBlueShift < 0)
			b16=((UINT16)b>>abs(gusBlueShift));
		else
			b16=((UINT16)b<<gusBlueShift);

		usColor = (r16&gusRedMask)|(g16&gusGreenMask)|(b16&gusBlueMask);

		if(usColor==0)
		{
			if((r+g+b)!=0)
				usColor=BLACK_SUBSTITUTE | gusAlphaMask;
		}
		else
			usColor |= gusAlphaMask;

		p16BPPPalette[ cnt ] = usColor;
	}

	return( p16BPPPalette );
}

/**********************************************************************************************
 Create16BPPPaletteShaded

	Creates an 8 bit to 16 bit palette table, and modifies the colors as it builds.

	Parameters:
		rscale, gscale, bscale:
				Color mode: Percentages (255=100%) of color to translate into destination palette.
				Mono mode:	Color for monochrome palette.
		mono:
				TRUE or FALSE to create a monochrome palette. In mono mode, Luminance values for
				colors are calculated, and the RGB color is shaded according to each pixel's brightness.

	This can be used in several ways:

	1) To "brighten" a palette, pass down RGB values that are higher than 100% ( > 255) for all
			three. mono=FALSE.
	2) To "darken" a palette, do the same with less than 100% ( < 255) values. mono=FALSE.

	3) To create a "glow" palette, select mono=TRUE, and pass the color in the RGB parameters.

	4) For gamma correction, pass in weighted values for each color.

**********************************************************************************************/
UINT16 *Create16BPPPaletteShaded( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, UINT32 bscale, BOOLEAN mono)
{
	UINT16 *p16BPPPalette, r16, g16, b16, usColor;
	UINT32 cnt, lumin;
	UINT32 rmod, gmod, bmod;
	UINT8	r,g,b;

	Assert( pPalette != NULL );

	p16BPPPalette = (UINT16 *) MemAlloc( sizeof( UINT16 ) * 256 );
	memset( p16BPPPalette, 0, sizeof( UINT16 ) * 256 );

	for ( cnt = 0; cnt < 256; cnt++ )
	{
		if(mono)
		{
			lumin=(pPalette[ cnt ].peRed*299/1000)+ (pPalette[ cnt ].peGreen*587/1000)+(pPalette[ cnt ].peBlue*114/1000);
			rmod=(rscale*lumin)/256;
			gmod=(gscale*lumin)/256;
			bmod=(bscale*lumin)/256;
		}
		else
		{
			rmod = (rscale*pPalette[ cnt ].peRed/256);
			gmod = (gscale*pPalette[ cnt ].peGreen/256);
			bmod = (bscale*pPalette[ cnt ].peBlue/256);
		}

		r = (UINT8)__min(rmod, 255);
		g = (UINT8)__min(gmod, 255);
		b = (UINT8)__min(bmod, 255);

		if(gusRedShift < 0)
			r16=((UINT16)r>>(-gusRedShift));
		else
			r16=((UINT16)r<<gusRedShift);

		if(gusGreenShift < 0)
			g16=((UINT16)g>>(-gusGreenShift));
		else
			g16=((UINT16)g<<gusGreenShift);


		if(gusBlueShift < 0)
			b16=((UINT16)b>>(-gusBlueShift));
		else
			b16=((UINT16)b<<gusBlueShift);

		// Prevent creation of pure black color
		usColor	= (r16&gusRedMask)|(g16&gusGreenMask)|(b16&gusBlueMask);

		if(usColor==0)
		{
			if((r+g+b)!=0)
				usColor=BLACK_SUBSTITUTE | gusAlphaMask;
		}
		else
			usColor |= gusAlphaMask;

		p16BPPPalette[ cnt ] = usColor;
	}
	return( p16BPPPalette );
}

// Convert from RGB to 16 bit value
UINT16 Get16BPPColor( UINT32 RGBValue )
{
	UINT16 r16, g16, b16, usColor = 0;
	UINT8	r,g,b;

	r = SGPGetRValue( RGBValue );
	g = SGPGetGValue( RGBValue );
	b = SGPGetBValue( RGBValue );

	if(gusRedShift < 0)
		r16=((UINT16)r>>abs(gusRedShift));
	else
		r16=((UINT16)r<<gusRedShift);

	if(gusGreenShift < 0)
		g16=((UINT16)g>>abs(gusGreenShift));
	else
		g16=((UINT16)g<<gusGreenShift);


	if(gusBlueShift < 0)
		b16=((UINT16)b>>abs(gusBlueShift));
	else
		b16=((UINT16)b<<gusBlueShift);

	usColor=(r16&gusRedMask)|(g16&gusGreenMask)|(b16&gusBlueMask);

	// if our color worked out to absolute black, and the original wasn't
	// absolute black, convert it to a VERY dark grey to avoid transparency
	// problems

	if(usColor==0)
	{
		if(RGBValue!=0)
			usColor=BLACK_SUBSTITUTE | gusAlphaMask;
	}
	else
		usColor	|=	gusAlphaMask;

	return(usColor);
}


// Convert from 16 BPP to RGBvalue
UINT32 GetRGBColor( UINT16 Value16BPP )
{
	UINT16 r16, g16, b16;
	UINT32 r,g,b,val;

	r16 = Value16BPP & gusRedMask;
	g16 = Value16BPP & gusGreenMask;
	b16 = Value16BPP & gusBlueMask;

	if(gusRedShift < 0)
		r=((UINT32)r16<<abs(gusRedShift));
	else
		r=((UINT32)r16>>gusRedShift);

	if(gusGreenShift < 0)
		g=((UINT32)g16<<abs(gusGreenShift));
	else
		g=((UINT32)g16>>gusGreenShift);

	if(gusBlueShift < 0)
		b=((UINT32)b16<<abs(gusBlueShift));
	else
		b=((UINT32)b16>>gusBlueShift);

	r &= 0x000000ff;
	g &= 0x000000ff;
	b &= 0x000000ff;

	val = FROMRGB(r,g,b);

	return(val);
}

//*****************************************************************************
//
// ConvertToPaletteEntry
//
// Parameter List : Converts from RGB to SGPPaletteEntry
//
// Return Value	pointer to the SGPPaletteEntry
//
// Modification History :
// Dec 15th 1996->modified for use by Wizardry
//
//*****************************************************************************

SGPPaletteEntry *ConvertRGBToPaletteEntry(UINT8 sbStart, UINT8 sbEnd, UINT8 *pOldPalette)
{
	UINT16 Index;
	SGPPaletteEntry *pPalEntry;
	SGPPaletteEntry *pInitEntry;

	pPalEntry = (SGPPaletteEntry *)MemAlloc(sizeof(SGPPaletteEntry) * 256);
	memset( pPalEntry, 0, sizeof(SGPPaletteEntry) * 256 );
	pInitEntry = pPalEntry;

	DbgMessage(TOPIC_HIMAGE, DBG_LEVEL_0, "Converting RGB palette to SGPPaletteEntry");

	for(Index=0; Index <= (sbEnd-sbStart);Index++)
	{
		pPalEntry->peRed = *(pOldPalette + (Index*3));
		pPalEntry->peGreen = *(pOldPalette + (Index*3) + 1);
		pPalEntry->peBlue = *(pOldPalette + (Index*3) + 2);
		pPalEntry->peFlags = 0;
		pPalEntry++;
	}
	return pInitEntry;
}

BOOLEAN GetETRLEImageData( HIMAGE hImage, ETRLEData *pBuffer )
{
	// Assertions
	Assert( hImage != NULL );
	Assert( pBuffer != NULL );

	// Create memory for data
	pBuffer->usNumberOfObjects = hImage->usNumberOfObjects;

	// Create buffer for objects
	pBuffer->pETRLEObject = (ETRLEObject *) MemAlloc( sizeof( ETRLEObject ) * pBuffer->usNumberOfObjects );
	if(!pBuffer->pETRLEObject)
	{
		return false;
	}
	CHECKF( pBuffer->pETRLEObject != NULL );
	memset( pBuffer->pETRLEObject, 0, sizeof( ETRLEObject ) * pBuffer->usNumberOfObjects );

	// Copy into buffer
	memcpy( pBuffer->pETRLEObject, hImage->pETRLEObject, sizeof( ETRLEObject ) * pBuffer->usNumberOfObjects );

	// Allocate memory for pixel data
	pBuffer->pPixData = MemAlloc( hImage->uiSizePixData );
	if(!pBuffer->pPixData)
	{
		return false;
	}
	CHECKF( pBuffer->pPixData != NULL );
	memset( pBuffer->pPixData, 0, hImage->uiSizePixData );

	pBuffer->uiSizePixData = hImage->uiSizePixData;

	// Copy into buffer
	memcpy( pBuffer->pPixData, hImage->pPixData8, pBuffer->uiSizePixData );

	return( TRUE );
}

void ConvertRGBDistribution565To555( UINT16 * p16BPPData, UINT32 uiNumberOfPixels )
{
	UINT16 *	pPixel;
	UINT32		uiLoop;

	SplitUINT32		Pixel;

	pPixel = p16BPPData;
	for (uiLoop = 0; uiLoop < uiNumberOfPixels; uiLoop++)
	{
		// If the pixel is completely black, don't bother converting it -- DB
		if(*pPixel!=0)
		{
			// we put the 16 pixel bits in the UPPER word of uiPixel, so that we can
			// right shift the blue value (at the bottom) into the LOWER word to protect it
			Pixel.usHigher = *pPixel;
			Pixel.uiValue >>= 5;
			// get rid of the least significant bit of green
			Pixel.usHigher >>= 1;
			// now shift back into the upper word
			Pixel.uiValue <<= 5;
			// and copy back
			*pPixel = Pixel.usHigher | gusAlphaMask;
		}
		pPixel++;
	}
}

void ConvertRGBDistribution565To655( UINT16 * p16BPPData, UINT32 uiNumberOfPixels )
{
	UINT16 *	pPixel;
	UINT32		uiLoop;

	SplitUINT32		Pixel;

	pPixel = p16BPPData;
	for (uiLoop = 0; uiLoop < uiNumberOfPixels; uiLoop++)
	{
		// we put the 16 pixel bits in the UPPER word of uiPixel, so that we can
		// right shift the blue value (at the bottom) into the LOWER word to protect it
		Pixel.usHigher = *pPixel;
		Pixel.uiValue >>= 5;
		// get rid of the least significant bit of green
		Pixel.usHigher >>= 1;
		// shift to the right some more...
		Pixel.uiValue >>= 5;
		// so we can left-shift the red value alone to give it an extra bit
		Pixel.usHigher <<= 1;
		// now shift back and copy
		Pixel.uiValue <<= 10;
		*pPixel = Pixel.usHigher;
		pPixel++;
	}
}

void ConvertRGBDistribution565To556( UINT16 * p16BPPData, UINT32 uiNumberOfPixels )
{
	UINT16 *	pPixel;
	UINT32		uiLoop;

	SplitUINT32		Pixel;

	pPixel = p16BPPData;
	for (uiLoop = 0; uiLoop < uiNumberOfPixels; uiLoop++)
	{
		// we put the 16 pixel bits in the UPPER word of uiPixel, so that we can
		// right shift the blue value (at the bottom) into the LOWER word to protect it
		Pixel.usHigher = *pPixel;
		Pixel.uiValue >>= 5;
		// get rid of the least significant bit of green
		Pixel.usHigher >>= 1;
		// shift back into the upper word
		Pixel.uiValue <<= 5;
		// give blue an extra bit (blank in the least significant spot)
		Pixel.usHigher <<= 1;
		// copy back
		*pPixel = Pixel.usHigher;
		pPixel++;
	}
}

void ConvertRGBDistribution565ToAny( UINT16 * p16BPPData, UINT32 uiNumberOfPixels )
{
	UINT16 *	pPixel;
	UINT32		uiRed, uiGreen, uiBlue, uiTemp, uiLoop;

	pPixel = p16BPPData;
	for (uiLoop = 0; uiLoop < uiNumberOfPixels; uiLoop++)
	{
		// put the 565 RGB 16-bit value into a 32-bit RGB value
		uiRed = (*pPixel) >> 11;
		uiGreen = (*pPixel & 0x07E0) >> 5;
		uiBlue = (*pPixel & 0x001F);
		uiTemp = FROMRGB(uiRed,uiGreen,uiBlue);
		// then convert the 32-bit RGB value to whatever 16 bit format is used
		*pPixel = Get16BPPColor( uiTemp );
		pPixel++;
	}
}
