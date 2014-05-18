// anv: totally not a copy of SkillMenu.cpp

#include "VehicleMenu.h"
#include "Soldier Profile Type.h"
#include "Overhead.h"
#include "Text.h"
#include "Isometric Utils.h"
#include "DisplayCover.h"
#include "worldman.h"
#include "Vehicles.h"
#include "soldier macros.h"
#include "Soldier Add.h"
#include "PATHAI.h"
#include "Points.h"
#include "Handle UI.h"
#include "Squads.h"
#include "Font Control.h"

// sevenfm: need this for correct calculation of vehicle menu position
extern INT16 gsInterfaceLevel;

UINT16 usVehicleMenuPosX = 0;
UINT16 usVehicleMenuPosY = 0;

INT32	sVehicleMenuTargetGridNo = NOWHERE;

SOLDIERTYPE *pCurrentSoldier = NULL;
SOLDIERTYPE *pCurrentVehicle = NULL;

void HideVehicleMenu( void )
{
	// Dirty!
	fMapPanelDirty = TRUE;
}

// Turns an option off by always returning FALSE
BOOLEAN Popup_VehicleMenuOptionOff( void )	{ return FALSE; }

void
VehicleMenuItem::SetupPopup(CHAR* name)
{
	// create a popup
	gPopup = new POPUP(name);
	
	// add a callback that lets the keyboard handler know we're done (and ready to pop up again)
	gPopup->setCallback(POPUP_CALLBACK_HIDE, new popupCallbackFunction<void, void>( &HideVehicleMenu ) );
}

//	Different VehicleItem classes. We need wrappers for each one, as we cannot pass a pointer to a member function (we need a concrete object)

/////////////////////////////// Vehicle Selection ////////////////////////////////////////////
VehicleSelection	gVehicleSelection;

void Wrapper_Function_VehicleSelection( UINT32 aVal )	{	gVehicleSelection.Functions(aVal);	}
void Wrapper_Setup_VehicleSelection( UINT32 aVal )		{	gVehicleSelection.Setup(aVal);	}
void Wrapper_Cancel_VehicleSelection( UINT32 aVal )		{	gVehicleSelection.Cancel();	}
/////////////////////////////// Vehicle Selection ////////////////////////////////////////////

/////////////////////////////// Vehicle Selection ////////////////////////////////////////////
void
VehicleSelection::Setup( UINT32 aVal )
{
	Destroy();

	if ( pCurrentSoldier == NULL )
		return;

	if ( pCurrentVehicle == NULL )
		return;

	SetupPopup("VehicleSelection");

	POPUP_OPTION *pOption;
	
	CHAR16 pStr[300];

	// create entries for each vehicle seat
	INT8 bVehicleID = pCurrentVehicle->bVehicleID;
	INT8 bSeatIndex = GetSeatIndexFromSoldier( pCurrentSoldier );
	INT32 iSeatingCapacity = gNewVehicle[ pVehicleList[ bVehicleID ].ubVehicleType ].iNewSeatingCapacities;

	for ( int i = 0; i < iSeatingCapacity; ++i)
	{
		swprintf( pStr, gNewVehicle[ pVehicleList[ bVehicleID ].ubVehicleType ].VehicleSeats[i].zSeatName );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Function_VehicleSelection, i ));

		// if seat is already taken, grey it out
		if ( pVehicleList[ bVehicleID ].pPassengers[ i ] != NULL )
		{			
			if( pCurrentSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
			{
				if( pVehicleList[ bVehicleID ].pPassengers[ i ] == pCurrentSoldier )
				{
					// Set this option off.
					pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_VehicleMenuOptionOff, NULL ));
				}
				else
				{
					// keep, but colour in yellow
					pOption->color_foreground = FONT_YELLOW;
				}
			}
			else
			{
				// Set this option off.
				pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_VehicleMenuOptionOff, NULL ));
			}			
		}

		// block swapping between different compartments
		if( gTacticalStatus.uiFlags & INCOMBAT )
		{
			if( bSeatIndex != (-1) )
			{
				if( gNewVehicle[pVehicleList[ pCurrentVehicle->bVehicleID ].ubVehicleType].VehicleSeats[bSeatIndex].ubCompartment !=
					gNewVehicle[pVehicleList[ pCurrentVehicle->bVehicleID ].ubVehicleType].VehicleSeats[i].ubCompartment )
				{
					// Set this option off.
					pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_VehicleMenuOptionOff, NULL ));
				}
			}
		}

		GetPopup()->addOption( *pOption );
	}

	// cancel option
	swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Cancel_VehicleSelection, 0 ) );
	GetPopup()->addOption( *pOption );
		
	// grab soldier's x,y screen position
	INT16 sX, sY;
	GetGridNoScreenPos( sVehicleMenuTargetGridNo, gsInterfaceLevel, &sX, &sY );
		
	if( sX < 0 ) sX = 0;
	if( sY < 0 ) sY = 0;

	usVehicleMenuPosX = sX + 30;		
	usVehicleMenuPosY = sY;

	if ( ( usVehicleMenuPosX + 200 ) > SCREEN_WIDTH )
		usVehicleMenuPosX = SCREEN_WIDTH - 200;

	if ( ( usVehicleMenuPosY + 130 ) > SCREEN_HEIGHT )
		usVehicleMenuPosY = SCREEN_HEIGHT - 190;

	SetPos(usVehicleMenuPosX, usVehicleMenuPosY);
}

