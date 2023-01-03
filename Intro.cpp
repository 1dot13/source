#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Intro.h"
	#include "Cinematics.h"
#else
	#include "sgp.h"
	#include "sysutil.h"
	#include "vobject_blitters.h"
	#include "font control.h"
	#include "cursor control.h"
	#include "render dirty.h"
	#include	"Text.h"
	#include	"Utilities.h"
	#include	"WordWrap.h"
	#include "text.h"
	#include "Line.h"
	#include "Intro.h"
	#include "Cinematics.h"
	#include "Cinematics Bink.h"
	#include "mainmenuscreen.h"
	#include "Music Control.h"
	#include "LibraryDataBase.h"
	#include "english.h"
	#include "soldier profile type.h"
	#include "MessageBoxScreen.h"
	#include "sgp_logger.h"
	#include "Soldier Profile.h"
	#include "Game Init.h"
	#include "INIReader.h"
#endif

#include <vfs/Core/vfs.h>

#include "Luaglobal.h"
#ifdef JA2UB
#include "strategicmap.h"
#include "Map Screen Interface Map.h"
#include "Map Screen Interface.h"
#include "End Game.h"
#include "Cinematics Bink.h"
#endif

#include "LuaInitNPCs.h"
#include "XML.h"

BOOLEAN Style_JA = TRUE;
extern INT8 Test = 0;

INTRO_NAMES_VALUES zVideoFile[255];
UINT32 iStringToUseLua = -1;

