#include "sgp.h"
#include "Debug Control.h"
#include "expat.h"
#include "XML.h"
#include "Interface.h"
#include "LuaInitNPCs.h"
#include "email.h"

struct
{
    PARSE_STAGE	curElement;

    CHAR8		szCharData[MAIL_STRING_SIZE + 1];
    CHAR16      currentMessage[MAIL_STRING_SIZE];
    UINT16      currentEmailIndex;
    UINT16      currentMessageIndex;

    UINT32		maxArraySize;
    UINT32		curIndex;
    UINT32		currentDepth;
    UINT32		maxReadDepth;
} typedef EmailXMLParseData;

BOOLEAN Emails_TextOnly;

static void XMLCALL
EmailOtherStartElementHandle(void* userData, const XML_Char* name, const XML_Char** atts)
{
    EmailXMLParseData* pData = (EmailXMLParseData*)userData;

    if (pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
    {
        if (strcmp(name, "EMAILS") == 0 && pData->curElement == ELEMENT_NONE)
        {
            pData->curElement = ELEMENT_LIST;

            pData->maxReadDepth++; //we are not skipping this element
        }
        else if (strcmp(name, "EMAIL") == 0 && pData->curElement == ELEMENT_LIST)
        {
            pData->curElement = ELEMENT;

            pData->maxReadDepth++; //we are not skipping this element
        }
        else if (pData->curElement == ELEMENT &&
            (strcmp(name, "Index") == 0 ||
                strcmp(name, "Sender") == 0 ||
                strcmp(name, "Subject") == 0 ||
                strcmp(name, "Message") == 0))
        {
            pData->curElement = ELEMENT_PROPERTY;

            pData->maxReadDepth++; //we are not skipping this element
        }

        pData->szCharData[0] = '\0';
    }

    pData->currentDepth++;

}

static void XMLCALL
EmailOtherCharacterDataHandle(void* userData, const XML_Char* str, int len)
{
    EmailXMLParseData* pData = (EmailXMLParseData*)userData;

    if ((pData->currentDepth <= pData->maxReadDepth) &&
        (strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
        ) {
        strncat(pData->szCharData, str, __min((unsigned int)len, MAX_CHAR_DATA_LENGTH - strlen(pData->szCharData)));
    }
}


static void XMLCALL
EmailOtherEndElementHandle(void* userData, const XML_Char* name)
{
    EmailXMLParseData* pData = (EmailXMLParseData*)userData;

    if (pData->currentDepth <= pData->maxReadDepth)
    {
        if (strcmp(name, "EMAILS") == 0)
        {
            pData->curElement = ELEMENT_NONE;
        }
        else if (strcmp(name, "EMAIL") == 0)
        {
            pData->curElement = ELEMENT_LIST;
            if ( Emails_TextOnly )
            {
                pData->currentEmailIndex += 1;
                pData->currentMessageIndex = 0;
            }
        }
        else if (strcmp(name, "Sender") == 0)
        {
            pData->curElement = ELEMENT;
            if ( !Emails_TextOnly )
            {
                gEmails.push_back(EMAIL_XML {});
                gEmails.back().Sender = atol(pData->szCharData);
            }
        }
        else if (strcmp(name, "Subject") == 0)
        {
            pData->curElement = ELEMENT;
            if ( !Emails_TextOnly )
            {
                MultiByteToWideChar(CP_UTF8, 0, pData->szCharData, -1, gEmails.back().Subject, sizeof(gEmails.back().Subject) / sizeof(gEmails.back().Subject[0]));
            }
            else
            {
                // Replace existing text with localized version
                const auto i = pData->currentEmailIndex;
                MultiByteToWideChar(CP_UTF8, 0, pData->szCharData, -1, gEmails[i].Subject, sizeof(gEmails[i].Subject) / sizeof(gEmails[i].Subject[0]));
            }
        }
        else if (strcmp(name, "Message") == 0)
        {
            pData->curElement = ELEMENT;
            MultiByteToWideChar(CP_UTF8, 0, pData->szCharData, -1, pData->currentMessage, sizeof(pData->currentMessage) / sizeof(pData->currentMessage[0]));

            if ( !Emails_TextOnly )
            {
                gEmails.back().Messages.emplace_back(pData->currentMessage);
            }
            else
            {
                // Replace existing text with localized version
                const auto i = pData->currentEmailIndex;
                const auto j = pData->currentMessageIndex;
                gEmails[i].Messages[j] = pData->currentMessage;

                pData->currentMessageIndex++;
            }
        }

        pData->maxReadDepth--;
    }
    pData->currentDepth--;
}

BOOLEAN ReadInExternalizedEmails(STR fileName, BOOLEAN localizedVersion)
{
    HWFILE		hFile;
    UINT32		uiBytesRead;
    UINT32		uiFSize;
    CHAR8* lpcBuffer;
    XML_Parser	parser = XML_ParserCreate(NULL);

    EmailXMLParseData pData;

    DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Emails.xml");

    Emails_TextOnly = localizedVersion;

    // Open file
    hFile = FileOpen(fileName, FILE_ACCESS_READ, FALSE);
    if (!hFile)
        return(localizedVersion);

    uiFSize = FileGetSize(hFile);
    lpcBuffer = (CHAR8*)MemAlloc(uiFSize + 1);

    //Read in block
    if (!FileRead(hFile, lpcBuffer, uiFSize, &uiBytesRead))
    {
        MemFree(lpcBuffer);
        return(FALSE);
    }

    lpcBuffer[uiFSize] = 0; //add a null terminator

    FileClose(hFile);


    XML_SetElementHandler(parser, EmailOtherStartElementHandle, EmailOtherEndElementHandle);
    XML_SetCharacterDataHandler(parser, EmailOtherCharacterDataHandle);


    memset(&pData, 0, sizeof(pData));
    XML_SetUserData(parser, &pData);

    gEmails.reserve(XML_JA2UB_SPECK_DISMISSALREFUND);
    if (!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
    {
        CHAR8 errorBuf[511];

        sprintf(errorBuf, "XML Parser Error in Emails.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
        LiveMessage(errorBuf);

        MemFree(lpcBuffer);
        return FALSE;
    }

    MemFree(lpcBuffer);


    XML_ParserFree(parser);


    return(TRUE);
}
