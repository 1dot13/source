local NPC_TALK_RADIUS = 4
local sDesiredMercDist
local NO_PROFILE = 200

Facts = 
{ 
	FACT_SKYRIDER_CLOSE_TO_CHOPPER = 43,
	FACT_MARIA_ESCORTED_AT_LEATHER_SHOP = 117,
	FACT_JOEY_NEAR_MARTHA = 110,
	FACT_JOHN_ALIVE = 190,
	FACT_MARY_OR_JOHN_ARRIVED = 192,
	FACT_CHALICE_STOLEN = 184,
	FACT_MARY_ALIVE = 187,
	FACT_FIRST_ROBOT_DESTROYED = 203,
    FACT_ROBOT_READY_SECOND_TIME = 205,
    FACT_SECOND_ROBOT_DESTROYED = 206,
	FACT_ESTONI_REFUELLING_POSSIBLE = 277,
	FACT_KINGPIN_KNOWS_MONEY_GONE = 103,
	FACT_PLAYER_REPAID_KINGPIN = 104,
	FACT_KINGPIN_NOT_IN_OFFICE = 256,
	FACT_PLAYER_KNOWS_ABOUT_BLOODCAT_LAIR = 335,
}

Attitude = 
{
	DEFENSIVE      = 0,
	BRAVESOLO      = 1,
	BRAVEAID       = 2,
	CUNNINGSOLO    = 3,
	CUNNINGAID     = 4,
	AGGRESSIVE     = 5,
	MAXATTITUDES   = 6,
	ATTACKSLAYONLY = 7,
}

Quests = 
{
	-- Quests ID							-- BinaryData\QUESTS.EDT Record
	QUEST_DELIVER_LETTER = 0,				-- Start quest 0, End quest 1   - Deliver Letter
	QUEST_FOOD_ROUTE = 1,					-- Start quest 2, End quest 3   - Food Route
	QUEST_KILL_TERRORISTS = 2,				-- Start quest 4, End quest 5   - Terrorists
	QUEST_KINGPIN_IDOL = 3,					-- Start quest 6, End quest 7   - Kingpin Chalice
	QUEST_KINGPIN_MONEY = 4,				-- Start quest 8, End quest 9   - Kingpin Money
	QUEST_RUNAWAY_JOEY = 5,					-- Start quest 10, End quest 11 - Runaway Joey
	QUEST_RESCUE_MARIA = 6,					-- Start quest 12, End quest 13 - Rescue Maria
	QUEST_CHITZENA_IDOL = 7,				-- Start quest 14, End quest 15 - Chitzena Chalice
	QUEST_HELD_IN_ALMA = 8,					-- Start quest 16, End quest 17 - Held in Alma
	QUEST_INTERROGATION = 9,				-- Start quest 18, End quest 19 - Interogation
	QUEST_ARMY_FARM = 10,					-- Start quest 20, End quest 21 - Hillbilly Problem
	QUEST_FIND_SCIENTIST = 11,				-- Start quest 22, End quest 23 - Find Scientist
	QUEST_DELIVER_VIDEO_CAMERA = 12,		-- Start quest 24, End quest 25 - Deliver Video Camera
	QUEST_BLOODCATS = 13,					-- Start quest 26, End quest 27 - Blood Cats
	QUEST_FIND_HERMIT = 14,					-- Start quest 28, End quest 29 - Find Hermit
	QUEST_CREATURES = 15,					-- Start quest 30, End quest 31 - Creatures
	QUEST_CHOPPER_PILOT = 16,				-- Start quest 32, End quest 33 - Find Chopper Pilot
	QUEST_ESCORT_SKYRIDER = 17,				-- Start quest 34, End quest 35 - Escort SkyRider
	QUEST_FREE_DYNAMO = 18,					-- Start quest 36, End quest 37 - Free Dynamo
	QUEST_ESCORT_TOURISTS = 19,				-- Start quest 38, End quest 39 - Escort Tourists
	QUEST_FREE_CHILDREN = 20,				-- Start quest 40, End quest 41 - Doreen
	QUEST_LEATHER_SHOP_DREAM = 21,			-- Start quest 42, End quest 43 - Leather Shop Dream
	QUEST_ESCORT_SHANK = 22,				-- Start quest 44, End quest 45 - Escort Shank
	QUEST_23 = 23,							-- Start quest 46, End quest 47 - No 23 Yet
	QUEST_24 = 24,							-- Start quest 48, End quest 49 - No 24 Yet
	QUEST_KILL_DEIDRANNA = 25,				-- Start quest 50, End quest 51 - Kill Deidranna
	
	-- max Quests 254
}

