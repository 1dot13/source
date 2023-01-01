	#include "builddefines.h"
	#include <stdio.h>
	#include "types.h"
	#include "Game Events.h"
	#include "Game Clock.h"
	#include "MemMan.h"
	#include "Debug.h"
	#include "Font Control.h"
	#include "message.h"
	#include "MiniEvents.h"
	#include "Text.h"

#ifdef JA2TESTVERSION

CHAR16 gEventName[NUMBER_OF_EVENT_TYPES_PLUS_ONE][40]={
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"Null",
	L"ChangeLightValue",
	L"WeatherStart",
	L"WeatherEnd",
	L"CheckForQuests",
	L"Ambient",
	L"AIMResetMercAnnoyance",
	L"BobbyRayPurchase",
	L"DailyUpdateBobbyRayInventory",
	L"UpdateBobbyRayInventory",
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"DailyUpdateOfMercSite",
	L"Day3AddEMailFromSpeck",
	L"DelayedHiringOfMerc",
	L"HandleInsuredMercs",
	L"PayLifeInsuranceForDeadMerc",
	L"MercDailyUpdate",
	L"MercAboutToLeaveComment",
	L"MercContractOver",
	L"GroupArrival",
	L"Day2AddEMailFromIMP",
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"MercComplainEquipment",
	L"HourlyUpdate",
	L"HandleMineIncome",
	L"SetupMineIncome",
	L"QueuedBattle",
	L"LeavingMercArriveInDrassen",
	L"LeavingMercArriveInOmerta",
	L"SetByNPCSystem",
	L"SecondAirportAttendantArrived",
	L"HelicopterHoverTooLong",
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"HelicopterHoverWayTooLong",
	L"HelicopterDoneRefuelling",
	L"MercLeaveEquipInOmerta",
	L"MercLeaveEquipInDrassen",
	L"DailyEarlyMorningEvents",
	L"GroupAboutToArrive",
	L"ProcessTacticalSchedule",
	L"BeginRainStorm",
	L"EndRainStorm",
	L"HandleTownOpinion",
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"SetupTownOpinion",
	L"DelayedDeathHandling",
	L"BeginAirRaid",
	L"TownLoyaltyUpdate",
	L"Meanwhile",
	L"BeginCreatureQuest",
	L"CreatureSpread",
	L"DecayCreatures",
	L"CreatureNightPlanning",
	L"CreatureAttack",
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"EvaluateQueenSituation",
	L"CheckEnemyControlledSector",
	L"TurnOnNightLights",
	L"TurnOffNightLights",
	L"TurnOnPrimeLights",
	L"TurnOffPrimeLights",
	L"MercAboutToLeaveComment",
	L"ForceTimeInterupt",
	L"EnricoEmailEvent",
	L"InsuranceInvestigationStarted",
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"InsuranceInvestigationOver",
	L"HandleMinuteUpdate",
	L"TemperatureUpdate",
	L"Keith going out of business",
	L"MERC site back online",
	L"Investigate Sector",
	L"CheckIfMineCleared",
	L"RemoveAssassin",
	L"BandageBleedingMercs",
	L"ShowUpdateMenu",
	//1234567890123456789012345678901234567890 (increase size of array if necessary)
	L"SetMenuReason",
	L"AddSoldierToUpdateBox",
	L"BeginContractRenewalSequence",
	L"RPC_WHINE_ABOUT_PAY",
	L"HaventMadeImpCharacterEmail",
	L"Rainstorm",
	L"Quarter Hour Update",
	L"MERC Merc went up level email delay",
	L"CPostalService delivery",
	L".",
#ifdef CRIPPLED_VERSION
	L"Crippled version end game check",
#endif
	L"HelicopterHoverForAMinute",
	L"HelicopterRefuelForAMinute",
	L"MilitiaMovementOrder",
	L"PMCEmail",
	L"PMCReinforcementArrival",
	L"KingpinBounty1",
	L"KingpinBounty2",
	L"KingpinBounty3",
	L"ASDUpdate",
	L"ASDPurchaseFuel",
	L"ASDPurchaseJeep",
	L"ASDPurchaseTank",
	L"ASDPurchaseHeli",
	L"ASDPurchaseRobot",
	L"EnemyHeliUpdate",
	L"EnemyHeliRepair",
	L"EnemyHeliRefuel",
	L"SAMsiteRepaired",
	L"MilitiaWebsiteEmail",
	L"Weather Normal",
	L"Weather Rain",
	L"Weather Thunderstorm",
	L"Weather Sandstorm",
	L"Weather Snow",
	L"Intel Enrico Email",
	L"Intel Photofact verify",
	L"Daily raid events",
	L"bloodcat attack",
	L"zombie attack",
	L"bandit attack",
	L"ArmyFinishTraining",
	L"MiniEvent",
	L"ARC_Event",
};

