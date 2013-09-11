--[[

	http://legion.itzg.pl/mzgb/pmwiki/pmwiki.php?n=JaggedAlliance2UBV1.Jav113lua

**********************
** Global variables **
**********************
	
	- giLairID	
		1 = Drassen
		2 = Cambria
		3 = Alma
		4 = Grumm


***************
** Functions **
***************
	
	- SectorEnemyControlled ( IDSector )
	  	check if enemy controls sector
	  
	- CALCULATE_STRATEGIC_INDEX( SectorX, SectorY )
	
	- SECTOR ( SectorX, SectorY )
	  
	- CheckMercIsDead (ProfilID)
	  	check merc is dead 

	- AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
	
	- IncrementTownLoyaltyEverywhere( idLoyalty ) 
      	increment all town loyalty
	  
	- IncrementTownLoyalty ( idLoyalty )
	  	increment town loyalty
	  
	- DecrementTownLoyalty ( idTown, idLoyalty )
      	decrement town loyalty
	  
	- DecrementTownLoyaltyEverywhere ( idLoyalty )
      	decrement all town loyalty
	  
	- SetTownLoyalty ( idTown, idLoyalty )
	  	set town loyalty


**************
** Examples **
**************
	
	if ( SectorEnemyControlled ( CALCULATE_STRATEGIC_INDEX (13,4) ) == false ) then
		-- instructions
	else
		-- instructions
	end
	
	if ( CheckMercIsDead (101) == true ) then
		-- merc is dead
	else
		-- instructions
	end
	
	-- 1  * 500 = 500   = 4%
	DecrementTownLoyaltyEverywhere ( 1 )

	-- 10 * 500 = 5000  = 45%
	IncrementTownLoyalty (10)

	-- 20 * 500 = 10000 = 90%
	DecrementTownLoyalty ( 2 , 20 )

	-- 30 * 500 = 15000 = 100%
	IncrementTownLoyaltyEverywhere (30)

	-- 100 = 100%
	SetTownLoyalty ( 1, 100 ) 

	-- 30  = 30%
	SetTownLoyalty ( 1, 30 ) 
			
	EventGlobal = 
	{
		GLOBAL_LOYALTY_BATTLE_WON = 0,
		.
		.
		MY_GLOBAL_LOYALTY_13 = 13,
		MY_GLOBAL_LOYALTY_14 = 14,
		.
		.	.
		MY_GLOBAL_LOYALTY_100 = 100,
	}
	
]]

Profil = 
{                   
    MIGUEL    = 57,
    DOREEN    = 139,
    MARTHA    = 109,
    KEITH     = 147,
    YANNI     = 108,
    AUNTIE    = 76,
    MATT      = 148,
    JOEY      = 90,
}

