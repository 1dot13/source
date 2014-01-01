#ifndef __GAP_H
#define __GAP_H

#include "faces.h"

typedef UINT8		AudioSample8;
typedef INT16		AudioSample16;

typedef struct
{
	SOUNDPARMS		spParams;
	CHAR8		zSoundFiles[10][64];
	UINT8		ubSndCounter;
	UINT8		ubMaxSndCounter;

} SUBSEQUENTSOUNDS;

extern SUBSEQUENTSOUNDS subsequentsounds;

void AudioGapListInit( CHAR8 *zSoundFile, AudioGapList	*pGapList );
void AudioGapListDone( AudioGapList	*pGapList );

void PollAudioGap( UINT32 uiSampleNum, AudioGapList *pGapList );


UINT32 PlayJA2GapSample( CHAR8 *zSoundFile, UINT32 usRate, UINT32 ubVolume, UINT32 ubLoops, UINT32 uiPan, AudioGapList* pData );

UINT32 PlayJA2MultipleGapSample( CHAR8 zSoundFiles[][64], UINT8 ubSoundsCount, UINT32 usRate, UINT32 ubVolume, UINT32 ubLoops, UINT32 uiPan, AudioGapList* pData );

#endif
