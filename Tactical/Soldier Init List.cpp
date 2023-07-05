#include "builddefines.h"
#include <stdio.h>
#include <memory.h>

#include "types.h"

#include "strategicmap.h"
#include "overhead.h"
#include "isometric utils.h"

#include "soldier add.h"
#include "soldier create.h"
#include "Soldier Init List.h"
#include "debug.h"
#include "Random.h"
#include "items.h"
#include "GameSettings.h"


#include "Map Information.h"
#include "soldier profile.h"
#include "Sys Globals.h"
#include "EditorMercs.h"
#include "Animation Data.h"
#include "message.h"
#include "Font Control.h"
#include "Sound Control.h"
#include "Quests.h"
#include "Render Fun.h"
#include "meanwhile.h"
#include "Map Screen Interface Map.h"
#include "mapscreen.h"
#include "Debug Control.h"
#include "Inventory Choosing.h"
#include "Strategic AI.h"
#include "strategic.h"
#include "Campaign Types.h"
#include "ai.h"
#include "NPC.h"
#include "Scheduling.h"
#include "opplist.h"
#include "MessageBoxScreen.h"
#include "screenids.h"
#include "SaveLoadScreen.h"
#include "Rotting Corpses.h"

#include "connect.h"
#include "Map Edgepoints.h"
#include "Queen Command.h"

#include "Map Edgepoints.h"
#include "Campaign.h"			// added by Flugente for HighestPlayerProgressPercentage()
#include "CampaignStats.h"		// added by Flugente
#include "Town Militia.h"		// added by Flugente
#include "PreBattle Interface.h"	// added by Flugente
#include "LuaInitNPCs.h"		// added by Flugente
#include "Soldier macros.h"		// added by Flugente
#include "Creature Spreading.h"	// added by Flugente


BOOLEAN gfOriginalList = TRUE;

SOLDIERINITNODE *gSoldierInitHead = NULL;
SOLDIERINITNODE *gSoldierInitTail = NULL;

SOLDIERINITNODE *gOriginalSoldierInitListHead = NULL;
SOLDIERINITNODE *gAlternateSoldierInitListHead = NULL;

#ifdef JA2BETAVERSION
BOOLEAN ValidateSoldierInitLinks( UINT8 ubCode );
BOOLEAN gfDoDialogOnceGameScreenFadesIn = FALSE;
#endif

UINT32 CountNumberOfNodesWithSoldiers()
{
	SOLDIERINITNODE *curr;
	UINT32 num = 0;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier )
		{
			num++;
		}
		curr = curr->next;
	}
	return num;
}

void SortSoldierInitList();

void InitSoldierInitList()
{
	if( gSoldierInitHead )
		KillSoldierInitList();
	gSoldierInitHead = NULL;
	gSoldierInitTail = NULL;
}

void KillSoldierInitList()
{
	while( gSoldierInitHead )
		RemoveSoldierNodeFromInitList( gSoldierInitTail );
	if( gfOriginalList )
		gOriginalSoldierInitListHead = NULL;
	else
		gAlternateSoldierInitListHead = NULL;
}

SOLDIERINITNODE* AddBasicPlacementToSoldierInitList ( BASIC_SOLDIERCREATE_STRUCT *pBasicPlacement )
{
	SOLDIERINITNODE *curr;
	//Allocate memory for node
	curr = (SOLDIERINITNODE*)MemAlloc( sizeof (SOLDIERINITNODE ) );
	Assert( curr );
	memset( curr, 0, sizeof( SOLDIERINITNODE ) );

	//Allocate memory for basic placement
	curr->pBasicPlacement = ( BASIC_SOLDIERCREATE_STRUCT* )MemAlloc( sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	if( !curr->pBasicPlacement )
	{
		AssertMsg( 0, "Failed to allocate memory for AddBasicPlacementToSoldierInitList." );
		return NULL;
	}

	//Copy memory for basic placement
	memcpy( curr->pBasicPlacement, pBasicPlacement, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );

	//It is impossible to set up detailed placement stuff now.
	//If there is any detailed placement information during map load, it will be added
	//immediately after this function call.
	curr->pDetailedPlacement = NULL;
	curr->pSoldier = NULL;

	//Insert the new node in the list in its proper place.
	if( !gSoldierInitHead )
	{
		gSoldierInitHead = curr;
		if( gfOriginalList )
			gOriginalSoldierInitListHead = curr;
		else
			gAlternateSoldierInitListHead = curr;
		gSoldierInitTail = curr;
		gSoldierInitHead->next = NULL;
		gSoldierInitHead->prev = NULL;
	}
	else
	{
		//TEMP:	no sorting, just enemies
		curr->prev = gSoldierInitTail;
		curr->next = NULL;
		gSoldierInitTail->next = curr;
		gSoldierInitTail = gSoldierInitTail->next;
	}
	if( gfOriginalList )
		gMapInformation.ubNumIndividuals++;
	return curr;
}

void RemoveSoldierNodeFromInitList( SOLDIERINITNODE *pNode )
{
	if( !pNode )
		return;
	if( gfOriginalList )
		gMapInformation.ubNumIndividuals--;
	if( pNode->pBasicPlacement )
	{
		MemFree( pNode->pBasicPlacement );
		pNode->pBasicPlacement = NULL;
	}
	if( pNode->pDetailedPlacement )
	{
		delete( pNode->pDetailedPlacement );
		pNode->pDetailedPlacement = NULL;
	}
	if( pNode->pSoldier )
	{
		if( pNode->pSoldier->ubID >= 20 )
		{
			TacticalRemoveSoldier( pNode->pSoldier->ubID );
		}
		else
		{
			//bug if gets here and we aren't exiting the game, maybe a bug even if we are exiting (but who cares)
			//bug exists since beginning of SVN
			DebugBreakpoint();
			//INT8 bug = 0;
		}
	}
	if( pNode == gSoldierInitHead )
	{
		gSoldierInitHead = gSoldierInitHead->next;
		if( gSoldierInitHead )
			gSoldierInitHead->prev = NULL;
		if( gfOriginalList )
			gOriginalSoldierInitListHead = gSoldierInitHead;
		else
			gAlternateSoldierInitListHead = gSoldierInitHead;
	}
	else if( pNode == gSoldierInitTail )
	{
		gSoldierInitTail = gSoldierInitTail->prev;
		gSoldierInitTail->next = NULL;
	}
	else
	{
		pNode->prev->next = pNode->next;
		pNode->next->prev = pNode->prev;
	}
	MemFree( pNode );
}

//These serialization functions are assuming the passing of a valid file
//pointer to the beginning of the save/load area, which is not necessarily at
//the beginning of the file.	This is just a part of the whole map serialization.
//dnl ch42 250909
BASIC_SOLDIERCREATE_STRUCT& BASIC_SOLDIERCREATE_STRUCT::operator=(const _OLD_BASIC_SOLDIERCREATE_STRUCT& src)
{
	if((void*)this != (void*)&src)
	{
		fDetailedPlacement = src.fDetailedPlacement;
		usStartingGridNo = src.sStartingGridNo;
		bTeam = src.bTeam;
		bRelativeAttributeLevel = src.bRelativeAttributeLevel;
		bRelativeEquipmentLevel = src.bRelativeEquipmentLevel;
		ubDirection = src.ubDirection;
		bOrders = src.bOrders;
		bAttitude = src.bAttitude;
		ubBodyType = src.ubBodyType;
		bPatrolCnt = src.bPatrolCnt;
		fOnRoof = src.fOnRoof;
		ubSoldierClass = src.ubSoldierClass;
		ubCivilianGroup = src.ubCivilianGroup;
		fPriorityExistance = src.fPriorityExistance;
		fHasKeys = src.fHasKeys;
		TranslateArrayFields(sPatrolGrid, src.sPatrolGrid, OLD_MAXPATROLGRIDS, INT16_INT32);
	}
	return(*this);
}

BOOLEAN BASIC_SOLDIERCREATE_STRUCT::Load(INT8** hBuffer, FLOAT dMajorMapVersion)
{
	if(dMajorMapVersion < 7.0)
	{
		_OLD_BASIC_SOLDIERCREATE_STRUCT OldBasicSoldierCreateStruct;
		LOADDATA(&OldBasicSoldierCreateStruct, *hBuffer, sizeof(_OLD_BASIC_SOLDIERCREATE_STRUCT));
		*this = OldBasicSoldierCreateStruct;
	}
	else
		LOADDATA(this, *hBuffer, sizeof(BASIC_SOLDIERCREATE_STRUCT));
	return(TRUE);
}

BOOLEAN BASIC_SOLDIERCREATE_STRUCT::Save(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	PTR pData = this;
	UINT32 uiBytesToWrite = sizeof(BASIC_SOLDIERCREATE_STRUCT);
	_OLD_BASIC_SOLDIERCREATE_STRUCT OldBasicSoldierCreateStruct;
	if(dMajorMapVersion == VANILLA_MAJOR_MAP_VERSION && ubMinorMapVersion == VANILLA_MINOR_MAP_VERSION)
	{
		memset(&OldBasicSoldierCreateStruct, 0, sizeof(_OLD_BASIC_SOLDIERCREATE_STRUCT));
		OldBasicSoldierCreateStruct.fDetailedPlacement = fDetailedPlacement;
		OldBasicSoldierCreateStruct.sStartingGridNo = usStartingGridNo;
		OldBasicSoldierCreateStruct.bTeam = bTeam;
		OldBasicSoldierCreateStruct.bRelativeAttributeLevel = bRelativeAttributeLevel;
		OldBasicSoldierCreateStruct.bRelativeEquipmentLevel = bRelativeEquipmentLevel;
		OldBasicSoldierCreateStruct.ubDirection = ubDirection;
		OldBasicSoldierCreateStruct.bOrders = bOrders;
		OldBasicSoldierCreateStruct.bAttitude = bAttitude;
		OldBasicSoldierCreateStruct.ubBodyType = ubBodyType;
		OldBasicSoldierCreateStruct.bPatrolCnt = bPatrolCnt;
		OldBasicSoldierCreateStruct.fOnRoof = fOnRoof;
		OldBasicSoldierCreateStruct.ubSoldierClass = ubSoldierClass;
		OldBasicSoldierCreateStruct.ubCivilianGroup = ubCivilianGroup;
		OldBasicSoldierCreateStruct.fPriorityExistance = fPriorityExistance;
		OldBasicSoldierCreateStruct.fHasKeys = fHasKeys;
		TranslateArrayFields(OldBasicSoldierCreateStruct.sPatrolGrid, sPatrolGrid, OLD_MAXPATROLGRIDS, INT32_INT16);
		pData = &OldBasicSoldierCreateStruct;
		uiBytesToWrite = sizeof(_OLD_BASIC_SOLDIERCREATE_STRUCT);
	}
	UINT32 uiBytesWritten = 0;
	FileWrite(hFile, pData, uiBytesToWrite, &uiBytesWritten);
	if(uiBytesToWrite == uiBytesWritten)
		return(TRUE);
	return(FALSE);
}

BOOLEAN LoadSoldiersFromMap(INT8** hBuffer, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	BASIC_SOLDIERCREATE_STRUCT tempBasicPlacement;
	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	SOLDIERINITNODE *pNode;
	UINT16 ubNumIndividuals = gMapInformation.ubNumIndividuals;
	BOOLEAN fCowInSector = FALSE;
	UseEditorAlternateList();
	KillSoldierInitList();
	UseEditorOriginalList();
	KillSoldierInitList();
	InitSoldierInitList();
	if(ubNumIndividuals > MAX_INDIVIDUALS)
	{
		AssertMsg(0, "Corrupt map check failed.	ubNumIndividuals is greater than MAX_INDIVIDUALS.");
		return(FALSE);// Too many mercs
	}
	if(!ubNumIndividuals)
		return(TRUE);// No mercs
	// Because we are loading the map, we needed to know how many guys are being loaded, but when we add them to the list here, it automatically increments that number, effectively doubling it, which would be a problem. Now that we know the number, we clear it here, so it gets built again.
	gMapInformation.ubNumIndividuals = 0;// MUST BE CLEARED HERE!!!
	for(UINT16 cnt=0; cnt<ubNumIndividuals; cnt++)
	{
		tempBasicPlacement.Load(hBuffer, dMajorMapVersion);
		for(int i=0; i<MAXPATROLGRIDS; i++)//dnl ch44 290909 BASIC_SOLDIERCREATE_STRUCT translation
			gMapTrn.GetTrnCnt(tempBasicPlacement.sPatrolGrid[i]);
		gMapTrn.GetTrnCnt(tempBasicPlacement.usStartingGridNo);
		pNode = AddBasicPlacementToSoldierInitList(&tempBasicPlacement);
		if(!pNode)
		{
			AssertMsg(0, "Failed to allocate memory for new basic placement in LoadSoldiersFromMap.");
			return(FALSE);
		}
		Assert(cnt < 256);
		pNode->ubNodeID = (UINT8)cnt;
		if(tempBasicPlacement.fDetailedPlacement)
		{
			// Add the static detailed placement information in the same newly created node as the basic placement. Read static detailed placement from file.
			if(!tempDetailedPlacement.Load(hBuffer, dMajorMapVersion, ubMinorMapVersion))
				return(FALSE);
			for(int i=0; i<MAXPATROLGRIDS; i++)//dnl ch44 290909 SOLDIERCREATE_STRUCT translation
				gMapTrn.GetTrnCnt(tempDetailedPlacement.sPatrolGrid[i]);
			gMapTrn.GetTrnCnt(tempDetailedPlacement.sInsertionGridNo);
			// Allocate memory for new static detailed placement
			pNode->pDetailedPlacement = new SOLDIERCREATE_STRUCT(tempDetailedPlacement);
			if(!pNode->pDetailedPlacement)
			{
				AssertMsg(0, "Failed to allocate memory for new detailed placement in LoadSoldiersFromMap.");
				return(FALSE);
			}
			if(tempDetailedPlacement.ubProfile != NO_PROFILE)
			{
				pNode->pDetailedPlacement->ubCivilianGroup = gMercProfiles[tempDetailedPlacement.ubProfile].ubCivilianGroup;
				pNode->pBasicPlacement->ubCivilianGroup = gMercProfiles[tempDetailedPlacement.ubProfile].ubCivilianGroup;
			}
		}
		if(tempBasicPlacement.ubBodyType == COW)
			fCowInSector = TRUE;
	}
	if(fCowInSector)
	{
		CHAR8 str[40];
		sprintf(str, "Sounds\\\\cowmoo%d.wav", Random(3)+1);
		PlayJA2SampleFromFile(str, RATE_11025, MIDVOLUME, 1, MIDDLEPAN);
	}
	return(TRUE);
}

BOOLEAN SaveSoldiersToMap(HWFILE hFile, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)
{
	UINT32 i;
	SOLDIERINITNODE *curr;
	if(gMapInformation.ubNumIndividuals > MAX_INDIVIDUALS)
		return(FALSE);
	// If we are perhaps in the alternate version of the editor, we don't want bad things to happen. This is probably the only place I know where the user gets punished now. If the person was in the alternate editor mode, then decided to save the game, the current mercs may not be there. This would be bad.	What we do is override any merc editing done while in this mode, and kill them all, while replacing them with the proper ones. Not only that, the alternate editing mode is turned off, and if intentions are to play the game, the user will be facing many enemies!
#ifdef JA2EDITOR
	if(!gfOriginalList)
		ResetAllMercPositions();
#endif
	curr = gSoldierInitHead;
	for(i=0; i<gMapInformation.ubNumIndividuals; i++)
	{
		if(!curr)
			return(FALSE);
		curr->ubNodeID = (UINT8)i;
		curr->pBasicPlacement->Save(hFile, dMajorMapVersion, ubMinorMapVersion);
		if(curr->pBasicPlacement->fDetailedPlacement)
		{
			if(!curr->pDetailedPlacement)
				return(FALSE);
			if(!curr->pDetailedPlacement->Save(hFile, TRUE, dMajorMapVersion, ubMinorMapVersion))
				return(FALSE);
		}
		curr = curr->next;
	}
	return(TRUE);
}

//Because soldiers, creatures, etc., maybe added to the game at anytime theoretically, the
//list will need to be sorted to reflect this.	It is quite likely that this won't be needed,
//but the flexibility is there just incase.	Now the list is sorted in the following manner:
//-1st priority:	Any nodes containing valid pointers to soldiers are moved to the end of the list.
//								We don't ever want to use two identical placements.
//-2nd priority:	Any nodes that have priority existance and detailed placement information are
//								put first in the list.
//-3rd priority:	Any nodes that have priority existance and no detailed placement information are used next.
//-4th priority:	Any nodes that have detailed placement and no priority existance information are used next.
//-5th priority:	The rest of the nodes are basic placements and are placed in the center of the list.	Of
//								these, they are randomly filled based on the number needed.
//NOTE:	This function is called by AddSoldierInitListTeamToWorld().	There is no other place it needs to
//			be called.
void SortSoldierInitList()
{
	SOLDIERINITNODE *temp, *curr;

	BOOLEAN	fFredoAtStart = FALSE;
	BOOLEAN	fFredoAtEnd = FALSE;

	if( !gSoldierInitHead )
		return;

	curr = gSoldierInitHead;
	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == FREDO )
		{
			fFredoAtStart = TRUE;
			break;
		}
		curr = curr->next;
	}

	//1st priority sort
	curr = gSoldierInitTail;
	while( curr )
	{
		if( curr->pSoldier && curr != gSoldierInitTail )
		{	//This node has an existing soldier, so move to end of list.
			//copy node
			temp = curr;
			if( temp == gSoldierInitHead )
			{ //If we dealing with the head, we need to move it now.
				gSoldierInitHead = gSoldierInitHead->next;
				if( gfOriginalList )
					gOriginalSoldierInitListHead = gSoldierInitHead;
				else
					gAlternateSoldierInitListHead = gSoldierInitHead;
				gSoldierInitHead->prev = NULL;
				temp->next = NULL;
			}
			curr = curr->prev;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to end of list
			temp->prev = gSoldierInitTail;
			temp->next = NULL;
			gSoldierInitTail->next = temp;
			gSoldierInitTail = temp;
		}
		else
		{
			curr = curr->prev;
		}
	}
	//4th -- put to start
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && !curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement && curr != gSoldierInitHead )
		{	//Priority existance nodes without detailed placement info are moved to beginning of list
			//copy node
			temp = curr;
			if( temp == gSoldierInitTail )
			{ //If we dealing with the tail, we need to move it now.
				gSoldierInitTail = gSoldierInitTail->prev;
				gSoldierInitTail->next = NULL;
				temp->prev = NULL;
			}
			curr = curr->next;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to beginning of list
			temp->prev = NULL;
			temp->next = gSoldierInitHead;
			gSoldierInitHead->prev = temp;
			gSoldierInitHead = temp;
			if( gfOriginalList )
				gOriginalSoldierInitListHead = gSoldierInitHead;
			else
				gAlternateSoldierInitListHead = gSoldierInitHead;
		}
		else
		{
			curr = curr->next;
		}
	}
	//3rd priority sort (see below for reason why we do 2nd after 3rd)
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->fPriorityExistance && !curr->pDetailedPlacement && curr != gSoldierInitHead )
		{	//Priority existance nodes without detailed placement info are moved to beginning of list
			//copy node
			temp = curr;
			if( temp == gSoldierInitTail )
			{ //If we dealing with the tail, we need to move it now.
				gSoldierInitTail = gSoldierInitTail->prev;
				gSoldierInitTail->next = NULL;
				temp->prev = NULL;
			}
			curr = curr->next;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to beginning of list
			temp->prev = NULL;
			temp->next = gSoldierInitHead;
			gSoldierInitHead->prev = temp;
			gSoldierInitHead = temp;
			if( gfOriginalList )
				gOriginalSoldierInitListHead = gSoldierInitHead;
			else
				gAlternateSoldierInitListHead = gSoldierInitHead;
		}
		else
		{
			curr = curr->next;
		}
	}
	//2nd priority sort (by adding these to the front, it'll be before the
	//3rd priority sort.	This is why we do it after.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement && curr != gSoldierInitHead )
		{	//Priority existance nodes are moved to beginning of list
			//copy node
			temp = curr;
			if( temp == gSoldierInitTail )
			{ //If we dealing with the tail, we need to move it now.
				gSoldierInitTail = gSoldierInitTail->prev;
				gSoldierInitTail->next = NULL;
				temp->prev = NULL;
			}
			curr = curr->next;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to beginning of list
			temp->prev = NULL;
			temp->next = gSoldierInitHead;
			gSoldierInitHead->prev = temp;
			gSoldierInitHead = temp;
			if( gfOriginalList )
				gOriginalSoldierInitListHead = gSoldierInitHead;
			else
				gAlternateSoldierInitListHead = gSoldierInitHead;
		}
		else
		{
			curr = curr->next;
		}
	}
	//4th priority sort
	//Done!	If the soldier existing slots are at the end of the list and the
	//			priority placements are at the beginning of the list, then the
	//			basic placements are in the middle.

	curr = gSoldierInitHead;
	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == FREDO )
		{
			fFredoAtEnd = TRUE;
			break;
		}
		curr = curr->next;
	}

}

