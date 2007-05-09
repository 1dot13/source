#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Sound Control.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "sys globals.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "worldman.h"
	#include "math.h"
	#include "points.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "opplist.h"
	#include "interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "items.h"
	#include "text.h"
	#include "Soldier Profile.h"
	#include "tile animation.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "explosion control.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "Random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "morale.h"
	#include "meanwhile.h"
	#include "SkillCheck.h"
	#include "gamesettings.h"
	#include "SaveLoadMap.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "utilities.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	INVTYPE		curItem;
	INVTYPE *	curArray;
	UINT32			maxArraySize;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef itemParseData;

BOOLEAN localizedTextOnly;

static void XMLCALL 
itemStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	itemParseData * pData = (itemParseData *)userData;

	//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("itemStartElementHandle: at element (%d), name = %s, depth = %d, maxdepth = %d, jar? = %d",pData->curElement,name,pData->currentDepth,pData->maxReadDepth,strcmp(name, "Jar") ) );
	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ITEMLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			// WANNE
			if ( !localizedTextOnly )
				memset(pData->curArray,0,sizeof(INVTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			// WANNE
			if ( !localizedTextOnly )
				memset(&pData->curItem,0,sizeof(INVTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szItemName") == 0 ||
				strcmp(name, "szLongItemName") == 0 ||
				strcmp(name, "szItemDesc") == 0 ||
				strcmp(name, "szBRName") == 0 ||
				strcmp(name, "szBRDesc") == 0 ||
				strcmp(name, "usItemClass") == 0 ||
				strcmp(name, "ubClassIndex") == 0 ||
				strcmp(name, "ubCursor") == 0 ||
				strcmp(name, "bSoundType") == 0 ||
				strcmp(name, "ubGraphicType") == 0 ||
				strcmp(name, "ubGraphicNum") == 0 ||
				strcmp(name, "ubWeight") == 0 ||
				strcmp(name, "ubPerPocket") == 0 ||
				strcmp(name, "usPrice") == 0 ||
				strcmp(name, "ubCoolness") == 0 ||
				strcmp(name, "bReliability") == 0 ||
				strcmp(name, "bRepairEase") == 0 ||
				strcmp(name, "Damageable") == 0 ||
				strcmp(name, "Repairable") == 0 ||
				strcmp(name, "WaterDamages") == 0 ||
				strcmp(name, "Metal") == 0 ||
				strcmp(name, "Sinks") == 0 ||
				strcmp(name, "ShowStatus") == 0 ||
				strcmp(name, "HiddenAddon") == 0 ||
				strcmp(name, "TwoHanded") == 0 ||
				strcmp(name, "NotBuyable") == 0 ||
				strcmp(name, "Attachment") == 0 ||
				strcmp(name, "HiddenAttachment") == 0 ||
				strcmp(name, "BigGunList") == 0 ||
				strcmp(name, "NotInEditor") == 0 ||
				strcmp(name, "DefaultUndroppable") == 0 ||
				strcmp(name, "Unaerodynamic") == 0 ||
				strcmp(name, "Electronic") == 0 ||
				strcmp(name, "Inseparable") == 0 ||
				strcmp(name, "BR_NewInventory") == 0 ||
				strcmp(name, "BR_UsedInventory") == 0 ||
				strcmp(name, "BR_ROF") == 0 ||
				strcmp(name, "PercentNoiseReduction") == 0 ||
				strcmp(name, "Bipod") == 0 ||
				strcmp(name, "BestLaserRange") == 0 ||
				strcmp(name, "ToHitBonus") == 0 ||
				strcmp(name, "RangeBonus") == 0 ||
				strcmp(name, "AimBonus") == 0 ||
				strcmp(name, "MinRangeForAimBonus") == 0 ||
				strcmp(name, "PercentAPReduction") == 0 ||
				strcmp(name, "PercentStatusDrainReduction") == 0 ||
				strcmp(name, "GrenadeLauncher") == 0 ||
				strcmp(name, "Duckbill") == 0 ||
				strcmp(name, "Detonator") == 0 ||
				strcmp(name, "RemoteDetonator") == 0 ||
				strcmp(name, "HideMuzzleFlash") == 0 ||
				strcmp(name, "Alcohol") == 0 ||
				strcmp(name, "GasMask") == 0 ||
				strcmp(name, "Hardware") == 0 ||
				strcmp(name, "Medical") == 0 ||
				strcmp(name, "DamageBonus") == 0 ||
				strcmp(name, "MeleeDamageBonus") == 0 ||
				strcmp(name, "Mortar") == 0 ||
				strcmp(name, "RocketLauncher") == 0 ||
				strcmp(name, "SingleShotRocketLauncher") == 0 ||
				strcmp(name, "DiscardedLauncherItem") == 0 ||
				strcmp(name, "BrassKnuckles") == 0 ||
				strcmp(name, "BloodiedItem") == 0 ||
				strcmp(name, "Crowbar") == 0 ||
				strcmp(name, "GLGrenade") == 0 ||
				strcmp(name, "FlakJacket") == 0 ||
				strcmp(name, "HearingRangeBonus") == 0 ||
				strcmp(name, "VisionRangeBonus") == 0 ||
				strcmp(name, "NightVisionRangeBonus") == 0 ||
				strcmp(name, "DayVisionRangeBonus") == 0 ||
				strcmp(name, "CaveVisionRangeBonus") == 0 ||
				strcmp(name, "BrightLightVisionRangeBonus") == 0 ||
				strcmp(name, "LeatherJacket") == 0 ||
				strcmp(name, "NeedsBatteries") == 0 ||
				strcmp(name, "Batteries") == 0 ||
				strcmp(name, "XRay") == 0 ||
				strcmp(name, "WireCutters") == 0 ||
				strcmp(name, "Toolkit") == 0 ||
				strcmp(name, "Canteen") == 0 ||
				strcmp(name, "Jar") == 0 ||
				strcmp(name, "CanAndString") == 0 ||
				strcmp(name, "Marbles") == 0 ||
				strcmp(name, "Walkman") == 0 ||
				strcmp(name, "RemoteTrigger") == 0 ||
				strcmp(name, "RobotRemoteControl") == 0 ||
				strcmp(name, "CamouflageKit") == 0 ||
				strcmp(name, "LocksmithKit") == 0 ||
				strcmp(name, "Mine") == 0 ||
				strcmp(name, "GasCan") == 0 ||
				strcmp(name, "ContainsLiquid") == 0 ||
				strcmp(name, "Rock") == 0 ||
				strcmp(name, "LockBomb") == 0 ||
				strcmp(name, "Flare") == 0 ||
				strcmp(name, "MetalDetector") == 0 ||
				strcmp(name, "FingerPrintID") == 0 ||
				strcmp(name, "Cannon") == 0 ||
				strcmp(name, "RocketRifle") == 0 ||
				strcmp(name, "MedicalKit") == 0 ||
				strcmp(name, "FirstAidKit") == 0 ||
				strcmp(name, "MagSizeBonus") == 0 ||
				strcmp(name, "PercentAutofireAPReduction") == 0 ||
				strcmp(name, "PercentBurstFireAPReduction") == 0 ||
				strcmp(name, "AutoFireToHitBonus") == 0 ||
				strcmp(name, "APBonus") == 0 ||
				strcmp(name, "RateOfFireBonus") == 0 ||
				strcmp(name, "BurstSizeBonus") == 0 ||
				strcmp(name, "PercentReadyTimeAPReduction") == 0 ||
				strcmp(name, "PercentReloadTimeAPReduction") == 0 ||
				strcmp(name, "BulletSpeedBonus") == 0 ||
				strcmp(name, "BurstToHitBonus") == 0 ||
				strcmp(name, "ThermalOptics") == 0 ||
				strcmp(name, "PercentTunnelVision") == 0 ||
				strcmp(name, "DefaultAttachment") == 0 ||
				strcmp(name, "CamoBonus") == 0 ||
				strcmp(name, "UrbanCamoBonus") == 0 ||
				strcmp(name, "DesertCamoBonus") == 0 ||
				strcmp(name, "SnowCamoBonus") == 0 ||
				strcmp(name, "StealthBonus") == 0 ||
				strcmp(name, "SciFi") == 0 ||

	strcmp(name, "fFlags") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("itemStartElementHandle: going into element, name = %s",name) );

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
itemCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	itemParseData * pData = (itemParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
itemEndElementHandle(void *userData, const XML_Char *name)
{
	itemParseData * pData = (itemParseData *)userData;
	char temp;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ITEMLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curItem.uiIndex < pData->maxArraySize) 
			{
				if ( pData->curItem.usItemClass != 0 )
					pData->curArray[pData->curItem.uiIndex] = pData->curItem; //write the item into the table
				else if ( sizeof(pData->curItem.szItemName)>0 && localizedTextOnly )
				{
					strcpy(pData->curArray[pData->curItem.uiIndex].szItemName,pData->curItem.szItemName);
					strcpy(pData->curArray[pData->curItem.uiIndex].szLongItemName,pData->curItem.szLongItemName);
					strcpy(pData->curArray[pData->curItem.uiIndex].szBRName,pData->curItem.szBRName);
					strcpy(pData->curArray[pData->curItem.uiIndex].szItemDesc,pData->curItem.szItemDesc);
					strcpy(pData->curArray[pData->curItem.uiIndex].szBRDesc,pData->curItem.szBRDesc);
				}
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.uiIndex = atol(pData->szCharData);
		}
		else if(strcmp(name, "szItemName") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: itemname");
			pData->curElement = ELEMENT;

			//if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
			//	strcpy(pData->curItem.szItemName,pData->szCharData);
			//else
			//{
			//	strncpy(pData->curItem.szItemName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
			//	pData->curItem.szItemName[MAX_CHAR_DATA_LENGTH] = '\0';
			//}

			// WANNE
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curItem.szItemName,pData->szCharData);
			else
			{
				strncpy(pData->curItem.szItemName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curItem.szItemName[MAX_CHAR_DATA_LENGTH] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
			{
				temp = pData->szCharData[i];
				pData->curItem.szItemName[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: itemname[%d] = %s, temp = %s",i,&pData->curItem.szItemName[i],&temp));
			}
		}
		else if(strcmp(name, "szLongItemName") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: longitemname");
			pData->curElement = ELEMENT;

			// WANNE
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
			{
				strcpy(pData->curItem.szLongItemName,pData->szCharData);
			}
			else
			{
				strncpy(pData->curItem.szLongItemName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curItem.szLongItemName[MAX_CHAR_DATA_LENGTH] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
			{
				temp = pData->szCharData[i];
				pData->curItem.szLongItemName[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: longitemname[%d] = %s, temp = %s",i,&pData->curItem.szLongItemName[i],&temp));
			}
			//if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
			//{
			//	strcpy(pData->curItem.szLongItemName,pData->szCharData);
			//}
			//else
			//{
			//	strncpy(pData->curItem.szLongItemName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
			//	pData->curItem.szLongItemName[MAX_CHAR_DATA_LENGTH] = '\0';
			//}
		}
		else if(strcmp(name, "szItemDesc") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: itemdesc");
			pData->curElement = ELEMENT;

			// WANNE
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curItem.szItemDesc,pData->szCharData);
			else
			{
				strncpy(pData->curItem.szItemDesc,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curItem.szItemDesc[MAX_CHAR_DATA_LENGTH] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
			{
				temp = pData->szCharData[i];
				pData->curItem.szItemDesc[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: itemdesc[%d] = %s, temp = %s",i,&pData->curItem.szItemDesc[i],&temp));
			}
		}
		else if(strcmp(name, "szBRName") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: brname");
			pData->curElement = ELEMENT;

			// WANNE
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curItem.szBRName,pData->szCharData);
			else
			{
				strncpy(pData->curItem.szBRName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curItem.szBRName[MAX_CHAR_DATA_LENGTH] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
			{
				temp = pData->szCharData[i];
				pData->curItem.szBRName[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: BRname[%d] = %s, temp = %s",i,&pData->curItem.szBRName[i],&temp));
			}
		}
		else if(strcmp(name, "szBRDesc") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: brdesc");
			pData->curElement = ELEMENT;

			// WANNE
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curItem.szBRDesc,pData->szCharData);
			else
			{
				strncpy(pData->curItem.szBRDesc,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curItem.szBRDesc[MAX_CHAR_DATA_LENGTH] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
			{
				temp = pData->szCharData[i];
				pData->curItem.szBRDesc[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: BRdesc[%d] = %s, temp = %s",i,&pData->curItem.szBRDesc[i],&temp));
			}
		}
		else if(strcmp(name, "usItemClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usItemClass = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubClassIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubClassIndex = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubCursor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubCursor = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bSoundType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bSoundType = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubGraphicType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubGraphicType = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubGraphicNum") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubGraphicNum = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubWeight") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubWeight = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubPerPocket") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubPerPocket = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usPrice") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usPrice = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubCoolness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubCoolness = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bReliability") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bReliability = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bRepairEase") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bRepairEase = (INT8) atol(pData->szCharData);
		}
		//else if(strcmp(name, "fFlags")	 == 0)
		//{
		//	pData->curElement = ELEMENT;
		//	pData->curItem.fFlags = (BOOLEAN) atol(pData->szCharData);
		//}
		else if(strcmp(name, "Damageable")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.damageable = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Repairable")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.repairable = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "WaterDamages")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.waterdamages = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Metal")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.metal = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Sinks")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.sinks = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ShowStatus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.showstatus = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "HiddenAddon")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.hiddenaddon = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "TwoHanded")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.twohanded = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "NotBuyable")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.notbuyable  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Attachment")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.attachment = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "HiddenAttachment")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.hiddenattachment = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "BigGunList")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.biggunlist = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "NotInEditor")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.notineditor = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "DefaultUndroppable")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.defaultundroppable = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Unaerodynamic")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.unaerodynamic = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Electronic")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.electronic = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Inseparable")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.inseparable = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "BR_NewInventory")	 == 0)
		{
			pData->curElement = ELEMENT;
			StoreInventory[pData->curItem.uiIndex][0] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "BR_UsedInventory")	 == 0)
		{
			pData->curElement = ELEMENT;
			StoreInventory[pData->curItem.uiIndex][1] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "BR_ROF")	 == 0)
		{
			pData->curElement = ELEMENT;
		 	WeaponROF[pData->curItem.uiIndex] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "CamoBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
		 	pData->curItem.camobonus = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "DesertCamoBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
		 	pData->curItem.desertCamobonus = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "UrbanCamoBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
		 	pData->curItem.urbanCamobonus = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "SnowCamoBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
		 	pData->curItem.snowCamobonus = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "StealthBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.stealthbonus = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentNoiseReduction")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentnoisereduction = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "GasMask")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.gasmask = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Bipod")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bipod = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ToHitBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.tohitbonus = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "RangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.rangebonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "AimBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.aimbonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "MinRangeForAimBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.minrangeforaimbonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentAPReduction")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentapreduction  = (INT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "MagSizeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.magsizebonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentAutofireAPReduction")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentautofireapreduction  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentBurstFireAPReduction")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentburstfireapreduction  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "AutoFireToHitBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.autofiretohitbonus   = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "APBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.APBonus = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "RateOfFireBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.rateoffirebonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "BurstSizeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.burstsizebonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "BurstToHitBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bursttohitbonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentReadyTimeAPReduction")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentreadytimeapreduction  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentReloadTimeAPReduction")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentreloadtimeapreduction  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "BulletSpeedBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bulletspeedbonus  = (INT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "PercentStatusDrainReduction")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentstatusdrainreduction  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "GrenadeLauncher")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.grenadelauncher  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "LockBomb")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.lockbomb  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Flare")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.flare = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Duckbill")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.duckbill  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Detonator")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.detonator  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RemoteDetonator")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.remotedetonator  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ThermalOptics")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.thermaloptics   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "SciFi")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.scifi   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "HideMuzzleFlash")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.hidemuzzleflash  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Cannon")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.cannon   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RocketRifle")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.rocketrifle   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Alcohol")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.alcohol  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Hardware")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.hardware  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Medical")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.medical  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "DamageBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.damagebonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "MeleeDamageBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.meleedamagebonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Mortar")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.mortar = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RocketLauncher")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.rocketlauncher  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "SingleShotRocketLauncher")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.singleshotrocketlauncher  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "DiscardedLauncherItem")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.discardedlauncheritem  = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "SingleShotRocketLauncher")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.singleshotrocketlauncher  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "BloodiedItem")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bloodieditem  = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "DefaultAttachment")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.defaultattachment  = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "BrassKnuckles")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.brassknuckles   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Crowbar")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.crowbar  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "GLGrenade")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.glgrenade   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "FlakJacket")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.flakjacket = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "HearingRangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.hearingrangebonus  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "VisionRangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.visionrangebonus   = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "NightVisionRangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.nightvisionrangebonus   = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "DayVisionRangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.dayvisionrangebonus   = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "CaveVisionRangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.cavevisionrangebonus   = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "BrightLightVisionRangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.brightlightvisionrangebonus    = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "LeatherJacket")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.leatherjacket    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "NeedsBatteries")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.needsbatteries    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Batteries")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.batteries    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "XRay")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.xray    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "WireCutters")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.wirecutters    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Toolkit")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.toolkit    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Canteen")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.canteen    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Marbles")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.marbles    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Walkman")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.walkman    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RemoteTrigger")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.remotetrigger    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RobotRemoteControl")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.robotremotecontrol    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "CamouflageKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.camouflagekit  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "LocksmithKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.locksmithkit = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Mine")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.mine   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "GasCan")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.gascan  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "CanAndString")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.canandstring     = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ContainsLiquid")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.containsliquid    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "FingerPrintID")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.fingerprintid    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "Rock")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.rock   = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "MedicalKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.medicalkit    = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "FirstAidKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.firstaidkit  = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "MetalDetector")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.metaldetector  = (BOOLEAN) atol(pData->szCharData);
		}
		if(strcmp(name, "PercentTunnelVision")	 == 0) //Madd: had to scrap the "else" due to a compiler limit
		{
			pData->curElement = ELEMENT;
			pData->curItem.percenttunnelvision  = (UINT8) atol(pData->szCharData);
		}
		if(strcmp(name, "Jar")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.jar    = (BOOLEAN) atol(pData->szCharData);
		}
		if(strcmp(name, "BestLaserRange")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bestlaserrange    = (INT16) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInItemStats(STR fileName, BOOLEAN localizedVersion )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;

	XML_Parser	parser = XML_ParserCreate(NULL);
	
	itemParseData pData;
	
	localizedTextOnly = localizedVersion;
	
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Items.xml" );

	// Open items file
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

	
	XML_SetElementHandler(parser, itemStartElementHandle, itemEndElementHandle);
	XML_SetCharacterDataHandler(parser, itemCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curArray = Item;
	pData.maxArraySize = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Items.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("done reading item stats"));
	//CHAR16	str[512];
	//for (int i = 0; i < MAXITEMS; i++)
	//{
	//	swprintf(str, Item[i].szLongItemName);
	//	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ReadInItemStats: long item name: %s", str));
	//}

	return( TRUE );
}
BOOLEAN WriteItemStats()
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Items out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;
		CHAR16 str[100];
		CHAR16 strDesc[500];

		FilePrintf(hFile,"<ITEMLIST>\r\n");
		for(cnt = 0;cnt < 351;cnt++)//just do the old limit for now
		{
			LoadShortNameItemInfo( (UINT16)cnt, str );

			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: itemname");
			STR16 szRemainder = str; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t<ITEM>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									cnt);

			FilePrintf(hFile,"\t\t<szItemName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = wcscspn(szRemainder, L"&<>\'\"\0");
				CHAR16 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%S",szRemainder);
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
			FilePrintf(hFile,"</szItemName>\r\n");


			LoadItemInfo( (UINT16)cnt, str,strDesc );

			szRemainder = str; 
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: longitemname");


			FilePrintf(hFile,"\t\t<szLongItemName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = wcscspn(szRemainder, L"&<>\'\"\0");
				CHAR16 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%S",szRemainder);
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
			FilePrintf(hFile,"</szLongItemName>\r\n");


			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: itemdesc");
			szRemainder = strDesc;

//			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: remainder set");

			FilePrintf(hFile,"\t\t<szItemDesc>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = wcscspn(szRemainder, L"&<>\'\"\0");
				CHAR16 invChar = szRemainder[uiCharLoc];
				
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: characters set");
	
				if(uiCharLoc)
				{
					//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("writeitemstats: setting remainder[%d] to \0",uiCharLoc));
					szRemainder[uiCharLoc] = '\0';
					//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("writeitemstats: printing '%s' to file",szRemainder));
					FilePrintf(hFile,"%S",szRemainder);
					//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("writeitemstats: setting remainder[%d] to %d",uiCharLoc,invChar));
					szRemainder[uiCharLoc] = invChar;
				}

				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: remainder incremented");
				szRemainder += uiCharLoc;

				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("writeitemstats: switching character %d", invChar));
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
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: character switched");
			}
			FilePrintf(hFile,"</szItemDesc>\r\n");
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: end tag printed");


			//CHAR16	sText[400];
			//UINT32	uiStartLoc=0;

//#define		BOBBYR_ITEM_DESC_NAME_SIZE				160
//#define		BOBBYR_ITEM_DESC_INFO_SIZE				640
//#define		BOBBYR_ITEM_DESC_FILE_SIZE				800

			//uiStartLoc = 800 * cnt;
			//LoadEncryptedDataFromFile("BINARYDATA\\BrayDesc.edt", sText, uiStartLoc, 160);

			//szRemainder = sText;

			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: brname");
			LoadBRName((UINT16)cnt,str);
			szRemainder = str;

			FilePrintf(hFile,"\t\t<szBRName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				CHAR16 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%S",szRemainder);
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
			FilePrintf(hFile,"</szBRName>\r\n");



			//uiStartLoc = 800 * cnt + 160;
			//LoadEncryptedDataFromFile("BINARYDATA\\BrayDesc.edt", sText, uiStartLoc, 640);

			//szRemainder = sText;

			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeitemstats: brdesc");
			LoadBRDesc((UINT16)cnt,strDesc );
			szRemainder = strDesc;

			FilePrintf(hFile,"\t\t<szBRDesc>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				CHAR16 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%S",szRemainder);
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
			FilePrintf(hFile,"</szBRDesc>\r\n");







			FilePrintf(hFile,"\t\t<usItemClass>%d</usItemClass>\r\n",						Item[cnt].usItemClass);
			FilePrintf(hFile,"\t\t<ubClassIndex>%d</ubClassIndex>\r\n",							Item[cnt].ubClassIndex);
			FilePrintf(hFile,"\t\t<ubCursor>%d</ubCursor>\r\n",							Item[cnt].ubCursor);
			FilePrintf(hFile,"\t\t<bSoundType>%d</bSoundType>\r\n",					Item[cnt].bSoundType);
			FilePrintf(hFile,"\t\t<ubGraphicType>%d</ubGraphicType>\r\n",					Item[cnt].ubGraphicType);
			FilePrintf(hFile,"\t\t<ubGraphicNum>%d</ubGraphicNum>\r\n",						Item[cnt].ubGraphicNum);
			FilePrintf(hFile,"\t\t<ubWeight>%d</ubWeight>\r\n",						Item[cnt].ubWeight);
			FilePrintf(hFile,"\t\t<ubPerPocket>%d</ubPerPocket>\r\n",									Item[cnt].ubPerPocket);
			FilePrintf(hFile,"\t\t<usPrice>%d</usPrice>\r\n",							Item[cnt].usPrice);
			FilePrintf(hFile,"\t\t<ubCoolness>%d</ubCoolness>\r\n",								Item[cnt].ubCoolness);
			FilePrintf(hFile,"\t\t<bReliability>%d</bReliability>\r\n",								Item[cnt].bReliability);
			FilePrintf(hFile,"\t\t<bRepairEase>%d</bRepairEase>\r\n",									Item[cnt].bRepairEase);

			//FilePrintf(hFile,"\t\t<damageable>%d</damageable>\r\n",						Item[cnt].fFlags & ITEM_DAMAGEABLE ? TRUE : FALSE);
			//FilePrintf(hFile,"\t\t<repairable>%d</repairable>\r\n",						Item[cnt].fFlags & ITEM_REPAIRABLE ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<waterdamages>%d</waterdamages>\r\n",						Item[cnt].fFlags & ITEM_WATER_DAMAGES ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<metal>%d</metal>\r\n",						Item[cnt].fFlags & ITEM_METAL ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<sinks>%d</sinks>\r\n",						Item[cnt].fFlags & ITEM_SINKS ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<showstatus>%d</showstatus>\r\n",						Item[cnt].fFlags & ITEM_SHOW_STATUS ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<hiddenaddon>%d</hiddenaddon>\r\n",						Item[cnt].fFlags & ITEM_HIDDEN_ADDON ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<twohanded>%d</twohanded>\r\n",						Item[cnt].fFlags & ITEM_TWO_HANDED ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<notbuyable>%d</notbuyable>\r\n",						Item[cnt].fFlags & ITEM_NOT_BUYABLE ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<attachment>%d</attachment>\r\n",						Item[cnt].fFlags & ITEM_ATTACHMENT ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<biggunlist>%d</biggunlist>\r\n",						Item[cnt].fFlags & ITEM_BIGGUNLIST ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<notineditor>%d</notineditor>\r\n",						Item[cnt].fFlags & ITEM_NOT_EDITOR ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<defaultundroppable>%d</defaultundroppable>\r\n",						Item[cnt].fFlags & ITEM_DEFAULT_UNDROPPABLE ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<unaerodynamic>%d</unaerodynamic>\r\n",						Item[cnt].fFlags & ITEM_UNAERODYNAMIC ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<electronic>%d</electronic>\r\n",						Item[cnt].fFlags & ITEM_ELECTRONIC ? TRUE : FALSE );
			//FilePrintf(hFile,"\t\t<inseparable>%d</inseparable>\r\n",						Item[cnt].fFlags & ITEM_INSEPARABLE ? TRUE : FALSE );

			FilePrintf(hFile,"\t\t<Damageable>%d</Damageable>\r\n",						Item[cnt].damageable );
			FilePrintf(hFile,"\t\t<Repairable>%d</Repairable>\r\n",						Item[cnt].repairable );
			FilePrintf(hFile,"\t\t<WaterDamages>%d</WaterDamages>\r\n",						Item[cnt].waterdamages  );
			FilePrintf(hFile,"\t\t<Metal>%d</Metal>\r\n",						Item[cnt].metal  );
			FilePrintf(hFile,"\t\t<Sinks>%d</Sinks>\r\n",						Item[cnt].sinks );
			FilePrintf(hFile,"\t\t<ShowStatus>%d</ShowStatus>\r\n",						Item[cnt].showstatus );
			FilePrintf(hFile,"\t\t<HiddenAddon>%d</HiddenAddon>\r\n",						Item[cnt].hiddenaddon  );
			FilePrintf(hFile,"\t\t<TwoHanded>%d</TwoHanded>\r\n",						Item[cnt].twohanded  );
			FilePrintf(hFile,"\t\t<NotBuyable>%d</NotBuyable>\r\n",						Item[cnt].notbuyable );
			FilePrintf(hFile,"\t\t<Attachment>%d</Attachment>\r\n",						Item[cnt].attachment  );
			FilePrintf(hFile,"\t\t<BigGunList>%d</BigGunList>\r\n",						Item[cnt].biggunlist   );
			FilePrintf(hFile,"\t\t<SciFi>%d</SciFi>\r\n",						Item[cnt].scifi   );
			FilePrintf(hFile,"\t\t<NotInEditor>%d</NotInEditor>\r\n",						Item[cnt].notineditor  );
			FilePrintf(hFile,"\t\t<DefaultUndroppable>%d</DefaultUndroppable>\r\n",						Item[cnt].defaultundroppable );
			FilePrintf(hFile,"\t\t<Unaerodynamic>%d</Unaerodynamic>\r\n",						Item[cnt].unaerodynamic );
			FilePrintf(hFile,"\t\t<Electronic>%d</Electronic>\r\n",						Item[cnt].electronic );
			FilePrintf(hFile,"\t\t<Inseparable>%d</Inseparable>\r\n",						Item[cnt].inseparable );

			FilePrintf(hFile,"\t\t<BR_NewInventory>%d</BR_NewInventory>\r\n",						StoreInventory[cnt][0] );
			FilePrintf(hFile,"\t\t<BR_UsedInventory>%d</BR_UsedInventory>\r\n",						StoreInventory[cnt][1] );
			FilePrintf(hFile,"\t\t<BR_ROF>%d</BR_ROF>\r\n",						WeaponROF[cnt]);

			FilePrintf(hFile,"\t\t<PercentNoiseReduction>%d</PercentNoiseReduction>\r\n",						Item[cnt].percentnoisereduction  );
			FilePrintf(hFile,"\t\t<HideMuzzleFlash>%d</HideMuzzleFlash>\r\n",						Item[cnt].hidemuzzleflash   );
			FilePrintf(hFile,"\t\t<Bipod>%d</Bipod>\r\n",						Item[cnt].bipod  );
			FilePrintf(hFile,"\t\t<RangeBonus>%d</RangeBonus>\r\n",						Item[cnt].rangebonus   );
			FilePrintf(hFile,"\t\t<ToHitBonus>%d</ToHitBonus>\r\n",						Item[cnt].tohitbonus    );
			FilePrintf(hFile,"\t\t<AimBonus>%d</AimBonus>\r\n",						Item[cnt].aimbonus   );
			FilePrintf(hFile,"\t\t<MinRangeForAimBonus>%d</MinRangeForAimBonus>\r\n",						Item[cnt].minrangeforaimbonus  );

			FilePrintf(hFile,"\t\t<MagSizeBonus>%d</MagSizeBonus>\r\n",						Item[cnt].magsizebonus    );
			FilePrintf(hFile,"\t\t<RateOfFireBonus>%d</RateOfFireBonus>\r\n",						Item[cnt].rateoffirebonus   );
			FilePrintf(hFile,"\t\t<BulletSpeedBonus>%d</BulletSpeedBonus>\r\n",						Item[cnt].bulletspeedbonus );
			FilePrintf(hFile,"\t\t<BurstSizeBonus>%d</BurstSizeBonus>\r\n",						Item[cnt].burstsizebonus );
			FilePrintf(hFile,"\t\t<BestLaserRange>%d</BestLaserRange>\r\n",						Item[cnt].bestlaserrange );
			FilePrintf(hFile,"\t\t<BurstToHitBonus>%d</BurstToHitBonus>\r\n",						Item[cnt].bursttohitbonus );
			FilePrintf(hFile,"\t\t<AutofireToHitBonus>%d</AutofireToHitBonus>\r\n",						Item[cnt].autofiretohitbonus);
			FilePrintf(hFile,"\t\t<APBonus>%d</APBonus>\r\n",						Item[cnt].APBonus );

			FilePrintf(hFile,"\t\t<PercentBurstFireAPReduction>%d</PercentBurstFireAPReduction>\r\n",						Item[cnt].percentburstfireapreduction    );
			FilePrintf(hFile,"\t\t<PercentAutofireAPReduction>%d</PercentAutofireAPReduction>\r\n",						Item[cnt].percentautofireapreduction    );
			FilePrintf(hFile,"\t\t<PercentReadyTimeAPReduction>%d</PercentReadyTimeAPReduction>\r\n",						Item[cnt].percentreadytimeapreduction );
			FilePrintf(hFile,"\t\t<PercentReloadTimeAPReduction>%d</PercentReloadTimeAPReduction>\r\n",						Item[cnt].percentreloadtimeapreduction );
			FilePrintf(hFile,"\t\t<PercentAPReduction>%d</PercentAPReduction>\r\n",						Item[cnt].percentapreduction   );
			FilePrintf(hFile,"\t\t<PercentStatusDrainReduction>%d</PercentStatusDrainReduction>\r\n",						Item[cnt].percentstatusdrainreduction   );

			FilePrintf(hFile,"\t\t<DamageBonus>%d</DamageBonus>\r\n",						Item[cnt].damagebonus   );
			FilePrintf(hFile,"\t\t<MeleeDamageBonus>%d</MeleeDamageBonus>\r\n",						Item[cnt].meleedamagebonus  );
			FilePrintf(hFile,"\t\t<GrenadeLauncher>%d</GrenadeLauncher>\r\n",						Item[cnt].grenadelauncher  );
			FilePrintf(hFile,"\t\t<Duckbill>%d</Duckbill>\r\n",						Item[cnt].duckbill  );
			FilePrintf(hFile,"\t\t<GLGrenade>%d</GLGrenade>\r\n",						Item[cnt].glgrenade  );
			FilePrintf(hFile,"\t\t<Mine>%d</Mine>\r\n",						Item[cnt].mine  );
			FilePrintf(hFile,"\t\t<Mortar>%d</Mortar>\r\n",						Item[cnt].mortar  );
			FilePrintf(hFile,"\t\t<RocketLauncher>%d</RocketLauncher>\r\n",						Item[cnt].rocketlauncher  );
			FilePrintf(hFile,"\t\t<SingleShotRocketLauncher>%d</SingleShotRocketLauncher>\r\n",						Item[cnt].singleshotrocketlauncher  );
			FilePrintf(hFile,"\t\t<DiscardedLauncherItem>%d</DiscardedLauncherItem>\r\n",						Item[cnt].discardedlauncheritem  );
			FilePrintf(hFile,"\t\t<RocketRifle>%d</RocketRifle>\r\n",						Item[cnt].rocketrifle);
			FilePrintf(hFile,"\t\t<Cannon>%d</Cannon>\r\n",						Item[cnt].cannon);
			FilePrintf(hFile,"\t\t<DefaultAttachment>%d</DefaultAttachment>\r\n",						Item[cnt].defaultattachment  );

			FilePrintf(hFile,"\t\t<BrassKnuckles>%d</BrassKnuckles>\r\n",						Item[cnt].brassknuckles  );
			FilePrintf(hFile,"\t\t<Crowbar>%d</Crowbar>\r\n",						Item[cnt].crowbar  );
			FilePrintf(hFile,"\t\t<BloodiedItem>%d</BloodiedItem>\r\n",						Item[cnt].bloodieditem  );
			FilePrintf(hFile,"\t\t<Rock>%d</Rock>\r\n",						Item[cnt].rock  );

			FilePrintf(hFile,"\t\t<CamoBonus>%d</CamoBonus>\r\n",						Item[cnt].camobonus  );
			FilePrintf(hFile,"\t\t<UrbanCamoBonus>%d</UrbanCamoBonus>\r\n",						Item[cnt].urbanCamobonus  );
			FilePrintf(hFile,"\t\t<DesertCamoBonus>%d</DesertCamoBonus>\r\n",						Item[cnt].desertCamobonus  );
			FilePrintf(hFile,"\t\t<SnowCamoBonus>%d</SnowCamoBonus>\r\n",						Item[cnt].snowCamobonus );
			FilePrintf(hFile,"\t\t<StealthBonus>%d</StealthBonus>\r\n",						Item[cnt].stealthbonus  );
			FilePrintf(hFile,"\t\t<FlakJacket>%d</FlakJacket>\r\n",						Item[cnt].flakjacket  );
			FilePrintf(hFile,"\t\t<LeatherJacket>%d</LeatherJacket>\r\n",						Item[cnt].leatherjacket  );

			FilePrintf(hFile,"\t\t<Detonator>%d</Detonator>\r\n",						Item[cnt].detonator  );
			FilePrintf(hFile,"\t\t<RemoteDetonator>%d</RemoteDetonator>\r\n",						Item[cnt].remotedetonator );
			FilePrintf(hFile,"\t\t<RemoteTrigger>%d</RemoteTrigger>\r\n",						Item[cnt].remotetrigger  );
			FilePrintf(hFile,"\t\t<LockBomb>%d</LockBomb>\r\n",						Item[cnt].lockbomb   );
			FilePrintf(hFile,"\t\t<Flare>%d</Flare>\r\n",						Item[cnt].flare   );

			FilePrintf(hFile,"\t\t<RobotRemoteControl>%d</RobotRemoteControl>\r\n",						Item[cnt].robotremotecontrol  );
			FilePrintf(hFile,"\t\t<Walkman>%d</Walkman>\r\n",						Item[cnt].walkman  );
			FilePrintf(hFile,"\t\t<HearingRangeBonus>%d</HearingRangeBonus>\r\n",						Item[cnt].hearingrangebonus  );

			FilePrintf(hFile,"\t\t<VisionRangeBonus>%d</VisionRangeBonus>\r\n",						Item[cnt].visionrangebonus  );
			FilePrintf(hFile,"\t\t<NightVisionRangeBonus>%d</NightVisionRangeBonus>\r\n",						Item[cnt].nightvisionrangebonus  );
			FilePrintf(hFile,"\t\t<DayVisionRangeBonus>%d</DayVisionRangeBonus>\r\n",						Item[cnt].dayvisionrangebonus  );
			FilePrintf(hFile,"\t\t<CaveVisionRangeBonus>%d</CaveVisionRangeBonus>\r\n",						Item[cnt].cavevisionrangebonus  );
			FilePrintf(hFile,"\t\t<BrightLightVisionRangeBonus>%d</BrightLightVisionRangeBonus>\r\n",						Item[cnt].brightlightvisionrangebonus  );
			FilePrintf(hFile,"\t\t<PercentTunnelVision>%d</PercentTunnelVision>\r\n",						Item[cnt].percenttunnelvision );
			FilePrintf(hFile,"\t\t<ThermalOptics>%d</ThermalOptics>\r\n",						Item[cnt].thermaloptics );
			FilePrintf(hFile,"\t\t<GasMask>%d</GasMask>\r\n",						Item[cnt].gasmask );


			FilePrintf(hFile,"\t\t<Alcohol>%d</Alcohol>\r\n",						Item[cnt].alcohol  );
			FilePrintf(hFile,"\t\t<Hardware>%d</Hardware>\r\n",						Item[cnt].hardware   );
			FilePrintf(hFile,"\t\t<Medical>%d</Medical>\r\n",						Item[cnt].medical  );
			FilePrintf(hFile,"\t\t<CamouflageKit>%d</CamouflageKit>\r\n",						Item[cnt].camouflagekit  );
			FilePrintf(hFile,"\t\t<LocksmithKit>%d</LocksmithKit>\r\n",						Item[cnt].locksmithkit  );
			FilePrintf(hFile,"\t\t<Toolkit>%d</Toolkit>\r\n",						Item[cnt].toolkit  );
			FilePrintf(hFile,"\t\t<FirstAidKit>%d</FirstAidKit>\r\n",						Item[cnt].firstaidkit   );
			FilePrintf(hFile,"\t\t<MedicalKit>%d</MedicalKit>\r\n",						Item[cnt].medicalkit   );
			FilePrintf(hFile,"\t\t<WireCutters>%d</WireCutters>\r\n",						Item[cnt].wirecutters  );
			FilePrintf(hFile,"\t\t<Canteen>%d</Canteen>\r\n",						Item[cnt].canteen );
			FilePrintf(hFile,"\t\t<GasCan>%d</GasCan>\r\n",						Item[cnt].gascan  );
			FilePrintf(hFile,"\t\t<Marbles>%d</Marbles>\r\n",						Item[cnt].marbles  );
			FilePrintf(hFile,"\t\t<CanAndString>%d</CanAndString>\r\n",						Item[cnt].canandstring  );
			FilePrintf(hFile,"\t\t<Jar>%d</Jar>\r\n",						Item[cnt].jar  );
			FilePrintf(hFile,"\t\t<XRay>%d</XRay>\r\n",						Item[cnt].xray  );
			FilePrintf(hFile,"\t\t<Batteries>%d</Batteries>\r\n",						Item[cnt].batteries  );
			FilePrintf(hFile,"\t\t<NeedsBatteries>%d</NeedsBatteries>\r\n",						Item[cnt].needsbatteries  );
			FilePrintf(hFile,"\t\t<ContainsLiquid>%d</ContainsLiquid>\r\n",						Item[cnt].containsliquid  );
			FilePrintf(hFile,"\t\t<MetalDetector>%d</MetalDetector>\r\n",						Item[cnt].metaldetector   );
			FilePrintf(hFile,"\t\t<FingerPrintID>%d</FingerPrintID>\r\n",						Item[cnt].fingerprintid    );

			FilePrintf(hFile,"\t</ITEM>\r\n");
		}
		FilePrintf(hFile,"</ITEMLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}