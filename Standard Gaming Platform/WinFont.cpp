
//#define UNICODE
#include "types.h"
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <windows.h>
#include <windowsx.h>
#include <stdarg.h>
#include <wchar.h>
#include <string.h>
#include "sgp.h"
#include "memman.h"
#include "fileman.h"
#include "Font.h"
#include "Debug.h"
#include "vsurface.h"
#include "vsurface_private.h"
#include "DirectX Common.h"
#include <ddraw.h>
#include "winfont.h"
#include "font.h"
#include "Font Control.h"
#include "GameSettings.h"

#include <vfs/Tools/vfs_property_container.h>

INT32 FindFreeWinFont( void );
BOOLEAN gfEnumSucceed = FALSE;

#ifdef CHINESE
#define DEC_INTERNAL_LEADING
#endif

// Private struct not to be exported
// to other modules

typedef struct
{
	HFONT		hFont;	
	COLORVAL	ForeColor;
	COLORVAL	BackColor;
	UINT8 Height;
	UINT8 Width[0x80];
#ifdef DEC_INTERNAL_LEADING
	UINT8 InternalLeading;
#endif
} HWINFONT;

LOGFONT gLogFont;
LONG gWinFontAdjust;
HWINFONT	WinFonts[WIN_LASTFONT];

INT32 WinFontMap[MAX_WINFONTMAP];

