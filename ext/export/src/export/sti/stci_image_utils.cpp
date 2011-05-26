#include "stci_image_utils.h"
#include "himage.h"

#include <cassert>

const int iCOMPRESS_TRANSPARENT = 0x80;
const int iCOMPRESS_RUN_MASK    = 0x7F;


UINT32 ja2xp::UnpackETRLEImageToBuffer(UINT8 *pBuffer, image_type *pImage, UINT16 usETRLEIndex)
{
	if(!pImage || !pBuffer)
	{
		return 0;
	}

	ETRLEObject *pETRLEObject = &(pImage->pETRLEObject[usETRLEIndex]);
	if(!pETRLEObject)
	{
		return 0;
	}

	UINT8 *pCurrent;
	pCurrent = &((UINT8 *)pImage->pPixData8)[pETRLEObject->uiDataOffset];
	unsigned int uiImageSize = pETRLEObject->usHeight * pETRLEObject->usWidth;

	unsigned int uiBufferPos = 0; 
	unsigned char ubCount;
	while(uiBufferPos < uiImageSize)
	{
		ubCount = *pCurrent & iCOMPRESS_RUN_MASK;
		if(*pCurrent & iCOMPRESS_TRANSPARENT)
		{
			pCurrent++;
			uiBufferPos += ubCount;
		}
		else
		{
			pCurrent++;
			memcpy(&pBuffer[uiBufferPos], pCurrent, ubCount);
			pCurrent += ubCount;
			uiBufferPos += ubCount;
		}
	}
	return uiBufferPos;
}

UINT32 ja2xp::UnpackETRLEImageToRGBABuffer(UINT8 *pBuffer, image_type *pImage, UINT16 usETRLEIndex)
{
	if(!pImage || !pBuffer)
	{
		return 0;
	}

	ETRLEObject *pETRLEObject = &(pImage->pETRLEObject[usETRLEIndex]);
	if(!pETRLEObject)
	{
		return 0;
	}

	SGPPaletteEntry* palette = pImage->pPalette;
	if(!palette)
	{
		return 0;
	}

	UINT8 *pCurrent;
	pCurrent = &((UINT8 *)pImage->pPixData8)[pETRLEObject->uiDataOffset];
	unsigned int uiImageSize = pETRLEObject->usHeight * pETRLEObject->usWidth;

	unsigned int uiBufferPos = 0; 
	unsigned char ubCount;
	while(uiBufferPos < (4*uiImageSize))
	{
		ubCount = *pCurrent & iCOMPRESS_RUN_MASK;
		if(*pCurrent & iCOMPRESS_TRANSPARENT)
		{
			pCurrent++;
			uiBufferPos += 4 * (int)ubCount;
			pBuffer += 4 * (int)ubCount;
		}
		else
		{
			pCurrent++;
			int count = ubCount;
			while(count--)
			{
				UINT8 cur = *pCurrent++;
				*pBuffer++ = palette[cur].peRed;
				*pBuffer++ = palette[cur].peGreen;
				*pBuffer++ = palette[cur].peBlue;
				*pBuffer++ = 255; // alpha
			}
			uiBufferPos += 4 * (int)ubCount;
		}
	}
	return uiBufferPos;
}


ja2xp::CIndexedSTIImage::CIndexedSTIImage()
{
	memset(&_header, 0, sizeof(STCIHeader));
}

ja2xp::CIndexedSTIImage::~CIndexedSTIImage()
{
	if(_palette)
	{
		delete[] _palette;
	}
	for(unsigned int i=0; i < _compressed_images.size(); ++i)
	{
		delete[] _compressed_images[i];
	}
}




bool ja2xp::CIndexedSTIImage::SetPalette(SGPPaletteEntry *pPal, int iSize)
{
	if(iSize < 0 ||iSize > 1024)
	{
		return false;
	}
	_pal_size = iSize;
	_palette = new STCIPaletteElement[iSize];
	for(int i=0; i<iSize;++i)
	{
		_palette[i].ubBlue = pPal[i].peBlue;
		_palette[i].ubGreen = pPal[i].peGreen;
		_palette[i].ubRed = pPal[i].peRed;
	}

	return true;
}

