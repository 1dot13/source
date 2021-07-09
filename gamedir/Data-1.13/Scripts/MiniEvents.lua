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
CGetTownId(x, y) returns {townId}
	- x, y indicate the sector coordinates to check
	- returns a town id, which will match up with the TOWNS enum below.
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
		townId = CGetTownId(x, y)

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

function GetTownName(townId)
	townName = ""

	if townId == TOWNS.OMERTA then
		townName = "Omerta"
	elseif townId == TOWNS.DRASSEN then
		townName = "Drassen"
	elseif townId == TOWNS.ALMA then
		townName = "Alma"
	elseif townId == TOWNS.GRUMM then
		townName = "Grumm"
	elseif townId == TOWNS.CAMBRIA then
		townName = "Cambria"
	elseif townId == TOWNS.SANMONA then
		townName = "San Mona"
	elseif townId == TOWNS.ESTONI then
		townName = "Estoni"
	elseif townId == TOWNS.BALIME then
		townName = "Balime"
	elseif townId == TOWNS.MEDUNA then
		townName = "Meduna"
	elseif townId == TOWNS.CHITZENA then
		townName = "Chitzena"
	end

	return townName
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

			local BODY = string.format("[%s] Your mercs are feeling a tad hungry when they come across a thicket of bushes covered in red berries. The berries look delicious, but they hesitate. They could be poisonous.", sectorStr)
			local BTN1 = "Eat the berries."
			local BTN2 = "Ignore the berries."
			local RESOLVE = function(topButton)
				if (topButton) then
					local result, skillOwnerName, skillOwnerId = CCheckForSkill(SKILLS.SURVIVAL, sector.X, sector.Y, sector.Z)

					if result then 
						CResolveEvent(string.format("[SURVIVAL][No effect] The squad grabs berries by the handful and is about to devour them when %s realizes that they are indeed poisonous and shouts a warning. They throw the berries back into the brush and look a little sad, yet grateful.", skillOwnerName))
					else
						for name, id in pairs(mercs) do
							CAdjustBreathMax(id, -25)
						end
						CResolveEvent("[All: -Breath] The squad grabs berries by the handful and devours them. They are the most delicious berries that they've ever tasted. This is the best decision they've made all day... until it isn't. Their stomachs become incredibly upset and, one they calm down, the team feels drained.")
					end
				else
					CResolveEvent("[No effect] Not wanting to regret making a stupid decision, the mercs refrain from eating the berries and continue about their day.")
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
				BodyText = string.format("[%s] Your mercs are feeling a tad hungry when they come across a thicket of bushes covered in green berries. The berries look delicious, but they hesitate. They could be poisonous.", sectorStr),
				TopButtonText = "Eat the berries.",
				BottomButtonText = "Ignore the berries.",
				Resolution = function(topButton)
					if (topButton) then
						for name, id in pairs(mercs) do
							CAdjustMorale(id, 3)
						end
						CResolveEvent("[All: +Morale] The squad grabs berries by the handful and devours them. They are the most delicious berries that they've ever tasted. This is the best decision they've made all day.")
					else
						CResolveEvent("[No effect] Not wanting to regret making a stupid decision, the mercs refrain from eating the berries and continue about their day.")
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
				BodyText = string.format("[%s] A gentle rustling in a nearby bush alerts your squad. They drop down and prepare for an ambush. Suddenly, a small puppy runs out towards the team and begins barking in a high-pitched tone and then wags its tail. What is such a small, defenseless puppy doing here?", sectorStr),
				TopButtonText = "Leave it alone.",
				BottomButtonText = "Look for its owner.",
				Resolution = function(topButton)
					if topButton then
						CResolveEvent("[No effect] The puppy is unable to charm your squad. They continue on, leaving the puppy to fend for itself.")
					else
						name, id = GetRandomMerc(mercs)
						for k, v in pairs(mercs) do
							CAdjustMorale(v, 2)
							CAdjustBreathMax(v, -10)
						end
						
						CResolveEvent(string.format("[All: -Breath, +Morale] It takes some time, but eventually the squad runs into a young girl desperately shouting into the wild. The puppy leaps from %s's arms and runs to her. She is incredibly relieved and quickly thanks the team before running back towards town.", name))
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
				BodyText = string.format("[%s] A glint coming from a grassy area catches your mercs' attention. The team moves closer, it appears to be a reinforced lockbox of some kind. It's rusted and in poor condition, indicating that it's been here for some time. The team keeps their distance. It could be a trap.", sectorStr),
				TopButtonText = string.format("Have %s try to pick the lock.", mecName),
				BottomButtonText = string.format("Have %s detonate the box.", expName),
				Resolution = function(topButton)
					if topButton then
						if mecVal >= 75 then
							id = GetMercIdFromName(mercs, mecName)
							CAddIntel(50)
							if id ~= -1 then
								CAdjustStat(id, STATS.MECHANICAL, 1)
							end
							
							CResolveEvent("[Success, +Intel, +MEC] The lockbox swings open to reveal documents in surprisingly good condition. It looks like this was a dead drop for someone. These papers look like they will be very valuable for certain parties.")
						else
							id = GetMercIdFromName(mercs, mecName)
							if id ~= -1 then
								CAdjustMorale(id, -1)
							end

							CResolveEvent(string.format("[Failure][-Morale] %s grows increasingly frustated, making no progress in opening the box. Eventually, their anger gets the better of them and they fire several rounds into the box, to reveal a messy stack of documents full of bullet holes.", mecName))
						end
					else
						id = GetMercIdFromName(mercs, expName)

						if id ~= -1 then
							CAdjustStat(id, STATS.EXPLOSIVE, 1)
						end

						if expVal >= 70 then
							CResolveEvent(string.format("[Success, +EXP] %s waves the squad back to a safe distance and detonates the box. There is a satisfying bang and puff of smoke. That box won't be bothering anyone anymore.", expName))
						else
							CApplyDamage(id, 15)
							CResolveEvent(string.format("[Success-ish, +EXP, -HP] After setting up a small explosive, %s gleefully presses the big red button and... nothing happens. They then make the rookie mistake of approaching the box when it finally goes off. %s is thrown back, a little worse for wear. Thankfully, they weren't closer.", expName, expName))
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
				BodyText = string.format("[%s] A small group of soldiers call out, and your mercs quickly turn and raise their weapons at them. The soldiers' hands are raised and their weapons are slung across their backs. \"Don't shoot!\" they say. \"We want to defect.\" Having spoken, they stop walking. It is very quiet.", sectorIdString),
				TopButtonText = "Accept them, and have them join you as militia.",
				BottomButtonText = "It could be a trap. Gun them down.",
				Resolution = function(topButton)
					if topButton then
						CCreateMilitia(math.random(2,5), math.random(2,5), math.random(0,2), sector.X, sector.Y)
						CResolveEvent("[+Militia] They breathe a collective sigh of relief having heard your response. The soldiers cheerfully approach your mercs and introduce themselves. \"Glad that went well,\" one of them says. \"We're all yours now.\"")
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(0,20))
						end

						CResolveEvent("[All: -HP] The soldiers barely finish speaking when your mercs open fire. They're caught by surprise, but a brief firefight ensues as your mercs finish them off.")
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
				BodyText = string.format("You receive a message from one of Miguel's rebel runners, asking to borrow one of your mercs for a few hours. The runner hasn't been entrusted with all of the details, but she is certain that the job won't involve any shooting. Probably. She also says Miguel won't take no for an answer, but he will compensate you for their time."),
				TopButtonText = string.format("Send %s.", mercs[1].Name),
				BottomButtonText = string.format("Send %s.", mercs[2].Name),
				Resolution = function(topButton)
					CAddIntel(20)

					if topButton then
						CAdjustStat(mercs[1].Id, STATS.STRENGTH, 1)
						CAdjustStat(mercs[1].Id, STATS.DEXTERITY, 1)
						CSendMercOnMiniEvent(mercs[1].Id, 8)
						CResolveEvent(string.format("[+Intel, %s: +STR, +DEX] It turns out that the job is mostly being a voice of authority while helping out the rebels do odd jobs. It's not very exciting, but at least %s is able to get some exercise in.", mercs[1].Name, mercs[1].Name))
					else
						CAdjustStat(mercs[2].Id, STATS.STRENGTH, 1)
						CAdjustStat(mercs[2].Id, STATS.DEXTERITY, 1)
						CSendMercOnMiniEvent(mercs[2].Id, 8)
						CResolveEvent(string.format("[+Intel, %s: +STR, +DEX] It turns out that the job is mostly being a voice of authority while helping out the rebels do odd jobs. It's not very exciting, but at least %s is able to get some exercise in.", mercs[2].Name, mercs[2].Name))
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
				BodyText = string.format("[%s] A group of men leap out of the wilderness and quickly surround your team, weapons drawn. Before anything happens, a voice calls out from behind. \"Whoa, everyone relax and lower your guns. This lot is the reason we can do this. Let 'em go. They'd probably kill you all in a fight anyway.\" The leader winks at your mercs.", sectorIdString),
				TopButtonText = "Attack the bandits.",
				BottomButtonText = "Continue on and let the bandits be.",
				Resolution = function(topButton)
					if topButton then
						name, id = GetRandomMerc(mercs)
						CAdjustStat(id, STATS.MARKSMANSHIP, 1)

						for k,v in pairs(mercs) do
							CApplyDamage(v, math.random(0,20))
						end

						CResolveEvent(string.format("[All: -HP, %s: +MRK] The bandits relax and begin to form up. With their backs turned, your mercs open fire. Some flee, but others happily engage. They put up a decent fight before the last one drops.", name))
					else
						CResolveEvent("[No effect] The bandits seem a little disappointed as they make way and prepare for their next victim.")
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
				BodyText = string.format("[Helicopter] \"I didn't think you could get airsick, Cap!\" Skyrider chuckles from the front of the chopper. %s didn't think so either, but is in no shape to say anything. The helicopter isn't the only thing furiously rumbling.", name),
				TopButtonText = string.format("Have %s try to tough it out.", name),
				BottomButtonText = string.format("%s vomits.", name),
				Resolution = function(topButton)
					if topButton then
						CAdjustBreathMax(id, -50)
						CResolveEvent(string.format("[%s: -Breath] Skyrider eventually manages to make the flight smoother, but %s is paler than a ghost by the time that happens. By sheer force of will, their stomach's contents stay there.", name, name))
					else
						for k,v in pairs(foundMercs) do
							if v == id then
								CAdjustMorale(id, -15)
								CAdjustBreathMax(id, -25)
							else
								CAdjustMorale(v, 5)
							end
						end

						CResolveEvent(string.format("[%s: -Breath, -Morale, Everyone else: +Morale] Despite %s's best efforts, they can't keep their stomach down. It gets everywhere, mostly soiling their clothes. Everyone else has a great laugh at their expense.", name, name))
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
				BodyText = string.format("[%s] It's slowly dawning on your team that, having travelled off the beaten path in search of a shortcut, they might be lost. It's probably possible to backtrack, but who knows? They may have actually found a shortcut.", sectorIdString),
				TopButtonText = "Press onward.",
				BottomButtonText = "Try to backtrack.",
				Resolution = function(topButton)
					result, skillName, skillId = CCheckForSkill(SKILLS.SURVIVAL, x, y, z)

					if topButton then
						if result then
							for k,v in pairs(mercsOnFoot) do
								CAdjustBreathMax(v, -15)
							end

							CResolveEvent(string.format("[SURVIVAL][All: -Breath] It's definitely no shortcut, but %s is eventually able to get the team back to familiar ground. The trek seems to have taken a bit of energy from everyone, though.", skillName))
						else
							lostTime = math.random(3,5)
							for k,v in pairs(mercsOnFoot) do
								CSendMercOnMiniEvent(v, lostTime)
							end

							CResolveEvent(string.format("[TEAM UNAVAILABLE] With a firm belief in the shortcut, your team continues on. After some time, they realize that they are truly lost. It will take a few hours to find their way back."))
						end
					else
						if result then
							CResolveEvent(string.format("[SURVIVAL][No effect] %s is able to lead the team back the way they came with minimal time wasted.", name))
						else
							for k,v in pairs(mercsOnFoot) do
								CAdjustBreathMax(v, -20)
								CAdjustMorale(v, -5)
							end

							CResolveEvent("[All: -Breath, -Morale] Finding the way back is a long and arduous task. By the time the squad is somewhere they recognize, they're exhausted and miserable.")
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
				BodyText = string.format("[%s] What should have been a minor nitpick quickly devolves into a heated argument, and soon enough %s and %s are standing and shouting at each other. It doesn't look like either of them will back down.", sectorIdString, fighters[1].Name, fighters[2].Name),
				TopButtonText = "Let them fight.",
				BottomButtonText = "Raise the stakes. $5000 to the winner.",
				Resolution = function(topButton)
					if topButton then
						CApplyDamage(fighters[1].Id, math.random(10,20))
						CApplyDamage(fighters[2].Id, math.random(10,20))
						CResolveEvent(string.format("[%s, %s: -HP] It's a spectacular scuffle, and it gives both of them the chance to blow off some steam. In the end, they're both a little bloodied, but more importantly a lot calmer.", fighters[1].Name, fighters[2].Name))
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
						CScreenMsg(string.format("Paid $5000 to %s for winning the fistfight.", fighters[winnerIndex].Name))

						CResolveEvent(string.format("[-$5000][%s: -HP, +STR, +AGI, +Morale, %s: -HP, +STR, +AGI] The promise of extra cash is encouragement enough to not pull punches. It gets ugly. %s and %s go all out and the fight ends with %s as the winner.", fighters[winnerIndex].Name, fighters[loserIndex].Name, fighters[1].Name, fighters[2].Name, fighters[winnerIndex].Name))
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
				BodyText = string.format("[%s] In the distance, your team can see a small group of bloodcats stalking a large group of civilians. While there is safety in numbers, more than a couple civilians will die before the bloodcats can be driven off or fled from.", sectorIdString),
				TopButtonText = "Try to warn the group of civilians.",
				BottomButtonText = "There is nothing that can be done.",
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
							CResolveEvent(string.format("[Success: MARKSMANSHIP][%s: +MRK] %s lines up a shot and fires. A moment later, the leading bloodcat drops, the bullet cleanly piercing its head. The noise alerts the civilians and spooks the bloodcats, allowing the panicking crowd to flee to safety.", bestMrkName, bestMrkName))
						else
							for name, id in pairs(mercs) do
								CAdjustMorale(id, -10)
							end
							CResolveEvent("[Failure][-Morale] Your team tries to warn the civilians as best they can - firing their weapons, shouting, making themselves noticeable - but it's too late. The bloodcats strike swiftly and a few civilians are dragged away, never to be seen again.")
						end
					else
						for name, id in pairs(mercs) do
							CAdjustMorale(id, -10)
						end
						CResolveEvent("[-Morale] The squad watches helplessly as a few civilians, taken completely by surprise, are mauled and dragged away from the rest. The bloodcats will eat well tonight.")
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
				BodyText = string.format("You receive a message from the rebels, asking for extended and more involved support. They're asking for %s specifically, and won't take anyone else. It sounds like they'll be involved in overseeing some rebel maneuvers, and it will take several days for the operation to complete.", selectedName),
				TopButtonText = string.format("Accept. Send %s.", selectedName),
				BottomButtonText = string.format("Decline. You can't spare %s right now.", selectedName),
				Resolution = function(topButton)
					if topButton then
						hours = math.random(168, 240)
						CSendMercOnMiniEvent(selectedProfileId, 65535)
						CResolveEvent(string.format("[%s: UNAVAILABLE] %s begins the trek to the rebel staging area. You're not sure when they'll return, but this had better be worth their absence.", selectedName, selectedName), 1, hours)
						CScreenMsg(string.format("%s has temporarily left to work with the rebels, and will return in a week or so.", selectedName))
					else
						CResolveEvent("[No effect] The rebels will have to manage without your direct support for now. Perhaps another time.")
					end
				end
			}
		end
	},
	[13] = -- send a merc to San Mona in 24 hours
	{
		INIT = function(mercList)
			CScreenMsg("Unknown contact wants to meet in San Mona in 24 hours!")
			return {
				BodyText = "A civilian bumps into one of your mercs and quickly shoves a crumpled wad of paper into their hand. He gives a stern but knowing look, then immediately walks away without a word. Unfolding the paper, it reads \"Have something for you. Meet in San Mona in 24 hours. Don't look for me. I'll find you.\"",
				TopButtonText = "Alright...",
				BottomButtonText = "Hmm...",
				Resolution = function(topButton)
					CResolveEvent("The message has been delivered. Whether you follow up is your decision.", 2, 24)
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
				BodyText = string.format("[%s] A lone traveller walks towards your team. As he passes, he turns and speaks: \"Could you spare a little money? Actually, a lot of money.\" As he speaks, a large number of bandits emerge from hiding, guns raised. \"So how about some generosity? Say, a thousand a head?\" Your mercs are in a significantly disadvantaged position.", sectorIdString),
				TopButtonText = "Pay the bandits.",
				BottomButtonText = "Have your mercs fight their way out.",
				Resolution = function(topButton)
					if topButton then
						payment = count * 1000
						CAddMoneyToPlayerAccount(-payment, true)
						CScreenMsg(string.format("Paid $%d to bandits.", payment))
						CResolveEvent(string.format("[-$%d] \"Well, thank you, kind strangers! It was a pleasure meeting you. Have a nice rest of your day!\" The man continues to grin as your mercs move on, and the bandits move off in the opposite direction.", payment))
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(25,40))
						end
						CResolveEvent("[All: -HP] Your team quickly opens fire on the bandits, who respond in kind. However, their weapons aren't well maintained and they aren't well trained, so their position isn't as advantageous as it seemed. You guess you know what the money would have been used for.")
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
				BodyText = string.format("[%s] Well off the beaten path, your team runs into a group of enemy soldiers who are escorting some shackled prisoners. They obviously don't recognize your team. \"Stay back!\" one warns. \"We are transporting dangerous criminals.\" A moment later, two prisoners slip free and sprint into the tall grass.", sectorIdString),
				TopButtonText = "Try to help free the rest of the prisoners.",
				BottomButtonText = "Stay back. Let the soldiers try to catch the escapees.",
				Resolution = function(topButton)
					if topButton then
						if count >= math.random(6) then
							for name, id in pairs(mercs) do
								CAdjustStat(id, STATS.AGILITY, 1)
							end
							CResolveEvent("[Success][All: +AGI] As the soldiers are distracted by the escaping prisoners, your team is able to quickly rush to the rest of the prisoners to free them. Chaos erupts as the prisoners flee in all directions. Most are able to escape as your team withdraws.")
						else
							for name, id in pairs(mercs) do
								CApplyDamage(id, math.random(20,30))
							end
							CResolveEvent("[Failure][All: -HP] A mix of bad luck and low headcount means that your team fails to free the remainder of the prisoners. Your team will need some patching up as they beat a quick retreat.")
						end
					else
						CResolveEvent("[No effect] Your team backs off, wanting to stay out of this encounter. They leave before they can find out what happened to the escapees.")
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
				BodyText = string.format("[%s] Driving along a narrow road, your team suddenly finds itself facing a massive group of fallen trees blocking the way forward. The placement of the trees seems somewhat odd, and the mercs have a suspicious feeling about the whole situation.", sectorIdString),
				TopButtonText = "Clear the path.",
				BottomButtonText = "Find a way around.",
				Resolution = function(topButton)
					if topButton then
						demo, demoName, demoId = CCheckForSkill(SKILLS.DEMOLITIONS, sector.X, sector.Y, sector.Z)
						bodybuilder, bbName, bbId = CCheckForSkill(SKILLS.BODYBUILDING, sector.X, sector.Y, sector.Z)

						if demo == true then
							CAdjustStat(demoId, STATS.EXPLOSIVE, 1)
							CResolveEvent(string.format("[Success: DEMOLITIONS][%s: +EXP] The trees are too dense and heavy to remove conventionally, but with the power of explosives, they are destroyed and swept aside with ease.", demoName))
						elseif bodybuilder == true then
							for name, id in pairs(mercs) do
								CAdjustStat(id, STATS.STRENGTH, 1)
								CAdjustBreathMax(id, -20)
							end
							CAdjustStat(bbId, STATS.LEADERSHIP, 1)
							CResolveEvent(string.format("[Success: BODYBUILDING][All: +STR, -Breath][%s: +LDR] The trees aren't part of an ambush, but that doesn't make them any easier to clear. It's hard work, but %s is able to make it a little easier on everyone.", bbName, bbName))
						else
							for name, id in pairs(mercs) do
								CAdjustStat(id, STATS.STRENGTH, 1)
								CAdjustBreathMax(id, -50)
							end
							CResolveEvent("[All: +STR, -Breath] The trees aren't part of an ambush, but that doesn't make them any easier to clear. It's hard and exhausting work.")
						end
					else
						result, vehicleName = CAdjustVehicleFuel(mercInVehicleId, -15)
						CResolveEvent(string.format("[%s: -Fuel] The squad attempts to find an alternate way around the obstruction. They eventually find one, but it was quite a roundabout path. You may need to plan to refuel sooner than expected.", vehicleName))
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
				BodyText = string.format("[%s] Your team hears the unmistakable sounds of a firefight not too far away. Moving closer, they see a group of the Queen's soldiers fighting a small rebel group. It's clear that the rebels are fighting a losing battle. They are outnumbered and pinned in a defensive position. Without intervention, they'll be overwhelmed and killed in a matter of minutes.", sectorIdString),
				TopButtonText = "Immediately engage to support the rebels.",
				BottomButtonText = "Wait for an opportune moment to attack.",
				Resolution = function(topButton)
					if topButton then
						dep, depName, depId = CCheckForSkill(SKILLS.DEPUTY, sector.X, sector.Y, sector.Z)

						if dep == true then
							CCreateMilitia(math.random(3,5), math.random(0,3), 0, sector.X, sector.Y)
							CResolveEvent(string.format("[Success: DEPUTY][+Militia] The tide of the battle clearly changes once %s takes charge of the situation. The soldiers are driven off and the remaining rebels place themselves under your command.", depName))
						else
							for name, id in pairs(mercs) do
								CApplyDamage(id, math.random(20,25))
							end
							CResolveEvent("[Failure][All: -HP] Your team rushes in and tries to save the rebels, but the soldiers are able to keep pinning the rebels while the bulk of their group engages your mercs. Your team is able to fend off the soldiers and drive them away, but not before all of the rebels are killed.")
						end
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(15))
						end
						CResolveEvent(string.format("[All: -HP] Your team waits for the right moment. Unfortunately, it doesn't come in time. The rebels are wiped out and the soldiers are rummaging through their corpses when your team opens fire. The soldiers are caught off guard, and go down without too much of a fight."))
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
				BodyText = string.format("[%s] Taking a quick break, your team rests in a secluded area. It's very serene and quiet, until the sound of voices slowly becomes louder and louder. Your mercs aren't sure if the approaching group knows that they aren't alone.", sectorIdString),
				TopButtonText = "Try to sneak out and take a look.",
				BottomButtonText = "Wait and see what the group does.",
				Resolution = function(topButton)
					if topButton then
						stealth, stealthName, stealthId = CCheckForSkill(SKILLS.STEALTHY, sector.X, sector.Y, sector.Z)
						if stealth == true then
							CResolveEvent(string.format("[Success: STEALTHY] %s's stealth skills prove sufficient enough to remain unnoticed by a large group of bandits. They pass by without incident.", stealthName))
						else
							for name, id in pairs(mercs) do
								CApplyDamage(id, math.random(20,30))
							end
							CResolveEvent("[Failure][All: -HP] Your mercs attempt to hide and stay quiet, but a snapped twig just as the group of bandits is passing by alerts them to your team's presence. Everyone is caught scrambling to ready their weapons, and a brief gunfight ensues before the bandits flee.")
						end
					else
						covops, covopsName, covopsId = CCheckForSkill(SKILLS.COVERT_OPS, sector.X, sector.Y, sector.Z)
						if covops == true then
							CAdjustStat(covopsId, STATS.WISDOM, 1)
							CResolveEvent(string.format("[COVERT OPS][%s: +WIS] A group of soldiers comes out of the brush. %s quickly convinces them they your team is a group of hunters hired by farmers to drive away wildlife. A short chat later, the soldiers move on.", covopsName, covopsName))
						else
							for name, id in pairs(mercs) do
								CAdjustBreathMax(id, -20)
							end
							CResolveEvent("[All: -Breath] A group of soldiers comes out of the brush, but your team was waiting for this. After the scouting party is dealt with, your mercs quickly pack up and leave before anyone comes to investigate the noise.")
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
				BodyText = string.format("[%s] Your team is approached and surrounded by a group of civilians in heavy cloaks. One of them steps forward to reveal his face, clearly diseased. \"Hello,\" he says weakly. \"As you can see, we've been afflicted with something and must beg you for money for treatment in Cambria.\" As he gestures around, a pistol can be seen under his cloak. \"We'd prefer not to take no as an answer.\"", sectorIdString),
				TopButtonText = "Give them some money.",
				BottomButtonText = "Refuse.",
				Resolution = function(topButton)
					if topButton then
						CAddMoneyToPlayerAccount(-10000, true)
						CScreenMsg("Paid $10000 to a group of diseased farmers.")
						CResolveEvent("[-$10000] A small amount of money is offered at first, but the group presses for more. Your mercs quickly agree, wanting to get away from the diseased group as quickly as possible. After all, it's your cash, not theirs. The group moves on, satisfied.")
					else
						doc, docName, docId = CCheckForSkill(SKILLS.PARAMEDIC, sector.X, sector.Y, sector.Z)
						if doc == true then
							CAdjustStat(docId, STATS.MEDICAL, 2)
							CResolveEvent(string.format("[PARAMEDIC][%s: +MED] Your mercs refuse to hand over any money. Before they get too incensed, %s offers to look at them and offer some on the spot care. A quick checkup later, the group is satisfied and moves on.", docName, docName))
						else
							for name, id in pairs(mercs) do
								CAdjustBreathMax(id, -20)
								CAdjustStat(id, STATS.STRENGTH, -3)
								CAdjustStat(id, STATS.DEXTERITY, -3)
								CAdjustStat(id, STATS.AGILITY, -3)
							end
							CResolveEvent("[All: -Breath, -STR, -DEX, -AGI] The group refuses to take no for an answer. When they get too close to your team, fists start flying. The group is driven off, but your mercs feel a little sick from getting so close.")
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
				BodyText = string.format("[%s] Your mercs are going about their business as an unusually large flock of white birds begins to pass by. They're just far away enough that they might make some good target practice.", sectorIdString),
				TopButtonText = "Shoot at the birds.",
				BottomButtonText = "Let the birds pass undisturbed.",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.MARKSMANSHIP, 1)
						end
						CResolveEvent("[All: +MRK] Your team quickly turns this into an impromptu shooting competition. Shots are fired, birds fall from the sky, and a good time was had.")
					else
						CResolveEvent("The shots would be difficult, and the payoff probably minimal. Your team looks down from the sky and resumes their day.")
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
				BodyText = string.format("[%s] Your mercs are going about their business as an unusually large flock of black birds begins to pass by. They're just far away enough that they might make some good target practice.", sectorIdString),
				TopButtonText = "Shoot at the birds.",
				BottomButtonText = "Let the birds pass undisturbed.",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, -7)
							CAdjustStat(id, STATS.DEXTERITY, -7)
							CAdjustStat(id, STATS.AGILITY, -7)
						end
						CResolveEvent("[All: -STR, -DEX, -AGI] As the first shots are fired, your team notices something odd. They are both further away and larger than expected, and your mercs have just got their attention. The flock of carrion birds rapidly descends on your mercs, swooping everyone with diseased beaks and talons. Your team runs for cover among the trees as the relentless flock overwhelms them.")
					else
						CResolveEvent("The shots would be difficult, and the payoff probably minimal. Your team looks down from the sky and resumes their day.")
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
				BodyText = string.format("[%s] Your mercs are going through a small wooded area when they suddenly see a large bloodcat charging directly at them. Did they get too close to its den? Perhaps a fresh kill? Can't know for sure. It's clearly enraged, and there is very little time to react.", sectorIdString),
				TopButtonText = "Attempt to calm the bloodcat.",
				BottomButtonText = "Try to shoot the bloodcat.",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, -5)
							CAdjustStat(id, STATS.DEXTERITY, -5)
							CAdjustStat(id, STATS.AGILITY, -5)
						end
						CResolveEvent("[All: -STR, -DEX, -AGI] It takes about one second to realize that this is an absolutely terrible idea. The bloodcat is ferocious and inflicts severe wounds before your team can grab their weapons and kill it.")
					else
						CResolveEvent("[No effect] Your mercs quickly grab their weapons as the bloodcat closes down on them. A quick volley drops the bloodcat and it crumples lifelessly at their feet.")
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
				BodyText = string.format("[%s] Your mercs are surprised by a small group of bandits yelling and charging over a hill towards them. One by one, the bandits slow and stop. They look at each other, and then at your team. Deciding your mercs aren't easy prey, they turn around and begin to retreat back up the hill.", sectorIdString),
				TopButtonText = "Chase them down and kill them.",
				BottomButtonText = "Follow them.",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.AGILITY, 1)
						end
						CResolveEvent("[All: +AGI] Your mercs immediately charge up the hill and open fire. The young bandits are no match and are dispatched quickly.")
					else
						scout, scoutName, scoutId = CCheckForSkill(SKILLS.SCOUTING, sector.X, sector.Y, sector.Z)
						if scout == true then
							CAddMoneyToPlayerAccount(5000)
							CScreenMsg("Gained $5000 from bandits' loot.")
							CResolveEvent(string.format("[Success: SCOUTING][+$5000] There isn't much cover on the other side of the hill, and %s is easily able to watch them retreat back to their camp. Your mercs take a roundabout route to get the drop on the bandits, most of them barely older than teenagers. They quickly surrender their weapons and disperse towards town as your mercs claim the loot left behind.", scoutName))
						else
							CResolveEvent("[Failure][No effect] Your squad creeps up to the hilltop, watching the bandits retreat and following them. The bandits notice, and quickly drop their weapons and scatter in all directions. While your team wasn't able to corner them, they at least won't be bothering anyone else anytime soon.")
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
				BodyText = string.format("[%s] Your squad comes across a fork in the path ahead of them. One path looks clear and easy, and the other is overgrown with thorny vines and plants. It looks like both paths will get them to where they're going, but the whole situation feels slightly off - almost as if someone or something is watching. Still, a decision must be made.", sectorIdString),
				TopButtonText = "Take the clear path.",
				BottomButtonText = "Take the overgrown path.",
				Resolution = function(topButton)
					if topButton then
						CResolveEvent("[No effect] Your team is on their guard as they walk down the clear path. Jumpy and alert, it turns out that it was just their imagination after all.")
					else
						for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, -5)
							CAdjustStat(id, STATS.AGILITY, -5)
						end
						CResolveEvent("[All: -STR, -AGI] The overgrown path, as expect, is very unpleasant to walk through. Your mercs are constantly being pricked and scratched by sharp thorns covered in a strange sap. Shortly after emerging from the path, your team begins to feel a little sick.")
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
				BodyText = string.format("[%s] An approaching smoke and dust cloud in the distance reveals itself to be a group of enemy soldiers charging towards your team. As the roaring soldiers get closer, your mercs prepare for a fight but notice some silhouettes cresting the hill next to them. They're signalling for your team to get down and take cover.", sectorIdString),
				TopButtonText = "Take cover.",
				BottomButtonText = "Attack the soldiers.",
				Resolution = function(topButton)
					if topButton then
						CCreateMilitia(math.random(5,10), math.random(3,5), math.random(0,3), sector.X, sector.Y)
						CResolveEvent("[+Militia] Your team dives into cover just as the silhouetted figures open fire on the soldiers' flank. They're caught completely by surprise and quickly panic and flee. The rebels descend from the hill to greet your team, and offer to join as militia under your command.")
					else
						for name, id in pairs(mercs) do
							CApplyDamage(id, math.random(10,20))
						end
						CResolveEvent("[All: -HP] Your team attacks the incoming soldiers just as the rebels open fire from the hilltop. The smoke and dust makes it very hard for the rebels to determine friend from foe, who fire into the skirmish. After the area clears, the rebels come down. \"Sorry. We were just trying to help.\" ")
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
				BodyText = "Perhaps it's time to give your mercenaries a chance to relax and unwind. A few drinks should raise everyone's spirits - they just need to find themselves in one of Arulco's many pubs. Tell them to get there in...",
				TopButtonText = "... 12 hours.",
				BottomButtonText = "... 24 hours.",
				Resolution = function(topButton)
					if topButton then
						CResolveEvent("You get the message out to everyone: drinks are on you. Just be there in 12 hours.", 3, 12)
					else
						CResolveEvent("You get the message out to everyone: drinks are on you. Just be there in 24 hours.", 3, 24)
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
				BodyText = string.format("[%s] On a trip through town, your team spots an older gentleman carrying a large, intricate vase out his front door. As they get closer, the old man starts to struggle with the vase, sweat pouring from his brow. Under the strain, he loses his balance, and the fragile vase is about to tumble out of his hands.", sectorIdString),
				TopButtonText = "Try to catch the vase.",
				BottomButtonText = "Watch the vase fall to the ground.",
				Resolution = function(topButton)
					if topButton then
						ath, athName, athId = CCheckForSkill(SKILLS.ATHLETICS, sector.X, sector.Y, sector.Z)
						if ath == true then
							CAddMoneyToPlayerAccount(1000)
							CScreenMsg("Gained $1000 for delivering a vase.")
							CResolveEvent(string.format("[Success: ATHLETICS][+$1000] %s races forward and wraps their arms around the vase, preventing tragedy. The old man is relieved and offers to pay your team for the trouble of delivering the vase across town.", athName))
						else
							CAddMoneyToPlayerAccount(-5000, true)
							CScreenMsg("Paid $5000 for breaking a vase.")
							CResolveEvent("[Failure][-$5000] Your squad rushes forward and tries to catch the falling vase, but trip over themselves just as they reach the old man. The vase falls and shatters on the ground. The old man is livid and demands compensation for breaking it. Your mercs pay him more than a fair amount.")
						end
					else
						CResolveEvent("[No effect] Your team watches helplessly as the vase flies out of the old man's hands and hits the ground, shattering into hundreds of pieces. He looks distraught and a little angry. Your mercs exchange a sympathetic glance with him as they continue on, leaving him to deal with the mess.")
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
				BodyText = string.format("[%s] The team is taking a quick break when they're approached by a merchant with a fake smile on his face. \"You must be the mercenaries doing such good work here,\" he says, \"and I have an offer for you. I have some goods that I need an eye kept on for the next few hours. Nothing complicated. Can I count on you?\"", sectorIdString),
				TopButtonText = "Agree to guard his goods.",
				BottomButtonText = "Decline.",
				Resolution = function(topButton)
					if topButton then
						hours = math.random(8,12)
						for name, id in pairs(mercs) do
							CSendMercOnMiniEvent(id, hours)
						end
						CAddMoneyToPlayerAccount(7500)
						CScreenMsg("Gained $7500 for guard duty.")
						CResolveEvent("[TEAM UNAVAILABLE][+$7500] \"Great! I'll come back when the deal is closed. Shouldn't be more than a few hours.\" Your squad heads to a nondescript building to watch over a large number of heavy crates. Aside from shooing away some curious kids, the time passes uneventfully.")
					else
						CResolveEvent("[No effect] You have plans for your mercs that cannot be delayed. The merchant looks worried, but leaves nonetheless.")
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
			townId = CGetTownId(sector.X, sector.Y)
			townName = GetTownName(townId)

			return {
				BodyText = string.format("[%s] A kid approaches your group excitedly. \"Hey, you guys are famous!\" he says with a big smile. \"One of my friends keeps going on about how awesome you are! Wanna come hang out with us? Tell us stories? Please?\"", sectorIdString),
				TopButtonText = "Agree to follow the kid.",
				BottomButtonText = "Tell him he's got the wrong group.",
				Resolution = function(topButton)
					if topButton then
						CAddTownLoyalty(townId, 2500)
						CResolveEvent(string.format("[%s: +Loyalty] The kid leads your group to a small group of his friends in the middle of town. His excitement never wavers, and all of them press your group for more stories and more details about their adventures. Eventually, they're satisfied and run off.", townName))
					else
						CResolveEvent("[No effect] The mercs try to calm the kid down and eventually turn him away, saying that he's confused them with another group. He looks crushed as your mercs walk away.")
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
				BodyText = string.format("[%s] Your group sees what must be one of the few remaining drivers in Arulco. He's standing in front of his car, its engine exposed, scratching his head. Looks like he could use some help.", sectorIdString),
				TopButtonText = "Stop and help him out.",
				BottomButtonText = "Continue on.",
				Resolution = function(topButton)
					if topButton then
						tech, techName, techId = CCheckForSkill(SKILLS.TECHNICIAN, sector.X, sector.Y, sector.Z)
						if tech == true then
							CAdjustStat(techId, STATS.MECHANICAL, 1)
							CAddIntel(20)
							CResolveEvent(string.format("[Success: TECHNICIAN][%s: +MEC][+Intel] Taking a closer look, %s quickly spots the problem and is able to help the man out. He's very grateful. Having driven around quite a bit recently, he shares what info he can about your enemy.", techName, techName))
						else
							CResolveEvent("[Failure][No effect] Try as they might, no one in your squad is able to successfully diagnose the problem. The man thanks them for their help anyway.")
						end
					else
						CResolveEvent("[No effect] Either you don't have the time or the expertise to help him out. Hopefully he'll get back up and running on his own.")
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
				BodyText = string.format("An opportunity has arisen to infiltrate a facility in Meduna. As a covert ops specialist, %s volunteers for the job. It sounds like there are a number of potential targets, but the number is likely to slim as time passes.", covopsName),
				TopButtonText = string.format("Send %s.", covopsName),
				BottomButtonText = "Decline.",
				Resolution = function(topButton)
					if topButton then
						CSendMercOnMiniEvent(covopsId, 65535)
						CScreenMsg(string.format("%s begins moving towards Meduna.", covopsName))
						CResolveEvent(string.format("[%s: UNAVAILABLE] %s begins preparations to move towards Meduna and should be ready to begin in a couple days.", covopsName, covopsName), 4, 48)
					else
						CResolveEvent("[No effect] You can't spare your covert ops operative at the moment. You pass on the opportunity.")
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
					BodyText = "The rebels conclude their actions without the merc you originally sent. Where and when they disappeared to is a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the rebels suffered more losses than expected, but managed to accomplish most of what they set out to do.")
					end
				}
			end
			
			return {
				BodyText = string.format("The rebels have meticulously planned simultaneous strikes and raids against several targets of opportunity. Over the past few days, %s was trained for a specific role to help out, which was to...", selectedName),
				TopButtonText = "... oversee and coordinate the teams over radio.",
				BottomButtonText = "... lead one of the strike teams.",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 10, 1, 1)
					CSendMercOnMiniEvent(selectedProfileId, 1)
					CAddIntel(100)

					if topButton then
						CAddSkill(selectedProfileId, SKILLS.RADIO_OPERATOR)
						CAdjustStat(selectedProfileId, STATS.WISDOM, 2)
						CScreenMsg(string.format("%s learned a new skill: Radio Operator", selectedName))
						CResolveEvent(string.format("[%s: learned Radio Operator skill, +WIS][+Intel] %s reports back in from a rebel camp, and will be ready for assignment again in an hour or so.", selectedName, selectedName))
					else
						CAddSkill(selectedProfileId, SKILLS.DEPUTY)
						CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 2)
						CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 5)
						CScreenMsg(string.format("%s learned a new skill: Deputy", selectedName))
						CResolveEvent(string.format("[%s: learned Deputy skill, +MRK, +LDR][+Intel] %s reports back in from a rebel camp, and will be ready for assignment again in an hour or so.", selectedName, selectedName))
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
					BodyText = "You've decided not to follow up on that mysterious message.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] Hard to say whether it was the right decision, but better safe than sorry.")
					end
				}
			end

			return {
				BodyText = string.format("Somewhere between San Mona's many bars, a civilian catches %s's attention. They're being waved towards the outskirts of town. Following him at a distance, %s realizes that they're being led out of town. A bit of unease is settling in.", selectedName, selectedName),
				TopButtonText = "Follow the civilian.",
				BottomButtonText = "Stop following and bail out.",
				Resolution = function(topButton)
					if topButton then
						if count == 1 then
							-- chance to be ambushed if solo
							if math.random(100) < 50 then
								CApplyDamage(selectedProfileId, math.random(15,30))
								CResolveEvent(string.format("It's an ambush! Fortunately for %s, one of the waiting gunmen is too trigger-happy and opens fire too early. It's a scramble back to town, and %s is lucky to escape alive.", selectedName, selectedName))
							else
								CAddMoneyToPlayerAccount(10000)
								CScreenMsg("Gained $10000 from Kingpin's coffers.")
								CResolveEvent(string.format("[+$10000] \"You came alone? Bold. We were able to... liberate some funds from Kingpin's coffers. You will be able to put it to better use than us. Good luck.\" %s transfers the money and returns to San Mona.", selectedName))
							end
						else
							CAddMoneyToPlayerAccount(10000)
							CScreenMsg("Gained $10000 from Kingpin's coffers.")
							CResolveEvent("[+$10000] \"We were able to... liberate some funds from Kingpin's coffers. You will be able to put it to better use than us. Good luck.\" The money is transferred to your account and the team returns to San Mona.")
						end
					else
						CResolveEvent("It was too risky. Your team heads back to the relative safety of San Mona. Better safe than sorry.")
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
					BodyText = "It looks like your team can't afford to take a break right now. Another time, perhaps.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] A few of your team voice disappointment, but there's a war to win.")
					end
				}
			end

			mercs = GetLargestMercGroup(mercsInTown)
			randomName, randomId = GetRandomMerc(mercs)
			x, y, z = CGetCoordinates(randomId)

			h2h, h2hName, h2hId = CCheckForSkill(SKILLS.HAND_TO_HAND, x, y, z)

			return {
				BodyText = string.format("%s's group is unwinding in a pub when a giant of a man crashes into their table, scattering their drinks everywhere. Towering over him is an even larger man, and they're both insulting each other and slurring heavily. The first man picks himself up, roars loudly, and charges back into his adversary, bowling over even more tables. The pub erupts into a brawl. No one wants their drinks spilled, after all.", randomName),
				TopButtonText = "Join the pub brawl!",
				BottomButtonText = "Try to calm everyone down and stop fighting.",
				Resolution = function(topButton)
					if topButton then
						for name, id in pairs(mercsInTown) do
							CAdjustMorale(id, 50)
						end
						
						if h2h == true then
							for name, id in pairs(mercs) do
							CAdjustStat(id, STATS.STRENGTH, 1)
							end
							CResolveEvent(string.format("[All: +Morale, +STR] It's %s's time to shine, and everyone follows their lead. Chairs are thrown, tables are broken, and nothing and no one are left unsmashed. It is a bar brawl that can truly be described as \"epic.\" The carnage is extreme.", h2hName))
						else
							CAddMoneyToPlayerAccount(-15000, true)
							CScreenMsg("Paid $15000 for pub brawl damages.")
							CResolveEvent("[-$15000][All: +Morale] There's nothing quite like a drunk pub brawl to lift spirits. It turns out to be a fantastic way for your team to unwind. Unfortunately, the owner doesn't see it that way, and asks for compensation for damages.")
						end
					else
						townId = CGetTownId(x, y)
						townName = GetTownName(townId)

						if h2h == true then
							CAddMoneyToPlayerAccount(-15000, true)
							CScreenMsg("Paid $15000 for pub brawl damages.")
							CAddTownLoyalty(townId, -4000)
							CResolveEvent(string.format("[HAND TO HAND][-$15000][%s: -Loyalty] Your squad begins trying to cool heads, but %s has other ideas, immediately laying out the two men. %s flies into a rage, and there's not much that can be done aside from paying for the resulting damages.", townName, h2hName, h2hName))
						else
							for name, id in pairs(mercsInTown) do
								CAdjustMorale(id, 15)
							end

							CAddTownLoyalty(townId, 4000)
							CResolveEvent(string.format("[%s: +Loyalty][All: +Morale] After restraining the two men and offering to replace the drinks of the most belligerent patrons, the place calms down a bit. The owner thanks you for your efforts and reimburses you for the drinks.", townName))
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
					BodyText = "Your volunteer has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
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
				Text = "Pose as a new recruit.",
				ResolveText = string.format("%s will go through some of the training that the Arulcan army offers. At the very least, it'll be eye-opening to see how the enemy trains, and maybe something can be learned too.", selectedName),
				EventId = 5
				}
			targetArmy = {
				Text = "Target military facilities.",
				ResolveText = string.format("There are plenty of potential targets in military facilities. %s will try to infiltrate one.", selectedName),
				EventId = 6
			}
			targetGovernment = {
				Text = "Target government facilities.",
				ResolveText = string.format("There is likely value in infiltrating government buildings. %s will scope them out for potential targets and information.", selectedName),
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
				BodyText = string.format("There are a couple promising leads to follow, but it's likely that %s will only have time for one. What should be pursued?", selectedName),
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			basicTraining = {
				Text = "Go through physical training.",
				ResolveText = string.format("%s joins the bulk of the recruits in basic physical training. It'll be a good opportunity to see how the army trains its basic soldier, and %s may benefit personally in the process.", selectedName, selectedName),
				EventId = 8
			}
			specialistTraining = {
				Text = "Volunteer for specialist training.",
				ResolveText = string.format("%s opts to see how the Arulcan army chooses and trains its specialists. These are the combat medics, radio operators, and officers that you've encountered before. With any luck, %s will be able to learn something for themselves as well.", selectedName, selectedName),
				EventId = 9
			}
			weaponsTraining = {
				Text = "Get general weapons training.",
				ResolveText = string.format("%s decides that weapon familiarity is a skillset lacking in their books. In any case, they'll have an inside look at how the Arulcan army gives weapons training to its soldiers.", selectedName),
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
				BodyText = string.format("The army apparently has lax recruitment standards, and %s is able to enrol without suspicion. The other recruits are mostly loyalists, with some international mercenaries and military personnel in charge of training and management. There are a couple promising avenues to follow here.", selectedName),
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			stealDocuments = {
				Text = "Try to steal documents.",
				ResolveText = string.format("%s will try to leak as many documents as possible to you. There is no specific target in mind - anything and everything should be useful.", selectedName),
				EventId = 11
			}
			poisonGarrison = {
				Text = "Try to poison the garrison.",
				ResolveText = string.format("Meduna's garrison is formidable, but every soldier needs food and water. %s will look for a vulnerability in that area to try to weaken the city's defenses.", selectedName),
				EventId = 12
			}
			sabotageVehicles = {
				Text = "Try to sabotage military vehicles.",
				ResolveText = string.format("While you're thankful that you haven't had to face the bulk of the Queen's armour, you realise that you'll have to do it eventually. You direct %s to try to weaken or disable them before your other mercs assault the city.", selectedName),
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
				BodyText = string.format("After staking out multiple targets, %s has narrowed things down to two vulnerabilities. In the meantime, they are able to give you some temporary intel on enemy positions.", selectedName),
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			siphonFunds = {
				Text = "Siphon funds.",
				ResolveText = string.format("%s will look for ways to redirect some of the country's income to your account instead of the Queen's coffers.", selectedName),
				EventId = 14
			}
			stealDocuments = {
				Text = "Steal documents.",
				ResolveText = string.format("Arulco is an unbelievably corrupt nation under the Queen's rule. %s will gather documents that will be useful to international parties.", selectedName),
				EventId = 15
			}
			stealArtifacts = {
				Text = "Liberate artifacts.",
				ResolveText = string.format("The Queen has amassed several culturally significant artifacts from around the country. Stealing these to return to the general public will likely have a very positive effect on how the population views your presence.", selectedName),
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
				BodyText = string.format("The day-to-day operations of Arulco are largely ignored by the Queen unless it affects her grip on power. To that end, %s should have an easier time operating in non-military buildings. There are a couple of opportunities here. What should be pursued?", selectedName),
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("Many of the other recruits are somewhat slow to pick things up, but %s has to constantly act at a lower level of competence than they actually are. It's not hard to act as such, considering how physically taxing this training is. That said, it's been a week of hard work. In that time, %s...", selectedName, selectedName),
				TopButtonText = "... focussed on general physical fitness.",
				BottomButtonText = "... tried to pick up a new skill.",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
					CAddIntel(50)
					if topButton then
						CAdjustStat(selectedProfileId, STATS.LIFE, 5)
						CAdjustStat(selectedProfileId, STATS.STRENGTH, 5)
						CAdjustStat(selectedProfileId, STATS.AGILITY, 5)
						CAdjustStat(selectedProfileId, STATS.DEXTERITY, 5)
						CResolveEvent(string.format("[%s: +HP, +STR, +AGI, +DEX][+Intel] Having a full week to focus on personal improvement, %s wastes no time. They emerge from the training in the best shape they've ever been in. %s will look for an opportunity to desert and will report back in shortly with what they've learned.", selectedName, selectedName, selectedName))
					else
						pool = {
							{
								SkillId = SKILLS.ATHLETICS, 
								SkillName = "Athletics"
							},
							{
								SkillId = SKILLS.BODYBUILDING,
								SkillName = "Bodybuilding"
							},
							{
								SkillId = SKILLS.STEALTHY,
								SkillName = "Stealthy"
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
							CResolveEvent(string.format("[%s: learned %s skill][+Intel] A week of dedicated training is just enough to improve oneself for the better, and %s is no exception. A couple new tricks should make them more effective at their role. All that's left is slipping out and reporting back in.", selectedName, learned, selectedName))
						else
							CAdjustStat(selectedProfileId, STATS.LIFE, 5)
							CAdjustStat(selectedProfileId, STATS.STRENGTH, 5)
							CAdjustStat(selectedProfileId, STATS.AGILITY, 5)
							CAdjustStat(selectedProfileId, STATS.DEXTERITY, 5)
							CResolveEvent(string.format("[%s: +HP, +STR, +AGI, +DEX][+Intel] Having a full week to focus on personal improvement, %s wastes no time. They emerge from the training in the best shape they've ever been in. %s will look for an opportunity to desert and will report back in shortly with what they've learned.", selectedName, selectedName, selectedName))
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			pool = {
				{
					SkillId = SKILLS.PARAMEDIC, 
					SkillName = "Paramedic",
					SkillText = "Combat medic training."
				},
				{
					SkillId = SKILLS.TECHNICIAN,
					SkillName = "Technician",
					SkillText = "Combat engineer training."
				},
				{
					SkillId = SKILLS.DEMOLITIONS,
					SkillName = "Demolitions",
					SkillText = "Explosives training."
				},
				{
					SkillId = SKILLS.RADIO_OPERATOR,
					SkillName = "Radio Operator",
					SkillText = "Communications training."
				},
				{
					SkillId = SKILLS.SCOUTING,
					SkillName = "Scouting",
					SkillText = "Reconnaissance training."
				},
				{
					SkillId = SKILLS.DEPUTY,
					SkillName = "Deputy",
					SkillText = "Leadership training."
				},
				{
					SkillId = SKILLS.NIGHT_OPS,
					SkillName = "Night Ops",
					SkillText = "Night operations training."
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
				secondOption = { SkillId = -1, SkillText = "General non-combat training."}
			else
				firstOption = { SkillId = -1, SkillText = "General non-combat training."}
				secondOption = { SkillId = -2, SkillText = "General weapons training."}
			end

			return {
				BodyText = string.format("Surprisingly, there is a wide variety of specialist training available to pursue. There is also a large amount of high-quality equipment available for training, an indication of where national spending goes. A couple of specialist courses pique %s's interest.", selectedName),
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
						CResolveEvent(string.format("[%s: learned %s skill][+Intel] Whether %s is a quick learner or the quality of the instructors is high, the training pays off quickly, and in spades. %s is able to learn much about the army's specialist training methods, and has picked up something for theirself as well. They'll make their way to San Mona shortly.", selectedName, choice.SkillName, selectedName, selectedName))
					elseif choice.SkillId == -1 then
						CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 20)
						CAdjustStat(selectedProfileId, STATS.MECHANICAL, 10)
						CAdjustStat(selectedProfileId, STATS.MEDICAL, 10)
						CAdjustStat(selectedProfileId, STATS.EXPLOSIVE, 10)
						CResolveEvent(string.format("[%s: +LDR, +MEC, +MED, +EXP][+Intel] Whether %s is a quick learner or the quality of the instructors is high, the training pays off quickly, and in spades. %s is able to learn much about the army's specialist training methods, and has picked up something for theirself as well. They'll make their way to San Mona shortly.", selectedName, selectedName, selectedName))
					else -- SkillId == -2
						CAdjustStat(selectedProfileId, STATS.AGILITY, 2)
						CAdjustStat(selectedProfileId, STATS.DEXTERITY, 6)
						CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 10)
						CResolveEvent(string.format("[%s: +AGI, +DEX, +MRK][+Intel] Whether %s is a quick learner or the quality of the instructors is high, the training pays off quickly, and in spades. %s is able to learn much about the army's specialist training methods, and has picked up something for theirself as well. They'll make their way to San Mona shortly.", selectedName, selectedName, selectedName))
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("The weapons training is shockingly fast and effective. The overseas instructors appear to be worth their weight in gold. There's always someone knowledgeable about any given weapon, its most effective uses, and so on. Whether the recruits pick up on that is another question. %s wasted no time and...", selectedName),
				TopButtonText = "... focussed on general weapons training.",
				BottomButtonText = "... tried to pick up a new skill.",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
					CAddIntel(50)
					if topButton then
						CAdjustStat(selectedProfileId, STATS.AGILITY, 2)
						CAdjustStat(selectedProfileId, STATS.DEXTERITY, 6)
						CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 10)
						CResolveEvent(string.format("[%s: +AGI, +DEX, +MRK][+Intel] Having a full week to focus on personal improvement, %s wastes no time. They emerge from the training a much better shot than before. %s will look for an opportunity to desert and will report back in shortly with what they've learned.", selectedName, selectedName, selectedName))
					else
						pool = {
							{
								SkillId = SKILLS.MARKSMAN, 
								SkillName = "Marksman"
							},
							{
								SkillId = SKILLS.HEAVY_WEAPONS,
								SkillName = "Heavy Weapons"
							},
							{
								SkillId = SKILLS.AUTO_WEAPONS,
								SkillName = "Auto Weapons"
							},
							{
								SkillId = SKILLS.GUNSLINGER,
								SkillName = "Gunslinger"
							},
							{
								SkillId = SKILLS.HUNTER,
								SkillName = "Hunter"
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
							CResolveEvent(string.format("[%s: learned %s skill][+Intel] A week of dedicated training is just enough to improve oneself for the better, and %s is no exception to this. They spent the entire week to specialise in a single weapon family and are now more than familiar with their ins and outs. All that's left is slipping out of training camp and reporting back in.", selectedName, learned, selectedName))
						else
							CAdjustStat(selectedProfileId, STATS.AGILITY, 2)
							CAdjustStat(selectedProfileId, STATS.DEXTERITY, 6)
							CAdjustStat(selectedProfileId, STATS.MARKSMANSHIP, 10)
						CResolveEvent(string.format("[%s: +AGI, +DEX, +MRK][+Intel] Having a full week to focus on personal improvement, %s wastes no time. They emerge from the training a much better shot than before. %s will look for an opportunity to desert and will report back in shortly with what they've learned.", selectedName, selectedName, selectedName))
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("Over the last week, %s has compiled a sizeable number of documents and is preparing to get them to you. However, it's time to escape. %s has overstayed their welcome and a growing number of people are suspicious. There are a couple potential avenues to get out safely.", selectedName, selectedName),
				TopButtonText = "Escape on foot at night.",
				BottomButtonText = "Escape on a vehicle.",
				Resolution = function(topButton)
					if topButton then
						CAddIntel(250)
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(36,72))
						CResolveEvent(string.format("[+Intel] There's no way that the enemy can completely surround the city, and %s slips out under cover of darkness. It's time-consuming to flee on foot, but %s will be able to rejoin you in a couple days' time.", selectedName, selectedName))
					else
						CAddIntel(125)
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(12,24))
						CResolveEvent(string.format("[+Intel] With the knowledge gained over the last week, %s has little trouble gaining access to the vehicle depot. However, as %s was forced to act swiftly to escape, they were not able to extract as much intel as they'd have liked. Nevertheless, they should be ready for reassignment soon.", selectedName, selectedName))
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("Having spent the last week preparing the poison, an opportunity has arisen to poison supplies destined for troops deployed stationed outside of Meduna. %s can complete their mission and then extract to San Mona, but first they need to confirm their target. The poison will likely have a more concentrated effect in Meduna, but weakening troops nationwide may be more beneficial at the moment.", selectedName, selectedName),
				TopButtonText = "Poison Meduna's garrison.",
				BottomButtonText = "Poison outgoing supplies.",
				Resolution = function(topButton)
					if topButton then
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
						for i=1,16 do
							for j=1,16 do
								townId = CGetTownId(i,j)
								if townId == TOWNS.MEDUNA then
									CAdjustEnemyStrengthInSector(i, j, -math.random(5,8), -math.random(5,8), -math.random(5,8), 0, 0, 0)
								end
							end
						end
						CResolveEvent(string.format("[MEDUNA: garrison reduced] %s concentrates the poison in Meduna's supply stores. It should have a noticeable effect before the affected supplies can be condemned. With the poison administered, %s heads for San Mona.", selectedName, selectedName))
					else
						CSetMercCoordinates(selectedProfileId, 5, 3, 0)
						CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
						for i=1,16 do
							for j=1,16 do
								townId = CGetTownId(i,j)
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
						CResolveEvent(string.format("[ALL TOWNS EXCEPT MEDUNA: garrison reduced] %s distributes their poison into supplies bound for the various town garrisons across the country. While the effect won't be as drastic as poisoning a single garrison, a slightly weaker enemy force is still something easier to deal with. With the job done, %s begins their exfiltration to San Mona.", selectedName, selectedName))
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("Meduna's armour division undergoes regular maintenance cycles, and %s would be able to exploit that to cripple at least some of the city's defenses. It may also be possible to go for a more destructive route and sabotage both the vehicles and the facility, but it would be unwise to attempt this would solid knowledge of explosives. %s should...", selectedName, selectedName),
				TopButtonText = "... sabotage the vehicles.",
				BottomButtonText = "... sabotage the vehicles explosively.",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))

					if topButton then
						for i=1,16 do
							for j=1,16 do
								townId = CGetTownId(i,j)
								if townId == TOWNS.MEDUNA then
									CAdjustEnemyStrengthInSector(i, j, 0, 0, 0, 0, 0, -1)
								end
							end
						end
						CAdjustStat(selectedProfileId, STATS.MECHANICAL, 5)
						CResolveEvent(string.format("[Success][%s: +MEC] %s is able to gain entry to the army's maintenance facilities. From there, security is surprisingly lax and it's not hard to sabotage the tanks that are in for maintenance. Less effective than destroying them outright, perhaps, but definitely safer. With the job done, %s will make their way out of Meduna and back to San Mona.", selectedName, selectedName, selectedName))
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
							message = string.format("[Failure] %s simply lacks the necessary knowledge to cause an explosion effective enough to disable tanks, let alone the facility. As there isn't enough time to act on another plan, %s will flee to San Mona.", selectedName, selectedName)
						elseif value < 80 then
							admin, troop, elite = -2, -3, -2
							jeep, tank = -2, -math.random(1,3)
							message = string.format("[Success][MEDUNA: armour division reduced] Carefully placed mini explosives go off across the tanks deployed to defend Meduna. While they aren't powerful enough to completely destroy the vehicles, enough are knocked out to weaken the city's defenses. %s is able to sneak out while the army tries to figure out what happened.", selectedName)
							CSetEnemyGroupVisibility(true)
						elseif value < 110 then
							admin, troop, elite = -20, -math.random(5,10), -math.random(3,7)
							robot, jeep, tank = -math.random(1,4), -2, -2
							message = string.format("[Success][MEDUNA: garrison reduced] Explosions rock the city as synchronised detonations go off in each of the defending tanks. Most are reduced to scrap, and some of the attached infantry are also caught in the blasts. Meduna's defenses have suffered a blow, and %s uses the chaos to flee to San Mona.", selectedName)
							CSetEnemyGroupVisibility(true)
						else
							admin, troop, elite = -20, -math.random(10,15), -math.random(10,15)
							robot, jeep, tank = -math.random(1,4), -10, -10
							message = string.format("[Success][MEDUNA: garrison severely reduced] A catastrophic explosion ripples through Meduna, originating from the army's maintenance facilities. Secondary explosions follow as deployed tanks go up in flames, their ammo reserved rigged to blow. Meduna's defenses are severely reduced, but there's no time for %s to revel in the destruction - they flee the city in the ensuing chaos.", selectedName)
							CSetEnemyGroupVisibility(true)
						end

						for i=1,16 do
							for j=1,16 do
								townId = CGetTownId(i,j)
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("Perhaps unsurprisingly, %s doesn't find very much resistance when learning about how the country handles its funds. The largest area of spending is the military, and a meager amount is sent to keep the mines operable. %s can either do a quick digital smash-and-grab, or spend a few more days to nab a significantly larger amount.", selectedName, selectedName),
				TopButtonText = "Deposit what you can and get out.",
				BottomButtonText = "Spend a few days for a big haul.",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)

					if topButton then
						CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))
						CAddMoneyToPlayerAccount(75000)
						CScreenMsg("Stole $75000 from the Queen.")
						CResolveEvent(string.format("[+$75000] %s simply redirects a small portion of the country's reserves to an overseas bank account, which is then laundered and placed into your warchest. With the task complete, %s will look for an exit and head to San Mona.", selectedName, selectedName))
					else
						CSendMercOnMiniEvent(selectedProfileId, math.random(120,144))
						CAddMoneyToPlayerAccount(500000)
						CScreenMsg("Stole $500000 from the Queen.")
						CResolveEvent(string.format("[+$500000] %s is able to deposit a huge amount of cash into your account. %s will need to spend a few days covering their tracks and silencing otherwise mouthy people. Once that's done, they'll report back in from San Mona.", selectedName, selectedName))
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("As it turns out, there is a wealth of information to be had. %s was able to gather this over the last week, and is ready to extract. However, some of the intel that was recovered could be used to sway some soldiers to your side. Doing so would render some intel no longer accurate or relevant. How should %s proceed?", selectedName, selectedName),
				TopButtonText = "Extract with the intel.",
				BottomButtonText = "Try to convince some soldiers to join you.",
				Resolution = function(topButton)
					CSetMercCoordinates(selectedProfileId, 5, 3, 0)
					CSendMercOnMiniEvent(selectedProfileId, math.random(24,48))

					if topButton then
						CAddIntel(500)
						CResolveEvent(string.format("[+Intel] %s finalises their escape plan and transmits the intel to you. It's a sizeable amount, and should prove useful. %s will extract to San Mona and will be ready for reassignment in about a day.", selectedName, selectedName))
					else
						CAddIntel(250)
						leadership, _, __ = CGetStat(STATS.LEADERSHIP, selectedProfileId)
						
						message = ""
						if leadership < 25 then
							message = string.format("[Failure][+Intel] %s lacks the charisma and leadership to convince anyone to join your side. With some intel now useless and their cover blown, %s is forced to flee Meduna to safe ground - San Mona.", selectedName, selectedName)
						elseif leadership < 70 then
							CCreateMilitia(0, 10, 10, 5, 3)
							CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 3)
							message = string.format("[Success][+Intel][%s: +LDR] Through a mix of blackmail, charm, and wits, %s was able to recruit a good chunk of soldiers to defect to the rebels' cause. They are ready and waiting for orders in San Mona. %s will catch up to them in a day or so to clean up loose ends and deliver the rest of the intel.", selectedName, selectedName, selectedName)
						else
							CCreateMilitia(0, 0, 25, 5, 3)
							CAdjustStat(selectedProfileId, STATS.LEADERSHIP, 3)
							message = string.format("[Success][+Intel][%s: +LDR] A large squad of the Queen's elites are swayed by %s to abandon her and join the rebel cause. They take their best equipment and are ready for orders in San Mona. %s will catch up to them in a day or so to clean up loose ends and deliver the rest of the intel.", selectedName, selectedName, selectedName)
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
					BodyText = "Your covert ops operative has disappeared. Where and when remains a mystery.",
					TopButtonText = "OK",
					BottomButtonText = "OK",
					Resolution = function(topButton)
						CResolveEvent("[No effect] With your nominated merc gone, the infiltration window closes.")
					end
				}
			end

			return {
				BodyText = string.format("A large number of Arulcan cultural artifacts are part of the Queen's personal collection. The populace would be ecstatic to see even a small number of them returned to their original places. Alternatively, there are also a decent number of international collectors looking to obtain these relics for a handsome price.", selectedName, selectedName),
				TopButtonText = "Return the artifacts to the locals.",
				BottomButtonText = "Sell to private buyers. The public will never know.",
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
						CResolveEvent(string.format("[ALL TOWNS: +Loyalty][%s: +WIS] %s is able to smuggle a few small relics out of the palace without anyone noticing. They'll need to spend a day or so to organise their return, and then will be ready for reassignment from San Mona.", selectedName, selectedName))
					else
						CAddMoneyToPlayerAccount(100000)
						CScreenMsg("Gained $100000 by smuggling Arulcan artifacts.")
						CResolveEvent(string.format("[+$100000] %s is able to smuggle a few small relics out of the palace without anyone noticing. It'll take about a day to organise international export, but %s immediately has buyers for the goods. With the task completed, they will make their way out of Meduna to San Mona for reassignment.", selectedName, selectedName))
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
