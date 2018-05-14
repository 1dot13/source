--[[

This script has two functions "HandleQuestCodeOnSectorEntry" and "HandleQuestCodeOnSectorExit".
First function is executed when merc enter to the sector and second function is executed 
when merc leave from the sector.

]]

Items = {
	MONEY = 219,
}

SectorY = {
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

EventTypes = {
	EVENT_SET_BY_NPC_SYSTEM = 27,
}

qStatus = {
	QUESTNOTSTARTED =0,
	QUESTINPROGRESS = 1,
	QUESTDONE = 2,
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
	QUEST_KINGPIN_ANGEL_MARIA = 26,
	
	-- max Quests 254
}

Facts = {
	FACT_JOEY_ESCORTED = 108,
	FACT_MARIA_ESCORTED = 116,
	FACT_ANGEL_LEFT_DEED = 120,
	FACT_CHALICE_STOLEN = 184,
	FACT_MUSEUM_ALARM_WENT_OFF = 278,
	FACT_KINGPIN_KNOWS_MONEY_GONE = 103,
	FACT_KINGPIN_DEAD = 308,
	FACT_KINGPIN_IS_ENEMY = 359,
	FACT_ALL_TERRORISTS_KILLED   =      156,
	FACT_BOUNTYHUNTER_SECTOR_1 = 380,
	FACT_BOUNTYHUNTER_SECTOR_2 = 381,
	FACT_BOUNTYHUNTER_KILLED_1 = 382,
	FACT_BOUNTYHUNTER_KILLED_2 = 383,
}

History = {
	HISTORY_ENTERED_HISTORY_MODE = 0,
	HISTORY_HIRED_MERC_FROM_AIM = 1,
	HISTORY_HIRED_MERC_FROM_MERC = 2,
	HISTORY_MERC_KILLED = 3,
	HISTORY_SETTLED_ACCOUNTS_AT_MERC = 4,
	HISTORY_ACCEPTED_ASSIGNMENT_FROM_ENRICO = 5,
	HISTORY_CHARACTER_GENERATED = 6,
	HISTORY_PURCHASED_INSURANCE = 7,
	HISTORY_CANCELLED_INSURANCE = 8,
	HISTORY_INSURANCE_CLAIM_PAYOUT = 9,
	HISTORY_EXTENDED_CONTRACT_1_DAY = 10,
	HISTORY_EXTENDED_CONTRACT_1_WEEK = 11,
	HISTORY_EXTENDED_CONTRACT_2_WEEK = 12,
	HISTORY_MERC_FIRED = 13,
	HISTORY_MERC_QUIT = 14,
	HISTORY_QUEST_STARTED = 15,
	HISTORY_QUEST_FINISHED = 16,
	HISTORY_TALKED_TO_MINER = 17,
	HISTORY_LIBERATED_TOWN = 18, 
	HISTORY_CHEAT_ENABLED = 19,
	HISTORY_TALKED_TO_FATHER_WALKER = 20,
	HISTORY_MERC_MARRIED_OFF = 21,
	HISTORY_MERC_CONTRACT_EXPIRED = 22,
	HISTORY_RPC_JOINED_TEAM = 23,
	HISTORY_ENRICO_COMPLAINED = 24,
	HISTORY_WONBATTLE = 25,
	HISTORY_MINE_RUNNING_OUT = 26,
	HISTORY_MINE_RAN_OUT = 27,
	HISTORY_MINE_SHUTDOWN = 28,
	HISTORY_MINE_REOPENED = 29,
	HISTORY_DISCOVERED_TIXA = 30,
	HISTORY_DISCOVERED_ORTA = 31,
	HISTORY_GOT_ROCKET_RIFLES = 32,
	HISTORY_DEIDRANNA_DEAD_BODIES = 33,
	HISTORY_BOXING_MATCHES = 34,
	HISTORY_SOMETHING_IN_MINES = 35,
	HISTORY_DEVIN = 36,
	HISTORY_MIKE = 37,
	HISTORY_TONY = 38,
	HISTORY_KROTT = 39,
	HISTORY_KYLE = 40,
	HISTORY_MADLAB = 41,
	HISTORY_GABBY = 42,
	HISTORY_KEITH_OUT_OF_BUSINESS = 43,
	HISTORY_HOWARD_CYANIDE = 44,
	HISTORY_KEITH = 45,
	HISTORY_HOWARD = 46,
	HISTORY_PERKO = 47,
	HISTORY_SAM = 48,
	HISTORY_FRANZ = 49,
	HISTORY_ARNOLD = 50,
	HISTORY_FREDO = 51,
	HISTORY_RICHGUY_BALIME = 52,
	HISTORY_JAKE = 53,
	HISTORY_BUM_KEYCARD = 54,
	HISTORY_WALTER = 55,
	HISTORY_DAVE = 56,
	HISTORY_PABLO = 57,
	HISTORY_KINGPIN_MONEY = 58,
	HISTORY_WON_BOXING = 59,
	HISTORY_LOST_BOXING = 60,
	HISTORY_DISQUALIFIED_BOXING = 61,
	HISTORY_FOUND_MONEY = 62,
	HISTORY_ASSASSIN = 63,
	HISTORY_LOSTTOWNSECTOR = 64,
	HISTORY_DEFENDEDTOWNSECTOR = 65,
	HISTORY_LOSTBATTLE = 66,
	HISTORY_FATALAMBUSH = 67,
	HISTORY_WIPEDOUTENEMYAMBUSH = 68,
	HISTORY_UNSUCCESSFULATTACK = 69,
	HISTORY_SUCCESSFULATTACK = 70,
	HISTORY_CREATURESATTACKED = 71,
	HISTORY_KILLEDBYBLOODCATS = 72,
	HISTORY_SLAUGHTEREDBLOODCATS = 73,
	HISTORY_NPC_KILLED = 74,
	HISTORY_GAVE_CARMEN_HEAD = 75,
	HISTORY_SLAY_MYSTERIOUSLY_LEFT = 76,
	HISTORY_MERC_KILLED_CHARACTER = 77,
}

Profil = 
{
	MIGUEL = 57,
	CARLOS = 58,
	IRA = 59,
	DIMITRI = 60,
	ROBOT = 62,
	DYNAMO = 66,
	SHANK = 67,
	CONRAD = 70,
	DARREL = 73,
	QUEEN = 75,
	CARMEN = 78,
	KINGPIN = 86,
	MARIA = 88,
	ANGEL = 89,
	KYLE = 95,
	FATIMA = 101,
	YANNI = 108,
	MARTHA = 109,
	JAKE = 113,
	ELDIN = 127,
	ELLIOT = 135,
	MADLAB = 146,
}

SoldierClass = 
{                   
    SOLDIER_CLASS_NONE = 0,
	SOLDIER_CLASS_ADMINISTRATOR = 1,
	SOLDIER_CLASS_ELITE = 2,
	SOLDIER_CLASS_ARMY = 3,
}

CivGroup =
{
	REBEL_CIV_GROUP = 1,
	KINGPIN_CIV_GROUP = 2,
	WARDEN_CIV_GROUP = 11,
	BOUNTYHUNTER_CIV_GROUP = 25,
	SCIENTIST_GROUP = 27,
	RADAR_TECHNICIAN_GROUP = 28,
	AIRPORT_STAFF_GROUP = 29,
	BARRACK_STAFF_GROUP = 30,
	FACTORY_GROUP = 31,
	ADMINISTRATIVE_STAFF_GROUP = 32,
	LOYAL_CIV_GROUP = 33,
	BLACKMARKET_GROUP = 34,
}

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

Skin = 
{
	PINKSKIN = 0,
	TANSKIN = 1,
	DARKSKIN = 2,
	BLACKSKIN = 3,
}

Hair = 
{
	WHITEHEAD = 0,
	BLACKHEAD = 1,
	BROWNHEAD = 2,
	BLONDEHEAD = 3,
	REDHEAD = 4,
}

Vest = 
{
	WHITEVEST = 0,
	GYELLOWSHIRT = 1,
	YELLOWVEST = 2,
	GREYVEST = 3,
	BROWNVEST = 4,
	PURPLESHIRT = 5,
	BLUEVEST = 6,
	JEANVEST = 7,
	GREENVEST = 8,
	REDVEST = 9,
	BLACKSHIRT = 10,
}

Pants = 
{
	BLUEPANTS = 0,
	BLACKPANTS = 1,
	JEANPANTS = 2,
	TANPANTS = 3,
	BEIGEPANTS = 4,
	GREENPANTS = 5,
}

local gsRobotGridNo

function HandleQuestCodeOnSectorEntry( sNewSectorX, sNewSectorY, bNewSectorZ )

	if ( sNewSectorX == 6 and sNewSectorY == SectorY.MAP_ROW_C and gubQuest( Quests.QUEST_RESCUE_MARIA ) == qStatus.QUESTDONE 
	and  not( gubQuest( Quests.QUEST_KINGPIN_ANGEL_MARIA ) == qStatus.QUESTINPROGRESS) ) then
		-- make sure Maria and Angel are gone
		SetCharacterSectorX(Profil.MARIA, 0)
		SetCharacterSectorY(Profil.MARIA, 0)
		SetCharacterSectorX(Profil.ANGEL, 0)
		SetCharacterSectorY(Profil.ANGEL, 0)
	end
	
	if ( sNewSectorX == 6 and sNewSectorY == SectorY.MAP_ROW_C and gubQuest( Quests.QUEST_LEATHER_SHOP_DREAM ) == qStatus.QUESTDONE ) then
		-- move Kyle to his shop
		SetCharacterSectorX(Profil.KYLE, 6)
		SetCharacterSectorY(Profil.KYLE, SectorY.MAP_ROW_C)
		SetProfileStrategicInsertionData(Profil.KYLE, 12380)
	end

	if ( sNewSectorX == 5 and sNewSectorY == SectorY.MAP_ROW_D ) then
		ResetBoxers()
	end

	if ( sNewSectorX == 3 and sNewSectorY == SectorY.MAP_ROW_P ) then
	
		-- heal up Elliot if he's been hurt
		if ( CheckNPCLife (Profil.ELLIOT) < GetNPCLifeMax (Profil.ELLIOT) ) then
			SetNPCLife (Profil.ELLIOT, GetNPCLifeMax(Profil.ELLIOT))
		end
	end
	
	if ( CheckFact( Facts.FACT_ALL_TERRORISTS_KILLED, 0 ) == true ) then
		SetCharacterSectorX(Profil.CARMEN, 0)
		SetCharacterSectorY(Profil.CARMEN, 0)
		SetCharacterSectorZ(Profil.CARMEN, 0)
	end
	--[[
	if ( CheckFact( Facts.FACT_ROBOT_RECRUITED_AND_MOVED, 0 ) == false ) then
		pRobot = FindSoldierByProfileID (Profil.ROBOT)
		if (pRobot) then
			-- robot is on our team and we have changed sectors, so we can
			-- replace the robot-under-construction in Madlab's sector
			--RemoveGraphicFromTempFile( gsRobotGridNo, SEVENTHISTRUCT1, GetCharacterSectorX(Profil.MADLAB), GetCharacterSectorY(Profil.MADLAB), GetCharacterSectorZ(Profil.MADLAB) )
			SetFactTrue( Facts.FACT_ROBOT_RECRUITED_AND_MOVED )
		end
	end
	]]
end

function HandleQuestCodeOnSectorExit( sOldSectorX, sOldSectorY, bOldSectorZ )

	if ( sOldSectorX == iniKINGPIN_MONEY_SECTOR_X and sOldSectorY == iniKINGPIN_MONEY_SECTOR_Y and bOldSectorZ == iniKINGPIN_MONEY_SECTOR_Z ) then
		CheckForKingpinsMoneyMissing( true )
	end

	if ( sOldSectorX == 13 and sOldSectorY == SectorY.MAP_ROW_H and bOldSectorZ == 0 and CheckFact( Facts.FACT_CONRAD_SHOULD_GO,0 ) == true) then
		-- remove Conrad from the map
		SetCharacterSectorX(Profil.CONRAD, 0)
		SetCharacterSectorY(Profil.CONRAD, 0)
	end

	if ( sOldSectorX == iniHOSPITAL_SECTOR_X and sOldSectorY == iniHOSPITAL_SECTOR_Y and bOldSectorZ == iniHOSPITAL_SECTOR_Z ) then
		CheckForMissingHospitalSupplies()
	end

	-- reset the state of the museum alarm for Eldin's quotes
	SetFactFalse( Facts.FACT_MUSEUM_ALARM_WENT_OFF )
	
end

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

-- We have an array of 1000 signed integers that a modder can use to set whatever data he wants.
-- We simply set up some enums here to make it easier for us to remember what is what
ModSpecificFacts =
{
	TIXA_PRISON_VOLUNTEERSGAINED = 123,
	TIXA_PRISON_SUBLEVEL_VOLUNTEERSGAINED = 124,
	ALMA_PRISON_VOLUNTEERSGAINED = 125,
}

-- this function is called whenever we liberate a sector. If fFirstTime is true, this is the first time we liberate this sector
function HandleSectorLiberation( sNewSectorX, sNewSectorY, bNewSectorZ, fFirstTime )

	-- are we liberating this sector for the first time?
	if ( fFirstTime ) then
		-- we get a few volunteers for liberating prisons - we assume prisoners would be more than eager to fight against the regime
		-- due to code limitations, fFirstTime is only used in surface sectors
		if ( bNewSectorZ == 0 ) then
			-- Tixa
			if ( sNewSectorX == 9 and sNewSectorY == SectorY.MAP_ROW_J ) then
				AddVolunteers( 30 )
				
				AddTransactionToPlayersBook(1, 0, 1800, 100)
				
				if ( GetUsedLanguage( nil ) == Languages.LANGUAGE_ENGLISH ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "The prisoners are very grateful for freeing them.")
				elseif ( GetUsedLanguage( nil ) == Languages.LANGUAGE_GERMAN ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "Die Gefangenen sind für die Befreiugng sehr dankbar.")
				else
					SetScreenMsg(FontColour.FONT_MCOLOR_DKRED, "Translation missing!")
				end
				
				-- inform us that there is a sublevel
				if ( (GetModderLUAFact(ModSpecificFacts.TIXA_PRISON_SUBLEVEL_VOLUNTEERSGAINED) == 0) ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "This prison seems to have a sublevel, where more important inmates are held.")
				end
				
				-- if we haven't yet freed the Alma prisoners, give us a tip about that
				if ( (GetModderLUAFact(ModSpecificFacts.ALMA_PRISON_VOLUNTEERSGAINED) == 0) ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "A few inmates inform us that there is another prison like this in Alma!")
				end
				
				AddIntel( 5 )
				
				SetModderLUAFact(ModSpecificFacts.TIXA_PRISON_VOLUNTEERSGAINED, 1)
				
			-- Alma
			elseif ( sNewSectorX == 13 and sNewSectorY == SectorY.MAP_ROW_I ) then
				AddVolunteers( 10 )
				
				SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "The prisoners are very grateful for freeing them.")
												
				-- if we haven't yet freed the Tixa prisoners, give us a tip about that
				if ( (GetModderLUAFact(ModSpecificFacts.TIXA_PRISON_VOLUNTEERSGAINED) == 0) ) then
					SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "A few inmates inform us that there is another prison like this in a place called Tixa. They aren't sure where it is though.")
				end
				
				AddIntel( 5 )
				
				SetModderLUAFact(ModSpecificFacts.ALMA_PRISON_VOLUNTEERSGAINED, 1)
			end
		end
	end
	
	-- extra volunteers on freeing Tixa underground. As enemy troops are not replenished underground, this only happens once
	if ( bNewSectorZ == 1 ) then
		-- Tixa
		if ( sNewSectorX == 9 and sNewSectorY == SectorY.MAP_ROW_J ) then
			AddVolunteers( 5 )
			
			SetScreenMsg(FontColour.FONT_MCOLOR_LTGREEN, "The prisoners are very grateful for freeing them.")
			
			AddIntel( 5 )
			
			SetModderLUAFact(ModSpecificFacts.TIXA_PRISON_SUBLEVEL_VOLUNTEERSGAINED, 1)
		end
	end