Profil = 
{
	BARRY = 0,
	BLOOD = 1,
	LYNX = 2,
	GRIZZLY = 3,
	VICKI = 4,
	TREVOR = 5,
	GRUNTY = 6,
	IVAN = 7,
	STEROID = 8,
	IGOR = 9,
	SHADOW = 10,
	RED = 11,
	REAPER = 12,
	FIDEL = 13,
	FOX = 14,
	SIDNEY = 15,
	GUS = 16,
	BUNS = 17,
	ICE = 18,
	SPIDER = 19,
	CLIFF = 20,
	BULL = 21,
	HITMAN = 22,
	BUZZ = 23,
	RAIDER = 24,
	RAVEN = 25,
	STATIC = 26,
	LEN = 27,
	DANNY = 28,
	MAGIC = 29,
	STEPHEN = 30,
	SCULLY = 31,
	MALICE = 32,
	DRQ = 33,
	NAILS = 34,
	THOR = 35,
	SCOPE = 36,
	WOLF = 37,
	MD = 38,
	MELTDOWN = 39,
	BIFF = 40,
	HAYWIRE = 41,
	GASKET = 42,
	RAZOR = 43,
	FLO = 44,
	GUMPY = 45,
	LARRY = 46,
	LARRY_STONED = 47,
	COUGAR = 48,
	NUMB = 49,
	BUBBA = 50,
	MIGUEL = 57,
	CARLOS = 58,
	IRA = 59, 
	DIMITRI = 60,
	DEVIN = 61,
	ROBOT = 62,
	HAMOUS = 63,
	SLAY = 64,
	DYNAMO = 66,
	SHANK = 67,
	IGGY = 68,
	VINCE = 69,
	CONRAD = 70,
	MADDOG = 72,
	DARREL = 73,
	PERKO = 74,
	DEIDRANNA = 75,
	AUNTIE = 76,
	CARMEN = 78,
	JOE = 79,
	STEVE = 80,
	RAT = 81,
	ANNIE = 82,
	CHRIS = 83,
	BOB = 84,
	BRENDA = 85,
	KINGPIN = 86,
	DARREN = 87,
	MARIA = 88,
	ANGEL = 89,
	JOEY = 90,
	TONY = 91,
	FRANK = 92,
	SPIKE = 93,
	DAMON = 94,
	KYLE = 95,
	MICKY = 96,
	SKYRIDER = 97,
	PABLO = 98,
	SAL = 99,
	FATHER = 100,
	FATIMA = 101,
	WARDEN = 102,
	GORDON = 103,
	GABBY = 104,
	ERNEST = 105,
	FRED = 106,
	MADAME = 107,
	YANNI = 108,
	MARTHA = 109,
	JOE = 110,
	JASMIN = 111,
	CHARLIE = 112,
	JAKE = 113,
	PACOS = 114,
	DAVE = 115,
	SKIPPER = 116,
	HANS = 117,
	JOHN = 118,	
	MARY = 119,
	GENERAL = 120,
	SERGEANT = 121,
	ARMAND = 122,
	LORA = 123,
	FRANZ = 124,
	HOWARD = 125,
	SAM = 126,
	ELDIN = 127,
	ARNIE = 128,
	TINA = 129,
	FREDO = 130,
	WALTER = 131,
	JENNY = 132,
	BILLY = 133,
	BREWSTER = 134,
	ELLIOT = 135,
	DEREK = 136,
	OLIVER = 137,
	WALDO = 138,
	DOREEN = 139,
	JIM = 140,
	JACK = 141,
	OLAF = 142,
	RAY = 143,
	OLGA = 144,
	TYRONE = 145,
	MADLAB = 146,
	KEITH = 147,
	MATT = 148,
	MIKE = 149,
	DARYL = 150,
	HERVE = 151,
	PETER = 152,
	ALBERTO = 153,
	CARLO = 154,
	MANNY = 155,
	OSWALD = 156,
	CALVIN = 157,
	CARL = 158,
}

What = 
{
	MERC_TYPE__PLAYER_CHARACTER = 0,
	MERC_TYPE__AIM_MERC = 1,
	MERC_TYPE__MERC = 2,
	MERC_TYPE__NPC = 3,
	MERC_TYPE__EPC = 4,
	MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT = 5,
	MERC_TYPE__VEHICLE = 6,
}

Team = 
{
	OUR_TEAM = 0,
	ENEMY_TEAM = 1,
	CREATURE_TEAM = 2,
	MILITIA_TEAM = 3,	
	CIV_TEAM = 4,	
}