#endif

void ValidateGameEvents();

STRATEGICEVENT									*gpEventList = NULL;

extern UINT32 guiGameClock;
extern BOOLEAN gfTimeInterruptPause;
BOOLEAN gfPreventDeletionOfAnyEvent = FALSE;
BOOLEAN gfEventDeletionPending = FALSE;

BOOLEAN gfProcessingGameEvents = FALSE;
UINT32	guiTimeStampOfCurrentlyExecutingEvent = 0;

//Determines if there are any events that will be processed between the current global time,
//and the beginning of the next global time.
BOOLEAN GameEventsPending( UINT32 uiAdjustment )
{
	#ifdef CRIPPLED_VERSION
	if( guiDay >= 8 )
	{
		return FALSE;
	}
	#endif
	if( !gpEventList )
		return FALSE;
	if( gpEventList->uiTimeStamp <= GetWorldTotalSeconds() + uiAdjustment )
		return TRUE;
	return FALSE;
}

//returns TRUE if any events were deleted
BOOLEAN DeleteEventsWithDeletionPending()
{
	STRATEGICEVENT *curr, *prev, *temp;
	BOOLEAN fEventDeleted = FALSE;
	//ValidateGameEvents();
	curr = gpEventList;
	prev = NULL;
	while( curr )
	{
		//ValidateGameEvents();
		if( curr->ubFlags & SEF_DELETION_PENDING )
		{
			if( prev )
			{ //deleting node in middle
				prev->next = curr->next;
				temp = curr;
				curr = curr->next;
				MemFree( temp );
				fEventDeleted = TRUE;
				//ValidateGameEvents();
				continue;
			}
			else
			{ //deleting head
				gpEventList = gpEventList->next;
				temp = curr;
				prev = NULL;
				curr = curr->next;
				MemFree( temp );
				fEventDeleted = TRUE;
				//ValidateGameEvents();
				continue;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	gfEventDeletionPending = FALSE;
	return fEventDeleted;
}


void AdjustClockToEventStamp( STRATEGICEVENT *pEvent, UINT32 *puiAdjustment )
{
	UINT32 uiDiff;

	uiDiff = pEvent->uiTimeStamp - guiGameClock;
	guiGameClock += uiDiff;
	*puiAdjustment -= uiDiff;

	//Calculate the day, hour, and minutes.
	guiDay = ( guiGameClock / NUM_SEC_IN_DAY );
	guiHour = ( guiGameClock - ( guiDay * NUM_SEC_IN_DAY ) ) / NUM_SEC_IN_HOUR;
	guiMin	= ( guiGameClock - ( ( guiDay * NUM_SEC_IN_DAY ) + ( guiHour * NUM_SEC_IN_HOUR ) ) ) / NUM_SEC_IN_MIN;

	#ifdef CRIPPLED_VERSION
	if( guiDay >= 8 )
	{
		guiDay = 8;
		guiHour = 0;
		guiMin = 0;
		return;
	}

	#endif

	swprintf( WORLDTIMESTR, L"%s %d, %02d:%02d", gpGameClockString[ STR_GAMECLOCK_DAY_NAME ], guiDay, guiHour, guiMin );
}

//If there are any events pending, they are processed, until the time limit is reached, or
//a major event is processed (one that requires the player's attention).
void ProcessPendingGameEvents( UINT32 uiAdjustment, UINT8 ubWarpCode )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ProcessPendingGameEvents");
	STRATEGICEVENT *curr, *pEvent, *prev, *temp;
	BOOLEAN fDeleteEvent = FALSE, fDeleteQueuedEvent = FALSE;

	#ifdef CRIPPLED_VERSION
	if( guiDay >= 8 )
	{
		return;
	}
	#endif

	gfTimeInterrupt = FALSE;
	gfProcessingGameEvents = TRUE;

	//While we have events inside the time range to be updated, process them...
	curr = gpEventList;
	prev = NULL; //prev only used when warping time to target time.
	while( !gfTimeInterrupt && curr && curr->uiTimeStamp <= guiGameClock + uiAdjustment )
	{
		fDeleteEvent = FALSE;
		//Update the time by the difference, but ONLY if the event comes after the current time.
		//In the beginning of the game, series of events are created that are placed in the list
		//BEFORE the start time.	Those events will be processed without influencing the actual time.
		if( curr->uiTimeStamp > guiGameClock && ubWarpCode != WARPTIME_PROCESS_TARGET_TIME_FIRST )
		{
			AdjustClockToEventStamp( curr, &uiAdjustment );
		}
		//Process the event
		if( ubWarpCode != WARPTIME_PROCESS_TARGET_TIME_FIRST )
		{
			fDeleteEvent = ExecuteStrategicEvent( curr );
		}
		else if( curr->uiTimeStamp == guiGameClock + uiAdjustment )
		{ //if we are warping to the target time to process that event first,
			if( !curr->next || curr->next->uiTimeStamp > guiGameClock + uiAdjustment )
			{ //make sure that we are processing the last event for that second
				AdjustClockToEventStamp( curr, &uiAdjustment );

				fDeleteEvent = ExecuteStrategicEvent( curr );

				if( curr && prev && fDeleteQueuedEvent )
				{ //The only case where we are deleting a node in the middle of the list
					prev->next = curr->next;
				}
			}
			else
			{ //We are at the current target warp time however, there are still other events following in this time cycle.
				//We will only target the final event in this time.	NOTE:	Events are posted using a FIFO method
				prev = curr;
				curr = curr->next;
				continue;
			}
		}
		else
		{ //We are warping time to the target time.	We haven't found the event yet,
			//so continuing will keep processing the list until we find it.	NOTE:	Events are posted using a FIFO method
			prev = curr;
			curr = curr->next;
			continue;
		}
		if( fDeleteEvent )
		{
			//Determine if event node is a special event requiring reposting
			switch( curr->ubEventType )
			{
				case RANGED_EVENT:
					AddAdvancedStrategicEvent( ENDRANGED_EVENT, curr->ubCallbackID, curr->uiTimeStamp+curr->uiTimeOffset, curr->uiParam );
					break;
				case PERIODIC_EVENT:
					pEvent = AddAdvancedStrategicEvent( PERIODIC_EVENT, curr->ubCallbackID, curr->uiTimeStamp+curr->uiTimeOffset, curr->uiParam );
					if( pEvent )
						pEvent->uiTimeOffset = curr->uiTimeOffset;
					break;
				case EVERYDAY_EVENT:
					AddAdvancedStrategicEvent( EVERYDAY_EVENT, curr->ubCallbackID, curr->uiTimeStamp+NUM_SEC_IN_DAY, curr->uiParam );
					break;
			}
			if( curr == gpEventList )
			{
				gpEventList = gpEventList->next;
				MemFree( curr );
				curr = gpEventList;
				prev = NULL;
				//ValidateGameEvents();
			}
			else
			{
				temp = curr;
				prev->next = curr->next;
				curr = curr->next;
				MemFree( temp );
				//ValidateGameEvents();
			}
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}

	gfProcessingGameEvents = FALSE;

	if( gfEventDeletionPending )
	{
		DeleteEventsWithDeletionPending();
	}

	if( uiAdjustment && !gfTimeInterrupt )
		guiGameClock += uiAdjustment;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ProcessPendingGameEvents done");
}


BOOLEAN AddSameDayStrategicEvent( UINT8 ubCallbackID, UINT32 uiMinStamp, UINT32 uiParam )
{
	return( AddStrategicEvent( ubCallbackID, uiMinStamp + GetWorldDayInMinutes(), uiParam ) );
}

BOOLEAN AddSameDayStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiSecondStamp, UINT32 uiParam )
{
	return( AddStrategicEventUsingSeconds( ubCallbackID, uiSecondStamp + GetWorldDayInSeconds(), uiParam ) );
}

BOOLEAN AddFutureDayStrategicEvent( UINT8 ubCallbackID, UINT32 uiMinStamp, UINT32 uiParam, UINT32 uiNumDaysFromPresent )
{
	UINT32 uiDay;
	uiDay = GetWorldDay();
	return( AddStrategicEvent( ubCallbackID, uiMinStamp + GetFutureDayInMinutes( uiDay + uiNumDaysFromPresent ), uiParam ) );
}

BOOLEAN AddFutureDayStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiSecondStamp, UINT32 uiParam, UINT32 uiNumDaysFromPresent )
{
	UINT32 uiDay;
	uiDay = GetWorldDay();
	return( AddStrategicEventUsingSeconds( ubCallbackID, uiSecondStamp + GetFutureDayInMinutes( uiDay + uiNumDaysFromPresent ) * 60, uiParam ) );
}

STRATEGICEVENT* AddAdvancedStrategicEvent( UINT8 ubEventType, UINT8 ubCallbackID, UINT32 uiTimeStamp, UINT32 uiParam )
{
	STRATEGICEVENT		*pNode, *pNewNode, *pPrevNode;

	if( gfProcessingGameEvents && uiTimeStamp <= guiTimeStampOfCurrentlyExecutingEvent )
	{ //Prevents infinite loops of posting events that are the same time or earlier than the event
		//currently being processed.
		#ifdef JA2TESTVERSION
			//if( ubCallbackID == EVENT_PROCESS_TACTICAL_SCHEDULE )
			{
				ScreenMsg( FONT_RED, MSG_DEBUG, L"%s Event Rejected:	Can't post events <= time while inside an event callback.	This is a special case situation that isn't a bug.", gEventName[ ubCallbackID ] );
			}
			//else
			//{
			//	AssertMsg( 0, String( "%S Event Rejected:	Can't post events <= time while inside an event callback.", gEventName[ ubCallbackID ] ) );
			//}
		#endif
		return NULL;
	}

	pNewNode = (STRATEGICEVENT *) MemAlloc( sizeof( STRATEGICEVENT ) );
	Assert( pNewNode );
	memset( pNewNode, 0, sizeof( STRATEGICEVENT ) );
	pNewNode->ubCallbackID		= ubCallbackID;
	pNewNode->uiParam					= uiParam;
	pNewNode->ubEventType			= ubEventType;
	pNewNode->uiTimeStamp			= uiTimeStamp;
	pNewNode->uiTimeOffset			= 0;

	// Search list for a place to insert
	pNode = gpEventList;

	// If it's the first head, do this!
	if( !pNode )
	{
		gpEventList = pNewNode;
		pNewNode->next = NULL;
	}
	else
	{
		pPrevNode = NULL;
		while( pNode )
		{
			if( uiTimeStamp < pNode->uiTimeStamp )
			{
				break;
			}
			pPrevNode = pNode;
			pNode = pNode->next;
		}

		// If we are at the end, set at the end!
		if ( !pNode )
		{
			pPrevNode->next = pNewNode;
			pNewNode->next	= NULL;
		}
		else
		{
			// We have a previous node here
			// Insert IN FRONT!
			if ( pPrevNode )
			{
				pNewNode->next = pPrevNode->next;
				pPrevNode->next = pNewNode;
			}
			else
			{	// It's the head
				pNewNode->next = gpEventList;
				gpEventList = pNewNode;
			}
		}
	}

	return pNewNode ;
}

BOOLEAN AddStrategicEvent( UINT8 ubCallbackID, UINT32 uiMinStamp, UINT32 uiParam )
{
	if( AddAdvancedStrategicEvent( ONETIME_EVENT, ubCallbackID, uiMinStamp*60, uiParam ) )
		return TRUE;
	return FALSE;
}

BOOLEAN AddStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiSecondStamp, UINT32 uiParam )
{
	if( AddAdvancedStrategicEvent( ONETIME_EVENT, ubCallbackID, uiSecondStamp, uiParam ) )
		return TRUE;
	return FALSE;
}


