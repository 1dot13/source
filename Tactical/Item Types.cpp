
#include "Item Types.h"
#include "Debug.h"
#include "Items.h"
#include "GameSettings.h"
#include "screenids.h"


int		BODYPOSFINAL		= GUNSLINGPOCKPOS;//RESET in initInventory
int		BIGPOCKFINAL		= MEDPOCK1POS;//RESET in initInventory
int		MEDPOCKSTART		= MEDPOCK1POS;//RESET in initInventory
int		MEDPOCKFINAL		= SMALLPOCK1POS;//RESET in initInventory
int		SMALLPOCKFINAL		= NUM_INV_SLOTS;//RESET in initInventory


OBJECTTYPE gTempObject;
OBJECTTYPE gStackTempObject;
std::list<LBENODE>	LBEArray;
int gLastLBEUniqueID = 0;

extern UINT32			guiCurrentItemDescriptionScreen;
extern BOOLEAN			fShowMapInventoryPool;
//extern BOOLEAN AutoPlaceObjectInInventoryStash( OBJECTTYPE *pItemPtr, INT32 sGridNo );

bool IsSlotAnLBESlot(int slot)
{
	switch(slot) {
	case VESTPOCKPOS:
	case LTHIGHPOCKPOS:
	case RTHIGHPOCKPOS:
	case CPACKPOCKPOS:
	case BPACKPOCKPOS:
		return true;
	}
	return false;
}

bool IsSlotASmallPocket(int slot)
{
	if (slot >= SMALLPOCKSTART && slot < SMALLPOCKFINAL) {
		return true;
	}
	return false;
}

void CreateLBE (OBJECTTYPE* pObj, UINT8 ubID, int numSubPockets)
{
	int uniqueID;
	LBENODE* pLBE = NULL;
	if (pObj->IsActiveLBE(0) == true) {
		uniqueID = (*pObj)[0]->data.lbe.uniqueID;
		for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
			if (iter->uniqueID == uniqueID) {
				pLBE = &(*iter);
				break;
			}
		}
		Assert(pLBE);
	}
	else {
		//CHRISL: I don't understand why we do this instead of using the old GetFreeLBEPackIndex function to
		// generate a uniqueID only when one is needed.
		uniqueID = gLastLBEUniqueID++;
		LBEArray.push_back(LBENODE());
		pLBE = &LBEArray.back();
		pLBE->uniqueID = uniqueID;
	}
	pLBE->ubID = ubID;
	pLBE->lbeIndex = Item[pObj->usItem].ubClassIndex;
	pLBE->lbeClass = LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeClass;
	pLBE->inv.clear();
	pLBE->inv.resize(numSubPockets);
	(*pObj)[0]->data.lbe.bLBE = -1;
	(*pObj)[0]->data.lbe.uniqueID = uniqueID;
}

bool DestroyLBEIfEmpty(OBJECTTYPE* pObj)
{
	if (pObj->IsActiveLBE(0) == true) {
		LBENODE* pLBE = pObj->GetLBEPointer(0);
		if (pLBE) {
			for (unsigned int x = 0; x < pLBE->inv.size(); ++x) {
				if (pLBE->inv[x].exists() == true) {
					return false;
				}
			}
			for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
				if (iter->uniqueID == pLBE->uniqueID) {
					LBEArray.erase(iter);
					break;
				}
			}
			(*pObj)[0]->data.lbe.uniqueID = 0;
			(*pObj)[0]->data.lbe.bLBE = 0;
			return true;
		}
	}
	return false;
}

void DestroyLBE(OBJECTTYPE* pObj)
{
	if(pObj->IsActiveLBE(0) == true)
	{
		LBENODE* pLBE = pObj->GetLBEPointer(0);
		if(pLBE)
		{
			for(unsigned int x = 0; x < pLBE->inv.size(); x++)
			{
				if(pLBE->inv[x].exists() == true)
				{
					pLBE->inv[x].initialize();
				}
			}
			for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
				if (iter->uniqueID == pLBE->uniqueID) {
					LBEArray.erase(iter);
					break;
				}
			}
			(*pObj)[0]->data.lbe.uniqueID = 0;
			(*pObj)[0]->data.lbe.bLBE = 0;
			return;
		}
	}
	return;
}

void MoveItemsInSlotsToLBE( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, LBENODE* pLBE, OBJECTTYPE* pObj)
{

	for(unsigned int i=0; i<LBESlots.size(); i++)	// Go through default pockets one by one
	{
		if(pSoldier->inv[LBESlots[i]].exists() == false)	// No item in this pocket
			continue;

		// Found an item in a default pocket so get it's ItemSize
		UINT16 dSize = CalculateItemSize(&pSoldier->inv[LBESlots[i]]);
		for(unsigned int j=0; j<pLBE->inv.size(); j++)	// Search through LBE and see if item fits anywhere
		{
			if(pLBE->inv[j].exists() == true)	// Item already stored in LBENODE pocket
				continue;
			// No item in this LBENODE pocket, is pocket active?
			if(LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[j] == NONE)	// Pocket is inactive
				continue;
			//dnl Pocket is active, so check if there is restriction of type for default item
			if(LBEPocketType[LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[j]].pRestriction != 0 && LBEPocketType[LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[j]].pRestriction != Item[pSoldier->inv[LBESlots[i]].usItem].ubClassIndex)
				continue;
			// Pocket is active, can default item fit in this pocket?
			if(LBEPocketType[LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[j]].ItemCapacityPerSize[dSize] == NONE)	// Pocket can't hold this item size
				continue;
			UINT16 dCap = LBEPocketType[LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbePocketIndex[j]].ItemCapacityPerSize[dSize];
			// Default item will fit in this pocket.  Setup the LBENODE if necessary

			//ADB the object is already in the LBE, we are just moving it from soldier inv to LBE inv
			pSoldier->inv[LBESlots[i]].MoveThisObjectTo(pLBE->inv[j], ALL_OBJECTS, NULL, STACK_SIZE_LIMIT, dCap);
			if (pSoldier->inv[LBESlots[i]].exists() == false) {
				break;
			}
			else {
				//move didn't work, maybe it was a stack?
				dSize = CalculateItemSize(&pSoldier->inv[LBESlots[i]]);
			}
		}
	}
	return;
}