extern FLOAT gAmbushRadiusModifier;

BOOLEAN AddPlacementToWorld( SOLDIERINITNODE *curr, GROUP *pGroup = NULL )
{
	UINT8 ubProfile;
	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	SOLDIERTYPE *pSoldier;
	UINT8 ubID;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld"));
	// First check if this guy has a profile and if so check his location such that it matches!
	// Get profile from placement info

	if ( ARMED_VEHICLE( curr->pBasicPlacement ) )
	{
		while (1)
		{
			ROTTING_CORPSE *pCorpse = GetCorpseAtGridNo( curr->pBasicPlacement->usStartingGridNo, 0); // I assume we don't find tanks on the roof
			if (pCorpse)
			{
				// Assume this is a dead tank and have the replacement tank haul it away
				RemoveCorpse( pCorpse->iID);
			}
			else
			{
				break;
			}
		}
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: decide on placement"));
	if( curr->pDetailedPlacement )
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: already have detailed placement"));
		ubProfile = curr->pDetailedPlacement->ubProfile;

		if( ubProfile != NO_PROFILE && !gfEditMode )
		{
			if( gMercProfiles[ ubProfile ].sSectorX != gWorldSectorX ||
				gMercProfiles[ ubProfile ].sSectorY != gWorldSectorY ||
				gMercProfiles[ ubProfile ].bSectorZ != gbWorldSectorZ ||
				gMercProfiles[ ubProfile ].ubMiscFlags & (PROFILE_MISC_FLAG_RECRUITED | PROFILE_MISC_FLAG_EPCACTIVE) ||
				//				gMercProfiles[ ubProfile ].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR ||
				!gMercProfiles[ ubProfile ].bLife	||
				gMercProfiles[ ubProfile ].fUseProfileInsertionInfo 
				)
			{
				return FALSE;
			}
		}
		//Special case code when adding icecream truck.
		if( !gfEditMode )
		{
			// CJC, August 18, 1999: don't do this code unless the ice cream truck is on our team
			if ( FindSoldierByProfileID( ICECREAMTRUCK, TRUE ) != NULL )
			{
				if( curr->pDetailedPlacement->ubBodyType == ICECREAMTRUCK )
				{ //Check to see if Hamous is here and not recruited.	If so, add truck
					if( gMercProfiles[ HAMOUS ].sSectorX != gWorldSectorX ||
						gMercProfiles[ HAMOUS ].sSectorY != gWorldSectorY ||
						gMercProfiles[ HAMOUS ].bSectorZ )
					{ //not here, so don't add
						return TRUE;
					}
					//Hamous is here.	Check to make sure he isn't recruited.
					if( gMercProfiles[ HAMOUS ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
					{
						return TRUE;
					}
				}
			}
		}
		CreateDetailedPlacementGivenStaticDetailedPlacementAndBasicPlacementInfo(
			&tempDetailedPlacement, curr->pDetailedPlacement, curr->pBasicPlacement, gWorldSectorX, gWorldSectorY );
	}
	else
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: create detailed placement"));
		CreateDetailedPlacementGivenBasicPlacementInfo( &tempDetailedPlacement, curr->pBasicPlacement, gWorldSectorX, gWorldSectorY );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: if not edit mode"));
	if ( !gfEditMode )
	{
		if( tempDetailedPlacement.bTeam == CIV_TEAM )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: set quest overrides"));
			// quest-related overrides
			if ( gWorldSectorX == gModSettings.ubPornShopTonySectorX && gWorldSectorY == gModSettings.ubPornShopTonySectorY && gbWorldSectorZ == gModSettings.ubPornShopTonySectorZ )
			{
				//DBrot: More rooms
				//UINT8	ubRoom;
				UINT16 usRoom;

				// Kinpin guys might be guarding Tony
				if ( tempDetailedPlacement.ubCivilianGroup == KINGPIN_CIV_GROUP && ( gTacticalStatus.fCivGroupHostile[ KINGPIN_CIV_GROUP ] == CIV_GROUP_WILL_BECOME_HOSTILE || ( (gubQuest[ QUEST_KINGPIN_MONEY ] == QUESTINPROGRESS) && (CheckFact( FACT_KINGPIN_CAN_SEND_ASSASSINS, KINGPIN )) ) ) )
				{
					if (tempDetailedPlacement.ubProfile == NO_PROFILE)
					{//dnl!!!
						// these guys should be guarding Tony!
						// sevenfm: PreRandom(1) always returns 0, use PreRandom(2) instead
						tempDetailedPlacement.sInsertionGridNo = gModSettings.iPornShopEntranceGridNo +
							//(INT16)(PreRandom(8) * (PreRandom(1) ? -1 : 1) + PreRandom(8) * (PreRandom(1) ? -1 : 1) * WORLD_ROWS);
							(INT16)(PreRandom(8) * (PreRandom(2) ? -1 : 1) + PreRandom(8) * (PreRandom(2) ? -1 : 1) * WORLD_ROWS);

						switch( PreRandom( 3 ) )
						{
						case 0:
							tempDetailedPlacement.bOrders = ONGUARD;
							break;
						case 1:
							tempDetailedPlacement.bOrders = CLOSEPATROL;
							break;
						case 2:
							tempDetailedPlacement.bOrders = ONCALL;
							break;

						}
					}
					else if (tempDetailedPlacement.ubProfile == BILLY )
					{//dnl!!!
						// billy should now be able to roam around
						// sevenfm: PreRandom(1) always returns 0, use PreRandom(2) instead
						tempDetailedPlacement.sInsertionGridNo = gModSettings.iPornShopEntranceGridNo +
							//(INT16)(PreRandom(30) * (PreRandom(1) ? -1 : 1) + PreRandom(30) * (PreRandom(1) ? -1 : 1) * WORLD_ROWS);
							(INT16)(PreRandom(30) * (PreRandom(2) ? -1 : 1) + PreRandom(30) * (PreRandom(2) ? -1 : 1) * WORLD_ROWS);
						tempDetailedPlacement.bOrders = SEEKENEMY;
					}
					else if ( tempDetailedPlacement.ubProfile == MADAME )
					{
						// she shouldn't be here!
						return( TRUE );
					}
					else if ( tempDetailedPlacement.ubProfile == NO_PROFILE && InARoom( tempDetailedPlacement.sInsertionGridNo, &usRoom ) && IN_BROTHEL( usRoom ) )
					{
						// must be a hooker, shouldn't be there
						return( TRUE );
					}
				}
			}
#ifdef JA2UB
//Ja25: no queen
#else
			else if ( !gfInMeanwhile && gWorldSectorX == gMercProfiles[ QUEEN ].sSectorX && gWorldSectorY == gMercProfiles[ QUEEN ].sSectorY && gbWorldSectorZ == gMercProfiles[ QUEEN ].bSectorZ )
			{ //Special civilian setup for queen's palace.
				if( gubFact[ FACT_QUEEN_DEAD ] )
				{
					if( tempDetailedPlacement.ubCivilianGroup == QUEENS_CIV_GROUP )
					{ //The queen's civs aren't added if queen is dead
						return TRUE;
					}
				}
				else
				{
					if( gfUseAlternateQueenPosition && tempDetailedPlacement.ubProfile == QUEEN )
					{
						tempDetailedPlacement.sInsertionGridNo = gModSettings.iQueenAlternateGridNo;//dnl!!!
						tempDetailedPlacement.bSectorZ = 0;
					}
					if( tempDetailedPlacement.ubCivilianGroup != QUEENS_CIV_GROUP )
					{ //The free civilians aren't added if queen is alive
						return TRUE;
					}
				}
			}
#endif
			else if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y	&& gbWorldSectorZ == 0 )
			{
				// Tixa prison, once liberated, should not have any civs without profiles unless
				// they are kids
				if( !StrategicMap[CALCULATE_STRATEGIC_INDEX(TIXA_SECTOR_X, TIXA_SECTOR_Y)].fEnemyControlled && tempDetailedPlacement.ubProfile == NO_PROFILE && tempDetailedPlacement.ubBodyType != HATKIDCIV && tempDetailedPlacement.ubBodyType != KIDCIV )
				{
					// not there
					return( TRUE );
				}
			}
			else if ( gWorldSectorX == gMercProfiles[ DOREEN ].sSectorX && gWorldSectorY == gMercProfiles[ DOREEN ].sSectorY && gbWorldSectorZ == gMercProfiles[ DOREEN ].bSectorZ )
			{
				if ( CheckFact( FACT_KIDS_ARE_FREE, 0 ) )
				{
					if ( tempDetailedPlacement.ubBodyType == HATKIDCIV || tempDetailedPlacement.ubBodyType == KIDCIV )
					{
						// not there any more!	kids have been freeeeed!
						return( TRUE );
					}
				}
			}
		}

		//SPECIAL!	Certain events in the game can cause profiled NPCs to become enemies.	The two cases are
		//adding Mike and Iggy.	We will only add one NPC in any given combat and the conditions for setting
		//the associated facts are done elsewhere.	There is also another place where NPCs can get added, which
		//is in TacticalCreateElite() used for inserting offensive enemies.
		if( tempDetailedPlacement.bTeam == ENEMY_TEAM && tempDetailedPlacement.ubSoldierClass == SOLDIER_CLASS_ELITE )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: set npcs to enemies"));
			OkayToUpgradeEliteToSpecialProfiledEnemy( &tempDetailedPlacement );
		}

		// Flugente: if this is an enemy, and we are using ambush code, place us somewhat away from the map center, where the player will be
		if ( (tempDetailedPlacement.bTeam == ENEMY_TEAM && ( GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE || GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE) ) ||
			 (tempDetailedPlacement.bTeam == CREATURE_TEAM && GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE) )
		{
			if ( (gGameExternalOptions.uAmbushEnemyEncircle == 1 && PythSpacesAway( tempDetailedPlacement.sInsertionGridNo, gMapInformation.sCenterGridNo ) <= gAmbushRadiusModifier * gGameExternalOptions.usAmbushEnemyEncircleRadius1) ||
				 ( gGameExternalOptions.uAmbushEnemyEncircle == 2) )
			{
				// we simply look for a entry point inside a bigger circle, but not inside the merc deployment zone.
				INT32 bettergridno = NOWHERE;
				UINT16 counter = 0;
				UINT8 ubDirection = DIRECTION_IRRELEVANT;

				while ( counter < 100 && (bettergridno == NOWHERE || PythSpacesAway( bettergridno, gMapInformation.sCenterGridNo ) <= gAmbushRadiusModifier * gGameExternalOptions.usAmbushEnemyEncircleRadius1) )
				{
					bettergridno = FindRandomGridNoBetweenCircles( gMapInformation.sCenterGridNo, gAmbushRadiusModifier * gGameExternalOptions.usAmbushEnemyEncircleRadius1, gAmbushRadiusModifier * gGameExternalOptions.usAmbushEnemyEncircleRadius2, ubDirection );
					++counter;
				}

				if ( bettergridno != NOWHERE )
				{
					tempDetailedPlacement.sInsertionGridNo = bettergridno;

					// have the soldier look inward. We add + 100 because later on we use this to signify that we want really enforce this direction
					tempDetailedPlacement.ubDirection = (UINT8)GetDirectionToGridNoFromGridNo( tempDetailedPlacement.sInsertionGridNo, gMapInformation.sCenterGridNo ) + 100;
				}
			}
		}
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: create soldier"));
	
	if ( is_networked && (ARMED_VEHICLE( (&tempDetailedPlacement) ) || tempDetailedPlacement.fOnRoof) )
		return TRUE;
		
	pSoldier = TacticalCreateSoldier( &tempDetailedPlacement, &ubID );
	
	if( pSoldier )
	{
		curr->pSoldier = pSoldier;
		curr->ubSoldierID = ubID;
		if (pGroup)
		{
			pSoldier->ubGroupID = pGroup->ubGroupID;
			pSoldier->pGroup = pGroup;
		}

		AddSoldierToSectorNoCalculateDirection( ubID );

		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->bTeam == ENEMY_TEAM && !pSoldier->inv[ HANDPOS ].usItem )
		{
			pSoldier = pSoldier;
		}
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: return true"));

		/*
		#ifdef JA2UB
		INT32 iCnt;
		for( iCnt=0; iCnt< NUM_ORIGINAL_INV_SLOTS; iCnt++)
		{
				if ( gGameOptions.ubInventorySystem == INVENTORY_OLD && gGameOptions.ubAttachmentSystem == ATTACHMENT_OLD )
				{
					if( pSoldier->inv[ iCnt ].usItem == 97 || pSoldier->inv[ iCnt ].usItem == 1346 || pSoldier->inv[ iCnt ].usItem == 99 
						|| pSoldier->inv[ iCnt ].usItem == 1347 || pSoldier->inv[ iCnt ].usItem == 584 || pSoldier->inv[ iCnt ].usItem == 551 ) //43
						pSoldier->inv[ iCnt ].usItem = 129; //335		
				}
		}	
		#endif	
		*/
		return TRUE;
	}
	else
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Failed to create soldier using TacticalCreateSoldier within AddPlacementToWorld"));
		LiveMessage( "Failed to create soldier using TacticalCreateSoldier within AddPlacementToWorld" );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: return false"));
	if(is_server)	ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"report this MP error (AddPlacementToWorld-FAIL!)");
	return FALSE;
}

