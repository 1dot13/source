--[[
For inner workings, see MiniEvents.h/cpp

It is *HIGHLY* recommend that you make backup of this file before modifying it.

Main entry points are at the bottom of this file: BeginRandomEvent() and BeginSpecificEvent()
You will probably only need to modify the Events and HiddenEvents tables.

Available calls:
CResolveEvent(text, nextEventId, hoursToNextEvent)
	- this is the exit handler for an event.
	- this MUST be called at the end of each event's Resolution function.
	- text appears in the message box after the player clicks on either button. It will be truncated to 450 characters.
	- nextEventId is optional. If specified, the next mini event will be triggered through BeginSpecificEvent() instead of BeginRandomEvent(), with this value as the eventId parameter.
	- hoursToNextEvent is optional. If specified, the next mini event will trigger in this many hours (must be an integer greater than zero). This timer IGNORES the minimum/maximum time between events defined by the user in JA2_Options.ini.
	- does not return anything
CScreenMsg(text)
	- text appears in the message log (bottom left in the strategic view)
	- does not return anything

CAddIntel(amount) returns {true/false}
	- amount can be positive (for credits) or negative (for debits)
	- if the player has Intel disabled, this instead adds or subtracts cash, at an exchange rate of 1 intel = $500
	- returns true on a successful transaction and false on an unsuccessful transaction
CAddMoneyToPlayerAccount(amount, negativeZeroesAccount) returns {true/false}
	- amount can be positive (for credits) or negative (for debits)
	- negativeZeroesAccount is optional. if true, a negative transaction will always succeed, setting the player's balance to zero if the debit exceeds it.
	  if negativeZeroesAccount is omitted, or false, the transaction will fail if the player's balance is less than the debit.
	- returns true on a successful transaction and false on an unsuccessful transaction
CAddSkill(profileId, skillId)
	- adds a skill to the specified merc if allowable. ie, you can't add auto weapons to a merc with two levels of it already.
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- skillId must be a value from the SKILLS enum below
	- does not return anything
CAddTownLoyalty(townId, amount)
	- townId must be a value from the TOWNS enum below
	- amount can be positive or negative
	- note that this is not percentage change. This applies a number of "points" to the town's loyalty, and the amount of points required for a 1% change may be
	  slightly different for each town (see Cities.xml, townRebelSentiment for each town's modifier).
	- does not return anything
CAdjustBreathMax(profileId, amount)
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- amount can be positive or negative
	- does not return anything
CAdjustEnemyStrengthInSector(x, y, admin, troop, elite, robot, jeep, tank)
	- adjusts the number of enemy types in the specified sector
	- x, y are sector coordinates (z will be 0)
	- admin/troop/elite/robot/jeep/tank are additions or subtractions to the sector
	- the number of each enemy is bound between 0 and 30 (ie, you can have 20 admins and 20 troops, but not 40 admins)
	- if the ASD is disabled, robots/jeeps/tanks will be set to 0
	- does not return anything
CAdjustMorale(profileId, amount)
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- amount can be any nonzero positive or negative value
	- does not return anything
CAdjustStat(profileId, stat, amount)
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- stat must be a value from the STATS enum below
	- amount can be positive or negative
	- negative amounts can be healed by doctoring. Note that only physical stats can be healed (LIFE, STRENGTH, DEXTERITY, AGILITY, WISDOM).
	- experience level cannot be modified.
	- does not return anything
CAdjustVehicleFuel(profileId, amount) returns {success, vehicleName}
	- adds or subtracts fuel from a vehicle
	- profileId is a merc's ID, and is used to determine which vehicle to affect (meaning that the merc has to be sitting in a vehicle)
	- amount can be positive (add fuel) or negative (lose fuel)
	- returns {success, vehicleName}, where success is true/false, and vehicleName is the name of the affected vehicle
CAdjustVehicleHealth(profileId, amount) returns {success, vehicleName}
	- repairs or damages a vehicle
	- profileId is a merc's ID, and is used to determine which vehicle to affect (meaning that the merc has to be sitting in a vehicle)
	- amount can be positive (repair/gain hp) or negative (damage/lose hp)
	- returns {success, vehicleName}, where success is true/false, and vehicleName is the name of the affected vehicle
CApplyPermanentStatDamage(profileId, stat, amount)
	- this reduces a merc's stats. Unlike CAdjustStat, stats reduced by this call CANNOT be healed through surgery (doctoring or visiting the hospital). 
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- stat must be a value from the STATS enum below
	- amount must be positive, or this will have no effect
	- does not return anything
CApplyDamage(profileId, amount)
	- this reduces a merc's current health
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- amount can be positive or negative. If positive, damages a merc. If negative, heals a merc.
	- this can kill a merc if their health is reduced to 0!
	- does not return anything
CCheckForAssignment(assignmentId, x, y, z) returns {true/false, nickname, id}
	- assignmentId must be a value from the ASSIGNMENTS enum below
	- x, y, z indicate the sector coordinates to check. These are optional. If excluded, searches through all mercs.
	- special case: you can check for ASSIGNMENTS.ON_DUTY and this will return true if any merc is in a squad (ie, SQUAD_1 through SQUAD_20)
	- returns true if ANY merc in the sector is on the specified assignment, false otherwise. nickname is the name of a random merc on the assignment, and id is their profileId.
CCheckForSkill(skillId) returns {result, nickname, id}
CCheckForSkill(skillId, x, y, z) returns {result, nickname, id}
CCheckForSkill(skillId, profileId) returns {result, nickname, id}
	- new trait system only
	- skillId must be a value from the SKILLS enum below
	- x, y, z indicate the sector coordinates to check. These are optional. If excluded, searches through all mercs.
	- profileId is optional. If included, only checks the specified merc.
	- returns {result, nickname, id}, where result is true if ANY merc has the specified skill, false otherwise. nickname is the name of a random merc with the skill, and id is their profileId.
CCheckForSleep(profileId) returns {true/false}
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- returns true if the merc with the specified profileId is sleeping
CCheckForTravel(profileId) returns {true/false}
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- returns true if the merc with the specified profileId is travelling between sectors
CCheckForTravelOnFoot(profileId) returns {true/false}
	- returns true if the merc with the specified profileId is travelling between sectors on foot
CCheckForTravelInHelicopter(profileId) returns {true/false}
	- returns true if the merc with the specified profileId is travelling between sectors in a helicopter
CCreateMilitia(greenMilitia, regularMilitia, eliteMilitia, x, y)
	- input parameters must be positive and indicate how much of each type of militia to create
	- militia will be created in the sector of the affected mercs
	- x, y indicate the sector coordinates to create the militia. Note that Z is excluded as militia cannot be placed underground.
	- does not return anything
CGetCoordinates(profileId) returns {x, y, z}
	- returns the x/y/z coordinates of the merc with the specified profileId
CGetHealth(profileId) returns {currentLife, maxLife}
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- returns the current and max health of the specified merc. If no merc could be found, returns { 0, 0 }
CGetHoursRemainingOnMiniEvent(profileId) returns {hours}
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- returns the hours remaining. -1 if the profileId does not match any mercs in the user's party
CGetSectorIDString(x, y, z) returns {sectorIdString}
	- x, y, z indicate the sector coordinates to check
	- returns a string containing the sector code and the name of the sector. This is the same as what the player sees in the bottom right corner of the strategic view.
CGetSkills(profileId)
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- returns a table of the merc's skills, indexed as an array (eg, { [1] = SKILLS.AUTO_WEAPONS, [2] = SKILLS.ATHLETICS })
CGetStat(stat) returns {statValue, nickname, id}
CGetStat(stat, x, y, z) returns {statValue, nickname, id}
CGetStat(stat, profileId) returns {statValue, nickname, id}
	- stat must be a value from the STATS enum below
	- note that LIFE refers to max health, not current health
	- x, y, z indicate the sector coordinates to check
	- profileId is optional. If specified, gets the stat of that merc. If not, gets the highest stat of any merc in the specified sector.
	- returns the value of the specified stat, the nickname of that merc, and the merc's profileId. The nickname and id are returned regardless of whether you input a profileId or not.
CGetProgress() returns {progress}
	- returns a value between 0 (just started the game) and 100 (at/near endgame)
CGetTownId(x, y) returns {townId, townName}
	- x, y indicate the sector coordinates to check
	- returns a town id, which will match up with the TOWNS enum below. Also returns the name of the town as defined in Cities.xml
CSendMercOnMiniEvent(profileId, hours)
	- if called with CSetMercCoordinates(), this should be called SECOND.
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- hours is the amount of time that the specified merc will be unavailable. Overrides any existing hours. Valid range is 1-65535.
	- does not return anything
CSetEnemyGroupVisibility(flag, x, y)
	- x, y are sector coordinates to flag, and are optional.
	- if x and y are not provided, this affects every sector on the map. otherwise, visibility is only flagged for the specified sector
	- visibility will be turned off automatically every hour (:00) as the visibility flag is shared with facilities. this means that a facility that generates sector visibility will be affected for at most 1 hour.
	- flag is either true or false
	- does not return anything
CSetMercCoordinates(profileId, x, y, z)
	- if called with CSendMercOnMiniEvent(), this should be called FIRST.
	- profileId is a merc's ID, which was passed in through BeginRandomEvent or BeginSpecificEvent
	- x, y, z are the coordinates to move to. Make sure the coordinates are valid! For z, 0 is ground level, 1 is 1 level below ground, etc.
	- does not return anything



Events should be formatted as follows:
	[<index>] = -- index must be a positive nonzero integer
	{
		INIT = function(mercList)
			-- in here, you can define how an event behaves. Note that mercList contains *all* of the player's mercs, formatted { nickname = profileId }
			-- this function MUST return a table with four elements: BodyText, TopButtonText, BottomButtonText, and Resolution
			-- Resolution is a function which contains one input parameter: a boolean indicating whether the player clicked the top button or not
			-- *IMPORTANT* Resolution MUST call CResolveEvent() at some point. This call wraps up the event and queues up the next one. If an integer value is passed as CResolveEvent's second parameter,
			   then the next mini event will be called through BeginSpecificEvent.
			-- example:

			return {
				BodyText = "This text appears in the main message box body.",
				TopButtonText = "This is the top button's text.",
				BottomButtonText = "This is the bottom button's text.",
				Resolution = function(result)
					if result then
						CResolveEvent("You clicked the top button")
					else
						CResolveEvent("You clicked the bottom button", 3)
					end
				end
			}
		end
	}

You can use tables to categorize and organize events as you see fit (eg for solo events, chained events, group events, daytime/nighttime events, etc).

Please ensure that index is unique for each event. Indices should also be sequential to make randomization easier.
TopButtonText and BottomButtonText will be truncated to 60 characters.
BodyText will be truncated to 450 characters.
The string in CResolveEvent will be truncated to 450 characters.
If you specify a second parameter in CResolveEvent, the next mini event will be triggered through BeginSpecificEvent with that value as its first parameter (eventId).

]]

-- enums follow. Please do not modify these unless there have been code changes.
local ASSIGNMENTS =
{
	SQUAD_1 = 0,
	SQUAD_2 = 1,
	SQUAD_3 = 2,
	SQUAD_4 = 3,
	SQUAD_5 = 4,
	SQUAD_6 = 5,
	SQUAD_7 = 6,
	SQUAD_8 = 7,
	SQUAD_9 = 8,
	SQUAD_10 = 9,
	SQUAD_11 = 10,
	SQUAD_12 = 11,
	SQUAD_13 = 12,
	SQUAD_14 = 13,
	SQUAD_15 = 14,
	SQUAD_16 = 15,
	SQUAD_17 = 16,
	SQUAD_18 = 17,
	SQUAD_19 = 18,
	SQUAD_20 = 19,
	ON_DUTY = 20,
	DOCTOR = 21,
	PATIENT = 22,
	VEHICLE = 23,
	IN_TRANSIT = 24,
	REPAIR = 25,
	RADIO_SCAN = 26,
	TRAIN_SELF = 27,
	TRAIN_TOWN = 28,
	ASSIGNMENT_UNUSED = 29,
	TRAIN_TEAMMATE = 30,
	TRAIN_BY_OTHER = 31,
	MOVE_EQUIPMENT = 32,
	FACILITY_STAFF = 33,
	FACILITY_EAT = 34,
	FACILITY_REST = 35,
	FACILITY_INTERROGATE_PRISONERS = 36,
	ASSIGNMENT_DEAD = 37,
	ASSIGNMENT_UNCONCIOUS = 38, -- unused
	ASSIGNMENT_POW = 39,
	ASSIGNMENT_HOSPITAL = 40,
	ASSIGNMENT_EMPTY = 41,
	FACILITY_PRISON_SNITCH = 42,
	FACILITY_SPREAD_PROPAGANDA = 43,
	FACILITY_SPREAD_PROPAGANDA_GLOBAL = 44,
	FACILITY_GATHER_RUMOURS = 45,
	SNITCH_SPREAD_PROPAGANDA = 46,
	SNITCH_GATHER_RUMOURS = 47,
	FACILITY_STRATEGIC_MILITIA_MOVEMENT = 48,
	DISEASE_DIAGNOSE = 49,
	DISEASE_DOCTOR_SECTOR = 50,
	FACILITY_DOCTOR = 51,
	FACILITY_PATIENT = 52,
	FACILITY_REPAIR = 53,
	FORTIFICATION = 54,
	TRAIN_WORKERS = 55,
	CONCEALED = 56,
	GATHERINTEL = 57,
	DOCTOR_MILITIA = 58,
	DRILL_MILITIA = 59,
	BURIAL = 60,
	ADMINISTRATION = 61,
	EXPLORATION = 62,
	ASSIGNMENT_MINIEVENT = 63,
}

local SKILLS =
{
	AUTO_WEAPONS = 1,
	HEAVY_WEAPONS = 2,
	MARKSMAN = 3,
	HUNTER = 4,
	GUNSLINGER = 5,
	HAND_TO_HAND = 6,
	DEPUTY = 7,
	TECHNICIAN = 8,
	PARAMEDIC = 9,
	AMBIDEXTROUS = 10,
	MELEE = 11,
	THROWING = 12,
	NIGHT_OPS = 13,
	STEALTHY = 14,
	ATHLETICS = 15,
	BODYBUILDING = 16,
	DEMOLITIONS = 17,
	TEACHING = 18,
	SCOUTING = 19,
	COVERT_OPS = 20,
	RADIO_OPERATOR = 21,
	SNITCH = 22,
	SURVIVAL = 23,
}

local STATS =
{
	LIFE = 0,
	STRENGTH = 1,
	AGILITY = 2,
	DEXTERITY = 3,
	WISDOM = 4,
	LEADERSHIP = 5,
	MARKSMANSHIP = 6,
	MECHANICAL = 7,
	EXPLOSIVE = 8,
	MEDICAL = 9,
	EXPLEVEL = 10,
}

-- This enum needs to match the uiIndex of each town as defined in Cities.xml
local TOWNS = 
{
	NO_TOWN  = 0,
	OMERTA   = 1,
	DRASSEN  = 2,
	ALMA     = 3,
	GRUMM    = 4,
	CAMBRIA  = 6,
	SANMONA  = 7,
	ESTONI   = 8,
	BALIME   = 10,
	MEDUNA   = 11,
	CHITZENA = 12,
}
-- end enums

-- Mini event variables and functions follow. Please do not modify these unless you know what you are doing.
math.randomseed(os.time())
local eventResolver

-- initialize the mini event and show the user a popup box
function ShowMsgBox(event)
	eventResolver = event.Resolution
	CMsgBox(event.TopButtonText, event.BottomButtonText, event.BodyText)
end


-- resolve the mini event
function ResolveMsgBox(result)
	if eventResolver ~= nil then
		eventResolver(result)
	end
end

function GetMercIdFromName(mercList, name)
	for k, v in pairs(mercList) do
		if name == k then
			return v
		end
	end

	return -1
end

function GetRandomMerc(mercList)
	local selectedName, selectedProfileId
	local count = 0
	for name, profileId in pairs(mercList) do
		count = count + 1
		if math.random() <= 1/count then
			selectedName = name
			selectedProfileId = profileId
		end
	end

	return selectedName, selectedProfileId
end

function Get2RandomMercs(mercList)
	local count = 0
	for k, v in pairs(mercList) do
		count = count + 1
	end

	if count <= 2 then
		return mercList
	end

	local ret = {}

	local r1 = math.random(1, count)
	local r2 = math.random(1, count-1)
	if r1 == r2 then
		r2 = r2 + 1
	end

	count = 1
	for k, v in pairs(mercList) do
		if r1 == count then
			ret[1] = { Name = k, Id = v }
		elseif r2 == count then
			ret[2] = { Name = k, Id = v }
		end
		count = count + 1
	end

	return ret
end

function GetAllMercsInSector(mercList, x, y, z)
	local mercsInSector = {}
	for name,id in pairs(mercList) do
		local xx, yy, zz = CGetCoordinates(id)
		if x == xx and y == yy and z == zz then
			mercsInSector[name] = id
		end
	end

	return mercsInSector
end

function GetAllMercsInRandomSector(mercList)
	local name, id = GetRandomMerc(mercList)
	local x, y, z = CGetCoordinates(id)
	local mercs = GetAllMercsInSector(mercList, x, y, z)

	return mercs, { X = x, Y = y, Z = z }
end

function GetMercsInTown(mercList)
	local mercsInTown = {}

	for name, id in pairs(mercList) do
		x, y, z = CGetCoordinates(id)
		townId, _ = CGetTownId(x, y)

		if townId == TOWNS.OMERTA
		or townId == TOWNS.DRASSEN
		or townId == TOWNS.ALMA
		or townId == TOWNS.GRUMM
		or townId == TOWNS.CAMBRIA
		or townId == TOWNS.SANMONA
		or townId == TOWNS.ESTONI
		or townId == TOWNS.BALIME
		or townId == TOWNS.MEDUNA
		or townId == TOWNS.CHITZENA then
			mercsInTown[name] = id
		end
	end

	return mercsInTown
end

function GetLargestMercGroup(mercList)
	local bestCount = 0
	local bestSector = {x=0, y=0, z=0}

	for name, id in pairs(mercList) do
		local x, y, z = CGetCoordinates(id)
		local mercs = GetAllMercsInSector(mercList, x, y, z)
		local count = 0 
		for k,v in pairs(mercs) do
			count = count + 1
		end

		if count > bestCount then
			bestCount = count
			bestSector = { x=x, y=y, z=z }
		end
	end

	return GetAllMercsInSector(mercList, bestSector.x, bestSector.y, bestSector.z)
end
-- end mini event variables and functions

-- Events follow
Events =
{
	-- note that 0 is not a valid index!
	[1] = -- berries
	{
		INIT = function(mercList)
			local mercs, sector = GetAllMercsInRandomSector(mercList)
			local sectorStr = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			local BODY = string.format("[%s] 你们遇到一片长着红色浆果的灌木丛。这些浆果看起来很好吃，但是众人犹豫了，它们可能有毒。", sectorStr)
			local BTN1 = "吃浆果。"
			local BTN2 = "不吃浆果。"
			local RESOLVE = function(topButton)
				if (topButton) then
					local result, skillOwnerName, skillOwnerId = CCheckForSkill(SKILLS.SURVIVAL, sector.X, sector.Y, sector.Z)

					if result then 
						CResolveEvent(string.format("[生存者][无事发生] 你摘了个果子刚要张嘴， %s 凭借丰富的生存经验认出了这是有毒的浆果，大声的喝止了你们。", skillOwnerName))
					else
						for name, id in pairs(mercs) do
							CAdjustBreathMax(id, -25)
						end
						CResolveEvent("[全体: -精力] 你们狼吞虎咽的吃着浆果。这是你们吃过的最美味的浆果。也是你们一天中做出的最佳决定。。。也许不是。你们开始上吐下泻，很快就精疲力尽了。")
					end
				else
					CResolveEvent("[无事发生] 谨慎起见，你们克制了自己的食欲，继续行军。")
				end
			end

			return { TopButtonText = BTN1, BottomButtonText = BTN2, BodyText = BODY, Resolution = RESOLVE }
		end
	},
	[2] = -- berries
	{
		INIT = function(mercList)
			local mercs, sector = GetAllMercsInRandomSector(mercList)
			local sectorStr = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们遇到一片长着绿色浆果的灌木丛。这些浆果看起来很诱人，但是众人犹豫了，它们可能有毒。", sectorStr),
				TopButtonText = "吃浆果。",
				BottomButtonText = "不吃浆果。",
				Resolution = function(topButton)
					if (topButton) then
						for name, id in pairs(mercs) do
							CAdjustMorale(id, 3)
						end
						CResolveEvent("[全体: +士气] 你们狼吞虎咽的吃着浆果。这是你们吃过的最美味的浆果。也是你们一天中做出的最好决定。")
					else
						CResolveEvent("[无事发生] 谨慎起见，你们克制了自己的食欲，继续行军。")
					end
				end
			}
		end
	},
	[3] = -- puppy
	{
		INIT = function(mercList)
			local mercs, sector = GetAllMercsInRandomSector(mercList)
			local sectorStr = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 附近灌木丛中发出窸窸窣窣的响声，你举起一只拳头示意队员们进入警戒状态。突然，一只小狗从灌木里跑出来，摇着尾巴向你吠叫。这种地方怎么会有一只小狗？", sectorStr),
				TopButtonText = "让小狗留在原地。",
				BottomButtonText = "寻找狗主人。",
				Resolution = function(topButton)
					if topButton then
						CResolveEvent("[无事发生] 在确认周围安全无误后，你们把小狗留在原地，继续行军。")
					else
						name, id = GetRandomMerc(mercs)
						for k, v in pairs(mercs) do
							CAdjustMorale(v, 2)
							CAdjustBreathMax(v, -10)
						end
						
						CResolveEvent(string.format("[全体: -精力, +士气] 你们花了一些时间寻找狗的主人，直到你们遇到了一个大声寻找小狗的年轻女孩。小狗从 %s 的怀里跳出来，向她跑去。女孩如释重负，向你们道谢后带着狗回镇子里了。", name))
					end
				end
			}
		end
	},
	[4] = -- lockbox
	{
		INIT = function(mercList)
			local mercs, sector = GetAllMercsInRandomSector(mercList)
			local sectorStr = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			mecVal, mecName = CGetStat(STATS.MECHANICAL, sector.X, sector.Y, sector.Z)
			expVal, expName = CGetStat(STATS.EXPLOSIVE, sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 草丛里的金属光泽引起了你们的注意，靠近后发现是一个保险箱。它锈迹斑斑，看上去已经在那有些年头了。你们互相保持戒备，这有可能是个陷阱。", sectorStr),
				TopButtonText = string.format("让 %s 开锁。", mecName),
				BottomButtonText = string.format("让 %s 炸掉箱子。", expName),
				Resolution = function(topButton)
					if topButton then
						if mecVal >= 75 then
							id = GetMercIdFromName(mercs, mecName)
							CAddIntel(50)
							if id ~= -1 then
								CAdjustStat(id, STATS.MECHANICAL, 1)
							end
							
							CResolveEvent("[成功, +情报, +机械] 保险箱被打开了，里面是保存完好的文件。文件里包含了大量政府高层的机密信息，看起来非常有利用价值。")
						else
							id = GetMercIdFromName(mercs, mecName)
							if id ~= -1 then
								CAdjustMorale(id, -1)
							end

							CResolveEvent(string.format("[失败][-士气] %s 尝试了无数次也没能打开箱子，最终他愤怒的朝锁开了几枪。轰开后发现里面是一堆满是弹孔的杂乱文件。", mecName))
						end
					else
						id = GetMercIdFromName(mercs, expName)

						if id ~= -1 then
							CAdjustStat(id, STATS.EXPLOSIVE, 1)
						end

						if expVal >= 70 then
							CResolveEvent(string.format("[成功, +爆破] %s 挥手示意让你们躲远点，随后按下了引爆开关。随着一声巨响，这个保险箱再也不会困扰任何人了。", expName))
						else
							CApplyDamage(id, 15)
							CResolveEvent(string.format("[不算成功, +爆破, -生命] 设置好炸弹后，%s 兴奋的按下引爆按钮，然而什么事也没发生。如果他经验再多一点也许就不会做出这种蠢事————他直接过去检查炸弹。炸弹在他快走到时爆炸了。%s 被冲击波掀翻在地，身受重伤。谢天谢地，你们没有靠近。", expName, expName))
						end
					end
				end
			}
		end
	},
	[5] = -- defectors
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们在荒野中遭遇了一小群神情慌张的士兵，你举起枪对准他们，他们即刻举起双手喊到\"别开枪!我们想要投诚！\" 说完就安静的站在原地。", sectorIdString),
				TopButtonText = "相信他们，让他们加入民兵。",
				BottomButtonText = "这是个陷阱，对他们开枪。",
				Resolution = function(topButton)
					if topButton then
						CCreateMilitia(math.random(2,5), math.random(2,5), math.random(0,2), sector.X, sector.Y)
						CResolveEvent("[+民兵] 听到你的回答后，他们松了一口气，士兵们争先恐后的向你做自我介绍，其中一人说\"上帝保佑，我们现在都是你的人了！\"")
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(0,20))
						end

						CResolveEvent("[全体: -生命] 士兵们话音刚落，你们就扣动了扳机，突如其来的火力迅速将他们击毙。枪声惊动了周围的敌人，下一场交火接踵而至。")
					end
				end
			}
		end
	},
	[6] = -- rebel runner
	{
		INIT = function(mercList)
			mercs = Get2RandomMercs(mercList)

			return {
				BodyText = string.format("一名反抗军信使捎来 Miguel 的口信：要你派遣一名佣兵帮助他执行秘密任务。信使除了确信任务不涉及射击外不知道其他细节，他还说 Miguel 不希望你拒绝他。"),
				TopButtonText = string.format("派 %s 去。", mercs[1].Name),
				BottomButtonText = string.format("派 %s 去。", mercs[2].Name),
				Resolution = function(topButton)
					CAddIntel(20)

					if topButton then
						CAdjustStat(mercs[1].Id, STATS.STRENGTH, 1)
						CAdjustStat(mercs[1].Id, STATS.DEXTERITY, 1)
						CSendMercOnMiniEvent(mercs[1].Id, 8)
						CResolveEvent(string.format("[+情报, %s: +力量, +灵巧] 这是一件帮反抗军做政治宣传的工作，闲暇时也帮忙干一些零活。虽然和想象中的不太一样，但 %s 还是因此获得了一些历练。", mercs[1].Name, mercs[1].Name))
					else
						CAdjustStat(mercs[2].Id, STATS.STRENGTH, 1)
						CAdjustStat(mercs[2].Id, STATS.DEXTERITY, 1)
						CSendMercOnMiniEvent(mercs[2].Id, 8)
						CResolveEvent(string.format("[+情报, %s: +力量, +灵巧] 这是一件帮反抗军做政治宣传的工作，闲暇时也帮忙干一些零活。虽然和想象中的不太一样，但 %s 还是因此获得了一些历练。", mercs[2].Name, mercs[2].Name))
					end
				end
			}
		end
	},
	[7] = -- bandit ambush
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 一大群匪徒突然从荒野中杀出，将你们团团包围，双方举着武器紧张对峙着。此时一个权威的声音从匪徒中传来\"嘿！大伙放下家伙，让他们走。这些是硬茬儿，硌牙得很。\"匪徒头子说完话向你眨了眨眼。", sectorIdString),
				TopButtonText = "剿灭匪徒。",
				BottomButtonText = "迅速离开。",
				Resolution = function(topButton)
					if topButton then
						name, id = GetRandomMerc(mercs)
						CAdjustStat(id, STATS.MARKSMANSHIP, 1)

						for k,v in pairs(mercs) do
							CApplyDamage(v, math.random(0,20))
						end

						CResolveEvent(string.format("[全体: -生命, %s: +枪法] 听到老大的命令，匪徒们放下武器让开道路。你们趁匪徒们放松的瞬间开火，一些匪徒立马夺路而逃，而另一些匪徒则奋起反击，战至最后一人。你们经历了一场血战。", name))
					else
						CResolveEvent("[无事发生] 匪徒们虽然有些失望，但还是乖乖让开道路，为迎接一下个遇害者做准备。")
					end
				end
			}
		end
	},
	[8] = -- airsick
	{
		INIT = function(mercList)
			local foundMercs = {}
			local count = 0
			for k,v in pairs(mercList) do
				result = CCheckForTravelInHelicopter(v)
				if result then
					foundMercs[k] = v
					count = count + 1
				end
			end

			-- if only one merc's flying, do something else
			if count <= 1 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			name, id = GetRandomMerc(foundMercs)

			return {
				BodyText = string.format("[直升机] \"我没想到你会晕机，头儿！！\" Skyrider 坐在直升机前面轻笑着。%s 想说点什么，却什么也说不出，他脑子里的嗡嗡声比飞机的隆隆声还大。", name),
				TopButtonText = string.format(" %s 忍住了。", name),
				BottomButtonText = string.format(" %s 吐了。", name),
				Resolution = function(topButton)
					if topButton then
						CAdjustBreathMax(id, -50)
						CResolveEvent(string.format("[%s: -精力] Skyrider 努力让飞机飞的更加平稳，%s 的脸色比幽灵还苍白。依靠强大的意志力，最终控制住了呕吐的欲望。", name, name))
					else
						for k,v in pairs(foundMercs) do
							if v == id then
								CAdjustMorale(id, -15)
								CAdjustBreathMax(id, -25)
							else
								CAdjustMorale(v, 5)
							end
						end

						CResolveEvent(string.format("[%s: -精力, -士气,其他人: +士气] 尽管 %s 尽了最大努力，他还是没能控制住自己的胃，呕吐物像暴风骤雨一样向机舱中的每个人喷射。这件事最终沦为了佣兵们闲暇时谈论的笑料。", name, name))
					end
				end
			}
		end
	},
	[9] = -- lost on foot
	{
		INIT = function(mercList)
			local foundMercs = {}
			local count = 0
			for k,v in pairs(mercList) do
				result = CCheckForTravelOnFoot(v)
				if result then
					foundMercs[k] = v
					count = count + 1
				end
			end

			-- if nobody's walking around, do something else
			if count == 0 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			name,id = GetRandomMerc(foundMercs)
			x, y, z = CGetCoordinates(id)

			mercsOnFoot = GetAllMercsInSector(mercList, x, y, z)
			sectorIdString = CGetSectorIDString(x, y, z)

			return {
				BodyText = string.format("[%s] 你们为了寻找一条捷径走到了一处人迹罕至的地点，再往前走有可能会迷路。或者考虑往回走，但没准前面真的是一条捷径呢？", sectorIdString),
				TopButtonText = "继续前进。",
				BottomButtonText = "往回走。",
				Resolution = function(topButton)
					result, skillName, skillId = CCheckForSkill(SKILLS.SURVIVAL, x, y, z)

					if topButton then
						if result then
							for k,v in pairs(mercsOnFoot) do
								CAdjustBreathMax(v, -15)
							end

							CResolveEvent(string.format("[生存者][全体: -精力] 这不是一条捷径，但 %s 凭借丰富的野外生存经验最终还是把队伍带回到了正路上，不过这次长途跋涉让大家感到有些疲劳。", skillName))
						else
							lostTime = math.random(3,5)
							for k,v in pairs(mercsOnFoot) do
								CSendMercOnMiniEvent(v, lostTime)
							end

							CResolveEvent(string.format("[队伍不可用] 你们选择继续前进，过了一段时间你们意识到自己真的迷路了。。。要多花好几个小时才能找到回去的路。"))
						end
					else
						if result then
							CResolveEvent(string.format("[生存者][无事发生] %s 凭借丰富的野外生存经验以最快的速度带领你们回到了熟悉的老路。", name))
						else
							for k,v in pairs(mercsOnFoot) do
								CAdjustBreathMax(v, -20)
								CAdjustMorale(v, -5)
							end

							CResolveEvent("[全体: -精力, -士气] 寻找归途是一项漫长而艰巨的任务。当你们最终回到熟悉的地方时，已经精疲力尽，苦不堪言。")
						end
					end
				end
			}
		end
	},
	[10] = -- fisticuffs
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			count = 0
			for k,v in pairs(mercs) do
				count = count + 1
			end

			-- we need 2 combatants and at least one observer
			if count <= 3 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			fighters = Get2RandomMercs(mercs)

			for i=1,2 do
				result, _, __ = CCheckForSkill(fighters[1].Id, SKILLS.HAND_TO_HAND)

				if result then
					fighters[i].CombatStrength = 25
				else
					fighters[i].CombatStrength = 0
				end

				result, _ = CGetHealth(fighters[i].Id)
				fighters[i].CombatStrength = fighters[i].CombatStrength + result

				result = CGetStat(STATS.STRENGTH, sector.X, sector.Y, sector.Z, fighters[i].Id)
				fighters[i].CombatStrength = fighters[i].CombatStrength + result

				result = CGetStat(STATS.DEXTERITY, sector.X, sector.Y, sector.Z, fighters[i].Id)
				fighters[i].CombatStrength = fighters[i].CombatStrength + result

				result = CGetStat(STATS.AGILITY, sector.X, sector.Y, sector.Z, fighters[i].Id)
				fighters[i].CombatStrength = fighters[i].CombatStrength + result

				fighters[i].CombatStrength = fighters[i].CombatStrength + math.random(0,10) 
			end

			return {
				BodyText = string.format("[%s] 因为一点鸡毛蒜皮的小事 %s 和 %s 大吵起来，他俩看起来谁都不会先让步。", sectorIdString, fighters[1].Name, fighters[2].Name),
				TopButtonText = "让他俩打一架。",
				BottomButtonText = "奖励5000$给胜利者。",
				Resolution = function(topButton)
					if topButton then
						CApplyDamage(fighters[1].Id, math.random(10,20))
						CApplyDamage(fighters[2].Id, math.random(10,20))
						CResolveEvent(string.format("[ %s , %s : -生命] 这是一场激烈的混战，双方都发泄了心中压抑的不满，最终两人都挂了点彩，冷静下来了。", fighters[1].Name, fighters[2].Name))
					else
						CAddMoneyToPlayerAccount(-5000)
						winnerIndex = 1
						loserIndex = 2
						if fighters[1].CombatStrength < fighters[2].CombatStrength then
							winnerIndex = 2
							loserIndex = 1
						end

						CApplyDamage(fighters[1].Id, math.random(20,40))
						CApplyDamage(fighters[2].Id, math.random(20,40))
						CAdjustMorale(fighters[winnerIndex].Id, 15)
						CAdjustStat(fighters[1].Id, STATS.STRENGTH, 1)
						CAdjustStat(fighters[1].Id, STATS.AGILITY, 1)
						CAdjustStat(fighters[2].Id, STATS.STRENGTH, 1)
						CAdjustStat(fighters[2].Id, STATS.AGILITY, 1)
						CScreenMsg(string.format("支付5000$给 %s 当做获胜奖金。", fighters[winnerIndex].Name))

						CResolveEvent(string.format("[-$5000][%s: -生命, +力量, +敏捷, +士气, %s: -生命, +力量, +敏捷] 额外的奖金激发了双方的兽性，%s 和 %s 全力以赴，战斗最终以 %s 获胜而告终。", fighters[winnerIndex].Name, fighters[loserIndex].Name, fighters[1].Name, fighters[2].Name, fighters[winnerIndex].Name))
					end
				end
			}
		end
	},
	[11] = -- stalking bloodcats
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们看到远处有几只血猫在跟踪一群平民，虽然平民人数众多，但血猫还是能在被赶走前弄死几个人。", sectorIdString),
				TopButtonText = "警告平民。",
				BottomButtonText = "什么都不做，静静的看着。",
				Resolution = function(topButton)
					if topButton then
						bestMrk = 0 
						bestMrkName = ""
						bestMrkId = -1
						for name, id in pairs(mercs) do
							mrk, _, __ = CGetStat(STATS.MARKSMANSHIP, id)
							skill, _, __ = CCheckForSkill(SKILLS.MARKSMAN, id)
							if skill == true then
								mrk = mrk + 10
							end
							
							if mrk > bestMrk then
								bestMrk = mrk
								bestMrkName = name
								bestMrkId = id
							end
						end

						if bestMrk > 95 then
							CAdjustStat(bestMrkId, STATS.MARKSMANSHIP, 1)
							CResolveEvent(string.format("[成功: 神枪手][%s: +枪法] %s 瞄准目标开火。片刻之后，领头的血猫倒下了，子弹干净利落地击穿了它的头部。枪声提醒了平民并吓坏了血猫，恐慌的人群逃到了安全的地方。", bestMrkName, bestMrkName))
						else
							for name, id in pairs(mercs) do
								CAdjustMorale(id, -10)
							end
							CResolveEvent("[失败][-士气] 你们想尽办法警告平民———鸣枪、大喊、挥舞双手————但都已为时过晚，血猫对平民发动了突袭，叼着几个人跑了。")
						end
					else
						for name, id in pairs(mercs) do
							CAdjustMorale(id, -10)
						end
						CResolveEvent("[-士气] 你们眼睁睁的看着平民被血猫叼走，它们今天可以饱餐一顿了。")
					end
				end
			}
		end
	},
	[12] = -- rebels call, opportunity to learn DEPUTY or RADIO_OPERATOR
	{
		INIT = function(mercList)
			progress = CGetProgress()
			if progress < 20 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			count = 0
			potentials = {}
			for k, v in pairs(mercList) do
				skills = CGetSkills(v)

				valid = true
				for _, skillId in pairs(skills) do
					if skillId == SKILLS.RADIO_OPERATOR or skillId == SKILLS.DEPUTY then
						valid = false
						break
					end
				end

				if valid then
					potentials[k] = v
					count = count + 1
				end
			end

			if count < 1 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			rand = math.random(count)

			iter = 1
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(potentials) do
				if iter == rand then
					selectedName = name
					selectedProfileId = id
					break
				end

				iter = iter + 1
			end

			if selectedProfileId == -1 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			return {
				BodyText = string.format("反抗军发来信息，要求你们提供更多的帮助。他们特别点名需要 %s 出席一项军事演习。这需要占用好几天时间。", selectedName),
				TopButtonText = string.format("同意派遣 %s 。", selectedName),
				BottomButtonText = string.format("抱歉，%s 现在有其他重要工作。", selectedName),
				Resolution = function(topButton)
					if topButton then
						hours = math.random(168, 240)
						CSendMercOnMiniEvent(selectedProfileId, 65535)
						CResolveEvent(string.format("[ %s: 不可用] %s 简单收拾了一下就启程出发了。你不确定他什么时候才能回来，希望你不会后悔这个决定。", selectedName, selectedName), 1, hours)
						CScreenMsg(string.format(" %s 将暂时离队，大概需要一周时间才能返回。", selectedName))
					else
						CResolveEvent("[无事发生] 反抗军只能在没有你支持的情况下完成演习。也许下次你可以抽出时间。")
					end
				end
			}
		end
	},
	[13] = -- send a merc to San Mona in 24 hours
	{
		INIT = function(mercList)
			CScreenMsg("神秘人希望24小时后在 San Mona 会面!")
			return {
				BodyText = "一名平民故意撞了你一下，趁机将一个皱巴巴的纸条塞进你的手里，留下一个意味深长的眼神后就走了。你打开纸条，上面写着：\"我有东西给你。24小时后在 San Mona见面，不要找我，我会去找你。\"",
				TopButtonText = "好吧...",
				BottomButtonText = "唔...",
				Resolution = function(topButton)
					CResolveEvent("消息已送达，是否继续事件取决于你的决定。", 2, 24)
				end
			}
		end
	},
	[14] = -- bandits
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)
			count = 0
			for k, v in pairs(mercs) do
				count = count + 1
			end

			return {
				BodyText = string.format("[%s] 一位孤独的旅行者走向你们，当他经过你们身边时，他转过身说：\"你能给我一些钱吗？实际上，是很多钱。\" 在他说话的同时，大批匪徒从四周涌了出来。他举起枪：\"那么，慷慨一点？一个人头一千美元怎么样？\"情况对你们非常不利。", sectorIdString),
				TopButtonText = "付钱给强盗。",
				BottomButtonText = "奋力突围。",
				Resolution = function(topButton)
					if topButton then
						payment = count * 1000
						CAddMoneyToPlayerAccount(-payment, true)
						CScreenMsg(string.format("支付 $%d 给强盗。", payment))
						CResolveEvent(string.format("[-$%d] \"谢谢你，善良的陌生人！很高兴认识你，祝你们度过愉快的一天！\"这个人一边咧着嘴笑，一边看着你们慢慢离去。", payment))
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(25,40))
						end
						CResolveEvent("[全体: -生命] 双方迅速进入交火状态，你发现匪徒们没有经过良好的训练，武器也没有得到妥善的维护，所以他们并不像看上去那么有优势。你很庆幸还没有人付过钱来武装他们。")
					end
				end
			}
		end
	},
	[15] = -- POWs
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			count = 0
			for k, v in pairs(mercs) do
				count = count + 1
			end

			return {
				BodyText = string.format("[%s] 你们在一处荒郊野岭遇到了一群护送囚犯的敌兵。他们显然不认识你们。\"往后退!\"其中一个警卫警告道\"我们正在运送危险的罪犯。\"过了一会儿，两名囚犯趁警卫走神溜了出来，冲进了高高的草丛中。", sectorIdString),
				TopButtonText = "帮助释放其他囚犯。",
				BottomButtonText = "后退，让警卫去抓逃犯。",
				Resolution = function(topButton)
					if topButton then
						if count >= math.random(6) then
							for name, id in pairs(mercs) do
								CAdjustStat(id, STATS.AGILITY, 1)
							end
							CResolveEvent("[成功][全体: +敏捷] 趁着警卫被逃跑的囚犯分散注意力时，你们迅速的释放了其他的囚犯。囚犯向四面八方逃跑，场面一片混乱，你们和大多数的囚犯都安全撤离了。")
						else
							for name, id in pairs(mercs) do
								CApplyDamage(id, math.random(20,30))
							end
							CResolveEvent("[失败][全体: -生命] 运气不好加上人手不足使你没能够及时的释放其他囚犯，有几个佣兵在撤退时受了伤。")
						end
					else
						CResolveEvent("[无事发生]  你们不想掺和这件事，迅速离开了。")
					end
				end
			}
		end
	},
	[16] = -- road obstruction, fallen trees
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			valid = false
			mercInVehicleId = -1
			for name, id in pairs(mercs) do
				if CCheckForTravel(id) == true then
					assignmentResult, assignmentName, assignmentId = CCheckForAssignment(ASSIGNMENTS.VEHICLE, sector.X, sector.Y, sector.Z)
					if assignmentResult == true then
						valid = true
						mercInVehicleId = id
						break
					end
				end
			end

			if valid == false then
				return Events[math.random(#Events)].INIT(mercList)
			end

			return {
				BodyText = string.format("[%s] 你们沿着一条蜿蜒的小路行驶，突然发现前面的道路被一片倒下的树木拦住了，树木倒下的位置很奇怪，直觉告诉你这件事情有蹊跷。", sectorIdString),
				TopButtonText = "扫清路障。",
				BottomButtonText = "绕路行驶。",
				Resolution = function(topButton)
					if topButton then
						demo, demoName, demoId = CCheckForSkill(SKILLS.DEMOLITIONS, sector.X, sector.Y, sector.Z)
						bodybuilder, bbName, bbId = CCheckForSkill(SKILLS.BODYBUILDING, sector.X, sector.Y, sector.Z)

						if demo == true then
							CAdjustStat(demoId, STATS.EXPLOSIVE, 1)
							CResolveEvent(string.format("[成功: 爆破][%s: +爆破]  树木很沉重，无法按常规方法移除，但由于炸药布置的恰达好处，它们被轻易的清除了。", demoName))
						elseif bodybuilder == true then
							for name, id in pairs(mercs) do
								CAdjustStat(id, STATS.STRENGTH, 1)
								CAdjustBreathMax(id, -20)
							end
							CAdjustStat(bbId, STATS.LEADERSHIP, 1)
							CResolveEvent(string.format("[成功: 健身][全体: +力量, -精力][%s: +领导]  这些树不是伏击的一部分，但这并不能使它们更容易被清除。这是一项艰苦的工作，但 %s 强健的体格能够让每个人都轻松一点。", bbName, bbName))
						else
							for name, id in pairs(mercs) do
								CAdjustStat(id, STATS.STRENGTH, 1)
								CAdjustBreathMax(id, -50)
							end
							CResolveEvent("[全体: +力量, -精力] 这些树不是伏击的一部分，但这并不能使它们更容易被清除。这是一项艰苦卓绝的工作。")
						end
					else
						result, vehicleName = CAdjustVehicleFuel(mercInVehicleId, -15)
						CResolveEvent(string.format("[%s: -燃料] 你们找到了另一条能到达目的地的路，但这条路要绕很远。您可能需要比预期更早地加油。", vehicleName))
					end
				end
			}
		end
	},
	[17] = -- skirmish
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们顺着不远处的枪声，找到了一小股正在和女王卫队交火的反抗军，他们处境非常不利，很明显如果你不帮忙，他们就死定了。", sectorIdString),
				TopButtonText = "立刻加入战斗。",
				BottomButtonText = "等待进攻时机。",
				Resolution = function(topButton)
					if topButton then
						dep, depName, depId = CCheckForSkill(SKILLS.DEPUTY, sector.X, sector.Y, sector.Z)

						if dep == true then
							CCreateMilitia(math.random(3,5), math.random(0,3), 0, sector.X, sector.Y)
							CResolveEvent(string.format("[成功: 班副][+民兵] %s 接管指挥权后，战局立刻发生了变化，敌军很快被击退了，剩下的反抗军加入了你的民兵组织。", depName))
						else
							for name, id in pairs(mercs) do
								CApplyDamage(id, math.random(20,25))
							end
							CResolveEvent("[失败][全体: -生命] 你们冲进战场试图与反抗军汇合，但敌军果断分兵阻止了你们汇合的企图。你们最终击退了敌军士兵，但是反抗军全部战死了。")
						end
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(15))
						end
						CResolveEvent(string.format("[全体: -生命] 你们默默的等待时机，不幸的是，没有在反抗军活着的时候等到。反抗军被全部消灭，你在敌军搜刮战利品时开火，他们被打了个措手不及，没怎么反抗就被击毙了。"))
					end
				end
			}
		end
	},
	[18] = -- passing group
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们行至一个僻静的地方在这里稍作休整，这里开始很安静，直到远处传来说话声，说话声越来越大，你不确定他们是否发现了你们。", sectorIdString),
				TopButtonText = "潜行过去看看。",
				BottomButtonText = "静静的等待。",
				Resolution = function(topButton)
					if topButton then
						stealth, stealthName, stealthId = CCheckForSkill(SKILLS.STEALTHY, sector.X, sector.Y, sector.Z)
						if stealth == true then
							CResolveEvent(string.format("[成功: 潜行] %s 用事实证明，他的隐蔽技巧高超到让一大群匪徒从他身边经过也不能注意到他。", stealthName))
						else
							for name, id in pairs(mercs) do
								CApplyDamage(id, math.random(20,30))
							end
							CResolveEvent("[失败][全体: -生命] 你们试图伪装自己，这群匪徒在经过你们身边时，一根树枝折断的声音暴露了你们的位置，双方发生了短暂的交火。")
						end
					else
						covops, covopsName, covopsId = CCheckForSkill(SKILLS.COVERT_OPS, sector.X, sector.Y, sector.Z)
						if covops == true then
							CAdjustStat(covopsId, STATS.WISDOM, 1)
							CResolveEvent(string.format("[特工][%s: +智慧]一个敌军的侦查小队从灌木丛中走来，%s 凭借出色的敌后作战经验让敌人相信你们是一群受雇于当地百姓的猎人，双方聊了一会儿，敌人就离开了。", covopsName, covopsName))
						else
							for name, id in pairs(mercs) do
								CAdjustBreathMax(id, -20)
							end
							CResolveEvent("[全体: -精力] 一个敌军的侦查小队从灌木丛中走出来，你们对他们发动了伏击。侦查小队被处理后，你们迅速收拾好行装，在增援到来前撤出了这里。")
						end
					end
				end
			}
		end
	},
	[19] = -- diseased farmers
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们被一群穿着厚重斗篷的农民包围了。其中一人走上前来，露出一张病容满面的脸。他有气无力的对你们说：\"你好，正如你们看到的，我们现在受到了一些困扰，必须向你们乞讨一些钱财以便在Cambria接受治疗。\"你顺着他手指的方向，看到他斗篷下藏着一只手抢，\"请不要拒绝我们。\"", sectorIdString),
				TopButtonText = "给他们一些钱。",
				BottomButtonText = "拒绝他们。",
				Resolution = function(topButton)
					if topButton then
						CAddMoneyToPlayerAccount(-10000, true)
						CScreenMsg("支付 $10000 给患病的农民。")
						CResolveEvent("[-$10000] 起初你只准备给他们一点钱，但这些农民想要更多。佣兵们希望你赶紧付钱了结此事，以便尽快离开患病的农民们。毕竟出钱的是你而不是他们。")
					else
						doc, docName, docId = CCheckForSkill(SKILLS.PARAMEDIC, sector.X, sector.Y, sector.Z)
						if doc == true then
							CAdjustStat(docId, STATS.MEDICAL, 2)
							CResolveEvent(string.format("[救护兵][%s: +医疗] 你们拒绝给钱，在局面即将失控时，%s 提议帮病人们做一些简单的诊断和护理，最终和平的化解了这次危机。", docName, docName))
						else
							for name, id in pairs(mercs) do
								CAdjustBreathMax(id, -20)
								CAdjustStat(id, STATS.STRENGTH, -3)
								CAdjustStat(id, STATS.DEXTERITY, -3)
								CAdjustStat(id, STATS.AGILITY, -3)
							end
							CResolveEvent("[全体: -精力, -力量, -灵巧, -敏捷] 农民们无法接受否定的答案，他们冲向你们，霎时间拳头满天飞。最终这群人被赶走了，但你们因为近距离和病患接触而感到浑身难受。")
						end
					end
				end
			}
		end
	},
	[20] = -- white birds
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 一群大白鸟从你们身边飞过，它们的距离和高度非常适合练习射击。你们准备瞄准它们。", sectorIdString),
				TopButtonText = "向鸟儿射击。",
				BottomButtonText = "让鸟儿安然无恙的飞过。",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.MARKSMANSHIP, 1)
						end
						CResolveEvent("[全体: +枪法] 你们把这当成了一次即兴射击比赛，枪响鸟落。非常开心。")
					else
						CResolveEvent("瞄准很难，命中更难，你们重新把视线投向大地，继续平凡的一天。")
					end
				end
			}
		end
	},
	[21] = -- black birds
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 一群大黑鸟从佣兵身边飞过，它们的距离和高度非常适合练习射击。你们准备瞄准它们。", sectorIdString),
				TopButtonText = "向鸟儿射击。",
				BottomButtonText = "让鸟儿安然无恙的飞过。",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, -7)
							CAdjustStat(id, STATS.DEXTERITY, -7)
							CAdjustStat(id, STATS.AGILITY, -7)
						end
						CResolveEvent("[全体: -力量, -灵巧, -敏捷] 当第一枪响起时，你们感到有些不对头，它们比预计的离你更远，也更大！枪声引起了它们的注意。腐肉鸟群用可致病的喙和爪扑向你们。你们在森林中四处寻找掩护，像一群待宰的羔羊。")
					else
						CResolveEvent("瞄准很难，命中更难，你们重新把视线投向大地，继续平凡的一天。")
					end
				end
			}
		end
	},
	[22] = -- charging bloodcat
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们正在穿过一片小树林，突然看到一只大血猫朝你飞扑过来，是离它们的巢穴太近了？还是附近刚刚经历过新鲜杀戮？显然它被激怒了，几乎没有给你留反应时间。", sectorIdString),
				TopButtonText = "试着让血猫冷静下来。",
				BottomButtonText = "向血猫射击。",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, -5)
							CAdjustStat(id, STATS.DEXTERITY, -5)
							CAdjustStat(id, STATS.AGILITY, -5)
						end
						CResolveEvent("[全体: -力量, -灵巧, -敏捷] 一秒钟后你才意识到这是一个糟糕透顶的想法。血猫很凶猛，在你举起武器之前它就对你们造成了严重的伤害。")
					else
						CResolveEvent("[无事发生] 当血猫快要飞扑到你身边时，你迅速举起武器快速击落了它。它毫无生气的倒在你们的脚下。")
					end
				end
			}
		end
	},
	[23] = -- weak bandits
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 一群匪徒叫嚣着从山坡上冲下来，吓了你一跳。匪徒们冲到能看清你们的距离后放慢脚步，发现你们个个是硬茬儿，面面相觑，转身开始往山上撤退。", sectorIdString),
				TopButtonText = "追杀他们。",
				BottomButtonText = "跟踪他们。",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.AGILITY, 1)
						end
						CResolveEvent("[全体: +敏捷] 你们立刻朝山上开火，这些年轻的匪徒不是对手，很快就全被击毙了。")
					else
						scout, scoutName, scoutId = CCheckForSkill(SKILLS.SCOUTING, sector.X, sector.Y, sector.Z)
						if scout == true then
							CAddMoneyToPlayerAccount(5000)
							CScreenMsg("从匪徒战利品中获得 $5000。")
							CResolveEvent(string.format("[成功: 侦查][+$5000] 山的另一边没有太多掩体，%s 很容易就找到了匪徒的窝点。你们迂回包抄了他们的营地，他们大多数人只有十几岁，战斗没多久就丢下武器四散溃逃了。你们因此缴获了一大批战利品。", scoutName))
						else
							CResolveEvent("[失败][无事发生] 你们跟着匪徒们爬到山顶，匪徒们发现自己被跟踪了，四散溃逃的无影无踪。虽然你们没能剿灭他们，但至少他们也不会打扰其他人了。")
						end
					end
				end
			}
		end
	},
	[24] = -- forked path
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们在一条小路上遇到了一个岔口。一边看起来平坦易行，另一边则荆棘丛生。两条小道都能到达目的地。这时你突然感到一丝莫名的不安，似乎有什么东西在盯着你们。现在你必须做出决定。", sectorIdString),
				TopButtonText = "走平坦易行的路。",
				BottomButtonText = "走荆棘丛生的路。",
				Resolution = function(topButton)
					if topButton then
						CResolveEvent("[无事发生] 你们在平坦的道路上时刻保持警惕，最后发现只是虚惊一场。")
					else
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, -5)
							CAdjustStat(id, STATS.AGILITY, -5)
						end
						CResolveEvent("[全体: -力量, -敏捷] 这条小路和想的一样难走，你们经常被流着奇怪汁液的锋利荆棘刮伤。走出这条路不久后，你们感觉好像生病了。")
					end
				end
			}
		end
	},
	[25] = -- soldier attack
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 远处荡起一阵烟尘，你看到一群敌兵咆哮着向你这边冲来，你们进入战斗状态准备迎敌。这时你看到旁边的山顶上有些模糊的人影，他们向你们发出了\“趴下隐蔽\”的信号。", sectorIdString),
				TopButtonText = "趴下隐蔽。",
				BottomButtonText = "发起攻击。",
				Resolution = function(topButton)
					if topButton then
						CCreateMilitia(math.random(5,10), math.random(3,5), math.random(0,3), sector.X, sector.Y)
						CResolveEvent("[+民兵] 你们在附近的掩体中藏好，敌兵接近后，山顶上模糊的人影们对他们发起了侧袭。敌兵大吃一惊，很快就四散奔逃。尔后，一支反抗军小队从山上下来，加入了你的民兵队伍。")
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(10,20))
						end
						CResolveEvent("[全体: -生命] 在山顶上的反抗军准备发起侧袭时，你们正在和敌军混战，战场上的烟雾和灰尘让反抗军无法辨别敌友。冲突结束后，反抗军对你们说\"对不起，我们没帮上忙\"。 ")
					end
				end
			}
		end
	},
	[26] = -- go to a pub
	{
		INIT = function(mercList)
			progress = CGetProgress()
			if progress < 25 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			return {
				BodyText = "也许应该让佣兵们放松一下，喝几杯酒应该能让每个人都振奋起来————在酒吧寻找真正的自我。你让他们玩多久？",
				TopButtonText = "... 12 小时。",
				BottomButtonText = "... 24 小时。",
				Resolution = function(topButton)
					if topButton then
						CResolveEvent("你告诉所有人: 你们有12小时可以尽情狂欢，酒钱你出。", 3, 12)
					else
						CResolveEvent("你告诉所有人: 你们有24小时可以尽情狂欢，酒钱你出。", 3, 24)
					end
				end
			}
		end
	},
	[27] = -- vase
	{
		INIT = function(mercList)
			mercs = GetMercsInTown(mercList)

			count = 0
			for k,v in pairs(mercs) do
				count = count + 1
			end

			if count == 0 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			mercs, sector = GetAllMercsInRandomSector(mercs)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 你们在镇子上闲逛，看见一个老先生搬着一个沉重的大花瓶从门里走出来。他满头大汗，看起来很吃力。他从你们身边经过失去了平衡，花瓶从手中滑落。", sectorIdString),
				TopButtonText = "试着抓住花瓶。",
				BottomButtonText = "看着花瓶掉在地上。",
				Resolution = function(topButton)
					if topButton then
						ath, athName, athId = CCheckForSkill(SKILLS.ATHLETICS, sector.X, sector.Y, sector.Z)
						if ath == true then
							CAddMoneyToPlayerAccount(1000)
							CScreenMsg("获得花瓶运费 $1000。")
							CResolveEvent(string.format("[成功: 运动员][+$1000] %s 眼疾手快，飞奔上去接住了花瓶。老人松了一口气，表示愿意出一笔不菲的费用雇佣你来搬运花瓶。", athName))
						else
							CAddMoneyToPlayerAccount(-5000, true)
							CScreenMsg("打碎花瓶支付赔偿金 $5000。")
							CResolveEvent("[失败][-$5000] 你们冲过去想接住花瓶，但是跑到老人身边时绊了一跤，花瓶掉在地上摔得粉碎。老人脸色铁青，要求你们赔偿他的损失，你们被狠狠地宰了一笔。")
						end
					else
						CResolveEvent("[无事发生] 你们眼睁睁的看着花瓶从老人手中跌落，摔的粉碎。老人捶胸顿足，你们同情的看了他一眼，帮他打扫碎片。")
					end
				end
			}
		end
	},
	[28] = -- guard duty
	{
		INIT = function(mercList)
			mercs = GetMercsInTown(mercList)

			count = 0
			for k,v in pairs(mercs) do
				count = count + 1
			end

			if count == 0 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			mercs, sector = GetAllMercsInRandomSector(mercs)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 在你们休息时，一位面带假笑的商人走过来\"你们一定是非常优秀的佣兵，也许我们可以做一笔交易。\"我有一批货需要你们看守几个小时。\"可以吗？\"", sectorIdString),
				TopButtonText = "同意看守货物。",
				BottomButtonText = "婉言拒绝。",
				Resolution = function(topButton)
					if topButton then
						hours = math.random(8,12)
						for name, id in pairs(mercs) do
							CSendMercOnMiniEvent(id, hours)
						end
						CAddMoneyToPlayerAccount(7500)
						CScreenMsg("获得守卫工资$7500。")
						CResolveEvent("[团队不可用][+$7500] \"太好了！交易完我就回来。应该用不了几个小时。\"你们在一座不起眼的大楼里看管一大堆沉重的板条箱。期间除了赶走一些好奇的孩子，什么也没发生。")
					else
						CResolveEvent("[无事发生] 你们有很重要的任务要进行，商人看上去很苦恼，但还是离开了。" )
					end
				end
			}
		end
	},
	[29] = -- kids
	{
		INIT = function(mercList)
			mercs = GetMercsInTown(mercList)

			count = 0
			for k,v in pairs(mercs) do
				count = count + 1
			end

			if count == 0 then
				return Events[math.random(#Events)].INIT(mercList)
			end

			mercs, sector = GetAllMercsInRandomSector(mercs)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)
			townId, townName = CGetTownId(sector.X, sector.Y)

			return {
				BodyText = string.format("[%s]一个小年轻兴奋的走近你们\"嘿，我认识你们。\"他说到\"我朋友说你们超棒！ 你们能跟我去我朋友那讲讲你们的故事吗，拜托！\"", sectorIdString),
				TopButtonText = "同意跟着小年轻。",
				BottomButtonText = "你认错人了。",
				Resolution = function(topButton)
					if topButton then
						CAddTownLoyalty(townId, 2500)
						CResolveEvent(string.format("[%s: +忠诚度] 这个小伙子把你们带到他的朋友那里，他们兴奋极了！不断央求你们讲更多的冒险经历。最终他们听够了故事，心满意足的跑了。", townName))
					else
						CResolveEvent("[无事发生] 你拒绝了他的请求，告诉他他把你们和另一群人搞混了。他看着你们离开，无比失落。")
					end
				end
			}
		end
	},
	[30] = -- broken down car
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] 真是稀奇！你们竟然看到了一位 Arulco 本地司机。他正一边检查引擎，一边挠头。看起来他可能需要一些帮助。", sectorIdString),
				TopButtonText = "停下来帮他。",
				BottomButtonText = "继续前进。",
				Resolution = function(topButton)
					if topButton then
						tech, techName, techId = CCheckForSkill(SKILLS.TECHNICIAN, sector.X, sector.Y, sector.Z)
						if tech == true then
							CAdjustStat(techId, STATS.MECHANICAL, 1)
							CAddIntel(20)
							CResolveEvent(string.format("[成功: 技工][%s: +机械][+情报] 经过仔细检查，%s 发现了故障原因，很快就把车修好了。司机非常感激，他经常在这附近开车，和你分享了一些有趣的情报。", techName, techName))
						else
							CResolveEvent("[失败][无事发生] 你们尽力了！最终也没能找到问题在哪。不管怎样，他还是很感谢你们。")
						end
					else
						CResolveEvent("[无事发生] 你没有时间也没有能力帮助他，希望他能自己搞定。")
					end
				end
			}
		end
	},
	[31] = -- covert ops
	{
		INIT = function(mercList)
			covops, covopsName, covopsId = CCheckForSkill(SKILLS.COVERT_OPS)

			if covops == false then
				return Events[math.random(#Events)].INIT(mercList)
			end

			return {
				BodyText = string.format("有一个秘密潜入 Meduna 的机会，作为一位秘密行动专家 %s 愿意试一试，听起来目前潜入的方法还挺多，但随着时间推移，方法会越来越少。", covopsName),
				TopButtonText = string.format("派 %s 去。", covopsName),
				BottomButtonText = "婉言拒绝。",
				Resolution = function(topButton)
					if topButton then
						CSendMercOnMiniEvent(covopsId, 65535)
						CScreenMsg(string.format("%s 开始向 Meduna 移动。", covopsName))
						CResolveEvent(string.format("[%s: 不可用] %s开始进行潜入准备工作，几天后就可以出发了。", covopsName, covopsName), 4, 48)
					else
						CResolveEvent("[无事发生] 你的队员有更重要的工作，他错过了这次机会。")
					end
				end
			}
		end
	},

	[999] =
	{
		INIT = function(mercList)
			mercs, sector = GetAllMercsInRandomSector(mercList)
			sectorIdString = CGetSectorIDString(sector.X, sector.Y, sector.Z)

			return {
				BodyText = string.format("[%s] ", sectorIdString),
				TopButtonText = "",
				BottomButtonText = "",
				Resolution = function(topButton)
					if topButton then
						CResolveEvent("")
					else
						CResolveEvent("")
					end
				end
			}
		end
	}
}

HiddenEvents =
{
	-- note that 0 is not a valid index!
	[1] = -- chain from Event 12
	{
		INIT = function(mercList)
			-- the occupied merc is the one with a super high mini event timer
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "反抗军在没有你支持的情况下完成了演习，随后就杳无音信了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如约出现在演习中，这让反抗军遭受了意料之外的损失，但他们最终还是达成了任务目标。")
					end
				}
			end
			
			return {
				BodyText = string.format("反抗军精心策划了一场突袭演习，这几天 %s 在接受特训，特训内容是。。 ", selectedName),
				TopButtonText = "... 学习用无线电调度团队。",
				BottomButtonText = "... 学习怎样指挥一只突击小队。",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 10, 1, 0)
					CSendMercOnMiniEvent(selectedProfileId, 1)
					CAddIntel(100)

					if topButton then
						CAddSkill(selectedProfileId, SKILLS.RADIO_OPERATOR)
						CAdjustStat(selectedProfileId, STATS.WISDOM, 2)
						CScreenMsg(string.format("%s 学会了一项新技能：无线电操作员", selectedName))
						CResolveEvent(string.format("[%s: 获得 无线电操作员 技能, +智力][+情报] %s 从反抗军基地发回信息，大约一小时后可以重新归队。", selectedName, selectedName))
					else
						CAddSkill(selectedProfileId, SKILLS.DEPUTY)
						CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 2)
						CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 5)
						CScreenMsg(string.format("%s 学会了一项新技能：班副", selectedName))
						CResolveEvent(string.format("[%s: 获得 班副 技能, +枪法, +领导][+情报] %s 从反抗军基地发回信息，大约一小时后可以重新归队。", selectedName, selectedName))
					end
				end
			}
		end
	},
	[2] = -- chain from Event 13
	{
		INIT = function(mercList)
			count = 0
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				x, y, z = CGetCoordinates(id)
				if CGetTownId(x,y) == TOWNS.SANMONA then
					count = count + 1
					selectedName = name
					selectedProfileId = id
				end
			end

			if count == 0 then
				return {
					BodyText = "你决定不去找神秘人",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 谁也不知道这决定是否正确，但是小心驶得万年船。")
					end
				}
			end

			return {
				BodyText = string.format("在San Mona众多酒吧之间，一位平民不断向城郊方向挥手，这引起了 %s 的注意。远远的地跟在后面。片刻之后 %s 意识到他们正在往城外走。心中隐约有些不安。", selectedName, selectedName),
				TopButtonText = "继续跟随平民。",
				BottomButtonText = "停止跟踪。",
				Resolution = function(topButton)
					if topButton then
						if count == 1 then
							-- chance to be ambushed if solo
							if math.random(100) < 50 then
								CApplyDamage(selectedProfileId, math.random(15,30))
								CResolveEvent(string.format("一场伏击！对 %s 来说幸运的是一名匪徒太过激动，提前扣发扳机暴露了自己。经过一番血战，%s 侥幸活着逃脱了。", selectedName, selectedName))
							else
								CAddMoneyToPlayerAccount(10000)
								CScreenMsg("从 Kingpin 的金库获得 $10000。")
								CResolveEvent(string.format("[+$10000]  \"就你一个人？胆子不小啊！也许我们可以从 Kingpin 的金库里搞点钱，你应该能干的比我们更好，祝你好运。\" 完事之后 %s 带着钱返回 San Mona。", selectedName))
							end
						else
							CAddMoneyToPlayerAccount(10000)
							CScreenMsg("从 Kingpin 的金库获得 $10000。")
							CResolveEvent("[+$10000] \"我们可以从 Kingpin 的金库里搞点钱，你们应该能干的比我们更好，祝你们好运。\" 完事之后你们带着钱返回 San Mona。")
						end
					else
						CResolveEvent("这太冒险了，你们返回了San Mona，毕竟小心驶得万年船。")
					end
				end
			}
		end
	},
	[3] = -- chain from Event 26
	{
		INIT = function(mercList)
			mercsInTown = GetMercsInTown(mercList)
			count = 0

			for k,v in pairs(mercList) do
				count = count + 1
			end

			if count == 0 then
				return {
					BodyText = "看来你们现在不能休息了，期待下次吧。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你们感到很失望，尽管如此该打的仗还是要打。")
					end
				}
			end

			mercs = GetLargestMercGroup(mercsInTown)
			randomName, randomId = GetRandomMerc(mercs)
			x, y, z = CGetCoordinates(randomId)

			h2h, h2hName, h2hId = CCheckForSkill(SKILLS.HAND_TO_HAND, x, y, z)

			return {
				BodyText = string.format(" %s 的小队正在一家酒吧放松，这时一个大汉撞到了他们的桌子上，酒水饮料洒的满地都是。在他边上是一个更高的大汉，他俩互相谩骂，前一个大汉站起来又冲向他的对手，两个人向保龄球一样，把酒吧撞的七零八落。酒吧里爆发了一场混战，毕竟谁也不希望自己的酒洒出来。", randomName),
				TopButtonText = "加入斗殴！",
				BottomButtonText = "试着让大家冷静下来，阻止斗殴。",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercsInTown) do
							CAdjustMorale(id, 50)
						end
						
						if h2h == true then
							for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, 1)
							end
							CResolveEvent(string.format("[全体: +士气, +力量] 这是一场可以记入史册的\"史诗级\"酒吧斗殴。%s 是全场的MVP！每个佣兵都追随着他的脚步，桌子被打碎，椅子被扔掉，没有人能阻挡，所有人都被打趴下了。", h2hName))
						else
							CAddMoneyToPlayerAccount(-15000, true)
							CScreenMsg("支付 $15000 赔偿酒吧损失。")
							CResolveEvent("[-$15000][全体: +士气] 没有什么能比一场酒吧斗殴更能让佣兵们放松了。不幸的是，酒吧老板不这么认为。他要求你们赔偿损失。")
						end
					else
						townId, townName = CGetTownId(x, y)

						if h2h == true then
							CAddMoneyToPlayerAccount(-15000, true)
							CScreenMsg("支付 $15000 赔偿酒吧损失。")
							CAddTownLoyalty(townId, -4000)
							CResolveEvent(string.format("[肉搏][-$15000][%s: -忠诚度] 你们努力保持冷静克制，可是 %s 最终没忍住。%s 狂怒着冲上去把这两个大汉放倒。不过这也让酒吧遭受了不菲的损失，除了赔偿别无它法。", townName, h2hName, h2hName))
						else
							for name, id in pairs(mercsInTown) do
								CAdjustMorale(id, 15)
							end

							CAddTownLoyalty(townId, 4000)
							CResolveEvent(string.format("[%s: +忠诚度][全体: +士气] 在努力把两人分开，并没收了他们的酒后，酒吧恢复了平静。老板非常感谢你们的付出，免了你们的单。", townName))
						end

					end
				end
			}
		end
	},
	[4] = -- chain from Event 31
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			--[[
			START	> pose as recruit	> weapons training		> stat boost OR learn random skill (marksman, auto weapons, heavy weapons, gunslinger)	> END
										> specialist training	> pick one of two random skills (paramedic, technician, demolitions, radio op, scouting, deputy, night ops)	> END
										> basic training		> stat boost OR learn random skill (athletics, stealthy, bodybuilding)	> END
					> target military facilities	> steal documents (+intel)	> END
													> poison garrison (-soldiers)	> END
													> sabotage tanks (-tanks)	> END
													> view enemy positions (global vis), repeatable/automatic?
					> target government facilities	> siphon funds (+$$$)	> END
													> steal documents (+intel)	> END
													> incite uprising (+militia)	> END
													> steal cultural artifacts (+loyalty)	> END
			]]

			poseAsRecruit = {
				Text = "冒充新兵。",
				ResolveText = string.format(" %s 将接受敌军的新兵训练，看看敌人怎么训练新兵，没准能学到一些有用的东西。", selectedName),
				EventId = 5
				}
			targetArmy = {
				Text = "渗透军事设施。",
				ResolveText = string.format("军事设施中有大量有价值的目标， %s 选择渗透这里。", selectedName),
				EventId = 6
			}
			targetGovernment = {
				Text = "渗透政府大楼。",
				ResolveText = string.format("政府大楼看起来很有价值， %s 将尝试从这里渗透。", selectedName),
				EventId = 7
			}

			firstOption = {}
			secondOption = {}

			roll = math.random(3)
			if roll == 1 then
				firstOption = poseAsRecruit
				secondOption = targetArmy
			elseif roll == 2 then
				firstOption = poseAsRecruit
				secondOption = targetGovernment
			else
				firstOption = targetArmy
				secondOption = targetGovernment
			end

			return {
				BodyText = string.format("有几个靠谱的线索可以跟进，但 %s 只能选一个，应该选？", selectedName),
				TopButtonText = firstOption.Text,
				BottomButtonText = secondOption.Text,
				Resolution = function(topButton)
					if topButton then
						CResolveEvent(firstOption.ResolveText, firstOption.EventId, 72)
					else
						CResolveEvent(secondOption.ResolveText, secondOption.EventId, 72)
					end
				end
			}
		end
	},
	[5] = -- chain from Hidden Event 4
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			basicTraining = {
				Text = "接受基础体能训练。",
				ResolveText = string.format("%s 接受新兵的基础体能训练，这是一个很好的了解敌人如何训练士兵的机会， %s 可能因此获益匪浅。", selectedName, selectedName),
				EventId = 8
			}
			specialistTraining = {
				Text = "接受专科技术训练。",
				ResolveText = string.format("%s 了解敌方军队如何培训各方面的专家，这包括军医、无线电操作人员和军官，如果运气好，%s 也能为自己学些东西。", selectedName, selectedName),
				EventId = 9
			}
			weaponsTraining = {
				Text = "接受武器射击训练。",
				ResolveText = string.format("%s 射击的经验无法从书本中获得，无论如何，他将深入敌军中了解他们如何对士兵进行武器训练。", selectedName),
				EventId = 10
			}

			firstOption = {}
			secondOption = {}

			roll = math.random(3)
			if roll == 1 then
				firstOption = basicTraining
				secondOption = specialistTraining
			elseif roll == 2 then
				firstOption = basicTraining
				secondOption = weaponsTraining
			else
				firstOption = specialistTraining
				secondOption = weaponsTraining
			end


			return {
				BodyText = string.format("新兵的招募很宽松，%s 的报名过程非常顺利，没有引起任何人怀疑。这里的新兵大多是女王的追随者，一些国际佣兵组织负责对他们进行训练和管理，在这里你有几种不同的途径获得你想要的信息。", selectedName),
				TopButtonText = firstOption.Text,
				BottomButtonText = secondOption.Text,
				Resolution = function(topButton)
					if topButton then
						CResolveEvent(firstOption.ResolveText, firstOption.EventId, 168)
					else
						CResolveEvent(secondOption.ResolveText, secondOption.EventId, 168)
					end
				end
			}
		end
	},
	[6] = -- chain from Hidden Event 4
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			stealDocuments = {
				Text = "收集军情。",
				ResolveText = string.format("%s 将尽可能多的收集各种机密文件，任何信息都是有用的！", selectedName),
				EventId = 11
			}
			poisonGarrison = {
				Text = "给驻军下毒。",
				ResolveText = string.format("虽然 Meduna 的驻军非常强大，但是每个士兵都要喝水吃饭， %s 将从此着手，尝试削弱城市的防御。", selectedName),
				EventId = 12
			}
			sabotageVehicles = {
				Text = "破坏军用车辆。",
				ResolveText = string.format("你很庆幸自己没有直面女王的装甲部队，但你认识到最终的决战还是绕不过这些铁壳子，你指示 %s 在总攻发起之前尝试破坏这些装甲们。", selectedName),
				EventId = 13
			}

			firstOption = {}
			secondOption = {}

			roll = math.random(3)
			if roll == 1 then
				firstOption = stealDocuments
				secondOption = poisonGarrison
			elseif roll == 2 then
				firstOption = stealDocuments
				secondOption = sabotageVehicles
			else
				firstOption = poisonGarrison
				secondOption = sabotageVehicles
			end

			return {
				BodyText = string.format("分析之后， %s 发现了两个比较明显的漏洞，或者也可以选择收集一些敌军的内幕情报。", selectedName),
				TopButtonText = firstOption.Text,
				BottomButtonText = secondOption.Text,
				Resolution = function(topButton)
					CSetEnemyGroupVisibility(true)
					if topButton then
						CResolveEvent(firstOption.ResolveText, firstOption.EventId, 168)
					else
						CResolveEvent(secondOption.ResolveText, secondOption.EventId, 168)
					end
				end
			}
		end
	},
	[7] = -- chain from Hidden Event 4
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			siphonFunds = {
				Text = "挪用资金。",
				ResolveText = string.format(" %s 设法将这个国家的收入转到你的账户，而不是女王的账户。", selectedName),
				EventId = 14
			}
			stealDocuments = {
				Text = "收集信息。",
				ResolveText = string.format("在女王的统治下，Arulco 政府腐败的令人难以置信。 %s 将收集这些能影响国际舆论的重要文件。", selectedName),
				EventId = 15
			}
			stealArtifacts = {
				Text = "盗取文物。",
				ResolveText = string.format("女王从全国各地搜刮了几件具有重要文化意义的文物。把这些文物还给人民，能让群众对你们刮目相看。", selectedName),
				EventId = 16
			}

			firstOption = {}
			secondOption = {}

			roll = math.random(3)
			if roll == 1 then
				firstOption = siphonFunds
				secondOption = stealDocuments
			elseif roll == 2 then
				firstOption = siphonFunds
				secondOption = stealArtifacts
			else
				firstOption = stealDocuments
				secondOption = stealArtifacts
			end

			return {
				BodyText = string.format("除了影响统治权利的事，女王不会过多关注 Arulco 的政府运作，因此 %s 更容易在非军事建筑里潜伏。这里有几个不错的行动机会，应该选择哪个？", selectedName),
				TopButtonText = firstOption.Text,
				BottomButtonText = secondOption.Text,
				Resolution = function(topButton)
					if topButton then
						CResolveEvent(firstOption.ResolveText, firstOption.EventId, 168)
					else
						CResolveEvent(secondOption.ResolveText, secondOption.EventId, 168)
					end
				end
			}
		end
	},
	[8] = -- chain from Hidden Event 5
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format("虽然新兵体能训练不是很难，但是新兵都是菜鸟，%s 也必须装成菜鸟的样子。在接下来一周的艰苦训练中，%s把精力集中在....", selectedName, selectedName),
				TopButtonText = "体能素质训练。",
				BottomButtonText = "技术动作训练。",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
					CAddIntel(50)
					if topButton then
						CAdjustStat(selectedProfileId, STATS.LIFE, 5)
						CAdjustStat(selectedProfileId, STATS.STRENGTH, 5)
						CAdjustStat(selectedProfileId, STATS.AGILITY, 5)
						CAdjustStat(selectedProfileId, STATS.DEXTERITY, 5)
						CResolveEvent(string.format("[%s: +生命, +力量, +敏捷, +灵巧][+情报] 整整一周 %s 废寝忘食全身心的投入训练，他以极佳的成绩从训练中脱颖而出，现在 %s 将伺机寻找逃跑的机会，并把所见所闻做成报告一并带回。", selectedName, selectedName, selectedName))
					else
						pool = {
							{
								SkillId = SKILLS.ATHLETICS, 
								SkillName = "运动员"
							},
							{
								SkillId = SKILLS.BODYBUILDING,
								SkillName = "健身"
							},
							{
								SkillId = SKILLS.STEALTHY,
								SkillName = "潜行"
							}
						}

						skills = CGetSkills(selectedProfileId)
						finalPool = {}

						for k,v in ipairs(pool) do
							found = false
							for _,id in ipairs(skills) do
								if id == v.SkillId then
									found = true
								end
							end

							if found == false then
								finalPool[#finalPool+1] = v
							end
						end

						-- learn a new skill. or if they already have athletics+bodybuilding+stealthy (!) then just do the same as the first option
						if #finalPool >= 1 then
							index = math.random(#finalPool)
							learned = finalPool[index].SkillName
							CAddSkill(selectedProfileId, finalPool[index].SkillId)
							CResolveEvent(string.format("[%s: 获得 %s 技能][+情报] 一周的技术专项训练足以让自己变强，%s 也不例外，一些新的技巧应该能让他更好的发挥自己的作用，剩下的就是要找到伺机跑路的机会，归队报道！", selectedName, learned, selectedName))
						else
							CAdjustStat(selectedProfileId, STATS.LIFE, 5)
							CAdjustStat(selectedProfileId, STATS.STRENGTH, 5)
							CAdjustStat(selectedProfileId, STATS.AGILITY, 5)
							CAdjustStat(selectedProfileId, STATS.DEXTERITY, 5)
							CResolveEvent(string.format("[%s: +生命, +力量, +敏捷, +灵巧][+情报] 整整一周 %s 废寝忘食全身心的投入训练，他以极佳的成绩从训练中脱颖而出，现在 %s 将伺机寻找逃跑的机会，并把所见所闻做成报告一并带回。", selectedName, selectedName, selectedName))
						end
					end
				end
			}
		end
	},
	[9] = -- chain from Hidden Event 5
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			pool = {
				{
					SkillId = SKILLS.PARAMEDIC, 
					SkillName = "救护兵",
					SkillText = "战场急救训练。"
				},
				{
					SkillId = SKILLS.TECHNICIAN,
					SkillName = "技工",
					SkillText = "战斗工兵训练。"
				},
				{
					SkillId = SKILLS.DEMOLITIONS,
					SkillName = "爆破",
					SkillText = "爆炸物训练。"
				},
				{
					SkillId = SKILLS.RADIO_OPERATOR,
					SkillName = "无线电操作员",
					SkillText = "无线电专项训练。"
				},
				{
					SkillId = SKILLS.SCOUTING,
					SkillName = "侦查",
					SkillText = "侦查训练。"
				},
				{
					SkillId = SKILLS.DEPUTY,
					SkillName = "班副",
					SkillText = "指挥训练。"
				},
				{
					SkillId = SKILLS.NIGHT_OPS,
					SkillName = "夜战",
					SkillText = "夜间作战训练。"
				}
			}

			skills = CGetSkills(selectedProfileId)
			finalPool = {}

			for k,v in ipairs(pool) do
				found = false
				for _,id in ipairs(skills) do
					if id == v.SkillId then
						found = true
					end
				end

				if found == false then
					finalPool[#finalPool+1] = v
				end
			end

			firstOption = {}
			secondOption = {}

			if #finalPool >= 2 then
				r1 = math.random(1, #finalPool)
				r2 = math.random(1, #finalPool-1)
				if r1 == r2 then
					r2 = r2 + 1
				end

				firstOption = finalPool[r1]
				secondOption = finalPool[r2]
			elseif #finalPool == 1 then
				firstOption = finalPool[1]
				secondOption = { SkillId = -1, SkillText = "常规非战斗技能训练。"}
			else
				firstOption = { SkillId = -1, SkillText = "常规非战斗技能训练。"}
				secondOption = { SkillId = -2, SkillText = "常规武器训练。"}
			end

			return {
				BodyText = string.format("这简直太震撼了！这里有各种各样的专业课，还配套了大量的先进培训设备，你突然明白了这个国家的钱都去了哪里。几门专业课激起了 %s 的兴趣。", selectedName),
				TopButtonText = firstOption.SkillText, 
				BottomButtonText = secondOption.SkillText, 
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
					CAddIntel(50)

					choice = {}
					if topButton then
						choice = firstOption
					else
						choice = secondOption
					end

					if choice.SkillId > 0 then
						CAddSkill(selectedProfileId, choice.SkillId)
						CResolveEvent(string.format("[%s: 获得 %s 技能][+情报]  %s 本身天资过人，这里的教官更是博学多才，培训的进展非常神速。%s 很快掌握了许多关于军队专业训练的方法和知识，这让他自己也受益匪浅。很快就可以准备回到San Mona报道了。", selectedName, choice.SkillName, selectedName, selectedName))
					elseif choice.SkillId == -1 then
						CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 20)
						CAdjustStat(selectedProfileId, STATS.MECHANICAL, 10)
						CAdjustStat(selectedProfileId, STATS.MEDICAL, 10)
						CAdjustStat(selectedProfileId, STATS.EXPLOSIVE, 10)
						CResolveEvent(string.format("[%s: +领导,+机械,+医疗,+爆破][+情报] %s 本身天资过人，这里的教官更是博学多才，培训的进展非常神速。%s 很快掌握了许多关于军队专业训练的方法和知识，这让他自己也受益匪浅。很快就可以准备回到San Mona报道了。", selectedName, selectedName, selectedName))
					else -- SkillId == -2
						CAdjustStat(selectedProfileId, STATS.AGILITY, 2)
						CAdjustStat(selectedProfileId, STATS.DEXTERITY, 6)
						CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 10)
						CResolveEvent(string.format("[%s: +敏捷,+灵巧,+枪法][+情报] %s 本身天资过人，这里的教官更是博学多才，培训的进展非常神速。%s 很快掌握了许多关于军队专业训练的方法和知识，这让他自己也受益匪浅。很快就可以准备回到San Mona报道了。", selectedName, selectedName, selectedName))
					end
				end
			}
		end
	},
	[10] = -- chain from Hidden Event 5
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format(" 武器实弹射击的教学效果惊人，这些外国教官经验丰富，几乎了解所有武器的最佳用途。虽然对新兵来说接受这些可能有点难。但是 %s 却一点都没有浪费时间。", selectedName),
				TopButtonText = "... 专注于常规武器训练。",
				BottomButtonText = "... 学习一种射击技巧。",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
					CAddIntel(50)
					if topButton then
						CAdjustStat(selectedProfileId, STATS.AGILITY, 2)
						CAdjustStat(selectedProfileId, STATS.DEXTERITY, 6)
						CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 10)
						CResolveEvent(string.format("[%s: +敏捷, +灵巧, +枪法][+情报] 整整一周 %s 废寝忘食全身心的投入训练，经过系统训练他的命中率比以前高多了，现在 %s 将伺机寻找逃跑的机会，并把所见所闻做成报告一并带回。", selectedName, selectedName, selectedName))
					else
						pool = {
							{
								SkillId = SKILLS.MARKSMAN, 
								SkillName = "神射手"
							},
							{
								SkillId = SKILLS.HEAVY_WEAPONS,
								SkillName = "重武器"
							},
							{
								SkillId = SKILLS.AUTO_WEAPONS,
								SkillName = "自动武器"
							},
							{
								SkillId = SKILLS.GUNSLINGER,
								SkillName = "快枪手"
							},
							{
								SkillId = SKILLS.HUNTER,
								SkillName = "猎兵"
							}
						}

						skills = CGetSkills(selectedProfileId)
						finalPool = {}

						for k,v in ipairs(pool) do
							found = false
							for _,id in ipairs(skills) do
								if id == v.SkillId then
									found = true
								end
							end

							if found == false then
								finalPool[#finalPool+1] = v
							end
						end


						-- learn a new skill. or if they already have athletics+bodybuilding+stealthy (!) then just do the same as the first option
						if #pool >= 1 then
							index = math.random(#pool)
							learned = pool[index].SkillName
							CAddSkill(selectedProfileId, pool[index].SkillId)
							CResolveEvent(string.format("[%s: 获得 %s 技能][+情报] 整整一周 %s 废寝忘食全身心的投入训练，他专心研究一种武器，已经对它们了如指掌。现在 %s 将伺机寻找逃跑的机会，并把所见所闻做成报告一并带回。", selectedName, learned, selectedName))
						else
							CAdjustStat(selectedProfileId, STATS.AGILITY, 2)
							CAdjustStat(selectedProfileId, STATS.DEXTERITY, 6)
							CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 10)
						CResolveEvent(string.format("[%s: +敏捷, +灵巧, +枪法][+情报] 整整一周 %s 废寝忘食全身心的投入训练，经过系统训练他的命中率比以前高多了，现在 %s 将伺机寻找逃跑的机会，并把所见所闻做成报告一并带回。", selectedName, selectedName, selectedName))
						end
					end
				end
			}
		end
	},
	[11] = -- chain from Hidden Event 6
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format("在过去的一周里，%s 收集了大量情报，他在这里已经待的太久了，有不少人开始对他的身份产生了怀疑。现在是时候撤退了，有两条撤退方案需要你选择。。。", selectedName, selectedName),
				TopButtonText = "趁夜色步行逃走。",
				BottomButtonText = "寻找车库，乘汽车逃走。",
				Resolution = function(topButton)
					if topButton then
						CAddIntel(250)
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(36,72))
						CResolveEvent(string.format("[+情报] 敌人不可能完全包围城市，%s 在夜色的掩护下溜出城去。步行跋涉需要很长时间，%s 预计将在几天后重新回归您的小队。", selectedName, selectedName))
					else
						CAddIntel(125)
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(12,24))
						CResolveEvent(string.format("[+情报] 凭借手中的情报，%s 很轻松的到达了存放车辆的仓库，然而由于撤退行动准备的比较仓促，他无法带走全部的资料。不过好消息是乘车很快就能回归您的小队。.", selectedName, selectedName))
					end
				end
			}
		end
	},
	[12] = -- chain from Hidden Event 6
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format("在你为本地投毒做计划时，另一个机会出现在眼前，一支从 Meduna 出发向外运送补给的队伍即将启程。你现在要选择向 Meduna 本地驻军投毒，还是向外运的物资投毒。前者会对本地驻军产生更集中的效果，而后者可能削弱全国的军队。不管怎样，%s 完成他的任务后，就会向San Mona撤退。", selectedName, selectedName),
				TopButtonText = "向 Meduna 驻军投毒。",
				BottomButtonText = "向外运的物资投毒。",
				Resolution = function(topButton)
					if topButton then
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
						for i=1,16 do
							for j=1,16 do
								townId, _ = CGetTownId(i,j)
								if townId == TOWNS.MEDUNA then
									CAdjustEnemyStrengthInSector(i, j, -math.random(5,8), -math.random(5,8), -math.random(5,8), 0, 0, 0)
								end
							end
						end
						CResolveEvent(string.format("[MEDUNA: 驻军减少] %s 将毒药投放在 Meduna 本地的补给物资中。在这些物资被发现并被销毁前应该会产生显着的影响。下毒之后，%s 启程前往 San Mona。", selectedName, selectedName))
					else
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
						for i=1,16 do
							for j=1,16 do
								townId, _ = CGetTownId(i,j)
								if townId == TOWNS.OMERTA
								or townId == TOWNS.DRASSEN
								or townId == TOWNS.ALMA
								or townId == TOWNS.GRUMM
								or townId == TOWNS.CAMBRIA
								or townId == TOWNS.SANMONA
								or townId == TOWNS.ESTONI
								or townId == TOWNS.BALIME
								or townId == TOWNS.CHITZENA then
									CAdjustEnemyStrengthInSector(i, j, -math.random(0,3), -math.random(0,3), -math.random(0,3), 0, 0, 0)
								end
							end
						end
						CResolveEvent(string.format("[除 MEDUNA 的其他城镇: 驻军减少] %s 将毒药投放到运往全国各个城镇驻军的补给品中。虽然效果不像集中投放那么剧烈，但体质稍弱一点的敌兵也无法抵抗。下毒之后，%s 启程前往 San Mona。", selectedName, selectedName))
					end
				end
			}
		end
	},
	[13] = -- chain from Hidden Event 6
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format("Meduna的装甲部队会进行定期检修，而 %s 能利用这一点，部分削弱他们的战斗力 。或者走一条破坏性更强的路线，直接炸毁车辆和设施，但这样做会面临不小的风险，这需要 %s 对爆破物有一定的了解。。。", selectedName, selectedName),
				TopButtonText = "利用检修的机会破坏车辆。",
				BottomButtonText = "... 炸毁车辆。",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))

					if topButton then
						for i=1,16 do
							for j=1,16 do
								townId, _ = CGetTownId(i,j)
								if townId == TOWNS.MEDUNA then
									CAdjustEnemyStrengthInSector(i, j, 0, 0, 0, 0, 0, -1)
								end
							end
						end
						CAdjustStat(selectedProfileId, STATS.MECHANICAL, 5)
						CResolveEvent(string.format("[成功][%s: +机械]  %s 潜入了敌人的维修设施，你很惊讶这里一点安全防卫措施都没有，对车辆破坏易如反掌，虽然可能不如直接炸毁它们有效，但这样做更安全。完成工作后，%s离开 Meduna 前往San Mona。", selectedName, selectedName, selectedName))
					else
						demo, demoName, demoId = CCheckForSkill(SKILLS.DEMOLITIONS, selectedProfileId)
						exp, expName, expId = CGetStat(STATS.EXPLOSIVE, selectedProfileId)

						value = exp
						if demo == true then
							value = value + 25
						end

						admin, troop, elite, robot, jeep, tank = 0, 0, 0, 0, 0, 0
						message = ""

						if value < 50 then
							message = string.format("[失败] %s 没有足够的爆破知识能引爆坦克，更不用说设施了。由于没有足够的时间执行另一个计划，%s 开始向 San Mona 逃跑。", selectedName, selectedName)
						elseif value < 80 then
							admin, troop, elite = -2, -3, -2
							jeep, tank = -2, -math.random(1,3)
							message = string.format("[成功][MEDUNA: 装甲部队减少] 在 Meduna 驻军坦克上小心的安置了一些微型炸药，虽然不足以完全摧毁坦克，但也足够削弱它们的战斗力。在军队还没弄清发生了什么事之前，%s 偷偷溜出城去。", selectedName)
							CSetEnemyGroupVisibility(true)
						elseif value < 110 then
							admin, troop, elite = -20, -math.random(5,10), -math.random(3,7)
							robot, jeep, tank = -math.random(1,4), -2, -2
							message = string.format("[成功][MEDUNA: 驻军减少] 驻军营地发生了一连串的爆炸，巨大的爆炸声震撼了整座城市。绝大部分坦克都被炸成废铁，步兵部队也受到了重创。 Meduna 的城市防御受到了沉重的打击，%s 趁乱逃往San Mona。", selectedName)
							CSetEnemyGroupVisibility(true)
						else
							admin, troop, elite = -20, -math.random(10,15), -math.random(10,15)
							robot, jeep, tank = -math.random(1,4), -10, -10
							message = string.format("[成功][MEDUNA: 驻军大量减少] 简直就是一场灾难！Meduna 的装甲部队维修基地发生了规模空前的爆炸。爆炸又引起了坦克弹药的殉爆，接二连三的炸不绝于耳。Meduna的防御被空前削弱。 %s 没有时间再陶醉在他制造的这场艺术中，他必须趁着这场混乱迅速逃离。", selectedName)
							CSetEnemyGroupVisibility(true)
						end

						for i=1,16 do
							for j=1,16 do
								townId, _ = CGetTownId(i,j)
								if townId == TOWNS.MEDUNA then
									CAdjustEnemyStrengthInSector(i, j, admin, troop, elite, robot, jeep, tank)
								end
							end
						end
						CResolveEvent(message)
					end
				end
			}
		end
	},
	[14] = -- chain from Hidden Event 7
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format(" 和预料的一样简单，%s 甚至没费吹灰之力就截获了本国的经济数据。经过分析，大量的金钱被用于军事领域，还有少量的一些钱用于维持矿山运营。%s 通过网络可以快速转移矿山的运营费用，或者多花些时间研究研究怎么搞到军费。", selectedName, selectedName),
				TopButtonText = "见好就收。",
				BottomButtonText = "放长线，钓大鱼。",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)

					if topButton then
						CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
						CAddMoneyToPlayerAccount(75000)
						CScreenMsg("从女王那偷了 $75000。")
						CResolveEvent(string.format("[+$75000] %s 将小部分资金转账到海外银行，通过各个势力运作，这笔钱最终洗白并存入你的账户。任务完成后，%s 寻找机会向 San Mona 撤退。", selectedName, selectedName))
					else
						CSendMercOnMiniEvent(selectedProfileId, math.random(120,144))
						CAddMoneyToPlayerAccount(500000)
						CScreenMsg("从女王那偷了 $500000。")
						CResolveEvent(string.format("[+$500000] 经过全力运作，%s 将一大笔钱存入了你的账户。接下来的几天时间里 %s 需要小心翼翼的掩盖行踪，清除关于他的一切痕迹，并让所以相关人员彻底闭嘴。完成这些任务后，他将回到San Mona报告。", selectedName, selectedName))
					end
				end
			}
		end
	},
	[15] = -- chain from Hidden Event 7
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format("事实证明，%s 在上周收集的信息里有大量可以提炼的情报。同时还有一份情报可以用来策反部分敌军。提炼情报和策反敌军你只有时间进行一个，%s 应该如何选择？", selectedName, selectedName),
				TopButtonText = "提炼情报。",
				BottomButtonText = "策反敌兵。",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))

					if topButton then
						CAddIntel(500)
						CResolveEvent(string.format("[+情报] %s 将他的撤退计划和巨量且有用的情报发送给你，尔后 %s 将按计划撤离到 San Mona，估计一天后就可以归队了。", selectedName, selectedName))
					else
						CAddIntel(250)
						leadership, _, __ = CGetStat(STATS.LEADERSHIP, selectedProfileId)
						
						message = ""
						if leadership < 25 then
							message = string.format("[失败][+情报] %s 缺乏能说服别人加入到你身边的魅力和领导力，策反计划失败了。辛苦收集到的大量情报必须即刻销毁。%s 紧急从 Meduna 撤退到安全的地方 - San Mona。", selectedName, selectedName)
						elseif leadership < 70 then
							CCreateMilitia(0, 10, 10, 5, 3)
							CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 3)
							message = string.format("[成功][+情报][%s: +领导] 通过勒索、魅力和智慧，%s 成功策反了大量敌军士兵。他们已经在 San Mona 蓄势待发。 %s 完成扫尾工作后也将在大约一天到后达那里。", selectedName, selectedName, selectedName)
						else
							CCreateMilitia(0, 0, 25, 5, 3)
							CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 3)
							message = string.format("[成功][+情报][%s: +领导]  一大批女王卫队在 %s 的影响下加入了反抗军。他们带着最好的装备在 San Mona 蓄势待发。 %s 完成扫尾工作后也将在大约一天到后达那里。", selectedName, selectedName, selectedName)
						end
						CResolveEvent(message)
					end
				end
			}
		end
	},
	[16] = -- chain from Hidden Event 7
	{
		INIT = function(mercList)
			selectedName = ""
			selectedProfileId = -1
			for name, id in pairs(mercList) do
				time = CGetHoursRemainingOnMiniEvent(id)
				if time > 65000 then
					selectedName = name
					selectedProfileId = id
					break
				end
			end

			if selectedProfileId == -1 then
				-- merc was dismissed/fired/other?
				return {
					BodyText = "你派遣的佣兵消失了，没有人知道他去哪了。",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[无事发生] 你派遣的佣兵没有如期出现，潜入的机会被错过了。")
					end
				}
			end

			return {
				BodyText = string.format("这大量的文物只是女王收藏的一部分。你可以把这些文物送回它们原来的地方，民众们会因此欣喜若狂。或者联系境外的收藏家，很多人愿意为这些文物出高价。", selectedName, selectedName),
				TopButtonText = "将文物还给本地人。",
				BottomButtonText = "联系私人买家，人民不会知道的。",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))

					if topButton then
						CAddTownLoyalty(TOWNS.OMERTA, 10000)
						CAddTownLoyalty(TOWNS.DRASSEN, 10000)
						CAddTownLoyalty(TOWNS.ALMA, 10000)
						CAddTownLoyalty(TOWNS.GRUMM, 10000)
						CAddTownLoyalty(TOWNS.CAMBRIA, 10000)
						CAddTownLoyalty(TOWNS.SANMONA, 10000)
						CAddTownLoyalty(TOWNS.ESTONI, 10000)
						CAddTownLoyalty(TOWNS.BALIME, 10000)
						CAddTownLoyalty(TOWNS.MEDUNA, 10000)
						CAddTownLoyalty(TOWNS.CHITZENA, 10000)
						CAdjustStat(selectedProfileId, STATS.WISDOM, 2)
						CResolveEvent(string.format("[全部城镇: +忠诚度][%s: +智慧] %s 能够在没有人注意的情况下将一些小文物偷偷带出了宫殿。他需要花一天左右的时间来准备撤退计划，然后出发前往 San Mona。", selectedName, selectedName))
					else
						CAddMoneyToPlayerAccount(100000)
						CScreenMsg("走私文物获得 $100000。")
						CResolveEvent(string.format("[+$100000] %s 能够在没有人注意的情况下将一些小文物带出了宫殿。%s 很快就联系好了境外买家，伪造出口手续大约还需要一天的时间， 完成任务后他就会向 San Mona 撤退。", selectedName, selectedName))
					end
				end
			}
		end
	}
}
-- end events

