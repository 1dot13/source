#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "strategic.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	#include "math.h"
	#include "worlddef.h"
	#include "renderworld.h"
	#include "Assignments.h"

	#include "Animation Control.h"
	#include "Animation Data.h"
	#include "Isometric Utils.h"
	#include "Event Pump.h"
	#include "Render Fun.h"
	#include "interface.h"
	#include "sysutil.h"
	#include "FileMan.h"
	#include "Random.h"
	#include "ai.h"
	#include "Interactive Tiles.h"
	#include "english.h"
	#include "overhead.h"
	#include "Soldier Profile.h"
	#include "Game Clock.h"
	#include "soldier create.h"
	#include "Merc Hiring.h"
	#include "Game Event Hook.h"
	#include "message.h"
	#include "strategicmap.h"
	#include "strategic.h"
	#include "items.h"
	#include "Soldier Add.h"
	#include "History.h"
	#include "Squads.h"
	#include "Strategic Merc Handler.h"
	#include "Dialogue Control.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Map.h"
	#include "screenids.h"
	#include "jascreens.h"
	#include "text.h"
	#include "Merc Contract.h"
	#include "LaptopSave.h"
	#include "personnel.h"
	#include "Auto Resolve.h"
	#include "Map Screen Interface Bottom.h"
	#include "Quests.h"
	#include "GameSettings.h"
	#include "DynamicDialogue.h"// added by Flugente
	#include "Dialogue Control.h"	// added by Flugente
#endif
#include "connect.h"

#ifdef JA2UB
#include "Soldier Control.h"
#include "Ja25 Strategic Ai.h"
#include "Ja25_Tactical.h"
#include "Campaign Types.h"
#include "MapScreen Quotes.h"
#include "opplist.h"
#include "Ja25Update.h"
#include "ub_config.h"
#endif

#ifdef JA2UB
#else
	// anv: for Kulba's odyssey
	#include "email.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define	MIN_FLIGHT_PREP_TIME	6

#ifdef JA2TESTVERSION
	BOOLEAN	gForceHireMerc=FALSE;
	void SetFlagToForceHireMerc( BOOLEAN fForceHire );
#endif

extern BOOLEAN		gfTacticalDoHeliRun;
extern BOOLEAN		gfFirstHeliRun;

// ATE: Globals that dictate where the mercs will land once being hired
// Default to Omerta
// Saved in general saved game structure
// HEADROCK HAM 3.5: Externalized coordinates
INT16 gsMercArriveSectorX = gGameExternalOptions.ubDefaultArrivalSectorX;
INT16 gsMercArriveSectorY = gGameExternalOptions.ubDefaultArrivalSectorY;

void CheckForValidArrivalSector( );

#ifdef JA2UB
void AddItemToMerc( UINT8 ubNewMerc, INT16 sItemType );

#define	NUM_INITIAL_GRIDNOS_FOR_HELI_CRASH		7

UINT32	gsInitialHeliGridNo[ NUM_INITIAL_GRIDNOS_FOR_HELI_CRASH ] =
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

INT16	gsInitialHeliRandomTimes[ NUM_INITIAL_GRIDNOS_FOR_HELI_CRASH ] =
{
	0,
	0,
	0,
	0,
	0,
	0,
	0
};