Town = 
{
	OMERTA   = 1,
	DRASSEN  = 2,
	ALMA     = 3,
	GRUMM    = 4,
	CAMBRIA  = 6,
	SANMONA  = 7,
	ESTONI   = 8,
	BALIME   = 10,
	MEDUNa   = 11,
	CHITZENA = 12,
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

-- gain pts per real loyalty pt
local GAIN_PTS_PER_LOYALTY_PT = 500

-- --- LOYALTY BONUSES ---
-- Omerta
local LOYALTY_BONUS_MIGUEL_READS_LETTER	= (10 * GAIN_PTS_PER_LOYALTY_PT)				-- multiplied by 4.5 due to Omerta's high seniment, so it's 45%
-- Drassen
local LOYALTY_BONUS_CHILDREN_FREED_DOREEN_KILLED = (10 * GAIN_PTS_PER_LOYALTY_PT)		-- +50% bonus for Drassen -- Overhead.cpp
local LOYALTY_BONUS_CHILDREN_FREED_DOREEN_SPARED = (20 * GAIN_PTS_PER_LOYALTY_PT)		-- +50% bonus for Drassen -- Overhead.cpp
-- Cambria
local LOYALTY_BONUS_MARTHA_WHEN_JOEY_RESCUED = (15 * GAIN_PTS_PER_LOYALTY_PT)			-- -25% for low Cambria sentiment
local LOYALTY_BONUS_KEITH_WHEN_HILLBILLY_SOLVED = (15 * GAIN_PTS_PER_LOYALTY_PT)		-- -25% for low Cambria sentiment
-- Chitzena
local LOYALTY_BONUS_YANNI_WHEN_CHALICE_RETURNED_LOCAL = (20 * GAIN_PTS_PER_LOYALTY_PT)	-- +75% higher in Chitzena -- no used
local LOYALTY_BONUS_YANNI_WHEN_CHALICE_RETURNED_GLOBAL = (10 * GAIN_PTS_PER_LOYALTY_PT)	-- for ALL towns!
-- Alma
local LOYALTY_BONUS_AUNTIE_WHEN_BLOODCATS_KILLED = (20 * GAIN_PTS_PER_LOYALTY_PT)		-- Alma's increases reduced by half due to low rebel sentiment
local LOYALTY_BONUS_MATT_WHEN_DYNAMO_FREED = (20 * GAIN_PTS_PER_LOYALTY_PT)				-- Alma's increases reduced by half due to low rebel sentiment
local LOYALTY_BONUS_FOR_SERGEANT_KROTT = (20 * GAIN_PTS_PER_LOYALTY_PT)					-- Alma's increases reduced by half due to low rebel sentiment (Interface Dialogue.cpp)
-- Everywhere
local LOYALTY_BONUS_TERRORISTS_DEALT_WITH = ( 5 * GAIN_PTS_PER_LOYALTY_PT)
local LOYALTY_BONUS_KILL_QUEEN_MONSTER = (10 * GAIN_PTS_PER_LOYALTY_PT)
-- Anywhere
-- Loyalty bonus for completing town training
local LOYALTY_BONUS_FOR_TOWN_TRAINING = ( 2 * GAIN_PTS_PER_LOYALTY_PT )					-- 2% (Town Militia.cpp i Map Screen Interface Map.cpp)

EventGlobal = 
{
    -- There are only for distance-adjusted global loyalty effects.	Others go into list above instead!
	GLOBAL_LOYALTY_BATTLE_WON = 0,
	GLOBAL_LOYALTY_BATTLE_LOST = 1,
	GLOBAL_LOYALTY_ENEMY_KILLED = 2,
	GLOBAL_LOYALTY_NATIVE_KILLED = 3,
	GLOBAL_LOYALTY_GAIN_TOWN_SECTOR = 4,
	GLOBAL_LOYALTY_LOSE_TOWN_SECTOR = 5,
	GLOBAL_LOYALTY_LIBERATE_WHOLE_TOWN = 6,
	GLOBAL_LOYALTY_ABANDON_MILITIA = 7,
	GLOBAL_LOYALTY_GAIN_MINE = 8,
	GLOBAL_LOYALTY_LOSE_MINE = 9,
	GLOBAL_LOYALTY_GAIN_SAM = 10,
	GLOBAL_LOYALTY_LOSE_SAM = 11,
	GLOBAL_LOYALTY_QUEEN_BATTLE_WON = 12,
	GLOBAL_LOYALTY_UNUSED = 13,
}


local function CheckConditionsForTriggeringCreatureQuest( sSectorX, sSectorY, bSectorZ )

	local ubValidMines = 0
	
	if ( newGAME_STYLE == 0 or iniENABLE_CREPITUS == 0 ) then 
		return -- No scifi, no creatures...
	end	
	
	-- giLairID from Luaglobal.cpp
	-- if ( giLairID ) -> that does not work in LUA, because that is always TRUE!!
	if ( giLairID ~= 0 ) then
		return	-- Creature quest already begun
	end
	
	-- Count the number of "infectible mines" the player occupies
	
	-- SEC_D13
	if ( SectorEnemyControlled ( CALCULATE_STRATEGIC_INDEX(13, SectorY.MAP_ROW_D) ) == false ) then
		ubValidMines = ubValidMines + 1
	end
	
	-- SEC_H8
	if ( SectorEnemyControlled ( CALCULATE_STRATEGIC_INDEX(8, SectorY.MAP_ROW_H) ) == false ) then
		ubValidMines = ubValidMines + 1
	end	
	
	-- SEC_I14
	if ( SectorEnemyControlled ( CALCULATE_STRATEGIC_INDEX(14, SectorY.MAP_ROW_I) ) == false ) then 
		ubValidMines = ubValidMines + 1
	end	
	
	-- SEC_H3
	if ( SectorEnemyControlled ( CALCULATE_STRATEGIC_INDEX(3, SectorY.MAP_ROW_H) ) == false ) then
		ubValidMines = ubValidMines + 1
	end	

	if ( ubValidMines >= 3 ) then
		InitCreatureQuest()
	end
end

function HandleGlobalLoyaltyEvent( ubEventType, sSectorX, sSectorY, bSectorZ )

	local iLoyaltyChange = 0

	if ubEventType == EventGlobal.GLOBAL_LOYALTY_BATTLE_WON then
		
		CheckConditionsForTriggeringCreatureQuest( sSectorX, sSectorY, bSectorZ )
		
		iLoyaltyChange = 500
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_QUEEN_BATTLE_WON then
		
		CheckConditionsForTriggeringCreatureQuest( sSectorX, sSectorY, bSectorZ )
		
		iLoyaltyChange = 1000
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_BATTLE_LOST then
		
		iLoyaltyChange = -750
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_ENEMY_KILLED then
		
		iLoyaltyChange = 50
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_NATIVE_KILLED then
		
		-- Note that there is special code (much more severe) for murdering civilians in the currently loaded sector.
		-- This event is intended more for processing militia casualties, and the like
		iLoyaltyChange = -50
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_ABANDON_MILITIA then
		
		-- It doesn't matter how many of them are being abandoned
		iLoyaltyChange = -750
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_GAIN_TOWN_SECTOR then
		
		iLoyaltyChange = 500
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_LOSE_TOWN_SECTOR then
		iLoyaltyChange = -1000
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_LIBERATE_WHOLE_TOWN then
		
		iLoyaltyChange = 1000
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_GAIN_MINE then
		
		iLoyaltyChange = 1000
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_LOSE_MINE then
		
		iLoyaltyChange = -1500
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_GAIN_SAM then
		
		iLoyaltyChange = 250
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )
		
	elseif ubEventType == EventGlobal.GLOBAL_LOYALTY_LOSE_SAM then
		
		iLoyaltyChange = -250	
		AffectAllTownsLoyaltyByDistanceFrom( iLoyaltyChange, sSectorX, sSectorY, bSectorZ )

	end
	
