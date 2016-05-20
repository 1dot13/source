#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
#include "builddefines.h"
#include <stdio.h>
#include "Types.h"
#include "civ quotes.h"
#include "mousesystem.h"
#include "strategicmap.h"
#include "WCheck.h"
#include "FileMan.h"
#include "encrypted file.h"
#include "MessageBoxScreen.h"
#include "Queen Command.h"
#include "Overhead.h"
#include "render dirty.h"
#include "merctextbox.h"
#include "ai.h"
#include "Text.h"
#include "screenids.h"
#include "Animation Data.h"
#include "Video.h"
#include "Font Control.h"
#include "message.h"
#include "local.h"
#include "renderworld.h"
#include "Interface.h"
#include "cursors.h"
#include "Dialogue Control.h"
#include "Quests.h"
#include "Strategic Town Loyalty.h"
#include "NPC.h"
#include "Strategic Mines.h"
#include "Random.h"
#endif
#include "connect.h"

// for enemy taunts
#include "Soldier Profile.h"
#include "Campaign.h"
#include "opplist.h"

#define			DIALOGUE_DEFAULT_WIDTH			200
#define			EXTREAMLY_LOW_TOWN_LOYALTY	20
#define			HIGH_TOWN_LOYALTY						80
#define			CIV_QUOTE_HINT							99

#define			MAX_APPLICABLE_TAUNTS			512

extern void CaptureTimerCallback( void );

BOOLEAN gfSurrendered = FALSE;

//--------------------------------------------------------------
//Not used 
typedef struct
{
	UINT16	ubNumEntries;
	UINT16	ubUnusedCurrentEntry; //Unused

} CIV_QUOTE;

CIV_QUOTE	gCivQuotes[ NUM_CIV_QUOTES]; //Not used 

UINT16	gubNumEntries[ NUM_CIV_QUOTES ] = // Not used 
{
	15,
	15,
	15,
	15,
	15,
	15,
	15,
	15,
	15,
	15,

	15,
	15,
	15,
	15,
	15,
	15,
	15,
	15,
	15,
	15,

	5,
	5,
	15,
	15,
	15,
	15,
	15,
	15,
	15,
	15,

	15,
	15,
	2,
	15,
	15,
	10,
	10,
	5,
	3,
	10,

	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3
};
//--------------------------------------------------------------

typedef struct
{
	BOOLEAN				bActive;
	MOUSE_REGION	MouseRegion;
	INT32					iVideoOverlay;
	INT32					iDialogueBox;
	UINT32				uiTimeOfCreation;
	UINT32				uiDelayTime;
	SOLDIERTYPE *	pCiv;
} QUOTE_SYSTEM_STRUCT;


QUOTE_SYSTEM_STRUCT	gCivQuoteData;

CHAR16	gzCivQuote[ 320 ];
UINT16	gusCivQuoteBoxWidth;
UINT16	gusCivQuoteBoxHeight;

// anv: store times, when enemy taunt will be finished (so they won't taunt 50 times / second)
UINT32	uiTauntFinishTimes[ TOTAL_SOLDIERS ];

TAUNT_VALUES zApplicableTaunts[NUM_TAUNT];

//--------------------------------------------------------------
void CopyNumEntriesIntoQuoteStruct( ) //  Not used 
{
	INT32	cnt;

	for ( cnt = 0; cnt < NUM_CIV_QUOTES; ++cnt )
	{
		if (cnt <= 50) 
			gCivQuotes[ cnt ].ubNumEntries = gubNumEntries[ cnt ];
		else 
			gCivQuotes[ cnt ].ubNumEntries = 15;
	}

}
//--------------------------------------------------------------

BOOLEAN GetCivQuoteText(UINT16 ubCivQuoteID, UINT16 ubEntryID, STR16 zQuote )
{
	CHAR8 zFileName[164];

	// Build filename....
	if ( ubCivQuoteID == CIV_QUOTE_HINT )
	{
		if ( gbWorldSectorZ > 0 )
		{
			//sprintf( zFileName, "NPCData\\miners.edt" );
				sprintf( zFileName,"NPCDATA\\CIV%02d.edt", CIV_QUOTE_MINERS_NOT_FOR_PLAYER );
		}
		else
		{
			sprintf( zFileName, "NPCData\\%c%d.edt", 'A' + (gWorldSectorY - 1) , gWorldSectorX );
		}
	}
	else
	{
		if (ubCivQuoteID <= 9)
			sprintf( zFileName,"NPCDATA\\CIV%02d.edt",ubCivQuoteID );
		else
			sprintf( zFileName,"NPCDATA\\CIV%d.edt",ubCivQuoteID );
	}

	CHECKF( FileExists( zFileName ) );

	// Get data...
	LoadEncryptedDataFromFile( zFileName, zQuote, ubEntryID * 320, 320 );

	if( zQuote[0] == 0 )
	{
		return( FALSE );
	}

	return( TRUE );
}

void SurrenderMessageBoxCallBack( UINT8 ubExitValue )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32				cnt = 0;

	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		// CJC Dec 1 2002: fix multiple captures
		BeginCaptureSquence();

	// Do capture....
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

		for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++)
		{
			// Are we active and in sector.....
			if ( pTeamSoldier->bActive && pTeamSoldier->bInSector )
			{
				if ( pTeamSoldier->stats.bLife != 0 )
				{
					EnemyCapturesPlayerSoldier( pTeamSoldier );

					RemoveSoldierFromTacticalSector( pTeamSoldier, TRUE );
				}
			}
		}

		EndCaptureSequence( );

		gfSurrendered = TRUE;
		SetCustomizableTimerCallbackAndDelay( 3000, CaptureTimerCallback, FALSE );

		ActionDone( gCivQuoteData.pCiv );
	}
	else
	{
		ActionDone( gCivQuoteData.pCiv );
	}
}

void ShutDownQuoteBox( BOOLEAN fForce )
{
	if ( !gCivQuoteData.bActive )
	{
		return;
	}

	// Check for min time....
	if ( ( GetJA2Clock( ) - gCivQuoteData.uiTimeOfCreation ) > 300 || fForce )
	{
		RemoveVideoOverlay( gCivQuoteData.iVideoOverlay );

		// Remove mouse region...
		MSYS_RemoveRegion( &(gCivQuoteData.MouseRegion) );

		RemoveMercPopupBoxFromIndex( gCivQuoteData.iDialogueBox );
		gCivQuoteData.iDialogueBox = -1;

		gCivQuoteData.bActive = FALSE;
#ifdef JA2UB
// no UB
#else
		// do we need to do anything at the end of the civ quote?
		if ( gCivQuoteData.pCiv && gCivQuoteData.pCiv->aiData.bAction == AI_ACTION_OFFER_SURRENDER )
		{
// Haydent
			if(!is_networked)
			{
				DoMessageBox( MSG_BOX_BASIC_STYLE, Message[ STR_SURRENDER ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, SurrenderMessageBoxCallBack, NULL );
			}
			else 
			{
				ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[39] );
				ActionDone( gCivQuoteData.pCiv );
			}
		}
#endif
	}
}

BOOLEAN ShutDownQuoteBoxIfActive( )
{
	if ( gCivQuoteData.bActive )
	{
		ShutDownQuoteBox( TRUE );

		return( TRUE );
	}

	return( FALSE );
}


