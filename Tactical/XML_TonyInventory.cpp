#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
    #include "Arms Dealer Init.h"
    #include "Font Control.h"
	#include "WordWrap.h"
	#include "soldier profile type.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	DEALER_POSSIBLE_INV		curInventory;
	DEALER_POSSIBLE_INV *	curArray;
	int				armsDealerIndex;
	unsigned		shopKeeperID;
	float			buyCostModifier;
	float			sellCostModifier;
	float			repairSpeed;
	float			repairCost;
	unsigned		initialCash;
	unsigned		dailyIncrement;
	unsigned		dailyMaximum;
	unsigned		dailyRetained;
	unsigned		minCoolness;
	unsigned		maxCoolness;
	unsigned		addToCoolness;
	unsigned		coolnessProgressRate;
	unsigned		dealerFlags;
	unsigned		daysDelayMin;
	unsigned		daysDelayMax;

	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef inventoryParseData;

// For error messages
XML_Parser	parser = 0;
STR gFileName = 0;
bool errorInData = false;
static const int NOARMSDEALERID = -1;
bool firstUIFlagEncountered = true;


/*
 * Nesting for inventory:
 *                  -->> ELEMENT_NONE
 * <INVENTORYLIST>  -->> ELEMENT_LIST
 *  <CONTROL>       -->> MERCHANT_CONTROL
 *   <ARMSDEALERINDEX>...
 *   <SHOPKEEPERID>...
 *   <BUYCOSTMODIFIER>...
 *   <SELLCOSTMODIFIER>...
 *   <REPAIRSPEED>...
 *   <REPAIRCOST>...
 *	 <REORDERDAYSDELAY>   -->> MERCHANT_REORDER
 *	  <REORDERMINIMUM>...
 *	  <REORDERMAXIMUM>...
 *	 </REORDERDAYSDELAY>  <<-- MERCHANT_CONTROL
 *   <CASH>         -->> MERCHANT_CASH
 *    <INITIAL>...
 *    <DAILY>       -->> MERCHANT_CASH_DAILY
 *     <INCREMENT>...
 *     <CASHMAXIMUM>...
 *     <DRAINPERCENT>...
 *    </DAILY>      <<-- MERCHANT_CASH
 *   </CASH>        <<-- MERCHANT_CONTROL
 *   <COOLNESS>     -->> MERCHANT_COOLNESS
 *    <COOLMINIMUM>...
 *    <COOLMAXIMUM>...
 *    <COOLADD>...
 *    <PROGRESSRATE>...
 *   </COOLNESS>    <<-- MERCHANT_CONTROL
 *   <BASICDEALERFLAGS>  -->> MERCHANT_FLAGS
 *    <ARMS_DEALER_.../>
 *   </BASICDEALERFLAGS>
 *  </CONTROL>      <<-- ELEMENT_LIST
 *  <INVENTORY>     -->> ELEMENT
 *   ...
 *  </INVENTORY>
 * </INVENTORYLIST>
 */


/* Move this to common file */
int y = 20;							// Where to start showing errors on the screen
int errorsLeftBeforeGivingUp = 5;	// This many then we stop reporting them

static void DisplayAndLogParserError (const char *err, const XML_Char *name) {
	if (errorsLeftBeforeGivingUp-- == 0)
		return;
	CHAR8 errorBuf[511];
	sprintf(errorBuf, "XML Parser Error at line %d in file %s: %s %s", XML_GetCurrentLineNumber(parser), gFileName, err, name);
	LiveMessage(errorBuf);
	CHAR16 str[511];
	swprintf( str, L"%S", errorBuf );
    DisplayWrappedString( 10, y+=28, 630, 2, FONT12ARIAL, FONT_RED, str, FONT_BLACK, TRUE, LEFT_JUSTIFIED );
	errorInData = true;
}

#define NameIs(a) (strcmp(name, a) == 0)


