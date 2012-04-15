/* $Id: sgp.c,v 1.4 2004/03/19 06:16:04 digicrab Exp $ */
//its test what doeas it do?
#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
	#include "JA2 Splash.h"
	#include "utilities.h"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "types.h"
	#include <windows.h>
	#include <windowsx.h>
	#include <stdio.h>
	#include <stdarg.h>
	#include <string.h>
	#include "sgp.h"
	#include "RegInst.h"
	#include "vobject.h"
	#include "font.h"
	#include "local.h"
	#include "Fileman.h"
	#include "input.h"
	#include "Random.h"
	#include "gameloop.h"
	#include "soundman.h"
	#ifdef JA2
		#include "JA2 Splash.h"
		#include "Timer Control.h"
	#endif
	#if !defined( JA2 ) && !defined( UTIL )
		#include "GameData.h"				// for MoveTimer() [Wizardry specific]
	#endif
	#include "LibraryDataBase.h"
	#include "utilities.h"
#endif

#include "GameSettings.h"
#include "input.h"
#include "zmouse.h"

#include <vfs/Aspects/vfs_settings.h>
#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_init.h>
#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Core/File/vfs_file.h>
#include <vfs/Tools/vfs_log.h>
#include <vfs/Tools/vfs_parser_tools.h>
#include <vfs/Tools/vfs_file_logger.h>

#include "sgp_logger.h"
#include "Text.h"
#include "LocalizedStrings.h"
#include "ExportStrings.h"
#include "ImportStrings.h"

#define USE_CONSOLE 0

#include <iostream>

#include <excpt.h>
#include "ExceptionHandling.h"

#include "dbt.h"
#include "INIReader.h"
#include "Console.h"
#include "Lua Interpreter.h"
#include "connect.h"
#include "english.h"

#ifdef JA2
	#include "BuildDefines.h"
	#include "Intro.h"
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif


static void MAGIC(std::string const& aarrrrgggh = "")
{}

static bool			s_VfsIsInitialized = false;
static std::list<vfs::Path> vfs_config_ini;

static bool			s_DebugKeyboardInput = false;
static vfs::Path	s_CodePage;

static vfs::FileLogger *vfslog = NULL;

int		iWindowedMode;

void SHOWEXCEPTION(sgp::Exception& ex)
{
	try {
		_ExceptionMessage(ex);
	}
	catch(sgp::Exception &ex2) {
		SGP_ERROR(ex2.what());
		exit(0);
	}
}

void SHOWEXCEPTION(vfs::Exception& ex)
{
	try {
		_ExceptionMessage(ex);
	}
	catch(vfs::Exception &ex2) {
		SGP_ERROR(ex2.what());
		exit(0);
	}
}

#define HANDLE_FATAL_ERROR \
	catch(sgp::Exception &ex){ \
		SGP_ERROR(ex.what()); \
		FatalError((const STR8)ex.what()); \
		exit(0); } \
	catch(vfs::Exception &ex){ \
		SGP_ERROR(ex.what()); \
		FatalError((const STR8)ex.getExceptionString().utf8().c_str()); \
		exit(0); } \
	catch(std::exception &ex){ \
		SGP_ERROR(ex.what()); \
		FatalError((const STR8)ex.what()); \
		exit(0); } \
	catch(const char* msg){ \
		SGP_ERROR(msg); \
		FatalError((const STR8)msg); \
		exit(0); } \
	catch(...){ \
		SGP_ERROR("Caught undefined exception"); \
		FatalError("Caught undefined exception"); \
		exit(0); }


extern UINT32		MemDebugCounter;
#ifdef JA2
	extern BOOLEAN	gfPauseDueToPlayerGamePause;
	extern int		iScreenMode;
	extern BOOL		bScreenModeCmdLine;
#endif

extern	BOOLEAN		CheckIfGameCdromIsInCDromDrive();
extern	void		QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam);

// Prototype Declarations
INT32 FAR PASCAL	SyncWindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam);
INT32 FAR PASCAL	WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam);
BOOLEAN				InitializeStandardGamingPlatform(HINSTANCE hInstance, int sCommandShow);
void				ShutdownStandardGamingPlatform(void);
void				CreateStandardGamingPlatform(HWND hWindow);
void				GetRuntimeSettings( );
void				SafeSGPExit(void);

static bool			CallGameLoop(bool wait);
static CRITICAL_SECTION gcsGameLoop;


int PASCAL HandledWinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow);

#if USE_CONSOLE
Console g_Console("", "", "Lua Console", "no");
#endif

#if !defined(JA2) && !defined(UTILS)
void				ProcessCommandLine(CHAR8 *pCommandLine);
BOOLEAN				RunSetup(void);

// Should the game immediately load the quick save at startup?
BOOLEAN				gfLoadAtStartup=FALSE;
BOOLEAN				gfUsingBoundsChecker=FALSE;
CHAR8				*gzStringDataOverride=NULL;
BOOLEAN				gfCapturingVideo = FALSE;

#endif

#ifdef USE_VFS
static void PopulateSectionFromCommandLine(vfs::PropertyContainer &oProps, vfs::String const& sSection);
#endif

HINSTANCE			ghInstance;


#ifdef JA2
	void ProcessJa2CommandLineBeforeInitialization(CHAR8 *pCommandLine);
#endif

// Global Variable Declarations
RECT				rcWindow;
POINT				ptWindowSize;

// moved from header file: 24mar98:HJH
UINT32				giStartMem;
//UINT8				gbPixelDepth;		// redefintion... look down a few lines (jonathanl)
// GLOBAL RUN-TIME SETTINGS

UINT32				guiMouseWheelMsg;			// For mouse wheel messages

BOOLEAN				gfApplicationActive;
BOOLEAN				gfProgramIsRunning;
BOOLEAN				gfGameInitialized = FALSE;
//UINT32			giStartMem; // redefintion... look up a few lines (jonathanl)
BOOLEAN				gfDontUseDDBlits	= FALSE;

// There were TWO of them??!?! -- DB
//CHAR8				gzCommandLine[ 100 ];
CHAR8				gzCommandLine[100];		// Command line given

CHAR8				gzErrorMsg[2048]="";
BOOLEAN				gfIgnoreMessages=FALSE;

// GLOBAL VARIBLE, SET TO DEFAULT BUT CAN BE CHANGED BY THE GAME IF INIT FILE READ
UINT8				gbPixelDepth = PIXEL_DEPTH;

INT32 FAR PASCAL SyncWindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam)
{
	INT32 retval;
	EnterCriticalSection(&gcsGameLoop);
	retval = WindowProcedure(hWindow, Message, wParam, lParam);
	LeaveCriticalSection(&gcsGameLoop);
	return retval;
}

bool				s_bExportStrings		= false;
extern bool			g_bUseXML_Strings;//	= false;
bool				g_bUseXML_Structures	= false;
bool				g_bUseXML_Tilesets		= false;

#ifdef USE_VFS
static vfs::Path	sp_force_load_jsd_xml_file;
#endif