end

function HandleLoyaltyChangeForNPCAction( ubNPCProfileId )

    -- Omerta loyalty increases when Miguel receives letter from Enrico
	if ubNPCProfileId == Profil.MIGUEL then
		IncrementTownLoyalty ( Town.OMERTA, LOYALTY_BONUS_MIGUEL_READS_LETTER )
	end

	-- Having freed the child labourers... she is releasing them herself!
	if ubNPCProfileId == Profil.DOREEN  then
		IncrementTownLoyalty( Town.DRASSEN, LOYALTY_BONUS_CHILDREN_FREED_DOREEN_SPARED )
	end

    -- If Joey is still alive
	if ubNPCProfileId == Profil.MARTHA then
		if ( CheckMercIsDead(Profil.JOEY) == false ) then 
			IncrementTownLoyalty( Town.CAMBRIA, LOYALTY_BONUS_MARTHA_WHEN_JOEY_RESCUED )
		end
	end

    -- Hillbilly problem solved
	if ubNPCProfileId == Profil.KEITH then
		IncrementTownLoyalty( Town.CAMBRIA, LOYALTY_BONUS_KEITH_WHEN_HILLBILLY_SOLVED )
	end

    -- Chalice of Chance returned to Chitzena
	if  ubNPCProfileId == Profil.YANNI then
	
		IncrementTownLoyalty( Town.CHITZENA, LOYALTY_BONUS_YANNI_WHEN_CHALICE_RETURNED_LOCAL )
	
		-- NOTE: This affects Chitzena,too, a second time, so first value is discounted for it
		IncrementTownLoyaltyEverywhere( LOYALTY_BONUS_YANNI_WHEN_CHALICE_RETURNED_GLOBAL )
	end

    -- Bloodcats killed
	if ubNPCProfileId == Profil.AUNTIE  then
		IncrementTownLoyalty( Town.ALMA, LOYALTY_BONUS_AUNTIE_WHEN_BLOODCATS_KILLED )
	end

    -- Brother Dynamo freed
	if ubNPCProfileId == Profil.MATT then
		IncrementTownLoyalty( Town.ALMA, LOYALTY_BONUS_MATT_WHEN_DYNAMO_FREED )
	end	

end