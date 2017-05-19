#ifndef __BriefingRoom_Data_H
#define __BriefingRoom_Data_H

#include "Quests.h"

BOOLEAN EnterEncyclopediaLocation();
void ExitEncyclopediaLocation();
void HandleEncyclopediaLocation();
void RenderEncyclopediaLocation(BOOLEAN bHidden);

extern BOOLEAN bBriefingRoom;
extern BOOLEAN bBriefingRoomSpecialMission;

#define NUM_MISSION NUM_SECTOR

#define NUM_MAX_TEMP	MAXITEMS+1

#define TEXT_NUM_ENC TEXT_NUM_ENCYCLOPEDIA_HELP_TEXT

#define PAGENONE						0
#define PAGEENCYCLOPEDIALOCATION		1
#define PAGEENCYCLOPEDIACHARACTER		2
#define PAGEENCYCLOPEDIAINVENTORY		3
#define PAGEENCYCLOPEDIAQUESTS			4
#define PAGEBRIEFINGROOM				5
#define PAGEBRIEFINGROOMSPECIALMISSION	6

#define		ENCYCLOPEDIA_DECRIPTION_SIZE				1120
#define		ENCYCLOPEDIA_NAME_SIZE						160
#define		NUM_SECTOR									4*255
#define 	MAX_ENCYCLOPEDIA_CHARS		500



typedef struct
{
	//Profiles/Locations
	UINT32   	uiIndex;
	
	CHAR16		Name[ENCYCLOPEDIA_NAME_SIZE];

	BOOLEAN   	Hidden;
	
	UINT32 sImagePositionX[4];
	UINT32 sImagePositionY[4];
	
	UINT32 MaxPages;
	INT32 MaxImages;
	
	BOOLEAN bvisible;
	
	CHAR16		sCode[ENCYCLOPEDIA_DECRIPTION_SIZE];
	
	INT32 NextMission;
	UINT32 MissionID;
	INT32 CheckMission;
	
}	BRIEFINGROOM_M_DATA;

#define MAX_IMAGES 4
#define MAX_PAGES 4

#define MISSIONNOSTARTED 0
#define MISSIONSTART 1
#define MISSIONEND  2

typedef struct
{
	UINT32   	uiIndex;
	BOOLEAN   	Hidden;
	BOOLEAN 	bvisible;
}	ENCYCLOPEDIA_PROF_SAVE;

typedef struct
{
	UINT32   	uiIndex;
	BOOLEAN   	Hidden;
	INT32   	CheckMission;
}	MISSION_SAVE;

extern void InitEncyklopediaBool();

extern void ShowNPCEncyclopediaEntry(UINT8 ubNPC, BOOLEAN hidden);

#ifdef ENABLE_BRIEFINGROOM
extern BRIEFINGROOM_M_DATA gLoadBriefingRoomReservedData1[ NUM_SECTOR ];
extern BRIEFINGROOM_M_DATA gLoadBriefingRoomReservedData2[ NUM_PROFILES ];
extern BRIEFINGROOM_M_DATA gLoadBriefingRoomReservedData3[ MAXITEMS ];
extern BRIEFINGROOM_M_DATA gLoadBriefingRoomReservedData4[ NUM_PROFILES ];
extern BRIEFINGROOM_M_DATA gLoadBriefingRoomReservedData5[ MAX_QUESTS ];

extern BOOLEAN saveBriefingRoomReservedData1[ NUM_SECTOR ];
extern ENCYCLOPEDIA_PROF_SAVE saveBriefingRoomReservedData2[ NUM_PROFILES ];
extern BOOLEAN saveBriefingRoomReservedData3[ MAXITEMS ];
extern BOOLEAN saveBriefingRoomReservedData4[ NUM_PROFILES ];
extern BOOLEAN saveBriefingRoomReservedData5[ MAX_QUESTS ];
#else
extern BRIEFINGROOM_M_DATA gLoadBriefingRoomReservedData1[];
#endif // ENABLE_BRIEFINGROOM

#ifdef ENABLE_BRIEFINGROOM
extern BRIEFINGROOM_M_DATA gbriefingRoomDataTemp[ NUM_MAX_TEMP ];
#endif

#ifdef ENABLE_BRIEFINGROOM
extern BRIEFINGROOM_M_DATA gBriefingRoomData[ NUM_MISSION ];
extern BRIEFINGROOM_M_DATA gBriefingRoomSpecialMissionData[ NUM_MISSION ];

extern BRIEFINGROOM_M_DATA gBriefingRoomDataBackup[ NUM_MISSION ];
extern BRIEFINGROOM_M_DATA gBriefingRoomSpecialMissionDataBackup[ NUM_MISSION ];

extern MISSION_SAVE saveBriefingRoomData[ NUM_MISSION ];
extern MISSION_SAVE saveBriefingRoomSpecialMissionData[ NUM_MISSION ];
extern MISSION_SAVE saveMissionData[ NUM_MISSION ];
#else
extern BRIEFINGROOM_M_DATA gBriefingRoomData[];
#endif // ENABLE_BRIEFINGROOM



extern void BackupBRandEncyclopedia ( BRIEFINGROOM_M_DATA *EncyBackup, BRIEFINGROOM_M_DATA *Ency, UINT32 NUM);

extern UINT32 IDPageEncyData;
extern void InitData ( BOOLEAN bInit);
extern void UnLoadMenuButtons ();
extern BOOLEAN ResetVal;

extern void CopyToTemp ( BRIEFINGROOM_M_DATA *Ency, BOOLEAN bFiltr, INT32 sort, INT32 TypFiltr, BOOLEAN ShowBox );
extern void ResetTemp();

extern BOOLEAN LoadBriefingRoomFromLoadGameFile( HWFILE hFile );
extern BOOLEAN SaveBriefingRoomToSaveGameFile( HWFILE hFile );



#endif