INT8 GetCivType( SOLDIERTYPE *pCiv )
{
	if ( pCiv->ubProfile != NO_PROFILE )
	{
		return( CIV_TYPE_NA );
	}

	// ATE: Check if this person is married.....
	// 1 ) check sector....
	if ( gWorldSectorX == 10 && gWorldSectorY == 6 && gbWorldSectorZ == 0 )
	{
		// 2 ) the only female....
		if ( pCiv->ubCivilianGroup == 0 && pCiv->bTeam != gbPlayerNum && pCiv->ubBodyType == REGFEMALE )
		{
			// She's a ho!
			return( CIV_TYPE_MARRIED_PC );
		}
	}

	// OK, look for enemy type - MUST be on enemy team, merc bodytype
	if ( pCiv->bTeam == ENEMY_TEAM && IS_MERC_BODY_TYPE( pCiv ) )
	{
		return( CIV_TYPE_ENEMY );
	}

	if ( pCiv->bTeam != CIV_TEAM && pCiv->bTeam != MILITIA_TEAM )
	{
		return( CIV_TYPE_NA );
	}

	switch( pCiv->ubBodyType )
	{
		case REGMALE:
		case BIGMALE:
		case STOCKYMALE:
		case REGFEMALE:
		case FATCIV:
		case MANCIV:
		case MINICIV:
		case DRESSCIV:
		case CRIPPLECIV:

			return( CIV_TYPE_ADULT );
			break;

		case ADULTFEMALEMONSTER:
		case AM_MONSTER:
		case YAF_MONSTER:
		case YAM_MONSTER:
		case LARVAE_MONSTER:
		case INFANT_MONSTER:
		case QUEENMONSTER:

			return( CIV_TYPE_NA );

		case HATKIDCIV:
		case KIDCIV:

			return( CIV_TYPE_KID );

		default:

			return( CIV_TYPE_NA );
	}

	//return( CIV_TYPE_NA ); // not needed when there is a default! (jonathanl)
}


void RenderCivQuoteBoxOverlay( VIDEO_OVERLAY *pBlitter )
{
	if ( gCivQuoteData.iVideoOverlay != -1 )
	{
		RenderMercPopUpBoxFromIndex( gCivQuoteData.iDialogueBox, pBlitter->sX, pBlitter->sY,	pBlitter->uiDestBuff );

		InvalidateRegion( pBlitter->sX, pBlitter->sY, pBlitter->sX + gusCivQuoteBoxWidth, pBlitter->sY + gusCivQuoteBoxHeight );
	}
}


void QuoteOverlayClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	static BOOLEAN fLButtonDown = FALSE;

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fLButtonDown = TRUE;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP && fLButtonDown )
	{
		// Shutdown quote box....
		ShutDownQuoteBox( FALSE );
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fLButtonDown = FALSE;
	}
}


void BeginCivQuote( SOLDIERTYPE *pCiv, UINT16 ubCivQuoteID, UINT16 ubEntryID, INT16 sX, INT16 sY )
{
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;
	CHAR16					zQuote[ 320 ];	
	
	// OK, do we have another on?
	if ( gCivQuoteData.bActive )
	{
		// Delete?
		ShutDownQuoteBox( TRUE );
	}
	
	// get text
	if ( !GetCivQuoteText( ubCivQuoteID, ubEntryID, zQuote ) )
	{
		return;
	}

	swprintf( gzCivQuote, L"\"%s\"", zQuote );

	if ( ubCivQuoteID == CIV_QUOTE_HINT )
	{
		MapScreenMessage( FONT_MCOLOR_WHITE, MSG_DIALOG, L"%s",	gzCivQuote );
	}

	// Create video oeverlay....
	memset( &VideoOverlayDesc, 0, sizeof( VIDEO_OVERLAY_DESC ) );

	//never use it anymore
	//SET_USE_WINFONTS( TRUE );
	//SET_WINFONT( giSubTitleWinFont );
	// Prepare text box
	gCivQuoteData.iDialogueBox = PrepareMercPopupBox( gCivQuoteData.iDialogueBox , BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER, gzCivQuote, DIALOGUE_DEFAULT_WIDTH, 0, 0, 0, &gusCivQuoteBoxWidth, &gusCivQuoteBoxHeight );
	//SET_USE_WINFONTS( FALSE );
	
	// OK, find center for box......
	sX = sX - ( gusCivQuoteBoxWidth / 2 );
	sY = sY - ( gusCivQuoteBoxHeight / 2 );

	// OK, limit to screen......
	{
		if ( sX < 0 )
		{
			sX = 0;
		}

		// CHECK FOR LEFT/RIGHT
		if ( ( sX + gusCivQuoteBoxWidth ) > SCREEN_WIDTH )
		{
			sX = SCREEN_WIDTH - gusCivQuoteBoxWidth;
		}

		// Now check for top
		if ( sY < gsVIEWPORT_WINDOW_START_Y )
		{
			sY = gsVIEWPORT_WINDOW_START_Y;
		}

		// Check for bottom
		if ( ( sY + gusCivQuoteBoxHeight ) > (SCREEN_HEIGHT - INV_INTERFACE_HEIGHT))
		{
			sY = (SCREEN_HEIGHT - INV_INTERFACE_HEIGHT) - gusCivQuoteBoxHeight;
		}
	}

	VideoOverlayDesc.sLeft			= sX;
	VideoOverlayDesc.sTop				= sY;
	VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + gusCivQuoteBoxWidth;
	VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + gusCivQuoteBoxHeight;
	VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
	VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
	VideoOverlayDesc.BltCallback = RenderCivQuoteBoxOverlay;

	gCivQuoteData.iVideoOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );


	//Define main region
	MSYS_DefineRegion( &(gCivQuoteData.MouseRegion), VideoOverlayDesc.sLeft, VideoOverlayDesc.sTop,	VideoOverlayDesc.sRight, VideoOverlayDesc.sBottom, MSYS_PRIORITY_HIGHEST,
						CURSOR_NORMAL, MSYS_NO_CALLBACK, QuoteOverlayClickCallback );
	// Add region
	MSYS_AddRegion( &(gCivQuoteData.MouseRegion) );
	
	gCivQuoteData.bActive = TRUE;

	gCivQuoteData.uiTimeOfCreation = GetJA2Clock( );

	gCivQuoteData.uiDelayTime = FindDelayForString( gzCivQuote ) + 500;

	gCivQuoteData.pCiv = pCiv;
}

