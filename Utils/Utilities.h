#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "Overhead types.h"
#include "sgp.h"


#define GETPIXELDEPTH( )	( gbPixelDepth )

// WANNE: Maximum number of characters in german description (German xml files)
#define MAXLINE		200

BOOLEAN CreateSGPPaletteFromCOLFile( SGPPaletteEntry *pPalette, SGPFILENAME ColFile );
BOOLEAN DisplayPaletteRep( PaletteRepID aPalRep, UINT8 ubXPos, UINT8 ubYPos, UINT32 uiDestSurface );

void FilenameForBPP(STR pFilename, STR pDestination);

BOOLEAN	WrapString( STR16 pStr, STR16 pStr2, UINT16 usWidth, INT32 uiFont );

BOOLEAN IfWinNT(void);
BOOLEAN IfWin95(void);

void HandleLimitedNumExecutions( );

BOOLEAN HandleJA2CDCheck( );
BOOLEAN HandleJA2CDCheckTwo( );

// WANNE: This method replaces characters in a given text with new characters
STR8 Replace(STR8 string, STR8 oldpiece, STR8 newpiece);

// WANNE: This method calls the replace method and replaces all german specific characters
// WANNE: Not used!
//STR8 ReplaceGermanSpecialCharacters(STR8 text);


// Snap: integer division that rounds the result to the nearest integer
template<class Integer>
inline Integer idiv(Integer a, Integer b)
{
	return a > 0 ? b > 0 ? (a + b/2) / b : (a - b/2) / b :
				b > 0 ? (a - b/2) / b : (a + b/2) / b ;
}

#endif
