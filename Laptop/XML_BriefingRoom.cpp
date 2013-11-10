#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "Editor All.h"
	#include "LuaInitNPCs.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "LuaInitNPCs.h"
	#include "email.h"
	#include "InterfaceItemImages.h"
	#include "Soldier Profile.h"
	#include "aim.h"
	#include "mercs.h"
	#include "Encrypted File.h"
	#include "GameSettings.h"
#endif

#include "BriefingRoom_Data.h"

typedef enum
{
	ENCYCLOPEDIA_ELEMENT_NONE = 0,
	ENCYCLOPEDIA_ELEMENT_LIST,
	ENCYCLOPEDIA_ELEMENT,
	ENCYCLOPEDIA_ELEMENT_PROPERTY,
	ENCYCLOPEDIA_ELEMENT_SUBLIST,
	ENCYCLOPEDIA_ELEMENT_SUBLIST_PROPERTY,
	
	
} ENCYCLOPEDIA_PARSE_STAGE;

typedef struct
{
	ENCYCLOPEDIA_PARSE_STAGE	curElement;

	CHAR8		szCharData[ENCYCLOPEDIA_DECRIPTION_SIZE+1];
	BRIEFINGROOM_M_DATA	curEncyclopediaData;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}encyclopediaLocationParseData;


BOOLEAN EncyclopediaLocation_TextOnly;

BRIEFINGROOM_M_DATA *pEncy;

//extern BOOLEAN LoadMercBioInfo(UINT8 ubIndex, STR16 pInfoString, STR16 pAddInfo);

UINT32 FileType = 0;
UINT32 MaxPage = 0;
UINT8 mID = 0;
UINT8			gMercArray[ NUM_PROFILES ];

#define	SIZE_MERC_BIO_INFO	400	* 2
#define SIZE_MERC_ADDITIONAL_INFO 160 * 2
#define	MERCBIOSFILENAME	"BINARYDATA\\MercBios.edt"
#define	AIMBIOSFILENAME		"BINARYDATA\\aimbios.edt"


CHAR16		MercInfoString[ SIZE_MERC_BIO_INFO ];
CHAR16		AdditionalInfoString[ SIZE_MERC_BIO_INFO ];

BOOLEAN bMERC;

BOOLEAN LoadEncyclopediaMercBio( UINT8 ubIndex, STR16 pInfoString, STR16 pAddInfo,UINT32 Type )
{
	HWFILE		hFile=NULL;
	UINT32		uiBytesRead;
	//UINT16		i;
	UINT32		uiStartSeekAmount;

	if ( Type == 1 )
	hFile = FileOpen(AIMBIOSFILENAME, FILE_ACCESS_READ, FALSE);
	else if ( Type == 2 )
	hFile = FileOpen(MERCBIOSFILENAME, FILE_ACCESS_READ, FALSE);
	
	if ( !hFile )
	{
		return( FALSE );
	}


	// Get current mercs bio info
	uiStartSeekAmount = (SIZE_MERC_BIO_INFO + SIZE_MERC_ADDITIONAL_INFO) * ubIndex;

	if ( FileSeek( hFile, uiStartSeekAmount, FILE_SEEK_FROM_START ) == FALSE )
	{
		return( FALSE );
	}

	if( !FileRead( hFile, pInfoString, SIZE_MERC_BIO_INFO, &uiBytesRead) )
	{
		return( FALSE );
	}
	
	DecodeString(pInfoString, SIZE_MERC_BIO_INFO);

	// Get the additional info
	uiStartSeekAmount = ((SIZE_MERC_BIO_INFO + SIZE_MERC_ADDITIONAL_INFO) * ubIndex )+ SIZE_MERC_BIO_INFO ;
	if ( FileSeek( hFile, uiStartSeekAmount, FILE_SEEK_FROM_START ) == FALSE )
	{
		return( FALSE );
	}

	if( !FileRead( hFile, pAddInfo, SIZE_MERC_ADDITIONAL_INFO, &uiBytesRead) )
	{
		return( FALSE );
	}

	DecodeString(pAddInfo, SIZE_MERC_BIO_INFO);

	FileClose(hFile);
	return(TRUE);
}

BOOLEAN LoadGraphicForItem( BRIEFINGROOM_M_DATA *pEncy, UINT32 i )
{
	CHAR8	 zName[ 100 ];
//	UINT32	uiVo;
	UINT16		ubGraphic, ubGraphicType;
	CHAR8	zString[512]; 
//	CHAR8	zString2[512];

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LoadTileGraphicForItem"));

	const char* ext = g_bUsePngItemImages ? ".png" : ".sti";

	// CHECK SUBCLASS
	ubGraphic = Item[i].ubGraphicNum;
	ubGraphicType = Item[i].ubGraphicType;

	if ( Item[i].ubGraphicType == 0 )
	{
		// CHECK SUBCLASS
		//ubGraphic++;

		if ( ubGraphic < 10 )
		{
			sprintf( zName, "gun0%d", ubGraphic );
		}
		else
		{
			sprintf( zName, "gun%d", ubGraphic );
		}
	}
	else
	{
		if ( ubGraphic < 10 )
		{
			sprintf( zName, "p%ditem0%d", ubGraphicType, ubGraphic );
		}
		else
		{
			sprintf( zName, "p%ditem%d", ubGraphicType, ubGraphic );
		}
	}

	//Load item
	sprintf( zString, "BIGITEMS\\%s%s", zName, ext );
	
	


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LoadTileGraphicForItem: done"));

	return( TRUE );
}