bool ja2xp::CIndexedSTIImage::AddCompressedImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y)
{
	if(!data || (data_size < 0) )
	{
		return false;
	}
	STCISubImage subimage;
	subimage.sOffsetX = image_offset_x;
	subimage.sOffsetY = image_offset_y;
	subimage.usWidth = image_width;
	subimage.usHeight = image_height;

	UINT8 *compressed = new UINT8[data_size];
	memcpy(compressed,data,data_size);

	subimage.uiDataLength = data_size;
	_header.uiStoredSize += data_size;
	_images.push_back(subimage);
	_compressed_images.push_back(compressed);
	_header.uiOriginalSize += image_width * image_height;
	_header.Indexed.usNumberOfSubImages += 1;

	return true;
}


bool ja2xp::CIndexedSTIImage::AddImage(UINT8 *data, UINT32 data_size, UINT32 image_width, UINT32 image_height, INT32 image_offset_x, INT32 image_offset_y, UINT8 *original_compressed, UINT32 iriginal_compressed_size)
{
	if(!data || (data_size != image_width*image_height) )
	{
		return false;
	}
	STCISubImage subimage;
	subimage.sOffsetX = image_offset_x;
	subimage.sOffsetY = image_offset_y;
	subimage.usWidth = image_width;
	subimage.usHeight = image_height;

	UINT8 *compressed = new UINT8[2*data_size];
	UINT8 *compressed_start = compressed;
	memset(compressed,0,data_size);
	UINT32 compressed_size = 0;

	unsigned int uiBufferPos = 0;
	
	bool bZeroRun = false;
	UINT8 uiRunLength = 0;
	UINT8 *uiRunStartPosition = data;
	if(*data == 0)
	{
		bZeroRun = true;
	}
	bool done = false;
	UINT32 scanline = 0;
	while(!done)
	{
		if(bZeroRun)
		{
			while((*data == 0) && (uiRunLength < 128) && (uiBufferPos < data_size) && (scanline < image_width))
			{
				data++;
				uiRunLength++;
				uiBufferPos++;
				scanline++;
			}
			uiRunStartPosition = compressed;
			*compressed++ = uiRunLength | iCOMPRESS_TRANSPARENT;
			compressed_size += 1;
		}
		else
		{
			uiRunStartPosition = compressed++;
			while((*data != 0) && (uiRunLength < 128) && (uiBufferPos < data_size) && (scanline < image_width))
			{
				*compressed++ = *data++;
				uiRunLength++;
				uiBufferPos++;
				scanline++;
			}
			*uiRunStartPosition = uiRunLength;
			compressed_size += uiRunLength+1;
		}
		// prepare next run
		uiRunLength = 0;
		bZeroRun = (*data != 0) ? false : true;
		if(scanline == image_width)
		{
			scanline = 0;
			// "close" scanline with a zero
			*compressed++ = 0;
			compressed_size += 1;
		}
		if(uiBufferPos >= data_size)
		{
			done = true;
		}
	}

	subimage.uiDataLength = compressed_size;
	_header.uiStoredSize += compressed_size;
	_images.push_back(subimage);
	_compressed_images.push_back(compressed_start);
	_header.uiOriginalSize += data_size;
	_header.Indexed.usNumberOfSubImages += 1;

	return true;
}