pQuest = 
{
	QUESTNOTSTARTED = 0,
	QUESTINPROGRESS = 1,
	QUESTDONE = 2,
}

Group = 
{
	NON_CIV_GROUP = 0,
	REBEL_CIV_GROUP = 1,
	KINGPIN_CIV_GROUP = 2,
	SANMONA_ARMS_GROUP = 3,
	ANGELS_GROUP = 4,
	BEGGARS_CIV_GROUP = 5,
	TOURISTS_CIV_GROUP = 6,
	ALMA_MILITARY_CIV_GROUP = 7,
	DOCTORS_CIV_GROUP = 8,
	COUPLE1_CIV_GROUP = 9,
	HICKS_CIV_GROUP = 10,
	WARDEN_CIV_GROUP = 11,
	JUNKYARD_CIV_GROUP = 12,
	FACTORY_KIDS_GROUP = 13,
	QUEENS_CIV_GROUP = 14,
}

Status = 
{
	CIV_GROUP_NEUTRAL = 0,
	CIV_GROUP_WILL_EVENTUALLY_BECOME_HOSTILE = 1,
	CIV_GROUP_WILL_BECOME_HOSTILE = 2,
	CIV_GROUP_HOSTILE = 3,
}

SectorY = 
{
	MAP_ROW_A = 1,
	MAP_ROW_B = 2,
	MAP_ROW_C = 3,
	MAP_ROW_D = 4,
	MAP_ROW_E = 5,
	MAP_ROW_F = 6,
	MAP_ROW_G = 7,
	MAP_ROW_H = 8,
	MAP_ROW_I = 9,
	MAP_ROW_J = 10,
	MAP_ROW_K = 11,
	MAP_ROW_L = 12,
	MAP_ROW_M = 13,
	MAP_ROW_N = 14,
	MAP_ROW_O = 15,
	MAP_ROW_P = 16,
}

-- text colours
FontColour =
{
	FONT_MCOLOR_DKWHITE = 134,
	FONT_MCOLOR_LTYELLOW = 144,
	FONT_MCOLOR_RED = 163,
	FONT_MCOLOR_DKRED = 164,
	FONT_MCOLOR_LTGREEN = 184,
}

-- these numbers aren't used in the code - we only use them in LUA
Languages =
{
	LANGUAGE_ENGLISH = 0,
	LANGUAGE_GERMAN = 1,
	LANGUAGE_RUSSIAN = 2,
	LANGUAGE_DUTCH = 3,
	LANGUAGE_POLISH = 4,
	LANGUAGE_FRENCH = 5,
	LANGUAGE_ITALIAN = 6,
	LANGUAGE_CHINESE = 7,
}

-- numbers for addressing which stat should get more experience
StatTypes = 
{	
	HEALTHAMT = 1,
	AGILAMT = 2,
	DEXTAMT = 3,
	WISDOMAMT = 4,
	MEDICALAMT = 5,
	EXPLODEAMT = 6,
	MECHANAMT = 7,
	MARKAMT = 8,
	EXPERAMT = 9,
	STRAMT = 10,
	LDRAMT = 11,
}

-- different teams
Teams =
{
	ENEMY_TEAM = 1,
	CREATURE_TEAM = 2,
	MILITIA_TEAM = 3,
	CIV_TEAM = 4,
}

local iLoop
local aimLoop

local HEARD_3_TURNS_AGO	= -4
local HEARD_2_TURNS_AGO	= -3
local HEARD_LAST_TURN	=  -2
local HEARD_THIS_TURN	= -1
local NOT_HEARD_OR_SEEN	= 0
local SEEN_CURRENTLY	= 1
local SEEN_THIS_TURN	= 2
local SEEN_LAST_TURN	= 3
local SEEN_2_TURNS_AGO	=4
local SEEN_3_TURNS_AGO = 5

-- local function
local function HandleJohnArrival( ID )

	local ID2 = nil
	local sDist

	if ( not ID ) then
		ID = FindSoldierByProfileID (Profil.JOHN)
		if ( not ID ) then
			return
		end
	end
	
	if ( PythSpacesAway( ID,8228) < 40 ) then
	
		if ( CheckFact( Facts.FACT_MARY_ALIVE, 0 ) == true ) then
			ID2 = FindSoldierByProfileID( Profil.MARY )
			if ( ID2 ) then
				if ( PythSpacesAway( ID, GetNPCGridNo(ID2) ) > 8 ) then
					-- Too far away!
					return
				end
			end
		end

		SetFactTrue( Facts.FACT_MARY_OR_JOHN_ARRIVED )
		ActionStopMerc(ID)

		-- If Mary is alive/dead
		if ( ID2 ) then
			ActionStopMerc(ID2)
			TriggerNPCRecord( ID, 13 )
		else
			TriggerNPCRecord( ID, 12 )
		end
		
	end
	
