	#include "types.h"
	#include "wcheck.h"
	#include "overhead.h"
	#include "ai.h"
	#include "Soldier Profile.h"

	#include "NPC.h"
	#include "Isometric Utils.h"
	#include "Quests.h"
	#include "interface dialogue.h"
	#include "Game Clock.h"
	#include "FileMan.h"
	#include "Random.h"
	#include "items.h"
	#include "interface.h"
	#include "Assignments.h"
	#include "soldier macros.h"
	#include "dialogue control.h"
	#include "Strategic Town Loyalty.h"
	#include "message.h"
	#include "Timer Control.h"
	#include "Soldier Add.h"
	#include "Soldier Tile.h"
	#include "Weapons.h"
	#include "meanwhile.h"

	#ifdef JA2TESTVERSION
		#include	"Quest Debug System.h"
		#include	"QuestText.h"
	#endif

	#include "SkillCheck.h"
	#include "Render Fun.h"
	#include "Text.h"
	#include "finances.h"
	#include "Arms Dealer Init.h"
	#include "Interface Items.h"
	#include "opplist.h"
	#include "Animation Control.h"
	#include "Scheduling.h"
	#include "Tactical Save.h"
	#include "Campaign Types.h"
	#include "GameSettings.h" // added by SANDRO
	#include "Soldier Profile.h"
	#include "GameVersion.h"
	#include "Soldier Profile.h"
	
	#include "BriefingRoom_Data.h"

	#include "Map Screen Helicopter.h"
	#include "Drugs And Alcohol.h"	// added by Flugente for DoesMercHavePersonality( ... )
	#include "SaveLoadGame.h"
	
#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25_Tactical.h"
#include "ub_config.h"
#endif

	
//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