static void XMLCALL
inventoryStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	AssertNotNIL(userData);
	AssertNotNIL(name);
	inventoryParseData & pData = *((inventoryParseData *)userData);

	if(pData.currentDepth <= pData.maxReadDepth) //are we reading this element?
	{
		if (NameIs("INVENTORYLIST") && pData.curElement == ELEMENT_NONE)
		{
			pData.curElement = ELEMENT_LIST;
			memset(pData.curArray,0,sizeof(DEALER_POSSIBLE_INV)*pData.maxArraySize);
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (NameIs("INVENTORY") && pData.curElement == ELEMENT_LIST)
		{
			pData.curElement = ELEMENT;
			memset(&pData.curInventory,0,sizeof(DEALER_POSSIBLE_INV));
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if(pData.curElement == ELEMENT &&
				 (NameIs("uiIndex") ||
				NameIs("sItemIndex") ||
				NameIs("ubOptimalNumber") ))
		{
			pData.curElement = ELEMENT_PROPERTY;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (NameIs("CONTROL") && pData.curElement == ELEMENT_LIST)
		{
			pData.curElement = MERCHANT_CONTROL;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (pData.curElement == MERCHANT_CONTROL &&
				  (NameIs("BUYCOSTMODIFIER") ||
				  NameIs("SELLCOSTMODIFIER") ||
				  NameIs("REPAIRSPEED") ||
				  NameIs("REPAIRCOST") ||
				  NameIs("SHOPKEEPERID") ||
				  NameIs("ARMSDEALERINDEX") ))
		{
			pData.curElement = ELEMENT_PROPERTY;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (NameIs("REORDERDAYSDELAY") && pData.curElement == MERCHANT_CONTROL)
		{
			pData.curElement = MERCHANT_REORDER;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (pData.curElement == MERCHANT_REORDER &&
				  (NameIs("REORDERMINIMUM") ||
				  NameIs("REORDERMAXIMUM")))
		{
			pData.curElement = ELEMENT_PROPERTY;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (NameIs("CASH") && pData.curElement == MERCHANT_CONTROL)
		{
			pData.curElement = MERCHANT_CASH;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (pData.curElement == MERCHANT_CASH &&
				  (NameIs("INITIAL")))
		{
			pData.curElement = ELEMENT_PROPERTY;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (NameIs("DAILY") && pData.curElement == MERCHANT_CASH)
		{
			pData.curElement = MERCHANT_CASH_DAILY;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (pData.curElement == MERCHANT_CASH_DAILY &&
				  (NameIs("INCREMENT") ||
				  NameIs("CASHMAXIMUM") ||
				  NameIs("RETAINED") ))
		{
			pData.curElement = ELEMENT_PROPERTY;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (NameIs("COOLNESS") && pData.curElement == MERCHANT_CONTROL)
		{
			pData.curElement = MERCHANT_COOLNESS;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (pData.curElement == MERCHANT_COOLNESS &&
				  (NameIs("COOLMINIMUM") ||
				  NameIs("COOLMAXIMUM") ||
				  NameIs("COOLADD") ||
				  NameIs("PROGRESSRATE") ))
		{
			pData.curElement = ELEMENT_PROPERTY;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (pData.curElement == MERCHANT_COOLNESS &&
				  (NameIs("USEBOBBYRAYSETTING") ||
		          NameIs("ALLINVENTORYALWAYSAVAILBLE")))
		{
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (NameIs("BASICDEALERFLAGS") && pData.curElement == MERCHANT_CONTROL)
		{
			pData.curElement = MERCHANT_FLAGS;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else if (pData.curElement == MERCHANT_FLAGS && 
				  (NameIs("ARMS_DEALER_HANDGUNCLASS") ||
				  NameIs("ARMS_DEALER_SMGCLASS") ||
				  NameIs("ARMS_DEALER_RIFLECLASS") ||
				  NameIs("ARMS_DEALER_MGCLASS") ||
				  NameIs("ARMS_DEALER_SHOTGUNCLASS") ||
				  NameIs("ARMS_DEALER_KNIFECLASS") ||
				  NameIs("ARMS_DEALER_BLADE") ||
				  NameIs("ARMS_DEALER_LAUNCHER") ||
				  NameIs("ARMS_DEALER_ARMOUR") ||
				  NameIs("ARMS_DEALER_MEDKIT") ||
				  NameIs("ARMS_DEALER_MISC") ||
				  NameIs("ARMS_DEALER_AMMO") ||
				  NameIs("ARMS_DEALER_GRENADE") ||
				  NameIs("ARMS_DEALER_BOMB") ||
				  NameIs("ARMS_DEALER_EXPLOSV") ||
				  NameIs("ARMS_DEALER_KIT") ||
				  NameIs("ARMS_DEALER_FACE") ||
				  NameIs("ARMS_DEALER_DETONATORS") ||
				  NameIs("ARMS_DEALER_ATTACHMENTS") ||
				  NameIs("ARMS_DEALER_ALCOHOL") ||
				  NameIs("ARMS_DEALER_ELECTRONICS") ||
				  NameIs("ARMS_DEALER_HARDWARE") ||
				  NameIs("ARMS_DEALER_MEDICAL") ||
				  NameIs("ARMS_DEALER_CREATURE_PARTS") ||
				  NameIs("ARMS_DEALER_ROCKET_RIFLE") ||
				  NameIs("ARMS_DEALER_ONLY_USED_ITEMS") ||
				  NameIs("ARMS_DEALER_GIVES_CHANGE") ||
				  NameIs("ARMS_DEALER_ACCEPTS_GIFTS") ||
				  NameIs("ARMS_DEALER_SOME_USED_ITEMS") ||
				  NameIs("ARMS_DEALER_HAS_NO_INVENTORY") ||
				  NameIs("ARMS_DEALER_ALL_GUNS") ||
				  NameIs("ARMS_DEALER_BIG_GUNS") ||
				  NameIs("ARMS_DEALER_ALL_WEAPONS")))
		{
			pData.curElement = MERCHANT_FLAGS;
			pData.maxReadDepth++; //we are not skipping this element
		}
		else {
			// Something unexpected, error!
			DisplayAndLogParserError ("unknown/unexpected start element", name);
		}
		pData.szCharData[0] = '\0';
	}
	pData.currentDepth++;
}

static void XMLCALL
inventoryCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	AssertNotNIL(userData);
	AssertNotNIL(str);
	inventoryParseData & pData = *((inventoryParseData *)userData);

	if( (pData.currentDepth <= pData.maxReadDepth) &&
		(strlen(pData.szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData.szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData.szCharData)));
	} else {
		DisplayAndLogParserError ("string too long", str);
	}
}


static bool CheckArmsDealerIndex(inventoryParseData &pData) {
	if (pData.armsDealerIndex == NOARMSDEALERID) {
		DisplayAndLogParserError("No ARMSDEALERINDEX yet", "");
		return false;
	} else {
		return true;
	}
}


static bool HandleUIFlag(inventoryParseData & pData, const XML_Char *name, const char*str, unsigned flag) {
	if (pData.curElement == MERCHANT_FLAGS && NameIs(str)) {
		if (CheckArmsDealerIndex(pData)) {
			if (firstUIFlagEncountered) {
				armsDealerInfo[pData.armsDealerIndex].uiFlags = flag;
				firstUIFlagEncountered = false;
			} else {
				armsDealerInfo[pData.armsDealerIndex].uiFlags |= flag;
			}
			return true;
		}
	}
	return false;
}



static void XMLCALL
inventoryEndElementHandle(void *userData, const XML_Char *name)
{
	AssertNotNIL(userData);
	AssertNotNIL(name);
	inventoryParseData & pData = *((inventoryParseData *)userData);

	if(pData.currentDepth <= pData.maxReadDepth) //we're at the end of an element that we've been reading
	{
		if (NameIs("INVENTORYLIST"))
		{
			pData.curElement = ELEMENT_NONE;
		}
		else if (NameIs("INVENTORY"))
		{
			pData.curElement = ELEMENT_LIST;

			if(pData.curInventory.uiIndex < pData.maxArraySize)
			{
				pData.curArray[pData.curInventory.uiIndex] = pData.curInventory; //write the inventory into the table
			}
		}
		else if (NameIs("uiIndex"))
		{
			pData.curElement = ELEMENT;
			pData.curInventory.uiIndex	= (UINT32) atol(pData.szCharData);
		}
		else if (NameIs("sItemIndex"))
		{
			pData.curElement = ELEMENT;
			pData.curInventory.sItemIndex	= (INT16) atol(pData.szCharData);
		}
		else if (NameIs("ubOptimalNumber"))
		{
			pData.curElement = ELEMENT;
			pData.curInventory.ubOptimalNumber = (UINT8) atol(pData.szCharData);
		}
		else if (NameIs("CONTROL"))
		{
			pData.curElement = ELEMENT_LIST;
		}
		else if (NameIs("ARMSDEALERINDEX"))
		{
			pData.curElement = MERCHANT_CONTROL;
 			pData.armsDealerIndex = (unsigned) atol(pData.szCharData);
			if (pData.armsDealerIndex >= 0 && pData.armsDealerIndex < NUM_ARMS_DEALERS)
				; // ok
			else {
				DisplayAndLogParserError("Invalid value (outside valid range) for", "ARMSDEALERINDEX");
			}
		}
		else if (NameIs("SHOPKEEPERID"))
		{
			pData.curElement = MERCHANT_CONTROL;
			if (CheckArmsDealerIndex(pData)) {
				pData.shopKeeperID = (unsigned) atol(pData.szCharData);
				if (pData.shopKeeperID >= 0 && pData.shopKeeperID < NUM_PROFILES)
					armsDealerInfo[pData.armsDealerIndex].ubShopKeeperID = pData.shopKeeperID;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "SHOPKEEPERID");
			}
		}
		else if (NameIs("BUYCOSTMODIFIER"))
		{
			pData.curElement = MERCHANT_CONTROL;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.buyCostModifier = (float) atof(pData.szCharData);
				if (pData.buyCostModifier >= 0 && pData.buyCostModifier < 5.0)
					armsDealerInfo[pData.armsDealerIndex].dBuyModifier = pData.buyCostModifier;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "BUYCOSTMODIFIER");
				if (pData.repairSpeed != 0 || pData.repairCost != 0)
					DisplayAndLogParserError("Shopkeepers may not both be buyers/sells and repairers", "BUYCOSTMODIFIER");
				else
					;  // Set buy/sell/repair flags
			}
		}
		else if (NameIs("SELLCOSTMODIFIER"))
		{
			pData.curElement = MERCHANT_CONTROL;
			if (CheckArmsDealerIndex(pData)) {
				pData.sellCostModifier = (float) atof(pData.szCharData);
				if (pData.sellCostModifier >= 0 && pData.sellCostModifier < 5.0)
					armsDealerInfo[pData.armsDealerIndex].dSellModifier = pData.sellCostModifier;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "SELLCOSTMODIFIER");
				if (pData.repairSpeed != 0 || pData.repairCost != 0)
					DisplayAndLogParserError("Shopkeepers may not both be buyers/sells and repairers", "SELLCOSTMODIFIER");
				else
					;  // Set buy/sell/repair flags
			}
		}
		else if (NameIs("REPAIRSPEED"))
		{
			pData.curElement = MERCHANT_CONTROL;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.repairSpeed = (float) atof(pData.szCharData);
				if (pData.repairSpeed >= 0 && pData.repairSpeed < 5.0)
					armsDealerInfo[pData.armsDealerIndex].dRepairSpeed = pData.repairSpeed;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "REPAIRSPEED");
				if (pData.buyCostModifier != 0 || pData.sellCostModifier != 0)
					DisplayAndLogParserError("Shopkeepers may not both be buyers/sells and repairers", "BUYCOSTMODIFIER");
				else
					;  // Set buy/sell/repair flags
			}
		}
		else if (NameIs("REPAIRCOST"))
		{
			pData.curElement = MERCHANT_CONTROL;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.repairCost = (float) atof(pData.szCharData);
				if (pData.repairCost >= 0 && pData.repairCost < 5.0)
					armsDealerInfo[pData.armsDealerIndex].dRepairCost = pData.repairCost;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "REPAIRCOST");
				if (pData.buyCostModifier != 0 || pData.sellCostModifier != 0)
					DisplayAndLogParserError("Shopkeepers may not both be buyers/sells and repairers", "REPAIRCOST");
				else
					;  // Set buy/sell/repair flags
			}
		}
		else if (NameIs("REORDERDAYSDELAY"))
		{
			pData.curElement = MERCHANT_CONTROL;
		}
		else if (NameIs("REORDERMINIMUM"))
		{
			pData.curElement = MERCHANT_REORDER;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.daysDelayMin = (unsigned) atol(pData.szCharData);
				if (pData.daysDelayMin >= 1 && pData.daysDelayMin <= 10)
					armsDealerInfo[pData.armsDealerIndex].daysDelayMin = pData.daysDelayMin;
				else
					DisplayAndLogParserError("Invalid value (outside valid range of 1..10) for", "REORDERMINIMUM");
			}
		}
		else if (NameIs("REORDERMAXIMUM"))
		{
			pData.curElement = MERCHANT_REORDER;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.daysDelayMax = (unsigned) atol(pData.szCharData);
				if (pData.daysDelayMax >= 1 && pData.daysDelayMax <= 10)
					armsDealerInfo[pData.armsDealerIndex].daysDelayMax = pData.daysDelayMax;
				else
					DisplayAndLogParserError("Invalid value (outside valid range of 1..10) for", "REORDERMAXIMUM");
			}
		}
		else if (NameIs("CASH"))
		{
			pData.curElement = MERCHANT_CONTROL;
		}
		else if (NameIs("INITIAL"))
		{
			pData.curElement = MERCHANT_CASH;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.initialCash = (unsigned) atol(pData.szCharData);
				if (pData.initialCash >= 0 && pData.initialCash <= 1000000)
					armsDealerInfo[pData.armsDealerIndex].iInitialCash = pData.initialCash;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "INITIAL");
			}
		}
		else if (NameIs("DAILY"))
		{
			pData.curElement = MERCHANT_CASH;
		}
		else if (NameIs("INCREMENT"))
		{
			pData.curElement = MERCHANT_CASH_DAILY;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.dailyIncrement = (unsigned) atol(pData.szCharData);
				if (pData.dailyIncrement >= 0 && pData.dailyIncrement <= 1000000)
					armsDealerInfo[pData.armsDealerIndex].dailyIncrement = pData.dailyIncrement;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "INCREMENT");
			}
		}
		else if (NameIs("CASHMAXIMUM"))
		{
			pData.curElement = MERCHANT_CASH_DAILY;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.dailyMaximum = (unsigned) atol(pData.szCharData);
				if (pData.dailyMaximum >= 0 && pData.dailyMaximum <= 1000000)
					armsDealerInfo[pData.armsDealerIndex].dailyMaximum = pData.dailyMaximum;
				else
					DisplayAndLogParserError("Invalid value (outside valid range) for", "CASHMAXIMUM");
			}
		}
		else if (NameIs("RETAINED"))
		{
			pData.curElement = MERCHANT_CASH_DAILY;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.dailyRetained = (unsigned) atol(pData.szCharData);
				if (pData.dailyRetained >= 0 && pData.dailyRetained <= 100)
					armsDealerInfo[pData.armsDealerIndex].dailyRetained = pData.dailyRetained;
				else
					DisplayAndLogParserError("Invalid value (outside valid range of 0..100) for", "RETAINED");
			}
		}
		else if (NameIs("COOLNESS"))
		{
			pData.curElement = MERCHANT_CONTROL;
		}
		else if (NameIs("COOLMINIMUM"))
		{
			pData.curElement = MERCHANT_COOLNESS;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.minCoolness = (unsigned) atol(pData.szCharData);
				if (pData.minCoolness >= 0 && pData.minCoolness <= 10)
					armsDealerInfo[pData.armsDealerIndex].minCoolness = pData.minCoolness;
				else
					DisplayAndLogParserError("Invalid value (outside valid range of 0..10) for", "COOLMINIMUM");
			}
		}
		else if (NameIs("COOLMAXIMUM"))
		{
			pData.curElement = MERCHANT_COOLNESS;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.maxCoolness = (unsigned) atol(pData.szCharData);
				if (pData.maxCoolness >= 0 && pData.maxCoolness <= 10)
					armsDealerInfo[pData.armsDealerIndex].maxCoolness = pData.maxCoolness;
				else
					DisplayAndLogParserError("Invalid value (outside valid range of 0..10) for", "COOLMAXIMUM");
			}
		}
		else if (NameIs("COOLADD"))
		{
			pData.curElement = MERCHANT_COOLNESS;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.addToCoolness = (unsigned) atol(pData.szCharData);
				if (pData.addToCoolness >= 0 && pData.addToCoolness <= 10)
					armsDealerInfo[pData.armsDealerIndex].addToCoolness = pData.addToCoolness;
				else
					DisplayAndLogParserError("Invalid value (outside valid range of 0..10) for", "COOLADD");
			}
		}
		else if (NameIs("PROGRESSRATE"))
		{
			pData.curElement = MERCHANT_COOLNESS;
			if (CheckArmsDealerIndex(pData)) {
	 			pData.coolnessProgressRate = (unsigned) atol(pData.szCharData);
				if (pData.coolnessProgressRate >= 0 && pData.coolnessProgressRate <= 100)
					armsDealerInfo[pData.armsDealerIndex].coolnessProgressRate = pData.coolnessProgressRate;
				else
					DisplayAndLogParserError("Invalid value (outside valid range of 0..100) for", "PROGRESSRATE");
			}
		}
		else if (NameIs("USEBOBBYRAYSETTING"))
		{
			pData.curElement = MERCHANT_COOLNESS;
			if (CheckArmsDealerIndex(pData)) {
	 			//pData.useBRSetting = true;
				armsDealerInfo[pData.armsDealerIndex].useBRSetting = true;
			}
		}
		else if (NameIs("ALLINVENTORYALWAYSAVAILBLE"))
		{
			pData.curElement = MERCHANT_COOLNESS;
			if (CheckArmsDealerIndex(pData)) {
	 			//pData.useBRSetting = true;
				armsDealerInfo[pData.armsDealerIndex].allInventoryAlwaysAvailable = true;
			}
		}
		else if (NameIs("BASICDEALERFLAGS"))
		{
			pData.curElement = MERCHANT_CONTROL;
		}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_HANDGUNCLASS", ARMS_DEALER_HANDGUNCLASS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_SMGCLASS", ARMS_DEALER_SMGCLASS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_RIFLECLASS", ARMS_DEALER_RIFLECLASS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_MGCLASS", ARMS_DEALER_MGCLASS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_SHOTGUNCLASS", ARMS_DEALER_SHOTGUNCLASS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_KNIFECLASS", ARMS_DEALER_KNIFECLASS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_BLADE", ARMS_DEALER_BLADE)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_LAUNCHER", ARMS_DEALER_LAUNCHER)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ARMOUR", ARMS_DEALER_ARMOUR)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_MEDKIT", ARMS_DEALER_MEDKIT)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_MISC", ARMS_DEALER_MISC)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_AMMO", ARMS_DEALER_AMMO)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_GRENADE", ARMS_DEALER_GRENADE)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_BOMB", ARMS_DEALER_BOMB)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_EXPLOSV", ARMS_DEALER_EXPLOSV)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_KIT", ARMS_DEALER_KIT)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_FACE", ARMS_DEALER_FACE)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_DETONATORS", ARMS_DEALER_DETONATORS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ATTACHMENTS", ARMS_DEALER_ATTACHMENTS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ALCOHOL", ARMS_DEALER_ALCOHOL)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ELECTRONICS", ARMS_DEALER_ELECTRONICS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_HARDWARE", ARMS_DEALER_HARDWARE)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_MEDICAL", ARMS_DEALER_MEDICAL)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_CREATURE_PARTS", ARMS_DEALER_CREATURE_PARTS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ROCKET_RIFLE", ARMS_DEALER_ROCKET_RIFLE)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ONLY_USED_ITEMS", ARMS_DEALER_ONLY_USED_ITEMS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_GIVES_CHANGE", ARMS_DEALER_GIVES_CHANGE)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ACCEPTS_GIFTS", ARMS_DEALER_ACCEPTS_GIFTS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_SOME_USED_ITEMS", ARMS_DEALER_SOME_USED_ITEMS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_HAS_NO_INVENTORY", ARMS_DEALER_HAS_NO_INVENTORY)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ALL_GUNS", ARMS_DEALER_ALL_GUNS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_BIG_GUNS", ARMS_DEALER_BIG_GUNS)) {}
		else if (HandleUIFlag(pData, name, "ARMS_DEALER_ALL_WEAPONS", ARMS_DEALER_ALL_WEAPONS)) {}
		else {
			// Something unexpected, error!
			DisplayAndLogParserError ("unknown/unexpected end element", name);
		}

		pData.maxReadDepth--;
	}

	pData.currentDepth--;
}




