//zilpin: pellet spread patterns externalized in XML
#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "math.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "items.h"
	#include "weapons.h"
	#include "text.h"
	#include "Soldier Profile.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "Random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "morale.h"
	#include "SkillCheck.h"
	#include "gamesettings.h"
	#include "SaveLoadMap.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

#define LIST_BUFFER_LEN         (32)
#define LIST_BUFFER_SIZE        (LIST_BUFFER_LEN * sizeof(t_SpreadPattern))
#define CALC_NEW_LIST_SIZE(c)   ( LIST_BUFFER_SIZE * (1+ ((c) / LIST_BUFFER_LEN)) )

#define ARRAY_BUFFER_LEN        (32)
#define ARRAY_BUFFER_SIZE       (ARRAY_BUFFER_LEN * sizeof(DOUBLE))
#define CALC_NEW_ARRAY_SIZE(c)  ( ARRAY_BUFFER_SIZE * (1+ ((c) / ARRAY_BUFFER_LEN)) )

struct
{
	PARSE_STAGE	curElement;

	CHAR8       szCharData[MAX_CHAR_DATA_LENGTH+1];

	UINT32      currentDepth;
	UINT32      maxReadDepth;
}
typedef spreadpatternParseData;


static void XMLCALL 
spreadpatternStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	spreadpatternParseData * pData = (spreadpatternParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "SPREADPATTERNLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			giSpreadPatternCount = 0;
			gpSpreadPattern = NULL;
			gpSpreadPattern = (t_SpreadPattern *)MemAlloc( LIST_BUFFER_SIZE );
			Assert(gpSpreadPattern);
			memset( gpSpreadPattern, 0, LIST_BUFFER_SIZE );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SPREADPATTERN") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT_LIST;

			giSpreadPatternCount++;
			if( !(giSpreadPatternCount % LIST_BUFFER_LEN) ){
				//End of buffer.  Expand buffer.
				gpSpreadPattern = (t_SpreadPattern *)MemRealloc(gpSpreadPattern, CALC_NEW_LIST_SIZE(giSpreadPatternCount) );
				Assert(gpSpreadPattern);
			}
			memset( &(gpSpreadPattern[giSpreadPatternCount-1]) , 0 , sizeof(t_SpreadPattern) );
			gpSpreadPattern[giSpreadPatternCount-1].iCount = 0;
			gpSpreadPattern[giSpreadPatternCount-1].xspread = 0.0;
			gpSpreadPattern[giSpreadPatternCount-1].yspread = 0.0;
			gpSpreadPattern[giSpreadPatternCount-1].method = giSpreadPatternMethod_Default;

			gpSpreadPattern[giSpreadPatternCount-1].x = (DOUBLE *)MemAlloc( ARRAY_BUFFER_SIZE );
			Assert( gpSpreadPattern[giSpreadPatternCount-1].x );
			memset( gpSpreadPattern[giSpreadPatternCount-1].x , 0 , ARRAY_BUFFER_SIZE );
			gpSpreadPattern[giSpreadPatternCount-1].y = (DOUBLE *)MemAlloc( ARRAY_BUFFER_SIZE );
			Assert( gpSpreadPattern[giSpreadPatternCount-1].y );
			memset( gpSpreadPattern[giSpreadPatternCount-1].y , 0 , ARRAY_BUFFER_SIZE );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "p") == 0 )//&& pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;
			gpSpreadPattern[giSpreadPatternCount-1].iCount++;
			if(  !(gpSpreadPattern[giSpreadPatternCount-1].iCount % ARRAY_BUFFER_LEN ) ){
				//End of buffer.  Expand.
				gpSpreadPattern[giSpreadPatternCount-1].x = (DOUBLE *)MemRealloc(gpSpreadPattern[giSpreadPatternCount-1].x, CALC_NEW_ARRAY_SIZE(gpSpreadPattern[giSpreadPatternCount-1].iCount) );
				Assert( gpSpreadPattern[giSpreadPatternCount-1].x );
				gpSpreadPattern[giSpreadPatternCount-1].y = (DOUBLE *)MemRealloc(gpSpreadPattern[giSpreadPatternCount-1].y, CALC_NEW_ARRAY_SIZE(gpSpreadPattern[giSpreadPatternCount-1].iCount) );
				Assert( gpSpreadPattern[giSpreadPatternCount-1].y );
			}
			gpSpreadPattern[giSpreadPatternCount-1].x[ gpSpreadPattern[giSpreadPatternCount-1].iCount - 1 ] = 0.0;
			gpSpreadPattern[giSpreadPatternCount-1].y[ gpSpreadPattern[giSpreadPatternCount-1].iCount - 1 ] = 0.0;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if( //pData->curElement == ELEMENT &&
				(  strcmp(name, "x") == 0
				|| strcmp(name, "y") == 0
				|| strcmp(name, "xspread") == 0
				|| strcmp(name, "yspread") == 0
				|| strcmp(name, "NAME") == 0
				|| strcmp(name, "method") == 0
				))
				
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
spreadpatternCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	spreadpatternParseData * pData = (spreadpatternParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		&& pData->curElement == ELEMENT_PROPERTY
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
spreadpatternEndElementHandle(void *userData, const XML_Char *name)
{
	DOUBLE d;
	
	spreadpatternParseData * pData = (spreadpatternParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "SPREADPATTERNLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "SPREADPATTERN") == 0)
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "p") == 0)
		{
			//I would prefer the format to be:
			//  <SPREADPATTERN><p x="0.1" y="0.1"/><p x="0.2" y="0.2"/></SPREADPATTERN>
			//but it's:
			//  <SPREADPATTERN><p/><x>0.1</x><y>0.1</y><p/><x>0.2</x><y>0.2</y></SPREADPATTERN>
			//because the XML editor will probably have problems with using attributes instead of tags.
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "x") == 0)
		{
			pData->curElement = ELEMENT;
			d = atof(pData->szCharData);
			gpSpreadPattern[giSpreadPatternCount-1].x[ gpSpreadPattern[giSpreadPatternCount-1].iCount-1 ] = d;
		}
		else if(strcmp(name, "y") == 0)
		{
			pData->curElement = ELEMENT;
			d = atof(pData->szCharData);
			gpSpreadPattern[giSpreadPatternCount-1].y[ gpSpreadPattern[giSpreadPatternCount-1].iCount-1 ] = d;
		}
		else if(strcmp(name, "xspread") == 0)
		{
			pData->curElement = ELEMENT;
			d = fabs( atof(pData->szCharData) );
			gpSpreadPattern[giSpreadPatternCount-1].xspread = d;
		}
		else if(strcmp(name, "yspread") == 0)
		{
			pData->curElement = ELEMENT;
			d = fabs( atof(pData->szCharData) );
			gpSpreadPattern[giSpreadPatternCount-1].yspread = d;
		}
		else if(strcmp(name, "NAME") == 0)
		{
			pData->curElement = ELEMENT;
			//Trim whitespace.
			while( pData->szCharData[0] == ' ' ) memmove( pData->szCharData , &pData->szCharData[1] , strlen(&pData->szCharData[1]) );
			while( strlen(pData->szCharData)>0 && pData->szCharData[ strlen(pData->szCharData)-1 ] == ' ' ) pData->szCharData[ strlen(pData->szCharData)-1 ]=0;
			//Clean and copy.
			memset(gpSpreadPattern[giSpreadPatternCount-1].Name,0,SPREADPATTERN_NAME_SIZE);
			strncpy(gpSpreadPattern[giSpreadPatternCount-1].Name,pData->szCharData,SPREADPATTERN_NAME_SIZE-1);
		}
		else if(strcmp(name, "method") == 0)
		{
			pData->curElement = ELEMENT;
			//Trim whitespace.
			while( pData->szCharData[0] == ' ' ) memmove( pData->szCharData , &pData->szCharData[1] , strlen(&pData->szCharData[1]) );
			while( strlen(pData->szCharData)>0 && pData->szCharData[ strlen(pData->szCharData)-1 ] == ' ' ) pData->szCharData[ strlen(pData->szCharData)-1 ]=0;
			//Find it.
			for(unsigned int i=0;i<SPREADPATTERNMETHOD_COUNT;++i){
				if(strcmp(pData->szCharData,  gSpreadPatternMethodNames[i] ) == 0){
					gpSpreadPattern[giSpreadPatternCount-1].method = i;
					break;
				}
			}
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;

}




