// font.c
#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "types.h"
	#include <stdio.h>
	#include <stdarg.h>
	#include <malloc.h>
	#include <windows.h>
	#include <stdarg.h>
	#include <wchar.h>
	#include "sgp.h"
	#include "pcx.h"
	#include "memman.h"
	#include "fileman.h"
	#include "Font.h"
	#include "Debug.h"

	#if defined( JA2 ) || defined( UTIL )
	#include "video.h"
	#else
	#include "video2.h"
	#endif

	#include "himage.h"
	#include "vobject.h"
	#include "vobject_blitters.h"

	#include <sstream>
#endif
//*******************************************************
//
//	Defines
//
//*******************************************************

#define PALETTE_SIZE	 768
#define STRING_DELIMITER 0
#define ID_BLACK		 0
#define MAX_FONTS				25

//*******************************************************
//
//	Typedefs
//
//*******************************************************

extern SGPPaletteEntry gSgpPalette[256]; // symbol already declared globally in video.cpp (jonathanl)

typedef struct
{
	UINT16 usDefaultPixelDepth;
	FontTranslationTable *pTranslationTable;
} FontManager;

FontManager *pFManager;
HVOBJECT	FontObjs[MAX_FONTS];
INT32		FontsLoaded=0;

// Destination printing parameters
INT32			FontDefault=(-1);
UINT32		FontDestBuffer=BACKBUFFER;

UINT32		FontDestPitch=640*2;

UINT32		FontDestBPP=16;
SGPRect		FontDestRegion={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};


BOOLEAN		FontDestWrap=FALSE;
UINT16		FontForeground16=0;
UINT16		FontBackground16=0;
UINT16		FontShadow16=DEFAULT_SHADOW;
UINT8			FontForeground8=0;
UINT8			FontBackground8=0;

// Temp, for saving printing parameters
INT32			SaveFontDefault=(-1);
UINT32		SaveFontDestBuffer=BACKBUFFER;

UINT32		SaveFontDestPitch=640*2;

UINT32		SaveFontDestBPP=16;
SGPRect		SaveFontDestRegion={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

BOOLEAN		SaveFontDestWrap=FALSE;
UINT16		SaveFontForeground16=0;
UINT16		SaveFontShadow16=0;
UINT16		SaveFontBackground16=0;
UINT8			SaveFontForeground8=0;
UINT8			SaveFontBackground8=0;

//*****************************************************************************
// SetFontColors
//
//	Sets both the foreground and the background colors of the current font. The
// top byte of the parameter word is the background color, and the bottom byte
// is the foreground.
//
//*****************************************************************************
void SetFontColors(UINT16 usColors)
{
UINT8 ubForeground, ubBackground;

	ubForeground=(UINT8)(usColors&0xff);
	ubBackground=(UINT8)((usColors&0xff00)>>8);

	SetFontForeground(ubForeground);
	SetFontBackground(ubBackground);

}

//*****************************************************************************
// SetFontForeground
//
//	Sets the foreground color of the currently selected font. The parameter is
// the index into the 8-bit palette. In 8BPP mode, that index number is used
// for the pixel value to be drawn for nontransparent pixels. In 16BPP mode,
// the RGB values from the palette are used to create the pixel color. Note
// that if you change fonts, the selected foreground/background colors will
// stay at what they are currently set to.
//
//*****************************************************************************
void SetFontForeground(UINT8 ubForeground)
{
UINT32 uiRed, uiGreen, uiBlue;

	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
	{
		//return;
		std::wstringstream wss;
		wss << L"invalid font index ( " << FontDefault << L" )";
		SGP_THROW(wss.str().c_str());
	}

	FontForeground8=ubForeground;

	uiRed=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubForeground].peRed;
	uiGreen=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubForeground].peGreen;
	uiBlue=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubForeground].peBlue;

	FontForeground16=Get16BPPColor(FROMRGB(uiRed, uiGreen, uiBlue));
    if ( iUseWinFonts) {
	    if (GET_WINFONT() != -1)
		    {
		     uiRed=FROMRGB(uiRed, uiGreen, uiBlue);
		     SetWinFontForeColor(GET_WINFONT(), &uiRed);
		    }
    }
}

void SetFontShadow(UINT8 ubShadow )
{
UINT32 uiRed, uiGreen, uiBlue;

	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;

	//FontForeground8=ubForeground;

	uiRed=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubShadow].peRed;
	uiGreen=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubShadow].peGreen;
	uiBlue=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubShadow].peBlue;

	FontShadow16=Get16BPPColor(FROMRGB(uiRed, uiGreen, uiBlue));

	if ( ubShadow != 0 )
	{
		if ( FontShadow16 == 0 )
		{
			FontShadow16 = 1;
		}
	}

}

//*****************************************************************************
// SetFontBackground
//
//	Sets the Background color of the currently selected font. The parameter is
// the index into the 8-bit palette. In 8BPP mode, that index number is used
// for the pixel value to be drawn for nontransparent pixels. In 16BPP mode,
// the RGB values from the palette are used to create the pixel color. If the
// background value is zero, the background of the font will be transparent.
// Note that if you change fonts, the selected foreground/background colors will
// stay at what they are currently set to.
//
//*****************************************************************************
void SetFontBackground(UINT8 ubBackground)
{
UINT32 uiRed, uiGreen, uiBlue;

	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
	{
		//return;
		std::wstringstream wss;
		wss << L"invalid color background value ( " << ubBackground << L" )";
		SGP_THROW(wss.str().c_str());
	}

	FontBackground8=ubBackground;

	uiRed=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubBackground].peRed;
	uiGreen=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubBackground].peGreen;
	uiBlue=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubBackground].peBlue;

	FontBackground16=Get16BPPColor(FROMRGB(uiRed, uiGreen, uiBlue));
    if ( iUseWinFonts) {
	    if (GET_WINFONT() != -1)
		    {
		     uiRed=FROMRGB(uiRed, uiGreen, uiBlue);
		     SetWinFontBackColor(GET_WINFONT(), &uiRed);
		    }
    }
}


//Kris:	These are new counterparts to the above functions.	They won't
//			effect an 8BPP font, only 16.
void SetRGBFontForeground( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue )
{
	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;
	FontForeground16 = Get16BPPColor( FROMRGB( uiRed, uiGreen, uiBlue ) );
	if (iUseWinFonts) {
		if (GET_WINFONT() != -1)
		{
			uiRed = FROMRGB(uiRed, uiGreen, uiBlue);
			SetWinFontForeColor(GET_WINFONT(), &uiRed);
		}
	}
}

void SetRGBFontBackground( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue )
{
	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;
	FontBackground16 = Get16BPPColor( FROMRGB( uiRed, uiGreen, uiBlue ) );
	if (iUseWinFonts) {
		if (GET_WINFONT() != -1)
		{
			uiRed = FROMRGB(uiRed, uiGreen, uiBlue);
			SetWinFontBackColor(GET_WINFONT(), &uiRed);
		}
	}
}

void SetRGBFontShadow( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue )
{
	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;
	FontShadow16 = Get16BPPColor( FROMRGB( uiRed, uiGreen, uiBlue ) );
}
//end Kris

// Flugente: set 16 bit colour directly
void SetRGBFontForeground( UINT16 usFontForeground16 )
{
	FontForeground16 = usFontForeground16;
	if (iUseWinFonts) {
		if (GET_WINFONT() != -1)
		{
			UINT32 uiRed = GetRGBColor(usFontForeground16);
			SetWinFontForeColor(GET_WINFONT(), &uiRed);
		}
	}
}

void SetRGBFontShadow( UINT16 usFontShadow16 )
{
	FontShadow16 = usFontShadow16;
}

//*****************************************************************************
// ResetFontObjectPalette
//
//	Sets the palette of a font, using an 8 bit palette (which is converted to
// the appropriate 16-bit palette, and assigned to the HVOBJECT).
//
//*****************************************************************************
BOOLEAN ResetFontObjectPalette(INT32 iFont)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	SetFontObjectPalette8BPP(iFont, FontObjs[iFont]->pPaletteEntry);

	return(TRUE);
}


//*****************************************************************************
// SetFontObjectPalette8BPP
//
//	Sets the palette of a font, using an 8 bit palette (which is converted to
// the appropriate 16-bit palette, and assigned to the HVOBJECT).
//
//*****************************************************************************
UINT16 *SetFontObjectPalette8BPP(INT32 iFont, SGPPaletteEntry *pPal8)
{
UINT16 *pPal16;

	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	if((pPal16=Create16BPPPalette(pPal8))==NULL)
		return(NULL);

	FontObjs[iFont]->p16BPPPalette=pPal16;
	FontObjs[iFont]->pShadeCurrent=pPal16;

	return(pPal16);
}

//*****************************************************************************
// SetFontObjectPalette16BPP
//
//	Sets the palette of a font, using a 16 bit palette.
//
//*****************************************************************************
UINT16 *SetFontObjectPalette16BPP(INT32 iFont, UINT16 *pPal16)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	FontObjs[iFont]->p16BPPPalette=pPal16;
	FontObjs[iFont]->pShadeCurrent=pPal16;

	return(pPal16);

}

//*****************************************************************************
// GetFontObjectPalette16BPP
//
//	Sets the palette of a font, using a 16 bit palette.
//
//*****************************************************************************
UINT16 *GetFontObjectPalette16BPP(INT32 iFont)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	return(FontObjs[iFont]->p16BPPPalette);
}

//*****************************************************************************
// GetFontObject
//
//	Returns the VOBJECT pointer of a font.
//
//*****************************************************************************
HVOBJECT GetFontObject(INT32 iFont)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	return(FontObjs[iFont]);
}