bool ja2xp::CIndexedSTIImage::WriteImage(vfs::tWritableFile* pFile)
{
	if(!pFile || !pFile->openWrite())
	{
		return false;
	}

	strncpy((char*)&(_header.cID[0]),"STCI",4);

	_header.uiOriginalSize = _compressed_images.size() * 640 * 480;
	//_header.uiStoredSize += 64 + 768 + _header.Indexed.usNumberOfSubImages*16; // equal to uiOriginalSize if data uncompressed
	_header.uiTransparentValue = 0;
	_header.fFlags = STCI_ETRLE_COMPRESSED | STCI_INDEXED | STCI_TRANSPARENT;
	_header.usHeight = 480;
	_header.usWidth = 640;

	_header.ubDepth = 8;
	_header.uiAppDataSize = _compressed_images.size() * 16;

	_header.Indexed.ubRedDepth = 8;
	_header.Indexed.ubGreenDepth = 8;
	_header.Indexed.ubBlueDepth = 8;
	_header.Indexed.uiNumberOfColours = 256;
	_header.Indexed.usNumberOfSubImages = _compressed_images.size();

	pFile->write((vfs::Byte*)&_header,STCI_HEADER_SIZE);

	pFile->write((vfs::Byte*)_palette,_pal_size*sizeof(STCIPaletteElement));

	UINT32 offset=0;
	// write sub-image headers
	for(unsigned int i=0; i<_images.size(); ++i)
	{
		_images[i].uiDataOffset = offset;
		pFile->write((vfs::Byte*)&_images[i],STCI_SUBIMAGE_SIZE);
		offset += _images[i].uiDataLength;
	}
	// write sub-images
	for(unsigned int i=0; i<_compressed_images.size(); ++i)
	{
		pFile->write((vfs::Byte*)_compressed_images[i],_images[i].uiDataLength);
	}
	// write application data
	int frames = _compressed_images.size()/8; // 8 = number of directions
	for(unsigned int i=0; i<_compressed_images.size(); ++i)
	{
		AuxObjectData aod;
		memset(&aod,0,sizeof(AuxObjectData));
		if((frames+1) % (i+1) == 0)
		{
			aod.ubNumberOfFrames = frames; 
			aod.fFlags = AUX_ANIMATED_TILE;
		}
		pFile->write((vfs::Byte*)&aod,sizeof(AuxObjectData));
	}
	pFile->close();
	return true;
}

bool ja2xp::CIndexedSTIImage::WriteToHIMAGE(HIMAGE pImage)
{
	if(!pImage)
	{
		return false;
	}
	pImage->usNumberOfObjects = _compressed_images.size();
	pImage->usWidth = 640;
	pImage->usHeight = 480;
	pImage->ubBitDepth = 8;
	pImage->fFlags = STCI_ETRLE_COMPRESSED | STCI_INDEXED | STCI_TRANSPARENT;;
	strncat(pImage->ImageFile,"test",4);
	pImage->iFileLoader = -1;
	pImage->pui16BPPPalette = NULL;


	pImage->pPalette = new SGPPaletteEntry[256];
	for(int j=0; j<256; ++j)
	{
		pImage->pPalette[j].peRed   = _palette[j].ubRed;
		pImage->pPalette[j].peGreen = _palette[j].ubGreen;
		pImage->pPalette[j].peBlue  = _palette[j].ubBlue;
		pImage->pPalette[j].peFlags = 0;
	}

	pImage->pETRLEObject = new ETRLEObject[pImage->usNumberOfObjects];
	UINT32 offset=0; // == size
	for(unsigned int i=0; i<_images.size(); ++i)
	{
		pImage->pETRLEObject[i].sOffsetX = _images[i].sOffsetX;
		pImage->pETRLEObject[i].sOffsetY = _images[i].sOffsetY;
		pImage->pETRLEObject[i].usHeight = _images[i].usHeight;
		pImage->pETRLEObject[i].usWidth  = _images[i].usWidth;
		pImage->pETRLEObject[i].uiDataOffset = offset;
		pImage->pETRLEObject[i].uiDataLength = _images[i].uiDataLength;
		offset += _images[i].uiDataLength;
	}

	pImage->uiSizePixData = offset;
	pImage->pPixData8 = new UINT8[offset];
	// write sub-images
	for(unsigned int i=0; i<_compressed_images.size(); ++i)
	{
		memcpy(
			&pImage->pPixData8[pImage->pETRLEObject[i].uiDataOffset],
			_compressed_images[i],
			pImage->pETRLEObject[i].uiDataLength);
	}

	pImage->uiAppDataSize = pImage->usNumberOfObjects * sizeof(AuxObjectData);
	pImage->pAppData = (UINT8*)(new AuxObjectData[pImage->usNumberOfObjects]);
	memset(pImage->pAppData,0,pImage->uiAppDataSize);

	// write application data
	int frames = _compressed_images.size()/8; // 8 = number of directions
	for(unsigned int i=0; i<pImage->usNumberOfObjects; ++i)
	{
		if(i % frames == 0)
		{
			AuxObjectData &aod = *((AuxObjectData*)(&pImage->pAppData[i*sizeof(AuxObjectData)]));
			aod.ubNumberOfFrames = frames; 
			aod.fFlags = AUX_ANIMATED_TILE;
		}
	}
	return true;
}



