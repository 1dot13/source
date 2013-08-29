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