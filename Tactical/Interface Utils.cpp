	#include "builddefines.h"
	#include <stdio.h>
	#include "sgp.h"
	#include "himage.h"
	#include "vobject.h"
	#include "interface utils.h"
	#include "render dirty.h"
	#include "interface.h"
	#include "sysutil.h"
	#include "faces.h"
	#include "render dirty.h"
	#include "weapons.h"
	#include "Overhead.h"
	#include "Soldier macros.h"
	#include "line.h"
	#include "WCheck.h"
	#include "Vehicles.h"
	#include "GameSettings.h"
	#include "Utilities.h"	// added by Flugente

#define			LIFE_BAR_SHADOW						FROMRGB( 108, 12, 12 )
#define			LIFE_BAR							FROMRGB( 200, 0, 0 )
#define			BANDAGE_BAR_SHADOW					FROMRGB( 156, 60, 60 )
#define			BANDAGE_BAR							FROMRGB( 222, 132, 132 )
#define			BLEEDING_BAR_SHADOW					FROMRGB( 128, 128, 60 )
#define			BLEEDING_BAR						FROMRGB( 240, 240, 20 )

#define			CURR_BREATH_BAR_SHADOW				FROMRGB( 17,	24, 170 ) // the lt blue current breath
#define			CURR_BREATH_BAR						FROMRGB( 46,	51, 243 )
#define			CURR_MAX_BREATH						FROMRGB( 0,		0,	0	) // the current max breath, black
#define			CURR_MAX_BREATH_SHADOW				FROMRGB( 0,		0,	0	)
#define			MORALE_BAR_SHADOW					FROMRGB( 8,		112, 12 )
#define			MORALE_BAR							FROMRGB( 8,		180, 12 )
#define			BREATH_BAR_SHADOW					FROMRGB( 60,	108, 108 ) // the MAX max breath, always at 100%
#define			BREATH_BAR							FROMRGB( 113,	178, 218 )
#define			BREATH_BAR_SHAD_BACK				FROMRGB( 1,1,1 )
#define			FACE_WIDTH							48
#define			FACE_HEIGHT							43


// backgrounds for breath max background
extern UINT32 guiBrownBackgroundForTeamPanel;
extern UINT32 guiGoldBackgroundForTeamPanel;

// selected grunt
extern UINT16 gusSelectedSoldier;

// car portraits
enum{
	ELDORADO_PORTRAIT =0,
	HUMMER_PORTRAIT,
	ICE_CREAM_TRUCT_PORTRAIT,
	JEEP_PORTRAIT,
	NUMBER_CAR_PORTRAITS,
};

// the ids for the car portraits
//INT32 giCarPortraits[ 4 ] = { -1, -1, -1, -1 };

INT32 giCarPortraits[ NUM_PROFILES ];

// the car portrait file names
STR pbCarPortraitFileNames[ ]={
	"INTERFACE\\eldorado.sti",
	"INTERFACE\\Hummer.sti",
	"INTERFACE\\ice Cream Truck.sti",
	"INTERFACE\\Jeep.sti",
};

// load int he portraits for the car faces that will be use in mapscreen
BOOLEAN LoadCarPortraitValues( void )
{
	VOBJECT_DESC	 VObjectDesc;
	
	for ( INT32 iCounter = 0; iCounter < NUM_PROFILES; ++iCounter )
	{
		// silversurfer: fixed to make sure that we only create objects for vehicles that have a face defined
		// otherwise CHECKF will fail and return FALSE breaking the for loop and ignoring any further vehicles
		if ( gMercProfiles[iCounter].Type == PROFILETYPE_VEHICLE && gNewVehicle[ iCounter ].szIconFace[0] != 0 )
		{
			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			strcpy( VObjectDesc.ImageFile, gNewVehicle[ iCounter ].szIconFace );
			CHECKF( AddVideoObject( &VObjectDesc, (UINT32 *)&giCarPortraits[ iCounter ] ) );
		}
	}
	
	return( TRUE );
}

// get rid of the images we loaded for the mapscreen car portraits
void UnLoadCarPortraits( void )
{
	for ( INT32 iCounter = 0; iCounter < NUM_PROFILES; ++iCounter )
	{
		if ( gMercProfiles[iCounter].Type == PROFILETYPE_VEHICLE )
		{
			DeleteVideoObjectFromIndex( giCarPortraits[ iCounter ] );
		}
	}
}