void AddPlacementToWorldByProfileID( UINT8 ubProfile )
{
	SOLDIERINITNODE * curr;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorldByProfileID"));

	curr = gSoldierInitHead;
	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == ubProfile && !curr->pSoldier )
		{
			//Matching profile, so add this placement.
			AddPlacementToWorld( curr );
			break;
		}
		curr = curr->next;
	}
}

UINT8 AddSoldierInitListTeamToWorld( INT8 bTeam, UINT8 ubMaxNum )
{
	UINT8 ubNumAdded = 0;
	SOLDIERINITNODE *mark;
	UINT8 ubSlotsToFill;
	UINT8 ubSlotsAvailable;
	SOLDIERINITNODE *curr;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListTeamToWorld"));
	//Sort the list in the following manner:
	//-Priority placements first
	//-Basic placements next
	//-Any placements with existing soldiers last (overrides others)
	SortSoldierInitList();

	if( giCurrentTilesetID == 1 ) //cave/mine tileset only
	{ //convert all civilians to miners which use uniforms and more masculine body types.
		curr = gSoldierInitHead;
		while( curr )
		{
			if( curr->pBasicPlacement->bTeam == CIV_TEAM && !curr->pDetailedPlacement )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_MINER;
				curr->pBasicPlacement->ubBodyType = -1;
			}
			curr = curr->next;
		}
	}

	//Count the current number of soldiers of the specified team
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pBasicPlacement->bTeam == bTeam && curr->pSoldier )
			ubNumAdded++;	//already one here!
		curr = curr->next;
	}

	curr = gSoldierInitHead;

	//First fill up all of the priority existance slots...
	while( curr && curr->pBasicPlacement->fPriorityExistance && ubNumAdded < ubMaxNum )
	{
		if( curr->pBasicPlacement->bTeam == bTeam )
		{
			//Matching team, so add this placement.
			if( AddPlacementToWorld( curr ) )
			{
				ubNumAdded++;
			}
		}
		curr = curr->next;
	}
	if( ubNumAdded == ubMaxNum )
		return ubNumAdded;

	//Now count the number of nodes that are basic placements of desired team
	//This information will be used to randomly determine which of these placements
	//will be added based on the number of slots we can still add.
	mark = curr;
	ubSlotsAvailable = 0;
	ubSlotsToFill = ubMaxNum - ubNumAdded;
	while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum )
	{
		if( curr->pBasicPlacement->bTeam == bTeam )
			ubSlotsAvailable++;
		curr = curr->next;
	}

	//we now have the number, so compared it to the num we can add, and determine how we will
	//randomly determine which nodes to add.
	if( !ubSlotsAvailable )
	{
		// Flugente: decide wether to spawn enemy assassins in this sector (not kingpin's hitmen, they are handled elsewhere)
		if ( bTeam == MILITIA_TEAM )
			SectorAddAssassins(gWorldSectorX, gWorldSectorY, gbWorldSectorZ);

		// Flugente: spawn prisoners of war in prison sectors
		if ( bTeam == CIV_TEAM )
		{
			SectorAddPrisonersofWar(gWorldSectorX, gWorldSectorY, gbWorldSectorZ);

			SectorAddDownedPilot( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

			BOOLEAN sectorhaseverbeenplayercontrolled = FALSE;
			if ( gbWorldSectorZ )
			{
				UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

				if ( pSector && pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins > 0 )
				{
					sectorhaseverbeenplayercontrolled = TRUE;
				}
			}
			else
				sectorhaseverbeenplayercontrolled = SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].fSurfaceWasEverPlayerControlled;

			LuaHandleSectorTacticalEntry( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, sectorhaseverbeenplayercontrolled );
		}

		//There aren't any basic placements of desired team, so exit.
		return ubNumAdded;
	}
	curr = mark;
	//while we have a list, with no active soldiers, the num added is less than the max num requested, and
	//we have slots available, process the list to add new soldiers.
	while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum && ubSlotsAvailable )
	{
		if( curr->pBasicPlacement->bTeam == bTeam )
		{
			if( ubSlotsAvailable <= ubSlotsToFill || Random( ubSlotsAvailable ) < ubSlotsToFill )
			{
				//found matching team, so add this soldier to the game.
				if( AddPlacementToWorld( curr ) )
				{
					ubNumAdded++;
					ubSlotsToFill--;
				}
				else
				{
					//if it fails to create the soldier, it is likely that it is because the slots in the tactical
					//engine are already full.	Besides, the strategic AI shouldn't be trying to fill a map with
					//more than the maximum allowable soldiers of team.	All teams can have a max of 32 individuals,
					//except for the player which is 20.	Players aren't processed in this list anyway.
					// 0verhaul:  Bzzzzzzzttttt.... Wrong!  Other reasons for a false return:
					//   Profiled merc not in sector
					//   Profiled merc is recruited
					//   Profiled merc is escorted
					//   Profiled merc is dead
					//   Profiled merc needs to use profile insertion data instead of placement
					// None of these instances is a reason to give up on the rest of the placements.
					//return ubNumAdded;
				}
				// 0verhaul:  However, move this to the point of successful insertion!
				//ubSlotsToFill--;
			}
			ubSlotsAvailable--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}

	// Flugente: decide wether to spawn enemy assassins in this sector (not kingpin's hitmen, they are handled elsewhere)
	if ( bTeam == MILITIA_TEAM )
		SectorAddAssassins(gWorldSectorX, gWorldSectorY, gbWorldSectorZ);

	// Flugente: spawn prisoners of war in prison sectors
	if ( bTeam == CIV_TEAM )
	{
		SectorAddPrisonersofWar(gWorldSectorX, gWorldSectorY, gbWorldSectorZ);

		SectorAddDownedPilot( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

		BOOLEAN sectorhaseverbeenplayercontrolled = FALSE;
		if ( gbWorldSectorZ )
		{
			UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

			if ( pSector && pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins > 0 )
			{
				sectorhaseverbeenplayercontrolled = TRUE;
			}
		}
		else
			sectorhaseverbeenplayercontrolled = SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].fSurfaceWasEverPlayerControlled;

		LuaHandleSectorTacticalEntry( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, sectorhaseverbeenplayercontrolled );
	}

	return ubNumAdded;
}

