#ifndef _CINEMATICS_BINK__H_
#define _CINEMATICS_BINK__H_


#include "bink.h"




// BINKFLIC uiFlags
#define BINK_FLIC_OPEN					0x00000001							// Flic is open
#define BINK_FLIC_PLAYING				0x00000002							// Flic is playing
#define BINK_FLIC_LOOP					0x00000004							// Play flic in a loop
#define BINK_FLIC_AUTOCLOSE				0x00000008							// Close when done
#define	BINK_FLIC_CENTER_VERTICAL		0x00000010							// Center the video on screen vertically




// Paused???

struct BINKFLIC
{
	const CHAR8					*cFilename;
	HWFILE						hFileHandle;
	HBINK						BinkHandle;
///	SmackBuf					*SmackBuffer;
	UINT32						uiFlags;
	LPDIRECTDRAWSURFACE2		lpDDS;
	HWND						hWindow;
	UINT32						uiFrame;
	UINT32						uiLeft, uiTop;
//	LPDIRECTDRAW2				lpDD;
//	UINT32						uiNumFrames;
//	UINT8						*pAudioData;
//	UINT8						*pCueData;

};



void			BinkInitialize(HWND hWindow, UINT32 uiWidth, UINT32 uiHeight);
BOOLEAN			BinkPollFlics(void);
void			BinkCloseFlic(BINKFLIC *pBink);
void			BinkShutdownVideo(void);
BINKFLIC		*BinkPlayFlic(const CHAR8 *cFilename, UINT32 uiLeft, UINT32 uiTop, UINT32 uiFlags );


#endif