// WANNE - BMP: DONE!
//SB: new .npc format
#define _MAKEFOURCC(ch0, ch1, ch2, ch3)	\
	((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) |	\
	((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))

#define NPCEX_SIGNATURE _MAKEFOURCC('B','1','1','3')

#define NUM_CIVQUOTE_SECTORS 20
#define MINERS_CIV_QUOTE_INDEX  16

INT16	gsCivQuoteSector[NUM_CIVQUOTE_SECTORS][2] =
{
	 2,		MAP_ROW_A,
	 2,		MAP_ROW_B,
	13,		MAP_ROW_B,
	13,		MAP_ROW_C,
	13,		MAP_ROW_D,
	 8,		MAP_ROW_F,
	 9,		MAP_ROW_F,
	 8,		MAP_ROW_G,
	 9,		MAP_ROW_G,
	 1,		MAP_ROW_H,

	 2,		MAP_ROW_H,
	 3,		MAP_ROW_H,
	 8,		MAP_ROW_H,
	13,		MAP_ROW_H,
	14,		MAP_ROW_I,
	11,		MAP_ROW_L,
	12,		MAP_ROW_L,
	 0,		0,    // THIS ONE USED NOW - FOR bSectorZ > 0.....
	 0,		0,
	 0,		0,
};

#define NO_FACT					(MAX_FACTS - 1)
#define NO_QUEST				255
#define QUEST_NOT_STARTED_NUM   100
#define QUEST_DONE_NUM			200
#define NO_QUOTE				255
#define IRRELEVANT				255
#define NO_MOVE					NOWHERE//dnl ch46 031009

NPCQuoteInfo *	gpNPCQuoteInfoArray[NUM_PROFILES] = { NULL };
NPCQuoteInfo *	gpBackupNPCQuoteInfoArray[NUM_PROFILES] = { NULL };
NPCQuoteInfo *	gpCivQuoteInfoArray[NUM_CIVQUOTE_SECTORS] = { NULL };

#ifdef JA2TESTVERSION
	// Warning: cheap hack approaching
	BOOLEAN					gfTriedToLoadQuoteInfoArray[NUM_PROFILES] = { FALSE };
#endif

UINT8 gubTeamPenalty;

BOOLEAN EnsureQuoteFileLoaded( UINT8 ubNPC );
NPCQuoteInfo * LoadQuoteFile( UINT8 ubNPC );
UINT8 NPCConsiderQuote( UINT8 ubNPC, UINT8 ubMerc, UINT8 ubApproach, UINT8 ubQuoteNum, UINT8 ubTalkDesire, NPCQuoteInfo * pNPCQuoteInfoArray );
UINT8 NPCConsiderReceivingItemFromMerc( UINT8 ubNPC, UINT8 ubMerc, OBJECTTYPE * pObj, NPCQuoteInfo * pNPCQuoteInfoArray, NPCQuoteInfo ** ppResultQuoteInfo, UINT8 *pubQuoteNum );
void PCsNearNPC( UINT8 ubNPC );
void TriggerClosestMercWhoCanSeeNPC( UINT8 ubNPC, NPCQuoteInfo *pQuotePtr );
BOOLEAN NPCHasUnusedRecordWithGivenApproach( UINT8 ubNPC, UINT8 ubApproach );

INT8	gbFirstApproachFlags[4] = { 0x01, 0x02, 0x04, 0x08 };


UINT8	gubAlternateNPCFileNumsForQueenMeanwhiles[] = { 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176 };
UINT8	gubAlternateNPCFileNumsForElliotMeanwhiles[] = { 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196 };

#ifdef JA2BETAVERSION
BOOLEAN gfDisplayScreenMsgOnRecordUsage = FALSE;
#endif

extern void PauseAITemporarily( void );
extern void PayOffSkyriderDebtIfAny( );
//
// NPC QUOTE LOW LEVEL ROUTINES
//

// WANNE - BMP: DONE!
NPCQuoteInfo * LoadQuoteFile( UINT8 ubNPC )
{
	CHAR8						zFileName[255];
	HWFILE					hFile;
	NPCQuoteInfo	* pFileData;
	UINT32					uiBytesRead;
	UINT32					uiFileSize;
	DWORD					uiSignature = 0; // SB // WANNE - BMP: DONE!

	if ( ubNPC == PETER || ubNPC == ALBERTO || ubNPC == CARLO )
	{
		sprintf( zFileName, "NPCData\\%03d.npc", ubNPC );
		if ( !FileExists( zFileName ) )
		{
			// use a copy of Herve's data file instead!
			sprintf( zFileName, "NPCData\\%03d.npc", HERVE );
		}
	}
	
#ifdef JA2UB
	if ( ubNPC == MANUEL_UB||
		ubNPC == BIGGENS_UB ||
		ubNPC == JOHN_K_UB ||
		ubNPC == TEX_UB ||
		ubNPC == GASTON_UB ||
		ubNPC == STOGIE_UB ||
		ubNPC == JERRY_MILO_UB ||
		ubNPC == PGMALE4_UB ||
		ubNPC == BETTY_UB ||
		ubNPC == RAUL_UB ||
		ubNPC == MORRIS_UB ||
		ubNPC == RUDY_UB )
	{
		sprintf( zFileName, "NPCData\\%03d.npc", ubNPC );
	}		
#endif
	else if ( gMercProfiles[ubNPC].Type == PROFILETYPE_AIM ||
		gMercProfiles[ubNPC].Type == PROFILETYPE_MERC ||
		(gMercProfiles[ubNPC].Type == PROFILETYPE_RPC && gMercProfiles[ubNPC].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ) ||
		gMercProfiles[ubNPC].Type == PROFILETYPE_IMP )
	{
		sprintf( zFileName, "NPCData\\000.npc" );
	}
	else
	{
		sprintf( zFileName, "NPCData\\%03d.npc", ubNPC );
	}
#ifdef JA2UB
//Ja25:  No meanwhiles
#else
	
	// ATE: Put some stuff i here to use a different NPC file if we are in a meanwhile.....
	if ( AreInMeanwhile( ) )
	{
		// If we are the queen....
		if ( ubNPC == QUEEN )
		{
			sprintf( zFileName, "NPCData\\%03d.npc", gubAlternateNPCFileNumsForQueenMeanwhiles[ GetMeanwhileID( ) ] );
		}
		// If we are elliot....
		else if ( ubNPC == ELLIOT )
		{
			sprintf( zFileName, "NPCData\\%03d.npc", gubAlternateNPCFileNumsForElliotMeanwhiles[ GetMeanwhileID( ) ] );
		}
	}
#endif
	CHECKN( FileExists( zFileName ) );

	hFile = FileOpen( zFileName, FILE_ACCESS_READ, FALSE );
	CHECKN( hFile );

	// WANNE - BMP: DONE!
	//<SB> new script format
	if (!FileRead( hFile, &uiSignature, sizeof(uiSignature), &uiBytesRead ) || uiBytesRead != sizeof(uiSignature) )
		return NULL;

	if(uiSignature == NPCEX_SIGNATURE)//new fmt
	{
		uiFileSize = sizeof( NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS;
		pFileData = (NPCQuoteInfo *)MemAlloc( uiFileSize );
		if (pFileData)
		{
			if (!FileRead( hFile, pFileData, uiFileSize, &uiBytesRead ) || uiBytesRead != uiFileSize )
			{
				MemFree( pFileData );
				pFileData = NULL;
			}
		}

		FileClose( hFile );
	}
	else //old fmt - make conversion
	{
		int iRecord;
		_old_NPCQuoteInfo * pFileData_old_;
		uiFileSize = sizeof( _old_NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS;
		pFileData_old_ = ( _old_NPCQuoteInfo * )MemAlloc( uiFileSize );
		FileSeek(hFile, 0, FILE_SEEK_FROM_START);
		if (pFileData_old_)
		{
			if (!FileRead( hFile, pFileData_old_, uiFileSize, &uiBytesRead ) || uiBytesRead != uiFileSize )
			{
				MemFree( pFileData_old_ );
				pFileData_old_ = NULL;
			}
		}
		
		FileClose( hFile );
		//check for Russian script & make a runtime conversion of it to International
		if( *(DWORD*)pFileData_old_ == 0x00350039 )
		{
			//just offset records 4 bytes backward
			_old_NPCQuoteInfo * pEnglishScript = ( _old_NPCQuoteInfo * )MemAlloc( uiFileSize );
			memcpy( pEnglishScript, ((char*)pFileData_old_)+4, uiFileSize-4 );
			MemFree( pFileData_old_ );
			pFileData_old_ = pEnglishScript;
		}

		// Now it's time for conversion
		uiFileSize = sizeof(NPCQuoteInfo) * NUM_NPC_QUOTE_RECORDS;
		pFileData = (NPCQuoteInfo*)MemAlloc(uiFileSize);
		for(iRecord=0; iRecord<NUM_NPC_QUOTE_RECORDS; iRecord++)
			pFileData[iRecord] = pFileData_old_[iRecord];//dnl ch46 021009
		MemFree(pFileData_old_);
	}

	// 
	//</SB> new script format
	return( pFileData );
}

//dnl ch46 021009
NPCQuoteInfo& NPCQuoteInfo::operator=(const _old_NPCQuoteInfo& src)
{
	if((void*)this != (void*)&src)
	{
		fFlags = src.fFlags;
		if(src.sRequiredGridno < 0)
		{
			sRequiredItem = NOTHING;
			sRequiredGridNo = -src.sRequiredGridno;
		}
		else
		{
			sRequiredItem = src.sRequiredItem;
			sRequiredGridNo = NOWHERE;
		}
		usFactMustBeTrue = src.usFactMustBeTrue;
		usFactMustBeFalse = src.usFactMustBeFalse;
		ubQuest = src.ubQuest;
		ubFirstDay = src.ubFirstDay;
		ubLastDay = src.ubLastDay;
		ubApproachRequired = src.ubApproachRequired;
		ubOpinionRequired = src.ubOpinionRequired;
		usUnused = 0;
		ubQuoteNum = src.ubQuoteNum;
		ubNumQuotes = src.ubNumQuotes;
		ubStartQuest = src.ubStartQuest;
		ubEndQuest = src.ubEndQuest;
		ubTriggerNPC = src.ubTriggerNPC;
		ubTriggerNPCRec = src.ubTriggerNPCRec;
		usSetFactTrue = src.usSetFactTrue;
		usGiftItem = src.usGiftItem;
		sActionData = src.sActionData;
		if(src.usGoToGridno >= OLD_WORLD_MAX)
			usGoToGridNo = NOWHERE;
		else
			usGoToGridNo = src.usGoToGridno;
	}
	return(*this);
}

void RevertToOriginalQuoteFile( UINT8 ubNPC )
{
	if ( gpBackupNPCQuoteInfoArray[ ubNPC ] && gpNPCQuoteInfoArray[ubNPC] )
	{
		MemFree( gpNPCQuoteInfoArray[ubNPC] );
		gpNPCQuoteInfoArray[ubNPC] = gpBackupNPCQuoteInfoArray[ubNPC];
		gpBackupNPCQuoteInfoArray[ubNPC] = NULL;
	}
}

void BackupOriginalQuoteFile( UINT8 ubNPC )
{
	gpBackupNPCQuoteInfoArray[ ubNPC ] = gpNPCQuoteInfoArray[ ubNPC ];
	gpNPCQuoteInfoArray[ubNPC] = NULL;
}

extern UINT32 guiCurrentSaveGameVersion;
BOOLEAN EnsureQuoteFileLoaded( UINT8 ubNPC )
{
	BOOLEAN			fLoadFile = FALSE;

	if ( ubNPC == ROBOT )
	{
		return( FALSE );
	}

	if(guiCurrentSaveGameVersion < FIXED_NPC_QUOTE_BUG)
 	{
		if (gpNPCQuoteInfoArray[ubNPC] == NULL)
		{
			fLoadFile = TRUE;
		}
		else
		{
			// WANNE:Reload the NPC quote files if only ONE quote exists. This seems to be a bug anywhere in the quote system.
			// This is a hack, but works :)
			// This should fix the bug, that special quests are not triggerd when talking to NPCs!
			if (gpNPCQuoteInfoArray[ubNPC][1].ubQuoteNum == 0 && gpNPCQuoteInfoArray[ubNPC][1].ubNumQuotes == 0)
			{
				fLoadFile = TRUE;
			}
		}
 	}
 	else
 	{
		if (gpNPCQuoteInfoArray[ubNPC] == NULL)
 		{
 			fLoadFile = TRUE;
 		}
		else if (gpNPCQuoteInfoArray[ubNPC][0].ubQuoteNum != 0 && gpNPCQuoteInfoArray[ubNPC][1].ubNumQuotes == 0)
 		{	// CHRISL: This will reset the quote file if, for some reason, the first quote exists but the 2nd doesn't
 			fLoadFile = TRUE;
 		}
 	}

	if ( gMercProfiles[ubNPC].Type == PROFILETYPE_RPC  )
	{
		if (gMercProfiles[ ubNPC ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED)
		{
			// recruited
			if ( gpBackupNPCQuoteInfoArray[ ubNPC ] == NULL )
			{
				// no backup stored of current script, so need to backup
				fLoadFile = TRUE;
				// set pointer to back up script!
				BackupOriginalQuoteFile( ubNPC );
			}
			// else have backup, are recruited, nothing special
		}
		else
		{
			// not recruited
			if ( gpBackupNPCQuoteInfoArray[ ubNPC ] != NULL )
			{
				// backup stored, restore backup
				RevertToOriginalQuoteFile( ubNPC );
			}
			// else are no backup, nothing special
		}
	}

	if ( fLoadFile )
	{
		gpNPCQuoteInfoArray[ubNPC] = LoadQuoteFile( ubNPC );
		if (gpNPCQuoteInfoArray[ubNPC] == NULL)
		{
#ifdef CRIPPLED_VERSION
			// make sure we're not trying to load NOPROFILE for some stupid reason
			if ( ubNPC != NO_PROFILE )
			{
				SOLDIERTYPE * pNull = NULL;
				pNull->stats.bLife = 0; // crash!
			}
#else

		#ifdef JA2TESTVERSION
			if (!gfTriedToLoadQuoteInfoArray[ubNPC]) // don't report the error a second time
			{

				ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"ERROR: NPC.C - NPC needs NPC file: %d.", ubNPC );
				gfTriedToLoadQuoteInfoArray[ubNPC] = TRUE;
			}
		#endif
#endif
			// error message at this point!
			return( FALSE );
		}
#ifdef CRIPPLED_VERSION
	// check a random record to make sure the bytes are set right in the header
	if ( gpNPCQuoteInfoArray[ubNPC][ 0 ].ubIdentifier[0] != '9')
	{
		if ( gpNPCQuoteInfoArray[ubNPC][ 0 ].ubIdentifier[2] != '5')
		{
			// crash!
			SOLDIERTYPE * pNull = NULL;
			pNull->stats.bLife = 0;
		}
	}
	
#endif
	}

	return( TRUE );
}

BOOLEAN ReloadQuoteFile( UINT8 ubNPC )
{
	if (gpNPCQuoteInfoArray[ubNPC] != NULL)
	{
		MemFree( gpNPCQuoteInfoArray[ubNPC] );
		gpNPCQuoteInfoArray[ubNPC] = NULL;
	}
	// zap backup if any
	if ( gpBackupNPCQuoteInfoArray[ ubNPC ] != NULL )
	{
		MemFree( gpBackupNPCQuoteInfoArray[ ubNPC ] );
		gpBackupNPCQuoteInfoArray[ ubNPC ] = NULL;
	}
	return( EnsureQuoteFileLoaded( ubNPC ) );
}

BOOLEAN ReloadQuoteFileIfLoaded( UINT8 ubNPC )
{
	if (gpNPCQuoteInfoArray[ubNPC] != NULL)
	{
		MemFree( gpNPCQuoteInfoArray[ubNPC] );
		gpNPCQuoteInfoArray[ubNPC] = NULL;
		return( EnsureQuoteFileLoaded( ubNPC ) );
	}
	else
	{
		return( TRUE );
	}
}

BOOLEAN RefreshNPCScriptRecord( UINT8 ubNPC, UINT8 ubRecord )
{
	UINT8 ubLoop;
	NPCQuoteInfo *		pNewArray;
	
	if ( ubNPC == NO_PROFILE )
	{
		// we have some work to do...
		// loop through all PCs, and refresh their copy of this record
		//for ( ubLoop = 0; ubLoop < FIRST_RPC; ubLoop++ ) // need more finesse here
		//new profiles by Jazz
		for ( ubLoop = 0; ubLoop < NUM_PROFILES; ++ubLoop ) // need more finesse here		
		{
			if ( gMercProfiles[ubLoop].Type == PROFILETYPE_AIM ||
				gMercProfiles[ubLoop].Type == PROFILETYPE_MERC ||
				gMercProfiles[ubLoop].Type == PROFILETYPE_IMP )
				RefreshNPCScriptRecord( ubLoop, ubRecord );
		}

		//new profiles by Jazz	
		//for ( ubLoop = FIRST_RPC; ubLoop < FIRST_NPC; ubLoop++ )
		for ( ubLoop = 0; ubLoop < NUM_PROFILES; ++ubLoop )
		{
			if ( gMercProfiles[ubLoop].Type == PROFILETYPE_RPC )
			{
				if ( gMercProfiles[ ubNPC ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED && gpBackupNPCQuoteInfoArray[ ubNPC ] != NULL )
				{
					RefreshNPCScriptRecord( ubLoop, ubRecord );
				}
			}
		}

		return( TRUE );
	}

	if ( gpNPCQuoteInfoArray[ ubNPC ] )
	{
		if ( CHECK_FLAG( gpNPCQuoteInfoArray[ ubNPC ][ ubRecord ].fFlags, QUOTE_FLAG_SAID ) )
		{
			// already used so we don't have to refresh!
			return( TRUE );
		}

		pNewArray = LoadQuoteFile( ubNPC );
		if ( pNewArray )
		{
			memcpy( &(gpNPCQuoteInfoArray[ ubNPC ][ ubRecord ]), &(pNewArray[ ubRecord ]), sizeof( NPCQuoteInfo ) );
			MemFree( pNewArray );
		}
	}

	return( TRUE );
}

//
// CIV QUOTE LOW LEVEL ROUTINES
//

NPCQuoteInfo * LoadCivQuoteFile( UINT8 ubIndex )
{
	CHAR8						zFileName[255];
	HWFILE					hFile;
	NPCQuoteInfo	* pFileData;
	UINT32					uiBytesRead;
	UINT32					uiFileSize;
	DWORD					uiSignature = 0; // SB // WANNE - BMP: DONE!

	if ( ubIndex == MINERS_CIV_QUOTE_INDEX )
	{
		sprintf( zFileName, "NPCData\\miners.npc" );
	}
	else
	{
	    sprintf( zFileName, "NPCData\\%c%d.npc", 'A' + ( gsCivQuoteSector[ ubIndex ][ 1 ] - 1 ), gsCivQuoteSector[ ubIndex ][ 0 ] );
	}

	CHECKN( FileExists( zFileName ) );

	hFile = FileOpen( zFileName, FILE_ACCESS_READ, FALSE );
	CHECKN( hFile );

	// -----------------------------------------------------------
	// WANNE: This fixes the bug, that NPCs do not give hints
	// Problem was, the missing conversion from old to new NPC structure!
	if (!FileRead( hFile, &uiSignature, sizeof(uiSignature), &uiBytesRead ) || uiBytesRead != sizeof(uiSignature) )
		return NULL;

	if(uiSignature == NPCEX_SIGNATURE)//new fmt
	{
		uiFileSize = sizeof( NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS;
		pFileData = (NPCQuoteInfo *)MemAlloc( uiFileSize );
		if (pFileData)
		{
			if (!FileRead( hFile, pFileData, uiFileSize, &uiBytesRead ) || uiBytesRead != uiFileSize )
			{
				MemFree( pFileData );
				pFileData = NULL;
			}
		}

		FileClose( hFile );
	}
	else //old fmt - make conversion
	{
		int iRecord;
		_old_NPCQuoteInfo * pFileData_old_;
		uiFileSize = sizeof( _old_NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS;
		pFileData_old_ = ( _old_NPCQuoteInfo * )MemAlloc( uiFileSize );
		FileSeek(hFile, 0, FILE_SEEK_FROM_START);
		if (pFileData_old_)
		{
			if (!FileRead( hFile, pFileData_old_, uiFileSize, &uiBytesRead ) || uiBytesRead != uiFileSize )
			{
				MemFree( pFileData_old_ );
				pFileData_old_ = NULL;
			}
		}
		
		FileClose( hFile );
		//check for Russian script & make a runtime conversion of it to International
		if( *(DWORD*)pFileData_old_ == 0x00350039 )
		{
			//just offset records 4 bytes backward
			_old_NPCQuoteInfo * pEnglishScript = ( _old_NPCQuoteInfo * )MemAlloc( uiFileSize );
			memcpy( pEnglishScript, ((char*)pFileData_old_)+4, uiFileSize-4 );
			MemFree( pFileData_old_ );
			pFileData_old_ = pEnglishScript;
		}

		// Now it's time for conversion
		uiFileSize = sizeof(NPCQuoteInfo) * NUM_NPC_QUOTE_RECORDS;
		pFileData = (NPCQuoteInfo*)MemAlloc(uiFileSize);
		for(iRecord=0; iRecord<NUM_NPC_QUOTE_RECORDS; iRecord++)
			pFileData[iRecord] = pFileData_old_[iRecord];//dnl ch46 021009
		MemFree(pFileData_old_);
	}

	// -----------------------------------------------------------

	// 
	//</SB> new script format
	return( pFileData );

	// -----------------------------------------------------------
	// WANNE: This is old vanilla JA2 code without conversion of NPC data to new format used for Big Maps project
	/*
	uiFileSize = sizeof( NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS;
	pFileData = (NPCQuoteInfo *)MemAlloc( uiFileSize );
	if (pFileData)
	{
		if (!FileRead( hFile, pFileData, uiFileSize, &uiBytesRead ) || uiBytesRead != uiFileSize )
		{
			MemFree( pFileData );
			pFileData = NULL;
		}
	}

	FileClose( hFile );

	return( pFileData );
	// -----------------------------------------------------------
	*/
}


BOOLEAN EnsureCivQuoteFileLoaded( UINT8 ubIndex )
{
	BOOLEAN			fLoadFile = FALSE;

	if (gpCivQuoteInfoArray[ubIndex] == NULL)
	{
		fLoadFile = TRUE;
	}

	if ( fLoadFile )
	{
		gpCivQuoteInfoArray[ubIndex] = LoadCivQuoteFile( ubIndex );
		if (gpCivQuoteInfoArray[ubIndex] == NULL)
		{
			return( FALSE );
		}
	}

	return( TRUE );
}

BOOLEAN ReloadCivQuoteFile( UINT8 ubIndex )
{
	if (gpCivQuoteInfoArray[ubIndex] != NULL)
	{
		MemFree( gpCivQuoteInfoArray[ubIndex] );
		gpCivQuoteInfoArray[ubIndex] = NULL;
	}
	return( EnsureCivQuoteFileLoaded( ubIndex ) );
}

BOOLEAN ReloadCivQuoteFileIfLoaded( UINT8 ubIndex )
{
	if (gpCivQuoteInfoArray[ubIndex] != NULL)
	{
		MemFree( gpCivQuoteInfoArray[ubIndex] );
		gpCivQuoteInfoArray[ubIndex] = NULL;
		return( EnsureCivQuoteFileLoaded( ubIndex ) );
	}
	else
	{
		return( TRUE );
	}
}

void ShutdownNPCQuotes( void )
{
	UINT8 ubLoop;

	for ( ubLoop = 0; ubLoop < NUM_PROFILES; ubLoop++ )
	{
		if ( gpNPCQuoteInfoArray[ ubLoop ] )
		{
			MemFree( gpNPCQuoteInfoArray[ ubLoop ] );
			gpNPCQuoteInfoArray[ ubLoop ] = NULL;
		}

		if ( gpBackupNPCQuoteInfoArray[ ubLoop ] != NULL )
		{
			MemFree( gpBackupNPCQuoteInfoArray[ ubLoop ] );
			gpBackupNPCQuoteInfoArray[ ubLoop ] = NULL;
		}

	}


	for ( ubLoop = 0; ubLoop < NUM_CIVQUOTE_SECTORS; ubLoop++ )
	{
		if ( gpCivQuoteInfoArray[ ubLoop ] )
		{
			MemFree( gpCivQuoteInfoArray[ ubLoop ] );
			gpCivQuoteInfoArray[ ubLoop ] = NULL;
		}
	}
}

//
// GENERAL LOW LEVEL ROUTINES
//

BOOLEAN ReloadAllQuoteFiles( void )
{
	for ( UINT8 ubProfile = 0; ubProfile < NUM_PROFILES; ubProfile++ )
	{
		if ( gMercProfiles[ubProfile].Type == PROFILETYPE_RPC ||
			gMercProfiles[ubProfile].Type == PROFILETYPE_NPC )
		{
			// zap backup if any
			if ( gpBackupNPCQuoteInfoArray[ ubProfile ] != NULL )
			{
				MemFree( gpBackupNPCQuoteInfoArray[ ubProfile ] );
				gpBackupNPCQuoteInfoArray[ ubProfile ] = NULL;
			}
			ReloadQuoteFileIfLoaded( ubProfile );
		}
	}

	// reload all civ quote files
	for ( UINT8 ubLoop = 0; ubLoop < NUM_CIVQUOTE_SECTORS; ubLoop++ )
	{
		ReloadCivQuoteFileIfLoaded( ubLoop );
	}

	return( TRUE );
}

//
// THE REST
//


void SetQuoteRecordAsUsed( UINT8 ubNPC, UINT8 ubRecord )
{
	if ( EnsureQuoteFileLoaded( ubNPC ) )
	{
		gpNPCQuoteInfoArray[ ubNPC ][ ubRecord ].fFlags |= QUOTE_FLAG_SAID;
	}
}

INT32 CalcThreateningEffectiveness( UINT8 ubMerc )
{
	SOLDIERTYPE * pSoldier;
	INT32					iStrength, iDeadliness;

	// effective threat is 1/3 strength, 1/3 weapon deadliness, 1/3 leadership

	pSoldier = FindSoldierByProfileID( ubMerc, TRUE );

	if ( !pSoldier )
	{
		return( 0 );
	}

	iStrength = EffectiveStrength( pSoldier, TRUE );

	if ( Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass & IC_WEAPON )
	{
		iDeadliness = Weapon[ pSoldier->inv[HANDPOS].usItem ].ubDeadliness;
	}
	else
	{
		iDeadliness = 0;
	}

	if ( iDeadliness == 0 )
	{
		// penalize!
		iDeadliness = -30;
	}

	// SANDRO - bonus for threatening for assertive people
	if ( gGameOptions.fNewTraitSystem && DoesMercHavePersonality( pSoldier, CHAR_TRAIT_ASSERTIVE ) )
	{
		iDeadliness += 50;
	}

	return( (EffectiveLeadership( pSoldier ) + iStrength + iDeadliness) / 3 ); //Trail minor bug fix.
}

INT32 GetEffectiveApproachValue( UINT8 usProfile, UINT8 usApproach, CHAR16* apStr )
{
	if ( usApproach >  APPROACH_RECRUIT )
		return 100;

	FLOAT val = 0;
	CHAR16	atStr[500];
	swprintf( atStr, L"" );
				
	if ( usApproach == APPROACH_THREATEN )
	{
		INT32 threateneffectiveness = CalcThreateningEffectiveness( usProfile );

		if ( apStr )
		{
			swprintf( atStr, szLaptopStatText[0], threateneffectiveness );
			wcscat( apStr, atStr );
		}

		val = threateneffectiveness;
	}
	else
	{
		if ( apStr )
		{
			swprintf( atStr, szLaptopStatText[1], gMercProfiles[usProfile].bLeadership );
			wcscat( apStr, atStr );
		}

		val = ((INT32)gMercProfiles[usProfile].bLeadership);
	}
	
	FLOAT approachfactor = (FLOAT)(gMercProfiles[usProfile].usApproachFactor[usApproach - 1]) / 100.0f;

	if ( apStr )
	{
		swprintf( atStr, szLaptopStatText[2], approachfactor );
		wcscat( apStr, atStr );
	}

	val *= approachfactor;

	// Flugente: backgrounds
	SOLDIERTYPE* pSoldier = FindSoldierByProfileID( usProfile, TRUE );

	if ( pSoldier )
	{
		UINT8 bgprperty = 0;

		if ( usApproach == APPROACH_FRIENDLY )
			bgprperty = BG_PERC_APPROACH_FRIENDLY;
		else if ( usApproach == APPROACH_DIRECT )
			bgprperty = BG_PERC_APPROACH_DIRECT;
		else if ( usApproach == APPROACH_THREATEN )
			bgprperty = BG_PERC_APPROACH_THREATEN;
		else if ( usApproach == APPROACH_RECRUIT )
			bgprperty = BG_PERC_APPROACH_RECRUIT;

		FLOAT bgmodifier = (FLOAT)((100.0f + pSoldier->GetBackgroundValue( bgprperty ))) / 100.0f;

		if ( apStr )
		{
			swprintf( atStr, szLaptopStatText[3], bgmodifier );
			wcscat( apStr, atStr );
		}

		val = val * bgmodifier;

		if ( apStr && gGameOptions.fNewTraitSystem && usApproach != APPROACH_THREATEN )
		{
			if ( DoesMercHavePersonality( pSoldier, CHAR_TRAIT_ASSERTIVE ) )
			{
				swprintf( atStr, L"  \n" );
				wcscat( apStr, atStr );

				swprintf( atStr, szLaptopStatText[4] );
				wcscat( apStr, atStr );
			}
			else if ( DoesMercHavePersonality( pSoldier, CHAR_TRAIT_MALICIOUS ) )
			{
				swprintf( atStr, L"  \n" );
				wcscat( apStr, atStr );

				swprintf( atStr, szLaptopStatText[5] );
				wcscat( apStr, atStr );
			}
		}
	}

	return (INT32)val;
}

UINT8 CalcDesireToTalk( UINT8 ubNPC, UINT8 ubMerc, INT8 bApproach )
{
	INT32 iWillingness;
	INT32	iPersonalVal, iTownVal, iApproachVal;
	INT32 iEffectiveLeadership;
	MERCPROFILESTRUCT * pNPCProfile;
	MERCPROFILESTRUCT * pMercProfile;

	pNPCProfile = &(gMercProfiles[ubNPC]);
	pMercProfile = &(gMercProfiles[ubMerc]);

	iPersonalVal = 50; /* + pNPCProfile->bMercTownReputation[ pNPCProfile->bTown ] */;
	if (OKToCheckOpinion(ubMerc)) {
		iPersonalVal += pNPCProfile->bMercOpinion[ubMerc];
	}

	// SANDRO - bonus for communication with people for assertive people
	if ( gGameOptions.fNewTraitSystem && bApproach != APPROACH_THREATEN)
	{
		INT16 id = GetSoldierIDFromMercID( ubMerc );
		if ( id > -1 )
		{
			if ( DoesMercHavePersonality( MercPtrs[id], CHAR_TRAIT_ASSERTIVE ) )
				iPersonalVal += 50;
			else if ( DoesMercHavePersonality( MercPtrs[id], CHAR_TRAIT_MALICIOUS ) )
				iPersonalVal -= 50;
		}
	}

	// ARM: NOTE - for towns which don't use loyalty (San Mona, Estoni, Tixa, Orta )
	// loyalty will always remain 0 (this was OKed by Ian)
	iTownVal = gTownLoyalty[ pNPCProfile->bTown ].ubRating;
	iTownVal = iTownVal * pNPCProfile->bTownAttachment / 100;

	if (bApproach == NPC_INITIATING_CONV || bApproach == APPROACH_GIVINGITEM )
	{
		iApproachVal = 100;
	}
	else
	{
		iEffectiveLeadership = GetEffectiveApproachValue( ubMerc, bApproach, NULL );
				
		iApproachVal = pNPCProfile->ubApproachVal[bApproach - 1] * iEffectiveLeadership / 50;
	}

	// NB if town attachment is less than 100% then we should make personal value proportionately more important!
	if ( pNPCProfile->bTownAttachment < 100 )
	{
		iPersonalVal = iPersonalVal * (100 + (100 - pNPCProfile->bTownAttachment) ) / 100;
	}
	iWillingness = (iPersonalVal / 2 + iTownVal / 2) * iApproachVal	/ 100 - gubTeamPenalty;

	if (bApproach == NPC_INITIATING_CONV)
	{
		iWillingness -= INITIATING_FACTOR;
	}

	if (iWillingness < 0)
	{
		iWillingness = 0;
	}

	return( (UINT8) iWillingness );
}

void ApproachedForFirstTime( MERCPROFILESTRUCT * pNPCProfile, INT8 bApproach )
{
	UINT8 ubLoop;
	UINT32	uiTemp;

	pNPCProfile->bApproached |= gbFirstApproachFlags[bApproach - 1];
	for (ubLoop = 1; ubLoop <= NUM_REAL_APPROACHES; ubLoop++)
	{
		uiTemp = (UINT32) pNPCProfile->ubApproachVal[ubLoop - 1] * (UINT32) pNPCProfile->ubApproachMod[bApproach - 1][ubLoop - 1] / 100;
		if (uiTemp > 255)
		{
			uiTemp = 255;
		}
		pNPCProfile->ubApproachVal[ubLoop-1] = (UINT8) uiTemp;
	}
}

UINT8 NPCConsiderTalking( UINT8 ubNPC, UINT8 ubMerc, INT8 bApproach, UINT8 ubRecord, NPCQuoteInfo * pNPCQuoteInfoArray, NPCQuoteInfo ** ppResultQuoteInfo, UINT8 *pubQuoteNum )
{
	// This function returns the opinion level required of the "most difficult" quote
	// that the NPC is willing to say to the merc.  It can also provide the quote #.
	MERCPROFILESTRUCT *		pNPCProfile=NULL;
	NPCQuoteInfo *				pNPCQuoteInfo=NULL;
	UINT8									ubTalkDesire, ubLoop, ubQuote, ubHighestOpinionRequired = 0;
	BOOLEAN								fQuoteFound = FALSE;
	UINT32								uiDay;
	UINT8									ubFirstQuoteRecord, ubLastQuoteRecord;
	SOLDIERTYPE						*pSoldier=NULL;

	ubTalkDesire = ubQuote = 0;

	pSoldier = FindSoldierByProfileID( ubNPC, FALSE );
	if (pSoldier == NULL)
	{
		return( 0 );
	}

	if (ppResultQuoteInfo)
	{
		(*ppResultQuoteInfo) = NULL;
	}

	if ( pubQuoteNum )
	{
		(*pubQuoteNum) = 0;
	}

	if (bApproach <= NUM_REAL_APPROACHES)
	{
		pNPCProfile = &(gMercProfiles[ubNPC]);

		// What's our willingness to divulge?
		ubTalkDesire = CalcDesireToTalk( ubNPC, ubMerc, bApproach );

		if ( bApproach < NUM_REAL_APPROACHES && !(pNPCProfile->bApproached & gbFirstApproachFlags[bApproach - 1]) )
		{
			ApproachedForFirstTime( pNPCProfile, bApproach );
		}
	}
	else if ( ubNPC == PABLO && bApproach == APPROACH_SECTOR_NOT_SAFE ) // for Pablo, consider as threaten
	{
		pNPCProfile = &(gMercProfiles[ubNPC]);

		// What's our willingness to divulge?
		ubTalkDesire = CalcDesireToTalk( ubNPC, ubMerc, APPROACH_THREATEN );

		if ( pNPCProfile->bApproached & gbFirstApproachFlags[APPROACH_THREATEN - 1] )
		{
			ApproachedForFirstTime( pNPCProfile, APPROACH_THREATEN );
		}
	}

	switch( bApproach )
	{
	/*
		case APPROACH_RECRUIT:
			ubFirstQuoteRecord = 0;
			ubLastQuoteRecord = 0;
			break;
			*/
		case TRIGGER_NPC:
			ubFirstQuoteRecord = ubRecord;
			ubLastQuoteRecord = ubRecord;
			break;
		default:
			ubFirstQuoteRecord = 0;
			ubLastQuoteRecord = NUM_NPC_QUOTE_RECORDS - 1;
			break;
	}

	uiDay = GetWorldDay();

	for (ubLoop = ubFirstQuoteRecord; ubLoop <= ubLastQuoteRecord; ubLoop++)
	{
		pNPCQuoteInfo = &(pNPCQuoteInfoArray[ ubLoop ]);

		// Check if we have the item / are in right spot
		if ( pNPCQuoteInfo->sRequiredItem > 0 )
		{
			if ( !ObjectExistsInSoldierProfile( ubNPC, pNPCQuoteInfo->sRequiredItem )  )
			{
				continue;
			}
		}		
		else if(!TileIsOutOfBounds(pNPCQuoteInfo->sRequiredGridNo))//dnl ch46 031009
		{
			if(pSoldier->sGridNo != pNPCQuoteInfo->sRequiredGridNo)
				continue;
		}

		if ( NPCConsiderQuote( ubNPC, ubMerc, bApproach, ubLoop, ubTalkDesire, pNPCQuoteInfoArray ) )
		{
			if (bApproach == NPC_INITIATING_CONV)
			{
				// want to find the quote with the highest required opinion rating that we're willing
				// to say
				if ( pNPCQuoteInfo->ubOpinionRequired > ubHighestOpinionRequired )
				{
					fQuoteFound = TRUE;
					ubHighestOpinionRequired = pNPCQuoteInfo->ubOpinionRequired;
					ubQuote = pNPCQuoteInfo->ubQuoteNum;
				}
			}
			else
			{
				// we do have a quote to say, and we want to say this one right away!
				if (ppResultQuoteInfo)
				{
					(*ppResultQuoteInfo) = pNPCQuoteInfo;
				}
				if ( pubQuoteNum )
				{
					(*pubQuoteNum) = ubLoop;
				}

				return( pNPCQuoteInfo->ubOpinionRequired );
			}
		}

	}

	// Whew, checked them all.  If we found a quote, return the appropriate values.
	if (fQuoteFound)
	{
		if (ppResultQuoteInfo)
		{
			(*ppResultQuoteInfo) = pNPCQuoteInfo;
		}
		if ( pubQuoteNum )
		{
			(*pubQuoteNum) = ubQuote;
		}

		return( ubHighestOpinionRequired );
	}
	else
	{
		if (ppResultQuoteInfo)
		{
			(*ppResultQuoteInfo) = NULL;
		}
		if ( pubQuoteNum )
		{
			(*pubQuoteNum) = 0;
		}
		return( 0 );
	}
}

UINT8 NPCConsiderReceivingItemFromMerc( UINT8 ubNPC, UINT8 ubMerc, OBJECTTYPE * pObj, NPCQuoteInfo * pNPCQuoteInfoArray, NPCQuoteInfo ** ppResultQuoteInfo, UINT8 *pubQuoteNum )
{
	// This function returns the opinion level required of the "most difficult" quote
	// that the NPC is willing to say to the merc.  It can also provide the quote #.
	MERCPROFILESTRUCT *		pNPCProfile;
	NPCQuoteInfo *				pNPCQuoteInfo;
	UINT8									ubTalkDesire, ubLoop, ubHighestOpinionRequired = 0;
	BOOLEAN								fQuoteFound = FALSE;
	UINT8									ubFirstQuoteRecord, ubLastQuoteRecord;
	UINT16								usItemToConsider;

	(*ppResultQuoteInfo) = NULL;
	(*pubQuoteNum)			 = 0;

	if ( CheckFact( FACT_NPC_IS_ENEMY, ubNPC ) && ubNPC != JOE )
	{
		// don't accept any items when we are the player's enemy
		return( 0 );
	}

	pNPCProfile = &(gMercProfiles[ubNPC]);

	// How much do we want to talk with this merc?

	ubTalkDesire = CalcDesireToTalk( ubNPC, ubMerc, APPROACH_GIVINGITEM );

	ubFirstQuoteRecord = 0;
	ubLastQuoteRecord = NUM_NPC_QUOTE_RECORDS - 1;

	usItemToConsider = pObj->usItem;
	if ( Item[ usItemToConsider ].usItemClass == IC_GUN && !Item[usItemToConsider].rocketlauncher  )
	{
		UINT8 ubWeaponClass;

		ubWeaponClass = Weapon[ usItemToConsider ].ubWeaponClass;
		if ( ubWeaponClass == RIFLECLASS || ubWeaponClass == MGCLASS )
		{
			usItemToConsider = ANY_RIFLE; // treat all rifles the same
		}
	}
	switch( usItemToConsider )
	{
		case HEAD_2:
		case HEAD_3:
		//case HEAD_4: // NOT Slay's head; it's different
		case HEAD_5:
		case HEAD_6:
		case HEAD_7:
			// all treated the same in the NPC code
			usItemToConsider = HEAD_2;
			break;
		case MONEY:
		case SILVER:
		case GOLD:
			if ((*pObj)[0]->data.money.uiMoneyAmount < LARGE_AMOUNT_MONEY)
			{
				SetFactTrue( FACT_SMALL_AMOUNT_OF_MONEY );
			}
			else
			{
				SetFactTrue( FACT_LARGE_AMOUNT_OF_MONEY );
			}

			// anv: Waldo!
			if ((*pObj)[0]->data.money.uiMoneyAmount >= CalculateHelicopterRepairCost( FALSE ) )
			{
				SetFactTrue( FACT_GIVEN_ENOUGH_TO_REPAIR_HELI );
			}
			if ((*pObj)[0]->data.money.uiMoneyAmount >= CalculateHelicopterRepairCost( TRUE ) )
			{
				SetFactTrue( FACT_GIVEN_ENOUGH_TO_SERIOUSLY_REPAIR_HELI );
			}
			break;
		case WINE:
		case BEER:
			usItemToConsider = ALCOHOL;
			break;
		default:
			break;
	}

	if ((*pObj)[0]->data.objectStatus < 50)
	{
		SetFactTrue( FACT_ITEM_POOR_CONDITION );
	}
	else
	{
		SetFactFalse( FACT_ITEM_POOR_CONDITION );
	}

	for (ubLoop = ubFirstQuoteRecord; ubLoop <= ubLastQuoteRecord; ubLoop++)
	{
		pNPCQuoteInfo = &(pNPCQuoteInfoArray[ ubLoop ]);

		// First see if we want that item....
		if ( pNPCQuoteInfo->sRequiredItem > 0 && ( pNPCQuoteInfo->sRequiredItem == usItemToConsider || pNPCQuoteInfo->sRequiredItem == ACCEPT_ANY_ITEM ) )
		{
			// Now see if everyhting else is OK
			if ( NPCConsiderQuote( ubNPC, ubMerc, APPROACH_GIVINGITEM, ubLoop, ubTalkDesire, pNPCQuoteInfoArray ) )
			{
				switch( ubNPC )
				{
					case DARREN:
						if (usItemToConsider == MONEY && pNPCQuoteInfo->sActionData == NPC_ACTION_DARREN_GIVEN_CASH)
						{
							if ((*pObj)[0]->data.money.uiMoneyAmount < 1000)
							{
								// refuse, bet too low - record 15
								(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[15];
								(*pubQuoteNum)       = 15;
								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
							else if ((*pObj)[0]->data.money.uiMoneyAmount > 5000)
							{
								// refuse, bet too high - record 16
								(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[16];
								(*pubQuoteNum)       = 16;
								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
							else
							{
								// accept - record 17
								/*
								{

									SOLDIERTYPE *					pSoldier;
									INT8									bMoney;
									INT8									bEmptySlot;

									pSoldier = FindSoldierByProfileID( DARREN, FALSE );
									bMoney = FindObjWithin( pSoldier, MONEY, BIGPOCK1POS, SMALLPOCK8POS );
									bEmptySlot = FindObjWithin( pSoldier, NOTHING, BIGPOCK1POS, SMALLPOCK8POS );
								}
								*/

								// record amount of bet
								gMercProfiles[ DARREN ].iBalance = (*pObj)[0]->data.money.uiMoneyAmount;
								SetFactFalse( FACT_DARREN_EXPECTING_MONEY );

								// if never fought before, use record 17
								// if fought before, today, use record 31
								// else use record 18
								if ( ! ( gpNPCQuoteInfoArray[ DARREN ][ 17 ].fFlags & QUOTE_FLAG_SAID ) ) // record 17 not used
								{
									(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[17];
									(*pubQuoteNum)       = 17;
								}
								else
								{
									// find Kingpin, if he's in his house, invoke the script to move him to the bar
									SOLDIERTYPE *		pKingpin;
									//DBrot: More Rooms
									//UINT8						ubKingpinRoom;
									UINT16 usKingpinRoom;

									pKingpin = FindSoldierByProfileID( KINGPIN, FALSE );
									if ( pKingpin && InARoom( pKingpin->sGridNo, &usKingpinRoom ) )
									{
										if ( IN_KINGPIN_HOUSE( usKingpinRoom ) )
										{
											// first boxer, bring kingpin over
											(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[17];
											(*pubQuoteNum)       = 17;
										}
										else
										{
											(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[31];
											(*pubQuoteNum)       = 31;
										}
									}
									else
									{
										(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[31];
										(*pubQuoteNum)       = 31;
									}

								}

								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
						}
						break;
					case ANGEL:
						if (usItemToConsider == MONEY && pNPCQuoteInfo->sActionData == NPC_ACTION_ANGEL_GIVEN_CASH)
						{
							if ((*pObj)[0]->data.money.uiMoneyAmount < Item[LEATHER_JACKET_W_KEVLAR].usPrice)
							{
								// refuse, bet too low - record 8
								(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[8];
								(*pubQuoteNum)       = 8;
								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
							else if ((*pObj)[0]->data.money.uiMoneyAmount > Item[LEATHER_JACKET_W_KEVLAR].usPrice)
							{
								// refuse, bet too high - record 9
								(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[9];
								(*pubQuoteNum)       = 9;
								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
							else
							{
								// accept - record 10
								(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[10];
								(*pubQuoteNum)       = 10;
								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
						}
						break;
					case MADAME:
						if ( usItemToConsider == MONEY )
						{
							if ( gMercProfiles[ ubMerc ].bSex == FEMALE )
							{
								// say quote about not catering to women!
								(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[5];
								(*pubQuoteNum)       = 5;
								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
							switch( (*pObj)[0]->data.money.uiMoneyAmount )
							{
								case 100:
								case 200: // Carla
									if ( CheckFact( FACT_CARLA_AVAILABLE, 0 ) )
									{
										gMercProfiles[ MADAME ].bNPCData += (INT8) ((*pObj)[0]->data.money.uiMoneyAmount / 100);
										TriggerNPCRecord( MADAME, 16 );
									}
									else
									{
										// see default case
										(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[25];
										(*pubQuoteNum)       = 25;
										return( (*ppResultQuoteInfo)->ubOpinionRequired );
									}
									break;
								case 500:
								case 1000: // Cindy
									if ( CheckFact( FACT_CINDY_AVAILABLE, 0 ) )
									{
										gMercProfiles[ MADAME ].bNPCData += (INT8) ((*pObj)[0]->data.money.uiMoneyAmount / 500);
										TriggerNPCRecord( MADAME, 17 );
									}
									else
									{
										// see default case
										(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[25];
										(*pubQuoteNum)       = 25;
										return( (*ppResultQuoteInfo)->ubOpinionRequired );
									}
									break;
								case 300:
								case 600: // Bambi
									if ( CheckFact( FACT_BAMBI_AVAILABLE, 0 ) )
									{
										gMercProfiles[ MADAME ].bNPCData += (INT8) ((*pObj)[0]->data.money.uiMoneyAmount / 300);
										TriggerNPCRecord( MADAME, 18 );
									}
									else
									{
										// see default case
										(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[25];
										(*pubQuoteNum)       = 25;
										return( (*ppResultQuoteInfo)->ubOpinionRequired );
									}
									break;
								case 400:
								case 800: // Maria?
									if ( gubQuest[ QUEST_RESCUE_MARIA ] == QUESTINPROGRESS )
									{
										gMercProfiles[ MADAME ].bNPCData += (INT8) ((*pObj)[0]->data.money.uiMoneyAmount / 400);
										TriggerNPCRecord( MADAME, 19 );
										break;
									}
									else
									{
										// see default case
										(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[25];
										(*pubQuoteNum)       = 25;
										return( (*ppResultQuoteInfo)->ubOpinionRequired );
									}
									break;
								default:
									// play quotes 39-42 (plus 44 if quest 22 on) plus 43 if >1 PC
									// and return money
									(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[25];
									(*pubQuoteNum)       = 25;
									return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}

						}
						break;
					case JOE:
						if ( ubNPC == JOE && usItemToConsider == MONEY && pNPCQuoteInfo->sActionData != NPC_ACTION_JOE_GIVEN_CASH )
						{
							break;
						}
						// else fall through
					case GERARD:
						if ( ubNPC == GERARD && usItemToConsider == MONEY && pNPCQuoteInfo->sActionData != NPC_ACTION_GERARD_GIVEN_CASH )
						{
							break;
						}
						// else fall through
					case STEVE:
					case VINCE:
					case WALTER:
					case FRANK:
						if (usItemToConsider == MONEY)
						{
							if ( ubNPC == VINCE || ubNPC == STEVE )
							{
								if ( CheckFact( FACT_VINCE_EXPECTING_MONEY, ubNPC ) == FALSE && gMercProfiles[ ubNPC ].iBalance < 0 && pNPCQuoteInfo->sActionData != NPC_ACTION_DONT_ACCEPT_ITEM )
								{
									// increment balance
									gMercProfiles[ ubNPC ].iBalance += (INT32) (*pObj)[0]->data.money.uiMoneyAmount;
									gMercProfiles[ ubNPC ].uiTotalCostToDate += (*pObj)[0]->data.money.uiMoneyAmount;
									if ( gMercProfiles[ ubNPC ].iBalance > 0 )
									{
										gMercProfiles[ ubNPC ].iBalance = 0;
									}
									ScreenMsg( FONT_YELLOW, MSG_INTERFACE, TacticalStr[ BALANCE_OWED_STR ], gMercProfiles[ubNPC].zNickname, -gMercProfiles[ubNPC].iBalance );
								}
								else if ( CheckFact( FACT_VINCE_EXPECTING_MONEY, ubNPC ) == FALSE && pNPCQuoteInfo->sActionData != NPC_ACTION_DONT_ACCEPT_ITEM )
								{
									// just accept cash!
									if ( ubNPC == VINCE )
									{
										(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[ 8 ];
									}
									else
									{
										(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[ 7 ];
									}
									return( (*ppResultQuoteInfo)->ubOpinionRequired );
								}
								else
								{
									// handle the player giving NPC some money
									HandleNPCBeingGivenMoneyByPlayer( ubNPC, (*pObj)[0]->data.money.uiMoneyAmount, pubQuoteNum );
									(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[ *pubQuoteNum ];
									return( (*ppResultQuoteInfo)->ubOpinionRequired );
								}
							}
							else
							{
								// handle the player giving NPC some money
								HandleNPCBeingGivenMoneyByPlayer( ubNPC, (*pObj)[0]->data.money.uiMoneyAmount, pubQuoteNum );
								(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[ *pubQuoteNum ];
								return( (*ppResultQuoteInfo)->ubOpinionRequired );
							}
						}
						break;
					case KINGPIN:
						if ( usItemToConsider == MONEY && gubQuest[ QUEST_KINGPIN_MONEY ] == QUESTINPROGRESS )
						{
							HandleNPCBeingGivenMoneyByPlayer( ubNPC, (*pObj)[0]->data.money.uiMoneyAmount, pubQuoteNum );
							(*ppResultQuoteInfo) = &pNPCQuoteInfoArray[ *pubQuoteNum ];
							return( (*ppResultQuoteInfo)->ubOpinionRequired );
						}
						break;
					default:
						//if ( usItemToConsider == MONEY && (ubNPC == SKYRIDER || (ubNPC >= FIRST_RPC && ubNPC < FIRST_NPC) ) )
						//new profiles by Jazz
						if ( usItemToConsider == MONEY && (ubNPC == SKYRIDER || gMercProfiles[ubNPC].Type == PROFILETYPE_RPC ) )
						{
							if ( gMercProfiles[ ubNPC ].iBalance < 0 && pNPCQuoteInfo->sActionData != NPC_ACTION_DONT_ACCEPT_ITEM )
							{
								// increment balance
								gMercProfiles[ ubNPC ].iBalance += (INT32) (*pObj)[0]->data.money.uiMoneyAmount;
								gMercProfiles[ ubNPC ].uiTotalCostToDate += (*pObj)[0]->data.money.uiMoneyAmount;
								if ( gMercProfiles[ ubNPC ].iBalance > 0 )
								{
									gMercProfiles[ ubNPC ].iBalance = 0;
								}
								ScreenMsg( FONT_YELLOW, MSG_INTERFACE, TacticalStr[ BALANCE_OWED_STR ], gMercProfiles[ubNPC].zNickname, -gMercProfiles[ubNPC].iBalance );
							}
						}
						break;
				}
				// This is great!
				// Return desire value
				(*ppResultQuoteInfo) = pNPCQuoteInfo;
				(*pubQuoteNum)       = ubLoop;

				return( pNPCQuoteInfo->ubOpinionRequired );
			}
		}
	}

	return( 0 );
}

BOOLEAN HandleNPCBeingGivenMoneyByPlayer( UINT8 ubNPC, UINT32 uiMoneyAmount, UINT8 *pQuoteValue )
{
	switch( ubNPC )
	{
		// handle for STEVE and VINCE
		case STEVE:
		case VINCE:
			{
				INT32		iCost;

				iCost = (INT32) CalcMedicalCost( ubNPC );

				// check amount of money
				if ( (INT32)uiMoneyAmount + giHospitalTempBalance + giHospitalRefund >= iCost )
				{
					// enough cash, check how much help is needed
					if( CheckFact( FACT_WOUNDED_MERCS_NEARBY , ubNPC) )
					{
						*pQuoteValue = 26;
					}
					else if( CheckFact( FACT_ONE_WOUNDED_MERC_NEARBY, ubNPC ) )
					{
						*pQuoteValue = 25;
					}

					if ( giHospitalRefund > 0 )
					{
						giHospitalRefund = __max( 0, giHospitalRefund - iCost + uiMoneyAmount );
					}
					giHospitalTempBalance = 0;
				}
				else
				{
					CHAR16		sTempString[ 100 ];

					swprintf( sTempString, L"%ld", iCost - uiMoneyAmount - giHospitalTempBalance );
					InsertDollarSignInToString( sTempString );

					// not enough cash
					ScreenMsg( FONT_MCOLOR_LTYELLOW,
						MSG_INTERFACE,
						Message[ STR_NEED_TO_GIVE_MONEY ],
						gMercProfiles[ ubNPC ].zNickname,
						sTempString );
					*pQuoteValue = 27;
					giHospitalTempBalance += uiMoneyAmount;
				}
			}
			break;
		case KINGPIN:
			if ( (INT32) uiMoneyAmount < -gMercProfiles[ KINGPIN ].iBalance )
			{
				*pQuoteValue = 9;
			}
			else
			{
				*pQuoteValue = 10;
			}
			gMercProfiles[ KINGPIN ].iBalance += (INT32) uiMoneyAmount;
			break;
		case WALTER:
			if ( gMercProfiles[ WALTER ].iBalance == 0 )
			{
				*pQuoteValue = 12;
			}
			else
			{
				*pQuoteValue = 13;
			}
			gMercProfiles[ WALTER ].iBalance += uiMoneyAmount;
			break;
		case FRANK:
			gArmsDealerStatus[ ARMS_DEALER_FRANK ].uiArmsDealersCash += uiMoneyAmount;
			break;
		case GERARD:
			gMercProfiles[ GERARD ].iBalance += uiMoneyAmount;
			if ( (gMercProfiles[ GERARD ].iBalance) >= 10000 )
			{
				*pQuoteValue = 12;
			}
			else
			{
				*pQuoteValue = 11;
			}
			break;
		case JOE:
			gMercProfiles[ JOE ].iBalance += uiMoneyAmount;
			if ( (gMercProfiles[ JOE ].iBalance) >= 10000 )
			{
				*pQuoteValue = 7;
			}
			else
			{
				*pQuoteValue = 6;
			}
			break;
	}

	return( TRUE );
}

UINT8 NPCConsiderQuote( UINT8 ubNPC, UINT8 ubMerc, UINT8 ubApproach, UINT8 ubQuoteNum, UINT8 ubTalkDesire, NPCQuoteInfo * pNPCQuoteInfoArray )
{
	//This function looks at a quote and determines if conditions for it have been met.
	// Returns 0 if none , 1 if one is found
	MERCPROFILESTRUCT *		pNPCProfile;
	NPCQuoteInfo *				pNPCQuoteInfo;
	UINT32								uiDay;
	BOOLEAN								fTrue;

	if ( ubNPC == NO_PROFILE )
	{
		pNPCProfile = NULL;
	}
	else
	{
		pNPCProfile = &(gMercProfiles[ubNPC]);
	}

	// How much do we want to talk with this merc?
	uiDay = GetWorldDay();

	pNPCQuoteInfo = &(pNPCQuoteInfoArray[ubQuoteNum]);

	#ifdef JA2TESTVERSION
		if ( ubNPC != NO_PROFILE && ubMerc != NO_PROFILE )
		{
			NpcRecordLoggingInit( ubNPC, ubMerc, ubQuoteNum, ubApproach );
		}
	#endif

	if (CHECK_FLAG( pNPCQuoteInfo->fFlags, QUOTE_FLAG_SAID ))
	{
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
			NpcRecordLogging( ubApproach, "Quote Already Said, leaving");
		#endif
		// skip quotes already said
		return( FALSE );
	}

	// if the quote is quest-specific, is the player on that quest?
	if (pNPCQuoteInfo->ubQuest != NO_QUEST)
	{
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
			NpcRecordLogging( ubApproach, "Quest(%d:'%S') Must be in Progress, status is %d. %s", pNPCQuoteInfo->ubQuest, QuestDescText[ pNPCQuoteInfo->ubQuest ], gubQuest[pNPCQuoteInfo->ubQuest], (gubQuest[pNPCQuoteInfo->ubQuest] != QUESTINPROGRESS) ? "False, return" : "True" );
		#endif

		if (pNPCQuoteInfo->ubQuest > QUEST_DONE_NUM)
		{
			if (gubQuest[pNPCQuoteInfo->ubQuest - QUEST_DONE_NUM] != QUESTDONE)
			{
				return( FALSE );
			}
		}
		else if (pNPCQuoteInfo->ubQuest > QUEST_NOT_STARTED_NUM)
		{
			if (gubQuest[pNPCQuoteInfo->ubQuest - QUEST_NOT_STARTED_NUM] != QUESTNOTSTARTED)
			{
				return( FALSE );
			}
		}
		else
		{
			if (gubQuest[pNPCQuoteInfo->ubQuest] != QUESTINPROGRESS)
			{
				return( FALSE );
			}
		}
	}

	// if there are facts to be checked, check them
	if (pNPCQuoteInfo->usFactMustBeTrue != NO_FACT)
	{
		fTrue = CheckFact( pNPCQuoteInfo->usFactMustBeTrue, ubNPC );
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
			NpcRecordLogging( ubApproach, "Fact (%d:'%S') Must be True, status is %s", pNPCQuoteInfo->usFactMustBeTrue, FactDescText[pNPCQuoteInfo->usFactMustBeTrue], (fTrue == FALSE) ? "False, returning" : "True" );
		#endif
		if (fTrue == FALSE)
		{
			return( FALSE );
		}


	}

	if (pNPCQuoteInfo->usFactMustBeFalse != NO_FACT)
	{
		fTrue = CheckFact( pNPCQuoteInfo->usFactMustBeFalse, ubNPC );

		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
			NpcRecordLogging( ubApproach, "Fact(%d:'%S') Must be False status is  %s", pNPCQuoteInfo->usFactMustBeFalse, FactDescText[pNPCQuoteInfo->usFactMustBeFalse], (fTrue == TRUE) ? "True, return" : "FALSE" );
		#endif

		if (fTrue == TRUE)
		{

			return( FALSE );
		}
	}

	// check for required approach
	// since the "I hate you" code triggers the record, triggering has to work properly
	// with the other value that is stored!
	if ( pNPCQuoteInfo->ubApproachRequired || !(ubApproach == APPROACH_FRIENDLY || ubApproach == APPROACH_DIRECT || ubApproach == TRIGGER_NPC ) )
	{
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
		NpcRecordLogging( ubApproach, "Approach Taken(%d) must equal required Approach(%d) = %s", ubApproach, pNPCQuoteInfo->ubApproachRequired, (ubApproach != pNPCQuoteInfo->ubApproachRequired) ? "TRUE, return" : "FALSE" );
		#endif

		if ( pNPCQuoteInfo->ubApproachRequired == APPROACH_ONE_OF_FOUR_STANDARD )
		{
			// friendly to recruit will match
			if ( ubApproach < APPROACH_FRIENDLY || ubApproach > APPROACH_RECRUIT )
			{
				return( FALSE );
			}
		}
		else if ( pNPCQuoteInfo->ubApproachRequired == APPROACH_FRIENDLY_DIRECT_OR_RECRUIT )
		{
			if ( ubApproach != APPROACH_FRIENDLY && ubApproach != APPROACH_DIRECT && ubApproach != APPROACH_RECRUIT )
			{
				return( FALSE );
			}
		}
		else if (ubApproach != pNPCQuoteInfo->ubApproachRequired)
		{
			return( FALSE );
		}
	}

	// check time constraints on the quotes
	if (pNPCProfile != NULL && pNPCQuoteInfo->ubFirstDay == MUST_BE_NEW_DAY)
	{
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
			NpcRecordLogging( ubApproach, "Time constraints. Current Day(%d) must <= Day last spoken too (%d) : %s", uiDay, pNPCProfile->ubLastDateSpokenTo, (uiDay <= pNPCProfile->ubLastDateSpokenTo) ? "TRUE, return" : "FALSE" );
		#endif

		if (uiDay <= pNPCProfile->ubLastDateSpokenTo)
		{
			// too early!
			return( FALSE );
		}
	}
	else if (uiDay < pNPCQuoteInfo->ubFirstDay)
	{
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
		NpcRecordLogging( ubApproach, "Current Day(%d) is before Required first day(%d) = %s", uiDay, pNPCQuoteInfo->ubFirstDay, (uiDay < pNPCQuoteInfo->ubFirstDay) ? "False, returning" : "True" );
		#endif
		// too early!
		return( FALSE );
	}

	if (uiDay > pNPCQuoteInfo->ubLastDay && uiDay < 255 )
	{
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
		NpcRecordLogging( ubApproach, "Current Day(%d) is after Required first day(%d) = %s", uiDay, pNPCQuoteInfo->ubFirstDay, (uiDay > pNPCQuoteInfo->ubLastDay) ? "TRUE, returning" : "FALSE" );
		#endif

		// too late!
		return( FALSE );
	}

	// check opinion required
	if ((pNPCQuoteInfo->ubOpinionRequired != IRRELEVANT) && (ubApproach != TRIGGER_NPC))
	{
		#ifdef JA2TESTVERSION
			//Add entry to the quest debug file
		NpcRecordLogging( ubApproach, "Opinion Required.  Talk Desire (%d), Opinion Required(%d) : %s", ubTalkDesire, pNPCQuoteInfo->ubOpinionRequired, (ubTalkDesire < pNPCQuoteInfo->ubOpinionRequired) ? "False, return" : "False, continue" );
		#endif

		if (ubTalkDesire < pNPCQuoteInfo->ubOpinionRequired )
		{
			return( FALSE );
		}
	}



	#ifdef JA2TESTVERSION
		//Add entry to the quest debug file
	NpcRecordLogging( ubApproach, "Return the quote opinion value! = TRUE");
	#endif

	// Return the quote opinion value!
	return( TRUE );

}


void ReplaceLocationInNPCData( NPCQuoteInfo * pNPCQuoteInfoArray, INT32 sOldGridNo, INT32 sNewGridNo )
{
	UINT8							ubFirstQuoteRecord, ubLastQuoteRecord, ubLoop;
	NPCQuoteInfo *		pNPCQuoteInfo;

	ubFirstQuoteRecord = 0;
	ubLastQuoteRecord = NUM_NPC_QUOTE_RECORDS - 1;
	for (ubLoop = ubFirstQuoteRecord; ubLoop <= ubLastQuoteRecord; ubLoop++)
	{
		pNPCQuoteInfo = &(pNPCQuoteInfoArray[ ubLoop ]);
		if(sOldGridNo == pNPCQuoteInfo->sRequiredGridNo)//dnl ch46 031009
			pNPCQuoteInfo->sRequiredGridNo = sNewGridNo;
		if (sOldGridNo == pNPCQuoteInfo->usGoToGridNo)
		{
			pNPCQuoteInfo->usGoToGridNo = sNewGridNo;
		}
	}
}

void ReplaceLocationInNPCDataFromProfileID( UINT8 ubNPC, INT32 sOldGridNo, INT32 sNewGridNo )
{
	NPCQuoteInfo *				pNPCQuoteInfoArray;

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return;
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	ReplaceLocationInNPCData( pNPCQuoteInfoArray, sOldGridNo, sNewGridNo );

}


void ResetOncePerConvoRecords( NPCQuoteInfo * pNPCQuoteInfoArray )
{
	UINT8 ubLoop;

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		if ( CHECK_FLAG(pNPCQuoteInfoArray[ubLoop].fFlags, QUOTE_FLAG_SAY_ONCE_PER_CONVO) )
		{
			TURN_FLAG_OFF( pNPCQuoteInfoArray[ubLoop].fFlags, QUOTE_FLAG_SAID );
		}
	}
}

void ResetOncePerConvoRecordsForNPC( UINT8 ubNPC )
{
	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return;
	}
	ResetOncePerConvoRecords( gpNPCQuoteInfoArray[ ubNPC ] );
	DebugQuestInfo(String("reset QUOTE_FLAG_SAID for profile %d", ubNPC));
}

void ResetOncePerConvoRecordsForAllNPCsInLoadedSector( void )
{	
	if ( gWorldSectorX == 0 || gWorldSectorY == 0 )
	{
		return;
	}

	for ( UINT8 IDnpc = 0; IDnpc < NUM_PROFILES; ++IDnpc )
	{	
		if ( gMercProfiles[IDnpc].Type == PROFILETYPE_RPC ||
			gMercProfiles[IDnpc].Type == PROFILETYPE_NPC )
		{	
			if ( gMercProfiles[IDnpc].sSectorX == gWorldSectorX &&
				 gMercProfiles[IDnpc].sSectorY == gWorldSectorY &&
				 gMercProfiles[IDnpc].bSectorZ == gbWorldSectorZ &&
				 gpNPCQuoteInfoArray[IDnpc] != NULL )
			{
				ResetOncePerConvoRecordsForNPC( IDnpc );
			}
		}	
	}
}


void ReturnItemToPlayerIfNecessary( UINT8 ubMerc, INT8 bApproach, UINT32 uiApproachData, NPCQuoteInfo * pQuotePtr )
{
	OBJECTTYPE  *		pObj;
	SOLDIERTYPE *		pSoldier;

	// if the approach was changed, always return the item
	// otherwise check to see if the record in question specified refusal
	if ( bApproach != APPROACH_GIVINGITEM || (pQuotePtr == NULL ) || (pQuotePtr->sActionData == NPC_ACTION_DONT_ACCEPT_ITEM ) )
	{
		pObj = (OBJECTTYPE *) uiApproachData;

		// Find the merc
		pSoldier = FindSoldierByProfileID( ubMerc, FALSE );

		// Try to auto place object and then if it fails, put into cursor
		if ( !AutoPlaceObject( pSoldier, pObj, FALSE ) )
		{
			InternalBeginItemPointer( pSoldier, pObj, NO_SLOT );
		}
		DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
	}
}

void Converse( UINT8 ubNPC, UINT8 ubMerc, INT8 bApproach, UINT32 uiApproachData )
{
	NPCQuoteInfo					QuoteInfo;
	NPCQuoteInfo *				pQuotePtr = &(QuoteInfo);
	NPCQuoteInfo *				pNPCQuoteInfoArray=NULL;
	MERCPROFILESTRUCT *		pProfile=NULL;
	UINT8									ubLoop, ubQuoteNum, ubRecordNum;
	SOLDIERTYPE *					pSoldier=NULL;
	UINT32								uiDay;
	OBJECTTYPE *					pObj=NULL;
	SOLDIERTYPE *					pNPC;
	BOOLEAN								fAttemptingToGiveItem;

	DebugQuestInfo(String("Converse: merc <%d> to NPC <%d> approach %d data %d", ubMerc, ubNPC, bApproach, uiApproachData));

	// we have to record whether an item is being given in order to determine whether,
	// in the case where the approach is overridden, we need to return the item to the
	// player
	fAttemptingToGiveItem = (bApproach == APPROACH_GIVINGITEM);

	pNPC = FindSoldierByProfileID( ubNPC, FALSE );
	if ( pNPC )
	{
		// set delay for civ AI movement
		pNPC->uiTimeSinceLastSpoke = GetJA2Clock();

		if ( CheckFact( FACT_CURRENT_SECTOR_IS_SAFE, ubNPC ) == FALSE )
		{
			if ( bApproach != TRIGGER_NPC && bApproach != APPROACH_GIVEFIRSTAID && bApproach != APPROACH_DECLARATION_OF_HOSTILITY && bApproach != APPROACH_ENEMY_NPC_QUOTE )
			{
				if ( NPCHasUnusedRecordWithGivenApproach( ubNPC, APPROACH_SECTOR_NOT_SAFE ) )
				{
					// override with sector-not-safe approach
					bApproach = APPROACH_SECTOR_NOT_SAFE;
				}
			}
		}

		// make sure civ is awake now
		pNPC->aiData.fAIFlags &= (~AI_ASLEEP);
	}

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!

		if ( fAttemptingToGiveItem )
		{
			ReturnItemToPlayerIfNecessary( ubMerc, bApproach, uiApproachData, NULL );
		}
		return;
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];
	
	//Legion jazz
	if (zHiddenNames[ubNPC].Hidden == TRUE) 
	{
		zHiddenNames[ubNPC].Hidden = FALSE;
		ShowNPCEncyclopediaEntry(ubNPC, TRUE);
	}


	pProfile = &(gMercProfiles[ubNPC]);
	switch( bApproach )
	{
		case NPC_INITIAL_QUOTE:
			// reset stuff
			DebugQuestInfo(String("ResetOncePerConvoRecords for <%d>", ubNPC));
			ResetOncePerConvoRecords( pNPCQuoteInfoArray );

			// CHEAP HACK
			// Since we don't have CONDITIONAL once-per-convo refreshes, do this in code
			// NB fact 281 is 'Darren has explained boxing rules'
			if ( ubNPC == DARREN && CheckFact( 281, DARREN ) == FALSE )
			{
				TURN_FLAG_OFF( pNPCQuoteInfoArray[11].fFlags, QUOTE_FLAG_SAID );
				DebugQuestInfo(String("reset QUOTE_FLAG_SAID for <%d> 'DARREN'", ubNPC));
			}

			// turn the NPC to face us
			// this '1' value is a dummy....
			NPCDoAction( ubNPC, NPC_ACTION_TURN_TO_FACE_NEAREST_MERC, 1 );

			if (pProfile->ubLastDateSpokenTo > 0)
			{
				uiDay = GetWorldDay();
				if (uiDay > pProfile->ubLastDateSpokenTo)
				{
					NPCConsiderTalking( ubNPC, ubMerc, APPROACH_SPECIAL_INITIAL_QUOTE, 0, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
					if (pQuotePtr != NULL)
					{
						// converse using this approach instead!
						if ( fAttemptingToGiveItem )
						{
							ReturnItemToPlayerIfNecessary( ubMerc, bApproach, uiApproachData, NULL );
						}
						Converse( ubNPC, ubMerc, APPROACH_SPECIAL_INITIAL_QUOTE, 0 );
						return;
					}
					// subsequent times approached intro
					ubQuoteNum = QUOTE_SUBS_INTRO;
				}
				else
				{
					// say nothing!
					if ( fAttemptingToGiveItem )
					{
						ReturnItemToPlayerIfNecessary( ubMerc, bApproach, uiApproachData, NULL );
					}
					return;
				}
			}
			else
			{
				// try special initial quote first
				NPCConsiderTalking( ubNPC, ubMerc, APPROACH_SPECIAL_INITIAL_QUOTE, 0, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
				if (pQuotePtr != NULL)
				{
					// converse using this approach instead!
					if ( fAttemptingToGiveItem )
					{
						ReturnItemToPlayerIfNecessary( ubMerc, bApproach, uiApproachData, NULL );
					}
					Converse( ubNPC, ubMerc, APPROACH_SPECIAL_INITIAL_QUOTE, 0 );
					return;
				}

				NPCConsiderTalking( ubNPC, ubMerc, APPROACH_INITIAL_QUOTE, 0, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
				if (pQuotePtr != NULL)
				{
					// converse using this approach instead!
					if ( fAttemptingToGiveItem )
					{
						ReturnItemToPlayerIfNecessary( ubMerc, bApproach, uiApproachData, NULL );
					}
					Converse( ubNPC, ubMerc, APPROACH_INITIAL_QUOTE, 0 );
					return;
				}

				// first time approached intro
				ubQuoteNum = QUOTE_INTRO;
			}
			TalkingMenuDialogue( ubQuoteNum );
			pProfile->ubLastQuoteSaid = ubQuoteNum;
			pProfile->bLastQuoteSaidWasSpecial = FALSE;
			break;
		case NPC_WHOAREYOU:
			ubQuoteNum = QUOTE_INTRO;
			TalkingMenuDialogue( ubQuoteNum );
			// For now, DO NOT remember for 'Come again?'
			break;
		case APPROACH_REPEAT:
			if (pProfile->ubLastQuoteSaid == NO_QUOTE)
			{
				// this should never occur now!
				TalkingMenuDialogue( QUOTE_INTRO );
			}
			else
			{
				if (pProfile->bLastQuoteSaidWasSpecial)
				{
					pQuotePtr = &(pNPCQuoteInfoArray[pProfile->ubLastQuoteSaid]);
					// say quote and following consecutive quotes
					for (ubLoop = 0; ubLoop < pQuotePtr->ubNumQuotes; ubLoop++)
					{
						// say quote #(pQuotePtr->ubQuoteNum + ubLoop)
						TalkingMenuDialogue( (UINT8)( pQuotePtr->ubQuoteNum + ubLoop ) );
					}
				}
				else
				{
					TalkingMenuDialogue( pProfile->ubLastQuoteSaid );
				}
			}
			break;
		default:
			switch( bApproach )
			{
				case APPROACH_GIVINGITEM:
					// first start by triggering any introduction quote if there is one...
					if ( pProfile->ubLastDateSpokenTo > 0)
					{
						uiDay = GetWorldDay();
						if (uiDay > pProfile->ubLastDateSpokenTo)
						{
							NPCConsiderTalking( ubNPC, ubMerc, APPROACH_SPECIAL_INITIAL_QUOTE, 0, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
							if (pQuotePtr != NULL)
							{
								// converse using this approach instead!
								Converse( ubNPC, ubMerc, APPROACH_SPECIAL_INITIAL_QUOTE, 0 );

								if ( ubNPC == DARREN )
								{
									// then we have to make this give attempt fail
									ReturnItemToPlayerIfNecessary( ubMerc, bApproach, uiApproachData, NULL );
									return;
								}
							}
						}
					}
					else
					{
						NPCConsiderTalking( ubNPC, ubMerc, APPROACH_INITIAL_QUOTE, 0, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
						if (pQuotePtr != NULL)
						{
							// converse using this approach instead!
							Converse( ubNPC, ubMerc, APPROACH_INITIAL_QUOTE, 0 );
						}
					}

					// If we are approaching because we want to give an item, do something different
					pObj = (OBJECTTYPE *) uiApproachData;
					NPCConsiderReceivingItemFromMerc( ubNPC, ubMerc, pObj, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
					break;
				case TRIGGER_NPC:
					// if triggering, pass in the approach data as the record to consider
					DebugMsg( TOPIC_JA2, DBG_LEVEL_0, String( "Handling trigger %S/%d at %lu", gMercProfiles[ ubNPC ].zNickname, (UINT8)uiApproachData, GetJA2Clock() ) );
					NPCConsiderTalking( ubNPC, ubMerc, bApproach, (UINT8)uiApproachData, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
					break;
				default:

					// Flugente: additional dialogue
					if ( bApproach == APPROACH_FRIENDLY )		AdditionalTacticalCharacterDialogue_CallsLua( FindSoldierByProfileID( ubMerc, FALSE ), ADE_DIALOGUE_NPC_FRIENDLY, ubNPC );
					else if ( bApproach == APPROACH_DIRECT )	AdditionalTacticalCharacterDialogue_CallsLua( FindSoldierByProfileID( ubMerc, FALSE ), ADE_DIALOGUE_NPC_DIRECT, ubNPC );
					else if ( bApproach == APPROACH_THREATEN )	AdditionalTacticalCharacterDialogue_CallsLua( FindSoldierByProfileID( ubMerc, FALSE ), ADE_DIALOGUE_NPC_THREATEN, ubNPC );
					else if ( bApproach == APPROACH_RECRUIT )	AdditionalTacticalCharacterDialogue_CallsLua( FindSoldierByProfileID( ubMerc, FALSE ), ADE_DIALOGUE_NPC_RECRUIT, ubNPC );
					else if ( bApproach == APPROACH_REPEAT )	AdditionalTacticalCharacterDialogue_CallsLua( FindSoldierByProfileID( ubMerc, FALSE ), ADE_DIALOGUE_NPC_REPEAT, ubNPC );

					NPCConsiderTalking( ubNPC, ubMerc, bApproach, 0, pNPCQuoteInfoArray, &pQuotePtr, &ubRecordNum );
					break;
			}
			if (pQuotePtr == NULL)
			{
				// say random everyday quote
				// do NOT set last quote said!
				switch( bApproach )
				{
					case APPROACH_FRIENDLY:
						if (pProfile->bFriendlyOrDirectDefaultResponseUsedRecently)
						{
							ubQuoteNum = QUOTE_GETLOST;
						}
						else
						{
							ubQuoteNum = QUOTE_FRIENDLY_DEFAULT1 + (UINT8) Random( 2 );
							pProfile->bFriendlyOrDirectDefaultResponseUsedRecently = TRUE;
						}
						break;
					case APPROACH_DIRECT:
						if (pProfile->bFriendlyOrDirectDefaultResponseUsedRecently)
						{
							ubQuoteNum = QUOTE_GETLOST;
						}
						else
						{
							ubQuoteNum = QUOTE_DIRECT_DEFAULT;
							pProfile->bFriendlyOrDirectDefaultResponseUsedRecently = TRUE;
						}
						break;
					case APPROACH_THREATEN:
						if (pProfile->bThreatenDefaultResponseUsedRecently)
						{
							ubQuoteNum = QUOTE_GETLOST;
						}
						else
						{
							ubQuoteNum = QUOTE_THREATEN_DEFAULT;
							pProfile->bThreatenDefaultResponseUsedRecently = TRUE;
						}
						break;
					case APPROACH_RECRUIT:
						if (pProfile->bRecruitDefaultResponseUsedRecently)
						{
							ubQuoteNum = QUOTE_GETLOST;
						}
						else
						{
							ubQuoteNum = QUOTE_RECRUIT_NO;
							pProfile->bRecruitDefaultResponseUsedRecently = TRUE;
						}
						break;
					case APPROACH_GIVINGITEM:
						ubQuoteNum = QUOTE_GIVEITEM_NO;

						/*
						CC - now handled below
						*/
						break;
					case TRIGGER_NPC:
						// trigger did not succeed - abort!!
						return;
					default:
						ubQuoteNum = QUOTE_INTRO;
						break;
				}
				TalkingMenuDialogue( ubQuoteNum );
				pProfile->ubLastQuoteSaid = ubQuoteNum;
				pProfile->bLastQuoteSaidWasSpecial = FALSE;
				if (ubQuoteNum == QUOTE_GETLOST)
				{
					if (ubNPC == 70 || ubNPC == 120)
					{
						// becomes an enemy
						NPCDoAction( ubNPC, NPC_ACTION_BECOME_ENEMY, 0 );
					}
					// close panel at end of speech
					NPCClosePanel();
				}
				else if ( ubQuoteNum == QUOTE_GIVEITEM_NO )
				{
					// close panel at end of speech
					NPCClosePanel();
					if ( pNPC )
					{
						switch( ubNPC )
						{
							case JIM:
							case JACK:
							case OLAF:
							case RAY:
							case OLGA:
							case TYRONE:
								// Start combat etc
								DebugAI(AI_MSG_INFO, pNPC, String("CancelAIAction: NPC coverse"));
								CancelAIAction( pNPC, TRUE );
								AddToShouldBecomeHostileOrSayQuoteList( pNPC->ubID );
							default:
								break;
						}
					}
				}
			}
			else
			{
#ifdef JA2BETAVERSION
				if ( gfDisplayScreenMsgOnRecordUsage )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Using record %d for %s", ubRecordNum, gMercProfiles[ ubNPC ].zNickname );
				}
#endif

				// turn before speech?
				if ( pQuotePtr->sActionData <= -NPC_ACTION_TURN_TO_FACE_NEAREST_MERC )
				{
					pSoldier = FindSoldierByProfileID( ubNPC, FALSE );
					ZEROTIMECOUNTER( pSoldier->timeCounters.AICounter );
					if (pSoldier->aiData.bNextAction == AI_ACTION_WAIT)
					{
						pSoldier->aiData.bNextAction = AI_ACTION_NONE;
						pSoldier->aiData.usNextActionData = 0;
					}
					NPCDoAction( ubNPC, (UINT16) -(pQuotePtr->sActionData), ubRecordNum );
				}
				if (pQuotePtr->ubQuoteNum != NO_QUOTE)
				{
					// say quote and following consecutive quotes
					for (ubLoop = 0; ubLoop < pQuotePtr->ubNumQuotes; ubLoop++)
					{
						TalkingMenuDialogue( (UINT8)( pQuotePtr->ubQuoteNum + ubLoop  ) );
					}
					pProfile->ubLastQuoteSaid = ubRecordNum;
					pProfile->bLastQuoteSaidWasSpecial = TRUE;
				}
				// set to "said" if we should do so
				if (pQuotePtr->fFlags & QUOTE_FLAG_ERASE_ONCE_SAID || pQuotePtr->fFlags & QUOTE_FLAG_SAY_ONCE_PER_CONVO)
				{
					TURN_FLAG_ON( pQuotePtr->fFlags, QUOTE_FLAG_SAID );
				}

				// Carry out implications (actions) of this record

				// Give NPC item if appropriate
				if (bApproach == APPROACH_GIVINGITEM )
				{
					if ( pQuotePtr->sActionData != NPC_ACTION_DONT_ACCEPT_ITEM )
					{
						PlaceObjectInSoldierProfile( ubNPC, pObj );

						// Find the GIVER....
						pSoldier = FindSoldierByProfileID( ubMerc, FALSE );

						// Is this one of us?
						if ( pSoldier->bTeam == gbPlayerNum )
						{
							for (int x = INV_START_POS; x < NUM_INV_SLOTS; ++x) {
								if (&(pSoldier->inv[x]) == pObj) {
									DeleteObj(&pSoldier->inv[x]);
									DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
									break;
								}
							}
						}
						else
						{
							RemoveObjectFromSoldierProfile( ubMerc, pObj->usItem );
						}
					}
					// CC: now handled below
					/*
					else
					{
						// ATE: Here, put back into inventory or place on ground....
						{
							// Find the merc
							pSoldier = FindSoldierByProfileID( ubMerc, FALSE );

							// Try to auto place object and then if it fails, put into cursor
							if ( !AutoPlaceObject( pSoldier, pObj, FALSE ) )
							{
								InternalBeginItemPointer( pSoldier, pObj, NO_SLOT );
							}
							DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );

						}
					}
					*/
				}
				else if (bApproach == APPROACH_RECRUIT)
				{
					// the guy just joined our party
				}

				// Set things
				if (pQuotePtr->usSetFactTrue != NO_FACT)
				{
					SetFactTrue( pQuotePtr->usSetFactTrue );
				}
				if (pQuotePtr->ubEndQuest != NO_QUEST)
				{
					EndQuest( pQuotePtr->ubEndQuest, gWorldSectorX, gWorldSectorY );
				}
				if (pQuotePtr->ubStartQuest != NO_QUEST)
				{
					StartQuest( pQuotePtr->ubStartQuest, gWorldSectorX, gWorldSectorY );
				}

				// Give item to merc?
				if ( pQuotePtr->usGiftItem >= TURN_UI_OFF )
				{
					switch ( pQuotePtr->usGiftItem )
					{
						case TURN_UI_OFF:
							if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
							{
								gTacticalStatus.uiFlags |= ENGAGED_IN_CONV;
								// Increment refrence count...
								giNPCReferenceCount = 1;
							}
							break;
						case TURN_UI_ON:
							// while the special ref count is set, ignore standard off
							if ( giNPCSpecialReferenceCount == 0 )
							{
								gTacticalStatus.uiFlags &= ~ENGAGED_IN_CONV;
								// Decrement refrence count...
								giNPCReferenceCount = 0;
							}
							break;
						case SPECIAL_TURN_UI_OFF:
							if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
							{

								gTacticalStatus.uiFlags |= ENGAGED_IN_CONV;
								// Increment refrence count...
								giNPCReferenceCount = 1;
								if ( giNPCSpecialReferenceCount < 0 )
								{
									// ???
									giNPCSpecialReferenceCount = 0;
								}
								// increment SPECIAL reference count
								giNPCSpecialReferenceCount += 1;
							}
							break;
						case SPECIAL_TURN_UI_ON:
							// Decrement SPECIAL reference count
							giNPCSpecialReferenceCount -= 1;
							// if special count is now 0, turn reactivate UI
							if ( giNPCSpecialReferenceCount == 0 )
							{
								gTacticalStatus.uiFlags &= ~ENGAGED_IN_CONV;
								giNPCReferenceCount = 0;
							}
							else if ( giNPCSpecialReferenceCount < 0 )
							{
								// ???
								giNPCSpecialReferenceCount = 0;
							}
							break;
					}
				}
				else if ( pQuotePtr->usGiftItem != 0 )
				{
					{
						INT8 bInvPos;

						// Get soldier
						pSoldier = FindSoldierByProfileID( ubNPC, FALSE );

						// Look for item....
						bInvPos = FindObj( pSoldier, pQuotePtr->usGiftItem );

						AssertMsg( bInvPos != NO_SLOT, "NPC.C:  Gift item does not exist in NPC." );

						TalkingMenuGiveItem( ubNPC, &(pSoldier->inv[ bInvPos ] ), bInvPos );
					}
				}
				// Action before movement?
				if ( pQuotePtr->sActionData < 0 && pQuotePtr->sActionData > -NPC_ACTION_TURN_TO_FACE_NEAREST_MERC )
				{
					pSoldier = FindSoldierByProfileID( ubNPC, FALSE );
					ZEROTIMECOUNTER( pSoldier->timeCounters.AICounter );
					if (pSoldier->aiData.bNextAction == AI_ACTION_WAIT)
					{
						pSoldier->aiData.bNextAction = AI_ACTION_NONE;
						pSoldier->aiData.usNextActionData = 0;
					}
					NPCDoAction( ubNPC, (UINT16) -(pQuotePtr->sActionData), ubRecordNum );
				}
				else if ( pQuotePtr->usGoToGridNo == NO_MOVE && pQuotePtr->sActionData > 0 )
				{
					pSoldier = FindSoldierByProfileID( ubNPC, FALSE );
					ZEROTIMECOUNTER( pSoldier->timeCounters.AICounter );
					if (pSoldier->aiData.bNextAction == AI_ACTION_WAIT)
					{
						pSoldier->aiData.bNextAction = AI_ACTION_NONE;
						pSoldier->aiData.usNextActionData = 0;
					}
					NPCDoAction( ubNPC, (UINT16) (pQuotePtr->sActionData), ubRecordNum );
				}

				// Movement?
				if ( pQuotePtr->usGoToGridNo != NO_MOVE )
				{
					pSoldier = FindSoldierByProfileID( ubNPC, FALSE );

					
                    if (pSoldier)
                    {
                        // stupid hack CC
					    if (ubNPC == KYLE)
					    {
						    // make sure he has keys
						    pSoldier->flags.bHasKeys = TRUE;
					    }

					    if (pSoldier->sGridNo == pQuotePtr->usGoToGridNo )
					    {
						    // search for quotes to trigger immediately!
						    pSoldier->ubQuoteRecord = ubRecordNum + 1; // add 1 so that the value is guaranteed nonzero
						    NPCReachedDestination( pSoldier, TRUE );
					    }
					    else
					    {
						    // turn off cowering
						    if ( pNPC->flags.uiStatusFlags & SOLDIER_COWERING) // FIXME: Dereferencing null pointer
						    {
							    //pNPC->flags.uiStatusFlags &= ~SOLDIER_COWERING;
							    pNPC->EVENT_InitNewSoldierAnim( STANDING, 0 , FALSE );
						    }

						    pSoldier->ubQuoteRecord = ubRecordNum + 1; // add 1 so that the value is guaranteed nonzero

						    if (pQuotePtr->sActionData == NPC_ACTION_TELEPORT_NPC)
						    {
							    BumpAnyExistingMerc( pQuotePtr->usGoToGridNo );
							    TeleportSoldier( pSoldier, pQuotePtr->usGoToGridNo, FALSE );
							    // search for quotes to trigger immediately!
							    NPCReachedDestination( pSoldier, FALSE );
						    }
						    else
						    {
							    NPCGotoGridNo( ubNPC, pQuotePtr->usGoToGridNo, ubRecordNum );
						    }
					    }
                    }
				}

				// Trigger other NPC?
				// ATE: Do all triggers last!
				if ( pQuotePtr->ubTriggerNPC != IRRELEVANT )
				{
					// Check for special NPC trigger codes
					if ( pQuotePtr->ubTriggerNPC == 0 )
					{
						TriggerClosestMercWhoCanSeeNPC( ubNPC, pQuotePtr );
					}
					else if ( pQuotePtr->ubTriggerNPC == 1 )
					{
						// trigger self
						TriggerNPCRecord( ubNPC, pQuotePtr->ubTriggerNPCRec );
					}
					else
					{
						TriggerNPCRecord( pQuotePtr->ubTriggerNPC, pQuotePtr->ubTriggerNPCRec );
					}
				}

				// Ian says it is okay to take this out!
				/*
				if (bApproach == APPROACH_ENEMY_NPC_QUOTE)
				{
					NPCClosePanel();
				}
				*/

			}
			break;
	}

	// Set last day spoken!
	switch( bApproach )
	{
		case APPROACH_FRIENDLY:
		case APPROACH_DIRECT:
		case APPROACH_THREATEN:
		case APPROACH_RECRUIT:
		case NPC_INITIATING_CONV:
		case NPC_INITIAL_QUOTE:
		case APPROACH_SPECIAL_INITIAL_QUOTE:
		case APPROACH_DECLARATION_OF_HOSTILITY:
		case APPROACH_INITIAL_QUOTE:
		case APPROACH_GIVINGITEM:
			// SANDRO - new records - NPCs discovered
			if ( pProfile->ubLastDateSpokenTo == 0 && FindSoldierByProfileID( ubMerc, TRUE ) != NULL )
			{
				gMercProfiles[ubMerc].records.usNPCsDiscovered++;
			}
			pProfile->ubLastDateSpokenTo = (UINT8) GetWorldDay();
			break;
		default:
			break;
	}

	// return item?
	if ( fAttemptingToGiveItem )
	{
		ReturnItemToPlayerIfNecessary( ubMerc, bApproach, uiApproachData, pQuotePtr );
	}
}

INT32 NPCConsiderInitiatingConv( SOLDIERTYPE * pNPC, UINT8 * pubDesiredMerc )
{
	INT32						sMyGridNo, sDist, sDesiredMercDist = 100;
	UINT8						ubNPC, ubMerc, ubDesiredMerc = NOBODY;
	UINT8						ubTalkDesire, ubHighestTalkDesire = 0;
	SOLDIERTYPE *		pMerc;
	SOLDIERTYPE *		pDesiredMerc = NULL;
	NPCQuoteInfo *	pNPCQuoteInfoArray;

	CHECKF( pubDesiredMerc );
	sMyGridNo = pNPC->sGridNo;

	ubNPC = pNPC->ubProfile;
	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( NOWHERE );
	}
	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	// loop through all mercs
	for ( ubMerc = 0; ubMerc < guiNumMercSlots; ubMerc++ )
	{
		pMerc = MercSlots[ ubMerc ];
		if (pMerc != NULL)
		{
			// only look for mercs on the side of the player
			if (pMerc->bSide != gbPlayerNum)
			{
				continue;
			}

			// only look for active mercs
			if (pMerc->bAssignment >= ON_DUTY )
			{
				continue;
			}

			// if they're not visible, don't think about it
			if (pNPC->aiData.bOppList[ubMerc] != SEEN_CURRENTLY)
			{
				continue;
			}

			// what's the opinion required for the highest-opinion quote that we would
			// say to this merc
			ubTalkDesire = NPCConsiderTalking( pNPC->ubProfile, pMerc->ubProfile, NPC_INITIATING_CONV, 0, pNPCQuoteInfoArray, NULL, NULL );
			if (ubTalkDesire > 0)
			{
				if (ubTalkDesire > ubHighestTalkDesire)
				{
					ubHighestTalkDesire = ubTalkDesire;
					ubDesiredMerc = ubMerc;
					pDesiredMerc = MercSlots[ubMerc];
					sDesiredMercDist = PythSpacesAway( sMyGridNo, pDesiredMerc->sGridNo );
				}
				else if (ubTalkDesire == ubHighestTalkDesire)
				{
					sDist = PythSpacesAway( sMyGridNo, MercSlots[ubMerc]->sGridNo );
					if (sDist < sDesiredMercDist)
					{
						// we can say the same thing to this merc, and they're closer!
						ubDesiredMerc = ubMerc;
						pDesiredMerc = MercSlots[ubMerc];
						sDesiredMercDist = sDist;
					}
				}
			}
		}
	}

	if (ubDesiredMerc == NOBODY)
	{
		return( NOWHERE );
	}
	else
	{
		*pubDesiredMerc = ubDesiredMerc;
		return ( pDesiredMerc->sGridNo );
	}
}

UINT8 NPCTryToInitiateConv( SOLDIERTYPE * pNPC )
{ // assumes current action is ACTION_APPROACH_MERC
	if (pNPC->aiData.bAction != AI_ACTION_APPROACH_MERC)
	{
		return( AI_ACTION_NONE );
	}
	if (PythSpacesAway( pNPC->sGridNo, MercPtrs[pNPC->aiData.usActionData]->sGridNo ) < CONVO_DIST)
	{
		// initiate conversation!
		Converse( pNPC->ubProfile, MercPtrs[pNPC->aiData.usActionData]->ubProfile, NPC_INITIATING_CONV, 0 );
		// after talking, wait a while before moving anywhere else
		return( AI_ACTION_WAIT );
	}
	else
	{
		// try to move towards that merc
		return( AI_ACTION_APPROACH_MERC );
	}
}


/*
BOOLEAN NPCOkToGiveItem( UINT8 ubNPC, UINT8 ubMerc, UINT16 usItem )
{
	// This function seems to be unused...

	NPCQuoteInfo					QuoteInfo;
	NPCQuoteInfo *				pQuotePtr = &(QuoteInfo);
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	UINT8									ubOpinionVal;
	UINT8									ubQuoteNum;

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}
	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	ubOpinionVal =  NPCConsiderReceivingItemFromMerc( ubNPC, ubMerc, usItem, pNPCQuoteInfoArray, &pQuotePtr, &ubQuoteNum );

	if ( ubOpinionVal )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}
*/
void NPCReachedDestination( SOLDIERTYPE * pNPC, BOOLEAN fAlreadyThere )
{
	// perform action or whatever after reaching our destination
	UINT8		ubNPC;
	NPCQuoteInfo *				pQuotePtr;
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	UINT8 ubLoop;
	UINT8									ubQuoteRecord;

	if ( pNPC->ubQuoteRecord == 0 )
	{
		ubQuoteRecord = 0;
	}
	else
	{
		ubQuoteRecord = (UINT8) (pNPC->ubQuoteRecord - 1);
	}

	// Clear values!
	pNPC->ubQuoteRecord = 0;
	if (pNPC->bTeam == gbPlayerNum)
	{
		// the "under ai control" flag was set temporarily; better turn it off now
		pNPC->flags.uiStatusFlags &= (~SOLDIER_PCUNDERAICONTROL);
		// make damn sure the AI_HANDLE_EVERY_FRAME flag is turned off
		pNPC->aiData.fAIFlags &= (AI_HANDLE_EVERY_FRAME);
	}

	ubNPC = pNPC->ubProfile;
	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return;
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];
	pQuotePtr = &(pNPCQuoteInfoArray[ubQuoteRecord]);
	// either we are supposed to consider a new quote record
	// (indicated by a negative gridno in the has-item field)
	// or an action to perform once we reached this gridno

	if ( pNPC->sGridNo == pQuotePtr->usGoToGridNo )
	{
		// check for an after-move action
		if ( pQuotePtr->sActionData > 0)
		{
			NPCDoAction( ubNPC, (UINT16) pQuotePtr->sActionData, ubQuoteRecord );
		}
	}

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( pNPC->sGridNo == pQuotePtr->sRequiredGridNo )//dnl ch46 031009
		{
			if ( NPCConsiderQuote( ubNPC, 0, TRIGGER_NPC, ubLoop, 0, pNPCQuoteInfoArray ) )
			{
				if (fAlreadyThere)
				{
					TriggerNPCRecord( ubNPC, ubLoop );
				}
				else
				{
					// trigger this quote
					TriggerNPCRecordImmediately( ubNPC, ubLoop );
				}
				return;
			}
		}
	}
}

void TriggerNPCRecord( UINT8 ubTriggerNPC, UINT8 ubTriggerNPCRec )
{
	// Check if we have a quote to trigger...
	NPCQuoteInfo *pQuotePtr;
	BOOLEAN      fDisplayDialogue = TRUE;

	DebugQuestInfo(String("TriggerNPCRecord: <%d> record %d", ubTriggerNPC, ubTriggerNPCRec));

	if (EnsureQuoteFileLoaded( ubTriggerNPC ) == FALSE)
	{
		// error!!!
		return;
	}
	pQuotePtr = &(gpNPCQuoteInfoArray[ ubTriggerNPC ][ ubTriggerNPCRec ]);
	if ( pQuotePtr->ubQuoteNum == IRRELEVANT )
	{
		fDisplayDialogue = FALSE;
	}

	if ( NPCConsiderQuote( ubTriggerNPC, 0, TRIGGER_NPC, ubTriggerNPCRec, 0, gpNPCQuoteInfoArray[ ubTriggerNPC ] ) )
	{
		NPCTriggerNPC( ubTriggerNPC, ubTriggerNPCRec, TRIGGER_NPC, fDisplayDialogue );
	}
	else
	{
		// don't do anything
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "WARNING: trigger of %d, record %d cannot proceed, possible error", ubTriggerNPC, ubTriggerNPCRec ) );
	}
}

void TriggerNPCRecordImmediately( UINT8 ubTriggerNPC, UINT8 ubTriggerNPCRec )
{
	// Check if we have a quote to trigger...
	NPCQuoteInfo *pQuotePtr;
	BOOLEAN      fDisplayDialogue = TRUE;

	DebugQuestInfo(String("TriggerNPCRecordImmediately: <%d> record %d", ubTriggerNPC, ubTriggerNPCRec));

	if (EnsureQuoteFileLoaded( ubTriggerNPC ) == FALSE)
	{
		// error!!!
		return;
	}
	pQuotePtr = &(gpNPCQuoteInfoArray[ ubTriggerNPC ][ ubTriggerNPCRec ]);
	if ( pQuotePtr->ubQuoteNum == IRRELEVANT )
	{
		fDisplayDialogue = FALSE;
	}


	if ( NPCConsiderQuote( ubTriggerNPC, 0, TRIGGER_NPC, ubTriggerNPCRec, 0, gpNPCQuoteInfoArray[ ubTriggerNPC ] ) )
	{
		// trigger IMMEDIATELY
		HandleNPCTriggerNPC( ubTriggerNPC, ubTriggerNPCRec, fDisplayDialogue, TRIGGER_NPC );
	}
	else
	{
		// don't do anything
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "WARNING: trigger of %d, record %d cannot proceed, possible error", ubTriggerNPC, ubTriggerNPCRec ) );
	}
}


void PCsNearNPC( UINT8 ubNPC )
{
	UINT8 ubLoop;
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	SOLDIERTYPE *pSoldier;
	NPCQuoteInfo *				pQuotePtr;


	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return;
	}
	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	// see what this triggers...
	SetFactTrue( FACT_PC_NEAR );

	// Clear values!
	// Get value for NPC
	pSoldier = FindSoldierByProfileID( ubNPC, FALSE );
	pSoldier->ubQuoteRecord = 0;

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( pSoldier->sGridNo == pQuotePtr->sRequiredGridNo )//dnl ch46 031009
		{
			if ( NPCConsiderQuote( ubNPC, 0, TRIGGER_NPC, ubLoop, 0, pNPCQuoteInfoArray ) )
			{
				// trigger this quote IMMEDIATELY!
				TriggerNPCRecordImmediately( ubNPC, ubLoop );
				break;
			}
		}
	}

	// reset fact
	SetFactFalse( FACT_PC_NEAR );
}

BOOLEAN PCDoesFirstAidOnNPC( UINT8 ubNPC )
{
	UINT8 ubLoop;
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	SOLDIERTYPE *pSoldier;
	NPCQuoteInfo *				pQuotePtr;

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}
	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	// Get ptr to NPC
	pSoldier = FindSoldierByProfileID( ubNPC, FALSE );
	// Clear values!
	pSoldier->ubQuoteRecord = 0;

	// Set flag...
	gMercProfiles[ ubNPC ].ubMiscFlags2 |= PROFILE_MISC_FLAG2_BANDAGED_TODAY;

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( pQuotePtr->ubApproachRequired == APPROACH_GIVEFIRSTAID )
		{
			if ( NPCConsiderQuote( ubNPC, 0, TRIGGER_NPC, ubLoop, 0, pNPCQuoteInfoArray ) )
			{
				// trigger this quote IMMEDIATELY!
				TriggerNPCRecordImmediately( ubNPC, ubLoop );
				return( TRUE );
			}
		}
	}
	return( FALSE );
}

void TriggerClosestMercWhoCanSeeNPC( UINT8 ubNPC, NPCQuoteInfo *pQuotePtr )
{
	// Loop through all mercs, gather closest mercs who can see and trigger one!
	UINT8	ubMercsInSector[ 40 ] = { 0 };
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	SOLDIERTYPE *pTeamSoldier, *pSoldier;
	INT32 cnt;

	// First get pointer to NPC
	pSoldier = FindSoldierByProfileID( ubNPC, FALSE );

	// Loop through all our guys and randomly say one from someone in our sector

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
	{
		// Add guy if he's a candidate...
		if ( OK_INSECTOR_MERC( pTeamSoldier ) && pTeamSoldier->aiData.bOppList[ pSoldier->ubID ] == SEEN_CURRENTLY )
		{
			ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
			ubNumMercs++;
		}
	}

	// If we are > 0
	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT8)Random( ubNumMercs );

		// Post action to close panel
		NPCClosePanel( );

		// If 64, do something special
		if ( pQuotePtr->ubTriggerNPCRec == QUOTE_RESPONSE_TO_MIGUEL_SLASH_QUOTE_MERC_OR_RPC_LETGO )
		{
			TacticalCharacterDialogueWithSpecialEvent( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], pQuotePtr->ubTriggerNPCRec, DIALOGUE_SPECIAL_EVENT_PCTRIGGERNPC, 57, 6 );
		}
		else
		{
			TacticalCharacterDialogue( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], pQuotePtr->ubTriggerNPCRec );
		}
	}

}

BOOLEAN TriggerNPCWithIHateYouQuote( UINT8 ubTriggerNPC )
{
	// Check if we have a quote to trigger...
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	NPCQuoteInfo	*pQuotePtr;
	BOOLEAN				fDisplayDialogue = TRUE;
	UINT8 ubLoop;

	if (EnsureQuoteFileLoaded( ubTriggerNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubTriggerNPC];

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( NPCConsiderQuote( ubTriggerNPC, 0, APPROACH_DECLARATION_OF_HOSTILITY, ubLoop, 0, pNPCQuoteInfoArray ) )
		{
			// trigger this quote!
			// reset approach required value so that we can trigger it
			//pQuotePtr->ubApproachRequired = TRIGGER_NPC;
			NPCTriggerNPC( ubTriggerNPC, ubLoop, APPROACH_DECLARATION_OF_HOSTILITY, TRUE );
			gMercProfiles[ ubTriggerNPC ].ubMiscFlags |= PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE;
			return( TRUE );
		}
	}
	return( FALSE );

}

BOOLEAN NPCHasUnusedRecordWithGivenApproach( UINT8 ubNPC, UINT8 ubApproach )
{
	// Check if we have a quote that could be used
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	NPCQuoteInfo	*pQuotePtr;
	UINT8 ubLoop;

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( NPCConsiderQuote( ubNPC, 0, ubApproach, ubLoop, 0, pNPCQuoteInfoArray ) )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN NPCHasUnusedHostileRecord( UINT8 ubNPC, UINT8 ubApproach )
{
	// this is just like the standard check BUT we must skip any
	// records using fact 289 and print debug msg for any records which can't be marked as used
	// Check if we have a quote that could be used
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	NPCQuoteInfo	*pQuotePtr;
	UINT8 ubLoop;

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( NPCConsiderQuote( ubNPC, 0, ubApproach, ubLoop, 0, pNPCQuoteInfoArray ) )
		{
			if ( pQuotePtr->usFactMustBeTrue == FACT_NPC_HOSTILE_OR_PISSED_OFF )
			{
				continue;
			}
			#ifdef JA2BETAVERSION
			if ( !(pQuotePtr->fFlags & QUOTE_FLAG_ERASE_ONCE_SAID)  )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Warning: possible infinite quote loop to follow." );
			}
			#endif
			return( TRUE );
		}
	}
	return( FALSE );
}


BOOLEAN NPCWillingToAcceptItem( UINT8 ubNPC, UINT8 ubMerc, OBJECTTYPE * pObj )
{
	// Check if we have a quote that could be used, that applies to this item
	NPCQuoteInfo *		pNPCQuoteInfoArray;
	NPCQuoteInfo *		pQuotePtr;
	UINT8							ubOpinion, ubQuoteNum;

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	ubOpinion = NPCConsiderReceivingItemFromMerc( ubNPC, ubMerc, pObj, pNPCQuoteInfoArray, &pQuotePtr, &ubQuoteNum );

	if ( pQuotePtr )
	{
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN GetInfoForAbandoningEPC( UINT8 ubNPC, UINT16 * pusQuoteNum, UINT16 * pusFactToSetTrue )
{
	// Check if we have a quote that could be used
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	NPCQuoteInfo	*pQuotePtr;
	UINT8 ubLoop;

	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubNPC];

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( NPCConsiderQuote( ubNPC, 0, APPROACH_EPC_IN_WRONG_SECTOR, ubLoop, 0, pNPCQuoteInfoArray ) )
		{
			*pusQuoteNum = pNPCQuoteInfoArray[ubLoop].ubQuoteNum;
			*pusFactToSetTrue = pNPCQuoteInfoArray[ubLoop].usSetFactTrue;
			return( TRUE );
		}
	}
	return( FALSE );
}

BOOLEAN TriggerNPCWithGivenApproach( UINT8 ubTriggerNPC, UINT8 ubApproach, BOOLEAN fShowPanel )
{
	// Check if we have a quote to trigger...
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	NPCQuoteInfo	*pQuotePtr;
	BOOLEAN				fDisplayDialogue = TRUE;
	UINT8 ubLoop;

	if (EnsureQuoteFileLoaded( ubTriggerNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ubTriggerNPC];

	for ( ubLoop = 0; ubLoop < NUM_NPC_QUOTE_RECORDS; ubLoop++ )
	{
		pQuotePtr = &(pNPCQuoteInfoArray[ubLoop]);
		if ( NPCConsiderQuote( ubTriggerNPC, 0, ubApproach, ubLoop, 0, pNPCQuoteInfoArray ) )
		{
			if ( pQuotePtr->ubQuoteNum == IRRELEVANT )
			{
				fShowPanel = FALSE;
			}
			else
			{
				fShowPanel = TRUE;
			}

			// trigger this quote!
			// reset approach required value so that we can trigger it
			//pQuotePtr->ubApproachRequired = TRIGGER_NPC;
			NPCTriggerNPC( ubTriggerNPC, ubLoop, ubApproach, fShowPanel );
			return( TRUE );
		}
	}
	return( FALSE );

}


BOOLEAN SaveNPCInfoToSaveGameFile( HWFILE hFile )
{
	UINT32		uiNumBytesWritten=0;
	UINT32		cnt;
	UINT8			ubOne = 1;
	UINT8			ubZero = 0;


	//Loop through all the NPC quotes
	for( cnt=0; cnt<NUM_PROFILES; cnt++)
	{
		//if there is a npc qutoe
		if( gpNPCQuoteInfoArray[ cnt ] )
		{
			//save a byte specify that there is an npc quote saved
			FileWrite( hFile, &ubOne, sizeof( UINT8 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT8 ) )
			{
				return( FALSE );
			}

			//Save the NPC quote entry
			FileWrite( hFile, gpNPCQuoteInfoArray[ cnt ], sizeof( NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS, &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
			{
				return( FALSE );
			}
		}
		else
		{
			//save a byte specify that there is an npc quote saved
			FileWrite( hFile, &ubZero, sizeof( UINT8 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT8 ) )
			{
				return( FALSE );
			}
		}
	}

	for( cnt = 0; cnt < NUM_CIVQUOTE_SECTORS; cnt++)
	{
		//if there is a civ quote
		if( gpCivQuoteInfoArray[ cnt ] )
		{
			//save a byte specify that there is an npc quote saved
			FileWrite( hFile, &ubOne, sizeof( UINT8 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT8 ) )
			{
				return( FALSE );
			}

			//Save the NPC quote entry
			FileWrite( hFile, gpCivQuoteInfoArray[ cnt ], sizeof( NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS, &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
			{
				return( FALSE );
			}
		}
		else
		{
			//save a byte specify that there is an npc quote saved
			FileWrite( hFile, &ubZero, sizeof( UINT8 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT8 ) )
			{
				return( FALSE );
			}
		}
	}


	return( TRUE );
}

BOOLEAN LoadNPCInfoFromSavedGameFile( HWFILE hFile, UINT32 uiSaveGameVersion )
{
	UINT32		uiNumBytesRead=0;
	UINT32		cnt;
	UINT8			ubLoadQuote=0;
	UINT32		uiNumberToLoad=0;
	INT32		numBytesRead = 0;
	INT8		padding;


	// If we are trying to restore a saved game prior to version 44, use the
	// MAX_NUM_SOLDIERS, else use NUM_PROFILES.  Dave used the wrong define!
	// Verion 111 increased NUM_PROFILES from 170 to 255
	if( uiSaveGameVersion >= 111 )
		uiNumberToLoad = NUM_PROFILES;
	else if( uiSaveGameVersion >= 44 )
		uiNumberToLoad = NUM_PROFILES_v111;
	else
		uiNumberToLoad = MAX_NUM_SOLDIERS;

	//Loop through all the NPC quotes
	for( cnt=0; cnt<uiNumberToLoad; cnt++ )
	{
		//Load a byte specify that there is an npc quote Loadd
		FileRead( hFile, &ubLoadQuote, sizeof( UINT8 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( UINT8 ) )
		{
			return( FALSE );
		}

		//if there is an existing quote
		if( gpNPCQuoteInfoArray[ cnt ] )
		{
			//delete it
			MemFree( gpNPCQuoteInfoArray[ cnt ] );
			gpNPCQuoteInfoArray[ cnt ] = NULL;
		}

		//if there is a npc quote
		if( ubLoadQuote )
		{
			//if there is no memory allocated
			if( gpNPCQuoteInfoArray[ cnt ] == NULL )
			{
				//allocate memory for the quote
				gpNPCQuoteInfoArray[ cnt ] = (NPCQuoteInfo *)MemAlloc( sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS );
				if( gpNPCQuoteInfoArray[ cnt ] == NULL )
					return( FALSE );
				memset( gpNPCQuoteInfoArray[ cnt ], 0, sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS );
			}

			//Load the NPC quote entry
			numBytesRead = 0;
			for(UINT32 cnt2 = 0; cnt2 < NUM_NPC_QUOTE_RECORDS; cnt2++)
			{
				if(guiCurrentSaveGameVersion >= STOMP12_SAVEGAME_DATATYPE_CHANGE)
				{
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].fFlags, sizeof(gpNPCQuoteInfoArray[ cnt ]->fFlags), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].sRequiredItem, sizeof(gpNPCQuoteInfoArray[ cnt ]->sRequiredItem), sizeof(INT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usFactMustBeTrue, sizeof(gpNPCQuoteInfoArray[ cnt ]->usFactMustBeTrue), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usFactMustBeFalse, sizeof(gpNPCQuoteInfoArray[ cnt ]->usFactMustBeFalse), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubQuest, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubQuest), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubFirstDay, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubFirstDay), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubLastDay, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubLastDay), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubApproachRequired, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubApproachRequired), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubOpinionRequired, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubOpinionRequired), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubQuoteNum, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubQuoteNum), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubNumQuotes, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubNumQuotes), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubStartQuest, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubStartQuest), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubEndQuest, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubEndQuest), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubTriggerNPC, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubTriggerNPC), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubTriggerNPCRec, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubTriggerNPCRec), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubFiller, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubFiller), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usSetFactTrue, sizeof(gpNPCQuoteInfoArray[ cnt ]->usSetFactTrue), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usGiftItem, sizeof(gpNPCQuoteInfoArray[ cnt ]->usGiftItem), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usUnused, sizeof(gpNPCQuoteInfoArray[ cnt ]->usUnused), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].sActionData, sizeof(gpNPCQuoteInfoArray[ cnt ]->sActionData), sizeof(INT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].sRequiredGridNo, sizeof(gpNPCQuoteInfoArray[ cnt ]->sRequiredGridNo), sizeof(INT32), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usGoToGridNo, sizeof(gpNPCQuoteInfoArray[ cnt ]->usGoToGridNo), sizeof(INT32), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usSourceDialogueLevel, sizeof(gpNPCQuoteInfoArray[ cnt ]->usSourceDialogueLevel), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usDestDialogueLevel, sizeof(gpNPCQuoteInfoArray[ cnt ]->usDestDialogueLevel), sizeof(UINT16), numBytesRead);
				}
				else
				{
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].fFlags, sizeof(gpNPCQuoteInfoArray[ cnt ]->fFlags), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].sRequiredItem, sizeof(gpNPCQuoteInfoArray[ cnt ]->sRequiredItem), sizeof(INT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].sRequiredGridNo, sizeof(gpNPCQuoteInfoArray[ cnt ]->sRequiredGridNo), sizeof(INT32), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usFactMustBeTrue, sizeof(gpNPCQuoteInfoArray[ cnt ]->usFactMustBeTrue), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usFactMustBeFalse, sizeof(gpNPCQuoteInfoArray[ cnt ]->usFactMustBeFalse), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubQuest, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubQuest), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubFirstDay, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubFirstDay), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubLastDay, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubLastDay), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubApproachRequired, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubApproachRequired), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubOpinionRequired, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubOpinionRequired), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &padding, sizeof(padding), sizeof(UINT8), numBytesRead);	//previous ubUnused no longer exists
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubQuoteNum, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubQuoteNum), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubNumQuotes, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubNumQuotes), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubStartQuest, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubStartQuest), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubEndQuest, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubEndQuest), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubTriggerNPC, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubTriggerNPC), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].ubTriggerNPCRec, sizeof(gpNPCQuoteInfoArray[ cnt ]->ubTriggerNPCRec), sizeof(UINT8), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usSetFactTrue, sizeof(gpNPCQuoteInfoArray[ cnt ]->usSetFactTrue), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usGiftItem, sizeof(gpNPCQuoteInfoArray[ cnt ]->usGiftItem), sizeof(UINT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].sActionData, sizeof(gpNPCQuoteInfoArray[ cnt ]->sActionData), sizeof(INT16), numBytesRead);
					numBytesRead = ReadFieldByField(hFile, &gpNPCQuoteInfoArray[ cnt ][cnt2].usGoToGridNo, sizeof(gpNPCQuoteInfoArray[ cnt ]->usGoToGridNo), sizeof(INT32), numBytesRead);
				}
			}
			if(guiCurrentSaveGameVersion >= STOMP12_SAVEGAME_DATATYPE_CHANGE)
			{
				while(numBytesRead != (sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS) )
					numBytesRead = ReadFieldByField(hFile, &padding, sizeof(padding), sizeof(INT8), numBytesRead);
			}
			else
			{
				while(numBytesRead != 1800 )
					numBytesRead = ReadFieldByField(hFile, &padding, sizeof(padding), sizeof(INT8), numBytesRead);
				numBytesRead += 200;
			}
			//FileRead( hFile, gpNPCQuoteInfoArray[ cnt ], sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS, &uiNumBytesRead );
			//if( uiNumBytesRead != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
			if( numBytesRead != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
			{
				return( FALSE );
			}
		}
		else
		{
		}
	}

	if ( uiSaveGameVersion >= 56 )
	{
		for( cnt = 0; cnt < NUM_CIVQUOTE_SECTORS; cnt++)
		{
			FileRead( hFile, &ubLoadQuote, sizeof( UINT8 ), &uiNumBytesRead );
			if( uiNumBytesRead != sizeof( UINT8 ) )
			{
				return( FALSE );
			}

			//if there is an existing quote
			if( gpCivQuoteInfoArray[ cnt ] )
			{
				//delete it
				MemFree( gpCivQuoteInfoArray[ cnt ] );
				gpCivQuoteInfoArray[ cnt ] = NULL;
			}

			// if there is a civ quote file
			if( ubLoadQuote )
			{
				//if there is no memory allocated
				if( gpCivQuoteInfoArray[ cnt ] == NULL )
				{
					//allocate memory for the quote
					gpCivQuoteInfoArray[ cnt ] = (NPCQuoteInfo *)MemAlloc( sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS );
					if( gpCivQuoteInfoArray[ cnt ] == NULL )
						return( FALSE );
					memset( gpCivQuoteInfoArray[ cnt ], 0, sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS );
				}

				//Load the civ quote entry
				FileRead( hFile, gpCivQuoteInfoArray[ cnt ], sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS, &uiNumBytesRead );
				if( uiNumBytesRead != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
				{
					return( FALSE );
				}
			}
		}
	}

	if ( uiSaveGameVersion < 88 )
	{
		RefreshNPCScriptRecord( NO_PROFILE, 5 ); // special pass-in value for "replace PC scripts"
		RefreshNPCScriptRecord( DARYL, 11 );
		RefreshNPCScriptRecord( DARYL, 14 );
		RefreshNPCScriptRecord( DARYL, 15 );
	}
	if ( uiSaveGameVersion < 89 )
	{
		RefreshNPCScriptRecord( KINGPIN, 23 );
		RefreshNPCScriptRecord( KINGPIN, 27 );
	}
	if ( uiSaveGameVersion < 90 )
	{
		RefreshNPCScriptRecord( KINGPIN, 25 );
		RefreshNPCScriptRecord( KINGPIN, 26 );
	}
	if ( uiSaveGameVersion < 92 )
	{
		RefreshNPCScriptRecord( MATT, 14 );
#ifdef JA2UB
//no Ub
#else
		RefreshNPCScriptRecord( AUNTIE, 8 );
#endif
	}
	if ( uiSaveGameVersion < 93 )
	{
		RefreshNPCScriptRecord( JENNY, 7 );
		RefreshNPCScriptRecord( JENNY, 8 );
		RefreshNPCScriptRecord( FRANK, 7 );
		RefreshNPCScriptRecord( FRANK, 8 );
		RefreshNPCScriptRecord( FATHER, 12 );
		RefreshNPCScriptRecord( FATHER, 13 );
	}
	if ( uiSaveGameVersion < 94 )
	{
		RefreshNPCScriptRecord( CONRAD, 0 );
		RefreshNPCScriptRecord( CONRAD, 2 );
		RefreshNPCScriptRecord( CONRAD, 9 );
	}
	if ( uiSaveGameVersion < 95 )
	{
		RefreshNPCScriptRecord( WALDO, 6 );
		RefreshNPCScriptRecord( WALDO, 7 );
		RefreshNPCScriptRecord( WALDO, 10 );
		RefreshNPCScriptRecord( WALDO, 11 );
		RefreshNPCScriptRecord( WALDO, 12 );
	}
	if ( uiSaveGameVersion < 96 )
	{
		RefreshNPCScriptRecord( HANS, 18 );
		RefreshNPCScriptRecord( ARMAND, 13 );
		RefreshNPCScriptRecord( DARREN, 4 );
		RefreshNPCScriptRecord( DARREN, 5 );
	}
	if ( uiSaveGameVersion < 97 )
	{
		RefreshNPCScriptRecord( JOHN, 22 );
		RefreshNPCScriptRecord( JOHN, 23 );
		RefreshNPCScriptRecord( SKYRIDER, 19 );
		RefreshNPCScriptRecord( SKYRIDER, 21 );
		RefreshNPCScriptRecord( SKYRIDER, 22 );
	}

	if ( uiSaveGameVersion < 98 )
	{
		RefreshNPCScriptRecord( SKYRIDER, 19 );
		RefreshNPCScriptRecord( SKYRIDER, 21 );
		RefreshNPCScriptRecord( SKYRIDER, 22 );
	}

	// anv: 
	if ( uiSaveGameVersion < WALDO_CAN_REPAIR_HELICOPTER )
	{
		RefreshNPCScriptRecord( WALDO, 5 );
		RefreshNPCScriptRecord( WALDO, 6 );
		RefreshNPCScriptRecord( WALDO, 7 );
		RefreshNPCScriptRecord( WALDO, 13 );
		RefreshNPCScriptRecord( WALDO, 14 );
		RefreshNPCScriptRecord( WALDO, 15 );
		RefreshNPCScriptRecord( WALDO, 16 );
		RefreshNPCScriptRecord( WALDO, 17 );
		RefreshNPCScriptRecord( WALDO, 18 );
	}
	
	return( TRUE );
}