UINT32		GetInitialHeliGridNo( );
UINT16	GetInitialHeliRandomTime();
#endif
INT8 HireMerc( MERC_HIRE_STRUCT *pHireMerc)
{
	SOLDIERTYPE	*pSoldier;
	UINT8		iNewIndex;
	UINT8		ubCurrentSoldier = pHireMerc->ubProfileID;
	MERCPROFILESTRUCT				*pMerc;
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	BOOLEAN fReturn = FALSE;
	pMerc = &gMercProfiles[ ubCurrentSoldier ];

	//If we are to disregard the ststus of the merc
	#ifdef JA2TESTVERSION
		if( !gForceHireMerc )
	#endif
	//If the merc is away, Dont hire him, or if the merc is only slightly annoyed at the player
	if( ( pMerc->bMercStatus != 0 ) && (pMerc->bMercStatus != MERC_ANNOYED_BUT_CAN_STILL_CONTACT ) && ( pMerc->bMercStatus != MERC_HIRED_BUT_NOT_ARRIVED_YET ) )
		return( MERC_HIRE_FAILED );


    //hayden, 7 member team limit setable in ini
	if( NumberOfMercsOnPlayerTeam() >= OUR_TEAM_SIZE_NO_VEHICLE || (is_client && NumberOfMercsOnPlayerTeam() >= cMaxMercs) )
		return( MERC_HIRE_OVER_PLAYER_LIMIT );

	// ATE: if we are to use landing zone, update to latest value
	// they will be updated again just before arrival...
	if ( pHireMerc->fUseLandingZoneForArrival )
	{
		pHireMerc->sSectorX	= gsMercArriveSectorX;
		pHireMerc->sSectorY	= gsMercArriveSectorY;
		pHireMerc->bSectorZ	= 0;
	}

	// BUILD STRUCTURES
	MercCreateStruct.initialize();
	MercCreateStruct.ubProfile						= ubCurrentSoldier;
	MercCreateStruct.fPlayerMerc					= TRUE;
	MercCreateStruct.sSectorX							= pHireMerc->sSectorX;
	MercCreateStruct.sSectorY							= pHireMerc->sSectorY;
	MercCreateStruct.bSectorZ							= pHireMerc->bSectorZ;
	MercCreateStruct.bTeam								= SOLDIER_CREATE_AUTO_TEAM;
	MercCreateStruct.fCopyProfileItemsOver= pHireMerc->fCopyProfileItemsOver;
	
	if(!cAllowMercEquipment && is_networked)
		MercCreateStruct.fCopyProfileItemsOver=0;//hayden : server overide

	if ( !TacticalCreateSoldier( &MercCreateStruct, &iNewIndex ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "TacticalCreateSoldier in HireMerc():	Failed to Add Merc");
		return( MERC_HIRE_FAILED );
	}

#ifdef JA2UB
	//JA25 UB
	//MErc mercs come with an umbrella
	if ( gMercProfiles[ubCurrentSoldier].Type == PROFILETYPE_MERC )
	{
		AddItemToMerc( iNewIndex, MERC_UMBRELLA ); //Data-1.13\TableData\Items\items.xml, uiIndex = 1361 or Data\TableData\Items\items.xml, uiIndex = 336
	}
	
	//if this is an AIM or MERC merc
	if( gJa25SaveStruct.fHaveAimandMercOffferItems )
	{
		//if its an aim merc
		if ( gMercProfiles[ubCurrentSoldier].Type == PROFILETYPE_AIM )
		{
			//give the mercs one of the promo items
			AddItemToMerc( iNewIndex, SAM_GARVER_COMBAT_KNIFE ); //1353
		}
		// if its a merc merc
		else if ( gMercProfiles[ubCurrentSoldier].Type == PROFILETYPE_MERC )
		{
			//give the mercs one of the promo items
			AddItemToMerc( iNewIndex, CHE_GUEVARA_CANTEEN ); //1359
			AddItemToMerc( iNewIndex, MERC_WRISTWATCH ); //1360
			AddItemToMerc( iNewIndex, SAM_GARVER_COMBAT_KNIFE ); //1353
		}
	}
#else //ja25: No enrico, therefore, no email
	if( DidGameJustStart() )
	{
		// OK, CHECK FOR FIRST GUY, GIVE HIM SPECIAL ITEM!
		if ( iNewIndex == 0 )
		{
			// OK, give this item to our merc!
			// make an objecttype
			CreateItem(LETTER, 100, &gTempObject);
			// Give it
			fReturn = AutoPlaceObject( MercPtrs[iNewIndex], &gTempObject, FALSE );
			// CHRISL: This condition should resolve the issue of the letter not being issued to the first merc
			if(!fReturn && (UsingNewInventorySystem() == true))
			{
				(MercPtrs[iNewIndex]->inv[NUM_INV_SLOTS-1]) = gTempObject;
				fReturn=TRUE;
			}
			Assert( fReturn );
		}

		// Set insertion for first time in chopper

		// ATE: Insert for demo , not using the heli sequence....
		pHireMerc->ubInsertionCode				= INSERTION_CODE_CHOPPER;
	}
#endif

	//record how long the merc will be gone for
	pMerc->bMercStatus = (UINT8)pHireMerc->iTotalContractLength;

	pSoldier = &Menptr[iNewIndex];

	//Copy over insertion data....
	pSoldier->ubStrategicInsertionCode = pHireMerc->ubInsertionCode;
	pSoldier->usStrategicInsertionData = pHireMerc->usInsertionData;
	// ATE: Copy over value for using alnding zone to soldier type
	pSoldier->flags.fUseLandingZoneForArrival = pHireMerc->fUseLandingZoneForArrival;


	// Set assignment
	//ATE: If first time, make ON_DUTY, otherwise GUARD
	if( ( pSoldier->bAssignment != IN_TRANSIT ) )
	{
		SetTimeOfAssignmentChangeForMerc( pSoldier );
	}
	ChangeSoldiersAssignment( pSoldier, IN_TRANSIT );

	//set the contract length
	pSoldier->iTotalContractLength = pHireMerc->iTotalContractLength;

	//reset the insurance values
	pSoldier->iStartOfInsuranceContract = 0;
	pSoldier->iTotalLengthOfInsuranceContract = 0;

	//Init the contract charge
//	pSoldier->iTotalContractCharge = 0;

	// store arrival time in soldier structure so map screen can display it
	pSoldier->uiTimeSoldierWillArrive = pHireMerc->uiTimeTillMercArrives;


	//Set the type of merc

	if (!is_networked)
	{
	if( DidGameJustStart() )
	{
#ifdef JA2UB
		//ja25ub
		//set a flag so we know we are doing the heli crash
		if ( gGameUBOptions.InGameHeliCrash == TRUE || gGameUBOptions.InGameHeli == FALSE)
			gfFirstTimeInGameHeliCrash = TRUE; //AA FALSE ??
		else
			gfFirstTimeInGameHeliCrash = FALSE;

		// Set time of initial merc arrival in minutes
#endif
		pHireMerc->uiTimeTillMercArrives = ( gGameExternalOptions.iGameStartingTime + gGameExternalOptions.iFirstArrivalDelay ) / NUM_SEC_IN_MIN;
#ifdef JA2UB

	if ( gGameUBOptions.InGameHeli == FALSE )
	{
		// Set the gridno for the soldier
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = GetInitialHeliGridNo( );

		//Set a "code" to enable the merc to be in the direction we set!
		pSoldier->ubInsertionDirection = Random( NUM_WORLD_DIRECTIONS ) + 100;

		if( pSoldier->ubStrategicInsertionCode == 0 )
		{
			Assert( 0 );
		}

		pSoldier->fWaitingToGetupFromJA25Start = TRUE;

		pSoldier->fIgnoreGetupFromCollapseCheck = TRUE;

		RESETTIMECOUNTER( pSoldier->GetupFromJA25StartCounter, GetInitialHeliRandomTime() );
		}
		else
		{
		pHireMerc->ubInsertionCode				= INSERTION_CODE_CHOPPER;
		}
#else
		// Set insertion for first time in chopper
		pHireMerc->ubInsertionCode				= INSERTION_CODE_CHOPPER;
#endif
		//set when the merc's contract is finished
		pSoldier->iEndofContractTime = GetMidnightOfFutureDayInMinutes( pSoldier->iTotalContractLength ) + ( GetHourWhenContractDone( pSoldier ) * 60 );
	}
	else
	{
		//set when the merc's contract is finished ( + 1 cause it takes a day for the merc to arrive )
		pSoldier->iEndofContractTime = GetMidnightOfFutureDayInMinutes( 1 + pSoldier->iTotalContractLength ) + ( GetHourWhenContractDone( pSoldier ) * 60 );
	}
	}
	// WANNE - MP: We need this, so the merc contract is correct!
	else
	{
		pSoldier->iEndofContractTime = GetMidnightOfFutureDayInMinutes( pSoldier->iTotalContractLength ) + ( GetHourWhenContractDone( pSoldier ) * 60 );
	}

	//Set the time and ID of the last hired merc will arrive
	LaptopSaveInfo.sLastHiredMerc.iIdOfMerc = pHireMerc->ubProfileID;
	LaptopSaveInfo.sLastHiredMerc.uiArrivalTime = pHireMerc->uiTimeTillMercArrives;

	if (!is_client)
	{
		//if we are trying to hire a merc that should arrive later, put the merc in the queue
		if( pHireMerc->uiTimeTillMercArrives	!= 0 )
		{
			AddStrategicEvent( EVENT_DELAYED_HIRING_OF_MERC, pHireMerc->uiTimeTillMercArrives,	pSoldier->ubID );
				
			//specify that the merc is hired but hasnt arrived yet
			pMerc->bMercStatus = MERC_HIRED_BUT_NOT_ARRIVED_YET;
		}
	}
	else
	{
		if(is_client)send_hire( iNewIndex, ubCurrentSoldier, pHireMerc->iTotalContractLength, MercCreateStruct.fCopyProfileItemsOver  );
		//send off hire info to network, also avail possibility for net-game exclusive hired pSoldier changes...
	}

	//if the merc is an AIM merc
	if ( gMercProfiles[ubCurrentSoldier].Type == PROFILETYPE_AIM )
	{
		pSoldier->ubWhatKindOfMercAmI = MERC_TYPE__AIM_MERC;
		//determine how much the contract is, and remember what type of contract he got
		if( pHireMerc->iTotalContractLength == 1 )
		{
			//pSoldier->iTotalContractCharge = gMercProfiles[ pSoldier->ubProfile ].sSalary;
			pSoldier->bTypeOfLastContract = CONTRACT_EXTEND_1_DAY;
		pSoldier->iTimeCanSignElsewhere = GetWorldTotalMin();
		}
		else if( pHireMerc->iTotalContractLength == 7 )
		{
			//pSoldier->iTotalContractCharge = gMercProfiles[ pSoldier->ubProfile ].uiWeeklySalary;
			pSoldier->bTypeOfLastContract = CONTRACT_EXTEND_1_WEEK;
		pSoldier->iTimeCanSignElsewhere = GetWorldTotalMin();
		}
		else if( pHireMerc->iTotalContractLength == 14 )
		{
			//pSoldier->iTotalContractCharge = gMercProfiles[ pSoldier->ubProfile ].uiBiWeeklySalary;
			pSoldier->bTypeOfLastContract = CONTRACT_EXTEND_2_WEEK;
		// These luck fellows need to stay the whole duration!
		pSoldier->iTimeCanSignElsewhere = pSoldier->iEndofContractTime;
		}

		// remember the medical deposit we PAID.	The one in his profile can increase when he levels!
		pSoldier->usMedicalDeposit = gMercProfiles[ pSoldier->ubProfile ].sMedicalDepositAmount;
	}
	//if the merc is from M.E.R.C.
	else if ( gMercProfiles[ubCurrentSoldier].Type == PROFILETYPE_MERC )
	{
		pSoldier->ubWhatKindOfMercAmI = MERC_TYPE__MERC;
		//pSoldier->iTotalContractCharge = -1;

		gMercProfiles[ pSoldier->ubProfile ].iMercMercContractLength = 1;

		//Set starting conditions for the merc
		pSoldier->iStartContractTime = GetWorldDay( );

		if(!is_client)AddHistoryToPlayersLog(HISTORY_HIRED_MERC_FROM_MERC, ubCurrentSoldier, GetWorldTotalMin(), -1, -1 );
	}
	//If the merc is from IMP, (ie a player character)
	else if ( gMercProfiles[ubCurrentSoldier].Type == PROFILETYPE_IMP )
	{
		pSoldier->ubWhatKindOfMercAmI = MERC_TYPE__PLAYER_CHARACTER;
		//pSoldier->iTotalContractCharge = -1;
	}
	//else its a NPC merc
	else
	{
		pSoldier->ubWhatKindOfMercAmI = MERC_TYPE__NPC;
		//pSoldier->iTotalContractCharge = -1;
	}
#ifdef JA2UB
	//Ja25:  Need to set start time for all mercs
	pSoldier->iStartContractTime = GetWorldDay( );
#endif
	//remove the merc from the Personnel screens departed list ( if they have never been hired before, its ok to call it )
	RemoveNewlyHiredMercFromPersonnelDepartedList( pSoldier->ubProfile );

	// Flugente: dynamic opinions
	CheckForFriendsofHated( pSoldier );

	gfAtLeastOneMercWasHired = TRUE;

	return( MERC_HIRE_OK );
}


