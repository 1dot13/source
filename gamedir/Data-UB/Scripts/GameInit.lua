--[[
http://legion.itzg.pl/mzgb/pmwiki/pmwiki.php?n=JaggedAlliance2UBV1.Jav113lua

**********************
** Global variables **
**********************

	- newDIFFICULTY_LEVEL (Start new game setting)
		1 = easy
		2 = experienced
		3 = expert
		4 = insane
        
	- newGAME_STYLE (Start new game setting)
		0 = realistic
		1 = sci fi
        
	- is_networked (Multiplayer)
		0 = no
		1 = yes
        
	- iniENABLE_CREPITUS (ja2_options.ini setting)
		0 = no
		1 = yes
        
	- iniSTARTING_CASH_NOVICE, iniSTARTING_CASH_EXPERIENCED, iniSTARTING_CASH_EXPERT, iniSTARTING_CASH_INSANE (ja2_options.ini setting)
		Starting cash
      
	- iniMERC_WEBSITE_IMMEDIATELY_AVAILABLE (ja2_options.ini setting)
		true = yes
		false = no

	- giHospitalTempBalance
		set hospital balance

	- gbHospitalPriceModifier
		set hospital modifier

	- giHospitalRefund


***************		  
** Functions **
***************
          
	- AddAlternateSector, AddAltSector, AddAltSectorNew 
		add alternative sector
	
	- AddNPC or AddNPCtoSector
		add NPC\EPC\RPC to sector
	
	- AddAltUnderGroundSector, AddAltUGSector or AddAltUGSectorNew
		add alternative underground sector
	
	- SetNPCData1 ( ProfilID, value )
		set NPCData1
	
	- GetStartingCashNovice()
		get starting cash novice level
	
	- GetStartingCashExperienced()
		get starting cash experienced level
	 
	- GetStartingCashExpert()
		get starting cash expert level
	  
	- GetStartingCashInsane()
		get starting cash insane level
	
	- GetWorldTotalMin()
		get world time
	
	- AddTransactionToPlayersBook(ubCode, ubSecondCode, uiDate, iAmount)
		add transaction to player
	
	- AddPreReadEmail (iMessageOffset, iMessageLength, ubSender)
		iMessageOffset - record from email.edt
		iMessageLength - record from email.edt
		ubSender - uiIndex from SenderNameList.xml 
	
	- AddEmail (iMessageOffset, iMessageLength, ubSender, iCurrentIMPPosition, iCurrentShipmentDestinationID)
		iMessageOffset - record from email.edt
		iMessageLength - record from email.edt
		ubSender - uiIndex from SenderNameList.xml
		iCurrentIMPPosition - Default = -1, or the profile ID of the IMP 
		iCurrentShipmentDestinationID - Default = -1, or the Bobby Ray shipment destination ID
	
	- HireMerc (MercID)
		hires the merc with the specified MercID


**************        
** Examples **
**************

    -- Add Fatima to sector
    Fatima = { }
    Fatima.MercProfiles = 101
    Fatima.sector = "A10-0"
    AddNPC(Fatima)
    
    -- Add Fatima to sector
    AddNPC( { MercProfiles = 101 , sector = "A10-0" } )
    
	-- Add Fatima to sector, only in a realistic game
	if newGAME_STYLE == 0 then
	    Fatima = { }
	    Fatima.MercProfiles = 101
	    Fatima.sector = "A10-0"
	    AddNPC(Fatima)
	end
	    
	-- Add alternative sector, only when starting cash = 4000 and game style SciFi
	if (iniSTARTING_CASH_NOVICE == 4000 and newGAME_STYLE == 1) then
	    Fatima = { }
	    Fatima.MercProfiles = 101
	    Fatima.sector = "A10-0"
	    AddNPC(Fatima)
	end
	
	-- Add alternative sector, only realistic game
	if newGAME_STYLE == 0 then
	    SectorA9 = { }
	    SectorA9.altSector = "A9"
	    AddAlternateSector(SectorA9)
	end
	
	-- Add alternative sector
	SectorA9 = { }
	SectorA9.altSector = "A9"
	AddAlternateSector(SectorA9)
	
	-- Add alternative underground sector
	A10_b1 = { }
	A10_b1.altSector = "A10-1"
	AddAltUGSector(A10_b1)	
	
	-- Add alternative underground sector
	AddAltUGSector( { altSector = "A10-1" } )	
	
	    -- Add Skyrider to sector
	AddNPCtoSector(97,9,1,0)
	
	-- Add alternative sector C1
	AddAltSectorNew (3,1)
	
	-- Add alternative sector C1-3
	AddAltUGSectorNew(3,1,3)
	
	-- Hire Ivan
	HireMerc(7)
]]

