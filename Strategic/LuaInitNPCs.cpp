#include <iostream>
#include <string>
#include <sstream>

#include "Lua Interpreter.h"
#include "Strategic Mines.h"
#include "BobbyRMailOrder.h"
#include "Tactical Save.h"
#include "connect.h"
#include "FileMan.h"
#include "GameSettings.h"
#include "Game Init.h"
#include "interface Dialogue.h"
#include "opplist.h"
#include "Strategic All.h"
#include "pits.h"
#include  "Game Event Hook.h"
#include "Creature Spreading.h"
#include "Luaglobal.h"
#include "Quests.h"
#include "ai.h"
#include "Game Clock.h"
#include "interface Dialogue.h"
#include "Map Screen Helicopter.h"
#include "NPC.h"
#include "Strategic AI.h"
#include "Tactical Save.h"
#include "Items.h"
#include "Handle Items.h"
#include "Overhead.h"
#include "LuaInitNPCs.h"
#include "ShopKeeper Interface.h"
#include "Explosion Control.h"
#include "soldier tile.h"
#include "Soldier Profile.h"
#include "Soldier Functions.h"
#include "soldier tile.h"
#include "Action Items.h"
#include "AIInternals.h"
#include "Handle Doors.h"
#include "soldier profile type.h"
#include "history.h"
#include "Merc Hiring.h"

#include "LOS.h"
#include "Music Control.h"

#ifdef JA2UB
#include "Ja25 Strategic Ai.h"
#include "Ja25_Tactical.h"
#include "Ja25Update.h"
#include "ub_config.h"
#include "Ja25Update.h"
#endif

#include "Intro.h"
#include "End Game.h"
#include "Queen Command.h"
#include "gamescreen.h"
#include "Arms Dealer Init.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "lua_state.h"
#include "lua_function.h"
#include <vfs/Core/vfs_string.h>

#include "Encrypted File.h"
//-------------------------- UB ------------------------------
extern	BOOLEAN	gfDoneWithSplashScreen;
extern UINT32 iStringToUseLua;
extern INT8 Test;

//Briefing room
static int l_SetEndMission(lua_State *L);
static int l_SetStartMission(lua_State *L);
static int l_CheckMission (lua_State *L);

void FatigueCharacter( SOLDIERTYPE *pSoldier );

static int l_AddCustomEmail (lua_State *L);

static int l_SetDefaultArrivalSector (lua_State *L);
static int l_GetDefaultArrivalSector (lua_State *L);
static int l_SetMercArrivalLocation(lua_State *L);
static int l_GetDefaultArrivalSectorX (lua_State *L);
static int l_GetDefaultArrivalSectorY (lua_State *L);

static int l_InitProfile(lua_State *L);

#ifdef JA2UB

static int l_InitMercgridNo0 (lua_State *L);
static int l_InitMercgridNo1 (lua_State *L);
static int l_InitMercgridNo2 (lua_State *L);
static int l_InitMercgridNo3 (lua_State *L);
static int l_InitMercgridNo4 (lua_State *L);
static int l_InitMercgridNo5 (lua_State *L);
static int l_InitMercgridNo6 (lua_State *L);
static int l_InitJerryGridNo (lua_State *L);

static int l_SetInternalLocateGridNo(lua_State *L);

static int l_setInGameHeliCrash (lua_State *L);
static int l_setJerryQuotes (lua_State *L);
static int l_setInJerry (lua_State *L);
static int l_setLaptopQuest (lua_State *L);
static int l_setInGameHeli (lua_State *L);


//static int l_getMercgridNo0 (lua_State *L);
//static int l_getMercgridNo1 (lua_State *L);
//static int l_getMercgridNo2 (lua_State *L);
//static int l_getMercgridNo3 (lua_State *L);
//static int l_getMercgridNo4 (lua_State *L);
//static int l_getMercgridNo5 (lua_State *L);
//static int l_getMercgridNo6 (lua_State *L);

static int l_Ja25SaveStructJohnKulbaIsInGame(lua_State *L);
static int l_Ja25SaveCheckStructJohnKulbaIsInGame(lua_State *L);

static int l_Ja25SaveStructubJohnKulbaInitialSectorY(lua_State *L);
static int l_Ja25SaveStructubJohnKulbaInitialSectorX(lua_State *L);

static int l_SetNumberJa25EnemiesInSurfaceSector(lua_State *L);
static int l_SetNumberOfJa25BloodCatsInSector(lua_State *L);

static int l_HasNpcSaidQuoteBefore(lua_State *L);

static int l_ShouldThePlayerStopWhenWalkingOnBiggensActionItem(lua_State *L);


static int l_HandleSeeingPowerGenFan (lua_State *L);
static int l_HandleSwitchToOpenFortifiedDoor (lua_State *L);
static int l_HandleSeeingFortifiedDoor (lua_State *L);
static int l_HandlePlayerHittingSwitchToLaunchMissles (lua_State *L);
static int l_HavePersonAtGridnoStop(lua_State *L);

#endif

static int l_WhoIsThere2 (lua_State *L);

static int l_GuaranteeAtLeastXItemsOfIndex(lua_State *L);
static int l_gMercProfileGearset(lua_State *L);

static int l_FindUnderGroundSector(lua_State *L);
static int l_AddEnemyToUnderGroundSector(lua_State *L);
static int l_FindUnderGroundSectorVisited(lua_State *L);

#ifdef JA2UB
static int l_EnterTacticalInFinalSector(lua_State *L);
#endif

static int l_ReStartingGame(lua_State *L);

//Intro
static int l_DisplaySirtechSplashScreen(lua_State *L);
static int l_gfIntroScreenExit(lua_State *L);
static int l_gfEnteringMapScreen(lua_State *L);
static int l_gfDoneWithSplashScreen(lua_State *L);
static int l_guiIntroExitScreen(lua_State *L);
static int l_gbIntroScreenMode(lua_State *L);
static int l_iStringToUse(lua_State *L);
static int l_StopVideo(lua_State *L);
static int l_StartVideo(lua_State *L);

UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );

BOOLEAN LoadLuaGlobalFromLoadGameFile( HWFILE hFile );
BOOLEAN SaveLuaGlobalToSaveGameFile( HWFILE hFile );

BOOLEAN LetHandleLoyaltyChangeForNPCAction(UINT8 ubNPCProfileId , UINT8 Init);
BOOLEAN LuaHandleGlobalLoyaltyEvent( UINT8 ubEventType, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ , UINT8 Init);
BOOLEAN LetLuaHandleEarlyMorningEvents(UINT8 Init);
BOOLEAN LetLuaHourlyQuestUpdate(UINT8 Init);
BOOLEAN LetLuaMyCustomHandleAtNewGridNo(UINT8 bNewSide, UINT8 ProfileId,  UINT8 Init);
BOOLEAN LetLuaPerformItemAction(UINT32 Action, INT32 sGridNo, UINT8 InitFunction);
BOOLEAN LetLuaGameInit(UINT8 Init);
BOOLEAN LetLuaInterfaceDialogue( UINT8 ubNPC, UINT8 InitFunction);
BOOLEAN LuaHandlePlayerTeamMemberDeath(UINT8 ProfileId, UINT8 Init);
BOOLEAN LuaHandleNPCTeamMemberDeath(UINT8 ProfileId, UINT8 Init);
BOOLEAN LuaCheckForKingpinsMoneyMissing( BOOLEAN fFirstCheck, UINT8 Init);
BOOLEAN LuaHandleQuestCodeOnSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init);
//BOOLEAN LuaHandleQuestCodeOnSectorEntry( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init);
BOOLEAN LuaHandleDelayedItemsArrival( UINT32 uiReason, UINT8 Init);
BOOLEAN LetLuaHandleNPCSystemEvent( UINT32 uiEvent, UINT8 Init);

BOOLEAN LuaCheckFact ( UINT16 usFact, UINT8 ubProfileID , UINT32 Init);
BOOLEAN LuaHandleNPCDoAction( UINT8 ubTargetNPC, UINT16 usActionCode, UINT8 ubQuoteNum , UINT8 InitFunction);


//AI ACTION

static int l_ACTION_ITEM_OPEN_DOOR (lua_State *L);
static int l_ACTION_ITEM_CLOSE_DOOR (lua_State *L);
//static int l_ACTION_ITEM_BLOW_UP (lua_State *L);
static int l_ACTION_ITEM_SMALL_PIT (lua_State *L);
static int l_ACTION_ITEM_LARGE_PIT (lua_State *L);
static int l_ACTION_ITEM_TOGGLE_DOOR (lua_State *L);
static int l_ACTION_ITEM_TOGGLE_ACTION1 (lua_State *L);
static int l_ACTION_ITEM_TOGGLE_ACTION2 (lua_State *L);
static int l_ACTION_ITEM_TOGGLE_ACTION3 (lua_State *L);
static int l_ACTION_ITEM_TOGGLE_ACTION4 (lua_State *L);
static int l_ACTION_ITEM_ENTER_BROTHEL (lua_State *L);
static int l_ACTION_ITEM_EXIT_BROTHEL (lua_State *L);
static int l_ACTION_ITEM_KINGPIN_ALARM (lua_State *L);
static int l_ACTION_ITEM_SEX (lua_State *L);
static int l_ACTION_ITEM_REVEAL_ROOM (lua_State *L);
static int l_ACTION_ITEM_LOCAL_ALARM (lua_State *L);
static int l_ACTION_ITEM_GLOBAL_ALARM (lua_State *L);
static int l_ACTION_ITEM_KLAXON (lua_State *L);
static int l_ACTION_ITEM_UNLOCK_DOOR (lua_State *L);
static int l_ACTION_ITEM_TOGGLE_LOCK (lua_State *L);
static int l_ACTION_ITEM_UNTRAP_DOOR (lua_State *L);
static int l_ACTION_ITEM_TOGGLE_PRESSURE_ITEMS (lua_State *L);
static int l_ACTION_ITEM_MUSEUM_ALARM (lua_State *L);
static int l_ACTION_ITEM_BLOODCAT_ALARM (lua_State *L);
	
static int l_SetGlobalLuaBool (lua_State *L);
static int l_SetGlobalLuaVal (lua_State *L);

static int l_SetGlobalLuaBoolFalse (lua_State *L);
static int l_SetGlobalLuaBoolTrue (lua_State *L);

static int l_GetGlobalLuaVal (lua_State *L);

static int l_CheckGlobalLuaBool (lua_State *L);
static int l_CheckGlobalLuaVal (lua_State *L);

static int l_AddNPC(lua_State *L);
static int l_AddAlternateSector(lua_State *L);
static int l_AddAltUnderGroundSector(lua_State *L);

static int l_IncrementTownLoyalty(lua_State *L);
static int l_IncrementTownLoyaltyEverywhere(lua_State *L);
static int l_DecrementTownLoyalty(lua_State *L);
static int l_SetTownLoyalty(lua_State *L);
static int l_Merc_Is_Dead (lua_State *L);
static int l_SetFactFalse(lua_State *L);
static int l_SetFactTrue(lua_State *L);
static int l_CheckFact (lua_State *L);
static int l_SetMoneyInSoldierProfile(lua_State *L);
static int l_CheckForMissingHospitalSupplies(lua_State *L);
static int l_RemoveObjectFromSoldierProfile(lua_State *L);
static int l_bThreatenDefaultResponseUsedRecently(lua_State *L);
static int l_bRecruitDefaultResponseUsedRecently(lua_State *L);
static int l_bFriendlyOrDirectDefaultResponseUsedRecently(lua_State *L);
static int l_ubMiscFlags2Set(lua_State *L);
static int l_ubMiscFlags2Check(lua_State *L);
static int l_ubMiscFlags1Set(lua_State *L);
static int l_ubMiscFlags1Check(lua_State *L);
static int l_bNPCData(lua_State *L);
static int l_bNPCData2(lua_State *L);
static int l_CheckNPCSector (lua_State *L);
static int l_ubLastDateSpokenTot (lua_State *L);
static int l_fNPCData (lua_State *L);
static int l_bLife (lua_State *L);
static int l_iBalance(lua_State *L);
static int l_fNPCData2 (lua_State *L);
static int l_fuiMoney (lua_State *L);
static int l_SetuiMoney(lua_State *L);
static int l_StartQuest(lua_State *L);
static int l_EndQuest(lua_State *L);
static int l_HospitalTempBalance(lua_State *L);
static int l_HospitalRefund(lua_State *L);
static int l_HospitalPriceModifier(lua_State *L);
static int l_gfPlayerTeamSawJoey(lua_State *L);
static int l_AddNPCTOSECTOR(lua_State *L);
static int l_AddAltUnderGroundSectorNew(lua_State *L);
static int l_AddAlternateSectorNew(lua_State *L);
static int l_TriggerNPCRecord(lua_State *L);
static int l_TriggerNPCRecordImmediately(lua_State *L);
static int l_SetCharacterSectorX (lua_State *L);
static int l_SetCharacterSectorY (lua_State *L);
static int l_SetCharacterSectorZ (lua_State *L);

static int l_GetCharacterSectorX (lua_State *L);
static int l_GetCharacterSectorY (lua_State *L);
static int l_GetCharacterSectorZ (lua_State *L);

static int l_SetPendingNewScreenSEXSCREEN (lua_State *L);
static int l_CheckCharacterSectorX (lua_State *L);
static int l_CheckCharacterSectorY (lua_State *L);
static int l_CheckCharacterSectorZ (lua_State *L);
static int l_CheckNPCSectorNew (lua_State *L);
static int l_GetWorldHour (lua_State *L);
static int l_FindItemSoldier (lua_State *L);

static int l_CheckNPCWoundedbyPlayer (lua_State *L);
static int l_CheckNPCWounded (lua_State *L);
static int l_CheckNPCInOkayHealth (lua_State *L);
static int l_CheckNPCBleeding (lua_State *L);
static int l_CheckNPCWithin (lua_State *L);

static int l_CheckGuyVisible (lua_State *L);
static int l_CheckNPCAt (lua_State *L);
static int l_CheckNPCIsEnemy (lua_State *L);
//static int l_CheckIfMercIsNearNPC (lua_State *L);
static int l_NumWoundedMercsNearby (lua_State *L);
static int l_NumMercsNear (lua_State *L);
static int l_CheckNPCIsEPC (lua_State *L);
static int l_NPCInRoom (lua_State *L);
static int l_NPCInRoomRange (lua_State *L);
static int l_PCInSameRoom (lua_State *L);
static int l_NumMalesPresent (lua_State *L);
static int l_FemalePresent (lua_State *L);
static int l_CheckPlayerHasHead (lua_State *L);
static int l_CheckNPCSectorBool (lua_State *L);
static int l_AIMMercWithin (lua_State *L);
static int l_CheckNPCCowering (lua_State *L);
static int l_CheckNPCIsUnderFire (lua_State *L);
static int l_NPCHeardShot (lua_State *L);
static int l_InTownSectorWithTrainingLoyalty (lua_State *L);
//static int l_CheckFact (lua_State *L);
static int l_gubQuest (lua_State *L);
static int l_UnRecruitEPC (lua_State *L);
static int l_MakeHostile (lua_State *L);

static int l_PythSpacesAway (lua_State *L);
static int l_FindSoldierTeam (lua_State *L);
static int l_ubWhatKindOfMercAmI (lua_State *L);

static int l_SetUpHelicopterForPlayer (lua_State *L);
static int l_GetGridoProfileID (lua_State *L);

static int lh_getIntegerFromTable(lua_State *L, const char * fieldname);
static std::string lh_getStringFromTable(lua_State *L, const char * fieldname);
static int lh_getBooleanFromTable(lua_State *L, const char * fieldname);
static bool locationStringToCoordinates_AltSector(std::string loc, UINT8* x, UINT8* y);
static bool locationStringToCoordinates(std::string loc, UINT8* x, UINT8* y, UINT8* z);

static int l_FindSoldierByProfileID (lua_State *L);;
static int l_TileIsOutOfBoundsClosestPC (lua_State *L);

static int l_SetgfBoxerFought (lua_State *L);
static int l_EVENT_StopMerc (lua_State *L);

static int l_SetEnterCombatMode (lua_State *L);

static int l_PlayerInARoom (lua_State *L);

static int l_GetWorldMinutesInDay (lua_State *L);

static int l_ExecuteStrategicAIAction (lua_State *L);

static int l_AddToShouldBecomeHostileOrSayQuoteList(lua_State *L);

static int l_AddPreReadEmail (lua_State *L);
static int l_AddEmail (lua_State *L);
static int l_AddEmailXML (lua_State *L);
static int l_AddEmailXML2 (lua_State *L);
static int l_AddEmailLevelUpXML (lua_State *L);

static int l_EVENT_SoldierGotHit (lua_State *L);
static int l_EVENT_InitNewSoldierAnim (lua_State *L);

static int l_ChangeNpcToDifferentSector (lua_State *L);

static int l_CreateItem (lua_State *L);
static int l_CreateMoney (lua_State *L);

static int l_RecruitRPC (lua_State *L);
static int l_RecruitEPC (lua_State *L);
static int l_CheckNPCIsRPC (lua_State *L);

static int l_gTacticalStatus(lua_State *L);

static int l_CheckCombatMode (lua_State *L);

static int l_EnterShopKeeperInterfaceScreen (lua_State *L);
static int l_SetOffBombsByFrequency (lua_State *L);

static int l_TeleportSoldier (lua_State *L);

static int l_Action_door (lua_State *L);

static int l_Action_door_open (lua_State *L);
static int l_Action_door_close (lua_State *L);

//maps
static int l_SetRender (lua_State *L);
static int l_ConvertGridNoToXY (lua_State *L);
static int l_AddStructToHead (lua_State *L);
static int l_RemoveStruct (lua_State *L);
static int l_ApplyMapChangesToMapTempFile (lua_State *L);

static int l_NPCGotoGridNo (lua_State *L);

static int l_fEnemyControlled (lua_State *L);
static int l_fCivGroupHostile (lua_State *L);
static int l_ItemTypeExistsAtLocation (lua_State *L);

static int l_SetSoldierSide (lua_State *L);
static int l_CheckSoldierSide (lua_State *L);
static int l_CheckSoldierNeutral (lua_State *L);
static int l_CheckSoldierCivilianGroup (lua_State *L);
static int l_CheckSoldierubProfile (lua_State *L);
static int l_ChangeSoldierTeam (lua_State *L);
static int l_CheckSoldierInSector (lua_State *L);
static int l_CheckSoldierActive (lua_State *L);
static int l_RecalculateOppCntsDueToNoLongerNeutral (lua_State *L);
static int l_CheckForPotentialAddToBattleIncrement (lua_State *L);
static int l_SetSoldierOrders (lua_State *L);


static int l_SetCivGroupHostile (lua_State *L);
static int l_GetLastID (lua_State *L);
static int l_GetFirstID (lua_State *L);
static int l_GetDirection (lua_State *L);

//merc
static int l_CheckMercPtrsInCivilianGroup (lua_State *L);
static int l_CheckMercPtsrInSector (lua_State *L);
static int l_CheckMercPtrsActive (lua_State *L);
static int l_CheckMercPtsrubIDSeenubID2 (lua_State *L);

static int l_MakeMercPtrsHostile (lua_State *L);

static int l_SetEnterCombatModeTeam (lua_State *L);


static int l_SoldierTo3DLocationLineOfSightTest(lua_State *L);
static int l_CancelAIAction(lua_State *L);
static int l_RESETTIMECOUNTER(lua_State *L);
static int l_SetNextActionData(lua_State *L);
static int l_SetNextAction(lua_State *L);
static int l_CheckAction(lua_State *L);
static int l_SetAlertStatus(lua_State *L);
static int l_SetNoiseVolume(lua_State *L);
static int l_SetNoiseGridno(lua_State *L);
static int l_SetNewSituation(lua_State *L);
static int l_TriggerNPCWithIHateYouQuote(lua_State *L);
static int l_CheckSoldierNoiseVolume(lua_State *L);
static int l_CheckSoldierAlertStatus(lua_State *L);
static int l_TacticalStatusTeamActive(lua_State *L);

static int l_ToggleActionItemsByFrequency (lua_State *L);

static int l_PlayJA2Sample (lua_State *L);

static int l_ActionInProgress(lua_State *L);

static int l_AnimMercPtsrInSector (lua_State *L);
static int l_SetNewSituationMercPtsr (lua_State *L);

static int l_gubPublicNoiseVolume(lua_State *L);
static int l_gsPublicNoiseGridNo(lua_State *L);

static int l_Add3X3Pit (lua_State *L);
static int l_Add5X5Pit (lua_State *L);
static int l_SearchForOtherMembersWithinPitRadiusAndMakeThemFall (lua_State *L);
static int l_TogglePressureActionItemsInGridNo(lua_State *L);

static int l_TacticalStatusTeamHuman (lua_State *L);
static int l_WearGasMaskIfAvailable (lua_State *L);
static int l_MakeNoise(lua_State *L);

static int l_WorldLevelDataTerrainID(lua_State *L);

static int l_SetOffPanicBombs (lua_State *L);

static int l_ubID (lua_State *L);

static int l_SetProfileFaceData (lua_State *L);
static int l_GetProfileFaceData (lua_State *L);

static int l_ClosestPC (lua_State *L);
static int l_TileIsOutOfBounds (lua_State *L);
static int l_AnimMercPtsrSoldierGotHit (lua_State *L);

static int l_HandleNPCGotoGridNo(lua_State *L);
static int l_HandleNPCDoAction(lua_State *L);
static int l_HandleNPCClosePanel(lua_State *L);
static int l_HandleNPCTriggerNPC(lua_State *L);
static int l_IS_CIV_BODY_TYPE (lua_State *L);
static int l_SetOffBombsInGridNo (lua_State *L);
static int l_ActivateSwitchInGridNo(lua_State *L);

static int l_UpdateAndDamageSAMIfFound(lua_State *L);
static int l_DoesSAMExistHere(lua_State *L);
static int l_UpdateSAMDoneRepair(lua_State *L);
static int l_ActiveTimedBombExists(lua_State *L);
static int l_RemoveAllActiveTimedBombs(lua_State *L);

static int l_CheckSoldierBodyType (lua_State *L);
static int l_GetSoldierBodyType (lua_State *L);
static int l_SetSoldierBodyType (lua_State *L);

static int l_ChangeMercPtrsTeam (lua_State *L);

static int l_AnimMercPtsrfAIFlags (lua_State *L);
static int l_AnimMercPtsrbNextAction (lua_State *L);
static int l_AnimMercPtsrusNextActionData (lua_State *L);
static int l_AnimMercPtsrsAbsoluteFinalDestination (lua_State *L);
static int l_AnimMercPtsrubStrategicInsertionCode (lua_State *L);
static int l_AnimMercPtsrusStrategicInsertionData (lua_State *L);

static int l_SetMusicMode (lua_State *L);
static int l_MusicPlay (lua_State *L);
static int l_MusicSetVolume (lua_State *L);
static int l_MusicGetVolume (lua_State *L);
//static int l_MusicStop (lua_State *L);
//static int l_MusicFadeOut (lua_State *L);
//static int l_MusicFadeIn (lua_State *L);
static int l_SetSoundEffectsVolume (lua_State *L);
static int l_SetSpeechVolume (lua_State *L);
static int l_GetSoundEffectsVolume (lua_State *L);
static int l_GetSpeechVolume (lua_State *L);

static int l_SetMercProfiles(lua_State *L);

static int l_PauseGame (lua_State *L);

static int l_SetInvestigateSector (lua_State *L);
static int l_CheckInvestigateSector (lua_State *L);

static int l_AddTransactionToPlayersBook (lua_State *L);
static int l_GetWorldTotalMin (lua_State *L);

static int l_GetStartingCashNovice (lua_State *L);
static int l_GetStartingCashExperienced (lua_State *L);
static int l_GetStartingCashExpert (lua_State *L);
static int l_GetStartingCashInsane (lua_State *L);

static int l_DeleteTalkingMenu (lua_State *L);

static int l_PlayerMercsInSector (lua_State *L);
static int l_GetPlayerMercsInSector (lua_State *L);
static int l_PlayerGroupsInSector (lua_State *L);
static int l_GetPlayerGroupsInSector(lua_State *L);

static int l_SetSoldierNonNeutral(lua_State *L);

static int l_bNeutral(lua_State *L);

static int l_AffectAllTownsLoyaltyByDistanceFrom (lua_State *L);

static int l_InitCreatureQuest (lua_State *L);

static int l_bAttitude(lua_State *L);

static int l_DecrementTownLoyaltyEverywhere(lua_State *L);

static int l_GetSoldierTeam (lua_State *L);

static int l_WhichBuddy (lua_State *L);

//Object
static int l_SetgWorldItemsExists(lua_State *L);
static int l_gWorldItemsExists(lua_State *L);
static int l_gWorldItemsObjectItem(lua_State *L);
static int l_gWorldItemsObjectDataMoney(lua_State *L);

static int l_AddHistoryToPlayersLog(lua_State *L);
static int l_AddFutureDayStrategicEvent(lua_State *L);

static int l_GetiBalance(lua_State *L);

static int l_CreateItemToPool (lua_State *L);
static int l_CreateToUnLoadedSector (lua_State *L);

static int l_AddSameDayStrategicEvent(lua_State *L);

static int l_FunctionCheckForKingpinsMoneyMissing(lua_State *L);

static int l_MoveItemPools(lua_State *L);
static int l_GetNumberOfWorldItemsFromTempItemFile(lua_State *L);

static int l_gubFact(lua_State *L);

static int l_GetgubFact(lua_State *L);

static int l_GetTacticalStatusEnemyInSector (lua_State *L);

static int l_CountNumberOfBobbyPurchasesThatAreInTransit(lua_State *L);

static int l_CheckForNewShipment(lua_State *L);

static int l_CheckTalkerStrong(lua_State *L);

static int l_CheckTalkerFemale(lua_State *L);


static int l_IssueHeadMinerQuote(lua_State *L);
static int l_GetHeadMinersMineIndex(lua_State *L);
static int l_PlayerSpokeToHeadMiner(lua_State *L);
static int l_IsHisMineRunningOut(lua_State *L);
static int l_IsHisMineEmpty(lua_State *L);
static int l_IsHisMineDisloyal(lua_State *L);
static int l_IsHisMineInfested(lua_State *L);
static int l_IsHisMineLostAndRegained(lua_State *L);
static int l_IsHisMineAtMaxProduction(lua_State *L);
static int l_ResetQueenRetookMine(lua_State *L);

static int l_GetiFirstArrivalDelay (lua_State *L);
static int l_GetubDefaultArrivalSectorX (lua_State *L);
static int l_GetubDefaultArrivalSectorY (lua_State *L);

static int l_SetiFirstArrivalDelay (lua_State *L);
static int l_SetubDefaultArrivalSectorX (lua_State *L);
static int l_SetubDefaultArrivalSectorY (lua_State *L);

static int l_SectorEnemy(lua_State *L);

static int l_ResetHistoryFact(lua_State *L);
static int l_SetHistoryFact(lua_State *L);

static int l_TacticalCharacterDialogue(lua_State *L);

static int l_DeleteMercInventory(lua_State *L);

static int l_IsMercDead(lua_State *L);
static int l_IsMercHireable(lua_State *L);
static int l_NumberOfMercsOnPlayerTeam(lua_State *L);
static int l_GetMercArrivalTimeOfDay(lua_State *L);
static int l_StrategicPythSpacesAway(lua_State *L);

static int l_ItemExistsAtLocation(lua_State *L);

static int l_Merc_Status (lua_State *L);

static int l_Get_Merc_Status (lua_State *L);

static int l_SetbLife (lua_State *L);
static int l_GetbLifeMax (lua_State *L);

static int l_HasHisMineBeenProducingForPlayerForSomeTime (lua_State *L);

static int l_AnotherFightPossible (lua_State *L);

static int l_PredictDailyIncomeFromAMine(lua_State *L);

static int l_SetGroupSectorValue(lua_State *L);
static int l_SetGroupNextSectorValue(lua_State *L);

static int l_FadeOutGameScreen(lua_State *L);

static int l_FadeInGameScreen(lua_State *L);

static int l_SetCurrentWorldSector(lua_State *L);
static int l_EnterSector(lua_State *L);

static int l_GetTownIdForSector(lua_State *L);
static int l_UpdateMercsInSector(lua_State *L);
static int l_CheckIfEntireTownHasBeenLiberated(lua_State *L);
static int l_CheckIfEntireTownHasBeenLost(lua_State *L);
static int l_DidFirstBattleTakePlaceInThisTown(lua_State *L);
static int l_SetTheFirstBattleSector(lua_State *L);
static int l_AdjustLoyaltyForCivsEatenByMonsters(lua_State *L);
static int l_IsTownUnderCompleteControlByPlayer(lua_State *L);
static int l_GetNumberOfWholeTownsUnderControl(lua_State *L);

static int l_GetusTotalDaysServed(lua_State *L);
static int l_SetusTotalDaysServed(lua_State *L);

static int l_GetTownLoyaltyfLiberatedAlready(lua_State *L);
static int l_GetTownLoyaltyfStarted(lua_State *L);
static int l_GetTownLoyaltyubRating(lua_State *L);
static int l_GetTownLoyaltysChange(lua_State *L);

static int l_SetTownLoyaltyfLiberatedAlready(lua_State *L);
static int l_SetTownLoyaltyfStarted(lua_State *L);
static int l_SetTownLoyaltyubRating(lua_State *L);
static int l_SetTownLoyaltysChange(lua_State *L);

static int l_GetgfTownUsesLoyalty(lua_State *L);
static int l_SetgfTownUsesLoyalty(lua_State *L);

static int l_GetgMercProfilesbTown(lua_State *L);
static int l_SetgMercProfilesbTown(lua_State *L);

static int l_GetTimeQuestWasStarted (lua_State *L);

static int l_RepairmanIsFixingItemsButNoneAreDoneYet(lua_State *L);

static int l_SECTOR(lua_State *L);
static int l_SectorInfoBloodCats(lua_State *L);

static int l_SpokenToHeadMiner(lua_State *L);
static int l_GetIdOfMineForSector(lua_State *L);
static int l_CALCULATE_STRATEGIC_INDEX(lua_State *L);

static int l_CheckTalkerUnpropositionedFemale(lua_State *L);
static int l_BoxerExists(lua_State *L);

static int l_CurrentPlayerProgressPercentage(lua_State *L);

static int l_UnderGroundSectorVisited(lua_State *L);

static int l_fTurnTimeLimit(lua_State *L);

static int l_InitFace(lua_State *L);

static int l_WhoIs(lua_State *L);

static int l_SetHandleGlobalLoyaltyEvent (lua_State *L);

//----05-08-2011---

// tactical tex box function
static int l_ExecuteTacticalTextBox(lua_State *L);

//Town function
static int l_VisibleTown (lua_State *L);
static int l_HiddenTown (lua_State *L);
//static int l_EraseTown (lua_State *L);
//static int l_ResizeTown (lua_State *L);

static int l_gubBoxerID(lua_State *L);
static int l_RemoveGraphicFromTempFile (lua_State *L);

using namespace std;

UINT16 idProfil;
UINT32 uiHourLua;
UINT32 uiDayLua;
UINT32 uiMinLua;

UINT16 PROFILLUA_sSectorX;
UINT16 PROFILLUA_sSectorY;
UINT8 PROFILLUA_bSectorZ;
UINT8 PROFILLUA_Level;
UINT8 PROFILLUA_ubID;
UINT32 PROFILLUA_sGridNo;
UINT8 PROFILLUA_ubDirectiono;
UINT8 PROFILLUA_bTeam;

