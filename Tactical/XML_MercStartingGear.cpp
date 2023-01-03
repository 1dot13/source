	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Soldier Profile.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8				szCharData[MAX_CHAR_DATA_LENGTH+1];
	MERCPROFILEGEAR		curMercStartingGear;
	MERCPROFILEGEAR		(*curArray)[NUM_MERCSTARTINGGEAR_KITS];
	UINT8				curIndex; // Holds the number of the current profile for whom we're reading gear.
	UINT8				curGears; // Holds the number of gears read so far for one index number.
	UINT32				maxArraySize;
	
	UINT32				currentDepth;
	UINT32				maxReadDepth;
}
typedef MercStartingGearParseData;

BOOLEAN localizedTextOnly_MercStartingGear;

static void XMLCALL 
MercStartingGearStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	MercStartingGearParseData * pData = (MercStartingGearParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MERCGEARLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			//pData->curArray = &(MERCPROFILEGEAR());
			//memset(pData->curArray,0,sizeof(MERCPROFILEGEAR)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element

			pData->curIndex = 0;
		}
		else if(strcmp(name, "MERCGEAR") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->curMercStartingGear = MERCPROFILEGEAR();
			//memset(&pData->curMercStartingGear,0,sizeof(MERCPROFILEGEAR));

			pData->maxReadDepth++; //we are not skipping this element

			pData->curGears = 0;
		}
		else if(pData->curElement == ELEMENT &&
			(strcmp(name, "mIndex") == 0 ||
			strcmp(name, "mName") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++;
		}
		else if(strcmp(name, "GEARKIT") == 0 && pData->curElement == ELEMENT)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->maxReadDepth++;
			//tais: absolute price needs to be set to -1 at start of every gearkit, if there's no tag for it the value will stay -1 and will be ignored in the code
			pData->curMercStartingGear.AbsolutePrice = -1;
		}
		else if(pData->curElement == ELEMENT_SUBLIST &&
				(strcmp(name, "mAbsolutePrice") == 0 ||
				strcmp(name, "mGearKitName") == 0 ||
				strcmp(name, "mPriceMod") == 0 ||
				strcmp(name, "mHelmet") == 0 ||
				strcmp(name, "mHelmetStatus") == 0 ||
				strcmp(name, "mHelmetDrop") == 0 ||
				strcmp(name, "mVest") == 0 ||
				strcmp(name, "mVestStatus") == 0 ||
				strcmp(name, "mVestDrop") == 0 ||
				strcmp(name, "mLeg") == 0 ||
				strcmp(name, "mLegStatus") == 0 ||
				strcmp(name, "mLegDrop") == 0 ||
				strcmp(name, "mWeapon") == 0 ||
				strcmp(name, "mWeaponStatus") == 0 ||
				strcmp(name, "mWeaponDrop") == 0 ||
				strcmp(name, "mBig0") == 0 ||
				strcmp(name, "mBig0Quantity") == 0 ||
				strcmp(name, "mBig0Status") == 0 ||
				strcmp(name, "mBig0Drop") == 0 ||
				strcmp(name, "mBig1") == 0 ||
				strcmp(name, "mBig1Quantity") == 0 ||
				strcmp(name, "mBig1Status") == 0 ||
				strcmp(name, "mBig1Drop") == 0 ||
				strcmp(name, "mBig2") == 0 ||
				strcmp(name, "mBig2Quantity") == 0 ||
				strcmp(name, "mBig2Status") == 0 ||
				strcmp(name, "mBig2Drop") == 0 ||
				strcmp(name, "mBig3") == 0 ||
				strcmp(name, "mBig3Quantity") == 0 ||
				strcmp(name, "mBig3Status") == 0 ||
				strcmp(name, "mBig3Drop") == 0 ||
				strcmp(name, "mSmall0") == 0 ||
				strcmp(name, "mSmall0Quantity") == 0 ||
				strcmp(name, "mSmall0Status") == 0 ||
				strcmp(name, "mSmall1") == 0 ||
				strcmp(name, "mSmall1Quantity") == 0 ||
				strcmp(name, "mSmall1Status") == 0 ||
				strcmp(name, "mSmall2") == 0 ||
				strcmp(name, "mSmall2Quantity") == 0 ||
				strcmp(name, "mSmall2Status") == 0 ||
				strcmp(name, "mSmall3") == 0 ||
				strcmp(name, "mSmall3Quantity") == 0 ||
				strcmp(name, "mSmall3Status") == 0 ||
				strcmp(name, "mSmall4") == 0 ||
				strcmp(name, "mSmall4Quantity") == 0 ||
				strcmp(name, "mSmall4Status") == 0 ||
				strcmp(name, "mSmall5") == 0 ||
				strcmp(name, "mSmall5Quantity") == 0 ||
				strcmp(name, "mSmall5Status") == 0 ||
				strcmp(name, "mSmall6") == 0 ||
				strcmp(name, "mSmall6Quantity") == 0 ||
				strcmp(name, "mSmall6Status") == 0 ||
				strcmp(name, "mSmall7") == 0 ||
				strcmp(name, "mSmall7Quantity") == 0 ||
				strcmp(name, "mSmall7Status") == 0 ||
				strcmp(name, "lVest") == 0 ||
				strcmp(name, "lVestStatus") == 0 ||
				strcmp(name, "lLeftThigh") == 0 ||
				strcmp(name, "lLeftThighStatus") == 0 ||
				strcmp(name, "lRightThigh") == 0 ||
				strcmp(name, "lRightThighStatus") == 0 ||
				strcmp(name, "lCPack") == 0 ||
				strcmp(name, "lCPackStatus") == 0 ||
				strcmp(name, "lBPack") == 0 ||
				strcmp(name, "lBPackStatus") == 0 ))
		{
			pData->curElement = ELEMENT_SUBLIST_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[HELMETPOS] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
MercStartingGearCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	MercStartingGearParseData * pData = (MercStartingGearParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && (strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH))
	{
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
MercStartingGearEndElementHandle(void *userData, const XML_Char *name)
{
	MercStartingGearParseData * pData = (MercStartingGearParseData *)userData;
	char temp;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "MERCGEARLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "MERCGEAR") == 0)
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "mIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.mIndex   = (UINT16) atol(pData->szCharData);
			pData->curIndex = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "mName") == 0)
		{
			pData->curElement = ELEMENT;
			if (strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH ) {
				strncpy(pData->curMercStartingGear.mName, pData->szCharData, mNameLength);
			}
			else
			{
				strncpy(pData->curMercStartingGear.mName,pData->szCharData, mNameLength);
				pData->curMercStartingGear.mName[mNameLength-1] = '\0';
			}

			for(int i=0;i<min( (int)strlen(pData->szCharData), mNameLength);i++)
			{
				temp = pData->szCharData[i];
				pData->curMercStartingGear.mName[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: itemname[%d] = %s, temp = %s",i,&pData->curItem.szItemName[i],&temp));
			}
		}
		else if(strcmp(name, "GEARKIT") == 0 )
		{
			// Write the gear into memory.
			if(pData->curIndex < pData->maxArraySize && pData->curGears < NUM_MERCSTARTINGGEAR_KITS)
			{
				if (!localizedTextOnly_MercStartingGear)
				{
					pData->curArray[pData->curIndex][pData->curGears] = pData->curMercStartingGear; //write the armour into the table

					//CHRISL: after writing the gearkit, we need to clear gear data so it won't inadvertantly be reused
					pData->curMercStartingGear.PriceModifier = 0;
					pData->curMercStartingGear.AbsolutePrice = -1;
					pData->curMercStartingGear.mGearKitName[0] = '\0';
					UINT32 invsize = pData->curMercStartingGear.inv.size();
					for(UINT32 i = 0; i < invsize; ++i)
					{
						pData->curMercStartingGear.inv[i] = 0;
						pData->curMercStartingGear.iStatus[i] = 0;
						pData->curMercStartingGear.iDrop[i] = 0;
						pData->curMercStartingGear.iNumber[i] = 0;
					}
					UINT32 lbesize = pData->curMercStartingGear.lbe.size();
					for(UINT32 i = 0; i < lbesize; ++i)
					{
						pData->curMercStartingGear.lbe[i] = 0;
						pData->curMercStartingGear.lStatus[i] = 0;
					}
				}
				else
				{
					wcscpy(gMercProfileGear[pData->curIndex][pData->curGears].mGearKitName, pData->curMercStartingGear.mGearKitName);
				}
			}

			pData->curElement = ELEMENT;
			// Increase the number of gears read.
			pData->curGears++;
		}
		else if(strcmp(name, "mPriceMod") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.PriceModifier = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mAbsolutePrice") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.AbsolutePrice = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mGearKitName") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curMercStartingGear.mGearKitName, sizeof(pData->curMercStartingGear.mGearKitName)/sizeof(pData->curMercStartingGear.mGearKitName[0]) );
			pData->curMercStartingGear.mGearKitName[sizeof(pData->curMercStartingGear.mGearKitName)/sizeof(pData->curMercStartingGear.mGearKitName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "mHelmet") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[HELMETPOS]  = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mVest") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[VESTPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mLeg") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[LEGPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mWeapon") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[HANDPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[BIGPOCK1POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[BIGPOCK2POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[BIGPOCK3POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[BIGPOCK4POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall0") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK1POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall1") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK2POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall2") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK3POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall3") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK4POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall4") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK5POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall5") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK6POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall6") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK7POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall7") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.inv[SMALLPOCK8POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mHelmetStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[HELMETPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mVestStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[VESTPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mLegStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[LEGPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mWeaponStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[HANDPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[BIGPOCK1POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[BIGPOCK2POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[BIGPOCK3POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[BIGPOCK4POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall0Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK1POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall1Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK2POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall2Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK3POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall3Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK4POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall4Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK5POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall5Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK6POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall6Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK7POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall7Status") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iStatus[SMALLPOCK8POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mHelmetDrop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[HELMETPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mVestDrop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[VESTPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mLegDrop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[LEGPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mWeaponDrop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[HANDPOS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0Drop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[BIGPOCK1POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1Drop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[BIGPOCK2POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2Drop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[BIGPOCK3POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3Drop") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iDrop[BIGPOCK4POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[BIGPOCK1POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[BIGPOCK2POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[BIGPOCK3POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[BIGPOCK4POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall0Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK1POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall1Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK2POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall2Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK3POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall3Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK4POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall4Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK5POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall5Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK6POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall6Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK7POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall7Quantity") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.iNumber[SMALLPOCK8POS] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lVest") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lbe[0] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lLeftThigh") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lbe[1] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lRightThigh") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lbe[2] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lCPack") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lbe[3] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lBPack") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lbe[4] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lVestStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lStatus[0] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lLeftThighStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lStatus[1] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lRightThighStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lStatus[2] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lCPackStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lStatus[3] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lBPackStatus") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curMercStartingGear.lStatus[4] = (int) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInMercStartingGearStats(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	MercStartingGearParseData pData;

	localizedTextOnly_MercStartingGear = localizedVersion;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading MercStartingGear.xml" );

	// Open gMercProfileGear file
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

	
	XML_SetElementHandler(parser, MercStartingGearStartElementHandle, MercStartingGearEndElementHandle);
	XML_SetCharacterDataHandler(parser, MercStartingGearCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));

	pData.curArray = gMercProfileGear;

	pData.maxArraySize = NUM_PROFILES; 
	
	XML_SetUserData(parser, &pData);

	if (!localizedTextOnly_MercStartingGear)
	{
		for(int i=0; i<NUM_PROFILES; ++i)
		{
			for(int i2=0; i2<NUM_MERCSTARTINGGEAR_KITS; ++i2)
			{
				gMercProfileGear[i][i2].clearInventory();
			}
		}
	}

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in MercStartingGear.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteMercStartingGearStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writemercstartinggearstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\MercStartingGear out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt, gearkit;
		CHAR16 str[100];

		FilePrintf(hFile, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
		FilePrintf(hFile,"<MERCGEARLIST>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES;cnt++)
		{
			FilePrintf(hFile,"\t<MERCGEAR>\r\n");
			FilePrintf(hFile,"\t\t<mIndex>%d</mIndex>\r\n",								cnt);
			FilePrintf(hFile,"\t\t<mName>%s</mName>\r\n",								gMercProfileGear[cnt][0].mName);
			for(gearkit = 0; gearkit < NUM_MERCSTARTINGGEAR_KITS; gearkit++)
			{
				if (gearkit > 0 && gMercProfileGear[cnt][gearkit].mGearKitName[0] == '\0')
				{
					FilePrintf(hFile, "\t\t<GEARKIT>\r\n");
					FilePrintf(hFile, "\t\t\t<mAbsolutePrice>%d</mAbsolutePrice>\r\n",	gMercProfileGear[cnt][gearkit].AbsolutePrice);
					FilePrintf(hFile, "\t\t</GEARKIT>\r\n");
					continue;
				}
				FilePrintf(hFile, "\t\t<GEARKIT>\r\n");
				FilePrintf(hFile, "\t\t\t<mPriceMod>%d</mPriceMod>\r\n",				gMercProfileGear[cnt][gearkit].PriceModifier);
				FilePrintf(hFile, "\t\t\t<mGearKitName>%s</mGearKitName>\r\n",			gMercProfileGear[cnt][gearkit].mGearKitName);
				FilePrintf(hFile, "\t\t\t<mAbsolutePrice>%d</mAbsolutePrice>\r\n",		gMercProfileGear[cnt][gearkit].AbsolutePrice);
				FilePrintf(hFile, "\t\t\t<mHelmet>%d</mHelmet>\r\n",					gMercProfileGear[cnt][gearkit].inv[HELMETPOS]);
				FilePrintf(hFile, "\t\t\t<mHelmetDrop>%d</mHelmetDrop>\r\n",			gMercProfileGear[cnt][gearkit].iDrop[HELMETPOS]);
				FilePrintf(hFile, "\t\t\t<mHelmetStatus>%d</mHelmetStatus>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[HELMETPOS]);
				FilePrintf(hFile, "\t\t\t<mVest>%d</mVest>\r\n",						gMercProfileGear[cnt][gearkit].inv[VESTPOS]);
				FilePrintf(hFile, "\t\t\t<mVestDrop>%d</mVestDrop>\r\n",				gMercProfileGear[cnt][gearkit].iDrop[VESTPOS]);
				FilePrintf(hFile, "\t\t\t<mVestStatus>%d</mVestStatus>\r\n",			gMercProfileGear[cnt][gearkit].iStatus[VESTPOS]);
				FilePrintf(hFile, "\t\t\t<mLeg>%d</mLeg>\r\n",							gMercProfileGear[cnt][gearkit].inv[LEGPOS]);
				FilePrintf(hFile, "\t\t\t<mLegDrop>%d</mLegDrop>\r\n",					gMercProfileGear[cnt][gearkit].iDrop[LEGPOS]);
				FilePrintf(hFile, "\t\t\t<mLegStatus>%d</mLegStatus>\r\n",				gMercProfileGear[cnt][gearkit].iStatus[LEGPOS]);
				FilePrintf(hFile, "\t\t\t<mWeapon>%d</mWeapon>\r\n",					gMercProfileGear[cnt][gearkit].inv[HANDPOS]);
				FilePrintf(hFile, "\t\t\t<mWeaponDrop>%d</mWeaponDrop>\r\n",			gMercProfileGear[cnt][gearkit].iDrop[HANDPOS]);				
				FilePrintf(hFile, "\t\t\t<mWeaponStatus>%d</mWeaponStatus>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[HANDPOS]);
				FilePrintf(hFile, "\t\t\t<mBig0>%d</mBig0>\r\n",						gMercProfileGear[cnt][gearkit].inv[BIGPOCK1POS]);
				FilePrintf(hFile, "\t\t\t<mBig0Status>%d</mBig0Status>\r\n",			gMercProfileGear[cnt][gearkit].iStatus[BIGPOCK1POS]);
				FilePrintf(hFile, "\t\t\t<mBig0Quantity>%d</mBig0Quantity>\r\n",		gMercProfileGear[cnt][gearkit].iNumber[BIGPOCK1POS]);
				FilePrintf(hFile, "\t\t\t<mBig0Drop>%d</mBig0Drop>\r\n",				gMercProfileGear[cnt][gearkit].iDrop[BIGPOCK1POS]);
				FilePrintf(hFile, "\t\t\t<mBig1>%d</mBig1>\r\n",						gMercProfileGear[cnt][gearkit].inv[BIGPOCK2POS]);				
				FilePrintf(hFile, "\t\t\t<mBig1Status>%d</mBig1Status>\r\n",			gMercProfileGear[cnt][gearkit].iStatus[BIGPOCK2POS]);
				FilePrintf(hFile, "\t\t\t<mBig1Quantity>%d</mBig1Quantity>\r\n",		gMercProfileGear[cnt][gearkit].iNumber[BIGPOCK2POS]);
				FilePrintf(hFile, "\t\t\t<mBig1Drop>%d</mBig1Drop>\r\n",				gMercProfileGear[cnt][gearkit].iDrop[BIGPOCK2POS]);
				FilePrintf(hFile, "\t\t\t<mBig2>%d</mBig2>\r\n",						gMercProfileGear[cnt][gearkit].inv[BIGPOCK3POS]);
				FilePrintf(hFile, "\t\t\t<mBig2Status>%d</mBig2Status>\r\n",			gMercProfileGear[cnt][gearkit].iStatus[BIGPOCK3POS]);
				FilePrintf(hFile, "\t\t\t<mBig2Quantity>%d</mBig2Quantity>\r\n",		gMercProfileGear[cnt][gearkit].iNumber[BIGPOCK3POS]);
				FilePrintf(hFile, "\t\t\t<mBig2Drop>%d</mBig2Drop>\r\n",				gMercProfileGear[cnt][gearkit].iDrop[BIGPOCK3POS]);
				FilePrintf(hFile, "\t\t\t<mBig3>%d</mBig3>\r\n",						gMercProfileGear[cnt][gearkit].inv[BIGPOCK4POS]);
				FilePrintf(hFile, "\t\t\t<mBig3Status>%d</mBig3Status>\r\n",			gMercProfileGear[cnt][gearkit].iStatus[BIGPOCK4POS]);
				FilePrintf(hFile, "\t\t\t<mBig3Quantity>%d</mBig3Quantity>\r\n",		gMercProfileGear[cnt][gearkit].iNumber[BIGPOCK4POS]);
				FilePrintf(hFile, "\t\t\t<mBig3Drop>%d</mBig3Drop>\r\n",				gMercProfileGear[cnt][gearkit].iDrop[BIGPOCK4POS]);
				FilePrintf(hFile, "\t\t\t<mSmall0>%d</mSmall0>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK1POS]);
				FilePrintf(hFile, "\t\t\t<mSmall0Status>%d</mSmall0Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK1POS]);
				FilePrintf(hFile, "\t\t\t<mSmall0Quantity>%d</mSmall0Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK1POS]);				
				FilePrintf(hFile, "\t\t\t<mSmall1>%d</mSmall1>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK2POS]);
				FilePrintf(hFile, "\t\t\t<mSmall1Status>%d</mSmall1Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK2POS]);
				FilePrintf(hFile, "\t\t\t<mSmall1Quantity>%d</mSmall1Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK2POS]);
				FilePrintf(hFile, "\t\t\t<mSmall2>%d</mSmall2>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK3POS]);
				FilePrintf(hFile, "\t\t\t<mSmall2Status>%d</mSmall2Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK3POS]);
				FilePrintf(hFile, "\t\t\t<mSmall2Quantity>%d</mSmall2Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK3POS]);				
				FilePrintf(hFile, "\t\t\t<mSmall3>%d</mSmall3>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK4POS]);
				FilePrintf(hFile, "\t\t\t<mSmall3Status>%d</mSmall3Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK4POS]);
				FilePrintf(hFile, "\t\t\t<mSmall3Quantity>%d</mSmall3Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK4POS]);				
				FilePrintf(hFile, "\t\t\t<mSmall4>%d</mSmall4>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK5POS]);
				FilePrintf(hFile, "\t\t\t<mSmall4Status>%d</mSmall4Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK5POS]);
				FilePrintf(hFile, "\t\t\t<mSmall4Quantity>%d</mSmall4Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK5POS]);				
				FilePrintf(hFile, "\t\t\t<mSmall5>%d</mSmall5>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK6POS]);
				FilePrintf(hFile, "\t\t\t<mSmall5Status>%d</mSmall5Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK6POS]);
				FilePrintf(hFile, "\t\t\t<mSmall5Quantity>%d</mSmall5Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK6POS]);				
				FilePrintf(hFile, "\t\t\t<mSmall6>%d</mSmall6>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK7POS]);
				FilePrintf(hFile, "\t\t\t<mSmall6Status>%d</mSmall6Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK7POS]);
				FilePrintf(hFile, "\t\t\t<mSmall6Quantity>%d</mSmall6Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK7POS]);
				FilePrintf(hFile, "\t\t\t<mSmall7>%d</mSmall7>\r\n",					gMercProfileGear[cnt][gearkit].inv[SMALLPOCK8POS]);
				FilePrintf(hFile, "\t\t\t<mSmall7Status>%d</mSmall7Status>\r\n",		gMercProfileGear[cnt][gearkit].iStatus[SMALLPOCK8POS]);
				FilePrintf(hFile, "\t\t\t<mSmall7Quantity>%d</mSmall7Quantity>\r\n",	gMercProfileGear[cnt][gearkit].iNumber[SMALLPOCK8POS]);
				FilePrintf(hFile, "\t\t\t<lVest>%d</lVest>\r\n",						gMercProfileGear[cnt][gearkit].lbe[0]);
				FilePrintf(hFile, "\t\t\t<lVestStatus>%d</lVestStatus>\r\n",			gMercProfileGear[cnt][gearkit].lStatus[0]);
				FilePrintf(hFile, "\t\t\t<lLeftThigh>%d</lLeftThigh>\r\n",				gMercProfileGear[cnt][gearkit].lbe[1]);
				FilePrintf(hFile, "\t\t\t<lLeftThighStatus>%d</lLeftThighStatus>\r\n",	gMercProfileGear[cnt][gearkit].lStatus[1]);
				FilePrintf(hFile, "\t\t\t<lRightThigh>%d</lRightThigh>\r\n",			gMercProfileGear[cnt][gearkit].lbe[2]);
				FilePrintf(hFile, "\t\t\t<lRightThighStatus>%d</lRightThighStatus>\r\n",gMercProfileGear[cnt][gearkit].lStatus[2]);
				FilePrintf(hFile, "\t\t\t<lCPack>%d</lCPack>\r\n",						gMercProfileGear[cnt][gearkit].lbe[3]);
				FilePrintf(hFile, "\t\t\t<lCPackStatus>%d</lCPackStatus>\r\n",			gMercProfileGear[cnt][gearkit].lStatus[3]);
				FilePrintf(hFile, "\t\t\t<lBPack>%d</lBPack>\r\n",						gMercProfileGear[cnt][gearkit].lbe[4]);
				FilePrintf(hFile, "\t\t\t<lBPackStatus>%d</lBPackStatus>\r\n",			gMercProfileGear[cnt][gearkit].lStatus[4]);
				FilePrintf(hFile, "\t\t</GEARKIT>\r\n");
			}			
			FilePrintf(hFile,"\t</MERCGEAR>\r\n");
		}
		FilePrintf(hFile,"</MERCGEARLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}