//*****************************************************************************
// FindFreeFont
//
//	Locates an empty slot in the font table.
//
//*****************************************************************************
INT32 FindFreeFont(void)
{
int count;

	for(count=0; count < MAX_FONTS; count++)
		if(FontObjs[count]==NULL)
			return(count);

	return(-1);

}

//*****************************************************************************
// LoadFontFile
//
//	Loads a font from an ETRLE file, and inserts it into one of the font slots.
//	This function returns (-1) if it fails, and debug msgs for a reason.
//	Otherwise the font number is returned.
//*****************************************************************************
INT32 LoadFontFile(const STR8 filename)
{
VOBJECT_DESC		vo_desc;
UINT32					LoadIndex;

	Assert(filename!=NULL);
	Assert(strlen(filename));

	if((LoadIndex=FindFreeFont())==(-1))
	{
		DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, String("Out of font slots (%s)", filename));
#ifdef JA2
			FatalError( "Cannot init FONT file %s", filename );
#endif
			return(-1);
	}

	vo_desc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy(vo_desc.ImageFile, filename);

	if((FontObjs[LoadIndex]=CreateVideoObject(&vo_desc))==NULL)
	{
		DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, String("Error creating VOBJECT (%s)", filename));
#ifdef JA2
			FatalError( "Cannot init FONT file %s", filename );
#endif
			return(-1);
	}

	if(FontDefault==(-1))
		FontDefault=LoadIndex;

	return(LoadIndex);
}

//*****************************************************************************
// UnloadFont - Delete the font structure
//
//	Deletes the video object of a particular font. Frees up the memory and
// resources allocated for it.
//
//*****************************************************************************
void UnloadFont(UINT32 FontIndex)
{
	Assert(FontIndex >= 0);
	Assert(FontIndex < MAX_FONTS);
	Assert(FontObjs[FontIndex]!=NULL);

	DeleteVideoObject(FontObjs[FontIndex]);
	FontObjs[FontIndex]=NULL;
}

//*****************************************************************************
// GetWidth
//
//	Returns the width of a given character in the font.
//
//*****************************************************************************
UINT32 GetWidth(HVOBJECT hSrcVObject, INT16 ssIndex)
{
	ETRLEObject *pTrav;

	// Assertions
	Assert( hSrcVObject != NULL );

	if ( ssIndex < 0 || ssIndex > 92 )
	{
		//int breakpoint=0;
	}

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ ssIndex ] );
	return((UINT32)(pTrav->usWidth+pTrav->sOffsetX));
}

//*****************************************************************************
// StringPixLengthArg
//
//		Returns the length of a string with a variable number of arguments, in
// pixels, using the current font. Maximum length in characters the string can
// evaluate to is 512.
//	'uiCharCount' specifies how many characters of the string are counted.
//*****************************************************************************
INT16 StringPixLengthArg(INT32 usUseFont, UINT32 uiCharCount, STR16 pFontString, ...)
{
va_list argptr;
CHAR16	string[512];

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	// make sure the character count is legal
	if (uiCharCount > wcslen(string))
	{
	uiCharCount = wcslen(string);
	}
	else
	{
	if (uiCharCount < wcslen(string))
	{
		// less than the full string, so whack off the end of it (it's temporary anyway)
		string[uiCharCount] = '\0';
	}
	}

	return(StringPixLength(string, usUseFont));
}

//*****************************************************************************
// StringPixLengthArg
//
// Returns the length of a string with a variable number of arguments, in
// pixels, using the current font. Maximum length in characters the string can
// evaluate to is 512.	Because this is for fast help text, all '|' characters are ignored for the
// width calculation.
// 'uiCharCount' specifies how many characters of the string are counted.
// YOU HAVE TO PREBUILD THE FAST HELP STRING!
//*****************************************************************************
INT16 StringPixLengthArgFastHelp(INT32 usUseFont, INT32 usBoldFont, UINT32 uiCharCount, STR16 pFontString )
{
	CHAR16	string[512];
	UINT32 i, index;
	INT16 sBoldDiff = 0;
	CHAR16 str[2];

	Assert(pFontString!=NULL);

	wcscpy( string, pFontString );

	// make sure the character count is legal
	if (uiCharCount > wcslen(string))
	{
	uiCharCount = wcslen(string);
	}
	else
	{
	if (uiCharCount < wcslen(string))
	{
		// less than the full string, so whack off the end of it (it's temporary anyway)
		string[uiCharCount] = '\0';
	}
	}
	//now eliminate all '|' characters from the string.
	i = 0;
	while( i < uiCharCount )
	{
		if( string[ i ] == '|' )
		{
			for( index = i; index < uiCharCount; index++ )
			{
				string[ index ] = string[ index + 1 ];
			}
			uiCharCount--;
			//now we have eliminated the '|' character, so now calculate the size difference of the
			//bolded character.
			str[ 0 ] = string[ i ];
			str[ 1 ] = 0;
			sBoldDiff += StringPixLength( str, usBoldFont ) - StringPixLength( str, usUseFont );
		}
		i++;
	}
	return StringPixLength(string, usUseFont) + sBoldDiff;
}



//*****************************************************************************************
//
//	StringNPixLength
//
//	Return the length of the of the string or count characters in the
//	string, which ever comes first.
//
//	Returns INT16
//
//	Created by:	 Gilles Beauparlant
//	Created on:	 12/1/99
//
//*****************************************************************************************
INT16 StringNPixLength(STR16 string, UINT32 uiMaxCount, INT32 UseFont)
{
	UINT32 Cur, uiCharCount;
	STR16 curletter;
	UINT16 transletter;

	Cur = 0;
	uiCharCount = 0;
	curletter = string;

	while((*curletter) != L'\0' && uiCharCount < uiMaxCount )
	{
		transletter=GetIndex(*curletter++);
		Cur+=GetWidth(FontObjs[UseFont], transletter);
		uiCharCount++;
	}
	return((INT16)Cur);
}

//*****************************************************************************
//
// StringPixLength
//
//	Returns the length of a string in pixels, depending on the font given.
//
//*****************************************************************************
INT16 StringPixLength(const STR16 string, INT32 UseFont)
{
	UINT32 Cur;
	UINT16 *curletter,transletter;
    if ( iUseWinFonts ) {
	    INT32 MapFont;
	    MapFont = WinFontMap[UseFont];
	    if (MapFont !=-1 )
		    {
		     return(WinFontStringPixLength(string, MapFont));
		    }
    }
	if (string == NULL)
	{
		return(0);
	}

	Cur=0;
	curletter = (UINT16 *)string;

	while((*curletter) != L'\0')
	{
		transletter=GetIndex(*curletter++);
		Cur+=GetWidth(FontObjs[UseFont], transletter);
	}
	return((INT16)Cur);
}

//*****************************************************************************************
//
//	SubstringPixLength
//
//	Return the length of the of a substring of a string
//
//	Returns INT16
//
//	Created by:	 Owen Wigg
//	Created on:	 26/04/2009
//
//*****************************************************************************************
INT16 SubstringPixLength(STR16 string, UINT32 iStart, UINT32 iEnd, INT32 UseFont)
{
	Assert (string != NULL);
	Assert(iStart >= 0 && iStart <= iEnd);
	Assert(iEnd <= wcslen(string));

	INT16 cnt = 0;

	for (unsigned int i=iStart; i <= iEnd; i++)
	{
		cnt += GetWidth(FontObjs[UseFont], GetIndex(string[i]));
	}

	return cnt;
}


//*****************************************************************************
//
// SaveFontSettings
//
//	Saves the current font printing settings into temporary locations.
//
//*****************************************************************************
void SaveFontSettings(void)
{
	SaveFontDefault=FontDefault;
	SaveFontDestBuffer=FontDestBuffer;
	SaveFontDestPitch=FontDestPitch;
	SaveFontDestBPP=FontDestBPP;
	SaveFontDestRegion=FontDestRegion;
	SaveFontDestWrap=FontDestWrap;
	SaveFontForeground16 = FontForeground16;
	SaveFontShadow16 = FontShadow16;
	SaveFontBackground16 = FontBackground16;
	SaveFontForeground8 = FontForeground8;
	SaveFontBackground8 = FontBackground8;
}


//*****************************************************************************
//
// RestoreFontSettings
//
//	Restores the last saved font printing settings from the temporary lactions
//
//*****************************************************************************
void RestoreFontSettings(void)
{
	FontDefault=SaveFontDefault;
	FontDestBuffer=SaveFontDestBuffer;
	FontDestPitch=SaveFontDestPitch;
	FontDestBPP=SaveFontDestBPP;
	FontDestRegion=SaveFontDestRegion;
	FontDestWrap=SaveFontDestWrap;
	FontForeground16 = SaveFontForeground16;
	FontShadow16 = SaveFontShadow16;
	FontBackground16 = SaveFontBackground16;
	FontForeground8 = SaveFontForeground8;
	FontBackground8 = SaveFontBackground8;
}


//*****************************************************************************
// GetHeight
//
//	Returns the height of a given character in the font.
//
//*****************************************************************************
UINT32 GetHeight(HVOBJECT hSrcVObject, INT16 ssIndex)
{
	ETRLEObject *pTrav;

	// Assertions
	Assert( hSrcVObject != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ ssIndex ] );
	return((UINT32)(pTrav->usHeight+pTrav->sOffsetY));
}

//*****************************************************************************
//
// GetFontHeight
//
//	Returns the height of the first character in a font.
//
//*****************************************************************************
UINT16 GetFontHeight(INT32 FontNum)
{
	Assert(FontNum >= 0);
	Assert(FontNum <= MAX_FONTS);
	Assert(FontObjs[FontNum]!=NULL);
    if ( iUseWinFonts ) {
	    INT32 MapFont;
	    MapFont = WinFontMap[FontNum];
	    if (FontNum != -1)
  	    {
  		    return (GetWinFontHeight(L"A", MapFont));
  	    } 
    }
	return((UINT16)GetHeight(FontObjs[FontNum], 0));
}