void DrawLifeUIBarEx( SOLDIERTYPE *pSoldier, INT16 sXPos, INT16 sYPos, INT16 sWidth, INT16 sHeight, BOOLEAN fErase, UINT32 uiBuffer )
{
	FLOAT											dStart, dEnd, dPercentage;
	//UINT16										usLineColor;

	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	UINT16										usLineColor;
	INT8											bBandage;

	// Erase what was there
	if( fErase )
	{
		RestoreExternBackgroundRect( sXPos, (INT16)(sYPos - sHeight), sWidth, (INT16)(sHeight + 1 ) );
	}

	if( pSoldier->stats.bLife == 0 )
	{
		// are they dead?
		return;
	}

	pDestBuf = LockVideoSurface( uiBuffer, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	dStart			= sYPos;
	dEnd			= 0;
	
	// FIRST DO MAX LIFE
	dPercentage = (FLOAT)pSoldier->stats.bLife / (FLOAT)100;
	FLOAT	dstart1		= dStart - dEnd;
	dStart				= (FLOAT)( dStart - dEnd );
	dEnd				=	dPercentage * sHeight;
	
	usLineColor = Get16BPPColor( LIFE_BAR_SHADOW );
	RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( LIFE_BAR );
	RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( LIFE_BAR_SHADOW );
	RectangleDraw( TRUE, sXPos+ 2, (INT32)dStart, sXPos + 2, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );
	
	// NOW DO BANDAGE
		
	// Calculate bandage
	FLOAT	dstart2		= dStart - dEnd;
	bBandage = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

	if ( bBandage )
	{
		dPercentage = (FLOAT)bBandage / (FLOAT)100;
		dStart			= (FLOAT)( dStart - dEnd );
		dEnd				=	( dPercentage * sHeight );

		usLineColor = Get16BPPColor( BANDAGE_BAR_SHADOW );
		RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

		usLineColor = Get16BPPColor( BANDAGE_BAR );
		RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

		usLineColor = Get16BPPColor( BANDAGE_BAR_SHADOW );
		RectangleDraw( TRUE, sXPos+ 2, (INT32)dStart, sXPos + 2, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );
	}

	// NOW DO BLEEDING
	FLOAT	dstart3		= dStart - dEnd;
	if ( pSoldier->bBleeding )
	{
		dPercentage = (FLOAT)pSoldier->bBleeding / (FLOAT)100;
		dStart			= (FLOAT)( dStart - dEnd );
		dEnd				=	( dPercentage * sHeight );

		usLineColor = Get16BPPColor( BLEEDING_BAR_SHADOW );
		RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

		usLineColor = Get16BPPColor( BLEEDING_BAR );
		RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

		usLineColor = Get16BPPColor( BLEEDING_BAR_SHADOW );
		RectangleDraw( TRUE, sXPos+ 2, (INT32)dStart, sXPos + 2, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );
	}

	UnLockVideoSurface( uiBuffer );
}


void DrawBreathUIBarEx( SOLDIERTYPE *pSoldier, INT16 sXPos, INT16 sYPos, INT16 sWidth, INT16 sHeight, BOOLEAN fErase, UINT32 uiBuffer )
{
	FLOAT										dStart, dEnd, dPercentage;
	//UINT16										usLineColor;

	UINT32										uiDestPitchBYTES;
	UINT8										*pDestBuf;
	UINT16										usLineColor;
	HVOBJECT									hHandle;

	// Erase what was there
	if ( fErase )
	{
		RestoreExternBackgroundRect( sXPos, (INT16)(sYPos - sHeight), sWidth, (INT16)(sHeight + 1 ) );
	}

	if( pSoldier->stats.bLife == 0 )
	{
		// are they dead?
		return;
	}

	//	DO MAX MAX BREATH
	dPercentage = 1.;
	dEnd		= dPercentage * sHeight;
	dStart		= sYPos;

	// brown guy
	GetVideoObject( &hHandle, guiBrownBackgroundForTeamPanel );

	// Flugente: for some reason, the image has sometimes been deleted. In that case, try again
	if ( !hHandle )
	{
		VOBJECT_DESC	VObjectDesc;
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP( "INTERFACE\\Bars.sti", VObjectDesc.ImageFile );
		CHECKV( AddVideoObject( &VObjectDesc, &guiBrownBackgroundForTeamPanel ) );

		// brown guy
		GetVideoObject( &hHandle, guiBrownBackgroundForTeamPanel );
	}

	// DO MAX BREATH
	if( guiCurrentScreen != MAP_SCREEN )
	{
		if( gusSelectedSoldier == pSoldier->ubID && gTacticalStatus.ubCurrentTeam == OUR_TEAM && OK_INTERRUPT_MERC( pSoldier ) )
		{
			// gold, the second entry in the .sti
			BltVideoObject( uiBuffer , hHandle, 1, sXPos, ( INT16 )( sYPos - sHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
		}
		else
		{
			// brown, first entry
			BltVideoObject( uiBuffer , hHandle, 0, sXPos, ( INT16 )( sYPos - sHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
		}
	}
	else
	{
		// brown, first entry
		BltVideoObject( uiBuffer , hHandle, 0, sXPos, ( INT16 )( sYPos - sHeight ), VO_BLT_SRCTRANSPARENCY, NULL );
	}

	pDestBuf = LockVideoSurface( uiBuffer, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	if( pSoldier->bBreathMax <= 97 )
	{
		dPercentage = (FLOAT)( ( pSoldier->bBreathMax + 3 ) )/ (FLOAT)100;
		dEnd				=	dPercentage * sHeight;
		dStart			= sYPos;

		// the old background colors for breath max diff
		usLineColor = Get16BPPColor( BREATH_BAR_SHAD_BACK );
		RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

		usLineColor = Get16BPPColor( BREATH_BAR_SHAD_BACK );
		RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

		usLineColor = Get16BPPColor( BREATH_BAR_SHAD_BACK );
		RectangleDraw( TRUE, sXPos+ 2, (INT32)dStart, sXPos + 2, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );
	}

	dPercentage = (FLOAT)pSoldier->bBreathMax / (FLOAT)100;
	dEnd				=	dPercentage * sHeight;
	dStart			= sYPos;

	usLineColor = Get16BPPColor( CURR_MAX_BREATH_SHADOW );
	RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( CURR_MAX_BREATH );
	RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( CURR_MAX_BREATH_SHADOW );
	RectangleDraw( TRUE, sXPos+ 2, (INT32)dStart, sXPos + 2, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

	// NOW DO BREATH
	dPercentage = (FLOAT)pSoldier->bBreath / (FLOAT)100;
	dEnd				=	dPercentage * sHeight;
	dStart			= sYPos;

	usLineColor = Get16BPPColor( CURR_BREATH_BAR_SHADOW );
	RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( CURR_BREATH_BAR );
	RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( CURR_BREATH_BAR_SHADOW );
	RectangleDraw( TRUE, sXPos+ 2, (INT32)dStart, sXPos + 2, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );
	
	UnLockVideoSurface( uiBuffer );
}

void DrawMoraleUIBarEx( SOLDIERTYPE *pSoldier, INT16 sXPos, INT16 sYPos, INT16 sWidth, INT16 sHeight, BOOLEAN fErase, UINT32 uiBuffer )
{
	FLOAT											dStart, dEnd, dPercentage;
	//UINT16										usLineColor;

	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	UINT16										usLineColor;

	// Erase what was there
	if ( fErase )
	{
		RestoreExternBackgroundRect( sXPos, (INT16)(sYPos - sHeight), sWidth, (INT16)(sHeight + 1 ) );
	}

	if( pSoldier->stats.bLife == 0 )
	{
		// are they dead?
		return;
	}

	pDestBuf = LockVideoSurface( uiBuffer, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	// FIRST DO BREATH
	dPercentage = (FLOAT)pSoldier->aiData.bMorale / (FLOAT)100;
	dEnd				=	dPercentage * sHeight;
	dStart			= sYPos;

	usLineColor = Get16BPPColor( MORALE_BAR_SHADOW );
	RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( MORALE_BAR );
	RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

	usLineColor = Get16BPPColor( MORALE_BAR_SHADOW );
	RectangleDraw( TRUE, sXPos+ 2, (INT32)dStart, sXPos + 2, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );
	
	UnLockVideoSurface( uiBuffer );
}


void DrawItemUIBarEx( OBJECTTYPE *pObject, UINT8 ubStatus, INT16 sXPos, INT16 sYPos, INT16 sWidth, INT16 sHeight, INT16 sColor1, INT16 sColor2, BOOLEAN fErase, UINT32 uiBuffer, UINT8 iter )
{
	FLOAT										dStart, dEnd, dPercentage;
	//UINT16										usLineColor;

	UINT32										uiDestPitchBYTES;
	UINT8										*pDestBuf;
	UINT16										usLineColor;
	INT16										sValue;
	
	if ( ubStatus >= DRAW_ITEM_STATUS_ATTACHMENT1 )
	{
		sValue = 0;	
		OBJECTTYPE* pAttachment = (*pObject)[iter]->GetAttachmentAtIndex( ubStatus - DRAW_ITEM_STATUS_ATTACHMENT1 );
		if (pAttachment->exists())
		{
			sValue = (*pAttachment)[iter]->data.objectStatus;

			if( Item[ pAttachment->usItem ].usItemClass & IC_AMMO )
			{
				sValue = sValue * 100 / Magazine[ Item[ pObject->usItem ].ubClassIndex ].ubMagSize;
				sValue = max(0, min(100, sValue));
			}
		}
	}
	else
	{
		if (ubStatus < pObject->ubNumberOfObjects)
			sValue = (*pObject)[ ubStatus ]->data.objectStatus;
		else
			sValue = 0;
	}

	// Adjust for ammo, other thingys..
	if( Item[ pObject->usItem ].usItemClass & IC_AMMO )
	{
		sValue = sValue * 100 / Magazine[ Item[ pObject->usItem ].ubClassIndex ].ubMagSize;

		if ( sValue < 0 )
		{
			sValue = (*pObject)[iter]->data.ubShotsLeft * 100 / Magazine[ Item[ pObject->usItem ].ubClassIndex ].ubMagSize;
			DebugMsg ( TOPIC_JA2, DBG_LEVEL_3, String("Ammo status: shots left %d * 100 / Mag Size %d = value %d",(*pObject)[iter]->data.ubShotsLeft,Magazine[ Item[ pObject->usItem ].ubClassIndex ].ubMagSize,sValue ));
		}

		if ( sValue > 100 )
			sValue = 100;
	}

	if( Item[ pObject->usItem ].usItemClass & IC_KEY )
		sValue =100;

	// Flugente
	if ( ubStatus == DRAW_ITEM_TEMPERATURE )
	{
		// the food item bar always has full size
		if ( UsingFoodSystem() && Item[pObject->usItem].foodtype > 0 )
		{
			sValue = 100;
		}
		else
		{
			// sevenfm: use GetGunOverheatDisplayPercentage so that OVERHEATING_DISPLAY_JAMPERCENTAGE option can have effect
			//sValue = (INT16)(100 * GetGunOverheatJamPercentage(pObject));
			sValue = (INT16)(100 * GetGunOverheatDisplayPercentage(pObject));

			// if temperature is 0 or below, do not display anything
			if ( sValue < 1)
				return;

			// cut off temperature at 100%, otherwise the bar will be out of its box
			sValue = min(sValue, 100);
		}
	}

	// ATE: Subtract 1 to exagerate bad status
	if ( sValue < 100 && sValue > 1 )
	{
		--sValue;
	}

	// Erase what was there
	/*if ( fErase )
	{
		RestoreExternBackgroundRect( sXPos, (INT16)(sYPos - sHeight), sWidth, (INT16)(sHeight + 1 ) );
	}*/

	pDestBuf = LockVideoSurface( uiBuffer, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		
	// FIRST DO BREATH
	dPercentage = (FLOAT)sValue / (FLOAT)100;
	dEnd				=	dPercentage * sHeight;
	dStart			= sYPos;

	// Flugente: draw sRepairThreshold in the background
	if ( gGameExternalOptions.fAdvRepairSystem && ubStatus != DRAW_ITEM_TEMPERATURE && Item[pObject->usItem].usItemClass & (IC_WEAPON|IC_ARMOUR) )
	{
		FLOAT repairthresholdend = sHeight * (FLOAT)(*pObject)[0]->data.sRepairThreshold / (FLOAT)100;
		
		usLineColor =  Get16BPPColor( FROMRGB( 57, 56, 41 ) );
		RectangleDraw( TRUE, sXPos, (INT32)(dStart - repairthresholdend), sXPos, (INT32)( dStart - sHeight ) , usLineColor, pDestBuf );

		usLineColor = Get16BPPColor( FROMRGB( 57, 56, 41 ) );
		RectangleDraw( TRUE, sXPos + 1, (INT32)(dStart - repairthresholdend), sXPos + 1, (INT32)( dStart - sHeight ) , usLineColor, pDestBuf );
	}

	usLineColor = sColor1;
	RectangleDraw( TRUE, sXPos, (INT32)dStart, sXPos, (INT32)( dStart - dEnd ) , usLineColor, pDestBuf );

	usLineColor = sColor2;
	RectangleDraw( TRUE, sXPos+ 1, (INT32)dStart, sXPos + 1, (INT32)( dStart - dEnd ), usLineColor, pDestBuf );

	UnLockVideoSurface( uiBuffer );

	if ( uiBuffer == guiSAVEBUFFER )
	{
		RestoreExternBackgroundRect( sXPos, (INT16)(sYPos - sHeight), sWidth, (INT16)(sHeight + 1 ) );
	}
	else
	{
		InvalidateRegion( sXPos, (INT16)(sYPos - sHeight), sXPos + sWidth, (INT16)(sYPos + 1 ) );
	}
}

void RenderSoldierFace( SOLDIERTYPE *pSoldier, INT16 sFaceX, INT16 sFaceY, BOOLEAN fAutoFace )
{
	BOOLEAN fDoFace = FALSE;
//	UINT8 ubVehicleType = 0;

	if ( pSoldier->bActive )
	{
		if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			// get the type of vehicle
//			ubVehicleType = pVehicleList[ pSoldier->bVehicleID ].ubVehicleType;

			// just draw the vehicle
//			BltVideoObjectFromIndex( guiSAVEBUFFER, giCarPortraits[ ubVehicleType ], 0, sFaceX, sFaceY, VO_BLT_SRCTRANSPARENCY, NULL );
			BltVideoObjectFromIndex( guiSAVEBUFFER, giCarPortraits[ pSoldier->ubProfile ], 0, sFaceX, sFaceY, VO_BLT_SRCTRANSPARENCY, NULL );
			RestoreExternBackgroundRect( sFaceX, sFaceY, FACE_WIDTH, FACE_HEIGHT );

			return;
		}

		if ( fAutoFace )
		{
			// OK, check if this face actually went active...
			if ( gFacesData[ pSoldier->iFaceIndex ].uiFlags & FACE_INACTIVE_HANDLED_ELSEWHERE )
			{
				// Render as an extern face...
				fAutoFace = FALSE;
			}
			else
			{
				SetAutoFaceActiveFromSoldier( FRAME_BUFFER, guiSAVEBUFFER, pSoldier->ubID, sFaceX, sFaceY );
			//	SetAutoFaceActiveFromSoldier( FRAME_BUFFER, FACE_AUTO_RESTORE_BUFFER, pSoldier->ubID , sFaceX, sFaceY );
			}
		}

		fDoFace = TRUE;

		if ( fDoFace )
		{
			if ( fAutoFace )
			{
				RenderAutoFaceFromSoldier( pSoldier->ubID );
			}
			else
			{
				ExternRenderFaceFromSoldier( guiSAVEBUFFER, pSoldier->ubID, sFaceX, sFaceY );
			}
		}
	}
	else
	{
		// Put close panel there
		//if(gbPixelDepth==16)
		//{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiCLOSE, 5, sFaceX, sFaceY, VO_BLT_SRCTRANSPARENCY, NULL );
		//}
		RestoreExternBackgroundRect( sFaceX, sFaceY, FACE_WIDTH, FACE_HEIGHT );
	}
}

// HEADROCK HAM 5: This function draws a rectangle around the item image in the zoomed inventory
void DrawItemOutlineZoomedInventory( INT16 sX, INT16 sY, INT16 sWidth, INT16 sHeight, INT16 sColor, UINT32 uiBuffer )
{
	UINT32 uiDestPitchBYTES;
	UINT8 *pDestBuf;

	pDestBuf = LockVideoSurface( uiBuffer, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	RectangleDraw( TRUE, sX, sY, sWidth, sHeight, sColor, pDestBuf );

	UnLockVideoSurface( uiBuffer );
}