// CHRISL: New function to move items from default pockets to usable pockets
BOOLEAN MoveItemsToActivePockets( SOLDIERTYPE *pSoldier, std::vector<INT8>& LBESlots, UINT32 uiHandPos, OBJECTTYPE *pObj )
{
	BOOLEAN	flag=FALSE;

	if(pObj->IsActiveLBE(0) == false) {
		CreateLBE(pObj, pSoldier->ubID, LBESlots.size());
	}

	LBENODE* pLBE = pObj->GetLBEPointer(0);
	MoveItemsInSlotsToLBE(pSoldier, LBESlots, pLBE, pObj);
	DestroyLBEIfEmpty(pObj);

	// We've put everything into the LBENODE that we could, now search other pockets for openings
	for(unsigned int x=0; x<LBESlots.size(); x++)
	{
		if(pSoldier->inv[LBESlots[x]].exists() == false)
			continue;
		for(int i=INV_START_POS; i<NUM_INV_SLOTS; i++)
		{
			for(unsigned int j=0; j<LBESlots.size(); j++)
			{
				flag = FALSE;
				if(i==LBESlots[j])
				{
					flag=true;
					break;
				}
			}
			if(flag)
				continue;
			//CHRISL: If we're putting on a BackPack or Combat Pack, we need to make sure we don't accidently break the
			//	pack rules since, at this stage, our new pack actually hasn't been placed.
			if(pSoldier->inv[i].exists() == false)	// No item in this pocket.  Place the item.
			{
				if((i == CPACKPOCKPOS && uiHandPos == BPACKPOCKPOS) || (i == BPACKPOCKPOS && uiHandPos == CPACKPOCKPOS))
				{	
					//DBrot: changed to bitwise comparison
					UINT32 newPack = LoadBearingEquipment[Item[pObj->usItem].ubClassIndex].lbeCombo;
					UINT32 chkPack = LoadBearingEquipment[Item[pSoldier->inv[i].usItem].ubClassIndex].lbeCombo;
					if(newPack == 0 || (newPack & chkPack) == 0)
						continue;
				}
				if(CanItemFitInPosition(pSoldier, &(pSoldier->inv[LBESlots[x]]), i, FALSE))
				{
					pSoldier->inv[LBESlots[x]].MoveThisObjectTo(pSoldier->inv[i], ALL_OBJECTS, pSoldier, i);
					break;
				}
			}
			else if(pSoldier->inv[i].usItem == pSoldier->inv[LBESlots[x]].usItem)	// Item is identical so maybe it can stack
			{
				pSoldier->inv[i].AddObjectsToStack(pSoldier->inv[LBESlots[x]], ALL_OBJECTS, pSoldier, i);
				break;
			}
		}
	}

	// now drop everything that wouldn't fit anywhere else
	for(unsigned int i=0; i<LBESlots.size() ;i++)
	{
		if(pSoldier->inv[LBESlots[i]].exists() == false)	// No item in pocket
			continue;
		AutoPlaceObjectToWorld(pSoldier, &pSoldier->inv[LBESlots[i]]);
		/*if(guiCurrentItemDescriptionScreen == MAP_SCREEN && fShowMapInventoryPool)
		{
			AutoPlaceObjectInInventoryStash(&pSoldier->inv[LBESlots[i]], pSoldier->sGridNo);
		}
		else
		{
			AddItemToPool( pSoldier->sGridNo, &pSoldier->inv[LBESlots[i]], 1, pSoldier->pathing.bLevel, 0 , -1 );
			NotifySoldiersToLookforItems( );
		}*/
		DeleteObj(&pSoldier->inv[LBESlots[i]]);
	}

	return(TRUE);
}

void GetLBESlots(UINT32 LBEType, std::vector<INT8>& LBESlots)
{
	switch (LBEType)
	{
		case VESTPOCKPOS:
			LBESlots.push_back(SMALLPOCK1POS);
			LBESlots.push_back(SMALLPOCK2POS);
			LBESlots.push_back(SMALLPOCK3POS);
			LBESlots.push_back(SMALLPOCK4POS);
			LBESlots.push_back(SMALLPOCK5POS);
			LBESlots.push_back(SMALLPOCK6POS);
			LBESlots.push_back(SMALLPOCK7POS);
			LBESlots.push_back(SMALLPOCK8POS);
			LBESlots.push_back(SMALLPOCK9POS);
			LBESlots.push_back(SMALLPOCK10POS);
			LBESlots.push_back(MEDPOCK1POS);
			LBESlots.push_back(MEDPOCK2POS);
			break;
		case LTHIGHPOCKPOS:
			LBESlots.push_back(SMALLPOCK11POS);
			LBESlots.push_back(SMALLPOCK12POS);
			LBESlots.push_back(SMALLPOCK13POS);
			LBESlots.push_back(SMALLPOCK14POS);
			LBESlots.push_back(MEDPOCK3POS);
			break;
		case RTHIGHPOCKPOS:
			LBESlots.push_back(SMALLPOCK15POS);
			LBESlots.push_back(SMALLPOCK16POS);
			LBESlots.push_back(SMALLPOCK17POS);
			LBESlots.push_back(SMALLPOCK18POS);
			LBESlots.push_back(MEDPOCK4POS);
			break;
		case CPACKPOCKPOS:
			LBESlots.push_back(SMALLPOCK19POS);
			LBESlots.push_back(SMALLPOCK20POS);
			LBESlots.push_back(SMALLPOCK21POS);
			LBESlots.push_back(SMALLPOCK22POS);
			LBESlots.push_back(BIGPOCK1POS);
			LBESlots.push_back(BIGPOCK2POS);
			LBESlots.push_back(BIGPOCK3POS);
			break;
		case BPACKPOCKPOS:
			LBESlots.push_back(SMALLPOCK23POS);
			LBESlots.push_back(SMALLPOCK24POS);
			LBESlots.push_back(SMALLPOCK25POS);
			LBESlots.push_back(SMALLPOCK26POS);
			LBESlots.push_back(SMALLPOCK27POS);
			LBESlots.push_back(SMALLPOCK28POS);
			LBESlots.push_back(SMALLPOCK29POS);
			LBESlots.push_back(SMALLPOCK30POS);
			LBESlots.push_back(BIGPOCK4POS);
			LBESlots.push_back(BIGPOCK5POS);
			LBESlots.push_back(BIGPOCK6POS);
			LBESlots.push_back(BIGPOCK7POS);
			break;
		default:
			//CHRISL: Technically, this should never occur but we don't want the program to crash.
			Assert(0);
			//DebugBreak();
	}
	return;
}


// CHRISL: New function to handle moving soldier items to lbe items
BOOLEAN MoveItemToLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos )
{
	std::vector<INT8> LBESlots;
	// Determine which LBE item we're removing so we can associate the correct pockets with it.
	GetLBESlots(uiHandPos, LBESlots);

	CreateLBE(&(pSoldier->inv[uiHandPos]), pSoldier->ubID, LBESlots.size());
	LBENODE* pLBE = pSoldier->inv[uiHandPos].GetLBEPointer(0);
	for(unsigned int i=0; i<LBESlots.size(); i++)
	{
		// Is there an item in this pocket?
		if(pSoldier->inv[LBESlots[i]].exists() == true)
		{
			//ADB the object is already in the LBE, we are just moving it from soldier inv to LBE inv
			pSoldier->inv[LBESlots[i]].MoveThisObjectTo(pLBE->inv[i], -1, pSoldier, LBESlots[i]);
		}
	}

	if (DestroyLBEIfEmpty(&pSoldier->inv[uiHandPos]) == true) {
		return(FALSE);
	}

	return (TRUE);
}

