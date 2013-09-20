math.randomseed( os.time() ); math.random(); math.random(); math.random()

local MAX_EMAIL_LINES = 10 		-- Max number of lines can be shown in a message
local MAX_MESSAGES_PAGE = 18 	-- Max number of messages per page

local IMP_EMAIL_INTRO = 0
local IMP_EMAIL_INTRO_LENGTH = 10
local ENRICO_CONGRATS = (IMP_EMAIL_INTRO + IMP_EMAIL_INTRO_LENGTH)					
local ENRICO_CONGRATS_LENGTH = 3
local IMP_EMAIL_AGAIN = (ENRICO_CONGRATS + ENRICO_CONGRATS_LENGTH)
local IMP_EMAIL_AGAIN_LENGTH = 6
local MERC_INTRO = (IMP_EMAIL_AGAIN + IMP_EMAIL_AGAIN_LENGTH)
local MERC_INTRO_LENGTH = 5
local MERC_NEW_SITE_ADDRESS = ( MERC_INTRO + MERC_INTRO_LENGTH )
local MERC_NEW_SITE_ADDRESS_LENGTH = 2
local AIM_MEDICAL_DEPOSIT_REFUND = ( MERC_NEW_SITE_ADDRESS + MERC_NEW_SITE_ADDRESS_LENGTH )
local AIM_MEDICAL_DEPOSIT_REFUND_LENGTH = 3
local IMP_EMAIL_PROFILE_RESULTS = ( AIM_MEDICAL_DEPOSIT_REFUND + AIM_MEDICAL_DEPOSIT_REFUND_LENGTH )
local IMP_EMAIL_PROFILE_RESULTS_LENGTH = 1
local MERC_WARNING = ( IMP_EMAIL_PROFILE_RESULTS_LENGTH + IMP_EMAIL_PROFILE_RESULTS )
local MERC_WARNING_LENGTH = 2
local MERC_INVALID = ( MERC_WARNING + MERC_WARNING_LENGTH )
local MERC_INVALID_LENGTH = 2							
local NEW_MERCS_AT_MERC = ( MERC_INVALID + MERC_INVALID_LENGTH )
local NEW_MERCS_AT_MERC_LENGTH = 2
local MERC_FIRST_WARNING = ( NEW_MERCS_AT_MERC + NEW_MERCS_AT_MERC_LENGTH )
local MERC_FIRST_WARNING_LENGTH = 2

-- merc up a level emails
local MERC_UP_LEVEL_BIFF = ( MERC_FIRST_WARNING + MERC_FIRST_WARNING_LENGTH )
local MERC_UP_LEVEL_LENGTH_BIFF = 2
local MERC_UP_LEVEL_HAYWIRE	 = ( MERC_UP_LEVEL_LENGTH_BIFF + MERC_UP_LEVEL_BIFF )
local MERC_UP_LEVEL_LENGTH_HAYWIRE = 2
local MERC_UP_LEVEL_GASKET = ( MERC_UP_LEVEL_LENGTH_HAYWIRE + MERC_UP_LEVEL_HAYWIRE )
local MERC_UP_LEVEL_LENGTH_GASKET = 2
local MERC_UP_LEVEL_RAZOR = ( MERC_UP_LEVEL_LENGTH_GASKET + MERC_UP_LEVEL_GASKET )
local MERC_UP_LEVEL_LENGTH_RAZOR = 2
local MERC_UP_LEVEL_FLO	 = ( MERC_UP_LEVEL_LENGTH_RAZOR + MERC_UP_LEVEL_RAZOR )
local MERC_UP_LEVEL_LENGTH_FLO = 2
local MERC_UP_LEVEL_GUMPY = ( MERC_UP_LEVEL_LENGTH_FLO + MERC_UP_LEVEL_FLO )
local MERC_UP_LEVEL_LENGTH_GUMPY = 2
local MERC_UP_LEVEL_LARRY = ( MERC_UP_LEVEL_LENGTH_GUMPY + MERC_UP_LEVEL_GUMPY )
local MERC_UP_LEVEL_LENGTH_LARRY = 2
local MERC_UP_LEVEL_COUGAR = ( MERC_UP_LEVEL_LENGTH_LARRY + MERC_UP_LEVEL_LARRY )
local MERC_UP_LEVEL_LENGTH_COUGAR = 2
local MERC_UP_LEVEL_NUMB = ( MERC_UP_LEVEL_LENGTH_COUGAR + MERC_UP_LEVEL_COUGAR )
local MERC_UP_LEVEL_LENGTH_NUMB = 2
local MERC_UP_LEVEL_BUBBA = ( MERC_UP_LEVEL_LENGTH_NUMB + MERC_UP_LEVEL_NUMB )
local MERC_UP_LEVEL_LENGTH_BUBBA = 2