INT32 FAR PASCAL WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam)
{
	static BOOLEAN fRestore = FALSE;

	if ( Message == WM_USER )
	{
		FreeConsole();
		return 0L;
	}
	BOOL visible = IsWindowVisible(hWindow);
	
	if(gfIgnoreMessages)
		return(DefWindowProc(hWindow, Message, wParam, lParam));

	// ATE: This is for older win95 or NT 3.51 to get MOUSE_WHEEL Messages
	//if ( Message == guiMouseWheelMsg )
	//{
	//	QueueEvent(MOUSE_WHEEL, wParam, lParam);
	//	return( 0L );
	//}



 
	switch(Message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
/*dnl kick this out, because in input.sgp MouseHandler() hook has priority so it will process same event twice, someone force MouseHandler() hook to always return unhandled events status so what ever mouse event you process in WindowProcedure() be aware that this event is already occur in MouseHandler() (mouse clicks, move etc.) Probably this is done because when you lost focus even if you click back on window region this will not restore them, so need condition in MouseHandler to restore window focus
//		case WM_MOUSEWHEEL:
//			{
//				QueueEvent(MOUSE_WHEEL, wParam, lParam);
//				break;
//			}
*/		
#ifdef JA2
	case WM_MOVE:
		// if( 1==iScreenMode )
		{
			GetClientRect(hWindow, &rcWindow);
			// Go ahead and clamp the client width and height
			rcWindow.right = SCREEN_WIDTH;
			rcWindow.bottom = SCREEN_HEIGHT;
			ClientToScreen(hWindow, (LPPOINT)&rcWindow);
			ClientToScreen(hWindow, (LPPOINT)&rcWindow+1);
			int xPos = (int)(short) LOWORD(lParam); 
			int yPos = (int)(short) HIWORD(lParam);
			BOOL needchange = FALSE;
			if (xPos < 0)
			{
				xPos = 0;
				needchange = TRUE;
			}
			if (yPos < 0)
			{
				yPos = 0;
				needchange = TRUE;
			}
			if (needchange)
			{
				SetWindowPos( hWindow, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
			}

		}
		break;
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO *mmi = (MINMAXINFO*)lParam;

			mmi->ptMaxSize = ptWindowSize;
			mmi->ptMaxTrackSize = mmi->ptMaxSize;
			mmi->ptMinTrackSize = mmi->ptMaxSize;
			break;
		}
#else
	case WM_MOUSEMOVE:
		break;

	case WM_SIZING:
		{
			LPRECT		lpWindow;
			INT32		iWidth, iHeight, iX, iY;
			BOOLEAN		fWidthByHeight=FALSE, fHoldRight=FALSE;

			lpWindow	= (LPRECT) lParam;

			iWidth		= lpWindow->right-lpWindow->left;
			iHeight		= lpWindow->bottom-lpWindow->top;
			iX			= (lpWindow->left + lpWindow->right)/2;
			iY			= (lpWindow->top + lpWindow->bottom)/2;

			switch(wParam)
			{
				case WMSZ_BOTTOMLEFT:
					fHoldRight=TRUE;
				case WMSZ_BOTTOM:
				case WMSZ_BOTTOMRIGHT:
					if(iHeight < SCREEN_HEIGHT)
					{
						lpWindow->bottom=lpWindow->top+SCREEN_HEIGHT;
						iHeight=SCREEN_HEIGHT;
					}
					fWidthByHeight=TRUE;
				break;

				case WMSZ_TOPLEFT:
					fHoldRight=TRUE;
				case WMSZ_TOP:
				case WMSZ_TOPRIGHT:
					if(iHeight < SCREEN_HEIGHT)
					{
						lpWindow->top=lpWindow->bottom-SCREEN_HEIGHT;
						iHeight=SCREEN_HEIGHT;
					}
					fWidthByHeight=TRUE;
					break;

				case WMSZ_LEFT:
					if(iWidth < SCREEN_WIDTH)
					{
						lpWindow->left=lpWindow->right-SCREEN_WIDTH;
						iWidth = SCREEN_WIDTH;
					}
					break;

				case WMSZ_RIGHT:
					if(iWidth < SCREEN_WIDTH)
					{
						lpWindow->right=lpWindow->left+SCREEN_WIDTH;
						iWidth = SCREEN_WIDTH;
					}
			}

			// Calculate width as a factor of height
			if(fWidthByHeight)
			{
				iWidth = iHeight * SCREEN_WIDTH / SCREEN_HEIGHT;
//				lpWindow->left = iX - iWidth/2;
//				lpWindow->right = iX + iWidth / 2;
				if(fHoldRight)
					lpWindow->left = lpWindow->right - iWidth;
				else
					lpWindow->right = lpWindow->left + iWidth;
			}
			else // Calculate height as a factor of width
			{
				iHeight = iWidth * SCREEN_HEIGHT / SCREEN_WIDTH;
//				lpWindow->top = iY - iHeight/2;
//				lpWindow->bottom = iY + iHeight/2;
				lpWindow->bottom = lpWindow->top + iHeight;
			}
	
/*
			switch(wParam)
			{
				case WMSZ_BOTTOM:
				case WMSZ_BOTTOMLEFT:
				case WMSZ_BOTTOMRIGHT:
					if(iHeight < SCREEN_HEIGHT)
					{
						lpWindow->bottom=lpWindow->top+SCREEN_HEIGHT;
					}
			}

			switch(wParam)
			{
				case WMSZ_TOP:
				case WMSZ_TOPLEFT:
				case WMSZ_TOPRIGHT:
					if(iHeight < SCREEN_HEIGHT)
					{
						lpWindow->top=lpWindow->bottom-SCREEN_HEIGHT;
					}
			}

			switch(wParam)
			{
				case WMSZ_BOTTOMLEFT:
				case WMSZ_LEFT:
				case WMSZ_TOPLEFT:
					if(iWidth < SCREEN_WIDTH)
					{
						lpWindow->left=lpWindow->right-SCREEN_WIDTH;
					}
			}

			switch(wParam)
			{
				case WMSZ_BOTTOMRIGHT:
				case WMSZ_RIGHT:
				case WMSZ_TOPRIGHT:
					if(iWidth < SCREEN_WIDTH)
					{
						lpWindow->right=lpWindow->left+SCREEN_WIDTH;
					}
			}
*/
		}
		break;

	case WM_SIZE:
		{
			UINT16 nWidth = LOWORD(lParam);	// width of client area 
			UINT16 nHeight = HIWORD(lParam); // height of client area 

			if(nWidth && nHeight)
			{
				switch(wParam)
				{
					case SIZE_MAXIMIZED:
						VideoFullScreen(TRUE);
						break;

					case SIZE_RESTORED:
						VideoResizeWindow();
						break;
				}
			}
		}
		break;

	case WM_MOVE:
		{
			INT32 xPos = (INT32)LOWORD(lParam);	// horizontal position
			INT32 yPos = (INT32)HIWORD(lParam);	// vertical position
		}
		break;
#endif
	case WM_SETCURSOR:
		SetCursor( NULL);
		return TRUE;

	case WM_TIMER:
#ifdef LUACONSOLE
		PollConsole( );
#endif
		if (gfApplicationActive)
		{
			GameLoop();		
		} 
		break;

	case WM_ACTIVATEAPP: 
		switch(wParam)
		{
		case TRUE: // We are restarting DirectDraw
			if (fRestore == TRUE)
			{
#ifdef JA2
				RestoreVideoManager();
				RestoreVideoSurfaces();	// Restore any video surfaces

				// unpause the JA2 Global clock
				if ( !gfPauseDueToPlayerGamePause )
				{
					PauseTime( FALSE );
				}
#else
				if(!VideoInspectorIsEnabled())
				{
					RestoreVideoManager();
					RestoreVideoSurfaces();	// Restore any video surfaces
				}

				MoveTimer(TIMER_RESUME);
#endif
				gfApplicationActive = TRUE;
			}
			break;
		case FALSE: // We are suspending direct draw
			if (iScreenMode == 0)
			{
#ifdef JA2
				// pause the JA2 Global clock
				//PauseTime( TRUE );
				SuspendVideoManager();
#else
#ifndef UTIL 
				if(!VideoInspectorIsEnabled())
				{
					SuspendVideoManager();
				}
#endif
#endif
				// suspend movement timer, to prevent timer crash if delay becomes long
				// * it doesn't matter whether the 3-D engine is actually running or not, or if it's even been initialized
				// * restore is automatic, no need to do anything on reactivation
#if !defined( JA2 ) && !defined( UTIL )
				MoveTimer(TIMER_SUSPEND);
#endif
				// gfApplicationActive = FALSE;
				fRestore = TRUE;
			}
			break;
		}
		break;

	case WM_CREATE:
		CreateStandardGamingPlatform(hWindow);
		break;

	case WM_DESTROY: 
		ShutdownStandardGamingPlatform();
//		ShowCursor(TRUE);
		PostQuitMessage(0);
		break;

	case WM_SETFOCUS:
		//if (iScreenMode == 0)
		{
#if !defined( JA2 ) && !defined( UTIL )
			if(!VideoInspectorIsEnabled())
			{
				RestoreVideoManager();
			}
			gfApplicationActive=TRUE;
//			RestrictMouseToXYXY(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
#else
			RestoreCursorClipRect( );
#endif
		}
		break;

	case WM_KILLFOCUS:
		if (iScreenMode == 0)
		{
#if !defined( JA2 ) && !defined( UTIL )
			if(!VideoInspectorIsEnabled())
			{
				SuspendVideoManager();
			}
			gfApplicationActive=FALSE;
			FreeMouseCursor();
#endif
			// Set a flag to restore surfaces once a WM_ACTIVEATEAPP is received
			fRestore = TRUE;
		}
		break;

#if defined( JA2 )
	case	WM_DEVICECHANGE:
		{
			//DEV_BROADCAST_HDR	*pHeader = (DEV_BROADCAST_HDR	*)lParam;

			////if a device has been removed
			//if( wParam == DBT_DEVICEREMOVECOMPLETE )
			//{
			//	//if its	a disk
			//	if( pHeader->dbch_devicetype == DBT_DEVTYP_VOLUME )
			//	{
			//		//check to see if the play cd is still in the cdrom
			//		if( !CheckIfGameCdromIsInCDromDrive() )
			//		{
			//		}
			//	}
			//}
		}
		break;
#endif

	case WM_SYSKEYUP:
	case WM_KEYUP:
		KeyUp(wParam, lParam);
		break;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
#ifdef USE_CODE_PAGE
			if(s_DebugKeyboardInput)
			{
				static vfs::Log& debugKeys = *vfs::Log::Create(L"DebugKeys.txt");
				static int input_counter = 1;
				debugKeys << (input_counter++) << " : " << (int)wParam << vfs::Log::endl;
			}
#endif // USE_CODE_PAGE
			KeyDown(wParam, lParam);
			gfSGPInputReceived =	TRUE;
			break;

		case WM_CHAR:
			{
				// WANNE: We disable this for now in multiplayer, because user could enter "\" for the file transfer path
				if (!is_networked)
				{
					if (wParam == '\\' &&
						lParam && KF_ALTDOWN)
					{
#if USE_CONSOLE
						g_Console.Create(ghWindow);
						cout << "LUA console ready" << endl;
						cout << "> ";

						// Reset the pressed keys
						gfKeyState[ ALT ] = FALSE;						
						gfKeyState[ 219 ] = FALSE;	// "\"
#endif
					}
				}				
			}
			break;

		case WM_INPUTREADY:
			{
				wstring *tstr = (wstring*) lParam;
				if (EvalLua( tstr->c_str()))
				{
					tstr->erase();
				}
				else
				{
					cout << ">";
				}

				cout << "> ";
			}
			break;
	default	:
		return DefWindowProc(hWindow, Message, wParam, lParam);
	}
	return 0L;
}

