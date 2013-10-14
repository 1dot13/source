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

-- max number of lines can be shown in a message
local MAX_EMAIL_LINES = 10
-- max number of messages per page	
local MAX_MESSAGES_PAGE = 18 	

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

-- merc left-me-a-message-and-now-I'm-back emails
local AIM_REPLY_BARRY = ( MERC_UP_LEVEL_LENGTH_BUBBA + MERC_UP_LEVEL_BUBBA )
local AIM_REPLY_LENGTH_BARRY = 2
local AIM_REPLY_MELTDOWN = (AIM_REPLY_BARRY + ( 39 * AIM_REPLY_LENGTH_BARRY ))
local AIM_REPLY_LENGTH_MELTDOWN	 = AIM_REPLY_LENGTH_BARRY

-- old EXISTING emails when player starts game. They must look "read"
local OLD_ENRICO_1 = ( AIM_REPLY_LENGTH_MELTDOWN + AIM_REPLY_MELTDOWN )
local OLD_ENRICO_1_LENGTH = 3
local OLD_ENRICO_2 = ( OLD_ENRICO_1 + OLD_ENRICO_1_LENGTH )
local OLD_ENRICO_2_LENGTH = 3
local RIS_REPORT = ( OLD_ENRICO_2 + OLD_ENRICO_2_LENGTH )
local RIS_REPORT_LENGTH	 = 2
local OLD_ENRICO_3 = ( RIS_REPORT + RIS_REPORT_LENGTH )
local OLD_ENRICO_3_LENGTH = 3

-- emails that occur from Enrico once player accomplishes things
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

-- insurance company emails
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

-- A package from Bobby Ray has arrived in Drassen
local BOBBYR_SHIPMENT_ARRIVED = ( LACK_PLAYER_PROGRESS_3 + LACK_PLAYER_PROGRESS_3_LENGTH )
local BOBBYR_SHIPMENT_ARRIVED_LENGTH = 4

-- John Kulba has left the gifts for the players in Drassen
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
	BARRY_MAIL = 6,
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
	Skyrider = 97,
	Micky = 96,
	Gabby = 104,
	Bob = 84,
	Devin = 61,
}

local DIF_LEVEL_EASY = 1
local DIF_LEVEL_MEDIUM = 2
local DIF_LEVEL_HARD = 3
local DIF_LEVEL_INSANE = 4