BOOLEAN SaveBackupNPCInfoToSaveGameFile( HWFILE hFile )
{
	UINT32		uiNumBytesWritten=0;
	UINT32		cnt;
	UINT8			ubOne = 1;
	UINT8			ubZero = 0;


	//Loop through all the NPC quotes
	for( cnt=0; cnt<NUM_PROFILES; cnt++)
	{
		//if there is a npc qutoe
		if( gpBackupNPCQuoteInfoArray[ cnt ] )
		{
			//save a byte specify that there is an npc quote saved
			FileWrite( hFile, &ubOne, sizeof( UINT8 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT8 ) )
			{
				return( FALSE );
			}

			//Save the NPC quote entry
			FileWrite( hFile, gpBackupNPCQuoteInfoArray[ cnt ], sizeof( NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS, &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
			{
				return( FALSE );
			}
		}
		else
		{
			//save a byte specify that there is an npc quote saved
			FileWrite( hFile, &ubZero, sizeof( UINT8 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT8 ) )
			{
				return( FALSE );
			}
		}
	}

	return( TRUE );
}

BOOLEAN LoadBackupNPCInfoFromSavedGameFile( HWFILE hFile, UINT32 uiSaveGameVersion )
{
	UINT32		uiNumBytesRead=0;
	UINT32		cnt;
	UINT8			ubLoadQuote=0;
	UINT32		uiNumberOfProfilesToLoad=0;
	INT32		numBytesRead = 0;
	INT8		padding;

	if( guiCurrentSaveGameVersion >= STOMP12_SAVEGAME_DATATYPE_CHANGE )
		uiNumberOfProfilesToLoad = NUM_PROFILES;
	else
		uiNumberOfProfilesToLoad = NUM_PROFILES_v111;

	//Loop through all the NPC quotes
	for( cnt=0; cnt<uiNumberOfProfilesToLoad; cnt++ )
	{
		//Load a byte specify that there is an npc quote Loadd
		FileRead( hFile, &ubLoadQuote, sizeof( UINT8 ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( UINT8 ) )
		{
			return( FALSE );
		}

		//if there is an existing quote
		if( gpBackupNPCQuoteInfoArray[ cnt ] )
		{
			//delete it
			MemFree( gpBackupNPCQuoteInfoArray[ cnt ] );
			gpBackupNPCQuoteInfoArray[ cnt ] = NULL;
		}

		//if there is a npc quote
		if( ubLoadQuote )
		{
			//if there is no memory allocated
			if( gpBackupNPCQuoteInfoArray[ cnt ] == NULL )
			{
				//allocate memory for the quote
				gpBackupNPCQuoteInfoArray[ cnt ] = (NPCQuoteInfo *)MemAlloc( sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS );
				if( gpBackupNPCQuoteInfoArray[ cnt ] == NULL )
					return( FALSE );
				memset( gpBackupNPCQuoteInfoArray[ cnt ], 0, sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS );
			}

			//Load the NPC quote entry
			numBytesRead = 0;
			if(guiCurrentSaveGameVersion >= STOMP12_SAVEGAME_DATATYPE_CHANGE)
			{
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->fFlags, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->fFlags), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredItem, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredItem), sizeof(INT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeTrue, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeTrue), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeFalse, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeFalse), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubQuest, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubQuest), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubFirstDay, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubFirstDay), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubLastDay, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubLastDay), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubApproachRequired, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubApproachRequired), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubOpinionRequired, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubOpinionRequired), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubQuoteNum, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubQuoteNum), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubNumQuotes, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubNumQuotes), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubStartQuest, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubStartQuest), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubEndQuest, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubEndQuest), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPC, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPC), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPCRec, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPCRec), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubFiller, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubFiller), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usSetFactTrue, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usSetFactTrue), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usGiftItem, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usGiftItem), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usUnused, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usUnused), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->sActionData, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->sActionData), sizeof(INT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredGridNo, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredGridNo), sizeof(INT32), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usGoToGridNo, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usGoToGridNo), sizeof(INT32), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usSourceDialogueLevel, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usSourceDialogueLevel), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usDestDialogueLevel, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usDestDialogueLevel), sizeof(UINT16), numBytesRead);
				while(numBytesRead != (sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS) )
					numBytesRead = ReadFieldByField(hFile, &padding, sizeof(padding), sizeof(INT8), numBytesRead);
			}
			else
			{
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->fFlags, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->fFlags), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredItem, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredItem), sizeof(INT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredGridNo, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->sRequiredGridNo), sizeof(INT32), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeTrue, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeTrue), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeFalse, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usFactMustBeFalse), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubQuest, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubQuest), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubFirstDay, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubFirstDay), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubLastDay, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubLastDay), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubApproachRequired, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubApproachRequired), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubOpinionRequired, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubOpinionRequired), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &padding, sizeof(padding), sizeof(UINT8), numBytesRead);	//previous ubUnused no longer exists
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubQuoteNum, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubQuoteNum), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubNumQuotes, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubNumQuotes), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubStartQuest, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubStartQuest), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubEndQuest, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubEndQuest), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPC, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPC), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPCRec, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->ubTriggerNPCRec), sizeof(UINT8), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usSetFactTrue, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usSetFactTrue), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usGiftItem, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usGiftItem), sizeof(UINT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->sActionData, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->sActionData), sizeof(INT16), numBytesRead);
				numBytesRead = ReadFieldByField(hFile, &gpBackupNPCQuoteInfoArray[ cnt ]->usGoToGridNo, sizeof(gpBackupNPCQuoteInfoArray[ cnt ]->usGoToGridNo), sizeof(INT32), numBytesRead);
				while(numBytesRead != 1800 )
					numBytesRead = ReadFieldByField(hFile, &padding, sizeof(padding), sizeof(INT8), numBytesRead);
				numBytesRead += 200;
			}
			//FileRead( hFile, gpBackupNPCQuoteInfoArray[ cnt ], sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS, &uiNumBytesRead );
			//if( uiNumBytesRead != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
			if( numBytesRead != sizeof( NPCQuoteInfo )  * NUM_NPC_QUOTE_RECORDS )
			{
				return( FALSE );
			}
		}
		else
		{
		}
	}

	return( TRUE );
}