UINT16 DetermineCivQuoteEntry( SOLDIERTYPE *pCiv, UINT16 *pubCivHintToUse, BOOLEAN fCanUseHints )
{
	UINT8	ubCivType;
#ifdef JA2UB

#else
	INT8	bTownId;
	INT8	bCivHint;
	INT8	bMineId;
#endif

	BOOLEAN	bCivLowLoyalty = FALSE;
	BOOLEAN	bCivHighLoyalty = FALSE;

	BOOLEAN bMiners = FALSE;
	UINT16 FileEDTQUoteID;
	
	(*pubCivHintToUse) = 0;

	ubCivType = GetCivType( pCiv );

	if ( pCiv->ubCivilianGroup < NUM_CIV_GROUPS )
	{
#ifdef JA2UB
		if ( pCiv->ubCivilianGroup > UNNAMED_CIV_GROUP_19 )
#else
		if ( pCiv->ubCivilianGroup > QUEENS_CIV_GROUP )
#endif
		{
			if ( pCiv->aiData.bNeutral )
			{
				return(FileEDTQUoteID = pCiv->ubCivilianGroup * 2 + 10);
			}
			else
			{
				return(FileEDTQUoteID = pCiv->ubCivilianGroup * 2 + 11);
			}
		}
	}
		
#ifdef JA2UB		
	if( ubCivType != CIV_TYPE_ENEMY )
	{
		//if the civ is not an enemy
		if ( pCiv->aiData.bNeutral )
		{
			return( CIV_QUOTE__CIV_NOT_ENEMY ); //43
		}
		else
		{
			//
			//the civ is an enemy
			//

			//if the civ can fight
			if( pCiv->ubBodyType == REGMALE || pCiv->ubBodyType == REGFEMALE || pCiv->ubBodyType == BIGMALE )
			{
				return( CIV_QUOTE__CIV_ENEMY_CAN_FIGHT); //40 
			}
			else if( pCiv->stats.bLife < pCiv->stats.bLifeMax )
			{
				return( CIV_QUOTE__CIV_HURT ); //42
			}
			else
			{
				return( CIV_QUOTE__CIV_ENEMY_GENERIC ); //41
			}
		}
	}


	if( ubCivType == CIV_TYPE_ENEMY )
	{
		// Determine what type of quote to say...
		// Are are we going to attack?

		if ( pCiv->aiData.bAction == AI_ACTION_TOSS_PROJECTILE || pCiv->aiData.bAction == AI_ACTION_FIRE_GUN ||
							pCiv->aiData.bAction == AI_ACTION_FIRE_GUN || pCiv->aiData.bAction == AI_ACTION_KNIFE_MOVE )
		{
			return( CIV_QUOTE_ENEMY_THREAT );
		}

		// Hurt?
		else if ( pCiv->stats.bLife < 30 )
		{
			return( CIV_QUOTE_ENEMY_HURT );
		}
		// elite?
		else if ( pCiv->ubSoldierClass == SOLDIER_CLASS_ELITE )
		{
			return( CIV_QUOTE_ENEMY_ELITE );
		}
		else
		{
			return( CIV_QUOTE_ENEMY_ADMIN );
		}
	}

	return( 255 );
#else	
			
	if ( ubCivType == CIV_TYPE_ENEMY )
	{
		// Determine what type of quote to say...
		// Are are we going to attack?

		if ( pCiv->aiData.bAction == AI_ACTION_TOSS_PROJECTILE || pCiv->aiData.bAction == AI_ACTION_FIRE_GUN ||
							pCiv->aiData.bAction == AI_ACTION_FIRE_GUN || pCiv->aiData.bAction == AI_ACTION_KNIFE_MOVE )
		{
			return( CIV_QUOTE_ENEMY_THREAT );
		}
		else if ( pCiv->aiData.bAction == AI_ACTION_OFFER_SURRENDER )
		{
			return( CIV_QUOTE_ENEMY_OFFER_SURRENDER );
		}
		// Hurt?
		else if ( pCiv->stats.bLife < 30 )
		{
			return( CIV_QUOTE_ENEMY_HURT );
		}
		// elite?
		else if ( pCiv->ubSoldierClass == SOLDIER_CLASS_ELITE )
		{
			return( CIV_QUOTE_ENEMY_ELITE );
		}
		else
		{
			return( CIV_QUOTE_ENEMY_ADMIN );
		}
	}

	// Are we in a town sector?
	// get town id
	bTownId = GetTownIdForSector( gWorldSectorX, gWorldSectorY );
	
	// If a married PC...
	if ( ubCivType == CIV_TYPE_MARRIED_PC )
	{
		return( CIV_QUOTE_PC_MARRIED );
	}

	// CIV GROUPS FIRST!
	// Hicks.....
	if ( pCiv->ubCivilianGroup == HICKS_CIV_GROUP )
	{
		// Are they friendly?
		//if ( gTacticalStatus.fCivGroupHostile[ HICKS_CIV_GROUP ] < CIV_GROUP_WILL_BECOME_HOSTILE )
		if ( pCiv->aiData.bNeutral )
		{
			return( CIV_QUOTE_HICKS_FRIENDLY );
		}
		else
		{
			return( CIV_QUOTE_HICKS_ENEMIES );
		}
	}

	// Goons.....
	if ( pCiv->ubCivilianGroup == KINGPIN_CIV_GROUP )
	{
		// Are they friendly?
		//if ( gTacticalStatus.fCivGroupHostile[ KINGPIN_CIV_GROUP ] < CIV_GROUP_WILL_BECOME_HOSTILE )
		if ( pCiv->aiData.bNeutral )
		{
			return( CIV_QUOTE_GOONS_FRIENDLY );
		}
		else
		{
			return( CIV_QUOTE_GOONS_ENEMIES );
		}
	}

	// ATE: Cowering people take precedence....
	if ( ( pCiv->flags.uiStatusFlags & SOLDIER_COWERING ) || ( pCiv->bTeam == CIV_TEAM && ( gTacticalStatus.uiFlags & INCOMBAT ) ) )
	{
		if ( ubCivType == CIV_TYPE_ADULT )
		{
			return( CIV_QUOTE_ADULTS_COWER );
		}
		else
		{
			return( CIV_QUOTE_KIDS_COWER );
		}
	}

	// Kid slaves...
	if ( pCiv->ubCivilianGroup == FACTORY_KIDS_GROUP )
	{
		// Check fact.....
		if ( CheckFact( FACT_DOREEN_HAD_CHANGE_OF_HEART, 0 ) || !CheckFact( FACT_DOREEN_ALIVE, 0 ) )
		{
			return( CIV_QUOTE_KID_SLAVES_FREE );
		}
		else
		{
			return( CIV_QUOTE_KID_SLAVES );
		}
	}

	// BEGGERS
	if ( pCiv->ubCivilianGroup == BEGGARS_CIV_GROUP )
	{
		// Check if we are in a town...
		if( bTownId != BLANK_SECTOR && gbWorldSectorZ == 0 )
		{
			if ( bTownId == SAN_MONA && ubCivType == CIV_TYPE_ADULT )
			{
				return( CIV_QUOTE_SAN_MONA_BEGGERS );
			}
		}

		// DO normal beggers...
		if ( ubCivType == CIV_TYPE_ADULT )
		{
			return( CIV_QUOTE_ADULTS_BEGGING );
		}
		else
		{
			return( CIV_QUOTE_KIDS_BEGGING );
		}
	}

	// REBELS
	if ( pCiv->ubCivilianGroup == REBEL_CIV_GROUP )
	{
		// DO normal beggers...
		if ( ubCivType == CIV_TYPE_ADULT )
		{
			return( CIV_QUOTE_ADULTS_REBELS );
		}
		else
		{
			return( CIV_QUOTE_KIDS_REBELS );
		}
	}

	// Do miltitia...
	if ( pCiv->bTeam == MILITIA_TEAM )
	{
		// Different types....
		if ( pCiv->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA )
		{
			return( CIV_QUOTE_GREEN_MILITIA );
		}
		if ( pCiv->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA )
		{
			return( CIV_QUOTE_MEDIUM_MILITIA );
		}
		if ( pCiv->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA )
		{
			return( CIV_QUOTE_ELITE_MILITIA );
		}
	}

	// If we are in medunna, and queen is dead, use these...
	if ( bTownId == MEDUNA && CheckFact( FACT_QUEEN_DEAD, 0 ) )
	{
		return( CIV_QUOTE_DEIDRANNA_DEAD );
	}

	// if in a town
	if( ( bTownId != BLANK_SECTOR ) && ( gbWorldSectorZ == 0 ) && gfTownUsesLoyalty[ bTownId ] )
	{
		// Check loyalty special quotes.....
		// EXTREMELY LOW TOWN LOYALTY...
		if ( gTownLoyalty[ bTownId ].ubRating < EXTREAMLY_LOW_TOWN_LOYALTY )
		{
			bCivLowLoyalty = TRUE;
		}

		// HIGH TOWN LOYALTY...
		if ( gTownLoyalty[ bTownId ].ubRating >= HIGH_TOWN_LOYALTY )
		{
			bCivHighLoyalty = TRUE;
		}
	}

	// ATE: OK, check if we should look for a civ hint....
	if ( fCanUseHints )
	{
		bCivHint = ConsiderCivilianQuotes( gWorldSectorX, gWorldSectorY, gbWorldSectorZ,	FALSE );
	}
	else
	{
		bCivHint = -1;
	}

	// ATE: check miners......
	if ( pCiv->ubSoldierClass == SOLDIER_CLASS_MINER )
	{
		bMiners = TRUE;

		// If not a civ hint available...
		if ( bCivHint == -1 )
		{
			// Check if they are under our control...

			// Should I go talk to miner?
			// Not done yet.

			// Are they working for us?
			bMineId = GetIdOfMineForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

			if ( PlayerControlsMine( bMineId ) )
			{
				return( CIV_QUOTE_MINERS_FOR_PLAYER );
			}
			else
			{
				return( CIV_QUOTE_MINERS_NOT_FOR_PLAYER );
			}
		}
	}

	// Is one availible?
	// If we are to say low loyalty, do chance
	if ( bCivHint != -1 && bCivLowLoyalty && !bMiners )
	{
		if ( Random( 100 ) < 25 )
		{
			// Get rid of hint...
			bCivHint = -1;
		}
	}

	// Say hint if availible...
	if ( bCivHint != -1 )
	{
		if ( ubCivType == CIV_TYPE_ADULT )
		{
			(*pubCivHintToUse) = bCivHint;

			// Set quote as used...
			ConsiderCivilianQuotes( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, TRUE );

			// retrun value....
			return( CIV_QUOTE_HINT );
		}
	}

	if ( bCivLowLoyalty )
	{
		if ( ubCivType == CIV_TYPE_ADULT )
		{
			return( CIV_QUOTE_ADULTS_EXTREMLY_LOW_LOYALTY );
		}
		else
		{
			return( CIV_QUOTE_KIDS_EXTREMLY_LOW_LOYALTY );
		}
	}

	if ( bCivHighLoyalty )
	{
		if ( ubCivType == CIV_TYPE_ADULT )
		{
			return( CIV_QUOTE_ADULTS_HIGH_LOYALTY );
		}
		else
		{
			return( CIV_QUOTE_KIDS_HIGH_LOYALTY );
		}
	}
	
	// All purpose quote here....
	if ( ubCivType == CIV_TYPE_ADULT )
	{
		return( CIV_QUOTE_ADULTS_ALL_PURPOSE );
	}
	else
	{
		return( CIV_QUOTE_KIDS_ALL_PURPOSE );
	}
#endif
}


