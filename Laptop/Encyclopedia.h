#ifndef __ENCYCLOPEDIA_H
#define __ENCYCLOPEDIA_H

#include "Types.h"
#include "Quests.h"
#include "soldier profile type.h"


enum
{
	ENCYCLOPEDIA_SLOGAN,
	ENCYCLOPEDIA_WARNING_1,
	ENCYCLOPEDIA_WARNING_2,
	ENCYCLOPEDIA_COPYRIGHT_1,
	ENCYCLOPEDIA_COPYRIGHT_2,
	ENCYCLOPEDIA_COPYRIGHT_3
};

void GameInitEncyclopedia();
BOOLEAN EnterEncyclopedia();
void ExitEncyclopedia();
void HandleEncyclopedia();
void RenderEncyclopedia();

BOOLEAN RemoveEncyclopediaDefaults();
BOOLEAN InitEncyclopediaDefaults();
BOOLEAN DrawEncyclopediaDefaults();

BOOLEAN DisplayEncyclopediaSlogan();
BOOLEAN DisplayEncyclopediaCopyright();

#define		ENCYCLOPEDIA_DECRIPTION_SIZE				1120
#define		ENCYCLOPEDIA_NAME_SIZE						160
#define		NUM_SECTOR									4*255
#define 	MAX_ENCYCLOPEDIA_CHARS		500

#define NUM_MAX_TEMP	MAXITEMS+1

#define TEXT_NUM_ENC TEXT_NUM_ENCYCLOPEDIA_HELP_TEXT

enum
{
	//Locations
	NONE_NONE = 0,
	LOCATION_CITIES,
	LOCATION_SAM,
	LOCATION_OTHER,
	LOCATION_MINES,
	LOCATION_MILITARY,
	LOCATION_LABORATORY,
	LOCATION_FACTORY,
	LOCATION_HOSPITAL,
	LOCATION_PRISON,
	LOCATION_AIRPORT,

	TEXT_NUM_ENCYCLOPEDIA_HELP_TEXT
};

//Profiles
enum
{
	
	PROFIL = 0,
	PROFIL_AIM,
	PROFIL_MERC,
	PROFIL_RPC,
	PROFIL_NPC,
	PROFIL_VEHICLES,
	PROFIL_IMP,
};

//SubGroup
enum
{
	PROFIL_NONE = 0,
	PROFIL_EPC = 1,
//	PROFIL_TERRORIS,
//	PROFIL_DEIDRANNA_SOLDIER,
//	PROFIL_HICK,
//	PROFIL_KINGPIN,
//	PROFIL_KINGPIN,
};


