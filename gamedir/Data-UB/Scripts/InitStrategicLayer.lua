-- SetNumberOfJa25BloodCatsInSector ( SectorX, SectorY, bNumBloodCats, bNumBloodCatPlacements )

local DIF_LEVEL_EASY = 1
local DIF_LEVEL_MEDIUM = 2
local DIF_LEVEL_HARD = 3
local DIF_LEVEL_INSANE = 4

function InitStrategicLayer ()

		-- I10 BloodCats
		if ( difficultyLevel == DIF_LEVEL_EASY ) then
			SetNumberOfJa25BloodCatsInSector ( 10, 9, 3, 5 )
		elseif ( difficultyLevel == DIF_LEVEL_MEDIUM ) then
			SetNumberOfJa25BloodCatsInSector ( 10, 9, 4, 5 )
		elseif ( difficultyLevel == DIF_LEVEL_HARD ) then	
			SetNumberOfJa25BloodCatsInSector ( 10, 9, 5, 5 )
		elseif ( difficultyLevel == DIF_LEVEL_INSANE ) then
			SetNumberOfJa25BloodCatsInSector ( 10, 9, 5, 5 )
		end
			
end