void HandleCivQuote( )
{
	if ( gCivQuoteData.bActive )
	{
		// Check for min time....
		if ( ( GetJA2Clock( ) - gCivQuoteData.uiTimeOfCreation ) > gCivQuoteData.uiDelayTime )
		{
			// Stop!
			ShutDownQuoteBox( TRUE );
		}
	}
}

void StartCivQuote( SOLDIERTYPE *pCiv )
{
	UINT16 ubCivQuoteID;
	INT16	sX, sY;
	UINT16	ubEntryID = 0;
	INT16	sScreenX, sScreenY;
	UINT16	ubCivHintToUse;
	UINT16 CivQuoteDelta = 0;
	
	UINT16 ubCivQuoteID2;
	UINT16 RandomVal;
	
	// ATE: Check for old quote.....
	// This could have been stored on last attempt...
	if ( pCiv->bCurrentCivQuote == CIV_QUOTE_HINT )
	{
		// Determine which quote to say.....
		// CAN'T USE HINTS, since we just did one...
		pCiv->bCurrentCivQuote = -1;
		pCiv->bCurrentCivQuoteDelta = 0;
		ubCivQuoteID = DetermineCivQuoteEntry( pCiv, &ubCivHintToUse, FALSE );
	}
	else
	{
		// Determine which quote to say.....
		ubCivQuoteID = DetermineCivQuoteEntry( pCiv, &ubCivHintToUse, TRUE );
	}
	
	if (ubCivQuoteID == CIV_QUOTE_ADULTS_REBELS || ubCivQuoteID == CIV_QUOTE_KIDS_REBELS || ubCivQuoteID == CIV_QUOTE_ENEMY_OFFER_SURRENDER ) 
	{
		RandomVal = 5;
	}
	else if (ubCivQuoteID == CIV_QUOTE_PC_MARRIED) 
	{
		RandomVal = 2;
	}
	else if (ubCivQuoteID == CIV_QUOTE_HICKS_SEE_US_AT_NIGHT) 
	{
		RandomVal = 3;
	}
	else 
		RandomVal = 15;

#ifdef JA2UB		
	if( ubCivQuoteID == 255 )
	{
		return;
	}
#endif	
	
	// Determine entry id
	// ATE: Try and get entry from soldier pointer....
	if ( ubCivQuoteID != CIV_QUOTE_HINT )
	{
		if ( pCiv->bCurrentCivQuote == -1 )
		{
			// Pick random one
			//pCiv->bCurrentCivQuote = (INT8)Random( gCivQuotes[ ubCivQuoteID ].ubNumEntries - 2 );
			ubCivQuoteID2  = Random(RandomVal-2);
			pCiv->bCurrentCivQuoteDelta = 0;
		}

		//ubEntryID	= pCiv->bCurrentCivQuote + pCiv->bCurrentCivQuoteDelta;
		ubEntryID	= ubCivQuoteID2 + pCiv->bCurrentCivQuoteDelta;
	}
	else
	{
		ubEntryID =ubCivHintToUse;

		// ATE: set value for quote ID.....
		//pCiv->bCurrentCivQuote			= ubCivQuoteID;
		ubCivQuoteID2 = ubCivQuoteID;
		CivQuoteDelta = ubEntryID;
		//pCiv->bCurrentCivQuoteDelta = ubEntryID;
	}

	// Flugente: if we are an assassin, we speak like the militia we emulate
	if ( pCiv->usSoldierFlagMask & SOLDIER_ASSASSIN )
	{
		switch ( pCiv->GetUniformType() )
		{
		case UNIFORM_MILITIA_REGULAR:
			ubCivQuoteID = CIV_QUOTE_MEDIUM_MILITIA;
			break;
		case UNIFORM_MILITIA_ELITE:
			ubCivQuoteID = CIV_QUOTE_ELITE_MILITIA;
			break;
		default:
			ubCivQuoteID = CIV_QUOTE_GREEN_MILITIA;
			break;
		}
	}

	// Determine location...
	// Get location of civ on screen.....
	GetSoldierScreenPos( pCiv, &sScreenX, &sScreenY );
	sX = sScreenX;
	sY = sScreenY;

	// begin quote
	BeginCivQuote( pCiv, ubCivQuoteID, ubEntryID, sX, sY );

	// Increment use
	if ( ubCivQuoteID != CIV_QUOTE_HINT )
	{
		//pCiv->bCurrentCivQuoteDelta++;
		CivQuoteDelta++;
		/*
		if ( pCiv->bCurrentCivQuoteDelta == 2 )
		{
			pCiv->bCurrentCivQuoteDelta = 0;
		}
		*/
		if ( CivQuoteDelta == 2 )
		{
			CivQuoteDelta = 0;
		}
	}
}

void InitCivQuoteSystem( )
{
	memset( &gCivQuotes, 0, sizeof( gCivQuotes ) );  //Not used 

	memset( &gCivQuoteData, 0, sizeof( gCivQuoteData ) );
	gCivQuoteData.bActive				= FALSE;
	gCivQuoteData.iVideoOverlay	= -1;
	gCivQuoteData.iDialogueBox	= -1;
}

//--------------------------------------------------------------
//is allowed remove. Not used  and remove from SaveLoadGame.cpp.
BOOLEAN SaveCivQuotesToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	FileWrite( hFile, &gCivQuotes, sizeof( gCivQuotes), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gCivQuotes ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

// anv: used now
//is allowed remove. Not used and remove from SaveLoadGame.cpp.
BOOLEAN LoadCivQuotesFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	// anv: reset uiTauntFinishTimes after game is loaded (so enemies can taunt after guiBaseJA2Clock is decreased)
	memset( &uiTauntFinishTimes, 0, sizeof( uiTauntFinishTimes ) );

	FileRead( hFile, &gCivQuotes, sizeof( gCivQuotes ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gCivQuotes ) )
	{
		return( FALSE );
	}

	CopyNumEntriesIntoQuoteStruct( ); //Not used 

	return( TRUE );
}
//--------------------------------------------------------------

