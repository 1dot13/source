#ifndef _MUSIC_CONTROL_H_
#define _MUSIC_CONTROL_H_

enum MusicList
{
	MARIMBAD2_MUSIC,
	MENUMIX_MUSIC,
	NOTHING_A_MUSIC,
	NOTHING_B_MUSIC,
	NOTHING_C_MUSIC,
	NOTHING_D_MUSIC,
	TENSOR_A_MUSIC,
	TENSOR_B_MUSIC,
	TENSOR_C_MUSIC,
	TRIUMPH_MUSIC,
	DEATH_MUSIC,
	BATTLE_A_MUSIC,
	BATTLE_B_MUSIC, //same as tensor B
	CREEPY_MUSIC,
	CREATURE_BATTLE_MUSIC,
	NUM_MUSIC
};

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
};

//extern UINT32 uiMusicHandle;
//extern BOOLEAN fMusicPlaying;
//extern UINT8 gubMusicMode;
//extern BOOLEAN gfForceMusicToTense;

UINT8 GetMusicMode(void);
BOOLEAN SetMusicMode(UINT8 ubMusicMode);

// only for editor (editscreen.cpp)
BOOLEAN MusicPlay(UINT32 uiNum);

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