//BOOLEAN GetETRLEPixelValue( UINT8 * pDest, HVOBJECT hVObject, UINT16 usETRLEIndex, UINT16 usX, UINT16 usY )
BOOLEAN ja2xp::GetETRLEPixelValue( UINT8 * pDest, image_type *hImage, UINT16 usETRLEIndex, UINT16 usX, UINT16 usY )
{
	UINT8 *					pCurrent;
	UINT16					usLoopX = 0;
	UINT16					usLoopY = 0;
	UINT16					ubRunLength;
	ETRLEObject *		pETRLEObject;

	// Do a bunch of checks
	assert( hImage != NULL );
	assert( usETRLEIndex < hImage->usNumberOfObjects );

	pETRLEObject = &(hImage->pETRLEObject[usETRLEIndex]);

	assert( usX < pETRLEObject->usWidth );
	assert( usY < pETRLEObject->usHeight );

	// Assuming everything's okay, go ahead and look...
	pCurrent = &((UINT8 *)hImage->pPixData8)[pETRLEObject->uiDataOffset];

	// Skip past all uninteresting scanlines
	while( usLoopY < usY )
	{
		while( *pCurrent != 0 )
		{
			if (*pCurrent & iCOMPRESS_TRANSPARENT)
			{
				pCurrent++;
			}
			else
			{
				pCurrent += *pCurrent & iCOMPRESS_RUN_MASK;
			}
		}
		usLoopY++;
	}

	// Now look in this scanline for the appropriate byte
	do
	{
		ubRunLength = *pCurrent & iCOMPRESS_RUN_MASK;

		if (*pCurrent & iCOMPRESS_TRANSPARENT)
		{
			if (usLoopX + ubRunLength >= usX)
			{
				*pDest = 0;
				return( TRUE );
			}
			else
			{
				pCurrent++;
			}
		}
		else
		{
			if (usLoopX + ubRunLength >= usX)
			{
				// skip to the correct byte; skip at least 1 to get past the byte defining the run
				pCurrent += (usX - usLoopX) + 1;
				*pDest = *pCurrent;
				return( TRUE );
			}
			else
			{
				pCurrent += ubRunLength + 1;
			}
		}
		usLoopX += ubRunLength;
	}
	while( usLoopX < usX );
	// huh???
	return( FALSE );
}

/*
BOOLEAN Blt8BPPDataTo8BPPBuffer( UINT8 *pBuffer, UINT32 uiDestPitchBYTES, image_type *hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex )
{
	static UINT32 uiOffset;
	static UINT32 usHeight, usWidth;
	static UINT8	 *SrcPtr, *DestPtr;
	static UINT32 LineSkip;
	static ETRLEObject *pTrav;
	static INT32	 iTempX, iTempY;


	// Assertions
	Assert( hSrcVObject != NULL );
	Assert( pBuffer != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ usIndex ] );
	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;
	uiOffset				= pTrav->uiDataOffset;

	// Add to start position of dest buffer
	iTempX = iX + pTrav->sOffsetX;
	iTempY = iY + pTrav->sOffsetY;

	// Validations
	CHECKF( iTempX >= 0 );
	CHECKF( iTempY >= 0 );


	SrcPtr= (UINT8 *)hSrcVObject->pPixData8 + uiOffset;
	DestPtr = (UINT8 *)pBuffer + (uiDestPitchBYTES*iTempY) + (iTempX);
	LineSkip=(uiDestPitchBYTES-(usWidth));

	__asm {

		mov		esi, SrcPtr
		mov		edi, DestPtr
		xor		eax, eax
		xor		ebx, ebx
		xor		ecx, ecx

BlitDispatch:

		mov		cl, [esi]
		inc		esi
		or		cl, cl
		js		BlitTransparent
		jz		BlitDoneLine

//BlitNonTransLoop:

		clc
		rcr		cl, 1
		jnc		BlitNTL2

		movsb

BlitNTL2:
		clc
		rcr		cl, 1
		jnc		BlitNTL3

		movsw

BlitNTL3:

		or		cl, cl
		jz		BlitDispatch

		xor		ebx, ebx

//BlitNTL4:

		rep		movsd

		jmp		BlitDispatch

BlitTransparent:

		and		ecx, 07fH
		xor		al, al
		rep		stosb

		jmp		BlitDispatch


BlitDoneLine:

		dec		usHeight
		jz		BlitDone
		add		edi, LineSkip
		jmp		BlitDispatch


BlitDone:
	}

	return(TRUE);

}
*/