// anv: start enemy taunt with probabilty depending on taunt settings
void PossiblyStartEnemyTaunt( SOLDIERTYPE *pCiv, TAUNTTYPE iTauntType, UINT32 uiTargetID )
{
	SOLDIERTYPE *pTarget = NULL;
	if( uiTargetID != NOBODY )
	{
		pTarget = MercPtrs[uiTargetID];
	}
	if (is_networked)	// No taunts in multiplayer
		return;

	// taunts disabled?
	if( gGameSettings.fOptions[TOPTION_ALLOW_TAUNTS] == FALSE )
	{
		return;
	}
	// uh, just in case
	if( pCiv == NULL )
	{
		return;
	}
	// is enemy blocked from taunting at the moment?
	if( uiTauntFinishTimes[pCiv->ubID] > GetJA2Clock() )
	{
		
		return;
	}
	// check if generated person
	if ( !(IS_MERC_BODY_TYPE( pCiv )) || !(pCiv->ubProfile == NO_PROFILE) )
	{
		return;
	}
	// only enemies and militia taunt
	if ( pCiv->bTeam != ENEMY_TEAM && pCiv->bTeam != MILITIA_TEAM )
	{
		return;
	}
	// only visible enemies taunt (unless set otherwise)
	if ( ( pCiv->bVisible == -1 ) && ( gTauntsSettings.fTauntOnlyVisibleEnemies == TRUE ) )
	{
		return;
	}
	// only enemies that are able to speak at the moment can taunt
	if ( pCiv->stats.bLife < OKLIFE || pCiv->bCollapsed || pCiv->bBreathCollapsed )
	{
		return;
	}
	// check probability
 	switch(iTauntType)
	{
		case TAUNT_FIRE_GUN:
			if( Random(100)+1 > gTauntsSettings.ubTauntFireGunChance )
				return;
			break;
		case TAUNT_FIRE_LAUNCHER:
			if( Random(100)+1 > gTauntsSettings.ubTauntFireLauncherChance )
				return;
			break;
		case TAUNT_ATTACK_BLADE:
			if( Random(100)+1 > gTauntsSettings.ubTauntAttackBladeChance)
				return;
			break;
		case TAUNT_ATTACK_HTH:
			if( Random(100)+1 > gTauntsSettings.ubTauntAttackHTHChance )
				return;
			break;

		case TAUNT_THROW_KNIFE:
			if( Random(100)+1 > gTauntsSettings.ubTauntThrowKnifeChance )
				return;
			break;
		case TAUNT_THROW_GRENADE:
			if( Random(100)+1 > gTauntsSettings.ubTauntThrowGrenadeChance )
				return;
			break;
		case TAUNT_CHARGE_BLADE:
			if( Random(100)+1 > gTauntsSettings.ubTauntChargeKnifeChance )
				return;
			break;
		case TAUNT_CHARGE_HTH:
			if( Random(100)+1 > gTauntsSettings.ubTauntChargeFistsChance )
				return;
			break;

		case TAUNT_STEAL:
			if( Random(100)+1 > gTauntsSettings.ubTauntStealChance )
				return;
			break;

		case TAUNT_RUN_AWAY:
			if( Random(100)+1 > gTauntsSettings.ubTauntRunAwayChance)
				return;
			break;
		case TAUNT_SEEK_NOISE:
			if( Random(100)+1 > gTauntsSettings.ubTauntSeekNoiseChance )
				return;
			break;
		case TAUNT_ALERT:
			if( Random(100)+1 > gTauntsSettings.ubTauntAlertChance )
				return;
			break;
		case TAUNT_SUSPICIOUS:
			if( Random(100)+1 > gTauntsSettings.ubTauntSuspiciousChance)
				return;
			break;

		case TAUNT_GOT_HIT:
		case TAUNT_GOT_HIT_BLOODLOSS:
		case TAUNT_GOT_HIT_EXPLOSION:
		case TAUNT_GOT_HIT_FALLROOF:
		case TAUNT_GOT_HIT_GAS:
		case TAUNT_GOT_HIT_GUNFIRE:
		case TAUNT_GOT_HIT_HTH:
		case TAUNT_GOT_HIT_BLADE:
		case TAUNT_GOT_HIT_OBJECT:
		case TAUNT_GOT_HIT_STRUCTURE_EXPLOSION:
		case TAUNT_GOT_HIT_TENTACLES:
		case TAUNT_GOT_HIT_THROWING_KNIFE:
			if( Random(100)+1 > gTauntsSettings.ubTauntGotHitChance )
				return;
			break;

		case TAUNT_GOT_BLINDED:
		case TAUNT_GOT_DEAFENED:
			if( Random(100)+1 > gTauntsSettings.ubTauntGotDeafenedBlindedChance)
				return;
			break;

		case TAUNT_GOT_ROBBED:
			if( Random(100)+1 > gTauntsSettings.ubTauntGotRobbedChance)
				return;
			break;

		case TAUNT_GOT_MISSED:
		case TAUNT_GOT_MISSED_GUNFIRE:
		case TAUNT_GOT_MISSED_BLADE:
		case TAUNT_GOT_MISSED_HTH:
		case TAUNT_GOT_MISSED_THROWING_KNIFE:
			if( Random(100)+1 > gTauntsSettings.ubTauntGotMissedChance )
				return;
			break;

		case TAUNT_HIT:
		case TAUNT_HIT_GUNFIRE:
		case TAUNT_HIT_BLADE:
		case TAUNT_HIT_HTH:
		case TAUNT_HIT_THROWING_KNIFE:
			if( Random(100)+1 > gTauntsSettings.ubTauntHitChance )
				return;
			break;

		case TAUNT_KILL:
		case TAUNT_KILL_GUNFIRE:
		case TAUNT_KILL_BLADE:
		case TAUNT_KILL_HTH:
		case TAUNT_KILL_THROWING_KNIFE:
			if( Random(100)+1 > gTauntsSettings.ubTauntKillChance )
				return;
			break;

		case TAUNT_HEAD_POP:
			if( Random(100)+1 > gTauntsSettings.ubTauntHeadPopChance )
				return;
			break;

		case TAUNT_MISS:
		case TAUNT_MISS_GUNFIRE:
		case TAUNT_MISS_BLADE:
		case TAUNT_MISS_HTH:
		case TAUNT_MISS_THROWING_KNIFE:
			if( Random(100)+1 > gTauntsSettings.ubTauntMissChance )
				return;
			break;

		case TAUNT_OUT_OF_AMMO:
			if( Random(100)+1 > gTauntsSettings.ubTauntOutOfAmmoChance )
				return;
			break;
		case TAUNT_RELOAD:
			if( Random(100)+1 > gTauntsSettings.ubTauntReloadChance )
				return;
			break;

		case TAUNT_NOTICED_UNSEEN:
			if( Random(100)+1 > gTauntsSettings.ubTauntNoticedUnseenChance )
				return;
			break;
		case TAUNT_SAY_HI:
			if( Random(100)+1 > gTauntsSettings.ubTauntSayHiChance )
				return;
			break;
		case TAUNT_INFORM_ABOUT:
			if( Random(100)+1 > gTauntsSettings.ubTauntInformAboutChance )
				return;
			break;

		case TAUNT_RIPOSTE:
			if( Random(100)+1 > gTauntsSettings.ubRiposteChance)
				return;
			break;

		default:
			break;
	}
	
	StartEnemyTaunt( pCiv, iTauntType, pTarget );
}