-- Initialize the pseudo random number generator
math.randomseed( os.time() ); math.random(); math.random(); math.random()
-- http://lua-users.org/wiki/MathLibraryTutorial



local MAX_EMAIL_LINES = 10 --max number of lines can be shown in a message
local MAX_MESSAGES_PAGE = 18 -- max number of messages per page

--JA25:  New emails
-- email # 1
local EMAIL_STARTGAME			=	0
local EMAIL_STARTGAME_LENGTH	=	8

-- email # 2
local EMAIL_PILOTMISSING		=	(EMAIL_STARTGAME + EMAIL_STARTGAME_LENGTH)		
local EMAIL_PILOTMISSING_LENGTH	=	2

-- email # 3
local EMAIL_MAKECONTACT			=	(EMAIL_PILOTMISSING + EMAIL_PILOTMISSING_LENGTH)	
local EMAIL_MAKECONTACT_LENGTH	=	3

-- email # 4
local EMAIL_MANUEL				=	(EMAIL_MAKECONTACT + EMAIL_MAKECONTACT_LENGTH)
local EMAIL_MANUEL_LENGTH		=	4

-- # email # 5  - only if Miguel is alive!
local EMAIL_MIGUELHELLO			=	(EMAIL_MANUEL + EMAIL_MANUEL_LENGTH)
local EMAIL_MIGUELHELLO_LENGTH	=	3

-- email # 6 - player not advancing fast enough
local EMAIL_CONCERNED			=	(EMAIL_MIGUELHELLO + EMAIL_MIGUELHELLO_LENGTH)
local EMAIL_CONCERNED_LENGTH	=	2

-- email # 7 - player still not advancing fast enough
local EMAIL_URGENT				=	(EMAIL_CONCERNED + EMAIL_CONCERNED_LENGTH)
local EMAIL_URGENT_LENGTH		=	3

-- email # 8a  - from Miguel
local EMAIL_MIGUELSORRY				=	(EMAIL_URGENT + EMAIL_URGENT_LENGTH)
local EMAIL_MIGUELSORRY_LENGTH		=	3

-- email # 8b  - from Miguel, mentioning Manuel
local EMAIL_MIGUELMANUEL			=	(EMAIL_MIGUELSORRY + EMAIL_MIGUELSORRY_LENGTH)
local EMAIL_MIGUELMANUEL_LENGTH		=	4

-- email # 9 - Miguel sick
local EMAIL_MIGUELSICK			=	(EMAIL_MIGUELMANUEL + EMAIL_MIGUELMANUEL_LENGTH)
local EMAIL_MIGUELSICK_LENGTH	=	3

-- email # 10a
local EMAIL_UNDERSTANDING		 =	(EMAIL_MIGUELSICK + EMAIL_MIGUELSICK_LENGTH)
local EMAIL_UNDERSTANDING_LENGTH =	3

-- email # 10b - if we need extra cash
local EMAIL_EXTRACASH			=	(EMAIL_UNDERSTANDING + EMAIL_UNDERSTANDING_LENGTH)
local EMAIL_EXTRACASH_LENGTH	=	4

-- email # 11
local EMAIL_PILOTFOUND			=	(EMAIL_EXTRACASH + EMAIL_EXTRACASH_LENGTH)
local EMAIL_PILOTFOUND_LENGTH	=	2

-- email # 12a - Miguel dead, Manuel never recruited
local EMAIL_CONGRATS			=	(EMAIL_PILOTFOUND + EMAIL_PILOTFOUND_LENGTH)
local EMAIL_CONGRATS_LENGTH		=	4

-- email # 12b - Miguel alive, Manuel never recruited
local EMAIL_CONGRATSICK				=	(EMAIL_CONGRATS + EMAIL_CONGRATS_LENGTH)
local EMAIL_CONGRATSICK_LENGTH		=	5