UINT8  PROFILLUA2_ubProfile;
UINT16 PROFILLUA2_sSectorX;
UINT16 PROFILLUA2_sSectorY;
UINT8  PROFILLUA2_bSectorZ;
UINT32 PROFILLUA2_sGridNo;
UINT8  PROFILLUA2_ubID;

LUA_GLOBAL	gLuaGlobal[1000]; 

ACTION_ITEM_VALUES ActionItemsValues[500];

void IniLuaGlobal()
{
UINT32 i;

	for (i= 0; i<=1000; i++ )
	{
		gLuaGlobal[i].fGlobalLuaBool = FALSE;
		gLuaGlobal[i].iGlobalLuaVal = 0;
	}
}

void IniGlobal_0(lua_State *L)
{
	
		lua_pushinteger(L, PROFILLUA_sSectorX);
		lua_setglobal(L, "ProfileIdsSectorX");
	
		lua_pushinteger(L, PROFILLUA_sSectorY);
		lua_setglobal(L, "ProfileIdsSectorY");
	
		lua_pushinteger(L, PROFILLUA_bSectorZ);
		lua_setglobal(L, "ProfileIdbSectorZ");
	
		lua_pushinteger(L, PROFILLUA_Level);
		lua_setglobal(L, "ProfileIdLevel");
	
		lua_pushinteger(L, PROFILLUA_ubID);
		lua_setglobal(L, "ProfileIdubID");
	
		lua_pushinteger(L, PROFILLUA_sGridNo);
		lua_setglobal(L, "ProfileIdsGridNo");
	
		lua_pushinteger(L, PROFILLUA_ubDirectiono);
		lua_setglobal(L, "ProfileIdubDirectiono");

		lua_pushinteger(L, PROFILLUA_bTeam);
		lua_setglobal(L, "ProfileIdbTeam");
		
}

void IniGlobal_1(lua_State *L)
{
		lua_pushinteger(L, PROFILLUA2_ubProfile);
		lua_setglobal(L, "ProfileId");
	
		lua_pushinteger(L, PROFILLUA2_sSectorX);
		lua_setglobal(L, "ProfileIdsSectorX");
	
		lua_pushinteger(L, PROFILLUA2_sSectorY);
		lua_setglobal(L, "ProfileIdsSectorY");
	
		lua_pushinteger(L, PROFILLUA2_bSectorZ);
		lua_setglobal(L, "ProfileIdbSectorZ");
	
		lua_pushinteger(L, PROFILLUA2_ubID);
		lua_setglobal(L, "ProfileIdubID");
	
		lua_pushinteger(L, PROFILLUA2_sGridNo);
		lua_setglobal(L, "ProfileIdsGridNo");	
		
}		
void IniFunction(lua_State *L)
{

	//Briefing room
	lua_register(L, "SetEndMission", l_SetEndMission);
	lua_register(L, "SetStartMission", l_SetStartMission);
	lua_register(L, "CheckMission", l_CheckMission);

	//Sector
	lua_register(L, "AddAlternateSector", l_AddAlternateSector);
	lua_register(L, "AddAltUnderGroundSector", l_AddAltUnderGroundSector);
	lua_register(L, "AddAltSector", l_AddAlternateSector);
	lua_register(L, "AddAltUGSector", l_AddAltUnderGroundSector);
	lua_register(L, "AddAltSectorNew", l_AddAlternateSectorNew);
	lua_register(L, "AddAltUGSectorNew", l_AddAltUnderGroundSectorNew);

	//Loyalty
	lua_register(L, "IncrementTownLoyalty", l_IncrementTownLoyalty);
	lua_register(L, "IncrementTownLoyaltyEverywhere", l_IncrementTownLoyaltyEverywhere);
	lua_register(L, "DecrementTownLoyalty", l_DecrementTownLoyalty);
	lua_register(L, "SetTownLoyalty", l_SetTownLoyalty);
	lua_register(L, "InTownSectorWithTrainingLoyalty",l_InTownSectorWithTrainingLoyalty);
	
	//Merc - Add
	
	lua_register(L, "SetMercProfiles", l_SetMercProfiles);
	lua_register(L, "AddNPC", l_AddNPC);
	lua_register(L, "AddNPCtoSector", l_AddNPCTOSECTOR);	
	lua_register(L, "ChangeNpcToDifferentSector", l_ChangeNpcToDifferentSector);	
	
	//Merc - Check
	lua_register(L, "CheckNPCSector", l_CheckNPCSector);	
	lua_register(L, "CheckLastDateSpokenTot", l_ubLastDateSpokenTot);	
	lua_register(L, "CheckNPCData1", l_fNPCData);	
	lua_register(L, "CheckNPCLife", l_bLife);
	lua_register(L, "CheckNPCData2", l_fNPCData2);
	lua_register(L, "CheckMoney", l_fuiMoney);
	lua_register(L, "CheckCharacterSectorX", l_CheckCharacterSectorX);
	lua_register(L, "CheckCharacterSectorY", l_CheckCharacterSectorY);	
	lua_register(L, "CheckNPCinSector", l_CheckNPCSectorNew);	
	lua_register(L, "CheckMiscFlags2", l_ubMiscFlags2Check);
	lua_register(L, "CheckMiscFlags1", l_ubMiscFlags1Check);
	lua_register(L, "CheckMercIsDead", l_Merc_Is_Dead);
	lua_register(L, "MercIsDead", l_Merc_Is_Dead);	
	lua_register(L, "CheckNPCWounded",l_CheckNPCWounded);
	lua_register(L, "CheckNPCWoundedByPlayer",l_CheckNPCWoundedbyPlayer);
	lua_register(L, "CheckNPCInOkayHealth",l_CheckNPCInOkayHealth);
	lua_register(L, "CheckNPCBleeding",l_CheckNPCBleeding);
	lua_register(L, "CheckNPCWithin",l_CheckNPCWithin);
	lua_register(L, "CheckGuyVisible",l_CheckGuyVisible);
	lua_register(L, "CheckNPCAt",l_CheckNPCAt);
	lua_register(L, "CheckNPCIsEnemy",l_CheckNPCIsEnemy);
//	lua_register(L, "CheckIfMercIsNearNPC",l_CheckIfMercIsNearNPC);
	lua_register(L, "CheckNPCIsEPC",l_CheckNPCIsEPC);
	lua_register(L, "CheckNPCIsRPC",l_CheckNPCIsRPC);
	lua_register(L, "NPCInRoom",l_NPCInRoom);
	lua_register(L, "NPCInRoomRange",l_NPCInRoomRange);
	lua_register(L, "PCInSameRoom",l_PCInSameRoom);
	lua_register(L, "InARoom", l_PlayerInARoom);	
	lua_register(L, "FemalePresent",l_FemalePresent);
	lua_register(L, "CheckPlayerHasHead",l_CheckPlayerHasHead);
	lua_register(L, "CheckNPCSectorBool",l_CheckNPCSectorBool);
	lua_register(L, "AIMMercWithin",l_AIMMercWithin);
	lua_register(L, "CheckNPCCowering",l_CheckNPCCowering);
	lua_register(L, "CheckNPCIsUnderFire",l_CheckNPCIsUnderFire);
	lua_register(L, "NPCHeardShot",l_NPCHeardShot);
	lua_register(L, "CheckSoldierSide", l_CheckSoldierSide);
	lua_register(L, "CheckSoldierNeutral", l_CheckSoldierNeutral);
	lua_register(L, "CheckSoldierCivilianGroup", l_CheckSoldierCivilianGroup);
	lua_register(L, "CheckSoldierubProfile", l_CheckSoldierubProfile);
	lua_register(L, "ChangeSoldierTeam", l_ChangeSoldierTeam);
	lua_register(L, "CheckSoldierInSector", l_CheckSoldierInSector);	
	lua_register(L, "CheckSoldierActive", l_CheckSoldierActive);	
	lua_register(L, "CheckForPotentialAddToBattleIncrement", l_CheckForPotentialAddToBattleIncrement);
	
	lua_register(L, "CheckFPAddBattleIncrement", l_CheckForPotentialAddToBattleIncrement);
	
	lua_register(L, "CheckAction", l_CheckAction);
	lua_register(L, "CheckSoldierNoiseVolume", l_CheckSoldierNoiseVolume);
	lua_register(L, "CheckSoldierAlertStatus", l_CheckSoldierAlertStatus);	
	
	//Merc - set
	lua_register(L, "SetThreatenDefaultResponseUsedRecently", l_bThreatenDefaultResponseUsedRecently);
	lua_register(L, "SetRecruitDefaultResponseUsedRecently", l_bRecruitDefaultResponseUsedRecently);
	lua_register(L, "SetFriendlyOrDirectDefaultResponseUsedRecently", l_bFriendlyOrDirectDefaultResponseUsedRecently);
	
	//nowe
	lua_register(L, "SetTDRUsedRecently", l_bThreatenDefaultResponseUsedRecently);
	lua_register(L, "SetRDRUsedRecently", l_bRecruitDefaultResponseUsedRecently);
	lua_register(L, "SetFDRUsedRecently", l_bFriendlyOrDirectDefaultResponseUsedRecently);
	
	lua_register(L, "SetMiscFlags2", l_ubMiscFlags2Set);
	lua_register(L, "SetMiscFlags1", l_ubMiscFlags1Set);
	lua_register(L, "SetNPCData1", l_bNPCData);	
	lua_register(L, "SetNPCData2", l_bNPCData2);
	lua_register(L, "SetNPCBalance", l_iBalance);
	lua_register(L, "SetMoney", l_SetuiMoney);
	lua_register(L, "SetCharacterSectorZ", l_SetCharacterSectorZ);
	lua_register(L, "SetCharacterSectorX", l_SetCharacterSectorX);
	lua_register(L, "SetCharacterSectorY", l_SetCharacterSectorY);	
	lua_register(L, "SetMoneyInSoldierProfile", l_SetMoneyInSoldierProfile);
	lua_register(L, "SetAlertStatus", l_SetAlertStatus);
	lua_register(L, "SetNoiseVolume", l_SetNoiseVolume);
	lua_register(L, "SetNoiseGridno", l_SetNoiseGridno);
	lua_register(L, "SetNewSituation", l_SetNewSituation);
	lua_register(L, "SetNextActionData", l_SetNextActionData);
	lua_register(L, "SetNextAction", l_SetNextAction);	
	lua_register(L, "SetProfileFaceData", l_SetProfileFaceData );
	lua_register(L, "SetSoldierSide", l_SetSoldierSide);	
	lua_register(L, "SetSoldierOrders", l_SetSoldierOrders);

	//Merc - Get
	lua_register(L, "GetDirection", l_GetDirection); //new
	lua_register(L, "GetNPCGridNo", l_GetGridoProfileID);
	lua_register(L, "GetProfileFaceData", l_GetProfileFaceData );	
	
	//Merc - tiggers
	lua_register(L, "TriggerNPCRecord", l_TriggerNPCRecord);
	lua_register(L, "TriggerNPCRecordImmediately", l_TriggerNPCRecordImmediately);
	lua_register(L, "TriggerNPCWithIHateYouQuote", l_TriggerNPCWithIHateYouQuote);	
	
	//Merc - other
	lua_register(L, "FindItemSoldier",l_FindItemSoldier);
	lua_register(L, "UnRecruitEPC",l_UnRecruitEPC);
	lua_register(L, "MakeHostile",l_MakeHostile);
	lua_register(L, "PythSpacesAway",l_PythSpacesAway);
	lua_register(L, "RemoveObjectFromSoldierProfile", l_RemoveObjectFromSoldierProfile);	
	lua_register(L, "TeleportSoldier", l_TeleportSoldier);		
	lua_register(L, "DeadSoldier", l_EVENT_SoldierGotHit);	
	lua_register(L, "FindSoldierByProfileID", l_FindSoldierByProfileID);	
	lua_register(L, "TileIsOutOfBoundsClosestPC", l_TileIsOutOfBoundsClosestPC);	
	lua_register(L, "RecruitRPC", l_RecruitRPC);
	lua_register(L, "RecruitEPC", l_RecruitEPC);	
	lua_register(L, "NPCGotoGridNo", l_NPCGotoGridNo);	
	lua_register(L, "RecalculateOppCntsDueToNoLongerNeutral", l_RecalculateOppCntsDueToNoLongerNeutral);
	lua_register(L, "ActionStopMerc", l_EVENT_StopMerc);
	lua_register(L, "AddToShouldBecomeHostileOrSayQuoteList", l_AddToShouldBecomeHostileOrSayQuoteList);	
	lua_register(L, "EnterGroupCombatMode", l_SetEnterCombatMode);	
	lua_register(L, "InitNewSoldierAnim", l_EVENT_InitNewSoldierAnim);
	lua_register(L, "EnterShopKeeperInterfaceScreen", l_EnterShopKeeperInterfaceScreen);
	lua_register(L, "SoldierTo3DLocationLineOfSightTest", l_SoldierTo3DLocationLineOfSightTest);
	lua_register(L, "CancelAIAction", l_CancelAIAction);
	lua_register(L, "RESETTIMECOUNTER", l_RESETTIMECOUNTER);
	lua_register(L, "ClosestPC", l_ClosestPC );
	lua_register(L, "TileIsOutOfBounds", l_TileIsOutOfBounds );	
	
	//Team
	lua_register(L, "FindSoldierTeam",l_FindSoldierTeam);
	lua_register(L, "WhatKindOfMercAmI",l_ubWhatKindOfMercAmI);	

//  lua_register(L, "ActionInProgress", l_ActionInProgress);	
	
	
//  lua_register(L, "MakeCivHostile", l_MakeCivHostile); //new	
	
	//Fact and Quest
	lua_register(L, "SetFactTrue", l_SetFactTrue);
	lua_register(L, "SetFactFalse", l_SetFactFalse);
	lua_register(L, "CheckFact", l_CheckFact);
	lua_register(L, "CheckQuest", l_gubQuest);
	lua_register(L, "StartQuest", l_StartQuest);
	lua_register(L, "EndQuest", l_EndQuest);
	
	//Other
	//lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "HospitalTempBalance", l_HospitalTempBalance);
	lua_register(L, "HospitalRefund", l_HospitalRefund);
	lua_register(L, "HospitalPriceModifier", l_HospitalPriceModifier);
	lua_register(L, "PlayerTeamSawJoey", l_gfPlayerTeamSawJoey);
	lua_register(L, "SetUpHelicopterForPlayer", l_SetUpHelicopterForPlayer);
	lua_register(L, "MakeNoise", l_MakeNoise );
	lua_register(L, "WorldLevelDataTerrainID", l_WorldLevelDataTerrainID );

	//Action - Bomb
	lua_register(L, "SetOffBombsByFrequency", l_SetOffBombsByFrequency);	
	lua_register(L, "SetOffBombs", l_SetOffBombsByFrequency);	
	lua_register(L, "ToggleActionItemsByFrequency", l_ToggleActionItemsByFrequency);
	
	//Action - other
	lua_register(L, "ActionDoor", l_Action_door);	
	lua_register(L, "ActionDoorOpen", l_Action_door_open);
	lua_register(L, "ActionDoorClose", l_Action_door_close);
	lua_register(L, "Add3X3Pit", l_Add3X3Pit );
	lua_register(L, "Add5X5Pit", l_Add5X5Pit );
	lua_register(L, "SearchForOtherMembersWithinPitRadiusAndMakeThemFall", l_SearchForOtherMembersWithinPitRadiusAndMakeThemFall );
	lua_register(L, "TogglePressureActionItemsInGridNo", l_TogglePressureActionItemsInGridNo );	

	//Global
	lua_register(L, "SetGlobalBool", l_SetGlobalLuaBool);
	lua_register(L, "SetGlobalVal", l_SetGlobalLuaVal);	
	lua_register(L, "SetGlobalTrue", l_SetGlobalLuaBoolTrue);
	lua_register(L, "SetGlobalFalse", l_SetGlobalLuaBoolFalse);
	lua_register(L, "CheckGlobalBool", l_CheckGlobalLuaBool);
	lua_register(L, "CheckGlobalVal", l_CheckGlobalLuaVal);	
	lua_register(L, "GetGlobalVal", l_GetGlobalLuaVal);	
	
	//Maps
	lua_register(L, "SetRender", l_SetRender);
	lua_register(L, "ConvertGridNoToXY", l_ConvertGridNoToXY);
	lua_register(L, "AddStructToHead", l_AddStructToHead);	
	lua_register(L, "RemoveStruct", l_RemoveStruct);	
	lua_register(L, "ApplyMapChangesToMapTempFile", l_ApplyMapChangesToMapTempFile);	
	lua_register(L, "SectorEnemyControlled", l_fEnemyControlled);	
	lua_register(L, "ItemTypeExistsAtLocation", l_ItemTypeExistsAtLocation);	
	
	//Time
	lua_register(L, "GetWorldMinutesInDay", l_GetWorldMinutesInDay);
	lua_register(L, "GetWorldHour", l_GetWorldHour);		
	lua_register(L, "ExecuteStrategicAIAction", l_ExecuteStrategicAIAction);	
	
	//Email
	lua_register(L, "AddPreReadEmail", l_AddPreReadEmail);
	lua_register(L, "AddEmail", l_AddEmail);
	lua_register(L, "AddEmailMercAvailableXML", l_AddEmailXML);
	lua_register(L, "AddEmailMercLevelUpXML", l_AddEmailLevelUpXML);
	lua_register(L, "AddEmailXML", l_AddEmailXML2);
	//Items
	lua_register(L, "CreateItem", l_CreateItem);
	lua_register(L, "CreateMoney", l_CreateMoney);	
	
	//AI ACTION
	lua_register(L, "ACTION_ITEM_OPEN_DOOR", l_ACTION_ITEM_OPEN_DOOR);	
	lua_register(L, "ACTION_ITEM_CLOSE_DOOR", l_ACTION_ITEM_CLOSE_DOOR );
	//lua_register(L, "ACTION_ITEM_BLOW_UP", l_ACTION_ITEM_BLOW_UP );
	lua_register(L, "ACTION_ITEM_SMALL_PIT", l_ACTION_ITEM_SMALL_PIT );
	lua_register(L, "ACTION_ITEM_LARGE_PIT", l_ACTION_ITEM_LARGE_PIT );
	lua_register(L, "ACTION_ITEM_TOGGLE_DOOR", l_ACTION_ITEM_TOGGLE_DOOR );
	lua_register(L, "ACTION_ITEM_TOGGLE_ACTION1", l_ACTION_ITEM_TOGGLE_ACTION1 );
	lua_register(L, "ACTION_ITEM_TOGGLE_ACTION2", l_ACTION_ITEM_TOGGLE_ACTION2 );
	lua_register(L, "ACTION_ITEM_TOGGLE_ACTION3", l_ACTION_ITEM_TOGGLE_ACTION3 );
	lua_register(L, "ACTION_ITEM_TOGGLE_ACTION4", l_ACTION_ITEM_TOGGLE_ACTION4);
	lua_register(L, "ACTION_ITEM_ENTER_BROTHEL", l_ACTION_ITEM_ENTER_BROTHEL );
	lua_register(L, "ACTION_ITEM_EXIT_BROTHEL", l_ACTION_ITEM_EXIT_BROTHEL );
	lua_register(L, "ACTION_ITEM_KINGPIN_ALARM", l_ACTION_ITEM_KINGPIN_ALARM );
	lua_register(L, "ACTION_ITEM_SEX", l_ACTION_ITEM_SEX);
	lua_register(L, "ACTION_ITEM_REVEAL_ROOM", l_ACTION_ITEM_REVEAL_ROOM);
	lua_register(L, "ACTION_ITEM_LOCAL_ALARM", l_ACTION_ITEM_LOCAL_ALARM);
	lua_register(L, "ACTION_ITEM_GLOBAL_ALARM", l_ACTION_ITEM_GLOBAL_ALARM);
	lua_register(L, "ACTION_ITEM_KLAXON", l_ACTION_ITEM_KLAXON);
	lua_register(L, "ACTION_ITEM_UNLOCK_DOOR", l_ACTION_ITEM_UNLOCK_DOOR);
	lua_register(L, "ACTION_ITEM_TOGGLE_LOCK", l_ACTION_ITEM_TOGGLE_LOCK);
	lua_register(L, "ACTION_ITEM_UNTRAP_DOOR", l_ACTION_ITEM_UNTRAP_DOOR);
	lua_register(L, "ACTION_ITEM_TOGGLE_PRESSURE_ITEMS", l_ACTION_ITEM_TOGGLE_PRESSURE_ITEMS);
	lua_register(L, "ACTION_ITEM_MUSEUM_ALARM", l_ACTION_ITEM_MUSEUM_ALARM);
	lua_register(L, "ACTION_ITEM_BLOODCAT_ALARM", l_ACTION_ITEM_BLOODCAT_ALARM );
	
	//Sounds
	lua_register(L, "PlayJA2Sample", l_PlayJA2Sample);	
	
	//Tactical
	lua_register(L, "CheckCivGroupHostile", l_fCivGroupHostile);	
	lua_register(L, "gTacticalStatus", l_gTacticalStatus);
	lua_register(L, "CheckCombatMode", l_CheckCombatMode);	
	
	lua_register(L, "GetTacticalStatusFirstID", l_GetFirstID); //new
	lua_register(L, "GetTacticalStatusLastID", l_GetLastID); //new	
	lua_register(L, "TacticalStatusTeamHuman", l_TacticalStatusTeamHuman );
	lua_register(L, "TacticalStatusTeamActive", l_TacticalStatusTeamActive);	
	lua_register(L, "SetPublicNoiseVolume", l_gubPublicNoiseVolume);
	lua_register(L, "SetPublicNoiseGridNo", l_gsPublicNoiseGridNo);

	//MercPtrs
	lua_register(L, "GetPlayerMercID", l_ubID );
	lua_register(L, "GetMapMercSlotID", l_ubID );
	
	lua_register(L, "SetOffPanicBombs", l_SetOffPanicBombs );
	lua_register(L, "WearGasMaskIfAvailable", l_WearGasMaskIfAvailable );	
	lua_register(L, "SetNewSituationMercPtrs", l_SetNewSituationMercPtsr);
	lua_register(L, "SetCivGroupHostile", l_SetCivGroupHostile); 	
	lua_register(L, "EnterTeamCombatMode", l_SetEnterCombatModeTeam);		
	lua_register(L, "DeadMercPtrs", l_AnimMercPtsrSoldierGotHit );	
	lua_register(L, "AnimMercPtrs", l_AnimMercPtsrInSector);
	
	lua_register(L, "CheckMercPtrsInCivilianGroup", l_CheckMercPtrsInCivilianGroup); 
	lua_register(L, "CheckMercPtrsInActive", l_CheckMercPtrsActive); 
	lua_register(L, "CheckMercPtrsInSector", l_CheckMercPtsrInSector); 	
	lua_register(L, "CheckMercPtrsID1SeenID2", l_CheckMercPtsrubIDSeenubID2); 	
	lua_register(L, "MakeMercPtrsHostile", l_MakeMercPtrsHostile);	
	
	//v0.9
	
	lua_register(L, "ChangeMercPtrsTeam", l_ChangeMercPtrsTeam);
	
	lua_register(L, "HandleNPCGotoGridNo", l_HandleNPCGotoGridNo );
	lua_register(L, "HandleNPCDoAction", l_HandleNPCDoAction );
	lua_register(L, "HandleNPCClosePanel", l_HandleNPCClosePanel );
	lua_register(L, "HandleNPCTriggerNPC", l_HandleNPCTriggerNPC );
	
	lua_register(L, "IS_CIV_BODY_TYPE", l_IS_CIV_BODY_TYPE );
	lua_register(L, "SetOffBombsInGridNo", l_SetOffBombsInGridNo );
	lua_register(L, "ActivateSwitchInGridNo", l_ActivateSwitchInGridNo );	
	
	lua_register(L, "UpdateAndDamageSAMIfFound", l_UpdateAndDamageSAMIfFound );
	lua_register(L, "DoesSAMExistHere", l_DoesSAMExistHere );
	lua_register(L, "UpdateSAMDoneRepair", l_UpdateSAMDoneRepair );
	lua_register(L, "ActiveTimedBombExists", l_ActiveTimedBombExists );
	lua_register(L, "RemoveAllActiveTimedBombs", l_RemoveAllActiveTimedBombs );
	
	lua_register(L, "CheckSoldierBodyType", l_CheckSoldierBodyType );
	lua_register(L, "GetSoldierBodyType", l_GetSoldierBodyType );
	lua_register(L, "SetSoldierBodyType", l_SetSoldierBodyType );
	
	lua_register(L, "MercPtsrAIFlags", l_AnimMercPtsrfAIFlags );
	lua_register(L, "MercPtsrNextAction", l_AnimMercPtsrbNextAction );
	lua_register(L, "MercPtsrNextActionData", l_AnimMercPtsrusNextActionData );
	lua_register(L, "MercPtsrAbsoluteFinalDestination", l_AnimMercPtsrsAbsoluteFinalDestination );
	lua_register(L, "MercPtsrStrategicInsertionCode", l_AnimMercPtsrubStrategicInsertionCode );
	lua_register(L, "MercPtsrStrategicInsertionData", l_AnimMercPtsrusStrategicInsertionData );
	
	lua_register(L, "PauseGame", l_PauseGame );
	
	lua_register(L, "SetMusicMode", l_SetMusicMode );
	lua_register(L, "MusicPlay", l_MusicPlay );
	lua_register(L, "MusicSetVolume", l_MusicSetVolume );
	lua_register(L, "MusicGetVolume", l_MusicGetVolume );
	//lua_register(L, "MusicStop", l_MusicStop );
	//lua_register(L, "MusicFadeOut", l_MusicFadeOut );
	//lua_register(L, "MusicFadeIn", l_MusicFadeIn );
	lua_register(L, "SetSoundEffectsVolume", l_SetSoundEffectsVolume );
	lua_register(L, "SetSpeechVolume", l_SetSpeechVolume );
	lua_register(L, "GetSoundEffectsVolume", l_GetSoundEffectsVolume );
	lua_register(L, "GetSpeechVolume", l_GetSpeechVolume );
	
	lua_register(L, "AddTransactionToPlayersBook", l_AddTransactionToPlayersBook );
	lua_register(L, "GetWorldTotalMin", l_GetWorldTotalMin );	
	
	//gGameExternalOptions
	lua_register(L, "SetInvestigateSector", l_SetInvestigateSector );
	lua_register(L, "CheckInvestigateSector", l_CheckInvestigateSector );
	
	lua_register(L, "GetStartingCashNovice", l_GetStartingCashNovice );
	lua_register(L, "GetStartingCashExperienced", l_GetStartingCashExperienced );
	lua_register(L, "GetStartingCashExpert", l_GetStartingCashExpert );
	lua_register(L, "GetStartingCashInsane", l_GetStartingCashInsane );
	
	lua_register(L, "DeleteTalkingMenu", l_DeleteTalkingMenu );

	lua_register(L, "PlayerMercsInSector", l_PlayerMercsInSector);
	lua_register(L, "GetPlayerMercsInSector", l_GetPlayerMercsInSector);
	lua_register(L, "PlayerGroupsInSector", l_PlayerGroupsInSector);
	lua_register(L, "GetPlayerGroupsInSector", l_GetPlayerGroupsInSector);

	lua_register(L, "SetSoldierNonNeutral", l_SetSoldierNonNeutral);
	
	lua_register(L, "CheckSoldierNeutral", l_bNeutral);	
	lua_register(L, "CheckSoldierAttitude", l_bAttitude);		
	
	lua_register(L, "AffectAllTownsLoyaltyByDistanceFrom", l_AffectAllTownsLoyaltyByDistanceFrom);	
	
	lua_register(L, "InitCreatureQuest", l_InitCreatureQuest);	
	
	lua_register(L, "DecrementTownLoyaltyEverywhere", l_DecrementTownLoyaltyEverywhere);

	lua_register(L, "GetSoldierTeam", l_GetSoldierTeam);
	
	lua_register(L, "WhichBuddy", l_WhichBuddy);
	
	
	//Object
	lua_register(L, "SetWorldItemsExists", l_SetgWorldItemsExists);
	lua_register(L, "GetWorldItemsExists", l_gWorldItemsExists);
	lua_register(L, "GetWorldItemsObjectItem", l_gWorldItemsObjectItem);
	lua_register(L, "GetWorldItemsObjectDataMoney", l_gWorldItemsObjectDataMoney);
	
	lua_register(L, "AddHistoryToPlayersLog", l_AddHistoryToPlayersLog);
	lua_register(L, "AddFutureDayStrategicEvent", l_AddFutureDayStrategicEvent);	

	lua_register(L, "GetNPCBalance", l_GetiBalance);
	
	lua_register(L, "AddCreateItemToPool", l_CreateItemToPool);
	lua_register(L, "AddCreateItemsToUnLoadedSector", l_CreateToUnLoadedSector);
	
	lua_register(L, "GetCharacterSectorZ", l_GetCharacterSectorZ);
	lua_register(L, "GetCharacterSectorX", l_GetCharacterSectorX);
	lua_register(L, "GetCharacterSectorY", l_GetCharacterSectorY);	
	
	lua_register(L, "AddSameDayStrategicEvent", l_AddSameDayStrategicEvent);

	//lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	
	lua_register(L, "MoveItemPools", l_MoveItemPools);
	
	
	lua_register(L, "GetNumberOfWorldItemsFromTempItemFile", l_GetNumberOfWorldItemsFromTempItemFile);
	
	lua_register(L, "gubFact", l_gubFact);
	
	lua_register(L, "GetgubFact", l_GetgubFact);
	
	lua_register(L, "GetTacticalStatusEnemyInSector", l_GetTacticalStatusEnemyInSector);
	
	lua_register(L, "CountNumberOfBobbyPurchasesThatAreInTransit", l_CountNumberOfBobbyPurchasesThatAreInTransit);	

	lua_register(L, "CheckForNewShipment", l_CheckForNewShipment);	
	
	lua_register(L, "NumMalesPresent",l_NumMalesPresent);
	lua_register(L, "NumWoundedMercsNearby",l_NumWoundedMercsNearby);
	lua_register(L, "NumMercsNear",l_NumMercsNear);
	lua_register(L, "CheckTalkerStrong",l_CheckTalkerStrong);	
	lua_register(L, "CheckTalkerFemale",l_CheckTalkerFemale);	
	
	lua_register(L, "CheckCharacterSectorZ", l_CheckCharacterSectorZ);
	
	lua_register(L, "IssueHeadMinerQuote", l_IssueHeadMinerQuote);
	lua_register(L, "GetHeadMinersMineIndex", l_GetHeadMinersMineIndex);
	lua_register(L, "PlayerSpokeToHeadMiner", l_PlayerSpokeToHeadMiner);
	lua_register(L, "IsHisMineRunningOut", l_IsHisMineRunningOut);
	lua_register(L, "IsHisMineEmpty", l_IsHisMineEmpty);
	lua_register(L, "IsHisMineDisloyal", l_IsHisMineDisloyal);
	lua_register(L, "IsHisMineInfested", l_IsHisMineInfested);
	lua_register(L, "IsHisMineLostAndRegained", l_IsHisMineLostAndRegained);
	lua_register(L, "IsHisMineAtMaxProduction", l_IsHisMineAtMaxProduction);
	lua_register(L, "ResetQueenRetookMine", l_ResetQueenRetookMine);
	
	
	lua_register(L, "GetFirstArrivalDelay", l_GetiFirstArrivalDelay);
	lua_register(L, "GetDefaultArrivalSectorX", l_GetubDefaultArrivalSectorX);
	lua_register(L, "GetDefaultArrivalSectorY", l_GetubDefaultArrivalSectorY);
	
	lua_register(L, "SetFirstArrivalDelay", l_SetiFirstArrivalDelay);
	lua_register(L, "SetDefaultArrivalSectorX", l_SetubDefaultArrivalSectorX);
	lua_register(L, "SetDefaultArrivalSectorY", l_SetubDefaultArrivalSectorY);
	
	lua_register(L, "ResetHistoryFact", l_ResetHistoryFact);
	lua_register(L, "SetHistoryFact", l_SetHistoryFact);
	
	lua_register(L, "TacticalCharacterDialogue", l_TacticalCharacterDialogue);
	
	lua_register(L, "DeleteMercInventory", l_DeleteMercInventory);
	
	lua_register(L, "DeleteSlotMercInventory", l_DeleteMercInventory);
	

	lua_register(L, "IsMercDead", l_IsMercDead);
	lua_register(L, "IsMercHireable", l_IsMercHireable);
	lua_register(L, "NumberOfMercsOnPlayerTeam", l_NumberOfMercsOnPlayerTeam);
	
	lua_register(L, "GetMercArrivalTimeOfDay", l_GetMercArrivalTimeOfDay);
	lua_register(L, "StrategicPythSpacesAway", l_StrategicPythSpacesAway);
	
	//-----------------------------------------------
	lua_register(L, "SectorEnemy", l_SectorEnemy);
	
	lua_register(L, "ItemExistsAtLocation", l_ItemExistsAtLocation);
	
	lua_register(L, "SetMercStatus", l_Merc_Status);
	
	lua_register(L, "GetMercStatus", l_Get_Merc_Status);	
	
	lua_register(L, "SetNPCLife", l_SetbLife);	
	lua_register(L, "GetNPCLifeMax", l_GetbLifeMax);	
	
	lua_register(L, "HasHisMineBeenProducingForPlayerForSomeTime", l_HasHisMineBeenProducingForPlayerForSomeTime);	

	lua_register(L, "AnotherFightPossible", l_AnotherFightPossible);	
	
	lua_register(L, "PredictDailyIncomeFromAMine", l_PredictDailyIncomeFromAMine);	
	
	//brak w faq
	lua_register(L, "SetGroupSectorValue", l_SetGroupSectorValue);	
	lua_register(L, "SetGroupNextSectorValue", l_SetGroupNextSectorValue);		
	
	lua_register(L, "FadeOutGameScreen", l_FadeOutGameScreen);	
	
	lua_register(L, "FadeInGameScreen", l_FadeInGameScreen);

	lua_register(L, "SetCurrentWorldSector", l_SetCurrentWorldSector);	
	
	lua_register(L, "EnterSector", l_EnterSector);
	
	lua_register(L, "gubQuest", l_gubQuest);
	
	lua_register(L, "GetTownIdForSector", l_GetTownIdForSector);
	lua_register(L, "UpdateMercsInSector", l_UpdateMercsInSector);
	lua_register(L, "CheckIfEntireTownHasBeenLiberated", l_CheckIfEntireTownHasBeenLiberated);
	lua_register(L, "CheckIfEntireTownHasBeenLost", l_CheckIfEntireTownHasBeenLost);
	lua_register(L, "DidFirstBattleTakePlaceInThisTown", l_DidFirstBattleTakePlaceInThisTown);
	lua_register(L, "SetTheFirstBattleSector", l_SetTheFirstBattleSector);
	lua_register(L, "AdjustLoyaltyForCivsEatenByMonsters", l_AdjustLoyaltyForCivsEatenByMonsters);
	lua_register(L, "IsTownUnderCompleteControlByPlayer", l_IsTownUnderCompleteControlByPlayer);
	lua_register(L, "GetNumberOfWholeTownsUnderControl", l_GetNumberOfWholeTownsUnderControl);
	
	lua_register(L, "GetCharacterTotalDaysServed", l_GetusTotalDaysServed);
	lua_register(L, "SetCharacterTotalDaysServed", l_SetusTotalDaysServed);
	
	lua_register(L, "GetTownLoyaltyLiberatedAlready", l_GetTownLoyaltyfLiberatedAlready);
	lua_register(L, "GetTownLoyaltyStarted", l_GetTownLoyaltyfStarted);
	lua_register(L, "GetTownLoyaltyRating", l_GetTownLoyaltyubRating);
	lua_register(L, "GetTownLoyaltyChange", l_GetTownLoyaltysChange);

	lua_register(L, "SetTownLoyaltyLiberatedAlready", l_SetTownLoyaltyfLiberatedAlready);
	lua_register(L, "SetTownLoyaltyStarted", l_SetTownLoyaltyfStarted);
	lua_register(L, "SetTownLoyaltyRating", l_SetTownLoyaltyubRating);
	lua_register(L, "SetTownLoyaltyChange", l_SetTownLoyaltysChange);	

	lua_register(L, "GetTownUsesLoyalty", l_GetgfTownUsesLoyalty);	
	lua_register(L, "SetTownUsesLoyalty", l_SetgfTownUsesLoyalty);

	lua_register(L, "GetCharacterTown", l_GetgMercProfilesbTown);
	lua_register(L, "SetCharacterTown", l_SetgMercProfilesbTown);
	
	lua_register(L, "GetTimeQuestWasStarted", l_GetTimeQuestWasStarted);
	
	lua_register(L, "RepairmanIsFixingItemsButNoneAreDoneYet", l_RepairmanIsFixingItemsButNoneAreDoneYet);	

	lua_register(L, "SECTOR", l_SECTOR);
	lua_register(L, "GetSectorInfoBloodCats", l_SectorInfoBloodCats);	
	
	lua_register(L, "SpokenToHeadMiner", l_SpokenToHeadMiner);	
	lua_register(L, "GetIdOfMineForSector", l_GetIdOfMineForSector);	

	lua_register(L, "CALCULATE_STRATEGIC_INDEX", l_CALCULATE_STRATEGIC_INDEX);	

	lua_register(L, "CheckTalkerUnpropositionedFemale", l_CheckTalkerUnpropositionedFemale);	
	
	lua_register(L, "BoxerExists", l_BoxerExists);
	lua_register(L, "CurrentPlayerProgressPercentage", l_CurrentPlayerProgressPercentage);
	
	lua_register(L, "UnderGroundSectorVisited", l_UnderGroundSectorVisited);	
	
	lua_register(L, "SetTurnTimeLimit", l_fTurnTimeLimit);		

	lua_register(L, "InitMercFace", l_InitFace);
	
	
#ifdef JA2UB	
	//john
	lua_register(L, "Ja25JohnKulbaIsInGame", l_Ja25SaveStructJohnKulbaIsInGame);
	lua_register(L, "Ja25CheckJohnKulbaIsInGame", l_Ja25SaveCheckStructJohnKulbaIsInGame);
	lua_register(L, "Ja25JohnKulbaInitialSectorY", l_Ja25SaveStructubJohnKulbaInitialSectorY);
	lua_register(L, "Ja25JohnKulbaInitialSectorX", l_Ja25SaveStructubJohnKulbaInitialSectorX);	
	
	lua_register(L, "SetNumberJa25EnemiesInSurfaceSector", l_SetNumberJa25EnemiesInSurfaceSector);	
	
	lua_register(L, "SetNumberOfJa25BloodCatsInSector", l_SetNumberOfJa25BloodCatsInSector);

	lua_register(L, "HasNpcSaidQuoteBefore", l_HasNpcSaidQuoteBefore);	
	
	lua_register(L, "ShouldThePlayerStopWhenWalkingOnBiggensActionItem", l_ShouldThePlayerStopWhenWalkingOnBiggensActionItem);	
	
	lua_register(L, "HandleSeeingPowerGenFan", l_HandleSeeingPowerGenFan);	
	lua_register(L, "HandleSwitchToOpenFortifiedDoor", l_HandleSwitchToOpenFortifiedDoor);	
	lua_register(L, "HandleSeeingFortifiedDoor", l_HandleSeeingFortifiedDoor);	
	lua_register(L, "HandlePlayerHittingSwitchToLaunchMissles", l_HandlePlayerHittingSwitchToLaunchMissles);
	lua_register(L, "HavePersonAtGridnoStop", l_HavePersonAtGridnoStop);
	
#endif	

	lua_register(L, "WhoIsThere2", l_WhoIsThere2);	
	
	lua_register(L, "WhoIs", l_WhoIs);	

	//Shop
	lua_register(L, "GuaranteeAtLeastXItemsOfIndex", l_GuaranteeAtLeastXItemsOfIndex);	
	
	lua_register(L, "MercProfileSetBIGPOCK2POS", l_gMercProfileGearset);
	lua_register(L, "MercProfileSetPOCKPOS", l_gMercProfileGearset);		
	
	lua_register(L, "FindUnderGroundSector", l_FindUnderGroundSector);
	lua_register(L, "AddEnemyToUnderGroundSector", l_AddEnemyToUnderGroundSector);
	lua_register(L, "FindUnderGroundSectorVisited", l_FindUnderGroundSectorVisited);
	
	lua_register(L, "SetCurrentWorldSector", l_SetCurrentWorldSector);	

#ifdef JA2UB
	lua_register(L, "EnterTacticalInFinalSector", l_EnterTacticalInFinalSector);	
#endif

	lua_register(L, "ReStartingGame", l_ReStartingGame);
	
	lua_register(L, "AddCustomEmail", l_AddCustomEmail);

	//intro
	lua_register(L, "DisplaySirtechSplashScreen", l_DisplaySirtechSplashScreen);
	
	lua_register(L, "SetEnteringMapScreen", l_gfEnteringMapScreen);
	lua_register(L, "SetIntroScreenExit", l_gfIntroScreenExit);
	lua_register(L, "SetDoneWithSplashScreen", l_gfDoneWithSplashScreen);
	lua_register(L, "SetIntroExitScreen", l_guiIntroExitScreen);	
	lua_register(L, "SetIntroScreenMode", l_gbIntroScreenMode);	

	lua_register(L, "SetVideo", l_iStringToUse);	
	lua_register(L, "StopVideo", l_StopVideo);	
	lua_register(L, "StartVideo", l_StartVideo);	

	lua_register(L, "SetGlobalLoyaltyEvent", l_SetHandleGlobalLoyaltyEvent);	
	
	lua_register(L, "SetDefaultArrivalSector", l_SetDefaultArrivalSector);	
	lua_register(L, "GetDefaultArrivalSector", l_GetDefaultArrivalSector);
	lua_register(L, "SetDefaultArrivalGridNo", l_SetMercArrivalLocation);
	lua_register(L, "GetDefaultArrivalSectorX", l_GetDefaultArrivalSectorX);
	lua_register(L, "GetDefaultArrivalSectorY", l_GetDefaultArrivalSectorY);
	
	lua_register(L, "InitialProfile", l_InitProfile);
	
	#ifdef JA2UB
	lua_register(L, "InitialHeliGridNo1", l_InitMercgridNo0);
	lua_register(L, "InitialHeliGridNo2", l_InitMercgridNo1);
	lua_register(L, "InitialHeliGridNo3", l_InitMercgridNo2);
	lua_register(L, "InitialHeliGridNo4", l_InitMercgridNo3);
	lua_register(L, "InitialHeliGridNo5", l_InitMercgridNo4);
	lua_register(L, "InitialHeliGridNo6", l_InitMercgridNo5);
	lua_register(L, "InitialHeliGridNo7", l_InitMercgridNo6);
	
	lua_register(L, "InitialJerryGridNo", l_InitJerryGridNo);
	
	lua_register(L, "InitialLaptopQuest", l_setLaptopQuest);
	
	lua_register(L, "InitialHeliCrash", l_setInGameHeliCrash );
	lua_register(L, "InitialJerryQuotes", l_setJerryQuotes );
	lua_register(L, "InitialJerry", l_setInJerry );
	lua_register(L, "InitialHeli", l_setInGameHeli );
	
	lua_register(L, "InternalLocateGridNo", l_SetInternalLocateGridNo );
	#endif
	
	//lua_register(L, "GetINITIALHELIGRIDNO1", l_getMercgridNo0);
	//lua_register(L, "GetINITIALHELIGRIDNO2", l_getMercgridNo1);
	//lua_register(L, "GetINITIALHELIGRIDNO3", l_getMercgridNo2);
	//lua_register(L, "GetINITIALHELIGRIDNO4", l_getMercgridNo3);
	//lua_register(L, "GetINITIALHELIGRIDNO5", l_getMercgridNo4);
	//lua_register(L, "GetINITIALHELIGRIDNO6", l_getMercgridNo5);
	//lua_register(L, "GetINITIALHELIGRIDNO7", l_getMercgridNo6);
	
	lua_register(L, "ExecuteTacticalTextBox", l_ExecuteTacticalTextBox);

	//Town function
	lua_register(L, "VisibleTown", l_VisibleTown);
	lua_register(L, "HiddenTown", l_HiddenTown);
	//lua_register(L, "EraseTown", l_EraseTown);
	//lua_register(L, "ResizeTown", l_ResizeTown);
	
	lua_register(L, "gubBoxerID", l_gubBoxerID);
	
	lua_register(L, "RemoveGraphicFromTempFile", l_RemoveGraphicFromTempFile);
	

}