// CHRISL: New function to handle moving lbe items to soldier items
BOOLEAN MoveItemFromLBEItem( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, OBJECTTYPE *pObj )
{
	std::vector<INT8> LBESlots;

	// Determine which LBE item we're adding so we can associate the correct pockets with it.
	GetLBESlots(uiHandPos, LBESlots);

	if(pSoldier->inv[uiHandPos].exists() == false)
		MoveItemsToActivePockets(pSoldier, LBESlots, uiHandPos, pObj);
	if(pObj->IsActiveLBE(0) == false) {
		return (FALSE);
	}

	LBENODE* pLBE = pObj->GetLBEPointer(0);
	for(unsigned int i=0; i<LBESlots.size(); i++)
	{
		// Is there an item in this LBE pocket?
		if(pLBE->inv[i].exists() == true)
		{
			pLBE->inv[i].MoveThisObjectTo(pSoldier->inv[LBESlots[i]], ALL_OBJECTS, pSoldier, LBESlots[i]);
			//check to make sure item was moved.  If not, try placing item in an active pocket
			if(pLBE->inv[i].exists() == true)
			{
				if(!AutoPlaceObject(pSoldier, &pLBE->inv[i], FALSE) && pSoldier->inv[LBESlots[i]].exists() == false)
				{
					//still can't place the item?  Force move to the pocket associated with the LBE if empty
					pSoldier->inv[LBESlots[i]].ForceAddObjectsToStack(pLBE->inv[i]);
				}
			}
		}
	}
	if (DestroyLBEIfEmpty(pObj) == false) {
		//we should have copied all the items from the LBE to the soldier
		//which means the LBE should be empty and destroyed.  However, if it's not empty, we need to force place
		//some items so that we can empty the LBE without losing anything.
		for(unsigned int i = 0; i < LBESlots.size(); i++)
		{
			if(pLBE->inv[i].exists() == true)
			{
				for(unsigned int j = BIGPOCKSTART; j < pSoldier->inv.size(); j++)
				{
					if(pSoldier->inv[j].exists() == false)
					{
						pSoldier->inv[j].ForceAddObjectsToStack(pLBE->inv[i]);
						break;
					}
				}
			}
			//Now, check one last time and if we still have an object, drop it to the ground
			if(pLBE->inv[i].exists() == true)
			{
				AddItemToPool(pSoldier->sGridNo, &pLBE->inv[i], 1, pSoldier->pathing.bLevel, 0, -1);
				//at this point, if we still haven't removed the item, the only thing left to do is delete it.
				pLBE->inv[i].initialize();
			}
		}
		DestroyLBE(pObj);
	}

	return (TRUE);
}

LBETYPE::LBETYPE(){
	memset(this, 0, SIZEOF_LBETYPE);
	lbePocketIndex.clear();
	lbePocketIndex.resize(MAX_ITEMS_IN_LBE);
}
LBETYPE::LBETYPE(const LBETYPE& src) {
	memcpy(this, &src, SIZEOF_LBETYPE);
	lbePocketIndex = src.lbePocketIndex;
}
LBETYPE& LBETYPE::operator=(const LBETYPE& src){
    if (this != &src) {
		memcpy(this, &src, SIZEOF_LBETYPE);
		lbePocketIndex = src.lbePocketIndex;
    }
    return *this;
}
LBETYPE::~LBETYPE(){
}
POCKETTYPE::POCKETTYPE(){
	memset(this, 0, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(gGameExternalOptions.guiMaxItemSize+1);//JMich
}
POCKETTYPE::POCKETTYPE(const POCKETTYPE& src){
	memcpy(this, &src, SIZEOF_POCKETTYPE);
	ItemCapacityPerSize.resize(gGameExternalOptions.guiMaxItemSize+1);//JMich
	ItemCapacityPerSize = src.ItemCapacityPerSize;
}
POCKETTYPE& POCKETTYPE::operator=(const POCKETTYPE& src){
    if (this != &src) {
		memcpy(this, &src, SIZEOF_POCKETTYPE);
		ItemCapacityPerSize = src.ItemCapacityPerSize;
    }
    return *this;
}
POCKETTYPE::~POCKETTYPE(){
}

bool OBJECTTYPE::IsActiveLBE(unsigned int index)
{
	if (exists() == true && Item[this->usItem].usItemClass == IC_LBEGEAR) {
		return ((*this)[index]->data.lbe.bLBE == -1);
	}
	return false;
}

bool OBJECTTYPE::HasAnyActiveLBEs(SOLDIERTYPE * pSoldier, UINT8 iter)
{
	//CHRISL: Since only IC_LBEGEAR items can be classified as LBENODEs, only allow this to return true if the item we're
	//	working with is actually the right class of item.
	if (exists() == true && Item[this->usItem].usItemClass == IC_LBEGEAR) {
		if(pSoldier != NULL){
			for (int x = 0; x < ubNumberOfObjects; ++x) {
				if ((*this)[x]->data.lbe.bLBE == -1) {
					return true;
				}
			}
		}
		else{
			if((*this)[iter]->data.lbe.bLBE == -1){
				return true;
			}
		}
	}
	return false;
}

LBENODE* OBJECTTYPE::GetLBEPointer(unsigned int index)
{
	if (LBEArray.empty() == false) {
		int uniqueID = (*this)[index]->data.lbe.uniqueID;
		for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
			if (iter->uniqueID == uniqueID) {
				return &(*iter);
			}
		}
	}
	//CHRISL: if the LBEArray is empty, we probably need to force the creation of an entry so that future functions don't crash.
	//return NULL;
	LBEArray.resize(1);
	return &(*LBEArray.begin());
}

bool OBJECTTYPE::exists()
{
	if(this == NULL)
		return(FALSE);
	return (ubNumberOfObjects > 0 && usItem != NOTHING);
}