BOOLEAN AddRangedStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiLengthMin, UINT32 uiParam )
{
	STRATEGICEVENT *pEvent;
	pEvent = AddAdvancedStrategicEvent( RANGED_EVENT, ubCallbackID, uiStartMin*60, uiParam );
	if( pEvent )
	{
		pEvent->uiTimeOffset = uiLengthMin * 60;
		return TRUE;
	}
	return FALSE;
}

BOOLEAN AddSameDayRangedStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiLengthMin, UINT32 uiParam)
{
	return AddRangedStrategicEvent( ubCallbackID, uiStartMin + GetWorldDayInMinutes(), uiLengthMin, uiParam );
}

BOOLEAN AddFutureDayRangedStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiLengthMin, UINT32 uiParam, UINT32 uiNumDaysFromPresent )
{
	return AddRangedStrategicEvent( ubCallbackID, uiStartMin + GetFutureDayInMinutes( GetWorldDay() + uiNumDaysFromPresent ), uiLengthMin, uiParam );
}

BOOLEAN AddRangedStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiLengthSeconds, UINT32 uiParam )
{
	STRATEGICEVENT *pEvent;
	pEvent = AddAdvancedStrategicEvent( RANGED_EVENT, ubCallbackID, uiStartSeconds, uiParam );
	if( pEvent )
	{
		pEvent->uiTimeOffset = uiLengthSeconds;
		return TRUE;
	}
	return FALSE;
}