BOOLEAN InitializeStandardGamingPlatform(HINSTANCE hInstance, int sCommandShow)
{
	FontTranslationTable *pFontTable;

	// now required by all (even JA2) in order to call ShutdownSGP
	atexit(SafeSGPExit);

	// First, initialize the registry keys.
	InitializeRegistryKeys( "Wizardry8", "Wizardry8key" );

	// For rendering DLLs etc.
#ifndef JA2
	AddSubdirectoryToPath("DLL");
#endif

	// Second, read in settings
	GetRuntimeSettings( );

	// Initialize the Debug Manager - success doesn't matter
	InitializeDebugManager();

	// Now start up everything else.
	RegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

	// this one needs to go ahead of all others (except Debug), for MemDebugCounter to work right...
	FastDebugMsg("Initializing Memory Manager");
	// Initialize the Memory Manager
	if (InitializeMemoryManager() == FALSE)
	{
		// We were unable to initialize the memory manager
		FastDebugMsg("FAILED : Initializing Memory Manager");
		return FALSE;
	}

#ifdef JA2
	FastDebugMsg("Initializing Mutex Manager");
	// Initialize the Dirty Rectangle Manager
	if (InitializeMutexManager() == FALSE)
	{
		// We were unable to initialize the game
		FastDebugMsg("FAILED : Initializing Mutex Manager");
		return FALSE;
	}
#endif

	FastDebugMsg("Initializing File Manager");
	// Initialize the File Manager
	if (InitializeFileManager(NULL) == FALSE)
	{
		// We were unable to initialize the file manager
		FastDebugMsg("FAILED : Initializing File Manager");
		return FALSE;
	}

	FastDebugMsg("Initializing Containers Manager");
	InitializeContainers();

	FastDebugMsg("Initializing Input Manager");
	// Initialize the Input Manager
	if (InitializeInputManager() == FALSE)
	{
		// We were unable to initialize the input manager
		FastDebugMsg("FAILED : Initializing Input Manager");
		return FALSE;
	}

	InitializeCriticalSection(&gcsGameLoop);

	FastDebugMsg("Initializing Video Manager");
	// Initialize DirectDraw (DirectX 2)
	if (InitializeVideoManager(hInstance, (UINT16) sCommandShow, (void *) WindowProcedure) == FALSE)
	{
		// We were unable to initialize the video manager
		FastDebugMsg("FAILED : Initializing Video Manager");
		return FALSE;
	}

	// Initialize Video Object Manager
	FastDebugMsg("Initializing Video Object Manager");
	if ( !InitializeVideoObjectManager( ) )
	{
		FastDebugMsg("FAILED : Initializing Video Object Manager");
		return FALSE;
	}

	// Initialize Video Surface Manager
	FastDebugMsg("Initializing Video Surface Manager");
	if ( !InitializeVideoSurfaceManager( ) )
	{ 
		FastDebugMsg("FAILED : Initializing Video Surface Manager");
		return FALSE;
	}

	//InitializeJA2TimerID();

#ifdef USE_VFS
	//vfs::Path exe_dir, exe_file;
	//os::getExecutablePath(exe_dir, exe_file);

	//// set current directory to exe's directory 
	//os::setCurrectDirectory(exe_dir);

	SGP_THROW_IFFALSE( vfs_init::initVirtualFileSystem( vfs_config_ini ), L"Initializing Virtual File System failed");


	s_VfsIsInitialized = true;

	getVFS()->getVirtualLocation(vfs::Path("Temp"),true)->setIsExclusive(true);
	getVFS()->getVirtualLocation(vfs::Path("ShadeTables"),true)->setIsExclusive(true);
	getVFS()->getVirtualLocation(vfs::Path(pMessageStrings[MSG_SAVEDIRECTORY]+3),true)->setIsExclusive(true);
	getVFS()->getVirtualLocation(vfs::Path(pMessageStrings[MSG_MPSAVEDIRECTORY]+3),true)->setIsExclusive(true);

	if(!sp_force_load_jsd_xml_file.empty())
	{
		try
		{
			std::string filename = vfs::String::as_utf8(sp_force_load_jsd_xml_file());
			STRUCTURE_FILE_REF *pStructureFileRef = LoadStructureFile((STR8)filename.c_str());
		}
		catch(std::exception &ex)
		{
			SGP_RETHROW(_BS(L"failed to load and/or process file : ") << sp_force_load_jsd_xml_file << _BS::wget, ex);
		}
	}

#ifdef USE_CODE_PAGE
	charSet::InitializeCharSets();
	
	if(!s_CodePage.empty())
	{
		try
		{
			CodePageReader cpr;
			cpr.ReadCodePage(s_CodePage);
		}
		catch(std::exception& ex)
		{
			std::wstringstream wss;
			wss << L"Could not process codepage file \"" << s_CodePage() << L"\"";
			SGP_RETHROW(wss.str().c_str(), ex);
		}
	}
#endif // USE_CODE_PAGE
#else
	// Snap: moved the following from InitJA2SplashScreen for clarity
	STRING512			CurrentDir;
	STRING512			DataDir;

	// Get Executable Directory
	GetExecutableDirectory( CurrentDir );

	// Adjust Current Dir
	sprintf( DataDir, "%s\\Data", CurrentDir );
	if ( !SetFileManCurrentDirectory( DataDir ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not find data directory, shutting down");
		return FALSE;
	}

	//Initialize the file database
	//InitializeFileDatabase( gGameLibaries, NUMBER_OF_LIBRARIES ); // ?!?! doesn't take parameters (jonathanl)
	InitializeFileDatabase();

	// Snap: Initialize the Data directory catalogue
	gDefaultDataCat.NewCat(DataDir);

	STRING512	ja2INIfile;
	strcat(ja2INIfile, "..\\");
	strcat(ja2INIfile, GAME_INI_FILE);

	// Snap: Get the custom Data directory location from GAME_INI_FILE (if any)
	// and initialize the custom catalogue
	char customDataPath[MAX_PATH];
	if ( GetPrivateProfileString( "Ja2 Settings","CUSTOM_DATA_LOCATION", "", customDataPath, MAX_PATH, ja2INIfile ) )
	{
		gCustomDataCat.NewCat(std::string(CurrentDir) + '\\' + customDataPath);
	}
#endif

	if(g_bUseXML_Strings)
	{
		if(s_bExportStrings)
		{
			Loc::ExportStrings();
		}
		Loc::ImportStrings();
	}

//#ifdef JA2
	InitJA2SplashScreen();
//#endif

	// Make sure we start up our local clock (in milliseconds)
	// We don't need to check for a return value here since so far its always TRUE
	InitializeClockManager();	// must initialize after VideoManager, 'cause it uses ghWindow

	// Create font translation table (store in temp structure)
	pFontTable = CreateEnglishTransTable( );
	if ( pFontTable == NULL )
	{
		return( FALSE );
	}

	// Initialize Font Manager
	FastDebugMsg("Initializing the Font Manager");
	// Init the manager and copy the TransTable stuff into it.
	if ( !InitializeFontManager( 8, pFontTable ) )
	{
		FastDebugMsg("FAILED : Initializing Font Manager");
		return FALSE;
	}
	// Don't need this thing anymore, so get rid of it (but don't de-alloc the contents)
	MemFree( pFontTable );

	FastDebugMsg("Initializing Sound Manager");
	// Initialize the Sound Manager (DirectSound)
#ifndef UTIL
	if (InitializeSoundManager() == FALSE)
	{
		// We were unable to initialize the sound manager
		FastDebugMsg("FAILED : Initializing Sound Manager");
		return FALSE;
	}
#endif

	FastDebugMsg("Initializing Game Manager");
	// Initialize the Game
	if (InitializeGame() == FALSE)
	{
		// We were unable to initialize the game
		FastDebugMsg("FAILED : Initializing Game Manager");
		return FALSE;
	}

	// Register mouse wheel message
	guiMouseWheelMsg = RegisterWindowMessage( MSH_MOUSEWHEEL );

	gfGameInitialized = TRUE;

	return TRUE;
}

static void TimerActivatedCallback(INT32 timer, PTR state)
{
	if (gfApplicationActive && gfProgramIsRunning)
	{
		if (CallGameLoop(false))
			YieldProcessor();
	}
}

void CreateStandardGamingPlatform(HWND hWindow)
{
	InitializeJA2Clock();

	if (!IsHiSpeedClockMode())
		SetTimer( hWindow, 0, 1, NULL);
	else
		AddTimerNotifyCallback(TimerActivatedCallback, hWindow);
}

void ShutdownStandardGamingPlatform(void)
{
#ifndef JA2
	static BOOLEAN Reenter = FALSE;

	//
	// Prevent multiple reentry into this function
	//

	if (Reenter == FALSE)
	{
		Reenter = TRUE;
	}
	else
	{
		return;
	}
#endif

	//
	// Shut down the different components of the SGP
	//
	ClearTimerNotifyCallbacks();

	// TEST
	SoundServiceStreams();

	if (gfGameInitialized)
	{
		ShutdownGame();
	}

	ShutdownButtonSystem();
	MSYS_Shutdown();

#ifndef UTIL
	ShutdownSoundManager();
#endif

	DestroyEnglishTransTable( );	// has to go before ShutdownFontManager()
	ShutdownFontManager();

	ShutdownClockManager();	// must shutdown before VideoManager, 'cause it uses ghWindow

#ifdef SGP_VIDEO_DEBUGGING
	PerformVideoInfoDumpIntoFile( "SGPVideoShutdownDump.txt", FALSE );
#endif

	ShutdownVideoSurfaceManager();
	ShutdownVideoObjectManager();
	ShutdownVideoManager();

	ShutdownInputManager();
	ShutdownContainers();
	ShutdownFileManager();
#ifdef JA2
	ShutdownMutexManager();
#endif

#ifdef EXTREME_MEMORY_DEBUGGING
	DumpMemoryInfoIntoFile( "ExtremeMemoryDump.txt", FALSE );
#endif

	ShutdownMemoryManager();	// must go last (except for Debug), for MemDebugCounter to work right...

	//
	// Make sure we unregister the last remaining debug topic before shutting
	// down the debugging layer
	UnRegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

	DeleteCriticalSection(&gcsGameLoop);

	ShutdownDebugManager();

	sgp::Logger::instance().shutdown();
	vfs::Log::flushDeleteAll();
	if(vfslog) delete vfslog;
	vfs::CVirtualFileSystem::shutdownVFS();
	vfs::ObjectAllocator::clear();
}

#ifdef USE_VFS
#include "MPJoinScreen.h"

vfs::String getGameID()
{
	static vfs::String _id;
	static bool has_id = false;
	if(!has_id)
	{
		CUniqueServerId::uniqueRandomString(_id);
		has_id = true;
	}
	return _id;
}
#endif

#include "debug_util.h"
#include <vfs/Aspects/vfs_logging.h>

class VfsLogAdapter : public vfs::Aspects::ILogger
{
public:
	VfsLogAdapter(sgp::Logger_ID ID, bool stacktrace = false) : _id(ID), _trace(stacktrace) {};

	virtual void Msg(const wchar_t* msg)
	{
		SGP_LOG(_id, msg);
		if(_trace)
		{
			sgp::dumpStackTrace(msg);
		}
	}
	virtual void Msg(const char* msg)
	{
		SGP_LOG(_id, msg);
		if(_trace)
		{
			sgp::dumpStackTrace(msg);
		}
	}
private:
	sgp::Logger_ID	_id;
	bool			_trace;
};

//#include <vfs/Aspects/vfs_synchronization.h>
//#include "sgp_mutex.h"
//class VfsMutex : public vfs::Aspects::IMutex
//{
//public:
//	virtual void lock(){
//		_mutex.lock();
//	}
//	virtual void unlock(){
//		_mutex.unlock();
//	}
//private:
//	sgp::Mutex _mutex;
//};
//class VfsMutexFactory : public vfs::Aspects::IMutexFactory
//{
//public:
//	virtual vfs::Aspects::IMutex* createMutex()
//	{
//		return new VfsMutex();
//	}
//};

int PASCAL WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow)
{
#ifdef _DEBUG
	// Use this one ONLY if you're having memory corruption issues that can be repeated in a short time
	// Otherwise it will just run out of memory.

	/****************************************************************************************************/
	/*                                                                                                  */
	/*               DEBUG MEMORY ALLOCATION ON THE HEAP :  uncomment when required                     */
	/*          ------------------------------------------------------------------------                */
	/*                                                                                                  */
	/*  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_1024_DF);    */
	/*  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG );                                            */
	/*                                                                                                  */
	/****************************************************************************************************/
#endif

//If we are to use exception handling
#ifdef ENABLE_EXCEPTION_HANDLING
	int Result = -1;

	__try
	{
		Result = HandledWinMain(hInstance, hPrevInstance, pCommandLine, sCommandShow);
	}
	__except( RecordExceptionInfo( GetExceptionInformation() ))
	{
		// Do nothing here - RecordExceptionInfo() has already done
		// everything that is needed. Actually this code won't even
		// get called unless you return EXCEPTION_EXECUTE_HANDLER from
		// the __except clause.


	}
	return Result;

}