void
VehicleSelection::Functions( UINT32 aVal  )
{
	INT32 sActionGridNo;
	UINT8 ubDirection;
	INT16 sAPCost = 0;
	INT32 sTempSelectedSoldier;

	Cancel();

	if( pCurrentSoldier->flags.uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) && pCurrentSoldier->iVehicleId == pCurrentVehicle->bVehicleID )
	{
		if( SwapVehicleSeat( pCurrentVehicle, pCurrentSoldier, aVal ) )
		{
			sTempSelectedSoldier = gusSelectedSoldier;
			SetCurrentSquad( CurrentSquad(), TRUE );
			gusSelectedSoldier = sTempSelectedSoldier;
		}
	}
	else if ( OK_ENTERABLE_VEHICLE( pCurrentVehicle ) && pCurrentVehicle->bVisible != -1 && OKUseVehicle( pCurrentVehicle->ubProfile ) )
	{
		// Find a gridno closest to sweetspot...
		sActionGridNo = FindGridNoFromSweetSpotWithStructDataFromSoldier( pCurrentSoldier, pCurrentSoldier->usUIMovementMode, 5, &ubDirection, 0, pCurrentVehicle, TRUE );
				
		if (!TileIsOutOfBounds(sActionGridNo))
		{
			// Calculate AP costs...
			//sAPCost = GetAPsToBeginFirstAid( pSoldier );
			sAPCost += PlotPath( pCurrentSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pCurrentSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pCurrentSoldier->bActionPoints);

			if ( EnoughPoints( pCurrentSoldier, sAPCost, 0, TRUE ) )
			{
				pCurrentSoldier->DoMercBattleSound( BATTLE_SOUND_OK1 );

				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pCurrentSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pCurrentSoldier->aiData.ubPendingAction = MERC_ENTER_VEHICLE;
					pCurrentSoldier->aiData.sPendingActionData2	= pCurrentVehicle->sGridNo;
					pCurrentSoldier->aiData.bPendingActionData3	= ubDirection;
					pCurrentSoldier->aiData.uiPendingActionData4	= aVal;
					pCurrentSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pCurrentSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pCurrentSoldier->usUIMovementMode, 3 , pCurrentSoldier->flags.fNoAPToFinishMove );
				}
				else
				{
					pCurrentSoldier->EVENT_SoldierEnterVehicle( pCurrentVehicle->sGridNo, ubDirection, aVal );
				}

				// OK, set UI
				SetUIBusy( pCurrentSoldier->ubID );
				//guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
			}
		}
	}
	
	Cancel();
	gVehicleSelection.Cancel();
}
/////////////////////////////// Vehicle Selection ////////////////////////////////////////////

void VehicleMenu( INT32 usMapPos, SOLDIERTYPE *pSoldier, SOLDIERTYPE *pVehicle )
{
	sVehicleMenuTargetGridNo = usMapPos;
	pCurrentSoldier = pSoldier;
	pCurrentVehicle = pVehicle;
	gVehicleSelection.Setup(0);
}