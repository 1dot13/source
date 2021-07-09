#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "InIReader.h"
#else
	#include "types.h"
	#include "Game Events.h"
	#include "soundman.h"
	#include "environment.h"
	#include "Ambient Control.h"
	#include "Quests.h"
//	#include "Sound Control.h"
	#include "AimMembers.h"
	#include "Strategic Event Handler.h"
	#include "BobbyR.h"
	#include "mercs.h"
	#include "email.h"
	#include "Game Clock.h"
	#include "Merc Hiring.h"
	#include "Insurance Contract.h"
	#include "Strategic Merc Handler.h"
	#include "Strategic Movement.h"
	#include "Assignments.h"
	#include "Strategic Mines.h"
	#include "Strategic Town Loyalty.h"
	#include "Message.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Helicopter.h"
	#include "Scheduling.h"
	#include "Arms Dealer Init.h"
	#include "Strategic town reputation.h"
	#include "air raid.h"
	#include "meanwhile.h"
	#include "overhead.h"
	#include "random.h"
	#include "Creature Spreading.h"
	#include "Strategic AI.h"
	#include "Merc Contract.h"
	#include "Strategic Status.h"
	#include "INIReader.h"
	#include "GameSettings.h"
	#include "english.h"
	#include "Input.h"
	#include "Soldier Profile.h"
	#include "laptop.h"
	#include "Campaign.h"
	#include "PostalService.h"
	#include "MilitiaSquads.h"
	#include "PMC.h"			// added by Flugente
	#include "finances.h"		// added by Flugente
	#include "ASD.h"			// added by Flugente
	#include "Player Command.h"	// added by Flugente
	#include "LuaInitNPCs.h"	// added by Flugente
	#include "MiniEvents.h"
#endif

#include "connect.h"

#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "mercs.h"
#include "ub_config.h"
#endif


#ifdef JA2BETAVERSION
extern BOOLEAN gfMercsNeverQuit;
#endif

extern void ValidateGameEvents();
extern void HandleHourlyUpdate();
extern void HandleQuarterHourUpdate();
extern void HandleMinuteUpdate();
extern BOOLEAN gfProcessingGameEvents;
extern UINT32	guiTimeStampOfCurrentlyExecutingEvent;
extern BOOLEAN gfPreventDeletionOfAnyEvent;
extern CPostalService gPostalService;


#ifdef CRIPPLED_VERSION
void CrippledVersionEndGameCheckCallBack( UINT8 bExitValue );
void CrippledVersionEndGameCheck();
#endif



BOOLEAN DelayEventIfBattleInProgress( STRATEGICEVENT *pEvent )
{
	STRATEGICEVENT *pNewEvent;
	if( gTacticalStatus.fEnemyInSector )
	{
		pNewEvent = AddAdvancedStrategicEvent( pEvent->ubEventType, pEvent->ubCallbackID, pEvent->uiTimeStamp + 180 + Random( 121 ), pEvent->uiParam );
		Assert( pNewEvent );
		pNewEvent->uiTimeOffset = pEvent->uiTimeOffset;
		return TRUE;
	}
	return FALSE;
}