void OBJECTTYPE::SpliceData(OBJECTTYPE& sourceObject, unsigned int numToSplice, StackedObjects::iterator beginIter)
{
	if (numToSplice == 0) {
		return;
	}
	//remember that sometimes objectStack is 1 while ubNumberOfObjects is 0
	if (ubNumberOfObjects == 0) {
		objectStack.clear();
	}
	StackedObjects::iterator stopIter = beginIter;
	for (unsigned int x = 0; x < numToSplice; ++x) {
		++stopIter;
	}
	objectStack.splice(objectStack.end(), sourceObject.objectStack, beginIter, stopIter);

	ubNumberOfObjects += numToSplice;
	//ADB ubWeight has been removed, see comments in OBJECTTYPE
	//ubWeight = CalculateObjectWeight(this);

	if (sourceObject.objectStack.empty() == true) {
		//init it so it is not empty, even though it no longer exists
		sourceObject.initialize();
	}
	else {
		sourceObject.ubNumberOfObjects -= numToSplice;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//sourceObject.ubWeight = CalculateObjectWeight(&sourceObject);
	}
}

int OBJECTTYPE::AddObjectsToStack(int howMany, int objectStatus)
{
	//This function is never called from a soldier, so get the max size
	int numToAdd = max(0, ItemSlotLimit( this, STACK_SIZE_LIMIT ) - ubNumberOfObjects);

	//if howMany is ALL_OBJECTS the stack will become full
	if (howMany != ALL_OBJECTS) {
		numToAdd = min(numToAdd, howMany);
	}

	if (numToAdd) {
		CreateItems(usItem, objectStatus, numToAdd, &gStackTempObject);
		this->AddObjectsToStack(gStackTempObject);
	}

	//returns how many were NOT added
	return howMany - numToAdd;
}

bool OBJECTTYPE::CanStack(OBJECTTYPE& sourceObject, int& numToStack)
{
	//stacking control, restrict certain things here
	if (numToStack > 0) {
		if (exists() == true) {
			if (Item[usItem].usItemClass == IC_MONEY) {
				//money doesn't stack, it merges
				// average out the status values using a weighted average...
				int thisStatus = (*this)[0]->data.money.bMoneyStatus * (*this)[0]->data.money.uiMoneyAmount;
				int sourceStatus = sourceObject[0]->data.money.bMoneyStatus * sourceObject[0]->data.money.uiMoneyAmount;
				int average = __max((*this)[0]->data.money.uiMoneyAmount + sourceObject[0]->data.money.uiMoneyAmount,1);

				(*this)[0]->data.objectStatus = ( (thisStatus + sourceStatus) / average);
				(*this)[0]->data.money.uiMoneyAmount += sourceObject[0]->data.money.uiMoneyAmount;
				//CHRISL: We need to delete sourceObject at this point since we've just merged the two items
				sourceObject.initialize();
				return false;
			}

			//keys can stack if the same key
			if (Item[usItem].usItemClass == IC_KEY) {
				if ((*this)[0]->data.key.ubKeyID != sourceObject[0]->data.key.ubKeyID) {
					return false;
				}
			}

			//for convenience sake, do not allow mixed flags to stack!
			//currently flags are stored on a per-stack basis - the problem with them
			//being on a per-object-in-stack basis is a check on the flags only ever checks the top object in the stack
			//continue on because you might find something else with the same flags
			//ADB added the following as of revision 1548
			//disallow stacking for objects with dissimilar UNDROPPABLE flags
			//objects can stack if they are both droppable or both undroppable, but not one droppable and the other not
			//they can also stack regardless of the other flags that are set, like OBJECT_NO_OVERWRITE
			//in the old code objects with OBJECT_NO_OVERWRITE not only are dropped, but also can stack with others without the same flag
			//I don't care to find out if this is a bug or not, I'm just going to mimic the original code.
			/*CHRISL: This is just stupid.  OBJECT_UNDROPPABLE is only a valid flag for NPCs so a player should never get an item
			with this flag set.  For some reason, however, player items can come with this flag set.  But this flag doesn't effect
			players in any way, so lets just clear the flag when we find it.*/
			//if ((sourceObject.fFlags & OBJECT_UNDROPPABLE) != (this->fFlags & OBJECT_UNDROPPABLE)) {
			//	DebugBreakpoint();
			//	numToStack = 0;
			//}
			if(sourceObject.fFlags & OBJECT_UNDROPPABLE)
				sourceObject.fFlags &= ~OBJECT_UNDROPPABLE;
			if(this->fFlags & OBJECT_UNDROPPABLE)
				this->fFlags &= ~OBJECT_UNDROPPABLE;

			//nor allow trapped items to stack
			if (sourceObject[0]->data.bTrap > 0
				|| (*this)[0]->data.bTrap > 0) {
				return false;
			}
			//TODO other specials if there are any
		}
	}
	return true;
}

int OBJECTTYPE::ForceAddObjectsToStack(OBJECTTYPE& sourceObject, int howMany)
{
	if (exists() == false) {
		//we are adding to an empty object, it can happen
		Assert(sourceObject.exists() == true);
		usItem = sourceObject.usItem;
		this->fFlags = sourceObject.fFlags;
		//this->usAttachmentSlotIndexVector = sourceObject.usAttachmentSlotIndexVector;
	}
	Assert(sourceObject.usItem == usItem);

	int numToAdd = sourceObject.ubNumberOfObjects;
	//if howMany is ALL_OBJECTS the stack will become full if sourceObject has enough
	if (howMany != ALL_OBJECTS) {
		numToAdd = min(numToAdd, howMany);
	}

	//this recalcs weight too!
	SpliceData(sourceObject, numToAdd, sourceObject.objectStack.begin());

	//returns how many were NOT added
	if (howMany != ALL_OBJECTS) {
		return howMany - numToAdd;
	}
	else {
		//ALL_OBJECTS means move all, if all were moved the new size should be 0
		return sourceObject.ubNumberOfObjects;
	}
}

int OBJECTTYPE::AddObjectsToStack(OBJECTTYPE& sourceObject, int howMany, SOLDIERTYPE* pSoldier, int slot, int cap, bool allowLBETransfer)
{
	int freeObjectsInStack;

	//can't add too much, can't take too many
	if(cap > 0){
		freeObjectsInStack = max(0, (cap - ubNumberOfObjects));
	}
	else{
		//freeObjectsInStack = max(0, (ItemSlotLimit( this, slot, pSoldier ) - ubNumberOfObjects));
		freeObjectsInStack = max(0, (ItemSlotLimit( &sourceObject, slot, pSoldier ) - ubNumberOfObjects));
	}
	int numToAdd = min (freeObjectsInStack, sourceObject.ubNumberOfObjects);
	//if howMany is ALL_OBJECTS the stack will become full if sourceObject has enough
	if (howMany != ALL_OBJECTS) {
		numToAdd = min(numToAdd, howMany);
	}

	if(numToAdd == 0){
		return 0;
	}
	if (this->CanStack(sourceObject, numToAdd) == false) {
		return 0;
	}

	//CHRISL: Moved to here so that we verify that the object can be created before starting the creation
	if (exists() == false) {
		//we are adding to an empty object, it can happen
		Assert(sourceObject.exists() == true);
		usItem = sourceObject.usItem;
		this->fFlags = sourceObject.fFlags;
		//this->usAttachmentSlotIndexVector = sourceObject.usAttachmentSlotIndexVector;
	}
	Assert(sourceObject.usItem == usItem);

	//this recalcs weight too!
	SpliceData(sourceObject, numToAdd, sourceObject.objectStack.begin());
	
	if (numToAdd && UsingNewInventorySystem() == true && pSoldier != NULL) {
		//CHRISL: For New Inventory system.  Are we handling an LBE item FROM a soldier?
		if(IsSlotAnLBESlot(slot) == true && allowLBETransfer == true)
		{
			//we know this must be an LBE because of the slot and ubNumberOfObjects
			//since we are putting this LBE into an LBE slot in a soldier
			//we need to move the items inside the LBE into the soldier's inventory
			MoveItemFromLBEItem( pSoldier, slot, this );
		}
	}

	//returns how many were NOT added
	if (howMany != ALL_OBJECTS) {
		return howMany - numToAdd;
	}
	else {
		//ALL_OBJECTS means move all, if all were moved the new size should be 0
		return sourceObject.ubNumberOfObjects;
	}
}

