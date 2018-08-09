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

	if ( sNewSectorX == 6 and sNewSectorY == SectorY.MAP_ROW_C and gubQuest( Quests.QUEST_RESCUE_MARIA ) == qStatus.QUESTDONE ) then
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

-- this function is called whenever we liberate a sector. If fFirstTime is true, this is the first time we liberate this sector
function HandleSectorLiberation( sNewSectorX, sNewSectorY, bNewSectorZ, fFirstTime )

end

-- this function is called whenever we recruit a RPC
function RecruitRPCAdditionalHandling( usProfile )

end

-- this function is called whenever we enter a sector in tactical
function HandleSectorTacticalEntry( sSectorX, sSectorY, bSectorZ, fHasEverBeenPlayerControlled )
	
	
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
		
	end
end