end

-- this function is called whenever we recruit a RPC
function RecruitRPCAdditionalHandling( usProfile )

	-- as the rebels join us, we gradually get more volunteers
	if ( usProfile == Profil.MIGUEL ) then
		AddVolunteers( 4 )
		AddIntel( 10 )
	elseif ( usProfile == Profil.CARLOS ) then
		AddVolunteers( 2 )
	elseif ( usProfile == Profil.DIMITRI ) then
		AddVolunteers( 2 )
		AddIntel( 2 )
	elseif ( usProfile == Profil.IRA ) then
		AddVolunteers( 2 )
		AddIntel( 5 )
	end
end

-- this function is called whenever we enter a sector in tactical
function HandleSectorTacticalEntry( sSectorX, sSectorY, bSectorZ, fHasEverBeenPlayerControlled )
	
	if ( gubQuest( Quests.QUEST_KINGPIN_ANGEL_MARIA ) == qStatus.QUESTINPROGRESS ) then
	
		-- Flugente: if the bounty hunter quest is active, add bounty hunters to sectors (location determined on quest start)
		if ( (CheckFact( Facts.FACT_BOUNTYHUNTER_KILLED_1, 0 ) == false) or (CheckFact( Facts.FACT_BOUNTYHUNTER_KILLED_2, 0 ) == false) and (bSectorZ == 0) ) then
			
			sector 		   = SECTOR(sSectorX, sSectorY)
			sector_hunter1 = GetFact( Facts.FACT_BOUNTYHUNTER_SECTOR_1 )
			sector_hunter2 = GetFact( Facts.FACT_BOUNTYHUNTER_SECTOR_2 )
			
			if ( (sector == sector_hunter1) and (CheckFact( Facts.FACT_BOUNTYHUNTER_KILLED_1, 0 ) == false)  ) then
				
				hostile = 0
				if ( (CheckFact( Facts.FACT_BOUNTYHUNTER_KILLED_2, 0 ) == true)  ) then
					hostile = 1
				end
				
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ADMINISTRATOR, 	13000, hostile)
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ARMY, 			7447,  hostile)
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ARMY, 			13032, hostile)
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 			19291, hostile)
				
				-- dont spawn in deep water
				if (sSectorX == 14 and sSectorY == 2) then
					CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 			16533, hostile)
				else
					CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 			13557, hostile)
				end
			end
			
			if ( (sector == sector_hunter2) and (CheckFact( Facts.FACT_BOUNTYHUNTER_KILLED_2, 0 ) == false) ) then
				
				hostile = 0
				if ( (CheckFact( Facts.FACT_BOUNTYHUNTER_KILLED_1, 0 ) == true)  ) then
					hostile = 1
				end
				
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ADMINISTRATOR, 	13000, hostile)
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ARMY, 			7447,  hostile)
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ARMY, 			13032, hostile)
				CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 			19291, hostile)
				
				-- dont spawn in deep water
				if (sSectorX == 14 and sSectorY == 2) then
					CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 			16533, hostile)
				else
					CreateArmedCivilain(CivGroup.BOUNTYHUNTER_CIV_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 			13557, hostile)
				end
			end
		end
		
	end
	
	-- Flugente: if this sector has not yet been liberated by the player, there might be some civilian enemy personnel here
	-- the idea is that these people are government employed, and won't stay around once you take the sector
	-- parameters of CreateCivilian:
	-- - tile where person should be created on the map
	-- - civilian group they should belong to (see also CivGRoupNames.xml)
	-- - number of the merchant the civilian should be (-1 for no merchant)
	-- - bodytype
	-- - vest colour (-1 for random)
	-- - pants colour (-1 for random)
	-- - hair colour (-1 for random)
	-- - skin colour (-1 for random)
	-- - optional item 1 (-1 for nothing)
	-- - optional item 2 (-1 for nothing)
	-- - optional item 3 (-1 for nothing)
	-- - optional item 4 (-1 for nothing
	
	if ( fHasEverBeenPlayerControlled == false ) then
		-- surface sectors
		if ( bSectorZ == 0 ) then
			-- central SAM
			if ( sSectorX == 8 and sSectorY == SectorY.MAP_ROW_I ) then
				CreateCivilian(16081, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.MANCIV, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 635, -1, -1)
				CreateCivilian(15443, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.REGFEMALE, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 635, -1, -1)
				CreateCivilian(10944, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.REGMALE, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 8, -1, -1)
			-- Drassen SAM
			elseif ( sSectorX == 15 and sSectorY == SectorY.MAP_ROW_D ) then
				CreateCivilian(11297, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.MANCIV, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 8, -1, -1)
			-- chitzena SAM
			elseif ( sSectorX == 2 and sSectorY == SectorY.MAP_ROW_D ) then
				CreateCivilian(10675, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.REGFEMALE, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 635, -1, -1)
				CreateCivilian(9718, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.REGMALE, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 8, -1, -1)
			-- Meduna SAM
			elseif ( sSectorX == 4 and sSectorY == SectorY.MAP_ROW_N ) then
				CreateCivilian(12071, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.REGFEMALE, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 635, -1, -1)
				CreateCivilian(12554, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.REGMALE, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 635, -1, -1)
				CreateCivilian(15085, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.MANCIV, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 8, -1, -1)
			-- Orta surface
			elseif ( sSectorX == 4 and sSectorY == SectorY.MAP_ROW_K ) then
				CreateCivilian(16065, CivGroup.SCIENTIST_GROUP, -1, Bodytype.DRESSCIV, Vest.WHITEVEST, Pants.BEIGEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(13991, CivGroup.SCIENTIST_GROUP, -1, Bodytype.MANCIV, Vest.WHITEVEST, Pants.BEIGEPANTS, -1, -1, -1, -1, -1, -1)
			-- Meduna airport
			elseif ( sSectorX == 3 and sSectorY == SectorY.MAP_ROW_N ) then
				CreateCivilian(12224, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.JEANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(11439, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.REGMALE, Vest.BROWNVEST, Pants.JEANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(11451, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.REGFEMALE, Vest.BROWNVEST, Pants.JEANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(13529, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.REGMALE, Vest.BROWNVEST, Pants.JEANPANTS, -1, -1, -1, -1, -1, -1)
			-- Drassen airport
			elseif ( sSectorX == 13 and sSectorY == SectorY.MAP_ROW_B ) then
				CreateCivilian(11719, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.JEANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(8054, CivGroup.RADAR_TECHNICIAN_GROUP, -1, Bodytype.MANCIV, Vest.GREYVEST, Pants.GREENPANTS, -1, -1, 210, 8, -1, -1)
				CreateCivilian(5990, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.REGFEMALE, Vest.BROWNVEST, Pants.JEANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(4562, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.REGMALE, Vest.BROWNVEST, Pants.JEANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(8112, CivGroup.AIRPORT_STAFF_GROUP, -1, Bodytype.FATCIV, Vest.BROWNVEST, Pants.JEANPANTS, Hair.WHITEHEAD, -1, -1, -1, -1, -1)
			-- Grumm factory
			elseif ( sSectorX == 2 and sSectorY == SectorY.MAP_ROW_H ) then
				CreateCivilian(12051, CivGroup.FACTORY_GROUP, -1, Bodytype.KIDCIV, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9337, CivGroup.FACTORY_GROUP, -1, Bodytype.HATKIDCIV, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9988, CivGroup.FACTORY_GROUP, -1, Bodytype.KIDCIV, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10165, CivGroup.FACTORY_GROUP, -1, Bodytype.HATKIDCIV, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10287, CivGroup.FACTORY_GROUP, -1, Bodytype.DRESSCIV, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
			-- Grumm factory
			elseif ( sSectorX == 2 and sSectorY == SectorY.MAP_ROW_G ) then
				CreateCivilian(11777, CivGroup.SCIENTIST_GROUP, -1, Bodytype.REGMALE, Vest.WHITEVEST, Pants.BEIGEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10019, CivGroup.FACTORY_GROUP, -1, Bodytype.REGFEMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(18944, CivGroup.FACTORY_GROUP, -1, Bodytype.REGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(12543, CivGroup.FACTORY_GROUP, -1, Bodytype.MANCIV, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
			-- Tixa
			elseif ( sSectorX == 9 and sSectorY == SectorY.MAP_ROW_J ) then
				CreateCivilian(9058, CivGroup.ADMINISTRATIVE_STAFF_GROUP, -1, Bodytype.MINICIV, Vest.PURPLESHIRT, Pants.BLUEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9992, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.REGFEMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(15895, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(12056, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.REGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(17043, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
			-- Alma prison
			elseif ( sSectorX == 13 and sSectorY == SectorY.MAP_ROW_I ) then
				CreateCivilian(10618, CivGroup.ADMINISTRATIVE_STAFF_GROUP, -1, Bodytype.DRESSCIV, Vest.PURPLESHIRT, Pants.BLUEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(12893, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.REGFEMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(11594, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(13514, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.REGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(10309, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(8223, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.REGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
			-- Alma warehouse
			elseif ( sSectorX == 14 and sSectorY == SectorY.MAP_ROW_H ) then
				CreateCivilian(20235, CivGroup.FACTORY_GROUP, -1, Bodytype.REGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10612, CivGroup.FACTORY_GROUP, -1, Bodytype.REGFEMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9656, CivGroup.FACTORY_GROUP, -1, Bodytype.REGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(12421, CivGroup.FACTORY_GROUP, -1, Bodytype.BIGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10334, CivGroup.FACTORY_GROUP, -1, Bodytype.REGFEMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(16374, CivGroup.FACTORY_GROUP, -1, Bodytype.BIGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
			-- Alma barracks
			elseif ( sSectorX == 13 and sSectorY == SectorY.MAP_ROW_H ) then
				CreateCivilian(11132, CivGroup.ADMINISTRATIVE_STAFF_GROUP, -1, Bodytype.MINICIV, Vest.PURPLESHIRT, Pants.BLUEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(11913, CivGroup.ADMINISTRATIVE_STAFF_GROUP, -1, Bodytype.DRESSCIV, Vest.PURPLESHIRT, Pants.BLUEPANTS, Hair.WHITEHEAD, -1, -1, -1, -1, -1)
				CreateCivilian(6793, CivGroup.BARRACK_STAFF_GROUP, -1, Bodytype.REGMALE, Vest.GREENVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9993, CivGroup.BARRACK_STAFF_GROUP, -1, Bodytype.REGFEMALE, Vest.GREENVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(12874, CivGroup.BARRACK_STAFF_GROUP, -1, Bodytype.BIGMALE, Vest.GREENVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9680, CivGroup.BARRACK_STAFF_GROUP, -1, Bodytype.REGMALE, Vest.GREENVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9688, CivGroup.BARRACK_STAFF_GROUP, -1, Bodytype.MANCIV, Vest.GREENVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(12086, CivGroup.BARRACK_STAFF_GROUP, -1, Bodytype.REGMALE, Vest.GREENVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(7942, CivGroup.BARRACK_STAFF_GROUP, -1, Bodytype.REGFEMALE, Vest.GREENVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
			-- Balime
			elseif ( sSectorX == 11 and sSectorY == SectorY.MAP_ROW_L ) then
				CreateCivilian(15951, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, 195, -1, -1, -1)
				CreateCivilian(17692, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(12546, CivGroup.LOYAL_CIV_GROUP, -1,-1, -1, -1, -1, -1, 195, -1, -1, -1)
				CreateCivilian(13826, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(13350, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(13491, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(16708, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, 195, -1, -1, -1)
				CreateCivilian(12264, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(13227, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
			-- Balime
			elseif ( sSectorX == 12 and sSectorY == SectorY.MAP_ROW_L ) then
				CreateCivilian(12599, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(11134, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10508, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10503, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, 195, -1, -1, -1)
				CreateCivilian(12102, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(14830, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(15297, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, 195, -1, -1, -1)
				CreateCivilian(16414, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9976, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1)
				CreateCivilian(14792, CivGroup.LOYAL_CIV_GROUP, -1, -1, -1, -1, -1, -1, 195, -1, -1, -1)
			end
		-- sublevel 1
		elseif ( bSectorZ == 1 ) then
			-- Orta
			if ( sSectorX == 4 and sSectorY == SectorY.MAP_ROW_K ) then
				CreateCivilian(14494, CivGroup.ADMINISTRATIVE_STAFF_GROUP, -1, Bodytype.MINICIV, Vest.PURPLESHIRT, Pants.BLUEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(13062, CivGroup.SCIENTIST_GROUP, -1, Bodytype.MANCIV, Vest.WHITEVEST, Pants.BEIGEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(13167, CivGroup.SCIENTIST_GROUP, -1, Bodytype.FATCIV, Vest.WHITEVEST, Pants.BEIGEPANTS, Hair.WHITEHEAD, -1, -1, -1, -1, -1)
				CreateCivilian(12048, CivGroup.SCIENTIST_GROUP, -1, Bodytype.REGMALE, Vest.WHITEVEST, Pants.BEIGEPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(9827, CivGroup.FACTORY_GROUP, -1, Bodytype.BIGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(10007, CivGroup.FACTORY_GROUP, -1, Bodytype.REGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
				CreateCivilian(17211, CivGroup.FACTORY_GROUP, -1, Bodytype.BIGMALE, Vest.YELLOWVEST, Pants.TANPANTS, -1, -1, -1, -1, -1, -1)
			-- Tixa
			elseif ( sSectorX == 9 and sSectorY == SectorY.MAP_ROW_J ) then
				CreateCivilian(12875, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.REGFEMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(12096, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(7287, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.REGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(5843, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.BIGMALE, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
				CreateCivilian(5378, CivGroup.WARDEN_CIV_GROUP, -1, Bodytype.MANCIV, Vest.BROWNVEST, Pants.BLACKPANTS, -1, -1, 213, 298, 1625, -1)
			end
		end
	end
	
	if ( bSectorZ == 0 ) then
		-- Drassen
		if ( sSectorX == 13 and sSectorY == SectorY.MAP_ROW_D) then
			if ( GetTownLoyaltyRating(2) > 50 ) then
				-- wine store
				CreateCivilian(17198, 0, 55, Bodytype.REGFEMALE, -1, -1, -1, -1, -1, -1, -1, -1)
			end
			-- general store
			CreateCivilian(10943, 0, 41, Bodytype.MANCIV, -1, -1, -1, -1, -1, -1, -1, -1)
		-- Alma
		elseif ( sSectorX == 14 and sSectorY == SectorY.MAP_ROW_I) then
			-- wine store
			CreateCivilian(8530, 0, 56, Bodytype.MANCIV, Vest.GREYVEST, -1, -1, -1, -1, -1, -1, -1)
			-- general store - both traders use the same inventory
			CreateCivilian(13839, 0, 42, Bodytype.FATCIV, Vest.YELLOWVEST, Pants.BLUEPANTS, -1, -1, -1, -1, -1, -1)
			CreateCivilian(12878, 0, 42, Bodytype.DRESSCIV, Vest.YELLOWVEST, Pants.BLUEPANTS, -1, -1, -1, -1, -1, -1)
		-- Cambria
		elseif ( sSectorX == 9 and sSectorY == SectorY.MAP_ROW_F) then
			-- drug dealer (university ruins)
			CreateCivilian(10032, 0, 60, Bodytype.HATKIDCIV, -1, -1, -1, -1, -1, -1, -1, -1)
		elseif ( sSectorX == 9 and sSectorY == SectorY.MAP_ROW_G) then
			-- wine store
			CreateCivilian(17657, 0, 57, Bodytype.MANCIV, Vest.JEANVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
			CreateCivilian(16055, 0, 57, Bodytype.REGFEMALE, Vest.JEANVEST, Pants.GREENPANTS, -1, -1, -1, -1, -1, -1)
			-- pharmacy
			CreateCivilian(11627, 0, 47, Bodytype.FATCIV, Vest.WHITEVEST, -1, Hair.WHITEHEAD, -1, -1, -1, -1, -1)
			-- restaurant
			CreateCivilian(15630, 0, 49, Bodytype.MANCIV, Vest.GYELLOWSHIRT, Pants.BEIGEPANTS, -1, -1, -1, -1, -1, -1)
			CreateCivilian(15321, 0, 49, Bodytype.MINICIV, Vest.GYELLOWSHIRT, Pants.BEIGEPANTS, -1, -1, -1, -1, -1, -1)
			-- general store
			CreateCivilian(15117, 0, 43, Bodytype.MANCIV, -1, -1, -1, -1, -1, -1, -1, -1)
		-- Grumm
		elseif ( sSectorX == 1 and sSectorY == SectorY.MAP_ROW_H) then
			-- general store
			CreateCivilian(12430, 0, 44, Bodytype.BIGMALE, -1, -1, -1, -1, -1, -1, -1, -1)
			-- wine store
			CreateCivilian(13533, 0, 58, Bodytype.MANCIV, -1, -1, -1, -1, -1, -1, -1, -1)
			-- restaurant
			CreateCivilian(12722, 0, 50, Bodytype.MINICIV, -1, -1, -1, -1, -1, -1, -1, -1)
			-- tool shop
			CreateCivilian(8362, 0, 61, Bodytype.REGFEMALE, -1, -1, -1, -1, -1, -1, -1, -1)
		-- Balime
		elseif ( sSectorX == 12 and sSectorY == SectorY.MAP_ROW_L) then
			-- luxury store
			CreateCivilian(9653, CivGroup.LOYAL_CIV_GROUP, 64, Bodytype.MANCIV, -1, -1, -1, -1, 212, 264, -1, -1)
		-- gas station near Balime	
		elseif ( sSectorX == 10 and sSectorY == SectorY.MAP_ROW_L) then
			-- restaurant
			CreateCivilian(11932, 0, 51, Bodytype.MINICIV, -1, -1, -1, -1, -1, -1, -1, -1)
		-- San Mona
		elseif ( sSectorX == 6 and sSectorY == SectorY.MAP_ROW_C) then
			-- only add merchants if Kingpin is alive and not hostile towards us
			if ( (CheckFact( Facts.FACT_KINGPIN_DEAD, 0 ) == false) and
			(CheckFact( Facts.FACT_KINGPIN_IS_ENEMY, 0 ) == false) and 
			(CheckMercIsDead ( Profil.KINGPIN ) == false) ) then
				-- restaurant
				CreateCivilian(16351, CivGroup.KINGPIN_CIV_GROUP, 66, Bodytype.DRESSCIV, -1, -1, -1, -1, -1, -1, -1, -1)
				-- armour store (Skin Tight Fashions)
				CreateCivilian(13010, CivGroup.KINGPIN_CIV_GROUP, 67, Bodytype.MINICIV, -1, -1, -1, -1, -1, -1, -1, -1)
				-- hunting store
				CreateCivilian(11098, CivGroup.KINGPIN_CIV_GROUP, 62, Bodytype.REGMALE, Vest.BROWNVEST, Pants.GREENPANTS, -1, -1, 763, 135, 288, 284)
			end
			
			-- if we haven't pissed of the black market, spawn dealer
			if ( CheckCivGroupHostile(CivGroup.BLACKMARKET_GROUP) == false ) then
				
				-- unlock the door so we can enter
				ACTION_ITEM_UNLOCK_DOOR(12834)
				
				-- black market dealer and bodyguards
				CreateCivilian(12992, CivGroup.BLACKMARKET_GROUP, 68, Bodytype.MANCIV, Vest.GREYVEST, Pants.JEANPANTS, Hair.WHITEHEAD, Skin.BLACKSKIN, 337, 264, -1, -1)
				CreateArmedCivilain(CivGroup.BLACKMARKET_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 12672, 0)
				CreateArmedCivilain(CivGroup.BLACKMARKET_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 13312, 0)
				CreateArmedCivilain(CivGroup.BLACKMARKET_GROUP, SoldierClass.SOLDIER_CLASS_ELITE, 12995, 0)
			end
			
		elseif ( sSectorX == 5 and sSectorY == SectorY.MAP_ROW_C) then
			-- only add merchants if Kingpin is alive and not hostile towards us
			if ( (CheckFact( Facts.FACT_KINGPIN_DEAD, 0 ) == false) and
			(CheckFact( Facts.FACT_KINGPIN_IS_ENEMY, 0 ) == false) and 
			(CheckMercIsDead ( Profil.KINGPIN ) == false) ) then
				-- general store
				CreateCivilian(6641, CivGroup.KINGPIN_CIV_GROUP, 45, Bodytype.REGMALE, -1, -1, -1, -1, 694, 107, 37, -1)
				-- construction materials store
				CreateCivilian(20549, CivGroup.KINGPIN_CIV_GROUP, 63, Bodytype.STOCKYMALE, -1, -1, -1, -1, 13, 161, 302, 135)
				-- restaurant
				CreateCivilian(16755, CivGroup.KINGPIN_CIV_GROUP, 52, Bodytype.MANCIV, -1, -1, -1, -1, 759, -1, -1, -1)
				-- wine store
				CreateCivilian(10804, CivGroup.KINGPIN_CIV_GROUP, 59, Bodytype.REGFEMALE, -1, -1, -1, -1, 340, 107, 302, 284)
			end
			
		-- Chitzena
		-- Meduna
		elseif ( sSectorX == 4 and sSectorY == SectorY.MAP_ROW_O) then
			-- posh general store
			CreateCivilian(10503, CivGroup.LOYAL_CIV_GROUP, 46, Bodytype.MINICIV, -1, -1, -1, -1, 212, -1, -1, -1)
			-- restaurant
			CreateCivilian(18465, CivGroup.LOYAL_CIV_GROUP, 53, Bodytype.MANCIV, -1, -1, -1, -1, -1, -1, -1, -1)
			-- restaurant
			CreateCivilian(9840, CivGroup.LOYAL_CIV_GROUP, 54, Bodytype.REGFEMALE, -1, -1, -1, -1, -1, -1, -1, -1)
			-- luxury store
			CreateCivilian(10916, CivGroup.LOYAL_CIV_GROUP, 65, Bodytype.MANCIV, -1, -1, -1, -1, 264, -1, -1, -1)
			-- pharmacy
			CreateCivilian(11579, CivGroup.LOYAL_CIV_GROUP, 48, Bodytype.FATCIV, -1, -1, -1, -1, -1, -1, -1, -1)
		end
	elseif ( bSectorZ == 1 ) then
		-- We spawn a rebel 'quartermaster' in the rebel basement. He doesn't have much to offer, and it isn't exactly quality ware, but better than nothing
		if ( sSectorX == 10 and sSectorY == SectorY.MAP_ROW_A) then
			CreateCivilian(11433, CivGroup.REBEL_CIV_GROUP, 40, Bodytype.CRIPPLECIV, Vest.BROWNVEST, Pants.GREENPANTS, Hair.WHITEHEAD, -1, 711, 545, 545, -1)
		end
	end
	
	
end

-- these colours can be used in the map
-- if you want to know how they look, test them on the map
-- if you want more colours, check whether you feel lucky and ask a coder
MapColour = {
	MAP_SHADE_BLACK = 0,
	
	MAP_SHADE_LT_GREEN = 1,
	MAP_SHADE_MD_GREEN = 2,
	MAP_SHADE_DK_GREEN = 3,
	
	MAP_SHADE_LT_RED = 4,
	MAP_SHADE_MD_RED = 5,
	MAP_SHADE_DK_RED = 6,
	
	MAP_SHADE_LT_YELLOW = 7,
	MAP_SHADE_MD_YELLOW = 8,
	MAP_SHADE_DK_YELLOW = 9,
	
	MAP_SHADE_LT_CYAN = 10,
	MAP_SHADE_MD_CYAN = 11,
	MAP_SHADE_DK_CYAN = 12,
	
	MAP_SHADE_LT_GREY = 13,
	MAP_SHADE_MD_GREY = 14,
	MAP_SHADE_DK_GREY = 15,
	
	MAP_SHADE_LT_BLUE = 16,
	MAP_SHADE_MD_BLUE = 17,
	MAP_SHADE_DK_BLUE = 18,
	
	MAP_SHADE_LT_PINK = 19,
	MAP_SHADE_ORANGE = 20,
	
	MAP_SHADE_MIX_RED = 21,
	MAP_SHADE_MIX_GREEN = 22,
	MAP_SHADE_MIX_BLUE = 23,
	MAP_SHADE_MIX_YELLOW = 24,
	
	MAP_SHADE_MIX_RED_GREEN = 25,
	MAP_SHADE_MIX_RED_BLUE = 26,
	MAP_SHADE_MIX_RED_YELLOW = 27,
	MAP_SHADE_MIX_GREEN_BLUE = 28,
	MAP_SHADE_MIX_GREEN_YELLOW = 29,
	MAP_SHADE_MIX_BLUE_YELLOW = 30,
	
	MAP_SHADE_MIX_RED_GREEN_BLUE = 31,
	MAP_SHADE_MIX_RED_GREEN_YELLOW = 32,
	MAP_SHADE_MIX_RED_BLUE_YELLOW = 33,
	
	MAP_SHADE_MIX_GREEN_BLUE_YELLOW = 34,
	MAP_SHADE_MIX_RED_GREEN_BLUE_YELLOW = 35,
}


-- These symbols are stored in Interface/IntelMapSymbols.sti
-- if you want more symbols, add them
MapSymbols = {
	BLOODCAT_ATTACK = 0,
	ZOMBIE_ATTACK = 1,
	BANDIT_ATTACK = 2,
	TERRORIST_LOCATION = 3,
	CHALICE = 4,
	ASSASSINATION_TARGET = 5,
	HELI = 6,
	PRISON = 7,
	CHURCH = 8,	
	RECRUITMENT = 9,
	FLAG = 10,
	QUESTIONMARK_BLUE = 11,		-- sector might be relevant for a quest
	EXCLAMATIONMARK_BLUE = 12,	-- sector is definetely relevant for a quest	
	QUESTIONMARK_GREEN = 13,	-- alternate colours for other uses?
	EXCLAMATIONMARK_GREEN = 14,
	QUESTIONMARK_RED = 15,
	EXCLAMATIONMARK_RED = 16,
	QUESTIONMARK_YELLOW = 17,
	EXCLAMATIONMARK_YELLOW = 18,	
}

-- this function allows us to data for the intel/quest map
-- aLevel is the world level (0: surface)
-- set data by using SetIntelAndQuestMapDataForSector(sectorx, sextory, MapColour, symbol, text for description window, short text for map)
function GetIntelAndQuestMapData( aLevel )

	-- surface
	if ( aLevel == 0 ) then
		
		-- mark sectors related to quests		
		if ( gubQuest( Quests.QUEST_DELIVER_LETTER ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.FATIMA), GetCharacterSectorY(Profil.FATIMA), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Deliver letter to rebels", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_FOOD_ROUTE ) == qStatus.QUESTINPROGRESS ) then
			
			-- list possible locations of Father Walker
			SetIntelAndQuestMapDataForSector(13, 3, -1, MapSymbols.CHURCH, "Find Father Walker", "")
			SetIntelAndQuestMapDataForSector(13, 4, -1, MapSymbols.CHURCH, "Find Father Walker", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_KINGPIN_IDOL ) == qStatus.QUESTINPROGRESS ) then
			
			if ( (CheckFact( Facts.FACT_CHALICE_STOLEN, 0 ) == true) ) then
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.KINGPIN), GetCharacterSectorY(Profil.KINGPIN), -1, MapSymbols.CHALICE, "Return chalice to Kingpin", "")
			else
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.ELDIN), GetCharacterSectorY(Profil.ELDIN), -1, MapSymbols.CHALICE, "Steal Chalice from museum", "")
			end
			
		end
		
		if ( gubQuest( Quests.QUEST_KINGPIN_MONEY ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.KINGPIN), GetCharacterSectorY(Profil.KINGPIN), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Return money to Kingpin", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_RUNAWAY_JOEY ) == qStatus.QUESTINPROGRESS ) then
			
			if ( (CheckFact( Facts.FACT_JOEY_ESCORTED, 0 ) == true) ) then
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.MARTHA), GetCharacterSectorY(Profil.MARTHA), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Return Joey to Martha", "")
			else
				-- we list all locations Martha mentions... even though he will never be present at some of them. Misdirection!
				SetIntelAndQuestMapDataForSector(6, 9, -1, MapSymbols.QUESTIONMARK_BLUE, "Find Joey", "")
				SetIntelAndQuestMapDataForSector(2, 1, -1, MapSymbols.QUESTIONMARK_BLUE, "Find Joey", "")				
				SetIntelAndQuestMapDataForSector(5, 3, -1, MapSymbols.QUESTIONMARK_BLUE, "Find Joey", "")
				SetIntelAndQuestMapDataForSector(6, 3, -1, MapSymbols.QUESTIONMARK_BLUE, "Find Joey", "")
				SetIntelAndQuestMapDataForSector(5, 4, -1, MapSymbols.QUESTIONMARK_BLUE, "Find Joey", "")
				SetIntelAndQuestMapDataForSector(4, 4, -1, MapSymbols.QUESTIONMARK_BLUE, "Find Joey", "")
			end
			
		end
		
		if ( gubQuest( Quests.QUEST_RESCUE_MARIA ) == qStatus.QUESTINPROGRESS ) then
			
			if ( (CheckFact( Facts.FACT_MARIA_ESCORTED, 0 ) == true) ) then
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.ANGEL), GetCharacterSectorY(Profil.ANGEL), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Return Maria to Angel", "")
			else
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.MARIA), GetCharacterSectorY(Profil.MARIA), -1, MapSymbols.PRISON, "Free Maria", "")
			end
			
		end
		
		if ( gubQuest( Quests.QUEST_CHITZENA_IDOL ) == qStatus.QUESTINPROGRESS ) then
			
			if ( (CheckFact( Facts.FACT_CHALICE_STOLEN, 0 ) == true) ) then
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.YANNI), GetCharacterSectorY(Profil.YANNI), -1, MapSymbols.CHALICE, "Return chalice to Yanni", "")
			else
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.ELDIN), GetCharacterSectorY(Profil.ELDIN), -1, MapSymbols.CHALICE, "Steal Chalice from museum", "")
			end
			
		end
				
		if ( gubQuest( Quests.QUEST_ARMY_FARM ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.DARREL), GetCharacterSectorY(Profil.DARREL), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Pacify the Hicks clan", "")
			
		end
				
		if ( gubQuest( Quests.QUEST_FIND_SCIENTIST ) == qStatus.QUESTINPROGRESS ) then
			
			-- list all possible sectors where Madlab might hide
			SetIntelAndQuestMapDataForSector(7, 8, -1, MapSymbols.QUESTIONMARK_BLUE, "Find scientist", "")
			SetIntelAndQuestMapDataForSector(16, 8, -1, MapSymbols.QUESTIONMARK_BLUE, "Find scientist", "")
			SetIntelAndQuestMapDataForSector(11, 9, -1, MapSymbols.QUESTIONMARK_BLUE, "Find scientist", "")
			SetIntelAndQuestMapDataForSector(4, 5, -1, MapSymbols.QUESTIONMARK_BLUE, "Find scientist", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_DELIVER_VIDEO_CAMERA ) == qStatus.QUESTINPROGRESS ) then
			
			-- list all possible sectors where Madlab might hide
			SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.MADLAB), GetCharacterSectorY(Profil.MADLAB), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Give gun & camera to Madlab", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_BLOODCATS ) == qStatus.QUESTINPROGRESS ) then
			
			-- Annie only mentions that the bloodcat nest is 'east of her house', so we are rather liberal with hints here
			SetIntelAndQuestMapDataForSector(15, 8, -1, MapSymbols.QUESTIONMARK_BLUE, "Destroy bloodcat nest", "")
			SetIntelAndQuestMapDataForSector(16, 8, -1, MapSymbols.QUESTIONMARK_BLUE, "Destroy bloodcat nest", "")
			SetIntelAndQuestMapDataForSector(15, 9, -1, MapSymbols.QUESTIONMARK_BLUE, "Destroy bloodcat nest", "")
			SetIntelAndQuestMapDataForSector(16, 9, -1, MapSymbols.QUESTIONMARK_BLUE, "Destroy bloodcat nest", "")
			SetIntelAndQuestMapDataForSector(15, 10, -1, MapSymbols.QUESTIONMARK_BLUE, "Destroy bloodcat nest", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_CHOPPER_PILOT ) == qStatus.QUESTINPROGRESS ) then
			
			-- list all possible sectors where Skyrider might hide
			SetIntelAndQuestMapDataForSector(15, 2, -1, MapSymbols.QUESTIONMARK_BLUE, "Find chopper pilot", "")
			SetIntelAndQuestMapDataForSector(14, 5, -1, MapSymbols.QUESTIONMARK_BLUE, "Find chopper pilot", "")
			SetIntelAndQuestMapDataForSector(12, 4, -1, MapSymbols.QUESTIONMARK_BLUE, "Find chopper pilot", "")
			SetIntelAndQuestMapDataForSector(16, 3, -1, MapSymbols.QUESTIONMARK_BLUE, "Find chopper pilot", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_ESCORT_SKYRIDER ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(13, 2, -1, MapSymbols.HELI, "Escort Skyrider here", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_FREE_DYNAMO ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.DYNAMO), GetCharacterSectorY(Profil.DYNAMO), -1, MapSymbols.PRISON, "Free Dynamo", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_ESCORT_TOURISTS ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(13, 2, -1, MapSymbols.HELI, "Escort John & Mary here", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_FREE_CHILDREN ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(13, 3, -1, MapSymbols.PRISON, "Free factory children", "")
			
		end
		
		if ( gubQuest( Quests.QUEST_LEATHER_SHOP_DREAM ) == qStatus.QUESTINPROGRESS ) then
			
			if ( (CheckFact( Facts.FACT_ANGEL_LEFT_DEED, 0 ) == true) ) then
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.KYLE), GetCharacterSectorY(Profil.KYLE), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Give deed to Kyle", "")
			else
				SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.ANGEL), GetCharacterSectorY(Profil.ANGEL), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Get deed from Angel", "")
			end
			
		end
		
		if ( gubQuest( Quests.QUEST_ESCORT_SHANK ) == qStatus.QUESTINPROGRESS ) then
			
			SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.JAKE), GetCharacterSectorY(Profil.JAKE), -1, MapSymbols.EXCLAMATIONMARK_BLUE, "Escort Shank to Jake", "")
			
		end
		
		if ( not (gubQuest( Quests.QUEST_KILL_DEIDRANNA ) == qStatus.QUESTDONE) ) then
			
			SetIntelAndQuestMapDataForSector(GetCharacterSectorX(Profil.QUEEN), GetCharacterSectorY(Profil.QUEEN), -1, MapSymbols.ASSASSINATION_TARGET, "Kill Deidranna", "")
			
		end
				
		if ( gubQuest( Quests.QUEST_KINGPIN_ANGEL_MARIA ) == qStatus.QUESTINPROGRESS ) then
			
			-- list all possible sectors where Maria & Angel might hide
			SetIntelAndQuestMapDataForSector(11, 1, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			SetIntelAndQuestMapDataForSector(5, 2, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			SetIntelAndQuestMapDataForSector(6, 2, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			SetIntelAndQuestMapDataForSector(8, 2, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			SetIntelAndQuestMapDataForSector(12, 2, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			SetIntelAndQuestMapDataForSector(14, 2, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			SetIntelAndQuestMapDataForSector(3, 3, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			SetIntelAndQuestMapDataForSector(7, 4, -1, MapSymbols.QUESTIONMARK_BLUE, "Kill or protect Angel & Maria", "")
			
		end
		
	end
end