int OBJECTTYPE::MoveThisObjectTo(OBJECTTYPE& destObject, int numToMove, SOLDIERTYPE* pSoldier, int slot, int cap)
{
	//ADB yes I said I normally remove functions like this, but this is different
	//this exists to make reading easier and to be more descriptive.
	return (PrivateRemoveObjectsFromStack(numToMove, &destObject, pSoldier, slot, cap));
}

int OBJECTTYPE::RemoveObjectsFromStack(int howMany)
{
	return (PrivateRemoveObjectsFromStack(howMany, NULL, NULL, STACK_SIZE_LIMIT));
}

int OBJECTTYPE::PrivateRemoveObjectsFromStack(int howMany, OBJECTTYPE* destObject, SOLDIERTYPE* pSoldier, int slot, int cap)
{
	//ADB this function only needs to know soldier and slot
	//if there is a dest object we are putting the removed objects into
	//in this case it is acting as a move and has probably been called by MoveThisObjectTo
	//otherwise it is acting as a delete and has probably been called by RemoveObjectsFromStack
	if (howMany == ALL_OBJECTS) {
		howMany = ubNumberOfObjects;
	}

	int numToRemove = min(ubNumberOfObjects, howMany);

	bool allowLBETransfer = false;
	if (numToRemove && UsingNewInventorySystem() == true && pSoldier != NULL) {
		//CHRISL: For New Inventory system.  Are we handling an LBE item FROM a soldier?
		if(IsSlotAnLBESlot(slot) == true) {
			//we know this must be an LBE because of the slot and ubNumberOfObjects
			//but are we taking the LBE out of the pocket or putting it in?

			if (pSoldier->inv[slot].exists() == true) {
				//the object exists and it is an LBE, so we must be taking it out of the pocket
				//since we are moving this LBE, it needs to have the items in its pockets moved
				//from the soldier's inv to the LBE's inv
				MoveItemToLBEItem( pSoldier, slot );
			}
			else {
				//we must be putting it into the pocket, so allow the LBE to
				//transfer its contents into the soldier
				//but first make room by putting anything in the soldier's default LBE pockets into other pockets
				std::vector<INT8> LBESlots;
				GetLBESlots(slot, LBESlots);
				MoveItemsToActivePockets(pSoldier, LBESlots, slot, this);
				allowLBETransfer = true;
			}
		}
	}

	if (destObject) {
		//destObject should be empty especially if numToRemove is 0
		//CHRISL: since destObject should be empty, let's just force it to be empty by initializing it.
		//if (destObject->exists() == true) {
			destObject->initialize();
		//}
		if (numToRemove > 0) {
			//this handles the removal too
			return destObject->AddObjectsToStack(*this, numToRemove, pSoldier, slot, cap, allowLBETransfer);
		}
	}
	else if (numToRemove > 0) {
		for (int x = 0; x < numToRemove; ++x) {
			objectStack.pop_front();
		}
		if (objectStack.empty() == true) {
			//init it so it is not empty, even though it no longer exists
			initialize();
		}
		else {
			ubNumberOfObjects -= numToRemove;
			//ADB ubWeight has been removed, see comments in OBJECTTYPE
			//ubWeight = CalculateObjectWeight(this);
		}
	}

	//returns how many were NOT removed
	return howMany - numToRemove;
}

bool OBJECTTYPE::RemoveObjectAtIndex(unsigned int index, OBJECTTYPE* destObject)
{
	//Note, this function does NOT care what the stack size of destObject will be
	//because destObject will never be an object in a soldier's inventory!
	if (index >= ubNumberOfObjects || exists() == false) {
		return false;
	}

	if (ubNumberOfObjects == 1) {
		if (destObject) {
			*destObject = *this;
		}
		this->initialize();
		return true;
	}

	if (destObject) {
		//destObject should be empty especially if it fails!!
		destObject->initialize();
	}

	StackedObjects::iterator spliceIter = objectStack.begin();
	for (unsigned int x = 0; x < index; ++x) {
		++spliceIter;
	}

	if (destObject) {
		destObject->usItem = usItem;
		destObject->fFlags = fFlags;
		//handles weight calc and ubNumber too
		destObject->SpliceData((*this), 1, spliceIter);
		//destObject->usAttachmentSlotIndexVector = usAttachmentSlotIndexVector;
	}
	else {
		objectStack.erase(spliceIter);
		if (objectStack.empty()) {
			this->initialize();
		}
		else {
			ubNumberOfObjects -= 1;
			//ADB ubWeight has been removed, see comments in OBJECTTYPE
			//ubWeight = CalculateObjectWeight(this);
		}
	}

	return true;
}

StackedObjectData* OBJECTTYPE::operator [](const unsigned int index)
{
	StackedObjects::iterator iter = objectStack.begin();
	if(index >= objectStack.size()){
		return &(*iter);
	}
	Assert(index < objectStack.size());
	for (unsigned int x = 0; x < index; ++x) {
		++iter;
	}
	return &(*iter);
}

const StackedObjectData* OBJECTTYPE::operator [](const unsigned int index) const {
	StackedObjects::const_iterator iter = objectStack.begin();
	if(index >= objectStack.size()){
		return &(*iter);
	}
	Assert(index < objectStack.size());
	for (unsigned int x = 0; x < index; ++x) {
		++iter;
	}
	return &(*iter);
}


//you may have noticed code like this:
//pSoldier->pTempObject	= (OBJECTTYPE *)MemAlloc( sizeof( OBJECTTYPE ) );
//memcpy( pSoldier->pTempObject, pObject, sizeof( OBJECTTYPE ) );
//That's setting yourself up for a memleak if pSoldier->pTempObject isn't null!