//------------------- intro -----------

BOOLEAN LuaIntro(UINT8 Init, UINT32 uiCurrentVideo, INT8 bIntroType, UINT32 iStringToUse )
{
	const char* filename = "scripts\\Intro.lua";

	LuaScopeState _LS(true);

	IniFunction( _LS.L() );
	IniGlobalGameSetting( _LS.L() );

	SGP_THROW_IFFALSE( _LS.L.EvalFile(filename), _BS("Cannot open file: ") << filename << _BS::cget );

	if ( Init == 0 )
	{
		LuaFunction(_LS.L, "PrepareToExitIntroScreen" ).Call(0);
	}
	
	if ( Init == 1 )
	{
		LuaFunction(_LS.L, "GetNextIntroVideo" ).Param(uiCurrentVideo).Call(0);
	}
	
	if ( Init == 2 )
	{
		LuaFunction(_LS.L, "SetIntroType" ).Param<int>(bIntroType).Call(0);
	}

	return true;
}

static int l_DisplaySirtechSplashScreen(lua_State *L)
{

	DisplaySirtechSplashScreen ();
	
	return 0;
}

static int l_gfIntroScreenExit(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
BOOLEAN BOOL;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) BOOL = lua_toboolean(L,i);
	}	
		
		gfIntroScreenExit = BOOL;
	return 0;
}

static int l_gfDoneWithSplashScreen(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
BOOLEAN BOOL;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) BOOL = lua_toboolean(L,i);
	}	
		
		gfDoneWithSplashScreen = BOOL;
	return 0;
}

static int l_guiIntroExitScreen(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 VAL;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) VAL = lua_tointeger(L,i);
	}	
		
		guiIntroExitScreen = VAL;
	return 0;
}

static int l_gbIntroScreenMode(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 VAL;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) VAL = lua_tointeger(L,i);
	}	
		
		gbIntroScreenMode = VAL;
	return 0;
}

static int l_iStringToUse(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 VAL;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) VAL = lua_tointeger(L,i);
	}	
		
		iStringToUseLua = VAL;
	return 0;
}

static int l_gfEnteringMapScreen(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
BOOLEAN BOOL;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) BOOL = lua_toboolean(L,i);
	}	
		
		gfEnteringMapScreen = BOOL;
	return 0;
}

static int l_StopVideo(lua_State *L)
{
	StopIntroVideo();
		Test = 1;
	return 0;
}

static int l_StartVideo(lua_State *L)
{

		Test = 0;
	return 0;
}

//------------------- End intro -----------
#ifdef JA2UB
static int l_EnterTacticalInFinalSector(lua_State *L)
{

	EnterTacticalInFinalSector ();
	
	return 0;
}
#endif

static int l_ReStartingGame(lua_State *L)
{

	ReStartingGame ();
	
	return 0;
}

#ifdef JA2UB
BOOLEAN LetLuaMakeBadSectorListFromMapsOnHardDrive(UINT8 Init)
{
	char * filename = "scripts\\MakeMapsOnHardDrive.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "MakeBadSectorListFromMapsOnHardDrive");
		lua_call(L,0,0); 
	}

	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LuaInitStrategicLayer(UINT8 Init)
{
	char * filename = "scripts\\InitStrategicLayer.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "InitStrategicLayer");
		lua_call(L,0,0); 
	}
	
	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

#endif

static int l_SetHandleGlobalLoyaltyEvent (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubEventType;
INT16 sSectorX;
INT16 sSectorY;
INT8 bSectorZ;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubEventType = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
		if (i == 4 ) bSectorZ = lua_tointeger(L,i);
	}

	HandleGlobalLoyaltyEvent( ubEventType, sSectorX, sSectorY, bSectorZ);
	
return 0;
}

static int l_AddCustomEmail (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT32 iMessageOffset;
INT32 iMessageLength;
UINT8 ubSender;
//INT32 iCurrentIMPPosition;
INT16 iCurrentShipmentDestinationID = -1;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) iMessageOffset = lua_tointeger(L,i);
		if (i == 2 ) iMessageLength = lua_tointeger(L,i);
		if (i == 3 ) ubSender = lua_tointeger(L,i);
		//if (i == 4 ) iDate = lua_tointeger(L,i);
		//if (i == 4 ) iCurrentIMPPosition = lua_tointeger(L,i);
		//if (i == 5) iCurrentShipmentDestinationID = lua_tointeger(L,i);
	}

	AddCustomEmail(iMessageOffset,iMessageLength,ubSender,	GetWorldTotalMin(), -1, -1, TYPE_EMAIL_OTHER);	
	
return 0;
}

static int l_gMercProfileGearset(lua_State *L)
{
UINT8 n = lua_gettop(L);
UINT8 ID;
UINT16 inv;
UINT8 status;
UINT8 number;
UINT8 drop;
UINT8 slot;
UINT8 i1;
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 3 ) i1 = lua_tointeger(L,i);
		if (i == 4 ) inv = lua_tointeger(L,i);
		if (i == 5 ) number = lua_tointeger(L,i);
		if (i == 6 ) status = lua_tointeger(L,i);
		if (i == 7 ) drop = lua_tointeger(L,i);
		if (i == 8 ) slot = lua_tointeger(L,i);
	}

		
gMercProfileGear[ID][i1].inv[slot] = inv;
gMercProfileGear[ID][i1].iNumber[slot] = number;
gMercProfileGear[ID][i1].iStatus[slot] = status;
gMercProfileGear[ID][i1].iDrop[slot] = drop;
	
	return 0;
}

static int l_GuaranteeAtLeastXItemsOfIndex(lua_State *L)
{
UINT8 n = lua_gettop(L);
UINT8 ShopID;
UINT16 ItemsID;
UINT32 VAL;
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ShopID = lua_tointeger(L,i);
		if (i == 2 ) ItemsID = lua_tointeger(L,i);
		if (i == 3 ) VAL = lua_tointeger(L,i);
	}

		
	GuaranteeAtLeastXItemsOfIndex( ShopID, ItemsID, VAL );
	
	return 0;
}
	
static int l_FindUnderGroundSectorVisited(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 sSectorX;
UINT8 sSectorY;
UINT8 bLevel;
UNDERGROUND_SECTORINFO *pSector;
BOOLEAN bol;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) bLevel = lua_tointeger(L,i);
	}

				pSector = FindUnderGroundSector( sSectorX, sSectorY, bLevel );

			if ( pSector )
			{
				if ( pSector->fVisited )
				{
					bol = TRUE;
				}
				else
				{
					bol = FALSE;
				}
			}
						
	lua_pushboolean(L, bol);
	
return 1;
}

static int l_FindUnderGroundSector(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 sSectorX;
UINT8 sSectorY;
UINT8 bLevel;
UNDERGROUND_SECTORINFO *pSector;
BOOLEAN bol;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) bLevel = lua_tointeger(L,i);
	}

				pSector = FindUnderGroundSector( sSectorX, sSectorY, bLevel );
				if ( pSector )
				{
					bol = TRUE; //ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
				}
				else
				{
					bol = FALSE; //pSector = NewUndergroundNode( (UINT8)iCol, (UINT8)iRow, bLevel );
				}
				
	lua_pushboolean(L, bol);
	
return 1;
}

static int l_AddEnemyToUnderGroundSector(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 sSectorX;
UINT8 sSectorY;
UNDERGROUND_SECTORINFO *pSector;
UINT8 bLevel;
UINT8 ubNumAdmins;
UINT8 ubNumTroops;
UINT8 ubNumElites;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) bLevel = lua_tointeger(L,i);
		if (i == 4 ) ubNumAdmins = lua_tointeger(L,i);
		if (i == 5 ) ubNumTroops = lua_tointeger(L,i);
		if (i == 6 ) ubNumElites = lua_tointeger(L,i);
		
	}

				pSector = FindUnderGroundSector( sSectorX, sSectorY, bLevel );
				pSector->ubNumAdmins = ubNumAdmins;
				pSector->ubNumTroops = ubNumTroops;
				pSector->ubNumElites = ubNumElites;
				
return 0;
}

static int l_WhoIs (lua_State *L)
{
UINT8 n = lua_gettop(L);
int i = 0;
INT8 bLevel = 0;
UINT8 Val = 0,Prof = 0, whois = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if ( i == 1 ) whois = lua_tointeger(L,i);
		if ( i == 2 ) Val = lua_tointeger(L,i);
	}	
	
	if ( whois == 1 ) 
		Prof = gProfilesAIM[ Val ].ProfilId;
	else if ( whois == 2 ) 
		Prof = gProfilesMERC[ Val ].ProfilId;
	else if ( whois == 3 ) 
		Prof = gProfilesRPC[ Val ].ProfilId;
	else if ( whois == 4 ) 
		Prof = gProfilesNPC[ Val ].ProfilId;
	else if ( whois == 5 ) 
		Prof = gProfilesVehicle[ Val ].ProfilId;
	else if ( whois == 6 ) 
		Prof = gProfilesIMP[ Val ].ProfilId;
	else			
		Prof = 0;
			
		lua_pushinteger(L, Prof);
			
	return 1;
}

#ifdef JA2UB
static int l_HandlePlayerHittingSwitchToLaunchMissles (lua_State *L)
{	

	HandlePlayerHittingSwitchToLaunchMissles( );
			
	return 0;
}

static int l_HavePersonAtGridnoStop(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT32 sGridNo = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}	
		
		HavePersonAtGridnoStop( sGridNo );
			
	return 0;
}

static int l_HandleSeeingPowerGenFan (lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
INT32 sGridNo = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}	
		
		HandleSeeingPowerGenFan( sGridNo );
			
	return 0;
}

static int l_HandleSwitchToOpenFortifiedDoor (lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
INT32 sGridNo = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}	
		
		HandleSwitchToOpenFortifiedDoor( sGridNo );
			
	return 0;
}

static int l_HandleSeeingFortifiedDoor (lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
INT32 sGridNo = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}	
		
		HandleSeeingFortifiedDoor( sGridNo );
			
	return 0;
}

static int l_ShouldThePlayerStopWhenWalkingOnBiggensActionItem (lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 ubRecordNum = 0;
BOOLEAN Bool;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubRecordNum = lua_tointeger(L,i);
	}	
		
		Bool = ShouldThePlayerStopWhenWalkingOnBiggensActionItem( ubRecordNum );
		
		lua_pushboolean(L, Bool);
			
	return 1;
}

static int l_HasNpcSaidQuoteBefore (lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT8 ubNPC = 0;
UINT8 ubRecord = 0;
BOOLEAN Bool;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubNPC = lua_tointeger(L,i);
		if (i == 2 ) ubRecord = lua_tointeger(L,i);
	}	
		
		Bool = HasNpcSaidQuoteBefore( ubNPC, ubRecord );
		
			lua_pushboolean(L, Bool);
			
	return 1;
}

static int l_SetNumberOfJa25BloodCatsInSector(lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
INT8	bNumBloodCats=0;
INT8	bBloodCatPlacements=0;	
INT16 sSectorX;
INT16 sSectorY;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) bNumBloodCats = lua_tointeger(L,i);
		if (i == 4 ) bBloodCatPlacements = lua_tointeger(L,i);
	}	
		
		SectorInfo[ SECTOR( sSectorX, sSectorY ) ].bBloodCatPlacements = bBloodCatPlacements;
		SectorInfo[ SECTOR( sSectorX, sSectorY ) ].bBloodCats = bNumBloodCats;
	return 0;
}

static int l_SetNumberJa25EnemiesInSurfaceSector(lua_State *L)
{
UINT8 n = lua_gettop(L);

int i;
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	
INT16 sSectorX;
INT16 sSectorY;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) ubNumAdmins = lua_tointeger(L,i);
		if (i == 4 ) ubNumTroops = lua_tointeger(L,i);
		if (i == 5 ) ubNumElites = lua_tointeger(L,i);
	}	
		SetNumberJa25EnemiesInSurfaceSector( SECTOR( sSectorX, sSectorY ), ubNumAdmins, ubNumTroops, ubNumElites );
	return 0;
}

static int l_Ja25SaveStructubJohnKulbaInitialSectorY(lua_State *L)
{
UINT32 Y;
UINT8 n = lua_gettop(L);

	 Y = lua_tointeger(L,n);

	gJa25SaveStruct.ubJohnKulbaInitialSectorY = Y;
	return 0;
}

static int l_Ja25SaveStructubJohnKulbaInitialSectorX(lua_State *L)
{
UINT32 X;
UINT8 n = lua_gettop(L);

	 X = lua_tointeger(L,n);

	gJa25SaveStruct.ubJohnKulbaInitialSectorX = X;
	return 0;
}

//gJa25SaveStruct.fJohnKulbaIsInGame
static int l_Ja25SaveCheckStructJohnKulbaIsInGame(lua_State *L)
{
BOOLEAN FactFalse;
UINT8 n = lua_gettop(L);

	FactFalse = gJa25SaveStruct.fJohnKulbaIsInGame;
	
	lua_pushboolean(L, FactFalse);

	return 1;
}


//gJa25SaveStruct.fJohnKulbaIsInGame
static int l_Ja25SaveStructJohnKulbaIsInGame(lua_State *L)
{
BOOLEAN FactFalse;
UINT8 n = lua_gettop(L);

	 FactFalse = lua_toboolean(L,n);

	gJa25SaveStruct.fJohnKulbaIsInGame = FactFalse;
	return 0;
}

#endif

//-------------- End UB ----------------

static int l_WhoIsThere2 (lua_State *L)
{
UINT8 n = lua_gettop(L);
int i;
UINT32 sGridNo;
INT8 bLevel = 0;
UINT8 Val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
		if (i == 2 ) bLevel = lua_tointeger(L,i);
	}	
		
		Val = WhoIsThere2( sGridNo, bLevel );
		
		lua_pushinteger(L, Val);
			
	return 1;
}

//set merc
static int l_SetMercProfiles(lua_State *L)
{
MERCPROFILESTRUCT * pProfile;
UINT8 idNPC = -1;
UINT16 x = -1;
UINT16 y = -1;
UINT8 z = -1;
UINT32 Orders = -1;
UINT16 usEyesX = -1;
UINT16 usEyesY = -1;
UINT16 usMouthX = -1;
UINT16 usMouthY = -1;
UINT32 uiEyeDelay = -1;
UINT32 uiMouthDelay = -1;
UINT32 uiWeeklySalary = -1;
UINT32 uiBiWeeklySalary = -1;
INT16  sSalary = -1;
UINT8  ubBodyType = -1;

		idNPC = lh_getIntegerFromTable(L, "IDProfile");

		if ( idNPC > -1 ) 
		{
		pProfile = &(gMercProfiles[ idNPC ]);
		
			x = lh_getIntegerFromTable(L, "SectorX");
			y = lh_getIntegerFromTable(L, "SectorY");
			z = lh_getIntegerFromTable(L, "SectorZ");
			
			usEyesX = lh_getIntegerFromTable(L, "EyesX");
			usEyesY = lh_getIntegerFromTable(L, "EyesY");
			usMouthX = lh_getIntegerFromTable(L, "MouthX");
			usMouthY = lh_getIntegerFromTable(L, "MouthY");
			uiEyeDelay = lh_getIntegerFromTable(L, "EyeDelay");
			uiMouthDelay = lh_getIntegerFromTable(L, "MouthDelay");
			
			uiWeeklySalary = lh_getIntegerFromTable(L, "WeeklySalary");
			uiBiWeeklySalary = lh_getIntegerFromTable(L, "BiWeeklySalary");
			sSalary = lh_getIntegerFromTable(L, "Salary");	
			ubBodyType = lh_getIntegerFromTable(L, "BodyType");	

		
			if ((x != -1 )&& (x >= 1 || x <= 16) )
				pProfile->sSectorX = x;
				
			if((y != -1) && (y >= 1 || y <= 16) )
				pProfile->sSectorY = y;
			
			if ((z != -1) && (z >= 0 || z <= 3) )
				pProfile->bSectorZ = z;
				
			if ( usEyesX != -1 )
				pProfile->usEyesX = usEyesX;
				
			if ( usEyesY != -1 )
				pProfile->usEyesY = usEyesY;
				
			if ( usMouthX != -1 )
				pProfile->usMouthX = usMouthX;
				
			if ( usMouthY != -1 )
				pProfile->usMouthY = usMouthY;
				
			if ( uiEyeDelay != -1 )
				pProfile->uiEyeDelay = uiEyeDelay;
				
			if ( uiMouthDelay != -1 )
				pProfile->uiMouthDelay = uiMouthDelay;
				
			if ( uiWeeklySalary != -1 )
				pProfile->uiWeeklySalary = uiWeeklySalary;
		
			if ( uiBiWeeklySalary != -1 )
				pProfile->uiBiWeeklySalary = uiBiWeeklySalary;
				
			if ( sSalary != -1 )
				pProfile->sSalary = sSalary;

			if ( ubBodyType != -1 )
				pProfile->ubBodyType = ubBodyType;			
		}
	
		
	return 0;
}
/*
BOOLEAN LuaCheckFact ( UINT16 usFact, UINT8 ubProfileID , UINT32 Init )
{
	char * filename = "scripts\\Quests.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "CheckFact");
		lua_pushnumber (L, usFact );
		lua_pushnumber (L, ubProfileID );		
		lua_call(L,2,0); 
	}
	
	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LuaCheckForKingpinsMoneyMissing( BOOLEAN fFirstCheck, UINT8 Init)
{
	char * filename = "scripts\\strategicmap.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "CheckForKingpinsMoneyMissing");
		lua_pushboolean (L, fFirstCheck );
		lua_call(L,1,0); 
	}
	
	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LuaHandleDelayedItemsArrival( UINT32 uiReason, UINT8 Init)
{
	char * filename = "scripts\\strategicmap.lua";
	UINT32 size, bytesRead;     
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}

	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleDelayedItemsArrival");
		lua_pushnumber (L, uiReason );
		lua_call(L,1,0); 
	}
	
	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LuaHandleQuestCodeOnSectorEntry( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init)
{
	char * filename = "scripts\\strategicmap.lua";
	UINT32 size, bytesRead;     
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}

	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleQuestCodeOnSectorEntry");
		lua_pushnumber (L, sSectorX );
		lua_pushnumber (L, sSectorY );
		lua_pushnumber (L, bSectorZ );
		lua_call(L,3,0); 
	}
	
	lua_close(L);

	delete[] buffer;
	
	
	return true;

}
*/
BOOLEAN LuaHandleQuestCodeOnSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 Init)
{
	char * filename = "scripts\\strategicmap.lua";
	UINT32 size, bytesRead;     
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}

	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleQuestCodeOnSectorExit");
		lua_pushnumber (L, sSectorX );
		lua_pushnumber (L, sSectorY );
		lua_pushnumber (L, bSectorZ );
		lua_call(L,3,0); 
	}
	
	if ( Init == 1 )
	{
		lua_getglobal(L , "HandleQuestCodeOnSectorEntry");
		lua_pushnumber (L, sSectorX );
		lua_pushnumber (L, sSectorY );
		lua_pushnumber (L, bSectorZ );
		lua_call(L,3,0); 
	}
	
	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LetLuaGameInit(UINT8 Init)
{
	char * filename = "scripts\\GameInit.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);
		SGP_THROW(msg);
	}

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
//	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "InitNewGame");
		lua_call(L,0,0); 
	}
	
	if ( Init == 1 )
	{
		lua_getglobal(L , "InitNPCs");
		lua_call(L,0,0); 
	}
	
	if ( Init == 2 )
	{
		lua_getglobal(L , "InitStrategicLayer");
		lua_call(L,0,0); 
	}	
	
	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LuaHandleNPCTeamMemberDeath(UINT8 ProfileId, UINT8 Init)
{
	char * filename = "scripts\\Overhead.lua";
	UINT32 size, bytesRead;
	char* buffer;
	
	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleNPCTeamMemberDeath");
		lua_pushnumber (L, ProfileId );
		lua_call(L,1,0); 
	}

	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LuaHandlePlayerTeamMemberDeath(UINT8 ProfileId, UINT8 Init)
{
	char * filename = "scripts\\Overhead.lua";
	UINT32 size, bytesRead;
	char* buffer;
	
	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandlePlayerTeamMemberDeath");
		lua_pushnumber (L, ProfileId );
		lua_call(L,1,0); 
	}

	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LetLuaMyCustomHandleAtNewGridNo(UINT8 bNewSide, UINT8 ProfileId, UINT8 Init)
{
	char * filename = "scripts\\Overhead.lua";
	UINT32 size, bytesRead;
	char* buffer;
	
	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);
		SGP_THROW(msg);
	}

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
//	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
//	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{

		IniGlobal_0(L);
		lua_getglobal(L , "HandleAtNewGridNo");
		
		lua_pushnumber (L, ProfileId );
		
		lua_call(L,1,0); 
	}
	
	if ( Init == 1 )
	{
		IniGlobal_1(L);
		lua_getglobal(L , "MakeCivHostile");
		
		lua_pushnumber (L, bNewSide );
		
		lua_call(L,1,0); 
	}

	lua_close(L);

	delete[] buffer;
	
	
	return true;

}