typedef struct
{
	//Profiles/Locations
	UINT32   	uiIndex;
	CHAR8		szFile[MAX_ENCYCLOPEDIA_CHARS];
	CHAR8		szFile2[MAX_ENCYCLOPEDIA_CHARS];
	CHAR8		szFile3[MAX_ENCYCLOPEDIA_CHARS];
	CHAR8		szFile4[MAX_ENCYCLOPEDIA_CHARS];
	
	CHAR16		Name[ENCYCLOPEDIA_NAME_SIZE];
	
	
	CHAR16		sDesc1[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc2[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc3[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc4[ENCYCLOPEDIA_DECRIPTION_SIZE];
/*	CHAR16		sDesc5[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc6[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc7[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc8[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc9[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc10[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc11[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc12[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc13[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc14[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc15[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc16[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc17[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc18[ENCYCLOPEDIA_DECRIPTION_SIZE];
	CHAR16		sDesc19[ENCYCLOPEDIA_DECRIPTION_SIZE];	
	CHAR16		sDesc20[ENCYCLOPEDIA_DECRIPTION_SIZE];
	*/
	
	BOOLEAN   	Hidden;
	
	CHAR16		sImageDesc1[150];
	CHAR16		sImageDesc2[150];
	CHAR16		sImageDesc3[150];
	CHAR16		sImageDesc4[150];
	
	
	UINT32 sImagePositionX[4];
	UINT32 sImagePositionY[4];
	/*
	UINT32 sImagePosition1X;
	UINT32 sImagePosition1Y;
	UINT32 sImagePosition2X;
	UINT32 sImagePosition2Y;
	UINT32 sImagePosition3X;
	UINT32 sImagePosition3Y;
	UINT32 sImagePosition4X;
	UINT32 sImagePosition4Y;
	*/
	
	UINT32 MaxPages;
	INT32 MaxImages;
	
	//Only locations
	INT32 LocType;
	INT16 SectorX;
	INT16 SectorY;
	INT16 SectorZ;	
	
	//Only profiles
	INT32 IDProfile;
	INT32 subGroup1;
	INT32 subCivGroup;
	
	INT32 InventoryClass;
	
	BOOLEAN Filtr;
	
	BOOLEAN enableDesc;
	
	BOOLEAN bvisible;
	
	CHAR16		sCode[150];
	
	CHAR8		sSounds[MAX_ENCYCLOPEDIA_CHARS];
	
	//Quests
	INT32 QuestID;
	INT32 pQuests;
	BOOLEAN SpecialQuestImage;
	CHAR8	BeforeImage[MAX_ENCYCLOPEDIA_CHARS];
	CHAR8	AfterImage[MAX_ENCYCLOPEDIA_CHARS];
	UINT32 ImagePositionQX[10];
	UINT32 ImagePositionQY[10];
	CHAR16 ImageDescBefore[150];
	CHAR16 ImageDescAfter[150];
	
	INT32 NextMission;
	UINT32 MissionID;
	INT32 CheckMission;
	
	//Inventory
	
}	ENCYCLOPEDIA_LOCATION;

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

#if (defined ENABLE_ENCYCLOPEDIA || defined ENABLE_BRIEFINGROOM) 
extern ENCYCLOPEDIA_LOCATION gEncyclopediaDataTemp[ NUM_MAX_TEMP ];
#endif

#ifdef ENABLE_ENCYCLOPEDIA
extern ENCYCLOPEDIA_LOCATION gEncyclopediaLocationDataBackup[ NUM_SECTOR ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaProfilesDataBackup[ NUM_PROFILES ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaInventoryDataBackup[ MAXITEMS ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaOldProfilesDataBackup[ NUM_PROFILES ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaQuestsDataBackup[ MAX_QUESTS ];

extern ENCYCLOPEDIA_LOCATION gEncyclopediaLocationData[ NUM_SECTOR ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaProfilesData[ NUM_PROFILES ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaInventoryData[ MAXITEMS ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaOldProfilesData[ NUM_PROFILES ];
extern ENCYCLOPEDIA_LOCATION gEncyclopediaQuestsData[ MAX_QUESTS ];
extern BOOLEAN saveEncyclopediaLocationData[ NUM_SECTOR ];
extern ENCYCLOPEDIA_PROF_SAVE saveEncyclopediaProfilesData[ NUM_PROFILES ];
extern BOOLEAN saveEncyclopediaInventoryData[ MAXITEMS ];
extern BOOLEAN saveEncyclopediaOldProfilesData[ NUM_PROFILES ];
extern BOOLEAN saveEncyclopediaQuestsData[ MAX_QUESTS ];
#else
extern ENCYCLOPEDIA_LOCATION gEncyclopediaProfilesData[];
#endif // ENABLE_ENCYCLOPEDIA

#ifdef ENABLE_BRIEFINGROOM
extern ENCYCLOPEDIA_LOCATION gBriefingRoomData[ NUM_SECTOR ];
extern ENCYCLOPEDIA_LOCATION gBriefingRoomSpecialMissionData[ NUM_SECTOR ];

extern ENCYCLOPEDIA_LOCATION gBriefingRoomDataBackup[ NUM_SECTOR ];
extern ENCYCLOPEDIA_LOCATION gBriefingRoomSpecialMissionDataBackup[ NUM_SECTOR ];

extern MISSION_SAVE saveBriefingRoomData[ NUM_SECTOR ];
extern MISSION_SAVE saveBriefingRoomSpecialMissionData[ NUM_SECTOR ];
extern MISSION_SAVE saveMissionData[ NUM_SECTOR ];
#else
extern ENCYCLOPEDIA_LOCATION gBriefingRoomData[];
#endif // ENABLE_BRIEFINGROOM

#endif