BOOLEAN ReadInInventoryStats(DEALER_POSSIBLE_INV *pInv, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	parser = XML_ParserCreate(NULL);
	gFileName = fileName;

	inventoryParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Inventory.xml" );

	// Open inventory file
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

	XML_SetElementHandler(parser, inventoryStartElementHandle, inventoryEndElementHandle);
	XML_SetCharacterDataHandler(parser, inventoryCharacterDataHandle);

	// Set the default values
	memset(&pData,0,sizeof(pData));
	pData.armsDealerIndex = NOARMSDEALERID;

	pData.curArray = pInv;
	pData.maxArraySize = MAXITEMS;

	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		DisplayAndLogParserError (XML_ErrorString(XML_GetErrorCode(parser)), "");
		errorInData = true;
	}

	MemFree(lpcBuffer);
	XML_ParserFree(parser);

	if (errorInData)
		// Assume message already generated
		return FALSE;
	else
		return( TRUE );
}


BOOLEAN WriteInventoryStats(DEALER_POSSIBLE_INV *pInv, STR fileName)
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeinventorystats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<INVENTORYLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<INVENTORY>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<sItemIndex>%d</sItemIndex>\r\n",								pInv[cnt].sItemIndex	);
			FilePrintf(hFile,"\t\t<ubOptimalNumber>%d</ubOptimalNumber>\r\n",								pInv[cnt].ubOptimalNumber	);

			FilePrintf(hFile,"\t</INVENTORY>\r\n");
		}
		FilePrintf(hFile,"</INVENTORYLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