SOLDIERTYPE * FindSoldierByProfileID_( UINT8 ubProfileID )
{
	UINT8 ubLoop, ubLoopLimit;
	SOLDIERTYPE * pSoldier;

		ubLoopLimit = MAX_NUM_SOLDIERS;
	
	for (ubLoop = 0, pSoldier = MercPtrs[0]; ubLoop < ubLoopLimit; ubLoop++, pSoldier++)
	{
			if (pSoldier->bActive && pSoldier->ubProfile == ubProfileID)
			{
			return( pSoldier );
			}
	}
	return( NULL );
}

SOLDIERTYPE * FindSoldierByProfileID2( UINT8 ubProfileID, BOOLEAN fPlayerMercsOnly )
{
	UINT8 cnt2, ubLoopLimit;
	SOLDIERTYPE * pSoldier;

		ubLoopLimit = gTacticalStatus.Team[CIV_TEAM].bLastID;

	cnt2 = gTacticalStatus.Team[ CIV_TEAM ].bFirstID;
  for ( pSoldier = MercPtrs[ cnt2 ]; cnt2 <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; cnt2++ ,pSoldier++)
	{
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->ubCivilianGroup == 17 )
		{
			return( pSoldier );
		}
	}
	return( NULL );
}

BOOLEAN FindSoldier( SOLDIERTYPE **ppSoldier, UINT16 usSoldierIndex, BOOLEAN fPlayerMercsOnly)
{
	SOLDIERTYPE	 Menptr[ TOTAL_SOLDIERS ];

	*ppSoldier = NULL;

	if ( usSoldierIndex < 0 || usSoldierIndex > TOTAL_SOLDIERS-1 )
	{
		return( FALSE );
	}

	if ( MercPtrs[ usSoldierIndex ]->bActive )
	{
		*ppSoldier = MercPtrs[ usSoldierIndex ];
		return( TRUE);
	}
	else
	{
		return( FALSE );
	}
}

//--------------------------------

BOOLEAN LuaHandleNPCDoAction( UINT8 ubTargetNPC, UINT16 usActionCode, UINT8 ubQuoteNum , UINT8 InitFunction)
{
	char * filename = "scripts\\InterfaceDialogue.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( InitFunction == 0 )
	{
		lua_getglobal(L , "HandleNPCDoAction");
		lua_pushnumber(L, ubTargetNPC);
		lua_pushnumber(L, usActionCode);
		lua_pushnumber(L, ubQuoteNum);
		lua_call(L,3,0); 
	}
	
	

	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LetLuaInterfaceDialogue( UINT8 ubNPC, UINT8 InitFunction)
{
	char * filename = "scripts\\InterfaceDialogue.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( InitFunction == 0 )
	{
		lua_getglobal(L , "CarmenLeavesSectorCallback");
		lua_call(L,0,0); 
	}
	
	if ( InitFunction == 0 )
	{
		lua_getglobal(L , "HandleStuffForNPCEscorted");
		lua_pushnumber(L, ubNPC);
		lua_call(L,1,0); 
	}
	
	

	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LetLuaPerformItemAction(UINT32 ActionID, INT32 sGridNo , UINT8 InitFunction)
{
	char * filename = "scripts\\ExplosionControl.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( InitFunction == 0 )
	{
		lua_getglobal(L , "PerformItemAction");
		
		lua_pushnumber(L, ActionID);
		
		lua_pushnumber(L, sGridNo);
		

		lua_call(L,2,0); 
		//lua_pop(L,1);
	}
	

	lua_close(L);

	delete[] buffer;
	
	
	return true;

}

BOOLEAN LetLuaHourlyQuestUpdate(UINT8 Init)
{
	char * filename = "scripts\\HourlyUpdate.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);
		SGP_THROW(msg);
	}

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);
	
	//init function
//	lua_register(L, "CheckFact", l_CheckFact);	
//	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
//	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);

	//-----boxer------ only hourly quest update 
	lua_register(L, "gfBoxerFought", l_SetgfBoxerFought);
	lua_register(L, "GetWorldHour", l_GetWorldHour);	
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HourlyQuestUpdate");
		lua_call(L,0,0); 
	}
	
	lua_close(L);

	delete[] buffer;

	return true;

}

//object

//---------------
#ifdef JA2UB
static int l_InitMercgridNo0 (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InitialHeliGridNo[ 0 ] = GridNo;
		
	return 0;
}

static int l_InitMercgridNo1 (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InitialHeliGridNo[ 1 ] = GridNo;
		
	return 0;
}

static int l_InitMercgridNo2 (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InitialHeliGridNo[ 2 ] = GridNo;
		
	return 0;
}

static int l_InitMercgridNo3 (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InitialHeliGridNo[ 3 ] = GridNo;
		
	return 0;
}

static int l_InitMercgridNo4 (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InitialHeliGridNo[ 4 ] = GridNo;
		
	return 0;
}

static int l_InitMercgridNo5 (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InitialHeliGridNo[ 5 ] = GridNo;
		
	return 0;
}

static int l_InitMercgridNo6 (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InitialHeliGridNo[ 6 ] = GridNo;
		
	return 0;
}

static int l_InitJerryGridNo (lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.JerryGridNo = GridNo;
		
	return 0;
}

static int l_setLaptopQuest (lua_State *L)
{
BOOLEAN set = TRUE;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) set = lua_toboolean(L,i);
		}	
		
		
		gGameUBOptions.LaptopQuestEnabled = set;

		
	return 0;
}

static int l_setInJerry (lua_State *L)
{
BOOLEAN set = TRUE;
UINT8  n = lua_gettop(L);
UINT32 GridNo = 0;
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) set = lua_toboolean(L,i);
			if (i == 2 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.InJerry = set;
		
		if ( GridNo > 0 )
		gGameUBOptions.JerryGridNo = GridNo;
		else if ( GridNo < -1 )
		gGameUBOptions.JerryGridNo = 15943;

	return 0;
}

static int l_setJerryQuotes (lua_State *L)
{
BOOLEAN set = TRUE;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) set = lua_toboolean(L,i);
		}	
		
		
		gGameUBOptions.JerryQuotes = set;

		
	return 0;
}

static int l_setInGameHeliCrash (lua_State *L)
{
BOOLEAN set = TRUE;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) set = lua_toboolean(L,i);
		}	
		
		
		gGameUBOptions.InGameHeliCrash = set;

		
	return 0;
}

static int l_setInGameHeli (lua_State *L)
{
BOOLEAN set = TRUE;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) set = lua_toboolean(L,i);
		}	
		
		
		gGameUBOptions.InGameHeli= set;

		
	return 0;
}

static int l_SetInternalLocateGridNo(lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
		
		gGameUBOptions.LOCATEGRIDNO = GridNo;
		
	return 0;
}
#endif


static int l_ExecuteTacticalTextBox(lua_State *L)
{
INT16 sLeftPosition = 110;
UINT32 idText = 0;
UINT8  n = lua_gettop(L);
wchar_t	sText[400];
UINT32	uiStartLoc=0;  
CHAR16	zString[512];
int i;

//#define			LANGMESSAGEFILE		"BinaryData\\TacticalMessages.EDT"
//#define 		EDT_SIZE 400 * 2

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) sLeftPosition = lua_tointeger(L,i);
			if (i == 2 ) idText = lua_tointeger(L,i);
			
		}	
	/*	
	if ( FileExists(LANGMESSAGEFILE) )
	{
		uiStartLoc = EDT_SIZE * idText;
		LoadEncryptedDataFromFile(LANGMESSAGEFILE, sText, uiStartLoc, EDT_SIZE);	
		swprintf( zString, sText );
	}
	else
	{
		//Create the string
		swprintf( zString, L"Empty Text" );
	}
	*/
		if ( idText >= 0 && idText <= 1000 )
		{
			swprintf( zString, XMLTacticalMessages[idText] );

			if ( sLeftPosition <= 0 ) sLeftPosition = 110;
	
			ExecuteTacticalTextBox( sLeftPosition, zString );
		}
		
	return 0;
}

static int l_SetMercArrivalLocation(lua_State *L)
{
UINT32 GridNo;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) GridNo = lua_tointeger(L,i);
		}	
	
		gGameExternalOptions.iInitialMercArrivalLocation = GridNo;
		
	return 0;
}

static int l_GetDefaultArrivalSectorY (lua_State *L)
{
INT16 sSectorY = 1;

		sSectorY = gsMercArriveSectorY;
		
		lua_pushinteger(L, sSectorY);
		
	return 1;
}

static int l_GetDefaultArrivalSectorX (lua_State *L)
{
INT16 sSectorX = 9;

		sSectorX = gsMercArriveSectorX;
		
		lua_pushinteger(L, sSectorX);
		
	return 1;
}

static int l_GetDefaultArrivalSector (lua_State *L)
{
INT16 sSectorX = 9;
INT16 sSectorY = 1;

		sSectorX = gsMercArriveSectorX;
		sSectorY = gsMercArriveSectorY;
		
		lua_pushinteger(L, sSectorX);
		lua_pushinteger(L, sSectorY);
		
	return 2;
}

static int l_SetDefaultArrivalSector(lua_State *L)
{
UINT8 sSectorX;
UINT8 sSectorY;
UINT8  n = lua_gettop(L);
int i;

	for (i= 1; i<=n; i++ )
		{
			if (i == 1 ) sSectorX = lua_tointeger(L,i);
			if (i == 2 ) sSectorY = lua_tointeger(L,i);
		}	
		
	if ( ( sSectorX >= 1 || sSectorX <= 16 ) && ( sSectorY >= 1 || sSectorY <= 16 ) )
		{
			gsMercArriveSectorX	= sSectorX;
			gsMercArriveSectorY	= sSectorY;
			
			#ifdef JA2UB
			JA2_5_START_SECTOR_X = sSectorX;
			JA2_5_START_SECTOR_Y = sSectorY;
			#endif
			gGameExternalOptions.ubDefaultArrivalSectorX = sSectorX;
			gGameExternalOptions.ubDefaultArrivalSectorY = sSectorY;
		}
		else
		{
		
			#ifdef JA2UB
			JA2_5_START_SECTOR_X = 7;
			JA2_5_START_SECTOR_Y = 8;
			
			gsMercArriveSectorX	= 7;
			gsMercArriveSectorY	= 8;
			
			gGameExternalOptions.ubDefaultArrivalSectorX = 7;
			gGameExternalOptions.ubDefaultArrivalSectorY = 8;
			
			#else
			
			gsMercArriveSectorX	= 9;
			gsMercArriveSectorY	= 1;
			

			gGameExternalOptions.ubDefaultArrivalSectorX = 9;
			gGameExternalOptions.ubDefaultArrivalSectorY = 1;
			#endif
		}
		
	return 0;
}

static int l_BoxerExists(lua_State *L)
{
BOOLEAN Bool;

	Bool = BoxerExists( );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_gubBoxerID(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 val,val2;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
		if (i == 2 ) val2 = lua_tointeger(L,i);
	}
	
	if ( val >= 0 && val <= 2 )
		gubBoxerID[ val ] = val2;
	
return 0;
}

static int l_CheckTalkerUnpropositionedFemale(lua_State *L)
{
BOOLEAN Bool;

	Bool = CheckTalkerUnpropositionedFemale( );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_RepairmanIsFixingItemsButNoneAreDoneYet(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubProfileID;
BOOLEAN Bool;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}
	
	Bool = RepairmanIsFixingItemsButNoneAreDoneYet( ubProfileID );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_CurrentPlayerProgressPercentage(lua_State *L)
{
UINT8  n = lua_gettop(L);

UINT8 val;

	val = CurrentPlayerProgressPercentage( );
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_SetgMercProfilesbTown(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubProfileID;
UINT32 Town;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
		if (i == 2 ) Town = lua_tointeger(L,i);
	}
	
	gMercProfiles[ ubProfileID ].bTown = Town;
	
return 0;
}

static int l_InitProfile(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubProfileID;
//UINT32 Town;
UINT16 x = 0;
UINT16 y = 0;
UINT8 z = 0;
UINT32 sGridNo;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
		if (i == 2 ) x = lua_tointeger(L,i);
		if (i == 3 ) y = lua_tointeger(L,i);
		if (i == 4 ) z = lua_tointeger(L,i);
		if (i == 5 ) sGridNo = lua_tointeger(L,i);
	}
	
	gMercProfiles[ ubProfileID ].sSectorX = x;
	gMercProfiles[ ubProfileID ].sSectorY = y;
	gMercProfiles[ ubProfileID ].bSectorZ = z;
	gMercProfiles[ ubProfileID ].sGridNo = sGridNo; 
	gMercProfiles[ ubProfileID ].fUseProfileInsertionInfo = TRUE;
	gMercProfiles[ ubProfileID ].ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	gMercProfiles[ ubProfileID ].usStrategicInsertionData = sGridNo;
	
return 0;
}

static int l_GetgMercProfilesbTown(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubProfileID;
UINT32 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}
	
	val = gMercProfiles[ ubProfileID ].bTown;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_SetusTotalDaysServed(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 ProfilID;
UINT32 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ProfilID = lua_tointeger(L,i);
		if (i == 2 ) val = lua_tointeger(L,i);

	}
	
	gMercProfiles[ ProfilID ].usTotalDaysServed = val;
	
return 0;
}

static int l_GetusTotalDaysServed(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 ProfilID;
UINT32 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ProfilID = lua_tointeger(L,i);
	}
	
	val = gMercProfiles[ ProfilID ].usTotalDaysServed;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_SetgfTownUsesLoyalty(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
BOOLEAN Bool;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
		if (i == 2 ) Bool = lua_tointeger(L,i);
	}
	
	gfTownUsesLoyalty[ bTown ] = Bool;
	
return 0;
}

static int l_GetgfTownUsesLoyalty(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
BOOLEAN Bool;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
	}
	
	Bool = gfTownUsesLoyalty[ bTown ];
	
	lua_pushboolean(L, Bool);
	
return 1;
}


static int l_SetTownLoyaltyfLiberatedAlready(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
BOOLEAN Bool;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
		if (i == 2 ) Bool = lua_tointeger(L,i);
	}
	
	gTownLoyalty[ bTown ].fLiberatedAlready = Bool;
	
return 0;
}

static int l_GetTownLoyaltyfLiberatedAlready(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
BOOLEAN Bool;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
	}
	
	Bool = gTownLoyalty[ bTown ].fLiberatedAlready;
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_SetTownLoyaltyfStarted(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
BOOLEAN Bool;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
		if (i == 2 ) Bool = lua_toboolean(L,i);
	}
	
	gTownLoyalty[ bTown ].fStarted = Bool;
	
return 0;
}

static int l_GetTownLoyaltyfStarted(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
BOOLEAN Bool;;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
	}
	
	Bool = gTownLoyalty[ bTown ].fStarted;
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_SetTownLoyaltysChange(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
INT16 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
		if (i == 2 ) val = lua_tointeger(L,i);
	}
	
	gTownLoyalty[ bTown ].sChange = val;
	
return 0;
}

static int l_GetTownLoyaltysChange(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
INT16 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
	}
	
	val = gTownLoyalty[ bTown ].sChange;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_SetTownLoyaltyubRating(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
UINT8 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
		if (i == 2 ) val = lua_tointeger(L,i);
	}
	
	gTownLoyalty[ bTown ].ubRating = val;
	
return 0;
}

static int l_GetTownLoyaltyubRating(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 bTown;
UINT8 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTown = lua_tointeger(L,i);
	}
	
	val = gTownLoyalty[ bTown ].ubRating;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetTownIdForSector(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sMapX;
INT16 sMapY;
UINT8 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sMapX = lua_tointeger(L,i);
		if (i == 2 ) sMapY = lua_tointeger(L,i);
	}
	
	val = GetTownIdForSector( sMapX, sMapY );
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_UpdateMercsInSector(lua_State *L)
{
	UINT8  n = lua_gettop(L);

INT16 sSectorX;
INT16 sSectorY;
INT8 bSectorZ;
int i;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) bSectorZ = lua_tointeger(L,i);
	}

	UpdateMercsInSector( sSectorX, sSectorY, bSectorZ );
	
return 0;
}

static int l_CheckIfEntireTownHasBeenLiberated(lua_State *L)
{
	UINT8  n = lua_gettop(L);
INT8 bTownId;
INT16 sSectorX;
INT16 sSectorY;
int i;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTownId = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
	}

	CheckIfEntireTownHasBeenLiberated( bTownId, sSectorX, sSectorY );
	
return 0;
}

static int l_CheckIfEntireTownHasBeenLost(lua_State *L)
{
	UINT8  n = lua_gettop(L);
INT8 bTownId;
INT16 sSectorX;
INT16 sSectorY;
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTownId = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
	}

	CheckIfEntireTownHasBeenLost( bTownId, sSectorX, sSectorY );
	
return 0;
}

static int l_DidFirstBattleTakePlaceInThisTown(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 bTownId;
BOOLEAN Bool;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTownId = lua_tointeger(L,i);
	}
	
	Bool = DidFirstBattleTakePlaceInThisTown( bTownId );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_SetTheFirstBattleSector(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorValue;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorValue = lua_tointeger(L,i);
	}
	
	SetTheFirstBattleSector( sSectorValue );
	
return 0;
}

// used when monsters attack a town sector without going through tactical and they win
static int l_AdjustLoyaltyForCivsEatenByMonsters(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorX;
INT16 sSectorY;
UINT8 ubHowMany;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) ubHowMany = lua_tointeger(L,i);
	}
	
	AdjustLoyaltyForCivsEatenByMonsters( sSectorX, sSectorY, ubHowMany);
	
return 0;
}

static int l_IsTownUnderCompleteControlByPlayer(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT32 val;
INT32 bTownId;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bTownId = lua_tointeger(L,i);
	}
	

	val = IsTownUnderCompleteControlByPlayer(bTownId);
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetNumberOfWholeTownsUnderControl(lua_State *L)
{
INT32 val;
	
	val = GetNumberOfWholeTownsUnderControl();
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_SetCurrentWorldSector(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sMapX;
INT16 sMapY;
INT8 bMapZ;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sMapX = lua_tointeger(L,i);
		if (i == 2 ) sMapY = lua_tointeger(L,i);
		if (i == 3 ) bMapZ = lua_tointeger(L,i);
	}
	
	Bool = SetCurrentWorldSector( sMapX, sMapY, bMapZ );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_EnterSector(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorX;
INT16 sSectorY; 
INT8 bSectorZ;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) bSectorZ = lua_tointeger(L,i);
	}
	
	Bool = EnterSector( sSectorX, sSectorY, bSectorZ );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_FadeInGameScreen(lua_State *L)
{
	FadeInGameScreen( );
	
 return 0;
}

static int l_FadeOutGameScreen(lua_State *L)
{
	FadeOutGameScreen( );
	
 return 0;
}

static int l_SetGroupNextSectorValue(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorX;
INT16 sSectorY;

UINT8 ubGroupID;
 
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) ubGroupID = lua_tointeger(L,i);
	}
	
 SetGroupNextSectorValue( sSectorX, sSectorY, ubGroupID );
	
 return 0;
}

static int l_SetGroupSectorValue(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorX;
INT16 sSectorY;
INT16 sSectorZ;
UINT8 ubGroupID;
 
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) sSectorZ = lua_tointeger(L,i);
		if (i == 4 ) ubGroupID = lua_tointeger(L,i);
	}
	
 SetGroupSectorValue( sSectorX, sSectorY, sSectorZ, ubGroupID );
	
 return 0;
}

				
static int l_DeleteMercInventory(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ID;
UINT32 inv;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) inv = lua_tointeger(L,i);
	}
	
	//pProfile = &(gMercProfiles[ID]);				
	
	gMercProfiles[ ID ].inv[ inv ] = 0; 
	gMercProfiles[ ID ].bInvStatus[ inv ] = 0;
	gMercProfiles[ ID ].bInvNumber[ inv ] = 0;	
	
 return 0;
}

static int l_StrategicPythSpacesAway(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 val;
INT16 sOrigin;
INT16 sDest;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sOrigin = lua_tointeger(L,i);
		if (i == 2 ) sDest = lua_tointeger(L,i);
	}
	
	val = StrategicPythSpacesAway( sOrigin,  sDest);
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetMercArrivalTimeOfDay(lua_State *L)
{
//UINT8  n = lua_gettop(L);

UINT32 val;

//	for (i= 1; i<=n; i++ )
//	{
//		if (i == 1 ) ubMercID = lua_tointeger(L,i);
//	}
	
	val = GetMercArrivalTimeOfDay( );
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_NumberOfMercsOnPlayerTeam(lua_State *L)
{
//UINT8  n = lua_gettop(L);

UINT8 val;


//	for (i= 1; i<=n; i++ )
//	{
//		if (i == 1 ) ubMercID = lua_tointeger(L,i);
//	}
	
	val = NumberOfMercsOnPlayerTeam( );
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_IsMercHireable(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT8 ubMercID;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMercID = lua_tointeger(L,i);
	}
	
	Bool = IsMercHireable( ubMercID );
	
	lua_pushboolean(L, Bool);
	
return 1;
}



static int l_IsMercDead(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT8 ubMercID;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMercID = lua_tointeger(L,i);
	}
	
	Bool = IsMercDead( ubMercID );
	
	lua_pushboolean(L, Bool);
	
return 1;
}
						
static int l_ResetHistoryFact(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ubCode; 
INT16 sSectorX; 
INT16 sSectorY;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCode = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
	}
	
	ResetHistoryFact( ubCode, sSectorX, sSectorY );
	
 return 0;
}

static int l_SetHistoryFact(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ubCode; 
UINT8 ubSecondCode;
UINT32 uiDate; 
INT16 sSectorX; 
INT16 sSectorY;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCode = lua_tointeger(L,i);
		if (i == 2 ) ubSecondCode = lua_tointeger(L,i);
		if (i == 3 ) uiDate = lua_tointeger(L,i);
		if (i == 4 ) sSectorX = lua_tointeger(L,i);
		if (i == 5 ) sSectorY = lua_tointeger(L,i);
	}
	
	SetHistoryFact( ubCode, ubSecondCode, uiDate, sSectorX, sSectorY );
	
	return 0;
}

static int l_SectorEnemy(lua_State *L)
{
UINT16 x = -1;
UINT16 y = -1;
UINT16 z = -1;
UINT8 TROOPS_ILOSC = 0;
UINT8 ELITA_ILOSC = 0;
UINT CREATURE_ILOSC = 0;

	if ( ( x >= 1 || x <= 16 ) && ( y >= 1 || y <= 16 ) )
		{
			x = lh_getIntegerFromTable(L, "SectorX");
			y = lh_getIntegerFromTable(L, "SectorY");
			
			TROOPS_ILOSC = lh_getIntegerFromTable(L, "Troops");
			ELITA_ILOSC = lh_getIntegerFromTable(L, "Elite");
			CREATURE_ILOSC = lh_getIntegerFromTable(L, "Creature");
				
			//z = lh_getIntegerFromTable(L, "SectorZ");
					
			SectorInfo[ (UINT8)SECTOR(  x,  y ) ].ubNumTroops = TROOPS_ILOSC;
			SectorInfo[ (UINT8)SECTOR(  x,  y ) ].ubNumElites = ELITA_ILOSC;
			SectorInfo[ (UINT8)SECTOR(  x,  y ) ].ubNumCreatures = CREATURE_ILOSC;	
		}
		
	return 0;
}

static int l_SECTOR(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT16 x,y;
UINT8 val;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) x = lua_tointeger(L,i);
		if (i == 2 ) y = lua_tointeger(L,i);
	}
					
			val = SECTOR(  x,  y );
		
	lua_pushinteger(L, val);
	
return 1;
}

static int l_CALCULATE_STRATEGIC_INDEX(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT16 x,y;
UINT8 val;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) x = lua_tointeger(L,i);
		if (i == 2 ) y = lua_tointeger(L,i);
	}
					
			val = CALCULATE_STRATEGIC_INDEX(  x,  y );
		
	lua_pushinteger(L, val);
	
return 1;
}

static int l_SectorInfoBloodCats(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 idSector;
UINT8 val;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) idSector = lua_tointeger(L,i);
	}
					
			val = SectorInfo[ idSector ].bBloodCats;
		
	lua_pushinteger(L, val);
	
return 1;
}

static int l_IsHisMineAtMaxProduction(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT8 ubMinerProfileId;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	Bool = 	IsHisMineAtMaxProduction(ubMinerProfileId );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_IsHisMineLostAndRegained(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT8 ubMinerProfileId;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	Bool = 	IsHisMineLostAndRegained(ubMinerProfileId );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_IsHisMineInfested(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ubMinerProfileId;
BOOLEAN Bool;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	Bool = 	IsHisMineInfested(ubMinerProfileId );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_IsHisMineDisloyal(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT8 ubMinerProfileId;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	Bool = 	IsHisMineDisloyal(ubMinerProfileId );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_IsHisMineEmpty(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubMinerProfileId;
BOOLEAN Bool;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	Bool = 	IsHisMineEmpty(ubMinerProfileId );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_IsHisMineRunningOut(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubMinerProfileId;
BOOLEAN Bool;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	Bool = 	IsHisMineRunningOut(ubMinerProfileId );
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_GetHeadMinersMineIndex(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ubMinerProfileId;
UINT8 ID2;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	ID2 = 	GetHeadMinersMineIndex(ubMinerProfileId);
	
		lua_pushinteger(L, ID2);
	
return 1;
}


static int l_PlayerSpokeToHeadMiner(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubMinerProfileId;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
	PlayerSpokeToHeadMiner( ubMinerProfileId );
	
return 0;
}

static int l_ResetQueenRetookMine(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubMinerProfileId;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMinerProfileId = lua_tointeger(L,i);
	}
	
    ResetQueenRetookMine( ubMinerProfileId );
	
return 0;
}

static int l_IssueHeadMinerQuote(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 bMineIndex;
UINT8 ubQuoteType;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bMineIndex = lua_tointeger(L,i);
		if (i == 2 ) ubQuoteType = lua_tointeger(L,i);
	}
	
	IssueHeadMinerQuote( bMineIndex, ubQuoteType );

return 0;
}

static int l_NumMercsNear(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ubProfileID;
UINT8 ubMaxDist;
UINT8 ID2;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
		if (i == 2 ) ubMaxDist = lua_tointeger(L,i);
	}
	
	ID2 = 	NumMercsNear( ubProfileID, ubMaxDist );
	
		lua_pushinteger(L, ID2);
	
return 1;
}

static int l_PredictDailyIncomeFromAMine(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
INT8 bMineIndex;
UINT32 val;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bMineIndex = lua_tointeger(L,i);
		if (i == 2 ) Bool = lua_toboolean(L,i);
	}
	
	val = PredictDailyIncomeFromAMine( bMineIndex, Bool );
	
		lua_pushinteger(L, val);
	
return 1;
}

static int l_GetIdOfMineForSector(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 val;
INT16 sSectorX;
INT16 sSectorY; 
INT8 bSectorZ;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) bSectorZ = lua_tointeger(L,i);
	}
	
	val = GetIdOfMineForSector( sSectorX, sSectorY, bSectorZ );
	
	lua_pushinteger(L, val);
return 1;
}

static int l_SpokenToHeadMiner(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT8 ubMineIndex;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMineIndex = lua_tointeger(L,i);
	}
	
	Bool = SpokenToHeadMiner( ubMineIndex );
	
	lua_pushboolean(L, Bool);
return 1;
}

static int l_AnotherFightPossible(lua_State *L)
{
BOOLEAN Bool;

	Bool = 	AnotherFightPossible( );
	
		lua_pushboolean(L, Bool);
return 1;
}

static int l_fTurnTimeLimit(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Bool = lua_tointeger(L,i);
	}
	

	gGameOptions.fTurnTimeLimit = Bool;
	
return 0;
}

static int l_HasHisMineBeenProducingForPlayerForSomeTime(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT8 ubProfileID;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}
	
	Bool = 	HasHisMineBeenProducingForPlayerForSomeTime( ubProfileID );
	
		lua_pushboolean(L, Bool);
	
return 1;
}

static int l_CheckTalkerFemale(lua_State *L)
{
UINT8  n = lua_gettop(L);

BOOLEAN Bool;
	
	Bool =  CheckTalkerFemale( );
	
		lua_pushboolean(L, Bool);
	
return 1;
}

static int l_CheckTalkerStrong(lua_State *L)
{
UINT8  n = lua_gettop(L);

BOOLEAN Bool;
	
	Bool =  CheckTalkerStrong( );
	
		lua_pushboolean(L, Bool);
	
return 1;
}

