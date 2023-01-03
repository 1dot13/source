#ifndef __IMAGE_H
#define __IMAGE_H

#include "MemMan.h"
#include "imgfmt.h"

// The HIMAGE module provides a common interface for managing image data. This module
// includes:
// - A set of data structures representing image data. Data can be 8 or 16 bpp and/or
//   compressed
// - A set of file loaders which load specific file formats into the internal data format
// - A set of blitters which blt the data to memory
// - A comprehensive automatic blitter which blits the appropriate type based on the
//   image header.


// Defines for type of file readers
#define PCX_FILE_READER					0x1
#define TGA_FILE_READER					0x2
#define STCI_FILE_READER				0x4
#define TRLE_FILE_READER				0x8
#define PNG_FILE_READER					0x10
#define JPC_FILE_READER					0x20
#define UNKNOWN_FILE_READER			0x200

// Defines for buffer bit depth
#define BUFFER_8BPP							0x1
#define BUFFER_16BPP						0x2

// Defines for image charactoristics
#define IMAGE_COMPRESSED				0x0001
#define IMAGE_TRLECOMPRESSED		0x0002
#define IMAGE_PALETTE						0x0004
#define IMAGE_BITMAPDATA				0x0008
#define IMAGE_APPDATA						0x0010
#define IMAGE_ALLIMAGEDATA			0x000C
#define IMAGE_ALLDATA						0x001C

// Palette structure, mimics that of Win32
typedef struct tagSGPPaletteEntry
{  
  UINT8 peRed; 
  UINT8 peGreen; 
  UINT8 peBlue; 
  UINT8 peFlags; 

} SGPPaletteEntry; 

#define AUX_FULL_TILE					0x01
#define AUX_ANIMATED_TILE			0x02
#define AUX_DYNAMIC_TILE			0x04
#define AUX_INTERACTIVE_TILE	0x08
#define AUX_IGNORES_HEIGHT		0x10
#define AUX_USES_LAND_Z				0x20

typedef struct
{
	UINT8		ubWallOrientation;
	UINT8		ubNumberOfTiles;
	UINT16	usTileLocIndex;
	UINT8		ubUnused1[3];
	UINT8		ubCurrentFrame;
	UINT8		ubNumberOfFrames;
	UINT8		fFlags;
	UINT8		ubUnused[6];
} AuxObjectData;

typedef struct
{
	INT8		bTileOffsetX;
	INT8		bTileOffsetY;
} RelTileLoc; // relative tile location

// TRLE subimage structure, mirroring that of ST(C)I
typedef struct tagETRLEObject
{	
	UINT32			uiDataOffset;
	UINT32			uiDataLength;
	INT16				sOffsetX;
	INT16				sOffsetY;
	UINT16			usHeight;
	UINT16			usWidth;
} ETRLEObject;

typedef struct tagETRLEData
{
	PTR								pPixData;
	UINT32						uiSizePixData;
	ETRLEObject *			pETRLEObject;
	UINT16						usNumberOfObjects;
} ETRLEData;

// Image header structure
typedef struct
{
	UINT16					usWidth;
	UINT16					usHeight;
	UINT8					ubBitDepth;
	UINT16					fFlags;
	SGPFILENAME				ImageFile;
	UINT32					iFileLoader;
	SGPPaletteEntry*		pPalette;
	UINT16*					pui16BPPPalette;
	UINT8*					pAppData;
	UINT32					uiAppDataSize;
	// This union is used to describe each data type and is flexible to include the
	// data strucutre of the compresssed format, once developed.
	union 
	{
		PTR				pImageData;
		PTR				pCompressedImageData;
		UINT8*			p8BPPData;
		UINT16*			p16BPPData;
		UINT32*			p32BPPData;
		struct
		{
			UINT8*			pPixData8;
			UINT32			uiSizePixData;
			ETRLEObject*	pETRLEObject;
			UINT16			usNumberOfObjects;
		};
	};

} image_type, *HIMAGE;


#define SGPGetRValue(rgb)   ((BYTE) (rgb))  
#define SGPGetBValue(rgb)   ((BYTE) ((rgb) >> 16))  
#define SGPGetGValue(rgb)   ((BYTE) (((UINT16) (rgb)) >> 8))  