BOOLEAN ExecuteStrategicEvent( STRATEGICEVENT *pEvent )
{

	BOOLEAN bMercDayOne = FALSE;
// BF : file access is bad, especially if a function is called so often.
#if 0
	// Kaiden: Opening the INI File
	CIniReader iniReader("..\\Ja2_Options.ini");

	if(is_networked) memset(&iniReader, 0, sizeof (CIniReader) );//disable ini in mp (taking default values)

	//Kaiden: Getting Value for MERC Available on Day one?
	// for some reason, this can't be in gamesettings.cpp
	// or it won't work.
	bMercDayOne = iniReader.ReadBoolean("Options","MERC_DAY_ONE",FALSE);
#endif
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ExecuteStrategicEvent");

	if( gGameExternalOptions.gfEnableEmergencyButton_SkipStrategicEvents && _KeyDown( NUM_LOCK ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Strategic event skipped!" );
		return TRUE;
	}

	BOOLEAN fOrigPreventFlag;

	fOrigPreventFlag = gfPreventDeletionOfAnyEvent;
	gfPreventDeletionOfAnyEvent = TRUE;
	//No events can be posted before this time when gfProcessingGameEvents is set, otherwise,
	//we have a chance of running into an infinite loop.
	guiTimeStampOfCurrentlyExecutingEvent = pEvent->uiTimeStamp;

	if( pEvent->ubFlags & SEF_DELETION_PENDING)
	{
		gfPreventDeletionOfAnyEvent = fOrigPreventFlag;
		return FALSE;
	}

	#ifdef JA2BETAVERSION
		//If we are currently in the AIViewer, only process the events that we want to process.
		//The rest of the events will be delayed until AFTER we leave the viewer.
		if( guiCurrentScreen == AIVIEWER_SCREEN )
		{
			if( pEvent->ubCallbackID != EVENT_BEGIN_CREATURE_QUEST &&
					pEvent->ubCallbackID != EVENT_CREATURE_SPREAD &&
					pEvent->ubCallbackID != EVENT_DELAYED_HIRING_OF_MERC &&
					pEvent->ubCallbackID != EVENT_GROUP_ARRIVAL	&&
					pEvent->ubCallbackID != EVENT_EVALUATE_QUEEN_SITUATION &&
					pEvent->ubCallbackID != EVENT_CHECK_ENEMY_CONTROLLED_SECTOR	)
			{
				gfPreventDeletionOfAnyEvent = fOrigPreventFlag;
				return FALSE;
			}
		}
	#endif
	#ifdef JA2BETAVERSION
		if( gfMercsNeverQuit )
		{
			if( pEvent->ubCallbackID == EVENT_MERC_ABOUT_TO_LEAVE_COMMENT ||
				pEvent->ubCallbackID == EVENT_MERC_CONTRACT_OVER )
			{
				gfPreventDeletionOfAnyEvent = fOrigPreventFlag;
				return FALSE;
			}
		}
	#endif
	// Look at the ID of event and do stuff according to that!
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ExecuteStrategicEvent: event callback id = %d", pEvent->ubCallbackID));

	switch( pEvent->ubCallbackID )
	{
		case EVENT_CHANGELIGHTVAL:
			// Change light to value
			gubEnvLightValue = (UINT8)pEvent->uiParam;
			if( !gfBasement && !gfCaves )
				gfDoLighting		= TRUE;
			break;
		case EVENT_CHECKFORQUESTS:
			CheckForQuests( GetWorldDay() );
			break;
		case EVENT_AMBIENT:
			if( pEvent->ubEventType == ENDRANGED_EVENT )
			{
				if ( pEvent->uiParam != NO_SAMPLE )
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ExecuteStrategicEvent: SoundRemoveSampleFlags");
					SoundRemoveSampleFlags( pEvent->uiParam, SAMPLE_RANDOM );
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ExecuteStrategicEvent: SoundRemoveSampleFlags done");
				}
			}
			else
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ExecuteStrategicEvent: SetupNewAmbientSound");
				pEvent->uiParam = SetupNewAmbientSound( pEvent->uiParam ); //bug Nr14
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ExecuteStrategicEvent: SetupNewAmbientSound done");
			}
			break;
		case EVENT_AIM_RESET_MERC_ANNOYANCE:
			ResetMercAnnoyanceAtPlayer( (UINT8) pEvent->uiParam );		
			break;
		// WANNE: This is the EVENT for the old Bobby Ray shipment code. Look for the EVENT "EVENT_POSTAL_SERVICE_SHIPMENT" below
		/*
		//The players purchase from Bobby Ray has arrived
		case EVENT_BOBBYRAY_PURCHASE:
			BobbyRayPurchaseEventCallback( (UINT8) pEvent->uiParam);
			break;
		*/
		//Gets called once a day ( at BOBBYRAY_UPDATE_TIME).	To simulate the items being bought and sold at bobby rays
		case EVENT_DAILY_UPDATE_BOBBY_RAY_INVENTORY:
			DailyUpdateOfBobbyRaysNewInventory();
			DailyUpdateOfBobbyRaysUsedInventory();
			DailyUpdateOfArmsDealersInventory();
			break;
		//Add items to BobbyR's new/used inventory
		case EVENT_UPDATE_BOBBY_RAY_INVENTORY:
			AddFreshBobbyRayInventory( (UINT16) pEvent->uiParam );
			break;
		//Called once a day to update the number of days that a hired merc from M.E.R.C. has been on contract.
		// Also if the player hasn't paid for a while Specks will start sending e-mails to the player
		case EVENT_DAILY_UPDATE_OF_MERC_SITE:
			DailyUpdateOfMercSite( (UINT16)GetWorldDay() );
			break;
	case EVENT_DAY3_ADD_EMAIL_FROM_SPECK:
			// WANNE: This fixes the bug, that Speck did not sent the email on Day 3, when MERC_WEBSITE_ALL_MERCS_AVAILABLE = TRUE!
			if ( gGameExternalOptions.fMercDayOne == FALSE /* && gGameExternalOptions.fAllMercsAvailable == FALSE */ )
			{
			#ifdef JA2UB
			//No JA25 UB
			#else
				AddEmail(MERC_INTRO, MERC_INTRO_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin( ), -1, -1 ,TYPE_EMAIL_EMAIL_EDT );
			#endif	
			}
			break;
		case EVENT_DAY2_ADD_EMAIL_FROM_IMP:
			AddEmail(IMP_EMAIL_PROFILE_RESULTS, IMP_EMAIL_PROFILE_RESULTS_LENGTH, IMP_PROFILE_RESULTS, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT);
			break;
		//If a merc gets hired and they dont show up immediately, the merc gets added to the queue and shows up
		// uiTimeTillMercArrives	minutes later
		case EVENT_DELAYED_HIRING_OF_MERC:
			MercArrivesCallback(	(UINT8) pEvent->uiParam );
			break;
		//handles the life insurance contract for a merc from AIM.
		case EVENT_HANDLE_INSURED_MERCS:
			DailyUpdateOfInsuredMercs();
			break;
		//handles when a merc is killed an there is a life insurance payout
		case EVENT_PAY_LIFE_INSURANCE_FOR_DEAD_MERC:
			InsuranceContractPayLifeInsuranceForDeadMerc( (UINT8) pEvent->uiParam );
			break;
		//gets called every day at midnight.
		case EVENT_MERC_DAILY_UPDATE:
			MercDailyUpdate();
			break;
		//gets when a merc is about to leave.
		case EVENT_MERC_ABOUT_TO_LEAVE_COMMENT:
			break;
		// show the update menu
		case( EVENT_SHOW_UPDATE_MENU ):
			AddDisplayBoxToWaitingQueue( );
			break;
		case EVENT_MERC_ABOUT_TO_LEAVE:
			FindOutIfAnyMercAboutToLeaveIsGonnaRenew( );
			break;
		//When a merc is supposed to leave
		case EVENT_MERC_CONTRACT_OVER:
			MercsContractIsFinished( (UINT8) pEvent->uiParam );
			break;
		case EVENT_ADDSOLDIER_TO_UPDATE_BOX:
			// if the grunt is currently active, add to update box
			if( Menptr[ pEvent->uiParam ].bActive )
			{
				AddSoldierToWaitingListQueue( &( Menptr[ pEvent->uiParam ] ) );
			}
			break;
		case EVENT_SET_MENU_REASON:
			AddReasonToWaitingListQueue( (UINT8) pEvent->uiParam );
			break;
		//Whenever any group (player or enemy) arrives in a new sector during movement.
		case EVENT_GROUP_ARRIVAL:
			//ValidateGameEvents();
			GroupArrivedAtSector( (UINT8)pEvent->uiParam, TRUE, FALSE );
			//ValidateGameEvents();
			break;
		case EVENT_MERC_COMPLAIN_EQUIPMENT:
			MercComplainAboutEquipment( (UINT8) pEvent->uiParam );
			break;
		case EVENT_HOURLY_UPDATE:
			HandleHourlyUpdate();
			break;
		case EVENT_MINUTE_UPDATE:
			HandleMinuteUpdate();
			break;
		case EVENT_HANDLE_MINE_INCOME:
			HandleIncomeFromMines( );
			//ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Income From Mines at %d", GetWorldTotalMin( ) );
			break;
		case EVENT_SETUP_MINE_INCOME:
			PostEventsForMineProduction();
			break;
		case EVENT_SETUP_TOWN_OPINION:
			PostEventsForSpreadOfTownOpinion( );
			break;
		case EVENT_HANDLE_TOWN_OPINION:
			HandleSpreadOfAllTownsOpinion( );
			break;
		case EVENT_SET_BY_NPC_SYSTEM:
			HandleNPCSystemEvent( pEvent->uiParam );
			break;
		case EVENT_SECOND_AIRPORT_ATTENDANT_ARRIVED:
			AddSecondAirportAttendant();
			break;
		case EVENT_HELICOPTER_HOVER_TOO_LONG:
			HandleHeliHoverLong( );
			break;
		case EVENT_HELICOPTER_HOVER_WAY_TOO_LONG:
			HandleHeliHoverTooLong( );
			break;
		case EVENT_HELICOPTER_HOVER_FOR_A_MINUTE:
			HandleHeliHoverForAMinute( );
			break;
		case EVENT_HELICOPTER_REFUEL_FOR_A_MINUTE:
			ReFuelHelicopterForAMinute( );
			break;
		case EVENT_MERC_LEAVE_EQUIP_IN_DRASSEN:
			HandleEquipmentLeftInDrassen( pEvent->uiParam );
			break;
		case EVENT_MERC_LEAVE_EQUIP_IN_OMERTA:
			HandleEquipmentLeftInOmerta( pEvent->uiParam );
			break;
		case EVENT_BANDAGE_BLEEDING_MERCS:
			BandageBleedingDyingPatientsBeingTreated( );
			break;
		case EVENT_DAILY_EARLY_MORNING_EVENTS:
			HandleEarlyMorningEvents();
			break;
		case EVENT_GROUP_ABOUT_TO_ARRIVE:
			HandleGroupAboutToArrive();
			break;
		case EVENT_PROCESS_TACTICAL_SCHEDULE:
			ProcessTacticalSchedule( (UINT8)pEvent->uiParam );
			break;
		case EVENT_BEGINRAINSTORM:
		case EVENT_ENDRAINSTORM:
		case EVENT_RAINSTORM:
			break;

		case EVENT_MAKE_CIV_GROUP_HOSTILE_ON_NEXT_SECTOR_ENTRANCE:
			MakeCivGroupHostileOnNextSectorEntrance( (UINT8)pEvent->uiParam );
			break;
//		case EVENT_CHECK_FOR_AIR_RAID:
//			CheckForAndSetupAirRaid( );
//			break;
		//case EVENT_BEGIN_AIR_RAID:
		//	BeginAirRaid( );
		//	break;

#ifdef JA2UB
// Ja25 No meanwhiles
#else
		case EVENT_MEANWHILE:
			if( !DelayEventIfBattleInProgress( pEvent ) )
			{
				BeginMeanwhile( (UINT8)pEvent->uiParam );
				InterruptTime();
			}
			break;
#endif
		case EVENT_BEGIN_CREATURE_QUEST:
			break;
		case EVENT_CREATURE_SPREAD:
			SpreadCreatures();
			break;
		case EVENT_DECAY_CREATURES:
			DecayCreatures();
			break;
		case EVENT_CREATURE_NIGHT_PLANNING:
			CreatureNightPlanning();
			break;
		case EVENT_CREATURE_ATTACK:
			CreatureAttackTown( (UINT8)pEvent->uiParam, FALSE );
			break;
		case EVENT_EVALUATE_QUEEN_SITUATION:
			EvaluateQueenSituation();
			break;
		case EVENT_CHECK_ENEMY_CONTROLLED_SECTOR:
			CheckEnemyControlledSector( (UINT8)pEvent->uiParam );
			break;
		case EVENT_TURN_ON_NIGHT_LIGHTS:
			TurnOnNightLights();
			break;
		case EVENT_TURN_OFF_NIGHT_LIGHTS:
			TurnOffNightLights();
			break;
		case EVENT_TURN_ON_PRIME_LIGHTS:
			TurnOnPrimeLights();
			break;
		case EVENT_TURN_OFF_PRIME_LIGHTS:
			TurnOffPrimeLights();
			break;
		case EVENT_INTERRUPT_TIME:
			InterruptTime( );
			break;
		case EVENT_ENRICO_MAIL:
			HandleEnricoEmail();
			break;
		case EVENT_INSURANCE_INVESTIGATION_STARTED:
			StartInsuranceInvestigation( (UINT8) pEvent->uiParam );
			break;
		case EVENT_INSURANCE_INVESTIGATION_OVER:
			EndInsuranceInvestigation( (UINT8) pEvent->uiParam );
			break;
		case EVENT_TEMPERATURE_UPDATE:
			UpdateTemperature( (UINT8) pEvent->uiParam );
			break;
		case EVENT_KEITH_GOING_OUT_OF_BUSINESS:
			// make sure killbillies are still alive, if so, set fact 274 true
			if( CheckFact( FACT_HILLBILLIES_KILLED, KEITH ) == FALSE )
			{
				//s et the fact true keith is out of business
				SetFactTrue( FACT_KEITH_OUT_OF_BUSINESS );
			}
			break;
		case EVENT_MERC_SITE_BACK_ONLINE:
			#ifdef JA2UB
			// no JA25 UB
			#else
			GetMercSiteBackOnline();
			#endif
			break;
		case EVENT_INVESTIGATE_SECTOR:
			InvestigateSector( (UINT8)pEvent->uiParam );
			break;
		case EVENT_CHECK_IF_MINE_CLEARED:
			// If so, the head miner will say so, and the mine's shutdown will be ended.
			HourlyMinesUpdate();		// not-so hourly, in this case!
			break;
		case EVENT_REMOVE_ASSASSIN:
			RemoveAssassin( (UINT8) pEvent->uiParam );
			break;
		case EVENT_BEGIN_CONTRACT_RENEWAL_SEQUENCE:
			BeginContractRenewalSequence( );
			break;
		case EVENT_RPC_WHINE_ABOUT_PAY:
			RPCWhineAboutNoPay( (UINT8) pEvent->uiParam );
			break;

		case EVENT_HAVENT_MADE_IMP_CHARACTER_EMAIL:
			HaventMadeImpMercEmailCallBack();
			break;

		case EVENT_QUARTER_HOUR_UPDATE:
			HandleQuarterHourUpdate();
			break;

		case EVENT_MERC_MERC_WENT_UP_LEVEL_EMAIL_DELAY:
			MERCMercWentUpALevelSendEmail( (UINT8) pEvent->uiParam );
			break;

		case EVENT_MERC_SITE_NEW_MERC_AVAILABLE:
			NewMercsAvailableAtMercSiteCallBack( );
			break;
		case EVENT_POSTAL_SERVICE_SHIPMENT:
			gPostalService.DeliverShipment(pEvent->uiParam);
			break;
#ifdef JA2UB			
		//Ja25 UB	
		case EVENT_ATTACK_INITIAL_SECTOR_IF_PLAYER_STILL_THERE:
			if ( gGameUBOptions.EventAttackInitialSectorIfPlayerStillThere == TRUE )
			{
			ShouldEnemiesBeAddedToInitialSector();
			}
			break;

		case EVENT_SAY_DELAYED_MERC_QUOTE:
			DelayedSayingOfMercQuote( pEvent->uiParam );
			break;

		case EVENT_DELAY_SOMEONE_IN_SECTOR_MSGBOX:
			SetMsgBoxForPlayerBeNotifiedOfSomeoneElseInSector();
			break;

		case EVENT_SECTOR_H8_DONT_WAIT_IN_SECTOR:
			HandleSayingDontStayToLongWarningInSectorH8();
			break;

		case EVENT_SEND_ENRICO_UNDERSTANDING_EMAIL:
			HandleEnricosUnderstandingEmail();
			break;
#endif

#ifdef CRIPPLED_VERSION
		case EVENT_CRIPPLED_VERSION_END_GAME_CHECK:
			CrippledVersionEndGameCheck();
			break;
#endif
		case EVENT_MILITIA_MOVEMENT_ORDER:
			MilitiaMovementOrder( (UINT8) pEvent->uiParam );
			break;

#ifdef JA2UB
		// No PCM in UB
#else
		case EVENT_PMC_EMAIL:
			AddEmail( PMC_INTRO, PMC_INTRO_LENGTH, PMC, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			break;
#endif
		
		case EVENT_PMC_REINFORCEMENT_ARRIVAL:
			HandlePMCArrival( (UINT8)pEvent->uiParam );
			break;
#ifdef JA2UB
		// No Kingpin Events in UB
#else
		case EVENT_KINGPIN_BOUNTY_INITIAL:
			// if Kingpin, Angel and Maria are still alive, we can start the quest
			if ( gMercProfiles[KINGPIN].bMercStatus != MERC_IS_DEAD && !CheckFact( FACT_KINGPIN_DEAD, NO_PROFILE ) && !CheckFact( FACT_KINGPIN_IS_ENEMY, NO_PROFILE ) && 
				 gMercProfiles[MARIA].bMercStatus != MERC_IS_DEAD && gMercProfiles[ANGEL].bMercStatus != MERC_IS_DEAD )
			{
				StartQuest( QUEST_KINGPIN_ANGEL_MARIA, gWorldSectorX, gWorldSectorY );

				AddEmail( KINGPIN_BOUNTY_INITIAL, KINGPIN_BOUNTY_INITIAL_LENGTH, KING_PIN, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			}
			break;

		case EVENT_KINGPIN_BOUNTY_END_KILLEDTHEM:
			if ( gMercProfiles[KINGPIN].bMercStatus != MERC_IS_DEAD && !CheckFact( FACT_KINGPIN_DEAD, NO_PROFILE ) && !CheckFact( FACT_KINGPIN_IS_ENEMY, NO_PROFILE ) )
			{
				AddEmail( KINGPIN_BOUNTY_KINGPIN_REWARD, KINGPIN_BOUNTY_KINGPIN_REWARD_LENGTH, KING_PIN, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );

				// also authorise payment from kingpin to the player
				AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin( ), 10000 );
			}

			// in any case, this quest is now over
			EndQuest( QUEST_KINGPIN_ANGEL_MARIA, gWorldSectorX, gWorldSectorY );

			break;

		case EVENT_KINGPIN_BOUNTY_END_TIME_PASSED:

			// if we eliminated all bounty hunters and Angel & Maria are still alive, they send us an email
			if ( gMercProfiles[MARIA].bMercStatus != MERC_IS_DEAD && gMercProfiles[ANGEL].bMercStatus != MERC_IS_DEAD
				 && CheckFact( FACT_BOUNTYHUNTER_KILLED_1, NO_PROFILE ) && CheckFact( FACT_BOUNTYHUNTER_KILLED_2, NO_PROFILE ) )
			{
				AddEmail( KINGPIN_BOUNTY_ANGEL_THANKS, KINGPIN_BOUNTY_ANGEL_THANKS_LENGTH, ANGEL_DASILVA, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );

				AddEmail( KINGPIN_BOUNTY_TARGET_GOTAWAY, KINGPIN_BOUNTY_TARGET_GOTAWAY_LENGTH, KING_PIN, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			}
			// we can assume that the bounty hunters for to them first - Kingpin sends an email and states that someone else finished the job
			else if ( gMercProfiles[KINGPIN].bMercStatus != MERC_IS_DEAD && !CheckFact( FACT_KINGPIN_DEAD, NO_PROFILE ) && !CheckFact( FACT_KINGPIN_IS_ENEMY, NO_PROFILE ) )
			{
				AddEmail( KINGPIN_BOUNTY_BH_GOTTARGET, KINGPIN_BOUNTY_BH_GOTTARGET_LENGTH, KING_PIN, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			}

			// in any case, this quest is now over
			EndQuest( QUEST_KINGPIN_ANGEL_MARIA, gWorldSectorX, gWorldSectorY );

			break;
#endif

		case EVENT_ASD_UPDATE:
			UpdateASD();
			break;

		case EVENT_ASD_PURCHASE_FUEL:	
			ASDReduceOrderedStrategicResources( ASD_FUEL, pEvent->uiParam );

			if ( IsSectorEnemyControlled( gModSettings.usASDSupplyArrivalSectorX, gModSettings.usASDSupplyArrivalSectorY, 0 ) )
				AddStrategicAIResources( ASD_FUEL, pEvent->uiParam );
			break;

		case EVENT_ASD_PURCHASE_JEEP:
			ASDReduceOrderedStrategicResources( ASD_JEEP, pEvent->uiParam );

			if ( IsSectorEnemyControlled( gModSettings.usASDSupplyArrivalSectorX, gModSettings.usASDSupplyArrivalSectorY, 0 ) )
				AddStrategicAIResources( ASD_JEEP, pEvent->uiParam );
			break;

		case EVENT_ASD_PURCHASE_TANK:
			ASDReduceOrderedStrategicResources( ASD_TANK, pEvent->uiParam );

			if ( IsSectorEnemyControlled( gModSettings.usASDSupplyArrivalSectorX, gModSettings.usASDSupplyArrivalSectorY, 0 ) )
				AddStrategicAIResources( ASD_TANK, pEvent->uiParam );
			break;

		case EVENT_ASD_PURCHASE_HELI:
			ASDReduceOrderedStrategicResources( ASD_HELI, pEvent->uiParam );

			if ( IsSectorEnemyControlled( gModSettings.usEnemyHeliBaseSectorX, gModSettings.usEnemyHeliBaseSectorY, 0 ) )
				AddStrategicAIResources( ASD_HELI, pEvent->uiParam );
			break;

		case EVENT_ASD_PURCHASE_ROBOT:
			ASDReduceOrderedStrategicResources( ASD_ROBOT, pEvent->uiParam );

			if ( IsSectorEnemyControlled( gModSettings.usASDSupplyArrivalSectorX, gModSettings.usASDSupplyArrivalSectorY, 0 ) )
				AddStrategicAIResources( ASD_ROBOT, pEvent->uiParam );
			break;

		case EVENT_ENEMY_HELI_UPDATE:
			UpdateEnemyHeli( pEvent->uiParam );
			break;

		case EVENT_ENEMY_HELI_REPAIR:
			UpdateEnemyHeliRepair( pEvent->uiParam );
			break;

		case EVENT_ENEMY_HELI_REFUEL:
			UpdateEnemyHeliRefuel( pEvent->uiParam );
			break;

		case EVENT_SAMSITE_REPAIRED:
			RepairSamSite( pEvent->uiParam );
			break;

#ifndef JA2UB
		case EVENT_MILITIAROSTER_EMAIL:
			AddEmail( MILITIAROSTER_INTRO, MILITIAROSTER_INTRO_LENGTH, MAIL_ENRICO, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			break;
#endif

		// Flugente: localized weather
		case EVENT_WEATHER_NORMAL:
			ChangeWeather( (UINT8)pEvent->uiParam, WEATHER_FORECAST_NORMAL );
			break;

		case EVENT_WEATHER_SHOWERS:
			ChangeWeather( (UINT8)pEvent->uiParam, WEATHER_FORECAST_RAIN );
			break;

		case EVENT_WEATHER_THUNDERSHOWERS:
			ChangeWeather( (UINT8)pEvent->uiParam, WEATHER_FORECAST_THUNDERSHOWERS );
			break;

		case EVENT_WEATHER_SANDSTORM:
			ChangeWeather( (UINT8)pEvent->uiParam, WEATHER_FORECAST_SANDSTORM );
			break;

		case EVENT_WEATHER_SNOW:
			ChangeWeather( (UINT8)pEvent->uiParam, WEATHER_FORECAST_SNOW );
			break;

#ifndef JA2UB
		case EVENT_INTEL_ENRICO_EMAIL:
			AddEmail( INTEL_ENRICO_INTRO, INTEL_ENRICO_INTRO_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			break;
#endif

		case EVENT_INTEL_PHOTOFACT_VERIFY:
			LuaVerifyPhotoState( (INT16)pEvent->uiParam );
			break;

		case EVENT_DAILY_RAID_EVENTS:
			HandleRaidEventPlanning();
			break;

		case EVENT_BLOODCAT_ATTACK:
			CreatureAttackTown_OtherCreatures( (UINT8)pEvent->uiParam, CREATURE_ATTACK_TYPE_BLOODCAT );
			break;

		case EVENT_ZOMBIE_ATTACK:
			CreatureAttackTown_OtherCreatures( (UINT8)pEvent->uiParam, CREATURE_ATTACK_TYPE_ZOMBIE );
			break;

		case EVENT_BANDIT_ATTACK:
			CreatureAttackTown_OtherCreatures( (UINT8)pEvent->uiParam, CREATURE_ATTACK_TYPE_BANDIT );
			break;

		case EVENT_ARMY_FINISH_TRAINING:
			giReinforcementPool += (INT32)pEvent->uiParam;
			giTotalRecruitsInTraining -= (INT32)pEvent->uiParam;
			break;

		case EVENT_MINIEVENT:
			if (!DelayEventIfBattleInProgress(pEvent))
			{
				CheckMiniEvents(pEvent->uiParam);
			}
			break;
	}
	gfPreventDeletionOfAnyEvent = fOrigPreventFlag;
	return TRUE;
}

#ifdef CRIPPLED_VERSION
void CrippledVersionEndGameCheck()
{
	CHAR16	zString[512];

	//Dont want this to appear before we arrive
	if( guiDay == 1 )
		return;

	if( guiDay >= 8 )
	{
		swprintf( zString, L"Game Over.	We hope you have enjoyed playing the limited version of Jagged Alliance 2 v1.13." );
	}
	else
	{
		swprintf( zString, L"You have %d game days left in this limited version of Jagged Alliance 2 v1.13.",	( 8 - guiDay ) );
	}

	DoScreenIndependantMessageBox( zString, MSG_BOX_FLAG_OK, CrippledVersionEndGameCheckCallBack );
}

void CrippledVersionEndGameCheckCallBack( UINT8 bExitValue )
{
	//if we should restart the game
	if( guiDay >= 8 )
	{
		//clean up the code
		ReStartingGame();

		//go to the main menu
		if( guiCurrentScreen == MAP_SCREEN )
		{
			SetPendingNewScreen(MAINMENU_SCREEN);
		}
		else
		{
			InternalLeaveTacticalScreen( MAINMENU_SCREEN );
		}
	}
}

#endif



