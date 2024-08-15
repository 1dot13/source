	#include "types.h"
	#include <stdio.h>
	#include <Windows.h>
	#include "sgp.h"
	#include "time.h"
	#include "vobject.h"
	#include "FileMan.h"
	#include "Utilities.h"
	#include "Font Control.h"
	#include "overhead.h"
	#include "overhead types.h"
	#include "wcheck.h"
	#include "sys globals.h"


extern BOOLEAN GetCDromDriveLetter( STR8	pString );

#define		DATA_8_BIT_DIR	"8-Bit\\"


// WANNE: Given a string, replaces all instances of "oldpiece" with "newpiece"
/*
 *
 * Modified this routine to eliminate recursion and to avoid infinite
 * expansion of string when newpiece contains oldpiece.	--Byron
*/
//STR8 Replace(STR8 string, STR8 oldpiece, STR8 newpiece)
//{
//	int str_index, newstr_index, oldpiece_index, end,
//
//		new_len, old_len, cpy_len;
//	STR8 c;
//	static char newstring[MAXLINE];
//
//	if ((c =	strstr(string, oldpiece)) == NULL)
//
//		return string;
//
//	new_len		= strlen(newpiece);
//	old_len		= strlen(oldpiece);
//	end			= strlen(string)	- old_len;
//	oldpiece_index = c - string;
//
//
//	newstr_index = 0;
//	str_index = 0;
//	while(str_index <= end && c != NULL)
//	{
//
//		//Copy characters from the left of matched pattern occurence
//		cpy_len = oldpiece_index-str_index;
//		strncpy(newstring+newstr_index, string+str_index, cpy_len);
//		newstr_index += cpy_len;
//		str_index	+= cpy_len;
//
//		//Copy replacement characters instead of matched pattern
//		strcpy(newstring+newstr_index, newpiece);
//		newstr_index += new_len;
//		str_index	+= old_len;
//
//		//Check for another pattern match
//		if((c =	strstr(string+str_index, oldpiece)) != NULL)
//		 oldpiece_index = c - string;
//
//
//	}
//	// Copy remaining characters from the right of last matched pattern
//	strcpy(newstring+newstr_index, string+str_index);
//
//	return newstring;
//}

// WANNE: Replaces german	specific characters
//STR8 ReplaceGermanSpecialCharacters(STR8 text)
//{
//	// ä
//	text = Replace(text, "Ã¤", "ä");
//	// Ä
//	text = Replace(text, "Ã„", "Ä");
//	// ö
//	text = Replace(text, "Ã¶", "ö");
//	// Ö
//	text = Replace(text, "Ã–", "Ö");
//	// ü
//	text = Replace(text, "Ã¼", "ü");
//	// Ü
//	text = Replace(text, "Ãœ", "Ü");
//	// ß
//	text = Replace(text, "ÃŸ", "ß");
//
//	return text;
//}



//#define	TIME_LIMITED_VERSION
void FilenameForBPP(STR pFilename, STR pDestination)
{
CHAR8 Drive[128], Dir[128], Name[128], Ext[128];

	if(GETPIXELDEPTH()==16)
	{
		// no processing for 16 bit names
		strcpy(pDestination, pFilename);
	}
	else
	{
		_splitpath(pFilename, Drive, Dir, Name, Ext);

		strcat(Name, "_8");

		strcpy(pDestination, Drive);
		//strcat(pDestination, Dir);
		strcat(pDestination, DATA_8_BIT_DIR);
		strcat(pDestination, Name);
		strcat(pDestination, Ext);
	}

}