function InitNPCs()

	-- Add the pilot at a random location!
	if is_networked == 0 then
		o = math.random(1, 4)
		if o == 1 then
			-- B15 - Skyrider
			AddNPCtoSector( Profil.Skyrider, 15, SectorY.MAP_ROW_B, 0 )
			AddAltSectorNew( 15, SectorY.MAP_ROW_B )
		elseif o == 2 then
			-- E14 - Skyrider
			AddNPCtoSector( Profil.Skyrider, 14, SectorY.MAP_ROW_E, 0 )
			AddAltSectorNew( 14, SectorY.MAP_ROW_E )
		elseif o == 3 then	
			-- D12 - Skyrider
			AddNPCtoSector( Profil.Skyrider, 12, SectorY.MAP_ROW_D, 0 )
			AddAltSectorNew( 12,SectorY.MAP_ROW_D )
		elseif o == 4 then
		-- C16 - Skyrider
			AddNPCtoSector( Profil.Skyrider, 16, SectorY.MAP_ROW_C, 0 )
			AddAltSectorNew( 16, SectorY.MAP_ROW_C )
		end
	end

	if is_networked == 1 then
			-- B15 - Skyrider
			AddNPCtoSector( Profil.Skyrider, 15, SectorY.MAP_ROW_B, 0 )
			AddAltSectorNew( 15, SectorY.MAP_ROW_B )
	end

	-- Set up Madlab's secret lab (he'll be added when the meanwhile scene occurs)
	-- Use alternate map in this sector
	if is_networked == 0 then
		i = math.random(1, 4)
		if i == 1 then
			--H7
			AddAltSectorNew( iniADD_MADLAB_SECTOR_1_X, iniADD_MADLAB_SECTOR_1_Y )
		elseif i == 2 then
			--H16
			AddAltSectorNew( iniADD_MADLAB_SECTOR_2_X, iniADD_MADLAB_SECTOR_2_Y )
		elseif i == 3 then	
			--I11
			AddAltSectorNew( iniADD_MADLAB_SECTOR_3_X, iniADD_MADLAB_SECTOR_3_Y )
		elseif i == 4 then
			--E4
			AddAltSectorNew( iniADD_MADLAB_SECTOR_4_X, iniADD_MADLAB_SECTOR_4_Y )
		end
	end
	
	if is_networked == 1 then
		--H7
		AddAltSectorNew( iniADD_MADLAB_SECTOR_1_X, iniADD_MADLAB_SECTOR_1_Y )
	end

	-- Add Micky in random location
	if is_networked == 0 then
		i = math.random(1, 5)
		if i == 1 then
			-- G9
			AddNPCtoSector( Profil.Micky, 9, SectorY.MAP_ROW_G, 0 ) 
		elseif i == 2 then
			-- D14
			AddNPCtoSector( Profil.Micky, 13, SectorY.MAP_ROW_D, 0 ) 
		elseif i == 3 then	
			-- C5
			AddNPCtoSector( Profil.Micky, 5, SectorY.MAP_ROW_C, 0 )
		elseif i == 4 then
			-- H2
			AddNPCtoSector( Profil.Micky, 2, SectorY.MAP_ROW_H, 0 )
		elseif i == 5 then
			-- C6
			AddNPCtoSector( Profil.Micky, 6, SectorY.MAP_ROW_C, 0 )
		end
	end

	if is_networked == 1 then
		-- G9
		AddNPCtoSector( Profil.Micky, 9, SectorY.MAP_ROW_G, 0 )
	end

	PlayerTeamSawJoey(false)

	-- Add Bob
	if 	(newGAME_STYLE == 1 and iniENABLE_CREPITUS == 1) then
		-- F8
		AddNPCtoSector( Profil.Bob, 8, SectorY.MAP_ROW_F, 0 )
	end

	-- Add Gabby in random location
	if 	(newGAME_STYLE == 1 and iniENABLE_CREPITUS == 1) then	
		i = math.random(1, 2)
		
		if i == 1 then
			-- H11
			AddNPCtoSector( Profil.Gabby, 11, SectorY.MAP_ROW_H, 0 )
			AddAltSectorNew( 11, SectorY.MAP_ROW_H )
		elseif i == 2 then
			-- I4
			AddNPCtoSector( Profil.Gabby, 4, SectorY.MAP_ROW_I, 0 )
			AddAltSectorNew( 4, SectorY.MAP_ROW_I )	
		end
	end
	
	-- Not SciFi, so use alternate map in Tixa's B1 level that doesn't have the stairs going down to the caves.
	if 	(newGAME_STYLE == 1 and iniENABLE_CREPITUS == 1) then	
	else
		--J9-1
		AddAltUGSectorNew( 9, SectorY.MAP_ROW_J, 1 )
	end

	-- Init hospital variables
	HospitalTempBalance( 0 )
	HospitalRefund( 0 )
	HospitalPriceModifier( Modifier.HOSPITAL_UNSET )

	-- Set up Devin so he will be placed
	SetNPCData1 ( Profil.Devin, 3 )
end

function InitNewGame()
  	
	  if ( is_networked == 0 ) then
		-- Setup two new messages!
		AddPreReadEmail(OLD_ENRICO_1,OLD_ENRICO_1_LENGTH,Sender.MAIL_ENRICO)
		AddPreReadEmail(OLD_ENRICO_2,OLD_ENRICO_2_LENGTH,Sender.MAIL_ENRICO)
		AddPreReadEmail(RIS_REPORT,RIS_REPORT_LENGTH,Sender.RIS_EMAIL)
		AddPreReadEmail(OLD_ENRICO_3,OLD_ENRICO_3_LENGTH,Sender.MAIL_ENRICO )

		AddEmail (IMP_EMAIL_INTRO, IMP_EMAIL_INTRO_LENGTH, Sender.CHAR_PROFILE_SITE, -1, -1)
		
		if( iniMERC_WEBSITE_IMMEDIATELY_AVAILABLE == true ) then
			AddEmail (MERC_INTRO, MERC_INTRO_LENGTH, Sender.SPECK_FROM_MERC, -1, -1)
		end
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