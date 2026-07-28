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
			SetScreenMsg(FontColour.FONT_MCOLOR_RED, "... Can't reach host, automated disconnect.")
			
			return
		-- we cannot access this anymore, but gained something previously
		elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_END ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_RED, "... Can't reach host, automated disconnect.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There probably wasn't anything else of interest on there anway.")
			
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
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Unauthorised access detected")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Forcing disconnect from server")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Please contact your system administrator for guidance")
			SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
			
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_END)
			else
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end
			
			return
		-- if we just fail, give us a message
		elseif ( successpts < 0 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Wrong password.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Logging you out.")
			
			return
		end
		
		-- we need to remember this, so we know whether to give a reward later on
		statebefore = GetModderLUAFact(sLuaactionid)
		
		-- ORTA sublevel
		-- info on Ernest's research (rocket rifle)
		if ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB1_SERVER ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The schematics of the rocket rifles are stored in here:")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Rocket rifle: personal infantry weapon based around 20mm rockets.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Each rifle holds one clip, with each containing 5 missiles.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There are several rocket variants:")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Armour-piercing")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- High-Explosive")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- High-Explosive Armour-piercing.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Cryogenic")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Even against armoured infantry the wounds caused by this weapon can be described as 'overkill'.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The weapon can also be used in an anti-material role with moderate to good success.")
			end
		-- this server is connected to Alma, hacking it lowers difficulty there
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB2_SERVER ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You can still access the network.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This server is still connected to the Alma base network.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It will be difficult, but you try to install a backdoor.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Establishing connection to Alma mil-net")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Password: ********")		

				-- this is really, REALLY hard to do
				if ( successpts > 80 ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Password accepted.")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... cd ~")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... chmod 777 *")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... chmod: You have no permission for this command")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... sudo chmod 777 *")
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN,     "We successfully infiltrated the server, hacking in Alma base SAM will now be easier.")
					
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
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN,     "We were also able to reset some locked workstations.")
					end
					
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
					SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_END)
				else
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "We failed to hack the server")				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Wrong password.")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Logging you out.")
				end
			end	
		-- info on weapon tests
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_HALLWAY_SERVER ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Weapon test results are stored on this server.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "In later stages of the project, the weapon was tested on live inmates.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The tests were filmed... the cruel demonstrations are gruesome.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "If these videos were to be published, this might cause serious headache for the companies involved.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You do not want to see that again.")
			end
		-- Ernest's PC
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB1_PC ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You dig through Dr. Poppin's personal research.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It seems an even more advance version of the rocket rifle has been developed.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Several files seem to be missing... you can't find the weapon specifics.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The advanced version never developed further than the prototypes stage.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It is not specified where the prototypes are.")
			end
		-- money
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_LAB2_PC ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "A researcher is still logged in.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Perhaps you can divert some of the project's funds?")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing Project managing")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Identification: Poppins, E.")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Initiating money transfer")
				
				-- depending on how skilled we are, we get different amounts of money
				AddTransactionToPlayersBook(1, 0, 1800, successpts * 40)
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We've diverted project funds to our accounts!")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You play a bit of solitaire.")
			end
		-- hint on MADLAB missing
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_CONTROL_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "TO: ORTA HEAD OF SECURITY")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "FROM: ROYAL ADVISORY")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The absence of Dr. N. Kairns is simply unacceptable.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "As head of research, he cannot be replaced.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Your predecessor failed in properly securing him. Don't fail on returning him.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "That sounds rather harsh. This Dr. Kairns sounds interesting.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The remaining files seem rather boring.")
			end
		-- hint on secret weapons stash
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_CONTROL_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This base seems to have different levels of clearance.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Some of the storage rooms are heavily fortified.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Explosives likely won't be enough, these doors require a key code.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It is likely the lead scientists or guards might have the key codes.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The files don't say what exactly is stored here though.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You wonder what secrets this base might contain.")
			end
		-- info: some of the secret weapons got away
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_ENTRANCE_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "A log lists recent arrivals and departures to this base.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "For a long time, this base only received supplies.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "But recently, quite a few shipments were sent from here to Meduna.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The logs don't say what was shipped, but the destination was the royal guard HQ.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You have a bad feeling about this.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It's a UNIX system... I know this!")
			end
		-- warning on poison gas trap
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_BASEMENT_ENTRANCE_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You find a schematic of the bunker.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "In the next room is a small lab with toilet next to it.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Then there's the centrall hallway.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It seems there is some sort of gas trap installed in the hallway.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "On the left and right of it are two huge manufacturing halls.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The main hallway is then flanked by the security offices. They seem to be heavily staffed.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Behind another security door are the main labs, but you have no camera access.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There is nothing else on the console, you have to explore the base yourself.")
			end
		-- ORTA: bloodcats are studied here, so the computer has data on them 
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_SERVER_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The server contains a manifest of the last months.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "A lot of resources arrive here for such a small observation post.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "In fact, a lot more than you see. You wonder where all of this equipment went.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Something is not right here...")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You see lists upon lists of equipment that has no use in observing giant cats.")
			end
		-- ORTA: bloodcats are studied here, so the computer has data on them 
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ORTA_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This computer contains a lot of data on bloodcats physiology and behaviour.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It seems there is a huge pack of bloodcats in the east of Alma.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "In fact, the pack is so huge that this might become a security concern.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You find a lot of rare cat pictures, but that's about it. Nothing more to learn from this.")
			end
		-- BALIME: the museum security consoles tell us a bit about the museum
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_BALIME_L12_MUSEUM_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The museum security system hasn't been upgraded for a few years.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "All cameras are in the same network, with one simple on/off switch.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It is likely the switch will be in this room.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The cameras give you a view of the museum, but that's it.")
			end			
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_BALIME_L12_MUSEUM_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There is a list of all exhibits on the computer.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Most of it are old sculptures and bronze works.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "But the most prized item is definetely the Chalice of Chance.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Apart from the chalice, the museum doesn't seem to have anything of interest.")
			end
		-- Drassen SAM: get info from the pc and use the command console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_DRASSENSAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "We've compromised an army personnel database!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decrypting files")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decryption complete")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing database")
								
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We were able to learn of enemy movement thanks to the data!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Unauthorised access detected")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Forcing disconnect from server")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Please contact your system administrator for guidance")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_DRASSENSAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This seems to be the console that controls the SAM.")
				
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
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Someone already hacked this thing - and they broke it better than you could.")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We've messed up the file system - they will need an expert to get this running again.")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You attempt to unhack it.")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We are successful, the targetting software is working again.")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Whoever hacked the controls was more capable then we are - we cannot release the controls.")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The software seems to work just fine, better not mess with it.")
					end
				end	
			end
		-- Chitzena SAM: get info from the pc and use the comman console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CHITZENASAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "We've compromised an army personnel database!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decrypting files")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decryption complete")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing database")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We were able to learn of enemy movement thanks to the data!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Unauthorised access detected")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Forcing disconnect from server")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Please contact your system administrator for guidance")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CHITZENASAM_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Apparently, this is the computer of the commanding officer.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You are lucky. He forgot to log out when he left.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing Arulcan Army Financial Services")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Identification: Cpt. Gutierrez, A.")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Initiating money transfer")
				
				-- depending on how skilled we are, we get different amounts of money
				AddTransactionToPlayersBook(1, 0, 1800, successpts * 30)
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We've wired quite a bit of the commander's personal funds to our accounts!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The commander probably won't mind.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There is rather large connection of porn in here, but nothing you haven't seen before.")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CHITZENASAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This seems to be the console that controls the SAM.")
				
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
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Someone already hacked this thing - and they broke it better than you could.")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We've messed up the file system - they will need an expert to get this running again.")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You attempt to unhack it.")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We are successful, the targetting software is working again.")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Whoever hacked the controls was more capable then we are - we cannot release the controls.")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The software seems to work just fine, better not mess with it.")
					end
				end	
			end
		-- central SAM: get info from the pc and use the command console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "We've compromised an army personnel database!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decrypting files")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decryption complete")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing database")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We were able to learn of enemy movement thanks to the data!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Unauthorised access detected")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Forcing disconnect from server")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Please contact your system administrator for guidance")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There is a plan on the base' security measures here.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Apart from the garrison, there should be some extra mortar shells in some storage crates.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It also seems part of the compound contains a minefield!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There seems to be no way to deactivate it remotely.")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You should avoid the south of the building.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You keep looking, but there is no detailed plan on the minefield here.")
			end
		-- hacking this pc lowers hacking difficulty in other places
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_PC_3 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It seems all of the SAM sites are connected to the Meduna SAM.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Perhaps you can hack it remotely?")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Establishing connection to Meduna air control")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Password: ********")		

				-- depending on how good we are, this might work
				if ( successpts > 50 ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Password accepted.")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... cd ~")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... rm -rf *")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... rm: You have no permission for this command")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... sudo rm -rf *")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... ...")
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN,     "We successfully hacked the server, hacking in Meduna SAM will now be easier.")
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
					SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_END)
				else
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "We failed to hack the server")				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Unauthorised access detected")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Forcing disconnect from server")
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Please contact your system administrator for guidance")
					SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
					SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
				end
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CENTRALSAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This seems to be the console that controls the SAM.")
				
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
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Someone already hacked this thing - and they broke it better than you could.")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We've messed up the file system - they will need an expert to get this running again.")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You attempt to unhack it.")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We are successful, the targetting software is working again.")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Whoever hacked the controls was more capable then we are - we cannot release the controls.")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The software seems to work just fine, better not mess with it.")
					end
				end	
			end
		-- Meduna SAM: get info from the pc and use the command console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNASAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "We've compromised an army personnel database!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decrypting files")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decryption complete")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing database")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 15 )
				elseif ( successpts > 20 ) then
					AddIntel( 10 )
				else
					AddIntel( 5 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We were able to learn of enemy movement thanks to the data!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Unauthorised access detected")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Forcing disconnect from server")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Please contact your system administrator for guidance")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end	
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNASAM_COMPUTERBANK_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This station keeps track of all aircraft arriving in Meduna airport.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Cargo planes from foreign countries seem to arrive frequently.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It seems the army is significantly employing foreign mercenaries.")
			end		
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNASAM_COMMANDCONSOLE ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This seems to be the console that controls the SAM.")
				
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
						SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Someone already hacked this thing - and they broke it better than you could.")
					else
						SetSamSiteHackStatus(sSectorX, sSectorY, ourhack)
						
						SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We've messed up the file system - they will need an expert to get this running again.")
					end
				else
					if ( samhackstatus < 100 ) then					
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You attempt to unhack it.")
						
						if ( ourhack <= samhackstatus ) then
							SetSamSiteHackStatus(sSectorX, sSectorY, 100)
							
							SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We are successful, the targetting software is working again.")
						else
							SetScreenMsg(FontColour.FONT_MCOLOR_RED, "Whoever hacked the controls was more capable then we are - we cannot release the controls.")
						end
					else
						SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The software seems to work just fine, better not mess with it.")
					end
				end	
			end
		-- MEDUNA PALACE BUNKER
		-- entrance desktop contains a list of important people allied to the queen
		-- if you examine the names very closely, and also did so in Alma, you might learn something interesting here :-) (not yet implemented ingame)
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There are logs on who enters and leaves this bunker.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Quite a few dignitaries are listed here.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- V. Garcia, businessman")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Cmdr. A. Morris, Tracona military")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- D. Harrows, Harrows Clothing Inc.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- A. Ricci, Ricci Mining and Exploration")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Major E. Tavarez, Cali military")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- D. Van Haussen, businessman")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Col. T. Meyers, US military")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Major J.A. Vargas, PG PMC")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- Dr. E. Poppins, R&D")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Being able to link these people to the regime might become useful.")
			end
		-- desktop, control room
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "According to protocol, in the event of an emergency, the queen is to be moved to here.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The existence of the bunker isn't common knowledge, and heavily fortified.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You see no immediate weakpoints.")			
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "An occupying force will have to crush the entire garrison to take the bunker.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You quickly test your minesweeper skills. It's still top-notch!")
			end
		-- 
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There is a plan on the bunkers' security measures here.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The bunker has two entries - one from the palace, one from an underground escape tunnel.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "A huge part of the bunker is off-limits to all but the queen's personal staff.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "There seems to be an active minefield in the hallway.")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The controls should be around here somewhere.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "That minefield looks very nasty even from here.")
			end
		-- placeholder - until we have something more interesting, let's have this be a corrupted server
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_MEDUNAPALACE_SERVER_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing file.")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... File corruption detected.")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Ending session.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Whatever was on this server, the contents have been deleted. No way to make sense of it.")
			end		
		-- ALMA storage
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMASTORAGE_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The contents of the storage builings are listed in here.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 1x M14")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 1x FAMAS")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- several sets of armour")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- ammunition and grenades")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- experimental rocket ammo")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- several containers of fuel")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "- 9x ballistic missile")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Apart from the manifest, the computers contain nothing interesting.")
			end
			
		-- ALMA prison
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMAPRISON_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The computer contains personnel files.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The guard units seem to shift every few weeks.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "You are able to deduct several troop movements.")
								
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
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You've already downloaded the files.")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMAPRISON_PC_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This prison is used by the army for detention of high-value inmates.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It seems detentions are also carried out here.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You view some of the recordings. They are sickening.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Prisoners were regularly beaten, electrocuted, put on the rack...")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The list goes on and on.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "If these recordings were to be made public, the regime would get a lof of bad PR.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You've seen enough.")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMAPRISON_PC_3 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The prison schematics are on this computer.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The barracks and offices are on the left.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The mess hall is in the center.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The inmates are all held in the right section.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It seems the entire inmate block can be flooded with poison gas remotely!")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "The control switch is in a hallway leading from the offices to the mess hall.")
			end
		-- Alma base
		-- the computer of Sgt. Krott has data on the secret weapons project
		-- if you are good, you can retrieve the location of the project lead (a general)
		-- keep in mind that this computer will be destroyed if the bomb goes off
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_PC_KROTT ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You see files about some top-secret weapons project.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It seems to be about some kind of new infantry weapon.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "You are able to pinpoint the location of the military project lead!")
				
				AddIntel( 50 )
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You've already downloaded the files.")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_PC_BUREAU ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You access the personal files of a Gen. Humphey.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "He seems to be the head of the army training program.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "His personal file suggests he is an 'advisor' of the US government.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Leaking this information might cause some diplomatic embarassment.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You browse the internet and find two new memes.")
			end
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_PC_FILEROOM ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Quite a bit of pay was moved via this desktop.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "You attempt to divert some funds.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing Arulcan Army Financial Services")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Identification: Gen. Humphey, T.")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Initiating money transfer")
				
				-- depending on how skilled we are, we get different amounts of money
				AddTransactionToPlayersBook(1, 0, 1800, successpts * 60)
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We've wired quite a bit of the army's pay to our accounts!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It's not like they will need it much longer.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You search for more ways to wreak havoc, and post some witty comments on a messageboard.")
			end
		-- info on town layout
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You find a detailed of all military installations in Alma.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This base serves as a joint command/training facility.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "In the east there seems to be some sort of storage facility.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The army maintains it's own prison for high-value inmates in the south.")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The population houses in the southeast, near the mine.")	
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "If you could hold this base, this would be an excellent tra.")
			end
		-- warning that parts of the base blow up if you press the red switch
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_2 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Maintenance logs indicate that this base was recently upgraded.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It seems some sensitive material was recently delivered.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "A failsafe is rigged to blow them up, with the controls somewhere in this room.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Whatever the army is trying to protect is likely valuable.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "In your opinion, anyone boobytrapping their own base lacks a few screws.")
			end
		-- info in foreign mercs (Conrad, Iggy, Mike)
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_3 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This console has files on foreign mercenaries hired by the army.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Conrad Gillit, training supervisor")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Igmus Palkov, heavy weapons")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Mike [REDACTED], strike team CO")				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Perhaps we can track down these mercenaries and get them to work for us.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Apart from Conrad, Iggy and Mike, no other foreign mercenaries stand out.")
			end
		-- info on suspected rebels
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_4 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You find old intelligence reports. This is intel on key rebel personnel.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Cordona, M. - Kill on sight")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Dasouza, C. - Kill on sight")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Marquez, S. - Neutralised")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Vargas, J. - [REDACTED]")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Valenciano, C. - Neutralised")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Dominguez, R. - Neutralised")				
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Rivas, M. - Presumed exile")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Serrano, A. - Neutralised")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Guzzo, D. - Kill on sight")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Portela, N. - Neutralised")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Casilla, A. - Neutralised")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Smythe, I. - Kill on sight")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "It seems the army has already eliminated most of the rebels. Only a handful remain.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The list of rebel losses makes for a grim read.")
			end
		-- info on enemy troop movement
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_5 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "We've compromised an army personnel database!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decrypting files")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decryption complete")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing database")
				
				-- give us intel
				if ( successpts > 50 ) then
					AddIntel( 30 )
				elseif ( successpts > 20 ) then
					AddIntel( 20 )
				else
					AddIntel( 15 )
				end
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "We were able to learn of enemy movement thanks to the data!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Unauthorised access detected")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Forcing disconnect from server")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Please contact your system administrator for guidance")
				SetScreenMsg(FontColour.FONT_MCOLOR_RED,     "... Automated server disconnect")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_END)
			end
		-- files on rebellious population (Dynamo etc.)
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_ALMABASE_CONSOLE_6 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Intelligence reports lists possible malcontents in Alma.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "While most of the populationis loyal to the queen, severe friction with the Duncan family remains.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "M. Duncan is the head of the Alma mine and as such irreplacable in the mid-term.")
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "His brother, G. Duncan, serves a term in Tixa for life for civil commotion and instigation.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Steps have been taken to replace Mr. Duncan as head miner for greater stability.")
				
				SetModderLUAFact(sLuaactionid, ActionState.STATE_GAVEREWARD_OK)
			elseif ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_GAVEREWARD_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "No other reports draw your eye.")
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
			SetScreenMsg(FontColour.FONT_MCOLOR_RED, "You can't make sense of any of this.")
			
			return
		end
		
		-- ORTA SUBLEVEL
		if ( sLuaactionid == ModSpecificActions.READING_ORTA_BASEMENT_BOOKSHELF_1 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "This bookshelf contains many textbooks on chemistry, applied physics and mechanics.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "You read a bit, but quickly become bored.")
			
		elseif ( sLuaactionid == ModSpecificActions.READING_ORTA_BASEMENT_BOOKSHELF_2 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It seems there was some research done on miniaturizing rockets.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "A rocket rifle would be small enough to be wielded by an ordinary infantryman.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Much effort was done to improve the accuracy of these rockets.")
			
		elseif ( sLuaactionid == ModSpecificActions.READING_ORTA_BASEMENT_BOOKSHELF_3 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Quite a few books are about biometric encryption.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Whatever was researched here, someone wanted to make sure no one without clearance could use it.")
			
		-- ORTA
		elseif ( sLuaactionid == ModSpecificActions.READING_ORTA_BOOKSHELF_1 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The books contain extensive knowledge on giant predatory cats.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Dust has settled on the books. There aren't used much, it seems.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "That is odd. What else would you study here if not bloodcats?")
			
		-- SAN Mona
		-- kingpin
		elseif ( sLuaactionid == ModSpecificActions.READING_SANMONA_KINGPIN_BOOKSHELF_1 ) then
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Kingpin seems to log quite a few business transactions in here.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "It seems he controls almost all of San Mona.")
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "But none of the records explicitly state something criminal is going on.")
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
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the general.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_GENERAL, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.WARDEN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the head warden.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_WARDEN, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.ERNEST ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the head researcher.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ERNEST, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.ARMAND ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Armand.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ARMAND, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.KINGPIN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Kingpin.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_KINGPIN, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.DARREN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Darren.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_DARREN, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.TONY ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Tony.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_TONY, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.JOE ) then
			
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Joe.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_JOE, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.MADLAB ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Dr. Kairns.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MADLAB, PhotoFlag.TAKEN )
			end
			
		elseif ( usTargetProfile == Profil.MIKE ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Mike.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MIKE, PhotoFlag.TAKEN )
			end
		
		elseif ( usTargetProfile == Profil.DEVIN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Devin.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_DEVIN, PhotoFlag.TAKEN )
			end
		
		elseif ( usTargetProfile == Profil.MICKY ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Micky.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MICKY, PhotoFlag.TAKEN )
			end
		
		elseif ( usTargetProfile == Profil.CARMEN ) then
		
			if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) < PhotoFlag.TAKEN ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of Carmen.")
				SetPhotoState( ModSpecificActions.PHOTO_FLAGS_CARMEN, PhotoFlag.TAKEN )
			end
		
		end
		
	elseif ( bSectorZ == 0 ) then
	
		-- Cambria hospital morgue				
		if ( (sSectorY == SectorY.MAP_ROW_F) and (sSectorX == 8) ) then
		
			if ((room == 70) or (room == 71) or (room == 72)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the morgue.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Alma
		elseif ( (sSectorY == SectorY.MAP_ROW_H) and (sSectorX == 13) ) then
		
			if ((room == 12) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the command center.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, PhotoFlag.TAKEN )
				
				end
				
			end
			
		elseif ( (sSectorY == SectorY.MAP_ROW_H) and (sSectorX == 14) ) then
		
			if ((room == 8) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the missile storage.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, PhotoFlag.TAKEN )
				
				end
				
			end
			
		elseif ( (sSectorY == SectorY.MAP_ROW_I) and (sSectorX == 13) ) then
		
			if ((room == 19) or (room == 20) or (room == 21)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the torture room.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Tixa
		elseif ( (sSectorY == SectorY.MAP_ROW_J) and (sSectorX == 9) ) then
		
			if ((room == 19) or (room == 20) or (room == 21)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the torture room.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Balime
		elseif ( (sSectorY == SectorY.MAP_ROW_L) and (sSectorX == 12) ) then
		
			if ((room == 37) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the museum's most prized exhibition.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Grumm
		elseif ( (sSectorY == SectorY.MAP_ROW_G) and (sSectorX == 2) ) then
		
			if ((room == 1) or (room == 7) or (room == 8)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the dangerous materials handled here.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Meduna
		elseif ( (sSectorY == SectorY.MAP_ROW_N) and (sSectorX == 5) ) then
		
			if ((room == 17) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the royal lodge.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA, PhotoFlag.TAKEN )
				
				end
				
			end
			
		elseif ( (sSectorY == SectorY.MAP_ROW_N) and (sSectorX == 4) ) then
		
			if ((room == 1) or (room == 2) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the control room.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, PhotoFlag.TAKEN )
				
				end
				
			end
		
		-- Drassen SAM
		elseif ( (sSectorY == SectorY.MAP_ROW_D) and (sSectorX == 15) ) then
		
			if ((room == 2) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the control room.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- Chitzena SAM
		elseif ( (sSectorY == SectorY.MAP_ROW_D) and (sSectorX == 2) ) then
		
			if ((room == 7) or (room == 8) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the control room.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- central SAM
		elseif ( (sSectorY == SectorY.MAP_ROW_I) and (sSectorX == 8) ) then
		
			if ((room == 16) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the control room.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, PhotoFlag.TAKEN )
				
				end
			
			end
			
		end
		
	elseif ( bSectorZ == 1 ) then
	
		-- Tixa
		if ( (sSectorY == SectorY.MAP_ROW_J) and (sSectorX == 9) ) then
		
			if ((room == 3) or (room == 9) or (room == 10) or (room == 12)) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the inhuman conditions here.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- Orta
		elseif ( (sSectorY == SectorY.MAP_ROW_K) and (sSectorX == 4) ) then
		
			if ((room == 13) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the stored and packaged experimental weaponry.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, PhotoFlag.TAKEN )
				
				end
				
			elseif ((room == 11) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the manufacturing process.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, PhotoFlag.TAKEN )
				
				end
				
			elseif ((room == 16) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the experiments in the laboratories.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, PhotoFlag.TAKEN )
				
				end
				
			end
			
		-- Meduna
		elseif ( (sSectorY == SectorY.MAP_ROW_P) and (sSectorX == 3) ) then
		
			if ((room == 24) ) then
			
				if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) < PhotoFlag.TAKEN ) then
				
					SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of the bunker control room.")
					
					SetPhotoState( ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, PhotoFlag.TAKEN )
				
				end
				
			end
		
		end
		
	elseif ( bSectorZ == 3 ) then
		
		-- only lvl 3 sectors are creature sectors (apart from a secret sector most players will never know of :-) ). So any location will do.
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) < PhotoFlag.TAKEN ) then
			
			SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "Took a photo of... whatever THIS is.")
			
			SetPhotoState( ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, PhotoFlag.TAKEN )
		
		end
		
	end
	
end

function GetPhotoData( aType )

	if ( aType == PhotoFlag.TAKEN ) then
	
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GENERAL) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GENERAL, "Positive identification of the commanding officer of the Alma military base.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_WARDEN, "We've identified the director of Tixa.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ERNEST, "We've identified the current head of research in Orta.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ARMAND, "We've come across a wealthy industrialist in Balime.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_KINGPIN, "We've identified the unofficial leader of San Mona - some kind of crime syndicate leader?")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DARREN, "A photo of the possible second in command of the 'Kingpin' crime syndicate.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TONY, "Contact with an arms dealer in San Mona has been established.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_JOE, "Visual identification of the queen's personal head of security.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MADLAB, "We managed to contact the former head researcher of the Orta research lab.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MIKE, "Visual identification of a high-ranking mercenary in the queen's payroll.")	end		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DEVIN, "We took a photo of an arms dealer specialising in explosives.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MICKY, "A shady character in a bar mentioned an interest in animal parts.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CARMEN, "We made the acquaintance of a head hunter.")	end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, "Photos of a filled morgue.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, "Visuals detailing the control room of the Alma command center.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, "Images taken in a storage facility show a number of artillery missiles.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, "Images show a secret torture room in a military prison.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, "We've documented a dedicated interrogation room in Tixa.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) == PhotoFlag.TAKEN ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, "Pictures seem to indicate systematic torture and killings in Tixa.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, "We've come across some sort of... unknown animals.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, "Horrific experiments are undertaken in a secret government facility.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, "Images show a storage room full of experimental weaponry.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, "A remote facility is involved in the manufacturing of prototype weaponry.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, "A museum showcases an important artifact.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) == PhotoFlag.TAKEN ) then		SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, "We have documented drastically unsafe working conditions in Grumm.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA, "It seems there is some sort of entertainment arena in Meduna.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, "We managed to photograph the control room of the royal bunker.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, "We documented the control center of a surface to air battery near Drassen.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) == PhotoFlag.TAKEN ) then			SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, "We documented the control center of a surface to air battery near Chitzena.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, "We documented the control center of a surface to air battery near Cambria.")	end
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) == PhotoFlag.TAKEN ) then				SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, "We documented the control center of a surface to air battery in Meduna.")	end
		
	elseif ( aType == PhotoFlag.VERIFIED ) then
	
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GENERAL) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GENERAL, "General Theo Humphey is a former US officer. He is in charge of the Alma training facility. His intimate knowledge of the military could be valuable.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GENERAL) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GENERAL, "General Theo Humphey was a former US officer. He was in charge of the Alma training facility. KIA.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_WARDEN, "Christine Woltz commands the Tixa correctional facility. Her knowledge of political prisoners could be a potential information goldmine.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_WARDEN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_WARDEN, "Christine Woltz commanded the Tixa correctional facility. KIA.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ERNEST, "Dr. Ernest Poppin is the current head researcher of Orta. He might have knowledge of important interna.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ERNEST) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ERNEST, "Dr. Ernest Poppin was Orta head of research before the lab fell. It is likely classified information was lost with his death.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ARMAND, "Armand Ricci, board member of Ricci Mining Corp., was identified in Balime. Ricci mining is an important regional player, and rumoured to be heavily invested in Balime as well.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ARMAND) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ARMAND, "A board member of Ricci Mining Corp. was killed under unknwon circumstances. The company quickly issued a statement blaming the rebel uprising for this terrorist attack.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_KINGPIN, "Peter Klauss commands the powerful San Mona crime syndicate. The relation of his ship to Deidranna is not fully understood and should be investigated further.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_KINGPIN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_KINGPIN, "Peter Klauss led the San Mona syndicate. His death is likely to have shattered the syndicate.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DARREN, "Darren Van Haussen is in charge of Kingpin's main club in San Mona. It is likely he is familiar with the syndicate's structure.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DARREN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DARREN, "Mr. Haussen was a high-level subordinate of Kinpin. KIA.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TONY, "'Tony' is a well-stocked arms dealer operating in San Mona from behind a front shop. Though initially believed otherwise, he is not part of the 'Kingpin' syndicate. Whether he belongs to some other hitherto unknown organisation is unclear, but likely.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TONY) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TONY, "An international arms dealer operated in San Mona. His death will make it harder for irregular organisations to acquire weaponry.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_JOE, "Joseph Papanus commands the queen's personal forces. What sparse information remains paints the picture of a very violent man. Due to his gambling addiction, it is believed that he could be motivated to seek new employment if significant monetary rewards are offered.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_JOE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_JOE, "Joseph Papanus was head of royal security. KIA.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MADLAB, "Dr. Nathaniel Kairns was head researcher of Orta until he deserted. His motivation remains unclear, but he is likely to posess useful intel and skills.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MADLAB) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MADLAB, "Once the project lead of Orta, Dr. Nathaniel Kairns could have been a valuable source of intel.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MIKE, "The former AIM mercenary 'Mike' has been identified as part of an elite division. This is a clear indicator that the army has stepped employment of outside forces significantly. It will likely lead to some consternation among parts of the mercenary world, AIM in particular.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MIKE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MIKE, "A veteran AIM member, 'Mike' was a high-ranking mercenary in royal services. His death will have deprived the army of one of their most competent officers.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DEVIN, "Devin Connell is more than just an arms dealer. Data reconciliation with MI5 revealed that he was linked to MI5 in some capacity. The exact details are disclosed, but his current business conduct might give some insight. It is possible he may also freelance as a mercenary from time to time.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_DEVIN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_DEVIN, "Devin Connell was an arms dealer specialising in explosives. Rumours about a possible IRA connection could not be established or rejected.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MICKY, "The notorious arms dealer Micky O'Brien has been in the business for decades. He has switched to trading the rare animals parts of protected species for south-east asia buyers.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MICKY) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MICKY, "After decades of trading arms, Micky O'Brien finally ran out of luck in Arulco.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CARMEN, "Carmen Dancio is a bounty hunter. He is currently in Arulco on the hunt for several targets on Interpol's 'Most Wanted' list. He is rather unscrupulous in achieving his targets.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CARMEN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CARMEN, "A seasoned bounty hunter, Carmen Dancio, was killed before further contact could be established.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, "Several images taken detail a morgue in a local hospital. The often borderline excessive wounds on the deceased concur with a widespread campaign of indiscriminate killings..")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE, "Several images taken detail a morgue in a local hospital. The often borderline excessive wounds on the deceased concur with a widespread campaign of indiscriminate killings. As the hospital was no longer under rebel control when the images were taken, goverment accountability is in question.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, "High-definition photos of the army headquarter command center still have to be investigated painstakingly, but should deliver a wealth of information to analysts.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM, "High-definition pictures of the army headquarter command center in alma are available. The usefulness is put into question, however, as the rebels have already taken the facility.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, "An army storage depot in Alma houses a number of rockets. This could possibly lead to identification of as-of-yet unknown artillery pieces the army might possess.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS, "An army storage depot in Alma houses a number of rockets. The regime denies responsibility and blames the rebels, who indeed control the depot, for subverting an arms embargo.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, "Rebels obtained images of a dedicated torture room in a military prison. This gives further credence to claims of widespread human rights abuses in the country.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE, "Rebels claim images show a dedicated torture room in a military prison. As the facility is under their control, the culpability of the regime is put into question by outside parties.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, "Dissidents claim that torture is so regular in a notorious prison that the regime has set up dedicated rooms for it. Images taken in secret paint a chilling picture indeed.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE, "Dissidents claim that torture is so regular in a notorious prison that the regime has set up dedicated rooms for it. The regime denies this and states that this 'is an obvious set-up by a terrorist gane no-one in their right mind would believe'.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, "The images coming to us from a secret section of a remote prison are hard to stomach, even for veteran analysts. The limited material dissidents managed to smuggle outside reveals both malicious, wanton destruction and ignorance of even bare lip-service to human decency.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS, "Rebel photographers documented serious human rights abuses of staggering magnitude in a government prison. Regime sympathizers deny these allegations and point out that any loss of life is soley to blame on the rebels, who recently took the facility after fierce fighting.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, "Scientists are baffled by images showing large, insect-like creatures from the caves of Arulco. Once the fighting has dies down, an expedition will want to research this.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3, "Scientists are baffled by images showing large, insect-like creatures from the caves of Arulco. As the rebels claim to have eradicated them, this is likely some kind of obscure hoax.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, "Images smuggled out of laboratory at an undisclosed location show bizarre images involving prisoners, giant cats and huge amounts of irradiated goo. Frankly, we're out of our depth here, but this is probably not good.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_LABS, "After taking a laboratory in fierce fighting, rebels leaked images, claiming there were experiments done involving live humans, large predatory cats and 'radioactive stuff'. We're honestly not sure how to proceed.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, "Estimates of the regime's strength will likely have to be revised, as images clearly show huge numbers of some sort of missile-based assault rifle in storage.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE, "Dissidents claim a former government storage depot contained huge numbers of missile-based weaponry. As the facility is no longer under government control, this cannot be fully verfified.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, "According to recent leaks, an underground facility is involved in the creation of experimental weapon systems. The exact nature of said systems has yet to be disclosed, but seems missile-based.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING, "A formerly regime-controlled facility was involved in weapons manufacturing, according to rebel sources. This is put into doubt by the fact that rebels might have altered the facility after taking it.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, "Arulco's most important artifact, the Chalice of Chance, is on display in Balime. Dissidents protest against the sacrilege of removing it from the ruins of Chitzena in the first place.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM, "Arulco's most important artifact, the Chalice of Chance, was recently stolen from the Balime museum. The current whereabouts are unknown, this represents yet another loss for arulcan culture.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, "Leaked images show clearly unsafe working conditions in the industrial city of Grumm. Irradiated equipment is used by workers without any sort of protection.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE, "Leaked images show clearly unsafe working conditions in the industrial city of Grumm. The regime denies any culpability and puts the blame soley on the rebels, who currently administer the region.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA, "In a bizarre twist, leaked photography from Meduna shows a small arena, fit for roman-style entertainment, including a luxurious lodge for the queen herself. This lavish display is in stark contrast with the often impoverished population.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, "In a daring operation, rebels were able to obtain images of the command room of the queen's personal bunker. The paranoid security system prominently includes an active mine field.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER, "Before it was taken by the rebels, the queen's personal bunker control room housed elaborate security systems, including a mine field.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, "Images leaked to us show details of a surface to air missile battery near Drassen. It can only be controlled via a command console, making that an obvious weakpoint.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN, "Rebels detailed the control center of a recently occupied anti-air installation in the north-east. It can be presumed that other installations use similar hardware.")
		end
				
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, "Images leaked to us show details of a surface to air missile battery near chitzena. The immediate placement at the coast is an obvious weakpoint, especially considering the lack of marine forces.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA, "Rebels detailed the control center of a recently occupied anti-air installation in the north-west. It can be presumed that other installations use similar hardware.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, "Images leaked to us show details of a surface to air missile battery in the center of the country. While the placement of this installation yields overlapping air coverage on much of the country's airspace, this leaves the south-east of the country without any air coverage.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL, "The central anti-air installation was recently taken by rebels after fierce fighting. Apart from the sizeable garrison, it included mine-fields and internal gas deployment systems.")
		end
		
		if ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) == PhotoFlag.VERIFIED ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, "Images leaked to us show details of a heavily guarded surface to air missile battery in Meduna. It can only be controlled via a command console, making that an obvious weakpoint. It is unknown whether the different batteries are somehow linked to each other.")
		elseif ( GetModderLUAFact(ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) == PhotoFlag.VERIFIED_RESULT_2 ) then	SetPhotoFactLaptopData(aType, ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA, "The anti-air battery in Meduna itself has fallen to the rebels. It is unknown whether there are any loyalist aircraft left against whom this could be used.")
		end
		
	end
	
