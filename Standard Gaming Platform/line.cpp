#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "line.h"
#endif

//**************************************************************************
//
//				Example Usage
//
//**************************************************************************

//	SEND THE PITCH IN BYTES
//	SetClippingRegionAndImageWidth( uiPitch, 15, 15, 30, 30 );
//
//	LineDraw( TRUE, 10, 10, 200, 200, colour, pImageData);
//	OR
//	RectangleDraw( TRUE, 10, 10, 200, 200, colour, pImageData);

//**************************************************************************
//
//				Line Drawing Functions
//
//**************************************************************************

int giImageWidth=0;
int giClipXMin=0;
int giClipXMax=0;
int giClipYMin=0;
int giClipYMax=0;

void DrawHorizontalRun(UINT8 **ScreenPtr, int XAdvance, int RunLength,
						int Color, int ScreenWidth);
void DrawVerticalRun(UINT8 **ScreenPtr, int XAdvance, int RunLength,
						int Color, int ScreenWidth);

void DrawHorizontalRun8(UINT8 **ScreenPtr, int XAdvance,
	int RunLength, int Color, int ScreenWidth);
void DrawVerticalRun8(UINT8 **ScreenPtr, int XAdvance,
	int RunLength, int Color, int ScreenWidth);


void SetClippingRegionAndImageWidth(
	int iImageWidth,
	int iClipStartX,
	int iClipStartY,
	int iClipWidth,
	int iClipHeight
	)
{
	giImageWidth = iImageWidth;
	giClipXMin = iClipStartX;
	giClipXMax = iClipStartX + iClipWidth-1;
	giClipYMin = iClipStartY;
	giClipYMax = iClipStartY + iClipHeight-1;
}

BOOL Clipt( FLOAT denom, FLOAT num, FLOAT *tE, FLOAT *tL )
{
	FLOAT	t;
	BOOL	accept;

	accept = TRUE;

	if ( denom > 0.0f )
	{
		t = num/denom;
		if ( t > *tL )
			accept = FALSE;
		else if ( t > *tE )
			*tE = t;
	}
	else if ( denom < 0.0f )
	{
		t = num/denom;
		if ( t < *tE )
			accept = FALSE;
		else if ( t < *tL )
			*tL = t;
	}
	else if ( num > 0 )
		accept = FALSE;

	return(accept);
}

BOOL ClipPoint( int x, int y )
{
	return( x <= giClipXMax && x >= giClipXMin &&
			y <= giClipYMax && y >= giClipYMin );
}

BOOL Clip2D( int *ix0, int *iy0, int *ix1, int *iy1 )
{
	BOOL	visible;
	FLOAT	te, tl;
	FLOAT	dx, dy;
	FLOAT	x0, y0, x1, y1;

	x0 = (FLOAT)*ix0;
	x1 = (FLOAT)*ix1;
	y0 = (FLOAT)*iy0;
	y1 = (FLOAT)*iy1;

	dx = x1-x0;
	dy = y1-y0;
	visible = FALSE;

	if ( dx == 0.0 && dy == 0.0 && ClipPoint(*ix0,*iy0) )
		visible = TRUE;
	else
	{
		te = 0.0f;
		tl = 1.0f;
		if ( Clipt( dx, (FLOAT)giClipXMin-x0, &te, &tl ) )
		{
			if ( Clipt(-dx, x0-(FLOAT)giClipXMax, &te, &tl ) )
			{
				if ( Clipt(dy, (FLOAT)giClipYMin-y0, &te, &tl ) )
				{
					if ( Clipt(-dy, y0-(FLOAT)giClipYMax, &te, &tl ) )
					{
						visible = TRUE;
						if ( tl < 1.0f )
						{
							x1 = x0 + tl*dx;
							y1 = y0 + tl*dy;
						}
						if ( te > 0 )
						{
							x0 = x0 + te*dx;
							y0 = y0 + te*dy;
						}
					}
				}
			}
		}
	}

	*ix0 = (int)x0;
	*ix1 = (int)x1;
	*iy0 = (int)y0;
	*iy1 = (int)y1;

	return( visible );
}