-- email # 12c - Miguel alive, Manuel dead
local EMAIL_CONGRATMIGMANUELDEAD		=	(EMAIL_CONGRATSICK + EMAIL_CONGRATSICK_LENGTH)
local EMAIL_CONGRATMIGMANUELDEAD_LENGTH	=	6

-- email # 12d - Miguel alive, Manuel recruited and alive
local EMAIL_CONGRATMIGMANUELALIVE		=	(EMAIL_CONGRATMIGMANUELDEAD + EMAIL_CONGRATMIGMANUELDEAD_LENGTH)
local EMAIL_CONGRATMIGMANUELALIVE_LENGTH	=	6

-- email # 12e - Miguel dead, Manuel dead
local EMAIL_CONGRATMANUELDEAD			=	(EMAIL_CONGRATMIGMANUELALIVE + EMAIL_CONGRATMIGMANUELALIVE_LENGTH)
local EMAIL_CONGRATMANUELDEAD_LENGTH	=	5

-- email # 12f -  Miguel dead, Manuel recruited and alive
local EMAIL_CONGRATMANUELALIVE			=	(EMAIL_CONGRATMANUELDEAD + EMAIL_CONGRATMANUELDEAD_LENGTH)
local EMAIL_CONGRATMANUELALIVE_LENGTH	=	5

-- email # 13 - original AIM email
local EMAIL_AIMDISCOUNT				=	(EMAIL_CONGRATMANUELALIVE + EMAIL_CONGRATMANUELALIVE_LENGTH)
local EMAIL_AIMDISCOUNT_LENGTH		=	7

local MANUEL_AT_HOME_NOT_USED			=	( EMAIL_AIMDISCOUNT + EMAIL_AIMDISCOUNT_LENGTH )
local MANUEL_AT_HOME_NOT_USED_LENGTH	=	1

-- Email #14 Initial IMP email
local IMP_EMAIL_INTRO			=	(MANUEL_AT_HOME_NOT_USED + MANUEL_AT_HOME_NOT_USED_LENGTH )
local IMP_EMAIL_INTRO_LENGTH	=	10

-- Email #15 Imp follow up
local IMP_EMAIL_AGAIN			= (IMP_EMAIL_INTRO + IMP_EMAIL_INTRO_LENGTH)
local IMP_EMAIL_AGAIN_LENGTH	= 5

-- Email #16 - ??  merc left-me-a-message-and-now-I'm-back emails
local AIM_REPLY_BARRY			=	( IMP_EMAIL_AGAIN + IMP_EMAIL_AGAIN_LENGTH )
local AIM_REPLY_LENGTH_BARRY	=	2

local AIM_REPLY_MELTDOWN		=	(AIM_REPLY_BARRY + ( 39 * AIM_REPLY_LENGTH_BARRY ))
local AIM_REPLY_LENGTH_MELTDOWN	=	AIM_REPLY_LENGTH_BARRY

local AIM_REFUND			=	( AIM_REPLY_MELTDOWN + AIM_REPLY_LENGTH_MELTDOWN )
local AIM_REFUND_LENGTH	=		3

local MERC_REFUND			=		( AIM_REFUND + AIM_REFUND_LENGTH )
local MERC_REFUND_LENGTH	=		3

local EMAIL_AIM_PROMOTION_1				=	( MERC_REFUND + MERC_REFUND_LENGTH )
local EMAIL_AIM_PROMOTION_1_LENGTH		=	4

local EMAIL_MERC_PROMOTION_1				=	( EMAIL_AIM_PROMOTION_1 + EMAIL_AIM_PROMOTION_1_LENGTH )
local EMAIL_MERC_PROMOTION_1_LENGTH		=	5

local EMAIL_AIM_PROMOTION_2				=	( EMAIL_MERC_PROMOTION_1 + EMAIL_MERC_PROMOTION_1_LENGTH )
local EMAIL_AIM_PROMOTION_2_LENGTH		=	5

local IMP_EMAIL_PROFILE_RESULTS				=	( EMAIL_AIM_PROMOTION_2 + EMAIL_AIM_PROMOTION_2_LENGTH )
local IMP_EMAIL_PROFILE_RESULTS_LENGTH		=	1