//*****************************************************************************
// GetIndex
//
//		Given a word-sized character, this function returns the index of the
//	cell in the font to print to the screen. The conversion table is built by
//	CreateEnglishTransTable()
//
//*****************************************************************************
INT16 GetIndex(CHAR16 siChar)
{
	UINT16 *pTrav;
	UINT16 ssCount=0;
	UINT16	usNumberOfSymbols = pFManager->pTranslationTable->usNumberOfSymbols;

//inshy: We don't need anymore ANSI convertation to UNICODE.
//siChar = GetUnicodeChar(siChar);

	// search the Translation Table and return the index for the font
	pTrav = pFManager->pTranslationTable->DynamicArrayOf16BitValues;
	while (ssCount < usNumberOfSymbols )
	{
		if (siChar == *pTrav)
		{
		return ssCount;
	}
		ssCount++;
		pTrav++;
	}

	// If here, present warning and give the first index
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, String("Error: Invalid character given %d", siChar));

	// Return 0 here, NOT -1 - we should see A's here now...
	return 0;
}

//inshy: We don't need anymore ANSI convertation to UNICODE.
/*
CHAR16 GetUnicodeChar(CHAR16 siChar)
{
		#ifdef GERMAN
		   //switch( siChar )
		   //{
		   //     // ?
		   //     case 252:          siChar = 252;          break;
		   //     // ?
		   //     case 220:          siChar = 220;          break;
		   //     // ?
		   //     case 228:          siChar = 228;          break;
		   //     // ?
		   //     case 196:          siChar = 196;          break;
		   //     // ?
		   //     case 246:          siChar = 246;          break;
		   //     // ?
		   //     case 214:          siChar = 214;          break;
		   //     // ?
		   //     case 223:          siChar = 223;          break;
		   //}
	#endif

	#ifdef RUSSIAN
		switch( siChar )
		{
			//capital letters
			case 168:          siChar = 1025;  break;	//U+0401		   d0 81     CYRILLIC CAPITAL LETTER IO
			case 192:          siChar = 1040;  break; //U+0410     A     d0 90     CYRILLIC CAPITAL LETTER A
			case 193:          siChar = 1041;  break;
			case 194:          siChar = 1042;  break;
			case 195:          siChar = 1043;  break;
			case 196:          siChar = 1044;  break;
			case 197:          siChar = 1045;  break;
			case 198:          siChar = 1046;  break;
			case 199:          siChar = 1047;  break;
			case 200:          siChar = 1048;  break;
			case 201:          siChar = 1049;  break;
			case 202:          siChar = 1050;  break;
			case 203:          siChar = 1051;  break;
			case 204:          siChar = 1052;  break;
			case 205:          siChar = 1053;  break;
			case 206:          siChar = 1054;  break;
			case 207:          siChar = 1055;  break;
			case 208:          siChar = 1056;  break;
			case 209:          siChar = 1057;  break;
			case 210:          siChar = 1058;  break;
			case 211:          siChar = 1059;  break;
			case 212:          siChar = 1060;  break;
			case 213:          siChar = 1061;  break;
			case 214:          siChar = 1062;  break;
			case 215:          siChar = 1063;  break;
			case 216:          siChar = 1064;  break;
			case 217:          siChar = 1065;  break;
			case 218:          siChar = 1066;  break;
			case 219:          siChar = 1067;  break;
			case 220:          siChar = 1068;  break;
			case 221:          siChar = 1069;  break;
			case 222:          siChar = 1070;  break;
			case 223:          siChar = 1071;  break; //U+042F           d0 af     CYRILLIC CAPITAL LETTER YA

			//small letters
            case 185:          siChar = 8470;  break;		// ?
            case 178:          siChar = 1030;  break;		// ?
            case 161:          siChar = 1038;  break;		// ?
            case 179:          siChar = 1110;  break;		// ?
            case 162:          siChar = 1118;  break;		// ?
            case 165:          siChar = 1168;  break;		// ?
            case 170:          siChar = 1028;  break;		// ?
            case 175:          siChar = 1031;  break;		// ?
            case 180:          siChar = 1169;  break;		// ?
            case 186:          siChar = 1108;  break;		// ?
            case 191:          siChar = 1111;  break;		// ?

			case 184:          siChar = 1105;  break; //U+0451           d1 91     CYRILLIC SMALL LETTER IO
			case 224:          siChar = 1072;  break; //U+0430     a     d0 b0     CYRILLIC SMALL LETTER A
			case 225:          siChar = 1073;  break;
			case 226:          siChar = 1074;  break;
			case 227:          siChar = 1075;  break;
			case 228:          siChar = 1076;  break;
			case 229:          siChar = 1077;  break;
			case 230:          siChar = 1078;  break;
			case 231:          siChar = 1079;  break;
			case 232:          siChar = 1080;  break;
			case 233:          siChar = 1081;  break;
			case 234:          siChar = 1082;  break;
			case 235:          siChar = 1083;  break;
			case 236:          siChar = 1084;  break;
			case 237:          siChar = 1085;  break;
			case 238:          siChar = 1086;  break;
			case 239:          siChar = 1087;  break; //U+043F           d0 bf     CYRILLIC SMALL LETTER PE
			case 240:          siChar = 1088;  break; //U+0440     p     d1 80     CYRILLIC SMALL LETTER ER
			case 241:          siChar = 1089;  break;
			case 242:          siChar = 1090;  break;
			case 243:          siChar = 1091;  break;
			case 244:          siChar = 1092;  break;
			case 245:          siChar = 1093;  break;
			case 246:          siChar = 1094;  break;
			case 247:          siChar = 1095;  break;
			case 248:          siChar = 1096;  break;
			case 249:          siChar = 1097;  break;
			case 250:          siChar = 1098;  break;
			case 251:          siChar = 1099;  break;
			case 252:          siChar = 1100;  break;
			case 253:          siChar = 1101;  break;
			case 254:          siChar = 1102;  break;
			case 255:          siChar = 1103;  break; //U+044F           d1 8f     CYRILLIC SMALL LETTER YA                
		}
	#endif

	#ifdef POLISH
	   switch( siChar )
	   {
			case 165:          siChar = 260;          break;
			case 198:          siChar = 262;          break;
			case 202:          siChar = 280;          break;
			case 163:          siChar = 321;          break;
			case 209:          siChar = 323;          break;
			case 211:          siChar = 211;          break;

			case 140:          siChar = 346;          break;
			case 175:          siChar = 379;          break;
			case 143:          siChar = 377;          break;
			case 185:          siChar = 261;          break;
			case 230:          siChar = 263;          break;
			case 234:          siChar = 281;          break;

			case 179:          siChar = 322;          break;
			case 241:          siChar = 324;          break;
			case 243:          siChar = 243;          break;
			case 156:          siChar = 347;          break;
			case 191:          siChar = 380;          break;
			case 159:          siChar = 378;          break;
	   }
	#endif

	#ifdef FRENCH
	   switch( siChar )
	   {
			//capital letters
			//case 192:          siChar = 192;          break;	//?
			//case 194:          siChar = 194;          break;	//?
			//case 199:          siChar = 199;          break;	//?
			//case 200:          siChar = 200;          break;	//?
			//case 201:          siChar = 201;          break;	//?
			//case 202:          siChar = 202;          break;	//?
			//case 203:          siChar = 203;          break;	//?
			//case 206:          siChar = 206;          break;	//?
			//case 207:          siChar = 207;          break;	//?
			//case 212:          siChar = 212;          break;	//?
			//case 217:          siChar = 217;          break;	//?
			//case 219:          siChar = 219;          break;	//?
			case 143:          siChar = 376;          break;	//?

			//small letters
			//case 224:          siChar = 224;          break;	//?
			//case 226:          siChar = 226;          break;	//?
			//case 231:          siChar = 231;          break;	//?
			//case 232:          siChar = 232;          break;	//?
			//case 233:          siChar = 233;          break;	//?
			//case 234:          siChar = 234;          break;	//?
			//case 235:          siChar = 235;          break;	//?
			//case 238:          siChar = 238;          break;	//?
			//case 239:          siChar = 239;          break;	//?
			//case 244:          siChar = 244;          break;	//?
			//case 249:          siChar = 249;          break;	//?
			//case 251:          siChar = 251;          break;	//?
			//case 255:          siChar = 255;          break;	//я
//inshy: italian letters
			//case 204:          siChar = 204;          break;	//I'
			//case 236:          siChar = 236;          break;	//i'
			//case 210:          siChar = 210;          break;	//O'
			//case 242:          siChar = 242;          break;	//o'
//inshy: French ligatures
			//Ligature
			//case 198:          siChar = 198;          break;	//?
			//case 140:          siChar = 338;          break;	//?
			//case 230:          siChar = 230;          break;	//?
			//case 156:          siChar = 339;          break;	//?

	   }
	#endif

	return siChar;
}
*/

//*****************************************************************************
// SetFont
//
//	Sets the current font number.
//
//*****************************************************************************
BOOLEAN SetFont(INT32 iFontIndex)
{
	//Assert(iFontIndex >= 0);
	//Assert(iFontIndex <= MAX_FONTS);
	//Assert(FontObjs[iFontIndex]!=NULL);
	SGP_THROW_IFFALSE( iFontIndex >= 0 ,"negative font index");
	SGP_THROW_IFFALSE( iFontIndex <= MAX_FONTS, "font index > MAX_FONTS" );
	SGP_THROW_IFFALSE( FontObjs[iFontIndex]!=NULL, "font is not initialized" );
    if ( iUseWinFonts ) {
	    SET_WINFONT(WinFontMap[iFontIndex]);
    }
	FontDefault=iFontIndex;
	return(TRUE);
}