void AddSoldierInitListEnemyDefenceSoldiers( UINT8 ubTotalAdmin, UINT8 ubTotalTroops, UINT8 ubTotalElite, UINT8 ubTotalRobots, UINT8 ubTotalTanks, UINT8 ubTotalJeeps )
{
	SOLDIERINITNODE *mark;
	SOLDIERINITNODE *curr;
	INT32 iRandom;
	UINT8 ubMaxNum;
	UINT8 ubRobotPDSlots = 0, ubRobotDSlots = 0, ubRobotPSlots = 0, ubRobotBSlots = 0;
	UINT8 ubElitePDSlots = 0, ubEliteDSlots = 0, ubElitePSlots = 0, ubEliteBSlots = 0;
	UINT8 ubTroopPDSlots = 0, ubTroopDSlots = 0, ubTroopPSlots = 0, ubTroopBSlots = 0;
	UINT8 ubAdminPDSlots = 0, ubAdminDSlots = 0, ubAdminPSlots = 0, ubAdminBSlots = 0;
	UINT8 ubTankPDSlots = 0, ubTankDSlots = 0, ubTankPSlots = 0, ubTankBSlots = 0;
	UINT8 ubJeepPDSlots = 0, ubJeepDSlots = 0, ubJeepPSlots = 0, ubJeepBSlots = 0;
	UINT8 ubFreeSlots;
	UINT8 *pCurrSlots=NULL;
	UINT8 *pCurrTotal=NULL;
	UINT8 ubCurrClass;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListEnemyDefenceSoldiers"));

	ResetMortarsOnTeamCount();
	ResetNumSquadleadersInArmyGroup(); // added by SANDRO

	//Specs call for only one profiled enemy can be in a sector at a time due to flavor reasons.
	gfProfiledEnemyAdded = FALSE;

	//Because the enemy defence forces work differently than the regular map placements, the numbers
	//of each type of enemy may not be the same.	Elites will choose the best placements, then army, then
	//administrators.

	ubMaxNum = ubTotalAdmin + ubTotalTroops + ubTotalElite + ubTotalRobots + ubTotalTanks + ubTotalJeeps;

	AssertLE (ubMaxNum, gGameExternalOptions.ubGameMaximumNumberOfEnemies);

	//Sort the list in the following manner:
	//-Priority placements first
	//-Basic placements next
	//-Any placements with existing soldiers last (overrides others)
	SortSoldierInitList();

	//Now count the number of nodes that are basic placements of desired team AND CLASS
	//This information will be used to randomly determine which of these placements
	//will be added based on the number of slots we can still add.
	curr = gSoldierInitHead;
	while( curr && !curr->pSoldier )
	{
		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM )
		{
			switch( curr->pBasicPlacement->ubSoldierClass )
			{
			case SOLDIER_CLASS_ELITE:
				if( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
					ubElitePDSlots++;
				else if( curr->pBasicPlacement->fPriorityExistance )
					ubElitePSlots++;
				else if( curr->pDetailedPlacement )
					ubEliteDSlots++;
				else
					ubEliteBSlots++;
				break;
			case SOLDIER_CLASS_ADMINISTRATOR:
			case SOLDIER_CLASS_BANDIT:
				if( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
					ubAdminPDSlots++;
				else if( curr->pBasicPlacement->fPriorityExistance )
					ubAdminPSlots++;
				else if( curr->pDetailedPlacement )
					ubAdminDSlots++;
				else
					ubAdminBSlots++;
				break;
			case SOLDIER_CLASS_ARMY:
				if( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
					ubTroopPDSlots++;
				else if( curr->pBasicPlacement->fPriorityExistance )
					ubTroopPSlots++;
				else if( curr->pDetailedPlacement )
					ubTroopDSlots++;
				else
					ubTroopBSlots++;
				break;
			case SOLDIER_CLASS_JEEP:
				if ( COMBAT_JEEP( curr->pDetailedPlacement ) )
				{
					if ( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
						ubJeepPDSlots++;
					else if ( curr->pBasicPlacement->fPriorityExistance )
						ubJeepPSlots++;
					else if ( curr->pDetailedPlacement )
						ubJeepDSlots++;
					else
						ubJeepBSlots++;
				}
				break;
			case SOLDIER_CLASS_TANK:
				if ( TANK( curr->pDetailedPlacement ) )
				{
					if( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
						ubTankPDSlots++;
					else if( curr->pBasicPlacement->fPriorityExistance )
						ubTankPSlots++;
					else if( curr->pDetailedPlacement )
						ubTankDSlots++;
					else
						ubTankBSlots++;
				}
				break;
			case SOLDIER_CLASS_ROBOT:
				if ( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
					ubRobotPDSlots++;
				else if ( curr->pBasicPlacement->fPriorityExistance )
					ubRobotPSlots++;
				else if ( curr->pDetailedPlacement )
					ubRobotDSlots++;
				else
					ubRobotBSlots++;
				break;
			}
		}
		curr = curr->next;
	}

	//ADD PLACEMENTS WITH PRIORITY EXISTANCE WITH DETAILED PLACEMENT INFORMATION FIRST
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY + 3; ++ubCurrClass )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_BANDIT:
			pCurrSlots = &ubAdminPDSlots;
			pCurrTotal = &ubTotalAdmin;
			break;
		case SOLDIER_CLASS_ELITE:
			pCurrSlots = &ubElitePDSlots;
			pCurrTotal = &ubTotalElite;
			break;
		case SOLDIER_CLASS_ARMY:
			pCurrSlots = &ubTroopPDSlots;
			pCurrTotal = &ubTotalTroops;
			break;
		case SOLDIER_CLASS_ARMY + 1:// SOLDIER_CLASS_TANK
			ubCurrClass = SOLDIER_CLASS_TANK;
			pCurrSlots = &ubTankPDSlots;
			pCurrTotal = &ubTotalTanks;
			break;
		case SOLDIER_CLASS_ARMY + 2:// SOLDIER_CLASS_JEEP
			ubCurrClass = SOLDIER_CLASS_JEEP;
			pCurrSlots = &ubJeepPDSlots;
			pCurrTotal = &ubTotalJeeps;
			break;
		 case SOLDIER_CLASS_ROBOT + 3: // SOLDIER_CLASS_ROBOT
			ubCurrClass = SOLDIER_CLASS_ROBOT;
			pCurrSlots = &ubRobotPDSlots;
			pCurrTotal = &ubTotalRobots;
			break;
		}
		//Now, loop through the priority existance and detailed placement section of the list.
		curr = gSoldierInitHead;
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots &&
			curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
//						else
//							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;
	curr = gSoldierInitHead;
	while( curr && curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		curr = curr->next;
	mark = curr;

	//ADD PLACEMENTS WITH PRIORITY EXISTANCE AND NO DETAILED PLACEMENT INFORMATION SECOND
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ubCurrClass++ )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_BANDIT:
			pCurrSlots = &ubAdminPSlots;
			pCurrTotal = &ubTotalAdmin;
			break;
		case SOLDIER_CLASS_ELITE:
			pCurrSlots = &ubElitePSlots;
			pCurrTotal = &ubTotalElite;
			break;
		case SOLDIER_CLASS_ARMY:
			pCurrSlots = &ubTroopPSlots;
			pCurrTotal = &ubTotalTroops;
			break;
		}
		//Now, loop through the priority existance and non detailed placement section of the list.
		curr = mark;
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots &&
			!curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;
	curr = mark;
	while( curr && !curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		curr = curr->next;
	mark = curr;

	//ADD PLACEMENTS WITH NO DETAILED PLACEMENT AND PRIORITY EXISTANCE INFORMATION SECOND
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ubCurrClass++ )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_BANDIT:
			pCurrSlots = &ubAdminDSlots;
			pCurrTotal = &ubTotalAdmin;
			break;
		case SOLDIER_CLASS_ELITE:
			pCurrSlots = &ubEliteDSlots;
			pCurrTotal = &ubTotalElite;
			break;
		case SOLDIER_CLASS_ARMY:
			pCurrSlots = &ubTroopDSlots;
			pCurrTotal = &ubTotalTroops;
			break;
		}
		//Now, loop through the priority existance and detailed placement section of the list.
		curr = mark;
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots &&
			curr->pDetailedPlacement && !curr->pBasicPlacement->fPriorityExistance )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;
	curr = mark;
	while( curr && curr->pDetailedPlacement && !curr->pBasicPlacement->fPriorityExistance )
		curr = curr->next;
	mark = curr;

	//Kris: January 11, 2000 -- NEW!!!
	//PRIORITY EXISTANT SLOTS MUST BE FILLED
	//This must be done to ensure all priority existant slots are filled before ANY other slots are filled,
	//even if that means changing the class of the slot.	Also, assume that there are no matching fits left
	//for priority existance slots.	All of the matches have been already assigned in the above passes.
	//We'll have to convert the soldier type of the slot to match.
	curr = gSoldierInitHead;
	while( curr && ubMaxNum && curr->pBasicPlacement->fPriorityExistance )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
		{
			//Choose which team to use.
			iRandom = Random( ubMaxNum );
			if( iRandom < ubTotalElite )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE;
				ubTotalElite--;
			}
			else if( iRandom < ubTotalElite + ubTotalTroops )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ARMY;
				ubTotalTroops--;
			}
			else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ADMINISTRATOR;
				ubTotalAdmin--;
			}
			else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin + ubTotalTanks )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_TANK;
				curr->pBasicPlacement->ubBodyType = TANK_NW;
				ubTotalTanks--;
			}
			else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin + ubTotalTanks + ubTotalJeeps )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_JEEP;
				curr->pBasicPlacement->ubBodyType = COMBAT_JEEP;
				ubTotalJeeps--;
			}
			else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin + ubTotalTanks + ubTotalJeeps + ubTotalRobots )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ROBOT;
				curr->pBasicPlacement->ubBodyType = ROBOTNOWEAPON;
				ubTotalRobots--;
			}
			else
				Assert(0);
			if( AddPlacementToWorld( curr ) )
			{
				ubMaxNum--;
			}
			else
				return;
		}
		curr = curr->next;
	}
	if( !ubMaxNum )
		return;

	//ADD REMAINING PLACEMENTS WITH BASIC PLACEMENT INFORMATION
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ++ubCurrClass )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_BANDIT:
			pCurrSlots = &ubAdminBSlots;
			pCurrTotal = &ubTotalAdmin;
			break;
		case SOLDIER_CLASS_ELITE:
			pCurrSlots = &ubEliteBSlots;
			pCurrTotal = &ubTotalElite;
			break;
		case SOLDIER_CLASS_ARMY:
			pCurrSlots = &ubTroopBSlots;
			pCurrTotal = &ubTotalTroops;
			break;
		}
		//Now, loop through the regular basic placements section of the list.
		curr = mark;
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;

	//If we are at this point, that means that there are some compatibility issues.	This is fine.	An example
	//would be a map containing 1 elite placement, and 31 troop placements.	If we had 3 elites move into this
	//sector, we would not have placements for two of them.	What we have to do is override the class information
	//contained in the list by choosing unused placements, and assign them to the elites.	This time, we will
	//use all free slots including priority placement slots (ignoring the priority placement information).

	//First, count up the total number of free slots.
	ubFreeSlots = 0;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			ubFreeSlots++;
		curr = curr->next;
	}

	// anv: try to place tanks outdoors
	curr = gSoldierInitHead;
	while( curr && ubFreeSlots && ubMaxNum )
	{
		if ( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM && !GridNoIndoors( curr->pBasicPlacement->usStartingGridNo ) && (ubTotalTanks || ubTotalJeeps) )
		{
			if ( ubTotalJeeps )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_JEEP;
				curr->pBasicPlacement->ubBodyType = COMBAT_JEEP;
				ubTotalJeeps--;
				if( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
				ubFreeSlots--;
			}
			else if ( ubTotalTanks )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_TANK;
				curr->pBasicPlacement->ubBodyType = TANK_NW;
				ubTotalTanks--;
				if ( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
				ubFreeSlots--;
			}
			else if ( ubTotalRobots )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ROBOT;
				curr->pBasicPlacement->ubBodyType = ROBOTNOWEAPON;
				ubTotalRobots--;
				if ( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
				ubFreeSlots--;
			}
		}
		curr = curr->next;
	}

	//Now, loop through the entire list again, but for the last time.	All enemies will be inserted now ignoring
	//detailed placements and classes.
	curr = gSoldierInitHead;
	while( curr && ubFreeSlots && ubMaxNum )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
		{
			//Randomly determine if we will use this slot; the more available slots in proportion to
			//the number of enemies, the lower the chance of accepting the slot.
			if( ubFreeSlots <= ubMaxNum || Random( ubFreeSlots ) < ubMaxNum )
			{
				//Choose which team to use.
				iRandom = Random( ubMaxNum );
				if( iRandom < ubTotalElite )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE;
					ubTotalElite--;
				}
				else if( iRandom < ubTotalElite + ubTotalTroops )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ARMY;
					ubTotalTroops--;
				}
				else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ADMINISTRATOR;
					ubTotalAdmin--;
				}
				else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin + ubTotalTanks )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_TANK;
					curr->pBasicPlacement->ubBodyType = TANK_NW;
					ubTotalTanks--;
				}
				else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin + ubTotalTanks + ubTotalJeeps )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_JEEP;
					curr->pBasicPlacement->ubBodyType = COMBAT_JEEP;
					ubTotalJeeps--;
				}
				else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin + ubTotalTanks + ubTotalJeeps + ubTotalRobots )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ROBOT;
					curr->pBasicPlacement->ubBodyType = ROBOTNOWEAPON;
					ubTotalRobots--;
				}
				else
					Assert(0);
				/* DISABLE THE OVERRIDE FOR NOW...
				if( curr->pDetailedPlacement )
				{ //delete the detailed placement information.
					delete( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
				}
				*/
				if( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
			}
			ubFreeSlots--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}
}

