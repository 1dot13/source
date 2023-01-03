
#include <cmath>
#include <cstdlib>
#include "types.h"
#include "string.h"
#include "himage.h"


// This is the color substituted to keep a 24bpp->16bpp color
// from going transparent (0x0000) -- DB

#define BLACK_SUBSTITUTE	0x0001


//UINT16 gusAlphaMask = 0;
//UINT16 gusRedMask = 0;
//UINT16 gusGreenMask = 0;
//UINT16 gusBlueMask = 0;
//INT16	gusRedShift = 0;
//INT16	gusBlueShift = 0;
//INT16	gusGreenShift = 0;
UINT16 gusAlphaMask = 0;
UINT16 gusRedMask = 0xF800;
UINT16 gusGreenMask = 0x07E0;
UINT16 gusBlueMask = 0x001F;
INT16	gusRedShift = 8;
INT16	gusBlueShift = -3;
INT16	gusGreenShift = 3;


// this funky union is used for fast 16-bit pixel format conversions
union SplitUINT32
{
	struct
	{
		UINT16	usLower;
		UINT16	usHigher;
	};
	UINT32	uiValue;
};

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
