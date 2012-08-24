Facts = {
	FACT_BROTHEL_OPEN = 251,
	FACT_MUSEUM_OPEN = 250,
	FACT_CLUB_OPEN = 252,
	FACT_PAST_CLUB_CLOSING_AND_PLAYER_WARNED = 107,
}

local p = 0

function HourlyQuestUpdate()

-- JA2 
--[[
	if (cHour >= 4 or cHour < 20) then
		SetFactFalse( Facts.FACT_BROTHEL_OPEN )
	else
		SetFactTrue( Facts.FACT_BROTHEL_OPEN )
	end


-- bar/nightclub
	if ( cHour > 15 or cHour < 2) then
	
		SetFactTrue( Facts.FACT_CLUB_OPEN )
		SetFactFalse( Facts.FACT_PAST_CLUB_CLOSING_AND_PLAYER_WARNED )

		-- reset boxes fought
		for i = 0,3 do
			-- set false
			gfBoxerFought(i,false)
		end

		-- if # of boxing matches the player has won is a multiple of
		-- 3, and the boxers haven't rested, then make them rest
		
		if ( gfBoxersResting == true ) then
			
			-- done resting now!
			gfBoxersResting = false
			gubBoxersRests = gubBoxersRests + 1
			
			p = gubBoxingMatchesWon / 3
			
		elseif ( p > gubBoxersRests ) then
			-- time for the boxers to rest!
			 gfBoxersResting = true
		end

		
	else
		SetFactFalse( Facts.FACT_CLUB_OPEN )
	end

-- museum
if ( cHour >= 9 or cHour < 18 ) then
	SetFactTrue( Facts.FACT_MUSEUM_OPEN )
else
	SetFactFalse( Facts.FACT_MUSEUM_OPEN )
end
]]
end