//If we are adding militia to our map, then we do a few things differently.
//First of all, they exist exclusively to the enemy troops, so if the militia exists in the
//sector, then they get to use the enemy placements.	However, we remove any orders from
//placements containing RNDPTPATROL or POINTPATROL orders, as well as remove any detailed
//placement information.
void AddSoldierInitListMilitia( UINT8 ubNumGreen, UINT8 ubNumRegs, UINT8 ubNumElites )
{
	SOLDIERINITNODE *mark;
	SOLDIERINITNODE *curr;
	INT32 iRandom;
	UINT8 ubMaxNum;
	BOOLEAN fDoPlacement;
	UINT8 ubEliteSlots = 0;
	UINT8 ubRegSlots = 0;
	UINT8 ubGreenSlots = 0;
	UINT8 ubFreeSlots;
	UINT8 *pCurrSlots=NULL;
	UINT8 *pCurrTotal=NULL;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListMilitia"));

	ubMaxNum = ubNumGreen + ubNumRegs + ubNumElites;

	//Sort the list in the following manner:
	//-Priority placements first
	//-Basic placements next
	//-Any placements with existing soldiers last (overrides others)
	SortSoldierInitList();

	curr = gSoldierInitHead;

	//First fill up only the priority existance slots (as long as the availability and class are okay)
	while( curr && curr->pBasicPlacement->fPriorityExistance && ubMaxNum )
	{
		fDoPlacement = TRUE;

		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM )
		{
			//Matching team (kindof), now check the soldier class...
			if( ubNumElites && curr->pBasicPlacement->ubSoldierClass == SOLDIER_CLASS_ELITE )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
				ubNumElites--;
			}
			else if( ubNumRegs && curr->pBasicPlacement->ubSoldierClass == SOLDIER_CLASS_ARMY )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;
				ubNumRegs--;
			}
			else if( ubNumGreen && curr->pBasicPlacement->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
				ubNumGreen--;
			}
			else
				fDoPlacement = FALSE;

			if ( fDoPlacement )
			{
				curr->pBasicPlacement->bTeam = MILITIA_TEAM;
				curr->pBasicPlacement->bOrders = STATIONARY;
				curr->pBasicPlacement->bAttitude = (INT8) Random( MAXATTITUDES );
				// silversurfer: Replace body type. Militia tanks are not allowed.
				if( curr->pBasicPlacement->fDetailedPlacement )
				{
					if ( curr->pDetailedPlacement && ARMED_VEHICLE( curr->pDetailedPlacement ) )
					{
						curr->pBasicPlacement->ubBodyType = PreRandom( REGFEMALE + 1 );
						// check for better spot next to the tank so the militia doesn't get stuck in the tank
						INT32 iNewSpot = FindNearestPassableSpot( curr->pBasicPlacement->usStartingGridNo );
						if(  iNewSpot != NOWHERE)
							curr->pBasicPlacement->usStartingGridNo = iNewSpot;
					}
				}
				else if ( curr->pBasicPlacement && ARMED_VEHICLE( curr->pBasicPlacement ) )
				{
					curr->pBasicPlacement->ubBodyType = PreRandom( REGFEMALE + 1 );
					// check for better spot next to the tank so the militia doesn't get stuck in the tank
					INT32 iNewSpot = FindNearestPassableSpot( curr->pBasicPlacement->usStartingGridNo );
					if(  iNewSpot != NOWHERE)
						curr->pBasicPlacement->usStartingGridNo = iNewSpot;
				}

				if( curr->pDetailedPlacement )
				{
					//delete the detailed placement information.
					delete( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeAttributeLevel ), curr->pBasicPlacement->ubSoldierClass );
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeEquipmentLevel ), curr->pBasicPlacement->ubSoldierClass );
				}

				if( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
			}
		}
		curr = curr->next;
	}

	if( !ubMaxNum )
		return;

	//Now count the number of nodes that are basic placements of desired team AND CLASS
	//This information will be used to randomly determine which of these placements
	//will be added based on the number of slots we can still add.
	mark = curr;
	while( curr && !curr->pSoldier )
	{
		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM )
		{
			switch( curr->pBasicPlacement->ubSoldierClass )
			{
			case SOLDIER_CLASS_ELITE:						ubEliteSlots++;	break;
			case SOLDIER_CLASS_ADMINISTRATOR:
			case SOLDIER_CLASS_BANDIT:						ubGreenSlots++; break;
			case SOLDIER_CLASS_ARMY:						ubRegSlots++; break;
			}
		}
		curr = curr->next;
	}

	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	// Flugente: changed the order of the classes, as we want to have elites first (relevant if militia takes equipment from the sector)
	UINT8 reorder[3];
	reorder[0] = SOLDIER_CLASS_ELITE;
	reorder[1] = SOLDIER_CLASS_ARMY;
	reorder[2] = SOLDIER_CLASS_ADMINISTRATOR;

	for( UINT8 i = 0; i < 3; ++i )
	{
		//First, prepare the counters.
		switch( reorder[i] )
		{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_BANDIT:
			pCurrSlots = &ubGreenSlots;
			pCurrTotal = &ubNumGreen;
			break;
		case SOLDIER_CLASS_ELITE:
			pCurrSlots = &ubEliteSlots;
			pCurrTotal = &ubNumElites;
			break;
		case SOLDIER_CLASS_ARMY:
			pCurrSlots = &ubRegSlots;
			pCurrTotal = &ubNumRegs;
			break;
		}

		//Now, loop through the basic placement of the list.
		curr = mark; //mark is the marker where the basic placements start.
		while( curr && !curr->pSoldier && ubMaxNum && *pCurrTotal && *pCurrSlots )
		{
			if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == reorder[i] )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						curr->pBasicPlacement->bTeam = MILITIA_TEAM;
						curr->pBasicPlacement->bOrders = STATIONARY;
						switch( reorder[i] )
						{
						case SOLDIER_CLASS_ADMINISTRATOR:
						case SOLDIER_CLASS_BANDIT:
							curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
							break;
						case SOLDIER_CLASS_ARMY:
							curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;
							break;
						case SOLDIER_CLASS_ELITE:
							curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
							break;
						}
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}

	if( !ubMaxNum )
		return;

	//If we are at this point, that means that there are some compatibility issues.	This is fine.	An example
	//would be a map containing 1 elite placement, and 31 troop placements.	If we had 3 elites move into this
	//sector, we would not have placements for two of them.	What we have to do is override the class information
	//contained in the list by choosing unused placements, and assign them to the elites.	This time, we will
	//use all free slots including priority placement slots (ignoring the priority placement information).

	//First, count up the total number of free slots.
	ubFreeSlots = 0;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && (curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM) )
			ubFreeSlots++;
		curr = curr->next;
	}

	//Now, loop through the entire list again, but for the last time.	All enemies will be inserted now ignoring
	//detailed placements and classes.
	curr = gSoldierInitHead;
	while( curr && ubFreeSlots && ubMaxNum )
	{
		if( !curr->pSoldier && (curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM) )
		{
			//Randomly determine if we will use this slot; the more available slots in proportion to
			//the number of enemies, the lower the chance of accepting the slot.
			if( ubFreeSlots <= ubMaxNum || Random( ubFreeSlots ) < ubMaxNum )
			{
				//Choose which team to use.
				iRandom = Random( ubMaxNum );
				if( iRandom < ubNumElites )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
					ubNumElites--;
				}
				else if( iRandom < ubNumElites + ubNumRegs )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;
					ubNumRegs--;
				}
				else if( iRandom < ubNumElites + ubNumRegs + ubNumGreen )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
					ubNumGreen--;
				}
				else
					Assert(0);
				curr->pBasicPlacement->bTeam = MILITIA_TEAM;
				curr->pBasicPlacement->bOrders = ONGUARD;
				curr->pBasicPlacement->bAttitude = (INT8) Random( MAXATTITUDES );

				// silversurfer: Replace body type. Militia tanks are not allowed.
				if ( ARMED_VEHICLE( curr->pBasicPlacement ) )
				{
					curr->pBasicPlacement->ubBodyType = PreRandom( REGFEMALE + 1 );
					// check for better spot next to the tank so the militia doesn't get stuck in the tank
					INT32 iNewSpot = FindNearestPassableSpot( curr->pBasicPlacement->usStartingGridNo );
					if(  iNewSpot != NOWHERE)
						curr->pBasicPlacement->usStartingGridNo = iNewSpot;
				}

				if( curr->pDetailedPlacement )
				{
					//delete the detailed placement information.
					delete( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeAttributeLevel), curr->pBasicPlacement->ubSoldierClass );
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeEquipmentLevel), curr->pBasicPlacement->ubSoldierClass );
				}

				if( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
			}
			ubFreeSlots--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}

	// Flugente: it is possible that a sector has more militia than placements. In this case they would not be placed, thereby not joining any battle
	// we solve this problem by adding them to a random gridno here, but only up to the minimum of MAX_MILITIA_PER_SECTOR, MAX_NUMBER_MILITIA_IN_TACTICAL and MAX_STRATEGIC_ENEMY_GROUP_SIZE
	if ( ubMaxNum && ( ubNumElites || ubNumRegs || ubNumGreen ) )
	{
		// determine how many militia have already been created in this sector (this can also have happened in another function), so count all active ones
		UINT16 tacticalmilitia = 0;
		for ( UINT16 i = gTacticalStatus.Team[MILITIA_TEAM].bFirstID; i <= gTacticalStatus.Team[MILITIA_TEAM ].bLastID; ++i )
		{
			if ( MercPtrs[i]->bInSector && MercPtrs[i]->bActive )
				++tacticalmilitia;
		}

		// is there still room tom add more?
		UINT16 militiamax = min( gGameExternalOptions.iMaxMilitiaPerSector, gGameExternalOptions.ubGameMaximumNumberOfRebels);
		militiamax = min( militiamax, gGameExternalOptions.iMaxEnemyGroupSize );

		while ( tacticalmilitia < militiamax )
		{
			SOLDIERTYPE* pSoldier = NULL;

			if ( ubNumElites )
			{
				pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_ELITE_MILITIA, gWorldSectorX, gWorldSectorY );
				--ubNumElites;
			}
			else if ( ubNumRegs )
			{
				pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_REG_MILITIA, gWorldSectorX, gWorldSectorY );
				--ubNumRegs;
			}
			else if ( ubNumGreen )
			{
				pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_GREEN_MILITIA, gWorldSectorX, gWorldSectorY );
				--ubNumGreen;
			}

			if ( pSoldier )
			{
				//no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = RandomGridNo();

				pSoldier->ubInsertionDirection = DIRECTION_IRRELEVANT;

				// Lesh: militia came from another sector can't reach opposite side of map in case of battle there
				//		they are often stop at a half way. trying to fix this
				if ( gTacticalStatus.Team[MILITIA_TEAM].bAwareOfOpposition )
				{
					pSoldier->aiData.bOrders = SEEKENEMY;
					pSoldier->aiData.bAlertStatus = STATUS_RED;
				}
				else
				{
					pSoldier->aiData.bOrders = ONGUARD;
					pSoldier->aiData.bAlertStatus = STATUS_GREEN;
					pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) + (Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS) * WORLD_COLS);
					pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
				}

				UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );

				++tacticalmilitia;
			}
			else
				return;
		}
	}
}