void MercArrivesCallback(	UINT8	ubSoldierID )
{
	MERCPROFILESTRUCT				*pMerc;
	SOLDIERTYPE							*pSoldier;
	UINT32									uiTimeOfPost;


	if (!is_networked)
	{
		// hayden - maybe you want to duke it out in omerta ;)
		// HEADROCK HAM 3.5: Externalized starting (safe) sector
		// HEADROCK HAM 3.5: Actually, this is really ridiculous. Why should enemies at the LZ be eliminated at all?
		// I'm taking the initiative and removing this from the code. Mainly because it ends up interfering with
		// externalized LZs combined with other features like "Always Real Time" and "Forced Turn Based".
		//if( !DidGameJustStart() && gsMercArriveSectorX == gGameExternalOptions.ubDefaultArrivalSectorX && gsMercArriveSectorY == gGameExternalOptions.ubDefaultArrivalSectorY )
		//	{ 
		//		//Mercs arriving in A9.  This sector has been deemed as the always safe sector.
		//		//Seeing we don't support entry into a hostile sector (except for the beginning),
		//		//we will nuke any enemies in this sector first.
		//		if( gWorldSectorX != gGameExternalOptions.ubDefaultArrivalSectorX || gWorldSectorY != gGameExternalOptions.ubDefaultArrivalSectorY || gbWorldSectorZ )
		//		{
		//			EliminateAllEnemies( (UINT8)gsMercArriveSectorX, (UINT8)gsMercArriveSectorY );
		//		}
		//	}
	}

	// This will update ANY soldiers currently schedules to arrive too
	CheckForValidArrivalSector( );

	// stop time compression until player restarts it
	StopTimeCompression();

	pSoldier = &Menptr[ ubSoldierID ];

	pMerc = &gMercProfiles[ pSoldier->ubProfile ];

// anv: handle Kulba's odyssey
#ifdef JA2UB
	// too many Kulbas
#else
	if(pSoldier->ubProfile == JOHN_MERC)
	{
		// just in case
		if( LaptopSaveInfo.ubJohnPossibleMissedFlights > 3 )
			LaptopSaveInfo.ubJohnPossibleMissedFlights = 3;
		// every time Kulba delays his arrival, chances of next delay decrease
		if( Random( 100 ) < LaptopSaveInfo.ubJohnPossibleMissedFlights * 25 ) 
		{			
			pSoldier->uiTimeSoldierWillArrive = pSoldier->uiTimeSoldierWillArrive + 720 + Random ( 720 );
			AddStrategicEvent( EVENT_DELAYED_HIRING_OF_MERC, pSoldier->uiTimeSoldierWillArrive,	pSoldier->ubID );
			if(LaptopSaveInfo.ubJohnPossibleMissedFlights == 3 )
				AddEmail( JOHN_KULBA_MISSED_FLIGHT_1, JOHN_KULBA_MISSED_FLIGHT_1_LENGTH, JOHN_KULBA, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			else if(LaptopSaveInfo.ubJohnPossibleMissedFlights == 2 )
				AddEmail( JOHN_KULBA_MISSED_FLIGHT_2, JOHN_KULBA_MISSED_FLIGHT_2_LENGTH, JOHN_KULBA, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			else if(LaptopSaveInfo.ubJohnPossibleMissedFlights == 1 )
				AddEmail( JOHN_KULBA_MISSED_FLIGHT_3, JOHN_KULBA_MISSED_FLIGHT_3_LENGTH, JOHN_KULBA, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );

			LaptopSaveInfo.ubJohnPossibleMissedFlights--;
			return;
		}
		else
		{
			// reset possible missed flights, for the future Kulba's adventures
			LaptopSaveInfo.ubJohnPossibleMissedFlights = 3;
		}
	}
#endif

	// add the guy to a squad
	AddCharacterToAnySquad( pSoldier );

	// ATE: Make sure we use global.....
	if ( pSoldier->flags.fUseLandingZoneForArrival )
	{
		pSoldier->sSectorX	= gsMercArriveSectorX;
		pSoldier->sSectorY	= gsMercArriveSectorY;
		pSoldier->bSectorZ	= 0;
	}

	// Add merc to sector ( if it's the current one )
	if ( gWorldSectorX == pSoldier->sSectorX && gWorldSectorY == pSoldier->sSectorY && pSoldier->bSectorZ == gbWorldSectorZ )
	{
		// OK, If this sector is currently loaded, and guy does not have CHOPPER insertion code....
		// ( which means we are at beginning of game if so )
		// Setup chopper....
		#ifdef JA2UB
		if ( pSoldier->ubStrategicInsertionCode != INSERTION_CODE_CHOPPER && pSoldier->sSectorX == gGameExternalOptions.ubDefaultArrivalSectorX && pSoldier->sSectorY == gGameExternalOptions.ubDefaultArrivalSectorY && gGameUBOptions.InGameHeli == TRUE )
		#else
		if ( pSoldier->ubStrategicInsertionCode != INSERTION_CODE_CHOPPER && pSoldier->sSectorX == gGameExternalOptions.ubDefaultArrivalSectorX && pSoldier->sSectorY == gGameExternalOptions.ubDefaultArrivalSectorY )
		#endif
		{
			gfTacticalDoHeliRun = TRUE;

			// OK, If we are in mapscreen, get out...
			if ( guiCurrentScreen == MAP_SCREEN )
			{
		// ATE: Make sure the current one is selected!
				ChangeSelectedMapSector( gWorldSectorX, gWorldSectorY, 0 );

				RequestTriggerExitFromMapscreen( MAP_EXIT_TO_TACTICAL );
			}

			pSoldier->ubStrategicInsertionCode = INSERTION_CODE_CHOPPER;
		}

		UpdateMercInSector( pSoldier, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
	}
	else
	{
		// OK, otherwise, set them in north area, so once we load again, they are here.
#ifdef JA2UB
		//pSoldier->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = gGameUBOptions.LOCATEGRIDNO;
#else
		//pSoldier->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = gGameExternalOptions.iInitialMercArrivalLocation;
#endif
	}

	if ( pSoldier->ubStrategicInsertionCode != INSERTION_CODE_CHOPPER )
	{
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, TacticalStr[ MERC_HAS_ARRIVED_STR ], pSoldier->GetName() );

		// ATE: He's going to say something, now that they've arrived...
		if ( gTacticalStatus.bMercArrivingQuoteBeingUsed == FALSE && !gfFirstHeliRun )
		{
			gTacticalStatus.bMercArrivingQuoteBeingUsed = TRUE;

			//Setup the highlight sector value (note this isn't for mines but using same system)
			gsSectorLocatorX = pSoldier->sSectorX;
			gsSectorLocatorY = pSoldier->sSectorY;

			TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_MINESECTOREVENT, 2, 0 );
			
//ja25ub
#ifdef JA2UB
			//if its the first time in, dont say anything
			if( !gfFirstTimeInGameHeliCrash )
#endif
			TacticalCharacterDialogue( pSoldier, QUOTE_MERC_REACHED_DESTINATION );

			TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_MINESECTOREVENT, 3, 0 );
			TacticalCharacterDialogueWithSpecialEventEx( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_UNSET_ARRIVES_FLAG, 0, 0, 0 );
		}
	}

	//record how long the merc will be gone for
	pMerc->bMercStatus = (UINT8)pSoldier->iTotalContractLength;

	// remember when excatly he ARRIVED in Arulco, in case he gets fired early
	pSoldier->uiTimeOfLastContractUpdate = GetWorldTotalMin();

	//set when the merc's contract is finished
	pSoldier->iEndofContractTime = GetMidnightOfFutureDayInMinutes( pSoldier->iTotalContractLength ) + ( GetHourWhenContractDone( pSoldier ) * 60 );

	// Do initial check for bad items
	if ( pSoldier->bTeam == gbPlayerNum )
	{
		//ATE: Try to see if our equipment sucks!
		if ( SoldierHasWorseEquipmentThanUsedTo( pSoldier ) )
		{
			// Randomly anytime between 9:00, and 10:00
			uiTimeOfPost =	540 + Random( 660 );

			if ( GetWorldMinutesInDay() < uiTimeOfPost )
			{
				AddSameDayStrategicEvent( EVENT_MERC_COMPLAIN_EQUIPMENT, uiTimeOfPost , pSoldier->ubProfile );
			}
		}
	}

	HandleMercArrivesQuotes( pSoldier );

	fTeamPanelDirty = TRUE;

	// if the currently selected sector has no one in it, select this one instead
	if ( !CanGoToTacticalInSector( sSelMapX, sSelMapY, ( UINT8 )iCurrentMapSectorZ ) )
	{
		ChangeSelectedMapSector( pSoldier->sSectorX, pSoldier->sSectorY, 0 );
	}

	return;
}


