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
--[[
Przyklad :
function MakeBadSectorListFromMapsOnHardDrive ()

	-- O15_b1
	if ( FindUnderGroundSector ( 15, SectorY.MAP_ROW_O , 1 ) == true and FindUnderGroundSectorVisited ( 15, SectorY.MAP_ROW_O , 1 ) == false ) then 
		AddEnemyToUnderGroundSector (15,SectorY.MAP_ROW_O ,1,0,32,0)
	end
	
	-- O14_b1
	if ( FindUnderGroundSector ( 15, SectorY.MAP_ROW_P, 1 ) == true and FindUnderGroundSectorVisited ( 15, SectorY.MAP_ROW_P , 1 ) == false ) then 
		AddEnemyToUnderGroundSector (15 ,SectorY.MAP_ROW_P, 1 ,0 ,32, 0)
	end

end
]]
function MakeBadSectorListFromMapsOnHardDrive ()



end
	