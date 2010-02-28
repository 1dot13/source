#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __SUMMARY_INFO_H
#define __SUMMARY_INFO_H

#include "Types.h"

//dnl ch28
#define GLOBAL_SUMMARY_VERSION	15
#define MINIMUMVERSION			7

typedef struct TEAMSUMMARY
{
	UINT8 ubTotal;
	UINT8 ubDetailed;
	UINT8 ubProfile;
	UINT8	ubExistance;
	UINT8 ubNumAnimals;
	UINT8 ubBadA, ubPoorA, ubAvgA, ubGoodA, ubGreatA; //attributes
	UINT8 ubBadE, ubPoorE, ubAvgE, ubGoodE, ubGreatE; //equipment
}TEAMSUMMARY; //15 bytes

typedef struct
{
	//start version 1
	UINT8 ubSummaryVersion;									
	UINT8 ubSpecial;									
	UINT16 usNumItems;								
	UINT16 usNumLights;								
	_OLD_MAPCREATE_STRUCT MapInfo;					
	TEAMSUMMARY EnemyTeam;						
	TEAMSUMMARY CreatureTeam;					
	TEAMSUMMARY RebelTeam;						
	TEAMSUMMARY CivTeam;							
	UINT8 ubNumDoors;									
	UINT8 ubNumDoorsLocked;						
	UINT8 ubNumDoorsTrapped;					
	UINT8 ubNumDoorsLockedAndTrapped;	
	//start version 2
	UINT8	ubTilesetID;								
	UINT8 ubNumRooms;									
	//start version	3									
	UINT8 ubNumElites;								
	UINT8 ubNumAdmins;								
	UINT8 ubNumTroops;								
	//start version 4				
	UINT8 ubEliteDetailed;						
	UINT8 ubAdminDetailed;						
	UINT8 ubTroopDetailed;						
	//start version 5									
	UINT8 ubEliteProfile;							
	UINT8 ubAdminProfile;							
	UINT8 ubTroopProfile;							
	//start version 6									
	UINT8	ubEliteExistance;						
	UINT8 ubAdminExistance;						
	UINT8 ubTroopExistance;
	//start version 7
	FLOAT	dMajorMapVersion;
	//start version 8
	UINT8 ubCivSchedules;
	//start version 9
	UINT8	ubCivCows;
	UINT8 ubCivBloodcats;
	//																//-----
																		//	190
	//start version 10
	_OLD_EXITGRID ExitGrid[4];			//5*4 //	20
	UINT16 usExitGridSize[4];	//2*4 //	8
	BOOLEAN fInvalidDest[4];					//	4
	UINT8	ubNumExitGridDests;				//		1
	BOOLEAN fTooManyExitGridDests;		//		1
	//																//-----
	//																//	224
	//start version 11
	UINT8	ubEnemiesReqWaypoints;			//		1
	//																//-----
	//																		225
	//start version 12
	UINT16 usWarningRoomNums;					//	2
																		//	227
	//start version 13
	UINT8 ubEnemiesHaveWaypoints;			//		1
	UINT32 uiNumItemsPosition;				//		4
																		//-----
																		//	232
	//start version 14
	UINT32 uiEnemyPlacementPosition;	//		4
																		//-----
																		//	236
	

	UINT8 ubPadding[164];							//	164
	//																//-----
	//																		400 total bytes
}_OLD_SUMMARYFILE;//dnl ch28 240909

class SUMMARYFILE//dnl ch28 260909
{
public:
	UINT8 ubSummaryVersion;//This byte in all versions must be first
	UINT8 ubSpecial;
	UINT16 ubNumRooms;
	UINT32 usNumItems;
	UINT32 usNumLights;
	UINT16 ubNumDoors;
	UINT16 ubNumDoorsLocked;
	UINT16 ubNumDoorsTrapped;
	UINT16 ubNumDoorsLockedAndTrapped;
	UINT16 ubNumElites;
	UINT16 ubNumAdmins;
	UINT16 ubNumTroops;
	UINT16 ubEliteDetailed;
	UINT16 ubAdminDetailed;
	UINT16 ubTroopDetailed;
	UINT16 ubEliteProfile;
	UINT16 ubAdminProfile;
	UINT16 ubTroopProfile;
	UINT16 ubEliteExistance;
	UINT16 ubAdminExistance;
	UINT16 ubTroopExistance;
	UINT16 ubCivSchedules;
	UINT16 ubCivCows;
	UINT16 ubCivBloodcats;
	UINT16 ubEnemiesReqWaypoints;
	UINT16 usWarningRoomNums;
	UINT16 ubEnemiesHaveWaypoints;
	UINT32 ubTilesetID;
	UINT32 uiNumItemsPosition;
	UINT32 uiEnemyPlacementPosition;
	FLOAT dMajorMapVersion;
	TEAMSUMMARY EnemyTeam;
	TEAMSUMMARY CreatureTeam;
	TEAMSUMMARY RebelTeam;
	TEAMSUMMARY CivTeam;
	MAPCREATE_STRUCT MapInfo;
	EXITGRID ExitGrid[4];
	BOOLEAN fInvalidDest[4];
	UINT16 usExitGridSize[4];
	UINT16 ubNumExitGridDests;
	BOOLEAN fTooManyExitGridDests;
public:
	SUMMARYFILE& operator=(const _OLD_SUMMARYFILE& src);
	BOOLEAN Load(STR sFileName);
	BOOLEAN Save(STR sFileName);
};

extern BOOLEAN gfAutoLoadA9;

BOOLEAN EvaluateWorld(STR8 pSector, UINT8 ubLevel);//dnl ch42 260909
void WriteSectorSummaryUpdate(STR8 sFileName, UINT8 ubLevel, SUMMARYFILE* pSummaryFileInfo);//dnl ch28 260909

extern BOOLEAN gfMustForceUpdateAllMaps;
extern BOOLEAN gfMajorUpdate;
BOOLEAN ReEvaluateWorld( const STR8	puiFilename );
void ApologizeOverrideAndForceUpdateEverything();
void ClearSummaryInfo();

#endif 
#endif
