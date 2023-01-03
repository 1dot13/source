#ifndef _LUAINITNPCS_H
#define _LUAINITNPCS_H

#include "Types.h"

typedef struct
{
	BOOLEAN	fGlobalLuaBool;
	UINT32	iGlobalLuaVal; 
} LUA_GLOBAL;

extern LUA_GLOBAL	gLuaGlobal[1000]; 


#define MAX_ACTION_NAMES_CHARS		30	

typedef struct
{
	UINT16 uiIndex;
	UINT8 ActionID;
	UINT32 BombItem;
	CHAR16	szName[MAX_ACTION_NAMES_CHARS];
	UINT8 BlowUp;

} ACTION_ITEM_VALUES;

extern ACTION_ITEM_VALUES ActionItemsValues[500];

extern UINT32 uiHourLua;
extern UINT32 uiDayLua;
extern UINT32 uiMinLua;
extern UINT16 PROFILLUA_sSectorX;
extern UINT16 PROFILLUA_sSectorY;
extern UINT8 PROFILLUA_bSectorZ;
extern UINT8 PROFILLUA_Level;
extern UINT8 PROFILLUA_ubID;
extern UINT32 PROFILLUA_sGridNo;
extern UINT8 PROFILLUA_ubDirectiono;
extern UINT8 PROFILLUA_bTeam;

extern UINT8  PROFILLUA2_ubProfile;
extern UINT16 PROFILLUA2_sSectorX;
extern UINT16 PROFILLUA2_sSectorY;
extern UINT8  PROFILLUA2_bSectorZ;
extern UINT8  PROFILLUA2_ubID;
extern UINT32 PROFILLUA2_sGridNo;

extern BOOLEAN LetHandleLoyaltyChangeForNPCAction(UINT8 ubNPCProfileId , UINT8 Init);
extern BOOLEAN LuaHandleGlobalLoyaltyEvent( UINT8 ubEventType, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ , UINT8 Init);
extern BOOLEAN LetLuaHandleEarlyMorningEvents(UINT8 Init);
extern BOOLEAN LetLuaHourlyQuestUpdate(UINT8 Init);
extern BOOLEAN LetLuaMyCustomHandleAtNewGridNo(UINT8 bNewSide, UINT8 ProfileId,  UINT8 Init);
extern BOOLEAN LetLuaPerformItemAction(UINT32 Action, INT32 sGridNo, UINT8 InitFunction);
extern BOOLEAN LetLuaGameInit(UINT8 Init);
extern BOOLEAN LetLuaInterfaceDialogue( UINT8 ubNPC, UINT8 InitFunction);
extern BOOLEAN LuaHandlePlayerTeamMemberDeath(UINT8 ProfileId, UINT8 Init);
extern BOOLEAN LuaHandleNPCTeamMemberDeath(UINT8 ProfileId, UINT8 Init);
extern BOOLEAN LuaCheckForKingpinsMoneyMissing( BOOLEAN fFirstCheck, UINT8 Init);
extern BOOLEAN LuaHandleQuestCodeOnSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init);
//extern BOOLEAN LuaHandleQuestCodeOnSectorEntry( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init);

void LuaHandleSectorLiberation( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, BOOLEAN fFirstTime );
void LuaHandleInteractiveActionResult( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ,
									   INT32 sGridNo, UINT8 bLevel, UINT8 ubId,
									   UINT16 usActionType, INT32 sLuaactionid, INT32 difficulty, UINT16 skill );
void LuaRecruitRPCAdditionalHandling( UINT8 usProfile );
void LuaHandleSectorTacticalEntry( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, BOOLEAN fHasEverBeenPlayerControlled );

extern BOOLEAN LuaHandleDelayedItemsArrival( UINT32 uiReason, UINT8 Init);
extern BOOLEAN LetLuaHandleNPCSystemEvent( UINT32 uiEvent, UINT8 Init);

extern BOOLEAN LuaCheckFact ( UINT16 usFact, UINT8 ubProfileID , UINT32 Init);
extern BOOLEAN LuaHandleNPCDoAction( UINT8 ubTargetNPC, UINT16 usActionCode, UINT8 ubQuoteNum , UINT8 InitFunction);

extern BOOLEAN LuaInternalQuest( UINT8 ubQuest, INT16 sSectorX, INT16 sSectorY, BOOLEAN fUpdateHistory, UINT32 Init );
extern BOOLEAN LuaExecuteStrategicEvent( UINT8 EventCallbackID, UINT32 uiTimeStamp, UINT32 uiTimeOffset, UINT8	ubEventType, UINT8 ubFlags, UINT32 EventParam, UINT32 Init);

extern void IniLuaGlobal();

extern BOOLEAN LoadLuaGlobalFromLoadGameFile( HWFILE hFile );
extern BOOLEAN SaveLuaGlobalToSaveGameFile( HWFILE hFile );


#ifdef JA2UB
extern BOOLEAN LuaInitStrategicLayer(UINT8 Init);
extern BOOLEAN LetLuaMakeBadSectorListFromMapsOnHardDrive(UINT8 Init);
#endif

//Intro
extern BOOLEAN LuaIntro(UINT8 Init, UINT32 uiCurrentVideo, INT8 bIntroType, UINT32 iStringToUse);
extern BOOLEAN LuaIDScripts(UINT8 Init, UINT8 ubTargetNPC, UINT16 usActionCode, UINT8 ubQuoteNum);

extern BOOLEAN LetLuaMusicControl(UINT8 Init);

void LuaHandleAdditionalDialogue( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubProfile, INT32 iFaceIndex, UINT16 usEventNr, UINT32 aData1, UINT32 aData2, UINT32 aData3 );
void LuaHandleReplaceQuote( UINT8 ubProfile, UINT16 usQuoteNum );
void LuaHandleNPCMerchantQuote( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT16 ubMerchantID, UINT8 ubBodyType, UINT16 usQuoteNum );
void LuaAddArmsDealerAdditionalIntelData();
void LuaAddPhotoData( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, INT32 sGridNo, INT8 bLevel, UINT8 ubPhotographerProfile, UINT16 room, UINT8 usTargetProfile );
void LuaGetPhotoData( UINT8 aType );
void LuaSetPhotoState( INT16 asIndex, UINT8 aState );
void LuaVerifyPhotoState( INT16 asIndex );
void LuaGetIntelAndQuestMapData( INT32 aLevel );
void SetFactoryLeftoverProgress( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT16 usFacilityType, UINT16 usProductionNumber, INT32 sProgressLeft );
INT32 GetFactoryLeftoverProgress( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT16 usFacilityType, UINT16 usProductionNumber );
#endif
