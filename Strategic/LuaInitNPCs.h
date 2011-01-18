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
extern BOOLEAN LuaHandleQuestCodeOnSectorExit( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init);
extern BOOLEAN LuaHandleQuestCodeOnSectorEntry( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init);
extern BOOLEAN LuaHandleDelayedItemsArrival( UINT32 uiReason, UINT8 Init);
extern BOOLEAN LetLuaHandleNPCSystemEvent( UINT32 uiEvent, UINT8 Init);

extern BOOLEAN LuaCheckFact ( UINT16 usFact, UINT8 ubProfileID , UINT32 Init);
extern BOOLEAN LuaHandleNPCDoAction( UINT8 ubTargetNPC, UINT16 usActionCode, UINT8 ubQuoteNum , UINT8 InitFunction);

extern void IniLuaGlobal();

extern BOOLEAN LoadLuaGlobalFromLoadGameFile( HWFILE hFile );
extern BOOLEAN SaveLuaGlobalToSaveGameFile( HWFILE hFile );

//Intro
extern BOOLEAN LuaIntro(UINT8 Init, UINT32 uiCurrentVideo, INT8 bIntroType, UINT32 iStringToUse);

#endif
