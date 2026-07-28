Profil = {
CARMEN 		= 78,
MARIA 		= 88,
JOEY 		= 90,
SKYRIDER 	= 97,
JOHN 		= 118,
MARY 		= 119,
}



Group = {
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

StatusGroup = {
 
	CIV_GROUP_NEUTRAL = 0,
	CIV_GROUP_WILL_EVENTUALLY_BECOME_HOSTILE = 1,
	CIV_GROUP_WILL_BECOME_HOSTILE = 2,
	CIV_GROUP_HOSTILE = 3,
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

Facts = 
{ 
	FACT_SKYRIDER_EVER_ESCORTED = 269,
}
	
function HandleStuffForNPCEscorted( ubNPC )
	if ( ubNPC == Profil.MARIA or ubNPC == Profil.JOEY ) then
		-- none
	elseif ( ubNPC == Profil.SKYRIDER ) then
		SetFactTrue( Facts.FACT_SKYRIDER_EVER_ESCORTED )
		
		if ( CheckQuest( Quests.QUEST_ESCORT_SKYRIDER) == qStatus.QUESTNOTSTARTED ) then
			StartQuest( Quests.QUEST_ESCORT_SKYRIDER, gWorldSectorX, gWorldSectorY )
		end
	elseif ( ubNPC == Profil.JOHN ) then		

			-- recruit Mary as well
			RecruitEPC( Profil.MARY )

			if ( CheckQuest(Quests.QUEST_ESCORT_TOURISTS ) == qStatus.QUESTNOTSTARTED ) then
				StartQuest( Quests.QUEST_ESCORT_TOURISTS, gWorldSectorX, gWorldSectorY )
			end
	elseif ( ubNPC == Profil.MARY ) then		

			-- recruit John as well
			RecruitEPC( Profil.JOHN )
			
			if ( CheckQuest(Quests.QUEST_ESCORT_TOURISTS ) == qStatus.QUESTNOTSTARTED ) then
				StartQuest( Quests.QUEST_ESCORT_TOURISTS, gWorldSectorX, gWorldSectorY )
			end
	end
end

function CarmenLeavesSectorCallback()
	if ( gWorldSectorX == 13 and gWorldSectorY == SectorY.MAP_ROW_C and gWorldSectorZ == 0 ) then
		TriggerNPCRecord( Profil.CARMEN, 34 )
	elseif ( gWorldSectorX == 9 and gWorldSectorY == SectorY.MAP_ROW_G and gWorldSectorZ == 0 ) then
		TriggerNPCRecord( Profil.CARMEN, 35 )
	elseif ( gWorldSectorX == 5 and gWorldSectorY == SectorY.MAP_ROW_C and gWorldSectorZ == 0 ) then 
		TriggerNPCRecord( Profil.CARMEN, 36 )
	end	
end