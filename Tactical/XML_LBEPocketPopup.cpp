	#include "sgp.h"
	#include "popup_class.h"
	#include "popup_definition.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "GameSettings.h"

// namespace'd because of name collision with POPUP class def
namespace POPUP_PARSE {
	enum
	{
		OUTSIDE_POCKET_LIST = 0,
		POCKET_LIST,

		POCKET,
		POCKET_PROPERTY,	// for reading pocket id
		
		POPUP,
		POPUP_PROPERTY,	// unused
		// for root-level popup
		OPTION,
		OPTION_PROPERTY,
		GENERATOR,
		GENERATOR_PROPERTY,

		SUBMENU,
		SUBMENU_PROPERTY,
		// for submenus
		SUBMENU_OPTION,
		SUBMENU_OPTION_PROPERTY,
		SUBMENU_GENERATOR,
		SUBMENU_GENERATOR_PROPERTY
	}
	typedef POPUP_PARSE_STAGE;
}


struct
{
	POPUP_PARSE::POPUP_PARSE_STAGE	curElement;
	UINT8		curSubPopupLevel;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	// popupDef and its pocket number
	UINT8			curPocketId;
	popupDef		*curPocketPopup;

	// for reading in options to popups and subPopups
	//popupDefOption	*curPocketPopupOption;	// not really used
	WCHAR			curPocketPopupOptionName[128];
	UINT16			curPocketPopupOptionCallback;
	UINT16			curPocketPopupOptionAvail;

	// for reading in subPopups
	popupDefSubPopupOption *curPocketSubPopupOption[POPUP_MAX_SUB_POPUPS];
	WCHAR			curPocketSubPopupOptionName[POPUP_MAX_SUB_POPUPS][128];

	// for reading in content generator references
	popupDefContentGenerator	*curPocketPopupGenerator;	// not really used
	UINT16			curPocketPopupGeneratorId;