--[[
This is the most common entry point into the script. The parameters are:
- mercTable: a table of pairs containing all of the mercs that can be affected by an event. Each pair is formatted { <MercNickname> = <MercProfileId> }.

You should initiate an event by calling ShowMsgBox() with an event indexed in the Events table.
For example:
	ShowMsgBox(Events[1].INIT(mercTable))
]]

function BeginRandomEvent(mercTable)
	-- pick an event at random, or do some logic here before selecting an event

	-- remove all mercs who are on a minievent
	validMercs = {}
	for name,id in pairs(mercTable) do
		if CGetHoursRemainingOnMiniEvent(id) == 0 then
			validMercs[name] = id
		end
	end


	ShowMsgBox(Events[math.random(#Events)].INIT(validMercs))
end

--[[
This is an alternate entry point into the script. The parameters are:
- eventId: an eventId. This should be an event indexed in an event table. Note that 0 is not a valid eventId.
- mercTable: a table of pairs containing all of the mercs that can be affected by an event. Each pair is formatted { <MercNickname> = <MercProfileId> }.

You should initiate an event by calling ShowMsgBox() with an event indexed in the Events or HiddenEvents table.
For example:
	ShowMsgBox(HiddenEvents[1].INIT(mercTable))

You probably won't need to change this.
]]
function BeginSpecificEvent(eventId, mercTable)
	ShowMsgBox(HiddenEvents[eventId].INIT(mercTable))
end
