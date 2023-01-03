#ifndef __AIMARCHIVES_H_
#define __AIMARCHIVES_H_

#include "Soldier Profile.h"

void GameInitAimArchives();
BOOLEAN EnterAimArchives();
void ExitAimArchives();
void HandleAimArchives();
void RenderAimArchives();
void EnterInitAimArchives();

typedef struct
{
	UINT8 uiIndex;
	CHAR16	szNickName[160];
	CHAR16	szName[160];
	CHAR16	szBio[1120];
	INT16 FaceID;
	BOOLEAN visibleOldMerc;

} OLD_MERC_ARCHIVES_VALUES;

extern OLD_MERC_ARCHIVES_VALUES gAimOldArchives[NUM_PROFILES];

#endif



 