BOOLEAN ReadInSpreadPatterns(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	spreadpatternParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading SpreadPatterns.xml" );

	// Open spread pattern file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );
	
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

	
	XML_SetElementHandler(parser, spreadpatternStartElementHandle, spreadpatternEndElementHandle);
	XML_SetCharacterDataHandler(parser, spreadpatternCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	//pData.maxArraySize = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in SpreadPatterns.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	//Output for debugging
	//WriteSpreadPatterns();

	return( TRUE );
}


BOOLEAN WriteSpreadPatterns()
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"WriteSpreadPatterns");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TableData\\SpreadPatterns_out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )  return( FALSE );


	{
		FilePrintf(hFile,"<SPREADPATTERNLIST>\r\n");
		FilePrintf(hFile,"<!-- giSpreadPatternCount = %d  -->\r\n", giSpreadPatternCount );
		for(INT32 i = 0 ; i < giSpreadPatternCount; i++)
		{
			FilePrintf(hFile,"\t<SPREADPATTERN>\r\n");
			FilePrintf(hFile,"\t<!-- index = %d -->\r\n",i);
			FilePrintf(hFile,"\t<Name>%s</Name>\r\n",gpSpreadPattern[i].Name);
			FilePrintf(hFile,"\t<method>%s</method> <!-- %d -->\r\n",gSpreadPatternMethodNames[gpSpreadPattern[i].method], gpSpreadPattern[i].method);
			FilePrintf(hFile,"\t<xspread>%2.4f</xspread>",gpSpreadPattern[i].xspread);
			FilePrintf(hFile," <yspread>%2.4f</yspread>\r\n",gpSpreadPattern[i].yspread);
			FilePrintf(hFile,"\t<!-- p iCount = %d -->\r\n",gpSpreadPattern[i].iCount);
			for(INT32 j=0;j<gpSpreadPattern[i].iCount;j++)
			{
				FilePrintf(hFile,"\t\t<p>");
				FilePrintf(hFile,"<x>%2.4f</x>",gpSpreadPattern[i].x[j]);
				FilePrintf(hFile,"<y>%2.4f</y>",gpSpreadPattern[i].y[j]);
				FilePrintf(hFile,"</p>\r\n");
			}
			FilePrintf(hFile,"\t</SPREADPATTERN>\r\n");
		}
		FilePrintf(hFile,"</SPREADPATTERNLIST>\r\n");
		FilePrintf(hFile,"<!--End Of File-->\r\n");
	}

	FileClose( hFile );

	return( TRUE );
}

