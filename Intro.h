#ifndef _INTRO__C_
#define _INTRO__C_



UINT32	IntroScreenShutdown( void );
UINT32	IntroScreenHandle( void );
UINT32	IntroScreenInit( void );

void	StopIntroVideo();

//enums used for when the intro screen can come up, used with 'gbIntroScreenMode'
enum EIntroType
{
#ifdef JA2UB	
	INTRO_HELI_CRASH,
#endif
	INTRO_BEGINNING,			//set when viewing the intro at the begining of the game
	INTRO_ENDING,				//set when viewing the end game video.

	INTRO_SPLASH,
};


extern	UINT32	guiSmackerSurface;


void SetIntroType( INT8 bIntroType );
extern void		DisplaySirtechSplashScreen();
extern UINT32		guiIntroExitScreen;
extern BOOLEAN		gfIntroScreenExit;
extern INT8 gbIntroScreenMode;

#define MAX_VIDEO_NAMES_CHARS		128

typedef struct
{
	UINT16		uiIndex; 
	CHAR16	szFile[MAX_VIDEO_NAMES_CHARS];

} INTRO_NAMES_VALUES;

extern INTRO_NAMES_VALUES zVideoFile[255];

#endif