	popupDef *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef pocketPopupParseData;

// maps generator name strings found in XML to generator IDs used by the function that binds them
static UINT16 mapGeneratorNameToId( CHAR8 * name ){

	if( strcmp(name,"dummy") == 0 ){
		return popupGenerators::dummy;
	} else if( strcmp(name,"addArmor") == 0 ){
		return popupGenerators::addArmor;
	} else if( strcmp(name,"addLBE") == 0 ){
		return popupGenerators::addLBE;
	} else if( strcmp(name,"addWeapons") == 0 ){
		return popupGenerators::addWeapons;
	} else if( strcmp(name,"addWeaponGroups") == 0 ){
		return popupGenerators::addWeaponGroups;
	} else if( strcmp(name,"addGrenades") == 0 ){
		return popupGenerators::addGrenades;
	} else if( strcmp(name,"addBombs") == 0 ){
		return popupGenerators::addBombs;
	} else if( strcmp(name,"addFaceGear") == 0 ){
		return popupGenerators::addFaceGear;
	} else if( strcmp(name,"addAmmo") == 0 ){
		return popupGenerators::addAmmo;
	} else if( strcmp(name,"addRifleGrenades") == 0 ){
		return popupGenerators::addRifleGrenades;
	} else if( strcmp(name,"addRocketAmmo") == 0 ){
		return popupGenerators::addRocketAmmo;
	} else if( strcmp(name,"addMisc") == 0 ){
		return popupGenerators::addMisc;
	} else if( strcmp(name,"addKits") == 0 ){
		return popupGenerators::addKits;
	} else return 0; // includes 'none'

}

// maps option callback name strings found in XML to callback IDs used by the function that binds them
static UINT16 mapCallbackNameToId( CHAR8 * name ){
	
	if( strcmp(name,"dummy") == 0 ){
		return 1;
	}  else return 0; // includes 'none'

}

// maps option availability check function name strings found in XML to their IDs used by the function that binds them
static UINT16 mapAvailNameToId( CHAR8 * name ){
	
	if( strcmp(name,"dummy") == 0 ){
		return 1;
	}  else return 0; // includes 'none'

}

static void XMLCALL
pocketPopupStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	pocketPopupParseData * pData = (pocketPopupParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "POCKETPOPUPS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = POPUP_PARSE::POCKET_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "POCKET") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = POPUP_PARSE::POCKET;

			pData->maxReadDepth++; //we are not skipping this element
			// TODO: assert if there's no dangling subpopup here, broken XML might cause trouble
			pData->curSubPopupLevel = 0; // we're in a new pocket now, so reset the subpopup level
		}
		else if(pData->curElement == POPUP_PARSE::POCKET &&
			   (strcmp(name, "pIndex") == 0 ))
		{
			pData->curElement = POPUP_PARSE::POCKET_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::POCKET &&
			   (strcmp(name, "popup") == 0 ))
		{
			pData->curElement = POPUP_PARSE::POPUP;

			pData->maxReadDepth++; //we are not skipping this element
			pData->curPocketPopup = new popupDef();
		}
		else if(pData->curElement == POPUP_PARSE::POPUP &&		// popup options (not in submenu)
			   (strcmp(name, "option") == 0 ))
		{
			pData->curElement = POPUP_PARSE::OPTION;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::OPTION &&		// popup option attributes (not in submenu)
			   (  strcmp(name, "name") == 0 
			   || strcmp(name, "action") == 0  
			   || strcmp(name, "availCheck") == 0 ))
		{
			pData->curElement = POPUP_PARSE::OPTION_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::POPUP &&		// content generators (not in submenu)
			   (strcmp(name, "generator") == 0 ))
		{
			pData->curElement = POPUP_PARSE::GENERATOR;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::GENERATOR &&		// content generator attributes (not in submenu)
			   (  strcmp(name, "id") == 0 ) )
		{
			pData->curElement = POPUP_PARSE::GENERATOR_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::POPUP &&		// submenu (first)
			   (strcmp(name, "subMenu") == 0 ))
		{
			pData->curElement = POPUP_PARSE::SUBMENU;

			// TODO: assert that this is the first submenu
			pData->curSubPopupLevel = 1; // we're still at popup level so this must be the first subpopup
			pData->curPocketSubPopupOption[ pData->curSubPopupLevel-1 ] = new popupDefSubPopupOption();

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::SUBMENU &&	// submenu (deep)
			   (strcmp(name, "subMenu") == 0 ))
		{
			pData->curElement = POPUP_PARSE::SUBMENU;

			pData->curSubPopupLevel++;
			pData->curPocketSubPopupOption[ pData->curSubPopupLevel-1 ] = new popupDefSubPopupOption();

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::SUBMENU &&		// submenu attributes
			   (  strcmp(name, "name") == 0 ) )
		{
			pData->curElement = POPUP_PARSE::SUBMENU_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::SUBMENU &&		// popup options (submenu)
			   (strcmp(name, "option") == 0 ))
		{
			pData->curElement = POPUP_PARSE::SUBMENU_OPTION;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::SUBMENU_OPTION &&		// popup option attributes (submenu)
			   (  strcmp(name, "name") == 0 
			   || strcmp(name, "action") == 0  
			   || strcmp(name, "availCheck") == 0 ))
		{
			pData->curElement = POPUP_PARSE::SUBMENU_OPTION_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::SUBMENU &&		// content generators (submenu)
			   (strcmp(name, "generator") == 0 ))
		{
			pData->curElement = POPUP_PARSE::SUBMENU_GENERATOR;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == POPUP_PARSE::SUBMENU_GENERATOR &&		// content generator attributes (submenu)
			   (  strcmp(name, "id") == 0 ) )
		{
			pData->curElement = POPUP_PARSE::SUBMENU_GENERATOR_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}


		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
pocketPopupCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	pocketPopupParseData * pData = (pocketPopupParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
pocketPopupEndElementHandle(void *userData, const XML_Char *name)
{
	pocketPopupParseData * pData = (pocketPopupParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "POCKETPOPUPS") == 0)
		{
			pData->curElement = POPUP_PARSE::OUTSIDE_POCKET_LIST;
		}
		else if(strcmp(name, "POCKET") == 0)
		{
			pData->curElement = POPUP_PARSE::POCKET_LIST;	
			
			// done with the pocket
			// nothing to do because we already saved the popup when closing <popup> tag
		
		}
		else if(strcmp(name, "pIndex") == 0)
		{
			pData->curElement = POPUP_PARSE::POCKET;
			pData->curPocketId	= (UINT8) atol(pData->szCharData);	// got pocket index
		}
		else if(strcmp(name, "popup") == 0)
		{
			pData->curElement = POPUP_PARSE::POCKET;
			// done with the popup definition
			LBEPocketPopup[ pData->curPocketId ] = *pData->curPocketPopup;	// place the popup definition in pocket popup index
		}
		else if(strcmp(name, "subMenu") == 0)
		{
			
			// done with the subpopup definition

			// rename the current option, we should've collected a name for it by now
			pData->curPocketSubPopupOption[ pData->curSubPopupLevel-1 ]->rename( new std::wstring( pData->curPocketSubPopupOptionName[ pData->curSubPopupLevel-1 ] ) );

			if( pData->curSubPopupLevel == 1 ){	// at first submenu level, add the current menu to the base popup
				pData->curElement = POPUP_PARSE::POPUP;

				pData->curPocketPopup->addSubPopup( pData->curPocketSubPopupOption[ pData->curSubPopupLevel-1 ] );
				pData->curSubPopupLevel = 0;
			} else {	// deep in submenu tree, add the current submenu to the one above
				pData->curElement = POPUP_PARSE::SUBMENU;

				pData->curPocketSubPopupOption[ pData->curSubPopupLevel - 2 ]->getSubDef()->addSubPopup( pData->curPocketSubPopupOption[ pData->curSubPopupLevel-1 ] );
				pData->curSubPopupLevel--;
			}

		}
		else if( pData->curElement == POPUP_PARSE::OPTION && strcmp(name, "option") == 0)	// option (popup)
		{
			pData->curElement = POPUP_PARSE::POPUP;
			// done with the option
			pData->curPocketPopup->addOption( new std::wstring( pData->curPocketPopupOptionName ), pData->curPocketPopupOptionCallback, pData->curPocketPopupOptionAvail );
		}
		else if( pData->curElement == POPUP_PARSE::SUBMENU_OPTION && strcmp(name, "option") == 0)	// option (sub-popup)
		{
			pData->curElement = POPUP_PARSE::SUBMENU;
			// done with the option
			pData->curPocketSubPopupOption[ pData->curSubPopupLevel-1 ]->getSubDef()->addOption( new std::wstring( pData->curPocketPopupOptionName ), pData->curPocketPopupOptionCallback, pData->curPocketPopupOptionAvail );
		}
		else if( pData->curElement == POPUP_PARSE::GENERATOR && strcmp(name, "generator") == 0)	// generator (popup)
		{
			pData->curElement = POPUP_PARSE::POPUP;
			// done with the generator
			pData->curPocketPopup->addGenerator( pData->curPocketPopupGeneratorId );
		}
		else if( pData->curElement == POPUP_PARSE::SUBMENU_GENERATOR && strcmp(name, "generator") == 0)	// generator (sub-popup)
		{
			pData->curElement = POPUP_PARSE::SUBMENU;
			// done with the generator
			pData->curPocketSubPopupOption[ pData->curSubPopupLevel-1 ]->getSubDef()->addGenerator( pData->curPocketPopupGeneratorId );
		}
		else if(strcmp(name, "name") == 0)
		{
			switch( pData->curElement ){
			case POPUP_PARSE::OPTION_PROPERTY:
				pData->curElement = POPUP_PARSE::OPTION;

				MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curPocketPopupOptionName, sizeof(pData->curPocketPopupOptionName)/sizeof(pData->curPocketPopupOptionName[0]) );
				pData->curPocketPopupOptionName[sizeof(pData->curPocketPopupOptionName)/sizeof(pData->curPocketPopupOptionName[0]) - 1] = '\0';

				break;

			case POPUP_PARSE::SUBMENU_OPTION_PROPERTY:
				pData->curElement = POPUP_PARSE::SUBMENU_OPTION;

				MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curPocketPopupOptionName, sizeof(pData->curPocketPopupOptionName)/sizeof(pData->curPocketPopupOptionName[0]) );
				pData->curPocketPopupOptionName[sizeof(pData->curPocketPopupOptionName)/sizeof(pData->curPocketPopupOptionName[0]) - 1] = '\0';

				break;

			case POPUP_PARSE::SUBMENU_PROPERTY:
				pData->curElement = POPUP_PARSE::SUBMENU;

				MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curPocketSubPopupOptionName[pData->curSubPopupLevel-1], sizeof(pData->curPocketSubPopupOptionName[pData->curSubPopupLevel-1])/sizeof(pData->curPocketSubPopupOptionName[pData->curSubPopupLevel-1][0]) );
				pData->curPocketSubPopupOptionName[pData->curSubPopupLevel-1][sizeof(pData->curPocketSubPopupOptionName[pData->curSubPopupLevel-1])/sizeof(pData->curPocketSubPopupOptionName[pData->curSubPopupLevel-1][0]) - 1] = '\0';

