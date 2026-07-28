-- SetNumberOfJa25BloodCatsInSector ( SectorX, SectorY, bNumBloodCats, bNumBloodCatPlacements )

local DIF_LEVEL_EASY = 1
local DIF_LEVEL_MEDIUM = 2
local DIF_LEVEL_HARD = 3
local DIF_LEVEL_INSANE = 4

function InitStrategicLayer ()

		-- I10 BloodCats
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
			UB_SetNumberOfJa25BloodCatsInSector ( 10, 9, 3, 5 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
			UB_SetNumberOfJa25BloodCatsInSector ( 10, 9, 4, 5 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
			UB_SetNumberOfJa25BloodCatsInSector ( 10, 9, 5, 5 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
			UB_SetNumberOfJa25BloodCatsInSector ( 10, 9, 5, 5 )
		end
			
end