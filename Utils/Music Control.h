#ifndef _MUSIC_CONTROL_H_
#define _MUSIC_CONTROL_H_

#include "Overhead Types.h"

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
	MUSIC_DIR,
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
	MUSIC_OLD_TYPE,
	MUSIC_TACTICAL_BATTLE_MUSIC,
	MUSIC_TACTICAL_CREEPY_MUSIC,
	OTHER_MUSIC_TACTICAL,
	MUSIC_TACTICAL_CIV_GROUP_BATTLE,
};

#ifdef NEWMUSIC
typedef struct
{
	UINT16 uiIndex;
	INT32 SoundTacticalVictory[4];
	INT32 SoundTacticalBattle[4];
	INT32 SoundTacticalNothing[4];
	INT32 SoundTacticalTensor[4];
	INT32 SoundTacticalDeath[4];
	INT32 SoundTacticalBattleCreature[4];
	INT32 SoundTacticalBattleCreepy[4];
	INT32 SoundTacticalBattleGroup[NUM_CIV_GROUPS];

} MUSIC_SOUND_VALUES;

extern MUSIC_SOUND_VALUES MusicSoundValues[256];
extern INT32 GlobalSoundID;
#endif

//extern UINT32 uiMusicHandle;
//extern BOOLEAN fMusicPlaying;
//extern UINT8 gubMusicMode;
//extern BOOLEAN gfForceMusicToTense;

UINT8 GetMusicMode(void);
BOOLEAN SetMusicMode(UINT8 ubMusicMode);

// only for editor (editscreen.cpp)
#ifdef NEWMUSIC
BOOLEAN MusicPlay(UINT32 uiNum, UINT32 MusicMode, BOOLEAN NewSound);
#else
BOOLEAN MusicPlay(UINT32 uiNum);
#endif

#ifdef NEWMUSIC
BOOLEAN SetMusicModeID(UINT8 ubMusicMode, INT32 SoundID);
#endif
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