BOOLEAN AddSameDayRangedStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiLengthSeconds, UINT32 uiParam)
{
	return AddRangedStrategicEventUsingSeconds( ubCallbackID, uiStartSeconds + GetWorldDayInSeconds(), uiLengthSeconds, uiParam );
}

BOOLEAN AddFutureDayRangedStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiLengthSeconds, UINT32 uiParam, UINT32 uiNumDaysFromPresent )
{
	return AddRangedStrategicEventUsingSeconds( ubCallbackID, uiStartSeconds + GetFutureDayInMinutes( GetWorldDay() + uiNumDaysFromPresent ) * 60, uiLengthSeconds, uiParam );
}

BOOLEAN AddEveryDayStrategicEvent( UINT8 ubCallbackID, UINT32 uiStartMin, UINT32 uiParam )
{
	if( AddAdvancedStrategicEvent( EVERYDAY_EVENT, ubCallbackID, GetWorldDayInSeconds() + uiStartMin * 60, uiParam ) )
		return TRUE;
	return FALSE;
}

BOOLEAN AddEveryDayStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiStartSeconds, UINT32 uiParam )
{
	if( AddAdvancedStrategicEvent( EVERYDAY_EVENT, ubCallbackID, GetWorldDayInSeconds() + uiStartSeconds, uiParam ) )
		return TRUE;
	return FALSE;
}