BOOLEAN IsMercHireable( UINT8 ubMercID )
{
	//If the merc has an .EDT file, is not away on assignment, and isnt already hired (but not arrived yet), he is not DEAD and he isnt returning home
	if( ( gMercProfiles[ ubMercID ].bMercStatus == MERC_HAS_NO_TEXT_FILE ) ||
			( gMercProfiles[ ubMercID ].bMercStatus > 0 ) ||
			( gMercProfiles[ ubMercID ].bMercStatus == MERC_HIRED_BUT_NOT_ARRIVED_YET ) ||
			( gMercProfiles[ ubMercID ].bMercStatus == MERC_IS_DEAD ) ||
			( gMercProfiles[ ubMercID ].uiDayBecomesAvailable > 0 ) ||
			( gMercProfiles[ ubMercID ].bMercStatus == MERC_WORKING_ELSEWHERE ) ||
			( gMercProfiles[ ubMercID ].bMercStatus == MERC_FIRED_AS_A_POW ) ||
			( gMercProfiles[ ubMercID ].bMercStatus == MERC_RETURNING_HOME ) )
		return(FALSE);
	else
		return(TRUE);
}

BOOLEAN IsMercDead( UINT8 ubMercID )
{
	if( gMercProfiles[ ubMercID ].bMercStatus == MERC_IS_DEAD )
		return(TRUE);
	else
		return(FALSE);
}

