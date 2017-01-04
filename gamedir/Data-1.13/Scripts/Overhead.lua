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
	FACT_MARY_ALIVE = 187,
	FACT_FIRST_ROBOT_DESTROYED = 203,
    FACT_ROBOT_READY_SECOND_TIME = 205,
    FACT_SECOND_ROBOT_DESTROYED = 206,
	FACT_ESTONI_REFUELLING_POSSIBLE = 277,
	FACT_KINGPIN_KNOWS_MONEY_GONE = 103,
	FACT_PLAYER_REPAID_KINGPIN = 104,
	FACT_KINGPIN_NOT_IN_OFFICE = 256,

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
	MIGUEL = 57,
	CARLOS = 58,
	IRA = 59, 
	DIMITRI = 60,	
	SLAY = 64,
	DYNAMO = 66,
	SHANK = 67,
	CARMEN = 78,
	MARIA = 88,
	ANGEL = 89,
	JOEY = 90,	
	SKYRIDER = 97,
	JOHN = 118,	
	MARY = 119,
	JIM = 140,
	JACK = 141,
	OLAF = 142,
	RAY = 143,
	OLGA = 144,
	TYRONE = 145,
	MADLAB = 146,
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
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "If these videos were to be published, this might cause serious headache for the companies invovled.")
				
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
		-- hint on Dr. Kairns missing
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
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The main hallway is then flanked by the security offices. They seem to be heaily staffed.")
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
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "But the most prized item is definetely the Chalice of Chance .")
				
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
				
				-- give us info on troop movement
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				
				-- if we are really good, more data
				if ( successpts > 50 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				elseif ( successpts > 20 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
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
		-- Chitzena SAM: get info from the pc and use the comman console
		elseif ( sLuaactionid == ModSpecificActions.COMPUTER_CHITZENASAM_PC_1 ) then
			if ( GetModderLUAFact(sLuaactionid) == ActionState.STATE_OK ) then
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "We've compromised an army personnel database!")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decrypting files")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Decryption complete")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "... Accessing database")
				
				-- give us info on troop movement
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				
				-- if we are really good, more data
				if ( successpts > 50 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				elseif ( successpts > 20 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
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
				
				-- give us info on troop movement
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				
				-- if we are really good, more data
				if ( successpts > 50 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				elseif ( successpts > 20 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
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
				
				-- give us info on troop movement
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				
				-- if we are really good, more data
				if ( successpts > 50 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				elseif ( successpts > 20 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
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
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "There is a plan on the bunkers security measures here.")
				SetScreenMsg(FontColour.FONT_MCOLOR_DKWHITE, "The bunker two entries - one from the palace, one from an underground escape tunnel.")
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
				
				-- give us info on troop movement
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				
				-- if we are really good, more data
				if ( successpts > 40 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				elseif ( successpts > 10 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
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
				
				AddInfo( InfoTypes.INFO_TYPE_VIP)
				
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
				
				-- give us info on troop movement
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				
				-- if we are really good, more data
				if ( successpts > 50 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
				elseif ( successpts > 20 ) then
					AddInfo( InfoTypes.INFO_TYPE_NORMAL)
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
		if ( sLuaactionid == ModSpecificActions.MINIGAME_TETRIS ) then
		
			-- playing a game costs $1
			if ( SoldierSpendMoney(ubID, 1) == 1 ) then
			
				SetMiniGameType(MiniGames.TETRIS)
				SetPendingNewScreen(ScreenTypes.MINIGAME)
			
			end
		elseif ( sLuaactionid == ModSpecificActions.MINIGAME_PONG ) then
		
			-- playing a game costs $1
			if ( SoldierSpendMoney(ubID, 1) == 1 ) then
			
				SetMiniGameType(MiniGames.PONG)
				SetPendingNewScreen(ScreenTypes.MINIGAME)
			
			end
		end
				
	end
end