//Do not place code in between WinMain and Handled WinMain



int PASCAL HandledWinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow)
{
//DO NOT REMOVE, used for exception handing list above in WinMain
#endif
	MSG				Message;
	HWND			hPrevInstanceWindow;
	UINT32			uiTimer = 0;



#ifdef USE_VFS
	vfs::Log::setSharedString( getGameID() );
#endif
	//if(!vfs::Aspects::getMutexFactory())
	//{
	//	vfs::Aspects::setMutexFactory( new VfsMutexFactory() );
	//}
	sgp::Logger_ID VFS_LOG = sgp::Logger::instance().createLogger();

	sgp::Logger::instance().connectFile(VFS_LOG, L"vfs.log", false, sgp::Logger::FLUSH_ON_DELETE);

	VfsLogAdapter* vfslog = new VfsLogAdapter(VFS_LOG, false);
	VfsLogAdapter* vfslog_error = new VfsLogAdapter(VFS_LOG, true);

	vfs::Aspects::setLogger(vfslog, vfslog, vfslog_error, NULL /* vfslog */);

	// Make sure that only one instance of this application is running at once
	// // Look for prev instance by searching for the window
	hPrevInstanceWindow = FindWindowEx( NULL, NULL, APPLICATION_NAME, APPLICATION_NAME );

	// One is found, bring it up!
	if ( hPrevInstanceWindow != NULL )
	{
		SetForegroundWindow( hPrevInstanceWindow );
		ShowWindow( hPrevInstanceWindow, SW_RESTORE );
		return( 0 );
	}

	FastDebugMsg("Initializing Random");
	// Initialize random number generator
	InitializeRandom(); // no Shutdown

	//rain
	//NSLoadSettings();
	//NSSaveSettings();
	//InitResolution();

	//EmergencyExitButtonInit();
	//end rain

#ifdef _DEBUG
	// Use this one ONLY if you're having memory corruption issues that can be repeated in a short time
	// Otherwise it will just run out of memory.
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	
	/****************************************************************************************************/
	/*                                                                                                  */
	/*               DEBUG MEMORY ALLOCATION ON THE HEAP :  uncomment when required                     */
	/*          ------------------------------------------------------------------------                */
	/*                                                                                                  */
	/*.._CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_1024_DF);    */
	/*                                                                                                  */
	/****************************************************************************************************/

#endif

	ghInstance = hInstance;

	// Copy commandline!
#ifdef JA2
	strncpy( gzCommandLine, pCommandLine, 100);
	gzCommandLine[99]='\0';

	//Process the command line BEFORE initialization
	ProcessJa2CommandLineBeforeInitialization( pCommandLine );
#else
	ProcessCommandLine(pCommandLine);
#endif

	// Mem Usage
	giStartMem = MemGetFree(	) / 1024;
	

#ifdef JA2
	// Handle Check for CD
	if ( !HandleJA2CDCheck( ) )
	{
		return( 0 );
	}
#else

	if(!RunSetup())
	{
		return(0);
	}
#endif

//	ShowCursor(FALSE);

#ifdef USE_VFS
	//vfs::Path exe_dir, exe_file;
	//os::getExecutablePath(exe_dir, exe_file);
	//os::setCurrectDirectory(exe_dir);
#else
	STRING512 sExecutableDir;
	GetExecutableDirectory( sExecutableDir );
	SetCurrentDirectory(sExecutableDir);
#endif
	try
	{
		// Inititialize the SGP
		if (InitializeStandardGamingPlatform(hInstance, sCommandShow) == FALSE)
		{
			// We failed to initialize the SGP
			return 0;
		}
	}
	HANDLE_FATAL_ERROR;

#ifdef USE_VFS
	vfs::Log::flushReleaseAll();
#endif

#ifdef LUACONSOLE
	if (1==iScreenMode)
	{
		CreateConsole();
	}
#endif

#ifdef JA2
#	ifdef ENGLISH
	try
	{
		SetIntroType( INTRO_SPLASH );
	}
	HANDLE_FATAL_ERROR;
#	endif
#endif

	gfApplicationActive = TRUE;
	gfProgramIsRunning = TRUE;

	FastDebugMsg("Running Game");

	// At this point the SGP is set up, which means all I/O, Memory, tools, etc... are available. All we need to do is 
	// attend to the gaming mechanics themselves
	Message.wParam = 0;

	try
	{
		MAGIC();
		while (gfProgramIsRunning)
		{
			if (!GetMessage(&Message, NULL, 0, 0))
			{
				// It's quitting time
				return Message.wParam;
			}
			// Ok, now that we have the message, let's handle it
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	catch(sgp::Exception &ex)
	{
		SGP_ERROR(ex.what());
		SHOWEXCEPTION(ex);
	}
	catch(vfs::Exception &ex)
	{
		SGP_ERROR(ex.what());
		SHOWEXCEPTION(ex);
	}
	catch(std::exception &ex)
	{
		sgp::Exception nex(ex.what());
		SGP_ERROR(nex.what());
		SHOWEXCEPTION(nex);
	}
	catch(const char* msg)
	{
		sgp::Exception ex(msg);
		SGP_ERROR(ex.what());
		SHOWEXCEPTION(ex);
	}
	catch(...)
	{
		sgp::Exception ex("Caught undefined exception");
		SGP_ERROR( ex.what() );
		SHOWEXCEPTION(ex);
	}

#if 0
	else
	{
		// Windows hasn't processed any messages, therefore we handle the rest
#ifdef LUACONSOLE
		PollConsole( );
#endif

		if (gfApplicationActive == FALSE)
		{
			// Well we got nothing to do but to wait for a message to activate
			WaitMessage();
		}
		else
		{
			// Well, the game is active, so we handle the game stuff
			GameLoop();

			// After this frame, reset input given flag
			gfSGPInputReceived	=	FALSE;
		}
	}
	}
#endif

	// This is the normal exit point
	FastDebugMsg("Exiting Game");
	PostQuitMessage(0);

	// SGPExit() will be called next through the atexit() mechanism...	This way we correctly process both normal exits and
	// emergency aborts (such as those caused by a failed assertion).

	// return wParam of the last message received
	return Message.wParam;
}


void SGPExit(void)
{
	static BOOLEAN fAlreadyExiting = FALSE;
	// helps prevent heap crashes when multiple assertions occur and call us
	if ( fAlreadyExiting )
	{
		return;
	}

	fAlreadyExiting = TRUE;
	gfProgramIsRunning = FALSE;

// Wizardry only
#if !defined( JA2 ) && !defined( UTIL )
	if (gfGameInitialized)
	{
		// ARM: if in DEBUG mode & we've ShutdownWithErrorBox, don't unload screens and release data structs to permit easier debugging
#ifdef _DEBUG
		if (gfIgnoreMessages)
		{
			fUnloadScreens = FALSE;
		}
#endif
		GameloopExit(fUnloadScreens);
	}
#endif

	ShutdownStandardGamingPlatform();
//	ShowCursor(TRUE);
	if(strlen(gzErrorMsg))
	{
		MessageBox(NULL, gzErrorMsg, "Error", MB_OK | MB_ICONERROR	);
	}

#ifndef JA2
	VideoDumpMemoryLeaks();
#endif

}

void GetRuntimeSettings( )
{
	int		iMaximize;
	
#ifndef USE_VFS
	CHAR8	zMaximize[ 50 ];
	CHAR8	zWindowedMode[ 50 ];

	// Runtime settings - for now use INI file - later use registry
	STRING512		INIFile;		// Path to the ini file
	CHAR8			zScreenResolution[ 50 ];
	
	// Get Executable Directory
	GetExecutableDirectory( INIFile );

	strcat(INIFile, "\\");
	strcat(INIFile, GAME_INI_FILE);
#else
	vfs::PropertyContainer oProps;
	oProps.initFromIniFile(GAME_INI_FILE);
	PopulateSectionFromCommandLine(oProps, "Ja2 Settings");
#endif
	
#ifndef USE_VFS
	if (GetPrivateProfileString( "Ja2 Settings","SCREEN_RESOLUTION", "", zScreenResolution, 50, INIFile ))
	{
		iResolution = atoi(zScreenResolution);
	}
	if (GetPrivateProfileString( "Ja2 Settings","SCREEN_MODE_WINDOWED_MAXIMIZE", "", zMaximize, 50, INIFile ))
	{
		iMaximize = atoi(zMaximize);
	}	
	if (GetPrivateProfileString( "Ja2 Settings","SCREEN_MODE_WINDOWED", "", zWindowedMode, 50, INIFile ))
	{
		iWindowedMode = atoi(zWindowedMode);
	}	
#else
	vfs::String loc = oProps.getStringProperty("Ja2 Settings", L"LOCALE");
	if(!loc.empty())
	{
		SGP_THROW_IFFALSE( setlocale(LC_ALL, loc.utf8().c_str()), _BS(L"invalid locale : ") << loc << _BS::wget );
	}

	iResolution = (int)oProps.getIntProperty(L"Ja2 Settings", L"SCREEN_RESOLUTION", -1);
	
	// WANNE: Always enable
	//iMaximize = (int)oProps.getIntProperty(L"Ja2 Settings", L"SCREEN_MODE_WINDOWED_MAXIMIZE", -1);
	iMaximize = 1;
	
	iWindowedMode = (int)oProps.getIntProperty(L"Ja2 Settings", L"SCREEN_MODE_WINDOWED", -1);

	vfs::Settings::setUseUnicode( !oProps.getBoolProperty(L"Ja2 Settings", L"VFS_NO_UNICODE", false) );

	std::list<vfs::String> ini_list;

	vfs::String vfs_config_file;
	if(oProps.getStringProperty(L"Ja2 Settings", L"VFS_CONFIG", vfs_config_file))
	{
		vfs::PropertyContainer temp_cont;
		temp_cont.initFromIniFile(vfs_config_file);
		vfs::String temp_str;
		if(temp_cont.getStringProperty(L"vfs_config", L"VFS_CONFIG_INI", temp_str))
		{
			oProps.setStringProperty(L"Ja2 Settings", L"VFS_CONFIG_INI", temp_str);
		}
	}
	if(oProps.getStringListProperty(L"Ja2 Settings", L"VFS_CONFIG_INI", ini_list, L""))
	{
		vfs_config_ini.clear();
		for(std::list<vfs::String>::iterator it = ini_list.begin(); it != ini_list.end(); ++it)
		{
			vfs_config_ini.push_back(*it);
		}
	}
	else
	{
		vfs_config_ini.push_back(L"vfs_config.ini");
	}
	std::list<vfs::String> merge_list;
	if(oProps.getStringListProperty(L"Ja2 Settings", L"MERGE_INI_FILES", merge_list, L""))
	{
		for(std::list<vfs::String>::iterator it = merge_list.begin(); it != merge_list.end(); ++it)
		{
			CIniReader::RegisterFileForMerging(*it);
		}
	}
	
	std::list<vfs::String> merge_list_ub;
	if(oProps.getStringListProperty(L"Ja2 Settings", L"MERGE_INI_FILES_UB", merge_list_ub, L""))
	{
		for(std::list<vfs::String>::iterator it = merge_list_ub.begin(); it != merge_list_ub.end(); ++it)
		{
			CIniReader::RegisterFileForMerging(*it);
		}
	}
#endif

#ifdef USE_VFS
	extern bool g_bUsePngItemImages;
	g_bUsePngItemImages		= oProps.getBoolProperty(L"Ja2 Settings", L"USE_PNG_ITEM_IMAGES", false);
	g_bUseXML_Structures	= oProps.getBoolProperty(L"Ja2 Settings", L"USE_XML_STRUCTURES", false);
	
	// WANNE: Always use XML tilesets (ja2Set.dat.xml), because now we have P4-P9 items integrated. The old method (ja2set.dat) will not work anymore!
	// To generate ja2Set.dat.xml, set "USE_XML_TILESETS = 1" in ja2.ini then start the game with the official (4870) ja2 1.13 executable. 
	// Yes, you have to start a game with an older executable where p4-p9 is not integrated (see: TileDat.h -> enum TileTypeDefines)
	// Once the game reaches the main menu, the ja2Set.dat.xml file will be 
	// available in the "Profiles" folder of the MOD
	//g_bUseXML_Tilesets = true;

	// WANNE: Yes, make it optional again
	g_bUseXML_Tilesets		= oProps.getBoolProperty(L"Ja2 Settings", L"USE_XML_TILESETS", false);
	
	g_bUseXML_Strings		= oProps.getBoolProperty(L"Ja2 Settings", L"USE_XML_STRINGS", false);
	s_bExportStrings		= oProps.getBoolProperty(L"Ja2 Settings", L"EXPORT_STRINGS", false);

	sp_force_load_jsd_xml_file = oProps.getStringProperty(L"Ja2 Settings", L"FORCE_LOAD_JSD_XML_FILE", L"");
#endif

#ifdef JA2EDITOR
#ifndef USE_VFS
	if (GetPrivateProfileString( "Ja2 Settings","EDITOR_SCREEN_RESOLUTION", "", zScreenResolution, 50, INIFile ))
	{
		iResolution = atoi(zScreenResolution);
	}
#else
	iResolution = (int)oProps.getIntProperty("Ja2 Settings","EDITOR_SCREEN_RESOLUTION", -1); 
#endif
#endif

	int	iResX;
	int iResY;

	switch (iResolution)
	{
		case _640x480:
			iResX = 640;
			iResY = 480;
			break;
		case _800x600:
			iResX = 800;
			iResY = 600;
			break;
		case _1024x600:
			iResX = 1024;
			iResY = 600;
			break;
		case _1280x720:
			iResX = 1280;
			iResY = 720;
			break;
		case _1024x768:
			iResX = 1024;
			iResY = 768;
			break;
		case _1280x768:
			iResX = 1280;
			iResY = 768;
			break;
		case _1360x768:
			iResX = 1360;
			iResY = 768;
			break;
		case _1366x768:
			iResX = 1366;
			iResY = 768;
			break;
		case _1280x800:
			iResX = 1280;
			iResY = 800;
			break;
		case _1440x900:
			iResX = 1440;
			iResY = 900;
			break;
		case _1600x900:
			iResX = 1600;
			iResY = 900;
			break;
		case _1280x960:
			iResX = 1280;
			iResY = 960;
			break;
		case _1440x960:
			iResX = 1440;
			iResY = 960;
			break;
		case _1770x1000:
			iResX = 1770;
			iResY = 1000;
			break;
		case _1280x1024:
			iResX = 1280;
			iResY = 1024;
			break;
		case _1360x1024:
			iResX = 1360;
			iResY = 1024;
			break;
		case _1600x1024:
			iResX = 1600;
			iResY = 1024;
			break;
		case _1440x1050:
			iResX = 1440;
			iResY = 1050;
			break;
		case _1680x1050:
			iResX = 1680;
			iResY = 1050;
			break;
		case _1920x1080:
			iResX = 1920;
			iResY = 1080;
			break;
		case _1600x1200:
			iResX = 1600;
			iResY = 1200;
			break;
		case _1920x1200:
			iResX = 1920;
			iResY = 1200;
			break;
		case _2560x1440:
			iResX = 2560;
			iResY = 1440;
			break;
		case _2560x1600:
			iResX = 2560;
			iResY = 1600;
			break;
		default:	// 800x600
			iResolution = _800x600;
			iResX = 800;
			iResY = 600;
			break;
	}

	if (iWindowedMode == 1 && iMaximize == 1)
	{
		if ((iResX - 16) >= 1024)
			iResX = iResX - 16;

		if ((iResY - 70) >= 768)
			iResY = iResY - 70;
	}

	// Adjust again

#ifndef USE_VFS
	gbPixelDepth = GetPrivateProfileInt( "SGP", "PIXEL_DEPTH", PIXEL_DEPTH, INIFile );

	SCREEN_WIDTH = GetPrivateProfileInt( "SGP", "WIDTH", iResX, INIFile );
	SCREEN_HEIGHT = GetPrivateProfileInt( "SGP", "HEIGHT", iResY, INIFile );

	iScreenWidthOffset = (SCREEN_WIDTH - 640) / 2;
	iScreenHeightOffset = (SCREEN_HEIGHT - 480) / 2;

	/* Sergeant_Kolja. 2007-02-20: runtime Windowed mode instead of compile-time */
	/* 1 for Windowed, 0 for Fullscreen */
	if( !bScreenModeCmdLine )
	iScreenMode = (int) GetPrivateProfileInt( "Ja2 Settings","SCREEN_MODE_WINDOWED", iScreenMode, INIFile );

	// WANNE: Should we play the intro?
	iPlayIntro = (int) GetPrivateProfileInt( "Ja2 Settings","PLAY_INTRO", iPlayIntro, INIFile );
#else
	gbPixelDepth = PIXEL_DEPTH;

	SCREEN_WIDTH = iResX;
	SCREEN_HEIGHT = iResY;

	iScreenWidthOffset = (SCREEN_WIDTH - 640) / 2;
	iScreenHeightOffset = (SCREEN_HEIGHT - 480) / 2;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		xResOffset = ((SCREEN_WIDTH - 640) / 2);
		yResOffset = ((SCREEN_HEIGHT - 480) / 2);	
	}
	else if (iResolution < _1024x768)
	{
		xResOffset = ((SCREEN_WIDTH - 800) / 2);
		yResOffset = ((SCREEN_HEIGHT - 600) / 2);
	}
	else
	{
		xResOffset = ((SCREEN_WIDTH - 1024) / 2);
		yResOffset = ((SCREEN_HEIGHT - 768) / 2);
	}

	xResSize = (SCREEN_WIDTH - 2 * xResOffset);		// one of the following: 1024 or 800 or 640
	yResSize = (SCREEN_HEIGHT - 2 * yResOffset);	// one of the follownig: 768 or 600 or 480

	/* Sergeant_Kolja. 2007-02-20: runtime Windowed mode instead of compile-time */
	/* 1 for Windowed, 0 for Fullscreen */
	if( !bScreenModeCmdLine )
	{
		iScreenMode = (int)oProps.getIntProperty("Ja2 Settings","SCREEN_MODE_WINDOWED", iScreenMode);
	}

	// WANNE: Should we play the intro?
	iPlayIntro = (int)oProps.getIntProperty("Ja2 Settings","PLAY_INTRO", iPlayIntro);

#ifdef USE_CODE_PAGE
	s_DebugKeyboardInput = oProps.getBoolProperty(L"Ja2 Settings", L"DEBUG_KEYS", false);
	s_CodePage = oProps.getStringProperty(L"Ja2 Settings", L"CODE_PAGE");
#endif // USE_CODE_PAGE

	// get timer/clock initialization state
	SetHiSpeedClockMode( oProps.getBoolProperty("Ja2 Settings", "HIGHSPEED_TIMER", false) ? TRUE : FALSE );	
#endif
}