BOOLEAN IsTheSoldierAliveAndConcious( SOLDIERTYPE		*pSoldier )
{
	if( pSoldier->stats.bLife >= CONSCIOUSNESS )
		return(TRUE);
	else
		return(FALSE);
}

UINT8	NumberOfMercsOnPlayerTeam()
{
	INT8			cnt;
	SOLDIERTYPE		*pSoldier;
	INT16			bLastTeamID;
	UINT8			ubCount=0;

	// Set locator to first merc
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

	if (! MercPtrs[cnt])
		return 0;

	for ( pSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pSoldier++)
	{
		AssertNotNIL(pSoldier);

		//if the is active, and is not a vehicle
		if( pSoldier->bActive && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			ubCount++;
		}
	}

	return( ubCount );
}


void HandleMercArrivesQuotes( SOLDIERTYPE *pSoldier )
{
	INT8										cnt, bHated, bLastTeamID;
	SOLDIERTYPE							*pTeamSoldier;
#ifdef JA2UB
	//if we are at the begining of the game going through the initial heli scequence
	if( pSoldier->fWaitingToGetupFromJA25Start )
	{
		//we can "leave" this function cause we dont want to do anything with buddy system
		return;
	}
#endif	
	// If we are approaching with helicopter, don't say any ( yet )
	if ( pSoldier->ubStrategicInsertionCode != INSERTION_CODE_CHOPPER )
	{
		// if we haven't met the rebels yet, characters issue some comments
		if ( gubQuest[QUEST_DELIVER_LETTER] == QUESTINPROGRESS )
		{
			// Player-generated characters issue a comment about arriving in Omerta.
			if ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER )
			{
				TacticalCharacterDialogue( pSoldier, QUOTE_PC_DROPPED_OMERTA );
			}
			// Flugente: additional dialogue
			else
			{
				AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_OMERTA_ENTRY );
			}
		}

		// Check to see if anyone hates this merc and will now complain
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;
		//loop though all the mercs
		for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pTeamSoldier++)
		{
			if ( pTeamSoldier->bActive )
			{
				if ( pTeamSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
				{
					bHated = WhichHated( pTeamSoldier->ubProfile, pSoldier->ubProfile );
					if ( bHated != -1 )
					{
						// hates the merc who has arrived and is going to gripe about it!
						switch( bHated )
						{
							case 0:
								TacticalCharacterDialogue( pTeamSoldier, QUOTE_HATED_1_ARRIVES );
								break;
							case 1:
								TacticalCharacterDialogue( pTeamSoldier, QUOTE_HATED_2_ARRIVES );
								break;
							case 2:
								TacticalCharacterDialogue( pTeamSoldier, QUOTE_HATED_3_ARRIVES );
								break;
							case 3:
								TacticalCharacterDialogue( pTeamSoldier, QUOTE_HATED_4_ARRIVES );
								break;
							case 4:
								TacticalCharacterDialogue( pTeamSoldier, QUOTE_HATED_5_ARRIVES );
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}
}


#ifdef JA2TESTVERSION
void SetFlagToForceHireMerc( BOOLEAN fForceHire )
{
	gForceHireMerc = fForceHire;
}
#endif


UINT32 GetMercArrivalTimeOfDay( )
{
	UINT32		uiCurrHour;
	UINT32		uiMinHour;

	// Pick a time...

	// First get the current time of day.....
	uiCurrHour = GetWorldHour( );

	// Subtract the min time for any arrival....
	uiMinHour	= uiCurrHour + MIN_FLIGHT_PREP_TIME;

	// OK, first check if we need to advance a whole day's time...
	// See if we have missed the last flight for the day...
	if ( ( uiCurrHour ) > 13	) // ( > 1:00 pm - too bad )
	{
		// 7:30 flight....
		return( GetMidnightOfFutureDayInMinutes( 1 ) + MERC_ARRIVE_TIME_SLOT_1 );
	}

	// Well, now we can handle flights all in one day....
	// Find next possible flight
	if ( uiMinHour <= 7 )
	{
		return( GetWorldDayInMinutes() + MERC_ARRIVE_TIME_SLOT_1 ); // 7:30 am
	}
	else if ( uiMinHour <= 13 )
	{
		return( GetWorldDayInMinutes() + MERC_ARRIVE_TIME_SLOT_2 ); // 1:30 pm
	}
	else
	{
		return( GetWorldDayInMinutes() + MERC_ARRIVE_TIME_SLOT_3 ); // 7:30 pm
	}
}


void UpdateAnyInTransitMercsWithGlobalArrivalSector( )
{
	INT32 cnt;
	SOLDIERTYPE		*pSoldier;

	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
	{
		if ( pSoldier->bActive )
		{
			if ( pSoldier->bAssignment == IN_TRANSIT )
			{
				if ( pSoldier->flags.fUseLandingZoneForArrival )
				{
					pSoldier->sSectorX	= gsMercArriveSectorX;
					pSoldier->sSectorY	= gsMercArriveSectorY;
					pSoldier->bSectorZ	= 0;
				}
			}
		}
	}
}

INT16 StrategicPythSpacesAway(INT16 sOrigin, INT16 sDest)
{
	INT16 sRows,sCols,sResult;

	sRows = abs((sOrigin / MAP_WORLD_X) - (sDest / MAP_WORLD_X));
	sCols = abs((sOrigin % MAP_WORLD_X) - (sDest % MAP_WORLD_X));


	// apply Pythagoras's theorem for right-handed triangle:
	// dist^2 = rows^2 + cols^2, so use the square root to get the distance
	sResult = (INT16)sqrt((double)(sRows * sRows) + (sCols * sCols));

	return(sResult);
}


// ATE: This function will check if the current arrival sector
// is valid
// if there are enemies present, it's invalid
// if so, search around for nearest non-occupied sector.
void CheckForValidArrivalSector( )
{
	INT16	sTop, sBottom;
	INT16	sLeft, sRight;
	INT16	cnt1, cnt2, sGoodX, sGoodY;
	UINT8	ubRadius = 4;
	UINT32	leftmost;
	UINT32	 sSectorGridNo, sSectorGridNo2;
	INT32	uiRange, uiLowestRange = 999999;
	BOOLEAN	fFound = FALSE;
	CHAR16 sString[ 1024 ];
	CHAR16 zShortTownIDString1[ 50 ];
	CHAR16 zShortTownIDString2[ 50 ];

	sSectorGridNo = gsMercArriveSectorX + ( MAP_WORLD_X * gsMercArriveSectorY );

	// Check if valid...
	if ( !StrategicMap[ sSectorGridNo ].fEnemyControlled )
	{
		return;
	}

	GetShortSectorString( gsMercArriveSectorX ,gsMercArriveSectorY, zShortTownIDString1 );


	// If here - we need to do a search!
	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSectorGridNo + ( MAP_WORLD_X * cnt1 ) )/ MAP_WORLD_X ) * MAP_WORLD_X;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sSectorGridNo2 = sSectorGridNo + ( MAP_WORLD_X * cnt1 ) + cnt2;

			if( sSectorGridNo2 >=1 && sSectorGridNo2 < ( ( MAP_WORLD_X - 1 ) * ( MAP_WORLD_X - 1 ) ) && sSectorGridNo2 >= leftmost && sSectorGridNo2 < ( leftmost + MAP_WORLD_X ) )
			{
				if ( !StrategicMap[ sSectorGridNo2 ].fEnemyControlled && (StrategicMap[ sSectorGridNo2 ].usAirType & AIRSPACE_ENEMY_ACTIVE) )
				{
					uiRange = StrategicPythSpacesAway( sSectorGridNo2, sSectorGridNo );

					if ( uiRange < uiLowestRange )
					{
						sGoodY = cnt1;
						sGoodX = cnt2;
						uiLowestRange = uiRange;
						fFound = TRUE;
					}
				}
			}
		}
	}

	if ( fFound )
	{
		gsMercArriveSectorX = gsMercArriveSectorX + sGoodX;
		gsMercArriveSectorY = gsMercArriveSectorY + sGoodY;

		UpdateAnyInTransitMercsWithGlobalArrivalSector( );

		GetShortSectorString( gsMercArriveSectorX ,gsMercArriveSectorY, zShortTownIDString2 );

//		swprintf( sString, L"Arrival of new recruits is being rerouted to sector %s, as scheduled drop-off point of sector %s is enemy occupied.", zShortTownIDString2, zShortTownIDString1 );
		swprintf( sString, New113Message[MSG113_ARRIVINGREROUTED], zShortTownIDString2, zShortTownIDString1 );

		DoScreenIndependantMessageBox(	sString, MSG_BOX_FLAG_OK, NULL );

	}
}
#ifdef JA2UB
UINT32	GetInitialHeliGridNo( )
{
	UINT8	ubCnt;
	UINT32	sGridNo;

	for( ubCnt=0; ubCnt<NUM_INITIAL_GRIDNOS_FOR_HELI_CRASH-1; ubCnt++)
	{
		if( gsInitialHeliGridNo[ ubCnt ] != 0 )
		{
			sGridNo = gsInitialHeliGridNo[ ubCnt ];
			gsInitialHeliGridNo[ ubCnt ] = 0;

			return( sGridNo );
		}
	}

	return( 16233 );
}

