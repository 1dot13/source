	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Vehicles.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	NEW_CAR	curNewCar;
	NEW_CAR *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			curSeatIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef newCarParseData;

BOOLEAN NewCar_TextOnly;

void InitNewVehicles ();

static void XMLCALL
newCarStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	newCarParseData * pData = (newCarParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "VEHICLES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;
			pData->maxReadDepth++;
		}
		else if(strcmp(name, "VEHICLE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;
			pData->maxReadDepth++;
		}
		else if(pData->curElement == ELEMENT &&
			  ( strcmp(name, "uiIndex") == 0 ||
			   strcmp(name, "MvtTypes") == 0 ||
			   strcmp(name, "SeatingCapacities") == 0 ||
			   strcmp(name, "EnterVehicleSndID") == 0 ||
			   strcmp(name, "MoveVehicleSndID") == 0 ||
			   strcmp(name, "VehicleTypeProfileID") == 0 ||
			   strcmp(name, "VehicleArmourType") == 0 ||
			   strcmp(name, "VehicleBodyType") == 0 ||
			   strcmp(name, "VehicleEnabled") == 0 ||
			   strcmp(name, "Pilot") == 0 ||
			   strcmp(name, "StiFaceIcon") == 0 ||	   
			   strcmp(name, "Name") == 0 ||
			   strcmp(name, "LongName") == 0 ||
			   strcmp(name, "ShortName") == 0 ||		   
			   strcmp(name, "Neutral") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;
			pData->maxReadDepth++;
		}
		else if(strcmp(name, "SEAT") == 0 && pData->curElement == ELEMENT)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->maxReadDepth++;
		}
		else if(pData->curElement == ELEMENT_SUBLIST &&
				(strcmp(name, "SeatIndex") == 0 ||
				strcmp(name, "SeatName") == 0 ||
				strcmp(name, "Driver") == 0 ||
				strcmp(name, "Hidden") == 0 ||
				strcmp(name, "BlockedView") == 0 ||
				strcmp(name, "BlockedShots") == 0 ||
				strcmp(name, "Rotation") == 0 ||
				strcmp(name, "OffsetX") == 0 ||
				strcmp(name, "OffsetY") == 0 ||
				strcmp(name, "Coverage") == 0 ||
				strcmp(name, "Compartment") == 0 ||
				strcmp(name, "MountedGun") == 0 ))
		{
			pData->curElement = ELEMENT_SUBLIST_PROPERTY;
			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
newCarCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	newCarParseData * pData = (newCarParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && (strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH))
	{
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
newCarEndElementHandle(void *userData, const XML_Char *name)
{
	char temp;

	newCarParseData * pData = (newCarParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "VEHICLES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "VEHICLE") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!NewCar_TextOnly)
			{	
				gNewVehicle[pData->curNewCar.uiIndex].uiIndex = pData->curNewCar.uiIndex;
				gNewVehicle[pData->curNewCar.uiIndex].iNewMvtTypes = pData->curNewCar.iNewMvtTypes;
				gNewVehicle[pData->curNewCar.uiIndex].iNewSeatingCapacities = pData->curNewCar.iNewSeatingCapacities;
				gNewVehicle[pData->curNewCar.uiIndex].iNewEnterVehicleSndID = pData->curNewCar.iNewEnterVehicleSndID;
				gNewVehicle[pData->curNewCar.uiIndex].iNewMoveVehicleSndID = pData->curNewCar.iNewMoveVehicleSndID;
				gNewVehicle[pData->curNewCar.uiIndex].ubNewVehicleTypeProfileID = pData->curNewCar.ubNewVehicleTypeProfileID;
				gNewVehicle[pData->curNewCar.uiIndex].sNewVehicleArmourType = pData->curNewCar.sNewVehicleArmourType;
				gNewVehicle[pData->curNewCar.uiIndex].NewVehicleBodyType = pData->curNewCar.NewVehicleBodyType;	
				gNewVehicle[pData->curNewCar.uiIndex].bNewVehicle = pData->curNewCar.bNewVehicle;
				gNewVehicle[pData->curNewCar.uiIndex].bNewNeutral = pData->curNewCar.bNewNeutral;
				gNewVehicle[pData->curNewCar.uiIndex].NewPilot = pData->curNewCar.NewPilot;	
				strcpy(gNewVehicle[pData->curNewCar.uiIndex].szIconFace, pData->curNewCar.szIconFace);
				gNewVehicle[ pData->curNewCar.uiIndex ].NewCarPortraits = -1;					
				wcscpy(gNewVehicle[pData->curNewCar.uiIndex].NewVehicleStrings, pData->curNewCar.NewVehicleStrings);
				wcscpy(gNewVehicle[pData->curNewCar.uiIndex].NewVehicleName, pData->curNewCar.NewVehicleName);
				wcscpy(gNewVehicle[pData->curNewCar.uiIndex].NewShortVehicleStrings, pData->curNewCar.NewShortVehicleStrings);
				gNewVehicle[ pData->curNewCar.uiIndex ].NewUsed = FALSE;
				memcpy( gNewVehicle[ pData->curNewCar.uiIndex ].VehicleSeats, pData->curNewCar.VehicleSeats, sizeof(pData->curNewCar.VehicleSeats) );

			}
			else
			{
				wcscpy(gNewVehicle[pData->curNewCar.uiIndex].NewVehicleStrings, pData->curNewCar.NewVehicleStrings);
				wcscpy(gNewVehicle[pData->curNewCar.uiIndex].NewVehicleName, pData->curNewCar.NewVehicleName);
				wcscpy(gNewVehicle[pData->curNewCar.uiIndex].NewShortVehicleStrings, pData->curNewCar.NewShortVehicleStrings);
			}
			memset( pData->curNewCar.VehicleSeats, 0, sizeof(pData->curNewCar.VehicleSeats) );
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.uiIndex	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "MvtTypes") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.iNewMvtTypes	= (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "SeatingCapacities") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.iNewSeatingCapacities	= (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "EnterVehicleSndID") == 0)
		{			
			pData->curElement = ELEMENT;
			pData->curNewCar.iNewEnterVehicleSndID	= (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "MoveVehicleSndID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.iNewMoveVehicleSndID	= (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "VehicleArmourType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.sNewVehicleArmourType	= (INT16) atol(pData->szCharData);
		}			
		else if(strcmp(name, "VehicleEnabled") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.bNewVehicle	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "Neutral") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.bNewNeutral	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "Pilot") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curNewCar.NewPilot	= (INT8) atol(pData->szCharData);
		}		
		else if(strcmp(name, "Name") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curNewCar.NewVehicleName, sizeof(pData->curNewCar.NewVehicleName)/sizeof(pData->curNewCar.NewVehicleName[0]) );
			pData->curNewCar.NewVehicleName[sizeof(pData->curNewCar.NewVehicleName)/sizeof(pData->curNewCar.NewVehicleName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "LongName") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curNewCar.NewVehicleStrings, sizeof(pData->curNewCar.NewVehicleStrings)/sizeof(pData->curNewCar.NewVehicleStrings[0]) );
			pData->curNewCar.NewVehicleStrings[sizeof(pData->curNewCar.NewVehicleStrings)/sizeof(pData->curNewCar.NewVehicleStrings[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ShortName") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curNewCar.NewShortVehicleStrings, sizeof(pData->curNewCar.NewShortVehicleStrings)/sizeof(pData->curNewCar.NewShortVehicleStrings[0]) );
			pData->curNewCar.NewShortVehicleStrings[sizeof(pData->curNewCar.NewShortVehicleStrings)/sizeof(pData->curNewCar.NewShortVehicleStrings[0]) - 1] = '\0';
		}
		else if(strcmp(name, "StiFaceIcon") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_MAIN_VEHICLE_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curNewCar.szIconFace,pData->szCharData);
			else
			{
				strncpy(pData->curNewCar.szIconFace,pData->szCharData,MAX_MAIN_VEHICLE_CHARS);
				pData->curNewCar.szIconFace[MAX_MAIN_VEHICLE_CHARS-1] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_MAIN_VEHICLE_CHARS-1);i++)
			{
				temp = pData->szCharData[i];
				pData->curNewCar.szIconFace[i] = temp;
			}
		}
		else if(strcmp(name, "SEAT") == 0 )
		{
			pData->curElement = ELEMENT;

			if (!NewCar_TextOnly)
			{	
				memcpy( gNewVehicle[pData->curNewCar.uiIndex].VehicleSeats, pData->curNewCar.VehicleSeats, sizeof(pData->curNewCar.VehicleSeats) );
			}
			else
			{
				wcscpy( gNewVehicle[pData->curNewCar.uiIndex].VehicleSeats[pData->curSeatIndex].zSeatName, pData->curNewCar.VehicleSeats[pData->curSeatIndex].zSeatName ); 
			}
		}
		else if(strcmp(name, "SeatIndex") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curSeatIndex = (int) atol(pData->szCharData);
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].ubSeatIndex = pData->curSeatIndex;
		}
		else if(strcmp(name, "SeatName") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curNewCar.VehicleSeats[pData->curSeatIndex].zSeatName, sizeof(pData->curNewCar.VehicleSeats[pData->curSeatIndex].zSeatName)/sizeof( pData->curNewCar.VehicleSeats[pData->curSeatIndex].zSeatName[0]) );
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].zSeatName[sizeof(pData->curNewCar.VehicleSeats[pData->curSeatIndex].zSeatName)/sizeof(pData->curNewCar.VehicleSeats[pData->curSeatIndex].zSeatName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "Driver") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].fDriver = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Hidden") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].fHidden = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "BlockedView") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].fBlockedView = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "BlockedShots") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].fBlockedShots = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Rotation") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].ubRotation = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "OffsetX") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].bOffsetX = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "OffsetY") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].bOffsetY = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Coverage") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].ubCoverage = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Compartment") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].ubCompartment = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "MountedGun") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curNewCar.VehicleSeats[pData->curSeatIndex].usMountedGun = (UINT32) atol(pData->szCharData);
		}

		pData->maxReadDepth--;

	}

	pData->currentDepth--;
}