//Whenever you see code like that, write this in it's place:
//OBJECTTYPE::CopyToOrCreateAt(&pSoldier->pTempObject, pObject);

//OR write OBJECTTYPE::DeleteMe(&pSoldier->pTempObject) if there is no memcpy
void OBJECTTYPE::CopyToOrCreateAt(OBJECTTYPE** ppTarget, OBJECTTYPE* pSource)
{
	//this is necessary because memcpy is no longer good enough for OBJECTTYPE
	if (*ppTarget == NULL) {
		*ppTarget = new OBJECTTYPE(*pSource);
	}
	else {
		//ADB leaving this in for a while, not sure if the code ever even reaches here and this will tell me
		DebugBreakpoint();
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Found mem leak, but it was corrected."));
		**ppTarget = *pSource;
	}
	return;
}

//OR write OBJECTTYPE::DeleteMe(&pSoldier->pTempObject) if there is no memcpy
void OBJECTTYPE::DeleteMe(OBJECTTYPE** ppObject)
{
	if (*ppObject != NULL) {
		delete *ppObject;
		*ppObject = NULL;
	}
	return;
}

StackedObjectData::StackedObjectData()
{
	initialize();
}

StackedObjectData::~StackedObjectData()
{
	attachments.clear();
}

OBJECTTYPE* StackedObjectData::GetAttachmentAtIndex(UINT8 index)
{
	attachmentList::iterator iter = attachments.begin();
	for (int x = 0; x < index && iter != attachments.end(); ++x) {
		++iter;
	}

	if (iter != attachments.end()) {
		return &(*iter);
	}
	return 0;
}

