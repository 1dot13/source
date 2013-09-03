///////////////////////////////////////////////////////////////////////////////
// HEADROCK PROFEX: PROFile EXternalization
//
// This file handles all reading from Merc Profiles.XML. It offers an external
// alternative to PROEDIT. Values that have no current use in the game were
// EXCLUDED. If you wish to add them simply follow the example set here. You
// can read MERCPROFILESTRUCT in the file "soldier profile type.h" to see
// all profile data. Add the ones you want to externalize to TEMPPROFILESTRUCT 
// in "Soldier Profile.h", and then add the appropriate lines where needed,
// in this file, following my example.
///////////////////////////////////////////////////////////////////////////////

#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "gamesettings.h"
	#include "XML.h"
	#include "Soldier Profile.h"
#endif
		#include "Soldier Profile.h"
//#define MAX_PROFILE_NAME_LENGTH 30

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	TEMPPROFILETYPE curProfile;
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef profileParseData;

TEMPPROFILETYPE tempProfiles[NUM_PROFILES+1];

BOOLEAN MercProfiles_TextOnly;

static void XMLCALL
profileStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	profileParseData * pData = (profileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MERCPROFILES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "PROFILE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"MergeStartElementHandle: setting memory for curMerge");

			memset(&pData->curProfile,0,sizeof(TEMPPROFILETYPE));

			pData->maxReadDepth++; //we are not skipping this element

			//pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "zName") == 0 ||
				strcmp(name, "zNickname") == 0 ||
				strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "ubFaceIndex") == 0 ||
				strcmp(name, "usEyesX") == 0 ||
				strcmp(name, "usEyesY") == 0 ||
				strcmp(name, "usMouthX") == 0 ||
				strcmp(name, "usMouthY") == 0 ||
				strcmp(name, "uiEyeDelay") == 0 ||
				strcmp(name, "uiMouthDelay") == 0 ||
				strcmp(name, "uiBlinkFrequency") == 0 ||
				strcmp(name, "uiExpressionFrequency") == 0 ||
				strcmp(name, "PANTS") == 0 ||
				strcmp(name, "VEST") == 0 ||
				strcmp(name, "SKIN") == 0 ||
				strcmp(name, "HAIR") == 0 ||
				strcmp(name, "bSex") == 0 ||
				strcmp(name, "ubBodyType") == 0 ||
				strcmp(name, "uiBodyTypeSubFlags") == 0 ||
				strcmp(name, "bAttitude") == 0 ||
				strcmp(name, "bCharacterTrait") == 0 || // added by SANDRO
				strcmp(name, "bDisability") == 0 ||
				strcmp(name, "ubNeedForSleep") == 0 ||
				strcmp(name, "bReputationTolerance") == 0 ||
				strcmp(name, "bDeathRate") == 0 ||
				strcmp(name, "bLifeMax") == 0 ||
				strcmp(name, "bLife") == 0 ||
				strcmp(name, "bStrength") == 0 ||
				strcmp(name, "bAgility") == 0 ||
				strcmp(name, "bDexterity") == 0 ||
				strcmp(name, "bWisdom") == 0 ||
				strcmp(name, "bMarksmanship") == 0 ||
				strcmp(name, "bExplosive") == 0 ||
				strcmp(name, "bLeadership") == 0 ||
				strcmp(name, "bMedical") == 0 ||
				strcmp(name, "bMechanical") == 0 ||
				strcmp(name, "bExpLevel") == 0 ||
				strcmp(name, "bEvolution") == 0 ||
				// added by SANDRO
				strcmp(name, "bOldSkillTrait") == 0 ||
				strcmp(name, "bOldSkillTrait2") == 0 ||
				strcmp(name, "bNewSkillTrait1") == 0 ||
				strcmp(name, "bNewSkillTrait2") == 0 ||
				strcmp(name, "bNewSkillTrait3") == 0 ||
				strcmp(name, "bNewSkillTrait4") == 0 ||
				strcmp(name, "bNewSkillTrait5") == 0 ||
				strcmp(name, "bNewSkillTrait6") == 0 ||
				strcmp(name, "bNewSkillTrait7") == 0 ||
				strcmp(name, "bNewSkillTrait8") == 0 ||
				strcmp(name, "bNewSkillTrait9") == 0 ||
				strcmp(name, "bNewSkillTrait10") == 0 ||
				strcmp(name, "bNewSkillTrait11") == 0 ||
				strcmp(name, "bNewSkillTrait12") == 0 ||
				strcmp(name, "bNewSkillTrait13") == 0 ||
				strcmp(name, "bNewSkillTrait14") == 0 ||
				strcmp(name, "bNewSkillTrait15") == 0 ||
				strcmp(name, "bNewSkillTrait16") == 0 ||
				strcmp(name, "bNewSkillTrait17") == 0 ||
				strcmp(name, "bNewSkillTrait18") == 0 ||
				strcmp(name, "bNewSkillTrait19") == 0 ||
				strcmp(name, "bNewSkillTrait20") == 0 ||
				strcmp(name, "bNewSkillTrait21") == 0 ||
				strcmp(name, "bNewSkillTrait22") == 0 ||
				strcmp(name, "bNewSkillTrait23") == 0 ||
				strcmp(name, "bNewSkillTrait24") == 0 ||
				strcmp(name, "bNewSkillTrait25") == 0 ||
				strcmp(name, "bNewSkillTrait26") == 0 ||
				strcmp(name, "bNewSkillTrait27") == 0 ||
				strcmp(name, "bNewSkillTrait28") == 0 ||
				strcmp(name, "bNewSkillTrait29") == 0 ||
				strcmp(name, "bNewSkillTrait30") == 0 ||

				strcmp(name, "bBuddy1") == 0 ||
				strcmp(name, "bBuddy2") == 0 ||
				strcmp(name, "bBuddy3") == 0 ||
				strcmp(name, "bBuddy4") == 0 ||
				strcmp(name, "bBuddy5") == 0 ||
				strcmp(name, "bLearnToLike") == 0 ||
				strcmp(name, "bLearnToLikeTime") == 0 ||
				strcmp(name, "bHated1") == 0 ||
				strcmp(name, "bHatedTime1") == 0 ||
				strcmp(name, "bHated2") == 0 ||
				strcmp(name, "bHatedTime2") == 0 ||
				strcmp(name, "bHated3") == 0 ||
				strcmp(name, "bHatedTime3") == 0 ||
				strcmp(name, "bHated4") == 0 ||
				strcmp(name, "bHatedTime4") == 0 ||
				strcmp(name, "bHated5") == 0 ||
				strcmp(name, "bHatedTime5") == 0 ||
				strcmp(name, "bLearnToHate") == 0 ||
				strcmp(name, "bLearnToHateTime") == 0 ||
				strcmp(name, "sSalary") == 0 ||
				strcmp(name, "uiWeeklySalary") == 0 ||
				strcmp(name, "uiBiWeeklySalary") == 0 ||
				strcmp(name, "bMedicalDeposit") == 0 ||
				strcmp(name, "sMedicalDepositAmount") == 0 ||
				strcmp(name, "usOptionalGearCost") == 0 ||
				strcmp(name, "bArmourAttractiveness") == 0 ||
				strcmp(name, "bMainGunAttractiveness") == 0 ||
				strcmp(name, "fGoodGuy") == 0 ||
				strcmp(name, "usApproachFactorFriendly") == 0 ||
				strcmp(name, "usApproachFactorDirect") == 0 ||
				strcmp(name, "usApproachFactorThreaten") == 0 ||
				strcmp(name, "usApproachFactorRecruit") == 0 ||
				
				//new tag
				strcmp(name, "Type")  == 0 ||				
				strcmp(name, "sSectorX")  == 0 ||
				strcmp(name, "sSectorY")  == 0 ||
				strcmp(name, "sSectorZ")  == 0 ||
				strcmp(name, "ubCivilianGroup")  == 0 ||
				strcmp(name, "bTown")  == 0 ||
				strcmp(name, "bTownAttachment")  == 0 ||
				strcmp(name, "usBackground")  == 0
												
				))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
profileCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	profileParseData * pData = (profileParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
profileEndElementHandle(void *userData, const XML_Char *name)
{
	profileParseData * pData = (profileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "MERCPROFILES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "PROFILE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curIndex < pData->maxArraySize)
			{
				// Write data into a temporary array that holds profiles. We will later copy data from that
				// temp array into the REAL profile array, one item at a time, replacing PROF.DAT data.
		
				if (!MercProfiles_TextOnly)
				{
					wcscpy(tempProfiles[pData->curIndex].zName, pData->curProfile.zName); 
					wcscpy(tempProfiles[pData->curIndex].zNickname, pData->curProfile.zNickname);		
					
					tempProfiles[pData->curIndex].ubFaceIndex = pData->curProfile.ubFaceIndex;
					tempProfiles[pData->curIndex].usEyesX = pData->curProfile.usEyesX;
					tempProfiles[pData->curIndex].usEyesY = pData->curProfile.usEyesY;
					tempProfiles[pData->curIndex].usMouthX = pData->curProfile.usMouthX;
					tempProfiles[pData->curIndex].usMouthY = pData->curProfile.usMouthY;
					tempProfiles[pData->curIndex].uiEyeDelay = pData->curProfile.uiEyeDelay;
					tempProfiles[pData->curIndex].uiMouthDelay = pData->curProfile.uiMouthDelay;
					tempProfiles[pData->curIndex].uiBlinkFrequency = pData->curProfile.uiBlinkFrequency;
					tempProfiles[pData->curIndex].uiExpressionFrequency = pData->curProfile.uiExpressionFrequency;

					memcpy( &(tempProfiles[pData->curIndex].PANTS), &(pData->curProfile.PANTS), sizeof(PaletteRepID) );
					memcpy( &(tempProfiles[pData->curIndex].VEST), &(pData->curProfile.VEST), sizeof(PaletteRepID) );
					memcpy( &(tempProfiles[pData->curIndex].SKIN), &(pData->curProfile.SKIN), sizeof(PaletteRepID) );
					memcpy( &(tempProfiles[pData->curIndex].HAIR), &(pData->curProfile.HAIR), sizeof(PaletteRepID) );

					tempProfiles[pData->curIndex].bSex = pData->curProfile.bSex;
					tempProfiles[pData->curIndex].ubBodyType = pData->curProfile.ubBodyType;
					tempProfiles[pData->curIndex].uiBodyTypeSubFlags = pData->curProfile.uiBodyTypeSubFlags;

					tempProfiles[pData->curIndex].bAttitude = pData->curProfile.bAttitude;
					tempProfiles[pData->curIndex].bCharacterTrait = pData->curProfile.bCharacterTrait; // added by SANDRO
					tempProfiles[pData->curIndex].bDisability = pData->curProfile.bDisability;
					tempProfiles[pData->curIndex].ubNeedForSleep = pData->curProfile.ubNeedForSleep;
					tempProfiles[pData->curIndex].bReputationTolerance = pData->curProfile.bReputationTolerance;
					tempProfiles[pData->curIndex].bDeathRate = pData->curProfile.bDeathRate;

					tempProfiles[pData->curIndex].bLifeMax = pData->curProfile.bLifeMax;
					tempProfiles[pData->curIndex].bLife = pData->curProfile.bLife;
					tempProfiles[pData->curIndex].bStrength = pData->curProfile.bStrength;
					tempProfiles[pData->curIndex].bAgility = pData->curProfile.bAgility;
					tempProfiles[pData->curIndex].bDexterity = pData->curProfile.bDexterity;
					tempProfiles[pData->curIndex].bWisdom = pData->curProfile.bWisdom;
					tempProfiles[pData->curIndex].bMarksmanship = pData->curProfile.bMarksmanship;
					tempProfiles[pData->curIndex].bExplosive = pData->curProfile.bExplosive;
					tempProfiles[pData->curIndex].bLeadership = pData->curProfile.bLeadership;
					tempProfiles[pData->curIndex].bMedical = pData->curProfile.bMedical;
					tempProfiles[pData->curIndex].bMechanical = pData->curProfile.bMechanical;
					tempProfiles[pData->curIndex].bExpLevel = pData->curProfile.bExpLevel;

					tempProfiles[pData->curIndex].bEvolution = pData->curProfile.bEvolution;
					// added by SANDRO
					tempProfiles[pData->curIndex].bOldSkillTrait = pData->curProfile.bOldSkillTrait;
					tempProfiles[pData->curIndex].bOldSkillTrait2 = pData->curProfile.bOldSkillTrait2;
					tempProfiles[pData->curIndex].bNewSkillTrait1 = pData->curProfile.bNewSkillTrait1;
					tempProfiles[pData->curIndex].bNewSkillTrait2 = pData->curProfile.bNewSkillTrait2;
					tempProfiles[pData->curIndex].bNewSkillTrait3 = pData->curProfile.bNewSkillTrait3;
					tempProfiles[pData->curIndex].bNewSkillTrait4 = pData->curProfile.bNewSkillTrait4;
					tempProfiles[pData->curIndex].bNewSkillTrait5 = pData->curProfile.bNewSkillTrait5;
					tempProfiles[pData->curIndex].bNewSkillTrait6 = pData->curProfile.bNewSkillTrait6;
					tempProfiles[pData->curIndex].bNewSkillTrait7 = pData->curProfile.bNewSkillTrait7;
					tempProfiles[pData->curIndex].bNewSkillTrait8 = pData->curProfile.bNewSkillTrait8;
					tempProfiles[pData->curIndex].bNewSkillTrait9 = pData->curProfile.bNewSkillTrait9;
					tempProfiles[pData->curIndex].bNewSkillTrait10 = pData->curProfile.bNewSkillTrait10;
					tempProfiles[pData->curIndex].bNewSkillTrait11 = pData->curProfile.bNewSkillTrait11;
					tempProfiles[pData->curIndex].bNewSkillTrait12 = pData->curProfile.bNewSkillTrait12;
					tempProfiles[pData->curIndex].bNewSkillTrait13 = pData->curProfile.bNewSkillTrait13;
					tempProfiles[pData->curIndex].bNewSkillTrait14 = pData->curProfile.bNewSkillTrait14;
					tempProfiles[pData->curIndex].bNewSkillTrait15 = pData->curProfile.bNewSkillTrait15;
					tempProfiles[pData->curIndex].bNewSkillTrait16 = pData->curProfile.bNewSkillTrait16;
					tempProfiles[pData->curIndex].bNewSkillTrait17 = pData->curProfile.bNewSkillTrait17;
					tempProfiles[pData->curIndex].bNewSkillTrait18 = pData->curProfile.bNewSkillTrait18;
					tempProfiles[pData->curIndex].bNewSkillTrait19 = pData->curProfile.bNewSkillTrait19;
					tempProfiles[pData->curIndex].bNewSkillTrait20 = pData->curProfile.bNewSkillTrait20;
					tempProfiles[pData->curIndex].bNewSkillTrait21 = pData->curProfile.bNewSkillTrait21;
					tempProfiles[pData->curIndex].bNewSkillTrait22 = pData->curProfile.bNewSkillTrait22;
					tempProfiles[pData->curIndex].bNewSkillTrait23 = pData->curProfile.bNewSkillTrait23;
					tempProfiles[pData->curIndex].bNewSkillTrait24 = pData->curProfile.bNewSkillTrait24;
					tempProfiles[pData->curIndex].bNewSkillTrait25 = pData->curProfile.bNewSkillTrait25;
					tempProfiles[pData->curIndex].bNewSkillTrait26 = pData->curProfile.bNewSkillTrait26;
					tempProfiles[pData->curIndex].bNewSkillTrait27 = pData->curProfile.bNewSkillTrait27;
					tempProfiles[pData->curIndex].bNewSkillTrait28 = pData->curProfile.bNewSkillTrait28;
					tempProfiles[pData->curIndex].bNewSkillTrait29 = pData->curProfile.bNewSkillTrait29;
					tempProfiles[pData->curIndex].bNewSkillTrait30 = pData->curProfile.bNewSkillTrait30;


					memcpy( &(tempProfiles[pData->curIndex].bBuddy), &(pData->curProfile.bBuddy), 5 * sizeof (UINT8));
					tempProfiles[pData->curIndex].bLearnToLike = pData->curProfile.bLearnToLike;
					tempProfiles[pData->curIndex].bLearnToLikeTime = pData->curProfile.bLearnToLikeTime;

					memcpy( &(tempProfiles[pData->curIndex].bHated), &(pData->curProfile.bHated), 5 * sizeof (UINT8));
					memcpy( &(tempProfiles[pData->curIndex].bHatedTime), &(pData->curProfile.bHatedTime), 5 * sizeof (INT8));
					tempProfiles[pData->curIndex].bLearnToHate = pData->curProfile.bLearnToHate;
					tempProfiles[pData->curIndex].bLearnToHateTime = pData->curProfile.bLearnToHateTime;

					tempProfiles[pData->curIndex].sSalary = pData->curProfile.sSalary;
					tempProfiles[pData->curIndex].uiWeeklySalary = pData->curProfile.uiWeeklySalary;
					tempProfiles[pData->curIndex].uiBiWeeklySalary = pData->curProfile.uiBiWeeklySalary;
					tempProfiles[pData->curIndex].bMedicalDeposit = pData->curProfile.bMedicalDeposit;
					tempProfiles[pData->curIndex].sMedicalDepositAmount = pData->curProfile.sMedicalDepositAmount;
					tempProfiles[pData->curIndex].usOptionalGearCost = pData->curProfile.usOptionalGearCost;

					tempProfiles[pData->curIndex].bArmourAttractiveness = pData->curProfile.bArmourAttractiveness;
					tempProfiles[pData->curIndex].bMainGunAttractiveness = pData->curProfile.bMainGunAttractiveness;
					
					tempProfiles[pData->curIndex].Type = pData->curProfile.Type;
					
					tempProfiles[pData->curIndex].sSectorX = pData->curProfile.sSectorX;
					tempProfiles[pData->curIndex].sSectorY = pData->curProfile.sSectorY;
					tempProfiles[pData->curIndex].bSectorZ = pData->curProfile.bSectorZ;
					tempProfiles[pData->curIndex].ubCivilianGroup = pData->curProfile.ubCivilianGroup;
					tempProfiles[pData->curIndex].bTown = pData->curProfile.bTown;
					tempProfiles[pData->curIndex].bTownAttachment = pData->curProfile.bTownAttachment;
					tempProfiles[pData->curIndex].usBackground = pData->curProfile.usBackground;

					tempProfiles[pData->curIndex].fGoodGuy = pData->curProfile.fGoodGuy;
					memcpy( &(tempProfiles[pData->curIndex].usApproachFactor), &(pData->curProfile.usApproachFactor), 4 * sizeof (UINT16));
					
				}
				else
				{
					wcscpy(tempProfiles[pData->curIndex].zName, pData->curProfile.zName); 
					wcscpy(tempProfiles[pData->curIndex].zNickname, pData->curProfile.zNickname);		
				}	
			}
		}

		else if(strcmp(name, "zName") == 0)
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curProfile.zName, sizeof(pData->curProfile.zName)/sizeof(pData->curProfile.zName[0]) );
			pData->curProfile.zName[sizeof(pData->curProfile.zName)/sizeof(pData->curProfile.zName[0]) - 1] = '\0';
		}

		else if(strcmp(name, "zNickname") == 0)
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curProfile.zNickname, sizeof(pData->curProfile.zNickname)/sizeof(pData->curProfile.zNickname[0]) );
			pData->curProfile.zNickname[sizeof(pData->curProfile.zNickname)/sizeof(pData->curProfile.zNickname[0]) - 1] = '\0';
		}

		else if(strcmp(name, "ubFaceIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.ubFaceIndex = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usEyesX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usEyesX = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usEyesY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usEyesY = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usMouthX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usMouthX = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usMouthY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usMouthY = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "uiEyeDelay") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.uiEyeDelay = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "uiMouthDelay") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.uiMouthDelay = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "uiBlinkFrequency") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.uiBlinkFrequency = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "uiExpressionFrequency") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.uiExpressionFrequency = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "PANTS") == 0)
		{
			pData->curElement = ELEMENT;
			strcpy(pData->curProfile.PANTS, pData->szCharData);
		}

		else if(strcmp(name, "VEST") == 0)
		{
			pData->curElement = ELEMENT;
			strcpy(pData->curProfile.VEST, pData->szCharData);
		}

		else if(strcmp(name, "SKIN") == 0)
		{
			pData->curElement = ELEMENT;
			strcpy(pData->curProfile.SKIN, pData->szCharData);
		}

		else if(strcmp(name, "HAIR") == 0)
		{
			pData->curElement = ELEMENT;
			strcpy(pData->curProfile.HAIR, pData->szCharData);
		}

		else if(strcmp(name, "bSex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bSex = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "ubBodyType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.ubBodyType = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "uiBodyTypeSubFlags") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.uiBodyTypeSubFlags = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bAttitude") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bAttitude = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		// SANDRO - added Character Trait
		else if(strcmp(name, "bCharacterTrait") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bCharacterTrait = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bDisability") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bDisability = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "ubNeedForSleep") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.ubNeedForSleep = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bReputationTolerance") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bReputationTolerance = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bDeathRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bDeathRate = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bLifeMax") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bLifeMax = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bLife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bLife = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bStrength") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bStrength = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bAgility") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bAgility = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bDexterity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bDexterity = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bWisdom") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bWisdom = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bMarksmanship") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bMarksmanship = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bExplosive") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bExplosive = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bLeadership") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bLeadership = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bMedical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bMedical = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bMechanical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bMechanical = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bExpLevel") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bExpLevel = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bEvolution") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bEvolution = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		////////////////////////////////////////////////////////////////////////////
		// SANDRO was here - messed this a bit
		else if(strcmp(name, "bOldSkillTrait") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bOldSkillTrait = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bOldSkillTrait2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bOldSkillTrait2 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bNewSkillTrait1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait1 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}		
		else if(strcmp(name, "bNewSkillTrait2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait2 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}		
		else if(strcmp(name, "bNewSkillTrait3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait3 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait4 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait5 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait6 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait7 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait8") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait8 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait9") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait9 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait10") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait10 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait11") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait11 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait12") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait12 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait13") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait13 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait14") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait14 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait15") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait15 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait16") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait16 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait17") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait17 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait18") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait18 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait19") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait19 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait20") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait20 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait21") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait21 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait22") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait22 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait23") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait23 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait24") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait24 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait25") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait25 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait26") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait26 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait27") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait27 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait28") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait28 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}									 
		else if(strcmp(name, "bNewSkillTrait29") == 0)
		{									 
			pData->curElement = ELEMENT;	 
			pData->curProfile.bNewSkillTrait29 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "bNewSkillTrait30") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bNewSkillTrait30 = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		////////////////////////////////////////////////////////////////////////////

		else if(strcmp(name, "bBuddy1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bBuddy[0] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bBuddy2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bBuddy[1] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bBuddy3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bBuddy[2] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bBuddy4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bBuddy[3] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bBuddy5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bBuddy[4] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bLearnToLike") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bLearnToLike = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bLearnToLikeTime") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bLearnToLikeTime = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bHated1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHated[0] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bHatedTime1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHatedTime[0] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bHated2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHated[1] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bHatedTime2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHatedTime[1] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bHated3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHated[2] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bHatedTime3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHatedTime[2] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bHated4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHated[3] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bHatedTime4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHatedTime[3] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "bHated5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHated[4] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bHatedTime5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bHatedTime[4] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bLearnToHate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bLearnToHate = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bLearnToHateTime") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bLearnToHateTime = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "sSalary") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.sSalary = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "uiWeeklySalary") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.uiWeeklySalary = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "uiBiWeeklySalary") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.uiBiWeeklySalary = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bMedicalDeposit") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bMedicalDeposit = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "sMedicalDepositAmount") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.sMedicalDepositAmount = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usOptionalGearCost") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usOptionalGearCost = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bArmourAttractiveness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bArmourAttractiveness = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "bMainGunAttractiveness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bMainGunAttractiveness = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "fGoodGuy") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.fGoodGuy = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usApproachFactorFriendly") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usApproachFactor[0] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usApproachFactorDirect") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usApproachFactor[1] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usApproachFactorThreaten") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usApproachFactor[2] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else if(strcmp(name, "usApproachFactorRecruit") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usApproachFactor[3] = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "uiIndex") == 0)
		{

			pData->curElement = ELEMENT;

			// Sets new index for writing.
			pData->curIndex = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "Type") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.Type = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		
		else if(strcmp(name, "sSectorX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.sSectorX = (UINT16) atol(pData->szCharData);
		}	
		else if(strcmp(name, "sSectorY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.sSectorY = (UINT16) atol(pData->szCharData);
		}			
		else if(strcmp(name, "sSectorZ") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bSectorZ = (INT8) atol(pData->szCharData);
		}			
		else if(strcmp(name, "ubCivilianGroup") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.ubCivilianGroup = (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "bTown") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bTown = (INT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "bTownAttachment") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.bTownAttachment = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usBackground") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curProfile.usBackground = (UINT8) atol(pData->szCharData);
		}	
		

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInMercProfiles(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	profileParseData pData;

	MercProfiles_TextOnly = localizedVersion;
	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading MercProfiles.xml" );

	// Open merges file
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


	XML_SetElementHandler(parser, profileStartElementHandle, profileEndElementHandle);
	XML_SetCharacterDataHandler(parser, profileCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS;
	pData.curIndex = -1;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in MercProfiles.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteMercProfiles()
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\MercProfiles out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<MERCPROFILES>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES ;cnt++)
		{

			FilePrintf(hFile,"\t<PROFILE>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",						cnt);
			
			if (cnt >= 0 && cnt < 40 )
			{	
				FilePrintf(hFile,"\t\t<Type>1</Type>\r\n");
			}
			else if (cnt >= 40 && cnt < 51 )
			{	
				FilePrintf(hFile,"\t\t<Type>2</Type>\r\n");
			}
			else if ( cnt >= FIRST_RPC && cnt < FIRST_NPC )
			{	
				FilePrintf(hFile,"\t\t<Type>3</Type>\r\n");
			}
			else if ( cnt >= FIRST_NPC && cnt < 160 )
			{	
				FilePrintf(hFile,"\t\t<Type>4</Type>\r\n");
			}
			else if ( cnt == 169 )
			{	
				FilePrintf(hFile,"\t\t<Type>4</Type>\r\n");
			}	
			else if ( cnt == 165 || cnt == 166 || cnt == 167 || cnt == 168 )
			{	
				FilePrintf(hFile,"\t\t<Type>2</Type>\r\n");
			}
			else if ( cnt == 51 || cnt == 52 || cnt == 53 || cnt == 54 || cnt == 55 || cnt == 56 )
			{	
				FilePrintf(hFile,"\t\t<Type>6</Type>\r\n");
			}
			else if ( cnt == 160 || cnt == 161 || cnt == 162 || cnt == 163 || cnt == 164 )
			{	
				FilePrintf(hFile,"\t\t<Type>5</Type>\r\n");
			}
			else 
			{
				FilePrintf(hFile,"\t\t<Type>0</Type>\r\n");			
			}
			

			//////////////////////////////
			// Write Character Name

			FilePrintf(hFile,"\t\t<zName>");

			STR16 szRemainder = gMercProfiles[cnt].zName; //the remaining string to be output (for making valid XML)

			while(szRemainder[0] != '\0')
			{
				//UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\0");
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

					//case '\'':
					//	FilePrintf(hFile,"&apos;");
					//	szRemainder++;
					//break;

					//case '\"':
					//	FilePrintf(hFile,"&quot;");
					//	szRemainder++;
					//break;
				}
			}

			FilePrintf(hFile,"</zName>\r\n");

			//////////////////////////////
			// Write Character Nickname

			FilePrintf(hFile,"\t\t<zNickname>");

			szRemainder = gMercProfiles[cnt].zNickname; //the remaining string to be output (for making valid XML)

			while(szRemainder[0] != '\0')
			{
				//UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\0");
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

					//case '\'':
					//	FilePrintf(hFile,"&apos;");
					//	szRemainder++;
					//break;

					//case '\"':
					//	FilePrintf(hFile,"&quot;");
					//	szRemainder++;
					//break;
				}
			}

			FilePrintf(hFile,"</zNickname>\r\n");
			//

			FilePrintf(hFile,"\t\t<ubFaceIndex>%d</ubFaceIndex>\r\n", gMercProfiles[ cnt ].ubFaceIndex);
			FilePrintf(hFile,"\t\t<usEyesX>%d</usEyesX>\r\n", gMercProfiles[ cnt ].usEyesX);
			FilePrintf(hFile,"\t\t<usEyesY>%d</usEyesY>\r\n", gMercProfiles[ cnt ].usEyesY);
			FilePrintf(hFile,"\t\t<usMouthX>%d</usMouthX>\r\n", gMercProfiles[ cnt ].usMouthX);
			FilePrintf(hFile,"\t\t<usMouthY>%d</usMouthY>\r\n", gMercProfiles[ cnt ].usMouthY);
			FilePrintf(hFile,"\t\t<uiEyeDelay>%d</uiEyeDelay>\r\n", gMercProfiles[ cnt ].uiEyeDelay);
			FilePrintf(hFile,"\t\t<uiMouthDelay>%d</uiMouthDelay>\r\n", gMercProfiles[ cnt ].uiMouthDelay);
			FilePrintf(hFile,"\t\t<uiBlinkFrequency>%d</uiBlinkFrequency>\r\n", gMercProfiles[ cnt ].uiBlinkFrequency);
			FilePrintf(hFile,"\t\t<uiExpressionFrequency>%d</uiExpressionFrequency>\r\n", gMercProfiles[ cnt ].uiExpressionFrequency);

			//////////////////////////////
			// Pant Color

			FilePrintf(hFile,"\t\t<PANTS>");

			STR8 zRemainder = gMercProfiles[cnt].PANTS;
			//strcpy(zRemainder, gMercProfiles[cnt].PANTS); //the remaining string to be output (for making valid XML)

			while(zRemainder[0] != '\0')
			{
				//UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				UINT32 uiCharLoc = strcspn(zRemainder,"&<>\0");
				CHAR8 invChar = zRemainder[uiCharLoc];

				if(uiCharLoc)
				{
					zRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",zRemainder);
					zRemainder[uiCharLoc] = invChar;
				}

				zRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						zRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						zRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						zRemainder++;
					break;

					//case '\'':
					//	FilePrintf(hFile,"&apos;");
					//	szRemainder++;
					//break;

					//case '\"':
					//	FilePrintf(hFile,"&quot;");
					//	szRemainder++;
					//break;
				}
			}

			FilePrintf(hFile,"</PANTS>\r\n");

			//////////////////////////////
			// Vest Color

			FilePrintf(hFile,"\t\t<VEST>");

			zRemainder = gMercProfiles[cnt].VEST;
			//strcpy(zRemainder, gMercProfiles[cnt].VEST); //the remaining string to be output (for making valid XML)

			while(zRemainder[0] != '\0')
			{
				//UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				UINT32 uiCharLoc = strcspn(zRemainder,"&<>\0");
				CHAR8 invChar = zRemainder[uiCharLoc];

				if(uiCharLoc)
				{
					zRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",zRemainder);
					zRemainder[uiCharLoc] = invChar;
				}

				zRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						zRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						zRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						zRemainder++;
					break;

					//case '\'':
					//	FilePrintf(hFile,"&apos;");
					//	szRemainder++;
					//break;

					//case '\"':
					//	FilePrintf(hFile,"&quot;");
					//	szRemainder++;
					//break;
				}
			}

			FilePrintf(hFile,"</VEST>\r\n");

			//////////////////////////////
			// Pant Color

			FilePrintf(hFile,"\t\t<SKIN>");

			zRemainder = gMercProfiles[cnt].SKIN;
			//strcpy(zRemainder,gMercProfiles[cnt].SKIN); //the remaining string to be output (for making valid XML)

			while(zRemainder[0] != '\0')
			{
				//UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				UINT32 uiCharLoc = strcspn(zRemainder,"&<>\0");
				CHAR8 invChar = zRemainder[uiCharLoc];

				if(uiCharLoc)
				{
					zRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",zRemainder);
					zRemainder[uiCharLoc] = invChar;
				}

				zRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						zRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						zRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						zRemainder++;
					break;

					//case '\'':
					//	FilePrintf(hFile,"&apos;");
					//	szRemainder++;
					//break;

					//case '\"':
					//	FilePrintf(hFile,"&quot;");
					//	szRemainder++;
					//break;
				}
			}

			FilePrintf(hFile,"</SKIN>\r\n");

			//////////////////////////////
			// Pant Color

			FilePrintf(hFile,"\t\t<HAIR>");

			zRemainder = gMercProfiles[cnt].HAIR;
			//strcpy(zRemainder,gMercProfiles[cnt].HAIR); //the remaining string to be output (for making valid XML)

			while(zRemainder[0] != '\0')
			{
				//UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				UINT32 uiCharLoc = strcspn(zRemainder,"&<>\0");
				CHAR8 invChar = zRemainder[uiCharLoc];

				if(uiCharLoc)
				{
					zRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",zRemainder);
					zRemainder[uiCharLoc] = invChar;
				}

				zRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						zRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						zRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						zRemainder++;
					break;

					//case '\'':
					//	FilePrintf(hFile,"&apos;");
					//	szRemainder++;
					//break;

					//case '\"':
					//	FilePrintf(hFile,"&quot;");
					//	szRemainder++;
					//break;
				}
			}

			FilePrintf(hFile,"</HAIR>\r\n");

			FilePrintf(hFile,"\t\t<bSex>%d</bSex>\r\n", gMercProfiles[ cnt ].bSex);
			FilePrintf(hFile,"\t\t<ubBodyType>%d</ubBodyType>\r\n", gMercProfiles[ cnt ].ubBodyType);
			FilePrintf(hFile,"\t\t<uiBodyTypeSubFlags>%d</uiBodyTypeSubFlags>\r\n", gMercProfiles[ cnt ].uiBodyTypeSubFlags);

			FilePrintf(hFile,"\t\t<bAttitude>%d</bAttitude>\r\n", gMercProfiles[ cnt ].bAttitude);
			// Added by SANDRO
			FilePrintf(hFile,"\t\t<bCharacterTrait>%d</bCharacterTrait>\r\n", gMercProfiles[ cnt ].bCharacterTrait);
			FilePrintf(hFile,"\t\t<bDisability>%d</bDisability>\r\n", gMercProfiles[ cnt ].bDisability);
			FilePrintf(hFile,"\t\t<ubNeedForSleep>%d</ubNeedForSleep>\r\n", gMercProfiles[ cnt ].ubNeedForSleep);

			FilePrintf(hFile,"\t\t<bReputationTolerance>%d</bReputationTolerance>\r\n", gMercProfiles[ cnt ].bReputationTolerance);
			FilePrintf(hFile,"\t\t<bDeathRate>%d</bDeathRate>\r\n", gMercProfiles[ cnt ].bDeathRate);

			FilePrintf(hFile,"\t\t<bLifeMax>%d</bLifeMax>\r\n", gMercProfiles[ cnt ].bLifeMax);
			FilePrintf(hFile,"\t\t<bLife>%d</bLife>\r\n", gMercProfiles[ cnt ].bLife);
			FilePrintf(hFile,"\t\t<bStrength>%d</bStrength>\r\n", gMercProfiles[ cnt ].bStrength);
			FilePrintf(hFile,"\t\t<bAgility>%d</bAgility>\r\n", gMercProfiles[ cnt ].bAgility);
			FilePrintf(hFile,"\t\t<bDexterity>%d</bDexterity>\r\n", gMercProfiles[ cnt ].bDexterity);
			FilePrintf(hFile,"\t\t<bWisdom>%d</bWisdom>\r\n", gMercProfiles[ cnt ].bWisdom);
			FilePrintf(hFile,"\t\t<bMarksmanship>%d</bMarksmanship>\r\n", gMercProfiles[ cnt ].bMarksmanship);
			FilePrintf(hFile,"\t\t<bExplosive>%d</bExplosive>\r\n", gMercProfiles[ cnt ].bExplosive);
			FilePrintf(hFile,"\t\t<bLeadership>%d</bLeadership>\r\n", gMercProfiles[ cnt ].bLeadership);
			FilePrintf(hFile,"\t\t<bMedical>%d</bMedical>\r\n", gMercProfiles[ cnt ].bMedical);
			FilePrintf(hFile,"\t\t<bMechanical>%d</bMechanical>\r\n", gMercProfiles[ cnt ].bMechanical);
			FilePrintf(hFile,"\t\t<bExpLevel>%d</bExpLevel>\r\n", gMercProfiles[ cnt ].bExpLevel);
			FilePrintf(hFile,"\t\t<bEvolution>%d</bEvolution>\r\n", gMercProfiles[ cnt ].bEvolution);
			////////////////////////////////////////////////////////////////////////////////////////////
			// SANDRO - old/new traits
			if (gGameOptions.fNewTraitSystem)
			{
				FilePrintf(hFile,"\t\t<bOldSkillTrait>%d</bOldSkillTrait>\r\n", 0);
				FilePrintf(hFile,"\t\t<bOldSkillTrait2>%d</bOldSkillTrait2>\r\n", 0);
				FilePrintf(hFile,"\t\t<bNewSkillTrait1>%d</bNewSkillTrait1>\r\n", gMercProfiles[ cnt ].bSkillTraits[0]);
				FilePrintf(hFile,"\t\t<bNewSkillTrait2>%d</bNewSkillTrait2>\r\n", gMercProfiles[ cnt ].bSkillTraits[1]);
				FilePrintf(hFile,"\t\t<bNewSkillTrait3>%d</bNewSkillTrait3>\r\n", gMercProfiles[ cnt ].bSkillTraits[2]);
				for ( INT8 bCnt2 = 3; bCnt2 < 30; bCnt2++ )
				{
					if ( gMercProfiles[ cnt ].bSkillTraits[ bCnt2 ] > 0 )
						FilePrintf(hFile,"\t\t<bNewSkillTrait%d>%d</bNewSkillTrait%d>\r\n", (bCnt2+1), gMercProfiles[ cnt ].bSkillTraits[bCnt2], (bCnt2+1));
				}					
			}
			else
			{
				FilePrintf(hFile,"\t\t<bOldSkillTrait>%d</bOldSkillTrait>\r\n", gMercProfiles[ cnt ].bSkillTraits[0]);
				FilePrintf(hFile,"\t\t<bOldSkillTrait2>%d</bOldSkillTrait2>\r\n", gMercProfiles[ cnt ].bSkillTraits[1]);
				FilePrintf(hFile,"\t\t<bNewSkillTrait1>%d</bNewSkillTrait1>\r\n", 0);
				FilePrintf(hFile,"\t\t<bNewSkillTrait2>%d</bNewSkillTrait2>\r\n", 0);
				FilePrintf(hFile,"\t\t<bNewSkillTrait3>%d</bNewSkillTrait3>\r\n", 0);
			}
			////////////////////////////////////////////////////////////////////////////////////////////

			FilePrintf(hFile,"\t\t<bBuddy1>%d</bBuddy1>\r\n", gMercProfiles[ cnt ].bBuddy[0]);
			FilePrintf(hFile,"\t\t<bBuddy2>%d</bBuddy2>\r\n", gMercProfiles[ cnt ].bBuddy[1]);
			FilePrintf(hFile,"\t\t<bBuddy3>%d</bBuddy3>\r\n", gMercProfiles[ cnt ].bBuddy[2]);
			FilePrintf(hFile,"\t\t<bBuddy4>%d</bBuddy4>\r\n", gMercProfiles[ cnt ].bBuddy[3]);
			FilePrintf(hFile,"\t\t<bBuddy5>%d</bBuddy5>\r\n", gMercProfiles[ cnt ].bBuddy[4]);

			FilePrintf(hFile,"\t\t<bLearnToLike>%d</bLearnToLike>\r\n", gMercProfiles[ cnt ].bLearnToLike);
			FilePrintf(hFile,"\t\t<bLearnToLikeTime>%d</bLearnToLikeTime>\r\n", gMercProfiles[ cnt ].bLearnToLikeTime);

			FilePrintf(hFile,"\t\t<bHated1>%d</bHated1>\r\n", gMercProfiles[ cnt ].bHated[0]);
			FilePrintf(hFile,"\t\t<bHatedTime1>%d</bHatedTime1>\r\n", gMercProfiles[ cnt ].bHatedTime[0]);
			FilePrintf(hFile,"\t\t<bHated2>%d</bHated2>\r\n", gMercProfiles[ cnt ].bHated[1]);
			FilePrintf(hFile,"\t\t<bHatedTime2>%d</bHatedTime2>\r\n", gMercProfiles[ cnt ].bHatedTime[1]);
			FilePrintf(hFile,"\t\t<bHated3>%d</bHated3>\r\n", gMercProfiles[ cnt ].bHated[2]);
			FilePrintf(hFile,"\t\t<bHatedTime3>%d</bHatedTime3>\r\n", gMercProfiles[ cnt ].bHatedTime[2]);
			FilePrintf(hFile,"\t\t<bHated4>%d</bHated4>\r\n", gMercProfiles[ cnt ].bHated[3]);
			FilePrintf(hFile,"\t\t<bHatedTime4>%d</bHatedTime4>\r\n", gMercProfiles[ cnt ].bHatedTime[3]);
			FilePrintf(hFile,"\t\t<bHated5>%d</bHated5>\r\n", gMercProfiles[ cnt ].bHated[4]);
			FilePrintf(hFile,"\t\t<bHatedTime5>%d</bHatedTime5>\r\n", gMercProfiles[ cnt ].bHatedTime[4]);

			FilePrintf(hFile,"\t\t<bLearnToHate>%d</bLearnToHate>\r\n", gMercProfiles[ cnt ].bLearnToHate);
			FilePrintf(hFile,"\t\t<bLearnToHateTime>%d</bLearnToHateTime>\r\n", gMercProfiles[ cnt ].bLearnToHateTime);

			FilePrintf(hFile,"\t\t<sSalary>%d</sSalary>\r\n", gMercProfiles[ cnt ].sSalary);
			FilePrintf(hFile,"\t\t<uiWeeklySalary>%d</uiWeeklySalary>\r\n", gMercProfiles[ cnt ].uiWeeklySalary);
			FilePrintf(hFile,"\t\t<uiBiWeeklySalary>%d</uiBiWeeklySalary>\r\n", gMercProfiles[ cnt ].uiBiWeeklySalary);
			FilePrintf(hFile,"\t\t<bMedicalDeposit>%d</bMedicalDeposit>\r\n", gMercProfiles[ cnt ].bMedicalDeposit);
			FilePrintf(hFile,"\t\t<sMedicalDepositAmount>%d</sMedicalDepositAmount>\r\n", gMercProfiles[ cnt ].sMedicalDepositAmount);
			FilePrintf(hFile,"\t\t<usOptionalGearCost>%d</usOptionalGearCost>\r\n", gMercProfiles[ cnt ].usOptionalGearCost);

			FilePrintf(hFile,"\t\t<bArmourAttractiveness>%d</bArmourAttractiveness>\r\n", gMercProfiles[ cnt ].bArmourAttractiveness);
			FilePrintf(hFile,"\t\t<bMainGunAttractiveness>%d</bMainGunAttractiveness>\r\n", gMercProfiles[ cnt ].bMainGunAttractiveness);

			FilePrintf(hFile,"\t\t<fGoodGuy>%d</fGoodGuy>\r\n", ((gMercProfiles[ cnt ].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) != 0));

			FilePrintf(hFile,"\t\t<usApproachFactorFriendly>%d</usApproachFactorFriendly>\r\n", gMercProfiles[ cnt ].usApproachFactor[0]);
			FilePrintf(hFile,"\t\t<usApproachFactorDirect>%d</usApproachFactorDirect>\r\n", gMercProfiles[ cnt ].usApproachFactor[1]);
			FilePrintf(hFile,"\t\t<usApproachFactorThreaten>%d</usApproachFactorThreaten>\r\n", gMercProfiles[ cnt ].usApproachFactor[2]);
			FilePrintf(hFile,"\t\t<usApproachFactorRecruit>%d</usApproachFactorRecruit>\r\n", gMercProfiles[ cnt ].usApproachFactor[3]);
			
			FilePrintf(hFile,"\t\t<sSectorX>%d</sSectorX>\r\n", gMercProfiles[ cnt ].sSectorX);
			FilePrintf(hFile,"\t\t<sSectorY>%d</sSectorY>\r\n", gMercProfiles[ cnt ].sSectorY);
			FilePrintf(hFile,"\t\t<sSectorZ>%d</sSectorZ>\r\n", gMercProfiles[ cnt ].bSectorZ);
			
			FilePrintf(hFile,"\t\t<ubCivilianGroup>%d</ubCivilianGroup>\r\n", gMercProfiles[ cnt ].ubCivilianGroup);
			FilePrintf(hFile,"\t\t<bTown>%d</bTown>\r\n", gMercProfiles[ cnt ].bTown);
			FilePrintf(hFile,"\t\t<bTownAttachment>%d</bTownAttachment>\r\n", gMercProfiles[ cnt ].bTownAttachment);
			FilePrintf(hFile,"\t\t<usBackground>%d</usBackground>\r\n", gMercProfiles[ cnt ].usBackground);
			

			FilePrintf(hFile,"\t</PROFILE>\r\n");
		}
		FilePrintf(hFile,"</MERCPROFILES>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