-- Merc left-me-a-message-and-now-I'm-back emails
local AIM_REPLY_BARRY = ( MERC_UP_LEVEL_LENGTH_BUBBA + MERC_UP_LEVEL_BUBBA )
local AIM_REPLY_LENGTH_BARRY = 2
local AIM_REPLY_MELTDOWN = (AIM_REPLY_BARRY + ( 39 * AIM_REPLY_LENGTH_BARRY ))
local AIM_REPLY_LENGTH_MELTDOWN	 = AIM_REPLY_LENGTH_BARRY

-- Old EXISTING emails when player starts game. They must look "read"
local OLD_ENRICO_1 = ( AIM_REPLY_LENGTH_MELTDOWN + AIM_REPLY_MELTDOWN )
local OLD_ENRICO_1_LENGTH = 3
local OLD_ENRICO_2 = ( OLD_ENRICO_1 + OLD_ENRICO_1_LENGTH )
local OLD_ENRICO_2_LENGTH = 3
local RIS_REPORT = ( OLD_ENRICO_2 + OLD_ENRICO_2_LENGTH )
local RIS_REPORT_LENGTH	 = 2
local OLD_ENRICO_3 = ( RIS_REPORT + RIS_REPORT_LENGTH )
local OLD_ENRICO_3_LENGTH = 3

-- Emails that occur from Enrico once player accomplishes things
local ENRICO_MIGUEL = ( OLD_ENRICO_3 + OLD_ENRICO_3_LENGTH )
local ENRICO_MIGUEL_LENGTH = 3
local ENRICO_PROG_20 = ( ENRICO_MIGUEL + ENRICO_MIGUEL_LENGTH )
local ENRICO_PROG_20_LENGTH = 3
local ENRICO_PROG_55 = ( ENRICO_PROG_20 + ENRICO_PROG_20_LENGTH )
local ENRICO_PROG_55_LENGTH = 3
local ENRICO_PROG_80 = ( ENRICO_PROG_55 + ENRICO_PROG_55_LENGTH )
local ENRICO_PROG_80_LENGTH = 3
local ENRICO_SETBACK = ( ENRICO_PROG_80 + ENRICO_PROG_80_LENGTH )
local ENRICO_SETBACK_LENGTH = 3
local ENRICO_SETBACK_2 = ( ENRICO_SETBACK + ENRICO_SETBACK_LENGTH )
local ENRICO_SETBACK_2_LENGTH = 3
local ENRICO_CREATURES = ( ENRICO_SETBACK_2 + ENRICO_SETBACK_2_LENGTH )
local ENRICO_CREATURES_LENGTH = 3

-- Insurance company emails
local INSUR_PAYMENT = ( ENRICO_CREATURES + ENRICO_CREATURES_LENGTH )
local INSUR_PAYMENT_LENGTH = 3
local INSUR_SUSPIC = ( INSUR_PAYMENT + INSUR_PAYMENT_LENGTH )
local INSUR_SUSPIC_LENGTH = 3
local INSUR_INVEST_OVER	 = ( INSUR_SUSPIC + INSUR_SUSPIC_LENGTH )
local INSUR_INVEST_OVER_LENGTH = 3
local INSUR_SUSPIC_2 = ( INSUR_INVEST_OVER + INSUR_INVEST_OVER_LENGTH )
local INSUR_SUSPIC_2_LENGTH	 = 3

local BOBBYR_NOW_OPEN = ( INSUR_SUSPIC_2 + INSUR_SUSPIC_2_LENGTH )
local BOBBYR_NOW_OPEN_LENGTH = 3

local KING_PIN_LETTER = ( BOBBYR_NOW_OPEN + BOBBYR_NOW_OPEN_LENGTH )
local KING_PIN_LETTER_LENGTH = 4

