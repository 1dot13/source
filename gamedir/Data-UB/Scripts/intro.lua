Video = {
  INTRO_NO_VIDEO	= -1,
  INTRO_REBEL_CRDT	= 0,
  INTRO_OMERTA = 1,
  INTRO_PRAGUE_CRDT = 2,
  INTRO_PRAGUE =  3,
 -- there are no more videos shown for the begining
  INTRO_END_END_SPEECH_MIGUEL = 4,
  INTRO_END_END_SPEECH_NO_MIGUEL = 5,
  INTRO_END_HELI_FLYBY = 6,
  INTRO_END_SKYRIDER_HELICOPTER = 7,
  INTRO_END_NOSKYRIDER_HELICOPTER = 8,
  INTRO_SPLASH_SCREEN = 9,
  INTRO_SPLASH_1 = 10,
  INTRO_HELI_CRASH_SCENE_1 =11,
  INTRO_END_SCENE_1 = 12,
 -- there are no more videos shown for the endgame
  INTRO_LAST_END_GAME = 13,
  INTRO_FIRST_VIDEO					= 255,
}

 IntroType = {
	INTRO_HELI_CRASH = 0,
	INTRO_BEGINNING =  1,		--  set when viewing the intro at the begining of the game
	INTRO_ENDING = 2,			-- set when viewing the end game video.
	INTRO_SPLASH = 3,
}

ScreenTypes = 
{
	EDIT_SCREEN = 0,
	SAVING_SCREEN  = 1,
	LOADING_SCREEN = 2,
	ERROR_SCREEN = 3,
	INIT_SCREEN = 4,
	GAME_SCREEN = 5,
	ANIEDIT_SCREEN = 6,
	PALEDIT_SCREEN = 7,
	DEBUG_SCREEN = 8,
	MAP_SCREEN = 9,
	LAPTOP_SCREEN = 10,
	LOADSAVE_SCREEN = 11,
	MAPUTILITY_SCREEN = 12,
	FADE_SCREEN = 13,
	MSG_BOX_SCREEN = 14,
	MAINMENU_SCREEN = 15,
	AUTORESOLVE_SCREEN = 16,
	SAVE_LOAD_SCREEN = 17,
	OPTIONS_SCREEN = 18,
	SHOPKEEPER_SCREEN = 19,
	SEX_SCREEN = 20,
	GAME_INIT_OPTIONS_SCREEN = 21,
	DEMO_EXIT_SCREEN = 22,
	INTRO_SCREEN = 23,
	CREDIT_SCREEN = 24,
}

function PrepareToExitIntroScreen()
	
-- Ja25: no begining intro
	-- if its the heli crash intro 
	if ( gbIntroScreenMode == IntroType.INTRO_HELI_CRASH ) then
-- go to the init screen
-- Ja25: no longer going to initscreen ( cause this is now AFTER mapscreen )

		SetIntroExitScreen ( ScreenTypes.GAME_SCREEN )
		SetCurrentWorldSector( sSelMapX, sSelMapY, iCurrentMapSectorZ )
		
	-- if its the intro at the begining of the game
	elseif ( gbIntroScreenMode == IntroType.INTRO_SPLASH ) then
	
		-- display a logo when exiting
		DisplaySirtechSplashScreen()

		SetDoneWithSplashScreen ( true )
		SetIntroExitScreen ( ScreenTypes.INIT_SCREEN )
		
	elseif ( gbIntroScreenMode == IntroType.INTRO_ENDING ) then
	
		SetIntroExitScreen ( ScreenTypes.GAME_SCREEN )
		SetCurrentWorldSector( iniDEFAULT_END_SECTOR_X, iniDEFAULT_END_SECTOR_Y, iniDEFAULT_END_SECTOR_Z )

		UB_EnterTacticalInFinalSector()

		-- Dont leave tactical
		SetEnteringMapScreen (false)
	else
		-- We want to reinitialize the game
		ReStartingGame()

		SetIntroExitScreen ( ScreenTypes.CREDIT_SCREEN )
	end

	SetIntroScreenExit (true)
end


function GetNextIntroVideo( uiCurrentVideo )

	-- UB Intro
	
	-- switch on whether it is the beginging or the end game video
	if ( gbIntroScreenMode == IntroType.INTRO_HELI_CRASH ) then
	
		if ( uiCurrentVideo == Video.INTRO_FIRST_VIDEO ) then
			StartVideo()
			SetVideo ( Video.INTRO_HELI_CRASH_SCENE_1 )
			StopVideo()
		end
		
	-- end game	
	elseif ( gbIntroScreenMode == IntroType.INTRO_ENDING ) then
	
		if ( uiCurrentVideo == Video.INTRO_FIRST_VIDEO ) then
			StartVideo()
			SetVideo ( Video.INTRO_END_SCENE_1 )
			StopVideo()			
		end

	elseif ( gbIntroScreenMode == IntroType.INTRO_SPLASH ) then
		
		if ( uiCurrentVideo == Video.INTRO_FIRST_VIDEO ) then
			StartVideo()
			SetVideo ( Video.INTRO_SPLASH_SCREEN )
			StopVideo()
		end	
	end
	
end

function SetIntroType( bIntroType )

-- UB Intro

	if( bIntroType == IntroType.INTRO_HELI_CRASH ) then
	
		SetIntroScreenMode ( IntroType.INTRO_HELI_CRASH )
	
	elseif( bIntroType == IntroType.INTRO_ENDING ) then
	
		SetIntroScreenMode ( IntroType.INTRO_ENDING )
	
	elseif( bIntroType == IntroType.INTRO_SPLASH ) then 
	
		SetIntroScreenMode ( IntroType.INTRO_SPLASH )
		
	end
end