//*****************************************************************************
// SetFontDestBuffer
//
//	Sets the destination buffer for printing to, the clipping rectangle, and
// sets the line wrap on/off. DestBuffer is a VOBJECT handle, not a pointer.
//
//*****************************************************************************
BOOLEAN SetFontDestBuffer(UINT32 DestBuffer, INT32 x1, INT32 y1, INT32 x2, INT32 y2, BOOLEAN wrap)
{
	Assert(x2 > x1);
	Assert(y2 > y1);

	FontDestBuffer=DestBuffer;

	FontDestRegion.iLeft=x1;
	FontDestRegion.iTop=y1;
	FontDestRegion.iRight=x2;
	FontDestRegion.iBottom=y2;

	FontDestWrap=wrap;

	return(TRUE);
}

//*****************************************************************************
// mprintf
//
//	Prints to the currently selected destination buffer, at the X/Y coordinates
// specified, using the currently selected font. Other than the X/Y coordinates,
// the parameters are identical to printf. The resulting string may be no longer
// than 512 word-characters. Uses monochrome font color settings
//*****************************************************************************
UINT32 mprintf(INT32 x, INT32 y, const STR16 pFontString, ...)
{
INT32		destx, desty;
CHAR16	*curletter, transletter;
va_list argptr;
CHAR16	string[512];
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

    if ( iUseWinFonts ) {
	    if (GET_WINFONT() != -1)
		    {
		     PrintWinFont(FontDestBuffer, GET_WINFONT(), x, y, string);
		     return(0);
		    }
    }

	curletter=string;

	destx=x;
	desty=y;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

	return(0);
}

void VarFindFontRightCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, const STR16 pFontString, ... )
{
	CHAR16	string[512];
	va_list argptr;

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	FindFontRightCoordinates( sLeft, sTop, sWidth, sHeight, string, iFontIndex, psNewX, psNewY );
}

void VarFindFontCenterCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, const STR16 pFontString, ... )
{
	CHAR16	string[512];
	va_list argptr;

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, string, iFontIndex, psNewX, psNewY );
}

void FindFontRightCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, const STR16 pStr, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY )
{
	INT16 xp,yp;

	// Compute the coordinates to right justify the text
	xp = ((sWidth-StringPixLength(pStr,iFontIndex))) + sLeft;
	yp = ((sHeight-GetFontHeight(iFontIndex)) / 2) + sTop;

	*psNewX = xp;
	*psNewY = yp;
}

void FindFontCenterCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, const STR16 pStr, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY )
{
	INT16 xp,yp;

	// Compute the coordinates to center the text
	xp = ((sWidth-StringPixLength(pStr,iFontIndex) + 1) / 2) + sLeft;
	yp = ((sHeight-GetFontHeight(iFontIndex)) / 2) + sTop;

	*psNewX = xp;
	*psNewY = yp;
}

//*****************************************************************************
// gprintf
//
//	Prints to the currently selected destination buffer, at the X/Y coordinates
// specified, using the currently selected font. Other than the X/Y coordinates,
// the parameters are identical to printf. The resulting string may be no longer
// than 512 word-characters.
//*****************************************************************************
UINT32 gprintf(INT32 x, INT32 y, const STR16 pFontString, ...)
{
INT32		destx, desty;
CHAR16	*curletter, transletter;
va_list argptr;
CHAR16	string[512];
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

    if ( iUseWinFonts ) {
	    if (GET_WINFONT() != -1)
		    {
		     PrintWinFont(FontDestBuffer, GET_WINFONT(), x, y, string);
		     return(0);
		    }
    }
	curletter=string;

	destx=x;
	desty=y;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		else
		{
			Blt8BPPDataTo16BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

	return(0);
}

UINT32 gprintfDirty(INT32 x, INT32 y, const STR16 pFontString, ...)
{
INT32		destx, desty;
CHAR16	*curletter, transletter;
va_list argptr;
CHAR16	string[512];
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);
    if ( iUseWinFonts ) {
	    if (GET_WINFONT() != -1)
		    {
		     PrintWinFont(FontDestBuffer, GET_WINFONT(), x, y, string);
  	 	     InvalidateRegion(x, y, x + StringPixLength(string, FontDefault), y + GetFontHeight(FontDefault));
		     return(0);
		    }
    }
	curletter=string;

	destx=x;
	desty=y;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		else
		{
			Blt8BPPDataTo16BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

#if defined ( JA2 ) || defined( UTIL )
	InvalidateRegion(x, y,
										x + StringPixLength(string, FontDefault),
										y + GetFontHeight(FontDefault));
#else
	InvalidateRegion(x, y,
										x + StringPixLength(string, FontDefault),
										y + GetFontHeight(FontDefault),
										INVAL_SRC_TRANS);
#endif

	return(0);
}
//*****************************************************************************
// gprintf_buffer
//
//	Prints to the currently selected destination buffer, at the X/Y coordinates
// specified, using the currently selected font. Other than the X/Y coordinates,
// the parameters are identical to printf. The resulting string may be no longer
// than 512 word-characters.
//*****************************************************************************
UINT32 gprintf_buffer( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, const STR16 pFontString, ...)
{
INT32		destx, desty;
CHAR16	*curletter, transletter;
va_list argptr;
CHAR16	string[512];

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontType], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontType], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		else
		{
			Blt8BPPDataTo16BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}

		destx+=GetWidth(FontObjs[FontType], transletter);
	}

	return(0);
}

UINT32	mprintf_buffer( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, const STR16 pFontString, ...)
{
INT32		destx, desty;
CHAR16	*curletter, transletter;
va_list argptr;
CHAR16	string[512];

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);
    if ( iUseWinFonts ) {
	    if (GET_WINFONT() != -1)
		    {
		     UnLockVideoSurface( CurrentSurface );
		     PrintWinFont(CurrentSurface, GET_WINFONT(), x, y, string);
		     pDestBuf = LockVideoSurface( CurrentSurface, &uiDestPitchBYTES );
		     return(0);
		    }
    }
	curletter=string;

	destx=x;
	desty=y;

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	return(0);
}

UINT32 mprintf_buffer_coded( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, const STR16 pFontString, ...)
{
INT32		destx, desty;
CHAR16	*curletter, transletter;
va_list argptr;
CHAR16	string[512];
UINT16	usOldForeColor;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);
    if ( iUseWinFonts ) {
	    if (GET_WINFONT() != -1)
		    {
		     UnLockVideoSurface( CurrentSurface );
		     PrintWinFont(CurrentSurface, GET_WINFONT(), x, y, pFontString);
		     pDestBuf = LockVideoSurface( CurrentSurface, &uiDestPitchBYTES );
		     return(0);
		    }
    }
	curletter=string;

	destx=x;
	desty=y;

	usOldForeColor = FontForeground16;

	while((*curletter)!=0)
	{
		if ( (*curletter) == 180 )
		{
			curletter++;
			SetFontForeground( (UINT8)(*curletter) );
			curletter++;
		}
		else if ( (*curletter) == 181 )
		{
			FontForeground16 = usOldForeColor;
			curletter++;
		}

		transletter=GetIndex(*curletter++);


		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	return(0);
}


