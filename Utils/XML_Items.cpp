	#include "sgp.h"
	#include "overhead types.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "math.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "items.h"
	#include "text.h"
	#include "Soldier Profile.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "Random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "morale.h"
	#include "SkillCheck.h"
	#include "gamesettings.h"
	#include "SaveLoadMap.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "utilities.h"
	#include "store inventory.h"

// Flugente: in order not to loop over MAXITEMS items if we only have a few thousand, remember the actual number of items in the xml
UINT32 gMAXITEMS_READ = 0;

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	INVTYPE		curItem;
	INVTYPE *	curArray;
	UINT32			maxArraySize;
	INT8		curStance;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef itemParseData;

BOOLEAN localizedTextOnly;

// HEADROCK HAM 4: Inherits data between stance-based modifiers
void InheritStanceModifiers( itemParseData *pData );

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

			if ( !localizedTextOnly )
				memset(pData->curArray,0,sizeof(INVTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			if ( !localizedTextOnly )
				memset(&pData->curItem,0,sizeof(INVTYPE));

			// Flugente: default value
			pData->curItem.usPortionSize = 100;

			// HEADROCK HAM 4: With the new stance-based variables, it is necessary to set vars to have an impossible
			// value. That way when they are later recorded in the item structs, a parent->child inheritence can occur
			// for children that do not have data put into them from XML.
			// -10000 has been selected to pose as "no value". Modders should never even reduce the value of any of
			// these tags below -100 anyway, and although it's not the best solution that's the only one I came up with.

			for (INT8 X = 0; X < 3; ++X)
			{
				pData->curItem.flatbasemodifier[X] = -10000;
				pData->curItem.percentbasemodifier[X] = -10000;
				pData->curItem.flataimmodifier[X] = -10000;
				pData->curItem.percentaimmodifier[X] = -10000;
				pData->curItem.percentcapmodifier[X] = -10000;
				pData->curItem.percenthandlingmodifier[X] = -10000;
				pData->curItem.targettrackingmodifier[X] = -10000;
				pData->curItem.percentdropcompensationmodifier[X] = -10000;
				pData->curItem.maxcounterforcemodifier[X] = -10000;
				pData->curItem.counterforceaccuracymodifier[X] = -10000;
				pData->curItem.aimlevelsmodifier[X] = -10000;
			}

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
				strcmp(name, "nasAttachmentClass") == 0 ||
				strcmp(name, "nasLayoutClass") == 0 ||
				strcmp(name, "AvailableAttachmentPoint") == 0 ||
				strcmp(name, "AttachmentPoint") == 0 ||
				strcmp(name, "AttachToPointAPCost") == 0 ||
				strcmp(name, "ubClassIndex") == 0 ||
				strcmp(name, "ubCursor") == 0 ||
				strcmp(name, "bSoundType") == 0 ||
				strcmp(name, "ubGraphicType") == 0 ||
				strcmp(name, "ubGraphicNum") == 0 ||
				strcmp(name, "ubWeight") == 0 ||
				strcmp(name, "ubPerPocket") == 0 ||
				strcmp(name, "ItemSize") == 0 ||
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
				strcmp(name, "PercentRangeBonus") == 0 ||
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
				strcmp(name, "ItemSizeBonus") == 0 ||
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
				strcmp(name, "AntitankMine" ) == 0 ||
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
				strcmp(name, "NewInv") == 0 ||
				strcmp(name, "AttachmentSystem") == 0 ||
				//zilpin: pellet spread patterns externalized in XML
				strcmp(name, "spreadPattern") == 0 ||
				// HEADROCK HAM 4: new NCTH variables.
				strcmp(name, "ScopeMagFactor") == 0 ||
				strcmp(name, "ProjectionFactor") == 0 ||
				strcmp(name, "PercentAccuracyModifier") == 0 ||
				strcmp(name, "RecoilModifierX") == 0 ||
				strcmp(name, "RecoilModifierY") == 0 ||
				strcmp(name, "PercentRecoilModifier") == 0 ||
				strcmp(name, "Barrel") == 0 ||
				strcmp(name, "usOverheatingCooldownFactor") == 0 ||
				strcmp(name, "overheatTemperatureModificator") == 0 ||
				strcmp(name, "overheatCooldownModificator") == 0 ||
				strcmp(name, "overheatJamThresholdModificator") == 0 ||
				strcmp(name, "overheatDamageThresholdModificator") == 0 ||
				strcmp(name, "AttachmentClass") == 0 ||
				strcmp(name, "TripWireActivation") == 0 ||
				strcmp(name, "TripWire") == 0 ||
				strcmp(name, "Directional") == 0 ||
				strcmp(name, "DrugType") == 0 ||
				strcmp(name, "BlockIronSight") == 0 ||
				strcmp(name, "ItemFlag") == 0 ||
				strcmp(name, "FoodType") == 0 ||
				strcmp(name, "DamageChance") == 0 ||
				strcmp(name, "DirtIncreaseFactor") == 0 ||

				strcmp(name, "fFlags") == 0 ||
				//JMich_SkillModifiers: Adding new flags
				strcmp(name, "LockPickModifier") == 0 ||
				strcmp(name, "CrowbarModifier") == 0 ||
				strcmp(name, "DisarmModifier") == 0 ||
				strcmp(name, "RepairModifier") == 0 ||
				strcmp(name, "usHackingModifier" ) == 0 ||
				strcmp(name, "usBurialModifier" ) == 0 ||
				
				strcmp(name, "usActionItemFlag") == 0 ||
				strcmp(name, "clothestype") == 0 ||
				strcmp(name, "randomitem") == 0 ||
				strcmp(name, "randomitemcoolnessmodificator") == 0 ||
				strcmp(name, "FlashLightRange") == 0 ||
				strcmp(name, "ItemChoiceTimeSetting") == 0 ||
				strcmp(name, "buddyitem") == 0 ||
				strcmp(name, "SleepModifier") == 0 ||
				strcmp(name, "usSpotting") == 0 ||
				strcmp(name, "sBackpackWeightModifier") == 0 ||
				strcmp(name, "AllowClimbing") == 0 ||
				strcmp(name, "cigarette" ) == 0 ||
				strcmp(name, "usPortionSize" ) == 0 ||
				strcmp(name, "DiseaseprotectionFace" ) == 0 ||
				strcmp(name, "DiseaseprotectionHand" ) == 0||
				strcmp(name, "usRiotShieldStrength" ) == 0 ||
				strcmp(name, "usRiotShieldGraphic" ) == 0 ||
				strcmp(name, "Bloodbag") == 0 ||
				strcmp(name, "Manpad" ) == 0 ||
				strcmp(name, "Beartrap") == 0 ||
				strcmp(name, "Camera") == 0 ||
				strcmp(name, "Waterdrum") == 0 ||
				strcmp(name, "BloodcatMeat") == 0 ||
				strcmp(name, "CowMeat") == 0 ||
				strcmp(name, "Beltfed") == 0 ||
				strcmp(name, "Ammobelt") == 0 ||
				strcmp(name, "AmmobeltVest") == 0 ||
				strcmp(name, "CamoRemoval") == 0 ||
				strcmp(name, "Cleaningkit") == 0 ||
				strcmp(name, "AttentionItem") == 0 ||
				strcmp(name, "Garotte") == 0 ||
				strcmp(name, "Covert") == 0 ||
				strcmp(name, "Corpse") == 0 ||
				strcmp(name, "BloodcatSkin") == 0 ||
				strcmp(name, "NoMetalDetection") == 0 ||
				strcmp(name, "JumpGrenade") == 0 ||
				strcmp(name, "Handcuffs") == 0 ||
				strcmp(name, "Taser") == 0 ||
				strcmp(name, "ScubaBottle") == 0 ||
				strcmp(name, "ScubaMask") == 0 ||
				strcmp(name, "ScubaFins") == 0 ||
				strcmp(name, "TripwireRoll") == 0 ||
				strcmp(name, "Radioset") == 0 ||
				strcmp(name, "SignalShell") == 0 ||
				strcmp(name, "Soda") == 0 ||
				strcmp(name, "RoofcollapseItem") == 0 ||
				strcmp(name, "LBEexplosionproof") == 0 ||
				strcmp(name, "EmptyBloodbag" ) == 0 ||
				strcmp(name, "MedicalSplint" ) == 0 ||
				strcmp(name, "sFireResistance" ) == 0 ||
				strcmp(name, "usAdministrationModifier" ) == 0) ||
				strcmp(name, "RobotDamageReduction") == 0 ||
				strcmp(name, "RobotStrBonus") == 0 ||
				strcmp(name, "RobotAgiBonus") == 0 ||
				strcmp(name, "RobotDexBonus") == 0 ||
				strcmp(name, "RobotTargetingSkillGrant") == 0 ||
				strcmp(name, "RobotChassisSkillGrant") == 0 ||
				strcmp(name, "RobotUtilitySkillGrant") == 0 ||
				strcmp(name, "ProvidesRobotCamo") == 0 ||
				strcmp(name, "ProvidesRobotNightVision") == 0 ||
				strcmp(name, "ProvidesRobotLaserBonus") == 0 ||
				strcmp(name, "DiseaseSystemExclusive") == 0 ||
				strcmp(name, "TransportGroupMinProgress") == 0 ||
				strcmp(name, "TransportGroupMaxProgress") == 0
				)
		{
			pData->curElement = ELEMENT_PROPERTY;
			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("itemStartElementHandle: going into element, name = %s",name) );

			pData->maxReadDepth++; //we are not skipping this element
		}
		// HEADROCK HAM 4: New depth: Stance-based variables
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "STAND_MODIFIERS") == 0 ||
				strcmp(name, "CROUCH_MODIFIERS") == 0 ||
				strcmp(name, "PRONE_MODIFIERS") == 0))
		{
			pData->curElement = ELEMENT_SUBLIST;

			// Set current stance.
			if (strcmp(name, "STAND_MODIFIERS") == 0)
			{
				pData->curStance = 0;
			}
			else if (strcmp(name, "CROUCH_MODIFIERS") == 0)
			{
				pData->curStance = 1;
			}
			else // prone
			{
				pData->curStance = 2;
			}

			pData->maxReadDepth++;
		}
		
		// HEADROCK HAM 4: Read stance-based variables
		else if(pData->curElement == ELEMENT_SUBLIST &&
				(strcmp(name, "FlatBase") == 0 ||
				strcmp(name, "PercentBase") == 0 ||
				strcmp(name, "FlatAim") == 0 ||
				strcmp(name, "PercentAim") == 0 ||
				strcmp(name, "PercentCap") == 0 ||
				strcmp(name, "PercentHandling") == 0 ||
				strcmp(name, "PercentTargetTrackingSpeed") == 0 ||
				strcmp(name, "PercentDropCompensation") == 0 ||
				strcmp(name, "PercentMaxCounterForce") == 0 ||
				strcmp(name, "PercentCounterForceAccuracy") == 0 ||
				strcmp(name, "AimLevels") == 0))
		{
			pData->curElement = ELEMENT_SUBLIST_PROPERTY;

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
				{
					// HEADROCK HAM 4: Inherit stance-base modifiers upwards.
					InheritStanceModifiers( pData );

					pData->curArray[pData->curItem.uiIndex] = pData->curItem; //write the item into the table

					// Flugente: new item -> read items increase
					gMAXITEMS_READ = pData->curItem.uiIndex;
				}
				else if ( sizeof(pData->curItem.szItemName)>0 && localizedTextOnly )
				{
					wcscpy(pData->curArray[pData->curItem.uiIndex].szItemName,pData->curItem.szItemName);
					wcscpy(pData->curArray[pData->curItem.uiIndex].szLongItemName,pData->curItem.szLongItemName);
					wcscpy(pData->curArray[pData->curItem.uiIndex].szBRName,pData->curItem.szBRName);
					wcscpy(pData->curArray[pData->curItem.uiIndex].szItemDesc,pData->curItem.szItemDesc);
					wcscpy(pData->curArray[pData->curItem.uiIndex].szBRDesc,pData->curItem.szBRDesc);
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

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curItem.szItemName, sizeof(pData->curItem.szItemName)/sizeof(pData->curItem.szItemName[0]) );
			pData->curItem.szItemName[sizeof(pData->curItem.szItemName)/sizeof(pData->curItem.szItemName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szLongItemName") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: longitemname");
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curItem.szLongItemName, sizeof(pData->curItem.szLongItemName)/sizeof(pData->curItem.szLongItemName[0]) );
			pData->curItem.szLongItemName[sizeof(pData->curItem.szLongItemName)/sizeof(pData->curItem.szLongItemName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szItemDesc") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: itemdesc");
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curItem.szItemDesc, sizeof(pData->curItem.szItemDesc)/sizeof(pData->curItem.szItemDesc[0]) );
			pData->curItem.szItemDesc[sizeof(pData->curItem.szItemDesc)/sizeof(pData->curItem.szItemDesc[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szBRName") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: brname");
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curItem.szBRName, sizeof(pData->curItem.szBRName)/sizeof(pData->curItem.szBRName[0]) );
			pData->curItem.szBRName[sizeof(pData->curItem.szBRName)/sizeof(pData->curItem.szBRName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szBRDesc") == 0)
		{
			//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"itemEndElementHandle: brdesc");
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curItem.szBRDesc, sizeof(pData->curItem.szBRDesc)/sizeof(pData->curItem.szBRDesc[0]) );
			pData->curItem.szBRDesc[sizeof(pData->curItem.szBRDesc)/sizeof(pData->curItem.szBRDesc[0]) - 1] = '\0';
		}
		else if(strcmp(name, "usItemClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usItemClass = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "nasAttachmentClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.nasAttachmentClass = (UINT64) atof(pData->szCharData);
		}
		else if(strcmp(name, "nasLayoutClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.nasLayoutClass = (UINT64) atof(pData->szCharData);
		}
		else if(strcmp(name, "AvailableAttachmentPoint") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ulAvailableAttachmentPoint |= (UINT64) atof(pData->szCharData);
		}
		else if(strcmp(name, "AttachmentPoint") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ulAttachmentPoint = (UINT64) atof(pData->szCharData);
		}
		else if(strcmp(name, "AttachToPointAPCost") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubAttachToPointAPCost = (UINT8) atol(pData->szCharData);
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
			pData->curItem.ubWeight = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubPerPocket") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubPerPocket = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ItemSize = (UINT16) atol(pData->szCharData);
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
		else if(strcmp(name, "Damageable")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_damageable;
		}
		else if(strcmp(name, "Repairable")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_repairable;
		}
		else if(strcmp(name, "WaterDamages")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_waterdamages;
		}
		else if(strcmp(name, "Metal")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_metal;
		}
		else if(strcmp(name, "Sinks")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_sinks;
		}
		else if(strcmp(name, "ShowStatus")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_showstatus;
		}
		else if(strcmp(name, "HiddenAddon")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_hiddenaddon;
		}
		else if(strcmp(name, "TwoHanded")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_twohanded;
		}
		else if(strcmp(name, "NotBuyable")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_notbuyable;
		}
		else if(strcmp(name, "Attachment")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_attachment;
		}
		else if(strcmp(name, "HiddenAttachment")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_hiddenattachment;
		}
		else if(strcmp(name, "BigGunList")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_biggunlist;
		}
		else if(strcmp(name, "NotInEditor")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_notineditor;
		}
		else if(strcmp(name, "DefaultUndroppable")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_defaultundroppable;
		}
		else if(strcmp(name, "Unaerodynamic")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_unaerodynamic;
		}
		else if(strcmp(name, "Electronic")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_electronic;
		}
		else if(strcmp(name, "Inseparable")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.inseparable = (UINT8) atol(pData->szCharData);
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
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_gasmask;
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
		else if(strcmp(name, "PercentRangeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentrangebonus  = (INT16) atol(pData->szCharData);
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
			pData->curItem.APBonus = (INT16)DynamicAdjustAPConstants(pData->curItem.APBonus, pData->curItem.APBonus);
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
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_grenadelauncher;
		}
		else if(strcmp(name, "LockBomb")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_lockbomb;
		}
		else if(strcmp(name, "Flare")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_flare;
		}
		else if(strcmp(name, "Duckbill")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_duckbill;
		}
		else if(strcmp(name, "Detonator")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_detonator;
		}
		else if(strcmp(name, "RemoteDetonator")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_remotedetonator;
		}
		else if(strcmp(name, "ThermalOptics")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_thermaloptics;
		}
		else if(strcmp(name, "SciFi")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_scifi;
		}
		else if(strcmp(name, "NewInv")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_newinv;
		}
		else if(strcmp(name, "AttachmentSystem")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubAttachmentSystem   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "HideMuzzleFlash")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_hidemuzzleflash;
		}
		else if(strcmp(name, "Cannon")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_cannon;
		}
		else if(strcmp(name, "RocketRifle")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_rocketrifle;
		}
		else if(strcmp(name, "Alcohol")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.alcohol  = (FLOAT) atof(pData->szCharData);

			pData->curItem.alcohol = max( 0.0f, pData->curItem.alcohol );
		}
		else if(strcmp(name, "Hardware")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_hardware;
		}
		else if(strcmp(name, "Medical")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_medical;
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
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_mortar;
		}
		else if(strcmp(name, "RocketLauncher")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_rocketlauncher;
		}
		else if(strcmp(name, "SingleShotRocketLauncher")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_singleshotrocketlauncher;
		}
		else if(strcmp(name, "DiscardedLauncherItem")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.discardedlauncheritem  = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "BloodiedItem")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bloodieditem  = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "DefaultAttachment")	 == 0)
		{
			pData->curElement = ELEMENT;
			for(UINT8 cnt = 0; cnt < MAX_DEFAULT_ATTACHMENTS; cnt++){
				if(pData->curItem.defaultattachments[cnt] == 0){
					pData->curItem.defaultattachments[cnt]  = (UINT16) atol(pData->szCharData);
					break;
				}
			}
		}
		else if(strcmp(name, "BrassKnuckles")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_brassknuckles;
		}
		else if(strcmp(name, "Crowbar")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_crowbar;
		}
		else if(strcmp(name, "GLGrenade")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_glgrenade;
		}
		else if(strcmp(name, "FlakJacket")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_flakjacket;
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
		if(strcmp(name, "ItemSizeBonus")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.itemsizebonus    = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "LeatherJacket")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_leatherjacket;
		}
		else if(strcmp(name, "NeedsBatteries")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_needsbatteries;
		}
		else if(strcmp(name, "Batteries")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_batteries;
		}
		else if(strcmp(name, "XRay")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_xray;
		}
		else if(strcmp(name, "WireCutters")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_wirecutters;
		}
		else if(strcmp(name, "Toolkit")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_toolkit;
		}
		else if(strcmp(name, "Canteen")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_canteen;
		}
		else if(strcmp(name, "Marbles")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_marbles;
		}
		else if(strcmp(name, "Walkman")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_walkman;
		}
		else if(strcmp(name, "RemoteTrigger")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_remotetrigger;
		}
		else if(strcmp(name, "RobotRemoteControl")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_robotremotecontrol;
		}
		else if(strcmp(name, "CamouflageKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_camouflagekit;
		}
		else if(strcmp(name, "LocksmithKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_locksmithkit;
		}
		else if(strcmp(name, "Mine")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_mine;
		}
		else if ( strcmp( name, "AntitankMine" ) == 0 )
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_antitankmine;
		}
		else if(strcmp(name, "GasCan")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_gascan;
		}
		else if(strcmp(name, "CanAndString")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_canandstring;
		}
		else if(strcmp(name, "ContainsLiquid")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_containsliquid;
		}
		else if(strcmp(name, "FingerPrintID")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_fingerprintid;
		}
		else if(strcmp(name, "Rock")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_rock;
		}
		else if(strcmp(name, "MedicalKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_medicalkit;
		}
		else if(strcmp(name, "FirstAidKit")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_firstaidkit;
		}
		else if(strcmp(name, "MetalDetector")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag |= ITEM_metaldetector;
		}
		if(strcmp(name, "PercentTunnelVision")	 == 0) //Madd: had to scrap the "else" due to a compiler limit
		{
			pData->curElement = ELEMENT;
			pData->curItem.percenttunnelvision  = (UINT8) atol(pData->szCharData);
		}
		if(strcmp(name, "Jar")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_jar;
		}
		if(strcmp(name, "BestLaserRange")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bestlaserrange    = (INT16) atol(pData->szCharData);
		}
		//zilpin: pellet spread patterns externalized in XML
		if(strcmp(name, "spreadPattern") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.spreadPattern = FindSpreadPatternIndex( pData->szCharData );
		}

		//////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: Read new variables from XML
		else if(strcmp(name, "ScopeMagFactor")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.scopemagfactor  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "ProjectionFactor")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.projectionfactor  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "PercentAccuracyModifier")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.percentaccuracymodifier  = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "RecoilModifierX")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.RecoilModifierX  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "RecoilModifierY")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.RecoilModifierY  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "PercentRecoilModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.PercentRecoilModifier = (INT16) atol(pData->szCharData);
		}


		//////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: Read stance-based variables and put them into the right place.
		else if(strcmp(name, "FlatBase") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.flatbasemodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentBase") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.percentbasemodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "FlatAim") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.flataimmodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentAim") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.percentaimmodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentCap") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.percentcapmodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentHandling") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.percenthandlingmodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentTargetTrackingSpeed") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.targettrackingmodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentDropCompensation") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.percentdropcompensationmodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentMaxCounterForce") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.maxcounterforcemodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentCounterForceAccuracy") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.counterforceaccuracymodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "AimLevels") == 0)
		{
			pData->curElement = ELEMENT_SUBLIST;
			pData->curItem.aimlevelsmodifier[pData->curStance] = (INT16) atol(pData->szCharData);
		}

		//////////////////////////////////////////////////////////////////
		// HEADROCK HAM 4: Close opened Stance Tags.
		else if(strcmp(name, "STAND_MODIFIERS") == 0 ||
				strcmp(name, "CROUCH_MODIFIERS") == 0 ||
				strcmp(name, "PRONE_MODIFIERS") == 0)
		{
			pData->curElement = ELEMENT;
		}

		// Flugente
		else if(strcmp(name, "Barrel")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_barrel;
		}
		else if(strcmp(name, "usOverheatingCooldownFactor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usOverheatingCooldownFactor  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "overheatTemperatureModificator") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.overheatTemperatureModificator  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "overheatCooldownModificator") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.overheatCooldownModificator  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "overheatJamThresholdModificator") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.overheatJamThresholdModificator  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "overheatDamageThresholdModificator") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.overheatDamageThresholdModificator  = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "AttachmentClass")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.attachmentclass   = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "TripWireActivation")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_tripwireactivation;
		}
		else if(strcmp(name, "TripWire")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_tripwire;
		}
		else if(strcmp(name, "Directional")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_directional;
		}
		else if(strcmp(name, "DrugType")	 == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.drugtype   = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "BlockIronSight")	 == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_blockironsight;
		}
		else if(strcmp(name, "FoodType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.foodtype = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		//JMich_SkillsModifiers: Parse new values
		else if(strcmp(name, "LockPickModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.LockPickModifier = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "CrowbarModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.CrowbarModifier = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "DisarmModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.DisarmModifier = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "RepairModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.RepairModifier = (INT8) atol(pData->szCharData);
		}
		else if ( strcmp( name, "usHackingModifier" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curItem.usHackingModifier = min(100, (UINT8)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "usBurialModifier" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curItem.usBurialModifier = min( 100, (UINT8)atol( pData->szCharData ) );
		}
		else if(strcmp(name, "DamageChance") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usDamageChance = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "DirtIncreaseFactor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.dirtIncreaseFactor = (FLOAT) atof(pData->szCharData);
		}
		else if(strcmp(name, "usActionItemFlag") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usActionItemFlag = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "clothestype") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.clothestype = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "randomitem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.randomitem = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "randomitemcoolnessmodificator") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.randomitemcoolnessmodificator = (INT8) atol(pData->szCharData);

			// no nonsense, only values between -20 and + 20
			pData->curItem.randomitemcoolnessmodificator = min(20, max(-20, pData->curItem.randomitemcoolnessmodificator) );
		}
		else if(strcmp(name, "FlashLightRange") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usFlashLightRange = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemChoiceTimeSetting") == 0)
		{
			pData->curElement = ELEMENT;
			// no nonsense, only values between 0 and + 2
			pData->curItem.usItemChoiceTimeSetting = min(2, max(0, (UINT8) atol(pData->szCharData) ) );
		}
		else if(strcmp(name, "buddyitem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.usBuddyItem = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "SleepModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.ubSleepModifier = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usSpotting") == 0)
		{
			pData->curElement = ELEMENT;
			// values between 0 and 100 only
			pData->curItem.usSpotting = min(100, max(0, (INT16) atol(pData->szCharData) ) );
		}
		else if (strcmp(name, "sBackpackWeightModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.sBackpackWeightModifier = (INT16)atol(pData->szCharData);
		}
		else if (strcmp(name, "AllowClimbing") == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_fAllowClimbing;
		}
		else if ( strcmp( name, "cigarette" ) == 0 )
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_cigarette;
		}
		else if ( strcmp( name, "usPortionSize" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curItem.usPortionSize = (UINT8)atol( pData->szCharData );
		}
		// Flugente: simple tags in the xml get translated into flags
		else if ( strcmp( name, "DiseaseprotectionFace" ) == 0 )
		{
			pData->curElement = ELEMENT;
			BOOLEAN val = (BOOLEAN)atol( pData->szCharData );

			if ( val )
				pData->curItem.usItemFlag |= DISEASEPROTECTION_1;
		}
		else if ( strcmp( name, "DiseaseprotectionHand" ) == 0 )
		{
			pData->curElement = ELEMENT;
			BOOLEAN val = (BOOLEAN)atol( pData->szCharData );

			if ( val )
				pData->curItem.usItemFlag |= DISEASEPROTECTION_2;
		}
		else if ( strcmp( name, "usRiotShieldStrength" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curItem.usRiotShieldStrength = min( 100, (UINT16)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "usRiotShieldGraphic" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curItem.usRiotShieldGraphic = (UINT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "Bloodbag" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= BLOOD_BAG;
		}
		else if ( strcmp( name, "Manpad" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= MANPAD;
		}
		else if ( strcmp( name, "Beartrap" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= BEARTRAP;
		}
		else if ( strcmp( name, "Camera" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= CAMERA;
		}
		else if ( strcmp( name, "Waterdrum" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= WATER_DRUM;
		}
		else if ( strcmp( name, "BloodcatMeat" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= MEAT_BLOODCAT;
		}
		else if ( strcmp( name, "CowMeat" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= MEAT_COW;
		}
		else if ( strcmp( name, "Beltfed" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= BELT_FED;
		}
		else if ( strcmp( name, "Ammobelt" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= AMMO_BELT;
		}
		else if ( strcmp( name, "AmmobeltVest" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= AMMO_BELT_VEST;
		}
		else if ( strcmp( name, "CamoRemoval" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= CAMO_REMOVAL;
		}
		else if ( strcmp( name, "Cleaningkit" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= CLEANING_KIT;
		}
		else if ( strcmp( name, "AttentionItem" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= ATTENTION_ITEM;
		}
		else if ( strcmp( name, "Garotte" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= GAROTTE;
		}
		else if ( strcmp( name, "Covert" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= COVERT;
		}
		else if ( strcmp( name, "Corpse" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= CORPSE;
		}
		else if ( strcmp( name, "BloodcatSkin" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= SKIN_BLOODCAT;
		}
		else if ( strcmp( name, "NoMetalDetection" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= NO_METAL_DETECTION;
		}
		else if ( strcmp( name, "JumpGrenade" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= JUMP_GRENADE;
		}
		else if ( strcmp( name, "Handcuffs" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= HANDCUFFS;
		}
		else if ( strcmp( name, "Taser" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= TASER;
		}
		else if ( strcmp( name, "ScubaBottle" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= SCUBA_BOTTLE;
		}
		else if ( strcmp( name, "ScubaMask" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= SCUBA_MASK;
		}
		else if ( strcmp( name, "ScubaFins" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= SCUBA_FINS;
		}
		else if ( strcmp( name, "TripwireRoll" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= TRIPWIREROLL;
		}
		else if ( strcmp( name, "Radioset" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= RADIO_SET;
		}
		else if ( strcmp( name, "SignalShell" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= SIGNAL_SHELL;
		}
		else if ( strcmp( name, "Soda" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= SODA;
		}
		else if ( strcmp( name, "RoofcollapseItem" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= ROOF_COLLAPSE_ITEM;
		}
		else if ( strcmp( name, "LBEexplosionproof" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= LBE_EXPLOSIONPROOF;
		}
		else if ( strcmp( name, "EmptyBloodbag" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= EMPTY_BLOOD_BAG;
		}
		else if ( strcmp( name, "MedicalSplint" ) == 0 )
		{
			pData->curElement = ELEMENT;

			if ( (BOOLEAN)atol( pData->szCharData ) )
				pData->curItem.usItemFlag |= MEDICAL_SPLINT;
		}
		else if ( strcmp( name, "sFireResistance" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curItem.sFireResistance = min( 100, (INT16)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "usAdministrationModifier" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curItem.usAdministrationModifier = (UINT8)atol( pData->szCharData );
		}
		else if (strcmp(name, "RobotDamageReduction") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.fRobotDamageReductionModifier = (FLOAT)atof(pData->szCharData);
		}
		else if (strcmp(name, "RobotStrBonus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bRobotStrBonus = (INT8)atol(pData->szCharData);
		}
		else if (strcmp(name, "RobotAgiBonus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bRobotAgiBonus = (INT8)atol(pData->szCharData);
		}
		else if (strcmp(name, "RobotDexBonus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bRobotDexBonus = (INT8)atol(pData->szCharData);
		}
		else if (strcmp(name, "RobotTargetingSkillGrant") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bRobotTargetingSkillGrant = (INT8)atol(pData->szCharData);
		}
		else if (strcmp(name, "RobotChassisSkillGrant") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bRobotChassisSkillGrant = (INT8)atol(pData->szCharData);
		}
		else if (strcmp(name, "RobotUtilitySkillGrant") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.bRobotUtilitySkillGrant = (INT8)atol(pData->szCharData);
		}
		else if (strcmp(name, "ProvidesRobotCamo") == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_fProvidesRobotCamo;
		}
		else if (strcmp(name, "ProvidesRobotNightVision") == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_fProvidesRobotNightVision;
		}
		else if (strcmp(name, "ProvidesRobotLaserBonus") == 0)
		{
			pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_fProvidesRobotLaserBonus;
		}
		else if (strcmp(name, "DiseaseSystemExclusive") == 0)
		{
		    pData->curElement = ELEMENT;
			if ((BOOLEAN)atol(pData->szCharData))
				pData->curItem.usItemFlag2 |= ITEM_DiseaseSystemExclusive;
		}
		else if (strcmp(name, "TransportGroupMinProgress") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.iTransportGroupMinProgress = (INT8)atoi(pData->szCharData);
		}
		else if (strcmp(name, "TransportGroupMaxProgress") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curItem.iTransportGroupMaxProgress = (INT8)atoi(pData->szCharData);
		}

		--pData->maxReadDepth;
	}

	--pData->currentDepth;
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

	if (localizedTextOnly == false)
	{
		// item read was x -> x+1 items
		++gMAXITEMS_READ;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

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
		for(cnt = 0;cnt < 351; ++cnt)//just do the old limit for now
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
			FilePrintf(hFile,"\t\t<nasAttachmentClass>%d</nasAttachmentClass>\r\n",			Item[cnt].nasAttachmentClass);
			FilePrintf(hFile,"\t\t<nasLayoutClass>%d</nasLayoutClass>\r\n",					Item[cnt].nasLayoutClass);
			FilePrintf(hFile,"\t\t<ubClassIndex>%d</ubClassIndex>\r\n",						Item[cnt].ubClassIndex);
			FilePrintf(hFile,"\t\t<ubCursor>%d</ubCursor>\r\n",								Item[cnt].ubCursor);
			FilePrintf(hFile,"\t\t<bSoundType>%d</bSoundType>\r\n",							Item[cnt].bSoundType);
			FilePrintf(hFile,"\t\t<ubGraphicType>%d</ubGraphicType>\r\n",					Item[cnt].ubGraphicType);
			FilePrintf(hFile,"\t\t<ubGraphicNum>%d</ubGraphicNum>\r\n",						Item[cnt].ubGraphicNum);
			FilePrintf(hFile,"\t\t<ubWeight>%d</ubWeight>\r\n",								Item[cnt].ubWeight);
			FilePrintf(hFile,"\t\t<ubPerPocket>%d</ubPerPocket>\r\n",						Item[cnt].ubPerPocket);
			FilePrintf(hFile,"\t\t<ItemSize>%d</ItemSize>\r\n",								Item[cnt].ItemSize);
			FilePrintf(hFile,"\t\t<usPrice>%d</usPrice>\r\n",								Item[cnt].usPrice);
			FilePrintf(hFile,"\t\t<ubCoolness>%d</ubCoolness>\r\n",							Item[cnt].ubCoolness);
			FilePrintf(hFile,"\t\t<bReliability>%d</bReliability>\r\n",						Item[cnt].bReliability);
			FilePrintf(hFile,"\t\t<bRepairEase>%d</bRepairEase>\r\n",						Item[cnt].bRepairEase);

			FilePrintf(hFile,"\t\t<AttachmentSystem>%d</AttachmentSystem>\r\n",				Item[cnt].ubAttachmentSystem   );
			FilePrintf(hFile,"\t\t<Inseparable>%d</Inseparable>\r\n",						Item[cnt].inseparable );

			FilePrintf(hFile,"\t\t<BR_NewInventory>%d</BR_NewInventory>\r\n",				StoreInventory[cnt][0] );
			FilePrintf(hFile,"\t\t<BR_UsedInventory>%d</BR_UsedInventory>\r\n",				StoreInventory[cnt][1] );
			FilePrintf(hFile,"\t\t<BR_ROF>%d</BR_ROF>\r\n",									WeaponROF[cnt]);

			FilePrintf(hFile,"\t\t<PercentNoiseReduction>%d</PercentNoiseReduction>\r\n",	Item[cnt].percentnoisereduction  );
			FilePrintf(hFile,"\t\t<Bipod>%d</Bipod>\r\n",									Item[cnt].bipod  );
			FilePrintf(hFile,"\t\t<RangeBonus>%d</RangeBonus>\r\n",							Item[cnt].rangebonus   );
			FilePrintf(hFile,"\t\t<PercentRangeBonus>%d</PercentRangeBonus>\r\n",			Item[cnt].rangebonus   );
			FilePrintf(hFile,"\t\t<ToHitBonus>%d</ToHitBonus>\r\n",							Item[cnt].tohitbonus    );
			FilePrintf(hFile,"\t\t<AimBonus>%d</AimBonus>\r\n",								Item[cnt].aimbonus   );
			FilePrintf(hFile,"\t\t<MinRangeForAimBonus>%d</MinRangeForAimBonus>\r\n",		Item[cnt].minrangeforaimbonus  );

			FilePrintf(hFile,"\t\t<MagSizeBonus>%d</MagSizeBonus>\r\n",						Item[cnt].magsizebonus    );
			FilePrintf(hFile,"\t\t<RateOfFireBonus>%d</RateOfFireBonus>\r\n",				Item[cnt].rateoffirebonus   );
			FilePrintf(hFile,"\t\t<BulletSpeedBonus>%d</BulletSpeedBonus>\r\n",				Item[cnt].bulletspeedbonus );
			FilePrintf(hFile,"\t\t<BurstSizeBonus>%d</BurstSizeBonus>\r\n",					Item[cnt].burstsizebonus );
			FilePrintf(hFile,"\t\t<BestLaserRange>%d</BestLaserRange>\r\n",					Item[cnt].bestlaserrange );
			FilePrintf(hFile,"\t\t<BurstToHitBonus>%d</BurstToHitBonus>\r\n",				Item[cnt].bursttohitbonus );
			FilePrintf(hFile,"\t\t<AutofireToHitBonus>%d</AutofireToHitBonus>\r\n",			Item[cnt].autofiretohitbonus);
			FilePrintf(hFile,"\t\t<APBonus>%d</APBonus>\r\n",								Item[cnt].APBonus );

			FilePrintf(hFile,"\t\t<PercentBurstFireAPReduction>%d</PercentBurstFireAPReduction>\r\n",		Item[cnt].percentburstfireapreduction    );
			FilePrintf(hFile,"\t\t<PercentAutofireAPReduction>%d</PercentAutofireAPReduction>\r\n",			Item[cnt].percentautofireapreduction    );
			FilePrintf(hFile,"\t\t<PercentReadyTimeAPReduction>%d</PercentReadyTimeAPReduction>\r\n",		Item[cnt].percentreadytimeapreduction );
			FilePrintf(hFile,"\t\t<PercentReloadTimeAPReduction>%d</PercentReloadTimeAPReduction>\r\n",		Item[cnt].percentreloadtimeapreduction );
			FilePrintf(hFile,"\t\t<PercentAPReduction>%d</PercentAPReduction>\r\n",							Item[cnt].percentapreduction   );
			FilePrintf(hFile,"\t\t<PercentStatusDrainReduction>%d</PercentStatusDrainReduction>\r\n",		Item[cnt].percentstatusdrainreduction   );

			FilePrintf(hFile,"\t\t<DamageBonus>%d</DamageBonus>\r\n",						Item[cnt].damagebonus   );
			FilePrintf(hFile,"\t\t<MeleeDamageBonus>%d</MeleeDamageBonus>\r\n",				Item[cnt].meleedamagebonus  );
			FilePrintf(hFile,"\t\t<DiscardedLauncherItem>%d</DiscardedLauncherItem>\r\n",	Item[cnt].discardedlauncheritem  );

			
			for(UINT8 cnt2 = 0; cnt2 < MAX_DEFAULT_ATTACHMENTS; cnt2++){
				if(Item[cnt].defaultattachments[cnt2] != 0){
					FilePrintf(hFile,"\t\t<DefaultAttachment>%d</DefaultAttachment>\r\n",	Item[cnt].defaultattachments[cnt2]  );
				}
			}


			FilePrintf(hFile,"\t\t<BloodiedItem>%d</BloodiedItem>\r\n",						Item[cnt].bloodieditem  );
			FilePrintf(hFile,"\t\t<CamoBonus>%d</CamoBonus>\r\n",							Item[cnt].camobonus  );
			FilePrintf(hFile,"\t\t<UrbanCamoBonus>%d</UrbanCamoBonus>\r\n",					Item[cnt].urbanCamobonus  );
			FilePrintf(hFile,"\t\t<DesertCamoBonus>%d</DesertCamoBonus>\r\n",				Item[cnt].desertCamobonus  );
			FilePrintf(hFile,"\t\t<SnowCamoBonus>%d</SnowCamoBonus>\r\n",					Item[cnt].snowCamobonus );
			FilePrintf(hFile,"\t\t<StealthBonus>%d</StealthBonus>\r\n",						Item[cnt].stealthbonus  );

			FilePrintf(hFile,"\t\t<HearingRangeBonus>%d</HearingRangeBonus>\r\n",						Item[cnt].hearingrangebonus  );
			FilePrintf(hFile,"\t\t<VisionRangeBonus>%d</VisionRangeBonus>\r\n",							Item[cnt].visionrangebonus  );
			FilePrintf(hFile,"\t\t<NightVisionRangeBonus>%d</NightVisionRangeBonus>\r\n",				Item[cnt].nightvisionrangebonus  );
			FilePrintf(hFile,"\t\t<DayVisionRangeBonus>%d</DayVisionRangeBonus>\r\n",					Item[cnt].dayvisionrangebonus  );
			FilePrintf(hFile,"\t\t<CaveVisionRangeBonus>%d</CaveVisionRangeBonus>\r\n",					Item[cnt].cavevisionrangebonus  );
			FilePrintf(hFile,"\t\t<BrightLightVisionRangeBonus>%d</BrightLightVisionRangeBonus>\r\n",	Item[cnt].brightlightvisionrangebonus  );
			FilePrintf(hFile,"\t\t<ItemSizeBonus>%d</ItemSizeBonus>\r\n",								Item[cnt].itemsizebonus  );
			FilePrintf(hFile,"\t\t<PercentTunnelVision>%d</PercentTunnelVision>\r\n",					Item[cnt].percenttunnelvision );
			FilePrintf(hFile,"\t\t<Alcohol>%3.2f</Alcohol>\r\n",											Item[cnt].alcohol  );

			// HEADROCK HAM 4: Print out new values
			FilePrintf(hFile,"\t\t<ScopeMagFactor>%d</ScopeMagFactor>\r\n",						Item[cnt].scopemagfactor    );
			FilePrintf(hFile,"\t\t<ProjectionFactor>%d</ProjectionFactor>\r\n",					Item[cnt].projectionfactor    );
			FilePrintf(hFile,"\t\t<PercentAccuracyModifier>%d</PercentAccuracyModifier>\r\n",	Item[cnt].percentaccuracymodifier    );
			FilePrintf(hFile,"\t\t<RecoilModifierX>%d</RecoilModifierX>\r\n",					Item[cnt].RecoilModifierX    );
			FilePrintf(hFile,"\t\t<RecoilModifierY>%d</RecoilModifierY>\r\n",					Item[cnt].RecoilModifierY    );
			FilePrintf(hFile,"\t\t<PercentRecoilModifier>%d</PercentRecoilModifier>\r\n",		Item[cnt].PercentRecoilModifier		);

			// HEADROCK HAM 4: Print out stance-based values
			FilePrintf(hFile,"\t\t<STAND_MODIFIERS>\r\n");
			FilePrintf(hFile,"\t\t\t<FlatBase>%d</FlatBase>\r\n",											Item[cnt].flatbasemodifier[0]    );
			FilePrintf(hFile,"\t\t\t<PercentBase>%d</PercentBase>\r\n",										Item[cnt].percentbasemodifier[0]    );
			FilePrintf(hFile,"\t\t\t<FlatAim>%d</FlatAim>\r\n",												Item[cnt].flataimmodifier[0]    );
			FilePrintf(hFile,"\t\t\t<PercentCap>%d</PercentCap>\r\n",										Item[cnt].percentcapmodifier[0]    );
			FilePrintf(hFile,"\t\t\t<PercentHandling>%d</PercentHandling>\r\n",								Item[cnt].percenthandlingmodifier[0]    );
			FilePrintf(hFile,"\t\t\t<PercentTargetTrackingSpeed>%d</PercentTargetTrackingSpeed>\r\n",		Item[cnt].targettrackingmodifier[0]    );
			FilePrintf(hFile,"\t\t\t<PercentDropCompensation>%d</PercentDropCompensation>\r\n",				Item[cnt].percentdropcompensationmodifier[0]    );
			FilePrintf(hFile,"\t\t\t<PercentMaxCounterForce>%d</PercentMaxCounterForce>\r\n",				Item[cnt].maxcounterforcemodifier[0]    );
			FilePrintf(hFile,"\t\t\t<PercentCounterForceAccuracy>%d</PercentCounterForceAccuracy>\r\n",		Item[cnt].counterforceaccuracymodifier[0]    );
			FilePrintf(hFile,"\t\t\t<AimLevels>%d</AimLevels>\r\n",											Item[cnt].aimlevelsmodifier[0]    );
			FilePrintf(hFile,"\t\t</STAND_MODIFIERS>\r\n");

			FilePrintf(hFile,"\t\t<CROUCH_MODIFIERS>\r\n");
			FilePrintf(hFile,"\t\t\t<FlatBase>%d</FlatBase>\r\n",											Item[cnt].flatbasemodifier[1]    );
			FilePrintf(hFile,"\t\t\t<PercentBase>%d</PercentBase>\r\n",										Item[cnt].percentbasemodifier[1]    );
			FilePrintf(hFile,"\t\t\t<FlatAim>%d</FlatAim>\r\n",												Item[cnt].flataimmodifier[1]    );
			FilePrintf(hFile,"\t\t\t<PercentCap>%d</PercentCap>\r\n",										Item[cnt].percentcapmodifier[1]    );
			FilePrintf(hFile,"\t\t\t<PercentHandling>%d</PercentHandling>\r\n",								Item[cnt].percenthandlingmodifier[1]    );
			FilePrintf(hFile,"\t\t\t<PercentTargetTrackingSpeed>%d</PercentTargetTrackingSpeed>\r\n",		Item[cnt].targettrackingmodifier[1]    );
			FilePrintf(hFile,"\t\t\t<PercentDropCompensation>%d</PercentDropCompensation>\r\n",				Item[cnt].percentdropcompensationmodifier[1]    );
			FilePrintf(hFile,"\t\t\t<PercentMaxCounterForce>%d</PercentMaxCounterForce>\r\n",				Item[cnt].maxcounterforcemodifier[1]    );
			FilePrintf(hFile,"\t\t\t<PercentCounterForceAccuracy>%d</PercentCounterForceAccuracy>\r\n",		Item[cnt].counterforceaccuracymodifier[1]    );
			FilePrintf(hFile,"\t\t\t<AimLevels>%d</AimLevels>\r\n",											Item[cnt].aimlevelsmodifier[1]    );
			FilePrintf(hFile,"\t\t</CROUCH_MODIFIERS>\r\n");

			FilePrintf(hFile,"\t\t<PRONE_MODIFIERS>\r\n");
			FilePrintf(hFile,"\t\t\t<FlatBase>%d</FlatBase>\r\n",											Item[cnt].flatbasemodifier[2]    );
			FilePrintf(hFile,"\t\t\t<PercentBase>%d</PercentBase>\r\n",										Item[cnt].percentbasemodifier[2]    );
			FilePrintf(hFile,"\t\t\t<FlatAim>%d</FlatAim>\r\n",												Item[cnt].flataimmodifier[2]    );
			FilePrintf(hFile,"\t\t\t<PercentCap>%d</PercentCap>\r\n",										Item[cnt].percentcapmodifier[2]    );
			FilePrintf(hFile,"\t\t\t<PercentHandling>%d</PercentHandling>\r\n",								Item[cnt].percenthandlingmodifier[2]    );
			FilePrintf(hFile,"\t\t\t<PercentTargetTrackingSpeed>%d</PercentTargetTrackingSpeed>\r\n",		Item[cnt].targettrackingmodifier[2]    );
			FilePrintf(hFile,"\t\t\t<PercentDropCompensation>%d</PercentDropCompensation>\r\n",				Item[cnt].percentdropcompensationmodifier[2]    );
			FilePrintf(hFile,"\t\t\t<PercentMaxCounterForce>%d</PercentMaxCounterForce>\r\n",				Item[cnt].maxcounterforcemodifier[2]    );
			FilePrintf(hFile,"\t\t\t<PercentCounterForceAccuracy>%d</PercentCounterForceAccuracy>\r\n",		Item[cnt].counterforceaccuracymodifier[2]    );
			FilePrintf(hFile,"\t\t\t<AimLevels>%d</AimLevels>\r\n",											Item[cnt].aimlevelsmodifier[2]    );
			FilePrintf(hFile,"\t\t</PRONE_MODIFIERS>\r\n");

			// Flugente FTW 1.2
			FilePrintf(hFile,"\t\t<usOverheatingCooldownFactor>%4.2f</usOverheatingCooldownFactor>\r\n",				Item[cnt].usOverheatingCooldownFactor    );
			FilePrintf(hFile,"\t\t<overheatTemperatureModificator>%4.2f</overheatTemperatureModificator>\r\n",		Item[cnt].overheatTemperatureModificator    );
			FilePrintf(hFile,"\t\t<overheatCooldownModificator>%4.2f</overheatCooldownModificator>\r\n",				Item[cnt].overheatCooldownModificator    );
			FilePrintf(hFile,"\t\t<overheatJamThresholdModificator>%4.2f</overheatJamThresholdModificator>\r\n",		Item[cnt].overheatJamThresholdModificator    );
			FilePrintf(hFile,"\t\t<overheatDamageThresholdModificator>%4.2f</overheatDamageThresholdModificator>\r\n",	Item[cnt].overheatDamageThresholdModificator    );

			FilePrintf(hFile,"\t\t<AttachmentClass>%4.2f</AttachmentClass>\r\n",							Item[cnt].attachmentclass );
			FilePrintf(hFile,"\t\t<DrugType>%d</DrugType>\r\n",											Item[cnt].drugtype );
			FilePrintf(hFile,"\t\t<FoodType>%d</FoodType>\r\n",											Item[cnt].foodtype );
			
			//JMich_SkillModifiers: Adding the values here as well
			FilePrintf(hFile, "\t\t<LockPickModifier>%d</LockPickModifier>\r\n",							Item[cnt].LockPickModifier );
			FilePrintf(hFile, "\t\t<CrowbarModifier>%d</CrowbarModifier>\r\n",							Item[cnt].CrowbarModifier );
			FilePrintf(hFile, "\t\t<DisarmModifier>%d</DisarmModifier>\r\n",								Item[cnt].DisarmModifier );
			FilePrintf(hFile, "\t\t<RepairModifier>%d</RepairModifier>\r\n",								Item[cnt].RepairModifier );
			FilePrintf(hFile, "\t\t<usHackingModifier>%d</usHackingModifier>\r\n",						Item[cnt].usHackingModifier );
			FilePrintf(hFile, "\t\t<usBurialModifier>%d</usBurialModifier>\r\n",							Item[cnt].usBurialModifier );
			
			FilePrintf(hFile,"\t\t<DamageChance>%d</DamageChance>\r\n",									Item[cnt].usDamageChance  );
			FilePrintf(hFile,"\t\t<DirtIncreaseFactor>%4.2f</DirtIncreaseFactor>\r\n",					Item[cnt].dirtIncreaseFactor  );

			FilePrintf(hFile,"\t\t<usActionItemFlag>%d</usActionItemFlag>\r\n",								Item[cnt].usActionItemFlag  );
			FilePrintf(hFile,"\t\t<clothestype>%d</clothestype>\r\n",										Item[cnt].clothestype  );
			FilePrintf(hFile,"\t\t<randomitem>%d</randomitem>\r\n",											Item[cnt].randomitem  );
			FilePrintf(hFile,"\t\t<randomitemcoolnessmodificator>%d</randomitemcoolnessmodificator>\r\n",	Item[cnt].randomitemcoolnessmodificator  );
			FilePrintf(hFile,"\t\t<FlashLightRange>%d</FlashLightRange>\r\n",								Item[cnt].usFlashLightRange  );
			FilePrintf(hFile,"\t\t<ItemChoiceTimeSetting>%d</ItemChoiceTimeSetting>\r\n",					Item[cnt].usItemChoiceTimeSetting  );
			FilePrintf(hFile,"\t\t<buddyitem>%d</buddyitem>\r\n",											Item[cnt].usBuddyItem  );
			FilePrintf(hFile,"\t\t<SleepModifier>%d</SleepModifier>\r\n",									Item[cnt].ubSleepModifier  );
			FilePrintf(hFile,"\t\t<usSpotting>%d</usSpotting>\r\n",											Item[cnt].usSpotting  );
			FilePrintf(hFile,"\t\t<sBackpackWeightModifier>%d</sBackpackWeightModifier>\r\n",				Item[cnt].sBackpackWeightModifier);
			FilePrintf(hFile,"\t\t<usPortionSize>%d</usPortionSize>\r\n",									Item[cnt].usPortionSize );
						
			FilePrintf(hFile,"\t\t<usRiotShieldStrength>%d</usRiotShieldStrength>\r\n",					Item[cnt].usRiotShieldStrength );
			FilePrintf(hFile,"\t\t<usRiotShieldGraphic>%d</usRiotShieldGraphic>\r\n",					Item[cnt].usRiotShieldGraphic );
			FilePrintf(hFile,"\t\t<sFireResistance>%d</sFireResistance>\r\n",							Item[cnt].sFireResistance );
			FilePrintf(hFile,"\t\t<usAdministrationModifier>%d</usAdministrationModifier>\r\n",			Item[cnt].usAdministrationModifier );
			FilePrintf(hFile,"\t\t<RobotDamageReductionModifier>%f</RobotDamageReductionModifier>\r\n",	Item[cnt].fRobotDamageReductionModifier );
			FilePrintf(hFile,"\t\t<RobotStrBonus>%d</RobotStrBonus>\r\n",								Item[cnt].bRobotStrBonus );
			FilePrintf(hFile,"\t\t<RobotAgiBonus>%d</RobotAgiBonus>\r\n",								Item[cnt].bRobotAgiBonus );
			FilePrintf(hFile,"\t\t<RobotDexBonus>%d</RobotDexBonus>\r\n",								Item[cnt].bRobotDexBonus );
			FilePrintf(hFile,"\t\t<RobotTargetingSkillGrant>%d</RobotTargetingSkillGrant>\r\n",			Item[cnt].bRobotTargetingSkillGrant );
			FilePrintf(hFile,"\t\t<RobotChassisSkillGrant>%d</RobotChassisSkillGrant>\r\n",				Item[cnt].bRobotChassisSkillGrant );
			FilePrintf(hFile,"\t\t<RobotUtilitySkillGrant>%d</RobotUtilitySkillGrant>\r\n",				Item[cnt].bRobotUtilitySkillGrant );

			// usItemFlag
			if ( HasItemFlag( cnt, BLOOD_BAG ) )				FilePrintf( hFile, "\t\t<Bloodbag>%d</Bloodbag>\r\n", 1 );
			if ( HasItemFlag( cnt, MANPAD ) )				FilePrintf( hFile, "\t\t<Manpad>%d</Manpad>\r\n", 1 );
			if ( HasItemFlag( cnt, BEARTRAP ) )				FilePrintf( hFile, "\t\t<Beartrap>%d</Beartrap>\r\n", 1 );
			if ( HasItemFlag( cnt, CAMERA ) )				FilePrintf( hFile, "\t\t<Camera>%d</Camera>\r\n", 1 );
			if ( HasItemFlag( cnt, WATER_DRUM ) )			FilePrintf( hFile, "\t\t<Waterdrum>%d</Waterdrum>\r\n", 1 );
			if ( HasItemFlag( cnt, MEAT_BLOODCAT ) )			FilePrintf( hFile, "\t\t<BloodcatMeat>%d</BloodcatMeat>\r\n", 1 );
			if ( HasItemFlag( cnt, MEAT_COW ) )				FilePrintf( hFile, "\t\t<CowMeat>%d</CowMeat>\r\n", 1 );
			if ( HasItemFlag( cnt, BELT_FED ) )				FilePrintf( hFile, "\t\t<Beltfed>%d</Beltfed>\r\n", 1 );
			if ( HasItemFlag( cnt, AMMO_BELT ) )				FilePrintf( hFile, "\t\t<Ammobelt>%d</Ammobelt>\r\n", 1 );
			if ( HasItemFlag( cnt, AMMO_BELT_VEST ) )		FilePrintf( hFile, "\t\t<AmmobeltVest>%d</AmmobeltVest>\r\n", 1 );
			if ( HasItemFlag( cnt, CAMO_REMOVAL ) )			FilePrintf( hFile, "\t\t<CamoRemoval>%d</CamoRemoval>\r\n", 1 );
			if ( HasItemFlag( cnt, CLEANING_KIT ) )			FilePrintf( hFile, "\t\t<Cleaningkit>%d</Cleaningkit>\r\n", 1 );
			if ( HasItemFlag( cnt, ATTENTION_ITEM ) )		FilePrintf( hFile, "\t\t<AttentionItem>%d</AttentionItem>\r\n", 1 );
			if ( HasItemFlag( cnt, GAROTTE ) )				FilePrintf( hFile, "\t\t<Garotte>%d</Garotte>\r\n", 1 );
			if ( HasItemFlag( cnt, COVERT ) )				FilePrintf( hFile, "\t\t<Covert>%d</Covert>\r\n", 1 );
			if ( HasItemFlag( cnt, CORPSE ) )				FilePrintf( hFile, "\t\t<Corpse>%d</Corpse>\r\n", 1 );
			if ( HasItemFlag( cnt, SKIN_BLOODCAT ) )			FilePrintf( hFile, "\t\t<BloodcatSkin>%d</BloodcatSkin>\r\n", 1 );
			if ( HasItemFlag( cnt, NO_METAL_DETECTION ) )	FilePrintf( hFile, "\t\t<NoMetalDetection>%d</NoMetalDetection>\r\n", 1 );
			if ( HasItemFlag( cnt, JUMP_GRENADE ) )			FilePrintf( hFile, "\t\t<JumpGrenade>%d</JumpGrenade>\r\n", 1 );
			if ( HasItemFlag( cnt, HANDCUFFS ) )				FilePrintf( hFile, "\t\t<Handcuffs>%d</Handcuffs>\r\n", 1 );
			if ( HasItemFlag( cnt, TASER ) )					FilePrintf( hFile, "\t\t<Taser>%d</Taser>\r\n", 1 );
			if ( HasItemFlag( cnt, SCUBA_BOTTLE ) )			FilePrintf( hFile, "\t\t<ScubaBottle>%d</ScubaBottle>\r\n", 1 );
			if ( HasItemFlag( cnt, SCUBA_MASK ) )			FilePrintf( hFile, "\t\t<ScubaMask>%d</ScubaMask>\r\n", 1 );
			if ( HasItemFlag( cnt, SCUBA_FINS ) )			FilePrintf( hFile, "\t\t<ScubaFins>%d</ScubaFins>\r\n", 1 );
			if ( HasItemFlag( cnt, TRIPWIREROLL ) )			FilePrintf( hFile, "\t\t<TripwireRoll>%d</TripwireRoll>\r\n", 1 );
			if ( HasItemFlag( cnt, RADIO_SET ) )				FilePrintf( hFile, "\t\t<Radioset>%d</Radioset>\r\n", 1 );
			if ( HasItemFlag( cnt, SIGNAL_SHELL ) )			FilePrintf( hFile, "\t\t<SignalShell>%d</SignalShell>\r\n", 1 );
			if ( HasItemFlag( cnt, SODA ) )					FilePrintf( hFile, "\t\t<Soda>%d</Soda>\r\n", 1 );
			if ( HasItemFlag( cnt, ROOF_COLLAPSE_ITEM ) )	FilePrintf( hFile, "\t\t<RoofcollapseItem>%d</RoofcollapseItem>\r\n", 1 );
			if ( HasItemFlag( cnt, DISEASEPROTECTION_1 ) )	FilePrintf( hFile, "\t\t<DiseaseprotectionFace>%d</DiseaseprotectionFace>\r\n", 1 );
			if ( HasItemFlag( cnt, DISEASEPROTECTION_2 ) )	FilePrintf( hFile, "\t\t<DiseaseprotectionHand>%d</DiseaseprotectionHand>\r\n", 1 );
			if ( HasItemFlag( cnt, LBE_EXPLOSIONPROOF ) )	FilePrintf( hFile, "\t\t<LBEexplosionproof>%d</LBEexplosionproof>\r\n", 1 );
			if ( HasItemFlag( cnt, EMPTY_BLOOD_BAG ) )		FilePrintf( hFile, "\t\t<EmptyBloodbag>%d</EmptyBloodbag>\r\n", 1 );
			if ( HasItemFlag( cnt, MEDICAL_SPLINT ) )		FilePrintf( hFile, "\t\t<MedicalSplint>%d</MedicalSplint>\r\n", 1 );
			if ( ItemIsDamageable( cnt ) )					FilePrintf( hFile, "\t\t<Damageable>%d</Damageable>\r\n", 1 );
			if ( ItemIsRepairable( cnt ) )					FilePrintf( hFile, "\t\t<Repairable>%d</Repairable>\r\n", 1 );
			if ( ItemIsDamagedByWater( cnt ) )				FilePrintf( hFile, "\t\t<WaterDamages>%d</WaterDamages>\r\n", 1 );
			if ( ItemIsMetal( cnt ) )						FilePrintf( hFile, "\t\t<Metal>%d</Metal>\r\n", 1 );
			if ( ItemSinks( cnt ) )							FilePrintf( hFile, "\t\t<Sinks>%d</Sinks>\r\n", 1 );
			if ( HasItemFlag( cnt, ITEM_showstatus ) )		FilePrintf( hFile, "\t\t<ShowStatus>%d</ShowStatus>\r\n", 1 );
			if ( ItemIsHiddenAddon( cnt ) )					FilePrintf( hFile, "\t\t<HiddenAddon>%d</HiddenAddon>\r\n", 1 );
			if ( ItemIsTwoHanded( cnt ) )					FilePrintf( hFile, "\t\t<TwoHanded>%d</TwoHanded>\r\n", 1 );
			if ( ItemIsNotBuyable( cnt ) )					FilePrintf( hFile, "\t\t<NotBuyable>%d</NotBuyable>\r\n", 1 );
			if ( ItemIsAttachment( cnt ) )					FilePrintf( hFile, "\t\t<Attachment>%d</Attachment>\r\n", 1 );
			if ( ItemIsHiddenAttachment( cnt ) )				FilePrintf( hFile, "\t\t<HiddenAttachment>%d</HiddenAttachment>\r\n", 1 );
			if ( ItemIsOnlyInTonsOfGuns( cnt ) )				FilePrintf( hFile, "\t\t<BigGunList>%d</BigGunList>\r\n", 1 );
			if ( ItemIsNotInEditor( cnt ) )					FilePrintf( hFile, "\t\t<NotInEditor>%d</NotInEditor>\r\n", 1 );
			if ( ItemIsUndroppableByDefault( cnt ) )			FilePrintf( hFile, "\t\t<DefaultUndroppable>%d</DefaultUndroppable>\r\n", 1 );
			if ( ItemIsUnaerodynamic( cnt ) )				FilePrintf( hFile, "\t\t<Unaerodynamic>%d</Unaerodynamic>\r\n", 1 );
			if ( ItemIsElectronic( cnt ) )					FilePrintf( hFile, "\t\t<Electronic>%d</Electronic>\r\n", 1 );
			if ( ItemIsCannon( cnt ) )						FilePrintf( hFile, "\t\t<Cannon>%d</Cannon>\r\n", 1 );
			if ( ItemIsRocketRifle( cnt ) )					FilePrintf( hFile, "\t\t<RocketRifle>%d</RocketRifle>\r\n", 1 );
			if ( ItemHasFingerPrintID( cnt ) )				FilePrintf( hFile, "\t\t<FingerPrintID>%d</FingerPrintID>\r\n", 1 );
			if ( ItemIsMetalDetector( cnt ) )				FilePrintf( hFile, "\t\t<MetalDetector>%d</MetalDetector>\r\n", 1 );
			if ( ItemIsGasmask( cnt ) )						FilePrintf( hFile, "\t\t<GasMask>%d</GasMask>\r\n", 1 );
			if ( ItemIsLockBomb( cnt ) )						FilePrintf( hFile, "\t\t<LockBomb>%d</LockBomb>\r\n", 1 );
			if ( ItemIsFlare( cnt ) )						FilePrintf( hFile, "\t\t<Flare>%d</Flare>\r\n", 1 );
			if ( ItemIsGrenadeLauncher( cnt ) )				FilePrintf( hFile, "\t\t<GrenadeLauncher>%d</GrenadeLauncher>\r\n", 1 );
			if ( ItemIsMortar( cnt ) )						FilePrintf( hFile, "\t\t<Mortar>%d</Mortar>\r\n", 1 );
			if ( ItemIsDuckbill( cnt ) )						FilePrintf( hFile, "\t\t<Duckbill>%d</Duckbill>\r\n", 1 );
			if ( ItemIsDetonator( cnt ) )					FilePrintf( hFile, "\t\t<Detonator>%d</Detonator>\r\n", 1 );
			if ( ItemIsRemoteDetonator( cnt ) )				FilePrintf( hFile, "\t\t<RemoteDetonator>%d</RemoteDetonator>\r\n", 1 );
			if ( ItemHasHiddenMuzzleFlash( cnt ) )			FilePrintf( hFile, "\t\t<HideMuzzleFlash>%d</HideMuzzleFlash>\r\n", 1 );
			if ( ItemIsRocketLauncher( cnt ) )				FilePrintf( hFile, "\t\t<RocketLauncher>%d</RocketLauncher>\r\n", 1 );

			// usItemFlag2
			if ( ItemIsSingleShotRocketLauncher( cnt ) )		FilePrintf( hFile, "\t\t<SingleShotRocketLauncher>%d</SingleShotRocketLauncher>\r\n", 1 );
			if ( ItemIsBrassKnuckles( cnt ) )				FilePrintf( hFile, "\t\t<BrassKnuckles>%d</BrassKnuckles>\r\n", 1 );
			if ( ItemIsCrowbar( cnt ) )						FilePrintf( hFile, "\t\t<Crowbar>%d</Crowbar>\r\n", 1 );
			if ( ItemIsGLgrenade( cnt ) )					FilePrintf( hFile, "\t\t<GLGrenade>%d</GLGrenade>\r\n", 1 );
			if ( ItemIsFlakJacket( cnt ) )					FilePrintf( hFile, "\t\t<FlakJacket>%d</FlakJacket>\r\n", 1 );
			if ( ItemIsLeatherJacket( cnt ) )				FilePrintf( hFile, "\t\t<LeatherJacket>%d</LeatherJacket>\r\n", 1 );
			if ( ItemIsBatteries( cnt ) )					FilePrintf( hFile, "\t\t<Batteries>%d</Batteries>\r\n", 1 );
			if ( ItemNeedsBatteries( cnt ) )					FilePrintf( hFile, "\t\t<NeedsBatteries>%d</NeedsBatteries>\r\n", 1 );
			if ( ItemHasXRay( cnt ) )						FilePrintf( hFile, "\t\t<XRay>%d</XRay>\r\n", 1 );
			if ( ItemIsWirecutters( cnt ) )					FilePrintf( hFile, "\t\t<WireCutters>%d</WireCutters>\r\n", 1 );
			if ( ItemIsToolkit( cnt ) )						FilePrintf( hFile, "\t\t<Toolkit>%d</Toolkit>\r\n", 1 );
			if ( ItemIsFirstAidKit( cnt ) )					FilePrintf( hFile, "\t\t<FirstAidKit>%d</FirstAidKit>\r\n", 1 );
			if ( ItemIsMedicalKit( cnt ) )					FilePrintf( hFile, "\t\t<MedicalKit>%d</MedicalKit>\r\n", 1 );
			if ( ItemIsCanteen( cnt ) )						FilePrintf( hFile, "\t\t<Canteen>%d</Canteen>\r\n", 1 );
			if ( ItemIsJar( cnt ) )							FilePrintf( hFile, "\t\t<Jar>%d</Jar>\r\n", 1 );
			if ( ItemIsCanAndString( cnt ) )					FilePrintf( hFile, "\t\t<CanAndString>%d</CanAndString>\r\n", 1 );
			if ( ItemIsMarbles( cnt ) )						FilePrintf( hFile, "\t\t<Marbles>%d</Marbles>\r\n", 1 );
			if ( ItemIsWalkman( cnt ) )						FilePrintf( hFile, "\t\t<Walkman>%d</Walkman>\r\n", 1 );
			if ( ItemIsRemoteTrigger( cnt ) )				FilePrintf( hFile, "\t\t<RemoteTrigger>%d</RemoteTrigger>\r\n", 1 );
			if ( ItemIsRobotRemote( cnt ) )					FilePrintf( hFile, "\t\t<RobotRemoteControl>%d</RobotRemoteControl>\r\n", 1 );
			if ( ItemIsCamoKit( cnt ) )						FilePrintf( hFile, "\t\t<CamouflageKit>%d</CamouflageKit>\r\n", 1 );
			if ( ItemIsLocksmithKit( cnt ) )					FilePrintf( hFile, "\t\t<LocksmithKit>%d</LocksmithKit>\r\n", 1 );
			if ( ItemIsMine( cnt ) )							FilePrintf( hFile, "\t\t<Mine>%d</Mine>\r\n", 1 );
			if ( ItemIsATMine( cnt ) )						FilePrintf( hFile, "\t\t<AntitankMine>%d</AntitankMine>\r\n", 1 );
			if ( ItemIsHardware( cnt ) )						FilePrintf( hFile, "\t\t<Hardware>%d</Hardware>\r\n", 1 );
			if ( ItemIsMedical( cnt ) )						FilePrintf( hFile, "\t\t<Medical>%d</Medical>\r\n", 1 );
			if ( ItemIsGascan( cnt ) )						FilePrintf( hFile, "\t\t<GasCan>%d</GasCan>\r\n", 1 );
			if ( ItemContainsLiquid( cnt ) )					FilePrintf( hFile, "\t\t<ContainsLiquid>%d</ContainsLiquid>\r\n", 1 );
			if ( ItemIsRock( cnt ) )							FilePrintf( hFile, "\t\t<Rock>%d</Rock>\r\n", 1 );
			if ( ItemIsThermalOptics( cnt ) )				FilePrintf( hFile, "\t\t<ThermalOptics>%d</ThermalOptics>\r\n", 1 );
			if ( ItemIsOnlyInScifi( cnt ) )					FilePrintf( hFile, "\t\t<SciFi>%d</SciFi>\r\n", 1 );
			if ( ItemIsOnlyInNIV( cnt ) )					FilePrintf( hFile, "\t\t<NewInv>%d</NewInv>\r\n", 1 );
			if ( ItemIsOnlyInDisease( cnt ) )				FilePrintf( hFile, "\t\t<DiseaseSystemExclusive>%d</DiseaseSystemExclusive>\r\n", 1 );
			if ( ItemIsBarrel( cnt ) )						FilePrintf( hFile, "\t\t<Barrel>%d</Barrel>\r\n", 1 );
			if ( ItemHasTripwireActivation( cnt ) )			FilePrintf( hFile, "\t\t<TripwireActivation>%d</TripwireActivation>\r\n", 1 );
			if ( ItemIsTripwire( cnt ) )						FilePrintf( hFile, "\t\t<TripWire>%d</TripWire>\r\n", 1 );
			if ( ItemIsDirectional( cnt ) )					FilePrintf( hFile, "\t\t<Directional>%d</Directional>\r\n", 1 );
			if ( ItemBlocksIronsight( cnt ) )				FilePrintf( hFile, "\t\t<BlockIronSight>%d</BlockIronSight>\r\n", 1 );
			if ( ItemAllowsClimbing( cnt ) )					FilePrintf( hFile, "\t\t<AllowClimbing>%d</AllowClimbing>\r\n", 1 );
			if ( ItemIsCigarette( cnt ) )					FilePrintf( hFile, "\t\t<Cigarette>%d</Cigarette>\r\n", 1 );
			if ( ItemProvidesRobotCamo( cnt ) )				FilePrintf( hFile, "\t\t<ProvidesRobotCamo>%d</ProvidesRobotCamo>\r\n", 1 );
			if ( ItemProvidesRobotNightvision( cnt ) )		FilePrintf( hFile, "\t\t<ProvidesRobotNightVision>%d</ProvidesRobotNightVision>\r\n", 1 );
			if ( ItemProvidesRobotLaserBonus( cnt ) )		FilePrintf( hFile, "\t\t<ProvidesRobotLaserBonus>%d</ProvidesRobotLaserBonus>\r\n", 1 );


			FilePrintf(hFile,"\t</ITEM>\r\n");
		}
		FilePrintf(hFile,"</ITEMLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}

// HEADROCK HAM 4: This function runs just before the items are written into the item array. It causes all stance bonuses
// to inherit their properties from the bonus "above" them, as long as they don't already have their own value defined.
void InheritStanceModifiers( itemParseData *pData )
{

	// Create a two-dimensional temp array to hold all the data about stance modifiers.
	INT16 TempArray[11][3];
	INT8 count = 0;
	INT8 arrcount = 0;

	// Copy stance modifier data into this array
	for (count = 0; count < 3; count++)
	{
		TempArray[0][count] = pData->curItem.flatbasemodifier[count];
		TempArray[1][count] = pData->curItem.percentbasemodifier[count];
		TempArray[2][count] = pData->curItem.flataimmodifier[count];
		TempArray[3][count] = pData->curItem.percentaimmodifier[count];
		TempArray[4][count] = pData->curItem.percentcapmodifier[count];
		TempArray[5][count] = pData->curItem.percenthandlingmodifier[count];
		TempArray[6][count] = pData->curItem.targettrackingmodifier[count];
		TempArray[7][count] = pData->curItem.percentdropcompensationmodifier[count];
		TempArray[8][count] = pData->curItem.maxcounterforcemodifier[count];
		TempArray[9][count] = pData->curItem.counterforceaccuracymodifier[count];
		TempArray[10][count] = pData->curItem.aimlevelsmodifier[count];
	}

	for (arrcount = 0; arrcount < 11; arrcount++)
	{
		if (TempArray[arrcount][2] == -10000)
		{
			if (TempArray[arrcount][1] == -10000)
			{
				TempArray[arrcount][1] = TempArray[arrcount][0];
				TempArray[arrcount][2] = TempArray[arrcount][0];
			}
			else
			{
				TempArray[arrcount][2] = TempArray[arrcount][1];
			}
		}
		else if (TempArray[arrcount][1] == -10000)
		{
			TempArray[arrcount][1] = TempArray[arrcount][0];
		}
		for (INT8 X = 0; X < 3; X++)
		{
			if (TempArray[arrcount][X] == -10000)
			{ 
				TempArray[arrcount][X] = 0; 
			}
		}
	}

	// Copy stance modifier data back out of the temp array
	for (count = 0; count < 3; count++)
	{
		pData->curItem.flatbasemodifier[count] = TempArray[0][count];
		pData->curItem.percentbasemodifier[count] = TempArray[1][count];
		pData->curItem.flataimmodifier[count] = TempArray[2][count];
		pData->curItem.percentaimmodifier[count] = TempArray[3][count];
		pData->curItem.percentcapmodifier[count] = TempArray[4][count];
		pData->curItem.percenthandlingmodifier[count] = TempArray[5][count];
		pData->curItem.targettrackingmodifier[count] = TempArray[6][count];
		pData->curItem.percentdropcompensationmodifier[count] = TempArray[7][count];
		pData->curItem.maxcounterforcemodifier[count] = TempArray[8][count];
		pData->curItem.counterforceaccuracymodifier[count] = TempArray[9][count];
		pData->curItem.aimlevelsmodifier[count] = TempArray[10][count];
	}

}