BOOLEAN CreateSGPPaletteFromCOLFile( SGPPaletteEntry *pPalette, SGPFILENAME ColFile )
{
	HWFILE	 hFileHandle;
	BYTE			bColHeader[ 8 ];
	UINT32		cnt;

	//See if files exists, if not, return error
	if ( !FileExists( ColFile ) )
	{
		// Return FALSE w/ debug
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot find COL file");
		return( FALSE );
	}

	// Open and read in the file
	if ( ( hFileHandle = FileOpen( ColFile, FILE_ACCESS_READ, FALSE)) == 0)
	{
		// Return FALSE w/ debug
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot open COL file");
		return( FALSE );
	}

	// Skip header
	FileRead( hFileHandle, bColHeader, sizeof( bColHeader ) , NULL);

	// Read in a palette entry at a time
	for ( cnt = 0; cnt < 256; cnt++ )
	{
	FileRead( hFileHandle, &pPalette[ cnt ].peRed, sizeof( UINT8 ) , NULL);
	FileRead( hFileHandle, &pPalette[ cnt ].peGreen, sizeof( UINT8 ) , NULL);
	FileRead( hFileHandle, &pPalette[ cnt ].peBlue, sizeof( UINT8 ) , NULL);
	}

	// Close file
	FileClose( hFileHandle );

	return( TRUE );
}

BOOLEAN DisplayPaletteRep( PaletteRepID aPalRep, UINT8 ubXPos, UINT8 ubYPos, UINT32 uiDestSurface )
{
	UINT16										us16BPPColor;
	UINT32										cnt1;
	UINT8											ubSize, ubType;
	INT16											sTLX, sTLY, sBRX, sBRY;
	UINT8											ubPaletteRep;

	// Create 16BPP Palette
	CHECKF( GetPaletteRepIndexFromID( aPalRep, &ubPaletteRep ) );

	SetFont( LARGEFONT1 );

	ubType = gpPalRep[ ubPaletteRep ].ubType;
	ubSize = gpPalRep[ ubPaletteRep ].ubPaletteSize;

	for ( cnt1 = 0; cnt1 < ubSize; cnt1++ )
	{
		sTLX = ubXPos + (UINT16)( ( cnt1 % 16 ) * 20 );
		sTLY = ubYPos + (UINT16)( ( cnt1 / 16 ) * 20 );
		sBRX = sTLX + 20;
		sBRY = sTLY + 20;

		us16BPPColor = Get16BPPColor( FROMRGB( gpPalRep[ ubPaletteRep ].r[ cnt1 ], gpPalRep[ ubPaletteRep ].g[ cnt1 ], gpPalRep[ ubPaletteRep ].b[ cnt1 ] ) );

		ColorFillVideoSurfaceArea( uiDestSurface, sTLX, sTLY, sBRX, sBRY, us16BPPColor );

	}

	gprintf( ubXPos + ( 16 * 20 ), ubYPos, L"%S", gpPalRep[ ubPaletteRep ].ID );

	return( TRUE );
}


BOOLEAN	WrapString( STR16 pStr, STR16 pStr2, UINT16 usWidth, INT32 uiFont )
{
	UINT32 Cur, uiLet, uiNewLet, uiHyphenLet;
	STR16 curletter;
	INT16 transletter;
	BOOLEAN	fLineSplit = FALSE;
	HVOBJECT	hFont;

	// CHECK FOR WRAP
	Cur=0;
	uiLet = 0;
	curletter = pStr;

	// GET FONT
	hFont = GetFontObject( uiFont );

	// LOOP FORWARDS AND COUNT
	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter);
		Cur+=GetWidth( hFont, transletter );

		if ( Cur > usWidth )
		{
			// We are here, loop backwards to find a space
			// Generate second string, and exit upon completion.
			uiHyphenLet = uiLet;	//Save the hyphen location as it won't change.
			uiNewLet = uiLet;
			while((*curletter)!=0)
			{
				if ( (*curletter) == 32 )
				{
					// Split Line!
					fLineSplit = TRUE;

					pStr[ uiNewLet ] = (INT16)'\0';

					wcscpy( pStr2, &(pStr[ uiNewLet + 1 ]) );
				}

				if ( fLineSplit )
					// HEADROCK HAM 3.6: This is erroneous. At this point, the function should RETURN, otherwise it
					// cuts another bit off pStr for no reason.
					return (fLineSplit);

				uiNewLet--;
				curletter--;

			}
			if( !fLineSplit)
			{
				//We completed the check for a space, but failed, so use the hyphen method.
				swprintf( pStr2, L"-%s", &(pStr[uiHyphenLet]) );
				pStr[uiHyphenLet] = (INT16)'\0';
				fLineSplit = TRUE;	//hyphen method
				// HEADROCK HAM 3.6: This is erroneous. At this point, the function should RETURN, otherwise it
				// cuts another bit off pStr for no reason.
				return (fLineSplit);
			}
		}

