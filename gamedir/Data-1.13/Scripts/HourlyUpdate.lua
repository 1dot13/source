Facts = 
{
	FACT_BROTHEL_OPEN = 251,
	FACT_MUSEUM_OPEN = 250,
	FACT_CLUB_OPEN = 252,
	FACT_PAST_CLUB_CLOSING_AND_PLAYER_WARNED = 107,
}

local p = 0

function HourlyQuestUpdate()

	if (cHour == 4) then
		SetFactFalse( Facts.FACT_BROTHEL_OPEN )
	elseif (cHour == 20) then
		SetFactTrue( Facts.FACT_BROTHEL_OPEN )
	end

	-- Bar/night club
	if ( cHour == 15 ) then
	
		SetFactTrue( Facts.FACT_CLUB_OPEN )
		SetFactFalse( Facts.FACT_PAST_CLUB_CLOSING_AND_PLAYER_WARNED )

		-- Reset boxes fought
		for i = 0,2 do
			-- Set false
			gfBoxerFought(i,false)
		end

		-- If # of boxing matches the player has won is a multiple of
		-- 3, and the boxers haven't rested, then make them rest
		
		if ( gfBoxersResting == true ) then
			
			-- Done resting now!
			SetgfBoxersResting(false)
			SetgubBoxersRests(gubBoxersRests + 1)
			
		elseif ( gubBoxingMatchesWon / 3 > gubBoxersRests ) then
			-- Time for the boxers to rest!
			SetgfBoxersResting(true)
		end

	elseif ( cHour == 2 ) then
		SetFactFalse( Facts.FACT_CLUB_OPEN )
	end

	-- Museum
	if ( cHour == 9 ) then
		SetFactTrue( Facts.FACT_MUSEUM_OPEN )
	elseif ( cHour == 18 ) then
		SetFactFalse( Facts.FACT_MUSEUM_OPEN )
	end

end
