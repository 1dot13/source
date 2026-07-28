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
	LAURA = 175,
	GRACE = 176,
	RUDOLF = 177,
	SKITZ = 179,
	BUNS_CHAOTIC = 215,
	LEON = 222,
	GARY = 223,
	SPARKY = 224,
	HURL = 227,
	MOUSE = 245,
	HECTOR = 246,
	REV = 247,
	STELLA = 248,
	MOSES = 250,
	TURTLE = 253,
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

-- (relevant) bodytypes
Bodytype = 
{
	REGMALE = 0,
	BIGMALE = 1,
	STOCKYMALE = 2,
	REGFEMALE = 3,
	FATCIV = 11,
	MANCIV = 12,
	MINICIV = 13,
	DRESSCIV = 14,
	HATKIDCIV = 15,
	KIDCIV = 16,
	CRIPPLECIV = 17,
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
	ACTIONTYPE_VARIOUS = 6,
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
	PICTURE = 2,
}

-- luaactionids that aren't used in ModSpecificActions
InteractiveAction =
{
	MINIGAME_TETRIS = 50,
	MINIGAME_PONG = 51,	
	BILLIARD = 52,
	TABLETENNIS = 53,
	TOILET = 54,
	TV = 55,
	STEAL_CASHREGISTER = 56,
	PICTURE_NEONTHECLUB = 57,			-- Neon sign: 'The club'
	PICTURE_NEONBEER = 58,				-- Neon sign: 'Beer'
	PICTURE_BEER = 59,					-- Poster: 'Beer'
	PICTURE_2xBEER = 60,				-- 2-tile Poster: 'Beer'
	PICTURE_PRIVATE = 61,				-- Sign: 'Private'
	PICTURE_XXXPITSTOP = 62,			-- Neon Sign: 'XXX Pit Stop'
	PICTURE_DELIVERY = 63,				-- 2-tile Poster: 'Delivery'
	PICTURE_GENERALSTORE = 64,			-- 2-tile Poster: 'General Store'
	PICTURE_SHADYLADY = 65,				-- 2-tile Poster: 'Shady Lady'
	COOK_COWMEAT_TO_STEAK = 66,			-- take cow meat from merc, add steak to merc
	TAKE_FIRE_EXTINGUISHER_1 = 67,		-- delete wall decal (requires replacing wall tile) and add item to inventory
	TAKE_FIRE_EXTINGUISHER_2 = 68,		-- delete wall decal (requires replacing wall tile) and add item to inventory
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
		
	if ( usActionType == ActionTypes.ACTIONTYPE_HACKING ) then
		
		-- if computer is locked, we only get an error message
		if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_END ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_RED, "... 无法连接主机,自动断开。")
			
			return
		-- we cannot access this anymore, but gained something previously
		elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_END ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_RED, "... 无法连接主机,自动断开")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "那里大概没有有趣的东西。")
			
			return
		end
		
		-- if we successfully hacked the Orta sublevel server that connects to Alma, hacking difficulty in Alma is lowered
		if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB2_SERVER) == ActionState.STATE_GAVEREWARD_OK) or (GetModderLUAFact(ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB2_SERVER) == ActionState.STATE_GAVEREWARD_END) ) then
			if ( sSectorX == 13 and sSectorY == SectorY.MAP_ROW_H) then			
				difficulty = difficulty - 20
			end
		end
		
		if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_CENTRALSAM_PC_3) == ActionState.STATE_GAVEREWARD_OK) or (GetModderLUAFact(ModSpecificActions.COMPUTER_CENTRALSAM_PC_3) == ActionState.STATE_GAVEREWARD_END) ) then
			if ( sSectorX == 4 and sSectorY == SectorY.MAP_ROW_N) then			
				difficulty = difficulty - 20
			end
		end
		
		-- if skill >= difficulty, we succeed
		successpts = skill - difficulty
		
		-- if we fail very badly, shut down server
		if ( successpts < - 30 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 检测到未授权访问")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 与服务器强制断开")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 请向您的系统管理员寻求指导")
			SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
			
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_END)
			else
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end
			
			return
		-- if we just fail, give us a message
		elseif ( successpts < 0 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 密码错误。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 弹出中。")
			
			return
		end
		
		-- we need to remember this, so we know whether to give a reward later on
		statebefore = GetModderLUAFact(sLuaactionid)
		
		-- ORTA sublevel
		-- info on Ernest's research (rocket rifle)
		if ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB1_SERVER ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "火箭枪设计图就存放在这里：")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "火箭枪：基于20mm火箭弹的单兵武器。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "每支枪配一个5发容量的弹匣。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "火箭弹有多个弹种：")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 穿甲弹")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 高爆弹")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 高爆穿甲弹")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 冷冻弹")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "即使对付着甲士兵，这种武器的杀伤力也'过大'了。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这种武器也能用来反器材。")
			end
		-- this server is connected to Alma, hacking it lowers difficulty there
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB2_SERVER ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "您可以访问网络。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这台服务器仍然与Alma基地保持连网。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "尽管很难，但是你试着装个后门。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 与Alma军用网确立连接中")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 密码：********")		

				-- this is really, REALLY hard to do
				if ( successpts > 80 ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 密码通过。")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... cd ~")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... chmod 777 *")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... chmod：你无权执行此命令")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... sudo chmod 777 *")
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN,     "我们已劫持这台服务器，黑进Alma基地的SAM更简单了。")
					
					-- if we are extremely good, we can reset some computers if they were locked in Alma
					if ( successpts > 80 ) then
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_KROTT) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_KROTT, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_KROTT) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_KROTT, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_BUREAU) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_BUREAU, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_BUREAU) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_BUREAU, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_FILEROOM) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_FILEROOM, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_FILEROOM) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_PC_FILEROOM, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_1) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_1, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_1) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_1, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_2) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_2, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_2) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_2, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_3) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_3, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_3) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_3, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_4) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_4, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_4) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_4, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_5) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_5, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_5) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_5, ActionState.STATE_GAVEREWARD_OK)
						end
						
						if ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_6) == ActionState.STATE_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_6, ActionState.STATE_OK)
						elseif ( (GetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_6) == ActionState.STATE_GAVEREWARD_END) )	then
							SetModderLUAFact(ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_6, ActionState.STATE_GAVEREWARD_OK)
						end
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN,     "我们还能重置某些锁定的工作站。")
					end
					
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
					SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_END)
				else
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "黑入服务器失败了")				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 密码错误。")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 弹出中。")
				end
			end	
		-- info on weapon tests
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_HALLWAY_SERVER ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "武器试验结果就在这台服务器中。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "在项目后期，该武器用活人囚犯做试验。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这些试验留有影像记录... 这些残忍的演示十分可怕。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "要是这些视频流出去, 那些参与的企业都会很头疼吧。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你不会想看第二次的。")
			end
		-- Ernest's PC
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB1_PC ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你挖通了Poppin博士的私人研究所。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "貌似已经开发出更先进的火箭枪。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "有些文件缺失了... 你找不到武器设计图。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "改进版并未比原型枪先进多少。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "未说明原型枪在哪。")
			end
		-- money
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB2_PC ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "有个研究员还在线。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "也许能截流项目资金？")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 进入项目管理")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 识别：Poppins, E。")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 开始转账")
				
				-- depending on how skilled we are, we get different amounts of money
				AddTransactionToPlayersBook(1, 0, 1800, successpts * 40)
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "项目资金转到自己户头啦！")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你玩了会纸牌游戏。")
			end
		-- hint on MADLAB missing
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_CONTROL_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "发给：ORTA 安保主管")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "来自：王家顾问")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "我们不允许N. Kairns博士缺席。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "作为研究所主管，他不可或缺。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你的前任没能抓到他。你不能再失败了。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "听起来很棘手。这位Kairns博士貌似很有趣。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "剩下的文件都是废话。")
			end
		-- hint on secret weapons stash
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_CONTROL_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这座基地有不同级别的权限。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "某些仓库有重兵把守。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "炸药可能不够用，有些门需要钥匙码。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "技术主管或者守卫可能有钥匙码。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "文件里没说仓库里放的是啥。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你想知道基地里的秘密。")
			end
		-- info: some of the secret weapons got away
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_ENTRANCE_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "一份最近抵达和离开基地的物资记录表。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "很长一段时间里，这座基地只接收物资。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "但是最近，这里往Meduna发送不少货物。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "记录表没说发送的是什么，但终点是皇家卫队指挥部。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你对此感到不妙。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "是UNIX系统... 这个我懂！")
			end
		-- warning on poison gas trap
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_ENTRANCE_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到建筑结构图。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "隔壁是个带厕所的小实验室。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这里是中央走廊。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "貌似走廊里有多种毒气陷阱。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "走廊左右是两座巨大的制造厂房。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "走廊一侧是安保部门。看起来人很多。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "另一扇安保门后是主实验室，但是没有摄像头和可利用。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "控制台上没有其他东西了，你需要自行探索基地。")
			end
		-- ORTA: bloodcats are studied here, so the computer has data on them 
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_SERVER_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "服务器里有前几个月的货单。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "从一个小监测站往这里提供这么多资源。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "实际上比你看到的还多得多。你琢磨起这批装备的去向。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "这不太对劲。。。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你看见设备清单里没有用于观察巨猫的。")
			end
		-- ORTA: bloodcats are studied here, so the computer has data on them 
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这台电脑中有大量血猫生理和习性资料。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "好像Alma以东有大群血猫。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "实际上，血猫数量过多已经成为安全隐患。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到很多稀有猫照片，但是关于这个的，没有发现更多内容。")
			end
		-- BALIME: the museum security consoles tell us a bit about the museum
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_BALIME_L12_MUSEUM_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "博物馆安保系统已经很多年没升级。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "所有摄像头都在同一个网络里，共用一个简单的开/关控制。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "控制台很可能就在这间屋子。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "摄像头可以帮你监视博物馆。")
			end			
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_BALIME_L12_MUSEUM_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "电脑里有所有展品清单。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "大部分是古雕塑和青铜器。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "但最值钱的还是圣杯。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "除了圣杯，博物馆里没有好东西。")
			end
		-- Drassen SAM: get info from the pc and use the command console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_DRASSENSAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "黑进一台军方人事部门资料库！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析文件")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析完成")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问资料库")
								
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "有了这些资料就能了解敌军动向！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 检测到未授权访问")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 强制断开服务器连接")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 请向系统管理员寻求帮助")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_DRASSENSAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "好像是控制SAM的控制台。")
				
				samhackstatus = GetSamSiteHackStatus(sSectorX, sSectorY)
				
				ourhack = 100 - 2 * successpts
				
				if ( ourhack < 0 ) then
					ourhack = 0
				end
								
				-- we can hack the SAM, decreasing its radius, or undo a previous hack
				-- for simplicity, we will always hack a SAM if enemies are nearby - we are either infiltrating or fighting the enemy in those cases
				-- otherwise, we obviously control the SAM, so try to unhack it			
				if ( NumNonPlayerTeamInSector(sSectorX, sSectorY, Teams.ENEMY_TEAM) > 0 ) then
					if ( ourhack >= samhackstatus ) then
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "有人黑进过这里 - 他们比你破坏得还彻底。")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "我们破坏过文件系统 - 想再跑起来得叫专家来。")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你想修复它。")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "成了，指向软件正常工作。")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "无论谁黑进这里都比我们厉害 - 我们不能放弃控制权。")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "软件貌似运行正常，最好别破坏它。")
					end
				end	
			end
		-- Chitzena SAM: get info from the pc and use the comman console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CHITZENASAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "黑进一台军方人事部门资料库！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析文件")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析完成")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问资料库")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "有了这些资料就能了解敌军动向！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 检测到未授权访问")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 强制断开服务器连接")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 请向系统管理员寻求帮助")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CHITZENASAM_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "显然，这是指挥官的电脑。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "很幸运，他走了但是没退出。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问Arulcan军方财务服务器")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 识别：Gutierrez上尉，A。")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 开始转账")
				
				-- depending on how skilled we are, we get different amounts of money
				AddTransactionToPlayersBook(1, 0, 1800, successpts * 30)
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "我们把这个军官的小金库转到自己户头了！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "他好像还没意识到。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "好多A片，都是你看过的。")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CHITZENASAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "好像是控制SAM的控制台。")
				
				samhackstatus = GetSamSiteHackStatus(sSectorX, sSectorY)
				
				ourhack = 100 - 2 * successpts
				
				if ( ourhack < 0 ) then
					ourhack = 0
				end
								
				-- we can hack the SAM, decreasing its radius, or undo a previous hack
				-- for simplicity, we will always hack a SAM if enemies are nearby - we are either infiltrating or figting the enemy in those cases
				-- otherwise, we obviously control the SAM, so try to unhack it			
				if ( NumNonPlayerTeamInSector(sSectorX, sSectorY, Teams.ENEMY_TEAM) > 0 ) then
					if ( ourhack >= samhackstatus ) then
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "有人黑进过这里 - 他们比你破坏得还彻底。")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "我们破坏过文件系统 - 想再跑起来得叫专家来。")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你想修复它。")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "成了，指向软件正常工作。")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "无论谁黑进这里都比我们厉害 - 我们不能放弃控制权。")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "软件貌似运行正常，最好别破坏它。")
					end
				end	
			end
		-- central SAM: get info from the pc and use the command console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "黑进一台军方人事部门资料库！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析文件")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析完成")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问资料库")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "有了这些资料就能了解敌军动向！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 检测到未授权访问")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 强制断开服务器连接")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 请向系统管理员寻求帮助")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "基地的安保措施里有份布置图。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "要塞以外，应该还有一些迫击炮弹存放在储藏箱里。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "貌似还有片雷区！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "貌似不能遥控关停。")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "避开建筑南部。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你继续查找，但是这里没有雷场的详细布置。")
			end
		-- hacking this pc lowers hacking difficulty in other places
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_PC_3 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "貌似所有SAM都连接到Meduna的SAM基地。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "也许能远程黑进去？")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 与Meduna空管建立连接")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 密码：********")		

				-- depending on how good we are, this might work
				if ( successpts > 50 ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 密码通过。")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... cd ~")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... rm -rf *")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... rm：你没有权限执行此命令")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... sudo rm -rf *")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... ...")
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN,     "成功黑进服务器，黑进Meduna的SAM基地更容易了。")
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
					SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_END)
				else
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "没能黑进服务器")				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 检测到未授权访问")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 强制断开服务器连接")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 请向系统管理员求助")
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
					SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
				end
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "好像是SAM导弹控制台。")
				
				samhackstatus = GetSamSiteHackStatus(sSectorX, sSectorY)
				
				ourhack = 100 - 2 * successpts
				
				if ( ourhack < 0 ) then
					ourhack = 0
				end
								
				-- we can hack the SAM, decreasing its radius, or undo a previous hack
				-- for simplicity, we will always hack a SAM if enemies are nearby - we are either infiltrating or figting the enemy in those cases
				-- otherwise, we obviously control the SAM, so try to unhack it			
				if ( NumNonPlayerTeamInSector(sSectorX, sSectorY, Teams.ENEMY_TEAM) > 0 ) then
					if ( ourhack >= samhackstatus ) then
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "有人曾经黑进这里 - 而且比我们破坏得更厉害。")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "已经破坏文件系统 - 想重新跑起来得找专家。")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你试着修复它。")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "成了，指向软件正常运行。")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "任何能黑进这里的都比我们强 - 我们不能放弃控制权。")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "软件正常运行，最好不要破坏它。")
					end
				end	
			end
		-- Meduna SAM: get info from the pc and use the command console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNASAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "黑进一台军方人事数据库！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析文件")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析完成")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问数据库")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "有了这些数据我们就能了解敌军动向！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 检测到未授权访问")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 强制断开服务器连接")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 请向系统管理员求助")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNASAM_COMPUTERBANK_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这台资料库中记录了所有抵达Meduna机场的飞机。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "从国外来的运输机太频繁了。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "很明显敌军在招募雇佣兵。")
			end		
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNASAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "好像是SAM导弹控制台。")
				
				samhackstatus = GetSamSiteHackStatus(sSectorX, sSectorY)
				
				ourhack = 100 - 2 * successpts
				
				if ( ourhack < 0 ) then
					ourhack = 0
				end
								
				-- we can hack the SAM, decreasing its radius, or undo a previous hack
				-- for simplicity, we will always hack a SAM if enemies are nearby - we are either infiltrating or figting the enemy in those cases
				-- otherwise, we obviously control the SAM, so try to unhack it			
				if ( NumNonPlayerTeamInSector(sSectorX, sSectorY, Teams.ENEMY_TEAM) > 0 ) then
					if ( ourhack >= samhackstatus ) then
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "有人曾经黑进这里 - 而且比我们破坏得更厉害。")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "已经破坏文件系统 - 想重新跑起来得找专家。")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你试着修复它。")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "成了，指向软件正常运行。")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "任何能黑进这里的都比我们强 - 我们不能放弃控制权。")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "软件正常运行，最好不要破坏它。")
					end
				end	
			end
		-- MEDUNA PALACE BUNKER
		-- entrance desktop contains a list of important people allied to the queen
		-- if you examine the names very closely, and also did so in Alma, you might learn something interesting here :-) (not yet implemented ingame)
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这里登记着出入地堡的人。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "很多大人物都在这。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- V. Garcia，商人")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Cmdr. A. Morris，Tracona军人")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- D. Harrows，Harrows服装有限公司")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- A. Ricci，Ricci矿业与勘探")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Major E. Tavarez，Cali军人")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- D. Van Haussen，商人")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Col. T. Meyers，美国军人")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Major J.A. Vargas，PG私人军事公司")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- E. Poppins博士，R&D")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "将这些人与政府连起来很有用。")
			end
		-- desktop, control room
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "根据协议，一旦发生紧急情况，女王会转移到这里。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "一般人不知道地堡的存在，地堡守卫森严。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你找不到弱点。")			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "占领军不得不歼灭整个卫戍部队才能攻陷地堡。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你速测自己的排雷技能。仍然很棒！")
			end
		-- 
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "地堡的安保措施里有份设计图。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "地堡有两个入口 - 一个在王宫，另一个在逃生地道。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "地堡中有一大片区域除女王侍从外所有人严禁进入。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "走廊中有一片雷区。")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "控制台应该在附近。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "在这看都觉得雷区太恶毒。")
			end
		-- placeholder - until we have something more interesting, let's have this be a corrupted server
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_SERVER_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问文件。")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 检测到文件被破坏。")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 结束。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "不管服务器里曾经有什么，都已经被删掉了。没办法修复。")
			end		
		-- ALMA storage
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMASTORAGE_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这里有仓库内物资清单。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 1x M14")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 1x FAMAS")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 几套护甲")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 弹药和手榴弹")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 实验用的火箭弹")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 燃料")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 9x 弹道导弹")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "除了清单，电脑里没有其他有用的信息。")
			end
			
		-- ALMA prison
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMAPRISON_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "电脑里有人事档案。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "每隔几周卫戍部队就会调换。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "你可以减去一些部队调动。")
								
				-- give us intel
				if ( successpts > 40 ) then
					AddIntel( 40 )
				elseif ( successpts > 10 ) then
					AddIntel( 20 )
				else
					AddIntel( 10 )
				end
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "文件下载完成。")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMAPRISON_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这座监狱是军方用来关押高价值囚犯的。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "看起来监狱依然在用。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你看了一些记录. 非常恶心.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "囚犯们被定期殴打，电击，十分痛苦。。。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "记录很长。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "如果把记录公布出去，政府的公共关系会暴跌。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "看够了。")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMAPRISON_PC_3 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这台电脑中有监狱结构图。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "兵营和办公室在左侧。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "食堂在中部。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "犯人全部关在右侧区域。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "关押犯人的区域可以被毒气全部覆盖！")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "控制开关在办公室至食堂的走廊中。")
			end
		-- Alma base
		-- the computer of Sgt. Krott has data on the secret weapons project
		-- if you are good, you can retrieve the location of the project lead (a general)
		-- keep in mind that this computer will be destroyed if the bomb goes off
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_PC_KROTT ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你会看到一些绝密武器项目的档案。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "它似乎是关于某种新型步兵武器。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "您可以精确定位军事项目负责人的位置！")
				
				AddIntel( 50 )
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "文件下载完成。")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_PC_BUREAU ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "访问Humphey将军的个人档案。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "他总管军队训练。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "他的个人档案记载他是美国政府的'顾问'。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "这条信息泄露出去会造成外交麻烦。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你浏览因特网并发现两条新消息。")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_PC_FILEROOM ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "很多人的薪水通过这台电脑发放。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "你试着转账。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问Arulcan军方财务服务器")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 识别：Humphey将军，T。")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 开始转账")
				
				-- depending on how skilled we are, we get different amounts of money
				AddTransactionToPlayersBook(1, 0, 1800, successpts * 60)
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "我们把很多军队资金转到自己户头上！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "他们不需要钱了。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你造成更多破坏，还在留言板上说风凉话。")
			end
		-- info on town layout
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你找到Alma全部军用设施的详细资料。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这座基地作为联合指挥/训练基地。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "东部有几座仓储设施。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "南部有军队自营的监狱用于关押高价值犯人。")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "民居位于东南部靠近矿井。")	
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "如果能控制这座基地就太好了。")
			end
		-- warning that parts of the base blow up if you press the red switch
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "维修保养记录表明这座基地近期经过升级改造。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "最近一些光敏材料被送到基地。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "一个故障保护就让他们飘了，控制台就在这间屋里。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "军队在保护某个特别有价值的东西。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "在你看来，在自家基地装诡雷的人都是脑子有坑。")
			end
		-- info in foreign mercs (Conrad, Iggy, Mike)
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_3 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这台电脑里有军方雇佣外国雇佣兵的文件。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Conrad Gillit，教官")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Igmus Palkov，重武器")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Mike [REDACTED]，突击队指挥官")				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "也许能抓捕招揽他们。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "除了Conrad，Iggy和Mike，没发现其他外国雇佣兵。")
			end
		-- info on suspected rebels
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_4 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你找到几份旧情报。是有关反抗军头目的。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Cordona，M。 - 发现就杀")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Dasouza，C。 - 发现就杀")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Marquez，S。 - 中立")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Vargas，J。 - [已修改]")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Valenciano，C。 - 中立")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Dominguez，R。 - 中立")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Rivas，M。 - 推测被流放")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Serrano，A。 - 中立")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Guzzo，D。 - 发现就杀")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Portela，N。 - 中立")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Casilla，A。 - 中立")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Smythe，I。 - 发现就杀")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "军队已经铲平大部分反抗军。只剩一小撮还在抵抗。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "反抗军的损失清单真是惨不忍睹。")
			end
		-- info on enemy troop movement
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_5 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "黑进一台军方人事数据库！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析文件")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 解析完成")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 访问数据库")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 30 )
				elseif ( successpts > 20 ) then
					AddIntel( 20 )
				else
					AddIntel( 15 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "有了这些数据我们就能了解敌军动向！")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 检测到未授权访问")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 强制断开服务器连接")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... 请向系统管理员求助")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... 自动断开服务器连接")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end
		-- files on rebellious population (Dynamo etc.)
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_6 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "情报列举了Alma可能存在的对政府不满者。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "大部分居民支持女王，与Duncan家有龃龉。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "M.Duncan是Alma矿井的矿长而且无可替代。")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "他的兄弟G.Duncan，因为煽动民众骚乱被判无期徒刑，关在Tixa监狱。")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "为了稳定，Mr.Duncan被逐步架空。")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "没有找到其他报告。")
			end
		end
		
		-- We can gain experience/wisdom by hacking successfully.
		-- give us the reward if the computer was pristine and we've now been given the 
		if ( ( statebefore == ActionState.STATE_OK ) and (GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK) ) then
			rewardpts = difficulty / 6
			
			-- award experience
			GiveExp(ubID, StatTypes.EXPERAMT, rewardpts)
			GiveExp(ubID, StatTypes.WISDOMAMT, rewardpts)
		end
			
	elseif ( usActionType == ActionTypes.ACTIONTYPE_READFILE ) then
	
		-- if skill >= difficulty, we succeed
		successpts = skill - difficulty
		
		-- if we fail to understand this, tell us so
		if ( successpts < 0 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_RED, "你搞不清楚。")
			
			return
		end
		
		-- ORTA SUBLEVEL
		if ( sLuaactionid == ModSpecificActions.READING_ORTA_BASEMENT_BOOKSHELF_1 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "书架上放着许多化学，应用物理和机械学的教科书。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "你翻了翻，感到很无聊。")
			
		elseif ( sLuaactionid == ModSpecificActions.READING_ORTA_BASEMENT_BOOKSHELF_2 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这里好像研究火箭弹小型化。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "小巧的火箭枪即使普通步兵也能使用。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "改进了火箭弹的精准性能。")
			
		elseif ( sLuaactionid == ModSpecificActions.READING_ORTA_BASEMENT_BOOKSHELF_3 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "很多书做了生物特征加密。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "不管这里研究的是什么，有人就是要未经授权的人无法使用火箭枪。")
			
		-- ORTA
		elseif ( sLuaactionid == ModSpecificActions.READING_ORTA_BOOKSHELF_1 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "这些书是关于巨型食肉猫的。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "书上有厚厚的积尘，貌似很久没动过了。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "很奇怪。在这不研究血猫还能研究啥呢？")
			
		-- SAN Mona
		-- kingpin
		elseif ( sLuaactionid == ModSpecificActions.READING_SANMONA_KINGPIN_BOOKSHELF_1 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Kingpin在这里经营好多生意。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "他几乎控制了整个San Mona。")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "但是没人记录他的罪行。")
		end
	
	elseif ( usActionType == ActionTypes.ACTIONTYPE_MINIGAME ) then
	
		-- for now, the only game we have is tetris
		if ( sLuaactionid == InteractiveAction.MINIGAME_TETRIS ) then
		
			-- playing a game costs $1
			if ( SoldierSpendMoney(ubID, 1) == 1 ) then
			
				SetMiniGameType(MiniGames.TETRIS)
				SetPendingNewScreen(ScreenTypes.MINIGAME)
			
			end
		elseif ( sLuaactionid == InteractiveAction.MINIGAME_PONG ) then
		
			-- playing a game costs $1
			if ( SoldierSpendMoney(ubID, 1) == 1 ) then
			
				SetMiniGameType(MiniGames.PONG)
				SetPendingNewScreen(ScreenTypes.MINIGAME)
			
			end
		end
		
	elseif ( usActionType == ActionTypes.ACTIONTYPE_VARIOUS ) then
	
		if ( sLuaactionid == InteractiveAction.BILLIARD ) then
		
			PlaySound("Sounds\\billiard.wav")
			
		elseif ( sLuaactionid == InteractiveAction.TABLETENNIS ) then
		
			PlaySound("Sounds\\tabletennis.wav")
			
		elseif ( sLuaactionid == InteractiveAction.TOILET ) then
		
			PlaySound("Sounds\\toilet.wav")
		
		elseif ( sLuaactionid == InteractiveAction.TV ) then
		
			PlaySound("Sounds\\tvstatic.wav")
			
		elseif ( sLuaactionid == InteractiveAction.STEAL_CASHREGISTER ) then
		
			PlaySound("Sounds\\cashregister.wav")
		
		elseif ( sLuaactionid == InteractiveAction.PICTURE_NEONTHECLUB ) then
			
			-- The first argument is the path of a .png picture (any other format will be ignored).
			-- The second argument determines whether we stretch the picture to fullscreen (1) or not (0). If the picture is larger than our screen, we stretch it anyway.
			-- Only call this function from tactical (the call will be ignored otherwise).
			DisplayPictureTactical("Interface\\neonsign_theclub.png", 1)
			
		elseif ( sLuaactionid == InteractiveAction.PICTURE_NEONBEER ) then
			
			DisplayPictureTactical("Interface\\neonsign_beer.png", 0)
			
		elseif ( sLuaactionid == InteractiveAction.PICTURE_BEER ) then
			
			DisplayPictureTactical("Interface\\poster_beer1.png", 0)

		elseif ( sLuaactionid == InteractiveAction.PICTURE_2xBEER ) then
			
			DisplayPictureTactical("Interface\\poster_yellow_green_bottle.png", 0)
		
		elseif ( sLuaactionid == InteractiveAction.PICTURE_PRIVATE ) then
			
			DisplayPictureTactical("Interface\\kingpinclub_entrance.png", 0)
			
		elseif ( sLuaactionid == InteractiveAction.PICTURE_XXXPITSTOP ) then
			
			DisplayPictureTactical("Interface\\neonsign_xxx.png", 0)
			
		elseif ( sLuaactionid == InteractiveAction.PICTURE_DELIVERY ) then
			
			DisplayPictureTactical("Interface\\delivery.png", 0)
			
		elseif ( sLuaactionid == InteractiveAction.PICTURE_GENERALSTORE ) then
			
			DisplayPictureTactical("Interface\\sign_generalstore.png", 0)
			
		elseif ( sLuaactionid == InteractiveAction.PICTURE_SHADYLADY ) then
			
			DisplayPictureTactical("Interface\\shady_lady.png", 0)
			
		elseif ( sLuaactionid == InteractiveAction.COOK_COWMEAT_TO_STEAK ) then
						
			-- 1565 cow meat
			-- 1559 Steak
			
			if ( HasItemInInventory(ubID, 1565) ) then
			
				CreateItemInvOrFloor(ubID, 1559)
				
				DestroyOneItemInInventory(ubID, 1565)
				
				PlaySound("Sounds\\cooking.wav")
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Cooked a steak.")
				
			end
			
		elseif ( sLuaactionid == InteractiveAction.TAKE_FIRE_EXTINGUISHER_1 ) then
		
			-- 1761 fire extinguisher
			
			CreateItemInvOrFloor(ubID, 1761)
			
			-- due to the way structures are handled, we the decal is on a tile next to the one we are handling, so account for that
			-- the empty wall is in another tileset
			DestroyAndReplaceDecal(sGridNo - 1, "build_29.sti", 6)
			
		elseif ( sLuaactionid == InteractiveAction.TAKE_FIRE_EXTINGUISHER_2 ) then
		
			CreateItemInvOrFloor(ubID, 1761)
			
			-- due to the way structures are handled, we the decal is on a tile next to the one we are handling, so account for that
			-- the empty wall is in another tileset
			DestroyAndReplaceDecal(sGridNo - 160, "build_29.sti", 9)
		
		end
		
	end
	
	
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
	
	-- price is affected by player progress - on higher progress items get cheaper (as they are less useful at that point)
	progress = CurrentPlayerProgressPercentage()
	ratio = (200.0 - progress) / 100.0
	
	-- black market:
	-- guns
	AddArmsDealerAdditionalIntelDataItem(68, 337, 10 * ratio, 2)	-- Five-Seven
	AddArmsDealerAdditionalIntelDataItem(68, 663, 15 * ratio, 1)	-- HK G11 PDW
	AddArmsDealerAdditionalIntelDataItem(68, 1073, 10 * ratio, 2)	-- HK UCP
	AddArmsDealerAdditionalIntelDataItem(68, 673, 13 * ratio, 2)	-- HK MP7A1
	AddArmsDealerAdditionalIntelDataItem(68, 686, 20 * ratio, 1)	-- Metal Storm Surf Zone
	AddArmsDealerAdditionalIntelDataItem(68, 1191, 25 * ratio, 1)	-- Magpul PDR-D
	AddArmsDealerAdditionalIntelDataItem(68, 65, 40 * ratio, 1)		-- Auto Rocket Rifle
	AddArmsDealerAdditionalIntelDataItem(68, 1334, 27 * ratio, 1)	-- AR57 6" Silenced
	AddArmsDealerAdditionalIntelDataItem(68, 335, 40 * ratio, 1)	-- Barret M82A2
	AddArmsDealerAdditionalIntelDataItem(68, 676, 35 * ratio, 1)	-- HL SL9SD
	AddArmsDealerAdditionalIntelDataItem(68, 774, 50 * ratio, 1)	-- VSSK Vychlop
	AddArmsDealerAdditionalIntelDataItem(68, 605, 22 * ratio, 1)	-- AICW
	AddArmsDealerAdditionalIntelDataItem(68, 748, 22 * ratio, 2)	-- XM-29 OICW
	AddArmsDealerAdditionalIntelDataItem(68, 752, 20 * ratio, 1)	-- XM-8 Sharpshooter
	AddArmsDealerAdditionalIntelDataItem(68, 1065, 23 * ratio, 1)	-- Steyr AUG-A3
	AddArmsDealerAdditionalIntelDataItem(68, 680, 16 * ratio, 1)	-- Jackhammer Mk3A1
	
	-- Ammo
	AddArmsDealerAdditionalIntelDataItem(68, 1455, 8 * ratio, 2)	-- 5.7x28 100 AET
	AddArmsDealerAdditionalIntelDataItem(68, 1450, 8 * ratio, 2)	-- 4.7x33 100 SAP
	AddArmsDealerAdditionalIntelDataItem(68, 1460, 8 * ratio, 2)	-- 4.6x30 100 AET	
	AddArmsDealerAdditionalIntelDataItem(68, 544, 5 * ratio, 2)		-- 9mm MS 72 SAP
	AddArmsDealerAdditionalIntelDataItem(68, 1518, 10 * ratio, 2)	-- 6x35 100 AP
	AddArmsDealerAdditionalIntelDataItem(68, 113, 5 * ratio, 5)		-- Minirocket 5 HEAP
	AddArmsDealerAdditionalIntelDataItem(68, 1738, 10 * ratio, 2)	-- Minirocket 5 Cryo
	AddArmsDealerAdditionalIntelDataItem(68, 116, 5 * ratio, 4)		-- .50 BMG 10 AP
	AddArmsDealerAdditionalIntelDataItem(68, 540, 3 * ratio, 4)		-- 7.62x37 10 AP
	AddArmsDealerAdditionalIntelDataItem(68, 1512, 7 * ratio, 3)	-- 12.7x97 Subsonic 15 AP
	AddArmsDealerAdditionalIntelDataItem(68, 1520, 10 * ratio, 2)	-- 6.8x43 SPC 100 AP
	AddArmsDealerAdditionalIntelDataItem(68, 1423, 3 * ratio, 5)	-- 12 gauge 25 Flechette
	AddArmsDealerAdditionalIntelDataItem(68, 1578, 1 * ratio, 10)	-- Darts 1 Neurotoxin
	
	-- Launchers
	AddArmsDealerAdditionalIntelDataItem(68, 908, 20 * ratio, 1)	-- XM-25 Grenade Launcher
	AddArmsDealerAdditionalIntelDataItem(68, 1736, 40 * ratio, 1)	-- Strela-2
	
	-- Grenades
	AddArmsDealerAdditionalIntelDataItem(68, 957, 4 * ratio, 2)		-- 20mm Thermobaric Clip
	AddArmsDealerAdditionalIntelDataItem(68, 963, 4 * ratio, 2)		-- 25mm Thermobaric Clip
	AddArmsDealerAdditionalIntelDataItem(68, 970, 2 * ratio, 5)		-- 40mm Thermobaric Grenade
	AddArmsDealerAdditionalIntelDataItem(68, 978, 4 * ratio, 5)		-- Thermobaric Rocket
	AddArmsDealerAdditionalIntelDataItem(68, 987, 4 * ratio, 2)		-- 40mm MS Thermobaric Clip
	AddArmsDealerAdditionalIntelDataItem(68, 989, 4 * ratio, 2)		-- 20mm Napalm Clip
	AddArmsDealerAdditionalIntelDataItem(68, 990, 4 * ratio, 2)		-- 25mm Napalm Clip
	AddArmsDealerAdditionalIntelDataItem(68, 991, 1 * ratio, 5)		-- 40mm Napalm Grenade
	AddArmsDealerAdditionalIntelDataItem(68, 993, 4 * ratio, 2)		-- 40mm MS Napalm Clip
	AddArmsDealerAdditionalIntelDataItem(68, 1737, 6 * ratio, 4)	-- Strela-2 Missile
	
	-- Explosives
	AddArmsDealerAdditionalIntelDataItem(68, 951, 4 * ratio, 3)		-- 60mm Mustard Gas Shell
	AddArmsDealerAdditionalIntelDataItem(68, 1523, 3 * ratio, 3)	-- M18 Claymore
	AddArmsDealerAdditionalIntelDataItem(68, 1735, 3 * ratio, 4)	-- TM-62P2 anti-tank Mine
	
	-- Face items
	AddArmsDealerAdditionalIntelDataItem(68, 1025, 20 * ratio, 2)	-- NVG Gen IV
	
	-- Misc
	AddArmsDealerAdditionalIntelDataItem(68, 216, 6 * ratio, 3)		-- Compound 18
	AddArmsDealerAdditionalIntelDataItem(68, 235, 2 * ratio, 4)		-- Regeneration Booster
	AddArmsDealerAdditionalIntelDataItem(68, 324, 50 * ratio, 1)	-- X-Ray Detector
	AddArmsDealerAdditionalIntelDataItem(68, 327, 11 * ratio, 2)	-- Tank of gas
	AddArmsDealerAdditionalIntelDataItem(68, 592, 3 * ratio, 2)		-- Sniper Suppressor
	AddArmsDealerAdditionalIntelDataItem(68, 1026, 2 * ratio, 2)	-- Trigger Group
	AddArmsDealerAdditionalIntelDataItem(68, 1538, 1 * ratio, 5)	-- Stim
	AddArmsDealerAdditionalIntelDataItem(68, 1744, 7 * ratio, 2)	-- Ballistic Shield
	
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
	
	if ( usTargetProfile ~= NO_PROFILE ) then
	
		if ( usTargetProfile == Profil.GENERAL ) then
			
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GENERAL) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张将军照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_GENERAL, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.WARDEN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张典狱长照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_WARDEN, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.ERNEST ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张研究主管的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ERNEST, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.ARMAND ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Armand的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ARMAND, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.KINGPIN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Kingpin的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_KINGPIN, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.DARREN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Darren的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_DARREN, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.TONY ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Tony的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_TONY, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.JOE ) then
			
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Joe的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_JOE, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.MADLAB ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Kairns博士的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MADLAB, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.MIKE ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Mike的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MIKE, PhotoFlag.TAKEN )
			end
		
		elseif ( usTargetProfile == Profil.DEVIN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Devin的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_DEVIN, PhotoFlag.TAKEN )
			end
		
		elseif ( usTargetProfile == Profil.MICKY ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Micky的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MICKY, PhotoFlag.TAKEN )
			end
		
		elseif ( usTargetProfile == Profil.CARMEN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张Carmen的照片。")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_CARMEN, PhotoFlag.TAKEN )
			end
		
		end
		
	elseif ( bSectorZ == 0 ) then
	
		-- Cambria hospital morgue				
		if ( (sSectorY == SectorY.MAP_ROW_F) and (sSectorX == 8) ) then
		
			if ((room == 70) or (room == 71) or (room == 72)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张太平间照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Alma
		elseif ( (sSectorY == SectorY.MAP_ROW_H) and (sSectorX == 13) ) then
		
			if ((room == 12) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张指挥中心照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, PhotoFlag.TAKEN )
				
				end
				
			end
			
		elseif ( (sSectorY == SectorY.MAP_ROW_H) and (sSectorX == 14) ) then
		
			if ((room == 8) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张火箭弹仓库照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, PhotoFlag.TAKEN )
				
				end
				
			end
			
		elseif ( (sSectorY == SectorY.MAP_ROW_I) and (sSectorX == 13) ) then
		
			if ((room == 19) or (room == 20) or (room == 21)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张拷问室照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Tixa
		elseif ( (sSectorY == SectorY.MAP_ROW_J) and (sSectorX == 9) ) then
		
			if ((room == 19) or (room == 20) or (room == 21)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张拷问室照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Balime
		elseif ( (sSectorY == SectorY.MAP_ROW_L) and (sSectorX == 12) ) then
		
			if ((room == 37) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张博物馆最有价值展品的照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Grumm
		elseif ( (sSectorY == SectorY.MAP_ROW_G) and (sSectorX == 2) ) then
		
			if ((room == 1) or (room == 7) or (room == 8)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张危险品照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Meduna
		elseif ( (sSectorY == SectorY.MAP_ROW_N) and (sSectorX == 5) ) then
		
			if ((room == 17) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张王宫哨卡照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA, PhotoFlag.TAKEN )
				
				end
				
			end
			
		elseif ( (sSectorY == SectorY.MAP_ROW_N) and (sSectorX == 4) ) then
		
			if ((room == 1) or (room == 2) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张控制室照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Drassen SAM
		elseif ( (sSectorY == SectorY.MAP_ROW_D) and (sSectorX == 15) ) then
		
			if ((room == 2) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张控制室照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- Chitzena SAM
		elseif ( (sSectorY == SectorY.MAP_ROW_D) and (sSectorX == 2) ) then
		
			if ((room == 7) or (room == 8) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张控制室照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- central SAM
		elseif ( (sSectorY == SectorY.MAP_ROW_I) and (sSectorX == 8) ) then
		
			if ((room == 16) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张控制室照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, PhotoFlag.TAKEN )
				
				end
			
			end
			
		end
		
	elseif ( bSectorZ == 1 ) then
	
		-- Tixa
		if ( (sSectorY == SectorY.MAP_ROW_J) and (sSectorX == 9) ) then
		
			if ((room == 3) or (room == 9) or (room == 10) or (room == 12)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张非人环境的照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- Orta
		elseif ( (sSectorY == SectorY.MAP_ROW_K) and (sSectorX == 4) ) then
		
			if ((room == 13) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张储存试验性武器的照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, PhotoFlag.TAKEN )
				
				end
				
			elseif ((room == 11) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张制造流程照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, PhotoFlag.TAKEN )
				
				end
				
			elseif ((room == 16) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张实验室的实验照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- Meduna
		elseif ( (sSectorY == SectorY.MAP_ROW_P) and (sSectorX == 3) ) then
		
			if ((room == 24) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张地堡控制室照片。")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, PhotoFlag.TAKEN )
				
				end
				
			end
		
		end
		
	elseif ( bSectorZ == 3 ) then
		
		-- only lvl 3 sectors are creature sectors (apart from a secret sector most players will never know of :-) ). So any location will do.
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) < PhotoFlag.TAKEN ) then
			
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "找到一张照片 ... 就是这个。")
			
			SetPhotoState( ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, PhotoFlag.TAKEN )
		
		end
		
	end
	
end

function GetPhotoData( aType )

	if ( aType == PhotoFlag.TAKEN ) then
	
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GENERAL) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GENERAL, "确认Alma军事基地指挥官。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_WARDEN, "确认Tixa主管。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ERNEST, "确认Orta现任研究主管。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ARMAND, "找到Balime企业家。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_KINGPIN, "确认San Mona的非官方统治者 - 或者说黑帮头子？")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DARREN, "一张疑似'Kingpin'黑帮二把手的照片。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TONY, "与联系San Mona的军火贩子取得联系。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_JOE, "确认女王的私人安保头子。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MADLAB, "已联系上Orta研究所的前主管。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MIKE, "在女王的薪水单中确认有高级雇佣兵。")	end		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DEVIN, "找到一张炸药贩子的照片。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MICKY, "有人在酒吧中提到对动物器官感兴趣。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CARMEN, "和猎头公司取得搭上线。")	end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, "停满尸体的太平间照片。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, "确认Alma指挥中心控制室。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, "几张存有大量导弹的仓库照片。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, "几张军队监狱拷问室的照片。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, "已将Tixa审讯室建档。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, "照片显示在Tixa中有系统地拷问和杀戮。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, "我们遭遇到一些未知动物。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, "政府在一座机密场所中进行恐怖的实验。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, "这些照片展示一座存满实验性武器的仓库。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, "生产原型武器的偏远工厂。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, "这座博物馆中有件珍贵文物。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, "已将Grumm的非人工作环境建档。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA, "Meduna里有供人娱乐的决斗场。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, "已拿到王宫地堡控制室的照片。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, "已取得Drassen地对空导弹阵列控制中心的详细资料。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, "已取得Chitzena地对空导弹阵列控制中心的详细资料。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, "已取得Cambria地对空导弹阵列控制中心的详细资料。")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, "已取得Meduna地对空导弹阵列控制中心的详细资料。")	end
		
	elseif ( aType == PhotoFlag.VERIFIED ) then
	
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GENERAL) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GENERAL, "Theo Humphey将军是一位前美国军官。他负责Alma训练设施。其自身的军事知识很有价值。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GENERAL) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GENERAL, "Theo Humphey将军是一位前美国军官。他负责Alma训练设施。已阵亡。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_WARDEN, "Christine Woltz主管Tixa监狱。她掌握的政治犯信息是潜在的宝库。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_WARDEN, "Christine Woltz主管Tixa监狱。已阵亡。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ERNEST, "Ernest Poppin博士是Orta的现任研究主管。他掌握重要的医学知识。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ERNEST, "Orta实验室倒闭前Ernest Poppin博士曾是研究主管。人死了重要信息也没了。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ARMAND, "Armand Ricci，Ricci矿业公司的董事会成员，确认在Balime。Ricci矿业公司的区域影响力很大，有传言说该公司在Balime投资甚多。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ARMAND, "Ricci矿业公司董事会成员在不明情况下被杀。公司很快发表声明，谴责叛军的恐怖行径。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_KINGPIN, "Peter Klauss号令San Mona黑帮。他和Deidranna女王的关系未明有待进一步确认。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_KINGPIN, "Peter Klauss号令San Mona黑帮。他的身亡严重打击了黑帮。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DARREN, "Darren Van Haussen是Kinpin在San Mona据点的主管。他熟悉黑帮的组织结构。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DARREN, "Haussen先生是Kinpin的心腹。已死亡。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TONY, "'Tony'是个量大管够的军火贩子，藏在San Mona一家店铺的后身。尽管最初做出误判，但他不是'Kingpin'黑帮成员。他是否属于其他不明组织还有待确认, 但可能性很大。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TONY, "San Mona有个国际军火贩子。他的身亡让非正规组织更难以获取武器。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_JOE, "Joseph Papanus统领女王的私人卫队。极少流出的信息表明他是个极端暴力的人。由于嗜赌，他会主动寻找下家，只要给的钱够多。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_JOE, "Joseph Papanus是王宫安保头子。已阵亡。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MADLAB, "Nathaniel Kairns博士在出逃之前是Orta的研究主管。动机不明，但是他掌握着重要技术。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MADLAB, "曾经是Orta的项目主管，Nathaniel Kairns博士有丰富的技术资源。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MIKE, "确认前AIM雇佣兵'Mike'已经进入精锐部门。显然政府军已经雇佣外部军队。这会在雇佣兵界造成恐慌，AIM尤甚。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MIKE, "从AIM的退役后，'Mike'是王家部队的高级雇佣兵。他的死亡让政府军失去一个强力的军官。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DEVIN, "Devin Connell不只是个军火贩子。与军情五处的数据校正表明他与军情五处有勾当。具体细节已经披露，但是他现在的生意也许能给出线索。有时他也会客串一把雇佣兵。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DEVIN, "Devin Connell是专门贩卖炸药的军火商。有传言说他与爱尔兰共和军有关但是有待确认。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MICKY, "臭名昭著的军火贩子Micky O'Brien已经干了几十年。他已经转行倒卖珍稀保护动物器官给东南亚买主。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MICKY, "倒卖几十年军火，Micky O'Brien终于在Arulco栽了。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CARMEN, "Carmen Dancio是个赏金猎人。他最近在Arulco抓捕几名在国际刑警组织名单上的'头号通缉犯'。他为了抓捕目标不择手段。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CARMEN, "Carmen Dancio，一位经验丰富的赏金猎人，在进一步取得联系前被杀了。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, "几张当地医院太平间的照片。死者身上的大量致命伤印证了有计划的无差别屠杀。。。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, "几张当地医院太平间的照片。死者身上的大量致命伤印证了有计划的无差别屠杀。拍摄时医院已经脱离叛军控制，政府的责任心实在成问题。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, "军队总部指挥中心的高清照片还在研究中，但是已经给分析师提供很多信息。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, "Alma军队总部指挥中心的高清照片已经可以使用。但是实用性很成问题，反抗军已经占领那里。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, "Alma内一座仓库存有大量火箭弹。可能属于未知的炮兵部队所有。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, "Alma内一座仓库存有大量火箭弹。政府拒绝承担责任并指责叛军占据仓库,违背军火禁运令。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, "反抗军拿到军队监狱审讯室的照片。这为该国普遍的人权问题增加可信度。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, "反抗军声称照片展示的是一处军队监狱的审讯室。他们已经控制监狱，政府的罪恶已经暴露出来。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, "反对派声称独裁征服在那座臭名昭著的监狱中设置专门的拷问室并定期拷问。秘密拍摄的照片看着让人毛骨悚然。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, "反对派声称独裁征服在那座臭名昭著的监狱中设置专门的拷问室并定期拷问。政府否认并声称'明显是恐怖分子的摆拍，心智正常的人都不会相信'。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, "照片来自某个机密地区的偏远监狱，即使资深分析师也难以忍受。反对派设法送出的有限材料揭露了恶意地、肆意地破坏和对人类道德毫不掩饰地践踏。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, "反抗军的摄影师们将政府监狱中的严重人权问题记录下来。政府支持者否认指控并指出死者都是叛军所为，因为监狱是经过激战才被叛军占领。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, "科学家们对Arulco洞窟里巨大的和虫子类似的生物感到疑惑。等战斗平息后，会有一支探险队去做研究。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, "科学家们对Arulco洞窟里巨大的和虫子类似的生物感到疑惑。反抗军声称已经将其全部剿灭，这听起来就像个怪谈。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, "流出的照片显示，这座机密地区的实验室涉及囚犯，巨型猫科动物和辐射。老实说，还好咱们不是这层次的，但这也许不是好事。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, "反抗军经过激战占领实验室后放出照片并声称实验涉及活人，巨型食肉猫科动物和'放射性物质'。我们真的不知道该怎么处理。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, "政府的实力评估需要更正, 照片清晰地显示出仓库里存放大量某种发射火箭弹的突击步枪。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, "反对派声称在曾经的政府仓库中发现大量发射火箭弹的武器. 但是此处已不在政府控制中，所以并不能完全证明是真的。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, "根据最近流出的信息，有座地下工厂用于生产实验性武器系统.系统的确切性能没有披露出来，但可能是基于火箭弹。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, "根据反抗军传来的消息，他们找到一座曾受政府控制的武器工厂。令人疑惑的是反抗军在占领后又将工厂改作他用。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, "Arulco最珍贵的文物，圣杯，正在Balime展出。反对派抗议将圣杯从Chitzena废墟中转移出来的亵渎行为。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, "Arulco最珍贵的文物，圣杯，在Balime博物馆中被人盗走。目前圣杯下落不明，这是Arulco文明的又一次损失。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, "流出来的照片清晰展示了工业城市Grumm的非人工作环境。工人们使用受过辐射的设备却没有任何防护。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, "流出来的照片清晰展示了工业城市Grumm的非人工作环境。政府否认并将责任推给目前控制这一地区的反抗军。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA, "从Meduna辗转流出的照片展示了一作小角斗场, 完全符合罗马风格的娱乐，还有一座奢华的女王私人行宫。与这奢华形成鲜明对比的是遍布全国的赤贫人口。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, "在一次大胆的行动中，反抗军拿到几张王宫地堡的指挥室照片。那种患有妄想症一样的安保系统中包括可以主动激发的雷区。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, "在被反抗军占领前，王宫地堡控制室装有复杂的安保系统，包括雷区。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, "流出的照片展示了Drassen附近的地对空导弹阵列。导弹只能通过指挥控制台控制，是个明显的弱点。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, "反抗军最近攻占一处位于东北方的防空设施。可以推断其他地方也有类似的设施。")
		end
				
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, "流出的照片展示了Chitzena附近的地对空导弹阵列。紧贴着海岸的地方是个弱点，特别是没有海军陆战队。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, "反抗军最近攻占一处位于西北方的防空设施。可以推断其他地方也有类似的设施。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, "流出的照片展示了国土中央的地对空导弹阵列。尽管这处导弹覆盖大部分领空，但是东南部领空没有被覆盖。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, "反抗军经过激战拿下中央对空设施。除了数量庞大的守军，还有雷区和建筑内部的毒气系统。")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, "流出的照片展示了位于Meduna被重兵把守的地对空导弹阵列。导弹只能通过指挥控制台控制，是个明显的弱点。但是各导弹阵列以何种方式彼此连接尚不清楚。")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, "Meduna的对空设施落入反抗军手中。尚不清楚是否还有政府支持者的飞机残留。")
		end
		
	end
	
end

function VerifyPhotoState(aIndex)
	
	if ( (aIndex >= ModSpecificActions.PHOTO_FLAGS_BEGIN) and (aIndex <= ModSpecificActions.PHOTO_FLAGS_END) and (GetModderLUAFact(aIndex) == PhotoFlag.UPLOADED) ) then
		
		if (aIndex == ModSpecificActions.PHOTO_FLAGS_GENERAL) then
		
			if ( MercIsDead(Profil.GENERAL) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 15 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Theo Humphey将军收到RIS赏金。因为人死了所以削减奖金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 30 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Theo Humphey将军收到RIS赏金。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_WARDEN) then
		
			if ( MercIsDead(Profil.WARDEN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 7 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Christine Woltz收到RIS赏金。因为人死了所以削减奖金。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 15 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Christine Woltz收到RIS赏金。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ERNEST) then
		
			if ( MercIsDead(Profil.ERNEST) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Ernest Poppin博士收到RIS赏金。因为人死了所以削减奖金。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 30 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Ernest Poppin博士收到RIS赏金。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ARMAND) then
		
			if ( MercIsDead(Profil.ARMAND) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Armand Ricci收到RIS赏金。因为人死了所以削减奖金。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Armand Ricci收到RIS赏金。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_KINGPIN) then
		
			if ( MercIsDead(Profil.KINGPIN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Peter Klauss收到RIS赏金。因为其组织已经被摧毁所以奖金减少。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 25 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Peter Klauss收到RIS赏金。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_DARREN) then
		
			if ( MercIsDead(Profil.DARREN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 3 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Darren Van Haussen收到RIS赏金。因为其死亡导致信息失效所以奖金减少。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Darren Van Haussen收到RIS赏金。")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_TONY) then
		
			if ( MercIsDead(Profil.TONY) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Tony收到RIS赏金。因为人死了所以削减奖金。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Tony收到RIS赏金。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_JOE) then
		
			if ( MercIsDead(Profil.JOE) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Joseph Papanus收到RIS赏金。因为其死亡导致信息失效所以奖金减少。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Joseph Papanus收到RIS赏金。")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MADLAB) then
		
			if ( MercIsDead(Profil.MADLAB) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Nathaniel Kairns博士收到RIS赏金。因为其死亡导致有价值的资源中断所以奖金减少。")
			
			else
								
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 35 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Nathaniel Kairns博士收到RIS赏金。")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MIKE) then
		
			if ( MercIsDead(Profil.MIKE) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 5 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Mike收到RIS赏金。因为其死亡导致信息失效所以奖金减少。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Mike收到RIS赏金。")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_DEVIN) then
		
			if ( MercIsDead(Profil.DEVIN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Devin Connell收到RIS赏金。因为其死亡无法端掉其身后组织所以奖金减少。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Devin Connell收到RIS赏金。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MICKY) then
		
			if ( MercIsDead(Profil.MICKY) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Micky O'Brien收到RIS赏金。因为其死亡导致信息失效所以奖金减少。")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Micky O'Brien收到RIS赏金。")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_CARMEN) then
		
			if ( MercIsDead(Profil.CARMEN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Carmen Dancio收到RIS赏金。因为其死亡导致信息失效所以奖金减少。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为发现Carmen Dancio收到RIS赏金。")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) then
		
			if ( GetNumHostilesInSector(8, SectorY.MAP_ROW_F, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 14 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Cambria医院情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 7 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Cambria医院情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) then
		
			if ( GetNumHostilesInSector(13, SectorY.MAP_ROW_H, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Alma军队总部情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Alma军队总部情报收到RIS赏金。因政府军队调动导致信息过时所以奖金减少。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) then
		
			if ( GetNumHostilesInSector(14, SectorY.MAP_ROW_H, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Alma军用仓库情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Alma军用仓库情报收到RIS赏金。因政府军队调动导致信息过时所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) then
		
			if ( GetNumHostilesInSector(13, SectorY.MAP_ROW_I, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作军队监狱刑讯情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作军队监狱刑讯情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作监狱刑讯情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作监狱刑讯情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 25 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作监狱刑讯情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作监狱刑讯情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) then
		
			if ( CheckQuest(Quests.QUEST_CREATURES) == pQuest.QUESTDONE )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作未知生物巢穴情报收到RIS赏金。因全部剿灭无法研究所以奖金减少。")
				
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 40 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作未知生物巢穴情报收到RIS赏金。")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作秘密实验室的神秘实验情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作秘密实验室的神秘实验情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作大量武器储存情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作大量武器储存情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 14 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作武器生产线情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 6 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作武器生产线情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) then
		
			if ( CheckFact( Facts.FACT_CHALICE_STOLEN, 0 ) == true )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 7 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为获得Balime文物情报收到RIS赏金。因文物下落不明所以奖金减少。")
				
			else
				
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为获得Balime文物情报收到RIS赏金。")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) then
		
			if ( GetNumHostilesInSector(2, SectorY.MAP_ROW_G, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Grumm非安全工作环境情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Grumm非安全工作环境情报收到RIS赏金。因该区脱离政府控制不再追究罪名所以奖金减少。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) then
		
			SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
			AddIntel( 9 )					
			SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作供女王娱乐的角斗场情报收到RIS赏金。")
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) then
		
			if ( GetNumHostilesInSector(3, SectorY.MAP_ROW_P, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 30 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作王宫地堡情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作王宫地堡情报收到RIS赏金。因不再与战争有关所以奖金减少。")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) then
		
			if ( GetNumHostilesInSector(15, SectorY.MAP_ROW_D, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Drassen附近防空设施的软件硬件情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 6 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Drassen附近防空设施的软件硬件情报收到RIS赏金。因该区脱离政府控制导致情报过时所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) then
		
			if ( GetNumHostilesInSector(2, SectorY.MAP_ROW_D, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Chitzena附近防空设施的软件硬件情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 6 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Chitzena附近防空设施的软件硬件情报收到RIS赏金。因该区脱离政府控制导致情报过时所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) then
		
			if ( GetNumHostilesInSector(2, SectorY.MAP_ROW_D, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 14 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Cambria附近防空设施的软件硬件情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 7 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Cambria附近防空设施的软件硬件情报收到RIS赏金。因该区脱离政府控制导致情报过时所以奖金减少。")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) then
		
			if ( GetNumHostilesInSector(4, SectorY.MAP_ROW_N, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 16 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Meduna附近防空设施的软件硬件情报收到RIS赏金。")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "因为制作Meduna附近防空设施的软件硬件情报收到RIS赏金。因该区脱离政府控制导致情报过时所以奖金减少。")
			
			end
		
		end		
	
	end
	
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
	SKILLS_RADIO_ACTIVATE_TURNCOATS_ALL = 6,

	SKILLS_INTEL_CONCEAL = 7,
	SKILLS_INTEL_GATHERINTEL = 8,
	SKILLS_CREATE_TURNCOAT = 9,
	SKILLS_ACTIVATE_TURNCOATS = 10,
	SKILLS_ACTIVATE_TURNCOATS_ALL = 11,	
	
	-- various
	SKILLS_SPOTTER = 12,
	SKILLS_FOCUS = 13,
	SKILLS_DRAG = 14,
};

-- Look up voicelines in Dialogue Control.h, only listing notable ones often used here
Voiceline = 
{
	QUOTE_SEE_ENEMY = 0,
	QUOTE_HEARD_SOMETHING = 6,
	QUOTE_SPOTTED_SOMETHING_ONE = 11,
	QUOTE_SPOTTED_SOMETHING_TWO = 12,
	QUOTE_OUT_OF_AMMO = 13,
	QUOTE_TAKEN_A_BREATING = 21,							-- took a lot of hits this turn
	QUOTE_CLOSE_CALL = 22,									-- almost hit by a projectile
	QUOTE_NO_LINE_OF_FIRE = 23,								-- really bad chance to hit target
	QUOTE_KILLED_AN_ENEMY = 27,
	QUOTE_HEADSHOT = 33,
	QUOTE_STARTING_TO_WHINE = 40,							-- complain: low morale
	
	-- these three lines are played at random when we talk to one of our mercs. Despite the name they don't have any special meaning
	QUOTE_NEGATIVE_COMPANY = 41,
	QUOTE_SOCIAL_TRAIT = 44,
	QUOTE_PASSING_DISLIKE = 45,
	
	QUOTE_ENEMY_PRESENCE = 59,
	QUOTE_WARNING_OUTSTANDING_ENEMY_AFTER_RT = 60,
	QUOTE_SECTOR_SAFE = 65,
}

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
	ADE_SEXUALHARASSMENT = 23,				-- the player wants to take the 'strip' command further than we are comfortable with
	ADE_LOCKBOMB = 24,						-- merc uses a shaped charge to blow the locks of a door, aData1 indicates whether it was a success (1) or failure (0)
	ADE_SNIPERWARNING = 25,					-- merc warns of sniper presence in this sector
	ADE_BOMB_HAS_BEEN_PLANTED = 26,			-- we planted a bomb/mine/etc., aData1 is the item index, aData2 indicates whether it was a success (1) or failure (0)
}

-- functions used here:
-- ubProfile: profile number of merc (don't change that unless you know what to do)
-- iFaceIndex: index (don't change that unless you know what to do)
-- path: path and name of soundfile we want to play (in .wav, .ogg or .mp3 format)
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
	
	if ( ubProfile == Profil.BUNS_CHAOTIC ) then
	
		-- we can 'react' on what lines other people use
		-- this happens when we are in the same sector, awake and concious
		-- aData1 indicates the profile number in this event
		-- aData2 indicates the specific voice line we react to
		-- aData3 indicates whether this person has been recruited (1) or not(0)
		if ( usEventNr == DialogueActionEvent.ADE_DIALOGUE_REACTION ) then
				
			if ( aData1 == Profil.FRED ) then		

				if ( aData2 == 12 ) then			SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//nicebutharmless.MP3", "Pleasant fellow, I suppose, but too honest for his own good.")
				end
				
			elseif ( aData1 == Profil.ELLIOT ) then

				if ( aData2 == 0 ) then				SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//elliot.mp3", "Death doesn't seem to have kept him from talking.")
				end
				
			end
		
		-- similar to the rebel's quotes, this is called upon visiting a sector for the first time (if no rebel does their quote first)
		elseif ( usEventNr == DialogueActionEvent.ADE_SECTOR_COMMENTARY ) then
		
			-- surface sectors
			if ( bSectorZ == 0 ) then
			
				-- Alma base
				if ( (sSectorX == 13) and (sSectorY == SectorY.MAP_ROW_H) ) then		SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//almabase.mp3", "This must be where the savant has set up shop. Very impressive.")
				
				-- Hicks farm
				elseif ( (sSectorX == 10) and (sSectorY == SectorY.MAP_ROW_F) ) then	SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//hicksfarm.ogg", "Nothing but crude brutes. The savant would not ally with them... at least... I hope not.")
		
				end
				
			end
			
		-- aData1 is the itemnumber of the grenade
		elseif ( usEventNr == DialogueActionEvent.ADE_GRENADEWARNING ) then
			
			SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//grenadewarning.ogg", "Be careful, it would probably burn your legs right off!")
			
		-- someone died, aData1 is the profile of the dead NPC, aData2 the profile of the killer, aData3 the bodytype of the dead
		elseif ( usEventNr == DialogueActionEvent.ADE_NPC_DEATH ) then
			
			if ( aData2 == Profil.BUNS_CHAOTIC ) then						SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//friendlyfire.MP3", "So sorry. I slipped.")
			
			elseif ( (aData1 == Profil.BUZZ) or
					 (aData1 == Profil.MELTDOWN) or
					 (aData1 == Profil.ANNIE) or
					 (aData1 == Profil.MADAME) or
					 (aData1 == Profil.DOREEN) ) then

				 SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//likedfemaledeath.MP3", "Her blood will be avenged.")
			
			elseif ( (aData1 == Profil.REAPER) or
					 (aData1 == Profil.HAYWIRE) or
					 (aData1 == Profil.RAZOR) or
					 (aData1 == Profil.SLAY) or
					 (aData1 == Profil.CARMEN) or
					 (aData1 == Profil.JOE) or
					 (aData1 == Profil.CHRIS) or
					 (aData1 == Profil.KINGPIN) or
					 (aData1 == Profil.MIKE) or
					 (aData1 == Profil.SKITZ) ) then

					 SetAdditionalDialogue(ubProfile, iFaceIndex, "Speech//Additional//Buns_chaotic//likedmaledeath.MP3", "Very sad. But he was weak.")
			
			end
			
		end
		
	end

end

-- Look up voicelines in Dialogue Control.h, only listing notable ones often used here
Voiceline = 
{
	QUOTE_SEE_ENEMY = 0,
	QUOTE_KILLED_AN_ENEMY = 27,
	
	-- these three lines are played at random when we talk to one of our mercs. Despite the name they don't have any special meaning
	QUOTE_NEGATIVE_COMPANY = 41,
	QUOTE_SOCIAL_TRAIT = 44,
	QUOTE_PASSING_DISLIKE = 45,
	
	QUOTE_ENEMY_PRESENCE = 59,
	QUOTE_WARNING_OUTSTANDING_ENEMY_AFTER_RT = 60,
	QUOTE_SECTOR_SAFE = 65,
}

-- This function is used to replace dialogue quotes with new ones. This way we can have more variety - a merc doesn't always use the same lines when something happens.
-- If a line isn't replaced, it plays normally, so it is a good idea to not always replace lines.
--
-- functions used here:
-- path: path and name of soundfile we want to play (in .wav, .ogg or .mp3 format)
-- text: "Text that should appear"
-- SetReplaceDialogue(path, text)
-- Do not use this function in any lua calls outside of HandleReplaceQuote(..)!
--
-- handle
-- ubProfile is the merc for whom this is called
-- usQuoteNum is the quote that the game wants to play
function HandleReplaceQuote( ubProfile, usQuoteNum )

	

end

-- functions used here:
-- 
-- path: path and name of soundfile we want to play (in .wav, .ogg or .mp3 format)
-- volume: optional sound volume (65: medium volume, 127: loud), default 65
-- PlaySound(path, volume)

-- handle
-- sSectorX, sSectorY and bSectorZ indicate the sector coordinates
-- ubMerchantID is the ID of the merchant for whom this is called
-- ubBodyType is the character model the merchant uses
-- usQuoteNum is the event of the dialogue that is supposed to be called 
function HandleNPCMerchantQuote(sSectorX, sSectorY, bSectorZ, ubMerchantID, ubBodyType, usQuoteNum )

	

end