void TriggerFriendWithHostileQuote( UINT8 ubNPC )
{
	UINT8						ubMercsAvailable[ 40 ] = { 0 };
	UINT8						ubNumMercsAvailable = 0, ubChosenMerc;
	SOLDIERTYPE *		pTeamSoldier;
	SOLDIERTYPE *		pSoldier;
	INT32						cnt;
	INT8						bTeam;

	// First get pointer to NPC
	pSoldier = FindSoldierByProfileID( ubNPC, FALSE );
	if (!pSoldier)
	{
		return;
	}
	bTeam = pSoldier->bTeam;

	// Loop through all our guys and find one to yell

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ bTeam ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ bTeam ].bLastID; cnt++,pTeamSoldier++ )
	{
		// Add guy if he's a candidate...
		if ( pTeamSoldier->bActive && pSoldier->bInSector && pTeamSoldier->stats.bLife >= OKLIFE && pTeamSoldier->bBreath >= OKBREATH && pTeamSoldier->aiData.bOppCnt > 0 && pTeamSoldier->ubProfile != NO_PROFILE )
		{
			if ( bTeam == CIV_TEAM && pSoldier->ubCivilianGroup != NON_CIV_GROUP && pTeamSoldier->ubCivilianGroup != pSoldier->ubCivilianGroup )
			{
				continue;
			}

			if ( ! ( gMercProfiles[ pTeamSoldier->ubProfile ].ubMiscFlags & PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE ) )
			{
				ubMercsAvailable[ ubNumMercsAvailable ] = pTeamSoldier->ubProfile;
				ubNumMercsAvailable++;
			}

		}
	}

	if ( bTeam == CIV_TEAM && pSoldier->ubCivilianGroup != NON_CIV_GROUP && gTacticalStatus.fCivGroupHostile[ pSoldier->ubCivilianGroup ] == CIV_GROUP_NEUTRAL )
	{
		CivilianGroupMemberChangesSides( pSoldier );
	}

	if (ubNumMercsAvailable > 0)
	{
		PauseAITemporarily();
		ubChosenMerc = (UINT8) Random( ubNumMercsAvailable );
		TriggerNPCWithIHateYouQuote( ubMercsAvailable[ ubChosenMerc ] );
	}
	else
	{
		// done... we should enter combat mode with this soldier's team starting,
		// after all the dialogue is completed
		NPCDoAction( ubNPC, NPC_ACTION_ENTER_COMBAT, 0 );
	}
}

