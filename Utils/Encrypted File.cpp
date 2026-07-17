	#include "Encrypted File.h"
	#include "FileMan.h"
	#include "DEBUG.H"

// anv: for selecting random line
#include "random.h"

// anv: loading random line from the file
BOOLEAN LoadEncryptedDataFromFileRandomLine(STR pFileName, STR16 pDestString, UINT32 uiSeekAmount)
{
	HWFILE		hFile;	
	UINT32		uiBytesRead;
	UINT32		uiSeekFrom;
 
	hFile = FileOpen(pFileName, FILE_ACCESS_READ, FALSE);
	if ( !hFile )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed to FileOpen");
		return( FALSE );
	}

	uiSeekFrom = Random( ( FileGetSize(hFile) / uiSeekAmount ) - 1 );

	if ( FileSeek( hFile, uiSeekFrom * uiSeekAmount, FILE_SEEK_FROM_START ) == FALSE )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileSeek");
		return( FALSE );
	}

	if( !FileRead( hFile, pDestString, uiSeekAmount, &uiBytesRead) )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileRead");
		return( FALSE );
	}

	DecodeString(pDestString, uiSeekAmount);

	FileClose(hFile);
	return(TRUE);
}

BOOLEAN LoadEncryptedDataFromFile(STR pFileName, STR16 pDestString, UINT32 uiSeekFrom, UINT32 uiSeekAmount)
{
	HWFILE		hFile;	
	UINT32		uiBytesRead;


	hFile = FileOpen(pFileName, FILE_ACCESS_READ, FALSE);
	if ( !hFile )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed to FileOpen");
		return( FALSE );
	}

	if ( FileSeek( hFile, uiSeekFrom, FILE_SEEK_FROM_START ) == FALSE )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileSeek");
		return( FALSE );
	}

	if( !FileRead( hFile, pDestString, uiSeekAmount, &uiBytesRead) )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileRead");
		return( FALSE );
	}



	//// Decrement, by 1, any value > 32
	//for(i=0; (i<uiSeekAmount) && (pDestString[i] != 0); i++ )
	//{
	//	if( pDestString[i] > 33 )
	//		pDestString[i] -= 1;		
	//	

	//}

	DecodeString(pDestString, uiSeekAmount);

	FileClose(hFile);
	return(TRUE);
}


void DecodeString(STR16 pDestString, UINT32 uiSeekAmount)
{
	UINT32		i;

	// Decrement, by 1, any value > 32
	for(i=0; (i<uiSeekAmount) && (pDestString[i] != 0); i++ )
	{
		if( pDestString[i] > 33 )
			pDestString[i] -= 1;	

	}
}