end

function VerifyPhotoState(aIndex)
	
	if ( (aIndex >= ModSpecificActions.PHOTO_FLAGS_BEGIN) and (aIndex <= ModSpecificActions.PHOTO_FLAGS_END) and (GetModderLUAFact(aIndex) == PhotoFlag.UPLOADED) ) then
		
		if (aIndex == ModSpecificActions.PHOTO_FLAGS_GENERAL) then
		
			if ( MercIsDead(Profil.GENERAL) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 15 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying General Theo Humphey. Amount lowered due to death.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 30 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying General Theo Humphey.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_WARDEN) then
		
			if ( MercIsDead(Profil.WARDEN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 7 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Christine Woltz. Amount lowered due to death.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 15 )
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Christine Woltz.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ERNEST) then
		
			if ( MercIsDead(Profil.ERNEST) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Dr. Ernest Poppin. Amount lowered due to death.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 30 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Dr. Ernest Poppin.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ARMAND) then
		
			if ( MercIsDead(Profil.ARMAND) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Armand Ricci. Amount lowered due to death.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Armand Ricci.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_KINGPIN) then
		
			if ( MercIsDead(Profil.KINGPIN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Peter Klauss. Amount lowered, as his organisation is effectively destroyed.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 25 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Peter Klauss.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_DARREN) then
		
			if ( MercIsDead(Profil.DARREN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 3 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Darren Van Haussen. Amount lowered as his death rendered the information obsolete.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Darren Van Haussen.")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_TONY) then
		
			if ( MercIsDead(Profil.TONY) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Tony. Amount lowered due to death.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Tony.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_JOE) then
		
			if ( MercIsDead(Profil.JOE) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Joseph Papanus. Amount lowered as his death rendered the information obsolete.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Joseph Papanus.")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MADLAB) then
		
			if ( MercIsDead(Profil.MADLAB) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Dr. Nathaniel Kairns. Amount lowered as his death removed a valuable source of intel.")
			
			else
								
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 35 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Dr. Nathaniel Kairns.")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MIKE) then
		
			if ( MercIsDead(Profil.MIKE) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 5 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Mike. Amount lowered as his death rendered the information obsolete.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Mike.")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_DEVIN) then
		
			if ( MercIsDead(Profil.DEVIN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Devin Connell. Amount lowered as his death lowers possible interest from disclosed organisations.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Devin.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MICKY) then
		
			if ( MercIsDead(Profil.MICKY) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Micky O'Brien. Amount lowered as his death rendered the information obsolete.")
				
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Micky O'Brien.")
				
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_CARMEN) then
		
			if ( MercIsDead(Profil.CARMEN) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Carmen Dancio. Amount lowered as his death rendered the information obsolete.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for identifying Carmen Dancio.")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_CAMBRIA_HOSPITAL_MORGUE) then
		
			if ( GetNumHostilesInSector(8, SectorY.MAP_ROW_F, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 14 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the conditions in Cambria hospital.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 7 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the conditions in Cambria hospital. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ALMA_HQ_CONTROLROOM) then
		
			if ( GetNumHostilesInSector(13, SectorY.MAP_ROW_H, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the Alma military headquarter.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the Alma military headquarter. Amount lowered as informatino is now outdated due to the ousting of regime troops.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ALMA_STORAGE_ROCKETS) then
		
			if ( GetNumHostilesInSector(14, SectorY.MAP_ROW_H, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the contents of a military depot in Alma.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the contents of a military depot in Alma. Amount lowered as information is now outdated due to the ousting of regime troops.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ALMA_PRISON_TORTURE) then
		
			if ( GetNumHostilesInSector(13, SectorY.MAP_ROW_I, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting torture in a military prison.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting torture in a military prison. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting torture in a prison.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting torture in a prison. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_TIXA_TORTURE_DOWNSTAIRS) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 25 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting torture in a prison.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting torture in a prison. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_CREATURE_LVL3) then
		
			if ( CheckQuest(Quests.QUEST_CREATURES) == pQuest.QUESTDONE )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting a nest of unknown creatures. Amount lowered as they have all been eradicated, making research impossible.")
				
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 40 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting a nest of unknown creatures.")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ORTA_LABS) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 15 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting bizarre experiments in secret lab.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting bizarre experiments in secret lab. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ORTA_STORAGE) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for recording huge amounts of weaponry in army storage.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for recording huge amounts of weaponry in army storage. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_ORTA_MANUFACTURING) then
		
			if ( GetNumHostilesInSector(9, SectorY.MAP_ROW_J, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 14 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting a weapons manufacturing line.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 6 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting a weapons manufacturing line. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_BALIME_MUSEUM) then
		
			if ( CheckFact( Facts.FACT_CHALICE_STOLEN, 0 ) == true )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 7 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for confirming a valuable artifact was in Balime. Amount lowered as its whereabouts are no longer known.")
				
			else
				
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 20 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for confirming a valuable artifact is in Balime.")
				
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_GRUMM_RADIOACTIVE) then
		
			if ( GetNumHostilesInSector(2, SectorY.MAP_ROW_G, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 10 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting unsafe working conditions in Grumm.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 4 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting unsafe working conditions in Grumm. Amount lowered as sector is no longer government-controlled, culpability for any crimes no longer undeniable.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MEDUNA_ARENA) then
		
			SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
			AddIntel( 9 )					
			SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the existence of a private arena for the queen's entertainment.")
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_MEDUNA_BUNKER) then
		
			if ( GetNumHostilesInSector(3, SectorY.MAP_ROW_P, 1) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 30 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the layout of the royal bunker.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the layout of the royal bunker. Amount lowered as this is no longer relevant to the war effort.")
			
			end
			
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_DRASSEN) then
		
			if ( GetNumHostilesInSector(15, SectorY.MAP_ROW_D, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation near Drassen.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 6 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation near Drassen. Amount lowered as sector is no longer government-controlled and thus the information is outdated.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_CHITZENA) then
		
			if ( GetNumHostilesInSector(2, SectorY.MAP_ROW_D, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 12 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation near Chitzena.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 6 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation near Chitzena. Amount lowered as sector is no longer government-controlled and thus the information is outdated.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_CENTRAL) then
		
			if ( GetNumHostilesInSector(2, SectorY.MAP_ROW_D, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 14 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation near Cambria.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 7 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation near Cambria. Amount lowered as sector is no longer government-controlled and thus the information is outdated.")
			
			end
		
		elseif (aIndex == ModSpecificActions.PHOTO_FLAGS_SAM_MEDUNA) then
		
			if ( GetNumHostilesInSector(4, SectorY.MAP_ROW_N, 0) )  then
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED )				
				AddIntel( 16 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation in Meduna.")
			
			else
			
				SetPhotoState( aIndex, PhotoFlag.VERIFIED_RESULT_2 )				
				AddIntel( 8 )					
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Received RIS compensation for documenting the hard- and software of an anti-air installation in Meduna. Amount lowered as sector is no longer government-controlled and thus the information is outdated.")
			
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