static void XMLCALL
encyclopediaLocationStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	encyclopediaLocationParseData * pData = (encyclopediaLocationParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "BRIEFINGROOM") == 0 && pData->curElement == ENCYCLOPEDIA_ELEMENT_NONE)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DATA") == 0 && pData->curElement == ENCYCLOPEDIA_ELEMENT_LIST)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ENCYCLOPEDIA_ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "Name") == 0 ||
				strcmp(name, "Hidden") == 0 ||
				strcmp(name, "MaxPages") == 0 ||
				strcmp(name, "MaxImages") == 0 ||
				strcmp(name, "ImagePositionX") == 0 ||
				strcmp(name, "ImagePositionY") == 0 ||
				strcmp(name, "NextMission") == 0 ))
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
encyclopediaLocationCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	encyclopediaLocationParseData * pData = (encyclopediaLocationParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
encyclopediaLocationEndElementHandle(void *userData, const XML_Char *name)
{
	encyclopediaLocationParseData * pData = (encyclopediaLocationParseData *)userData;
	
//	char temp;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "BRIEFINGROOM") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT_NONE;
		}
		else if(strcmp(name, "DATA") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT_LIST;	
			
			if (!EncyclopediaLocation_TextOnly)
				{
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, pData->curEncyclopediaData.Name);	
					
					pEncy[pData->curEncyclopediaData.uiIndex].uiIndex = pData->curEncyclopediaData.uiIndex;
					pEncy[pData->curEncyclopediaData.uiIndex].MaxPages = pData->curEncyclopediaData.MaxPages;
					pEncy[pData->curEncyclopediaData.uiIndex].MaxImages = pData->curEncyclopediaData.MaxImages;
					
					//if ( pData->curEncyclopediaData.MaxImages > MAX_IMAGES ) 
						//pEncy[pData->curEncyclopediaData.uiIndex].MaxImages = 0;

					//if ( pData->curEncyclopediaData.MaxPages > MAX_PAGES ) 
						//pEncy[pData->curEncyclopediaData.uiIndex].MaxPages = 0;						
					
					if (pData->curEncyclopediaData.Hidden == FALSE )
					   pEncy[pData->curEncyclopediaData.uiIndex].Hidden = TRUE;
					else
					   pEncy[pData->curEncyclopediaData.uiIndex].Hidden = FALSE;
					   
				
					if ( FileType == 4 ) //briefing room
						pEncy[pData->curEncyclopediaData.uiIndex].NextMission = pData->curEncyclopediaData.NextMission;
					else
						pEncy[pData->curEncyclopediaData.uiIndex].NextMission = -1;
					
					pEncy[pData->curEncyclopediaData.uiIndex].MissionID = pData->curEncyclopediaData.uiIndex;
					
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionX[0] = pData->curEncyclopediaData.sImagePositionX[0];
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionY[0] = pData->curEncyclopediaData.sImagePositionY[0];
					
				}
				else
				{
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, pData->curEncyclopediaData.Name);	
				}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.uiIndex	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "Name") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.Name, sizeof(pData->curEncyclopediaData.Name)/sizeof(pData->curEncyclopediaData.Name[0]) );
			pData->curEncyclopediaData.Name[sizeof(pData->curEncyclopediaData.Name)/sizeof(pData->curEncyclopediaData.Name[0]) - 1] = '\0';
		}
		else if(strcmp(name, "Hidden") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.Hidden	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "MaxPages") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.MaxPages	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "MaxImages") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.MaxImages	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePositionX") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionX[0]	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "ImagePositionY") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionY[0]	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "SecretCode") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sCode, sizeof(pData->curEncyclopediaData.sCode)/sizeof(pData->curEncyclopediaData.sCode[0]) );
			pData->curEncyclopediaData.sCode[sizeof(pData->curEncyclopediaData.sCode)/sizeof(pData->curEncyclopediaData.sCode[0]) - 1] = '\0';
		}
		else if(strcmp(name, "NextMission") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.NextMission	= (INT32) atol(pData->szCharData);
		}		
		
		
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInBriefingRoom(STR fileName, BOOLEAN localizedVersion, BRIEFINGROOM_M_DATA *Ency, UINT32 FileType2 )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	encyclopediaLocationParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading BriefingRoom.xml" );

	EncyclopediaLocation_TextOnly = localizedVersion;
	
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( localizedVersion );

	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree(lpcBuffer);
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );


	XML_SetElementHandler(parser, encyclopediaLocationStartElementHandle, encyclopediaLocationEndElementHandle);
	XML_SetCharacterDataHandler(parser, encyclopediaLocationCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);
	
	pEncy = Ency;
	FileType = FileType2;


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in BriefingRoom.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