static int l_NumMalesPresent(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 id;
UINT8 ubProfileID;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);  
	}
	
	id = NumMalesPresent( ubProfileID );
	lua_pushinteger(L, id);
	
return 1;
}

static int l_NumWoundedMercsNearby(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

INT8 ID,ID2;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
	ID2 = 	NumWoundedMercsNearby(ID);
	
		lua_pushinteger(L, ID2);
	
return 1;
}


static int l_gubFact(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT32 Fact;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Fact = lua_tointeger(L,i);
		if (i == 2 ) Bool = lua_toboolean(L,i);
	}
		gubFact[Fact] = Bool;
	
return 0;
}

static int l_CheckPlayerHasHead(lua_State *L)
{
UINT8  n = lua_gettop(L);

BOOLEAN Bool;

	Bool = CheckPlayerHasHead();
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_GetgubFact(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT32 Fact;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Fact = lua_tointeger(L,i);
	}
		Bool = gubFact[Fact];
		
	lua_pushboolean(L, Bool);
	
return 1;
}


static int l_CheckForNewShipment(lua_State *L)
{
UINT8  n = lua_gettop(L);

BOOLEAN Bool;

	Bool = CheckForNewShipment();
		
	lua_pushboolean(L, Bool);
	
return 1;
}


static int l_CountNumberOfBobbyPurchasesThatAreInTransit(lua_State *L)
{
UINT8  n = lua_gettop(L);


UINT16 id;

	id = CountNumberOfBobbyPurchasesThatAreInTransit();
		
	lua_pushinteger(L, id);
	
return 1;
}

static int l_FunctionCheckForKingpinsMoneyMissing(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Bool = lua_toboolean(L,i);
	}
		CheckForKingpinsMoneyMissing( Bool );
	
return 0;
}


static int l_GetNumberOfWorldItemsFromTempItemFile(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
INT16 sMapX;
INT16 sMapY;
INT8 bMapZ;
UINT32 puiNumberOfItems;
BOOLEAN fIfEmptyCreate;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sMapX = lua_tointeger(L,i);
		if (i == 2 ) sMapY = lua_tointeger(L,i);
		if (i == 3 ) bMapZ = lua_tointeger(L,i);
		if (i == 4 ) puiNumberOfItems = lua_tointeger(L,i);
		if (i == 5 ) fIfEmptyCreate = lua_toboolean(L,i);
	}
	
		Bool = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &puiNumberOfItems, fIfEmptyCreate );

		lua_pushboolean(L, Bool);
		
return 1;
}

static int l_MoveItemPools(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT32 sStartPos;
INT32 sEndPos;
INT8 bStartLevel;
INT8 bEndLevel;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sStartPos = lua_tointeger(L,i);
		if (i == 2 ) sEndPos = lua_tointeger(L,i);
		if (i == 3 ) bStartLevel = lua_tointeger(L,i);
		if (i == 4 ) bEndLevel = lua_tointeger(L,i);
	}
		MoveItemPools( sStartPos, sEndPos, bStartLevel, bEndLevel );
	
return 0;
}


static int l_AddFutureDayStrategicEvent(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubCallbackID;
UINT32 uiMinStamp;
UINT32 uiParam; 
UINT32 uiNumDaysFromPresent;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCallbackID = lua_tointeger(L,i);
		if (i == 2 ) uiMinStamp = lua_tointeger(L,i);
		if (i == 3 ) uiParam = lua_tointeger(L,i);
		if (i == 4 ) uiNumDaysFromPresent = lua_tointeger(L,i);
	}
		AddFutureDayStrategicEvent( ubCallbackID, uiMinStamp, uiParam, uiNumDaysFromPresent );
	
return 0;
}

static int l_AddSameDayStrategicEvent(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubCallbackID;
UINT32 uiMinStamp; 
UINT32 uiParam;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCallbackID = lua_tointeger(L,i);
		if (i == 2 ) uiMinStamp = lua_tointeger(L,i);
		if (i == 3 ) uiParam = lua_tointeger(L,i);
	}
		AddSameDayStrategicEvent( ubCallbackID, uiMinStamp, uiParam );
	
return 0;
}

static int l_AddHistoryToPlayersLog(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubCode;
UINT8 ubSecondCode; 
UINT32 uiDate; 
INT16 sSectorX; 
INT16 sSectorY;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCode = lua_tointeger(L,i);
		if (i == 2 ) ubSecondCode = lua_tointeger(L,i);
		if (i == 3 ) uiDate = lua_tointeger(L,i);
		if (i == 4 ) sSectorX = lua_tointeger(L,i);
		if (i == 5 ) sSectorY = lua_tointeger(L,i);
	}
		AddHistoryToPlayersLog( ubCode, ubSecondCode, uiDate, sSectorX, sSectorY );
	
return 0;
}

static int l_gWorldItemsObjectDataMoney(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT32 uiLoop, MoneyAmount;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiLoop = lua_tointeger(L,i);
	}
		MoneyAmount =  gWorldItems[uiLoop].object[0]->data.money.uiMoneyAmount;
		lua_pushinteger(L, MoneyAmount);
	
return 1;
}

static int l_gWorldItemsObjectItem(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT32 uiLoop, Object;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiLoop = lua_tointeger(L,i);
	}
		Object =  gWorldItems[ uiLoop ].object.usItem;
		lua_pushinteger(L, Object);
	
return 1;
}

static int l_ItemExistsAtLocation(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
INT32 sGridNo;
INT32 iItemIndex;
UINT8 ubLevel;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
		if (i == 2 ) iItemIndex = lua_tointeger(L,i);
		if (i == 3 ) ubLevel = lua_tointeger(L,i);
	}
		Bool =  ItemExistsAtLocation( sGridNo, iItemIndex, ubLevel );
		lua_pushboolean(L, Bool);
	
return 1;
}

static int l_gWorldItemsExists(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT32 uiLoop;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiLoop = lua_tointeger(L,i);
	}

		Bool = gWorldItems[ uiLoop ].fExists;
			
		lua_pushboolean(L, Bool);
	
return 1;
}

static int l_SetgWorldItemsExists(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;
UINT32 uiLoop;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiLoop = lua_tointeger(L,i);
		if (i == 2 ) Bool = lua_tointeger(L,i);
	}

		gWorldItems[ uiLoop ].fExists = Bool;
			
		
	
return 0;
}

//----------10-05-2010---------------------------

static int l_bAttitude(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;

SOLDIERTYPE * pSoldier;
UINT32 Attitude;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) Attitude = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
			{
				Attitude = pSoldier->aiData.bAttitude;
			}

		lua_pushinteger(L, Attitude);
	
return 1;
}

static int l_InitCreatureQuest (lua_State *L)
{
UINT8  n = lua_gettop(L);


	InitCreatureQuest();
	
return 0;
}

static int l_AffectAllTownsLoyaltyByDistanceFrom (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 iLoyaltyChange;
INT16 sSectorX; 
INT16 sSectorY;
INT8 bSectorZ;
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) iLoyaltyChange = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
		if (i == 4 ) bSectorZ = lua_tointeger(L,i);
	}
	
	AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ );
	
return 0;
}

static int l_PlayerMercsInSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT8 ubSectorX;
UINT8 ubSectorY; 
UINT8 ubSectorZ;
UINT8 val;	
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubSectorX = lua_tointeger(L,i);
		if (i == 2 ) ubSectorY = lua_tointeger(L,i);
		if (i == 3 ) ubSectorZ = lua_tointeger(L,i);
	}
	val =  PlayerMercsInSector( ubSectorX, ubSectorY, ubSectorZ );
		
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetPlayerMercsInSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT8 ubSectorX;
UINT8 ubSectorY; 
UINT8 ubSectorZ;
UINT8 val;	
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubSectorX = lua_tointeger(L,i);
		if (i == 2 ) ubSectorY = lua_tointeger(L,i);
		if (i == 3 ) ubSectorZ = lua_tointeger(L,i);
	}	
		val = PlayerMercsInSector( ubSectorX, ubSectorY, ubSectorZ );
		
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetPlayerGroupsInSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT8 bSectorX;
UINT8 bSectorY; 
UINT8 bSectorZ;
UINT8 val;	
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) bSectorX = lua_tointeger(L,i);
		if (i == 2 ) bSectorY = lua_tointeger(L,i);
		if (i == 3 ) bSectorZ = lua_tointeger(L,i);
	}	
		val = PlayerGroupsInSector( bSectorX, bSectorY, bSectorZ );
		
	lua_pushinteger(L, val);
	
return 1;
}

static int l_PlayerGroupsInSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT8 ubSectorX;
UINT8 ubSectorY; 
UINT8 ubSectorZ;

UINT8 Group;
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubSectorX = lua_tointeger(L,i);
		if (i == 2 ) ubSectorY = lua_tointeger(L,i);
		if (i == 3 ) ubSectorZ = lua_tointeger(L,i);
	}
	
	Group = PlayerGroupsInSector( ubSectorX, ubSectorY, ubSectorZ );
		
	lua_pushinteger(L, Group);
	
return 1;
}


static int l_DeleteTalkingMenu (lua_State *L)
{
UINT8  n = lua_gettop(L);
	
	DeleteTalkingMenu( );
	
return 0;
}

static int l_SetubDefaultArrivalSectorY (lua_State *L)
{
UINT8  n = lua_gettop(L);

int i;

UINT32 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
	}
	 gGameExternalOptions.ubDefaultArrivalSectorY = val;
	
	
return 0;
}

static int l_SetiFirstArrivalDelay (lua_State *L)
{
UINT8  n = lua_gettop(L);

int i;

UINT32 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
	}
	 gGameExternalOptions.iFirstArrivalDelay = val;
	
return 0;
}

static int l_SetubDefaultArrivalSectorX (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 val;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
	}
	gGameExternalOptions.ubDefaultArrivalSectorX = val;
	
return 0;
}


//----
static int l_GetubDefaultArrivalSectorY (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 val;
	
	val = gGameExternalOptions.ubDefaultArrivalSectorY;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetiFirstArrivalDelay (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 val;
	
	val = gGameExternalOptions.iFirstArrivalDelay;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetubDefaultArrivalSectorX (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 val;
	
	val = gGameExternalOptions.ubDefaultArrivalSectorX;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetStartingCashInsane (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 val;
	
	val = gGameExternalOptions.iStartingCashInsane;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetStartingCashExpert (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 val;
	
	val = gGameExternalOptions.iStartingCashExpert;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetStartingCashExperienced (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 val;
	
	val = gGameExternalOptions.iStartingCashExperienced;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetStartingCashNovice (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 val;
	
	val = gGameExternalOptions.iStartingCashNovice;
	
	lua_pushinteger(L, val);
	
return 1;
}

static int l_GetWorldTotalMin (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 uiDate; 

	uiDate = GetWorldTotalMin();
return 0;
}

static int l_AddTransactionToPlayersBook (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubCode;
UINT8 ubSecondCode; 
UINT32 uiDate; 
INT32 iAmount;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCode = lua_tointeger(L,i);
		if (i == 2 ) ubSecondCode = lua_tointeger(L,i);
		if (i == 3 ) uiDate = lua_tointeger(L,i);
		if (i == 4 ) iAmount = lua_tointeger(L,i);
	}

	AddTransactionToPlayersBook(ubCode, ubSecondCode, uiDate, iAmount);
return 0;
}


static int l_CheckInvestigateSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
BOOLEAN Bool;
	
	Bool = gGameExternalOptions.gfInvestigateSector;
	    
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_SetInvestigateSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Bool = lua_toboolean(L,i);
	}
	
	gGameExternalOptions.gfInvestigateSector = Bool;
	
return 0;
}

//static int l_MusicFadeIn (lua_State *L)
//{
//UINT8  n = lua_gettop(L);
//
//	MusicFadeIn();
//	
//return 0;
//}

//static int l_MusicFadeOut (lua_State *L)
//{
//UINT8  n = lua_gettop(L);
//
//	MusicFadeOut();
//	
//return 0;
//}

//static int l_MusicStop (lua_State *L)
//{
//UINT8  n = lua_gettop(L);
//
//	MusicStop( );
//	
//return 0;
//}

static int l_MusicGetVolume (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 uiVolume;

	uiVolume = MusicGetVolume( );
	
	lua_pushinteger(L, uiVolume);
	
return 1;
}

static int l_GetSpeechVolume (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 uiVolume;

	uiVolume = GetSpeechVolume( );
	
	lua_pushinteger(L, uiVolume);
	
return 1;
}

static int l_GetSoundEffectsVolume (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT32 uiVolume;

	uiVolume = GetSoundEffectsVolume( );
	
	lua_pushinteger(L, uiVolume);
	
return 1;
}

static int l_SetSoundEffectsVolume (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 uiVolume;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiVolume = lua_tointeger(L,i);
	}
	
	SetSoundEffectsVolume( uiVolume );
	
return 0;
}

static int l_SetSpeechVolume (lua_State *L)
{
UINT8  n = lua_gettop(L);

int i;
UINT32 uiVolume;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiVolume = lua_tointeger(L,i);
	}
	
	SetSpeechVolume( uiVolume );
	
return 0;
}

static int l_MusicSetVolume (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 uiVolume;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiVolume = lua_tointeger(L,i);
	}
	
	MusicSetVolume( uiVolume );
	
return 0;
}

static int l_MusicPlay (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 uiNum;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiNum = lua_tointeger(L,i);
	}
	
	MusicPlay( uiNum );
	
return 0;
}

static int l_SetMusicMode (lua_State *L)
{
UINT8  n = lua_gettop(L);

int i;
UINT8 ubMusicMode;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubMusicMode = lua_tointeger(L,i);
	}
	
	SetMusicMode( ubMusicMode );
	
return 0;
}

static int l_PlayerTeamFull (lua_State *L)
{
UINT8  n = lua_gettop(L);
BOOLEAN Full;


		Full =  PlayerTeamFull( );
		
	lua_pushboolean(L, Full);
	
return 1;
}

static int l_PauseGame (lua_State *L)
{
UINT8  n = lua_gettop(L);
BOOLEAN Pause;
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Pause = lua_toboolean(L,i);
	}

	
	if (Pause == TRUE)
	{
	HandlePlayerPauseUnPauseOfGame( );
	}
	
return 0;
}

static int l_SetProfileFaceData (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubCharNum;
UINT8 ubFaceIndex;
UINT16 usEyesX;
UINT16 usEyesY;
UINT16 usMouthX;
UINT16 usMouthY;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCharNum = lua_tointeger(L,i);
		if (i == 2 ) ubFaceIndex = lua_tointeger(L,i);
		if (i == 3 ) usEyesX = lua_tointeger(L,i);
		if (i == 4 ) usEyesY = lua_tointeger(L,i);
		if (i == 5 ) usMouthX = lua_tointeger(L,i);
		if (i == 6 ) usMouthY = lua_tointeger(L,i);
	}

	SetProfileFaceData( ubCharNum, ubFaceIndex, usEyesX, usEyesY, usMouthX, usMouthY );
	
return 0;
}

static int l_GetProfileFaceData (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubCharNum;
UINT8 ubFaceIndex;
UINT16 usEyesX;
UINT16 usEyesY;
UINT16 usMouthX;
UINT16 usMouthY;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCharNum = lua_tointeger(L,i);
	}

	ubFaceIndex = gMercProfiles[ ubCharNum ].ubFaceIndex;
	usEyesX = gMercProfiles[ ubCharNum ].usEyesX;
	usEyesY = gMercProfiles[ ubCharNum ].usEyesY;
	usMouthX = gMercProfiles[ ubCharNum ].usMouthX;
	usMouthY = gMercProfiles[ ubCharNum ].usMouthY;
	
	lua_pushinteger(L, ubFaceIndex);
	lua_pushinteger(L, usEyesX);
	lua_pushinteger(L, usEyesY);
	lua_pushinteger(L, usMouthX);
	lua_pushinteger(L, usMouthY);
	
return 5;
}

static int l_SetOffPanicBombs (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ubID;
INT8 bPanicTrigger;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) bPanicTrigger = lua_tointeger(L,i);
	}

	SetOffPanicBombs( ubID, bPanicTrigger );
	
return 0;
}

static int l_ToggleActionItemsByFrequency (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT16 ID;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}

	ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + ID );
	
return 0;
}

static int l_PlayJA2Sample (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 sGridNo;
UINT16 sample;
INT8 SoundVolume2;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sample = lua_tointeger(L,i);
		if (i == 2 ) SoundVolume2 = lua_tointeger(L,i);
		if (i == 3 ) sGridNo = lua_tointeger(L,i);
	}
	PlayJA2Sample( sample, RATE_11025, SoundVolume( SoundVolume2, sGridNo ), 5, SoundDir( sGridNo ) );
return 0;
}

//-----------------07-05-2010--------------------

static int l_WorldLevelDataTerrainID(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT32 sGridNo, sGridNo2;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

	sGridNo2 = gpWorldLevelData[sGridNo].ubTerrainID;
	
	lua_pushboolean(L, sGridNo2);
	
return 1;
}

static int l_MakeNoise(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubNoiseMaker;
INT32 sGridNo;
INT8 bLevel;
UINT8 ubTerrType; 
UINT8 ubVolume;
UINT8 ubNoiseType;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubNoiseMaker = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
		if (i == 3 ) bLevel = lua_tointeger(L,i);
		if (i == 4 ) ubTerrType = lua_tointeger(L,i);
		if (i == 5 ) ubVolume = lua_tointeger(L,i);
		if (i == 6 ) ubNoiseType = lua_tointeger(L,i);
	}

	MakeNoise( ubNoiseMaker, sGridNo, bLevel, ubTerrType, ubVolume, ubNoiseType );
	
return 0;
}

static int l_TacticalStatusTeamHuman(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 team;
BOOLEAN Bool;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) team = lua_tointeger(L,i);
	}

	if (gTacticalStatus.Team[team].bHuman )
		Bool = TRUE;
	else
		Bool = FALSE;
	
		lua_pushboolean(L, Bool);
	
return 1;
}


static int l_TacticalStatusTeamActive(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 team;
BOOLEAN Bool;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) team = lua_tointeger(L,i);
	}

	if (gTacticalStatus.Team[team].bTeamActive )
		Bool = TRUE;
	else
		Bool = FALSE;
	
		lua_pushboolean(L, Bool);
	
return 1;
}

static int l_bNeutral(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
BOOLEAN Bool;
SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
			{
				if ( pSoldier->aiData.bNeutral )
					Bool = TRUE;
				else
					Bool = FALSE;
			}

		lua_pushboolean(L, Bool);
	
return 1;
}

static int l_CheckSoldierAlertStatus(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 Status_alert, ID;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
			Status_alert = pSoldier->aiData.bAlertStatus;		

		lua_pushinteger(L, Status_alert);
	
return 1;
}

static int l_InitFace(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT8  IDFace = -1;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) IDFace = lua_tointeger(L,i);
	}
	
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
			{
				DeleteSoldierFace( pSoldier );
				// create new face
				if  (IDFace > -1 )
				   pSoldier->iFaceIndex = IDFace;//InitSoldierFace( pSoldier );
				else
				   pSoldier->iFaceIndex = InitSoldierFace( pSoldier );
				
			}

return 0;
}

static int l_ActionInProgress(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID,cnt,cnt2;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) cnt = lua_tointeger(L,i);
	}
	
		if ( ID > -1 )
		{
			pSoldier = FindSoldierByProfileID2( ID, TRUE );
			if (pSoldier)
				{
					pSoldier->aiData.bActionInProgress = ExecuteAction( pSoldier);	
				}
		}
		else
		{
						
			cnt2 = gTacticalStatus.Team[ CIV_TEAM ].bFirstID;
					for ( pSoldier = MercPtrs[ cnt2 ]; cnt2 <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; cnt2++ ,pSoldier++)
							{
								if (pSoldier->bActive )
									{
										if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->ubProfile == NO_PROFILE  )
											{
												pSoldier->aiData.bActionInProgress = ExecuteAction( pSoldier);	
											}
									}
							}
			
			
		}
	
return 0;
}

static int l_SetSoldierNonNeutral(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
		
	if ( ID != NO_PROFILE ) 
	{
		pSoldier = FindSoldierByProfileID( ID, FALSE );	
		if (pSoldier)
			{
				SetSoldierNonNeutral (pSoldier); 
			}
	}
	else
	{
		pSoldier = FindSoldierByProfileID_( ID );
		if ( pSoldier->ubProfile == ID ) 
		{
			
			if (pSoldier)
			{
				 SetSoldierNonNeutral (pSoldier);
			}	
		}
	}

return 0;
}

static int l_CheckSoldierNoiseVolume(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 NoiseVolume, ID;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
			{

			NoiseVolume = pSoldier->aiData.ubNoiseVolume;		
			}

		lua_pushinteger(L, NoiseVolume);
	
return 1;
}


static int l_TogglePressureActionItemsInGridNo(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}
	
		TogglePressureActionItemsInGridNo( sGridNo );
	
return 0;
}



static int l_TriggerNPCWithIHateYouQuote(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
		TriggerNPCWithIHateYouQuote(ID);
	
return 0;
}

static int l_SetNewSituation(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
BOOLEAN Bool = FALSE;
SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
			pSoldier = FindSoldierByProfileID( ID, FALSE );
			if (pSoldier)
			{
			SetNewSituation(pSoldier);
			}
		
return 0;
}

static int l_SetNoiseGridno(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 sGridNo;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			pSoldier->aiData.sNoiseGridno = sGridNo;
		}
	
return 0;
}

static int l_SetNoiseVolume(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 NoiseVolume;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) NoiseVolume = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			pSoldier->aiData.ubNoiseVolume = NoiseVolume;
		}
	
return 0;
}

static int l_TacticalCharacterDialogue(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 Quote;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) Quote = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			TacticalCharacterDialogue (pSoldier,Quote);
		}
	
return 0;
}

static int l_SetAlertStatus(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 AlertStatus;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) AlertStatus = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			pSoldier->aiData.bAlertStatus = AlertStatus;
		}
	
return 0;
}

static int l_CheckAction(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 Action;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			Action = pSoldier->aiData.bAction; 
		}
		
	lua_pushinteger(L, Action);
	
return 1;
}

static int l_SetNextAction(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 NextAction;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) NextAction = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			pSoldier->aiData.bNextAction = NextAction;
		}

return 0;
}

static int l_ActiveTimedBombExists(lua_State *L)
{
UINT8  n = lua_gettop(L);

BOOLEAN Bool;

	 Bool = ActiveTimedBombExists ();
		
	lua_pushboolean(L, Bool);
	
return 1;
}


static int l_DoesSAMExistHere(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorX;
INT16 sSectorY;
INT16 sSectorZ;
INT32 sGridNo;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) sSectorZ = lua_tointeger(L,i);
		if (i == 4 ) sGridNo = lua_tointeger(L,i);
	}
	
	Bool =  DoesSAMExistHere( sSectorX, sSectorY, sSectorZ, sGridNo );
		
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_RemoveAllActiveTimedBombs(lua_State *L)
{
UINT8  n = lua_gettop(L);

RemoveAllActiveTimedBombs( );
	
return 0;
}

static int l_UpdateSAMDoneRepair(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorX;
INT16 sSectorY;
INT16 sSectorZ;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) sSectorZ = lua_tointeger(L,i);
	}
	
	UpdateSAMDoneRepair( sSectorX, sSectorY, sSectorZ );
	
return 0;
}

static int l_UpdateAndDamageSAMIfFound(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT16 sSectorX;
INT16 sSectorY;
INT16 sSectorZ;
INT32 sGridNo;
UINT8 ubDamage; 

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
		if (i == 3 ) sSectorZ = lua_tointeger(L,i);
		if (i == 4 ) sGridNo = lua_tointeger(L,i);
		if (i == 5 ) ubDamage = lua_tointeger(L,i);
	}
	
	UpdateAndDamageSAMIfFound( sSectorX, sSectorY, sSectorZ, sGridNo, ubDamage );
	
return 0;
}

static int l_ActivateSwitchInGridNo(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubID;
INT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
	}
	
	if ( sGridNo > 0 && ubID > -1 )
			ActivateSwitchInGridNo( ubID, sGridNo );
return 0;
}

static int l_HandleNPCTriggerNPC(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubTargetNPC;
UINT8 ubTargetRecord;
BOOLEAN fShowDialogueMenu;
UINT8 ubTargetApproach;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		if (i == 2 ) ubTargetRecord = lua_tointeger(L,i);
		if (i == 3 ) fShowDialogueMenu = lua_toboolean(L,i);
		if (i == 4 ) ubTargetApproach = lua_tointeger(L,i);
	}
	
	if ( ubTargetNPC > -1 )
			HandleNPCTriggerNPC ( ubTargetNPC, ubTargetRecord, fShowDialogueMenu, ubTargetApproach );
return 0;
}

static int l_HandleNPCGotoGridNo(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubTargetNPC;
INT32 usGridNo;
UINT8 ubQuoteNum;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		if (i == 2 ) usGridNo = lua_tointeger(L,i);
		if (i == 3 ) ubQuoteNum = lua_tointeger(L,i);
	}
	
	if ( ubTargetNPC > -1 )
			HandleNPCGotoGridNo (ubTargetNPC , usGridNo, ubQuoteNum);
return 0;
}

static int l_HandleNPCClosePanel(lua_State *L)
{
UINT8  n = lua_gettop(L);

	HandleNPCClosePanel ();
		
return 0;
}

static int l_HandleNPCDoAction(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT16 usActionCode; 
UINT8 ubQuoteNum;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) usActionCode = lua_tointeger(L,i);
		if (i == 3 ) ubQuoteNum = lua_tointeger(L,i);
	}
	
	if ( ID > -1 )
			HandleNPCDoAction (ID , usActionCode, ubQuoteNum);
		

return 0;
}

static int l_SetNextActionData(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 NextActionData;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) NextActionData = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			pSoldier->aiData.usNextActionData = NextActionData;
		}

return 0;
}

static int l_RESETTIMECOUNTER(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 Counter;

SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) Counter = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			RESETTIMECOUNTER( pSoldier->timeCounters.AICounter, Counter );
		}

return 0;
}

static int l_CancelAIAction(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			CancelAIAction( pSoldier, TRUE );
		}

return 0;
}

static int l_SoldierTo3DLocationLineOfSightTest(lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ID;
UINT32 sGridNo;
BOOLEAN Bool;
SOLDIERTYPE * pSoldier;
INT8 bLevel;
INT8 bCubeLevel;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
		if (i == 3 ) bLevel = lua_tointeger(L,i);
		if (i == 4 ) bCubeLevel = lua_tointeger(L,i);
	}
	
		pSoldier = FindSoldierByProfileID( ID, FALSE );
		if (pSoldier)
		{
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, bLevel, bCubeLevel, TRUE ) )
				Bool = TRUE;
			else
				Bool = FALSE;	
		}
		
			lua_pushboolean(L, Bool);
return 1;
}
//------------

static int l_NPCGotoGridNo (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT32 Gridno,sAdjustedGridNo;

UINT8 ID;
SOLDIERTYPE *		pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) Gridno = lua_tointeger(L,i);
	}

	if ( ID  >= 0 && Gridno >= 1 )
	{
				pSoldier = FindSoldierByProfileID( ID, FALSE );
				if (pSoldier)
				{
					if (NewOKDestination( pSoldier, Gridno, TRUE, 0 ) )
					{
						// go for it!
						NPCGotoGridNo( ID, Gridno, 1 );
					}
					else
					{
						sAdjustedGridNo = FindAdjacentGridEx( pSoldier, Gridno, NULL, NULL, FALSE, FALSE );
						if (sAdjustedGridNo != -1)
						{
							NPCGotoGridNo( ID, sAdjustedGridNo, 1 );
						}
					}
				}
	}
return 0;
}

//-------------------
//Maps function

static int l_ApplyMapChangesToMapTempFile (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Bool = lua_toboolean(L,i);
	}

	// Remove old graphic
	ApplyMapChangesToMapTempFile( Bool );
	
return 0;
}

/*
static int l_gTacticalStatus_Team (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool = FALSE;
INT32	cnt;
SOLDIERTYPE *pSoldier;
UINt8 Team;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Team = lua_tointeger(L,i);
	}
	
	
return 0;
}

*/
/*
static int l_AddExitGridToWorld (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
EXITGRID ExitGrid;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Bool = lua_toboolean(L,i);
	}

		ExitGrid.ubGotoSectorX = 8;
		ExitGrid.ubGotoSectorY = 4;
		ExitGrid.ubGotoSectorZ = 1;
	
		AddExitGridToWorld( 6004, &ExitGrid );			
		AddExitGridToWorld( 6005, &ExitGrid );
			
		AddExitGridToWorld( 6164, &ExitGrid );			
		AddExitGridToWorld( 6165, &ExitGrid );
	
return 0;
}
*/

static int l_RemoveStruct (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool = FALSE;
UINT16 usTileIndex;
UINT32 Gridno, uiCheckType;
UINT16 usSubIndex;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiCheckType = lua_tointeger(L,i);
		if (i == 2 ) usSubIndex = lua_tointeger(L,i);
		if (i == 3 ) Gridno = lua_tointeger(L,i);
	}

	GetTileIndexFromTypeSubIndex( uiCheckType, usSubIndex, &usTileIndex );
	RemoveStruct( Gridno, usTileIndex );
	
return 0;
}

static int l_AddStructToHead (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool = FALSE;
UINT16 usTileIndex;
UINT32 Gridno, uiCheckType;
UINT16 usSubIndex;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiCheckType = lua_tointeger(L,i);
		if (i == 2 ) usSubIndex = lua_tointeger(L,i);
		if (i == 3 ) Gridno = lua_tointeger(L,i);
	}

	GetTileIndexFromTypeSubIndex( uiCheckType, usSubIndex, &usTileIndex );
	AddStructToHead( Gridno, usTileIndex );
	
return 0;
}

static int l_ConvertGridNoToXY (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN Bool = FALSE;
UINT32 Gridno,dis;
INT16 sX, sY;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Gridno = lua_tointeger(L,i);
		if (i == 2 ) dis = lua_tointeger(L,i);
	}

	// Redo movement costs....
	ConvertGridNoToXY( Gridno, &sX, &sY ); 
	RecompileLocalMovementCostsFromRadius( Gridno, dis );
	
return 0;
}

static int l_SetRender (lua_State *L)
{
UINT8  n = lua_gettop(L);


	gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;
	
	InvalidateWorldRedundency( );
	SetRenderFlags(RENDER_FLAG_FULL);
	
return 0;
}
 
//-----------------
static int l_SetUpHelicopterForPlayer (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT16 SextorX;
UINT16 SextorY;

UINT8 SkyDrive;
UINT8 vehicle = 163;	// Default: Helicopter

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) SextorX = lua_tointeger(L,i);
		if (i == 2 ) SextorY = lua_tointeger(L,i);
		if (i == 3 ) SkyDrive = lua_tointeger(L,i);
		if (i == 4 ) vehicle = lua_tointeger(L,i);
	}

	SetUpHelicopterForPlayer( SextorX, SextorY, SkyDrive, vehicle );
	
return 0;
}

//-----Ai Action------------