//NEW:	Period Events
//Event will get processed automatically once every X minutes.
BOOLEAN AddPeriodStrategicEvent( UINT8 ubCallbackID, UINT32 uiOnceEveryXMinutes, UINT32 uiParam )
{
	STRATEGICEVENT *pEvent;
	pEvent = AddAdvancedStrategicEvent( PERIODIC_EVENT, ubCallbackID, GetWorldDayInSeconds() + uiOnceEveryXMinutes * 60, uiParam );
	if( pEvent )
	{
		pEvent->uiTimeOffset = uiOnceEveryXMinutes * 60;
		return TRUE;
	}
	return FALSE;
}

BOOLEAN AddPeriodStrategicEventUsingSeconds( UINT8 ubCallbackID, UINT32 uiOnceEveryXSeconds, UINT32 uiParam )
{
	STRATEGICEVENT *pEvent;
	pEvent = AddAdvancedStrategicEvent( PERIODIC_EVENT, ubCallbackID, GetWorldDayInSeconds() + uiOnceEveryXSeconds, uiParam );
	if( pEvent )
	{
		pEvent->uiTimeOffset = uiOnceEveryXSeconds;
		return TRUE;
	}
	return FALSE;
}

BOOLEAN AddPeriodStrategicEventWithOffset( UINT8 ubCallbackID, UINT32 uiOnceEveryXMinutes, UINT32 uiOffsetFromCurrent, UINT32 uiParam )
{
	STRATEGICEVENT *pEvent;
	pEvent = AddAdvancedStrategicEvent( PERIODIC_EVENT, ubCallbackID, GetWorldDayInSeconds() + uiOffsetFromCurrent * 60, uiParam );
	if( pEvent )
	{
		pEvent->uiTimeOffset = uiOnceEveryXMinutes * 60;
		return TRUE;
	}
	return FALSE;
}