local LACK_PLAYER_PROGRESS_1 = ( KING_PIN_LETTER + KING_PIN_LETTER_LENGTH )
local LACK_PLAYER_PROGRESS_1_LENGTH = 3

local LACK_PLAYER_PROGRESS_2 = ( LACK_PLAYER_PROGRESS_1 + LACK_PLAYER_PROGRESS_1_LENGTH )
local LACK_PLAYER_PROGRESS_2_LENGTH = 3

local LACK_PLAYER_PROGRESS_3 = ( LACK_PLAYER_PROGRESS_2 + LACK_PLAYER_PROGRESS_2_LENGTH )
local LACK_PLAYER_PROGRESS_3_LENGTH = 3

-- A package from bobby r has arrived in Drassen
local BOBBYR_SHIPMENT_ARRIVED = ( LACK_PLAYER_PROGRESS_3 + LACK_PLAYER_PROGRESS_3_LENGTH )
local BOBBYR_SHIPMENT_ARRIVED_LENGTH = 4

-- John Kulba has left the gifts for theplayers in drassen
local JOHN_KULBA_GIFT_IN_DRASSEN = ( BOBBYR_SHIPMENT_ARRIVED + BOBBYR_SHIPMENT_ARRIVED_LENGTH )
local JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH = 4

-- When a merc dies on ANOTHER assignment ( ie not with the player )
local MERC_DIED_ON_OTHER_ASSIGNMENT = ( JOHN_KULBA_GIFT_IN_DRASSEN + JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH )
local MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH = 5

local INSUR_1HOUR_FRAUD = ( MERC_DIED_ON_OTHER_ASSIGNMENT + MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH )
local INSUR_1HOUR_FRAUD_LENGTH = 3

-- When a merc is fired, and is injured
local AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND = ( INSUR_1HOUR_FRAUD + INSUR_1HOUR_FRAUD_LENGTH )
local AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND_LENGTH = 3

-- When a merc is fired, and is dead
local AIM_MEDICAL_DEPOSIT_NO_REFUND = ( AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND + AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND_LENGTH )
local AIM_MEDICAL_DEPOSIT_NO_REFUND_LENGTH = 3

local BOBBY_R_MEDUNA_SHIPMENT	 = 	( AIM_MEDICAL_DEPOSIT_NO_REFUND + AIM_MEDICAL_DEPOSIT_NO_REFUND_LENGTH )
local BOBBY_R_MEDUNA_SHIPMENT_LENGTH = 	4