//open door
static int l_ACTION_ITEM_OPEN_DOOR (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;


OBJECTTYPE DoorCloser;

UINT32 sGridNo;

STRUCTURE *pStructure;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

		pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
		if (pStructure)
		{
			if (pStructure->fFlags & STRUCTURE_OPEN)
			{
				// it's already open - this MIGHT be an error but probably not
				// because we are basically just ensuring that the door is open
			}
			else
			{
				if (pStructure->fFlags & STRUCTURE_BASE_TILE)
				{
					HandleDoorChangeFromGridNo( NULL, sGridNo, FALSE );
				}
				else
				{
					HandleDoorChangeFromGridNo( NULL, pStructure->sBaseGridNo, FALSE );
				}
				gfExplosionQueueMayHaveChangedSight = TRUE;
			}
		}
	
return 0;
}	

//Close Door
static int l_ACTION_ITEM_CLOSE_DOOR (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;


OBJECTTYPE DoorCloser;

UINT32 sGridNo;

STRUCTURE *pStructure;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

		pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
		if (pStructure)
		{
			if (pStructure->fFlags & STRUCTURE_OPEN)
			{
				if (pStructure->fFlags & STRUCTURE_BASE_TILE)
				{
					HandleDoorChangeFromGridNo( NULL, sGridNo , FALSE );
				}
				else
				{
					HandleDoorChangeFromGridNo( NULL, pStructure->sBaseGridNo, FALSE );
				}
				gfExplosionQueueMayHaveChangedSight = TRUE;
			}
			else
			{
				// it's already closed - this MIGHT be an error but probably not
				// because we are basically just ensuring that the door is closed
			}
		}
	
return 0;
}	


static int l_ACTION_ITEM_TOGGLE_DOOR (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;
STRUCTURE *pStructure;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

		pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
		if (pStructure)
		{
			if (pStructure->fFlags & STRUCTURE_BASE_TILE)
			{
				HandleDoorChangeFromGridNo( NULL, sGridNo, FALSE );
			}
			else
			{
				HandleDoorChangeFromGridNo( NULL, pStructure->sBaseGridNo , FALSE );
			}
			gfExplosionQueueMayHaveChangedSight = TRUE;
		}
	
return 0;
}	

//locked unlocked door
static int l_ACTION_ITEM_UNLOCK_DOOR (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;
DOOR * pDoor;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}
			pDoor = FindDoorInfoAtGridNo( sGridNo );
			if ( pDoor )
			{
				pDoor->fLocked = FALSE;
			}
	
return 0;
}			

static int l_ACTION_ITEM_TOGGLE_LOCK (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;
DOOR * pDoor;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}
			pDoor = FindDoorInfoAtGridNo( sGridNo );
			if ( pDoor )
			{
				if ( pDoor->fLocked )
				{
					pDoor->fLocked = FALSE;
				}
				else
				{
					pDoor->fLocked = TRUE;
				}
			}

return 0;
}	

static int l_ACTION_ITEM_UNTRAP_DOOR (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;
DOOR * pDoor;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}
			pDoor = FindDoorInfoAtGridNo( sGridNo );
			if ( pDoor )
			{
				pDoor->ubTrapLevel = 0;
				pDoor->ubTrapID = NO_TRAP;
			}
		
return 0;
}	

static int l_ACTION_ITEM_SMALL_PIT (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

		Add3X3Pit( sGridNo );
		SearchForOtherMembersWithinPitRadiusAndMakeThemFall( sGridNo, 1 );
		
return 0;
}

static int l_SearchForOtherMembersWithinPitRadiusAndMakeThemFall (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;
INT16 sRadius;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
		if (i == 2 ) sRadius = lua_tointeger(L,i);
	}
		SearchForOtherMembersWithinPitRadiusAndMakeThemFall( sGridNo, sRadius );
		
return 0;
}

static int l_Add5X5Pit (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}
		Add5X5Pit( sGridNo );
		
return 0;
}

static int l_Add3X3Pit (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}
		Add3X3Pit( sGridNo );
		
return 0;
}

static int l_ACTION_ITEM_LARGE_PIT (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

		Add5X5Pit( sGridNo );
		SearchForOtherMembersWithinPitRadiusAndMakeThemFall( sGridNo, 2 );
		
return 0;
}

static int l_ACTION_ITEM_TOGGLE_ACTION1 (lua_State *L)
{
UINT8  n = lua_gettop(L);
OBJECTTYPE DoorCloser;

	ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 1 );
		
return 0;
}
		
static int l_ACTION_ITEM_TOGGLE_ACTION2 (lua_State *L)
{
UINT8  n = lua_gettop(L);
OBJECTTYPE DoorCloser;

	ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 2 );
		
return 0;
}

static int l_ACTION_ITEM_TOGGLE_ACTION3 (lua_State *L)
{
UINT8  n = lua_gettop(L);
OBJECTTYPE DoorCloser;

	ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 3 );
		
return 0;
}

static int l_ACTION_ITEM_TOGGLE_ACTION4 (lua_State *L)
{
UINT8  n = lua_gettop(L);
OBJECTTYPE DoorCloser;

	ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 4 );
		
return 0;
}

static int l_ACTION_ITEM_TOGGLE_PRESSURE_ITEMS (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

TogglePressureActionItemsInGridNo( sGridNo );
		
return 0;
}

static int l_ACTION_ITEM_ENTER_BROTHEL (lua_State *L)
{
UINT8  n = lua_gettop(L);
OBJECTTYPE DoorCloser;
	
return 0;
}

static int l_ACTION_ITEM_EXIT_BROTHEL (lua_State *L)
{
UINT8  n = lua_gettop(L);
OBJECTTYPE DoorCloser;
	
return 0;
}

static int l_ACTION_ITEM_KINGPIN_ALARM (lua_State *L)
{
UINT8  n = lua_gettop(L);
OBJECTTYPE DoorCloser;
		
return 0;
}

static int l_ACTION_ITEM_SEX (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

	if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			UINT8	ubID;
			OBJECTTYPE DoorCloser;
			INT16	sTeleportSpot;
			INT16	sDoorSpot;
			UINT8	ubDirection;
			UINT8	ubRoom, ubOldRoom;

			ubID = WhoIsThere2( sGridNo, 0 );
			if ( (ubID != NOBODY) && (MercPtrs[ ubID ]->bTeam == gbPlayerNum) )
			{
				if ( InARoom( sGridNo, &ubRoom ) && InARoom( MercPtrs[ ubID ]->sOldGridNo, &ubOldRoom ) && ubOldRoom != ubRoom )
				{
					// also require there to be a miniskirt civ in the room
					if ( HookerInRoom( ubRoom ) )
					{

						// stop the merc...
						MercPtrs[ ubID ]->EVENT_StopMerc( MercPtrs[ ubID ]->sGridNo, MercPtrs[ ubID ]->ubDirection );

						switch( sGridNo )
						{
						case 13414:
							sDoorSpot = 13413;
							sTeleportSpot = 13413;
							break;
						case 11174:
							sDoorSpot = 11173;
							sTeleportSpot = 11173;
							break;
						case 12290:
							sDoorSpot = 12290;
							sTeleportSpot = 12291;
							break;

						default:

							sDoorSpot = NOWHERE;
							sTeleportSpot = NOWHERE;


						}
						
						if (!TileIsOutOfBounds(sDoorSpot) && !TileIsOutOfBounds(sTeleportSpot) )
						{
							// close the door...
							DoorCloser[0]->data.misc.bActionValue = ACTION_ITEM_CLOSE_DOOR;
							PerformItemAction( sDoorSpot, &DoorCloser );

							// have sex
							HandleNPCDoAction( 0, NPC_ACTION_SEX, 0 );

							// move the merc outside of the room again
							sTeleportSpot = FindGridNoFromSweetSpotWithStructData( MercPtrs[ ubID ], STANDING, sTeleportSpot, 2, &ubDirection, FALSE );
							MercPtrs[ ubID ]->ChangeSoldierState( STANDING, 0, TRUE );
							TeleportSoldier( MercPtrs[ ubID ], sTeleportSpot, FALSE );

							HandleMoraleEvent( MercPtrs[ ubID ], MORALE_SEX, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
							FatigueCharacter( MercPtrs[ ubID ] );
							FatigueCharacter( MercPtrs[ ubID ] );
							FatigueCharacter( MercPtrs[ ubID ] );
							FatigueCharacter( MercPtrs[ ubID ] );
							DirtyMercPanelInterface( MercPtrs[ ubID ], DIRTYLEVEL1 );
						}
					}

				}

			}
		}

		
return 0;
}

static int l_ACTION_ITEM_REVEAL_ROOM (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

			UINT8 ubRoom;
			if ( InAHiddenRoom( sGridNo, &ubRoom ) )
			{
				RemoveRoomRoof( sGridNo, ubRoom, NULL );
			}
			
return 0;
}

static int l_ACTION_ITEM_LOCAL_ALARM (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

MakeNoise( NOBODY, sGridNo, 0, gpWorldLevelData[sGridNo].ubTerrainID, 30, NOISE_SILENT_ALARM );
			
return 0;
}


static int l_ACTION_ITEM_GLOBAL_ALARM (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

CallAvailableEnemiesTo( sGridNo );
			
return 0;
}

static int l_ACTION_ITEM_BLOODCAT_ALARM (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

CallAvailableTeamEnemiesTo( sGridNo, CREATURE_TEAM );
			
return 0;
}

static int l_ACTION_ITEM_KLAXON (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 5, SoundDir( sGridNo ) );
			
return 0;
}

static int l_ACTION_ITEM_MUSEUM_ALARM (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 sGridNo;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}

		PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 5, SoundDir( sGridNo ) );
		CallEldinTo( sGridNo );
		
return 0;
}

//------------------------------------------------		
static int l_Action_door (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
BOOLEAN Bool;
UINT32 Gridno;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Bool = lua_toboolean(L,i);
		if (i == 2 ) Gridno = lua_tointeger(L,i);

	}

		
		if (Bool == FALSE)
			{	
				DoorCloser[0]->data.misc.bActionValue = ACTION_ITEM_CLOSE_DOOR;
					PerformItemAction ( Gridno, &DoorCloser );
			}
			else
			{		
				DoorCloser[0]->data.misc.bActionValue = ACTION_ITEM_OPEN_DOOR;
					PerformItemAction ( Gridno, &DoorCloser );
			}
	
return 0;
}			

static int l_Action_door_open (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 Gridno;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Gridno = lua_tointeger(L,i);

	}
		DoorCloser[0]->data.misc.bActionValue = ACTION_ITEM_OPEN_DOOR;
		PerformItemAction ( Gridno, &DoorCloser );
		
return 0;
}	

static int l_Action_door_close (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
OBJECTTYPE DoorCloser;
UINT32 Gridno;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Gridno = lua_tointeger(L,i);

	}
		DoorCloser[0]->data.misc.bActionValue = ACTION_ITEM_CLOSE_DOOR;
		PerformItemAction ( Gridno, &DoorCloser );
			
return 0;
}	
//-----------------------------------

//action
static int l_SetOffBombsByFrequency (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 ACTION;
UINT8 ID;
SOLDIERTYPE *		pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ACTION = lua_tointeger(L,i);
		if (i == 2 ) ID = lua_tointeger(L,i);
	}

	if (ACTION >= 1 || ACTION <=4)
	{
			pSoldier = FindSoldierByProfileID( ID, TRUE );
			if (pSoldier)
			{
				SetOffBombsByFrequency( pSoldier->ubID, FIRST_MAP_PLACED_FREQUENCY + ACTION );
			}
	}
return 0;
}


//action
static int l_TeleportSoldier (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 Gridno;
UINT8 ID;
SOLDIERTYPE *		pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ID = lua_tointeger(L,i);
		if (i == 2 ) Gridno = lua_tointeger(L,i);
	}

	if (ID  >= 0 && Gridno >=1)
	{
			pSoldier = FindSoldierByProfileID( ID, TRUE );
			if (pSoldier)
			{
				TeleportSoldier( pSoldier, Gridno, TRUE );
			}
	}
return 0;
}

//-------------------------------------------

static int l_SetGlobalLuaBoolFalse (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT32 val;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
	}

	gLuaGlobal[val].fGlobalLuaBool = FALSE;
	
	
return 0;
}

static int l_SetGlobalLuaBoolTrue (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 val;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
	}

	gLuaGlobal[val].fGlobalLuaBool = TRUE;
	
	
return 0;
}

static int l_SetGlobalLuaBool (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
BOOLEAN set;
UINT32 val;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
		if (i == 2 ) set = lua_toboolean(L,i);
	}

if ( val >=0 || 1000<= val ) 
	gLuaGlobal[val].fGlobalLuaBool = set;
	
return 0;
}

static int l_SetGlobalLuaVal (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 val;
INT32 set;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}

if ( val >=0 ||  1000<= val ) 
	gLuaGlobal[val].iGlobalLuaVal = set;
	
return 0;
}

static int l_GetGlobalLuaVal (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 val;
INT32 set;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
	}

	set = gLuaGlobal[val].iGlobalLuaVal;
	
		lua_pushinteger(L, set);
	
return 1;
}

static int l_CheckGlobalLuaBool (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 val;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
		//if (i == 2 ) set = lua_toboolean(L,i);
	}

	Bool = gLuaGlobal[val].fGlobalLuaBool;

	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_CheckGlobalLuaVal (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 val;
INT32 set;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) val = lua_tointeger(L,i);
	}

	 set = gLuaGlobal[val].iGlobalLuaVal;

	lua_pushinteger(L, set);
	
return 1;
}

static int l_ItemTypeExistsAtLocation (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT32 GridNo,ItemIndex;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ItemIndex = lua_tointeger(L,i);
		if (i == 2 ) GridNo = lua_tointeger(L,i);
	}

	 Bool = ItemTypeExistsAtLocation( GridNo, ItemIndex, 0, NULL );	

	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_fCivGroupHostile (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 Group;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Group = lua_tointeger(L,i);
	}

	 Bool = gTacticalStatus.fCivGroupHostile[ Group ];
	
	lua_pushboolean(L, Bool);
	
return 1;
}

static int l_fEnemyControlled (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

BOOLEAN Bool;
UINT16 id;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	 Bool = StrategicMap[ id ].fEnemyControlled;
		
	lua_pushboolean(L, Bool);
	
return 1;
}
 
//anim
static int l_EVENT_InitNewSoldierAnim (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;


SOLDIERTYPE *		pSoldier;
UINT8 ubTargetNPC;

UINT32 ANIM;
INT32 cnt;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		//if (i == 2 ) BodyType = lua_tointeger(L,i);
		if (i == 2 ) ANIM = lua_tointeger(L,i);
		if (i == 3 ) cnt = lua_tointeger(L,i);
		//if (i == 4 ) PlayerControl = lua_tointeger(L,i);
	}

		if (ubTargetNPC > -1)
		{
			pSoldier = FindSoldierByProfileID ( ubTargetNPC, TRUE );
			if ( pSoldier )//&& pSoldier->ubBodyType == BodyType )
			{
				DeleteTalkingMenu();
				pSoldier->EVENT_InitNewSoldierAnim( ANIM, 0, TRUE );
			}	
		}
		else if (ubTargetNPC == -1)
		{
		
	
		if ( MercPtrs[ cnt ]->bInSector )
			MercPtrs[ cnt ]->EVENT_InitNewSoldierAnim( ANIM, 0, TRUE );

		}
		
return 0;
}

static int l_EnterShopKeeperInterfaceScreen (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

UINT8 ubCharacterNum;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCharacterNum = lua_tointeger(L,i);
	}

		if( HandleShopKeepHasBeenShutDown( ubCharacterNum ) == FALSE )
		{
			DeleteTalkingMenu( );
			EnterShopKeeperInterfaceScreen( ubCharacterNum );
		}
						
return 0;
}

static int l_CheckCombatMode (lua_State *L)
{
UINT8  n = lua_gettop(L);
BOOLEAN Bool;

	Bool = ( gTacticalStatus.uiFlags & INCOMBAT );

lua_pushboolean(L, Bool);
						
return 1;
}

//---------------------------------------------
static int l_SetEnterCombatMode (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT16 group;
UINT8		ubLoop;
SOLDIERTYPE *		pGoon;
UINT8 ubID;
				
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) group = lua_tointeger(L,i);
		if (i == 2 ) ubID = lua_tointeger(L,i);
	}

						for ( ubLoop = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; ubLoop <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; ubLoop++ )
						{
						pGoon = MercPtrs[ ubLoop ];
							if ( pGoon->ubCivilianGroup == group && pGoon->bActive && pGoon->bInSector && pGoon->stats.bLife >= OKLIFE && pGoon->aiData.bOppList[ ubID ] == SEEN_CURRENTLY )
								{
									MakeCivHostile( pGoon, 2 );
								if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
									{
									EnterCombatMode( pGoon->bTeam );
									}
								}
						}
	
return 0;
}

static int l_SetEnterCombatModeTeam (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 Team;
				
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Team = lua_tointeger(L,i);
	}
	
			if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
			{
				EnterCombatMode( Team );
			}
	
return 0;
}

static int l_MakeMercPtrsHostile (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubID;
				
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}

	
		MakeCivHostile( MercPtrs[ ubID ], 2 );
	
return 0;
}


static int l_ChangeNpcToDifferentSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubNpcId;
INT16 sSectorX;
INT16 sSectorY;
INT8 bSectorZ;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubNpcId  = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
		if (i == 4 ) bSectorZ = lua_tointeger(L,i);
	}

	ChangeNpcToDifferentSector( ubNpcId, sSectorX, sSectorY, bSectorZ );
	
return 0;
}

static int l_PlayerInARoom (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 ubRoom,ubRoom2;
UINT32 sGridNo;
BOOLEAN Bool;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
		if (i == 2 ) ubRoom2 = lua_tointeger(L,i);
	}
	
	if ( InARoom( sGridNo, &ubRoom ) && ubRoom == ubRoom2 )
	{
		Bool = TRUE;
	}
	else
	{
		Bool = FALSE;
	}
	
	lua_pushboolean(L, Bool);
	
return 1;
}


static int l_EVENT_SoldierGotHit (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
SOLDIERTYPE *pTarget2;
UINT8 UID;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		if ( gMercProfiles[ UID ].bMercStatus  != MERC_IS_DEAD )
		{
				pTarget2 = FindSoldierByProfileID( UID, FALSE );
				if (pTarget2)
				{
					DeleteTalkingMenu();
						if ( pTarget2->stats.bLife >= 0 ) 
						{
						pTarget2->EVENT_SoldierGotHit( 1, 100, 10, pTarget2->ubDirection, 320, NOBODY , FIRE_WEAPON_NO_SPECIAL, AIM_SHOT_TORSO, 0, NOWHERE );
						}			
				}
		}
	
return 0;
}

/*
static int l_SetSoldierSide (lua_State *L)
{

UINT8  n = lua_gettop(L);
int i;
INT8 bNewSide;
SOLDIERTYPE *pSoldier;
UINT8 ubID;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) bNewSide = lua_tointeger(L,i);
	}

		if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive )
			Bool = TRUE;
		else
			Bool = FALSE;
			
		if ( Bool == TRUE )
		{	pSoldier = MercPtrs[ ubID ];
			if (pSoldier)
			{
			pSoldier->bSide = 	bNewSide;
			}
		}

return 0;
}

*/

static int l_SetSoldierSide (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 bNewSide;
SOLDIERTYPE *pSoldier;
UINT8 UID;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
		if (i == 2 ) bNewSide = lua_tointeger(L,i);
	}

	if ( UID != NO_PROFILE ) 
	{
		pSoldier = FindSoldierByProfileID( UID, FALSE );
		if (pSoldier)
			{
				 pSoldier->bSide = bNewSide;
			}
	}
	else
	{
		pSoldier = FindSoldierByProfileID_( UID );
		if ( pSoldier->ubProfile == UID ) 
		{
			if (pSoldier)
			{
				 pSoldier->bSide = bNewSide;
			}	
		}
	}
						
return 0;
}


static int l_CheckSoldierSide (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 bNewSide;
SOLDIERTYPE *pSoldier;
UINT8 UID;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}
	
		
	if ( UID != NO_PROFILE ) 
	{
		pSoldier = FindSoldierByProfileID( UID, FALSE);
		if (pSoldier)
			{
				bNewSide = pSoldier->bSide;
			}
	}
	else
	{
		pSoldier = FindSoldierByProfileID_( UID );
		if ( pSoldier->ubProfile == UID ) 
		{
			if (pSoldier)
			{
				bNewSide = pSoldier->bSide;
			}	
		}
	}
		
	lua_pushinteger(L, bNewSide);
										
return 1;
}

static int l_CheckSoldierNeutral (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

SOLDIERTYPE *pSoldier;
UINT8 UID;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

	
	if ( UID != NO_PROFILE ) 
	{
		pSoldier = FindSoldierByProfileID( UID, FALSE );
		if (pSoldier)
			{
				if ( pSoldier->aiData.bNeutral )
					Bool = TRUE;
				else
					Bool = FALSE;
			}
	}
	else
	{
		pSoldier = FindSoldierByProfileID_( UID );
		if ( pSoldier->ubProfile == UID ) 
		{
			if (pSoldier)
			{
				if ( pSoldier->aiData.bNeutral )
					Bool = TRUE;
				else
					Bool = FALSE;
			}	
		}
	}
			
			
	lua_pushboolean(L, Bool);
						
return 1;
}

static int l_CheckSoldierCivilianGroup (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 bGroup;
SOLDIERTYPE *pSoldier;
UINT8 UID;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
	if (pSoldier)
		{
			bGroup = pSoldier->ubCivilianGroup;
		}
		
	lua_pushinteger(L, bGroup);
										
return 1;
}

static int l_CheckSoldierubProfile (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 bGroup;
SOLDIERTYPE *pSoldier;
UINT8 UID;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
	if (pSoldier)
		{
		
			bGroup = pSoldier->ubProfile;
		}
		
	lua_pushinteger(L, bGroup);
										
return 1;
}

static int l_SetSoldierOrders (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

SOLDIERTYPE *pSoldier;
UINT8 Orders , UID;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
		if (i == 2 ) Orders = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
	if (pSoldier)
		{
		
			 pSoldier->aiData.bOrders = Orders;
		}
										
return 0;
}

static int l_CheckForPotentialAddToBattleIncrement (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

SOLDIERTYPE *pSoldier;
UINT8 UID;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
		
	if (pSoldier)
		CheckForPotentialAddToBattleIncrement( pSoldier );
								
return 0;
}

static int l_RecalculateOppCntsDueToNoLongerNeutral (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

SOLDIERTYPE *pSoldier;
UINT8 UID;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
		
	if (pSoldier)
		RecalculateOppCntsDueToNoLongerNeutral( pSoldier );
			
							
return 0;
}

static int l_CheckSoldierActive (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

SOLDIERTYPE *pSoldier;
UINT8 UID;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
	if (pSoldier)
		{
			if ( pSoldier->bActive )
				Bool = TRUE;
			else
				Bool = FALSE;
		}
		
		lua_pushboolean(L, Bool);
								
return 1;
}

static int l_CheckSoldierInSector (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;

SOLDIERTYPE *pSoldier;
UINT8 UID;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
	if (pSoldier)
		{
			if ( pSoldier->bInSector ) 
				Bool = TRUE;
			else
				Bool = FALSE;
		}
		
		lua_pushboolean(L, Bool);
								
return 1;
}

static int l_GetSoldierTeam (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 Side;
SOLDIERTYPE *pSoldier;
UINT8 UID;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
			if (pSoldier)
				{
					Side = pSoldier->bSide;
				}	
				
		lua_pushinteger(L, Side);
		
return 1;
}

static int l_ChangeSoldierTeam (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 Side;
SOLDIERTYPE *pSoldier;
UINT8 UID;


	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
		if (i == 2 ) Side = lua_tointeger(L,i);
	}

		pSoldier = FindSoldierByProfileID( UID, FALSE );
	if (pSoldier)
		{
		
			ChangeSoldierTeam( pSoldier, Side );
		}
		
							
return 0;
}

static int l_ChangeMercPtrsTeam (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
INT8 Side;
SOLDIERTYPE *pSoldier;
UINT8 UID;
BOOLEAN Bool;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) UID = lua_tointeger(L,i);
		if (i == 2 ) Side = lua_tointeger(L,i);
	}
	
		if ( MercPtrs[ UID ]->bInSector && MercPtrs[ UID ]->bActive )
			Bool = TRUE;
		else
			Bool = FALSE;
			
		if ( Bool == TRUE )
		{	pSoldier = MercPtrs[ UID ];
			if (pSoldier)
			{
			pSoldier->bSide = Side;
			}
		}
						
return 0;
}

//-------------------


static int l_ExecuteStrategicAIAction (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT16 usActionCode;// See list in "interface Dialogue.h"
INT16 sSectorX;		// Target X location of strategic action
INT16 sSectorY;		// Target Y location of strategic action
INT32 option1 = 0;	// Option 1, vaies with action
INT32 option2 = 0;	// Option 2, vaies with action

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) usActionCode = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
		if (i == 4 ) option1 = lua_tointeger(L,i);
		if (i == 5 ) option2 = lua_tointeger(L,i);
	}

	ExecuteStrategicAIAction( usActionCode, sSectorX, sSectorY, option1, option2);	
	
return 0;
}

//AddEmailXML
static int l_AddEmailXML2 (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;

	//INT32 iMessageOffset;
	//INT32 iMessageLength;
	UINT8 ubSender;
	UINT8 uiIndex;
	//INT32 iCurrentIMPPosition;
	
	UINT8 pMerc = 0;
	UINT8 iMerc = 0;
	UINT8 oMerc = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiIndex = lua_tointeger(L,i);
		//if (i == 1 ) iMessageOffset = lua_tointeger(L,i);
		//if (i == 2 ) iMessageLength = lua_tointeger(L,i);
		if (i == 2 ) ubSender = lua_tointeger(L,i);
		//if (i == 4 ) iCurrentIMPPosition = lua_tointeger(L,i);
	}
	
		oMerc = uiIndex;
		iMerc = oMerc * 1;
						
		if ( oMerc != 0 )
			pMerc = oMerc + 1;
		else
			pMerc = 0;
			
		AddEmailTypeXML(pMerc,iMerc, ubSender, GetWorldTotalMin(), -1, TYPE_EMAIL_OTHER);
	//AddEmailTypeXML(iMessageOffset,iMessageLength, ubSender, GetWorldTotalMin(), iCurrentIMPPosition, TYPE_EMAIL_AIM_AVAILABLE);
	return 0;
}

//AddEmailXML
static int l_AddEmailXML (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;

	//INT32 iMessageOffset;
	//INT32 iMessageLength;
	//UINT8 ubSender;
	UINT8 uiIndex;
	//INT32 iCurrentIMPPosition;
	
	UINT8 pMerc = 0;
	UINT8 iMerc = 0;
	UINT8 oMerc = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiIndex = lua_tointeger(L,i);
		//if (i == 1 ) iMessageOffset = lua_tointeger(L,i);
		//if (i == 2 ) iMessageLength = lua_tointeger(L,i);
		//if (i == 3 ) ubSender = lua_tointeger(L,i);
		//if (i == 4 ) iCurrentIMPPosition = lua_tointeger(L,i);
	}
	
		oMerc = uiIndex;
		iMerc = oMerc * 1;
						
		if ( oMerc != 0 )
			pMerc = oMerc + 1;
		else
			pMerc = 0;
			
	if ( gProfilesAIM[uiIndex].ProfilId == uiIndex )
		AddEmailTypeXML(pMerc,iMerc, iMerc, GetWorldTotalMin(), -1, TYPE_EMAIL_AIM_AVAILABLE);
	//AddEmailTypeXML(iMessageOffset,iMessageLength, ubSender, GetWorldTotalMin(), iCurrentIMPPosition, TYPE_EMAIL_AIM_AVAILABLE);
	return 0;
}

//AddEmailXML
static int l_AddEmailLevelUpXML (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;

	//INT32 iMessageOffset;
	//INT32 iMessageLength;
	//UINT8 ubSender;
	UINT8 uiIndex;
	//INT32 iCurrentIMPPosition;
	
	UINT8 pMerc = 0;
	UINT8 iMerc = 0;
	UINT8 oMerc = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiIndex = lua_tointeger(L,i);
		//if (i == 1 ) iMessageOffset = lua_tointeger(L,i);
		//if (i == 2 ) iMessageLength = lua_tointeger(L,i);
		//if (i == 3 ) ubSender = lua_tointeger(L,i);
		//if (i == 4 ) iCurrentIMPPosition = lua_tointeger(L,i);
	}
	
		oMerc = uiIndex;
		iMerc = oMerc * 1;
						
		if ( oMerc != 0 )
			pMerc = oMerc + 1;
		else
			pMerc = 0;
			
	if ( gProfilesMERC[uiIndex].ProfilId == uiIndex )
		AddEmailTypeXML(pMerc,iMerc, iMerc, GetWorldTotalMin(), -1, TYPE_EMAIL_MERC_LEVEL_UP);
	//AddEmailTypeXML(iMessageOffset,iMessageLength, ubSender, GetWorldTotalMin(), iCurrentIMPPosition, TYPE_EMAIL_MERC_LEVEL_UP);
	return 0;
}

//AddEmail
static int l_AddEmail (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;

	INT32 iMessageOffset;
	INT32 iMessageLength;
	UINT8 ubSender;
	INT32 iCurrentIMPPosition;
	INT16 iCurrentShipmentDestinationID = -1;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) iMessageOffset = lua_tointeger(L,i);
		if (i == 2 ) iMessageLength = lua_tointeger(L,i);
		if (i == 3 ) ubSender = lua_tointeger(L,i);
		if (i == 4 ) iCurrentIMPPosition = lua_tointeger(L,i);
		if (i == 5) iCurrentShipmentDestinationID = lua_tointeger(L,i);
	}

	AddEmail(iMessageOffset,iMessageLength,ubSender,	GetWorldTotalMin(), iCurrentIMPPosition, iCurrentShipmentDestinationID, TYPE_EMAIL_EMAIL_EDT);	
	
	return 0;
}
	
//AddPreReadEmail	
static int l_AddPreReadEmail (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;


INT32 iMessageOffset;
INT32 iMessageLength;
UINT8 ubSender;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) iMessageOffset = lua_tointeger(L,i);
		if (i == 2 ) iMessageLength = lua_tointeger(L,i);
		if (i == 3 ) ubSender = lua_tointeger(L,i);
	}

	AddPreReadEmail(iMessageOffset,iMessageLength,ubSender,	GetWorldTotalMin(), TYPE_EMAIL_EMAIL_EDT);	
	
return 0;
}
	
//gfBoxerFought
static int l_SetgfBoxerFought (lua_State *L)
{
UINT8  n = lua_gettop(L);
UINT8 id;
BOOLEAN Bool;
int i;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,n);
		if (i == 2 ) Bool = lua_toboolean(L,n);
	}

gfBoxerFought[id] = Bool;
		
return 0;
}

static int l_GetWorldHour (lua_State *L)
{
UINT32 h2;

h2 = GetWorldHour();

lua_pushinteger(L, h2);
		
return 1;
}

static int l_GetWorldMinutesInDay (lua_State *L)
{
UINT32 uiTime;

uiTime = GetWorldMinutesInDay();

lua_pushinteger(L, uiTime);
		
return 1;
}

//---------------------------------------------------------------------------------------

BOOLEAN LetLuaHandleEarlyMorningEvents(UINT8 Init)
{
	char * filename = "scripts\\StrategicEventHandler.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);
		SGP_THROW(msg);
	}

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);
	
	//init function