UINT32 mprintf_coded( INT32 x, INT32 y, const STR16 pFontString, ...)
{
INT32		destx, desty;
CHAR16	*curletter, transletter;
va_list argptr;
CHAR16	string[512];
UINT16	usOldForeColor;
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(string, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);
    if ( iUseWinFonts ) {
    	if (GET_WINFONT() != -1)
		{
		 PrintWinFont(FontDestBuffer, GET_WINFONT(), x, y, pFontString);
		 return(0);
		}
    }
	curletter=string;

	destx=x;
	desty=y;

	usOldForeColor = FontForeground16;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		if ( (*curletter) == 180 )
		{
			curletter++;
			SetFontForeground( (UINT8)(*curletter) );
			curletter++;
		}
		else if ( (*curletter) == 181 )
		{
			FontForeground16 = usOldForeColor;
			curletter++;
		}

		transletter=GetIndex(*curletter++);


		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// we get a ctd if we try try to write outside of the screen
		if(desty + GetHeight(FontObjs[FontDefault], transletter) > SCREEN_HEIGHT)
		{
			// don't forget to unlock buffer
			UnLockVideoSurface( FontDestBuffer );
			return 0;
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

	return(0);
}



//*****************************************************************************
// InitializeFontManager
//
//	Starts up the font manager system with the appropriate translation table.
//
//*****************************************************************************
BOOLEAN InitializeFontManager(UINT16 usDefaultPixelDepth, FontTranslationTable *pTransTable)
{
FontTranslationTable *pTransTab;
int count;
UINT16 uiRight, uiBottom;
UINT8 uiPixelDepth;

	FontDefault=(-1);
	FontDestBuffer=BACKBUFFER;
	FontDestPitch=0;

	//	FontDestBPP=0;

	GetCurrentVideoSettings( &uiRight, &uiBottom, &uiPixelDepth );
	FontDestRegion.iLeft = 0;
	FontDestRegion.iTop = 0;
	FontDestRegion.iRight=(INT32)uiRight;
	FontDestRegion.iBottom=(INT32)uiBottom;
	FontDestBPP=(UINT32)uiPixelDepth;


	FontDestWrap=FALSE;

	// register the appropriate debug topics
	if(pTransTable == NULL)
	{
	return FALSE;
	}
	RegisterDebugTopic(TOPIC_FONT_HANDLER, "Font Manager");

	if ((pFManager = (FontManager *)MemAlloc(sizeof(FontManager)))==NULL)
	{
	return FALSE;
	}

	if((pTransTab = (FontTranslationTable *)MemAlloc(sizeof(FontTranslationTable)))==NULL)
	{
	return FALSE;
	}

	pFManager->pTranslationTable = pTransTab;
	pFManager->usDefaultPixelDepth = usDefaultPixelDepth;
	pTransTab->usNumberOfSymbols = pTransTable->usNumberOfSymbols;
	pTransTab->DynamicArrayOf16BitValues = pTransTable->DynamicArrayOf16BitValues;

	// Mark all font slots as empty
	for(count=0; count < MAX_FONTS; count++)
		FontObjs[count]=NULL;

	return TRUE;
}


//*****************************************************************************
// ShutdownFontManager
//
//	Shuts down, and deallocates all fonts.
//*****************************************************************************
void ShutdownFontManager(void)
{
	INT32 count;

	UnRegisterDebugTopic(TOPIC_FONT_HANDLER, "Font Manager");
	if(pFManager)
		MemFree(pFManager);

	for(count=0; count < MAX_FONTS; count++)
	{
		if(FontObjs[count]!=NULL)
			UnloadFont(count);
	}
}


//*****************************************************************************
// DestroyEnglishTransTable
//
// Destroys the English text->font map table.
//*****************************************************************************
void DestroyEnglishTransTable( void )
{
	if(pFManager)
	{
		if (pFManager->pTranslationTable != NULL)
		{
			if (pFManager->pTranslationTable->DynamicArrayOf16BitValues != NULL)
			{
				MemFree( pFManager->pTranslationTable->DynamicArrayOf16BitValues );
			}

			MemFree( pFManager->pTranslationTable );

			pFManager->pTranslationTable = NULL;
		}
	}
}


//*****************************************************************************
// CreateEnglishTransTable
//
// Creates the English text->font map table.
//*****************************************************************************
FontTranslationTable *CreateEnglishTransTable(	)
{
	FontTranslationTable *pTable = NULL;
	UINT16	*temp;

	pTable = (FontTranslationTable *)MemAlloc(sizeof(FontTranslationTable));
	memset(pTable, 0, sizeof(FontTranslationTable) );

	//#ifdef JA2
	//	// ha ha, we have more than Wizardry now (again)
	//	pTable->usNumberOfSymbols = 255;
	//#else
	//	pTable->usNumberOfSymbols = 255;
	//#endif

	pTable->usNumberOfSymbols = 255;

	pTable->DynamicArrayOf16BitValues = (UINT16 *)MemAlloc(pTable->usNumberOfSymbols * 2);
	temp = pTable->DynamicArrayOf16BitValues;

	*temp = 'A';
	temp++;
	*temp='B';
	temp++;
	*temp ='C';
	temp++;
	*temp ='D';
	temp++;
	*temp = 'E';
	temp++;
	*temp = 'F';
	temp++;
	*temp='G';
	temp++;
	*temp ='H';
	temp++;
	*temp ='I';
	temp++;
	*temp = 'J';
	temp++;
	*temp = 'K';
	temp++;
	*temp='L';
	temp++;
	*temp ='M';
	temp++;
	*temp ='N';
	temp++;
	*temp = 'O';
	temp++;
	*temp = 'P';
	temp++;
	*temp='Q';
	temp++;
	*temp ='R';
	temp++;
	*temp ='S';
	temp++;
	*temp = 'T';
	temp++;
	*temp ='U';
	temp++;
	*temp ='V';
	temp++;
	*temp = 'W';
	temp++;
	*temp = 'X';
	temp++;
	*temp='Y';
	temp++;
	*temp ='Z';
	temp++;
	*temp ='a';
	temp++;
	*temp = 'b';
	temp++;
	*temp ='c';
	temp++;
	*temp = 'd';
	temp++;
	*temp ='e';
	temp++;
	*temp = 'f';
	temp++;
	*temp ='g';
	temp++;
	*temp = 'h';
	temp++;
	*temp ='i';
	temp++;
	*temp = 'j';
	temp++;
	*temp ='k';
	temp++;
	*temp = 'l';
	temp++;
	*temp ='m';
	temp++;
	*temp = 'n';
	temp++;
	*temp ='o';
	temp++;
	*temp = 'p';
	temp++;
	*temp ='q';
	temp++;
	*temp = 'r';
	temp++;
	*temp ='s';
	temp++;
	*temp = 't';
	temp++;
	*temp ='u';
	temp++;
	*temp = 'v';
	temp++;
	*temp ='w';
	temp++;
	*temp = 'x';
	temp++;
	*temp ='y';
	temp++;
	*temp = 'z';
	temp++;
	*temp ='0';
	temp++;
	*temp = '1';
	temp++;
	*temp ='2';
	temp++;
	*temp = '3';
	temp++;
	*temp ='4';
	temp++;
	*temp = '5';
	temp++;
	*temp ='6';
	temp++;
	*temp = '7';
	temp++;
	*temp ='8';
	temp++;
	*temp = '9';
	temp++;
	*temp ='!';
	temp++;
	*temp = '@';
	temp++;
	*temp ='#';
	temp++;
	*temp = '$';
	temp++;
	*temp ='%';
	temp++;
	*temp = '^';
	temp++;
	*temp ='&';
	temp++;
	*temp = '*';
	temp++;
	*temp ='(';
	temp++;
	*temp = ')';
	temp++;
	*temp ='-';
	temp++;
	*temp = '_';
	temp++;
	*temp ='+';
	temp++;
	*temp = '=';
	temp++;
	*temp ='|';
	temp++;
	*temp = '\\';
	temp++;
	*temp ='{';
	temp++;
	*temp = '}';// 80
	temp++;
	*temp = '[';
	temp++;
	*temp =']';
	temp++;
	*temp = ':';
	temp++;
	*temp =';';
	temp++;
	*temp = '"';
	temp++;
	*temp ='\'';
	temp++;
	*temp = '<';
	temp++;
	*temp = '>';
	temp++;
	*temp =',';
	temp++;
	*temp = '.';
	temp++;
	*temp ='?';
	temp++;
	*temp = '/';
	temp++;
	*temp = ' '; //93
	temp++;

	// GERMAN letters in UNICODE
	*temp = 196; // "A" umlaut
	temp++;
	*temp = 214; // "O" umlaut
	temp++;
	*temp = 220; // "U" umlaut
	temp++;
	*temp = 228; // "a" umlaut
	temp++;
	*temp = 246; // "o" umlaut
	temp++;
	*temp = 252; // "u" umlaut
	temp++;
	*temp = 223; // double-s that looks like a beta/B
	temp++;

	// RUSSIAN letters in UNICODE
	*temp = 1040; //?
	temp++;
	*temp = 1041; //?
	temp++;
	*temp = 1042; //?
	temp++;
	*temp = 1043; //?
	temp++;
	*temp = 1044; //?
	temp++;
	*temp = 1045; //?
	temp++;
	*temp = 1046; //?
	temp++;
	*temp = 1047; //?
	temp++;
	*temp = 1048; //?
	temp++;
	*temp = 1049; //?
	temp++;
	*temp = 1050; //?
	temp++;
	*temp = 1051; //?
	temp++;
	*temp = 1052; //?
	temp++;
	*temp = 1053; //?
	temp++;
	*temp = 1054; //?
	temp++;
	*temp = 1055; //?
	temp++;
	*temp = 1056; //?
	temp++;
	*temp = 1057; //?
	temp++;
	*temp = 1058; //?
	temp++;
	*temp = 1059; //?
	temp++;
	*temp = 1060; //?
	temp++;
	*temp = 1061; //?
	temp++;
	*temp = 1062; //?
	temp++;
	*temp = 1063; //?
	temp++;
	*temp = 1064; //?
	temp++;
	*temp = 1065; //?
	temp++;
	*temp = 1066; //?
	temp++;
	*temp = 1067; //?
	temp++;
	*temp = 1068; //?
	temp++;
	*temp = 1069; //?
	temp++;
	*temp = 1070; //?
	temp++;
	*temp = 1071; //?
	temp++;	
	*temp = 1072; // ?
	temp++;
	*temp = 1073; // ?
	temp++;
	*temp = 1074; // ?
	temp++;
	*temp = 1075; // ?
	temp++;
	*temp = 1076; // ?
	temp++;
	*temp = 1077; // ?
	temp++;
	*temp = 1078; // ?
	temp++;
	*temp = 1079; // ?
	temp++;
	*temp = 1080; // ?
	temp++;
	*temp = 1081; // ?
	temp++;
	*temp = 1082; // ?
	temp++;
	*temp = 1083; // ?
	temp++;
	*temp = 1084; // ?
	temp++;
	*temp = 1085; // ?
	temp++;
	*temp = 1086; // ?
	temp++;
	*temp = 1087; // ?
	temp++;
	*temp = 1088; // ?
	temp++;
	*temp = 1089; // ?
	temp++;
	*temp = 1090; // ?
	temp++;
	*temp = 1091; // ?
	temp++;
	*temp = 1092; // ?
	temp++;
	*temp = 1093; // ?
	temp++;
	*temp = 1094; // ?
	temp++;
	*temp = 1095; // ?
	temp++;
	*temp = 1096; // ?
	temp++;
	*temp = 1097; // ?
	temp++;
	*temp = 1098; // ?
	temp++;
	*temp = 1099; // ?
	temp++;
	*temp = 1100; // ?
	temp++;
	*temp = 1101; // ?
	temp++;
	*temp = 1102; // ?
	temp++;
	*temp = 1103; // я
	temp++;

	// BELORUSSIAN and UKRAINIAN letters in UNICODE
	*temp = 8470; // ?
	temp++;
	*temp = 1025; // ?
	temp++;
	*temp = 1030; // ?
	temp++;
	*temp = 1038; // ?
	temp++;
	*temp = 1105; // ?
	temp++;
	*temp = 1110; // ?
	temp++;
	*temp = 1118; // ?
	temp++;
	*temp = 1168; // ?
	temp++;
	*temp = 1028; // ?
	temp++;
	*temp = 1031; // ?
	temp++;
	*temp = 1169; // ?
	temp++;
	*temp = 1108; // ?
	temp++;
	*temp = 1111; // ?
	temp++;

	// POLISH letters in UNICODE
	*temp = 260; // ?(on)
	temp++;
	*temp = 262; // ?(tse)
	temp++;
	*temp = 280; // ?(en')
	temp++;
	*temp = 321; // ?(el')
	temp++;
	*temp = 323; // ?(en)
	temp++;
	*temp = 211; // ?(o kratkoe)
	temp++;
	*temp = 346; // ?(es')
	temp++;
	*temp = 379; // ?(zhet)
	temp++;
	*temp = 377; // ?(zet)
	temp++;
	*temp = 261; // ?(он)
	temp++;
	*temp = 263; // ?(це)
	temp++;
	*temp = 281; // ?(эн)
	temp++;
	*temp = 322; // ?(эл?
	temp++;
	*temp = 324; // ?(эн?
	temp++;
	*temp = 243; // ?(?кратко?
	temp++;
	*temp = 347; // ?(эс?
	temp++;
	*temp = 380; // ?(же?
	temp++;
	*temp = 378; // ?(зе?
	temp++;

	// FRENCH and ITALIAN letters in UNICODE
	*temp = 192; //?
	temp++;
	*temp = 194; //?
	temp++;
	*temp = 199; //?
	temp++;
	*temp = 200; //?
	temp++;
	*temp = 201; //?
	temp++;
	*temp = 202; //?
	temp++;
	*temp = 203; //?
	temp++;
	*temp = 206; //?
	temp++;
	*temp = 207; //?
	temp++;
	*temp = 212; //?
	temp++;
	*temp = 217; //?
	temp++;
	*temp = 219; //?
	temp++;
	*temp = 376; //?
	temp++;
//small letters
	*temp = 224; //?
	temp++;
	*temp = 226; //?
	temp++;
	*temp = 231; //?
	temp++;
	*temp = 232; //?
	temp++;
	*temp = 233; //?
	temp++;
	*temp = 234; //?
	temp++;
	*temp = 235; //?
	temp++;
	*temp = 238; //?
	temp++;
	*temp = 239; //?
	temp++;
	*temp = 244; //?
	temp++;
	*temp = 249; //?
	temp++;
	*temp = 251; //?
	temp++;
	*temp = 255; //я
	temp++;
//Italian letters
	*temp = 204; //I'
	temp++;
	*temp = 236; //i'
	temp++;
	*temp = 210; //O'
	temp++;
	*temp = 242; //o'
	temp++;

//French ligatures
	*temp = 198; //?
	temp++;
	*temp = 338; //?
	temp++;
	*temp = 230; //?
	temp++;
	*temp = 339; //?
	temp++;


	// Font glyphs for spell targeting icons
	//ATE: IMPORTANT! INcreate the array above if you add any new items here...
	//temp++;
	//*temp = FONT_GLYPH_TARGET_POINT;
	//temp++;
	//*temp = FONT_GLYPH_TARGET_CONE;
	//temp++;
	//*temp = FONT_GLYPH_TARGET_SINGLE;
	//temp++;
	//*temp = FONT_GLYPH_TARGET_GROUP;
	//temp++;
	//*temp = FONT_GLYPH_TARGET_NONE;

	// 200
	
	return pTable;
}

//*****************************************************************************
//
// LoadFontFile
//
// Parameter List : filename - File created by the utility tool to open
//
// Return Value	pointer to the base structure
//
// Modification History :
// Dec 15th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*FontBase *LoadFontFile(UINT8 *pFilename)
{
	HWFILE			hFileHandle;
	UINT32			uiFileSize;
	UINT32			uiHeightEach;
	UINT32			uiTotalSymbol;
	UINT32			uiNewoffst, uiOldoffst;
	FontBase		*pFontBase;
	SGPPaletteEntry *pNewPalette;
	UINT8			*pPalette;

	if (pFManager == NULL)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Did not Initialize Font Manager");
	return NULL;
	}

	// Open and read in the file
	if ((hFileHandle = FileOpen(pFilename, FILE_ACCESS_READ, FALSE)) == 0)
	{ // damn we failed to open the file
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Cannot open font file");
	return NULL;
	}

	uiFileSize = FileGetSize(hFileHandle);
	if (uiFileSize == 0)
	{ // we failed to size up the file
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Font file is empty");
	FileClose(hFileHandle);
	return NULL;
	}

	// Allocate memory for the font header file
	if ((pFontBase = (FontBase *)MemAlloc(sizeof(FontBase))) == NULL)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not malloc memory");
	FileClose(hFileHandle);
	}

	// read in these values from the file
	if (FileRead(hFileHandle, &uiHeightEach, sizeof(UINT32), NULL) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not read Height from File");
	FileClose(hFileHandle);
	return NULL;
	}

	if (FileRead(hFileHandle, &uiTotalSymbol, sizeof(UINT32), NULL) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not read Total Symbol from File");
	FileClose(hFileHandle);
	return NULL;
	}

	// Assign the proper values to the Base structure
	pFontBase->uiHeightEach = uiHeightEach;
	pFontBase->uiTotalElements = uiTotalSymbol;
	pFontBase->pFontObject = (FontObject *)MemAlloc(uiTotalSymbol * sizeof(FontHeader));
	pPalette = (UINT8 *)MemAlloc(PALETTE_SIZE);
	uiOldoffst = (sizeof(FontHeader) + sizeof(FontObject)*pFontBase->uiTotalElements);
	uiNewoffst = uiFileSize - uiOldoffst;
	pFontBase->pPixData8 = (UINT8 *)MemAlloc(uiNewoffst);

	//seek past the FontHeader
	if (FileSeek(hFileHandle, sizeof(FontHeader), FILE_SEEK_FROM_START) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not seek FileHeader");
	FileClose(hFileHandle);
	return NULL;
	}

	//read in the FontObject
	if (FileRead(hFileHandle, pFontBase->pFontObject, (uiTotalSymbol)*sizeof(FontHeader), NULL) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not seek Font Objects");
	FileClose(hFileHandle);
	return NULL;
	}

	if (FileSeek(hFileHandle, uiOldoffst, FILE_SEEK_FROM_START) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not seek Old offset");
	FileClose(hFileHandle);
	return NULL;
	}

	// read in the Pixel data
	if (FileRead(hFileHandle, pFontBase->pPixData8, uiNewoffst, NULL) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not seek Pixel data");
	FileClose(hFileHandle);
	return NULL;
	}

	// seek proper position to read in Palette
	if (FileSeek(hFileHandle, sizeof(UINT32)*3, FILE_SEEK_FROM_START) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not seek Palette Start");
	FileClose(hFileHandle);
	return NULL;
	}

	// read in Palette
	if (FileRead(hFileHandle, pPalette, PALETTE_SIZE, NULL) == FALSE)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not read Palette");
	FileClose(hFileHandle);
	return NULL;
	}

	// set the default pixel depth
	pFontBase->siPixelDepth = pFManager->usDefaultPixelDepth;
	FileClose(hFileHandle);

	// convert from RGB to SGPPaletteEntry
	pNewPalette = ConvertToPaletteEntry(0, 255, pPalette);
	pFontBase->pPalette = pNewPalette;

	// create the 16BPer Pixel palette
	if ((pFontBase->pPalet16 = Create16BPPPalette(pNewPalette)) == NULL)
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Could not create 16 bit palette");
	return NULL;
	}
	// return the FontBase structure
	return pFontBase;
}	*/



