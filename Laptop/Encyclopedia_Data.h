#ifndef __Encyclopedia_Data_H
#define __Encyclopedia_Data_H

#include "Encyclopedia.h"
#include "Encyclopedia_Data.h"

void GameInitEncyclopediaLocation();
BOOLEAN EnterEncyclopediaLocation();
void ExitEncyclopediaLocation();
void HandleEncyclopediaLocation();
void RenderEncyclopediaLocation(BOOLEAN bHidden);

extern BOOLEAN bEncyclopediaLocation;
extern BOOLEAN bEncyclopediaCharacter;
extern BOOLEAN bEncyclopediaInventory;
extern BOOLEAN bEncyclopediaQuests;
extern BOOLEAN bBriefingRoom;
extern BOOLEAN bBriefingRoomSpecialMission;

extern UINT32 IDPageEncyData;
extern void InitData ( BOOLEAN bInit);
extern void UnLoadMenuButtons ();
extern BOOLEAN ResetVal;

extern void CopyToTemp ( ENCYCLOPEDIA_LOCATION *Ency, BOOLEAN bFiltr, INT32 sort, INT32 TypFiltr, BOOLEAN ShowBox );
extern void ResetTemp();

extern BOOLEAN LoadEncyclopediaFromLoadGameFile( HWFILE hFile );
extern BOOLEAN SaveEncyclopediaToSaveGameFile( HWFILE hFile );

#define PAGENONE						0
#define PAGEENCYCLOPEDIALOCATION		1
#define PAGEENCYCLOPEDIACHARACTER		2
#define PAGEENCYCLOPEDIAINVENTORY		3
#define PAGEENCYCLOPEDIAQUESTS			4
#define PAGEBRIEFINGROOM				5
#define PAGEBRIEFINGROOMSPECIALMISSION	6


#endif