end
-- end local function

-- local function
local function HandleMaryArrival( ID )

	local sDist
	
	if ( not ID ) then
		ID = FindSoldierByProfileID (Profil.MARY)
		if ( not ID ) then
			return
		end
	end
	
	if ( CheckFact( Facts.FACT_JOHN_ALIVE,0) == true ) then
		return
	--new requirements: player close by
	elseif ( PythSpacesAway( ID,8228) < 40 ) then
	
		if ( not TileIsOutOfBounds ( ClosestPC( ID, sDist )) and sDist > NPC_TALK_RADIUS * 2 ) then
			--too far away
			return
		end
		
		SetFactTrue( Facts.FACT_MARY_OR_JOHN_ARRIVED )
		ActionStopMerc(ID)
		TriggerNPCRecord( ID, 13 )
	end

end
-- end local function

function HandleAtNewGridNo( ProfileId )

	if ( CheckFact ( Facts.FACT_ESTONI_REFUELLING_POSSIBLE, 0) == true and CheckQuest(Quests.QUEST_ESCORT_SHANK) == pQuest.QUESTINPROGRESS ) then
			EndQuest( Quests.QUEST_ESCORT_SHANK, gWorldSectorX, gWorldSectorY ) 	
	end	

	TeamSoldier = FindSoldierTeam (ProfileId)
	
	if ( TeamSoldier == Team.OUR_TEAM ) then -- Team
	
		-- Kingping expecting visit from player (Sector D5)
		-- The fact has to be TRUE. If FALSE then Kingpin attack the player.
		if ( CheckFact( 98, ProfileId ) == false ) then 
		   if ( NPCInRoomRange( ProfileId, 30, 39 ) == true and gWorldSectorX == 5 and gWorldSectorY == SectorY.MAP_ROW_D and gWorldSectorZ == 0 )then 
		
				for iLoop = GetTacticalStatusFirstID(Team.CIV_TEAM),GetTacticalStatusLastID(Team.CIV_TEAM) do
					if ( CheckMercPtrsInSector (iLoop) == true and CheckMercPtrsInActive(iLoop) == true and CheckMercPtrsInCivilianGroup (iLoop) == 2 ) then
						for aimLoop=GetTacticalStatusFirstID(Team.OUR_TEAM),GetTacticalStatusLastID(Team.OUR_TEAM) do
							if ( CheckMercPtrsID1SeenID2(iLoop,aimLoop) == SEEN_CURRENTLY ) then
								MakeMercPtrsHostile( iLoop )
							end
						end	
					end	
				end
 
				if ( CheckCombatMode == false ) then
					EnterTeamCombatMode(Team.CIV_TEAM)
				end	
	
			end	
		end
	
		if ( WhatKindOfMercAmI (ProfileId) == What.MERC_TYPE__EPC ) then -- what EPC
		
			-- Skyrider
			if ( ProfileId == Profil.SKYRIDER and CheckNPCSectorBool( Profil.SKYRIDER, 13, SectorY.MAP_ROW_B, 0 ) == true and PythSpacesAway( Profil.SKYRIDER,8842 ) < 11 ) then
					ActionStopMerc(Profil.SKYRIDER)
					SetFactTrue( Facts.FACT_SKYRIDER_CLOSE_TO_CHOPPER )
					TriggerNPCRecord( Profil.SKYRIDER, 15 )
					SetUpHelicopterForPlayer( 13, SectorY.MAP_ROW_B, Profil.SKYRIDER, 163 ) -- 163 helicopter
			
			-- Mary & John				
			elseif ( ( CheckNPCSectorBool( Profil.MARY, 13, SectorY.MAP_ROW_B, 0 ) == true ) or ( CheckNPCSectorBool( Profil.JOHN, 13, SectorY.MAP_ROW_B, 0 ) == true ) ) then
					
					-- Mary	
					if ( ProfileId == Profil.MARY ) then
						HandleMaryArrival( Profil.MARY )
					-- John
					elseif ( ProfileId == Profil.JOHN ) then
						HandleJohnArrival( Profil.JOHN )
					end
					
			-- Maria		
			elseif ( ProfileId == Profil.MARIA and CheckNPCSectorBool( Profil.MARIA, 6, SectorY.MAP_ROW_C, 0) == true and CheckFact(Facts.FACT_MARIA_ESCORTED_AT_LEATHER_SHOP,Profil.MARIA) == true ) then
			
				if ( NPCInRoom( Profil.ANGEL, 2 ) == true ) then
					TriggerNPCRecord( Profil.ANGEL, 12 )
				end

			-- Joey
			elseif ( ProfileId == Profil.JOEY and CheckNPCSectorBool( Profil.JOEY, 8, SectorY.MAP_ROW_G, 0) == true and CheckFact(Facts.FACT_JOEY_NEAR_MARTHA,0) == true ) then
					ActionStopMerc(Profil.JOEY)
					TriggerNPCRecord( Profil.JOEY, 9 )
			end
		
		-- Drassen stuff for John & Mary
		elseif ( CheckQuest(Quests.QUEST_ESCORT_TOURISTS) == pQuest.QUESTINPROGRESS and ProfileIdsSectorX == 13 and ProfileIdsSectorY == SectorY.MAP_ROW_B and ProfileIdbSectorZ == 0 ) then
			
		if ( CheckFact( Facts.FACT_JOHN_ALIVE, 0 ) == true ) then
				HandleJohnArrival( nil )
		else
				HandleMaryArrival( nil )
		end
		
	elseif ( TeamSoldier == Team.CIV_TEAM and ProfileId ~= NO_PROFILE and CheckSoldierNeutral( ProfileId ) == true ) then
	
		if ( ProfileId == Profil.JIM or ProfileId == Profil.JACK or ProfileId == Profil.OLAF or ProfileId == Profil.RAY or ProfileId == Profil.OLGA or ProfileId == Profil.TYRONE ) then
	
			if ( not TileIsOutOfBounds( ClosestPC( ProfileId, sDesiredMercDist )) ) then 
				if ( sDesiredMercDist <= NPC_TALK_RADIUS * 2 ) then
					CancelAIAction ( ProfileId ) 
					AddToShouldBecomeHostileOrSayQuoteList( GetPlayerMercID(ProfileId) )
				end
			end
		end
		
		end -- End what EPC
	end -- End team
