#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "Overhead types.h"
#include "sgp.h"


#define GETPIXELDEPTH( )	( gbPixelDepth )

BOOLEAN CreateSGPPaletteFromCOLFile( SGPPaletteEntry *pPalette, SGPFILENAME ColFile );
BOOLEAN DisplayPaletteRep( PaletteRepID aPalRep, UINT8 ubXPos, UINT8 ubYPos, UINT32 uiDestSurface );

void FilenameForBPP(STR pFilename, STR pDestination);

BOOLEAN	 WrapString( INT16 *pStr, INT16 *pStr2, UINT16 usWidth, INT32 uiFont );

BOOLEAN IfWinNT(void);
BOOLEAN IfWin95(void);

void HandleLimitedNumExecutions( );

BOOLEAN HandleJA2CDCheck( );
BOOLEAN HandleJA2CDCheckTwo( );

// Snap: integer division that rounds the result to the nearest integer
template<class Integer>
inline Integer idiv(Integer a, Integer b)
{
	return a > 0 ? b > 0 ? (a + b/2) / b : (a - b/2) / b :
				   b > 0 ? (a - b/2) / b : (a + b/2) / b ;
}

#endif