void LineDraw( BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color, UINT8 *ScreenPtr)
{
	int Temp, AdjUp, AdjDown, ErrorTerm, XAdvance, XDelta, YDelta;
	int WholeStep, InitialPixelCount, FinalPixelCount, i, RunLength;
	int ScreenWidth=giImageWidth/2;
	char col2 = Color>>8;
	char col1 = Color & 0x00FF;

	if ( fClip )
	{
		if ( !Clip2D( &XStart, &YStart, &XEnd, &YEnd ) )
			return;
	}

	/*	We'll always draw top to bottom, to reduce the number of cases we have to
		handle, and to make lines between the same endpoints draw the same pixels */
	if (YStart > YEnd) {
		Temp = YStart;
		YStart = YEnd;
		YEnd = Temp;
		Temp = XStart;
		XStart = XEnd;
		XEnd = Temp;
	}

	// point to the bitmap address first pixel to draw
	ScreenPtr = ScreenPtr + YStart*giImageWidth + XStart*2;

	/*	Figure out whether we're going left or right, and how far we're
		going horizontally */
	if ((XDelta = XEnd - XStart) < 0)
	{
		XAdvance = -1;
		XDelta = -XDelta;
	}
	else
	{
		XAdvance = 1;
	}
	/* Figure out how far we're going vertically */
	YDelta = YEnd - YStart;

	/* Special-case horizontal, vertical, and diagonal lines, for speed
	and to avoid nasty boundary conditions and division by 0 */
	if (XDelta == 0)
	{
		/* Vertical line */
		for (i=0; i<=YDelta; i++)
		{
			ScreenPtr[0] = col1;
			ScreenPtr[1] = col2;
			ScreenPtr += giImageWidth;
		}
		return;
	}
	if (YDelta == 0)
	{
		/* Horizontal line */
		for (i=0; i<=XDelta; i++)
		{
			ScreenPtr[0] = col1;
			ScreenPtr[1] = col2;
			ScreenPtr += XAdvance*2;
		}
		return;
	}
	if (XDelta == YDelta)
	{
		/* Diagonal line */
		for (i=0; i<=XDelta; i++)
		{
			ScreenPtr[0] = col1;
			ScreenPtr[1] = col2;
			ScreenPtr += (XAdvance*2) + giImageWidth;
		}
		return;
	}

	/* Determine whether the line is X or Y major, and handle accordingly */
	if (XDelta >= YDelta)
	{
		/* X major line */
		/* Minimum # of pixels in a run in this line */
		WholeStep = XDelta / YDelta;

		/* Error term adjust each time Y steps by 1; used to tell when one
		 extra pixel should be drawn as part of a run, to account for
		 fractional steps along the X axis per 1-pixel steps along Y */
		AdjUp = (XDelta % YDelta) * 2;

		/* Error term adjust when the error term turns over, used to factor
		 out the X step made at that time */
		AdjDown = YDelta * 2;

		/* Initial error term; reflects an initial step of 0.5 along the Y
		 axis */
		ErrorTerm = (XDelta % YDelta) - (YDelta * 2);

		/* The initial and last runs are partial, because Y advances only 0.5
		 for these runs, rather than 1. Divide one full run, plus the
		 initial pixel, between the initial and last runs */
		InitialPixelCount = (WholeStep / 2) + 1;
		FinalPixelCount = InitialPixelCount;

		/* If the basic run length is even and there's no fractional
		 advance, we have one pixel that could go to either the initial
		 or last partial run, which we'll arbitrarily allocate to the
		 last run */
		if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
		{
		 InitialPixelCount--;
		}
		/* If there're an odd number of pixels per run, we have 1 pixel that can't
		 be allocated to either the initial or last partial run, so we'll add 0.5
		 to error term so this pixel will be handled by the normal full-run loop */
		if ((WholeStep & 0x01) != 0)
		{
		 ErrorTerm += YDelta;
		}
		/* Draw the first, partial run of pixels */
		DrawHorizontalRun(&ScreenPtr, XAdvance, InitialPixelCount, Color, ScreenWidth);
		/* Draw all full runs */
		for (i=0; i<(YDelta-1); i++)
		{
		 RunLength = WholeStep;	/* run is at least this long */
		 /* Advance the error term and add an extra pixel if the error
			term so indicates */
		 if ((ErrorTerm += AdjUp) > 0)
		 {
			RunLength++;
			ErrorTerm -= AdjDown;	/* reset the error term */
		 }
		 /* Draw this scan line's run */
		 DrawHorizontalRun(&ScreenPtr, XAdvance, RunLength, Color, ScreenWidth);
		}
		/* Draw the final run of pixels */
		DrawHorizontalRun(&ScreenPtr, XAdvance, FinalPixelCount, Color, ScreenWidth);
		return;
	}
	else
	{
		/* Y major line */

		/* Minimum # of pixels in a run in this line */
		WholeStep = YDelta / XDelta;

		/* Error term adjust each time X steps by 1; used to tell when 1 extra
		 pixel should be drawn as part of a run, to account for
		 fractional steps along the Y axis per 1-pixel steps along X */
		AdjUp = (YDelta % XDelta) * 2;

		/* Error term adjust when the error term turns over, used to factor
		 out the Y step made at that time */
		AdjDown = XDelta * 2;

		/* Initial error term; reflects initial step of 0.5 along the X axis */
		ErrorTerm = (YDelta % XDelta) - (XDelta * 2);

		/* The initial and last runs are partial, because X advances only 0.5
		 for these runs, rather than 1. Divide one full run, plus the
		 initial pixel, between the initial and last runs */
		InitialPixelCount = (WholeStep / 2) + 1;
		FinalPixelCount = InitialPixelCount;

		/* If the basic run length is even and there's no fractional advance, we
		 have 1 pixel that could go to either the initial or last partial run,
		 which we'll arbitrarily allocate to the last run */
		if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
		{
		 InitialPixelCount--;
		}
		/* If there are an odd number of pixels per run, we have one pixel
		 that can't be allocated to either the initial or last partial
		 run, so we'll add 0.5 to the error term so this pixel will be
		 handled by the normal full-run loop */
		if ((WholeStep & 0x01) != 0)
		{
		 ErrorTerm += XDelta;
		}
		/* Draw the first, partial run of pixels */
		DrawVerticalRun(&ScreenPtr, XAdvance, InitialPixelCount, Color, ScreenWidth);

		/* Draw all full runs */
		for (i=0; i<(XDelta-1); i++)
		{
		 RunLength = WholeStep;	/* run is at least this long */
		 /* Advance the error term and add an extra pixel if the error
			term so indicates */
		 if ((ErrorTerm += AdjUp) > 0)
		 {
			RunLength++;
			ErrorTerm -= AdjDown;	/* reset the error term */
		 }
		 /* Draw this scan line's run */
		 DrawVerticalRun(&ScreenPtr, XAdvance, RunLength, Color, ScreenWidth);
		}
		/* Draw the final run of pixels */
		DrawVerticalRun(&ScreenPtr, XAdvance, FinalPixelCount, Color, ScreenWidth);
		return;
	}
}