/*void UnloadFont(FontBase *pFontBase)
{
	// free allocated memory in FontBase
	if(pFontBase!=NULL)
	{
		if(pFontBase->pPalette!=NULL)
			MemFree(pFontBase->pPalette);
		if(pFontBase->pPalet16!=NULL)
			MemFree(pFontBase->pPalet16);
		if(pFontBase->pFontObject!=NULL)
			MemFree(pFontBase->pFontObject);
		if(pFontBase->pPixData8!=NULL)
			MemFree(pFontBase->pPixData8);
		if(pFontBase->pPixData16!=NULL)
			MemFree(pFontBase->pPixData16);
		MemFree(pFontBase);
	}
}	*/



//*****************************************************************************
//
// GetMaxFontWidth - Gets the maximum font width
//
// Parameter List : pointer to the base structure
//
// Return Value	Maximum font width
//
// Modification History :
// Dec 15th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*UINT16 GetMaxFontWidth(FontBase *pFontBase)
{
	FontObject *pWidth;
	UINT32 siBiggest = 0;
	UINT16 siCount;

	Assert(pFontBase != NULL);
	pWidth = pFontBase->pFontObject;
	// traverse the FontObject structure to find the biggest width
	for(siCount = 0; siCount < pFontBase->uiTotalElements; siCount++)
	{
		if( pWidth->uiFontWidth > siBiggest)
		{
		siBiggest = pWidth->uiFontWidth;
	}
		pWidth++;
	}
	// return the max width
	return (UINT16)siBiggest;
} */

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

/*
SGPPaletteEntry *ConvertToPaletteEntry(UINT8 sbStart, UINT8 sbEnd, UINT8 *pOldPalette)
{
	UINT16 Index;
	SGPPaletteEntry *pPalEntry;
	SGPPaletteEntry *pInitEntry;

	pPalEntry = (SGPPaletteEntry *)MemAlloc(sizeof(SGPPaletteEntry) * 256);
	pInitEntry = pPalEntry;
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Converting RGB palette to SGPPaletteEntry");
	for(Index=0; Index <= (sbEnd-sbStart);Index++)
	{
	pPalEntry->peRed = *(pOldPalette + (Index*3));
	pPalEntry->peGreen = *(pOldPalette + (Index*3) + 1);
 	pPalEntry->peBlue = *(pOldPalette + (Index*3) + 2);
	pPalEntry->peFlags = 0;
	pPalEntry++;
	}
	return pInitEntry;
} */

