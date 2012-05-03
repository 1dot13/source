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
	#include "Item Statistics.h"
	#include "LuaInitNPCs.h"
	#include "email.h"
	#include "InterfaceItemImages.h"
	#include "Soldier Profile.h"
	#include "aim.h"
	#include "mercs.h"
	#include "Encrypted File.h"
	#include "GameSettings.h"
#endif

#include "Encyclopedia_Data.h"
#include "Encyclopedia.h"

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
	ENCYCLOPEDIA_LOCATION	curEncyclopediaData;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}encyclopediaLocationParseData;


BOOLEAN EncyclopediaLocation_TextOnly;

ENCYCLOPEDIA_LOCATION *pEncy;

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
	HWFILE		hFile;
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

BOOLEAN LoadGraphicForItem( ENCYCLOPEDIA_LOCATION *pEncy, UINT32 i )
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
	
	strncpy(pEncy[i].szFile, zString,MAX_ENCYCLOPEDIA_CHARS);


	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("LoadTileGraphicForItem: done"));

	return( TRUE );
}

static void XMLCALL
encyclopediaLocationStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	encyclopediaLocationParseData * pData = (encyclopediaLocationParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ENCYCLOPEDIA") == 0 && pData->curElement == ENCYCLOPEDIA_ELEMENT_NONE)
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
				strcmp(name, "EnableDesc") == 0 ||
				strcmp(name, "Hidden") == 0 ||
				strcmp(name, "Type") == 0 ||
				strcmp(name, "subGroup") == 0 ||
				strcmp(name, "SectorX") == 0 ||
				strcmp(name, "SectorY") == 0 ||
				strcmp(name, "Level") == 0 ||
				strcmp(name, "MaxPages") == 0 ||
				strcmp(name, "PageDesc1") == 0 ||
				strcmp(name, "PageDesc2") == 0 ||
				strcmp(name, "PageDesc3") == 0 ||
				strcmp(name, "PageDesc4") == 0 ||
				strcmp(name, "PageDesc5") == 0 ||
				strcmp(name, "PageDesc6") == 0 ||
				strcmp(name, "PageDesc7") == 0 ||
				strcmp(name, "PageDesc8") == 0 ||
				strcmp(name, "PageDesc9") == 0 ||
				strcmp(name, "PageDesc10") == 0 ||
				strcmp(name, "PageDesc11") == 0 ||
				strcmp(name, "PageDesc12") == 0 ||
				strcmp(name, "PageDesc13") == 0 ||
				strcmp(name, "PageDesc14") == 0 ||
				strcmp(name, "PageDesc15") == 0 ||
				strcmp(name, "PageDesc16") == 0 ||
				strcmp(name, "PageDesc17") == 0 ||
				strcmp(name, "PageDesc18") == 0 ||
				strcmp(name, "PageDesc19") == 0 ||
				strcmp(name, "PageDesc20") == 0 ||
				strcmp(name, "MaxImages") == 0 ||
				strcmp(name, "szFile1") == 0 ||
				strcmp(name, "szFile2") == 0 ||
				strcmp(name, "szFile3") == 0 ||
				strcmp(name, "szFile4") == 0 ||
				strcmp(name, "ImageDesc1") == 0 ||
				strcmp(name, "ImageDesc2") == 0 ||
				strcmp(name, "ImageDesc3") == 0 ||
				strcmp(name, "ImageDesc4") == 0 ||
				strcmp(name, "ImagePosition1X") == 0 ||
				strcmp(name, "ImagePosition1Y") == 0 ||
				strcmp(name, "ImagePosition2X") == 0 ||
				strcmp(name, "ImagePosition2Y") == 0 ||				
				strcmp(name, "ImagePosition3X") == 0 ||
				strcmp(name, "ImagePosition3Y") == 0 ||
				strcmp(name, "ImagePosition4X") == 0 ||
				strcmp(name, "ImagePosition4Y") == 0 ||
				strcmp(name, "SpecialQuestImage") == 0 ||
				strcmp(name, "BeforeImage") == 0 ||
				strcmp(name, "AfterImage") == 0 ||
				strcmp(name, "ImagePositionBeforeX") == 0 ||
				strcmp(name, "ImagePositionBeforeY") == 0 ||
				strcmp(name, "ImagePositionAfterX") == 0 ||
				strcmp(name, "ImagePositionAfterY") == 0 ||
				strcmp(name, "ImageDescBefore") == 0 ||
				strcmp(name, "ImageDescAfter") == 0 ||
				strcmp(name, "szSounds") == 0 ||
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
	
	char temp;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "ENCYCLOPEDIA") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT_NONE;
		}
		else if(strcmp(name, "DATA") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT_LIST;	
			
			if (!EncyclopediaLocation_TextOnly)
				{
				
					//pEncy[pData->curEncyclopediaData.uiIndex].enableDesc = FALSE;
					
			if ( pData->curEncyclopediaData.enableDesc == TRUE ) pEncy[pData->curEncyclopediaData.uiIndex].enableDesc = FALSE;
			else if ( pData->curEncyclopediaData.enableDesc == FALSE ) pEncy[pData->curEncyclopediaData.uiIndex].enableDesc = TRUE;
	
					pEncy[pData->curEncyclopediaData.uiIndex].uiIndex = pData->curEncyclopediaData.uiIndex;
					
					if ( pEncy[pData->curEncyclopediaData.uiIndex].SpecialQuestImage == TRUE ) FileType = 3;
					
					if ( pEncy[pData->curEncyclopediaData.uiIndex].SpecialQuestImage == TRUE )
					{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, pData->curEncyclopediaData.sDesc1);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc2, pData->curEncyclopediaData.sDesc2);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc3, pData->curEncyclopediaData.sDesc3);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc4, pData->curEncyclopediaData.sDesc4);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc5, pData->curEncyclopediaData.sDesc5);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc6, pData->curEncyclopediaData.sDesc6);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc7, pData->curEncyclopediaData.sDesc7);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc8, pData->curEncyclopediaData.sDesc8);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc9, pData->curEncyclopediaData.sDesc9);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc10, pData->curEncyclopediaData.sDesc10);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc11, pData->curEncyclopediaData.sDesc11);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc12, pData->curEncyclopediaData.sDesc12);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc13, pData->curEncyclopediaData.sDesc13);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc14, pData->curEncyclopediaData.sDesc14);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc15, pData->curEncyclopediaData.sDesc15);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc16, pData->curEncyclopediaData.sDesc16);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc17, pData->curEncyclopediaData.sDesc17);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc18, pData->curEncyclopediaData.sDesc18);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc19, pData->curEncyclopediaData.sDesc19);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc20, pData->curEncyclopediaData.sDesc20);
						
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, pData->curEncyclopediaData.Name);	
					}
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == TRUE && pEncy[pData->curEncyclopediaData.uiIndex].SpecialQuestImage == FALSE )
					{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, pData->curEncyclopediaData.sDesc1);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc2, pData->curEncyclopediaData.sDesc2);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc3, pData->curEncyclopediaData.sDesc3);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc4, pData->curEncyclopediaData.sDesc4);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc5, pData->curEncyclopediaData.sDesc5);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc6, pData->curEncyclopediaData.sDesc6);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc7, pData->curEncyclopediaData.sDesc7);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc8, pData->curEncyclopediaData.sDesc8);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc9, pData->curEncyclopediaData.sDesc9);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc10, pData->curEncyclopediaData.sDesc10);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc11, pData->curEncyclopediaData.sDesc11);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc12, pData->curEncyclopediaData.sDesc12);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc13, pData->curEncyclopediaData.sDesc13);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc14, pData->curEncyclopediaData.sDesc14);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc15, pData->curEncyclopediaData.sDesc15);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc16, pData->curEncyclopediaData.sDesc16);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc17, pData->curEncyclopediaData.sDesc17);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc18, pData->curEncyclopediaData.sDesc18);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc19, pData->curEncyclopediaData.sDesc19);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc20, pData->curEncyclopediaData.sDesc20);
						
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, pData->curEncyclopediaData.Name);	
					}
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE && FileType == 1 )
					{
							for(int cnt = 0;cnt < NUM_PROFILES;cnt++)
							{
								if ( gConditionsForMercAvailabilityTemp[cnt].ProfilId != 0 ) 
								{
									//mID = gConditionsForMercAvailabilityTemp[cnt].MercBio;
									gMercArray[ cnt ] = gConditionsForMercAvailability[cnt].ProfilId;
								}
							}	

							for(int cnt = 0;cnt < NUM_PROFILES;cnt++)
							{
								if ( gMercArray[ cnt ] == pData->curEncyclopediaData.uiIndex ) 
								{
									mID = gConditionsForMercAvailabilityTemp[cnt].MercBio;
								}
							}		
						
						if (tempProfiles[pData->curEncyclopediaData.uiIndex].Type == 1 )//if ( gProfilesAIM[ pData->curEncyclopediaData.uiIndex ].ProfilId == pData->curEncyclopediaData.uiIndex )
						{
							LoadEncyclopediaMercBio( gAimAvailabilityTemp[pData->curEncyclopediaData.uiIndex].AimBio, MercInfoString, AdditionalInfoString,1);
						}
						else  if  (tempProfiles[pData->curEncyclopediaData.uiIndex].Type == 2 )//if ( gProfilesAIM[ pData->curEncyclopediaData.uiIndex ].ProfilId == pData->curEncyclopediaData.uiIndex )
						{
							LoadEncyclopediaMercBio( mID, MercInfoString, AdditionalInfoString,2);
						}
						else
						{
							MercInfoString[0] = 0;
							AdditionalInfoString[0] = 0;
							pEncy[pData->curEncyclopediaData.uiIndex].enableDesc = TRUE;
						}
						
						if( MercInfoString[0] != 0)
						{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, MercInfoString);
						MaxPage = 0;
						}
						if( AdditionalInfoString[0] != 0)
						{
						MaxPage = 2;
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc2, AdditionalInfoString);
						}	
					}
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE && FileType == 2 )
					{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, Item[pData->curEncyclopediaData.uiIndex].szItemDesc);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, Item[pData->curEncyclopediaData.uiIndex].szLongItemName);					
					}
					
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc1, pData->curEncyclopediaData.sImageDesc1);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc2, pData->curEncyclopediaData.sImageDesc2);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc3, pData->curEncyclopediaData.sImageDesc3);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc4, pData->curEncyclopediaData.sImageDesc4);
					
					//Specjal Quest Image
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].ImageDescBefore, pData->curEncyclopediaData.ImageDescBefore);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].ImageDescAfter, pData->curEncyclopediaData.ImageDescAfter);
					
					pEncy[pData->curEncyclopediaData.uiIndex].QuestID = -1;
					pEncy[pData->curEncyclopediaData.uiIndex].pQuests = -1;
					pEncy[pData->curEncyclopediaData.uiIndex].SpecialQuestImage = pData->curEncyclopediaData.SpecialQuestImage;

					pEncy[pData->curEncyclopediaData.uiIndex].ImagePositionQX[0] = pData->curEncyclopediaData.ImagePositionQX[0];
					pEncy[pData->curEncyclopediaData.uiIndex].ImagePositionQY[0] = pData->curEncyclopediaData.ImagePositionQY[0];
					
					pEncy[pData->curEncyclopediaData.uiIndex].ImagePositionQX[1] = pData->curEncyclopediaData.ImagePositionQX[1];
					pEncy[pData->curEncyclopediaData.uiIndex].ImagePositionQY[1] = pData->curEncyclopediaData.ImagePositionQY[1];
					
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].BeforeImage, pData->curEncyclopediaData.BeforeImage,MAX_ENCYCLOPEDIA_CHARS);
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].AfterImage, pData->curEncyclopediaData.AfterImage,MAX_ENCYCLOPEDIA_CHARS);

					
					pEncy[pData->curEncyclopediaData.uiIndex].SectorX = pData->curEncyclopediaData.SectorX;
					pEncy[pData->curEncyclopediaData.uiIndex].SectorY = pData->curEncyclopediaData.SectorY;
					pEncy[pData->curEncyclopediaData.uiIndex].SectorZ = pData->curEncyclopediaData.SectorZ;
					
					if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == TRUE )
					pEncy[pData->curEncyclopediaData.uiIndex].MaxPages = pData->curEncyclopediaData.MaxPages;
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE  && FileType == 1 )
					pEncy[pData->curEncyclopediaData.uiIndex].MaxPages = MaxPage;
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE  && FileType == 2 )
					pEncy[pData->curEncyclopediaData.uiIndex].MaxPages = 0;
					
					pEncy[pData->curEncyclopediaData.uiIndex].MaxImages = pData->curEncyclopediaData.MaxImages;
					
					if ( pData->curEncyclopediaData.MaxImages > MAX_IMAGES ) 
						pEncy[pData->curEncyclopediaData.uiIndex].MaxImages = 0;

					if ( pData->curEncyclopediaData.MaxPages > MAX_PAGES ) 
						pEncy[pData->curEncyclopediaData.uiIndex].MaxPages = 0;						
					
					if (pData->curEncyclopediaData.Hidden == FALSE )
					   pEncy[pData->curEncyclopediaData.uiIndex].Hidden = TRUE;
					else
					   pEncy[pData->curEncyclopediaData.uiIndex].Hidden = FALSE;
					   
					if (pEncy[pData->curEncyclopediaData.uiIndex].Hidden == TRUE ) 
						pEncy[pData->curEncyclopediaData.uiIndex].Filtr = TRUE;
					else
					   pEncy[pData->curEncyclopediaData.uiIndex].Filtr = FALSE; 
					   	   
				if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE && FileType == 2 ) //0 - location ,  2 - guns, 1 - profiles
					LoadGraphicForItem( pEncy, pData->curEncyclopediaData.uiIndex);
				else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE && FileType == 1 ) //0 - location ,  2 - guns, 1 - profiles					
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].szFile, pData->curEncyclopediaData.szFile,MAX_ENCYCLOPEDIA_CHARS);
				else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == TRUE )
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].szFile, pData->curEncyclopediaData.szFile,MAX_ENCYCLOPEDIA_CHARS);
					
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].szFile2, pData->curEncyclopediaData.szFile2,MAX_ENCYCLOPEDIA_CHARS);
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].szFile3, pData->curEncyclopediaData.szFile3,MAX_ENCYCLOPEDIA_CHARS);
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].szFile4, pData->curEncyclopediaData.szFile4,MAX_ENCYCLOPEDIA_CHARS);
				
					strncpy(pEncy[pData->curEncyclopediaData.uiIndex].sSounds, pData->curEncyclopediaData.sSounds,MAX_ENCYCLOPEDIA_CHARS);
				
					if ( FileType == 4 ) //briefing room
					pEncy[pData->curEncyclopediaData.uiIndex].NextMission = pData->curEncyclopediaData.NextMission;
					else
					pEncy[pData->curEncyclopediaData.uiIndex].NextMission = -1;
					
					pEncy[pData->curEncyclopediaData.uiIndex].MissionID = pData->curEncyclopediaData.uiIndex;
					
					pEncy[pData->curEncyclopediaData.uiIndex].IDProfile = -1;
					
					if ( pData->curEncyclopediaData.subGroup1 == 0 ) 
					pEncy[pData->curEncyclopediaData.uiIndex].subGroup1 = -1;
					else
					pEncy[pData->curEncyclopediaData.uiIndex].subGroup1 = pData->curEncyclopediaData.subGroup1;
					
					pEncy[pData->curEncyclopediaData.uiIndex].LocType = pData->curEncyclopediaData.LocType;
					
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionX[0] = pData->curEncyclopediaData.sImagePositionX[0];
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionY[0] = pData->curEncyclopediaData.sImagePositionY[0];
					
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionX[1] = pData->curEncyclopediaData.sImagePositionX[1];
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionY[1] = pData->curEncyclopediaData.sImagePositionY[1];		
					
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionX[2] = pData->curEncyclopediaData.sImagePositionX[2];
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionY[2] = pData->curEncyclopediaData.sImagePositionY[2];
					
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionX[3] = pData->curEncyclopediaData.sImagePositionX[3];
					pEncy[pData->curEncyclopediaData.uiIndex].sImagePositionY[3] = pData->curEncyclopediaData.sImagePositionY[3];
					
				}
				else
				{
				
					if ( pEncy[pData->curEncyclopediaData.uiIndex].SpecialQuestImage == TRUE )
					{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, pData->curEncyclopediaData.sDesc1);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc2, pData->curEncyclopediaData.sDesc2);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc3, pData->curEncyclopediaData.sDesc3);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc4, pData->curEncyclopediaData.sDesc4);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc5, pData->curEncyclopediaData.sDesc5);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc6, pData->curEncyclopediaData.sDesc6);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc7, pData->curEncyclopediaData.sDesc7);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc8, pData->curEncyclopediaData.sDesc8);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc9, pData->curEncyclopediaData.sDesc9);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc10, pData->curEncyclopediaData.sDesc10);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc11, pData->curEncyclopediaData.sDesc11);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc12, pData->curEncyclopediaData.sDesc12);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc13, pData->curEncyclopediaData.sDesc13);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc14, pData->curEncyclopediaData.sDesc14);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc15, pData->curEncyclopediaData.sDesc15);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc16, pData->curEncyclopediaData.sDesc16);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc17, pData->curEncyclopediaData.sDesc17);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc18, pData->curEncyclopediaData.sDesc18);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc19, pData->curEncyclopediaData.sDesc19);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc20, pData->curEncyclopediaData.sDesc20);
						
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, pData->curEncyclopediaData.Name);	
					}
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == TRUE && pEncy[pData->curEncyclopediaData.uiIndex].SpecialQuestImage == FALSE )
					{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, pData->curEncyclopediaData.sDesc1);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc2, pData->curEncyclopediaData.sDesc2);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc3, pData->curEncyclopediaData.sDesc3);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc4, pData->curEncyclopediaData.sDesc4);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc5, pData->curEncyclopediaData.sDesc5);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc6, pData->curEncyclopediaData.sDesc6);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc7, pData->curEncyclopediaData.sDesc7);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc8, pData->curEncyclopediaData.sDesc8);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc9, pData->curEncyclopediaData.sDesc9);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc10, pData->curEncyclopediaData.sDesc10);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc11, pData->curEncyclopediaData.sDesc11);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc12, pData->curEncyclopediaData.sDesc12);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc13, pData->curEncyclopediaData.sDesc13);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc14, pData->curEncyclopediaData.sDesc14);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc15, pData->curEncyclopediaData.sDesc15);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc16, pData->curEncyclopediaData.sDesc16);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc17, pData->curEncyclopediaData.sDesc17);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc18, pData->curEncyclopediaData.sDesc18);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc19, pData->curEncyclopediaData.sDesc19);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc20, pData->curEncyclopediaData.sDesc20);
						
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, pData->curEncyclopediaData.Name);
					}	
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE && FileType == 1 )
					{
					
							for(int cnt = 0;cnt < NUM_PROFILES;cnt++)
							{
								if ( gConditionsForMercAvailabilityTemp[cnt].ProfilId != 0 ) 
								{
									//mID = gConditionsForMercAvailabilityTemp[cnt].MercBio;
									gMercArray[ cnt ] = gConditionsForMercAvailability[cnt].ProfilId;
								}
							}	

							for(int cnt = 0;cnt < NUM_PROFILES;cnt++)
							{
								if ( gMercArray[ cnt ] == pData->curEncyclopediaData.uiIndex ) 
								{
									mID = gConditionsForMercAvailabilityTemp[cnt].MercBio;
								}
							}		
						
						if (tempProfiles[pData->curEncyclopediaData.uiIndex].Type == 1 )//if ( gProfilesAIM[ pData->curEncyclopediaData.uiIndex ].ProfilId == pData->curEncyclopediaData.uiIndex )
						{
							LoadEncyclopediaMercBio( gAimAvailabilityTemp[pData->curEncyclopediaData.uiIndex].AimBio, MercInfoString, AdditionalInfoString,1);
						}
						else  if  (tempProfiles[pData->curEncyclopediaData.uiIndex].Type == 2 )//if ( gProfilesAIM[ pData->curEncyclopediaData.uiIndex ].ProfilId == pData->curEncyclopediaData.uiIndex )
						{
							LoadEncyclopediaMercBio( mID, MercInfoString, AdditionalInfoString,2);
						}
						else 
						{
							MercInfoString[0] = 0;
							AdditionalInfoString[0] = 0;
							pEncy[pData->curEncyclopediaData.uiIndex].enableDesc = TRUE;
						}
						
						if( MercInfoString[0] != 0)
						{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, MercInfoString);
						MaxPage = 0;
						}
						if( AdditionalInfoString[0] != 0)
						{
						MaxPage = 1;
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc2, AdditionalInfoString);
						}	
					}					
					else if ( pEncy[pData->curEncyclopediaData.uiIndex].enableDesc == FALSE  && FileType == 2 )
					{
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sDesc1, Item[pData->curEncyclopediaData.uiIndex].szItemDesc);
						wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].Name, Item[pData->curEncyclopediaData.uiIndex].szLongItemName);
					}

					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc1, pData->curEncyclopediaData.sImageDesc1);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc2, pData->curEncyclopediaData.sImageDesc2);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc3, pData->curEncyclopediaData.sImageDesc3);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].sImageDesc4, pData->curEncyclopediaData.sImageDesc4);
					
					//Specjal Quest Image
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].ImageDescBefore, pData->curEncyclopediaData.ImageDescBefore);
					wcscpy(pEncy[pData->curEncyclopediaData.uiIndex].ImageDescAfter, pData->curEncyclopediaData.ImageDescAfter);
					
				}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.uiIndex	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "Name") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.Name, sizeof(pData->curEncyclopediaData.Name)/sizeof(pData->curEncyclopediaData.Name[0]) );
			pData->curEncyclopediaData.Name[sizeof(pData->curEncyclopediaData.Name)/sizeof(pData->curEncyclopediaData.Name[0]) - 1] = '\0';
		}
		else if(strcmp(name, "EnableDesc") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.enableDesc	= (BOOLEAN) atol(pData->szCharData);	
		}		
		else if(strcmp(name, "Hidden") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.Hidden	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Type") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.LocType	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "subGroup") == 0 ) //&& FileType == 1 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.subGroup1	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "SectorX") == 0 ) //&& FileType == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.SectorX	= (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "SectorY") == 0 ) //&& FileType == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.SectorY	= (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Level") == 0 )//&& FileType == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.SectorZ	= (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "MaxPages") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.MaxPages	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "PageDesc1") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc1, sizeof(pData->curEncyclopediaData.sDesc1)/sizeof(pData->curEncyclopediaData.sDesc1[0]) );
			pData->curEncyclopediaData.sDesc1[sizeof(pData->curEncyclopediaData.sDesc1)/sizeof(pData->curEncyclopediaData.sDesc1[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc2") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc2, sizeof(pData->curEncyclopediaData.sDesc2)/sizeof(pData->curEncyclopediaData.sDesc2[0]) );
			pData->curEncyclopediaData.sDesc2[sizeof(pData->curEncyclopediaData.sDesc2)/sizeof(pData->curEncyclopediaData.sDesc2[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc3") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc3, sizeof(pData->curEncyclopediaData.sDesc3)/sizeof(pData->curEncyclopediaData.sDesc3[0]) );
			pData->curEncyclopediaData.sDesc3[sizeof(pData->curEncyclopediaData.sDesc3)/sizeof(pData->curEncyclopediaData.sDesc3[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc4") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc4, sizeof(pData->curEncyclopediaData.sDesc4)/sizeof(pData->curEncyclopediaData.sDesc4[0]) );
			pData->curEncyclopediaData.sDesc4[sizeof(pData->curEncyclopediaData.sDesc4)/sizeof(pData->curEncyclopediaData.sDesc4[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc5") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc5, sizeof(pData->curEncyclopediaData.sDesc5)/sizeof(pData->curEncyclopediaData.sDesc5[0]) );
			pData->curEncyclopediaData.sDesc5[sizeof(pData->curEncyclopediaData.sDesc5)/sizeof(pData->curEncyclopediaData.sDesc5[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc6") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc6, sizeof(pData->curEncyclopediaData.sDesc6)/sizeof(pData->curEncyclopediaData.sDesc6[0]) );
			pData->curEncyclopediaData.sDesc6[sizeof(pData->curEncyclopediaData.sDesc6)/sizeof(pData->curEncyclopediaData.sDesc6[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc7") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc7, sizeof(pData->curEncyclopediaData.sDesc7)/sizeof(pData->curEncyclopediaData.sDesc7[0]) );
			pData->curEncyclopediaData.sDesc7[sizeof(pData->curEncyclopediaData.sDesc7)/sizeof(pData->curEncyclopediaData.sDesc7[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc8") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc8, sizeof(pData->curEncyclopediaData.sDesc8)/sizeof(pData->curEncyclopediaData.sDesc8[0]) );
			pData->curEncyclopediaData.sDesc8[sizeof(pData->curEncyclopediaData.sDesc8)/sizeof(pData->curEncyclopediaData.sDesc8[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc9") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc9, sizeof(pData->curEncyclopediaData.sDesc9)/sizeof(pData->curEncyclopediaData.sDesc9[0]) );
			pData->curEncyclopediaData.sDesc9[sizeof(pData->curEncyclopediaData.sDesc9)/sizeof(pData->curEncyclopediaData.sDesc9[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc10") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc10, sizeof(pData->curEncyclopediaData.sDesc10)/sizeof(pData->curEncyclopediaData.sDesc10[0]) );
			pData->curEncyclopediaData.sDesc10[sizeof(pData->curEncyclopediaData.sDesc10)/sizeof(pData->curEncyclopediaData.sDesc10[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc11") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc11, sizeof(pData->curEncyclopediaData.sDesc11)/sizeof(pData->curEncyclopediaData.sDesc11[0]) );
			pData->curEncyclopediaData.sDesc11[sizeof(pData->curEncyclopediaData.sDesc11)/sizeof(pData->curEncyclopediaData.sDesc11[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc12") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc12, sizeof(pData->curEncyclopediaData.sDesc12)/sizeof(pData->curEncyclopediaData.sDesc12[0]) );
			pData->curEncyclopediaData.sDesc12[sizeof(pData->curEncyclopediaData.sDesc12)/sizeof(pData->curEncyclopediaData.sDesc12[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc13") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc13, sizeof(pData->curEncyclopediaData.sDesc13)/sizeof(pData->curEncyclopediaData.sDesc13[0]) );
			pData->curEncyclopediaData.sDesc13[sizeof(pData->curEncyclopediaData.sDesc13)/sizeof(pData->curEncyclopediaData.sDesc13[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc14") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc14, sizeof(pData->curEncyclopediaData.sDesc14)/sizeof(pData->curEncyclopediaData.sDesc14[0]) );
			pData->curEncyclopediaData.sDesc14[sizeof(pData->curEncyclopediaData.sDesc14)/sizeof(pData->curEncyclopediaData.sDesc14[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc15") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc15, sizeof(pData->curEncyclopediaData.sDesc15)/sizeof(pData->curEncyclopediaData.sDesc15[0]) );
			pData->curEncyclopediaData.sDesc15[sizeof(pData->curEncyclopediaData.sDesc15)/sizeof(pData->curEncyclopediaData.sDesc15[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc16") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc16, sizeof(pData->curEncyclopediaData.sDesc16)/sizeof(pData->curEncyclopediaData.sDesc16[0]) );
			pData->curEncyclopediaData.sDesc16[sizeof(pData->curEncyclopediaData.sDesc16)/sizeof(pData->curEncyclopediaData.sDesc16[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc17") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc17, sizeof(pData->curEncyclopediaData.sDesc17)/sizeof(pData->curEncyclopediaData.sDesc17[0]) );
			pData->curEncyclopediaData.sDesc17[sizeof(pData->curEncyclopediaData.sDesc17)/sizeof(pData->curEncyclopediaData.sDesc17[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc18") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc18, sizeof(pData->curEncyclopediaData.sDesc18)/sizeof(pData->curEncyclopediaData.sDesc18[0]) );
			pData->curEncyclopediaData.sDesc18[sizeof(pData->curEncyclopediaData.sDesc18)/sizeof(pData->curEncyclopediaData.sDesc18[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc19") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc19, sizeof(pData->curEncyclopediaData.sDesc19)/sizeof(pData->curEncyclopediaData.sDesc19[0]) );
			pData->curEncyclopediaData.sDesc19[sizeof(pData->curEncyclopediaData.sDesc19)/sizeof(pData->curEncyclopediaData.sDesc19[0]) - 1] = '\0';
		}
		else if(strcmp(name, "PageDesc20") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sDesc20, sizeof(pData->curEncyclopediaData.sDesc20)/sizeof(pData->curEncyclopediaData.sDesc20[0]) );
			pData->curEncyclopediaData.sDesc20[sizeof(pData->curEncyclopediaData.sDesc20)/sizeof(pData->curEncyclopediaData.sDesc20[0]) - 1] = '\0';
		}
		else if(strcmp(name, "MaxImages") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.MaxImages	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "szFile1") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
	
			if(MAX_ENCYCLOPEDIA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curEncyclopediaData.szFile,pData->szCharData);
			else
			{
				strncpy(pData->curEncyclopediaData.szFile,pData->szCharData,MAX_ENCYCLOPEDIA_CHARS);
				pData->curEncyclopediaData.szFile[MAX_ENCYCLOPEDIA_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_ENCYCLOPEDIA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curEncyclopediaData.szFile[i] = temp;
			}
			
		}
		else if(strcmp(name, "szFile2") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
	
			if(MAX_ENCYCLOPEDIA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curEncyclopediaData.szFile2,pData->szCharData);
			else
			{
				strncpy(pData->curEncyclopediaData.szFile2,pData->szCharData,MAX_ENCYCLOPEDIA_CHARS);
				pData->curEncyclopediaData.szFile2[MAX_ENCYCLOPEDIA_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_ENCYCLOPEDIA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curEncyclopediaData.szFile2[i] = temp;
			}
			
		}
		else if(strcmp(name, "szFile3") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
	
			if(MAX_ENCYCLOPEDIA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curEncyclopediaData.szFile3,pData->szCharData);
			else
			{
				strncpy(pData->curEncyclopediaData.szFile3,pData->szCharData,MAX_ENCYCLOPEDIA_CHARS);
				pData->curEncyclopediaData.szFile3[MAX_ENCYCLOPEDIA_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_ENCYCLOPEDIA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curEncyclopediaData.szFile3[i] = temp;
			}
			
		}
		else if(strcmp(name, "szFile4") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
	
			if(MAX_ENCYCLOPEDIA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curEncyclopediaData.szFile4,pData->szCharData);
			else
			{
				strncpy(pData->curEncyclopediaData.szFile4,pData->szCharData,MAX_ENCYCLOPEDIA_CHARS);
				pData->curEncyclopediaData.szFile4[MAX_ENCYCLOPEDIA_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_ENCYCLOPEDIA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curEncyclopediaData.szFile4[i] = temp;
			}
			
		}
		
		else if(strcmp(name, "ImageDesc1") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sImageDesc1, sizeof(pData->curEncyclopediaData.sImageDesc1)/sizeof(pData->curEncyclopediaData.sImageDesc1[0]) );
			pData->curEncyclopediaData.sImageDesc1[sizeof(pData->curEncyclopediaData.sImageDesc1)/sizeof(pData->curEncyclopediaData.sImageDesc1[0]) - 1] = '\0';	
		}
		else if(strcmp(name, "ImageDesc2") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sImageDesc2, sizeof(pData->curEncyclopediaData.sImageDesc2)/sizeof(pData->curEncyclopediaData.sImageDesc2[0]) );
			pData->curEncyclopediaData.sImageDesc2[sizeof(pData->curEncyclopediaData.sImageDesc2)/sizeof(pData->curEncyclopediaData.sImageDesc2[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ImageDesc3") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sImageDesc3, sizeof(pData->curEncyclopediaData.sImageDesc3)/sizeof(pData->curEncyclopediaData.sImageDesc3[0]) );
			pData->curEncyclopediaData.sImageDesc3[sizeof(pData->curEncyclopediaData.sImageDesc3)/sizeof(pData->curEncyclopediaData.sImageDesc3[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ImageDesc4") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sImageDesc4, sizeof(pData->curEncyclopediaData.sImageDesc4)/sizeof(pData->curEncyclopediaData.sImageDesc4[0]) );
			pData->curEncyclopediaData.sImageDesc4[sizeof(pData->curEncyclopediaData.sImageDesc4)/sizeof(pData->curEncyclopediaData.sImageDesc4[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ImagePosition1X") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionX[0]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePosition1Y") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionY[0]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePosition2X") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionX[1]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePosition2Y") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionY[1]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePosition3X") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionX[2]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePosition3Y") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionY[2]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePosition4X") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionX[3]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePosition4Y") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.sImagePositionY[3]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "SpecialQuestImage") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.SpecialQuestImage = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "BeforeImage") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			
			if(MAX_ENCYCLOPEDIA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curEncyclopediaData.BeforeImage,pData->szCharData);
			else
			{
				strncpy(pData->curEncyclopediaData.BeforeImage,pData->szCharData,MAX_ENCYCLOPEDIA_CHARS);
				pData->curEncyclopediaData.BeforeImage[MAX_ENCYCLOPEDIA_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_ENCYCLOPEDIA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curEncyclopediaData.BeforeImage[i] = temp;
			}
		}
		else if(strcmp(name, "AfterImage") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			
			if(MAX_ENCYCLOPEDIA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curEncyclopediaData.AfterImage,pData->szCharData);
			else
			{
				strncpy(pData->curEncyclopediaData.AfterImage,pData->szCharData,MAX_ENCYCLOPEDIA_CHARS);
				pData->curEncyclopediaData.AfterImage[MAX_ENCYCLOPEDIA_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_ENCYCLOPEDIA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curEncyclopediaData.AfterImage[i] = temp;
			}
		}
		else if(strcmp(name, "ImagePositionBeforeX") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.ImagePositionQX[0]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePositionBeforeY") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.ImagePositionQY[0]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePositionAfterX") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.ImagePositionQX[1]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePositionAfterY") == 0)
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
			pData->curEncyclopediaData.ImagePositionQY[1]	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImageDescBefore") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.ImageDescBefore, sizeof(pData->curEncyclopediaData.ImageDescBefore)/sizeof(pData->curEncyclopediaData.ImageDescBefore[0]) );
			pData->curEncyclopediaData.ImageDescBefore[sizeof(pData->curEncyclopediaData.ImageDescBefore)/sizeof(pData->curEncyclopediaData.ImageDescBefore[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ImageDescAfter") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.ImageDescAfter, sizeof(pData->curEncyclopediaData.ImageDescAfter)/sizeof(pData->curEncyclopediaData.ImageDescAfter[0]) );
			pData->curEncyclopediaData.ImageDescAfter[sizeof(pData->curEncyclopediaData.ImageDescAfter)/sizeof(pData->curEncyclopediaData.ImageDescAfter[0]) - 1] = '\0';
		}
		else if(strcmp(name, "SecretCode") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curEncyclopediaData.sCode, sizeof(pData->curEncyclopediaData.sCode)/sizeof(pData->curEncyclopediaData.sCode[0]) );
			pData->curEncyclopediaData.sCode[sizeof(pData->curEncyclopediaData.sCode)/sizeof(pData->curEncyclopediaData.sCode[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szSounds") == 0 )
		{
			pData->curElement = ENCYCLOPEDIA_ELEMENT;
	
			if(MAX_ENCYCLOPEDIA_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curEncyclopediaData.sSounds,pData->szCharData);
			else
			{
				strncpy(pData->curEncyclopediaData.sSounds,pData->szCharData,MAX_ENCYCLOPEDIA_CHARS);
				pData->curEncyclopediaData.sSounds[MAX_ENCYCLOPEDIA_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_ENCYCLOPEDIA_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curEncyclopediaData.sSounds[i] = temp;
			}
			
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

BOOLEAN ReadInEncyclopediaLocation(STR fileName, BOOLEAN localizedVersion, ENCYCLOPEDIA_LOCATION *Ency, UINT32 FileType2 )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	encyclopediaLocationParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EncyclopediaLocation.xml" );

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

		sprintf(errorBuf, "XML Parser Error in EncyclopediaLocation.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
