#ifndef __WINFONT_
#define __WINFONT_
#include <windows.h>

void InitWinFonts( );
void ShutdownWinFonts( );

INT32 CreateWinFont( LOGFONT &logfont );
void	DeleteWinFont( INT32 iFont );

void SetWinFontBackColor( INT32 iFont, COLORVAL *pColor );
void SetWinFontForeColor( INT32 iFont, COLORVAL *pColor );

void PrintWinFont( UINT32 uiDestBuf, INT32 iFont, INT32 x, INT32 y, STR16 pFontString, ...);

INT16 WinFontStringPixLength( STR16	string, INT32 iFont );
INT16 GetWinFontHeight( STR16 string, INT32 iFont );
UINT32	WinFont_mprintf( INT32 iFont, INT32 x, INT32 y, STR16 pFontString, ...);

//if you cahnge this enum, you must change FontInfo struct in WinFont.cpp too.
enum {
WIN_LARGEFONT1 = 0, 
WIN_SMALLFONT1,
WIN_TINYFONT1, 
WIN_12POINTFONT1, 
WIN_COMPFONT,
WIN_SMALLCOMPFONT,
WIN_10POINTROMAN,
WIN_12POINTROMAN,
WIN_14POINTSANSSERIF,
WIN_10POINTARIAL,
WIN_14POINTARIAL,
WIN_12POINTARIAL,
WIN_BLOCKYFONT,
WIN_BLOCKYFONT2,
WIN_10POINTARIALBOLD,
WIN_12POINTARIALFIXEDFONT,
WIN_16POINTARIAL,
WIN_BLOCKFONTNARROW,
WIN_14POINTHUMANIST,
WIN_HUGEFONT,
WIN_LASTFONT
};
#define MAX_WINFONTMAP 25
extern INT32 WinFontMap[MAX_WINFONTMAP];

#endif