UINT16	GetInitialHeliRandomTime()
{
	BOOLEAN fDone=FALSE;
	UINT8		ubRandom;
	UINT16	usTime;
	UINT16	usCounter=0;

	while( !fDone )
	{
		ubRandom = Random( NUM_INITIAL_GRIDNOS_FOR_HELI_CRASH-1 );

		if( gsInitialHeliRandomTimes[ ubRandom ] != 0 )
		{
			usTime = gsInitialHeliRandomTimes[ ubRandom ];
			gsInitialHeliRandomTimes[ ubRandom ] = 0;
			return( usTime );
		}

		if( usCounter > 1000 )
			fDone = TRUE;

		usCounter++;
	}
	return( 1000 + Random( 2000 ) );
}

void InitializeHeliGridnoAndTime( BOOLEAN fLoading )
{
	Assert( NUM_INITIAL_GRIDNOS_FOR_HELI_CRASH == 7 );

	if( !fLoading )
	{
		gfFirstTimeInGameHeliCrash = FALSE;
	}

	gsInitialHeliGridNo[ 0 ] = gGameUBOptions.InitialHeliGridNo[ 0 ];//14947;
	gsInitialHeliGridNo[ 1 ] = gGameUBOptions.InitialHeliGridNo[ 1 ];//15584;//16067;
	gsInitialHeliGridNo[ 2 ] = gGameUBOptions.InitialHeliGridNo[ 2 ];//15754;
	gsInitialHeliGridNo[ 3 ] = gGameUBOptions.InitialHeliGridNo[ 3 ];//16232;
	gsInitialHeliGridNo[ 4 ] = gGameUBOptions.InitialHeliGridNo[ 4 ];//16067;
	gsInitialHeliGridNo[ 5 ] = gGameUBOptions.InitialHeliGridNo[ 5 ];//16230;
	gsInitialHeliGridNo[ 6 ] = gGameUBOptions.InitialHeliGridNo[ 6 ];//15272;

	gsInitialHeliRandomTimes[ 0 ] = gGameUBOptions.InitalHeliRandomTimes[ 0 ];//1300;
	gsInitialHeliRandomTimes[ 1 ] = gGameUBOptions.InitalHeliRandomTimes[ 1 ];//2000;
	gsInitialHeliRandomTimes[ 2 ] = gGameUBOptions.InitalHeliRandomTimes[ 2 ];//2750;
	gsInitialHeliRandomTimes[ 3 ] = gGameUBOptions.InitalHeliRandomTimes[ 3 ];//3400;
	gsInitialHeliRandomTimes[ 4 ] = gGameUBOptions.InitalHeliRandomTimes[ 4 ];//4160;
	gsInitialHeliRandomTimes[ 5 ] = gGameUBOptions.InitalHeliRandomTimes[ 5 ];//4700;
	gsInitialHeliRandomTimes[ 6 ] = gGameUBOptions.InitalHeliRandomTimes[ 6 ];//5630;
}