History = 
{
	HISTORY_ENTERED_HISTORY_MODE = 0,
	HISTORY_HIRED_MERC_FROM_AIM = 1,
	HISTORY_HIRED_MERC_FROM_MERC = 2,
	HISTORY_MERC_KILLED = 3,
	HISTORY_SETTLED_ACCOUNTS_AT_MERC = 4,
	HISTORY_ACCEPTED_ASSIGNMENT_FROM_ENRICO = 5,
	HISTORY_CHARACTER_GENERATED = 6,
	HISTORY_PURCHASED_INSURANCE = 7,
	HISTORY_CANCELLED_INSURANCE = 8,
	HISTORY_INSURANCE_CLAIM_PAYOUT = 9,
	HISTORY_EXTENDED_CONTRACT_1_DAY = 10,
	HISTORY_EXTENDED_CONTRACT_1_WEEK = 11,
	HISTORY_EXTENDED_CONTRACT_2_WEEK = 12,
	HISTORY_MERC_FIRED = 13,
	HISTORY_MERC_QUIT = 14,
	HISTORY_QUEST_STARTED = 15,
	HISTORY_QUEST_FINISHED = 16,
	HISTORY_TALKED_TO_MINER = 17,
	HISTORY_LIBERATED_TOWN = 18, 
	HISTORY_CHEAT_ENABLED = 19,
	HISTORY_TALKED_TO_FATHER_WALKER = 20,
	HISTORY_MERC_MARRIED_OFF = 21,
	HISTORY_MERC_CONTRACT_EXPIRED = 22,
	HISTORY_RPC_JOINED_TEAM = 23,
	HISTORY_ENRICO_COMPLAINED = 24,
	HISTORY_WONBATTLE = 25,
	HISTORY_MINE_RUNNING_OUT = 26,
	HISTORY_MINE_RAN_OUT = 27,
	HISTORY_MINE_SHUTDOWN = 28,
	HISTORY_MINE_REOPENED = 29,
	HISTORY_DISCOVERED_TIXA = 30,
	HISTORY_DISCOVERED_ORTA = 31,
	HISTORY_GOT_ROCKET_RIFLES = 32,
	HISTORY_DEIDRANNA_DEAD_BODIES = 33,
	HISTORY_BOXING_MATCHES = 34,
	HISTORY_SOMETHING_IN_MINES = 35,
	HISTORY_DEVIN = 36,
	HISTORY_MIKE = 37,
	HISTORY_TONY = 38,
	HISTORY_KROTT = 39,
	HISTORY_KYLE = 40,
	HISTORY_MADLAB = 41,
	HISTORY_GABBY = 42,
	HISTORY_KEITH_OUT_OF_BUSINESS = 43,
	HISTORY_HOWARD_CYANIDE = 44,
	HISTORY_KEITH = 45,
	HISTORY_HOWARD = 46,
	HISTORY_PERKO = 47,
	HISTORY_SAM = 48,
	HISTORY_FRANZ = 49,
	HISTORY_ARNOLD = 50,
	HISTORY_FREDO = 51,
	HISTORY_RICHGUY_BALIME = 52,
	HISTORY_JAKE = 53,
	HISTORY_BUM_KEYCARD = 54,
	HISTORY_WALTER = 55,
	HISTORY_DAVE = 56,
	HISTORY_PABLO = 57,
	HISTORY_KINGPIN_MONEY = 58,
	HISTORY_WON_BOXING = 59,
	HISTORY_LOST_BOXING = 60,
	HISTORY_DISQUALIFIED_BOXING = 61,
	HISTORY_FOUND_MONEY = 62,
	HISTORY_ASSASSIN = 63,
	HISTORY_LOSTTOWNSECTOR = 64,
	HISTORY_DEFENDEDTOWNSECTOR = 65,
	HISTORY_LOSTBATTLE = 66,
	HISTORY_FATALAMBUSH = 67,
	HISTORY_WIPEDOUTENEMYAMBUSH = 68,
	HISTORY_UNSUCCESSFULATTACK = 69,
	HISTORY_SUCCESSFULATTACK = 70,
	HISTORY_CREATURESATTACKED = 71,
	HISTORY_KILLEDBYBLOODCATS = 72,
	HISTORY_SLAUGHTEREDBLOODCATS = 73,
	HISTORY_NPC_KILLED = 74,
	HISTORY_GAVE_CARMEN_HEAD = 75,
	HISTORY_SLAY_MYSTERIOUSLY_LEFT = 76,
	HISTORY_MERC_KILLED_CHARACTER = 77,
}

Sender = 
{
	MAIL_ENRICO = 0,
	CHAR_PROFILE_SITE = 1,
	GAME_HELP = 2,
	IMP_PROFILE_RESULTS = 3,
	SPECK_FROM_MERC = 4,
	RIS_EMAIL = 5,
	BARRY_MAIL = 6,
	BOBBY_R = 7,
	KING_PIN = 8,
	JOHN_KULBA = 9,
	AIM_SITE = 10,
}

Items = 
{
	MONEY = 219,
	SW38 = 5,
	STUN_GRENADE = 131,
	DESERTEAGLE = 7,
	GASMASK = 213,
}

Profil = 
{                   
    FATHER = 100,
    HAMOUS = 63,
    PROF_ICECREAM = 162,
    RAT   = 81,
	VINCE = 69,
	STEVE = 80,
	JENNY = 132,
	DARREN = 87,
	TONY = 91,
	DEVIN = 61,
	CARMEN = 78,
	SAL = 99,
	PABLO = 98,
	SPIKE = 93,
	ROBOT = 62,
	JOEY = 90,	
	MADLAB = 146,
	CONRAD = 70,
	KINGPIN = 86,
	MADAME = 107,
}