end
	
ActionTypes =
{
	ACTIONTYPE_INVALID = 0,
	ACTIONTYPE_HACKING = 1,
	ACTIONTYPE_READFILE = 2,
	ACTIONTYPE_WATERTAP = 3,
	ACTIONTYPE_SODAMACHINE = 4,
	ACTIONTYPE_MINIGAME = 5,
}

InfoTypes = 
{	
	INFO_TYPE_NORMAL = 0,
	INFO_TYPE_VIP = 1,
}

ScreenTypes = 
{
	MINIGAME = 30,
}

MiniGames =
{
	TETRIS = 0,
	PONG = 1,
}

-- We have an array of 1000 signed integers that a modder can use to set whatever data he wants.
-- We simply set up some enums here to make it easier for us to remember what is what
ModSpecificActions =
{
	-- |||||||||||||||||||||||||||||||||| computers |||||||||||||||||||||||||||||||||||||
	-- Orta sublevel 1
	COMPUTER_ORTA_BASEMENT_LAB1_SERVER = 0,
	COMPUTER_ORTA_BASEMENT_LAB2_SERVER = 1,
	COMPUTER_ORTA_BASEMENT_HALLWAY_SERVER = 2,
	COMPUTER_ORTA_BASEMENT_LAB1_PC = 3,
	COMPUTER_ORTA_BASEMENT_LAB2_PC = 4,
	COMPUTER_ORTA_BASEMENT_CONTROL_CONSOLE_1 = 5,
	COMPUTER_ORTA_BASEMENT_CONTROL_CONSOLE_2 = 6,
	COMPUTER_ORTA_BASEMENT_ENTRANCE_CONSOLE_1 = 7,
	COMPUTER_ORTA_BASEMENT_ENTRANCE_CONSOLE_2 = 8,
	
	-- Orta
	COMPUTER_ORTA_SERVER_1 = 9,
	COMPUTER_ORTA_PC_1 = 10,
	
	-- Balime
	COMPUTER_BALIME_L12_MUSEUM_CONSOLE_1 = 11,
	COMPUTER_BALIME_L12_MUSEUM_CONSOLE_2 = 12,
	
	-- Drassen SAM
	COMPUTER_DRASSENSAM_PC_1 = 13,
	COMPUTER_DRASSENSAM_COMMANDCONSOLE = 14,
	
	-- Chitzena SAM
	COMPUTER_CHITZENASAM_PC_1 = 15,
	COMPUTER_CHITZENASAM_PC_2 = 16,
	COMPUTER_CHITZENASAM_COMMANDCONSOLE = 17,
	
	-- central SAM
	COMPUTER_CENTRALSAM_PC_1 = 18,
	COMPUTER_CENTRALSAM_PC_2 = 19,
	COMPUTER_CENTRALSAM_PC_3 = 20,
	COMPUTER_CENTRALSAM_COMMANDCONSOLE = 21,
	
	-- Meduna SAM
	COMPUTER_MEDUNASAM_PC_1 = 22,
	COMPUTER_MEDUNASAM_COMPUTERBANK_1 = 23,
	COMPUTER_MEDUNASAM_COMMANDCONSOLE = 24,
	
	-- Meduna palace
	COMPUTER_MEDUNAPALACE_PC_1 = 25,
	COMPUTER_MEDUNAPALACE_PC_2 = 26,
	COMPUTER_MEDUNAPALACE_CONSOLE_1 = 27,
	COMPUTER_MEDUNAPALACE_SERVER_1 = 28,
	
	-- Alma storage
	COMPUTER_ALMASTORAGE_PC_1 = 29,
	
	-- Alma prison
	COMPUTER_ALMAPRISON_PC_1 = 30,
	COMPUTER_ALMAPRISON_PC_2 = 31,
	COMPUTER_ALMAPRISON_PC_3 = 32,
	
	-- Alma base
	COMPUTER_ALMABASE_PC_KROTT = 33,
	COMPUTER_ALMABASE_PC_BUREAU = 34,
	COMPUTER_ALMABASE_PC_FILEROOM = 35,
	COMPUTER_ALMABASE_CONSOLE_1 = 36,
	COMPUTER_ALMABASE_CONSOLE_2 = 37,
	COMPUTER_ALMABASE_CONSOLE_3 = 38,
	COMPUTER_ALMABASE_CONSOLE_4 = 39,
	COMPUTER_ALMABASE_CONSOLE_5 = 40,
	COMPUTER_ALMABASE_CONSOLE_6 = 41,	
	-- |||||||||||||||||||||||||||||||||| computers |||||||||||||||||||||||||||||||||||||
	
	-- |||||||||||||||||||||||||||||||||| books |||||||||||||||||||||||||||||||||||||
	-- Orta sublevel 1
	READING_ORTA_BASEMENT_BOOKSHELF_1 = 45,
	READING_ORTA_BASEMENT_BOOKSHELF_2 = 46,
	READING_ORTA_BASEMENT_BOOKSHELF_3 = 47,
	
	-- Orta
	READING_ORTA_BOOKSHELF_1 = 48,
	
	-- San Mona
	READING_SANMONA_KINGPIN_BOOKSHELF_1 = 49,	
	-- |||||||||||||||||||||||||||||||||| books |||||||||||||||||||||||||||||||||||||
	
	-- |||||||||||||||||||||||||||||||||| minigames |||||||||||||||||||||||||||||||||||||
	MINIGAME_TETRIS = 50,
	MINIGAME_PONG = 51,
	-- |||||||||||||||||||||||||||||||||| minigames |||||||||||||||||||||||||||||||||||||
	
	-- |||||||||||||||||||||||||||||||||| photo data |||||||||||||||||||||||||||||||||||||
	PHOTO_FLAGS_BEGIN = 60,
	
	PHOTO_FLAGS_GENERAL = 61,
	PHOTO_FLAGS_WARDEN = 62,
	PHOTO_FLAGS_ERNEST = 63,
	PHOTO_FLAGS_ARMAND = 64,
	PHOTO_FLAGS_KINGPIN = 65,
	PHOTO_FLAGS_DARREN = 66,
	PHOTO_FLAGS_TONY = 67,
	PHOTO_FLAGS_JOE = 68,
	PHOTO_FLAGS_MADLAB = 69,
	PHOTO_FLAGS_MIKE = 70,
	PHOTO_FLAGS_DEVIN = 71,
	PHOTO_FLAGS_MICKY = 72,
	PHOTO_FLAGS_CARMEN = 73,
		
	PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE = 80,
	PHOTO_FLAGS_ALMA_HQ_CONTROLROOM = 81,
	PHOTO_FLAGS_ALMA_STORAGE_ROCKETS = 82,
	PHOTO_FLAGS_ALMA_PRISON_TORTURE = 83,
	PHOTO_FLAGS_TIXA_TORTURE = 84,
	PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS = 85,
	PHOTO_FLAGS_CREATURE_LVL3 = 86,	
	PHOTO_FLAGS_ORTA_LABS = 87,
	PHOTO_FLAGS_ORTA_STORAGE = 88,
	PHOTO_FLAGS_ORTA_MANUFACTURING = 89,
	PHOTO_FLAGS_BALIME_MUSEUM = 90,	
	PHOTO_FLAGS_GRUMM_RADIOACTIVE = 91,
	PHOTO_FLAGS_MEDUNA_ARENA = 92,
	PHOTO_FLAGS_MEDUNA_BUNKER = 93,
	
	PHOTO_FLAGS_SAM_DRASSEN = 94,
	PHOTO_FLAGS_SAM_CHITZENA = 95,
	PHOTO_FLAGS_SAM_CENTRAL = 96,
	PHOTO_FLAGS_SAM_MEDUNA = 97,
	
	PHOTO_FLAGS_END = 100,
	-- |||||||||||||||||||||||||||||||||| photo data |||||||||||||||||||||||||||||||||||||
	
}