//*****************************************************************************
//
// SetFontPalette - Sets the Palette
//
// Parameter List : pointer to the base structure
//					new pixel depth
//					new Palette size
//					pointer to palette data
//
// Return Value	BOOLEAN
//
// Modification History :
// Dec 15th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*BOOLEAN SetFontPalette(FontBase *pFontBase, UINT16 siPixelDepth, SGPPaletteEntry *pPalData)
{
	Assert(pFontBase != NULL);
	Assert(pPalData != NULL);
	MemFree(pFontBase->pPalette);

	// assign the new palette to the Base structure
	pFontBase->pPalette = pPalData;
	pFontBase->siPixelDepth = siPixelDepth;
	return TRUE;
}	*/

//*****************************************************************************
//
// SetFont16BitData - Sets the font structure to hold 16 bit data
//
// Parameter List : pointer to the base structure
//					pointer to new 16 bit data
//
// Return Value	BOOLEAN
//
// Modification History :
// Dec 15th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*BOOLEAN SetFont16BitData(FontBase *pFontBase, UINT16 *pData16)
{
	Assert(pFontBase != NULL);
	Assert(pData16 != NULL);
	MemFree(pFontBase->pPixData16);
	pFontBase->pPixData16 = pData16;
	return TRUE;
}	*/



//*****************************************************************************
//
// Blt8Imageto16Dest
//
// Parameter List : Start offset
//					End Offset
//					Dest x, y
//					Font Width
//					Pointer to Base structure
//					Pointer to destination buffer
//					Destination Pitch
//					Height of Each element
//
// Return Value	: BOOLEAN
//
// Modification History :
// Nov 26th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*BOOLEAN Blt8Imageto16Dest(UINT32 uiOffStart, UINT32 uiOffEnd, UINT16 siX, UINT16 siY, UINT32 uiWidth, FontBase *pFontBase, UINT8 *pFrameBuffer, UINT16 siDestPitch, UINT16 siHeightEach)
{
	UINT8	*pTrav;
	UINT16 *pFrameTrav;
	UINT16 *p16BPPPalette;
	UINT16	usEffectiveWidth;
	UINT32	uiFrameCount;
	UINT8	amount;
	UINT32	row, count;
	UINT16	modamount, divamount;
	UINT32 trace,modtrace;
	UINT8 sub=0;


	pTrav = pFontBase->pPixData8;
	pFrameTrav = (UINT16 *)pFrameBuffer;
	p16BPPPalette = pFontBase->pPalet16;
	trace = 0;
	modtrace = 0;
	// effective width is pitch/2 as 16 bits per pixel
	usEffectiveWidth = (UINT16)(siDestPitch / 2);
	uiFrameCount = siY*usEffectiveWidth + siX;
	trace += uiFrameCount;
	modtrace = trace % 640;
	pFrameTrav += uiFrameCount;
	pTrav += uiOffStart;

	count = 0;
	row = 0;
	amount = 0;
	while (count < (uiOffEnd-uiOffStart))
	{
	amount = 0;
	if (*pTrav == ID_BLACK)
	{
		pTrav++;
		count++;
		amount = *pTrav;
		modamount = (UINT8)(amount) % (UINT8) uiWidth;
		divamount = (UINT8)(amount) / (UINT8) uiWidth;
		if ((divamount == 0) && ((row+amount) < (UINT16)uiWidth))
		{
			pFrameTrav += amount;
			trace += amount;
			modtrace = trace % 640;
			row += amount;
			row++;
		}
		else
		{
		if (((row+amount) >= (UINT16)uiWidth) && (divamount ==0))
		 {
				pFrameTrav -= row;
				trace -= row;
			 modtrace = trace % 640;
			 row = amount-((UINT16)uiWidth-row);
			 pFrameTrav += usEffectiveWidth+row;
				trace += usEffectiveWidth+row;
				modtrace = trace % 640;
			 row++;
		 }
		 else
		 {
			 pFrameTrav += (divamount*usEffectiveWidth);
				trace += (divamount*usEffectiveWidth);
				modtrace = trace % 640;
				if(row+modamount > uiWidth)
				{
					sub = (UINT8)((row+modamount) % uiWidth);
					pFrameTrav -= row;
				 trace -= row;
				 modtrace = trace % 640;
					pFrameTrav += usEffectiveWidth+sub;
				 trace += usEffectiveWidth + sub;
				 modtrace = trace % 640;
				 row = sub;
				 row++;
				}else
				{
					pFrameTrav += modamount;
				 trace += modamount;
				 modtrace = trace % 640;
				 row = modamount;
				 row++;
				}
		 }
		}
	} else
	{
		if(row >= uiWidth)
		{
			pFrameTrav += (usEffectiveWidth-uiWidth);
			trace += (usEffectiveWidth-uiWidth);
			modtrace = trace % 640;
			*pFrameTrav = p16BPPPalette[*pTrav];
		 row = 1;
		}
		else
		{
			*pFrameTrav = p16BPPPalette[*pTrav];
		 row++;
		}
	}

	pFrameTrav++;
	trace++;
	modtrace = trace % 640;
	pTrav++;
	count++;
	}

	return TRUE;
}	*/


//*****************************************************************************
//
// Blt8Imageto8Dest
//
// Parameter List : Start offset
//					End Offset
//					Dest x, y
//					Font Width
//					Pointer to Base structure
//					Pointer to destination buffer
//					Destination Pitch
//					Height of Each element
//
// Return Value	: BOOLEAN
//
// Modification History :
// Nov 26th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*BOOLEAN Blt8Imageto8Dest(UINT32 uiOffStart, UINT32 uiOffEnd, UINT16 siX, UINT16 siY, UINT32 uiWidth, FontBase *pFontBase, UINT8 *pFrameBuffer, UINT16 siDestPitch, UINT16 siHeightEach)
{
	UINT8	*pTrav;
	UINT32	uiFrameCount;
	UINT8	*pFrameTrav;
	UINT8	amount;
	UINT32	row,count;
	UINT16	modamount,divamount;

	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Blitting 8 to 8");
	Assert(pFontBase != NULL);
	Assert(pFrameBuffer != NULL);

	// get the pointers
	pTrav = pFontBase->pPixData8;
	pFrameTrav = (UINT8 *)pFrameBuffer;

	uiFrameCount = siY*siDestPitch + siX;
	pFrameTrav +=uiFrameCount;
	pTrav += uiOffStart;
	// perform blitting

	count=0;
	row = 0;
	amount = 0;
	while (count < (uiOffEnd-uiOffStart))
	{
	amount = 0;
	if (*pTrav == ID_BLACK)
	{
		pTrav++;
		count++;
		amount = *pTrav;
		modamount = amount % (UINT8) uiWidth;
		divamount = amount / (UINT8) uiWidth;
		if ((divamount == 0) && ((row+amount) < (UINT16)uiWidth))
		{
			pFrameTrav += amount;
			row += amount;
			row++;
		}
		else
		{
		if (((row+amount) >= (UINT16)uiWidth) && (divamount ==0))
		 {
			pFrameTrav -= row;
			 row = amount-((UINT16)uiWidth-row);
			 pFrameTrav += siDestPitch+row;
			 row++;
		 }
		 else
		 {
			 pFrameTrav += (divamount*siDestPitch)+modamount;
			 row = modamount;
			 row++;
		 }
		}
	} else
	{
		if (row >= uiWidth)
		{
		pFrameTrav += (siDestPitch-uiWidth);
		*pFrameTrav = *pTrav;
		 row = 1;
		}
		else
		{
		*pFrameTrav = *pTrav;
		 row++;
		}
	}

	pFrameTrav++;
	pTrav++;
		count++;
	}

	return TRUE;
} */

//*****************************************************************************
//
// Blt16Imageto16Dest
//
// Parameter List : Start offset
//					End Offset
//					Dest x, y
//					Font Width
//					Pointer to Base structure
//					Pointer to destination buffer
//					Destination Pitch
//					Height of Each element
//
// Return Value	: BOOLEAN
//
// Modification History :
// Nov 26th 1996->modified for use by Wizardry
//
//*****************************************************************************
/*BOOLEAN Blt16Imageto16Dest(UINT32 uiOffStart, UINT32 uiOffEnd, UINT16 siX, UINT16 siY, UINT32 uiWidth, FontBase *pFontBase, UINT8 *pFrameBuffer, UINT16 siDestPitch, UINT16 siHeightEach)
{
	UINT16 *pTrav;
	UINT32	uiFrameCount;
	UINT16 *pFrameTrav;
	UINT16	amount;
	UINT32	row,count;
	UINT16	modamount,divamount;
	UINT16	usEffectiveWidth;

	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Blitting 16 to 16");
	Assert(pFontBase != NULL);
	Assert(pFrameBuffer != NULL);

	//get the pointers
	pTrav = pFontBase->pPixData16;
	pFrameTrav = (UINT16 *)pFrameBuffer;

	// effective width is pitch/2 as 16 bits per pixel
	usEffectiveWidth = (UINT16)(siDestPitch / 2);
	uiFrameCount = siY*usEffectiveWidth + siX;
	pFrameTrav +=uiFrameCount;
	pTrav += uiOffStart;

	count=0;
	row = 0;
	amount = 0;
	while (count < (uiOffEnd-uiOffStart))
	{
	amount = 0;
	if (*pTrav == ID_BLACK)
	{
		pTrav++;
		count++;
		amount = *pTrav;
		modamount = amount % (UINT8) uiWidth;
		divamount = amount / (UINT8) uiWidth;
		if ((divamount == 0) && ((row+amount) < (UINT16)uiWidth))
		{
			pFrameTrav += amount;
			row += amount;
			row++;
		}
		else
		{
		if (((row+amount) >= (UINT16)uiWidth) && (divamount ==0))
		 {
			pFrameTrav -= row;
			 row = amount-((UINT16)uiWidth-row);
			 pFrameTrav += usEffectiveWidth+row;
			 row++;
		 }
		 else
		 {
			 pFrameTrav += (divamount*usEffectiveWidth)+modamount;
			 row = modamount;
			 row++;
		 }
		}
	} else
	{
		if(row >= uiWidth)
		{
		pFrameTrav += (usEffectiveWidth-uiWidth);
		*pFrameTrav = *pTrav;
		 row = 1;
		}
		else
		{
		*pFrameTrav = *pTrav;
		 row++;
		}
	}

	pFrameTrav++;
		pTrav++;
		count++;
	}

	return TRUE;
}	*/