Facts = 
{
	FACT_VINCE_EXPECTING_MONEY = 149,
	FACT_TONY_NOT_AVAILABLE = 86,
	FACT_DAVE_HAS_GAS = 226,
	FACT_PABLOS_BRIBED = 41, 
	FACT_PACKAGE_DAMAGED = 58,
	FACT_SHIPMENT_DELAYED_24_HOURS = 56,
	FACT_PACKAGE_LOST_PERMANENTLY = 58,
	FACT_PABLO_RETURNED_GOODS = 39,
	FACT_PABLO_PUNISHED_BY_PLAYER = 37,
	FACT_REALLY_NEW_BOBBYRAY_SHIPMENT_WAITING = 34,
	FACT_LARGE_SIZED_SHIPMENT_WAITING = 62,
	FACT_LARGE_SIZED_OLD_SHIPMENT_WAITING = 35,
	FACT_24_HOURS_SINCE_DOCTOR_TALKED_TO = 306,
	FACT_24_HOURS_SINCE_JOEY_RESCUED = 305,
	FACT_KINGPIN_KNOWS_MONEY_GONE = 103,
	FACT_KINGPIN_DEAD = 308,
	FACT_KINGPIN_CAN_SEND_ASSASSINS = 276,
	FACT_FOOD_QUEST_OVER = 52,
	FACT_WILLIS_HEARD_ABOUT_JOEY_RESCUE = 271,
	FACT_BRENDA_PATIENCE_TIMER_EXPIRED = 360,
	FACT_FIRST_ROBOT_DESTROYED = 203,
	FACT_ROBOT_READY_SECOND_TIME = 205,
	FACT_ROBOT_READY = 202,
	FACT_JOEY_ESCORTED = 108,
	FACT_KIDS_ARE_FREE = 318,
	FACT_OK_USE_HUMMER = 224,
}