// *****************************************************************************
//
// Function prototypes
// 
// *****************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

namespace ImageFileType
{
	enum TestOrder
	{
		DEFAULT = 0,  // check extension
		JPC,          // ignore file extension, test only *.jpc
		JPC_FALLBACK, // test *.jpc, fallback to original type
		PNG,          // ignore file extension, test only *.png
		PNG_FALLBACK, // test *.png, fallback to original type
	};
};

// This function will return NULL if it fails, and call SetLastError() to set
// error information
HIMAGE CreateImage( SGPFILENAME ImageFile, UINT16 fContents, ImageFileType::TestOrder order = ImageFileType::JPC_FALLBACK);

// This function destroys the HIMAGE structure as well as its contents
BOOLEAN DestroyImage( HIMAGE hImage );

// This function releases data allocated to various parts of the image based
// on the contents flags passed as a parameter.  If a contents flag is given
// and the image does not contain that data, no error is raised
BOOLEAN ReleaseImageData( HIMAGE hImage, UINT16 fContents );

// This function will attept to Load data from an existing image object's filename
// In this way, dynamic loading of image data can be done
BOOLEAN LoadImageData( HIMAGE hImage, UINT16 fContents );

// This function will run the appropriate copy function based on the type of HIMAGE object
BOOLEAN CopyImageToBuffer( HIMAGE hImage, UINT32 fBufferType, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect );

// The following blitters are used by the function above as well as clients
#ifndef NO_ZLIB_COMPRESSION
	BOOLEAN Copy8BPPCompressedImageTo8BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect );
	BOOLEAN Copy8BPPCompressedImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect );
	BOOLEAN Copy16BPPCompressedImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect );
	// This function will extract a compressed image into a non-compressed buffer
	BOOLEAN Extract8BPPCompressedImageToBuffer( HIMAGE hImage, BYTE *pDestBuf );
	BOOLEAN Extract16BPPCompressedImageToBuffer( HIMAGE hImage, BYTE *pDestBuf );
#endif

BOOLEAN Copy8BPPImageTo8BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect );
BOOLEAN Copy8BPPImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect );
BOOLEAN Copy16BPPImageTo16BPPBuffer( HIMAGE hImage, BYTE *pDestBuf, UINT16 usDestWidth, UINT16 usDestHeight, UINT16 usX, UINT16 usY, SGPRect *srcRect );


// This function will create a buffer in memory of ETRLE data, excluding palette
BOOLEAN GetETRLEImageData( HIMAGE hImage, ETRLEData *pBuffer );

// UTILITY FUNCTIONS

// Used to create a 16BPP Palette from an 8 bit palette, found in himage.c
UINT16 *Create16BPPPaletteShaded( SGPPaletteEntry *pPalette, UINT32 rscale, UINT32 gscale, UINT32 bscale, BOOLEAN mono);
UINT16 *Create16BPPPalette( SGPPaletteEntry *pPalette );
UINT16 Get16BPPColor( UINT32 RGBValue );
UINT32 GetRGBColor( UINT16 Value16BPP );
SGPPaletteEntry *ConvertRGBToPaletteEntry(UINT8 sbStart, UINT8 sbEnd, UINT8 *pOldPalette);

extern UINT16 gusAlphaMask;
extern UINT16 gusRedMask;
extern UINT16 gusGreenMask;
extern UINT16 gusBlueMask;
extern INT16  gusRedShift;
extern INT16  gusBlueShift;
extern INT16  gusGreenShift;

// used to convert 565 RGB data into different bit-formats
void ConvertRGBDistribution565To555( UINT16 * p16BPPData, UINT32 uiNumberOfPixels );
void ConvertRGBDistribution565To655( UINT16 * p16BPPData, UINT32 uiNumberOfPixels );
void ConvertRGBDistribution565To556( UINT16 * p16BPPData, UINT32 uiNumberOfPixels );
void ConvertRGBDistribution565ToAny( UINT16 * p16BPPData, UINT32 uiNumberOfPixels );

#ifdef __cplusplus
}
#endif

#endif