BOOLEAN AddPeriodStrategicEventUsingSecondsWithOffset( UINT8 ubCallbackID, UINT32 uiOnceEveryXSeconds, UINT32 uiOffsetFromCurrent, UINT32 uiParam )
{
	STRATEGICEVENT *pEvent;
	pEvent = AddAdvancedStrategicEvent( PERIODIC_EVENT, ubCallbackID, GetWorldDayInSeconds() + uiOffsetFromCurrent, uiParam );
	if( pEvent )
	{
		pEvent->uiTimeOffset = uiOnceEveryXSeconds;
		return TRUE;
	}
	return FALSE;
}

void DeleteAllStrategicEventsOfType( UINT8 ubCallbackID )
{
	STRATEGICEVENT	*curr, *prev, *temp;
	prev = NULL;
	curr = gpEventList;
	while( curr )
	{
		if( curr->ubCallbackID == ubCallbackID && !(curr->ubFlags & SEF_DELETION_PENDING) )
		{
			if( gfPreventDeletionOfAnyEvent )
			{
				curr->ubFlags |= SEF_DELETION_PENDING;
				gfEventDeletionPending = TRUE;
				prev = curr;
				curr = curr->next;
				continue;
			}
			//Detach the node
			if( prev )
				prev->next = curr->next;
			else
				gpEventList = curr->next;

			//isolate and remove curr
			temp = curr;
			curr = curr->next;
			MemFree( temp );
			//ValidateGameEvents();
		}
		else
		{	//Advance all the nodes
			prev = curr;
			curr = curr->next;
		}
	}
}

void DeleteAllStrategicEvents()
{
	STRATEGICEVENT *temp;
	while( gpEventList )
	{
		temp = gpEventList;
		gpEventList = gpEventList->next;
		MemFree( temp );
		//ValidateGameEvents();
		temp = NULL;
	}
	gpEventList = NULL;
}