local iStartingCash = 0

Fincances = 
{
	ACCRUED_INTEREST = 0,
	ANONYMOUS_DEPOSIT = 1,
	TRANSACTION_FEE = 2,
	HIRED_MERC = 3,
	BOBBYR_PURCHASE = 4,
	PAY_SPECK_FOR_MERC = 5,
	MEDICAL_DEPOSIT = 6,
	IMP_PROFILE = 7,
	PURCHASED_INSURANCE = 8,
	REDUCED_INSURANCE = 9,
	EXTENDED_INSURANCE = 10,
	CANCELLED_INSURANCE = 11,
	INSURANCE_PAYOUT = 12,
	EXTENDED_CONTRACT_BY_1_DAY = 13,
	EXTENDED_CONTRACT_BY_1_WEEK = 14,
	EXTENDED_CONTRACT_BY_2_WEEKS = 15,
	DEPOSIT_FROM_GOLD_MINE = 16,
	DEPOSIT_FROM_SILVER_MINE = 17,
	PURCHASED_FLOWERS = 18,
	FULL_MEDICAL_REFUND = 19,
	PARTIAL_MEDICAL_REFUND = 20,
	NO_MEDICAL_REFUND = 21,
	PAYMENT_TO_NPC = 22,
	TRANSFER_FUNDS_TO_MERC = 23,
	TRANSFER_FUNDS_FROM_MERC = 24,
	TRAIN_TOWN_MILITIA = 25,
	PURCHASED_ITEM_FROM_DEALER = 26,
	MERC_DEPOSITED_MONEY_TO_PLAYER_ACCOUNT = 27,
	SOLD_ITEMS = 28,
	FACILITY_OPERATIONS = 29, 	
	MILITIA_UPKEEP = 30, 		
}