//*****************************************************************************
//
// GetOffset
//
// Parameter List : Given the index, gets the corresponding offset
//
// Return Value	: offset
//
// Modification History :
// Nov 26th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*UINT32 GetOffset(FontBase *pFontBase, INT16 ssIndex)
{
	FontObject *pTrav;
	UINT16 siCount=0;

	Assert(pFontBase != NULL);
	// gets the offset based on the index
	if (((UINT32)ssIndex > pFontBase->uiTotalElements) || (ssIndex < 0))
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Incorrect index value passed");
	return 0;
	}
	pTrav = pFontBase->pFontObject;
	while (siCount != ssIndex)
	{
	siCount++;
	pTrav++;
	}

	return pTrav->uiFontOffset;
} */


//*****************************************************************************
//
// GetOffLen
//
// Parameter List : Given the index, gets the corresponding offset
// length which is the number of compressed pixels
//
// Return Value	: offset
//
// Modification History :
// Nov 26th 1996->modified for use by Wizardry
//
//*****************************************************************************
/*UINT32 GetOffLen(FontBase *pFontBase, INT16 ssIndex)
{
	FontObject *pTrav;
	UINT16 siCount=0;

	Assert(pFontBase != NULL);
	// gets the offset based on the index
	if (((UINT32)ssIndex > pFontBase->uiTotalElements) || (ssIndex < 0))
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Incorrect index value passed");
	return 0;
	}
	pTrav = pFontBase->pFontObject;
	while(siCount != ssIndex)
	{
	siCount++;
	pTrav++;
	}

	return pTrav->uiOffLen;
} */


//*****************************************************************************
//
// PrintFontString
//
// Parameter List : pointer to \0 (NULL) terminated font string
//					x,y,TotalWidth, TotalHeight is the bounding rectangle where
//					the font is to be printed
//					Multiline if true will print on multiple lines otherwise on 1 line
//					Pointer to base structure
//
// Return Value	: BOOLEAN
//
// Modification History :
// Nov 26th 1996->modified for use by Wizardry
//
//*****************************************************************************

/*BOOLEAN PrintFontString(UINT16 *pFontString, UINT8 *pDestBuffer, UINT16 siDestWidth, UINT16 siDestPixelDepth, UINT16 siDestPitch, UINT16 siDestHeight, UINT16 siX, UINT16 siY, UINT16 siTotalWidth, UINT16 siTotalHeight, BOOLEAN fMultiLine, FontBase *pFontBase)
{
	UINT16	siScreenHt;
	UINT16	siScreenWt;
	UINT16	siChar, siHeightEach;
	INT16	ssIndex;
	UINT32	uiWidth, uiOffsetSt, uiOffsetEnd, uiOldoffst;
	UINT16 *pTempFStr;
	UINT16	siNewX, siNewY;
	UINT16	siInitX, siInitY;
	UINT32	uiLen;

	// check for NULL pointers passed in
	Assert(pFontBase != NULL);
	Assert(pFontString != NULL);
	Assert(pDestBuffer != NULL);

	siScreenWt = siDestWidth;
	siScreenHt = siDestHeight;

	// check for invalid coordinates
	if((siX<0) || (siX>siScreenWt) || (siY<0) || (siY>siScreenHt) || (siTotalWidth<0) || (siTotalWidth>siScreenWt) ||	(siTotalHeight<0) || (siTotalHeight>siScreenHt))
	{
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Invalid coordinates passed in");
	return FALSE;
	}
	pTempFStr = pFontString;

	siNewX = siX;
	siNewY = siY;
	siInitX = siX;
	siInitY = siY;

	// Get the height of each font and the offset
	siHeightEach = GetFontHeight(pFontBase);
	uiOldoffst = (sizeof(FontHeader) + sizeof(FontObject)*pFontBase->uiTotalElements);

	// calls the blt routine until the string != to \0
	while(*pTempFStr != STRING_DELIMITER)
	{
	siChar = *pTempFStr;
	// get the index value for the font
	if((ssIndex = GetIndex(siChar)) == -1)
		{
		return FALSE;
	}

	// get the width of the font
	uiWidth = GetWidth(pFontBase, ssIndex);

	// get the font offset
	uiOffsetSt = GetOffset(pFontBase, ssIndex);

	uiLen = GetOffLen(pFontBase,ssIndex);

	// uiOffsetSt -= uiOldoffst;
	uiOffsetEnd = uiOffsetSt + uiLen;

	// if Multiline = FALSE and reached the end of line - cannot continue
	if ((((siNewX+uiWidth) > siScreenWt) || ((siNewX+uiWidth) >= siTotalWidth)) && (fMultiLine == FALSE))
	{
	 DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Cannot continue writing");
		return FALSE;
	}

	// check if boundary is reached
	if ((((siNewX+uiWidth) >= siScreenWt) || ((siNewX+uiWidth) >= siTotalWidth)) && (fMultiLine == TRUE))
	{
		if (((siInitY+siHeightEach) > siScreenHt) || ((siInitY+siHeightEach) >= siTotalHeight))
		{
		DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Cannot continue writing");
		return FALSE;
		}
		//call the appropriate blit routines
		siNewX = siInitX;
		siNewY += siHeightEach;
		siInitY = siNewY;
		if ((siDestPixelDepth == 16) && (pFontBase->siPixelDepth == 16))
		{
		Blt16Imageto16Dest(uiOffsetSt, uiOffsetEnd, siNewX, siNewY, uiWidth, pFontBase, pDestBuffer, siDestPitch, siHeightEach);
		}
		else
	 {
		if ((siDestPixelDepth == 16) && (pFontBase->siPixelDepth == 8))
		{
			Blt8Imageto16Dest(uiOffsetSt, uiOffsetEnd, siNewX, siNewY, uiWidth, pFontBase, pDestBuffer, siDestPitch, siHeightEach);
		}
		 else
		{
			if ((siDestPixelDepth == 8) && (pFontBase->siPixelDepth == 8))
			{ // if(SetPalette(pFontBase->pPalette) == FALSE)
			//		 return FALSE;
			 Blt8Imageto8Dest(uiOffsetSt, uiOffsetEnd, siNewX, siNewY, uiWidth, pFontBase, pDestBuffer, siDestPitch, siHeightEach);
			}
			else
			{
			DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Invalid pixel depth / destination surface depth");
			return FALSE;
			}
		}
				siNewX += (UINT16)uiWidth;
		}
	} else
	{ // if it isnt end of boundary copy at current location
	 if((siDestPixelDepth == 16) && (pFontBase->siPixelDepth == 16))
	 {
		Blt16Imageto16Dest(uiOffsetSt, uiOffsetEnd, siNewX, siNewY, uiWidth, pFontBase, pDestBuffer, siDestPitch, siHeightEach);
		}
		else
	 {
		if((siDestPixelDepth == 16) && (pFontBase->siPixelDepth == 8))
		 {
			Blt8Imageto16Dest(uiOffsetSt, uiOffsetEnd, siNewX, siNewY, uiWidth, pFontBase, pDestBuffer, siDestPitch, siHeightEach);
		}
		else
		 {
			if ((siDestPixelDepth == 8) && (pFontBase->siPixelDepth == 8))
			{
			Blt8Imageto8Dest(uiOffsetSt, uiOffsetEnd, siNewX, siNewY, uiWidth, pFontBase, pDestBuffer, siDestPitch, siHeightEach);
			}
			else
			{
			DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, "Invalid pixel depth / destination surface depth");
			return FALSE;
			}
		}
		siNewX += (UINT16)uiWidth;
	 }
	}
	// increment string pointer
	pTempFStr++;
	}
	return TRUE;
}

*/

/*BOOLEAN InitializeFontManager(UINT16 usDefaultPixelDepth, FontTranslationTable *pTransTable)
{
FontTranslationTable *pTransTab;

	// register the appropriate debug topics
	if(pTransTable == NULL)
	{
	return FALSE;
	}
	RegisterDebugTopic(TOPIC_FONT_HANDLER, "Font Manager");

	if ((pFManager = (FontManager *)MemAlloc(sizeof(FontManager)))==NULL)
	{
	return FALSE;
	}

	if((pTransTab = (FontTranslationTable *)MemAlloc(sizeof(FontTranslationTable)))==NULL)
	{
	return FALSE;
	}

	pFManager->pTranslationTable = pTransTab;
	pFManager->usDefaultPixelDepth = usDefaultPixelDepth;
	pTransTab->usNumberOfSymbols = pTransTable->usNumberOfSymbols;
	pTransTab->DynamicArrayOf16BitValues = pTransTable->DynamicArrayOf16BitValues;

	return TRUE;
}	*/

/*void ShutdownFontManager(void)
{
	UnRegisterDebugTopic(TOPIC_FONT_HANDLER, "Font Manager");
	MemFree(pFManager);
}	*/