-- General defines for various states
ActionState = 
{
	-- hacking
	STATE_OK = 0,				-- starting state, we can hack this
	STATE_END = 1,				-- computer is shut down and cannot be used
	STATE_GAVEREWARD_OK = 2,	-- computer is still useable, we already got whatever reward there was
	STATE_GAVEREWARD_END = 3,	-- computer is shut down and cannot be used, we already got whatever reward there was
}

-- handle the result of an interactive action that has a special id set for lua
-- for more info, see InteractiveActions.xml
function HandleInteractiveActionResult(sSectorX, sSectorY, bSectorZ, sGridNo, bLevel, ubID, usActionType, sLuaactionid, difficulty, skill )
		
end

-- states of a photo fact
PhotoFlag = 
{
	NONE				= 0,
	TAKEN 				= 1,
	UPLOADED 			= 2,
	VERIFIED 			= 3,	-- the code only has one value for verification, but we use several ones for different results
	VERIFIED_RESULT_2 	= 4,
}

-- In this function we can define what and how many items a intel-based dealer should trade with, and how much an item is worth in intel
-- AddArmsDealerAdditionalIntelData takes 4 arguments:
-- usDealer: shopkeeper this is intended for
-- usItem: number of item
-- sIntelPrice: price of item in intel
-- sOptimalNumber: how many items the trader should have at maximum
function AddArmsDealerAdditionalIntelData()
	
	
end