BOOLEAN StackedObjectData::RemoveAttachmentAtIndex(UINT8 index, attachmentList::iterator * returnIter)
{
	attachmentList::iterator iter = attachments.begin();
	for (int x = 0; iter != attachments.end(); ++x, ++iter) {
		if(x == index){
			if(UsingNewAttachmentSystem()==true){
				OBJECTTYPE null;
				iter = attachments.erase(iter);
				if(returnIter){
					*returnIter = attachments.insert(iter, null);
				} else {
					attachments.insert(iter, null);
				}
				return TRUE;
			} else {
				if(returnIter){
					*returnIter = attachments.erase(iter);
				} else {
					attachments.erase(iter);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

attachmentList::iterator StackedObjectData::RemoveAttachmentAtIter(attachmentList::iterator iter)
{
	if(UsingNewAttachmentSystem()==true){
		OBJECTTYPE null;
		iter = attachments.erase(iter);
		iter = attachments.insert(iter, null);
		return iter;
	} else {
		iter = attachments.erase(iter);
		return iter;
	}
}

BOOLEAN StackedObjectData::AddAttachmentAtIndex(UINT8 index, OBJECTTYPE Attachment)
{
	AssertMsg(UsingNewAttachmentSystem()==true, "Cannot use the AddAttachmentAtIndex function without NAS");

	attachmentList::iterator iter = attachments.begin();
	for (int x = 0; iter != attachments.end(); ++x, ++iter) {
		if(x == index){
			iter = attachments.erase(iter);
			attachments.insert(iter, Attachment);
			return TRUE;
		}
	}
	return FALSE;
}

UINT16 StackedObjectData::AttachmentListSize()
{
	UINT16 attachmentSize = 0;
	for (attachmentList::iterator iter = attachments.begin(); iter != attachments.end(); ++iter) {
		if(iter->exists()){
			attachmentSize++;
		}
	}
	return attachmentSize;
}
bool StackedObjectData::operator==(StackedObjectData& compare)
{
	BOOLEAN pass = FALSE;
	if(this->data.objectStatus == compare.data.objectStatus){
		if(this->data.gun.bGunAmmoStatus == compare.data.gun.bGunAmmoStatus){
			if(this->data.gun.bGunStatus == compare.data.gun.bGunStatus){
				if(this->data.gun.ubGunAmmoType == compare.data.gun.ubGunAmmoType){
					if(this->data.gun.ubGunShotsLeft == compare.data.gun.ubGunShotsLeft){
						if(this->data.gun.ubGunState == compare.data.gun.ubGunState){
							if(this->data.gun.usGunAmmoItem == compare.data.gun.usGunAmmoItem){
								pass = TRUE;
							}
						}
					}
				}
			}
		}
	}
	return(pass && this->attachments == compare.attachments);
	//return (this->data == compare.data
	//	&& this->attachments == compare.attachments);
}

bool StackedObjectData::operator==(const StackedObjectData& compare)const
{
	BOOLEAN pass = FALSE;
	if(this->data.objectStatus == compare.data.objectStatus){
		if(this->data.gun.bGunAmmoStatus == compare.data.gun.bGunAmmoStatus){
			if(this->data.gun.bGunStatus == compare.data.gun.bGunStatus){
				if(this->data.gun.ubGunAmmoType == compare.data.gun.ubGunAmmoType){
					if(this->data.gun.ubGunShotsLeft == compare.data.gun.ubGunShotsLeft){
						if(this->data.gun.ubGunState == compare.data.gun.ubGunState){
							if(this->data.gun.usGunAmmoItem == compare.data.gun.usGunAmmoItem){
								pass = TRUE;
							}
						}
					}
				}
			}
		}
	}
	return(pass && this->attachments == compare.attachments);
	//return (this->data == compare.data
	//	&& this->attachments == compare.attachments);
}

bool ObjectData::operator==(ObjectData& compare)
{
	return (memcmp(this, &compare, sizeof(ObjectData)) == 0);
}

bool ObjectData::operator==(const ObjectData& compare)const
{
	return (memcmp(this, &compare, sizeof(ObjectData)) == 0);
}

ObjectData::ObjectData(const ObjectData& src)
{
	if ((void*)this != (void*)&src) {
		//first get rid of any LBE this might have
		DeleteLBE();

		//copy over the data
		this->bTrap = src.bTrap;
		this->fUsed = src.fUsed;
		this->ubImprintID = src.ubImprintID;
		this->bTemperature = src.bTemperature;
		//copy over the union
		this->gun = src.gun;

		//duplicate the LBE data
		DuplicateLBE();
	}
}

ObjectData& ObjectData::operator =(const ObjectData& src)
{
	if ((void*)this != (void*)&src) {
		//first get rid of any LBE this might have
		DeleteLBE();

		//copy over the data
		this->bTrap = src.bTrap;
		this->fUsed = src.fUsed;
		this->ubImprintID = src.ubImprintID;
		this->bTemperature = src.bTemperature;
		//copy over the union
		this->gun = src.gun;

		//duplicate the LBE data
		DuplicateLBE();
	}
	return *this;
}

ObjectData::~ObjectData()
{
	DeleteLBE();
}

void ObjectData::DeleteLBE()
{
	/*CHRISL: We still need to resolve the incrementing uniqueID issue, but ADB has a valid concern for
	leaving this code in place. We just need to update things so that the uniqueID only increments when we
	actually have to create a new LBENODE. */
	return;
	if (LBEArray.empty() == false) {
		if (this->lbe.bLBE == -1) {
			int uniqueID = this->lbe.uniqueID;
			for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
				if (iter->uniqueID == uniqueID) {
					LBEArray.erase(iter);
					break;
				}
			}
		}
	}
}

void ObjectData::DuplicateLBE()
{
	/*CHRISL: We still need to resolve the incrementing uniqueID issue, but ADB has a valid concern for
	leaving this code in place. We just need to update things so that the uniqueID only increments when we
	actually have to create a new LBENODE. */
	return;
	if (this->lbe.bLBE == -1) {
		LBENODE* pLBE = NULL;
		int uniqueID = this->lbe.uniqueID;
		for (std::list<LBENODE>::iterator iter = LBEArray.begin(); iter != LBEArray.end(); ++iter) {
			if (iter->uniqueID == uniqueID) {
				pLBE = &(*iter);
				break;
			}
		}
		uniqueID = gLastLBEUniqueID++;
		LBEArray.push_back(LBENODE());
		LBENODE* pInserted = &LBEArray.back();
		*pInserted = *pLBE;
		pInserted->uniqueID = uniqueID;
		this->lbe.uniqueID = uniqueID;
	}
}

//dnl ch33 120909
OLD_OBJECTTYPE_101& OLD_OBJECTTYPE_101::operator=(OBJECTTYPE& src)
{
	if((void*)this != (void*)&src)
	{
		memset(this, 0, sizeof(OLD_OBJECTTYPE_101));
		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		this->ubWeight = (UINT8)CalculateObjectWeight(&src);//dnl??? need test, probably this should be calculated after you create old object
		this->fFlags = src.fFlags;
		this->ubMission = src.ubMission;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		this->usItem = src.usItem;
		if(ubNumberOfObjects == 1)
		{
			this->ugYucky.bGunStatus = (INT8)src[0]->data.gun.bGunStatus;
			switch(Item[src.usItem].usItemClass)
			{
			case IC_MONEY:
				this->ugYucky.uiMoneyAmount = src[0]->data.money.uiMoneyAmount;
				break;
			case IC_KEY:
				ugYucky.ubKeyID = src[0]->data.key.ubKeyID;
				break;
			case IC_GRENADE:
			case IC_BOMB:
				this->ugYucky.bDetonatorType = src[0]->data.misc.bDetonatorType;
				this->ugYucky.usBombItem = src[0]->data.misc.usBombItem;
				this->ugYucky.bDelay = src[0]->data.misc.bDelay;
				this->ugYucky.ubBombOwner = src[0]->data.misc.ubBombOwner;
				this->ugYucky.bActionValue = src[0]->data.misc.bActionValue;
				this->ugYucky.ubTolerance = src[0]->data.misc.ubTolerance;
				break;
			default:
				this->ugYucky.ubGunAmmoType = src[0]->data.gun.ubGunAmmoType;
				this->ugYucky.ubGunShotsLeft = (UINT8)src[0]->data.gun.ubGunShotsLeft;
				this->ugYucky.usGunAmmoItem = src[0]->data.gun.usGunAmmoItem;
				this->ugYucky.bGunAmmoStatus = (INT8)src[0]->data.gun.bGunAmmoStatus;
				this->ugYucky.ubGunState = src[0]->data.gun.ubGunState;
				break;
			}
			this->bTrap = src[0]->data.bTrap;
			this->ubImprintID = src[0]->data.ubImprintID;
			this->fUsed = src[0]->data.fUsed;
			if(src.usItem == OWNERSHIP)
			{
				this->ugYucky.ubOwnerProfile = (UINT8)src[0]->data.owner.ubOwnerProfile;
				this->ugYucky.ubOwnerCivGroup = src[0]->data.owner.ubOwnerCivGroup;
			}
			int i = 0;
			for(attachmentList::iterator iter=src[0]->attachments.begin(); iter!=src[0]->attachments.end(); ++iter)
			{
				//dnl??? this part should check if attachment is valid for 1.12, but then means you will lost some stuff from NIV, and if you wnat play map in older 1.13 versions this is not good
				//if(iter->usItem)
				//	;
				this->usAttachItem[i] = iter->usItem;
				this->bAttachStatus[i] = (INT8)(*iter)[0]->data.objectStatus;
				if(++i >= OLD_MAX_ATTACHMENTS_101)
					break;
			}
		}
		else
		{
			memset(&this->ugYucky, 0, sizeof(Version101::OLD_OBJECTTYPE_101_UNION));
			for(int i=0; i<ubNumberOfObjects; i++)
				this->ugYucky.bStatus[i] = (INT8)src[i]->data.objectStatus;
			this->bTrap = src[0]->data.bTrap;
			this->ubImprintID = src[0]->data.ubImprintID;
			this->fUsed = src[0]->data.fUsed;
		}
	}
	return(*this);
}

// Constructor
OBJECTTYPE::OBJECTTYPE()
{
	initialize();
}
void OBJECTTYPE::initialize()
{

	memset(this, 0, SIZEOF_OBJECTTYPE_POD);

	//this is an easy way to init it and get rid of attachments
	objectStack.clear();
	//ubNumberOfObjects should be 0 so any loop checking it will not work
	//however objectStack should always have at least one, because there are so
	//many uses of (*pObj)[0]->data.objectStatus and such
	objectStack.resize(1);
}

bool OBJECTTYPE::operator==(const OBJECTTYPE& compare)const
{
	if ( memcmp(this, &compare, SIZEOF_OBJECTTYPE_POD) == 0) {
		if (this->objectStack == compare.objectStack) {
			return true;
		}
	}
	return false;
}

bool OBJECTTYPE::operator==(OBJECTTYPE& compare)
{
	if ( memcmp(this, &compare, SIZEOF_OBJECTTYPE_POD) == 0) {
		if (this->objectStack == compare.objectStack) {
			return true;
		}
		if((*this)[0]->operator == (*compare.operator[](0)))
			return true;
	}
	return false;
}

//Copy Ctor
OBJECTTYPE::OBJECTTYPE(const OBJECTTYPE& src)
{
	if ((void*)this != (void*)&src) {
		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = src.ubWeight;
		this->fFlags = src.fFlags;
		this->ubMission = src.ubMission;
		this->objectStack = src.objectStack;
		//this->usAttachmentSlotIndexVector = src.usAttachmentSlotIndexVector;
	}
}

// Assignment operator
OBJECTTYPE& OBJECTTYPE::operator=(const OBJECTTYPE& src)
{
	if ((void*)this != (void*)&src) {
		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = src.ubWeight;
		this->fFlags = src.fFlags;
		this->ubMission = src.ubMission;
		this->objectStack = src.objectStack;
		//this->usAttachmentSlotIndexVector = src.usAttachmentSlotIndexVector;
	}
	return *this;
}


// Conversion operator
OBJECTTYPE& OBJECTTYPE::operator=(const OLD_OBJECTTYPE_101& src)
{
	if ((void*)this != (void*)&src) {
		//makes changes to size easier as we don't have to init new arrays with 0 by hand
		this->initialize();

		this->usItem = src.usItem;
		this->ubNumberOfObjects = src.ubNumberOfObjects;
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = src.ubWeight;
		this->fFlags = src.fFlags;

		this->ubMission = src.ubMission;

		//in some cases we need to reference the objectStatus or something, even though the item is totally empty
		//therefore, keep ubNumberOfObjects at 0 but resize objectStack to at least 1
		this->objectStack.resize(max(ubNumberOfObjects, 1));
		if (ubNumberOfObjects == 0) {
			this->usItem = NONE;
		}

		//and now the big change, the union
		//copy the old data, making sure not to write over, since the old size is actually 9 bytes
		if (ubNumberOfObjects == 1) {
			//CHRISL: Instead of a memcpy, copy values individually so we can account for the larger union size
			//memcpy(&((*this)[0]->data.gun), &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,sizeof(ObjectData)));
			//Start by setting status as this is the same for every structure in the union
			(*this)[0]->data.gun.bGunStatus = src.ugYucky.bGunStatus;
			//Next, clear the rest of the union so we are working with a clean setup.  I'm clearing gun simply because it's
			//	the largest structure and should therefore clear all values.  There may be a better way to do this.
			(*this)[0]->data.gun.ubGunAmmoType = 0;
			(*this)[0]->data.gun.ubGunShotsLeft = 0;
			(*this)[0]->data.gun.usGunAmmoItem = 0;
			(*this)[0]->data.gun.bGunAmmoStatus = 0;
			(*this)[0]->data.gun.ubGunState = 0;
			//Lastly, convert values from the old format to the new based on the type of object
			switch(Item[src.usItem].usItemClass)
			{
			case IC_MONEY:
				(*this)[0]->data.money.uiMoneyAmount = src.ugYucky.uiMoneyAmount;
				break;
			case IC_KEY:
				(*this)[0]->data.key.ubKeyID = src.ugYucky.ubKeyID;
				break;
			case IC_GRENADE:
			case IC_BOMB:
				(*this)[0]->data.misc.bDetonatorType = src.ugYucky.bDetonatorType;
				(*this)[0]->data.misc.usBombItem = src.ugYucky.usBombItem;
				(*this)[0]->data.misc.bDelay = src.ugYucky.bDelay;	// includes bFrequency
				(*this)[0]->data.misc.ubBombOwner = src.ugYucky.ubBombOwner;
				(*this)[0]->data.misc.bActionValue = src.ugYucky.bActionValue;
				(*this)[0]->data.misc.ubTolerance = src.ugYucky.ubTolerance;	// includes ubLocationID
				break;
			default:
				//This should cover all other possibilities since only Money, Key and "Bomb/Misc" have layouts that don't
				//	exactly line up with gun
				(*this)[0]->data.gun.ubGunAmmoType = src.ugYucky.ubGunAmmoType;
				(*this)[0]->data.gun.ubGunShotsLeft = src.ugYucky.ubGunShotsLeft;
				(*this)[0]->data.gun.usGunAmmoItem = src.ugYucky.usGunAmmoItem;
				(*this)[0]->data.gun.bGunAmmoStatus = src.ugYucky.bGunAmmoStatus;
				(*this)[0]->data.gun.ubGunState = src.ugYucky.ubGunState;
				break;
			}

			(*this)[0]->data.bTrap = src.bTrap;		// 1-10 exp_lvl to detect
			(*this)[0]->data.ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
			(*this)[0]->data.fUsed = src.fUsed;				// flags for whether the item is used or not
			(*this)[0]->data.bTemperature = 0.0;

			if(src.usItem == OWNERSHIP)//dnl ch29 120909
			{
				(*this)[0]->data.owner.ubOwnerProfile = src.ugYucky.ubOwnerProfile;
				(*this)[0]->data.owner.ubOwnerCivGroup = src.ugYucky.ubOwnerCivGroup;
			}

			//it's unlikely max will get less over the versions, but still, check the min
			for (int x = 0; x < OLD_MAX_ATTACHMENTS_101; ++x)
			{
				if (src.usAttachItem[x] != NOTHING) {
					CreateItem(src.usAttachItem[x], src.bAttachStatus[x], &gTempObject);
					(*this)[0]->attachments.push_back(gTempObject);
				}
			}
		}
		else {
			//we are loading a stack of objects, the union must either be
			//bStatus[OLD_MAX_OBJECTS_PER_SLOT_101] or
			//ubShotsLeft[OLD_MAX_OBJECTS_PER_SLOT_101]
			//and we know it has no attachments

			Version101::OLD_OBJECTTYPE_101_UNION ugYucky;
			memcpy(&ugYucky, &src.ugYucky, __min(SIZEOF_OLD_OBJECTTYPE_101_UNION,sizeof(ObjectData)));

			for (int x = 0; x < max(ubNumberOfObjects, 1); ++x) {
				(*this)[x]->data.objectStatus = ugYucky.bStatus[x];
				(*this)[x]->data.bTrap = src.bTrap;		// 1-10 exp_lvl to detect
				(*this)[x]->data.ubImprintID = src.ubImprintID;	// ID of merc that item is imprinted on
				(*this)[x]->data.fUsed = src.fUsed;				// flags for whether the item is used or not
				(*this)[x]->data.bTemperature = 0.0;
			}
		}

		//CHRISL: Deal with NAS
/*		if(UsingNewAttachmentSystem()==true){
			for(int x = 0; x < max(ubNumberOfObjects, 1); ++x) {
				std::vector<UINT16>	usAttachmentSlotIndexVector = GetItemSlots(this, x);
				if((*this)[x]->attachments.size() != usAttachmentSlotIndexVector.size())
					(*this)[x]->attachments.resize(usAttachmentSlotIndexVector.size());
			}
		}*/
		RemoveProhibitedAttachments(NULL, this, this->usItem);
		//just a precaution
		//ADB ubWeight has been removed, see comments in OBJECTTYPE
		//this->ubWeight = CalculateObjectWeight(this);
	}
	return *this;
}


OBJECTTYPE::~OBJECTTYPE()
{
	objectStack.clear();
}
