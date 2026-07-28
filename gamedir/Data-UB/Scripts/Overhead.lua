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
	-- UB Quests
	QUEST_DESTROY_MISSLES = 23,				 -- Start quest 46, End quest 47
	QUEST_FIX_LAPTOP = 24,					 -- Start quest 48, End quest 49
	QUEST_GET_RID_BLOODCATS_AT_BETTYS = 25,  -- Start quest 50, End quest 51
	QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL = 26, -- Start quest 52, End quest 53
	
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

	
	
end
-- end local function

-- local function
local function HandleMaryArrival( ID )

	

end
-- end local function

function HandleAtNewGridNo( ProfileId )

	if ( FindSoldierByProfileID (UB_GetTexID()) == UB_GetTexID() ) then
		SetKeyProfile (UB_GetTexID(),true)
	end
	
	if ( FindSoldierByProfileID (UB_GetBettyID()) == UB_GetBettyID() ) then
		SetKeyProfile (UB_GetBettyID(),true)
	end


	
	
	TeamSoldier = FindSoldierTeam (ProfileId)
	if ( TeamSoldier == Team.OUR_TEAM ) then -- Team
	

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
		
	elseif ( usActionType == ActionTypes.ACTIONTYPE_READFILE ) then
		
	elseif ( usActionType == ActionTypes.ACTIONTYPE_MINIGAME ) then
		
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


-- shopkeeper quotes while in the shop interface
Shopkeeperquote = 
{
	SK_QUOTES_DEALER_OFFERED_MONEY_AS_A_GIFT = 10,
	SK_QUOTES_PLAYER_FIRST_ENTERS_SKI = 11,
	SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_1 = 12,
	SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_2 = 13,
	SK_QUOTES_RANDOM_QUOTE_WHILE_ITEMS_CHOSEN_TO_TRADE = 14,
	SK_QUOTES_RANDOM_QUOTE_WHILE_ITEMS_CHOSEN_TO_SELL_OR_REPAIR = 15,
	SK_QUOTES_PLAYER_REQUESTED_EVALUATION = 16,
	SK_QUOTES_DURING_EVALUATION_STUFF_REJECTED = 17,
	SK_QUOTES_EVALUATION_RESULT_VALUE_OF_ZERO = 18,
	SK_QUOTES_EVALUATION_RESULT_SOME_REALLY_DAMAGED_ITEMS = 19,
	SK_QUOTES_EVALUATION_RESULT_NORMAL = 20,
	SK_QUOTES_EVAULATION_PLAYER_DOESNT_HAVE_ENOUGH_VALUE = 21,
	SK_QUOTES_PLAYER_HAS_EXACTLY_ENOUGH_MONEY_FOR_TRANSACTION = 22,
	SK_QUOTES_PLAYER_HAS_TOO_MUCH_MONEY_FOR_TRANSACTION = 23,
	SK_QUOTES_PRESSES_DONE_HAS_AT_LEAST_1_TRANSACTION = 24,
	SK_QUOTES_PRESSED_DONE_HASNT_MADE_TRANSACTION = 25,
	SK_QUOTES_PRESSED_DONE_STILL_HAS_STUFF_IN_OFFER_AREA = 26,
	SK_QUOTES_CANT_AFFORD_TO_BUY_OR_TOO_MUCH_TO_REPAIR = 27,
}

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
