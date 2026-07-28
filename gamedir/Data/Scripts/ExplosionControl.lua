Profil = {
	ELDIN = 127,
	BIGGENS = 61, -- UB
	
}

Team = {
	OUR_TEAM = 0,
	ENEMY_TEAM = 1,
	CREATURE_TEAM = 2,
	MILITIA_TEAM = 3,	
	CIV_TEAM = 4,	
	PLAYER_PLAN	= 5,
	LAN_TEAM_ONE = 6,
	LAN_TEAM_TWO = 7,
	LAN_TEAM_THREE = 8,
	LAN_TEAM_FOUR = 9,
}

StatusAlert = {
	STATUS_GREEN = 0,
	STATUS_YELLOW = 1,		
	STATUS_RED = 2,	
	STATUS_BLACK = 3,		
}

ActionType = {
	AI_ACTION_GET_CLOSER = 5,
}

Facts = {
	FACT_MUSEUM_ALARM_WENT_OFF = 278,
}

SoundVolume = {
	LOWVOLUME	=	25,
	BTNVOLUME	=	40,
	MIDVOLUME	=	65,
	HIGHVOLUME	=	127,
}

Sample = {
	KLAXON_ALARM = 214,
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

Civ_status = {
	CIV_GROUP_NEUTRAL = 0,
	CIV_GROUP_WILL_EVENTUALLY_BECOME_HOSTILE = 1,
	CIV_GROUP_WILL_BECOME_HOSTILE = 2,
	CIV_GROUP_HOSTILE = 3,
}

Noise = {
	NOISE_UNKNOWN = 0,
	NOISE_MOVEMENT = 1,
	NOISE_CREAKING = 2,
	NOISE_SPLASHING = 3,
	NOISE_BULLET_IMPACT = 4,
	NOISE_GUNFIRE = 5,
	NOISE_EXPLOSION = 6,
	NOISE_SCREAM = 7,
	NOISE_ROCK_IMPACT = 8,
	NOISE_GRENADE_IMPACT = 9,
	NOISE_WINDOW_SMASHING = 10,
	NOISE_DOOR_SMASHING = 11,
	NOISE_SILENT_ALARM = 12, 
}

local HEARD_3_TURNS_AGO	= -4
local HEARD_2_TURNS_AGO	= -3
local HEARD_LAST_TURN	=  -2
local HEARD_THIS_TURN	= -1
local NOT_HEARD_OR_SEEN	= 0
local SEEN_CURRENTLY	= 1
local SEEN_THIS_TURN	= 2
local SEEN_LAST_TURN	= 3
local SEEN_2_TURNS_AGO	=4
local SEEN_3_TURNS_AGO = 5

local ubProfil = 0
local MAX_MISC_NOISE_DURATION = 12
local iLoop
local NOBODY = 254
local LAST_TEAM = Team.LAN_TEAM_FOUR
local bTeam

function PerformItemAction( ActionID , sGridNo )

local ubID

-- Open Door
if ActionID == 1 then
	ACTION_ITEM_OPEN_DOOR (sGridNo)
end

-- Open Close
if ActionID == 2 then
	ACTION_ITEM_CLOSE_DOOR (sGridNo)
end

-- 3x3 Hidden Pit
if ActionID == 4 then
	Add3X3Pit (sGridNo)
	SearchForOtherMembersWithinPitRadiusAndMakeThemFall ( sGridNo, 1 )
end

-- 5x5 Hidden Pit
if ActionID == 5 then
	Add5X5Pit (sGridNo)
	SearchForOtherMembersWithinPitRadiusAndMakeThemFall ( sGridNo, 2 )
end

-- Toggle Door
if ActionID == 6 then
	ACTION_ITEM_TOGGLE_DOOR (sGridNo)
end

-- Toggle Action1s
if ActionID == 7 then
	ToggleActionItemsByFrequency( 1 )
end

-- Toggle Action2s
if ActionID == 8 then
	ToggleActionItemsByFrequency ( 2 )
end

-- Toggle Action3s
if ActionID == 9 then
	ToggleActionItemsByFrequency( 3 )
end

-- Toggle Action4s
if ActionID == 10 then
	ToggleActionItemsByFrequency( 4 )
end

-- Enter Brothel
if ActionID == 11 then
	-- Empty Action
end

-- Exit Brothel
if ActionID == 12 then
	-- Empty Action
end

-- Kingpin Alarm
if ActionID == 13 then

	PlayJA2Sample( Sample.KLAXON_ALARM, SoundVolume.MIDVOLUME, sGridNo )
	
	if ( TacticalStatusTeamActive(Team.CIV_TEAM) == true ) then
	
		SetPublicNoiseGridNo( Team.CIV_TEAM, sGridNo )
		SetPublicNoiseVolume( Team.CIV_TEAM, MAX_MISC_NOISE_DURATION )
	
		for iLoop = GetTacticalStatusFirstID(Team.CIV_TEAM),GetTacticalStatusLastID(Team.CIV_TEAM) do
			if ( CheckMercPtrsInSector (iLoop) == true and CheckMercPtrsInActive(iLoop) == true and CheckMercPtrsInCivilianGroup (iLoop) == Group.KINGPIN_CIV_GROUP ) then
				  SetNewSituationMercPtrs ( iLoop )
			end
		end
	end	
		
		SetCivGroupHostile( Group.KINGPIN_CIV_GROUP , Civ_status.CIV_GROUP_HOSTILE )
		
		for iLoop = GetTacticalStatusFirstID(Team.CIV_TEAM),GetTacticalStatusLastID(Team.CIV_TEAM) do
			if ( CheckMercPtrsInSector (iLoop) == true and CheckMercPtrsInActive(iLoop) == true and CheckMercPtrsInCivilianGroup (iLoop) == Group.KINGPIN_CIV_GROUP ) then
				for aimLoop=GetTacticalStatusFirstID(Team.OUR_TEAM),GetTacticalStatusLastID(Team.OUR_TEAM) do
					if ( CheckMercPtrsID1SeenID2(iLoop,aimLoop) == SEEN_CURRENTLY ) then
						MakeMercPtrsHostile( iLoop )
					end
				end	
			end	
		end
 
	if ( CheckCombatMode == false ) then
		EnterTeamCombatMode(Team.CIV_TEAM)
	end	
	
end

-- Sex with Prostitute
if ActionID == 14 then
	ACTION_ITEM_SEX (sGridNo)
end

-- Reveal Room
if ActionID == 15 then
	ACTION_ITEM_REVEAL_ROOM (sGridNo)
end

-- Local Alarm
if ActionID == 16 then
	MakeNoise( NOBODY, sGridNo, 0, WorldLevelDataTerrainID(sGridNo), 30, Noise.NOISE_SILENT_ALARM )
end

-- Global Alarm
if ActionID == 17 then

	for i = 0 , LAST_TEAM do
		if ( TacticalStatusTeamActive(i) == true ) then
			if ( TacticalStatusTeamHuman(i) == false and ( not i == Team.CIV_TEAM ) ) then
				
				SetPublicNoiseGridNo( i, sGridNo )
				SetPublicNoiseVolume( i, MAX_MISC_NOISE_DURATION )
				
				iLoop = GetTacticalStatusFirstID(i)
				for o = iLoop,GetTacticalStatusLastID(iLoop) do
					if ( CheckMercPtrsInSector (o) == true and CheckMercPtrsInActive(o) == true ) then
						SetNewSituationMercPtrs( o )
						WearGasMaskIfAvailable( o )			
					end
				end
			end
		end
	end
	
end

-- Klaxon Sound
if ActionID == 18 then
	PlayJA2Sample( Sample.KLAXON_ALARM, SoundVolume.MIDVOLUME, sGridNo )
end

-- Unlock door
if ActionID == 19 then
	ACTION_ITEM_UNLOCK_DOOR (sGridNo)
end

-- Toggle lock
if ActionID == 20 then
	ACTION_ITEM_TOGGLE_LOCK (sGridNo)
end

-- Untrap door
if ActionID == 21 then
	ACTION_ITEM_UNTRAP_DOOR (sGridNo)
end

-- Tog pressure items
if ActionID == 22 then
	TogglePressureActionItemsInGridNo (sGridNo)
end

-- Museum alarm
if ActionID == 23 then

	ubProfil = Profil.ELDIN
	
	PlayJA2Sample( Sample.KLAXON_ALARM, SoundVolume.MIDVOLUME, sGridNo )
	
	-- Eldin becomes aware of a very important "noise" coming from here!
	-- So long as he hasn't already heard a noise a sec ago...
	if ( TacticalStatusTeamActive(Team.CIV_TEAM) == true ) then
		-- new situation for Eldin
		if ( CheckSoldierInSector(ubProfil) == true and CheckSoldierActive(ubProfil) == true and CheckNPCLife(ubProfil) > 0 and ( CheckSoldierAlertStatus(ubProfil) == StatusAlert.STATUS_GREEN and CheckSoldierNoiseVolume(ubProfil) ==  6  ) ) then
				if ( SoldierTo3DLocationLineOfSightTest( ubProfil, sGridNo, 0, 0 ) == true ) then
					
					-- sees the player now!
					TriggerNPCWithIHateYouQuote(ubProfil)
					SetNewSituation(ubProfil)
				else
					SetNoiseGridno(ubProfil,sGridNo)
					SetNoiseVolume(ubProfil,6)
					SetAlertStatus(ubProfil,StatusAlert.STATUS_GREEN)
					if  ( not CheckAction(ubProfil) == ActionType.AI_ACTION_GET_CLOSER  ) or CheckFact (Facts.FACT_MUSEUM_ALARM_WENT_OFF, 0) == false then
						CancelAIAction( ubProfil)
						SetNextAction(ubProfil , ActionType.AI_ACTION_GET_CLOSER )
						SetNextActionData(ubProfil , sGridNo)
						RESETTIMECOUNTER( ubProfil, 100 )
					end
				end
			SetFactTrue( Facts.FACT_MUSEUM_ALARM_WENT_OFF )
		end
	end
	
	
end

-- Bloodcat alarm
if ActionID == 24 then
	bTeam = Team.CREATURE_TEAM

		if ( TacticalStatusTeamActive(bTeam) == true ) then
			if ( TacticalStatusTeamHuman(bTeam) == false and ( not bTeam == Team.CIV_TEAM ) ) then
				
				SetPublicNoiseGridNo( bTeam, sGridNo )
				SetPublicNoiseVolume( bTeam, MAX_MISC_NOISE_DURATION )
				
				iLoop = GetTacticalStatusFirstID(bTeam)
				for o = iLoop,GetTacticalStatusLastID(bTeam) do
					if ( CheckMercPtrsInSector (o) == true and CheckMercPtrsInActive(o) == true ) then
						SetNewSituationMercPtrs( o )
						WearGasMaskIfAvailable( o )			
					end
				end
			end
		end
end

end