//Draws a pixel in the specified color
void PixelDraw( BOOLEAN fClip, INT32 xp, INT32 yp, INT16 sColor, UINT8 *pScreen )
{
	if ( fClip && !ClipPoint( xp, yp ) )
		return;
	
	// point to the bitmap address first pixel to draw
	pScreen += yp * giImageWidth + xp * 2;

	INT8 col2 = sColor >> 8;
	INT8 col1 = sColor & 0x00ff;

	pScreen[ 0 ] = col1;
	pScreen[ 1 ] = col2;
}

// Flugente: alter the colour of existing pixels instead of fully replacing the colour
void PixelAlterColour(BOOLEAN fClip, INT32 xp, INT32 yp, INT16 sColor, UINT8 *pScreen)
{
	if ( fClip && !ClipPoint( xp, yp ) )
		return;
	
	// point to the bitmap address first pixel to draw
	pScreen += yp * giImageWidth + xp * 2;
	
	INT8 col2 = sColor >> 8;
	INT8 col1 = sColor & 0x00ff;
	
	pScreen[0] |= col1;
	pScreen[1] |= col2;
}

/* Draws a horizontal run of pixels, then advances the bitmap pointer to
	the first pixel of the next run. */
void DrawHorizontalRun(UINT8 **ScreenPtr, int XAdvance,
	int RunLength, int Color, int ScreenWidth)
{
	int i;
	UINT8 *WorkingScreenPtr = *ScreenPtr;
	UINT8 col2 = Color>>8;
	UINT8 col1 = Color & 0x00FF;

	for (i=0; i<RunLength; i++)
	{
		WorkingScreenPtr[0] = col1;
		WorkingScreenPtr[1] = col2;
		WorkingScreenPtr += XAdvance*2;
	}
	/* Advance to the next scan line */
	WorkingScreenPtr += giImageWidth;
	*ScreenPtr = WorkingScreenPtr;
}

