#ifndef FADE_SCREEN_H
#define FADE_SCREEN_H

#define	FADE_OUT_VERSION_ONE			1
#define	FADE_OUT_VERSION_FASTER		2
#define	FADE_OUT_VERSION_SIDE			3
#define	FADE_OUT_SQUARE						4
#define	FADE_OUT_REALFADE					5

#define	FADE_IN_VERSION_ONE				10
#define	FADE_IN_SQUARE						11
#define FADE_IN_REALFADE					12

typedef void (*FADE_HOOK)( void );	

extern FADE_HOOK		gFadeInDoneCallback;
extern FADE_HOOK		gFadeOutDoneCallback;


typedef void (*FADE_FUNCTION)( void );	


extern BOOLEAN						gfFadeInitialized;
extern BOOLEAN						gfFadeIn;
extern INT8							gbFadeType;
extern FADE_FUNCTION			gFadeFunction;
extern BOOLEAN						gfFadeInVideo;

void BeginFade( UINT32 uiExitScreen, INT8 bFadeValue, INT8 bType, UINT32 uiDelay );

BOOLEAN HandleBeginFadeIn( UINT32 uiScreenExit );
BOOLEAN HandleBeginFadeOut( UINT32 uiScreenExit );

BOOLEAN HandleFadeOutCallback( );
BOOLEAN HandleFadeInCallback( );

void FadeInNextFrame( );
void FadeOutNextFrame( );

#endif