//Searches for and removes the first event matching the supplied information.	There may very well be a need
//for more specific event removal, so let me know (Kris), of any support needs.	Function returns FALSE if
//no events were found or if the event wasn't deleted due to delete lock,
BOOLEAN DeleteStrategicEvent( UINT8 ubCallbackID, UINT32 uiParam )
{
	STRATEGICEVENT *curr, *prev;
	curr = gpEventList;
	prev = NULL;
	while( curr )
	{ //deleting middle
		if( curr->ubCallbackID == ubCallbackID && curr->uiParam == uiParam )
		{
			if( !(curr->ubFlags & SEF_DELETION_PENDING) )
			{
				if( gfPreventDeletionOfAnyEvent )
				{
					curr->ubFlags |= SEF_DELETION_PENDING;
					gfEventDeletionPending = TRUE;
					return FALSE;
				}
				if( prev )
				{
					prev->next = curr->next;
				}
				else
				{
					gpEventList = gpEventList->next;
				}
				MemFree( curr );
				//ValidateGameEvents();
				return TRUE;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	return FALSE;
}

std::vector< std::pair<UINT32, UINT32> > GetAllStrategicEventsOfType( UINT8 ubCallbackID )
{
	std::vector< std::pair<UINT32, UINT32> > vec;

	STRATEGICEVENT* curr = gpEventList;
	while ( curr )
	{
		if ( curr->ubCallbackID == ubCallbackID )
		{
			vec.push_back( std::pair<UINT32, UINT32>( curr->uiTimeStamp, curr->uiParam ) );
		}

		curr = curr->next;
	}

	return vec;
}

//part of the game.sav files (not map files)
BOOLEAN SaveStrategicEventsToSavedGame( HWFILE hFile )
{
	UINT32	uiNumBytesWritten=0;
	STRATEGICEVENT sGameEvent;

	UINT32	uiNumGameEvents=0;
	STRATEGICEVENT *pTempEvent = gpEventList;

	//Go through the list and determine the number of events
	while( pTempEvent )
	{
		pTempEvent = pTempEvent->next;
		uiNumGameEvents++;
	}


	//write the number of strategic events
	FileWrite( hFile, &uiNumGameEvents, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return(FALSE);
	}


	//loop through all the events and save them.
	pTempEvent = gpEventList;
	while( pTempEvent )
	{
		//save the current structure
		memcpy( &sGameEvent, pTempEvent, sizeof( STRATEGICEVENT ) );

		//write the current strategic event
		FileWrite( hFile, &sGameEvent, sizeof( STRATEGICEVENT ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( STRATEGICEVENT ) )
		{
			return(FALSE);
		}

		pTempEvent = pTempEvent->next;
	}


	return( TRUE );
}


BOOLEAN LoadStrategicEventsFromSavedGame( HWFILE hFile )
{
	UINT32		uiNumGameEvents;
	STRATEGICEVENT sGameEvent;
	UINT32		cnt;
	UINT32		uiNumBytesRead=0;
	STRATEGICEVENT *pTemp = NULL;


	//erase the old Game Event queue
	DeleteAllStrategicEvents();


	//Read the number of strategic events
	FileRead( hFile, &uiNumGameEvents, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return(FALSE);
	}


	pTemp = NULL;

	//loop through all the events and save them.
	for( cnt=0; cnt<uiNumGameEvents; cnt++ )
	{
		STRATEGICEVENT *pTempEvent = NULL;

		// allocate memory for the event
		pTempEvent = (STRATEGICEVENT *) MemAlloc( sizeof( STRATEGICEVENT ) );
		if( pTempEvent == NULL )
			return( FALSE );

		//Read the current strategic event
		FileRead( hFile, &sGameEvent, sizeof( STRATEGICEVENT ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( STRATEGICEVENT ) )
		{
			return(FALSE);
		}


		memcpy( pTempEvent, &sGameEvent, sizeof( STRATEGICEVENT ) );

		// Add the new node to the list

		//if its the first node,
		if( cnt == 0 )
		{
			// assign it as the head node
			gpEventList = pTempEvent;

			//assign the 'current node' to the head node
			pTemp = gpEventList;
		}
		else
		{
			// add the new node to the next field of the current node
			pTemp->next = pTempEvent;

			//advance the current node to the next node
			pTemp = pTemp->next;
		}

		// NULL out the next field ( cause there is no next field yet )
		pTempEvent->next = NULL;
	}

	InitMiniEvents();

	return( TRUE );
}

void LockStrategicEventFromDeletion( STRATEGICEVENT *pEvent )
{
	pEvent->ubFlags |= SEF_PREVENT_DELETION;
}

void UnlockStrategicEventFromDeletion( STRATEGICEVENT *pEvent )
{
	pEvent->ubFlags &= ~SEF_PREVENT_DELETION;
}

void ValidateGameEvents()
{
	STRATEGICEVENT *curr;
	curr = gpEventList;
	while( curr )
	{
		curr = curr->next;
		if( curr == (STRATEGICEVENT*)0xdddddddd )
		{
			return;
		}
	}
}