function SetPhotoState( aIndex, aState )
	
	if ( (aIndex >= ModSpecificActions.PHOTO_FLAGS_BEGIN) and (aIndex <= ModSpecificActions.PHOTO_FLAGS_END) and (aState >= PhotoFlag.NONE) and (aState <= PhotoFlag.VERIFIED_RESULT_2) ) then
		
		SetModderLUAFact( aIndex, aState )
		
	end
end

-- sSectorX, sSectorY, bSectorZ: sector coordinates
-- sGridNo: tile that we photographed. When we take a photo this function is run on all tiles we see in a 2-tile radius around where we clicked
-- bLevel: height level (floor or roof)
-- ubPhotographerProfile: profile of the merc taking the photo
-- room: if > 0, this tile is inside this room
-- usTargetProfile: if we photographed a NPC, this will be their profile number, otherwise NO_PROFILE
function AddPhotoData( sSectorX, sSectorY, bSectorZ, sGridNo, bLevel, ubPhotographerProfile, room, usTargetProfile )
	
	
	
end

function GetPhotoData( aType )

	
	
end

function VerifyPhotoState(aIndex)
	
	
	
end

-- types of weather
Weather = 
{
	WEATHER_FORECAST_NORMAL = 0,
	WEATHER_FORECAST_RAIN = 1,
	WEATHER_FORECAST_THUNDERSHOWERS = 2,
	WEATHER_FORECAST_SANDSTORM = 3,
	WEATHER_FORECAST_SNOW = 4,
};

-- skills
Skill =
{		
	-- radio operator
	SKILLS_RADIO_ARTILLERY = 0,
	SKILLS_RADIO_JAM = 1,
	SKILLS_RADIO_SCAN_FOR_JAM = 2,
	SKILLS_RADIO_LISTEN = 3,
	SKILLS_RADIO_CALLREINFORCEMENTS = 4,
	SKILLS_RADIO_TURNOFF = 5,

	SKILLS_INTEL_CONCEAL = 6,
	SKILLS_INTEL_GATHERINTEL = 7,
	
	-- various
	SKILLS_SPOTTER = 8,
	SKILLS_FOCUS = 9,
	SKILLS_DRAG = 10,
};