//		if ( fLineSplit )
//			break;

		uiLet++;
		curletter++;
	}

	return( fLineSplit );

}


BOOLEAN IfWinNT(void)
{
	OSVERSIONINFO OsVerInfo;

	OsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&OsVerInfo);

	if ( OsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return(TRUE);
	else
		return(FALSE);
}

BOOLEAN IfWin95(void)
{
	OSVERSIONINFO OsVerInfo;

	OsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&OsVerInfo);

	if ( OsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		return(TRUE);
	else
		return(FALSE);
}


void HandleLimitedNumExecutions( )
{
	// Get system directory
	HWFILE	 hFileHandle;
	CHAR8	ubSysDir[ 512 ];
	INT8	bNumRuns;

	GetSystemDirectory( (LPSTR) ubSysDir, sizeof( ubSysDir ) );

	// Append filename
	strcat( ubSysDir, "\\winaese.dll" );

	// Open file and check # runs...
	if ( FileExists( (STR)ubSysDir ) )
	{
		// Open and read
		if ( ( hFileHandle = FileOpen( (STR)ubSysDir, FILE_ACCESS_READ, FALSE)) == 0)
		{
			return;
		}

		// Read value
		FileRead( hFileHandle, &bNumRuns, sizeof( bNumRuns ) , NULL);

		// Close file
		FileClose( hFileHandle );

		if ( bNumRuns <= 0 )
		{
			// Fail!
			SET_ERROR( "Error 1054: Cannot execute - contact Sir-Tech Software." );
			return;
		}

	}
	else
	{
		bNumRuns = 10;
	}

	// OK, decrement # runs...
	bNumRuns--;

	// Open and write
	if ( ( hFileHandle = FileOpen( (STR)ubSysDir, FILE_ACCESS_WRITE, FALSE)) == 0)
	{
		return;
	}

	// Write value
	FileWrite( hFileHandle, &bNumRuns, sizeof( bNumRuns ) , NULL);

	// Close file
	FileClose( hFileHandle );

}


SGPFILENAME	gCheckFilenames[] =
{
	"DATA\\INTRO.SLF",
	"DATA\\LOADSCREENS.SLF",
	"DATA\\MAPS.SLF",
	"DATA\\NPC_SPEECH.SLF",
	"DATA\\SPEECH.SLF",
};


UINT32 gCheckFileMinSizes[] =
{
	68000000,
	36000000,
	87000000,
	187000000,
	236000000
};


BOOLEAN DoJA2FilesExistsOnDrive( CHAR8 *zCdLocation )
{
	BOOLEAN fFailed = FALSE;
	CHAR8		zCdFile[ SGPFILENAME_LEN ];
	INT32	cnt;
	HWFILE	hFile;

	for ( cnt = 0; cnt < 4; cnt++ )
	{
		// OK, build filename
		sprintf( zCdFile, "%s%s", zCdLocation, gCheckFilenames[ cnt ] );

		hFile = FileOpen( zCdFile, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );

		// Check if it exists...
		if ( !hFile )
		{
			fFailed = TRUE;
			FileClose( hFile );
		break;
		}
		FileClose( hFile );
	}

	return( !fFailed );
}
