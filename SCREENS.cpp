#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "Screens.h"
#endif

int iResolution;		// INI file
/* WANNE, Sgt.Kolja
 * INI file (Windowed or Fullscreen) 
 * REPLACE all defines WINDOWED_MODE with this variable
 * this is the LAST occurence of WINDOWED_MODE !
 * If (and only if) optionally set, we got a preset of
 * Windowed Screen. But this can be overloaded by INI or CmdLine
 */
#ifdef WINDOWED_MODE 
int iScreenMode = 1;	/* 1 for Windowed, 0 for Fullscreen */
#else
int iScreenMode = 0;	/* 0 for Fullscreen */
#endif
BOOL bScreenModeCmdLine = FALSE;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

int iScreenWidthOffset;
int iScreenHeightOffset;

BOOLEAN fDisplayOverheadMap;

Screens GameScreens[MAX_SCREENS] =
{
  { EditScreenInit,							EditScreenHandle,							EditScreenShutdown					}, 
  { SavingScreenInitialize,     SavingScreenHandle,						SavingScreenShutdown				}, // Title Screen
  { LoadingScreenInitialize,    LoadingScreenHandle,					LoadingScreenShutdown				}, // Title Screen
  { ErrorScreenInitialize,      ErrorScreenHandle,						ErrorScreenShutdown					}, // Title Screen
  { InitScreenInitialize,       InitScreenHandle,							InitScreenShutdown					}, // Title Screen
  { MainGameScreenInit,					MainGameScreenHandle,					MainGameScreenShutdown			},
	{ AniEditScreenInit,					AniEditScreenHandle,					AniEditScreenShutdown				},
	{ PalEditScreenInit,					PalEditScreenHandle,					PalEditScreenShutdown				},
	{ DebugScreenInit,						DebugScreenHandle,						DebugScreenShutdown					},
	{ MapScreenInit,							MapScreenHandle,							MapScreenShutdown						},
	{ LaptopScreenInit,						LaptopScreenHandle,						LaptopScreenShutdown				},
	{ LoadSaveScreenInit,					LoadSaveScreenHandle,					LoadSaveScreenShutdown			},
	{ MapUtilScreenInit,					MapUtilScreenHandle,					MapUtilScreenShutdown				},
	{ FadeScreenInit,							FadeScreenHandle,							FadeScreenShutdown					},
	{ MessageBoxScreenInit,				MessageBoxScreenHandle,				MessageBoxScreenShutdown		},
	{ MainMenuScreenInit,					MainMenuScreenHandle,					MainMenuScreenShutdown			},
	{ AutoResolveScreenInit,			AutoResolveScreenHandle,			AutoResolveScreenShutdown		},
	{ SaveLoadScreenInit,					SaveLoadScreenHandle,					SaveLoadScreenShutdown			},
	{ OptionsScreenInit,					OptionsScreenHandle,					OptionsScreenShutdown				},
	{ ShopKeeperScreenInit,				ShopKeeperScreenHandle,				ShopKeeperScreenShutdown		},
	{ SexScreenInit,							SexScreenHandle,							SexScreenShutdown						},
	{ GameInitOptionsScreenInit,	GameInitOptionsScreenHandle,	GameInitOptionsScreenShutdown		},
	{ DemoExitScreenInit,					DemoExitScreenHandle,					DemoExitScreenShutdown			},
	{ IntroScreenInit,						IntroScreenHandle,						IntroScreenShutdown					},
	{ CreditScreenInit,						CreditScreenHandle,						CreditScreenShutdown				},


#ifdef JA2BETAVERSION
	{ AIViewerScreenInit,					AIViewerScreenHandle,			AIViewerScreenShutdown			},
#endif

	{ QuestDebugScreenInit,				QuestDebugScreenHandle,		QuestDebugScreenShutdown		}
};