BOOLEAN ReadInNewVehicles(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	newCarParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Vehicles.xml" );

	NewCar_TextOnly = localizedVersion;
	
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

	XML_SetElementHandler(parser, newCarStartElementHandle, newCarEndElementHandle);
	XML_SetCharacterDataHandler(parser, newCarCharacterDataHandle);

	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Vehicles.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}

void InitNewVehicles ()
{
	UINT32 iCount;
	
	for( iCount=0; iCount<NUM_PROFILES; iCount++ )
	{	
		gNewVehicle[ iCount ].uiIndex = iCount;
		gNewVehicle[ iCount ].iNewMvtTypes = 1;
		gNewVehicle[ iCount ].iNewSeatingCapacities = 6;
		gNewVehicle[ iCount ].iNewEnterVehicleSndID = 85;
		gNewVehicle[ iCount ].iNewMoveVehicleSndID  = 82;
		gNewVehicle[ iCount ].ubNewVehicleTypeProfileID = -1;
		gNewVehicle[ iCount ].sNewVehicleArmourType = 164;
		gNewVehicle[ iCount ].NewVehicleBodyType = 0;
		gNewVehicle[ iCount ].bNewNeutral = TRUE;
		gNewVehicle[ iCount ].bNewVehicle = FALSE;
		gNewVehicle[ iCount ].NewPilot = -1;	
		gNewVehicle[ iCount ].NewCarPortraits = -1;
		gNewVehicle[ iCount ].szIconFace[MAX_MAIN_VEHICLE_CHARS-1] = '\0';			
		gNewVehicle[ iCount ].NewVehicleStrings[127] = '\0';
		gNewVehicle[ iCount ].NewVehicleName[127] = '\0';
		gNewVehicle[ iCount ].NewShortVehicleStrings[127] = '\0';	
		gNewVehicle[ iCount ].NewUsed = FALSE;
		memset( &gNewVehicle[ iCount ].VehicleSeats, 0, sizeof(gNewVehicle[ iCount ].VehicleSeats) );
	}
}

