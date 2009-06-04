#ifndef __JASCREENS_H_
#define __JASCREENS_H_

#include "sgp.h"
#include "ScreenIds.h"
#include "mousesystem.h"
#include "Button System.h"
#include <list>
#include "VFS/utf8string.h"

extern UINT32 EditScreenInit(void);
extern UINT32 EditScreenHandle(void);
extern UINT32 EditScreenShutdown(void);

extern UINT32 LoadSaveScreenInit(void);
extern UINT32 LoadSaveScreenHandle(void);
extern UINT32 LoadSaveScreenShutdown(void);

extern UINT32 SavingScreenInitialize(void);
extern UINT32 SavingScreenHandle(void);
extern UINT32 SavingScreenShutdown(void);

extern UINT32 LoadingScreenInitialize(void);
extern UINT32 LoadingScreenHandle(void);
extern UINT32 LoadingScreenShutdown(void);

extern UINT32 ErrorScreenInitialize(void);
extern UINT32 ErrorScreenHandle(void);
extern UINT32 ErrorScreenShutdown(void);

extern UINT32 InitScreenInitialize(void);
extern UINT32 InitScreenHandle(void);
extern UINT32 InitScreenShutdown(void);

extern UINT32 MainGameScreenInit(void);
extern UINT32 MainGameScreenHandle(void);
extern UINT32 MainGameScreenShutdown(void);

#ifdef JA2BETAVERSION
extern UINT32 AIViewerScreenInit(void);
extern UINT32 AIViewerScreenHandle(void);
extern UINT32 AIViewerScreenShutdown(void);
#endif

extern	UINT32	QuestDebugScreenInit(void);
extern	UINT32	QuestDebugScreenHandle(void);
extern	UINT32	QuestDebugScreenShutdown(void);

UINT32 AniEditScreenInit(void);
UINT32 AniEditScreenHandle(void);
UINT32 AniEditScreenShutdown(void);

UINT32 PalEditScreenInit(void);
UINT32 PalEditScreenHandle(void);
UINT32 PalEditScreenShutdown(void);

UINT32 DebugScreenInit(void);
UINT32 DebugScreenHandle(void);
UINT32 DebugScreenShutdown(void);

UINT32 MapScreenInit(void);
UINT32 MapScreenHandle(void);
UINT32 MapScreenShutdown(void);

UINT32 LaptopScreenInit(void);
UINT32 LaptopScreenHandle(void);
UINT32 LaptopScreenShutdown(void);


UINT32	MapUtilScreenInit( void );
UINT32	MapUtilScreenHandle( void );
UINT32	MapUtilScreenShutdown( void );

UINT32	FadeScreenInit( void );
UINT32	FadeScreenHandle( void );
UINT32	FadeScreenShutdown( void );

UINT32	MessageBoxScreenInit( void );
UINT32	MessageBoxScreenHandle( void );
UINT32	MessageBoxScreenShutdown( void );

UINT32	MainMenuScreenInit( void );
UINT32	MainMenuScreenHandle( void );
UINT32	MainMenuScreenShutdown( void );

UINT32	AutoResolveScreenInit( void );
UINT32	AutoResolveScreenHandle( void );
UINT32	AutoResolveScreenShutdown( void );


UINT32	SaveLoadScreenShutdown( void );
UINT32	SaveLoadScreenHandle( void );
UINT32	SaveLoadScreenInit( void );

UINT32	OptionsScreenShutdown( void );
UINT32	OptionsScreenHandle( void );
UINT32	OptionsScreenInit( void );

UINT32	ShopKeeperScreenInit( void );
UINT32	ShopKeeperScreenHandle( void );
UINT32	ShopKeeperScreenShutdown( void );

UINT32	SexScreenInit( void );
UINT32	SexScreenHandle( void );
UINT32	SexScreenShutdown( void );

UINT32	GameInitOptionsScreenInit( void );
UINT32	GameInitOptionsScreenHandle( void );
UINT32	GameInitOptionsScreenShutdown( void );

UINT32	DemoExitScreenInit( void );
UINT32	DemoExitScreenHandle( void );
UINT32	DemoExitScreenShutdown( void );

extern	UINT32	IntroScreenShutdown( void );
extern	UINT32	IntroScreenHandle( void );
extern	UINT32	IntroScreenInit( void );

extern	UINT32	CreditScreenInit( void );
extern	UINT32	CreditScreenHandle( void );
extern	UINT32	CreditScreenShutdown( void );

// OJW - 20081129
extern  UINT32	MPJoinScreenInit( void );
extern  UINT32	MPJoinScreenHandle( void );
extern  UINT32	MPJoinScreenShutdown( void );

extern  UINT32	MPHostScreenInit( void );
extern  UINT32	MPHostScreenHandle( void );
extern  UINT32	MPHostScreenShutdown( void );

// OJW - 20081222
extern  UINT32	MPScoreScreenInit( void );
extern  UINT32	MPScoreScreenHandle( void );
extern  UINT32	MPScoreScreenShutdown( void );

// OJW - 20090314
extern  UINT32	MPChatScreenInit( void );
extern  UINT32	MPChatScreenHandle( void );
extern  UINT32	MPChatScreenShutdown( void );

// OJW - 20090422
extern  UINT32	MPConnectScreenInit( void );
extern  UINT32	MPConnectScreenHandle( void );
extern  UINT32	MPConnectScreenShutdown( void );

// External functions
void DisplayFrameRate( );

void HandleTitleScreenAnimation();

//External Globals
extern CHAR8					gubFilename[ 200 ];
extern UINT32				guiCurrentScreen;
extern UINT32				guiPendingScreen;

typedef void (*RENDER_HOOK)( void );	

void SetRenderHook( RENDER_HOOK pRenderOverride );
void SetCurrentScreen( UINT32 uiNewScreen );
void SetDebugRenderHook( RENDER_HOOK pDebugRenderOverride, INT8 ubPage );

void DisableFPSOverlay( BOOLEAN fEnable );

void EnterTacticalScreen( );
void LeaveTacticalScreen( UINT32 uiNewScreen );

//////////////////////////////////////////////////////////////////////////

struct SExceptionData
{
	utf8string		message,function,file;
	int				line;
};

extern std::list<SExceptionData> g_ExceptionList;

void PrintExceptionList();


#endif