/* Draws a vertical run of pixels, then advances the bitmap pointer to
	the first pixel of the next run. */
void DrawVerticalRun(UINT8 **ScreenPtr, int XAdvance,
	int RunLength, int Color, int ScreenWidth)
{
	int i;
	UINT8 *WorkingScreenPtr = *ScreenPtr;
	UINT8 col2 = Color>>8;
	UINT8 col1 = Color & 0x00FF;

	for (i=0; i<RunLength; i++)
	{
		WorkingScreenPtr[0] = col1;
		WorkingScreenPtr[1] = col2;
		WorkingScreenPtr += giImageWidth;
	}
	/* Advance to the next column */
	WorkingScreenPtr += XAdvance*2;
	*ScreenPtr = WorkingScreenPtr;
}


/* Draws a rectangle between the specified endpoints in color Color. */
void RectangleDraw( BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color, UINT8 *ScreenPtr)
{
	LineDraw( fClip, XStart, YStart, XEnd,	YStart, Color, ScreenPtr);
	LineDraw( fClip, XStart, YEnd,	XEnd,	YEnd,	Color, ScreenPtr);
	LineDraw( fClip, XStart, YStart, XStart, YEnd,	Color, ScreenPtr);
	LineDraw( fClip, XEnd,	YStart, XEnd,	YEnd,	Color, ScreenPtr);
}

/***********************************************************************************
* 8-Bit Versions
*
*
*
* Added by Derek Beland
***********************************************************************************/

/* Draws a rectangle between the specified endpoints in color Color. */
void RectangleDraw8( BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color, UINT8 *ScreenPtr)
{
	LineDraw8( fClip, XStart, YStart, XEnd,	YStart, Color, ScreenPtr);
	LineDraw8( fClip, XStart, YEnd,	XEnd,	YEnd,	Color, ScreenPtr);
	LineDraw8( fClip, XStart, YStart, XStart, YEnd,	Color, ScreenPtr);
	LineDraw8( fClip, XEnd,	YStart, XEnd,	YEnd,	Color, ScreenPtr);
}