				break;

			}
		}
		else if(strcmp(name, "action") == 0)
		{
			switch( pData->curElement ){
				case POPUP_PARSE::OPTION_PROPERTY:
					pData->curElement = POPUP_PARSE::OPTION;
					break;

				case POPUP_PARSE::SUBMENU_OPTION_PROPERTY:
					pData->curElement = POPUP_PARSE::SUBMENU_OPTION;
					break;
			}

			pData->curPocketPopupOptionCallback	= mapCallbackNameToId(pData->szCharData);
		}
		else if(strcmp(name, "availCheck") == 0)
		{
			switch( pData->curElement ){
				case POPUP_PARSE::OPTION_PROPERTY:
					pData->curElement = POPUP_PARSE::OPTION;
					break;

				case POPUP_PARSE::SUBMENU_OPTION_PROPERTY:
					pData->curElement = POPUP_PARSE::SUBMENU_OPTION;
					break;
			}

			pData->curPocketPopupOptionAvail	= mapAvailNameToId(pData->szCharData);
		}
		else if(strcmp(name, "id") == 0)
		{
			switch( pData->curElement ){
				case POPUP_PARSE::GENERATOR_PROPERTY:
					pData->curElement = POPUP_PARSE::GENERATOR;
					break;

				case POPUP_PARSE::SUBMENU_GENERATOR_PROPERTY:
					pData->curElement = POPUP_PARSE::SUBMENU_GENERATOR;
					break;
			}

			pData->curPocketPopupGeneratorId	= mapGeneratorNameToId(pData->szCharData);
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}


BOOLEAN ReadInLBEPocketPopups(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	pocketPopupParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading pocketPopups.xml" );

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

	
	XML_SetElementHandler(parser, pocketPopupStartElementHandle, pocketPopupEndElementHandle);
	XML_SetCharacterDataHandler(parser, pocketPopupCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);
	
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Pocket.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	/*
	// dummy popup

	 popupDef* popup = new popupDef();
	 popup->addOption(new std::wstring(L"Option one"),NULL,NULL);
	 popup->addOption(new std::wstring(L"Option two"),NULL,NULL);
	 popup->addOption(new std::wstring(L"Option three"),NULL,NULL);

	LBEPocketPopup[5] = *popup;
	*/

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}