void SafeSGPExit(void)
{
	// SGPExit tends to use resources that are already uninitialized so handle 
	__try
	{
		SGPExit();
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		// The application is in exit and best effort to clean up 
		//  has failed so just ignore and continue silently
	}
}


void ShutdownWithErrorBox(CHAR8 *pcMessage)
{
	strncpy(gzErrorMsg, pcMessage, 255);
	gzErrorMsg[255]='\0';
	gfIgnoreMessages=TRUE;

	exit(0);
}

#if !defined(JA2) && !defined(UTILS)

void ProcessCommandLine(CHAR8 *pCommandLine)
{
	CHAR8 cSeparators[] = "\t =";
	CHAR8 *pCopy=NULL, *pToken;

	pCopy = (CHAR8 *)MemAlloc(strlen(pCommandLine) + 1);

	Assert(pCopy);
	if(!pCopy)
	{
		return;
	}
	memcpy(pCopy, pCommandLine, strlen(pCommandLine)+1);

	pToken=strtok(pCopy, cSeparators);
	while(pToken)
	{
		if(!_strnicmp(pToken, "/NOSOUND", 8))
		{
			SoundEnableSound(FALSE);
		}
		else if(!_strnicmp(pToken, "/INSPECTOR", 10))
		{
			VideoInspectorEnable();
		}
		else if(!_strnicmp(pToken, "/VIDEOCFG", 9))
		{
			pToken=strtok(NULL, cSeparators);
			VideoSetConfigFile(pToken);
		}
		else if(!_strnicmp(pToken, "/LOAD", 5))
		{
			gfLoadAtStartup=TRUE;
		}
		else if(!_strnicmp(pToken, "/WINDOW", 7))
		{
			VideoFullScreen(FALSE);
		}
		else if(!_strnicmp(pToken, "/BC", 7))
		{
			gfUsingBoundsChecker = TRUE;
		}
		else if(!_strnicmp(pToken, "/CAPTURE", 7))
		{
			gfCapturingVideo = TRUE;
		}
		else if(!_strnicmp(pToken, "/NOOCT", 6))
		{
			NoOct();
		}
		else if(!_strnicmp(pToken, "/STRINGDATA", 11))
		{
			pToken=strtok(NULL, cSeparators);
			gzStringDataOverride = (CHAR8 *)MemAlloc(strlen(pToken) + 1);
			strcpy(gzStringDataOverride, pToken);
		}

		pToken=strtok(NULL, cSeparators);
	}

	MemFree(pCopy);
}