/* Draws a line between the specified endpoints in color Color. */
void LineDraw8( BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color, UINT8 *ScreenPtr)
{
	int Temp, AdjUp, AdjDown, ErrorTerm, XAdvance, XDelta, YDelta;
	int WholeStep, InitialPixelCount, FinalPixelCount, i, RunLength;
	int ScreenWidth = giImageWidth;
	UINT8 col1 = Color & 0x00FF;

	if ( fClip )
	{
		if ( !Clip2D( &XStart, &YStart, &XEnd, &YEnd ) )
			return;
	}

	/*	We'll always draw top to bottom, to reduce the number of cases we have to
		handle, and to make lines between the same endpoints draw the same pixels */
	if (YStart > YEnd) {
		Temp = YStart;
		YStart = YEnd;
		YEnd = Temp;
		Temp = XStart;
		XStart = XEnd;
		XEnd = Temp;
	}

	// point to the bitmap address first pixel to draw
	ScreenPtr = ScreenPtr + YStart*giImageWidth + XStart;

	/*	Figure out whether we're going left or right, and how far we're
		going horizontally */
	if ((XDelta = XEnd - XStart) < 0)
	{
		XAdvance = -1;
		XDelta = -XDelta;
	}
	else
	{
		XAdvance = 1;
	}
	/* Figure out how far we're going vertically */
	YDelta = YEnd - YStart;

	/* Special-case horizontal, vertical, and diagonal lines, for speed
	and to avoid nasty boundary conditions and division by 0 */
	if (XDelta == 0)
	{
		/* Vertical line */
		for (i=0; i<=YDelta; i++)
		{
			*ScreenPtr = col1;
			ScreenPtr += giImageWidth;
		}
		return;
	}
	if (YDelta == 0)
	{
		/* Horizontal line */
		for (i=0; i<=XDelta; i++)
		{
			*ScreenPtr = col1;
			ScreenPtr += XAdvance;
		}
		return;
	}
	if (XDelta == YDelta)
	{
		/* Diagonal line */
		for (i=0; i<=XDelta; i++)
		{
			*ScreenPtr = col1;
			ScreenPtr += (XAdvance + giImageWidth);
		}
		return;
	}

	/* Determine whether the line is X or Y major, and handle accordingly */
	if (XDelta >= YDelta)
	{
		/* X major line */
		/* Minimum # of pixels in a run in this line */
		WholeStep = XDelta / YDelta;

		/* Error term adjust each time Y steps by 1; used to tell when one
		 extra pixel should be drawn as part of a run, to account for
		 fractional steps along the X axis per 1-pixel steps along Y */
		AdjUp = (XDelta % YDelta) * 2;

		/* Error term adjust when the error term turns over, used to factor
		 out the X step made at that time */
		AdjDown = YDelta * 2;

		/* Initial error term; reflects an initial step of 0.5 along the Y
		 axis */
		ErrorTerm = (XDelta % YDelta) - (YDelta * 2);

		/* The initial and last runs are partial, because Y advances only 0.5
		 for these runs, rather than 1. Divide one full run, plus the
		 initial pixel, between the initial and last runs */
		InitialPixelCount = (WholeStep / 2) + 1;
		FinalPixelCount = InitialPixelCount;

		/* If the basic run length is even and there's no fractional
		 advance, we have one pixel that could go to either the initial
		 or last partial run, which we'll arbitrarily allocate to the
		 last run */
		if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
		{
		 InitialPixelCount--;
		}
		/* If there're an odd number of pixels per run, we have 1 pixel that can't
		 be allocated to either the initial or last partial run, so we'll add 0.5
		 to error term so this pixel will be handled by the normal full-run loop */
		 if ((WholeStep & 0x01) != 0)
		{
		 ErrorTerm += YDelta;
		}
		/* Draw the first, partial run of pixels */
		DrawHorizontalRun8(&ScreenPtr, XAdvance, InitialPixelCount, Color, ScreenWidth);
		/* Draw all full runs */
		for (i=0; i<(YDelta-1); i++)
		{
		 RunLength = WholeStep;	/* run is at least this long */
		 /* Advance the error term and add an extra pixel if the error
			term so indicates */
		 if ((ErrorTerm += AdjUp) > 0)
		 {
			RunLength++;
			ErrorTerm -= AdjDown;	/* reset the error term */
		 }
		 /* Draw this scan line's run */
		 DrawHorizontalRun8(&ScreenPtr, XAdvance, RunLength, Color, ScreenWidth);
		}
		/* Draw the final run of pixels */
		DrawHorizontalRun8(&ScreenPtr, XAdvance, FinalPixelCount, Color, ScreenWidth);
		return;
	}
	else
	{
		/* Y major line */

		/* Minimum # of pixels in a run in this line */
		WholeStep = YDelta / XDelta;

		/* Error term adjust each time X steps by 1; used to tell when 1 extra
		 pixel should be drawn as part of a run, to account for
		 fractional steps along the Y axis per 1-pixel steps along X */
		AdjUp = (YDelta % XDelta) * 2;

		/* Error term adjust when the error term turns over, used to factor
		 out the Y step made at that time */
		AdjDown = XDelta * 2;

		/* Initial error term; reflects initial step of 0.5 along the X axis */
		ErrorTerm = (YDelta % XDelta) - (XDelta * 2);

		/* The initial and last runs are partial, because X advances only 0.5
		 for these runs, rather than 1. Divide one full run, plus the
		 initial pixel, between the initial and last runs */
		InitialPixelCount = (WholeStep / 2) + 1;
		FinalPixelCount = InitialPixelCount;

		/* If the basic run length is even and there's no fractional advance, we
		 have 1 pixel that could go to either the initial or last partial run,
		 which we'll arbitrarily allocate to the last run */
		if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
		{
		 InitialPixelCount--;
		}
		/* If there are an odd number of pixels per run, we have one pixel
		 that can't be allocated to either the initial or last partial
		 run, so we'll add 0.5 to the error term so this pixel will be
		 handled by the normal full-run loop */
		if ((WholeStep & 0x01) != 0)
		{
		 ErrorTerm += XDelta;
		}
		/* Draw the first, partial run of pixels */
		DrawVerticalRun8(&ScreenPtr, XAdvance, InitialPixelCount, Color, ScreenWidth);

		/* Draw all full runs */
		for (i=0; i<(XDelta-1); i++)
		{
		 RunLength = WholeStep;	/* run is at least this long */
		 /* Advance the error term and add an extra pixel if the error
			term so indicates */
		 if ((ErrorTerm += AdjUp) > 0)
		 {
			RunLength++;
			ErrorTerm -= AdjDown;	/* reset the error term */
		 }
		 /* Draw this scan line's run */
		 DrawVerticalRun8(&ScreenPtr, XAdvance, RunLength, Color, ScreenWidth);
		}
		/* Draw the final run of pixels */
		DrawVerticalRun8(&ScreenPtr, XAdvance, FinalPixelCount, Color, ScreenWidth);
		return;
	}
}


