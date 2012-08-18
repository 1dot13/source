#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "weapons.h"
	#include "overhead.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif


struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	AMMOTYPE		curAmmoType;
	AMMOTYPE *	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef ammotypeParseData;

static void XMLCALL
ammotypeStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	ammotypeParseData * pData = (ammotypeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "AMMOTYPELIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(AMMOTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "AMMOTYPE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curAmmoType,0,sizeof(AMMOTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "fontColour") == 0 ||
				strcmp(name, "grayed") == 0 ||
				strcmp(name, "offNormal") == 0 ||
				strcmp(name, "onNormal") == 0 ||
				strcmp(name, "structureImpactReductionMultiplier") == 0 ||
				strcmp(name, "armourImpactReductionMultiplier") == 0 ||
				strcmp(name, "afterArmourDamageMultiplier") == 0 ||
				strcmp(name, "beforeArmourDamageMultiplier") == 0 ||
				strcmp(name, "multipleBulletDamageMultiplier") == 0 ||
				strcmp(name, "structureImpactReductionDivisor") == 0 ||
				strcmp(name, "armourImpactReductionDivisor") == 0 ||
				strcmp(name, "afterArmourDamageDivisor") == 0 ||
				strcmp(name, "beforeArmourDamageDivisor") == 0 ||
				strcmp(name, "multipleBulletDamageDivisor") == 0 ||
				strcmp(name, "zeroMinimumDamage") == 0 ||
				strcmp(name, "canGoThrough") == 0 ||
				strcmp(name, "standardIssue") == 0 ||
				strcmp(name, "numberOfBullets") == 0 ||
				//zilpin: pellet spread patterns externalized in XML
				strcmp(name, "spreadPattern") == 0 ||
				strcmp(name, "highExplosive") == 0 ||
				strcmp(name, "explosionSize") == 0 ||
				strcmp(name, "antiTank") == 0 ||
				strcmp(name, "dart") == 0 ||
				strcmp(name, "knife") == 0 ||
				strcmp(name, "ignoreArmour") == 0 ||
				strcmp(name, "lockBustingPower") == 0 ||
				strcmp(name, "acidic") == 0 ||
				strcmp(name, "tracerEffect") == 0 ||
				strcmp(name, "monsterSpit") == 0 ||
				strcmp(name, "temperatureModificator") == 0 ||
				strcmp(name, "PoisonPercentage") == 0 ||
				strcmp(name, "dirtModificator") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
ammotypeCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	ammotypeParseData * pData = (ammotypeParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
ammotypeEndElementHandle(void *userData, const XML_Char *name)
{
	ammotypeParseData * pData = (ammotypeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "AMMOTYPELIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "AMMOTYPE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curAmmoType.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curAmmoType.uiIndex] = pData->curAmmoType; //write the ammotype into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.uiIndex	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "fontColour") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.fontColour	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "grayed") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.grayed	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "offNormal") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.offNormal	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "onNormal") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.onNormal	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "structureImpactReductionMultiplier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.structureImpactReductionMultiplier	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "armourImpactReductionMultiplier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.armourImpactReductionMultiplier	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "afterArmourDamageMultiplier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.afterArmourDamageMultiplier	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "beforeArmourDamageMultiplier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.beforeArmourDamageMultiplier	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "multipleBulletDamageMultiplier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.multipleBulletDamageMultiplier	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "structureImpactReductionDivisor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.structureImpactReductionDivisor	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "armourImpactReductionDivisor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.armourImpactReductionDivisor	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "afterArmourDamageDivisor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.afterArmourDamageDivisor	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "beforeArmourDamageDivisor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.beforeArmourDamageDivisor	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "multipleBulletDamageDivisor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.multipleBulletDamageDivisor	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "lockBustingPower") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.lockBustingPower	= (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "zeroMinimumDamage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.zeroMinimumDamage	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "canGoThrough") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.canGoThrough	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "standardIssue") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.standardIssue	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "numberOfBullets") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.numberOfBullets	= (UINT8) atol(pData->szCharData);
		}
		//zilpin: pellet spread patterns externalized in XML
		else if(strcmp(name, "spreadPattern") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.spreadPattern = FindSpreadPatternIndex( pData->szCharData );
		}
		else if(strcmp(name, "highExplosive") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.highExplosive	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "explosionSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.explosionSize	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "antiTank") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.antiTank	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "dart") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.dart	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "knife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.knife	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "monsterSpit") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.monsterSpit	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ignoreArmour") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.ignoreArmour	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "acidic") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.acidic	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "tracerEffect") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.tracerEffect	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "temperatureModificator") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.temperatureModificator	= (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "PoisonPercentage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.poisonPercentage	= (INT16) atol(pData->szCharData);

			// check: poisonPercentage must be between 0 and 100, fit bad values
			pData->curAmmoType.poisonPercentage = max(0, pData->curAmmoType.poisonPercentage);
			pData->curAmmoType.poisonPercentage = min(100, pData->curAmmoType.poisonPercentage);
		}
		else if(strcmp(name, "dirtModificator") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAmmoType.dirtModificator	= (FLOAT) atof(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInAmmoTypeStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	ammotypeParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading AmmoTypes.xml" );

	// Open ammotype file
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


	XML_SetElementHandler(parser, ammotypeStartElementHandle, ammotypeEndElementHandle);
	XML_SetCharacterDataHandler(parser, ammotypeCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = AmmoTypes;
	pData.maxArraySize = MAXITEMS;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AmmoTypes.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteAmmoTypeStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeammotypestats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\AmmoType out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<AMMOTYPELIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<AMMOTYPE>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",												cnt );
			FilePrintf(hFile,"\t\t<fontColour>%d</fontColour>\r\n",											AmmoTypes[cnt].fontColour	);
			FilePrintf(hFile,"\t\t<grayed>%d</grayed>\r\n",													AmmoTypes[cnt].grayed	);
			FilePrintf(hFile,"\t\t<offNormal>%d</offNormal>\r\n",											AmmoTypes[cnt].offNormal	);
			FilePrintf(hFile,"\t\t<onNormal>%d</onNormal>\r\n",												AmmoTypes[cnt].onNormal	);
			FilePrintf(hFile,"\t\t<structureImpactReductionMultiplier>%d</structureImpactReductionMultiplier>\r\n",	AmmoTypes[cnt].structureImpactReductionMultiplier	);
			FilePrintf(hFile,"\t\t<structureImpactReductionDivisor>%d</structureImpactReductionDivisor>\r\n",		AmmoTypes[cnt].structureImpactReductionDivisor	);
			FilePrintf(hFile,"\t\t<armourImpactReductionMultiplier>%d</armourImpactReductionMultiplier>\r\n",		AmmoTypes[cnt].armourImpactReductionMultiplier	);
			FilePrintf(hFile,"\t\t<armourImpactReductionDivisor>%d</armourImpactReductionDivisor>\r\n",		AmmoTypes[cnt].armourImpactReductionDivisor	);
			FilePrintf(hFile,"\t\t<beforeArmourDamageMultiplier>%d</beforeArmourDamageMultiplier>\r\n",		AmmoTypes[cnt].beforeArmourDamageMultiplier	);
			FilePrintf(hFile,"\t\t<beforeArmourDamageDivisor>%d</beforeArmourDamageDivisor>\r\n",			AmmoTypes[cnt].beforeArmourDamageDivisor	);
			FilePrintf(hFile,"\t\t<afterArmourDamageMultiplier>%d</afterArmourDamageMultiplier>\r\n",		AmmoTypes[cnt].afterArmourDamageMultiplier	);
			FilePrintf(hFile,"\t\t<afterArmourDamageDivisor>%d</afterArmourDamageDivisor>\r\n",				AmmoTypes[cnt].afterArmourDamageDivisor	);
			FilePrintf(hFile,"\t\t<zeroMinimumDamage>%d</zeroMinimumDamage>\r\n",							AmmoTypes[cnt].zeroMinimumDamage	);
			FilePrintf(hFile,"\t\t<canGoThrough>%d</canGoThrough>\r\n",										AmmoTypes[cnt].canGoThrough	);
			FilePrintf(hFile,"\t\t<standardIssue>%d</standardIssue>\r\n",									AmmoTypes[cnt].standardIssue	);
			FilePrintf(hFile,"\t\t<numberOfBullets>%d</numberOfBullets>\r\n",								AmmoTypes[cnt].numberOfBullets	);
			FilePrintf(hFile,"\t\t<multipleBulletDamageMultiplier>%d</multipleBulletDamageMultiplier>\r\n",	AmmoTypes[cnt].multipleBulletDamageMultiplier	);
			FilePrintf(hFile,"\t\t<multipleBulletDamageDivisor>%d</multipleBulletDamageDivisor>\r\n",		AmmoTypes[cnt].multipleBulletDamageDivisor	);
			FilePrintf(hFile,"\t\t<highExplosive>%d</highExplosive>\r\n",									AmmoTypes[cnt].highExplosive	);
			FilePrintf(hFile,"\t\t<explosionSize>%d</explosionSize>\r\n",									AmmoTypes[cnt].explosionSize	);
			FilePrintf(hFile,"\t\t<antiTank>%d</antiTank>\r\n",												AmmoTypes[cnt].antiTank	);
			FilePrintf(hFile,"\t\t<dart>%d</dart>\r\n",														AmmoTypes[cnt].dart	);
			FilePrintf(hFile,"\t\t<knife>%d</knife>\r\n",													AmmoTypes[cnt].knife	);
			FilePrintf(hFile,"\t\t<monsterSpit>%d</monsterSpit>\r\n",										AmmoTypes[cnt].monsterSpit	);
			FilePrintf(hFile,"\t\t<acidic>%d</acidic>\r\n",													AmmoTypes[cnt].acidic	);
			FilePrintf(hFile,"\t\t<ignoreArmour>%d</ignoreArmour>\r\n",										AmmoTypes[cnt].ignoreArmour	);
			FilePrintf(hFile,"\t\t<lockBustingPower>%d</lockBustingPower>\r\n",								AmmoTypes[cnt].lockBustingPower	);
			FilePrintf(hFile,"\t\t<tracerEffect>%d</tracerEffect>\r\n",										AmmoTypes[cnt].tracerEffect	);
			FilePrintf(hFile,"\t\t<temperatureModificator>%4.2f</temperatureModificator>\r\n",				AmmoTypes[cnt].temperatureModificator	);
			FilePrintf(hFile,"\t\t<PoisonPercentage>%d</PoisonPercentage>\r\n",								AmmoTypes[cnt].poisonPercentage	);
			FilePrintf(hFile,"\t\t<dirtModificator>%4.2f</dirtModificator>\r\n",							AmmoTypes[cnt].dirtModificator	);


			FilePrintf(hFile,"\t</AMMOTYPE>\r\n");
		}
		FilePrintf(hFile,"</AMMOTYPELIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