-- different dialogue action events
DialogueActionEvent = 
{
	ADE_DISCOVER_ROOM = 0,					-- merc is the 1st one to explore this room, aData1 indicates the room in this event
	ADE_DIALOGUE_REACTION = 1,				-- someone else used a line, and we might answer to that, aData1 indicates the profile number in this event, aData2 indicates the specific voice line we react to, aData3 indicates whether this person has been recruited (1) or not(0)
	ADE_MERC_ARRIVES = 2,					-- a mercenary arrives in Arulco. aData1 is the profile of the arriving merc, aData2 is 1 if the player hasn't made contact with the rebels yet
	ADE_SECTOR_COMMENTARY = 3,				-- similar to the rebel's quotes, this is called upon visiting a sector for the first time (if no rebel does their quote first)
	ADE_MERCHANT_CHAT = 4,					-- we greet a merchant (used with non-profile merchants, in order to be a bit more lively), aData1 is the id of the merchant
	ADE_DIALOGUE_NPC_FRIENDLY = 5,			-- in a conversation, we can say something when clicking on a button
	ADE_DIALOGUE_NPC_DIRECT = 6,
	ADE_DIALOGUE_NPC_THREATEN = 7,
	ADE_DIALOGUE_NPC_RECRUIT = 8,
	ADE_DIALOGUE_NPC_REPEAT = 9,
	ADE_DIALOGUE_RPC_RECRUIT_SUCCESS = 10,	-- we've successfully recruited an RPC, and might comment on that
	ADE_SEX = 11,							-- does exactly what you think it does
	ADE_WEATHERCHANGE = 12,					-- the weather has changed in the current sector, and we can warn the player about that, aData1 is type of Weather
	ADE_SKILL_RESULT = 13,					-- we used a skill, and comment on the result, aData1 is of Skill, aData2 indicates whether it was a success (1) or failure (0)
	ADE_GRENADEWARNING = 14,				-- a delayed enemy grenade was dropped nearby, and we can shout a warning, aData1 is the itemnumber of the grenade
	ADE_CONSUMEITEM = 15,					-- we consumed an item, aData1 is the item index
	ADE_NPC_DEATH = 16,						-- someone died, aData1 is the profile of the dead NPC, aData2 the profile of the killer, aData3 the bodytype of the dead
	ADE_WITNESS_GOOD = 17,					-- similar to comment upon buddy doing good. aData1 is the profile of person, aData2 is 0 if we saw a kill, 1 if they passed a skill check (and we're less than 15 tiles away)
	ADE_BANDAGE_PERFORM_BEGIN = 18,			-- we bandage someone, aData1 is the profile of person
	ADE_BANDAGE_PERFORM_END = 19,			-- we finished bandaging someone, aData1 is the profile of person
	ADE_BANDAGE_RECEIVE_BEGIN = 20,			-- someone bandages us, aData1 is the profile of person
	ADE_BANDAGE_RECEIVE_END = 21,			-- someone finished bandaging us, aData1 is the profile of person
	ADE_ZOMBIESRISE = 22,					-- a new wave of zombies is spawned from corpses
}

-- functions used here:
-- ubProfile: profile number of merc (don't change that unless you know what to do)
-- iFaceIndex: index (don't change that unless you know what to do)
-- path: path and name of soundfile we want to play (in .wav or .ogg format)
-- text: "Text that should appear"
-- SetAdditionalDialogue(ubProfile, iFaceIndex, path, text)
-- Do not use this function in any lua calls outside of HandleAdditionalDialogue(..)!
-- 
-- path: path and name of soundfile we want to play (in .wav or .ogg format)
-- volume: optional sound volume (65: medium volume, 127: loud), default 65
-- PlaySound(path, volume)

-- handle
-- sSectorX, sSectorY and bSectorZ indicate the sector coordinates
-- ubProfile is the merc for whom this is called
-- iFaceIndex is the face of the merc. DON'T CHANGE THIS UNLESS YOU KNOW WHAT YOU ARE DOING!
-- usEventNr indicates which event from DialogueActionEvent is used here
-- aData1, aData2, aData3 are additional data, see event description on what they do in each event
function HandleAdditionalDialogue(sSectorX, sSectorY, bSectorZ, ubProfile, iFaceIndex, usEventNr, aData1, aData2, aData3 )
	
end