/* Draws a horizontal run of pixels, then advances the bitmap pointer to
	the first pixel of the next run. */
void DrawHorizontalRun8(UINT8 **ScreenPtr, int XAdvance,
	int RunLength, int Color, int ScreenWidth)
{
	int i;
	UINT8 *WorkingScreenPtr = *ScreenPtr;
	UINT8 col1 = Color & 0x00FF;

	for (i=0; i<RunLength; i++)
	{
		*WorkingScreenPtr = col1;
		WorkingScreenPtr += XAdvance;
	}
	/* Advance to the next scan line */
	WorkingScreenPtr += giImageWidth;
	*ScreenPtr = WorkingScreenPtr;
}

/* Draws a vertical run of pixels, then advances the bitmap pointer to
	the first pixel of the next run. */
void DrawVerticalRun8(UINT8 **ScreenPtr, int XAdvance,
	int RunLength, int Color, int ScreenWidth)
{
	int i;
	UINT8 *WorkingScreenPtr = *ScreenPtr;
	UINT8 col1 = Color & 0x00FF;

	for (i=0; i<RunLength; i++)
	{
		*WorkingScreenPtr = col1;
		WorkingScreenPtr += giImageWidth;
	}
	/* Advance to the next column */
	WorkingScreenPtr += XAdvance;
	*ScreenPtr = WorkingScreenPtr;
}