//	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
//	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}

	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleEarlyMorningEvents");
		lua_call(L,0,0); 
	}
	/*
	if ( Init == 1 )
	{
		lua_getglobal(L , "HandlePossiblyDamagedPackage");
		lua_call(L,0,0); 
	}
	
	if ( Init == 2 )
	{
		lua_getglobal(L , "AddSecondAirportAttendant");
		lua_call(L,0,0); 
	}
	
	if ( Init == 3 )
	{
		lua_getglobal(L , "SetPabloToUnbribed");
		lua_call(L,0,0); 
	}
	
	if ( Init == 4 )
	{
		lua_getglobal(L , "CheckForMissingHospitalSupplies");
		lua_call(L,0,0); 
	}
	*/
	lua_close(L);

	delete[] buffer;

	return true;

}

/*
BOOLEAN LetLuaHandleNPCSystemEvent( UINT32 uiEvent, UINT8 Init)
{
	char * filename = "scripts\\StrategicEventHandler.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
		return false;

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);


	lua_State *L = lua_open();
	luaL_openlibs(L);

	//init function
	lua_register(L, "CheckFact", l_CheckFact);	
	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}

	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleNPCSystemEvent");
		lua_pushnumber(L, uiEvent);
		lua_call(L,1,0); 
	}
	
	lua_close(L);

	delete[] buffer;

	return true;

}
*/
//---------------

static int l_FindItemSoldier (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;


SOLDIERTYPE *pSoldier;
UINT8 ubTargetNPC;
UINT32 ItemIndex;
UINT32 Grido;
INT8 bItemIn;
BOOLEAN fPlayerMercsOnly;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		if (i == 2 ) ItemIndex = lua_tointeger(L,i);
		if (i == 3 ) Grido = lua_tointeger(L,i);
		if (i == 4 ) fPlayerMercsOnly = lua_toboolean(L,i);
	}

			pSoldier = FindSoldierByProfileID( ubTargetNPC, fPlayerMercsOnly);
			if (pSoldier)
			{
				bItemIn = FindObj( pSoldier, ItemIndex );
				if (bItemIn != NO_SLOT)
					{
					AddItemToPool( Grido, &(pSoldier->inv[bItemIn]), -1 , 0, 0, 0 );
					DeleteObj( &(pSoldier->inv[ bItemIn ]) );
					RemoveObjectFromSoldierProfile( ubTargetNPC, ItemIndex );
					}
			}
return 0;
}

//Create items
static int l_CreateItem (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;
	INT32				iWorldItem;
	OBJECTTYPE	Object;	
	INT32 sGridNo;
	UINT16 usItem;
	INT16 bStatus;
	UINT8 bLevel;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) usItem = lua_tointeger(L,i);
		if (i == 2 ) bStatus = lua_tointeger(L,i);
		if (i == 3 ) sGridNo = lua_tointeger(L,i);
		if (i == 4 ) bLevel = lua_tointeger(L,i);
	}

	CreateItem( usItem, bStatus, &Object ); 
	AddItemToPoolAndGetIndex( sGridNo, &Object, -1, bLevel, 0, 0, -1, &iWorldItem );

	return 0;
}

//Create items
static int l_CreateToUnLoadedSector (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;
	OBJECTTYPE	Object;	
	INT32 sGridNo;
	UINT16 usItem;
	INT16 bStatus;
	INT16 sMapX;
	INT16 sMapY; 
	INT8 bMapZ;
	UINT32 uiNumberOfItems;
	UINT8 ubLevel;
	UINT16 usFlags; 
	INT8 bRenderZHeightAboveLevel;
	INT8 bVisible;
	BOOLEAN fReplaceEntireFile;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) usItem = lua_tointeger(L,i);
		if (i == 2 ) bStatus = lua_tointeger(L,i);
		if (i == 3 ) sMapX = lua_tointeger(L,i);
		if (i == 4 ) sMapY = lua_tointeger(L,i);
		if (i == 5 ) bMapZ = lua_tointeger(L,i);
		if (i == 6 ) sGridNo = lua_tointeger(L,i);
		if (i == 7 ) uiNumberOfItems = lua_tointeger(L,i);
		if (i == 8 ) ubLevel = lua_tointeger(L,i);
		if (i == 9 ) usFlags = lua_tointeger(L,i);
		if (i == 10 ) bRenderZHeightAboveLevel = lua_tointeger(L,i);
		if (i == 11 ) bVisible = lua_tointeger(L,i);
		if (i == 12 ) fReplaceEntireFile = lua_toboolean(L,i);
	}

	CreateItem( usItem, bStatus, &Object ); 
	AddItemsToUnLoadedSector( sMapX, sMapY, bMapZ, sGridNo, uiNumberOfItems, &Object, ubLevel, usFlags, bRenderZHeightAboveLevel, bVisible, fReplaceEntireFile );

	return 0;
}

//Create items
static int l_CreateItemToPool (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;
	OBJECTTYPE	Object;	
	INT32 sGridNo;
	UINT16 usItem;
	INT16 bStatus;
	UINT8 ubLevel;
	INT8 bVisible;
	UINT16 usFlags;
	INT8 bRenderZHeightAboveLevel;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) usItem = lua_tointeger(L,i);
		if (i == 2 ) bStatus = lua_tointeger(L,i);
		if (i == 3 ) sGridNo = lua_tointeger(L,i);
		if (i == 5 ) bVisible = lua_tointeger(L,i);
		if (i == 5 ) ubLevel = lua_tointeger(L,i);
		if (i == 6 ) usFlags = lua_tointeger(L,i);
		if (i == 7 ) bRenderZHeightAboveLevel = lua_tointeger(L,i);
	}

	CreateItem( usItem, bStatus, &Object ); 
	AddItemToPool( sGridNo, &Object, bVisible, ubLevel, usFlags, bRenderZHeightAboveLevel );

	return 0;
}
	
static int l_CreateMoney (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	INT32 iWorldItem = 0;
	OBJECTTYPE	Object;	
	INT32 sGridNo = 0;
	UINT16 money = 0;

	UINT8 bLevel = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) money = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
		if (i == 3 ) bLevel = lua_tointeger(L,i);
	}

	CreateMoney(money, &Object ); 
	AddItemToPoolAndGetIndex( sGridNo, &Object, -1, bLevel, 0, 0, -1, &iWorldItem );

	return 0;
}
	
//RecruitRPC
static int l_RecruitRPC (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	RecruitRPC( id );
		
	return 0;
}
		
//RecruitEPC
static int l_RecruitEPC (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	if ( !CheckNPCIsEPC(id) ) 
		RecruitEPC( id );
		
	return 0;
}

//UnRecruitEPC
static int l_UnRecruitEPC (lua_State *L)
{
UINT8  n = lua_gettop(L);
int i;
UINT8 id;

	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

if (CheckNPCIsEPC(id))
	UnRecruitEPC( id );
	
return 0;
}

static int l_FindSoldierTeam (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 team = 0;
	UINT8 ubTargetNPC;
	SOLDIERTYPE *pSoldier;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC,FALSE);
	if (pSoldier )
		{
			
			if ( pSoldier->bTeam == OUR_TEAM )
				team = 0;
			else if ( pSoldier->bTeam == ENEMY_TEAM )
				team = 1;
			else if ( pSoldier->bTeam == CREATURE_TEAM )
				team = 2;
			else if ( pSoldier->bTeam == MILITIA_TEAM )	
				team = 3;
			else if ( pSoldier->bTeam == CIV_TEAM )	
				team = 4;
			else if ( pSoldier->bTeam == PLAYER_PLAN )	
				team = 5;
		}
	
	lua_pushinteger(L, team);
		
	return 1;
}

static int l_EVENT_StopMerc (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 )
			ubTargetNPC = lua_tointeger(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, FALSE);
	if (pSoldier )
	{	
		pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );
	}
		
return 0;
}

static int l_GetGridoProfileID (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;

	UINT32 grido = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
	//	if (i == 2 ) fPlayerMercsOnly = lua_toboolean(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, FALSE);
	if ( pSoldier )
		grido = pSoldier->sGridNo;	
	else
		grido = 0;
	
	lua_pushinteger(L, grido);
		
return 1;
}

static int l_GetDirection (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;

	UINT32 Direction;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, FALSE);
	if ( pSoldier )
		Direction = pSoldier->ubDirection;	
	else
		Direction = 0;
	
	lua_pushinteger(L, Direction);
		
	return 1;
}

static int l_ubID (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;

	UINT32 ubID = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) 
			ubTargetNPC = lua_tointeger(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, FALSE);
	if ( pSoldier )
		ubID = pSoldier->ubID;	
	else
		ubID = -1;
	
	lua_pushinteger(L, ubID);
		
	return 1;
}
//------------
static int l_GetFirstID (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i;
	UINT8 team = 0;
	UINT32 id;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) team = lua_tointeger(L,i);
	}
	
	id = gTacticalStatus.Team[ team ].bFirstID;
	
	lua_pushinteger(L, id);
		
	return 1;
}

static int l_GetLastID (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 team = 0;
	UINT32 id = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) team = lua_tointeger(L,i);
	}
	
	id = gTacticalStatus.Team[ team ].bLastID;
	
	lua_pushinteger(L, id);
		
	return 1;
}

static int l_SetCivGroupHostile (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 status = 0,Civ = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Civ = lua_tointeger(L,i);
		if (i == 2 ) status = lua_tointeger(L,i);
	}
	
	gTacticalStatus.fCivGroupHostile[ Civ ] = status;
		
	return 0;
}

//Merc 
static int l_CheckMercPtrsActive (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	UINT32 ubID = 0;

	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bActive )
		Bool = TRUE;
	else
		Bool = FALSE;
	
	lua_pushboolean(L, Bool);
	
	return 1;
}

static int l_CheckMercPtsrInSector (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	UINT32 ubID = 0;

	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
	
	lua_pushboolean(L, Bool);

	return 1;
}

static int l_WearGasMaskIfAvailable(lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubID = 0;
	BOOLEAN Bool = FALSE;
	SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive)
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
	{
		pSoldier = MercPtrs[ ubID ];
		
		if (pSoldier)
		{
		WearGasMaskIfAvailable(pSoldier);
		}
	}

	return 0;
}

static int l_SetNewSituationMercPtsr(lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubID = 0;
	BOOLEAN Bool = FALSE;
	SOLDIERTYPE * pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive)
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
	{
		pSoldier = MercPtrs[ ubID ];
		
		if (pSoldier)
		{
		SetNewSituation(pSoldier);
		}
	}

	return 0;
}

static int l_gsPublicNoiseGridNo(lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 Team = 0;
	UINT32 sGridNo = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Team = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
	}
	
	gsPublicNoiseGridNo[ Team ] = sGridNo;

	return 0;
}

static int l_gubPublicNoiseVolume(lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 Team = 0;
	UINT32 Duration = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Team = lua_tointeger(L,i);
		if (i == 2 ) Duration = lua_tointeger(L,i);
	}

	//MAX_MISC_NOISE_DURATION
	gubPublicNoiseVolume[ Team ] = Duration;

	return 0;
}

static int l_AnimMercPtsrInSector (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	UINT32 ubID = 0,Anim = 0;

	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) Anim = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->EVENT_InitNewSoldierAnim( Anim, 0, TRUE );
		
	return 0;
}
					
static int l_AnimMercPtsrfAIFlags(lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->aiData.fAIFlags |= AI_HANDLE_EVERY_FRAME;
		
	return 0;
}
					
static int l_AnimMercPtsrusStrategicInsertionData(lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,GridNo = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) GridNo = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->usStrategicInsertionData = GridNo;
		
	return 0;
}
					
					
static int l_AnimMercPtsrubStrategicInsertionCode (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,GridNo = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) GridNo = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->ubStrategicInsertionCode = GridNo;
		
	return 0;
}	

static int l_WhichBuddy (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID1 = 0,ubID2 = 0,bBuddyIndex = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID1 = lua_tointeger(L,i);
		if (i == 2 ) ubID2 = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID1 ]->bInSector && MercPtrs[ ubID2 ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		bBuddyIndex = WhichBuddy( ubID1, ubID2 );
		
	lua_pushinteger(L, bBuddyIndex);
		
	return 1;
}

static int l_AnimMercPtsrsAbsoluteFinalDestination (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,GridNo = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) GridNo = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->sAbsoluteFinalDestination = GridNo;
		
	return 0;
}
					
static int l_AnimMercPtsrusNextActionData (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,GridNo = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) GridNo = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->aiData.usNextActionData = GridNo;
		
	return 0;
}
					
static int l_AnimMercPtsrbNextAction (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,AI_ACTION = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) AI_ACTION = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->aiData.bNextAction = AI_ACTION;
		
	return 0;
}

static int l_SetSoldierBodyType (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,Anim = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) Anim = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		MercPtrs[ ubID ]->ubBodyType = 	Anim;
			
	return 0;
}

static int l_GetSoldierBodyType (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,Anim = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
		Anim = MercPtrs[ ubID ]->ubBodyType;
		
		lua_pushinteger(L, Anim);
			
	return 1;
}

static int l_CheckSoldierBodyType (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,Anim = 0;
	BOOLEAN Bool = FALSE, Bool2 = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) Anim = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive )
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
	{
		if ( MercPtrs[ ubID ]->ubBodyType == Anim )
			Bool2 = TRUE;
		else
			Bool2 = FALSE;
	}
	
	lua_pushboolean(L, Bool2);
			
	return 1;
}

static int l_IS_CIV_BODY_TYPE (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive && IS_CIV_BODY_TYPE (MercPtrs[ ubID ]) == TRUE )
		Bool = TRUE;
	else
		Bool = FALSE;
				
	lua_pushboolean(L, Bool);
		
	return 1;
}

static int l_SetOffBombsInGridNo (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubID = 0;
	INT32 sGridNo = 0;
	BOOLEAN fAllBombs = FALSE;
	INT8 bLevel = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
		if (i == 3 ) fAllBombs = lua_toboolean(L,i);
		if (i == 4 ) bLevel = lua_tointeger(L,i);
	}
	
	SetOffBombsInGridNo( ubID, sGridNo, fAllBombs,bLevel );
	return 0;
}


static int l_AnimMercPtsrSoldierGotHit (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		//if (i == 2 ) Anim = lua_tointeger(L,i);
	}
	
	if ( MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->bActive)
		Bool = TRUE;
	else
		Bool = FALSE;
		
	if ( Bool == TRUE )
	{
		if ( MercPtrs[ ubID ]->stats.bLife >= 0 ) 
		{
			MercPtrs[ ubID ]->EVENT_SoldierGotHit( 1, 100, 10, MercPtrs[ ubID ]->ubDirection, 320, NOBODY , FIRE_WEAPON_NO_SPECIAL, AIM_SHOT_TORSO, 0, NOWHERE );
		}
	}

	return 0;
}

static int l_CheckMercPtsrubIDSeenubID2 (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubID = 0,ubID2 = 0;
	UINT16 seen = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
		if (i == 2 ) ubID2 = lua_tointeger(L,i);
	}
	
	seen = MercPtrs[ ubID ]->aiData.bOppList[ ubID2 ];
	
	lua_pushinteger(L, seen);
	
	return 1;
}

static int l_CheckMercPtrsInCivilianGroup (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 group = 0;
	UINT32 ubID = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}
	
	group = MercPtrs[ ubID ]->ubCivilianGroup;
	
	lua_pushinteger(L, group);
	return 1;
}

//------------

//INT32 NewGridNo(INT32 sGridNo, INT16 sDirInc);
//INT16 DirectionInc(UINT8 ubDirection);
//INT32 OutOfBounds(INT32 sGridNo, INT32 sProposedGridNo);

static int l_TileIsOutOfBounds (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	INT32 sGridNo = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
	}
	
	TileIsOutOfBounds(sGridNo);
			
	return 0;
}

static int l_ClosestPC (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;
	INT32 psDistance = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		if (i == 2 ) psDistance = lua_tointeger(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, FALSE);
	if ( pSoldier )
	{
		ClosestPC( pSoldier, &psDistance );
	}
			
	return 0;
}

static int l_TileIsOutOfBoundsClosestPC (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;
	BOOLEAN  Bool = FALSE , fPlayerMercsOnly = FALSE;
	INT32 sDist = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		if (i == 2 ) fPlayerMercsOnly = lua_toboolean(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, fPlayerMercsOnly);
	if ( pSoldier )
	{
		
		if ( TileIsOutOfBounds(ClosestPC( pSoldier, &sDist )) && sDist > NPC_TALK_RADIUS * 2 )
				Bool = TRUE;
			else
				Bool = FALSE;
		
	}
	
	lua_pushboolean(L, Bool);
		
	return 1;
}

static int l_FindSoldierByProfileID (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0,ubLoop = 0,ubLoopLimit = 0;
	SOLDIERTYPE *pSoldier;
	UINT8 id = -1;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
	//	if (i == 2 ) fPlayerMercsOnly = lua_tointeger(L,i);
	}
		
	ubLoopLimit = MAX_NUM_SOLDIERS;

	for (ubLoop = 0, pSoldier = MercPtrs[0]; ubLoop < ubLoopLimit; ubLoop++, pSoldier++)
	{
		if (pSoldier->bActive && pSoldier->ubProfile == ubTargetNPC)
		{
			id = pSoldier->ubProfile;
		}
	}
		
	lua_pushinteger(L, id);
		
	return 1;
}

static int l_ubWhatKindOfMercAmI (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 team = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
	//	if (i == 2 ) fPlayerMercsOnly = lua_toboolean(L,i);	
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC,TRUE);//, fPlayerMercsOnly);
	if (pSoldier )
	{
		
		if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER )
			team = 0;
		else if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
			team = 1;
		else if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC )
			team = 2;
		else if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__NPC )	
			team = 3;
		else if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC )	
			team = 4;
		else if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT )	
			team = 5;
		else if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__VEHICLE )	
			team = 6;
	}
	
	lua_pushinteger(L, team);
		
	return 1;
}

static int l_PythSpacesAway (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	INT32 sOrigin = 0;
	INT32 sDest = 0;

	SOLDIERTYPE *pSoldier;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		if (i == 2 ) sDest = lua_tointeger(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, FALSE);
	
	if (pSoldier )	
		sOrigin = PythSpacesAway( pSoldier->sGridNo, sDest );		

	lua_pushinteger(L, sOrigin);
		
	return 1;
}

static int l_MakeHostile (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 ubTargetNPC = 0;
	SOLDIERTYPE *pSoldier;
	BOOLEAN fPlayerMercsOnly = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubTargetNPC = lua_tointeger(L,i);
		if (i == 2 ) fPlayerMercsOnly = lua_toboolean(L,i);
	}
	
	pSoldier = FindSoldierByProfileID( ubTargetNPC, fPlayerMercsOnly );
	if (pSoldier )
	{
		if ( pSoldier->ubCivilianGroup != NON_CIV_GROUP )
		{
			if ( gTacticalStatus.fCivGroupHostile[ pSoldier->ubCivilianGroup ] == CIV_GROUP_NEUTRAL )
			{
				CivilianGroupMemberChangesSides( pSoldier );
			}
		}
		else
		{
			// make hostile
			MakeCivHostile( pSoldier, 2 );
		}
		DeleteTalkingMenu();
		if ( ! ( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			EnterCombatMode( pSoldier->bTeam );
		}
	}

	return 0;
}			
//---------------

//-------------New--------------
static int l_GetTimeQuestWasStarted (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 ubCode = 0, ubCode2 = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubCode = lua_tointeger(L,i);
	}
	
	ubCode2 = GetTimeQuestWasStarted( ubCode );
		
	lua_pushinteger(L, ubCode2);
		
	return 1;
}

static int l_gubQuest (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 quest = 0;
	UINT8 Bool = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) quest = lua_tointeger(L,i);
	}

	if (gubQuest[ quest ] == QUESTNOTSTARTED)
		Bool = 0;
	else if (gubQuest[ quest ] == QUESTINPROGRESS)
		Bool = 1;
	else if (gubQuest[ quest ] == QUESTDONE)	
		Bool = 2;
	lua_pushinteger(L, Bool);
		
		
	return 1;
}

static int l_GetTacticalStatusEnemyInSector (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	BOOLEAN Bool = FALSE;	
	Bool = gTacticalStatus.fEnemyInSector;
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//InTownSectorWithTrainingLoyalty
static int l_InTownSectorWithTrainingLoyalty (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	INT16 sSectorX = 0;
	INT16 sSectorY = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sSectorX = lua_tointeger(L,i);
		if (i == 2 ) sSectorY = lua_tointeger(L,i);
	}

	Bool = InTownSectorWithTrainingLoyalty( sSectorX, sSectorY );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//NPCHeardShot
static int l_NPCHeardShot (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = NPCHeardShot( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//CheckNPCIsUnderFire
static int l_CheckNPCIsUnderFire (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = CheckNPCIsUnderFire( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//CheckNPCCowering
static int l_CheckNPCCowering (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = CheckNPCCowering( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//AIMMercWithin
static int l_AIMMercWithin (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	INT32 sGridNo = 0;
	INT16 sDistance = 0; 
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) sGridNo = lua_tointeger(L,i);
		if (i == 2 ) sDistance = lua_tointeger(L,i);
	}

	Bool = AIMMercWithin( sGridNo, sDistance );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//CheckNPCSectorBool
static int l_CheckNPCSectorBool (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;
	INT16 sSectorX = 0;
	INT16 sSectorY = 0;
	INT8 bSectorZ = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
		if (i == 2 ) sSectorX = lua_tointeger(L,i);
		if (i == 3 ) sSectorY = lua_tointeger(L,i);
		if (i == 4 ) bSectorZ = lua_tointeger(L,i);
	}

	//if (gMercProfiles[ id ].sSectorX == sSectorX && gMercProfiles[ id ].sSectorY == sSectorY && gMercProfiles[ id ].bSectorZ == bSectorZ)
	//	Bool = TRUE;
	//else
	//	Bool = FALSE;

	Bool = CheckNPCSector( ubProfileID, sSectorX, sSectorY, bSectorZ );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//FemalePresent
static int l_FemalePresent (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = FemalePresent( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//PCInSameRoom
static int l_PCInSameRoom (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = PCInSameRoom( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//NPCInRoomRange
static int l_NPCInRoomRange (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;
	UINT8 ubRoomID1 = 0;
	UINT8 ubRoomID2 = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
		if (i == 2 ) ubRoomID1 = lua_tointeger(L,i);
		if (i == 3 ) ubRoomID2 = lua_tointeger(L,i);
	}

	Bool = NPCInRoomRange( ubProfileID, ubRoomID1,ubRoomID2 );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//NPCInRoom
static int l_NPCInRoom (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;
	UINT8 ubRoomID = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
		if (i == 2 ) ubRoomID = lua_tointeger(L,i);
	}

	Bool = NPCInRoom( ubProfileID, ubRoomID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}


//CheckNPCIsRPC
static int l_CheckNPCIsRPC (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = CheckNPCIsRPC( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}


//CheckNPCIsEPC
static int l_CheckNPCIsEPC (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = CheckNPCIsEPC( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//CheckNPCIsEnemy
static int l_CheckNPCIsEnemy (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	BOOLEAN Bool = FALSE;
	UINT8 ubProfileID = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubProfileID = lua_tointeger(L,i);
	}

	Bool = CheckNPCIsEnemy( ubProfileID );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}


//CheckNPCAt
static int l_CheckNPCAt (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	BOOLEAN Bool = FALSE;
	UINT8 ubNPC = 0;
	INT32 sGridNo = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubNPC = lua_tointeger(L,i);
		if (i == 2 ) sGridNo = lua_tointeger(L,i);
	}

	Bool = CheckNPCAt( ubNPC, sGridNo );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//CheckGuyVisible
static int l_CheckGuyVisible (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	BOOLEAN Bool = FALSE;
	UINT8 ubNPC = 0;
	UINT8 ubGuy = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubNPC = lua_tointeger(L,i);
		if (i == 2 ) ubGuy = lua_tointeger(L,i);
	}

	Bool = CheckGuyVisible( ubNPC, ubGuy );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}



//CheckNPCWithin
static int l_CheckNPCWithin (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	BOOLEAN Bool = FALSE;
	UINT8 ubFirstNPC = 0;
	UINT8 ubSecondNPC = 0;
	UINT8 ubMaxDistance = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubFirstNPC = lua_tointeger(L,i);
		if (i == 2 ) ubSecondNPC = lua_tointeger(L,i);
		if (i == 3 ) ubMaxDistance = lua_tointeger(L,i);
	}

	Bool = CheckNPCWithin( ubFirstNPC, ubSecondNPC, ubMaxDistance );

	lua_pushboolean(L, Bool);
		
	return 1;
}


//CheckNPCInOkayHealth
static int l_CheckNPCInOkayHealth (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	Bool = CheckNPCInOkayHealth( id );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}


//CheckNPCWounded
static int l_CheckNPCWounded (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	Bool = CheckNPCWounded( id, FALSE );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

static int l_CheckNPCWoundedbyPlayer (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	Bool = CheckNPCWounded( id, TRUE );
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//------------
//CheckNPCBleeding
static int l_CheckNPCBleeding (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	Bool = CheckNPCBleeding (id);
		
	lua_pushboolean(L, Bool);
		
	return 1;
}

//Check character SectorZ
static int l_CheckCharacterSectorZ (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT16 SectorZ = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	SectorZ = gMercProfiles[ id ].bSectorZ;
	
	lua_pushinteger(L, SectorZ);
		
	return 1;
}

//Check character SectorY
static int l_CheckCharacterSectorY (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT16 SectorY = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	SectorY = gMercProfiles[ id ].sSectorY;
		
	lua_pushinteger(L, SectorY);
		
	return 1;
}

//Check character SectorX
static int l_CheckCharacterSectorX (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	UINT8 id = 0;
	UINT16 SectorX = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	SectorX = gMercProfiles[ id ].sSectorX;
	
	lua_pushinteger(L, SectorX);
	
	return 1;
}

//SetPendingNewScreen( SEX_SCREEN );
static int l_SetPendingNewScreenSEXSCREEN (lua_State *L)
{
	UINT8  n = lua_gettop(L);

	SetPendingNewScreen( SEX_SCREEN );
			
	return 0;
}
//--------------

//Set charcter to sector Z
static int l_GetCharacterSectorZ (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT8 SectorZ = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	SectorZ = gMercProfiles[ id ].bSectorZ;
		
	lua_pushinteger(L, SectorZ);
		
	return 1;
}

//Set charcter to sector Y
static int l_GetCharacterSectorY (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT16 SectorY = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	SectorY = gMercProfiles[ id ].sSectorY;
		
	lua_pushinteger(L, SectorY);
		
	return 1;
}

//Set charcter to sector X
static int l_GetCharacterSectorX (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT16 SectorX = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}

	SectorX = gMercProfiles[ id ].sSectorX;
		
	lua_pushinteger(L, SectorX);
		
	return 1;
}

//-----------
//Set charcter to sector Z
static int l_SetCharacterSectorZ (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT8 SectorZ = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
		if (i == 2 ) SectorZ = lua_tointeger(L,i);
	}

	gMercProfiles[ id ].bSectorZ = SectorZ;
		
	return 0;
}

//Set charcter to sector Y
static int l_SetCharacterSectorY (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT16 SectorY = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
		if (i == 2 ) SectorY = lua_tointeger(L,i);
	}

	gMercProfiles[ id ].sSectorY = SectorY;
		
	return 0;
}

//Set charcter to sector X
static int l_SetCharacterSectorX (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT8 id = 0;
	UINT16 SectorX = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
		if (i == 2 ) SectorX = lua_tointeger(L,i);
	}

	gMercProfiles[ id ].sSectorX = SectorX;
		
return 0;
}

//Run TriggerRecord
static int l_TriggerNPCRecord(lua_State *L)
{
	UINT8 prof = 0,set = 0;
	UINT8  n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) prof = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}

	TriggerNPCRecord (prof,set);
	
	return 0;
}

//Run TriggerNPCRecordImmediately
static int l_TriggerNPCRecordImmediately(lua_State *L)
{
	UINT8 prof = 0,set = 0;
	UINT8  n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) prof = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}

	TriggerNPCRecordImmediately (prof,set);
	
	return 0;
}

//Add undern Ground Sector
static int l_AddAltUnderGroundSectorNew(lua_State *L)
{
	UINT8 x = 0,y = 0,z = 0;
	UINT8  n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) x = lua_tointeger(L,i);
		if (i == 2 ) y = lua_tointeger(L,i);
		if (i == 3 ) z = lua_tointeger(L,i);
	}
	
	if ((x>=1 || x<=16) && (y>=1 || y<=16) && (z>=0 || z<=3) )
	{	
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( x, y, z ); 
		if( pSector )
		{
			pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		}
	}
	
	return 0;	
}

static int l_UnderGroundSectorVisited(lua_State *L)
{
	UINT8 x = 0,y = 0,z = 0;
	UINT8  n = lua_gettop(L);
	int i = 0;
	BOOLEAN Bool = FALSE;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) x = lua_tointeger(L,i);
		if (i == 2 ) y = lua_tointeger(L,i);
		if (i == 3 ) z = lua_tointeger(L,i);
	}
	
	if ((x>=1 || x<=16) && (y>=1 || y<=16) && (z>=0 || z<=3) )
	{	
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( x, y, z ); 
		if( pSector )
		{
			Bool = pSector->fVisited;
			
		}
	}
	
	lua_pushboolean(L, Bool);	
		
	return 1;	
}

//Add underGroundSector
static int l_AddAlternateSectorNew(lua_State *L)
{
	UINT8 x = 0,y = 0;
	UINT8  n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) x = lua_tointeger(L,i);
		if (i == 2 ) y = lua_tointeger(L,i);
	}

	if ((x>=1 || x<=16) && (y>=1 || y<=16))
	{	
		SectorInfo[ SECTOR( x, y ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
	}
	
	return 0;
}

//Add character to sector
static int l_AddNPCTOSECTOR(lua_State *L)
{
	MERCPROFILESTRUCT * pProfile;
	UINT8 x = 0,y = 0,z = 0,id = 0;
	UINT8  n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
		if (i == 2 ) x = lua_tointeger(L,i);
		if (i == 3 ) y = lua_tointeger(L,i);
		if (i == 4 ) z = lua_tointeger(L,i);
	}
	 
	if ( (x>=0 || x<=16) && (y>=0 || y<=16) && (z>=0 || z<=3) )
	{
		pProfile = &(gMercProfiles[ id ]);

		pProfile->sSectorX = x;
		pProfile->sSectorY = y;
		pProfile->bSectorZ = z;
	}
	
	return 0;
}


//Check character Sector New
static int l_CheckNPCSectorNew (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	UINT8 SectorZ = 0,id = 0;
	UINT16 SectorX = 0,SectorY = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}
	
	SectorX = gMercProfiles[ id ].sSectorX;
	SectorY = gMercProfiles[ id ].sSectorY;
	SectorZ = gMercProfiles[ id ].bSectorZ;

	lua_pushinteger(L, id);
	lua_pushinteger(L, SectorX);
	lua_pushinteger(L, SectorY);
	lua_pushinteger(L, SectorZ);	
		
	return 4;
}