BOOLEAN RunSetup(void)
{
	if(!FileExists(VideoGetConfigFile()))
	{
		_spawnl(_P_WAIT, "3DSetup.EXE", "3DSetup.EXE", VideoGetConfigFile(), NULL);
	}
	return(FileExists(VideoGetConfigFile()));
}

#endif



void ProcessJa2CommandLineBeforeInitialization(CHAR8 *pCommandLine)
{
	CHAR8 cSeparators[]="\t =";
	CHAR8	*pCopy=NULL, *pToken;

	pCopy=(CHAR8 *)MemAlloc(strlen(pCommandLine) + 1);

	Assert(pCopy);
	if(!pCopy)
		return;

	memcpy(pCopy, pCommandLine, strlen(pCommandLine)+1);

	pToken=strtok(pCopy, cSeparators);
	while(pToken)
	{
		//if its the NO SOUND option
		if(!_strnicmp(pToken, "/NOSOUND", 8))
		{
			//disable the sound
			SoundEnableSound(FALSE);
		}
		else if(!_strnicmp(pToken, "/FULLSCREEN", 11))
		{
			//overwrite Graphic setting from JA2_settings.ini
			iScreenMode=0; /* 1 for Windowed, 0 for Fullscreen */
			bScreenModeCmdLine = TRUE; /* if set TRUE, INI is no longer evaluated */
			/* no resolution read from Args. Still from INI, but could be added here, too...*/
		}
		else if(!_strnicmp(pToken, "/WINDOW", 7))
		{
			//overwrite Graphic setting from JA2_settings.ini
			iScreenMode=1; /* 1 for Windowed, 0 for Fullscreen */
			bScreenModeCmdLine = TRUE; /* if set TRUE, INI is no longer evaluated */
			/* no resolution read from Args. Still from INI, but could be added here, too...*/
		}

		//get the next token
		pToken=strtok(NULL, cSeparators);
	}

	MemFree(pCopy);
}