void AddSoldierInitListCreatures( BOOLEAN fQueen, UINT8 ubNumLarvae, UINT8 ubNumInfants,
								 UINT8 ubNumYoungMales, UINT8 ubNumYoungFemales, UINT8 ubNumAdultMales, 
								 UINT8 ubNumAdultFemales )
{
	SOLDIERINITNODE *curr;
	INT32 iRandom;
	UINT8 ubFreeSlots;
	BOOLEAN fDoPlacement;
	UINT8 ubNumCreatures;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListCreatures"));

	SortSoldierInitList();

	//Okay, if we have a queen, place her first.	She MUST have a special placement, else
	//we can't use anything.
	ubNumCreatures = (UINT8)(ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales + ubNumAdultFemales);
	if( fQueen )
	{
		curr = gSoldierInitHead;
		while( curr	)
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM && curr->pBasicPlacement->ubBodyType == QUEENMONSTER )
			{
				if( !AddPlacementToWorld( curr ) )
				{
					fQueen = FALSE;
					break;
				}
			}
			curr = curr->next;
		}
		if( !fQueen )
		{
#ifdef JA2BETAVERSION
			ScreenMsg( FONT_RED, MSG_ERROR, L"Couldn't place the queen." );
#endif
		}
	}

	//First fill up only the priority existance slots (as long as the availability and bodytypes match)
	curr = gSoldierInitHead;
	while( curr && curr->pBasicPlacement->fPriorityExistance && ubNumCreatures )
	{
		fDoPlacement = TRUE;

		if( curr->pBasicPlacement->bTeam == CREATURE_TEAM )
		{
			//Matching team, now check the soldier class...
			if( ubNumLarvae && curr->pBasicPlacement->ubBodyType == LARVAE_MONSTER )
				ubNumLarvae--;
			else if( ubNumInfants && curr->pBasicPlacement->ubBodyType == INFANT_MONSTER )
				ubNumInfants--;
			else if( ubNumYoungMales && curr->pBasicPlacement->ubBodyType == YAM_MONSTER )
				ubNumYoungMales--;
			else if( ubNumYoungFemales && curr->pBasicPlacement->ubBodyType == YAF_MONSTER )
				ubNumYoungFemales--;
			else if( ubNumAdultMales && curr->pBasicPlacement->ubBodyType == AM_MONSTER )
				ubNumAdultMales--;
			else if( ubNumAdultFemales && curr->pBasicPlacement->ubBodyType == ADULTFEMALEMONSTER )
				ubNumAdultFemales--;
			else
				fDoPlacement = FALSE;
			if ( fDoPlacement )
			{
				if( AddPlacementToWorld( curr ) )
				{
					ubNumCreatures--;
				}
				else
					return;
			}
		}
		curr = curr->next;
	}
	if( !ubNumCreatures )
		return;

	//Count how many free creature slots are left.
	curr = gSoldierInitHead;
	ubFreeSlots = 0;
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM )
			ubFreeSlots++;
		curr = curr->next;
	}
	//Now, if we still have creatures to place, do so completely randomly, overriding priority
	//placements, etc.
	curr = gSoldierInitHead;
	while( curr && ubFreeSlots && ubNumCreatures )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM )
		{
			//Randomly determine if we will use this slot; the more available slots in proportion to
			//the number of enemies, the lower the chance of accepting the slot.
			if( ubFreeSlots <= ubNumCreatures || Random( ubFreeSlots ) < ubNumCreatures )
			{
				//Choose which team to use.
				iRandom = Random( ubNumCreatures );

				if( ubNumLarvae && iRandom < ubNumLarvae )
				{
					ubNumLarvae--;
					curr->pBasicPlacement->ubBodyType = LARVAE_MONSTER;
				}
				else if( ubNumInfants && iRandom < ubNumLarvae + ubNumInfants )
				{
					ubNumInfants--;
					curr->pBasicPlacement->ubBodyType = INFANT_MONSTER;
				}
				else if( ubNumYoungMales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales )
				{
					ubNumYoungMales--;
					curr->pBasicPlacement->ubBodyType = YAM_MONSTER;
				}
				else if( ubNumYoungFemales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales )
				{
					ubNumYoungFemales--;
					curr->pBasicPlacement->ubBodyType = YAF_MONSTER;
				}
				else if( ubNumAdultMales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales )
				{
					ubNumAdultMales--;
					curr->pBasicPlacement->ubBodyType = AM_MONSTER;
				}
				else if( ubNumAdultFemales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales + ubNumAdultFemales )
				{
					ubNumAdultFemales--;
					curr->pBasicPlacement->ubBodyType = ADULTFEMALEMONSTER;
				}
				else
					Assert(0);
				if( curr->pDetailedPlacement )
				{ //delete the detailed placement information.
					MemFree( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
				}
				if( AddPlacementToWorld( curr ) )
				{
					ubNumCreatures--;
				}
				else
				{
					return;
				}
			}
			ubFreeSlots--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}
}

void AddSoldierInitListOtherCreatures( UINT8 usNum )
{
	SOLDIERINITNODE *curr;
	UINT8 ubFreeSlots;
	BOOLEAN fDoPlacement;
	UINT8 ubNumCreatures = usNum;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "AddSoldierInitListBloodcats" ) );

	SortSoldierInitList();	

	//First fill up only the priority existance slots (as long as the availability and bodytypes match)
	curr = gSoldierInitHead;
	while ( curr && curr->pBasicPlacement->fPriorityExistance && ubNumCreatures )
	{
		fDoPlacement = TRUE;

		if ( curr->pBasicPlacement->bTeam == CREATURE_TEAM )
		{
			//Matching team, now check the soldier class...
			switch ( guCreatureAttackType )
			{
			case CREATURE_ATTACK_TYPE_ZOMBIE:
				if ( curr->pBasicPlacement->ubBodyType > REGFEMALE )
					fDoPlacement = FALSE;
				break;

			case CREATURE_ATTACK_TYPE_BANDIT:
				if ( curr->pBasicPlacement->ubBodyType > REGFEMALE )
					fDoPlacement = FALSE;
				break;

			case CREATURE_ATTACK_TYPE_BLOODCAT:
				if ( curr->pBasicPlacement->ubBodyType != BLOODCAT )
					fDoPlacement = FALSE;
				break;
			}
			break;

			if ( fDoPlacement )
			{
				if ( AddPlacementToWorld( curr ) )
				{
					--ubNumCreatures;
				}
				else
					return;
			}
		}
		curr = curr->next;
	}

	if ( !ubNumCreatures )
		return;

	//Count how many free creature slots are left.
	curr = gSoldierInitHead;
	ubFreeSlots = 0;
	while ( curr )
	{
		if ( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM )
			++ubFreeSlots;

		curr = curr->next;
	}

	//Now, if we still have creatures to place, do so completely randomly, overriding priority
	//placements, etc.
	curr = gSoldierInitHead;
	while ( curr && ubFreeSlots && ubNumCreatures )
	{
		if ( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM )
		{
			//Randomly determine if we will use this slot; the more available slots in proportion to
			//the number of enemies, the lower the chance of accepting the slot.
			if ( ubFreeSlots <= ubNumCreatures || Random( ubFreeSlots ) < ubNumCreatures )
			{
				switch ( guCreatureAttackType )
				{
				case CREATURE_ATTACK_TYPE_ZOMBIE:
					curr->pBasicPlacement->ubBodyType = Random( ADULTFEMALEMONSTER );
					break;

				case CREATURE_ATTACK_TYPE_BANDIT:
					curr->pBasicPlacement->ubBodyType = Random( ADULTFEMALEMONSTER );
					break;

				case CREATURE_ATTACK_TYPE_BLOODCAT:
					curr->pBasicPlacement->ubBodyType = BLOODCAT;
					break;
				}
				break;

				if ( curr->pDetailedPlacement )
				{
					//delete the detailed placement information.
					MemFree( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
				}

				if ( AddPlacementToWorld( curr ) )
				{
					--ubNumCreatures;
				}
				else
				{
					return;
				}
			}
			
			--ubFreeSlots;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}
}

SOLDIERINITNODE* FindSoldierInitNodeWithProfileID( UINT16 usProfile );
SOLDIERINITNODE* FindSoldierInitNodeWithProfileID( UINT16 usProfile )
{
	SOLDIERINITNODE *curr;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == usProfile )
			return curr;
		curr = curr->next;
	}
	return NULL;
}


SOLDIERINITNODE* FindSoldierInitNodeWithID( UINT16 usID )
{
	SOLDIERINITNODE *curr;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier->ubID == usID )
			return curr;
		curr = curr->next;
	}
	return NULL;
}

void UseEditorOriginalList()
{
	SOLDIERINITNODE *curr;
	gfOriginalList = TRUE;
	gSoldierInitHead = gOriginalSoldierInitListHead;
	curr = gSoldierInitHead;
	if( curr )
	{
		while( curr->next )
			curr = curr->next;
	}
	if( curr )
		gSoldierInitTail = curr;
}

void UseEditorAlternateList()
{
	SOLDIERINITNODE *curr;
	gfOriginalList = FALSE;
	gSoldierInitHead = gAlternateSoldierInitListHead;
	curr = gSoldierInitHead;
	if( curr )
	{
		while( curr->next )
			curr = curr->next;
	}
	if( curr )
		gSoldierInitTail = curr;
}

//Any killed people that used detailed placement information must prevent that from occurring
//again in the future.	Otherwise, the sniper guy with 99 marksmanship could appear again
//if the map was loaded again!
void EvaluateDeathEffectsToSoldierInitList( SOLDIERTYPE *pSoldier )
{
	SOLDIERINITNODE *curr;
	UINT8 ubNodeID;
	curr = gSoldierInitHead;
	ubNodeID = 0;
	if( pSoldier->bTeam == MILITIA_TEAM )
		return;
	while( curr )
	{
		if( curr->pSoldier == pSoldier )
		{ //Matching soldier found
			if( curr->pDetailedPlacement )
			{ //This soldier used detailed placement information, so we must save the
				//node ID into the temp file which signifies that the

				//RECORD UBNODEID IN TEMP FILE.

				curr->pSoldier = NULL;
				delete( curr->pDetailedPlacement );
				curr->pDetailedPlacement = NULL;
				return;
			}
		}
		ubNodeID++;
		curr = curr->next;
	}
}

void RemoveDetailedPlacementInfo( UINT8 ubNodeID )
{
	SOLDIERINITNODE *curr;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->ubNodeID == ubNodeID )
		{
			if( curr->pDetailedPlacement )
			{
				delete( curr->pDetailedPlacement );
				curr->pDetailedPlacement = NULL;
				return;
			}
		}
		curr = curr->next;
	}
}

//For the purpose of keeping track of which soldier belongs to which placement within the game,
//the only way we can do this properly is to save the soldier ID from the list and reconnect the
//soldier pointer whenever we load the game.
BOOLEAN SaveSoldierInitListLinks( HWFILE hfile )
{
	SOLDIERINITNODE *curr;
	UINT32 uiNumBytesWritten;
	UINT8 ubSlots = 0;

	//count the number of soldier init nodes...
	curr = gSoldierInitHead;
	while( curr )
	{
		ubSlots++;
		curr = curr->next;
	}
	//...and save it.
	FileWrite( hfile, &ubSlots, 1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
	{
		return FALSE;
	}
	//Now, go through each node, and save just the ubSoldierID, if that soldier is alive.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier && !curr->pSoldier->bActive )
		{
			curr->ubSoldierID = 0;
		}
		FileWrite( hfile, &curr->ubNodeID, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			return FALSE;
		}
		FileWrite( hfile, &curr->ubSoldierID, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			return FALSE;
		}
		curr = curr->next;
	}
	return TRUE;
}