Flags2 = 
{
	PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR = 1,
	PROFILE_MISC_FLAG2_LEFT_COUNTRY = 2,
	PROFILE_MISC_FLAG2_BANDAGED_TODAY = 4,
	PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE = 8,
	PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE = 16,
	PROFILE_MISC_FLAG2_MARRIED_TO_HICKS = 32,
	PROFILE_MISC_FLAG2_ASKED_BY_HICKS = 64,
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

NpcAction = 
{
	NPC_ACTION_ADD_JOEY_TO_WORLD = 136,
	NPC_ACTION_RETURN_STOLEN_SHIPMENT_ITEMS = 108,
	NPC_ACTION_SET_RANDOM_PACKAGE_DAMAGE_TIMER = 116,
	NPC_ACTION_ENABLE_CAMBRIA_DOCTOR_BONUS = 139,
	NPC_ACTION_TRIGGER_END_OF_FOOD_QUEST = 112,				
	NPC_ACTION_DELAYED_MAKE_BRENDA_LEAVE = 121,
	NPC_ACTION_SET_DELAY_TILL_GIRLS_AVAILABLE = 126,
	NPC_ACTION_SET_GIRLS_AVAILABLE = 125,
	NPC_ACTION_READY_ROBOT = 194,
	NPC_ACTION_SEND_ENRICO_MIGUEL_EMAIL	= 230,
	NPC_ACTION_TIMER_FOR_VEHICLE	= 211,
	NPC_ACTION_FREE_KIDS = 117,
}

EventTypes = 
{
	EVENT_SET_BY_NPC_SYSTEM = 27,
}

Group = 
{
	KINGPIN_CIV_GROUP = 2,
}

StatusGroup = 
{
	CIV_GROUP_NEUTRAL = 0,
	CIV_GROUP_WILL_EVENTUALLY_BECOME_HOSTILE = 1,
	CIV_GROUP_WILL_BECOME_HOSTILE = 2,
	CIV_GROUP_HOSTILE = 3,
}

qStatus = 
{
	QUESTNOTSTARTED =0,
	QUESTINPROGRESS = 1,
	QUESTDONE = 2,
}

Quests = 
{
	-- Quests ID							-- BinaryData\QUESTS.EDT Record
	QUEST_DELIVER_LETTER = 0,				-- Start quest 0, End quest 1   - Deliver Letter
	QUEST_FOOD_ROUTE = 1,					-- Start quest 2, End quest 3   - Food Route
	QUEST_KILL_TERRORISTS = 2,				-- Start quest 4, End quest 5   - Terrorists
	QUEST_KINGPIN_IDOL = 3,					-- Start quest 6, End quest 7   - Kingpin Chalice
	QUEST_KINGPIN_MONEY = 4,				-- Start quest 8, End quest 9   - Kingpin Money
	QUEST_RUNAWAY_JOEY = 5,					-- Start quest 10, End quest 11 - Runaway Joey
	QUEST_RESCUE_MARIA = 6,					-- Start quest 12, End quest 13 - Rescue Maria
	QUEST_CHITZENA_IDOL = 7,				-- Start quest 14, End quest 15 - Chitzena Chalice
	QUEST_HELD_IN_ALMA = 8,					-- Start quest 16, End quest 17 - Held in Alma
	QUEST_INTERROGATION = 9,				-- Start quest 18, End quest 19 - Interogation
	QUEST_ARMY_FARM = 10,					-- Start quest 20, End quest 21 - Hillbilly Problem
	QUEST_FIND_SCIENTIST = 11,				-- Start quest 22, End quest 23 - Find Scientist
	QUEST_DELIVER_VIDEO_CAMERA = 12,		-- Start quest 24, End quest 25 - Deliver Video Camera
	QUEST_BLOODCATS = 13,					-- Start quest 26, End quest 27 - Blood Cats
	QUEST_FIND_HERMIT = 14,					-- Start quest 28, End quest 29 - Find Hermit
	QUEST_CREATURES = 15,					-- Start quest 30, End quest 31 - Creatures
	QUEST_CHOPPER_PILOT = 16,				-- Start quest 32, End quest 33 - Find Chopper Pilot
	QUEST_ESCORT_SKYRIDER = 17,				-- Start quest 34, End quest 35 - Escort SkyRider
	QUEST_FREE_DYNAMO = 18,					-- Start quest 36, End quest 37 - Free Dynamo
	QUEST_ESCORT_TOURISTS = 19,				-- Start quest 38, End quest 39 - Escort Tourists
	QUEST_FREE_CHILDREN = 20,				-- Start quest 40, End quest 41 - Doreen
	QUEST_LEATHER_SHOP_DREAM = 21,			-- Start quest 42, End quest 43 - Leather Shop Dream
	QUEST_ESCORT_SHANK = 22,				-- Start quest 44, End quest 45 - Escort Shank
	-- UB Quests
	QUEST_DESTROY_MISSLES = 23,				 -- Start quest 46, End quest 47
	QUEST_FIX_LAPTOP = 24,					 -- Start quest 48, End quest 49
	QUEST_GET_RID_BLOODCATS_AT_BETTYS = 25,  -- Start quest 50, End quest 51
	QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL = 26, -- Start quest 52, End quest 53
	
	-- max Quests 254
}

ProfileType = 
{
	None = 0,
	AIM = 1,
	MERC = 2,
	RPC = 3,
	NPC = 4,
	Vehicle = 5,
	IMP = 6,
}

local MERC_OK = 0
local MERC_HAS_NO_TEXT_FILE = -1	
local MERC_ANNOYED_BUT_CAN_STILL_CONTACT = -2	
local MERC_ANNOYED_WONT_CONTACT = -3	
local MERC_HIRED_BUT_NOT_ARRIVED_YET = -4	
local MERC_IS_DEAD = -5
local MERC_RETURNING_HOME = -6	
local MERC_WORKING_ELSEWHERE = -7		
local MERC_FIRED_AS_A_POW = -8

local guiPabloExtraDaysBribed = 0

local BOBBYR_SHIPPING_DEST_SECTOR_X	= 13
local BOBBYR_SHIPPING_DEST_SECTOR_Y	= 2
local BOBBYR_SHIPPING_DEST_SECTOR_Z	= 0
local BOBBYR_SHIPPING_DEST_GRIDNO = 10112
local PABLOS_STOLEN_DEST_GRIDNO	= 1
local LOST_SHIPMENT_GRIDNO = 2

local NPC_SYSTEM_EVENT_ACTION_PARAM_BONUS = 10000

local gubCambriaMedicalObjects

function HandleEarlyMorningEvents()

-- JA2 Remove
--[[
	for i = 0, 254 do
		if ( WhoIs(ProfileType.RPC,i) == i or WhoIs(ProfileType.NPC,i) == i or WhoIs(ProfileType.Vehicle,i) == i ) then
			-- Set FALSE
			-- Loop through all *NPCs* and reset "default response used recently" flags
			SetThreatenDefaultResponseUsedRecently(i,false)
			SetRecruitDefaultResponseUsedRecently(i,false)
			SetFriendlyOrDirectDefaultResponseUsedRecently(i,false)
			SetMiscFlags2(i,Flags2.PROFILE_MISC_FLAG2_BANDAGED_TODAY)
		end
	end


	r = math.random(1, 4)
	SetNPCData1(Profil.FATHER,r)

	-- Set Walker's location
	o = math.random(1, 2)
	if (o > 1) then
		--  Move the father to the other sector, provided neither are loaded
		if  (not ( ( gWorldSectorX == 13) and ( ( gWorldSectorY == 3) or gWorldSectorY == 4 ) and ( gbWorldSectorZ == 0 ) ))  then

			SetCharacterSectorX(Profil.FATHER,13)
			SectorY = CheckCharacterSectorY(Profil.FATHER)
		
			-- Swap his location
			if ( SectorY == 3 ) then
				SetCharacterSectorY(Profil.FATHER,4)
			else
				SetCharacterSectorY(Profil.FATHER,3)
			end
		end
	end

	if ( CheckLastDateSpokenTot(Profil.TONY) > 0 and not ( gWorldSectorX == 5 and gWorldSectorY == 3 and gWorldSectorZ == 0 )) then
		
		-- San Mona C5 is not loaded so make Tony possibly not available
		p = math.random(0, 100)

		if ( p <= iniCHANCE_TONY_AVAILABLE ) then 
			-- Tony IS available
			SetFactFalse( Facts.FACT_TONY_NOT_AVAILABLE )
			AddNPCtoSector (Profil.TONY,5,3,0)
		else
			-- Tony is NOT available
			SetFactTrue( Facts.FACT_TONY_NOT_AVAILABLE )
			AddNPCtoSector (Profil.TONY,0,0,0)
		end
	end

	if ( CheckLastDateSpokenTot(Profil.DEVIN) == 0 ) then

		DData = CheckNPCData1 (Profil.DEVIN)
		i = DData + 1
		SetNPCData1 (Profil.DEVIN,i)

		if ( CheckNPCData1 (Profil.DEVIN) > 3 ) then
			Devinid,Devinx,Deviny,Devinz = CheckNPCinSector(Profil.DEVIN)
			if ( not ( (gWorldSectorX == Devinx) and (gWorldSectorY == Deviny) and (gWorldSectorZ == 0) ) ) then
				
				-- Ok, Devin's sector not loaded, so time to move!
				-- Might be same sector as before, if so, oh well!
				p = math.random(1, 5)
				if ( p == 1 ) then
					--G9
				AddNPCtoSector (Profil.DEVIN,9,7,0)
				elseif ( p == 2 ) then
					--D13
					AddNPCtoSector (Profil.DEVIN,13,4,0)		
				elseif ( p == 3 ) then
					--C5
					AddNPCtoSector (Profil.DEVIN,5,3,0)					
				elseif ( p == 4 ) then
					--H2
					AddNPCtoSector (Profil.DEVIN,2,8,0)	
				elseif ( p == 5 ) then
					--C6
					AddNPCtoSector (Profil.DEVIN,6,3,0)		
				end			
			end
		end
	end

	-- Stop moving the truck if Hamous is dead!!
	-- Stop moving them if the player has the truck or Hamous is hired!
	
	HAMOUSid,HAMOUSx,HAMOUSy,HAMOUSz = CheckNPCinSector(Profil.HAMOUS)

	if ( CheckNPCLife(Profil.HAMOUS) > 0 and CheckMercIsDead (Profil.HAMOUS) == false and CheckMercIsDead (Profil.PROF_ICECREAM) == false and (not ( (gWorldSectorX == HAMOUSx) and (gWorldSectorY == HAMOUSy) and (gWorldSectorZ == 0) )) ) then

		-- Ok, HAMOUS's sector not loaded, so time to move!
		-- Might be same sector as before, if so, oh well!
		i = math.random(1, 5)
		if i == 1 then
			-- G6
			AddNPCtoSector (Profil.HAMOUS,6,7,0) 
			AddNPCtoSector (Profil.PROF_ICECREAM,6,7,0) 
		elseif i == 2 then
			-- F12
			AddNPCtoSector (Profil.HAMOUS,12,6,0) 
			AddNPCtoSector (Profil.PROF_ICECREAM,12,6,0) 
		elseif i == 3 then
			-- D7
			AddNPCtoSector (Profil.HAMOUS,7,4,0) 
			AddNPCtoSector (Profil.PROF_ICECREAM,7,4,0) 
		elseif i == 4 then
			-- D3
			AddNPCtoSector (Profil.HAMOUS,3,4,0) 
			AddNPCtoSector (Profil.PROF_ICECREAM,3,4,0) 
		elseif i == 5 then
			-- D9
			AddNPCtoSector (Profil.HAMOUS,9,4,0) 
			AddNPCtoSector (Profil.PROF_ICECREAM,9,4,0) 
		end
	end

	-- Does Rat take off?
	if ( CheckNPCData1 (Profil.RAT) > 0) then
		SetCharacterSectorX (Profil.RAT,0)
		SetCharacterSectorY (Profil.RAT,0)
		SetCharacterSectorZ (Profil.RAT,0)
	end
	
	-- Empty money from pockets of Vince 69, Willis 80, and Jenny 132
	SetMoneyInSoldierProfile( Profil.VINCE, 0 )
	SetMoneyInSoldierProfile( Profil.STEVE, 0 )
	SetMoneyInSoldierProfile( Profil.JENNY, 0 )

	-- Vince is no longer expecting money
	SetFactFalse( Facts.FACT_VINCE_EXPECTING_MONEY )

	-- Reset Darren's balance and money
	SetNPCBalance(Profil.DARREN,0)
	SetMoneyInSoldierProfile( Profil.DARREN, 15000 )
	
	-- Set Carmen to be placed on the map in case he moved and is waiting off screen
	if CheckMiscFlags2(Profil.CARMEN,Flags2.PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR) == 1 then

		SetMiscFlags2(Profil.CARMEN, Flags2.PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR)
	
		-- Move Carmen to C13
		AddNPCtoSector (Profil.CARMEN,13,3,0) 

		-- We should also reset # of terrorist heads and give him cash
		if CheckNPCData2 (Profil.CARMEN) > 0 then
		
			Money = CheckMoney(Profil.CARMEN)
			
			if (Money < 10000 ) then	
				uiAmount = 0
			else
				uiAmount = Money
			end
			
			uiAmount = uiAmount + 10000 * CheckNPCData2( Profil.CARMEN )
			SetMoneyInSoldierProfile( Profil.CARMEN, uiAmount )
			SetNPCData2(Profil.CARMEN,0)

			for i = 248, 254 do
				RemoveObjectFromSoldierProfile( Profil.CARMEN, i )
			end
		end
	else
	
		-- Randomize where he'll be today... so long as his sector's not loaded
		CARMENid,CARMENx,CARMENy,CARMENz = CheckNPCinSector(Profil.CARMEN)
		if ( CARMENx ~= gWorldSectorX or CARMENy ~= gWorldSectorY ) then
			
			i = math.random(1, 3)
			if i == 1 then
				AddNPCtoSector (Profil.CARMEN,5,3,0) -- C5
			elseif i == 2 then
				AddNPCtoSector (Profil.CARMEN,13,3,0) -- C13
			elseif i == 3 then
				AddNPCtoSector (Profil.CARMEN,9,7,0) -- G9
			end
		
			-- He should have $5000... unless the player forgot to meet him
			if ( CheckMoney(Profil.CARMEN) < 5000 ) then
				SetMoneyInSoldierProfile( Profil.CARMEN, 5000 )
			end	
		end
	end

	i = math.random(1, 3)	
	if i == 1 then
		SetFactTrue( Facts.FACT_DAVE_HAS_GAS )
	else
		SetFactFalse( Facts.FACT_DAVE_HAS_GAS )
	end	
	
	if ( gWorldSectorX == iniHOSPITAL_SECTOR_X and gWorldSectorY == iniHOSPITAL_SECTOR_Y and gWorldSectorZ == iniHOSPITAL_SECTOR_Z ) then
		CheckForMissingHospitalSupplies()
	end
]]
end