#ifdef USE_VFS
static void PopulateSectionFromCommandLine(vfs::PropertyContainer &oProps, vfs::String const& sSection)
{
	const wchar_t* lpCommandLine = GetCommandLineW();
	int argc = 0, nchars = 0;
	ParseCommandLine( lpCommandLine, NULL, NULL, &argc, &nchars);
	wchar_t **argv = (wchar_t **)_alloca(argc * sizeof(wchar_t *) + nchars * sizeof(wchar_t));
	ParseCommandLine( lpCommandLine, argv, (wchar_t *)(((char*)argv) + argc * sizeof(wchar_t*)), &argc, &nchars);

	for (int i = 1; i < argc; i++)
	{
		wchar_t *arg = argv[i];
		if (arg == NULL)
			continue;
		if (arg[0] == L'-' || arg[0] == L'/')
		{
			wchar_t *pkey = arg+1;
			wchar_t *psep = wcspbrk(arg, L":=");
			wchar_t *param = (psep ? psep+1 : NULL);
			if (psep) *psep = 0;
			if ( (param == NULL || param[0] == 0) && ( i+1<argc && ( argv[i+1][0] != L'-' || argv[i+1][0] != L'/' ) ) ) {
				param = argv[++i];
				argv[i] = NULL;
			}
			if (param != NULL)
			{
				oProps.setStringProperty(sSection, pkey, param);
			}
		}
	}
}
#endif