BOOLEAN LoadSoldierInitListLinks( HWFILE hfile )
{
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;

	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID &&
						ubSoldierID <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID ||
						ubSoldierID >= gTacticalStatus.Team[ CIV_TEAM ].bFirstID &&
						ubSoldierID <= gTacticalStatus.Team[ CIV_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return TRUE;
}

void AddSoldierInitListBloodcats()
{
	SECTORINFO *pSector;
	SOLDIERINITNODE *curr;
	UINT8 ubSectorID = 0;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListBloodcats"));

	SECTORINFO dummy;

	if( gbWorldSectorZ )
	{
		// This function can only cope with regular SECTORINFO objects.
		// So let's fake a sector above ground and copy over data from UNDERGROUND_SECTORINFO.
		UNDERGROUND_SECTORINFO *pUndergroundSector
			= FindUnderGroundSector(gWorldSectorX, gWorldSectorY, gbWorldSectorZ);

		if (!pUndergroundSector)
			return;

		pSector = &dummy;
		pSector->bBloodCats = pUndergroundSector->ubNumBloodcats;
		pSector->bBloodCatPlacements = pUndergroundSector->ubNumBloodcats;
	}
	else
	{
		ubSectorID = (UINT8)SECTOR( gWorldSectorX, gWorldSectorY );
		pSector = &SectorInfo[ ubSectorID ];
	}

	if( !pSector->bBloodCatPlacements )
	{ //This map has no bloodcat placements, so don't waste CPU time.
		return;
	}
	else
	{ //We don't yet know the number of bloodcat placements in this sector so
		//count them now, and permanently record it.
		INT8 bBloodCatPlacements = 0;
		curr = gSoldierInitHead;
		while( curr )
		{
			if( curr->pBasicPlacement->ubBodyType == BLOODCAT )
			{
				bBloodCatPlacements++;
			}
			curr = curr->next;
		}
		// No placements on the map itself?
		if( !bBloodCatPlacements )
		{
			// Don't place!
			return;
		}
		// HEADROCK HAM 3.6: Check has been changed completely. We now use whichever value is lower - the ones we've
		// set from XML, or the ones existing on the map. Either could override the other, if it is lower.
		pSector->bBloodCatPlacements = __min(bBloodCatPlacements, pSector->bBloodCatPlacements);
		pSector->bBloodCats = __min(pSector->bBloodCats, pSector->bBloodCatPlacements);

		/*if( bBloodCatPlacements != pSector->bBloodCatPlacements &&
			ubSectorID != SEC_I16 && ubSectorID != SEC_N5 )
		{
#ifdef JA2BETAVERSION
			CHAR16 str[200];
			swprintf( str, L"Table specifies that there are %d bloodcat placements in sector %c%d, but the map actually has %d bloodcat placements. Map value takes precedence. KM,LC:1",
								pSector->bBloodCatPlacements, gWorldSectorY + 'A' - 1, gWorldSectorX, bBloodCatPlacements );
			DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
#endif
			// HEADROCK HAM 3.5: This "solution" is extremely silly, as it prevents legal placement of bloodcats
			// on the map if any discrepancy is encountered, which limits modders severely. Also, because the
			// pSector->bBloodCatPlacements value is hardcoded, there is virtually no way for modders to increase
			// the number of bloodcats on their own. 
			//pSector->bBloodCatPlacements = bBloodCatPlacements;
			//pSector->bBloodCats = -1;
			// A better solution is to limit the number of bloodcats on the map based on whichever is lower - the
			// hardcode, or the map-read value.
			pSector->bBloodCatPlacements = __min(bBloodCatPlacements, pSector->bBloodCatPlacements);
			pSector->bBloodCats = __min(pSector->bBloodCats, pSector->bBloodCatPlacements);
			if( !bBloodCatPlacements )
			{
				return;
			}
		}*/
	}
	if( pSector->bBloodCats > 0 )
	{ //Add them to the world now...
		UINT8 ubNumAdded = 0;
		UINT8 ubMaxNum = (UINT8)pSector->bBloodCats;
		SOLDIERINITNODE *mark;
		UINT8 ubSlotsToFill;
		UINT8 ubSlotsAvailable;
		SOLDIERINITNODE *curr;

		//Sort the list in the following manner:
		//-Priority placements first
		//-Basic placements next
		//-Any placements with existing soldiers last (overrides others)
		SortSoldierInitList();

		//Count the current number of soldiers of the specified team
		curr = gSoldierInitHead;
		while( curr )
		{
			if( curr->pBasicPlacement->ubBodyType == BLOODCAT && curr->pSoldier )
				ubNumAdded++;	//already one here!
			curr = curr->next;
		}

		curr = gSoldierInitHead;

		//First fill up all of the priority existance slots...
		while( curr && curr->pBasicPlacement->fPriorityExistance && ubNumAdded < ubMaxNum )
		{
			if( curr->pBasicPlacement->ubBodyType == BLOODCAT )
			{
				//Matching team, so add this placement.
				if( AddPlacementToWorld( curr ) )
				{
					ubNumAdded++;
				}
			}
			curr = curr->next;
		}
		if( ubNumAdded == ubMaxNum )
			return;

		//Now count the number of nodes that are basic placements of desired team
		//This information will be used to randomly determine which of these placements
		//will be added based on the number of slots we can still add.
		mark = curr;
		ubSlotsAvailable = 0;
		ubSlotsToFill = ubMaxNum - ubNumAdded;
		while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum )
		{
			if( curr->pBasicPlacement->ubBodyType == BLOODCAT )
				ubSlotsAvailable++;
			curr = curr->next;
		}

		//we now have the number, so compared it to the num we can add, and determine how we will
		//randomly determine which nodes to add.
		if( !ubSlotsAvailable )
		{	//There aren't any basic placements of desired team, so exit.
			return;
		}
		curr = mark;
		//while we have a list, with no active soldiers, the num added is less than the max num requested, and
		//we have slots available, process the list to add new soldiers.
		while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum && ubSlotsAvailable )
		{
			if( curr->pBasicPlacement->ubBodyType == BLOODCAT )
			{
				if( ubSlotsAvailable <= ubSlotsToFill || Random( ubSlotsAvailable ) < ubSlotsToFill )
				{
					//found matching team, so add this soldier to the game.
					if( AddPlacementToWorld( curr ) )
					{
						ubNumAdded++;
					}
					else
					{
						//if it fails to create the soldier, it is likely that it is because the slots in the tactical
						//engine are already full.	Besides, the strategic AI shouldn't be trying to fill a map with
						//more than the maximum allowable soldiers of team.	All teams can have a max of 32 individuals,
						//except for the player which is 20.	Players aren't processed in this list anyway.
						return;
					}
					ubSlotsToFill--;
				}
				ubSlotsAvailable--;
				//With the decrementing of the slot vars in this manner, the chances increase so that all slots
				//will be full by the time the end of the list comes up.
			}
			curr = curr->next;
		}
		return;
	}
}


SOLDIERINITNODE * FindSoldierInitListNodeByProfile( UINT8 ubProfile )
{
	SOLDIERINITNODE * curr;

	curr = gSoldierInitHead;

	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == ubProfile )
		{
			return( curr );
		}
		curr = curr->next;
	}
	return( NULL );
}

//This is the code that loops through the profiles starting at the RPCs, and adds them using strategic insertion
//information, and not editor placements.	The key flag involved for doing it this way is the gMercProfiles[i].fUseProfileInsertionInfo.
void AddProfilesUsingProfileInsertionData()
{
	INT32 i;
	SOLDIERTYPE *pSoldier;
	SOLDIERINITNODE * curr;

	//for( i = FIRST_RPC; i < ( PROF_HUMMER ); i++ )
	//new profiles by Jazz
	for( i = 0; i < NUM_PROFILES; ++i )
	{
	if ( gMercProfiles[i].Type == PROFILETYPE_RPC || 
		gMercProfiles[i].Type == PROFILETYPE_NPC )
	{	
		//Perform various checks to make sure the soldier is actually in the same sector, alive, and so on.
		//More importantly, the flag to use profile insertion data must be set.
		if( gMercProfiles[ i ].sSectorX != gWorldSectorX ||
			gMercProfiles[ i ].sSectorY != gWorldSectorY ||
			gMercProfiles[ i ].bSectorZ != gbWorldSectorZ ||
			gMercProfiles[ i ].bMercStatus == MERC_RETURNING_HOME ||
			gMercProfiles[ i ].bMercStatus == MERC_FIRED_AS_A_POW ||
			gMercProfiles[ i ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ||
			gMercProfiles[ i ].ubMiscFlags & PROFILE_MISC_FLAG_EPCACTIVE ||
//			gMercProfiles[ i ].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR ||
			!gMercProfiles[ i ].bLife ||
			!gMercProfiles[ i ].fUseProfileInsertionInfo
			)
		{ //Don't add, so skip to the next soldier.
			continue;
		}
		pSoldier = FindSoldierByProfileID( (UINT8)i, FALSE );
		if( !pSoldier )
		{ //Create a new soldier, as this one doesn't exist
			SOLDIERCREATE_STRUCT		MercCreateStruct;
			UINT8									ubID;

			//Set up the create struct so that we can properly create the profile soldier.
			MercCreateStruct.initialize();
			MercCreateStruct.bTeam						= CIV_TEAM;
			MercCreateStruct.ubProfile				= (UINT8)i;
			MercCreateStruct.sSectorX					= gWorldSectorX;
			MercCreateStruct.sSectorY					= gWorldSectorY;
			MercCreateStruct.bSectorZ					= gbWorldSectorZ;

			pSoldier = TacticalCreateSoldier( &MercCreateStruct, &ubID );
		}
		if ( pSoldier )
		{ //Now, insert the soldier.
			pSoldier->ubStrategicInsertionCode = gMercProfiles[ i ].ubStrategicInsertionCode;
			pSoldier->usStrategicInsertionData = gMercProfiles[ i ].usStrategicInsertionData;
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			// CJC: Note well that unless an error occurs, UpdateMercInSector calls
			// AddSoldierToSector
			// AddSoldierToSector( pSoldier->ubID );

			// check action ID values
			if ( gMercProfiles[ i ].ubQuoteRecord )
			{
				pSoldier->ubQuoteRecord = gMercProfiles[ i ].ubQuoteRecord;
				pSoldier->ubQuoteActionID = gMercProfiles[ i ].ubQuoteActionID;
				if ( pSoldier->ubQuoteActionID == QUOTE_ACTION_ID_CHECKFORDEST )
				{
					// gridno will have been changed to destination... so we're there...
					NPCReachedDestination( pSoldier, FALSE );
				}
			}

			// make sure this person's pointer is set properly in the init list
			curr = FindSoldierInitListNodeByProfile( pSoldier->ubProfile );
			if ( curr )
			{
				curr->pSoldier = pSoldier;
				curr->ubSoldierID = pSoldier->ubID;
				// also connect schedules here
				if ( curr->pDetailedPlacement->ubScheduleID != 0 )
				{
					SCHEDULENODE * pSchedule = GetSchedule( curr->pDetailedPlacement->ubScheduleID );
					if ( pSchedule )
					{
						pSchedule->ubSoldierID = pSoldier->ubID;
						pSoldier->ubScheduleID = curr->pDetailedPlacement->ubScheduleID;
					}
				}
			}
		}

	}
	}
}

void AddProfilesNotUsingProfileInsertionData()
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddProfilesNotUsingProfileInsertionData"));
	SOLDIERINITNODE *curr;
	//Count the current number of soldiers of the specified team
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier &&
			curr->pBasicPlacement->bTeam == CIV_TEAM &&
			curr->pDetailedPlacement &&
			curr->pDetailedPlacement->ubProfile != NO_PROFILE &&
			!gMercProfiles[ curr->pDetailedPlacement->ubProfile ].fUseProfileInsertionInfo &&
			gMercProfiles[ curr->pDetailedPlacement->ubProfile ].bLife )
		{
			AddPlacementToWorld( curr );
		}
		curr = curr->next;
	}
}

#ifdef JA2BETAVERSION
extern void ErrorDetectedInSaveCallback( UINT8 bValue );

BOOLEAN ValidateSoldierInitLinks( UINT8 ubCode )
{
	SOLDIERINITNODE *curr;
	UINT32 uiNumInvalids = 0;
	CHAR16 str[512];
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier )
		{
			if( curr->pSoldier->ubID < 20 && !MercPtrs[ curr->pSoldier->ubID ]->bActive )
			{
				uiNumInvalids++;
			}
		}
		curr = curr->next;
	}
	if( uiNumInvalids || ubCode == 4)
	{
		switch( ubCode )
		{
		case 1: //loading save
			swprintf( str, L"Error detected in save file WHILE LOADING.  Please send save and text files associated with save to Kris and Dave."
										L"  After doing so, go back into the game and immediately resave the game which will fix the problem."
										L"  This is the bug responsible for mercs disappearing.  Be prepared to answer lots of questions..." );
			DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, str, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, ErrorDetectedInSaveCallback );
			break;
		case 2: //saving game
			//swprintf( str, L"Error detected WHILE SAVING file.  Please send save and text files associated with save to Kris and Dave."
			//							L"  After doing so, go back into the game and try reloading the new save and saving it again which *could* fix the problem."
			//							L"  This is the bug responsible for mercs disappearing.  Be prepared to answer lots of questions..." );
			//if( guiPreviousOptionScreen == MAP_SCREEN )
			//	DoMapMessageBox( MSG_BOX_BASIC_STYLE, str, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
			//else
			//	DoMessageBox( MSG_BOX_BASIC_STYLE, str, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
			break;
		case 3: //entering sector using temp files (before fade in)
			gfDoDialogOnceGameScreenFadesIn = TRUE;
			break;
		case 4: //after fade in
			gfDoDialogOnceGameScreenFadesIn = FALSE;
			swprintf( str, L"Error detected while entering sector USING TEMP FILES.  Please send previous save and text files associated with save to Kris and Dave."
										L"  After doing so, go back into the game and saving the game, reloading it, and saving it again *could* fix it."
										L"  This is the bug responsible for mercs disappearing.  Be prepared to answer lots of questions..." );
			DoMessageBox( MSG_BOX_BASIC_STYLE, str, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
			break;
		}
		return FALSE;
	}
	return TRUE;
}
#endif //betaversion error checking functions


BOOLEAN NewWayOfLoadingEnemySoldierInitListLinks( HWFILE hfile )
{
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;

	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID &&
						ubSoldierID <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return TRUE;
}


BOOLEAN NewWayOfLoadingCivilianInitListLinks( HWFILE hfile )
{
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;

	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ CIV_TEAM ].bFirstID &&
						ubSoldierID <= gTacticalStatus.Team[ CIV_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return( TRUE );
}


BOOLEAN LookAtButDontProcessEnemySoldierInitListLinks( HWFILE hfile )
{
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;

	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID &&
						ubSoldierID <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return TRUE;
}