void InitJerryMiloInfo()
{
 if ( gGameUBOptions.InJerry == TRUE )
{
  //  return; //AA
	//Set Jerry Milo's Gridno h7
	gMercProfiles[ JERRY_MILO_UB ].sSectorX = JA2_5_START_SECTOR_X;
	gMercProfiles[ JERRY_MILO_UB ].sSectorY = JA2_5_START_SECTOR_Y;
	gMercProfiles[ JERRY_MILO_UB ].bSectorZ = 0;

	gMercProfiles[ JERRY_MILO_UB ].sGridNo = gGameUBOptions.JerryGridNo; //15109;

	gMercProfiles[ JERRY_MILO_UB ].fUseProfileInsertionInfo = TRUE;

	gMercProfiles[ JERRY_MILO_UB ].ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	gMercProfiles[ JERRY_MILO_UB ].usStrategicInsertionData = gGameUBOptions.JerryGridNo; //15109;
	
}
	
if ( gGameUBOptions.InGameHeliCrash == TRUE )
	{
	//init Jerry Milo quotes
	InitJerryQuotes();
	}
}


void UpdateJerryMiloInInitialSector()
{
	SOLDIERTYPE	*pSoldier = NULL;
	SOLDIERTYPE	*pJerrySoldier=NULL;

	
    //SectorInfo[ SEC_H7 ].fSurfaceWasEverPlayerControlled = TRUE;
	  SectorInfo[ (UINT8)SECTOR( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY ) ].fSurfaceWasEverPlayerControlled = TRUE;
    //SectorInfo[ SEC_H7 ].ubNumAdmins = 2;
	StrategicMap[ (UINT8)SECTOR( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY ) ].fEnemyControlled = FALSE;
    
if ( gGameUBOptions.InGameHeli == TRUE )
	return; //AA

if ( gGameUBOptions.InGameHeliCrash == TRUE )
   { 
	//if it is the first sector we are loading up, place Jerry in the map
	if( !gfFirstTimeInGameHeliCrash )
		return;

	if ( gGameUBOptions.InJerry == TRUE ) 
	{
	pSoldier = FindSoldierByProfileID( JERRY_MILO_UB, FALSE ); //JERRY
	if( pSoldier == NULL )
	{
		Assert( 0 );
	}
	
	}

	//the internet part of the laptop isnt working.  It gets broken in the heli crash.
	if ( gGameUBOptions.LaptopQuestEnabled == TRUE )
	StartQuest( QUEST_FIX_LAPTOP, -1, -1 );

	//Record the initial sector as ours
	//SectorInfo[ SEC_H7 ].fSurfaceWasEverPlayerControlled = TRUE;
	  SectorInfo[ (UINT8)SECTOR( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY ) ].fSurfaceWasEverPlayerControlled = TRUE;
	  StrategicMap[ (UINT8)SECTOR( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY ) ].fEnemyControlled = FALSE;
 
	  if ( gGameUBOptions.InJerry == TRUE ) 
	{
	//Set some variable so Jerry will be on the ground
	pSoldier->fWaitingToGetupFromJA25Start = TRUE;
	pSoldier->fIgnoreGetupFromCollapseCheck = TRUE;

	//pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO; //to by³o wy³¹czone
	//pSoldier->usStrategicInsertionData = GetInitialHeliGridNo( ); //to by³o wy³¹czone

	RESETTIMECOUNTER( pSoldier->GetupFromJA25StartCounter, gsInitialHeliRandomTimes[ 6 ] + 800 + Random( 400 ) );

	//should we be on our back or tummy
	if( Random( 100 ) < 50 )
		pSoldier->EVENT_InitNewSoldierAnim( STAND_FALLFORWARD_STOP, 1, TRUE );
	else
		pSoldier->EVENT_InitNewSoldierAnim( FALLBACKHIT_STOP, 1, TRUE );
	}

//Wont work cause it gets reset every frame
	//make sure we can see Jerry
	
	if ( gGameUBOptions.InJerry == TRUE ) 
	{
	pJerrySoldier = FindSoldierByProfileID(JERRY_MILO_UB, FALSE );//JERRY
	if( pJerrySoldier != NULL )
	{
		//Make sure we can see the pilot
		gbPublicOpplist[OUR_TEAM][ pJerrySoldier->ubID ] = SEEN_CURRENTLY;
		pJerrySoldier->bVisible = TRUE;
	}

	}

	//Lock the interface
	guiPendingOverrideEvent = LU_BEGINUILOCK;
	}
}

void AddItemToMerc( UINT8 ubNewMerc, INT16 sItemType )
{


	// OK, give this item to our merc!
	OBJECTTYPE gTempObject;
	BOOLEAN	fReturn=FALSE;

	// make an objecttype
        CreateItem(sItemType, 100, &gTempObject);

	// Give it 
	fReturn = AutoPlaceObject( MercPtrs[ ubNewMerc ], &gTempObject, FALSE );
	
			if(!fReturn && (UsingNewInventorySystem() == true))
			{
				(MercPtrs[ubNewMerc]->inv[NUM_INV_SLOTS-1]) = gTempObject;
				fReturn=TRUE;
			}
	Assert( fReturn );
	

}
#endif
