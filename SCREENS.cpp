// WANNE 2 <changed some lines>
#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "Screens.h"
#endif

int iResolution;		// INI file
// WANNE
int iScreenMode;		// INI file	(Windowed or Fullscreen) // REPLACE all defines WINDOWED_MODE with this variable

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

int iScreenWidthOffset;
int iScreenHeightOffset;

// WANNE 2
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