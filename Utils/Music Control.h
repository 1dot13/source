#ifndef _MUSIC_CONTROL_H_
#define _MUSIC_CONTROL_H_

#include "Overhead Types.h"

enum MusicMode
{
	MUSIC_NONE,
	MUSIC_RESTORE,
	MUSIC_MAIN_MENU,
	MUSIC_TACTICAL_NOTHING,
	MUSIC_TACTICAL_ENEMYPRESENT,
	MUSIC_TACTICAL_BATTLE,
	MUSIC_TACTICAL_VICTORY,
	MUSIC_TACTICAL_DEATH,
	MUSIC_LAPTOP,
	MUSIC_OLD_TYPE,
	MUSIC_TACTICAL_BATTLE_MUSIC,
	MUSIC_TACTICAL_CREEPY_MUSIC,
	OTHER_MUSIC_TACTICAL,
	MUSIC_TACTICAL_CIV_GROUP_BATTLE,
};

enum NewMusicList
{
	MUSICLIST_MAIN_MENU,
	MUSICLIST_LAPTOP,
	MUSICLIST_TACTICAL_NOTHING,
	MUSICLIST_TACTICAL_ENEMYPRESENT,
	MUSICLIST_TACTICAL_BATTLE,
	MUSICLIST_TACTICAL_ENEMYPRESENT_NIGHT,
	MUSICLIST_TACTICAL_BATTLE_NIGHT,
	MUSICLIST_TACTICAL_VICTORY,
	MUSICLIST_TACTICAL_DEATH,
	MUSICLIST_TACTICAL_CREEPY,
	MUSICLIST_TACTICAL_CREEPY_BATTLE,

	MAX_MUSIC,
};

extern std::vector<STR> MusicLists[MAX_MUSIC];

void InitializeMusicLists();

UINT8 GetMusicMode(void);
BOOLEAN SetMusicMode(UINT8 ubMusicMode);

// Used in lua scripting and editscreen.cpp
BOOLEAN MusicPlay(NewMusicList mode, UINT8 songIndex);

UINT32 MusicGetVolume(void);
BOOLEAN MusicSetVolume(UINT32 uiVolume);

BOOLEAN MusicPoll(BOOLEAN fForce);

void SetMusicFadeSpeed(INT8 bFadeSpeed);

BOOLEAN UsingCreatureMusic(void);
void UseCreatureMusic(BOOLEAN fUseCreatureMusic);

// only for luaglobal.cpp
BOOLEAN IsMusicPlaying(void);
UINT32 GetMusicHandle(void);

//BOOLEAN MusicStop(void);
//BOOLEAN MusicFadeOut(void);
//BOOLEAN MusicFadeIn(void);
//void FadeMusicForXSeconds( UINT32 uiDelay );

#endif