UINT8 ActionIDForMovementRecord( UINT8 ubNPC, UINT8 ubRecord )
{
	// Check if we have a quote to trigger...
	NPCQuoteInfo *				pNPCQuoteInfoArray;
	NPCQuoteInfo	*pQuotePtr;
	BOOLEAN				fDisplayDialogue = TRUE;

	if ( EnsureQuoteFileLoaded( ubNPC ) == FALSE )
	{
		// error!!!
		return( FALSE );
	}

	pNPCQuoteInfoArray = gpNPCQuoteInfoArray[ ubNPC ];

	pQuotePtr = &( pNPCQuoteInfoArray[ ubRecord ] );

	switch( pQuotePtr->sActionData )
	{
		case NPC_ACTION_TRAVERSE_MAP_EAST:
			return( QUOTE_ACTION_ID_TRAVERSE_EAST );

		case NPC_ACTION_TRAVERSE_MAP_SOUTH:
			return( QUOTE_ACTION_ID_TRAVERSE_SOUTH );

		case NPC_ACTION_TRAVERSE_MAP_WEST:
			return( QUOTE_ACTION_ID_TRAVERSE_WEST );

		case NPC_ACTION_TRAVERSE_MAP_NORTH:
			return( QUOTE_ACTION_ID_TRAVERSE_NORTH );

		default:
			return( QUOTE_ACTION_ID_CHECKFORDEST );

	}
}