// SANDRO - soldier taunts 
void StartEnemyTaunt( SOLDIERTYPE *pCiv, TAUNTTYPE iTauntType, SOLDIERTYPE *pTarget )
{
	CHAR16	sTauntText[ 320 ];	
	CHAR16	gzTauntQuote[ 320 ];
	UINT16	iApplicableTaunts = 0;

	// Flugente: zombies don't talk
	if ( pCiv->IsZombie() )
		return;

	// gCivQuoteData.bActive is checked in ShowTauntPopupBox() instead, taunt can be shown in log though!
	// if we have a different quote on, return, this one is not important
	//if ( gCivQuoteData.bActive )
	//{
	//	return;
	//}

	// anv: check all taunts, and remember those applicable
	for(UINT16 i=0; i<num_found_taunt; ++i)
	{
		// check if attitudes are ok
		switch( pCiv->aiData.bAttitude )
		{
			case CUNNINGAID:
				if( !(zTaunt[ i ].uiFlags & TAUNT_A_CUNNING_AID) )
					continue;
				break;
			case CUNNINGSOLO:
				if( !(zTaunt[ i ].uiFlags & TAUNT_A_CUNNING_SOLO) )
					continue;
				break;
			case BRAVEAID:
				if( !(zTaunt[ i ].uiFlags & TAUNT_A_BRAVE_AID) )
					continue;
			case BRAVESOLO:
				if( !(zTaunt[ i ].uiFlags & TAUNT_A_BRAVE_SOLO) )
					continue;
				break;
			case DEFENSIVE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_A_DEFENSIVE) )
					continue;
				break;
			case AGGRESSIVE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_A_AGGRESSIVE) )
					continue;
				break;
			default:
				break;
		}
		// check if situation is ok
		switch(iTauntType)
		{
			// actions
			case TAUNT_FIRE_GUN:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_FIRE_GUN) )
					continue;
				break;
			case TAUNT_FIRE_LAUNCHER:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_FIRE_LAUNCHER) )
					continue;
				break;
			case TAUNT_ATTACK_BLADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_ATTACK_BLADE) )
					continue;
				break;
			case TAUNT_ATTACK_HTH:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_ATTACK_HTH) )
					continue;
				break;

			case TAUNT_THROW_KNIFE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_THROW_KNIFE) )
					continue;
				break;
			case TAUNT_THROW_GRENADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_THROW_GRENADE) )
					continue;
				break;

			case TAUNT_OUT_OF_AMMO:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_OUT_OF_AMMO) )
					continue;
				break;
			case TAUNT_RELOAD:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_RELOAD) )
					continue;
				break;

			// AI routines
			case TAUNT_CHARGE_BLADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_CHARGE_BLADE) )
					continue;
				break;
			case TAUNT_CHARGE_HTH:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_CHARGE_HTH) )
					continue;
				break;
			case TAUNT_STEAL:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_STEAL) )
					continue;
				break;
			case TAUNT_RUN_AWAY:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_RUN_AWAY) )
					continue;
				break;
			case TAUNT_SEEK_NOISE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_SEEK_NOISE) )
					continue;
				break;
			case TAUNT_ALERT:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_ALERT) )
					continue;
				break;
			case TAUNT_SUSPICIOUS:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_SUSPICIOUS) )
					continue;
				break;
			case TAUNT_NOTICED_UNSEEN:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_NOTICED_UNSEEN) )
					continue;
				break;
			case TAUNT_SAY_HI:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_SAY_HI) )
					continue;
				break;
			case TAUNT_INFORM_ABOUT:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_INFORM_ABOUT) )
					continue;
				break;

			// got_hit_xxx
			case TAUNT_GOT_HIT:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT) )
					continue;
				break;
			case TAUNT_GOT_HIT_GUNFIRE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_GUNFIRE) )
					continue;
				break;
			case TAUNT_GOT_HIT_BLADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_BLADE) )
					continue;
				break;
			case TAUNT_GOT_HIT_HTH:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_HTH) )
					continue;
				break;
			case TAUNT_GOT_HIT_FALLROOF:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_FALLROOF) )
					continue;
				break;
			case TAUNT_GOT_HIT_BLOODLOSS:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_BLOODLOSS) )
					continue;
				break;
			case TAUNT_GOT_HIT_EXPLOSION:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_EXPLOSION) )
					continue;
				break;
			case TAUNT_GOT_HIT_GAS:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_GAS) )
					continue;
				break;
			case TAUNT_GOT_HIT_TENTACLES:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_TENTACLES) )
					continue;
				break;
			case TAUNT_GOT_HIT_STRUCTURE_EXPLOSION:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_STRUCTURE_EXPLOSION) )
					continue;
				break;
			case TAUNT_GOT_HIT_OBJECT:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT_OBJECT) )
					continue;
				break;
			case TAUNT_GOT_HIT_THROWING_KNIFE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_HIT) )
					continue;
				break;

			case TAUNT_GOT_DEAFENED:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_DEAFENED) )
					continue;
				break;
			case TAUNT_GOT_BLINDED:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_BLINDED) )
					continue;
				break;

			// got_missed_xxx
			case TAUNT_GOT_MISSED:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_MISSED) )
					continue;
				break;
			case TAUNT_GOT_MISSED_GUNFIRE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_MISSED_GUNFIRE) )
					continue;
				break;
			case TAUNT_GOT_MISSED_BLADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_MISSED_BLADE) )
					continue;
				break;
			case TAUNT_GOT_MISSED_HTH:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_MISSED_HTH) )
					continue;
				break;
			case TAUNT_GOT_MISSED_THROWING_KNIFE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_GOT_MISSED_THROWING_KNIFE) )
					continue;
				break;

			// hit_xxx
			case TAUNT_HIT:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_HIT) )
					continue;
				break;
			case TAUNT_HIT_GUNFIRE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_HIT_GUNFIRE) )
					continue;
				break;
			case TAUNT_HIT_BLADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_HIT_BLADE) )
					continue;
				break;
			case TAUNT_HIT_HTH:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_HIT_HTH) )
					continue;
				break;
			case TAUNT_HIT_THROWING_KNIFE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_HIT_THROWING_KNIFE) )
					continue;
				break;

			// kill_xxx
			case TAUNT_KILL:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_KILL) )
					continue;
				break;
			case TAUNT_KILL_GUNFIRE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_KILL_GUNFIRE) )
					continue;
				break;
			case TAUNT_KILL_BLADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_KILL_BLADE) )
					continue;
				break;
			case TAUNT_KILL_HTH:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_KILL_HTH) )
					continue;
				break;
			case TAUNT_KILL_THROWING_KNIFE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_KILL_THROWING_KNIFE) )
					continue;
				break;
			case TAUNT_HEAD_POP:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_HEAD_POP) )
					continue;
				break;

			// miss_xxx
			case TAUNT_MISS:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_MISS) )
					continue;
				break;
			case TAUNT_MISS_GUNFIRE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_MISS_GUNFIRE) )
					continue;
				break;
			case TAUNT_MISS_BLADE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_MISS_BLADE) )
					continue;
				break;
			case TAUNT_MISS_HTH:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_MISS_HTH) )
					continue;
				break;
			case TAUNT_MISS_THROWING_KNIFE:
				if( !(zTaunt[ i ].uiFlags & TAUNT_S_MISS_THROWING_KNIFE) )
					continue;
				break;

			// ripostes to merc quotes
			case TAUNT_RIPOSTE:
				if( !(gTacticalStatus.ubLastQuoteSaid == zTaunt[ i ].value[TAUNT_RIPOSTE_QUOTE]) )
					continue;
				break;

			default:
				continue;
				break;
		}

		// class and predefined profiles
		switch( pCiv->ubSoldierClass )
		{
			case SOLDIER_CLASS_ADMINISTRATOR:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_C_ADMIN) )
					continue;
				if( (zTaunt[ i ].value[TAUNT_PROFILE_ADMIN] != -1 ) && !(zTaunt[ i ].value[TAUNT_PROFILE_ADMIN] == pCiv->usSoldierProfile ) )
					continue;
				break;
			case SOLDIER_CLASS_ARMY:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_C_ARMY) )
					continue;
				if( (zTaunt[ i ].value[TAUNT_PROFILE_ARMY] != -1 ) && !(zTaunt[ i ].value[TAUNT_PROFILE_ARMY] == pCiv->usSoldierProfile ) )
					continue;
				break;
			case SOLDIER_CLASS_ELITE:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_C_ELITE) )
					continue;
				if( (zTaunt[ i ].value[TAUNT_PROFILE_ELITE] != -1 ) && !(zTaunt[ i ].value[TAUNT_PROFILE_ELITE] == pCiv->usSoldierProfile ) )
					continue;
				break;
			case SOLDIER_CLASS_GREEN_MILITIA:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_C_GREEN) )
					continue;
				if( (zTaunt[ i ].value[TAUNT_PROFILE_GREEN] != -1 ) && !( zTaunt[ i ].value[TAUNT_PROFILE_GREEN] == pCiv->usSoldierProfile ) )
					continue;
				break;
			case SOLDIER_CLASS_REG_MILITIA:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_C_REGULAR) )
					continue;
				if( (zTaunt[ i ].value[TAUNT_PROFILE_ARMY] != -1 ) && !(zTaunt[ i ].value[TAUNT_PROFILE_ARMY] == pCiv->usSoldierProfile ) )
					continue;
				break;
			case SOLDIER_CLASS_ELITE_MILITIA:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_C_VETERAN) )
					continue;
				if( (zTaunt[ i ].value[TAUNT_PROFILE_ARMY] != -1 ) && !(zTaunt[ i ].value[TAUNT_PROFILE_ARMY] == pCiv->usSoldierProfile ) )
					continue;
				break;
			default:
				return;
				break;
		}

		// gender
		switch( pCiv->ubBodyType )
		{
			case REGMALE:
			case BIGMALE:
			case STOCKYMALE:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_G_MALE) )
					continue;
				break;
			case REGFEMALE:
				if( !(zTaunt[ i ].uiFlags2 & TAUNT_G_FEMALE) )
					continue;
				break;
			default:
				return;
				break;
		}

		// energy, health
		if( zTaunt[ i ].value[TAUNT_ENERGY_GT] != -1 )
		{
			if( pCiv->bBreath <= zTaunt[ i ].value[TAUNT_ENERGY_GT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_ENERGY_LT] != -1 )
		{
			if( pCiv->bBreath >= zTaunt[ i ].value[TAUNT_ENERGY_LT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_ENERGY_MAX_GT] != -1 )
		{
			if( pCiv->bBreathMax <= zTaunt[ i ].value[TAUNT_ENERGY_MAX_GT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_ENERGY_MAX_LT] != -1 )
		{
			if( pCiv->bBreathMax >= zTaunt[ i ].value[TAUNT_ENERGY_MAX_LT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_HEALTH_GT] != -1 )
		{
			if( pCiv->stats.bLife <= zTaunt[ i ].value[TAUNT_HEALTH_GT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_HEALTH_GT] != -1 )
		{
			if( pCiv->stats.bLife <= zTaunt[ i ].value[TAUNT_TARGET_HEALTH_GT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_HEALTH_LT] != -1 )
		{
			if( pCiv->stats.bLife >= zTaunt[ i ].value[TAUNT_HEALTH_LT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_HEALTH_MAX_GT] != -1 )
		{
			if( pCiv->stats.bLifeMax <= zTaunt[ i ].value[TAUNT_HEALTH_MAX_GT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_HEALTH_MAX_LT] != -1 )
		{
			if( pCiv->stats.bLifeMax >= zTaunt[ i ].value[TAUNT_HEALTH_MAX_LT] )
				continue;
		}
		// morale
		if( zTaunt[ i ].value[TAUNT_MORALE_GT] != -1 )
		{
			if( pCiv->aiData.bMorale <= zTaunt[ i ].value[TAUNT_MORALE_GT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_MORALE_LT] != -1 )
		{
			if( pCiv->aiData.bMorale >= zTaunt[ i ].value[TAUNT_MORALE_LT] )
				continue;
		}
		// experience
		if( zTaunt[ i ].value[TAUNT_EXP_LEVEL_GT] != -1 )
		{
			if( pCiv->stats.bExpLevel <= zTaunt[ i ].value[TAUNT_EXP_LEVEL_GT] )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_EXP_LEVEL_LT] != -1 )
		{
			if( pCiv->stats.bExpLevel >= zTaunt[ i ].value[TAUNT_EXP_LEVEL_LT] )
				continue;
		}

		// game progress
		if( zTaunt[ i ].value[TAUNT_PROGRESS_GT] != -1 )
		{
			if( zTaunt[ i ].value[TAUNT_PROGRESS_GT] >= CurrentPlayerProgressPercentage() )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_PROGRESS_LT] != -1 )
		{
			if( zTaunt[ i ].value[TAUNT_PROGRESS_GT] <= CurrentPlayerProgressPercentage() )
				continue;
		}

		// facts
		if( zTaunt[ i ].value[TAUNT_FACT_TRUE] != -1 )
		{
			if( gubFact[ zTaunt[ i ].value[TAUNT_FACT_TRUE] ] != TRUE )
				continue;
		}
		if( zTaunt[ i ].value[TAUNT_FACT_FALSE] != -1 )
		{
			if( gubFact[ zTaunt[ i ].value[TAUNT_FACT_FALSE] ] != FALSE )
				continue;
		}

		// target limitations
		if( pTarget != NULL )
		{
			// target should be zombie
			if( zTaunt[ i ].uiFlags2 & TAUNT_T_ZOMBIE )
			{
			// anv: moved ifdef - if zombies are off, we want to skip any taunts with TAUNT_T_ZOMBIE flag
				if( !pTarget->IsZombie() )
					continue;
			}

			// target merc profile
			if( zTaunt[ i ].value[TAUNT_TARGET_MERC_PROFILE] != -1 )
			{
				if( pTarget->ubProfile != zTaunt[ i ].value[TAUNT_TARGET_MERC_PROFILE] )
					continue;
			}
			// target type
			if( zTaunt[ i ].value[TAUNT_TARGET_TYPE] != -1 )
			{
				if( pTarget->ubProfile != zTaunt[ i ].value[TAUNT_TARGET_TYPE] )
					continue;
			}
			// target gender
			switch( pTarget->ubBodyType )
			{
				case REGMALE:
				case BIGMALE:
				case STOCKYMALE:
					if( !(zTaunt[ i ].uiFlags2 & TAUNT_T_MALE) )
						continue;
					break;
				case REGFEMALE:
					if( !(zTaunt[ i ].uiFlags2 & TAUNT_T_FEMALE) )
						continue;
					break;
				default:
					return;
					break;
			}
			// target appearance
			if( zTaunt[ i ].value[TAUNT_TARGET_APPEARANCE] != -1 )
			{
				// check if pTarget has his own predefined profile (ubProfile = 200 for generated characters)
				if( pTarget->ubProfile != NO_PROFILE )
				{
					if( gMercProfiles[pTarget->ubProfile].bAppearance != zTaunt[ i ].value[TAUNT_TARGET_APPEARANCE] )
						continue;
				}
				else
					continue;
			}

			// target type
			switch( zTaunt[ i ].value[TAUNT_TARGET_TYPE] )
			{
				case 0:
					if( pTarget->ubWhatKindOfMercAmI != MERC_TYPE__PLAYER_CHARACTER )
						continue;
					break;
				case 1:
					if( pTarget->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
						continue;
					break;
				case 2:
					if( pTarget->ubWhatKindOfMercAmI != MERC_TYPE__MERC )
						continue;
					break;
				case 3:
					if( pTarget->ubWhatKindOfMercAmI != MERC_TYPE__NPC )
						continue;
					break;
				case 4:
					if( pTarget->ubWhatKindOfMercAmI != MERC_TYPE__EPC )
						continue;
					break;
				case 5:
					if( pTarget->ubWhatKindOfMercAmI != MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT )
						continue;
					break;
				case 6:
					if( pTarget->ubWhatKindOfMercAmI != MERC_TYPE__VEHICLE )
						continue;
					break;
				default:
					break;
			}

			// target energy, health
			if( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_GT] != -1 )
			{
				if( pTarget->bBreath <= zTaunt[ i ].value[TAUNT_TARGET_ENERGY_GT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_LT] != -1 )
			{
				if( pTarget->bBreath >= zTaunt[ i ].value[TAUNT_TARGET_ENERGY_LT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_MAX_GT] != -1 )
			{
				if( pTarget->bBreathMax <= zTaunt[ i ].value[TAUNT_TARGET_ENERGY_MAX_GT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_MAX_LT] != -1 )
			{
				if( pTarget->bBreathMax >= zTaunt[ i ].value[TAUNT_TARGET_ENERGY_MAX_LT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_GT] != -1 )
			{
				if( pTarget->stats.bLife <= zTaunt[ i ].value[TAUNT_TARGET_HEALTH_GT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_GT] != -1 )
			{
				if( pTarget->stats.bLife <= zTaunt[ i ].value[TAUNT_TARGET_HEALTH_GT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_LT] != -1 )
			{
				if( pTarget->stats.bLife >= zTaunt[ i ].value[TAUNT_TARGET_HEALTH_LT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_MAX_GT] != -1 )
			{
				if( pTarget->stats.bLifeMax <= zTaunt[ i ].value[TAUNT_TARGET_HEALTH_MAX_GT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_MAX_LT] != -1 )
			{
				if( pTarget->stats.bLifeMax >= zTaunt[ i ].value[TAUNT_TARGET_HEALTH_MAX_LT] )
					continue;
			}
			// morale
			if( zTaunt[ i ].value[TAUNT_TARGET_MORALE_GT] != -1 )
			{
				if( pTarget->aiData.bMorale <= zTaunt[ i ].value[TAUNT_TARGET_MORALE_GT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_MORALE_LT] != -1 )
			{
				if( pTarget->aiData.bMorale >= zTaunt[ i ].value[TAUNT_TARGET_MORALE_LT] )
					continue;
			}
			// experience
			if( zTaunt[ i ].value[TAUNT_TARGET_EXP_LEVEL_GT] != -1 )
			{
				if( pTarget->stats.bExpLevel <= zTaunt[ i ].value[TAUNT_TARGET_EXP_LEVEL_GT] )
					continue;
			}
			if( zTaunt[ i ].value[TAUNT_TARGET_EXP_LEVEL_LT] != -1 )
			{
				if( pTarget->stats.bExpLevel >= zTaunt[ i ].value[TAUNT_TARGET_EXP_LEVEL_LT] )
					continue;
			}
		}
		else // pTarget==NULL
		{
			if( ( zTaunt[ i ].value[TAUNT_TARGET_MERC_PROFILE] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_APPEARANCE] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_GT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_LT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_MAX_GT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_ENERGY_MAX_LT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_GT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_LT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_MAX_GT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_HEALTH_MAX_LT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_EXP_LEVEL_GT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_EXP_LEVEL_LT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_MORALE_GT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_MORALE_LT] != -1 ) ||
				( zTaunt[ i ].value[TAUNT_TARGET_TYPE] != -1 ) || 
				( zTaunt[ i ].uiFlags2 & TAUNT_T_ZOMBIE ) )
					continue;

		}
		// everything ok, current taunt is applicable, remember it
		zApplicableTaunts[iApplicableTaunts] = zTaunt[i];
		iApplicableTaunts++;
		if(iApplicableTaunts >= MAX_APPLICABLE_TAUNTS)
			continue;
	}
	// are there any applicable taunts?
	if( iApplicableTaunts > 0 )
	{
		// use random one
		// use censored version if setting is set
		UINT16 iChosenTaunt = Random(iApplicableTaunts); 
		if( gTauntsSettings.fTauntCensoredMode == TRUE && zApplicableTaunts[ iChosenTaunt ].szCensoredText[0] != 0 )
		{
			swprintf( sTauntText, zApplicableTaunts[ iChosenTaunt ].szCensoredText );
		}
		else
		{
			swprintf( sTauntText, zApplicableTaunts[ iChosenTaunt ].szText );
		}

		swprintf( gzTauntQuote, L"\"%s\"", sTauntText );

		// block this enemy from taunting for a time being
		uiTauntFinishTimes[pCiv->ubID] = GetJA2Clock() + min( gTauntsSettings.sMaxDelay , max( gTauntsSettings.sMinDelay, FindDelayForString( gzTauntQuote ) + gTauntsSettings.sModDelay ) ); 

		if( gTauntsSettings.fTauntMakeNoise == TRUE )
			MakeNoise( pCiv->ubID, pCiv->sGridNo, pCiv->pathing.bLevel, pCiv->bOverTerrainType, gTauntsSettings.sVolume, NOISE_VOICE, gzTauntQuote );
		else
		{
			if(gTauntsSettings.fTauntShowPopupBox == TRUE)
			{	
				if( gbPublicOpplist[gbPlayerNum][pCiv->ubID] == SEEN_CURRENTLY || gTauntsSettings.fTauntAlwaysShowPopupBox == TRUE )
				{
					ShowTauntPopupBox( pCiv, gzTauntQuote );
				}
			}
			if(gTauntsSettings.fTauntShowInLog == TRUE)
			{
				if( gbPublicOpplist[gbPlayerNum][pCiv->ubID] == SEEN_CURRENTLY || gTauntsSettings.fTauntAlwaysShowInLog == TRUE )
				{
					ScreenMsg( FONT_GRAY2, MSG_INTERFACE, L"%s: %s", pCiv->GetName(), gzTauntQuote );
				}
			}
		}
	}
	else
	{
		return;
	}

}

void ShowTauntPopupBox( SOLDIERTYPE *pCiv, STR16 gzTauntQuote )
{
	INT16	sX, sY;
	INT16	sScreenX, sScreenY;
	VIDEO_OVERLAY_DESC		VideoOverlayDesc;

	// stop if other civ quote is already being shown 
	if( gCivQuoteData.bActive == TRUE )
	{
		return;
	}

	// Determine location...
	// Get location of civ on screen.....
	GetSoldierScreenPos( pCiv, &sScreenX, &sScreenY );
	sX = sScreenX;
	// Flugente: have the box appear a bit above the soldier. Otherwise it will obstruct us from aiming at him, which is annoying if it happens very often
	sY = sScreenY - 40;

	// Create video oeverlay....
	memset( &VideoOverlayDesc, 0, sizeof( VIDEO_OVERLAY_DESC ) );

	// Prepare text box
	gCivQuoteData.iDialogueBox = PrepareMercPopupBox( gCivQuoteData.iDialogueBox , BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER, gzTauntQuote, DIALOGUE_DEFAULT_WIDTH, 0, 0, 0, &gusCivQuoteBoxWidth, &gusCivQuoteBoxHeight );

	// OK, find center for box......
	sX = sX - ( gusCivQuoteBoxWidth / 2 );
	sY = sY - ( gusCivQuoteBoxHeight / 2 );

	// OK, limit to screen......
	{
		if ( sX < 0 )
		{
			sX = 0;
		}

		// CHECK FOR LEFT/RIGHT
		if ( ( sX + gusCivQuoteBoxWidth ) > SCREEN_WIDTH )
		{
			sX = SCREEN_WIDTH - gusCivQuoteBoxWidth;
		}

		// Now check for top
		if ( sY < gsVIEWPORT_WINDOW_START_Y )
		{
			sY = gsVIEWPORT_WINDOW_START_Y;
		}

		// Check for bottom
		if ( ( sY + gusCivQuoteBoxHeight ) > (SCREEN_HEIGHT - INV_INTERFACE_HEIGHT))
		{
			sY = (SCREEN_HEIGHT - INV_INTERFACE_HEIGHT) - gusCivQuoteBoxHeight;
		}
	}

	VideoOverlayDesc.sLeft			= sX;
	VideoOverlayDesc.sTop				= sY;
	VideoOverlayDesc.sRight			= VideoOverlayDesc.sLeft + gusCivQuoteBoxWidth;
	VideoOverlayDesc.sBottom		= VideoOverlayDesc.sTop + gusCivQuoteBoxHeight;
	VideoOverlayDesc.sX					= VideoOverlayDesc.sLeft;
	VideoOverlayDesc.sY					= VideoOverlayDesc.sTop;
	VideoOverlayDesc.BltCallback = RenderCivQuoteBoxOverlay;

	gCivQuoteData.iVideoOverlay =	RegisterVideoOverlay( 0, &VideoOverlayDesc );

	//Define main region
	MSYS_DefineRegion( &(gCivQuoteData.MouseRegion), VideoOverlayDesc.sLeft, VideoOverlayDesc.sTop,	VideoOverlayDesc.sRight, VideoOverlayDesc.sBottom, MSYS_PRIORITY_HIGHEST,
						CURSOR_NORMAL, MSYS_NO_CALLBACK, QuoteOverlayClickCallback );
	// Add region
	MSYS_AddRegion( &(gCivQuoteData.MouseRegion) );

	gCivQuoteData.bActive = TRUE;

	gCivQuoteData.uiTimeOfCreation = GetJA2Clock( );
	
	gCivQuoteData.uiDelayTime = min( gTauntsSettings.sMaxDelay , max( gTauntsSettings.sMinDelay, FindDelayForString( gzTauntQuote ) + gTauntsSettings.sModDelay ) );

	gCivQuoteData.pCiv = pCiv;
}