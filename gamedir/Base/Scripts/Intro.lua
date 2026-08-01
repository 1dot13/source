Video = 
{
	INTRO_NO_VIDEO	= -1,
	INTRO_REBEL_CRDT = 0,
	INTRO_OMERTA = 1,
	INTRO_PRAGUE_CRDT = 2,
	INTRO_PRAGUE =  3,
	-- There are no more videos shown for the begining
	INTRO_END_END_SPEECH_MIGUEL = 4,
	INTRO_END_END_SPEECH_NO_MIGUEL = 5,
	INTRO_END_HELI_FLYBY = 6,
	INTRO_END_SKYRIDER_HELICOPTER = 7,
	INTRO_END_NOSKYRIDER_HELICOPTER = 8,
	INTRO_SPLASH_SCREEN = 9,
	INTRO_SPLASH_TALONSOFT = 10,
	-- There are no more videos shown for the endgame
	INTRO_LAST_END_GAME = 11,
	INTRO_FIRST_VIDEO = 255,
}

 IntroType = 
 {
	INTRO_BEGINNING =  0,		-- Set when viewing the intro at the begining of the game
	INTRO_ENDING = 1,			-- Set when viewing the end game video.
	INTRO_SPLASH = 2,
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

	if ( gbIntroScreenMode == IntroType.INTRO_BEGINNING ) then
		SetIntroExitScreen ( ScreenTypes.INIT_SCREEN )
	-- if its the intro at the begining of the game
	elseif ( gbIntroScreenMode == IntroType.INTRO_SPLASH ) then
		-- display a logo when exiting
		DisplaySirtechSplashScreen()

		SetDoneWithSplashScreen ( true )
		SetIntroExitScreen ( ScreenTypes.INIT_SCREEN )	
	else
		-- We want to reinitialize the game
	    ReStartingGame()

		SetIntroExitScreen ( ScreenTypes.CREDIT_SCREEN )
	end

	SetIntroScreenExit (true)
	
end

function GetNextIntroVideo( uiCurrentVideo )

	-- switch on whether it is the beginging or the end game video
	if ( gbIntroScreenMode == IntroType.INTRO_BEGINNING ) then

		-- the video at the begining of the game
		if ( uiCurrentVideo == Video.INTRO_FIRST_VIDEO ) then
			StartVideo()
			SetVideo ( Video.INTRO_REBEL_CRDT )
		elseif ( uiCurrentVideo == Video.INTRO_REBEL_CRDT ) then
			SetVideo ( Video.INTRO_OMERTA )
		elseif ( uiCurrentVideo == Video.INTRO_OMERTA ) then
			SetVideo ( Video.INTRO_PRAGUE_CRDT )		
		elseif ( uiCurrentVideo == Video.INTRO_PRAGUE_CRDT ) then
			SetVideo ( Video.INTRO_PRAGUE )	
		elseif ( uiCurrentVideo == Video.INTRO_PRAGUE ) then
			SetVideo ( Video.INTRO_NO_VIDEO )	
			StopVideo()
		end
			
	-- end game
	elseif ( gbIntroScreenMode == IntroType.INTRO_ENDING ) then
		
		if ( uiCurrentVideo == Video.INTRO_FIRST_VIDEO ) then
				StartVideo()
				-- if Miguel is dead, play the flic with out him in it
				if ( CheckMercIsDead(57) == true ) then -- MIGUEL
					SetVideo ( Video.INTRO_END_END_SPEECH_NO_MIGUEL )	
				else
					SetVideo ( Video.INTRO_END_END_SPEECH_MIGUEL )	
				end
				
				
		elseif ( uiCurrentVideo == Video.INTRO_END_END_SPEECH_MIGUEL or uiCurrentVideo == Video.INTRO_END_END_SPEECH_NO_MIGUEL ) then
			SetVideo ( Video.INTRO_END_HELI_FLYBY )	
		elseif ( uiCurrentVideo == Video.INTRO_END_HELI_FLYBY ) then
		
				-- if SkyRider is dead, play the flic without him
				if ( CheckMercIsDead(97) == true ) then --SKYRIDER
					SetVideo ( Video.INTRO_END_NOSKYRIDER_HELICOPTER )	
				else
					SetVideo ( Video.INTRO_END_SKYRIDER_HELICOPTER )	
				end
				StopVideo()	
		end	

	elseif( gbIntroScreenMode == IntroType.INTRO_SPLASH ) then
	
		if ( uiCurrentVideo == Video.INTRO_FIRST_VIDEO ) then
			StartVideo()
			SetVideo ( Video.INTRO_SPLASH_SCREEN )
		elseif ( uiCurrentVideo == Video.INTRO_SPLASH_SCREEN ) then
			SetVideo ( Video.INTRO_SPLASH_TALONSOFT )
			StopVideo()
		end
			
	end
	
end

function SetIntroType( bIntroType )

	if( bIntroType == IntroType.INTRO_BEGINNING ) then
	
		SetIntroScreenMode ( IntroType.INTRO_BEGINNING )
	
	elseif( bIntroType == IntroType.INTRO_ENDING ) then
	
		SetIntroScreenMode ( IntroType.INTRO_ENDING )
	
	elseif( bIntroType == IntroType.INTRO_SPLASH ) then 
	
		SetIntroScreenMode ( IntroType.INTRO_SPLASH )
		
	end

end