//Check character Sector
static int l_CheckNPCSector (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;

	UINT8 SectorZ = 0,Z1 = 0,id = 0;
	UINT16 SectorX = 0,SectorY = 0,X1 = 0,Y1 = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
		if (i == 2 ) X1 = lua_tointeger(L,i);
		if (i == 3 ) Y1 = lua_tointeger(L,i);
		if (i == 4 ) Z1 = lua_tointeger(L,i);
	}

	if( gMercProfiles[ id].sSectorX ==  X1)
	{
		SectorX = gMercProfiles[ id ].sSectorX;
	}

	if (gMercProfiles[ id ].sSectorY == Y1)
	{
		SectorY = gMercProfiles[ id ].sSectorY;
	}

	if (gMercProfiles[ id ].bSectorZ == Z1)
	{
		SectorZ = gMercProfiles[ id ].bSectorZ;
	}
		
	lua_pushinteger(L, id);
	lua_pushinteger(L, SectorX);
	lua_pushinteger(L, SectorY);
	lua_pushinteger(L, SectorZ);	
		
	return 4;
}

//Check Life character
static int l_bLife (lua_State *L)
{
	UINT8 IDNPC = lua_gettop(L);
	UINT32 Life = 0;
	UINT8 Profile = 0;

	Profile = lua_tointeger(L,IDNPC);

	Life = gMercProfiles[ Profile ].bLife;

	lua_pushinteger(L, Life);
			
	return 1;
}

//Set Life character
static int l_SetbLife (lua_State *L)
{
	UINT8 n = lua_gettop(L);
	UINT32 Life = 0;
	UINT8 Profile = 0;
	int i = 0;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) Life = lua_tointeger(L,i);
	}
	
	gMercProfiles[ Profile ].bLife = Life;
			
	return 0;
}

//Get Life Maxcharacter
static int l_GetbLifeMax (lua_State *L)
{
	UINT8 n = lua_gettop(L);
	UINT32 Life = 0;
	UINT8 Profile = 0;
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
	}
	
	Life = gMercProfiles[ Profile ].bLifeMax;

	lua_pushinteger(L, Life);
			
	return 1;
}

//check character LastDateSpokenTot
static int l_ubLastDateSpokenTot (lua_State *L)
{
	UINT8 IDNPC = lua_gettop(L);
	UINT32 LastDateSpokenTo = 0;
	UINT8 Profile = 0;

	Profile = lua_tointeger(L,IDNPC);

	LastDateSpokenTo = gMercProfiles[ Profile ].ubLastDateSpokenTo;

	lua_pushinteger(L, LastDateSpokenTo);
			
	return 1;
}

//check character NPCData 
static int l_fNPCData (lua_State *L)
{
	UINT8 IDNPC = lua_gettop(L);
	UINT32 NPCData = 0;
	UINT8 Profile = 0;

	Profile = lua_tointeger(L,IDNPC);

	NPCData = gMercProfiles[ Profile ].bNPCData;

	lua_pushinteger(L, NPCData);
			
	return 1;

}

//check character NPCData2
static int l_fNPCData2 (lua_State *L)
{
	UINT8 IDNPC = lua_gettop(L);
	UINT32 NPCData2 = 0;
	UINT8 Profile = 0;

	Profile = lua_tointeger(L,IDNPC);

	NPCData2 = gMercProfiles[ Profile ].bNPCData2;

	lua_pushinteger(L, NPCData2);
			
	return 1;

}

//check character money 
static int l_fuiMoney (lua_State *L)
{
	UINT8 IDNPC = lua_gettop(L);
	UINT32 Money = 0;
	UINT8 Profile = 0;

	Profile = lua_tointeger(L,IDNPC);

	Money = gMercProfiles[ Profile ].uiMoney;

	lua_pushinteger(L, Money);
			
	return 1;

}

//set money to character
static int l_SetuiMoney(lua_State *L)
{
	UINT32 set = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}
	
	gMercProfiles[Profile].uiMoney = set;

	return 0;
}

//set start quest
static int l_StartQuest(lua_State *L)
{
	UINT8 n = lua_gettop(L);
	UINT8 Quest = 0;
	UINT16 X = 0,Y = 0;
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Quest = lua_tointeger(L,i);
		if (i == 2 ) X = lua_tointeger(L,i);
		if (i == 3 ) Y = lua_tointeger(L,i);
	}

	StartQuest( Quest, X, Y );

	return 0;
}

//set end quest
static int l_EndQuest(lua_State *L)
{
	UINT8 n = lua_gettop(L);
	UINT8 Quest = 0;
	UINT16 X = 0,Y = 0;
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Quest = lua_tointeger(L,i);
		if (i == 2 ) X = lua_tointeger(L,i);
		if (i == 3 ) Y = lua_tointeger(L,i);
	}

	EndQuest( Quest, X, Y );

	return 0;
}
//get
static int l_GetiBalance(lua_State *L)
{
	UINT32 set = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
	}

	
	set = gMercProfiles[Profile].iBalance;

	lua_pushinteger(L, set);
		
	return 1;
}
//set balance character
static int l_iBalance(lua_State *L)
{
	UINT32 set = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}

	gMercProfiles[Profile].iBalance = set;

	return 0;
}

//set chracter NPCDATA1
static int l_bNPCData(lua_State *L)
{
	UINT32 set = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}
	
	gMercProfiles[Profile].bNPCData = set;

	return 0;
}

//set chracter NPCDATA2
static int l_bNPCData2(lua_State *L)
{
	UINT32 set = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}
	
	gMercProfiles[Profile].bNPCData2 = set;

	return 0;
}


//check character MiscFlags2 
static int l_ubMiscFlags2Check (lua_State *L)
{
	UINT32 set = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	UINT8 Flag = 0;
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}

	if (set == 1)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR)
			Flag = 1;		
	}
	else  if (set == 2)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG2_LEFT_COUNTRY)
			Flag = 1;		
	}
	else  if (set == 4)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG2_BANDAGED_TODAY)
			Flag = 1;		
	}
	else  if (set == 8)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE)
			Flag = 1;
	}
	else  if (set == 16)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE)
			Flag = 1;
	}
	else  if (set == 32)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG2_MARRIED_TO_HICKS)
			Flag = 1;
	}
	else  if (set == 64 )	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG2_ASKED_BY_HICKS)
			Flag = 1;		
	}

	// Set the return value of the function
	// --> Flag == 0 -> The specified Flag is not set
	// --> Flag == 1 -> The specified Flag is set	
	lua_pushinteger(L, Flag);
				
	// Always return 1, this tells LUA that the the function run without a problem.
	// But THIS is not the return value of the function. The return value is specified in the "lua_pushinteger()" call above!!!
	return 1;
}

//check character MiscFlags1 
static int l_ubMiscFlags1Check (lua_State *L)
{
	UINT32 set = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);

	UINT8 Flag = 0;
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}

	if (set == 1)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_RECRUITED)
			Flag = 1;		
	}
	else  if (set == 2)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_HAVESEENCREATURE)
			Flag = 1;		
	}
	else  if (set == 4)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_FORCENPCQUOTE)
			Flag = 1;		
	}
	else  if (set == 8)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_WOUNDEDBYPLAYER)
			Flag = 1;
	}
	else  if (set == 16)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS)
			Flag = 1;
	}
	else  if (set == 32)	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE)
			Flag = 1;
	}
	else  if (set == 64 )	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_EPCACTIVE)
			Flag = 1;		
	}
	else  if (set == 128 )	
	{
		if (gMercProfiles[Profile].ubMiscFlags2 & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS)
			Flag = 1;		
	}

	//Flag = (gMercProfiles[Profile].ubMiscFlags & set);


	lua_pushinteger(L, Flag);
			
	return 1;

}

//set chracter MiscFlags1
static int l_ubMiscFlags1Set(lua_State *L)
{
	UINT32 set = 0;
	UINT32 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}
	
	 if (set == 0)	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_RECRUITED);
	 }
	 else  if (set == 1)	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_HAVESEENCREATURE);
	 }
	 else  if (set == 2)	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_FORCENPCQUOTE);
	 }
	 else  if (set == 3)	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_WOUNDEDBYPLAYER);
	 }
	 else  if (set == 4)	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS);
	 }
	 else  if (set == 5)	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE);
	 }
	 else  if (set == 6 )	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_EPCACTIVE);
	 }
	 else  if (set == 7 )	
	 {
		gMercProfiles[Profile].ubMiscFlags &= (~PROFILE_MISC_FLAG_ALREADY_USED_ITEMS);
	 }

/*
PROFILE_MISC_FLAG3_PLAYER_LEFT_MSG_FOR_MERC_AT_AIM
PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE
PROFILE_MISC_FLAG3_PLAYER_HAD_CHANCE_TO_HIRE
PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL
PROFILE_MISC_FLAG3_NPC_PISSED_OFF
PROFILE_MISC_FLAG3_MERC_MERC_IS_DEAD_AND_QUOTE_SAID
PROFILE_MISC_FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH
PROFILE_MISC_FLAG3_GOODGUY
*/
	return 0;
}
		
//set chracter MiscFlags2
static int l_ubMiscFlags2Set(lua_State *L)
{
	UINT32 set = 0;
	UINT32 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i);
	}
	
	 if (set == 0)
	 {
		gMercProfiles[Profile].ubMiscFlags2 = 0;
	 }
	 else if (set == 1)	
	 {
		gMercProfiles[Profile].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR);
	 }
	 else  if (set == 2)	
	 {
		gMercProfiles[Profile].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_LEFT_COUNTRY);
	 }
	 else  if (set == 4)	
	 {
		gMercProfiles[Profile].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_BANDAGED_TODAY);
	 }
	 else  if (set == 8)	
	 {
		gMercProfiles[Profile].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE);
	 }
	 else  if (set == 16)	
	 {
		gMercProfiles[Profile].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE);
	 }
	 else  if (set == 32)	
	 {
		gMercProfiles[Profile].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_MARRIED_TO_HICKS);
	 }
	 else  if (set == 64 )	
	 {
		gMercProfiles[Profile].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_ASKED_BY_HICKS);
	 }

	return 0;
}
		
//set chracter ThreatenDefaultResponseUsedRecently
static int l_bThreatenDefaultResponseUsedRecently(lua_State *L)
{
	BOOLEAN set = FALSE;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_toboolean(L,i);
	}
	
	gMercProfiles[Profile].bThreatenDefaultResponseUsedRecently = set;

	return 0;
}

//set chracter RecruitDefaultResponseUsedRecently
static int l_bRecruitDefaultResponseUsedRecently(lua_State *L)
{
	BOOLEAN set = FALSE;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_toboolean(L,i);
	}
	
	gMercProfiles[Profile].bRecruitDefaultResponseUsedRecently = set;

	return 0;
}

//set chracter FriendlyOrDirectDefaultResponseUsedRecently
static int l_bFriendlyOrDirectDefaultResponseUsedRecently(lua_State *L)
{
	BOOLEAN set = FALSE;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) set = lua_toboolean(L,i);
	}
	
	gMercProfiles[Profile].bFriendlyOrDirectDefaultResponseUsedRecently = set;

	return 0;
}

//remove object from chracter inventory slot
static int l_RemoveObjectFromSoldierProfile(lua_State *L)
{
	UINT32 cnt = 0;
	UINT8 Profile = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) Profile = lua_tointeger(L,i);
		if (i == 2 ) cnt = lua_tointeger(L,i);
	}
	
	RemoveObjectFromSoldierProfile( Profile, cnt );

	return 0;
}

//CheckForMissingHospitalSupplies
static int l_CheckForMissingHospitalSupplies(lua_State *L)
{
	CheckForMissingHospitalSupplies ();
	return 0;
}

//Set money in charcter
static int l_SetMoneyInSoldierProfile(lua_State *L)
{
	UINT8 Profile = 0;
	UINT32 Money = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
	if (i == 1 ) Profile = lua_tointeger(L,i);
	if (i == 2 ) Money = lua_tointeger(L,i);

	}

	SetMoneyInSoldierProfile (Profile, Money);
	return 0;
}

static int l_AddToShouldBecomeHostileOrSayQuoteList(lua_State *L)
{
	UINT8 ubID = 0;
	UINT8 n = lua_gettop(L);
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) ubID = lua_tointeger(L,i);
	}

	AddToShouldBecomeHostileOrSayQuoteList( ubID );
	return 0;
}

// set fact = false
static int l_SetFactFalse(lua_State *L)
{
	UINT32 FactFalse = 0;
	UINT8 n = lua_gettop(L);

	 FactFalse = lua_tointeger(L,n);

	SetFactFalse (FactFalse);
	return 0;
}

//set fact = true
static int l_SetFactTrue(lua_State *L)
{
	UINT32 FactTrue = 0;
	UINT8 n = lua_gettop(L);

	FactTrue = lua_tointeger(L,n);

	SetFactTrue (FactTrue);
		
	return 0;
}

//Check fact is true or false
static int l_CheckFact (lua_State *L)
{
	UINT32 fcheck = lua_gettop(L);
	UINT32 checkfact = 0,fact = 0;
	UINT8 ubProfileID = -1;
	UINT8 n = lua_gettop(L);

	int i;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) 
			fact = lua_tointeger(L,i);
		if (i == 2 ) 
			ubProfileID = lua_tointeger(L,i);
	}

	if ( ubProfileID != 255 && CheckFact( fact, ubProfileID ) )
	{	
		checkfact= TRUE;
	}
	else
	{
		checkfact = FALSE;
	}
	
	lua_pushboolean(L, checkfact);
		
	return 1;

}

//---------------------------------------

BOOLEAN LetHandleLoyaltyChangeForNPCAction(UINT8 ubNPCProfileId , UINT8 Init)
{
	char * filename = "scripts\\StrategicTownLoyalty.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);
		SGP_THROW(msg);
	}

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

		//init function
//	lua_register(L, "CheckFact", l_CheckFact);
//	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
//	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleLoyaltyChangeForNPCAction");
		lua_pushnumber(L, ubNPCProfileId );
		
		lua_call(L,1,0); 
	}

	lua_close(L);

	delete[] buffer;

	return true;
}

BOOLEAN LuaHandleGlobalLoyaltyEvent( UINT8 ubEventType, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ , UINT8 Init)
{
	char * filename = "scripts\\StrategicTownLoyalty.lua";
	UINT32 size, bytesRead;
	char* buffer;

	HWFILE file = FileOpen(filename, FILE_ACCESS_READ, FALSE);

	if (!file)
	{
		string msg("Cannot open file: ");
		msg.append(filename);
		SGP_THROW(msg);
	}

	size = FileSize(filename);
	buffer = new char[size+1];
	buffer[size] = 0;
	FileRead(file, buffer, size, &bytesRead);
	FileClose(file);

	lua_State *L = lua_open();
	luaL_openlibs(L);

		//init function
//	lua_register(L, "CheckFact", l_CheckFact);
//	lua_register(L, "CheckForMissingHospitalSupplies", l_CheckForMissingHospitalSupplies);
//	lua_register(L, "CheckForKingpinsMoneyMissing", l_FunctionCheckForKingpinsMoneyMissing);
	IniFunction(L);
	IniGlobalGameSetting(L);
	
	if (luaL_dostring(L, buffer))
	{
		// oh noes, error
		// TODO: write to log or something
		return false;
	}
	
	if ( Init == 0 )
	{
		lua_getglobal(L , "HandleGlobalLoyaltyEvent");
		lua_pushnumber(L, ubEventType );
		lua_pushnumber(L, sSectorX );
		lua_pushnumber(L, sSectorY );
		lua_pushnumber(L, bSectorZ );
		
		lua_call(L,4,0); 
	}

	lua_close(L);

	delete[] buffer;

	return true;
}

//set merc is ready
static int l_Merc_Status (lua_State *L)
{
	UINT8 n = lua_gettop(L);

	UINT8 id = 0;
	int i = 0;
	UINT32 set = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
		if (i == 2 ) set = lua_tointeger(L,i); 
	}
	
	if ( set == 0  ) gMercProfiles[ id ].bMercStatus  = MERC_OK;
	if ( set == -1 ) gMercProfiles[ id ].bMercStatus  = MERC_HAS_NO_TEXT_FILE;	
	if ( set == -2 ) gMercProfiles[ id ].bMercStatus  = MERC_ANNOYED_BUT_CAN_STILL_CONTACT;	
	if ( set == -3 ) gMercProfiles[ id ].bMercStatus  = MERC_ANNOYED_WONT_CONTACT;	
	if ( set == -4 ) gMercProfiles[ id ].bMercStatus  = MERC_HIRED_BUT_NOT_ARRIVED_YET;	
	if ( set == -5 ) gMercProfiles[ id ].bMercStatus  = MERC_IS_DEAD;	
	if ( set == -6 ) gMercProfiles[ id ].bMercStatus  = MERC_RETURNING_HOME;	
	if ( set == -7 ) gMercProfiles[ id ].bMercStatus  = MERC_WORKING_ELSEWHERE;		
	if ( set == -8 ) gMercProfiles[ id ].bMercStatus  = MERC_FIRED_AS_A_POW;			

	return 0;
}

static int l_Get_Merc_Status (lua_State *L)
{
	UINT8 n = lua_gettop(L);

	UINT8 id = 0;
	int i = 0;
	UINT32 set = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) id = lua_tointeger(L,i);
	}
	
	if ( gMercProfiles[ id ].bMercStatus  == MERC_OK ) set = 0;
	if ( gMercProfiles[ id ].bMercStatus  == MERC_HAS_NO_TEXT_FILE)	set = -1;
	if ( gMercProfiles[ id ].bMercStatus  == MERC_ANNOYED_BUT_CAN_STILL_CONTACT)	set = -2;
	if ( gMercProfiles[ id ].bMercStatus  == MERC_ANNOYED_WONT_CONTACT)set = -3;
	if ( gMercProfiles[ id ].bMercStatus  == MERC_HIRED_BUT_NOT_ARRIVED_YET)	set = -4;
	if ( gMercProfiles[ id ].bMercStatus  == MERC_IS_DEAD)	set = -5;
	if ( gMercProfiles[ id ].bMercStatus  == MERC_RETURNING_HOME)	set = -6;
	if ( gMercProfiles[ id ].bMercStatus  == MERC_WORKING_ELSEWHERE)	set = -7;	
	if ( gMercProfiles[ id ].bMercStatus  == MERC_FIRED_AS_A_POW)		set = -8;	
	
	lua_pushinteger (L, set);

	return 1;
}

//Check merc is dead
static int l_Merc_Is_Dead (lua_State *L)
{
	UINT8 npcID = lua_gettop(L);
	BOOLEAN dead = FALSE;
	UINT8 id = 0;
	
	id = lua_tointeger(L,npcID);
	
	if ( gMercProfiles[ id ].bMercStatus  == MERC_IS_DEAD )
	{
		dead = TRUE;
	}
	else
	{
		dead = FALSE;
	}
	
	lua_pushboolean (L, dead);

	return 1;
}

// add town loyalty 
static int l_IncrementTownLoyalty(lua_State *L)
{
	UINT32 n = lua_gettop(L);
	UINT32 idLoyalty = 0,idTown = 0;

	//for (i= 1; i<=n; i++ )
	//{
	//	if (i == 1 ) 
			idTown = lua_tointeger(L,1);
	//	if (i == 2 ) 
			idLoyalty = lua_tointeger(L,2);
	//}
	
	if (idLoyalty >= 1 )
	{
		IncrementTownLoyalty( idTown, idLoyalty );
	}	

	return 0;
}

static int l_DecrementTownLoyaltyEverywhere(lua_State *L)
{
	UINT32 n = lua_gettop(L);
	UINT32 idLoyalty = 0;


//	for (i= 1; i<=n; i++ )
	//{
	//	if (i == 1 ) 
			idLoyalty = lua_tointeger(L,1);
	//}
	
	if (idLoyalty >= 1 )
	{
		DecrementTownLoyaltyEverywhere( idLoyalty );
	}	

	return 0;
}

// add loyalty 
static int l_IncrementTownLoyaltyEverywhere(lua_State *L)
{
	UINT32 n = lua_gettop(L);
	UINT32 idLoyalty = 0;


//	for (i= 1; i<=n; i++ )
	//{
	//	if (i == 1 ) 
			idLoyalty = lua_tointeger(L,1);
	//}
	
	
	if (idLoyalty >= 1 )
	{
		IncrementTownLoyaltyEverywhere( idLoyalty );
	}	

	return 0;
}

// remove town loyalty 
static int l_DecrementTownLoyalty(lua_State *L)
{
	UINT32 n = lua_gettop(L);
	UINT32 idLoyalty = 0, idTown = 0;


	//for (i= 1; i<=n; i++ )
	//{
	//	if (i == 1 ) 
	idTown = lua_tointeger(L,1);
	///	if (i == 2 ) 
	idLoyalty = lua_tointeger(L,2);
	//}
	
	if (idLoyalty >= 1 )
	{
		DecrementTownLoyalty( idTown, idLoyalty );
	}	
	
	return 0;
}

// set town loyalty 0 - 100% 
static int l_SetTownLoyalty(lua_State *L)
{
	UINT32 idLoyalty,idTown;
	UINT8 n = lua_gettop(L);
	int i = 0;
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) idTown = lua_tointeger(L,i);
		if (i == 2 ) idLoyalty = lua_tointeger(L,i);
	}
	
	if (idLoyalty >= 1 )
	{
		SetTownLoyalty( idTown, idLoyalty );
	}	
	
	return 0;
}
//------------------------------------------------

//set Hospital Temp Balance
static int l_HospitalTempBalance(lua_State *L)
{
	UINT32 Balance = 0;
	UINT8 n = lua_gettop(L);

	Balance = lua_tointeger(L,n);

	giHospitalTempBalance = Balance;
	return 0;
}

//set PlayerTeamSawJoey
static int l_gfPlayerTeamSawJoey(lua_State *L)
{
	BOOLEAN set = FALSE;
	UINT8 n = lua_gettop(L);

	set = lua_toboolean(L,n);

	gfPlayerTeamSawJoey = set;
	return 0;
}

// set
static int l_HospitalRefund(lua_State *L)
{
	UINT32 Balance = 0;
	UINT8 n = lua_gettop(L);

	Balance = lua_tointeger(L,n);

	giHospitalRefund = Balance;
	return 0;
}

//set
static int l_HospitalPriceModifier(lua_State *L)
{
	UINT32 Balance = 0;
	UINT8 n = lua_gettop(L);

	Balance = lua_tointeger(L,n);

	gbHospitalPriceModifier = Balance;
	return 0;
}

//add character to sector
static int l_AddNPC(lua_State *L)
{
	MERCPROFILESTRUCT * pProfile;

	std::string sector_id = lh_getStringFromTable(L, "sector");
	UINT8 idNPC = 0;
	UINT8 x = 0,y = 0,z = 0;

	if (locationStringToCoordinates(sector_id, &x, &y, &z))
	{
		idNPC = lh_getIntegerFromTable(L, "MercProfiles");

		pProfile = &(gMercProfiles[ idNPC ]);
	
		pProfile->sSectorX = x;
		pProfile->sSectorY = y;
		pProfile->bSectorZ = z;
	}
		
	return 0;
}

//add alternate sector
static int l_AddAlternateSector(lua_State *L)
{

	std::string sector_id = lh_getStringFromTable(L, "altSector");

	UINT8 x = 0,y = 0;

	if (locationStringToCoordinates_AltSector(sector_id, &x, &y))
	{
		SectorInfo[ SECTOR( x, y ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
	}
	return 0;
}

//add alternate under ground sector
static int l_AddAltUnderGroundSector(lua_State *L)
{
	std::string sector_id = lh_getStringFromTable(L, "altSector");

	UINT8 x = 0,y = 0,z = 0;

	if (locationStringToCoordinates(sector_id, &x, &y, &z))
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( x, y, z ); 
		if( pSector )
		{
			pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		}
	}
	
	return 0;	
}

//--------------Briefing room-----------------------30-06-2011

static int l_SetEndMission(lua_State *L)
{
	UINT32 idMission, nextMission;
	UINT8 n = lua_gettop(L);
	int i = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) idMission = lua_tointeger(L,i);
	}
	
	if ( gBriefingRoomData[idMission].CheckMission == MISSIONSTART ) 
	{
		gBriefingRoomData[idMission].CheckMission = MISSIONEND; //set end mission 
		nextMission = gBriefingRoomData[idMission].NextMission;
		if ( nextMission != -1 )
		gBriefingRoomData[nextMission].Hidden = TRUE; // set next mission
	}
	 
	return 0;
}

static int l_SetStartMission(lua_State *L)
{
	UINT32 idMission;
	UINT8 n = lua_gettop(L);
	int i = 0;
	
	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) idMission = lua_tointeger(L,i);
	}
	
	if ( gBriefingRoomData[idMission].CheckMission != MISSIONSTART || gBriefingRoomData[idMission].CheckMission != MISSIONEND || gBriefingRoomData[idMission].CheckMission == MISSIONNOSTARTED ) 
	{
		gBriefingRoomData[idMission].CheckMission = MISSIONSTART; //set start mission 
		gBriefingRoomData[idMission].Hidden = TRUE; // set next mission
	}
	 
	return 0;
}

static int l_CheckMission (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	UINT32 idMission = 0;
	UINT32 Bool = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) idMission = lua_tointeger(L,i);
	}

	if ( gBriefingRoomData[idMission].CheckMission == MISSIONNOSTARTED)
		Bool = 0;
	else if ( gBriefingRoomData[idMission].CheckMission == MISSIONSTART)
		Bool = 1;
	else if (gBriefingRoomData[idMission].CheckMission == MISSIONEND)	
		Bool = 2;
		
	lua_pushinteger(L, Bool);
		
		
	return 1;
}

//--------------------------------------------------------------

static int l_RemoveGraphicFromTempFile (lua_State *L)
{
	UINT8  n = lua_gettop(L);
	int i = 0;
	INT32 uiMapIndex;
	UINT16 usIndex;
	INT16 sSectorX;
	INT16 sSectorY;
	UINT8 ubSectorZ; 

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) uiMapIndex = lua_tointeger(L,i);
		if (i == 2 ) usIndex = lua_tointeger(L,i);
		if (i == 3 ) sSectorX = lua_tointeger(L,i);
		if (i == 4 ) sSectorY = lua_tointeger(L,i);
		if (i == 5 ) ubSectorZ = lua_tointeger(L,i);
	}
	
	RemoveGraphicFromTempFile( uiMapIndex, usIndex, sSectorX, sSectorY, ubSectorZ );


	return 0;
}

//------------ Towns function
/*
static int l_ResizeTown (lua_State *L)
{
	UINT8 n = lua_gettop(L);

	UINT16 x = 0;
	UINT16 y = 0;
	UINT8 TownID;
	
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) x = lua_tointeger(L,i);
		if (i == 2 ) y = lua_tointeger(L,i);
		if (i == 3 ) TownID = lua_tointeger(L,i);		
	}
	
	if ( TownID > 0 && TownID <= NUM_TOWNS ) //MAX_TOWNS
		StrategicMap[SECTOR( x, y )].bNameId = TownID;

	return 0;
}

static int l_EraseTown (lua_State *L)
{
	UINT8 n = lua_gettop(L);

	UINT8 TownID,cnt;
	
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) TownID = lua_tointeger(L,i);		
	}
		
	for (cnt = 0; cnt < 256; cnt++)
	{
		if ( StrategicMap[cnt].bNameId == TownID && TownID < NUM_TOWNS ) StrategicMap[cnt].bNameId = 0;
	}

	return 0;
}
*/
static int l_HiddenTown (lua_State *L)
{
	UINT8 n = lua_gettop(L);
	UINT8 TownID;
	
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) TownID = lua_tointeger(L,i);
	}
	
	if ( TownID > 0 && TownID <= NUM_TOWNS )
		gfHiddenTown[TownID] = FALSE;

	return 0;
}

static int l_VisibleTown (lua_State *L)
{
	UINT8 n = lua_gettop(L);
	UINT8 TownID;
	
	int i = 0;

	for (i= 1; i<=n; i++ )
	{
		if (i == 1 ) TownID = lua_tointeger(L,i);
	}
	
	if ( TownID > 0 && TownID <= NUM_TOWNS )
		gfHiddenTown[TownID] = TRUE;

	return 0;
}

//---------------

static int lh_getBooleanFromTable(lua_State *L, const char * fieldname)
{
	lua_getfield(L, -1, fieldname);
	int i = lua_toboolean(L, -1);
	lua_pop(L, 1);
	return i;
}

static int lh_getIntegerFromTable(lua_State *L, const char * fieldname)
{
	lua_getfield(L, -1, fieldname);
	int i = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return i;
}

static std::string lh_getStringFromTable(lua_State *L, const char * fieldname)
{
	lua_getfield(L, -1, fieldname);
	string s = lua_tostring(L, -1);
	lua_pop(L, 1);
	return s;
}

static bool locationStringToCoordinates_AltSector(std::string loc, UINT8* x, UINT8* y)
{
	int length = loc.length();
	
	// gather row
	if (length < 2 || length > 3)
		return false;
	
	char row = loc[0];
	if (row >= 'A' && row <= 'P')
	{
		*y = row - 'A' + 1;
	}
	else if (row >= 'a' && row <= 'p')
	{
		*y = row - 'a' + 1;
	}
	else
	{
		return false;
	}
	
	// gather column
#if 0
	loc = loc.substr(1);
	stringstream ss = stringstream();
	if (loc[0] >= '0' && loc[0] <= '9')
	{
		ss << loc[0];
		loc = loc.substr(1);
	}
	if (loc[0] >= '0' && loc[0] <= '9')
	{
		ss << loc[0];
		loc = loc.substr(1);
	}
#else
	stringstream ss(loc.substr(1));
#endif
	int col = 0;
	ss >> col;
	if (col >= 1 && col <= 16)
	{
		*x = col;
	}
	else
	{
		return false;
	}
	
	return true;
}

// extract x,y,z from strings like "D13-1"
static bool locationStringToCoordinates(std::string loc, UINT8* x, UINT8* y, UINT8* z)
{
	int length = loc.length();
	
	// gather row
	if (length < 4 || length > 5)
		return false;
	
	char row = loc[0];
	if (row >= 'A' && row <= 'P')
	{
		*y = row - 'A' + 1;
	}
	else if (row >= 'a' && row <= 'p')
	{
		*y = row - 'a' + 1;
	}
	else
	{
		return false;
	}
	
	// gather column
#if 0
	loc = loc.substr(1);
	stringstream ss = stringstream();
	if (loc[0] >= '0' && loc[0] <= '9')
	{
		ss << loc[0];
		loc = loc.substr(1);
	}
	if (loc[0] >= '0' && loc[0] <= '9')
	{
		ss << loc[0];
		loc = loc.substr(1);
	}
#else
	stringstream ss(loc);
#endif
	int col = 0;
	ss >> col;
	if (col >= 1 && col <= 16)
	{
		*x = col;
	}
	else
	{
		return false;
	}
	
	// gather level
	char lvl = loc[loc.length() - 1];
	if (lvl >= '0' && lvl <= '3')
	{
		*z = lvl - '0';
	}
	else
	{
		return false;
	}
	return true;
}



static int l_gTacticalStatus(lua_State *L)
{

	return 0;
}


//--------------------------------------------------------------


BOOLEAN SaveLuaGlobalToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	FileWrite( hFile, &gLuaGlobal, sizeof( gLuaGlobal), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gLuaGlobal ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN LoadLuaGlobalFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	FileRead( hFile, &gLuaGlobal, sizeof( gLuaGlobal ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gLuaGlobal ) )
	{
		return( FALSE );
	}

	return( TRUE );
}
//--------------------------------------------------------------