static LONG __stdcall SGPExceptionFilter(int exceptionCount, EXCEPTION_POINTERS* pExceptInfo)
{
#ifdef ENABLE_EXCEPTION_HANDLING
	extern BOOL ERGetFirstModuleException(EXCEPTION_POINTERS*, HMODULE, LPSTR, INT, LPSTR, INT, INT *);
	extern STR GetExceptionString( DWORD uiExceptionCode );
	CHAR funcName[64], sourceName[MAX_PATH];
	INT lineNum = 0;
	if (exceptionCount >= 1)
	{
		bool showAssert = true;
		__try{
			// the exception handler writer can fail with exceptions too
			RecordExceptionInfo(pExceptInfo);

			LPCSTR exceptMsg = GetExceptionString(pExceptInfo->ExceptionRecord->ExceptionCode);
			if ( ERGetFirstModuleException(pExceptInfo, NULL, funcName, _countof(funcName), sourceName, _countof(sourceName), &lineNum ) )
			{
				_FailMessage(exceptMsg, lineNum, funcName, sourceName);
				showAssert = false;
			}
		} __except (EXCEPTION_EXECUTE_HANDLER) {}
		if (showAssert) AssertMsg(FALSE, "Unhanded exception processing GameLoop unable to recover.");
	}

#endif

	return EXCEPTION_EXECUTE_HANDLER;
}

static void SGPGameLoop()
{
	try
	{
		GameLoop();
	}
	catch(sgp::Exception &ex)
	{
		SGP_ERROR(ex.what());
		SHOWEXCEPTION(ex);
	}
	catch(vfs::Exception &ex)
	{
		SGP_ERROR(ex.what());
		SHOWEXCEPTION(ex);
	}
	catch(std::exception &ex)
	{
		sgp::Exception nex(ex.what());
		SGP_ERROR(nex.what());
		SHOWEXCEPTION(nex);
	}
	catch(const char* msg)
	{
		sgp::Exception ex(msg);
		SGP_ERROR(ex.what());
		SHOWEXCEPTION(ex);
	}
}

static bool CallGameLoop(bool wait)
{
	static int numUnsuccessfulTries = 0;
	if (wait)
	{
		EnterCriticalSection(&gcsGameLoop);
	}
	else
	{
		if ( !TryEnterCriticalSection(&gcsGameLoop) )
			return false;
	}

	__try
	{
		__try
		{
			SGPGameLoop();
			numUnsuccessfulTries = 0;
		}
		__except( SGPExceptionFilter(++numUnsuccessfulTries, GetExceptionInformation()) )
		{
		}
	}
	__finally
	{
		LeaveCriticalSection(&gcsGameLoop);
	}

	// Give it several attempts to recover from random exceptions and to display error screen
	if (numUnsuccessfulTries > 5)
		ShutdownWithErrorBox("Unhandled exception. Unable to recover.");

	return true;
}

