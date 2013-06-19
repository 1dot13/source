--[[
http://legion.itzg.pl/mzgb/pmwiki/pmwiki.php?n=JaggedAlliance2UBV1.Jav113lua
Remember! Not to use function EndQuest(QuestID, x, y) and StartQuest(QuestID, x, y) in the file "Quests.lua".
]]


nQuests = 
{
	-- Quests ID							-- BinaryData\QUESTS.EDT Record
	QUEST_DELIVER_LETTER = 0,				-- Start quest 0, End quest 1
	QUEST_FOOD_ROUTE = 1,					-- Start quest 2, End quest 3
	QUEST_KILL_TERRORISTS = 2,				-- Start quest 4, End quest 5
	QUEST_KINGPIN_IDOL = 3,					-- Start quest 6, End quest 7
	QUEST_KINGPIN_MONEY = 4,				-- Start quest 8, End quest 9
	QUEST_RUNAWAY_JOEY = 5,					-- Start quest 10, End quest 11
	QUEST_RESCUE_MARIA = 6,					-- Start quest 12, End quest 13
	QUEST_CHITZENA_IDOL = 7,				-- Start quest 14, End quest 15
	QUEST_HELD_IN_ALMA = 8,					-- Start quest 16, End quest 17
	QUEST_INTERROGATION = 9,				-- Start quest 18, End quest 19
	QUEST_ARMY_FARM = 10,					-- Start quest 20, End quest 21
	QUEST_FIND_SCIENTIST = 11,				-- Start quest 22, End quest 23
	QUEST_DELIVER_VIDEO_CAMERA = 12,		-- Start quest 24, End quest 25
	QUEST_BLOODCATS = 13,					-- Start quest 26, End quest 27
	QUEST_FIND_HERMIT = 14,					-- Start quest 28, End quest 29
	QUEST_CREATURES = 15,					-- Start quest 30, End quest 31
	QUEST_CHOPPER_PILOT = 16,				-- Start quest 32, End quest 33
	QUEST_ESCORT_SKYRIDER = 17,				-- Start quest 34, End quest 35
	QUEST_FREE_DYNAMO = 18,					-- Start quest 36, End quest 37
	QUEST_ESCORT_TOURISTS = 19,				-- Start quest 38, End quest 39
	QUEST_FREE_CHILDREN = 20,				-- Start quest 40, End quest 41
	QUEST_LEATHER_SHOP_DREAM = 21,			-- Start quest 42, End quest 43
	QUEST_ESCORT_SHANK = 22,				-- Start quest 44, End quest 45
	QUEST_23 = 23,							-- Start quest 46, End quest 47
	QUEST_24 = 24,							-- Start quest 48, End quest 49
	QUEST_KILL_DEIDRANNA = 25,				-- Start quest 50, End quest 51
	
	-- max Quests 254
}

qStatus = {
            QUESTNOTSTARTED = 0,
            QUESTINPROGRESS = 1,
            QUESTDONE = 2,
          }
		  
Profiles = 
{	
	DYNAMO = 66,
	CARMEN = 78,
	MADAME = 107,
}

nHistory = {
	HISTORY_QUEST_STARTED = 15,
	HISTORY_QUEST_FINISHED = 16,
	}
	
nFacts = {

	FACT_ESTONI_REFUELLING_POSSIBLE = 277,

}
	
local NO_PROFILE = 200

function InternalStartQuest( ubQuest, sSectorX, sSectorY, fUpdateHistory )

if ( CheckQuest(ubQuest) == qStatus.QUESTNOTSTARTED ) then 
		SetQuest( ubQuest, qStatus.QUESTINPROGRESS ) 	
			if ( fUpdateHistory == true) then		
				if ( is_networked == 0 ) then
					   SetHistoryFact( nHistory.HISTORY_QUEST_STARTED, ubQuest, GetWorldTotalMin(), sSectorX, sSectorY )
					end	
			end		
	else
		SetQuest( ubQuest, qStatus.QUESTINPROGRESS )
	end

end
		
function InternalEndQuest( ubQuest, sSectorX, sSectorY, fUpdateHistory )

  if ( not ( CheckQuest(ubQuest) == qStatus.QUESTDONE ) and fUpdateHistory == true ) then
    
		if ( ubQuest == nQuests.QUEST_FOOD_ROUTE ) then
			-- food quest is handled differently (completing when spoken to father Walker)
		elseif ( ubQuest == nQuests.QUEST_DELIVER_LETTER ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 3, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_LEATHER_SHOP_DREAM ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 3, NO_PROFILE )	
		elseif ( ubQuest == nQuests.QUEST_ESCORT_SKYRIDER ) then	
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_CHOPPER_PILOT ) then	
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_FREE_CHILDREN ) then	
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_RESCUE_MARIA ) then	
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_FIND_SCIENTIST ) then	
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_DELIVER_VIDEO_CAMERA ) then	
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_FIND_HERMIT ) then	
				GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_FREE_DYNAMO ) then		
			
				-- don't give Dynamo reward for himself
				if ( CheckMercIsDead ( Profiles.DYNAMO ) == false ) then
					GiveQuestRewardPoint( sSectorX, sSectorY, 4, Profiles.DYNAMO )
				end
				
		elseif ( ubQuest == nQuests.QUEST_KINGPIN_IDOL ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE )	
		elseif ( ubQuest == nQuests.QUEST_CHITZENA_IDOL ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE )	
		elseif ( ubQuest == nQuests.QUEST_HELD_IN_ALMA ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE )	
		elseif ( ubQuest == nQuests.QUEST_RUNAWAY_JOEY ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE )	
		elseif ( ubQuest == nQuests.QUEST_ESCORT_TOURISTS ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE )	
		elseif ( ubQuest == nQuests.QUEST_ARMY_FARM ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 6, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_KINGPIN_MONEY ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 6, NO_PROFILE )		
		elseif ( ubQuest == nQuests.QUEST_BLOODCATS ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 7, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_INTERROGATION ) then
				-- we get here only if wiped out enemies after interrogation
				GiveQuestRewardPoint( sSectorX, sSectorY, 8, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_CREATURES ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 8, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_KILL_TERRORISTS ) then
				
				-- only reduced experiences if we chosen Slay over Carmen
				if ( CheckMercIsDead ( Profiles.CARMEN ) == true ) then
					GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE )
				else
					GiveQuestRewardPoint( sSectorX, sSectorY, 9, NO_PROFILE )
				end	
				
		elseif ( ubQuest == nQuests.QUEST_KILL_DEIDRANNA ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 25, NO_PROFILE )
		elseif ( ubQuest == nQuests.QUEST_ESCORT_SHANK ) then
				GiveQuestRewardPoint( sSectorX, sSectorY, 5, NO_PROFILE )	
		else
			GiveQuestRewardPoint( sSectorX, sSectorY, 4, NO_PROFILE )	
		end
		
		
  end 
  
  if ( CheckQuest(ubQuest) == qStatus.QUESTINPROGRESS ) then
	
		SetQuest( ubQuest, qStatus.QUESTDONE ) 
			
		if ( fUpdateHistory == true ) then
			ResetHistoryFact( ubQuest, sSectorX, sSectorY )
		end
  else
	    SetQuest( ubQuest, qStatus.QUESTDONE ) 
  end

	if ( ubQuest == nQuests.QUEST_RESCUE_MARIA ) then
	
		-- cheap hack to try to prevent Madame Layla from thinking that you are
		-- still in the brothel with Maria...
		SetNPCData1(Profiles.MADAME, 0)
		SetNPCData2(Profiles.MADAME, 0)
	end
	
	
end