struct {
	char FontName[32]; //the key name
	LOGFONT LogFont;   //read it from GAME_INI_FILE
	COLORVAL Color;
} FontInfo[] = {
	{"LargeFont1", {-12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(98, 98, 98)},
	{"SmallFont1", {-11, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(98, 98, 98)},
	{"TinyFont1", {-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(98, 98, 98)},
	{"12PointFont1", {-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, 0},
	{"CompFont", {-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(0, 255, 0)},
	{"SmallCompFont", {-10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(98, 98, 98)},
	{"10PointRoman", {-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(0, 255 , 0)},
	{"12PointRoman", {-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(0, 255 , 0)},
	{"14PointSansSerif", {-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(0, 255 , 0)},
	{"10PointArial", {-11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, 0},
	{"14PointArial", {-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(0, 255 , 0)},
	{"12PointArial", {-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(222, 222, 222)},
	{"BlockyFont", {-11, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(217, 217, 217)},
	{"BlockyFont2", {-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(217, 217, 217)},
	{"10PointArialBold", {-11, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, 0},
	{"12PointArialFixedFont", {-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, 0},
	{"16PointArial", {-20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(222, 222, 222)},
	{"BlockFontNarrow", {-11, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(128, 0 , 0)},
	{"14PointHumanist", {-15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(255, 0, 0)},
	{"HugeFont", {-19, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_DONTCARE, "ja2font3"}, FROMRGB(0, 255, 0)}
};

void Convert16BitStringTo8Bit( CHAR8 *dst, CHAR16 *src )
{
  //hope 'dst' is big enough
  WideCharToMultiByte(CP_ACP, 0, src, -1, dst, 512 ,NULL,NULL);
  return;
}

void GetWinFontInfo()
{
/*
;default settings, can be changed in GAME_INI_FILE

[LargeFont1]
Name=Tahoma
Height=-12
Weight=400

[SmallFont1]
Name=Tahoma
Height=-12
Weight=400

[TinyFont1]
Name=Tahoma
Height=-12
Weight=400

[12PointFont1]
Name=Tahoma
Height=-14
Weight=400

[CompFont]
Name=Tahoma
Height=-12
Weight=400

[SmallCompFont]
Name=Tahoma
Height=-12
Weight=400

[10PointRoman]
Name=Tahoma
Height=-12
Weight=400

[12PointRoman]
Name=Tahoma
Height=-14
Weight=400

[14PointSansSerif]
Name=Tahoma
Height=-16
Weight=400

[10PointArial]
Name=Tahoma
Height=-12
Weight=400

[14PointArial]
Name=Tahoma
Height=-16
Weight=700

[12PointArial]
Name=Tahoma
Height=-14
Weight=400

[BlockyFont]
Name=Tahoma
Height=-12
Weight=700

[BlockyFont2]
Name=Tahoma
Height=-12
Weight=700

[10PointArialBold]
Name=Tahoma
Height=-12
Weight=400

[12PointArialFixedFont]
Name=Tahoma
Height=-14
Weight=400

[16PointArial]
Name=Tahoma
Height=-18
Weight=400

[BlockFontNarrow]
Name=Tahoma
Height=-12
Weight=700

[14PointHumanist]
Name=Tahoma
Height=-16
Weight=400

[HugeFont]
Name=Tahoma
Height=-20
Weight=400
*/
#ifndef USE_VFS
	char INIFile[MAX_PATH];
    GetExecutableDirectory( INIFile );
	strcat(INIFile, "\\");
	strcat(INIFile, GAME_INI_FILE);

	
	gWinFontAdjust = GetPrivateProfileInt("Ja2 Settings", "WIN_FONT_ADJUST", 0, INIFile);
	for (UINT16 i=0; i<WIN_LASTFONT; i++)
	{
		GetPrivateProfileString(FontInfo[i].FontName, "Name", FontInfo[i].LogFont.lfFaceName, FontInfo[i].LogFont.lfFaceName, LF_FACESIZE, INIFile);
		FontInfo[i].LogFont.lfHeight = gWinFontAdjust + GetPrivateProfileInt(FontInfo[i].FontName, "Height", FontInfo[i].LogFont.lfHeight, INIFile);
		FontInfo[i].LogFont.lfWeight = GetPrivateProfileInt(FontInfo[i].FontName, "Weight", FontInfo[i].LogFont.lfWeight, INIFile);
	}
#else
	vfs::PropertyContainer props;
	props.initFromIniFile(GAME_INI_FILE);
    
	gWinFontAdjust = (LONG)props.getIntProperty(L"Ja2 Settings", L"WIN_FONT_ADJUST", 0);
	for (UINT16 i=0; i<WIN_LASTFONT; i++)
	{
		vfs::String name = props.getStringProperty(FontInfo[i].FontName, L"Name", FontInfo[i].LogFont.lfFaceName);
		strncpy(FontInfo[i].LogFont.lfFaceName, name.utf8().c_str(), LF_FACESIZE);
		
		FontInfo[i].LogFont.lfHeight = gWinFontAdjust + (LONG)props.getIntProperty(FontInfo[i].FontName, "Height", FontInfo[i].LogFont.lfHeight);
		FontInfo[i].LogFont.lfWeight = (LONG)props.getIntProperty(FontInfo[i].FontName, "Weight", FontInfo[i].LogFont.lfWeight);
	}
#endif
}

void InitWinFonts( )
{
	INT32 FontSlot[WIN_LASTFONT];
	memset( WinFonts, 0, sizeof( WinFonts ) );
    
	GetWinFontInfo();
	
	for (int i = 0; i<WIN_LASTFONT; i++)
	{
		FontSlot[i] = CreateWinFont(FontInfo[i].LogFont);
		SetWinFontForeColor(FontSlot[i], &FontInfo[i].Color);
	}

	//can't use switch here, because FONT12ARIAL, LARGEFONT1... are variables
	for (int i=0; i<MAX_WINFONTMAP; i++)
		{
		 if (i == FONT12ARIAL)
		   {WinFontMap[i] = FontSlot[WIN_12POINTARIAL];}
		 else if (i == LARGEFONT1)
		   {WinFontMap[i] = FontSlot[WIN_LARGEFONT1];}
		 else if (i == SMALLFONT1)
		   {WinFontMap[i] = FontSlot[WIN_SMALLFONT1];}
		 else if  (i == TINYFONT1)
		   {WinFontMap[i] = FontSlot[WIN_TINYFONT1];}
		 else if  (i == FONT12POINT1)
		   {WinFontMap[i] = FontSlot[WIN_12POINTFONT1];}
		 else if  (i == COMPFONT)
		   {WinFontMap[i] = FontSlot[WIN_COMPFONT];}
		 else if  (i == SMALLCOMPFONT)
		   {WinFontMap[i] = FontSlot[WIN_SMALLCOMPFONT];}
		 else if  (i == FONT10ROMAN)
		   {WinFontMap[i] = FontSlot[WIN_10POINTROMAN];}
		 else if  (i == FONT12ROMAN)
		   {WinFontMap[i] = FontSlot[WIN_12POINTROMAN];}
		 else if  (i == FONT14SANSERIF)
		   {WinFontMap[i] = FontSlot[WIN_14POINTSANSSERIF];}
		 else if  (i == MILITARYFONT1)
		   {WinFontMap[i] = FontSlot[WIN_BLOCKYFONT];}
		 else if  (i == FONT10ARIAL)
		   {WinFontMap[i] = FontSlot[WIN_10POINTARIAL];}
		 else if  (i == FONT14ARIAL)
		   {WinFontMap[i] = FontSlot[WIN_14POINTARIAL];}
		 else if  (i == FONT10ARIALBOLD)
		   {WinFontMap[i] = FontSlot[WIN_10POINTARIALBOLD];}
		 else if  (i == BLOCKFONT)
		   {WinFontMap[i] = FontSlot[WIN_BLOCKYFONT];}
		 else if  (i == BLOCKFONT2)
		   {WinFontMap[i] = FontSlot[WIN_BLOCKYFONT2];}
		 else if  (i == FONT12ARIALFIXEDWIDTH)
		   {WinFontMap[i] = FontSlot[WIN_12POINTARIALFIXEDFONT];}
		 else if  (i == FONT16ARIAL)
		   {WinFontMap[i] = FontSlot[WIN_16POINTARIAL];}
		 else if  (i == BLOCKFONTNARROW)
		   {WinFontMap[i] = FontSlot[WIN_BLOCKFONTNARROW];}
		 else if  (i == FONT14HUMANIST)
		   {WinFontMap[i] = FontSlot[WIN_14POINTHUMANIST];}
		 else {WinFontMap[i] = -1;}
		}
}

void ShutdownWinFonts( )
{
	for (int i=0; i<MAX_WINFONTMAP; i++)
		{DeleteWinFont(i);}
}

INT32 FindFreeWinFont( void )
{
	INT32 iCount;

	for( iCount = 0; iCount < MAX_WINFONTMAP; iCount++ )
	{
		if( WinFonts[ iCount ].hFont == NULL )
	{
			return( iCount );
	}
	}

	return( -1 );
}


HWINFONT *GetWinFont( INT32 iFont )
{
	if ( iFont == -1 || iFont >=WIN_LASTFONT)
	{
	return( NULL );
	}

	if ( WinFonts[ iFont ].hFont == NULL )
	{
	return( NULL );
	}
	else
	{
	return( &( WinFonts[ iFont ] ) );
	}
}


INT32 CreateWinFont( LOGFONT &logfont )
{
	INT32	iFont;
	HFONT	hFont;
	// Find free slot
	iFont = FindFreeWinFont( );

	if ( iFont == -1 )
	{
	return( iFont );
	}


	//ATTEMPT TO LOAD THE FONT NOW
	hFont = CreateFontIndirect( &logfont );
	if (hFont == NULL)
	{
		FatalError( "Cannot load subtitle Windows Font: %S.", logfont.lfFaceName);
		return( -1 );
	}

	// Set font....
	WinFonts[ iFont ].hFont = hFont;

	HDC hdc = GetDC(NULL);
	SIZE RectSize;
	SelectObject(hdc, hFont );
	wchar_t str[2]=L"\1";
	for (int i = 1; i<0x80; i++)
	{
		GetTextExtentPoint32W( hdc, str, 1, &RectSize );
		WinFonts[iFont].Width[i]=(UINT8)RectSize.cx;
		str[0]++;
	}
	str[0] = L'啊';
    GetTextExtentPoint32W( hdc, str, 1, &RectSize );
    WinFonts[iFont].Width[0] = (UINT8)RectSize.cx;
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	WinFonts[ iFont ].Height = (UINT8)tm.tmAscent;
#ifdef DEC_INTERNAL_LEADING
	WinFonts[ iFont ].InternalLeading = (UINT8)tm.tmInternalLeading;
#endif
	ReleaseDC(NULL, hdc);

	return( iFont );
}

void DeleteWinFont( INT32 iFont )
{
	HWINFONT *pWinFont;
	
	pWinFont = GetWinFont( iFont );

	if ( pWinFont != NULL )
	{
		DeleteObject( pWinFont->hFont );
		pWinFont->hFont = NULL;
	}
}

	
void SetWinFontForeColor( INT32 iFont, COLORVAL *pColor )
{
	HWINFONT *pWinFont;
	
	pWinFont = GetWinFont( iFont );

	if ( pWinFont != NULL )
	{
	pWinFont->ForeColor = ( *pColor );
	}
}

	
void SetWinFontBackColor( INT32 iFont, COLORVAL *pColor )
{
	HWINFONT *pWinFont;
	
	pWinFont = GetWinFont( iFont );

	if ( pWinFont != NULL )
	{
	pWinFont->BackColor = ( *pColor );
	}
}


void PrintWinFont( UINT32 uiDestBuf, INT32 iFont, INT32 x, INT32 y, STR16 pFontString, ...)
{
	va_list				 argptr;
	CHAR16									string[512];
	HVSURFACE				hVSurface;
	LPDIRECTDRAWSURFACE2	pDDSurface;
	HDC					 hdc;
	HWINFONT				*pWinFont;
	int					 len;
	
	pWinFont = GetWinFont( iFont );

	if ( pWinFont == NULL )
	{
	return;
	}

	va_start(argptr, pFontString);			// Set up variable argument pointer
	len = vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	// Get surface...
	GetVideoSurface( &hVSurface, uiDestBuf );

	pDDSurface = GetVideoSurfaceDDSurface( hVSurface );

	IDirectDrawSurface2_GetDC( pDDSurface, &hdc );

	SelectObject(hdc, pWinFont->hFont );
	SetTextColor( hdc, pWinFont->ForeColor );
	SetBkColor(hdc, pWinFont->BackColor );
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_TOP|TA_LEFT);

#ifdef DEC_INTERNAL_LEADING
	if (y - pWinFont->InternalLeading >=0)
	{
		y -= pWinFont->InternalLeading;
	}
#endif
	TextOutW( hdc, x, y, string, len );

	IDirectDrawSurface2_ReleaseDC( pDDSurface, hdc );

}

INT16 WinFontStringPixLength( STR16 string2, INT32 iFont )
{
	HWINFONT				*pWinFont;
#ifndef CHINESE
	HDC					 hdc;
	SIZE					RectSize;
#endif
	pWinFont = GetWinFont( iFont );

	if ( pWinFont == NULL )
	{
	return( 0 );
	}

#ifdef CHINESE
	wchar_t *p=string2;
    UINT32 size = 0;
	while (*p!=0)
	{
		if (*p != L'|')
		{
		if (*p>0x80)
			{size+=pWinFont->Width[0];}
		else
			{size+=pWinFont->Width[*p];}
		}
		p++;
	}
	return size;
#else
	hdc = GetDC(NULL);
	SelectObject(hdc, pWinFont->hFont );
	GetTextExtentPoint32W( hdc, string2, lstrlenW(string2), &RectSize );
	ReleaseDC(NULL, hdc);
	
	return( (INT16)RectSize.cx );
#endif
}


INT16 GetWinFontHeight( STR16 string2, INT32 iFont )
{
	HWINFONT				*pWinFont;
//	HDC					 hdc;
//	SIZE					RectSize;

	pWinFont = GetWinFont( iFont );

	if ( pWinFont == NULL )
	{
	return( 0 );
	}
	else
	{
#ifdef CHINESE //zwwooooo: Correct tactical interface font height to fixed Chinese characters smearing bug
		if (iFont==WIN_TINYFONT1 || iFont==WIN_SMALLFONT1 || iFont==WIN_14POINTARIAL) return pWinFont->Height+2;
#endif
		return pWinFont->Height;
    }
	/*
	hdc = GetDC(NULL);
	SelectObject(hdc, pWinFont->hFont );
	GetTextExtentPoint32W( hdc, string2, lstrlenW(string2), &RectSize );
	ReleaseDC(NULL, hdc);
	
	return( (INT16)RectSize.cy );*/
}

UINT32	WinFont_mprintf( INT32 iFont, INT32 x, INT32 y, STR16 pFontString, ...)
{
	va_list				 argptr;
	CHAR16				 string[512];

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	PrintWinFont( FontDestBuffer, iFont, x,	y, string );

	return( 1 );
}