//zilpin: pellet spread patterns externalized in XML
//This function is used primarily by XML loaders for Items and AmmoTypes.
//Given a string will determine if it is a SpreadPattern name or literal index,
//and return an integer index of the spreadpattern in the gpSpreadPattern array.
//If the name is not found, or the index was out of range, then return 0.
int FindSpreadPatternIndex( const STR strName )
{
	UINT32 ui = 0;
	INT32 i,n=0;
	char str[SPREADPATTERN_NAME_SIZE]={};

	//Copy search name sans leading and trailing space.
	strncpy(str,strName,SPREADPATTERN_NAME_SIZE-1);
	str[SPREADPATTERN_NAME_SIZE-1]=0;
	while( str[0] == ' ' ) memmove( str , &str[1] , strlen(&str[1]) );
	while( strlen(str)>0 && str[ strlen(str)-1 ] == ' ' ) str[ strlen(str)-1 ]=0;

	//Named pattern or raw index?
	for(ui=0;ui<strlen(str);ui++) 
		if( !isdigit(str[ui]) ) 
			break;

	if(ui<strlen(str))
	{
		//Holds named SpreadPattern.  Search.
		for(i=0;i<giSpreadPatternCount;i++)
		{
			if( !strcmp(str, gpSpreadPattern[i].Name) )
			{
				//Match!  Assign and return.
				n=i;
				break;
			}
		}
	}else{
		//Holds numeric SpreadPattern.
		//Validate.
		n = atol(str);
		if( n<0 || n>=giSpreadPatternCount )
			n=0;
	}

	return n;
}