void StripEnemyDetailedPlacementsIfSectorWasPlayerLiberated()
{
	SECTORINFO *pSector;
	SOLDIERINITNODE *curr;

	if( !gfWorldLoaded || gbWorldSectorZ )
	{ //No world loaded or underground.	Underground sectors don't matter
		//seeing enemies (not creatures) never rejuvenate underground.
		return;
	}

	pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

	if( !pSector->uiTimeLastPlayerLiberated )
	{ //The player has never owned the sector.
		return;
	}

	//The player has owned the sector at one point.	By stripping all of the detailed placements, only basic
	//placements will remain.	This prevents tanks and "specially detailed" enemies from coming back.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pDetailedPlacement )
		{
			if( curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				// pDetailedPlacement has been C++'d
				//MemFree( curr->pDetailedPlacement );
				delete curr->pDetailedPlacement;
				curr->pDetailedPlacement = NULL;
				curr->pBasicPlacement->fDetailedPlacement = FALSE;
				curr->pBasicPlacement->fPriorityExistance = FALSE;
				curr->pBasicPlacement->ubBodyType = -1;
				RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeAttributeLevel ), curr->pBasicPlacement->ubSoldierClass );
				RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeEquipmentLevel ), curr->pBasicPlacement->ubSoldierClass );
			}
		}
		curr = curr->next;
	}

}

////////
////////	For militia squad attack!
////////
//dnl ch56 141009
//#define CENTRAL_GRIDNO 13202
//#define CENTRAL_RADIUS 30

void AddSoldierInitListMilitiaOnEdge( UINT8 ubStrategicInsertionCode, UINT8 ubNumGreen, UINT8 ubNumReg, UINT8 ubNumElites )
{
	SOLDIERTYPE *pSoldier;
	MAPEDGEPOINTINFO MapEdgepointInfo;
	UINT8 ubCurrSlot;
	UINT8 ubTotalSoldiers;
	UINT8 bDesiredDirection=0;

	ubTotalSoldiers = ubNumGreen + ubNumReg + ubNumElites;

	// WANNE: If we have no militia soldiers->exit!
	if (ubTotalSoldiers == 0)
		return;

	// Flugente: if militia picks up equipment from sectors, it is necessary to know from where it comes
	INT16 sX = gWorldSectorX;
	INT16 sY = gWorldSectorY;
	
	switch( ubStrategicInsertionCode )
	{
	case INSERTION_CODE_NORTH:	
		bDesiredDirection = SOUTHEAST;
		--sY;
		break;
	case INSERTION_CODE_EAST:
		bDesiredDirection = SOUTHWEST;
		++sX;
		break;
	case INSERTION_CODE_SOUTH:
		bDesiredDirection = NORTHWEST;
		++sY;
		break;
	case INSERTION_CODE_WEST:
		bDesiredDirection = NORTHEAST;
		--sX;
		break;
	default:	
		AssertMsg( 0, "Illegal direction passed to AddSoldierInitListMilitiaOnEdge()" );
		break;
	}
#ifdef JA2TESTVERSION
	ScreenMsg( FONT_RED, MSG_INTERFACE, L"Militia reinforcements have arrived!	(%d admins, %d troops, %d elite)", ubNumGreen, ubNumReg, ubNumElites );
#endif

	// Flugente: if militia takes items from sector inventories, then militia coming from neighbouring sectors will have to take it from there

	ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, (UINT8)(ubNumGreen + ubNumReg + ubNumElites) );
	ubCurrSlot = 0;
	while( ubTotalSoldiers )
	{
		if( ubNumElites && Random( ubTotalSoldiers ) < ubNumElites )
		{
			ubNumElites--;
			ubTotalSoldiers--;
			pSoldier = TacticalCreateMilitia(SOLDIER_CLASS_ELITE_MILITIA, sX, sY);

			// Lesh: if pSoldier is NULL then no slot for a new men or other problems
			//		it better to leave this function is such case
			if ( !pSoldier )
				return;

			pSoldier->ubInsertionDirection = bDesiredDirection;

			// Lesh: militia came from another sector can't reach opposite side of map in case of battle there
			//		they are often stop at a half way. trying to fix this
			if ( gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition )
			{
				pSoldier->aiData.bOrders = SEEKENEMY;
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				pSoldier->aiData.bOrders = ONGUARD;
				pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
				pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
				pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
			}

			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if( ubNumReg && (UINT8)Random( ubTotalSoldiers ) < (UINT8)(ubNumElites + ubNumReg) )
		{
			ubNumReg--;
			ubTotalSoldiers--;
			pSoldier = TacticalCreateMilitia(SOLDIER_CLASS_REG_MILITIA, sX, sY);

			// Lesh: if pSoldier is NULL then no slot for a new men or other problems
			//		it better to leave this function is such case
			if ( !pSoldier )
				return;

			pSoldier->ubInsertionDirection = bDesiredDirection;

			// Lesh: militia came from another sector can't reach opposite side of map in case of battle there
			//		they are often stop at a half way. trying to fix this
			if ( gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition )
			{
				pSoldier->aiData.bOrders = SEEKENEMY;
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				pSoldier->aiData.bOrders = ONGUARD;
				pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
				pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
				pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
			}

			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if( ubNumGreen && (UINT8)Random( ubTotalSoldiers ) < (UINT8)(ubNumElites + ubNumReg + ubNumGreen) )
		{
			ubNumGreen--;
			ubTotalSoldiers--;
			pSoldier = TacticalCreateMilitia(SOLDIER_CLASS_GREEN_MILITIA, sX, sY);

			// Lesh: if pSoldier is NULL then no slot for a new men or other problems
			//		it better to leave this function is such case
			if ( !pSoldier )
				return;

			pSoldier->ubInsertionDirection = bDesiredDirection;

			// Lesh: militia came from another sector can't reach opposite side of map in case of battle there
			//		they are often stop at a half way. trying to fix this
			if ( gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition )
			{
				pSoldier->aiData.bOrders = SEEKENEMY;
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				pSoldier->aiData.bOrders = ONGUARD;
				pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
				pSoldier->aiData.sNoiseGridno = (CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
				pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
			}

			//if ( GetTimeOfDayAmbientLightLevel() < NORMAL_LIGHTLEVEL_DAY + 2 )
			//	gTacticalStatus.Team[ ENEMY_TEAM ].bAwareOfOpposition = TRUE;
			//gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition = TRUE;

			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		
		// HEADROCK HAM 3.2: Experimental, militia reinforcements arrive with 0 APs.
		if (gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 1 || gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 3)
		{
			pSoldier->bActionPoints = 0;

			// Flugente: due to a fix, also note here that the reinforcements get no APs.
			pSoldier->usSoldierFlagMask |= SOLDIER_NO_AP;

			// Flugente: campaign stats
			if ( IsOurSoldier(pSoldier) )
				gCurrentIncident.usIncidentFlags |= INCIDENT_REINFORCEMENTS_PLAYERSIDE;
			else
				gCurrentIncident.usIncidentFlags |= INCIDENT_REINFORCEMENTS_ENEMY;
		}
	}
}

extern UINT32 GetWorldTotalMin( );

// Flugente: decide wether to spawn enemy assassins in this sector (not kingpin's hitmen, they are handled elsewhere)
void SectorAddAssassins( INT16 sMapX, INT16 sMapY, INT16 sMapZ )
{
	// this needs to be turned on on, and new trait system needs to be active
	if ( !gGameExternalOptions.fEnemyAssassins || !gGameOptions.fNewTraitSystem )
		return;

	// only possible if a certain progress has been reached - the queen only sends out assassins after a while
	if ( HighestPlayerProgressPercentage() < gGameExternalOptions.usAssassinMinimumProgress )
		return;

	// not in underground sectors
	if ( sMapZ > 0 )
		return;

	// does not work atm, time gets reset too early
	// not if we have recently been in this sector
	//if ( pSector->uiTimeCurrentSectorWasLastLoaded + 10 > GetWorldTotalMin() )
		//return;

	// do not spawn if militia size is very small (we will be spotted much easier by the player)
	UINT32 totalmilitia = NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM );
	if ( totalmilitia < gGameExternalOptions.usAssassinMinimumMilitia )
		return;

	// count current number of civilians
	UINT16 numberofcivs = 0;
	SOLDIERTYPE* pTeamSoldier = NULL;
	INT32 cnt = gTacticalStatus.Team[ CIV_TEAM ].bFirstID;
	INT32 lastid = gTacticalStatus.Team[ CIV_TEAM ].bLastID;
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt < lastid; ++cnt, ++pTeamSoldier)
	{
		// check if teamsoldier exists in this sector
		if ( pTeamSoldier && pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->sSectorX == sMapX && pTeamSoldier->sSectorY == sMapY && pTeamSoldier->bSectorZ == sMapZ )
			++numberofcivs;
	}

	// we can't spawn if all civilian slots are already taken
	if ( numberofcivs >= gGameExternalOptions.ubGameMaximumNumberOfCivilians )
		return;

	// now count militia, and which type (green, regular, elite) is most numerous - that will be the best type to blend in	
	UINT8 militiacnt = MilitiaInSectorOfRank( sMapX, sMapY, GREEN_MILITIA );
	UINT8 militiadisguise = GREEN_MILITIA;
	if ( MilitiaInSectorOfRank( sMapX, sMapY, REGULAR_MILITIA ) > militiacnt )
	{
		militiacnt = MilitiaInSectorOfRank( sMapX, sMapY, REGULAR_MILITIA );
		militiadisguise = REGULAR_MILITIA;
	}

	if ( MilitiaInSectorOfRank( sMapX, sMapY, ELITE_MILITIA ) > militiacnt )
	{
		militiadisguise = ELITE_MILITIA;
	}
		
	// the bigger the militia, the more likely infiltration
	// if militia is at maximum, there is a 10% chance of infiltration
	UINT32 resultrange = gGameExternalOptions.ubGameMaximumNumberOfRebels * 10;
	UINT32 modifiedmilitianumber = totalmilitia * gGameExternalOptions.usAssassinPropabilityModifier / 100;
	if ( Random(resultrange) >= modifiedmilitianumber )
		return;

	CreateAssassin( militiadisguise );
	++numberofcivs;

	// if there is still room for a second assassin, and we are feeling very lucky again, add a second one
	if ( numberofcivs < gGameExternalOptions.ubGameMaximumNumberOfCivilians && Random(resultrange) < modifiedmilitianumber )
	{
		CreateAssassin( militiadisguise );
		++numberofcivs;
	}
}

// Flugente: decide wether to create prisoners of war in a sector. Not to be confused with player POWs
void SectorAddPrisonersofWar( INT16 sMapX, INT16 sMapY, INT16 sMapZ )
{
	// this needs to be turned on on
	if ( !gGameExternalOptions.fAllowPrisonerSystem )
		return;
	
	// not in underground sectors
	if ( sMapZ > 0 )
		return;

	// get sector
	SECTORINFO *pSector = &SectorInfo[ SECTOR( sMapX, sMapY ) ];
	if ( !pSector )
		return;

	// only continue if there are prisoners in this sector that need to be placed
	INT16 aPrisoners[PRISONER_MAX] = {0};
	UINT16 numprisoners = GetNumberOfPrisoners( pSector, aPrisoners );
	if ( !numprisoners )
		return;
		
	// count current number of civilians and already placed pows
	UINT16 numberofcivs = 0;
	UINT16 numberofpows = 0;
	SOLDIERTYPE* pTeamSoldier = NULL;
	INT32 cnt = gTacticalStatus.Team[ CIV_TEAM ].bFirstID;
	INT32 lastid = gTacticalStatus.Team[ CIV_TEAM ].bLastID;
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt < lastid; ++cnt, ++pTeamSoldier)
	{
		// check if teamsoldier exists in this sector
		if ( pTeamSoldier && pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->sSectorX == sMapX && pTeamSoldier->sSectorY == sMapY && pTeamSoldier->bSectorZ == sMapZ )
			++numberofcivs;

		// count how many pows are already placed
		if ( pTeamSoldier->usSoldierFlagMask & SOLDIER_POW_PRISON )
			++numberofpows;
	}

	// we can't spawn if all civilian slots are already taken (we leave a bit of reserve for more important civs)
	UINT8 maxcivs = max(0, gGameExternalOptions.ubGameMaximumNumberOfCivilians - 3);

	for (UINT16 i = numberofpows; i < numprisoners; ++i)
	{
		if ( numberofcivs < maxcivs )
		{
			CreatePrisonerOfWar();
			++numberofcivs;
		}
		else
			break;
	}
}

// Flugente: decide wether to add a downed pilot if a helicopter was shot down here
void SectorAddDownedPilot( INT16 sMapX, INT16 sMapY, INT16 sMapZ )
{
	// not in underground sectors
	if ( sMapZ > 0 )
		return;

	// get sector
	SECTORINFO *pSector = &SectorInfo[SECTOR( sMapX, sMapY )];
	if ( !pSector )
		return;

	if ( pSector->usSectorInfoFlag & SECTORINFO_ENEMYHELI_SHOTDOWN )
	{
		// it is likely that a pilot has survived, but not guaranteed
		if ( Chance( 75 ) )
		{
			CreateDownedPilot();
		}

		// remove the flag. We can only find the pilot the first time we visit this sector after the heli was shut down
		pSector->usSectorInfoFlag &= ~SECTORINFO_ENEMYHELI_SHOTDOWN;
	}
}