void HandleNPCChangesForTacticalTraversal( SOLDIERTYPE * pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE || (pSoldier->aiData.fAIFlags & AI_CHECK_SCHEDULE) )
	{
		return;
	}

	switch( pSoldier->ubQuoteActionID )
	{
		case QUOTE_ACTION_ID_TRAVERSE_EAST:
			gMercProfiles[pSoldier->ubProfile].sSectorX++;

			// Call to change the NPC's Sector Location
			ChangeNpcToDifferentSector( pSoldier->ubProfile,
							gMercProfiles[pSoldier->ubProfile].sSectorX,
							gMercProfiles[pSoldier->ubProfile].sSectorY,
							gMercProfiles[pSoldier->ubProfile].bSectorZ );
			break;
		case QUOTE_ACTION_ID_TRAVERSE_SOUTH:
			gMercProfiles[pSoldier->ubProfile].sSectorY++;

			// Call to change the NPC's Sector Location
			ChangeNpcToDifferentSector( pSoldier->ubProfile,
							gMercProfiles[pSoldier->ubProfile].sSectorX,
							gMercProfiles[pSoldier->ubProfile].sSectorY,
							gMercProfiles[pSoldier->ubProfile].bSectorZ );
			break;
		case QUOTE_ACTION_ID_TRAVERSE_WEST:
			gMercProfiles[pSoldier->ubProfile].sSectorX--;

			// Call to change the NPC's Sector Location
			ChangeNpcToDifferentSector( pSoldier->ubProfile,
							gMercProfiles[pSoldier->ubProfile].sSectorX,
							gMercProfiles[pSoldier->ubProfile].sSectorY,
							gMercProfiles[pSoldier->ubProfile].bSectorZ );
			break;
		case QUOTE_ACTION_ID_TRAVERSE_NORTH:
			gMercProfiles[pSoldier->ubProfile].sSectorY--;

			// Call to change the NPC's Sector Location
			ChangeNpcToDifferentSector( pSoldier->ubProfile,
							gMercProfiles[pSoldier->ubProfile].sSectorX,
							gMercProfiles[pSoldier->ubProfile].sSectorY,
							gMercProfiles[pSoldier->ubProfile].bSectorZ );
			break;
		default:
			break;
	}
}


void HandleVictoryInNPCSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	// handle special cases of victory in certain sector
	INT16 sSector = 0;

	// not the surface?..leave
	if( sSectorZ != 0 )
	{
		return;
	}

	// grab sector value
	sSector = SECTOR( sSectorX, sSectorY );

	switch( sSector )
	{
		case( SEC_F10 ):
		{
			// we won over the hillbillies
			// set fact they are dead
			// silversurfer: Only if Darren and Daryl are dead and we didn't just fight other opponents in their sector!
			if ( gMercProfiles[DARREL].bMercStatus == MERC_IS_DEAD && gMercProfiles[DARYL].bMercStatus == MERC_IS_DEAD )
			{
				if( CheckFact( FACT_HILLBILLIES_KILLED, KEITH ) == FALSE )
				{
					SetFactTrue( FACT_HILLBILLIES_KILLED );
				}

				// check if keith is out of business
				if( CheckFact( FACT_KEITH_OUT_OF_BUSINESS, KEITH ) == TRUE )
				{
					SetFactFalse( FACT_KEITH_OUT_OF_BUSINESS );
				}
			}
		}
	}

	return;
}

BOOLEAN HandleShopKeepHasBeenShutDown( UINT8 ubCharNum )
{
	// check if shopkeep has been shutdown, if so handle
	switch( ubCharNum )
	{
		case( KEITH ):
		{
			// if keith out of business, do action and leave
			if( CheckFact( FACT_KEITH_OUT_OF_BUSINESS, KEITH ) == TRUE )
			{
				TriggerNPCRecord( KEITH, 11 );

				return( TRUE );
			}
			else if( CheckFact( FACT_LOYALTY_LOW, KEITH ) == TRUE )
			{
				// loyalty is too low
				TriggerNPCRecord( KEITH, 7 );

				return( TRUE );
			}
		}
	}

	return( FALSE );
}

