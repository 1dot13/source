#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include <string.h>
#include "sgp.h"
#include "Sound Control.h"
#include "Debug Control.h"
#include "expat.h"
#include "XML.h"
#endif

extern char szSoundEffects[MAX_SAMPLES][255];

struct
{
    PARSE_STAGE     curElement;
    CHAR8           szCharData[MAX_CHAR_DATA_LENGTH+1];
    UINT32          maxArraySize;
    UINT32          curIndex;
    UINT32          currentDepth;
    UINT32          maxReadDepth;
}
typedef soundParseData;

static void XMLCALL
soundStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
    soundParseData * pData = (soundParseData *)userData;

    if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
    {
        if(strcmp(name, "SOUNDLIST") == 0 && pData->curElement == ELEMENT_NONE)
        {
            pData->curElement = ELEMENT_LIST;

            pData->maxReadDepth++; //we are not skipping this element
        }
        else if(strcmp(name, "SOUND") == 0 && pData->curElement == ELEMENT_LIST)
        {
            pData->curElement = ELEMENT;

            pData->maxReadDepth++; //we are not skipping this element
            pData->curIndex++;
        }

        pData->szCharData[0] = '\0';
    }

    pData->currentDepth++;

}

static void XMLCALL
soundCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
    soundParseData * pData = (soundParseData *)userData;

    if((pData->currentDepth <= pData->maxReadDepth) && (strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH))
    {
        strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
    }
}


static void XMLCALL
soundEndElementHandle(void *userData, const XML_Char *name)
{
    soundParseData * pData = (soundParseData *)userData;

    if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
    {
        if(strcmp(name, "SOUNDLIST") == 0)
        {
            pData->curElement = ELEMENT_NONE;
        }
        else if(strcmp(name, "SOUND") == 0)
        {
            pData->curElement = ELEMENT_LIST;

            if(pData->curIndex < pData->maxArraySize)
            {
                char temp;
                for(int i=0;i<min((int)strlen(pData->szCharData),255);i++)
                {
                    temp = pData->szCharData[i];
                    szSoundEffects[pData->curIndex][i] = temp;
                }
            }
        }
        pData->maxReadDepth--;
    }
    pData->currentDepth--;
}

BOOLEAN ReadInSoundArray(STR fileName)
{
    HWFILE		hFile;
    UINT32		uiBytesRead;
    UINT32		uiFSize;
    CHAR8 *		lpcBuffer;
    XML_Parser	parser = XML_ParserCreate(NULL);
    soundParseData pData;
    DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("Loading %s",SOUNDSFILENAME ) );

    // Open sounds file
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

    XML_SetElementHandler(parser, soundStartElementHandle, soundEndElementHandle);
    XML_SetCharacterDataHandler(parser, soundCharacterDataHandle);
    memset(&pData,0,sizeof(pData));
    pData.maxArraySize = MAX_SAMPLES;
    pData.curIndex = -1;
    XML_SetUserData(parser, &pData);
    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
    {
        CHAR8 errorBuf[511];
        sprintf(errorBuf, "XML Parser Error in %s.xml: %s at line %d", SOUNDSFILENAME, XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
        LiveMessage(errorBuf);
        MemFree(lpcBuffer);
        return FALSE;
    }
    MemFree(lpcBuffer);
    XML_ParserFree(parser);
    return( TRUE );
}

BOOLEAN WriteSoundArray()
{
    HWFILE		hFile;
    DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("WriteSoundArray"));
    //Debug code; make sure that what we got from the file is the same as what's there
    // Open a new file
    hFile = FileOpen( "TABLEDATA\\Sounds out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
    if ( !hFile )
        return( FALSE );
    UINT32 cnt;

    FilePrintf(hFile,"<SOUNDLIST>\r\n");
    for(cnt = 0;cnt < NUM_SAMPLES;cnt++)
    {
        FilePrintf(hFile,"\t<SOUND>");
        STR8 szRemainder = szSoundEffects[cnt]; //the remaining string to be output (for making valid XML)
        while(szRemainder[0] != '\0')
        {
            UINT32 uiCharLoc = strcspn(szRemainder,"&<>\'\"\0");
            char invChar = szRemainder[uiCharLoc];

            if(uiCharLoc)
            {
                szRemainder[uiCharLoc] = '\0';
                FilePrintf(hFile,"%s",szRemainder);
                szRemainder[uiCharLoc] = invChar;
            }
            szRemainder += uiCharLoc;
            switch(invChar)
            {
                case '&':
                    FilePrintf(hFile,"&amp;");
                    szRemainder++;
                    break;

                case '<':
                    FilePrintf(hFile,"&lt;");
                    szRemainder++;
                    break;

                case '>':
                    FilePrintf(hFile,"&gt;");
                    szRemainder++;
                    break;

                case '\'':
                    FilePrintf(hFile,"&apos;");
                    szRemainder++;
                    break;

                case '\"':
                    FilePrintf(hFile,"&quot;");
                    szRemainder++;
                    break;
            }
        }
        FilePrintf(hFile,"</SOUND>\r\n");
    }
    FilePrintf(hFile,"</SOUNDLIST>\r\n");
    FileClose( hFile );
    return( TRUE );
}