Sender = 
{
	MAIL_ENRICO = 0,
	CHAR_PROFILE_SITE = 1,
	GAME_HELP = 2,
	IMP_PROFILE_RESULTS = 3,
	SPECK_FROM_MERC = 4,
	RIS_EMAIL = 5,
	INSURANCE_COMPANY = 46,
	BOBBY_R = 47,
	KING_PIN = 48,
	JOHN_KULBA = 49,
	AIM_SITE = 50,
}

 Modifier = 
 {  
    HOSPITAL_UNSET = 0,
    HOSPITAL_NORMAL = 1,
    HOSPITAL_BREAK = 2,
    HOSPITAL_COST = 3,
    HOSPITAL_FREEBIE = 4,
    HOSPITAL_RANDOM_FREEBIE = 5,
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

Profil = 
{
	Manuel = 60,
	Biggens = 61,
	Kulba = 62,
	Tex = 64,
	JerryMilo = 76,
	Morris = 75,
}

Facts = 
{
FACT_IMPORTED_SAVE_AND_MARY_WAS_DEAD = 400,
FACT_TEX_IS_IN_GAME_AND_ALIVE_IN_STORE = 402,
}

local DIF_LEVEL_EASY = 1
local DIF_LEVEL_MEDIUM = 2
local DIF_LEVEL_HARD = 3
local DIF_LEVEL_INSANE = 4

ShopID = {
	ARMS_DEALER_TONY = 0,
	ARMS_DEALER_FRANZ = 1,
	ARMS_DEALER_KEITH = 2,
	ARMS_DEALER_JAKE = 3,
	ARMS_DEALER_GABBY = 4,
	ARMS_DEALER_HOWARD = 5,
	ARMS_DEALER_SAM = 6,
	ARMS_DEALER_FRANK = 7,
	ARMS_DEALER_BAR_BRO_1 = 8,
	ARMS_DEALER_BAR_BRO_2 = 9,
	ARMS_DEALER_BAR_BRO_3 = 10,
	ARMS_DEALER_BAR_BRO_4 = 11,
	ARMS_DEALER_MICKY = 12,
	ARMS_DEALER_ARNIE = 13,
	ARMS_DEALER_FREDO = 14,
	ARMS_DEALER_RAUL = 15, -- Raul UB
	ARMS_DEALER_ELGIN = 16,
	ARMS_DEALER_MANNY = 17,
	ARMS_DEALER_BETTY = 18, -- Betty UB
}

local function AddTexsVideosToBettysInventory()
	GuaranteeAtLeastXItemsOfIndex( ShopID.ARMS_DEALER_BETTY, UB_itemTEX_MOVIE_ATTACK_CLYDESDALES, 1 ) -- Attack of the Clydesdales
	GuaranteeAtLeastXItemsOfIndex( ShopID.ARMS_DEALER_BETTY, UB_itemTEX_MOVIE_WILD_EAST, 1 ) -- Wild, Wild East
	GuaranteeAtLeastXItemsOfIndex( ShopID.ARMS_DEALER_BETTY, UB_itemTEX_MOVIE_HAVE_HONDA, 1 ) -- Have Honda, Will Travel
end

function InitStrategicLayer()

			UB_InitialHeliCrash(UB_iniINGAMEHELICRASH)

			if ( UB_iniJERRY == true ) then
				AddProfileToMap (UB_GetJerryID(),iniDEFAULT_ARRIVAL_SECTOR_X,iniDEFAULT_ARRIVAL_SECTOR_Y,0,UB_iniJERRYGRIDNO) -- get id from UB_Options.ini
				--InitialProfile ( UB_GetJerryID(), iniDEFAULT_ARRIVAL_SECTOR_X, iniDEFAULT_ARRIVAL_SECTOR_Y,0, UB_iniJERRYGRIDNO )
			end
			
			UB_InitialJerryQuotes(iniJERRYQUOTES)
			UB_InitialLaptopQuest(iniLAPTOP_QUEST)
			
			UB_InitialHeliGridNo1(iniINITIALHELIGRIDNO1)
			UB_InitialHeliGridNo2(iniINITIALHELIGRIDNO2)
			UB_InitialHeliGridNo3(iniINITIALHELIGRIDNO3)
			UB_InitialHeliGridNo4(iniINITIALHELIGRIDNO4)
			UB_InitialHeliGridNo5(iniINITIALHELIGRIDNO5)
			UB_InitialHeliGridNo6(iniINITIALHELIGRIDNO6)
			UB_InitialHeliGridNo7(iniINITIALHELIGRIDNO7)
	
			-- get DEFAULT_ARRIVAL_SECTOR_X and DEFAULT_ARRIVAL_SECTOR_Y from UB_Options.ini
			SetDefaultArrivalSector( iniDEFAULT_ARRIVAL_SECTOR_X, iniDEFAULT_ARRIVAL_SECTOR_Y ) 
		
			-- get NEW_MERC_ARRIVAL_LOCATION from UB_Options.ini
			UB_InternalLocateGridNo( iniNEW_MERC_ARRIVAL_LOCATION )
end


function InitNPCs()
local ran
local ubNumAdmins = 0
local ubNumTroops = 0
local ubNumElites = 0
local Test = false
local JohnKulbaText = true
	
if ( UB_RandomManuelText == true )	then			

	-- Miguel is dead
	Dead = math.random(1, 2)
	if ( Dead == 1 ) then  
		SetFactTrue( 414 ) -- set fact 
	end

	-- Fatima or Pacos is dead
	Dead = math.random(1, 2)
	if ( Dead == 1 ) then  
		SetFactTrue( 415 ) -- set fact 
	end
		
	-- Carlos is dead
	Dead = math.random(1, 2)
	if ( Dead == 1 ) then  
		SetFactTrue( 416 ) -- set fact 
	end
		
	-- Ira is dead
	Dead = math.random(1, 2)
	if ( Dead == 1 ) then  
		SetFactTrue( 417 ) -- set fact 
	end

	-- Dimitri is dead
	Dead = math.random(1, 2)
	if ( Dead == 1 ) then  
		SetFactTrue( 418 ) -- set fact 
	end		
end

-- Random John or Tex
if ( UB_enabledJohnAndTex == false) then

ran = math.random(1, 2)

		if ( ran == 1 ) then  
		
			if ( UB_JohnKulbaText == true ) then
				-- Mary is dead
				MaryDead = math.random(1, 2)
				if ( MaryDead == 1 ) then  	
					SetFactTrue( Facts.FACT_IMPORTED_SAVE_AND_MARY_WAS_DEAD ) -- set fact
				end
			end
		
		--John is available
			UB_JohnKulbaIsInGame (true)
		
			--Tex is not available
			SetFactFalse( Facts.FACT_TEX_IS_IN_GAME_AND_ALIVE_IN_STORE )
			
					UB_JohnKulbaInitialSectorY (SectorY.MAP_ROW_I)
					-- Randomly choose John's starting sector
					if( math.random(1, 100 ) < 50 ) then
						UB_JohnKulbaInitialSectorX (10)
						AddProfileToMap (UB_GetJohnID(),10,9,0,10503) -- get id from UB_Options.ini
					else
						UB_JohnKulbaInitialSectorX (11)
						AddProfileToMap (UB_GetJohnID(),11,9,0,17674) -- get id from UB_Options.ini
					end
							

		else
			--John is not available
			UB_JohnKulbaIsInGame (false)	
			
				-- added video to Betty Shop
				if ( UB_CheckJohnKulbaIsInGame() == false ) then
					AddTexsVideosToBettysInventory()
				end
				
			--Tex is available
			SetFactTrue( Facts.FACT_TEX_IS_IN_GAME_AND_ALIVE_IN_STORE )
			
			AddProfileToMap (UB_GetTexID(),10,9,0,12890) -- get id from UB_Options.ini
		end
		
		
-- John and Tex	in game	
elseif ( UB_enabledJohnAndTex == true) then

			if ( UB_JohnKulbaText == true ) then
				-- Mary is dead
				MaryDead = math.random(1, 2)
				if ( MaryDead == 1 ) then  	
					SetFactTrue( Facts.FACT_IMPORTED_SAVE_AND_MARY_WAS_DEAD ) -- set fact
				end
			end

			--John is available
			UB_JohnKulbaIsInGame (true)
			
			UB_JohnKulbaInitialSectorY (SectorY.MAP_ROW_I)
				
				--Randomly choose John's starting sector
			if( math.random(1, 100 ) < 50 ) then
				UB_JohnKulbaInitialSectorX (10)
				AddProfileToMap (UB_GetJohnID(),10,9,0,10503) -- get id from UB_Options.ini
			else
				UB_JohnKulbaInitialSectorX (11)
				AddProfileToMap (UB_GetJohnID(),11,9,0,17674) -- get id from UB_Options.ini
			end
				
			-- added video to Betty Shop
			AddTexsVideosToBettysInventory()	
				
			--Tex is available
			SetFactTrue( Facts.FACT_TEX_IS_IN_GAME_AND_ALIVE_IN_STORE )
			
			AddProfileToMap (UB_GetTexID(),10,9,0,12890) -- get id from UB_Options.ini

end

-- Add UB profiles
AddProfileToMap (UB_GetManuelID(),9,9,0,15758) -- get id from UB_Options.ini
AddProfileToMap (UB_GetBiggensID(),13,9,0,14824) -- get id from UB_Options.ini
AddProfileToMap (UB_GetBettyID(),10,9,0,12897) -- get id from UB_Options.ini
AddProfileToMap (UB_GetRaulID(),11,9,0,10630) -- get id from UB_Options.ini
AddProfileToMap (UB_GetMorrisID(),15,12,3,14752) -- get id from UB_Options.ini
AddProfileToMap (UB_GetRudyID(),13,10,0,18320) -- get id from UB_Options.ini

-- Add test UB profiles
--[[
AddProfileToMap (UB_GetManuelID(),7,8,0,15108) -- get id from UB_Options.ini
AddProfileToMap (UB_GetBiggensID(),7,8,0,15107) -- get id from UB_Options.ini
AddProfileToMap (UB_GetJohnID(),7,8,0,15106) -- get id from UB_Options.ini
AddProfileToMap (UB_GetTexID(),7,8,0,15105) -- get id from UB_Options.ini
AddProfileToMap (UB_GetGastonID(),7,8,0,15104) -- get id from UB_Options.ini
AddProfileToMap (UB_GetJerryID(),7,8,0,15103) -- get id from UB_Options.ini
AddProfileToMap (UB_GetPgmale4ID(),7,8,0,15102) -- get id from UB_Options.ini
AddProfileToMap (UB_GetBettyID(),7,8,0,15101) -- get id from UB_Options.ini
AddProfileToMap (UB_GetRaulID(),7,8,0,15109) -- get id from UB_Options.ini
AddProfileToMap (UB_GetMorrisID(),7,8,0,15110) -- get id from UB_Options.ini
AddProfileToMap (UB_GetRudyID(),7,8,0,15111) -- get id from UB_Options.ini
]]


-- Added enemy to sector
if ( Test == false ) then
		-- H7
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
			ubNumAdmins = 0
			ubNumTroops = 0
			ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
			ubNumAdmins = 0
			ubNumTroops = 0
			ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
			ubNumAdmins = 0
			ubNumTroops = 0
			ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
			ubNumAdmins = 0
			ubNumTroops = 0
			ubNumElites = 0
		end
		UB_SetNumberJa25EnemiesInSurfaceSector( 7, 8, ubNumAdmins, ubNumTroops, ubNumElites )


		-- H8
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0
				ubNumTroops = 6 + math.random( 1, 4 )
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 13 + math.random( 1, 3 )
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 16 + math.random( 1, 6 )
				ubNumElites = 2 + math.random( 1,2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then	
				ubNumAdmins = 0
				ubNumTroops = 19 + math.random( 1, 9 )
				ubNumElites = 2 + math.random( 1, 2 )
		end
				
		UB_SetNumberJa25EnemiesInSurfaceSector( 8,8, ubNumAdmins, ubNumTroops, ubNumElites )
	

	-- Guard Post
	-- H9
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0
				ubNumTroops = 10 + math.random( 1, 5 )
				ubNumElites = 1
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0
				ubNumTroops = 16 + math.random( 1, 4 )
				ubNumElites = 1
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 19 + math.random( 1, 4 )
				ubNumElites = 3 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 19 + math.random( 1, 6 )
				ubNumElites = 3 + math.random( 1, 2 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 9,8, ubNumAdmins, ubNumTroops, ubNumElites )

	-- H10
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0 
				ubNumTroops = 6 + math.random( 1, 5 )
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 9 + math.random( 1, 3 )
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0
				ubNumTroops = 10 + math.random( 1, 4 )
				ubNumElites = 2 + math.random( 1, 4 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 20 + math.random( 1, 4 )
				ubNumElites = 2 + math.random( 1, 4 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 10,8, ubNumAdmins, ubNumTroops, ubNumElites )

	-- H11
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0
				ubNumTroops = 6 + math.random( 1, 3 )
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 8 + math.random( 1, 3 )
				ubNumElites = 2 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0
				ubNumTroops = 10 + math.random( 1, 4 )
				ubNumElites = 4 + math.random( 1, 3 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 20 + math.random( 1, 4 )
				ubNumElites = 2 + math.random( 1, 3 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 11,8, ubNumAdmins, ubNumTroops, ubNumElites )


	-- I9
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0
				ubNumTroops = 6 + math.random( 1, 5 )
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0
				ubNumTroops = 9 + math.random( 1, 4 )
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0
				ubNumTroops = 10 + math.random( 1, 4 )
				ubNumElites = 2 + math.random( 1, 4 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0
				ubNumTroops = 20 + math.random( 1, 6 )
				ubNumElites = 2 + math.random( 1, 4 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 9,9, ubNumAdmins, ubNumTroops, ubNumElites )

	-- First part of town
	-- I10
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0
				ubNumTroops = 13 + math.random( 1, 6 )
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0
				ubNumTroops = 16 + math.random( 1, 4 )
				ubNumElites = 1 + math.random( 1, 3 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0
				ubNumTroops = 14 + math.random( 1, 4 )
				ubNumElites = 5 + math.random( 1, 3 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0
				ubNumTroops = 20 + math.random( 1, 4 )
				ubNumElites = 5 + math.random( 1, 3 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 10,9, ubNumAdmins, ubNumTroops, ubNumElites )
		
	-- Second part of town
	-- I11
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0
				ubNumTroops = 14 + math.random( 1, 7 )
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 16 + math.random( 1, 5 )
				ubNumElites = 2 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0
				ubNumTroops = 14 + math.random( 1, 4 )
				ubNumElites = 5 + math.random( 1, 4 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0
				ubNumTroops = 17 + math.random( 1, 4 )
				ubNumElites = 5 + math.random( 1, 4 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 11,9, ubNumAdmins, ubNumTroops, ubNumElites )

	-- I12
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0 
				ubNumTroops = 8 + math.random( 1, 6 )
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 12 + math.random( 1, 5 )
				ubNumElites = 2 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 12 + math.random( 1, 6 )
				ubNumElites = 3 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 20 + math.random( 1, 6 )
				ubNumElites = 3 + math.random( 1, 2 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 12,9, ubNumAdmins, ubNumTroops, ubNumElites )

	-- Abandoned mine
	-- I13
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0 
				ubNumTroops = 0 
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 0 
				ubNumElites = 0
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 0 
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 0 
				ubNumElites = 0 
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 13,9, ubNumAdmins, ubNumTroops, ubNumElites )

	-- J11
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0 
				ubNumTroops = 10 + math.random( 1, 6 )
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 10 + math.random( 1, 3 )
				ubNumElites = 3 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 11 + math.random( 1, 5 )
				ubNumElites = 6 + math.random( 1, 3 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 20 + math.random( 1, 5 )
				ubNumElites = 6 + math.random( 1, 3 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 11,10, ubNumAdmins, ubNumTroops, ubNumElites )

	-- J12
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0 
				ubNumTroops = 12 + math.random( 1, 4 )
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 10 + math.random( 1, 4 )
				ubNumElites = 1 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 11 + math.random( 1, 3 )
				ubNumElites = 3 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 21 + math.random( 1, 3 )
				ubNumElites = 3 + math.random( 1, 2 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 12,10, ubNumAdmins, ubNumTroops, ubNumElites )

	-- Power Generator, Ground Level
	-- J13
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0 
				ubNumTroops = 15 + math.random( 1, 6 )
				ubNumElites = 1 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 16 + math.random( 1, 5 )
				ubNumElites = 5 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 18 + math.random( 1, 4 )
				ubNumElites = 6 + math.random( 1, 3 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 18 + math.random( 1, 4 )
				ubNumElites = 7 + math.random( 1, 3 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 13,10, ubNumAdmins, ubNumTroops, ubNumElites )

	-- Complex, Ground Level
	-- K15
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
				ubNumAdmins = 0 
				ubNumTroops = 14 + math.random( 1, 5 )
				ubNumElites = 0 
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
				ubNumAdmins = 0 
				ubNumTroops = 18 + math.random( 1, 3 )
				ubNumElites = 4 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
				ubNumAdmins = 0 
				ubNumTroops = 17 + math.random( 1, 3 )
				ubNumElites = 8 + math.random( 1, 2 )
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
				ubNumAdmins = 0 
				ubNumTroops = 17 + math.random( 1, 3 )
				ubNumElites = 8 + math.random( 1, 2 )
		end

		UB_SetNumberJa25EnemiesInSurfaceSector( 15,11, ubNumAdmins, ubNumTroops, ubNumElites )
end

	-- init hospital variables
	HospitalTempBalance( 0 )
	HospitalRefund( 0 )
	HospitalPriceModifier( Modifier.HOSPITAL_UNSET )

end

function InitNewGame()

  	if ( is_networked == 0 ) then

		--Add the initial email from enrico
		AddEmail( EMAIL_STARTGAME, EMAIL_STARTGAME_LENGTH, Sender.MAIL_ENRICO, -1 , -1 )

		--Add the initial email from AIM
		AddEmail( EMAIL_AIMDISCOUNT, EMAIL_AIMDISCOUNT_LENGTH, Sender.AIM_SITE, -1 , -1 )
		
		AddEmail(IMP_EMAIL_INTRO,IMP_EMAIL_INTRO_LENGTH,Sender.CHAR_PROFILE_SITE,  -1 , -1 )
		
	end	
	
		if ( newDIFFICULTY_LEVEL == DIF_LEVEL_EASY ) then
			iStartingCash = GetStartingCashNovice()
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_MEDIUM ) then
			iStartingCash = GetStartingCashExperienced()
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_HARD ) then	
			iStartingCash = GetStartingCashExpert()
		elseif ( newDIFFICULTY_LEVEL == DIF_LEVEL_INSANE ) then
			iStartingCash = GetStartingCashInsane()
		end
		
		AddTransactionToPlayersBook( Fincances.ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), iStartingCash )
end