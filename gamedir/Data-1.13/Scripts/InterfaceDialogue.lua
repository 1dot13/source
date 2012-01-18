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

Quests = {
	QUEST_DELIVER_LETTER = 0,
	QUEST_FOOD_ROUTE = 1,
	QUEST_KILL_TERRORISTS = 2,
	QUEST_KINGPIN_IDOL = 3,
	QUEST_KINGPIN_MONEY = 4,
	QUEST_RUNAWAY_JOEY = 5,
	QUEST_RESCUE_MARIA = 6,
	QUEST_CHITZENA_IDOL = 7,
	QUEST_HELD_IN_ALMA = 8,
	QUEST_INTERROGATION = 9,
	QUEST_ARMY_FARM = 10,
	QUEST_FIND_SCIENTIST = 11,
	QUEST_DELIVER_VIDEO_CAMERA = 12,
	QUEST_BLOODCATS = 13,
	QUEST_FIND_HERMIT = 14,
	QUEST_CREATURES = 15,
	QUEST_CHOPPER_PILOT = 16,
	QUEST_ESCORT_SKYRIDER = 17,
	QUEST_FREE_DYNAMO = 18,
	QUEST_ESCORT_TOURISTS = 19,
	QUEST_FREE_CHILDREN = 20,
	QUEST_LEATHER_SHOP_DREAM = 21,
	QUEST_KILL_DEIDRANNA = 25,
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