// BF
class VideoPlayer
{
public:
	enum VideoType
	{
		VT_NONE = 0,
		VT_SMK  = 1,
		VT_BINK = 2,
	};
	VideoPlayer(UINT32 type) : _type(type), _smk(NULL), _bink(NULL), _current(VT_NONE)
	{
	}
	void Initialize()
	{
		if(_type & VT_SMK)
		{
			SmkInitialize( ghWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
		if(_type & VT_BINK)
		{
			BinkInitialize(ghWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
	void Shutdown()
	{
		stopVideo(); // if one is still "running"
		if(_type & VT_SMK)
		{
			SmkShutdown();
		}
		if(_type & VT_BINK)
		{
			BinkShutdownVideo();
		}
	}

	bool isValid()
	{
		if(	(_current == VT_SMK && _smk && !_bink) || (_current == VT_BINK && !_smk && _bink) )
		{
			return true;
		}
		return false;
	}

	bool isPlaying()
	{
		if(	_current == VT_SMK && _smk && !_bink )
		{
			return SmkPollFlics() != 0;
		}
		else if(_current == VT_BINK && !_smk && _bink) 
		{
			return BinkPollFlics() != 0;
		}
		else if(_current == VT_NONE && !_smk && !_bink)
		{
			return false;
		}
		SGP_THROW(L"Invalid Video Player state : last video was not properly stopped ");
	}
	
	void stopVideo()
	{
		if(_current == VT_SMK && _smk && !_bink)
		{
			SmkCloseFlic(_smk);
			_smk = NULL;
		}
		else if(_current == VT_BINK && !_smk && _bink)
		{
			BinkCloseFlic(_bink);
			_bink = NULL;
		}
		_current = VT_NONE;
	}

	bool startVideo(std::string const& filename)
	{
		if(isPlaying())
		{
			return false;
		}

		// stop running video before starting a new one
		stopVideo();

		if(filename.empty())
		{
			return false;
		}
		
		/// check file extension
		std::string ext;
		std::string::size_type pos = filename.find_last_of(".");
		if(pos != std::string::npos)
		{
			ext = filename.substr(pos, filename.size() - pos);
		}
		if(vfs::StrCmp::Equal(ext, ".smk"))
		{
			return startSmkVideo(filename);
		}
		else if(vfs::StrCmp::Equal(ext, ".bik"))
		{
			return startBinkVideo(filename);
		}
		else if(ext.empty())
		{
			// try .bik first
			if(!startBinkVideo(filename + ".bik"))
			{
				return startSmkVideo(filename + ".smk");
			}
			return true;
		}
		else
		{
			SGP_INFO(L"Unknown video file format");
		}
		return false;
	}
private:
	bool startSmkVideo(std::string const& filename)
	{
		_smk = SmkPlayFlic( filename.c_str(), (SCREEN_WIDTH-640)/2, (SCREEN_HEIGHT-480)/2, TRUE );
		if(_smk)
		{
			_current = VT_SMK;
			return true;
		}
		return false;
	}
	bool startBinkVideo(std::string const& filename)
	{
		_bink = BinkPlayFlic( filename.c_str(), (SCREEN_WIDTH-640)/2, (SCREEN_HEIGHT-480)/2, BINK_FLIC_AUTOCLOSE | BINK_FLIC_CENTER_VERTICAL);
		if(_bink)
		{
			_current = VT_BINK;
			return true;
		}
		return false;
	}
private:
	UINT32		_type;
	SMKFLIC*	_smk;
	BINKFLIC*	_bink;
	VideoType	_current;
};

static VideoPlayer s_VP(VideoPlayer::VT_SMK | VideoPlayer::VT_BINK);

extern STR16	gzIntroScreen[];
extern HVSURFACE ghFrameBuffer;

enum
{
	INTRO_TXT__CANT_FIND_INTRO,
};

//ddd


//ggg
BOOLEAN		gfIntroScreenEntry;
BOOLEAN		gfIntroScreenExit;

UINT32		guiIntroExitScreen = INTRO_SCREEN;


extern	BOOLEAN	gfDoneWithSplashScreen;

//BF SMKFLIC *gpSmackFlic = NULL;

//enums for the various smacker files
enum EGameVideos
{
	INTRO_NO_VIDEO						= -1,
	INTRO_REBEL_CRDT					= 0,
	INTRO_OMERTA,
	INTRO_PRAGUE_CRDT,
	INTRO_PRAGUE,

	//there are no more videos shown for the begining


	INTRO_END_END_SPEECH_MIGUEL,
	INTRO_END_END_SPEECH_NO_MIGUEL,
	INTRO_END_HELI_FLYBY,
	INTRO_END_SKYRIDER_HELICOPTER,
	INTRO_END_NOSKYRIDER_HELICOPTER,

	INTRO_SPLASH_SCREEN,
	INTRO_SPLASH_TALONSOFT,

#ifdef JA2UB

	INTRO_HELI_CRASH_SCENE_1,
	INTRO_END_SCENE_1,

#endif

	//there are no more videos shown for the endgame
	INTRO_LAST_END_GAME,

	INTRO_FIRST_VIDEO					= 255
};

EGameVideos	giCurrentIntroBeingPlayed = INTRO_NO_VIDEO;
/*
CHAR		*gpzSmackerFileNames[] =
{
	//begining of the game
	"INTRO\\Rebel_cr.smk",
	"INTRO\\Omerta.smk",
	"INTRO\\Prague_cr.smk",
	"INTRO\\Prague.smk",


	//endgame
	"INTRO\\Throne_Mig.smk",
	"INTRO\\Throne_NoMig.smk",
	"INTRO\\Heli_FlyBy.smk",
	"INTRO\\Heli_Sky.smk",
	"INTRO\\Heli_NoSky.smk",

	"INTRO\\SplashScreen.smk",
	"INTRO\\TalonSoftid_endhold.smk",
};
*/
typedef std::map<EGameVideos,std::string> VideoFileNames_t;
static VideoFileNames_t s_VFN;

//enums used for when the intro screen can come up, either begining game intro, or end game cinematic
INT8	gbIntroScreenMode=-1;


extern	void		CDromEjectionErrorMessageBoxCallBack( UINT8 bExitValue );

void		GetIntroScreenUserInput();
BOOLEAN		EnterIntroScreen();
void		RenderIntroScreen();
void		ExitIntroScreen();
void		HandleIntroScreen();
void		PrepareToExitIntroScreen();
EGameVideos	GetNextIntroVideo( EGameVideos uiCurrentVideo );
void		StartPlayingIntroFlic( EGameVideos iIndexOfFlicToPlay );
void		DisplaySirtechSplashScreen();


//ppp


UINT32	IntroScreenInit( void )
{
	//Set so next time we come in, we can set up
	gfIntroScreenEntry = TRUE;

	CIniReader inireader("IntroVideos.ini");
	
	// BF: If NO_DEFAULT_VALUES is set to true, then only the explicitely used videos will be shown.
	BOOLEAN no_defaults                        = inireader.ReadBoolean("INTRO", "NO_DEFAULT_VALUES", false);

	s_VFN[INTRO_REBEL_CRDT]					= inireader.ReadString("INTRO_BEGINNING", "INTRO_REBEL_CRDT",  no_defaults ? "" : "INTRO\\Rebel_cr");
	s_VFN[INTRO_OMERTA]						= inireader.ReadString("INTRO_BEGINNING", "INTRO_OMERTA",      no_defaults ? "" : "INTRO\\Omerta");
	s_VFN[INTRO_PRAGUE_CRDT]				= inireader.ReadString("INTRO_BEGINNING", "INTRO_PRAGUE_CRDT", no_defaults ? "" : "INTRO\\Prague_cr");
	s_VFN[INTRO_PRAGUE]						= inireader.ReadString("INTRO_BEGINNING", "INTRO_PRAGUE",      no_defaults ? "" : "INTRO\\Prague");

	//there are no more videos shown for the begining
	s_VFN[INTRO_END_END_SPEECH_MIGUEL]		= inireader.ReadString("INTRO_ENDING","INTRO_END_END_SPEECH_MIGUEL",     no_defaults ? "" : "INTRO\\Throne_Mig");
	s_VFN[INTRO_END_END_SPEECH_NO_MIGUEL]	= inireader.ReadString("INTRO_ENDING","INTRO_END_END_SPEECH_NO_MIGUEL",  no_defaults ? "" : "INTRO\\Throne_NoMig");
	s_VFN[INTRO_END_HELI_FLYBY]				= inireader.ReadString("INTRO_ENDING","INTRO_END_HELI_FLYBY",            no_defaults ? "" : "INTRO\\Heli_FlyBy");
	s_VFN[INTRO_END_SKYRIDER_HELICOPTER]	= inireader.ReadString("INTRO_ENDING","INTRO_END_SKYRIDER_HELICOPTER",   no_defaults ? "" : "INTRO\\Heli_Sky");
	s_VFN[INTRO_END_NOSKYRIDER_HELICOPTER]	= inireader.ReadString("INTRO_ENDING","INTRO_END_NOSKYRIDER_HELICOPTER", no_defaults ? "" : "INTRO\\Heli_NoSky");

	s_VFN[INTRO_SPLASH_SCREEN]				= inireader.ReadString("INTRO_SPLASH","INTRO_SPLASH_SCREEN",    no_defaults ? "" : "INTRO\\SplashScreen");
	s_VFN[INTRO_SPLASH_TALONSOFT]			= inireader.ReadString("INTRO_SPLASH","INTRO_SPLASH_TALONSOFT", no_defaults ? "" : "INTRO\\TalonSoftid_endhold");

	//UB
#ifdef JA2UB
	s_VFN[INTRO_HELI_CRASH_SCENE_1]		= inireader.ReadString("INTRO_BEGINNING","INTRO_HELI_CRASH_SCENE",    no_defaults ? "" : "INTRO\\Intro");
	s_VFN[INTRO_END_SCENE_1]			= inireader.ReadString("INTRO_ENDING","INTRO_END_SCENE",    no_defaults ? "" : "INTRO\\MissileEnding");
#endif
	

	//there are no more videos shown for the endgame

	return( 1 );
}


UINT32	IntroScreenShutdown( void )
{
	return( 1 );
}


UINT32	IntroScreenHandle( void )
{
	if( gfIntroScreenEntry )
	{
		EnterIntroScreen();
		gfIntroScreenEntry = FALSE;
		gfIntroScreenExit = FALSE;

		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	//RestoreBackgroundRects();//shadooow: fixes graphical artefacts visible around end game videos


	GetIntroScreenUserInput();

	HandleIntroScreen();

	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();


	if( gfIntroScreenExit )
	{
		ExitIntroScreen();
		gfIntroScreenExit = FALSE;
		gfIntroScreenEntry = TRUE;
	}

	return( guiIntroExitScreen );
}


BOOLEAN EnterIntroScreen()
{

Test = 0;

	ClearMainMenu();

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	// Don't play music....
	SetMusicMode( MUSIC_NONE );

	// WANNE: No we have an INI option for the choise if the intro should be played
	if (!iPlayIntro && gbIntroScreenMode != INTRO_ENDING)
	{
		PrepareToExitIntroScreen();
		return( TRUE );
	}

	//initialize video player
	s_VP.Initialize();

	//get the index opf the first video to watch
	EGameVideos iFirstVideoID = GetNextIntroVideo( INTRO_FIRST_VIDEO );

	if( iFirstVideoID != -1 )
	{
		StartPlayingIntroFlic( iFirstVideoID );

		guiIntroExitScreen = INTRO_SCREEN;
	}

	//Got no intro video, exit
	else
	{
		PrepareToExitIntroScreen();
	}


	return( TRUE );
}

void RenderIntroScreen()
{
}

void ExitIntroScreen()
{
	//shutdown video player
	s_VP.Shutdown();
}

void StopIntroVideo()
{
	s_VP.stopVideo();
}

void HandleIntroScreen()
{
	//if we are exiting this screen, this frame, dont update the screen
	if( gfIntroScreenExit )
		return;

	//handle video in each frame
	BOOLEAN	fFlicStillPlaying = s_VP.isPlaying();

	//if the flic is not playing
	if( !fFlicStillPlaying )
	{
		EGameVideos iNextVideoToPlay = GetNextIntroVideo( giCurrentIntroBeingPlayed );

		if( iNextVideoToPlay != -1 )
		{
			StartPlayingIntroFlic( iNextVideoToPlay );
		}
		else
		{
			PrepareToExitIntroScreen();
			giCurrentIntroBeingPlayed = INTRO_NO_VIDEO;
		}
	}

	InvalidateScreen();
}



void GetIntroScreenUserInput()
{
	InputAtom Event;
	POINT	MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	while( DequeueEvent( &Event ) )
	{
		// HOOK INTO MOUSE HOOKS
		switch( Event.usEvent)
		{
			case LEFT_BUTTON_DOWN:
				MouseSystemHook(LEFT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_UP:
				MouseSystemHook(LEFT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y ,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_DOWN:
				MouseSystemHook(RIGHT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_UP:
				MouseSystemHook(RIGHT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_REPEAT:
				MouseSystemHook(RIGHT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_REPEAT:
				MouseSystemHook(LEFT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
		}

		if( Event.usEvent == KEY_UP )
		{
			switch( Event.usParam )
			{
				case ESC:
					s_VP.stopVideo();
					PrepareToExitIntroScreen();
					break;
				case SPACE:
					s_VP.stopVideo();
					break;

#ifdef JA2TESTVERSION

				case 'r':
					break;

				case 'i':
					InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
					break;

#endif
			}
		}
	}

	// if the user presses either mouse button
	if( gfLeftButtonState || gfRightButtonState )
	{
		//advance to the next flic
		s_VP.stopVideo();
	}
}


void PrepareToExitIntroScreen()
{
#ifdef LUA_INTRO
	LuaIntro(0, 0, 0, 0);
#else
	//if its the intro at the begining of the game
	if( gbIntroScreenMode == INTRO_BEGINNING )
	{
		//go to the init screen
		guiIntroExitScreen = INIT_SCREEN;
	}
	else if( gbIntroScreenMode == INTRO_SPLASH )
	{
		//display a logo when exiting
		DisplaySirtechSplashScreen();

		gfDoneWithSplashScreen = TRUE;
		guiIntroExitScreen = INIT_SCREEN;
	}
	else
	{
		//We want to reinitialize the game
		ReStartingGame();

//		guiIntroExitScreen = MAINMENU_SCREEN;
		guiIntroExitScreen = CREDIT_SCREEN;
	}

	gfIntroScreenExit = TRUE;
#endif
}


EGameVideos GetNextIntroVideo( EGameVideos uiCurrentVideo )
{
	EGameVideos iStringToUse = INTRO_NO_VIDEO;
#ifdef LUA_INTRO
	switch( Test )
	{
		case 0:
		LuaIntro(1, uiCurrentVideo, 0, 0);
		iStringToUse = (EGameVideos)iStringToUseLua;
		//Test = 1;
		break;
	}
#else
	//switch on whether it is the beginging or the end game video
	switch( gbIntroScreenMode )
	{
		//the video at the begining of the game
		case INTRO_BEGINNING:
		{
			switch( uiCurrentVideo )
			{
				case INTRO_FIRST_VIDEO:
					iStringToUse = INTRO_REBEL_CRDT;
					break;
				case INTRO_REBEL_CRDT:
					iStringToUse = INTRO_OMERTA;
					break;
				case INTRO_OMERTA:
					iStringToUse = INTRO_PRAGUE_CRDT;
					break;
				case INTRO_PRAGUE_CRDT:
					iStringToUse = INTRO_PRAGUE;
					break;
				case INTRO_PRAGUE:
					iStringToUse = INTRO_NO_VIDEO;
					break;
//				case INTRO_LAST_INTRO:
//					iStringToUse = -1;
//					break;
			}
			break;
		}

		//end game
		case INTRO_ENDING:
		{
			switch( uiCurrentVideo )
			{
				case INTRO_FIRST_VIDEO:
				{
					//if Miguel is dead, play the flic with out him in it
					if( gMercProfiles[ MIGUEL ].bMercStatus == MERC_IS_DEAD )
						iStringToUse = INTRO_END_END_SPEECH_NO_MIGUEL;
					else
						iStringToUse = INTRO_END_END_SPEECH_MIGUEL;
					break;
				}
				case INTRO_END_END_SPEECH_MIGUEL:
				case INTRO_END_END_SPEECH_NO_MIGUEL:
				{
					iStringToUse = INTRO_END_HELI_FLYBY;
					break;
				}
				//if SkyRider is dead, play the flic without him
				case INTRO_END_HELI_FLYBY:
				{
					if( gMercProfiles[ SKYRIDER ].bMercStatus == MERC_IS_DEAD )
						iStringToUse = INTRO_END_NOSKYRIDER_HELICOPTER;
					else
						iStringToUse = INTRO_END_SKYRIDER_HELICOPTER;
					break;
				}
			}
			break;
		}

		case INTRO_SPLASH:
			switch( uiCurrentVideo )
			{
				case INTRO_FIRST_VIDEO:
					iStringToUse = INTRO_SPLASH_SCREEN;
					break;
				case INTRO_SPLASH_SCREEN:
					//iStringToUse = SMKINTRO_SPLASH_TALONSOFT;
					break;
			}
			break;
	}

#endif
	return( iStringToUse );
}


void StartPlayingIntroFlic( EGameVideos iIndexOfFlicToPlay )
{
	if( iIndexOfFlicToPlay != -1 )
	{
		giCurrentIntroBeingPlayed = iIndexOfFlicToPlay;
		//start playing a flic
		if(s_VP.startVideo( s_VFN[iIndexOfFlicToPlay] ))
		{
			if(!s_VP.isValid())
			{
				//do a check
#ifdef JA2BETAVERSION
				PrepareToExitIntroScreen();
#else
				DoScreenIndependantMessageBox( gzIntroScreen[INTRO_TXT__CANT_FIND_INTRO], MSG_BOX_FLAG_OK, CDromEjectionErrorMessageBoxCallBack );
#endif
			}
		}
	}
}


void SetIntroType( INT8 bIntroType )
{
#ifdef LUA_INTRO
	LuaIntro(2, 0, bIntroType, 0);
#else
	if( bIntroType == INTRO_BEGINNING )
	{
		gbIntroScreenMode = INTRO_BEGINNING;
	}
	else if( bIntroType == INTRO_ENDING )
	{
		gbIntroScreenMode = INTRO_ENDING;
	}
	else if( bIntroType == INTRO_SPLASH )
	{
		gbIntroScreenMode = INTRO_SPLASH;
	}
#endif
}


void DisplaySirtechSplashScreen()
{
	// CLEAR THE FRAME BUFFER
	UINT32 uiDestPitchBYTES;
	UINT8* pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	memset(pDestBuf, 0, SCREEN_HEIGHT * uiDestPitchBYTES );
	UnLockVideoSurface( FRAME_BUFFER );

	VOBJECT_DESC VObjectDesc;
	memset( &VObjectDesc, 0, sizeof( VOBJECT_DESC ) );
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE | VOBJECT_CREATE_FROMPNG_FALLBACK;
	FilenameForBPP("INTERFACE\\SirtechSplash.sti", VObjectDesc.ImageFile);
//	FilenameForBPP("INTERFACE\\TShold.sti", VObjectDesc.ImageFile);

	UINT32 uiLogoID;
	if( !AddVideoObject(&VObjectDesc, &uiLogoID) )
	{
		FilenameForBPP("GERMAN\\SPLASH_GERMAN.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject(&VObjectDesc, &uiLogoID) )
		{
			/*
			* This is the place, where most non english coders stranding.
			* Don't hesitate, don't give up!
			* I'll now tell You what You made wrong
			*								(2006-10-10, Sergeant_Kolja)
			*/
			#ifdef _DEBUG
			#	if defined(ENGLISH)
				AssertMsg( 0, String( "Wheter English nor German works. May be You built English - but have only German or other foreign Disk?" ) );
			#	elif defined(GERMAN)
				AssertMsg( 0, String( "Weder Englisch noch Deutsch geht. Deutsche Version kompiliert und mit englischer CDs gestartet? Das geht nicht!" ) );
			#	endif
			#endif
			AssertMsg( 0, String( "Failed to load %s", VObjectDesc.ImageFile ) );
			return;
		}
	}

	HVOBJECT hPixHandle;
	GetVideoObject(&hPixHandle, uiLogoID);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,(SCREEN_WIDTH-640)/2, (SCREEN_HEIGHT-480)/2, VO_BLT_SRCTRANSPARENCY,NULL);
	DeleteVideoObjectFromIndex(uiLogoID);

	InvalidateScreen();
	RefreshScreen( NULL );
}