#ifdef JA2BETAVERSION
void ToggleNPCRecordDisplay( void )
{
	if ( gfDisplayScreenMsgOnRecordUsage )
	{
		gfDisplayScreenMsgOnRecordUsage = FALSE;
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Turning record reporting OFF" );
	}
	else
	{
		gfDisplayScreenMsgOnRecordUsage = TRUE;
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Turning record reporting ON" );
	}
}
#endif

void UpdateDarrelScriptToGoTo( SOLDIERTYPE * pSoldier )
{
	// change destination in Darrel record 10 to go to a gridno adjacent to the
	// soldier's gridno, and destination in record 11
	INT32 sAdjustedGridNo;
	UINT8 ubDummyDirection;
	SOLDIERTYPE *		pDarrel;

	pDarrel = FindSoldierByProfileID( DARREL, FALSE );
	if ( !pDarrel )
	{
		return;
	}

	// find a spot to an alternate location nearby
	sAdjustedGridNo = FindGridNoFromSweetSpotExcludingSweetSpot( pDarrel, pSoldier->sGridNo, 5, &ubDummyDirection );	
	if (TileIsOutOfBounds(sAdjustedGridNo))
	{
		// yikes! try again with a bigger radius!
		sAdjustedGridNo = FindGridNoFromSweetSpotExcludingSweetSpot( pDarrel, pSoldier->sGridNo, 10, &ubDummyDirection );		
		if (TileIsOutOfBounds(sAdjustedGridNo))
		{
			// ok, now we're completely foobar
			return;
		}
	}

	EnsureQuoteFileLoaded( DARREL );
	gpNPCQuoteInfoArray[ DARREL ][ 10 ].usGoToGridNo = sAdjustedGridNo;
	gpNPCQuoteInfoArray[ DARREL ][ 11 ].sRequiredGridNo = sAdjustedGridNo;//dnl ch46 031009
	gpNPCQuoteInfoArray[ DARREL ][ 11 ].ubTriggerNPC = pSoldier->ubProfile;
}

BOOLEAN RecordHasDialogue( UINT8 ubNPC, UINT8 ubRecord )
{
	if (EnsureQuoteFileLoaded( ubNPC ) == FALSE)
	{
		// error!!!
		return( FALSE );
	}

	if ( gpNPCQuoteInfoArray[ ubNPC ][ ubRecord ].ubQuoteNum != NO_QUOTE && gpNPCQuoteInfoArray[ ubNPC ][ ubRecord ].ubQuoteNum != 0 )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

INT8 FindCivQuoteFileIndex( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	UINT8 ubLoop;

  if ( sSectorZ > 0 )
  {
    return( MINERS_CIV_QUOTE_INDEX );
  }
  else
  {
	  for ( ubLoop = 0; ubLoop < NUM_CIVQUOTE_SECTORS; ubLoop++ )
	  {
		  if ( gsCivQuoteSector[ ubLoop ][ 0 ] == sSectorX && gsCivQuoteSector[ ubLoop ][ 1 ] == sSectorY )
		  {
			  return( ubLoop );
		  }
	  }
  }
	return( -1 );
}

INT8 ConsiderCivilianQuotes( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, BOOLEAN fSetAsUsed )
{
	INT8							bLoop, bCivQuoteSectorIndex;
	NPCQuoteInfo *		pCivQuoteInfoArray;

	bCivQuoteSectorIndex = FindCivQuoteFileIndex( sSectorX, sSectorY, sSectorZ );
	if ( bCivQuoteSectorIndex == -1 )
	{
		// no hints for this sector!
		return( -1 );
	}

	if ( EnsureCivQuoteFileLoaded( bCivQuoteSectorIndex ) == FALSE )
	{
		// error!!!
		return( -1 );
	}

	pCivQuoteInfoArray = gpCivQuoteInfoArray[ bCivQuoteSectorIndex ];

	for ( bLoop = 0; bLoop < NUM_NPC_QUOTE_RECORDS; bLoop++ )
	{
		if ( NPCConsiderQuote( NO_PROFILE, NO_PROFILE, 0, bLoop, 0, pCivQuoteInfoArray ) )
		{
			if ( fSetAsUsed )
			{
				TURN_FLAG_ON( pCivQuoteInfoArray[ bLoop ].fFlags, QUOTE_FLAG_SAID );
			}

			if (pCivQuoteInfoArray[ bLoop ].ubStartQuest != NO_QUEST)
			{
				StartQuest( pCivQuoteInfoArray[ bLoop ].ubStartQuest, gWorldSectorX, gWorldSectorY );
			}

			// return quote #
			return( pCivQuoteInfoArray[ bLoop ].ubQuoteNum );
		}
	}

	return( -1 );
}

#ifdef JA2UB
//UB

BOOLEAN HasNpcSaidQuoteBefore( UINT8 ubNPC, UINT8 ubRecord )
{
	if( CHECK_FLAG( gpNPCQuoteInfoArray[ ubNPC ][ ubRecord ].fFlags, QUOTE_FLAG_SAID ) )
		return( TRUE );
	else
		return( FALSE );
}
#endif
