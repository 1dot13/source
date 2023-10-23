#ifndef __FONT_H_
#define __FONT_H_

#include "Types.h"
#include "himage.h"
#include "vobject.h"
//#ifdef WINFONTS
#include "WinFont.h"
//#endif
#include "Font Control.h"

#define		MAX_FONTS				25

#define		DEFAULT_SHADOW			2
#define		MILITARY_SHADOW			67
#define		NO_SHADOW						0


// these are bogus! No palette is set yet!
// font foreground color symbols
#define		FONT_FCOLOR_WHITE			208
#define		FONT_FCOLOR_RED				162
#define		FONT_FCOLOR_NICERED		164
#define		FONT_FCOLOR_BLUE			203
#define		FONT_FCOLOR_GREEN			184
#define		FONT_FCOLOR_YELLOW		144
#define		FONT_FCOLOR_BROWN			184
#define		FONT_FCOLOR_ORANGE		76
#define		FONT_FCOLOR_PURPLE		160

// font background color symbols
#define		FONT_BCOLOR_WHITE			208
#define		FONT_BCOLOR_RED				162
#define		FONT_BCOLOR_BLUE			203
#define		FONT_BCOLOR_GREEN			184
#define		FONT_BCOLOR_YELLOW		144
#define		FONT_BCOLOR_BROWN			80
#define		FONT_BCOLOR_ORANGE		76
#define		FONT_BCOLOR_PURPLE		160



// typedefs

typedef struct
{
	UINT16	usNumberOfSymbols;
	UINT16 *DynamicArrayOf16BitValues;

} FontTranslationTable;
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
extern INT32		FontDefault;
extern UINT32		FontDestBuffer;
extern UINT32		FontDestPitch;
extern UINT32		FontDestBPP;
extern SGPRect	FontDestRegion;
extern BOOLEAN	FontDestWrap;
	
#define					SetFontDestObject(x)	(SetFontDestBuffer(x,										\
																												FontDestRegion.left,	\
																												FontDestRegion.top,		\
																												FontDestRegion.right,	\
																												FontDestRegion.bottom,\
																												FontDestWrap))

#define					SetFontDestClip(x1, y1, x2, y2)	(SetFontDestBuffer(FontDestBuffer,	\
																																	x1, y1,						\
																																	x2, y2,						\
																																	FontDestWrap))
#define					SetFontDestWrap(x)	(SetFontDestBuffer(FontDestBuffer,				\
																												FontDestRegion.left,	\
																												FontDestRegion.top,		\
																												FontDestRegion.right,	\
																												FontDestRegion.bottom,\
																												x))
// functions

void SetFontColors(UINT16 usColors);
void SetFontForeground(UINT8 ubForeground);
void SetFontBackground(UINT8 ubBackground);
void SetFontShadow(UINT8 ubBackground);

//Kris:	added these
void SetRGBFontForeground( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue );
void SetRGBFontBackground( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue );
void SetRGBFontShadow( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue );

// Flugente: set 16 bit colour directly
void SetRGBFontForeground( UINT16 usFontForeground16 );
void SetRGBFontShadow( UINT16 usFontShadow16 );

BOOLEAN ResetFontObjectPalette(INT32 iFont);
UINT16 *SetFontObjectPalette8BPP(INT32 iFont, SGPPaletteEntry *pPal8);
UINT16 *SetFontObjectPalette16BPP(INT32 iFont, UINT16 *pPal16);
UINT16 *GetFontObjectPalette16BPP(INT32 iFont);

void DestroyEnglishTransTable( void );

extern BOOLEAN	IsFontLoaded(INT32 iFont);
extern HVOBJECT	GetFontObject(INT32 iFont);
extern UINT32		gprintf(INT32 x, INT32 y, const STR16 pFontString, ...);
extern UINT32	gprintfDirty(INT32 x, INT32 y, const STR16 pFontString, ...);
extern UINT32		mprintf(INT32 x, INT32 y, const STR16 pFontString, ...);
extern UINT32		gprintf_buffer( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, const STR16 pFontString, ...);
extern UINT32	mprintf_buffer( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, const STR16 pFontString, ...);

// Function for displaying coded test. Since it's slower to do this, it's separate from	the normal fuctions
#define FONT_CODE_BEGINCOLOR				180
#define FONT_CODE_RESETCOLOR				181

UINT32 mprintf_buffer_coded( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, const STR16 pFontString, ...);
UINT32 mprintf_coded( INT32 x, INT32 y, const STR16 pFontString, ...);


extern BOOLEAN	SetFontDestBuffer(UINT32 DestBuffer, INT32 x1, INT32 y1, INT32 x2, INT32 y2, BOOLEAN wrap);
extern BOOLEAN	SetFont(INT32 iFontIndex);

extern INT32		LoadFontFile(const STR8 pFileName);
extern UINT16	GetFontHeight(INT32 FontNum);
extern BOOLEAN	InitializeFontManager(UINT16 usDefaultPixDepth, FontTranslationTable *pTransTable);
extern void		ShutdownFontManager(void);
extern void			UnloadFont(UINT32 FontIndex);

extern FontTranslationTable *CreateEnglishTransTable(	);

extern INT16 GetIndex(CHAR16 siChar);
extern UINT32 GetWidth(HVOBJECT hSrcVObject, INT16 ssIndex);

extern INT16 StringPixLengthArgFastHelp( INT32 usUseFont, INT32 usBoldFont, UINT32 uiCharCount, STR16 pFontString );
extern INT16 StringPixLengthArg(INT32 usUseFont, UINT32 uiCharCount, STR16 pFontString, ...);
extern INT16 StringPixLength(const STR16 string,INT32 UseFont);
extern INT16 SubstringPixLength(STR16 string, UINT32 iStart, UINT32 iEnd, INT32 UseFont);
extern INT16 StringNPixLength(STR16 string, UINT32 uiMaxCount, INT32 UseFont);
extern void SaveFontSettings(void);
extern void RestoreFontSettings(void);

void VarFindFontRightCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, const STR16 pFontString, ... );

void VarFindFontCenterCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, const STR16 pFontString, ... );

void FindFontRightCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, const STR16 pStr, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY );

void FindFontCenterCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, const STR16 pStr, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY );

//extern FontBase *LoadFontFile(CHAR8 *pFileName);
//extern UINT8	*GetFontPalette(CHAR8 *pFileName);
//extern UINT16	GetMaxFontWidth(FontBase *pBase);
//extern void		UnloadFont(FontBase *pBase);
//extern BOOLEAN	SetFontPalette(FontBase *pFont, UINT16 siDepthPix, SGPPaletteEntry *pNewPalette);
// make sure the pFontString is terminated by 0
//extern BOOLEAN	PrintFontString(STR16 pFontString, UINT8 *pDestBuffer, UINT16 siDestWidth, UINT16 siDestPixelDepth, UINT16 siDestPitch, UINT16 siDestHeight, UINT16 siX, UINT16 siY, UINT16 siTotalWidth, UINT16 siTotalHeight, BOOLEAN MultiLine, FontBase *pFontBase);
//extern BOOLEAN	SetFont16BitData(FontBase *pFontBase, UINT16 *pData16);

/*
#ifdef __cplusplus
}
#endif
*/

#endif

CHAR16 GetUnicodeChar(CHAR16 siChar);
