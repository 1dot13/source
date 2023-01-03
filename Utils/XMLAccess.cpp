#include "Types.h"

#define	WEAPONSFILENAME					"TABLEDATA\\Weapons.dat"
#define MAX_CHAR_DATA_LENGTH			500

enum
{
	ELEMENT_NONE = 0,
	ELEMENT_LIST,
	ELEMENT,
	ELEMENT_PROPERTY,
}
typedef PARSE_STAGE;

struct
{
	PARSE_STAGE	curElement;

	INT8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	WEAPONTYPE		curWeapon;
	WEAPONTYPE *	curWeaponList;
	UINT32			maxWeapons;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef ParseData;


static void XMLCALL
StartElementHandle(void *userData, const char *name, const char **atts)
{
	ParseData * pData = (ParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "LIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENTLIST;

			memset(pData->curWeaponList,0,sizeof(WEAPONTYPE)*pData->maxWeapons);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "WEAPON") == 0 && pData->curElement == ELEMENTLIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curWeapon,0,sizeof(WEAPONTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szWeaponName") == 0 ||
				strcmp(name, "ubWeaponClass") == 0 ||
				strcmp(name, "ubWeaponType") == 0 ||
				strcmp(name, "ubCalibre") == 0 ||
				strcmp(name, "ubReadyTime") == 0 ||
				strcmp(name, "ubShotsPer4Turns") == 0 ||
				strcmp(name, "ubShotsPerBurst") == 0 ||
				strcmp(name, "ubBurstPenalty") == 0 ||
				strcmp(name, "ubBulletSpeed") == 0 ||
				strcmp(name, "ubImpact") == 0 ||
				strcmp(name, "ubDeadliness") == 0 ||
				strcmp(name, "bAccuracy") == 0 ||
				strcmp(name, "ubMagSize") == 0 ||
				strcmp(name, "usRange")	 == 0 ||
				strcmp(name, "usReloadDelay") == 0 ||
				strcmp(name, "ubAttackVolume") == 0 ||
				strcmp(name, "ubHitVolume") == 0 ||
				strcmp(name, "sSound") == 0 ||
				strcmp(name, "sBurstSound") == 0 ||
				strcmp(name, "sReloadSound") == 0 ||
				strcmp(name, "sLocknLoadSound") == 0 ||
				strcmp(name, "bBaseAutofireCost") == 0 ||
				strcmp(name, "bAutofireShotsPerFiveAP") == 0))
		{
			pData->curElement = ELEMENT_PROPERY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
weaponCharacterDataHandle(void *userData, const char *str, int len)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
weaponEndElementHandle(void *userData, const char *name)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "WEAPONLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "WEAPON") == 0)
		{
			pData->curElement = ELEMENTLIST;

			if(pData->curWeapon.uiIndex < pData->maxWeapons)
			{
				pData->curWeaponList[pData->curWeapon.uiIndex] = pData->curWeapon; //write the weapon into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.uiIndex = atol(pData->szCharData);
		}
		else if(strcmp(name, "szWeaponName") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_NAME_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curWeapon.szWeaponName,pData->szCharData);
			else
			{
				strncpy(pData->curWeapon.szWeaponName,pData->szCharData,MAX_NAME_LENGTH);
				pData->curWeapon.szWeaponName[MAX_NAME_LENGTH] = '\0';
			}
		}
		else if(strcmp(name, "ubWeaponClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubWeaponClass = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubWeaponType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubWeaponType = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubCalibre") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubCalibre = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubReadyTime") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubReadyTime = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubShotsPer4Turns") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubShotsPer4Turns = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubShotsPerBurst") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubShotsPerBurst = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubBurstPenalty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubBurstPenalty = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubBulletSpeed") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubBulletSpeed = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubImpact") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubImpact = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDeadliness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubDeadliness = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bAccuracy") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.bAccuracy = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubMagSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubMagSize = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usRange")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.usRange = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usReloadDelay") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.usReloadDelay = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubAttackVolume") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubAttackVolume = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubHitVolume") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.ubHitVolume = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "sSound") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.sSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sBurstSound") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.sBurstSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sReloadSound") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.sReloadSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "sLocknLoadSound") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.sLocknLoadSound = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "bBaseAutofireCost") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.bBaseAutofireCost = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bAutofireShotsPerFiveAP") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeapon.bAutofireShotsPerFiveAP = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInWeaponStats()
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	weaponParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading weapons.dat" );

	// Open weapons file
	hFile = FileOpen( WEAPONSFILENAME, FILE_ACCESS_READ, FALSE );
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

	
	XML_SetElementHandler(parser, weaponStartElementHandle, weaponEndElementHandle);
	XML_SetCharacterDataHandler(parser, weaponCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curWeaponList = Weapon;
	pData.maxWeapons = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Weapons.dat: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

#ifdef JA2TESTVERSION
	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\~Weapons.dat", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<WEAPONLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			INT8 * szRemainder = Weapon[cnt].szWeaponName; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t<WEAPON>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									Weapon[cnt].uiIndex);

			FilePrintf(hFile,"\t\t<szWeaponName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = strcspn(szRemainder,"&<>\'\"\0");
				INT8 invChar = szRemainder[uiCharLoc];
				
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
			FilePrintf(hFile,"</szWeaponName>\r\n");

			FilePrintf(hFile,"\t\t<ubWeaponClass>%d</ubWeaponClass>\r\n",						Weapon[cnt].ubWeaponClass);
			FilePrintf(hFile,"\t\t<ubWeaponType>%d</ubWeaponType>\r\n",							Weapon[cnt].ubWeaponType);
			FilePrintf(hFile,"\t\t<ubCalibre>%d</ubCalibre>\r\n",								Weapon[cnt].ubCalibre);
			FilePrintf(hFile,"\t\t<ubReadyTime>%d</ubReadyTime>\r\n",							Weapon[cnt].ubReadyTime);
			FilePrintf(hFile,"\t\t<ubShotsPer4Turns>%d</ubShotsPer4Turns>\r\n",					Weapon[cnt].ubShotsPer4Turns);
			FilePrintf(hFile,"\t\t<ubShotsPerBurst>%d</ubShotsPerBurst>\r\n",					Weapon[cnt].ubShotsPerBurst);
			FilePrintf(hFile,"\t\t<ubBurstPenalty>%d</ubBurstPenalty>\r\n",						Weapon[cnt].ubBurstPenalty);
			FilePrintf(hFile,"\t\t<ubBulletSpeed>%d</ubBulletSpeed>\r\n",						Weapon[cnt].ubBulletSpeed);
			FilePrintf(hFile,"\t\t<ubImpact>%d</ubImpact>\r\n",									Weapon[cnt].ubImpact);
			FilePrintf(hFile,"\t\t<ubDeadliness>%d</ubDeadliness>\r\n",							Weapon[cnt].ubDeadliness);
			FilePrintf(hFile,"\t\t<bAccuracy>%d</bAccuracy>\r\n",								Weapon[cnt].bAccuracy);
			FilePrintf(hFile,"\t\t<ubMagSize>%d</ubMagSize>\r\n",								Weapon[cnt].ubMagSize);
			FilePrintf(hFile,"\t\t<usRange>%d</usRange>\r\n",									Weapon[cnt].usRange);
			FilePrintf(hFile,"\t\t<usReloadDelay>%d</usReloadDelay>\r\n",						Weapon[cnt].usReloadDelay);
			FilePrintf(hFile,"\t\t<ubAttackVolume>%d</ubAttackVolume>\r\n",						Weapon[cnt].ubAttackVolume);
			FilePrintf(hFile,"\t\t<ubHitVolume>%d</ubHitVolume>\r\n",							Weapon[cnt].ubHitVolume);
			FilePrintf(hFile,"\t\t<sSound>%d</sSound>\r\n",										Weapon[cnt].sSound);
			FilePrintf(hFile,"\t\t<sBurstSound>%d</sBurstSound>\r\n",							Weapon[cnt].sBurstSound);
			FilePrintf(hFile,"\t\t<sReloadSound>%d</sReloadSound>\r\n",							Weapon[cnt].sReloadSound);
			FilePrintf(hFile,"\t\t<sLocknLoadSound>%d</sLocknLoadSound>\r\n",					Weapon[cnt].sLocknLoadSound);
			FilePrintf(hFile,"\t\t<bBaseAutofireCost>%d</bBaseAutofireCost>\r\n",				Weapon[cnt].bBaseAutofireCost);
			FilePrintf(hFile,"\t\t<bAutofireShotsPerFiveAP>%d</bAutofireShotsPerFiveAP>\r\n",	Weapon[cnt].bAutofireShotsPerFiveAP);
			FilePrintf(hFile,"\t</WEAPON>\r\n");
		}
		FilePrintf(hFile,"</WEAPONLIST>\r\n");
	}
	FileClose( hFile );
#endif

	XML_ParserFree(parser);

	return( TRUE );
}
