#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "strategic.h"
#else
	#include "builddefines.h"
	#include "bullets.h"
	#include <stdio.h>
	#include <string.h>
	#include "wcheck.h"
	#include "stdlib.h"
	#include "debug.h"
	#include "math.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "renderworld.h"
	#include "Assignments.h"
	#include "Soldier Control.h"
	#include "Animation Control.h"
	#include "Animation Data.h"
	#include "Isometric Utils.h"
	#include "Event Pump.h"
	#include "Timer Control.h"
	#include "Render Fun.h" 
	#include "Render Dirty.h"
	#include "mousesystem.h"
	#include "interface.h"
	#include "sysutil.h"
	#include "FileMan.h"
	#include "points.h"
	#include "Random.h"
	#include "ai.h"
	#include "Interactive Tiles.h"
	#include "soldier ani.h"
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
	#include "Mercs.h"
	#include "Handle Doors.h"
	#include "Campaign Types.h"
	#include "Tactical Save.h"
	#include "BobbyRMailOrder.h"
	#include "Finances.h"
	#include "TeamTurns.h"
	#include "gameloop.h"
	#include "Options Screen.h"
#include "physics.h"
#include "Explosion Control.h"
#include "SmokeEffects.h"
#include "MPChatScreen.h"
#include "sgp_logger.h"
#endif

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "RakSleep.h"

#include "FileListTransfer.h"
#include "FileListTransferCBInterface.h"
#include "FileOperations.h"
#include "SuperFastHash.h"
#include "RakAssert.h"
#include "IncrementalReadInterface.h"

#include "BitStream.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "music control.h"
#include "map edgepoints.h"

#include "fresh_header.h"
#include "network.h"
#include "opplist.h"

#include "tactical placement gui.h"
#include "prebattle interface.h"
#include "mapscreen.h"

#include "MessageBoxScreen.h"

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_init.h>
#include <vfs/Core/vfs_profile.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Core/File/vfs_file.h>
#include "transfer_rules.h"

#include "keys.h"
#include "new.h"
#include "Types.h"
#include "connect.h"
#include "message.h"
#include "Event pump.h"
#include "Soldier Init List.h"
#include "Overhead.h"
#include "weapons.h"
#include "Merc Hiring.h"
#include "soldier profile.h"
#include "environment.h"
#include "lighting.h"
#include "laptop.h"
#include "interface panels.h"
#include "game init.h"
#include "Debug Control.h"
#include "MPConnectScreen.h"
#include "IniReader.h"
#include "Map Screen Interface Border.h"

extern CHAR16 gzFileTransferDirectory[100];

// WANNE: FILE TRANSFER
BOOLEAN		fClientReceivedAllFiles;
STRING512	client_executableDir;
STRING512	client_fileTransferDirectoryPath;	// the clients file transfer directory absolut path
STRING512	server_fileTransferDirectoryPath;	// the server file transfer directory absolut path
INT16		fileTransferProgress = 0;
INT16		serverSyncClientsDirectory = 0;

unsigned char GetPacketIdentifier(Packet *p);
unsigned char packetIdentifier;

// OJW - 20090405
STRING512	gCurrentTransferFilename;
INT32		gCurrentTransferBytes = 0;
INT32		gTotalTransferBytes = 0;

extern BOOLEAN gfTemporaryDisablingOfLoadPendingFlag;

extern INT8 SquadMovementGroups[ ];
RakPeerInterface *client;
// WANNE: FILE TRANSFER
FileListTransfer fltClient;	// flt2

char *ReplaceCharactersInString_Client(char *str, char *orig, char *rep)
{
	static char buffer[4096];
	char *p;

	if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
		return str;

	strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
	buffer[p-str] = '\0';

	sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

	return buffer;
}

class ClientTransferCB : public FileListTransferCBInterface
{
	public:
		// This method gets called for each file when it is completly received by the client.
		// Now the file will be saved on the client
		bool OnFile(OnFileStruct *onFileStruct)
		{

			if(!transferRules)
			{
				transferRules = new CTransferRules();
				transferRules->initFromTxtFile("transfer_rules.txt");
			}
			// Get the directory path of the file and output it to the user!
			char* targetFileName = ExtractFilename(onFileStruct->fileName);
			//ScreenMsg( FONT_BCOLOR_ORANGE, MSG_CHAT, MPClientMessage[58], targetFileName);

			vfs::Path fileName(onFileStruct->fileName);
			vfs::String::str_t const& valid_str = fileName.c_wcs();
			vfs::String::size_t pos = valid_str.find(L":");
			if(pos != vfs::String::str_t::npos)
			{
				// absolute path?? these are invalid and the server is not supposed to send us such paths
				// potentialy malicious server -> output error
				return false;
			}
			if(valid_str.substr(0,2) == vfs::Const::DOTDOT())
			{
				// trying to break out from the profile?
				// potentialy malicious server -> output error
				return false;
			}
			if(transferRules && (transferRules->applyRule(valid_str) == CTransferRules::DENY))
			{
				// sent file was on our ignore list
				// it may be OK that the server's list and the clients' lists diverge
				// send message to server that we cannot accept this file
				return false;
			}

			strcpy(gCurrentTransferFilename,onFileStruct->fileName);
			
			try
			{
				vfs::COpenWriteFile wfile(fileName,true,true);
				wfile->write(onFileStruct->fileData,onFileStruct->finalDataLength);
			}
			catch(vfs::Exception& ex)
			{
				SGP_ERROR(ex.what());
				ScreenMsg( FONT_BCOLOR_BLUE, MSG_CHAT, MPClientMessage[70], targetFileName);				
			}

			/*
			// Make sure it worked
			unsigned int hash1 = SuperFastHashFile(fileToSend);
			if (RakNet::BitStream::DoEndianSwap())
				RakNet::BitStream::ReverseBytesInPlace((unsigned char*) &hash1, sizeof(hash1));
			unsigned int hash2 = SuperFastHashFile(targetFileName);
			if (RakNet::BitStream::DoEndianSwap())
				RakNet::BitStream::ReverseBytesInPlace((unsigned char*) &hash2, sizeof(hash2));
			RakAssert(hash1==hash2);
			*/

			//ScreenMsg( FONT_BCOLOR_ORANGE, MSG_MPSYSTEM, L"Saved file local in %S", file);

			// Return true to have RakNet delete the memory allocated to hold this file.
			// False if you hold onto the memory, and plan to delete it yourself later
			return true;
		}

		// WANNE: FILE TRANSFER: This method gets called each periodically
		virtual void OnFileProgress(OnFileStruct *onFileStruct,unsigned int partCount,unsigned int partTotal,unsigned int partLength, char *firstDataChunk)
		{
			static UINT32 iNextTransferProgressUpdateTime;

			gCurrentTransferBytes += (INT32)(onFileStruct->finalDataLength * (float)(1.0f/(float)partTotal));

			if (guiBaseJA2NoPauseClock >= iNextTransferProgressUpdateTime)
			{
				char *relativeFname = ReplaceCharactersInString_Client(onFileStruct->fileName, server_fileTransferDirectoryPath,"");
				strcpy(gCurrentTransferFilename,relativeFname);

				iNextTransferProgressUpdateTime = guiBaseJA2NoPauseClock + 100;
				// update all clients of our file transfer progress
				INT8 currentProgress = (INT8)(100.0f * (float)((float)gCurrentTransferBytes/(float)gTotalTransferBytes));

				SetConnectScreenSubMessageA(gCurrentTransferFilename); // setting this also causes connect screen to refresh

				if (recieved_settings && CLIENT_NUM > 0 && CLIENT_NUM <= 4)
				{
					client_progress[CLIENT_NUM-1] = currentProgress;
					client_downloading[CLIENT_NUM-1] = 1;
					fDrawCharacterList = true;

					progress_struct prog;
					prog.client_num = CLIENT_NUM;
					prog.downloading = 1;
					prog.progress = currentProgress;

					client->RPC("sendDOWNLOADSTATUS",(const char*)&prog, (int)sizeof(progress_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				}
			}

			/*INT16 currentProgress = 100 * partCount/partTotal;
			INT16 fileTransferProgressNew = 0;

			// WANNE: FILE TRANSFER: Only output (0, 25, 50, 75) percentage
			if (currentProgress < 25)
				fileTransferProgressNew = 0;
			else if (currentProgress < 50)
				fileTransferProgressNew = 25;
			else if (currentProgress < 75)
				fileTransferProgressNew = 50;
			else
				fileTransferProgressNew = 75;

			if (fileTransferProgressNew != fileTransferProgress)
			{
				char* targetFileName = ExtractFilename(onFileStruct->fileName);
				fileTransferProgress = fileTransferProgressNew;
				ScreenMsg( FONT_BCOLOR_BROWN, MSG_MPSYSTEM, MPClientMessage[59], targetFileName, fileTransferProgress);
			}*/


			//ScreenMsg( FONT_BLUE, MSG_MPSYSTEM, L"(%i%%) %S", 100*partCount/partTotal, onFileStruct->fileName);
			//ScreenMsg( FONT_BLUE, MSG_MPSYSTEM, L"%i (%i%%) %i/%i %S %ib->%ib / %ib->%ib\n", onFileStruct->setID, 100*partCount/partTotal, onFileStruct->fileIndex+1, onFileStruct->setCount, onFileStruct->fileName, onFileStruct->compressedTransmissionLength, onFileStruct->finalDataLength, onFileStruct->setTotalCompressedTransmissionLength, onFileStruct->setTotalFinalLength, firstDataChunk);

			//printf("%i (%i%%) %i/%i %s %ib->%ib / %ib->%ib\n", onFileStruct->setID, 100*partCount/partTotal, onFileStruct->fileIndex+1, onFileStruct->setCount, onFileStruct->fileName, onFileStruct->compressedTransmissionLength, onFileStruct->finalDataLength, onFileStruct->setTotalCompressedTransmissionLength, onFileStruct->setTotalFinalLength, firstDataChunk);
		}

		virtual bool OnDownloadComplete(void)
		{
			//printf("Download complete.\n");
			if (serverSyncClientsDirectory)
			{
				ScreenMsg( FONT_RED, MSG_MPSYSTEM, MPClientMessage[60]);
				SetConnectScreenSubMessageW(MPClientMessage[60]); // setting this also causes connect screen to refresh

				//fileTransferProgress = 0;
				if(transferRules)
				{
					delete transferRules;
					transferRules = NULL;
				}

				if (recieved_settings && CLIENT_NUM > 0 && CLIENT_NUM <= 4)
				{
					// notify ourselves
					client_progress[CLIENT_NUM-1] = 0;
					client_downloading[CLIENT_NUM-1] = 0;
					// notify others
					progress_struct prog;
					prog.client_num = CLIENT_NUM;
					prog.downloading = 0; // notify clients we have finished
					prog.progress = 0;

					fDrawCharacterList = true;

					client->RPC("sendDOWNLOADSTATUS",(const char*)&prog, (int)sizeof(progress_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
				}
			}

			// Set a flag that we have received all the files. This is used that when another client connects we do not want to receive the files again!
			fClientReceivedAllFiles = TRUE;

			// Returning false automatically deallocates the automatically allocated handler that was created by DirectoryDeltaTransfer
			return false;
		}

	private:
		char *ExtractFilename(char *pathname) 
		{
			char *s;

			if ((s=strrchr(pathname, '\\')) != NULL) s++;
			else if ((s=strrchr(pathname, '/')) != NULL) s++;
			else if ((s=strrchr(pathname, ':')) != NULL) s++;
			else s = pathname;
			return s;
		}

		// client has an ignore list too, as the server doesn't have to play by the rules
		static CTransferRules* transferRules;

} transferCallback;


CTransferRules* ClientTransferCB::transferRules = NULL;


ClientTransferCB transferCBClient;

// OJW - 20081222
player_stats gMPPlayerStats[5];

// OJW - 20090503 - get rid of compile error
#pragma pack (1)

typedef struct
{
	UINT8		ubProfileID;
	int		team;
	BOOLEAN fCopyProfileItemsOver;
	INT8 bTeam;

} send_hire_struct;

typedef struct
{
	UINT8	ubProfileID;

} send_dismiss_struct;

typedef struct
{
	UINT8		usSoldierID;
	FLOAT dNewXPos;
	FLOAT dNewYPos;

} gui_pos;

typedef struct
{
	UINT8		usSoldierID;
	INT16	usNewDirection;

} gui_dir;

typedef struct
{	
	UINT8 tsnetbTeam;
	UINT8 tsubNextTeam;
} turn_struct;

typedef struct
	{

		SOLDIERCREATE_STRUCT standard_data;

		OBJECTTYPE slot[55];

	} AI_STRUCT;

typedef struct
{
	BULLET net_bullet;
	UINT16 usHandItem;

}netb_struct;

typedef struct
{
	UINT8 ubID;
	INT8 bTeam;
	UINT8 gubOutOfTurnPersons;
	UINT8 gubOutOfTurnOrder[MAXMERCS];
	BOOLEAN fMarkInterruptOccurred;
	UINT8 Interrupted;
} INT_STRUCT;

typedef struct
{
	UINT8 client_num;
	bool status;
	UINT8 ready_stage;
} ready_struct;

typedef struct
{
	INT32 remote_id;
	INT32 local_id;

}bullets_table;

typedef struct
	{
		UINT8 ubResult;
	}kickR;

typedef struct
{
	UINT8 client_num;
	BOOLEAN bToAll;
	CHAR16 msg[512];
} chat_msg;

// OJW - 20091002 - explosions
typedef struct
{
	float dLifeSpan;
	float dX;
	float dY;
	float dZ;
	float dForceX;
	float dForceY;
	float dForceZ;
	UINT32 sTargetGridNo;
	UINT8 ubID;
	UINT8 ubActionCode;
	UINT32 uiActionData;
	UINT16 usItem;
	INT32 RealObjectID; // the local ID on the initiating client
	bool IsThrownGrenade; // could be mortar
	UINT32 uiPreRandomIndex;
} physics_object;

typedef struct
{
	float dX;
	float dY;
	float dZ;
	INT32 sGridNo;
	bool bWasDud;
	UINT8 ubOwnerID;
	INT32 RealObjectID; // the local ID on the initiating client
	UINT32 uiPreRandomIndex; // send out our current pre-random index
} grenade_result;

typedef struct
{
	UINT32 sGridNo;
	UINT8 ubID;
	UINT16 usItem;
	UINT8 ubItemStatus;
	UINT32 uiWorldIndex; // the local World Index of this bomb on its creators client
	UINT16 usFlags;
	UINT8 ubLevel;
	INT8 bDetonatorType;
	INT8 bDelayFreq;
} explosive_obj;

typedef struct
{
	UINT8 ubID;
	UINT32 uiWorldItemIndex;
	UINT8 ubMPTeamIndex;
	UINT32 uiPreRandomIndex; // send out our current pre-random index
} detonate_struct;

typedef struct
{
	UINT32 uiWorldItemIndex;
	UINT8 ubMPTeamIndex;
	UINT8 ubID;
	UINT32 sGridNo;
	UINT32 uiPreRandomIndex; // send out our current pre-random index
} disarm_struct;

typedef struct
{
	INT32 sGridNo;
	UINT8 ubRadius;
	UINT16 usItem;
	UINT8 ubOwner;
	BOOLEAN fSubsequent;
	INT8 bLevel;
	INT32 iSmokeEffectID;
	UINT32 uiPreRandomIndex;
} spreadeffect_struct;

typedef struct
{
	UINT8 ubDamageFunc; // 1 - gas damage , 2 - explosive damage
	UINT8 ubSoldierID;
	UINT16 usExplosiveClassID;
	INT16 sSubsequent;
	BOOL fRecompileMovementCosts;
	INT16 sWoundAmt;
	INT16 sBreathAmt;
	UINT8 ubAttackerID;
	UINT16 usItem;
	INT32 sBombGridNo;
	UINT32 uiDist;
	UINT32 uiPreRandomIndex;
} explosiondamage_struct;

bullets_table bTable[11][50];

char client_names[4][30];
// OJW - 20081204
int	 client_ready[4];
int	 client_edges[5];
int	 client_teams[4];
int	 random_mercs[7];
// OJW - 20090305
int	 client_downloading[4];
int	 client_progress[4];
int		TEAM;

UINT8	netbTeam;
UINT8	ubID_prefix;

bool is_connected=false;
bool is_connecting=false;
bool is_client=false;
bool is_server=false;
bool is_networked=false;
bool is_host=false; // OJW - added 20081130 - new flag to signal our intention to host, coming in from the HOST screen
bool auto_retry=true;
int  giNumTries = MAX_CONNECT_RETRIES; // default is 5 retries
UINT32 giNextRetryTime = 0;
bool requested=false;
int kit[20];
bool allowlaptop=false;
bool recieved_settings=false;
// OJW - 20090422
bool recieved_transfer_settings = false;
bool getReal=false;

int CLIENT_NUM;

// --------------------
// Global Client Variables
// --------------------
UINT8 cStartingSectorEdge;
char cKitBag[100];
char cClientName[30];
UINT8	gRandomStartingEdge;
UINT8	gRandomMercs;
char	cServerName[30];
char cGameDataSyncDirectory[100];
UINT8 cReportHiredMerc;
UINT8 cWeaponReadyBonus;
UINT8 cDisableSpectatorMode;
UINT8 gEnemyEnabled;
UINT8 gCreatureEnabled;
UINT8 gMilitiaEnabled;
UINT8 gCivEnabled;
UINT8 cAllowMercEquipment;
UINT8 cSameMercAllowed;
UINT8 cGameType;
UINT8 cMaxMercs;
FLOAT cDamageMultiplier;
UINT8 cMaxClients;
UINT8 cDisableMorale;
// --------------------

// --------------------
// Other client variables
// --------------------
bool goahead = 0;
int numready = 0;
int readystage = 0;
bool status = 0;
bool wiped;
UINT32 iScoreScreenTime = 0;
int iTeamsWiped = 0; // counts how many teams have wiped
bool is_game_over = false;
bool isOwnTeamWipedOut = false;
int iDisconnectedScreen = 0;
int bClosingChatBoxToStartGame = false;
UINT32 iCCStartGameTime = 0;
bool is_game_started = false;

void overide_callback( UINT8 ubResult );
void kick_callback( UINT8 ubResult );
void turn_callback (UINT8 ubResult);
void ChatCallback (UINT8 ubResult);
void HandleClientConnectionLost();
void disconnected_callback(UINT8 ubResult);

// OJW - 20081222
void send_gameover( void );
void StartScoreScreen(); // this screen will send us to the multiplayer score screen

// OJW - 20090403
extern BOOLEAN		gfMPSScoreScreenCanContinue; // can the score screen continue

// OJW - 20090430
SystemAddress serverAddr;

// OJW - 20090507
settings_struct gMPServerSettings; // store a copy of our settings after we receive them
CHAR16			gszDisconnectReason[255]; // the reason we were disconnected from the server

// OJW - added 20081130
// <TODO> - add retry timer and notification
void NetworkAutoStart()
{
	if (!is_networked)
		return; // not networked, bad call

	if (!is_host || is_server) 
	{
		// is pure client or the server has been started
		// so do client connection checking...though probably not needed for the server client. cant hurt.

		// user JOIN'd game
		if (is_client && is_connected)
		{
			return; // should be set up and running
		}
		else if (is_connecting)
		{
			return; // we're waiting on a connection , do nothing
		}
		else
		{
			if (giNumTries <= 0 || !auto_retry)
				return; // dont auto-retry

			if (guiBaseJA2NoPauseClock < giNextRetryTime)
				return; // we are waiting for a retry timer

			// try and connect
			giNumTries--;
			connect_client();

			// next rety time is set in client_packet()
		}
	}
	else 
	{
		// is host and server isnt started so start it
		start_server();
	}
}

void InvalidClientSettingsOkBoxCallback( UINT8 bExitValue )
{
	// yes, load the game
	if( bExitValue == MSG_BOX_RETURN_OK )
	{		
		// gracefully disconnect to the main menu
		client_disconnect();
		guiPendingScreen = MP_JOIN_SCREEN;
	}

	return;
}


// OJW - this is just a fudge for now and needs to be improved later
bool are_clients_downloading()
{
	bool bDownloading = false;
	for (int i=0; i < 4; i++)
	{
		if (client_downloading[i] == 1)
			bDownloading = true;
	}

	return bDownloading;
}



void HireRandomMercs()
{
	MERC_HIRE_STRUCT HireMercStruct;
	for (int i=0; i < cMaxMercs; i++)
	{
		memset(&HireMercStruct, 0, sizeof(MERC_HIRE_STRUCT));

		HireMercStruct.ubProfileID = random_mercs[i];// use the merc list recieved from the server

		//DEF: temp
		HireMercStruct.sSectorX = gsMercArriveSectorX;
		HireMercStruct.sSectorY = gsMercArriveSectorY;
		HireMercStruct.fUseLandingZoneForArrival = TRUE;
		HireMercStruct.ubInsertionCode	= INSERTION_CODE_ARRIVING_GAME;
		HireMercStruct.iTotalContractLength = 1;
		HireMercStruct.fCopyProfileItemsOver = true;

		gMercProfiles[ HireMercStruct.ubProfileID ].ubMiscFlags |= PROFILE_MISC_FLAG_ALREADY_USED_ITEMS;

		HireMerc(&HireMercStruct);
	}

	fDrawCharacterList = true;
	fTeamPanelDirty = true;
	ReBuildCharactersList();
}


//*****************
//RPC sends and recieves:
//********************

void send_path (  SOLDIERTYPE *pSoldier, INT32 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  )
{
	if(pSoldier->ubID < 120)
	{
		//ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, L"Sending new path" );

		EV_S_SENDPATHTONETWORK SNetPath;

		if(pSoldier->ubID < 20)
			SNetPath.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		else
			SNetPath.usSoldierID = pSoldier->ubID;
	
		SNetPath.sAtGridNo=pSoldier->sGridNo;
				
		SNetPath.ubNewState=usMovementAnim;
		SNetPath.usCurrentPathIndex=pSoldier->pathing.usPathIndex;
		memcpy(SNetPath.usPathData, pSoldier->pathing.usPathingData, sizeof(UINT16)*30);
		SNetPath.usPathDataSize=pSoldier->pathing.usPathDataSize;
		SNetPath.sDestGridNo=sDestGridNo;
			
		client->RPC("sendPATH",(const char*)&SNetPath, (int)sizeof(EV_S_SENDPATHTONETWORK)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recievePATH(RPCParameters *rpcParameters)
{
	//ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, L"Recieving new path," );
				
	EV_S_SENDPATHTONETWORK* SNetPath = (EV_S_SENDPATHTONETWORK*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ SNetPath->usSoldierID ];


	memcpy(pSoldier->pathing.usPathingData, SNetPath->usPathData,sizeof(UINT16)*30);

	pSoldier->pathing.sDestination = SNetPath->sDestGridNo;
	pSoldier->pathing.sFinalDestination = SNetPath->sDestGridNo;
	pSoldier->pathing.usPathIndex=SNetPath->usCurrentPathIndex;
	pSoldier->pathing.usPathDataSize=SNetPath->usPathDataSize;

	SendGetNewSoldierPathEvent( pSoldier, SNetPath->sDestGridNo, SNetPath->ubNewState );	

	INT16 sCellX, sCellY;	
	
	sCellX = CenterX( SNetPath->sAtGridNo );
	sCellY = CenterY( SNetPath->sAtGridNo );
	
	if (( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_MOVING | ANIM_SPECIALMOVE ) ) && !(pSoldier->flags.fNoAPToFinishMove ) )
	{
	}
	else
	{
		pSoldier->EVENT_InternalSetSoldierPosition( sCellX, sCellY ,FALSE, FALSE, FALSE );		
	}

	if(pSoldier)pSoldier->EVENT_InitNewSoldierAnim( SNetPath->ubNewState, 0, FALSE );			
}


void send_stance ( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance )

{
	if(pSoldier->ubID < 120)
	{
		EV_S_CHANGESTANCE			SChangeStance;

		SChangeStance.ubNewStance   = ubDesiredStance;
		//SChangeStance.usSoldierID  = pSoldier->ubID;
		

		if(pSoldier->ubID < 20)
			SChangeStance.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		else
			SChangeStance.usSoldierID = pSoldier->ubID;


		SChangeStance.sXPos				= pSoldier->sX;
		SChangeStance.sYPos				= pSoldier->sY;
		SChangeStance.uiUniqueId = pSoldier -> uiUniqueSoldierIdValue;

		//ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, L"change stance: %d",ubDesiredStance );
	
		client->RPC("sendSTANCE",(const char*)&SChangeStance, (int)sizeof(EV_S_CHANGESTANCE)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}


void recieveSTANCE(RPCParameters *rpcParameters)
{


		EV_S_CHANGESTANCE* SChangeStance = (EV_S_CHANGESTANCE*)rpcParameters->input;
	
		SOLDIERTYPE *pSoldier = MercPtrs[ SChangeStance->usSoldierID ];
	
		pSoldier->ChangeSoldierStance( SChangeStance->ubNewStance );

		//SendChangeSoldierStanceEvent( pSoldier, SChangeStance->ubNewStance );
		//AddGameEvent( S_CHANGESTANCE, 0, &SChangeStance );
		//********* implemented using event pump system ... :)
}



void send_dir ( SOLDIERTYPE *pSoldier, UINT16 usDesiredDirection )

{
	if((is_server && pSoldier->ubID < 120) || (!is_server && pSoldier->ubID < 20))
	{

		EV_S_SETDESIREDDIRECTION	SSetDesiredDirection;

		//SSetDesiredDirection.usSoldierID = pSoldier->ubID;
			

		if(pSoldier->ubID < 20)
			SSetDesiredDirection.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		else
			SSetDesiredDirection.usSoldierID = pSoldier->ubID;

	

		SSetDesiredDirection.usDesiredDirection = usDesiredDirection;
		SSetDesiredDirection.uiUniqueId = pSoldier -> uiUniqueSoldierIdValue;

		client->RPC("sendDIR",(const char*)&SSetDesiredDirection, (int)sizeof(EV_S_SETDESIREDDIRECTION)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}


void recieveDIR(RPCParameters *rpcParameters)
{

		EV_S_SETDESIREDDIRECTION* SSetDesiredDirection = (EV_S_SETDESIREDDIRECTION*)rpcParameters->input;			
			

		SOLDIERTYPE *pSoldier = MercPtrs[ SSetDesiredDirection->usSoldierID ];

		pSoldier->EVENT_SetSoldierDesiredDirection( SSetDesiredDirection->usDesiredDirection );

		//AddGameEvent( S_SETDESIREDDIRECTION, 0, &SSetDesiredDirection );
		//********* implemented using event pump system ... :)
}

void send_fire( SOLDIERTYPE *pSoldier, INT32 sTargetGridNo )
{
	if(pSoldier->ubID < 120)
	{
	
	EV_S_BEGINFIREWEAPON SBeginFireWeapon;


		

	if(pSoldier->ubID < 20)
		SBeginFireWeapon.usSoldierID = (pSoldier->ubID)+ubID_prefix;
	else
		SBeginFireWeapon.usSoldierID = pSoldier->ubID;


	SBeginFireWeapon.sTargetGridNo = sTargetGridNo;
	SBeginFireWeapon.bTargetLevel = pSoldier->bTargetLevel;
	SBeginFireWeapon.bTargetCubeLevel = pSoldier->bTargetCubeLevel;
	SBeginFireWeapon.uiUniqueId = pSoldier->usAttackingWeapon;
		

	client->RPC("sendFIRE",(const char*)&SBeginFireWeapon, (int)sizeof(EV_S_BEGINFIREWEAPON)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieveFIRE(RPCParameters *rpcParameters)
{		
	EV_S_BEGINFIREWEAPON* SBeginFireWeapon = (EV_S_BEGINFIREWEAPON*)rpcParameters->input;
	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"SendBeginFireWeaponEvent" );

	SOLDIERTYPE *pSoldier = MercPtrs[ SBeginFireWeapon->usSoldierID ];

	pSoldier->sTargetGridNo = SBeginFireWeapon->sTargetGridNo;
	pSoldier->bTargetLevel = SBeginFireWeapon->bTargetLevel;
	pSoldier->bTargetCubeLevel = SBeginFireWeapon->bTargetCubeLevel;
	pSoldier->usAttackingWeapon = SBeginFireWeapon->uiUniqueId; //cheap hack to pass wep id. 
					
	SendBeginFireWeaponEvent( pSoldier, SBeginFireWeapon->sTargetGridNo );
}



void send_hit(  EV_S_WEAPONHIT *SWeaponHit  )
{
	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"sendHIT" );
	//EV_S_WEAPONHIT* pWeaponHit =  (EV_S_WEAPONHIT*)pEventData;
	//SOLDIERTYPE *pSoldier = MercPtrs[ usSoldierID ];

	EV_S_WEAPONHIT weaphit_struct;				
			
	memcpy( &weaphit_struct , SWeaponHit, sizeof( EV_S_WEAPONHIT ));
	
	UINT16  usSoldierID=weaphit_struct.usSoldierID;

	if(SWeaponHit->usSoldierID < 20)weaphit_struct.usSoldierID = weaphit_struct.usSoldierID+ubID_prefix;
	if(SWeaponHit->ubAttackerID < 20)weaphit_struct.ubAttackerID = weaphit_struct.ubAttackerID+ubID_prefix;
		
	client->RPC("sendHIT",(const char*)&weaphit_struct, (int)sizeof(EV_S_WEAPONHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveHIT(RPCParameters *rpcParameters)
{
	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveHIT" );
		
	EV_S_WEAPONHIT* SWeaponHit = (EV_S_WEAPONHIT*)rpcParameters->input;
	
	SOLDIERTYPE *pSoldier = MercPtrs[ SWeaponHit->usSoldierID ];
	UINT16 usSoldierID;
	UINT8 ubAttackerID;

	if((SWeaponHit->usSoldierID >= ubID_prefix) && (SWeaponHit->usSoldierID < (ubID_prefix+6))) // within our netbTeam range...
		usSoldierID = (SWeaponHit->usSoldierID - ubID_prefix);
	else
		usSoldierID = SWeaponHit->usSoldierID;

	if((SWeaponHit->ubAttackerID >= ubID_prefix) && (SWeaponHit->ubAttackerID < (ubID_prefix+6)))
		ubAttackerID = (SWeaponHit->ubAttackerID - ubID_prefix);
	else
		ubAttackerID = SWeaponHit->ubAttackerID;

	WeaponHit( usSoldierID, SWeaponHit->usWeaponIndex, SWeaponHit->sDamage, SWeaponHit->sBreathLoss, SWeaponHit->usDirection, SWeaponHit->sXPos, SWeaponHit->sYPos, SWeaponHit->sZPos, SWeaponHit->sRange, ubAttackerID, SWeaponHit->fHit, SWeaponHit->ubSpecial, SWeaponHit->ubLocation );

	if(SWeaponHit->fStopped)
	{
		INT8 bTeam=pSoldier->bTeam;
		INT32 iBullet = bTable[bTeam][SWeaponHit->iBullet].local_id;
				
		RemoveBullet(iBullet);

		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"removed bullet" );	
	}		
}


void send_dismiss(UINT8 ubCurrentSoldierID)
{
	send_dismiss_struct sDismissMerc;

	sDismissMerc.ubProfileID = ubCurrentSoldierID + ubID_prefix;

	client->RPC("sendDISMISS",(const char*)&sDismissMerc, (int)sizeof(send_dismiss_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void send_hire( UINT8 iNewIndex, UINT8 ubCurrentSoldier, INT16 iTotalContractLength, BOOLEAN fCopyProfileItemsOver)
{
	send_hire_struct sHireMerc;

	sHireMerc.ubProfileID=ubCurrentSoldier;
	sHireMerc.team=TEAM;
	sHireMerc.fCopyProfileItemsOver=fCopyProfileItemsOver;
	sHireMerc.bTeam=netbTeam;

	SOLDIERTYPE *pSoldier = MercPtrs[ iNewIndex ];

	UINT8 sectorEdge = cStartingSectorEdge;
	
	// WANNE - MP: Center
	if (sectorEdge == MP_EDGE_CENTER)
		sectorEdge = INSERTION_CODE_CENTER;

	pSoldier->ubStrategicInsertionCode = sectorEdge;

	if(ubCurrentSoldier==64)//slay
	{
		pSoldier->ubBodyType = REGMALE;
		gMercProfiles[ pSoldier->ubProfile ].ubBodyType = REGMALE;
	}

	ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[44], pSoldier->name);

	AddCharacterToAnySquad( pSoldier );
	
	//add recruited flag
	gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags |= PROFILE_MISC_FLAG_RECRUITED;

	OBJECTTYPE		Object;
	int cnt;
	for (cnt=0; cnt<20;cnt++)
	{
		int item=kit[cnt];

		if(item != 0)
		{
			CreateItem( item, 100, &Object );
			AutoPlaceObject( pSoldier, &Object, TRUE );
		}
	}

	client->RPC("sendHIRE",(const char*)&sHireMerc, (int)sizeof(send_hire_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveDISMISS(RPCParameters *rpcParameters)
{
	send_dismiss_struct* sDismissMerc = (send_dismiss_struct*)rpcParameters->input;

	// Get soldier we should dismiss
	SOLDIERTYPE * pSoldier=MercPtrs[ sDismissMerc->ubProfileID ];

	TacticalRemoveSoldier( pSoldier->ubID );
}

void recieveHIRE(RPCParameters *rpcParameters)
{
	send_hire_struct* sHireMerc = (send_hire_struct*)rpcParameters->input;

	SOLDIERTYPE	*pSoldier;
	UINT8		iNewIndex;
	UINT8		ubCount=0;

	SOLDIERCREATE_STRUCT		MercCreateStruct;
	BOOLEAN fReturn = FALSE;
	
	MercCreateStruct.ubProfile						= sHireMerc->ubProfileID;
	MercCreateStruct.fPlayerMerc					= 0;
	MercCreateStruct.sSectorX							= gsMercArriveSectorX;
	MercCreateStruct.sSectorY							= gsMercArriveSectorY;
	MercCreateStruct.bSectorZ							= 0;
	MercCreateStruct.bTeam								= sHireMerc->bTeam;
	MercCreateStruct.fCopyProfileItemsOver			= sHireMerc->fCopyProfileItemsOver;

	TacticalCreateSoldier( &MercCreateStruct, &iNewIndex ) ;

	pSoldier = &Menptr[iNewIndex];
	pSoldier->flags.uiStatusFlags |= SOLDIER_PC;
	gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags |= PROFILE_MISC_FLAG_RECRUITED;
	
	if(!cSameMercAllowed)
		gMercProfiles[ pSoldier->ubProfile ].bMercStatus = MERC_WORKING_ELSEWHERE;

	pSoldier->bSide=0; //default coop only
	gTacticalStatus.Team[MercCreateStruct.bTeam	].bSide=0;

	if(MercCreateStruct.ubProfile==64)//slay
	{
		pSoldier->ubBodyType = REGMALE;
		gMercProfiles[ pSoldier->ubProfile ].ubBodyType = REGMALE;
	}	
	
	if(cGameType==MP_TYPE_DEATHMATCH)//all vs all only
	{
		pSoldier->bSide=1;
		gTacticalStatus.Team[MercCreateStruct.bTeam	].bSide=1;
		
	}
	if(cGameType==MP_TYPE_TEAMDEATMATCH) //allow teams
	{
		if(sHireMerc->team != TEAM)
		{
			pSoldier->bSide=1;
			gTacticalStatus.Team[MercCreateStruct.bTeam	].bSide=1;
		}
	}

	AddSoldierToSector( iNewIndex );

	if(cReportHiredMerc == 1)
	{	
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[5],MercCreateStruct.bTeam-5,client_names[MercCreateStruct.bTeam-6],pSoldier->name );
	}
	else 
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[6],MercCreateStruct.bTeam-5,client_names[MercCreateStruct.bTeam-6] );
	}
}

void send_gui_pos(SOLDIERTYPE *pSoldier,  FLOAT dNewXPos, FLOAT dNewYPos)
{
	gui_pos gnPOS;

	gnPOS.usSoldierID = (pSoldier->ubID)+ubID_prefix;
	
	gnPOS.dNewXPos = dNewXPos;
	gnPOS.dNewYPos = dNewYPos;

	client->RPC("sendguiPOS",(const char*)&gnPOS, (int)sizeof(gui_pos)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveguiPOS(RPCParameters *rpcParameters)
{
	gui_pos* gnPOS = (gui_pos*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ gnPOS->usSoldierID ];

	INT32 sNewGridNo;

	sNewGridNo = GETWORLDINDEXFROMWORLDCOORDS(gnPOS->dNewXPos, gnPOS->dNewYPos );
	pSoldier->usStrategicInsertionData=sNewGridNo;
	pSoldier->ubStrategicInsertionCode=INSERTION_CODE_GRIDNO;
	pSoldier->sInsertionGridNo = pSoldier->usStrategicInsertionData;

	pSoldier->EVENT_SetSoldierPosition( gnPOS->dNewXPos, gnPOS->dNewYPos );
}

void send_gui_dir(SOLDIERTYPE *pSoldier, UINT16	usNewDirection)
{	
	gui_dir gnDIR;

	gnDIR.usSoldierID = (pSoldier->ubID)+ubID_prefix;
	gnDIR.usNewDirection = usNewDirection;
	
	client->RPC("sendguiDIR",(const char*)&gnDIR, (int)sizeof(gui_dir)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveguiDIR(RPCParameters *rpcParameters)
{
	gui_dir* gnDIR = (gui_dir*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ gnDIR->usSoldierID ];
	
	pSoldier->EVENT_SetSoldierDirection( gnDIR->usNewDirection );
}


void send_EndTurn( UINT8 ubNextTeam )
{
	if(ubNextTeam==0)
	{
		// translate next team id for clients
		ubNextTeam=netbTeam;
	}
	
	turn_struct tStruct;

	if(is_server)
		Sawarded=false;

	tStruct.tsnetbTeam = netbTeam;
	tStruct.tsubNextTeam = ubNextTeam;
	
	client->RPC("sendEndTurn",(const char*)&tStruct, (int)sizeof(turn_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveEndTurn(RPCParameters *rpcParameters)
{
	turn_struct* tStruct = (turn_struct*)rpcParameters->input;
	UINT8 sender_bTeam;
	UINT8 ubTeam;
	sender_bTeam=tStruct->tsnetbTeam;
	ubTeam=tStruct->tsubNextTeam;
	
	if(is_server)
		Sawarded=false;
	
	// if the message was recieved from the server..
	if(is_server || sender_bTeam==6)
	{
		// if not the server and we're not in combat...
		if (!is_server && !(gTacticalStatus.uiFlags & INCOMBAT))
		{
			EnterCombatMode(0); 
		}

		if(ubTeam==netbTeam)ubTeam=0;
		{
			if(!is_server && is_client)
				EndTurnEvents();
		}

		if(!is_server && is_client)
			EndTurn( ubTeam );

		requested=false ;//request for realtime made or not
		BeginTeamTurn( ubTeam );
	}
}

UINT8 numenemyLAN( UINT8 ubSectorX, UINT8 ubSectorY )
{
	SOLDIERTYPE *pSoldier;
	UINT8				cnt ; //first posible lan player
	UINT8				ubNumEnemies = 0;

	for ( cnt=120 ; cnt <= 155; cnt++ )
	{
		pSoldier = MercPtrs[cnt];
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife > 0 )
		{
			if ( !pSoldier->aiData.bNeutral && (pSoldier->bSide != 0 ) )
			{
				ubNumEnemies++;
			}
		}	
	}

	return ubNumEnemies;
}

void send_AI( SOLDIERCREATE_STRUCT *pCreateStruct, UINT8 *pubID )
{
	AI_STRUCT send_inv;
	send_inv.standard_data = *pCreateStruct;

	for(int x=0;x<55;x++)
	{
		send_inv.slot[x] = pCreateStruct->Inv[x];
	}
	
	client->RPC("sendAI",(const char*)&send_inv, (int)sizeof(AI_STRUCT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveAI (RPCParameters *rpcParameters)
{
	UINT8 iNewIndex;
	SOLDIERTYPE *pSoldier;
	AI_STRUCT* send_inv = (AI_STRUCT*)rpcParameters->input;

	SOLDIERCREATE_STRUCT new_standard_data; //as originally my soldiercreate_struct would get its INV mangled through the RPC, so i send it packaged with it, then create a new struct and add in the bits that are still ok from the original struct, along with the packaged INV components...

	new_standard_data.bAgility = send_inv->standard_data.bAgility;
	new_standard_data.bAIMorale = send_inv->standard_data.bAIMorale;
	new_standard_data.bAttitude = send_inv->standard_data.bAttitude;
	new_standard_data.bBodyType = send_inv->standard_data.bBodyType;
	new_standard_data.bDexterity = send_inv->standard_data.bDexterity;
	new_standard_data.ubDirection = send_inv->standard_data.ubDirection;
	new_standard_data.bExpLevel = send_inv->standard_data.bExpLevel;
	new_standard_data.bExplosive = send_inv->standard_data.bExplosive;
	new_standard_data.bLeadership = send_inv->standard_data.bLeadership;
	new_standard_data.bLife = send_inv->standard_data.bLife;
	new_standard_data.bLifeMax = send_inv->standard_data.bLifeMax;
	new_standard_data.bMarksmanship = send_inv->standard_data.bMarksmanship;
	new_standard_data.bMechanical = send_inv->standard_data.bMechanical;
	new_standard_data.bMedical = send_inv->standard_data.bMedical;
	new_standard_data.bMorale = send_inv->standard_data.bMorale;
	new_standard_data.bOrders = send_inv->standard_data.bOrders;
	new_standard_data.bPatrolCnt = send_inv->standard_data.bPatrolCnt;
	new_standard_data.bSectorZ = send_inv->standard_data.bSectorZ;
	new_standard_data.bStrength = send_inv->standard_data.bStrength;
	new_standard_data.bTeam = send_inv->standard_data.bTeam;
	new_standard_data.bUseGivenVehicleID = send_inv->standard_data.bUseGivenVehicleID;
	new_standard_data.bWisdom = send_inv->standard_data.bWisdom;
	new_standard_data.endOfPOD = send_inv->standard_data.endOfPOD;
	new_standard_data.fCopyProfileItemsOver = send_inv->standard_data.fCopyProfileItemsOver;
	new_standard_data.fHasKeys = send_inv->standard_data.fHasKeys;
	new_standard_data.fKillSlotIfOwnerDies = send_inv->standard_data.fKillSlotIfOwnerDies;
	new_standard_data.fOnRoof = send_inv->standard_data.fOnRoof;
	new_standard_data.fPlayerMerc = send_inv->standard_data.fPlayerMerc;
	new_standard_data.fPlayerPlan = send_inv->standard_data.fPlayerPlan;
	new_standard_data.fStatic = send_inv->standard_data.fStatic;
	new_standard_data.fUseExistingSoldier = send_inv->standard_data.fUseExistingSoldier;
	new_standard_data.fUseGivenVehicle = send_inv->standard_data.fUseGivenVehicle;
	new_standard_data.fVisible = send_inv->standard_data.fVisible;
	memcpy( new_standard_data.HeadPal , send_inv->standard_data.HeadPal, sizeof( PaletteRepID ));
	memcpy( new_standard_data.MiscPal , send_inv->standard_data.MiscPal, sizeof( PaletteRepID ));
	memcpy( new_standard_data.name , send_inv->standard_data.name, sizeof( CHAR16 ) * 10 );
	memcpy( new_standard_data.PantsPal , send_inv->standard_data.PantsPal, sizeof( PaletteRepID ));
	new_standard_data.pExistingSoldier = send_inv->standard_data.pExistingSoldier;
	new_standard_data.sInsertionGridNo = send_inv->standard_data.sInsertionGridNo;
	memcpy( new_standard_data.SkinPal , send_inv->standard_data.SkinPal, sizeof( PaletteRepID ));
	memcpy( new_standard_data.sPatrolGrid, send_inv->standard_data.sPatrolGrid, sizeof( INT32 ) * MAXPATROLGRIDS );//dnl ch27 230909
	new_standard_data.sSectorX = send_inv->standard_data.sSectorX;
	new_standard_data.sSectorY = send_inv->standard_data.sSectorY;
	new_standard_data.ubCivilianGroup = send_inv->standard_data.ubCivilianGroup;
	new_standard_data.ubProfile = send_inv->standard_data.ubProfile;
	new_standard_data.ubScheduleID = send_inv->standard_data.ubScheduleID;
	new_standard_data.ubSoldierClass = send_inv->standard_data.ubSoldierClass;
	memcpy( new_standard_data.VestPal , send_inv->standard_data.VestPal, sizeof( PaletteRepID ));

	for(int x=0;x<55;x++)
	{
		if(send_inv->slot[x].usItem != 0)
			CreateItems( send_inv->slot[x].usItem, 100, send_inv->slot[x].ubNumberOfObjects, &new_standard_data.Inv[x] );
	}

	new_standard_data.fPlayerPlan=1;

	TacticalCreateSoldier( &new_standard_data, &iNewIndex );
	pSoldier = &Menptr[iNewIndex];
	pSoldier->flags.uiStatusFlags |= SOLDIER_PC;

	AddSoldierToSector( iNewIndex );
}

void send_ready ( void )
{
	ready_struct info;
	info.client_num = CLIENT_NUM;

	if(readystage==0)
	{
		info.ready_stage = 0;
		if(status==0)
		{
			info.status = 1; 
			status=1;
			numready = numready+1;
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[7],numready,cMaxClients );
			// OJW - 20081204
			client_ready[info.client_num-1]=1;
			fDrawCharacterList = true; // set the character list to be redrawn
		}
		else
		{
			info.status = 0; 
			status=0;
			numready = numready-1;
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[8],numready,cMaxClients );
			// OJW - 20081204
			client_ready[info.client_num-1]=0;
			fDrawCharacterList = true; // set the character list to be redrawn
		}	
	}
			
	if(is_server && numready == cMaxClients) //all ready. and server tells all to load...
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[9] );

		goahead=1;
		readystage=1;

		info.ready_stage = 1;
		info.status = 1; 					
	}	
			
	client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

	if(is_server && numready == cMaxClients)
	{
		status=0;//reset
		numready=0;
		start_battle();//server loads
	}
}

void recieveREADY (RPCParameters *rpcParameters)
{
	ready_struct* info = (ready_struct*)rpcParameters->input;

	if(info->ready_stage==1)//recived ok for go ahead from server for level load
	{
		numready++;
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[10], info->client_num,client_names[info->client_num-1],numready,cMaxClients );
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[9] );
		status=0;//reset
		numready=0;
		goahead=1;
		start_battle();
	}
	else if (info->ready_stage != 36) // recieved status update from client
	{
		if (info->status==1)
		{
			numready = numready+1;
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[10], info->client_num,client_names[info->client_num-1],numready,cMaxClients );
			// OJW - 20081204
			client_ready[info->client_num-1]=1;
			fDrawCharacterList = true; // set the character list to be redrawn
		}
		else
		{
			numready = numready-1;
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[11], info->client_num,client_names[info->client_num-1],numready,cMaxClients );
			// OJW - 20081204
			client_ready[info->client_num-1]=0;
			fDrawCharacterList = true; // set the character list to be redrawn
		}
		
		if(is_server && numready == cMaxClients) //all ready. and server tells all to load...and loads himself
		{
			goahead=1;
			readystage=1;
			send_ready();
			start_battle();
		}
	}
	else if(info->ready_stage==36)//server allows laptop access
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[36] );
		allowlaptop=1;
		if (gRandomMercs)
			HireRandomMercs();

		// server is starting the game, adjust the games max players
		// this will allow the game to be started, rather than saying 2/4 players are connected
		// where there are only two players connected
		int iPlayersConnected = 0;
		for (int i=0; i< 4; i++)
			if (client_names[i] != NULL && strcmp(client_names[i],"") != 0)
				iPlayersConnected++;

		cMaxClients = iPlayersConnected;
	}
}

void send_loaded (void)
{
	ready_struct info;
	info.client_num = CLIENT_NUM;
	info.ready_stage = 1;//done loading level
	info.status=1;

	numready++;
	if(numready==cMaxClients && is_server)
	{
		lockui(1);//unlock ui
		readystage=0;
		numready=0;

		info.ready_stage = 2;//done placing mercs
		info.status=1;
	}

	client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void send_donegui ( UINT8 ubResult )
{
	if(ubResult==1 && status==0)
		return;//avoid double remove callback response from final message box removal
		
	ready_struct info;
	info.client_num = CLIENT_NUM;
	
	if(ubResult==1)
		DialogRemoved(1);//cleanup msgbox after not ready
	
	if(status==0)//now ready
	{
		status=1;
		numready++;
		info.ready_stage = 3;//done placing mercs
		info.status=1;
		
		SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
		DoMessageBox( MSG_BOX_BASIC_STYLE, MPClientMessage[12],  guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT, send_donegui,  &CenterRect );

		if(numready==cMaxClients && is_server)//all done
		{
			numready=0;
			status=0;
			info.ready_stage = 4;//done placing mercs
			info.status=1;
			gMsgBox.bHandled = MSG_BOX_RETURN_OK;
			KillTacticalPlacementGUI(); //send and kill
			ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPClientMessage[13]);
		}
	}
	else if(status==1)//was ready
	{
		status=0;
		numready--;
		info.ready_stage = 3;//not done placing mercs
		info.status=0;
	}

	client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveGUI (RPCParameters *rpcParameters)
{
	ready_struct* info = (ready_struct*)rpcParameters->input;

	if(info->ready_stage==1 && info->status==1)
	{
		numready++;
		if(numready==cMaxClients && is_server)
		{
			lockui(1);//unlock ui
			readystage=0;
			numready=0;

			ready_struct info;
			info.client_num = CLIENT_NUM;
			info.ready_stage = 2;//done placing mercs
			info.status=1;

			client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}

	if(info->ready_stage==2 && info->status ==1)
	{
		lockui(1);//unlock ui
		readystage=0;
		numready=0;
	}

	if(info->ready_stage==3 && info->status==1)//recieved client done placement
	{
		numready++;
		if(numready==cMaxClients && is_server)//all done
		{
			numready=0;

			ready_struct info;
			info.client_num = CLIENT_NUM;
			info.ready_stage = 4;//all done placing mercs, kill all
			info.status=1;
			gMsgBox.bHandled = MSG_BOX_RETURN_OK;
			status=0;
			KillTacticalPlacementGUI();
			ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPClientMessage[13]);

			client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}

	if(info->ready_stage==3 && info->status==0)//recieved client retracted place ready...
	{
		numready--;
	}

	if(info->ready_stage==4 && info->status==1)
	{
		gMsgBox.bHandled = MSG_BOX_RETURN_OK;
		KillTacticalPlacementGUI();
		ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPClientMessage[13]);
		numready=0;
		status=0;
	}
}

void allowlaptop_callback ( UINT8 ubResult )
{
	if(ubResult==2)
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[36] );
		allowlaptop=1;

		ready_struct info;
		info.client_num = CLIENT_NUM;
		info.ready_stage=36;
		info.status=1;

		if (gRandomMercs)
			HireRandomMercs();

		// server client is starting the game, adjust the games max players
		// this will allow the game to be started, rather than saying 2/4 players are connected
		// where there are only two players connected
		int iPlayersConnected = 0;
		for (int i=0; i< 4; i++)
			if (client_names[i] != NULL && strcmp(client_names[i],"") != 0)
				iPlayersConnected++;

		cMaxClients = iPlayersConnected;

		client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void StartBattleChatBoxClosedCallback(void)
{
	// The chat box has been closed, now we can start
	SetCurrentWorldSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ );
	is_game_started = true;
}

void start_battle ( void )
{ 
	if(!is_client)
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[14] );
	}
	else if(!allowlaptop && is_server)
	{
		bool numPlayersValid = TRUE;
		bool clientsFinishedDownloading = TRUE;
		bool teamsValid = TRUE;

		// check that another player is actually connected
		int iPlayersConnected = 0;
		for (int i=0; i< 4; i++)
			if (client_names[i] != NULL && strcmp(client_names[i],"") != 0)
				iPlayersConnected++;

		if (iPlayersConnected <= 1)
		{
			numPlayersValid = FALSE;

			// notify the server that at least one other player must be connected
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[51] );
		}
		else if (are_clients_downloading())
		{
			clientsFinishedDownloading = FALSE;

			// notify the server that some of the clients are still downloading
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[63] );
		}
		// WANNE - MP: If choosen team deathmatch, there must be at least 2 different teams
		else if (cGameType == MP_TYPE_TEAMDEATMATCH)
		{
			bool areTeamsValid = FALSE;
			int clientTeam = client_teams[0];

			for (int i = 1; i < iPlayersConnected; i++)
			{
				if (clientTeam != client_teams[i])
				{
					areTeamsValid = TRUE;
					break;
				}
			}

			if (!areTeamsValid)
			{
				teamsValid = FALSE;

				// notify the server that the teams are not different for the choosen team-deathmatch
				ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[68] );
			}	
		}

		// Go to "ready" state!
		if (numPlayersValid && clientsFinishedDownloading && teamsValid)
		{
			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, MPClientMessage[35],  guiCurrentScreen, MSG_BOX_FLAG_YESNO | MSG_BOX_FLAG_USE_CENTERING_RECT, allowlaptop_callback,  &CenterRect );
		}
	}
	else if(allowlaptop)
	{
		if ( NumberOfMercsOnPlayerTeam() ==0)
		{
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[15] );
		}
		else if(goahead==1)
		{	
			goahead=0; // this ensures that we dont reload the sector again the next time this function is called

			if (is_game_started)
				return; // 20090317 - OJW - there is a bug or out of sync condition that rarely somestimes causes a client to reach here
						// twice, and reload the sector, which causes an assertion failure on line 1782 of strategicmap.cpp in SetCurrentWorldSector()
						// therefore i'm adding this to make sure if they client loaded the map once, it wont do it again
			
			status=0;//reset
			numready=0;

			// Find first active soldier. This is needed, because if the soldier is dismissed it does not belong to any group
			SOLDIERTYPE *pSoldier;
			for (int i = 0; i <= 19; i++)
			{
				if (MercPtrs[ i ]->bActive)
				{
					pSoldier = MercPtrs[i];
					break;
				}
			}

			//SOLDIERTYPE *pSoldier = MercPtrs[ 0 ];
			UINT8 ubGroupID = pSoldier->ubGroupID;

			GROUP *pGroup;
			pGroup = GetGroup( ubGroupID ); 
			gpBattleGroup = pGroup;
			gubPBSectorX = gpBattleGroup->ubSectorX;
			gubPBSectorY = gpBattleGroup->ubSectorY;
			gubPBSectorZ = gpBattleGroup->ubSectorZ;

			gfEnterTacticalPlacementGUI = 1;
			// OJW - 20090205
			iTeamsWiped = 0; // reset the number of wiped teams to Zero

			UINT32	i;
			for(i=0; i<4;i++)
			{	
				CHAR16 name[30];
				int nm = mbstowcs( name, client_names[i], sizeof (char)*30 );
				//copy in client specified name for the player turn bar :)
				if(nm)
				{
					// OJW - 20090318 - fixed name copying bug with multiple games
					CHAR16 full[255];
					memset(full,0,sizeof(CHAR16)*255);
					swprintf(full, MPClientMessage[57],i+1,name);

					memcpy( TeamTurnString[ (i+6) ] , full, sizeof( CHAR16) * 255 );
				}
			}

			// this closes the chat window if its open and the game is starting
			// if this is open then the client will crash when the screen returns
			if (guiCurrentScreen == MP_CHAT_SCREEN)
			{
				gChatBox.bHandled = MSG_BOX_RETURN_NO;
				bClosingChatBoxToStartGame = true;
				iCCStartGameTime = guiBaseJA2NoPauseClock+1000;
			}
			else
			{
				SetCurrentWorldSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ );
				is_game_started = true;
			}
		}
		else
		{
			send_ready();
		}
	}
	else if(!allowlaptop && is_client && !is_server)
	{
	   ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[16] );
	}
}

void DropOffItemsInSector( UINT8 ubOrderNum )
{
	BOOLEAN	fSectorLoaded = FALSE;
	OBJECTTYPE		Object;
	UINT32	uiCount = 0;
	OBJECTTYPE	*pObject=NULL;
	UINT16	usNumberOfItems=0, usItem;
	UINT8		ubItemsDelivered, ubTempNumItems;
	UINT32	i;

	// determine if the sector is loaded
	if( ( gWorldSectorX == gsMercArriveSectorX ) && ( gWorldSectorY == gsMercArriveSectorY ) && ( gbWorldSectorZ == 0 ) )
		fSectorLoaded = TRUE;
	else
		fSectorLoaded = FALSE;

	SetSectorFlag( gsMercArriveSectorX, gsMercArriveSectorY, 0, SF_ALREADY_VISITED);//allows update of item count

	for(i=0; i<gpNewBobbyrShipments[ ubOrderNum ].ubNumberPurchases; i++)
	{
		// Count how many items were purchased
		usNumberOfItems += gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].ubNumberPurchased;
	}

	//if we are NOT currently in the right sector
	if( !fSectorLoaded )
	{
		//build an array of objects to be added
		pObject = new OBJECTTYPE[ usNumberOfItems ];
		if( pObject == NULL )
			return;
	}

	uiCount = 0;

	//loop through the number of purchases
	for( i=0; i< gpNewBobbyrShipments->ubNumberPurchases; i++)
	{
		ubItemsDelivered = gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].ubNumberPurchased;
		usItem = gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].usItemIndex;

		while ( ubItemsDelivered )
		{
			// treat 0s as 1s :-)
			ubTempNumItems = __min( ubItemsDelivered, __max( 1, Item[ usItem ].ubPerPocket ) );
			CreateItems( usItem, gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].bItemQuality, ubTempNumItems, &Object );

			// stack as many as possible
			if( fSectorLoaded )
			{
				AddItemToPool( 12880, &Object, 1, 0, WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE, 0 );
			}
			else
			{
				pObject[ uiCount ] = Object;
				uiCount++;
			}

			ubItemsDelivered -= ubTempNumItems;
		}
	}

	//if the sector WASNT loaded
	if( !fSectorLoaded )
	{
		//add all the items from the array that was built above

		//The item are to be added to the Top part of Drassen, grid loc's  10112, 9950
		if( !AddItemsToUnLoadedSector( gsMercArriveSectorX, gsMercArriveSectorY, 0, 12880, uiCount, pObject, 0, WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE, 0, 1, FALSE ) )
		{
			//error
			Assert( 0 );
		}
		delete[] pObject ;
		pObject = NULL;
	}

	//mark that the shipment has arrived
	gpNewBobbyrShipments[ ubOrderNum ].fActive = FALSE;
}

void send_stop (EV_S_STOP_MERC *SStopMerc) // used to stop a merc when he spots new enemies...
{
	EV_S_STOP_MERC stop_struct;
	if(SStopMerc->usSoldierID < 120)
	{
		if(SStopMerc->usSoldierID < 20)
		{
			stop_struct.usSoldierID = (SStopMerc->usSoldierID)+ubID_prefix;
		}
		else
			stop_struct.usSoldierID = SStopMerc->usSoldierID;

		stop_struct.sGridNo=SStopMerc->sGridNo;
		stop_struct.ubDirection=SStopMerc->ubDirection;
		stop_struct.fset=SStopMerc->fset;
		
		if(stop_struct.fset==FALSE)
		{
			return;
		}

		stop_struct.sXPos=SStopMerc->sXPos;
		stop_struct.sYPos=SStopMerc->sYPos;
		client->RPC("sendSTOP",(const char*)&stop_struct, (int)sizeof(EV_S_STOP_MERC)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieveSTOP (RPCParameters *rpcParameters)
{
	EV_S_STOP_MERC* SStopMerc =(EV_S_STOP_MERC*)rpcParameters->input;
	
	SOLDIERTYPE *pSoldier = MercPtrs[ SStopMerc->usSoldierID ];
		
	pSoldier->EVENT_InternalSetSoldierPosition( SStopMerc->sXPos, SStopMerc->sYPos,FALSE, FALSE, FALSE );
	pSoldier->EVENT_SetSoldierDirection( SStopMerc->ubDirection );
	pSoldier->AdjustNoAPToFinishMove( SStopMerc->fset );
	pSoldier->flags.bTurningFromPronePosition = FALSE;	
}

void send_interrupt (SOLDIERTYPE *pSoldier)
{
	INT_STRUCT INT;

	INT.ubID = pSoldier->ubID;
	INT.bTeam = pSoldier->bTeam;
	memcpy(INT.gubOutOfTurnOrder, gubOutOfTurnOrder, sizeof(UINT8) * MAXMERCS);
	INT.gubOutOfTurnPersons = gubOutOfTurnPersons;
	
	INT.Interrupted=gusSelectedSoldier+ubID_prefix;

	if(INT.bTeam==0)
	{
		INT.bTeam=netbTeam;
		INT.ubID=INT.ubID+ubID_prefix;
	}

	for(int i=0; i <= INT.gubOutOfTurnPersons; i++)
	{
		if(INT.gubOutOfTurnOrder[i] < 20)
		{
			INT.gubOutOfTurnOrder[i]=INT.gubOutOfTurnOrder[i]+ubID_prefix;
		}
	}
	
	if(INT.bTeam !=netbTeam) 
		gTacticalStatus.ubCurrentTeam=INT.bTeam;
	
	client->RPC("sendINTERRUPT",(const char*)&INT, (int)sizeof(INT_STRUCT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

// WANNE - MP: Here we have to add AddTopMessage() on the clients
void recieveINTERRUPT (RPCParameters *rpcParameters)
{
	INT_STRUCT* INT = (INT_STRUCT*)rpcParameters->input;
	SOLDIERTYPE* pOpponent = MercPtrs[ INT->Interrupted];

	if(INT->bTeam==netbTeam)//for us or AI
	{
		INT->bTeam=0;
		INT->ubID=INT->ubID - ubID_prefix;

		for(int i=0; i <= INT->gubOutOfTurnPersons; i++)
		{
			if((INT->gubOutOfTurnOrder[i] >= ubID_prefix) && (INT->gubOutOfTurnOrder[i] < (ubID_prefix+6)))
			{
				INT->gubOutOfTurnOrder[i]=INT->gubOutOfTurnOrder[i]-ubID_prefix;
			}
		}
		memcpy(gubOutOfTurnOrder,INT->gubOutOfTurnOrder, sizeof(UINT8) * MAXMERCS);
		gubOutOfTurnPersons = INT->gubOutOfTurnPersons;

		// AI has interrupted
		if (INT->bTeam == 1)
		{
			//AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, TeamTurnString[ INT->bTeam ] );
		}
		else
		{
			AddTopMessage( PLAYER_INTERRUPT_MESSAGE, TeamTurnString[ INT->bTeam ] );
		}
	}

	// WANNE - MP: This seems to cause the HANG on AI interrupt where we have to press ALT + E on the server!
	if(	INT->bTeam != 0)
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[17] );
		//stop moving merc who was interrupted and init UI bar
		SOLDIERTYPE* pMerc = MercPtrs[ INT->ubID ];	
		pMerc->HaultSoldierFromSighting(TRUE);
		FreezeInterfaceForEnemyTurn();
		InitEnemyUIBar( 0, 0 );
		fInterfacePanelDirty = DIRTYLEVEL2;
		gTacticalStatus.fInterruptOccurred = TRUE;
	}
	else
	{
		//it for us ! :)
		if(INT->gubOutOfTurnPersons==0)//indicates finished interrupt maybe can just call end interrupt
		{
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"old int finish" );
		}
		else //start our interrupt turn
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[37] );

			SOLDIERTYPE* pSoldier = MercPtrs[ INT->ubID ];
			ManSeesMan(pSoldier,pOpponent,pOpponent->sGridNo,pOpponent->pathing.bLevel,2,1);
			StartInterrupt();
		}
	}
}

void intAI (SOLDIERTYPE *pSoldier )
{
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[17] );
	AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, TeamTurnString[ pSoldier->bTeam ] );
	gTacticalStatus.fInterruptOccurred = TRUE;		
}

void end_interrupt ( BOOLEAN fMarkInterruptOccurred )
{
	SOLDIERTYPE * pSoldier = MercPtrs[(gubOutOfTurnOrder[gubOutOfTurnPersons])];

	INT_STRUCT INT;
	INT.ubID = pSoldier->ubID;
	INT.bTeam = pSoldier->bTeam;
	memcpy(INT.gubOutOfTurnOrder, gubOutOfTurnOrder, sizeof(UINT8) * MAXMERCS);
	INT.gubOutOfTurnPersons = gubOutOfTurnPersons;
	INT.fMarkInterruptOccurred=fMarkInterruptOccurred;
	if(is_server)Sawarded=false;

	if(INT.bTeam==0)
	{
		INT.bTeam=netbTeam;
		INT.ubID=INT.ubID+ubID_prefix;
	}

	for(int i=0; i <= INT.gubOutOfTurnPersons; i++)
	{
		if(INT.gubOutOfTurnOrder[i] < 20)
		{
			INT.gubOutOfTurnOrder[i]=INT.gubOutOfTurnOrder[i]+ubID_prefix;
		}
	}
	
	client->RPC("endINTERRUPT",(const char*)&INT, (int)sizeof(INT_STRUCT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void resume_turn(RPCParameters *rpcParameters)
{
	INT_STRUCT* INT = (INT_STRUCT*)rpcParameters->input;

	if(is_server)
		Sawarded=false;
	
	if(INT->bTeam==netbTeam || (INT->bTeam==1 && is_server))//may need working
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[18] );

		for(int i=0; i <= INT->gubOutOfTurnPersons; i++)
		{
			if((INT->gubOutOfTurnOrder[i] >= ubID_prefix) && (INT->gubOutOfTurnOrder[i] < (ubID_prefix+6)))
			{
				INT->gubOutOfTurnOrder[i]=INT->gubOutOfTurnOrder[i]-ubID_prefix;
			}
		}

		memcpy(gubOutOfTurnOrder,INT->gubOutOfTurnOrder, sizeof(UINT8) * MAXMERCS);
		gubOutOfTurnPersons = INT->gubOutOfTurnPersons;
		EndInterrupt( INT->fMarkInterruptOccurred );
	}
	// WANNE - MP: This happens, when client 1 (=server) has done its interrupt and now it is enemies turn!
	else if(INT->bTeam==1)
	{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"im not server but ai just got back its turn after being interrupted..." );
		AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ 1 ] );
	}
}

void grid_display ( void ) //print mouse coordinates, helpfull for crate placement.
{
	INT16	sGridX, sGridY;
	UINT32 usMapPos;

	GetMouseXY( &sGridX, &sGridY );
	usMapPos = MAPROWCOLTOPOS( sGridY, sGridX );
	
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[19] );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[20], sGridX, sGridY );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[21], usMapPos );
}

void overide_callback( UINT8 ubResult )
{
	if(is_server)
	{
		if(ubResult==1)
		{
			allowlaptop=true;
		}

		if(ubResult==2)//overide stop #1 (awaiting client ready for launch/load)
		{	
			goahead=0;
			status=0;//reset
			numready=0;
			SOLDIERTYPE *pSoldier = MercPtrs[ 0 ];
			UINT8 ubGroupID = pSoldier->ubGroupID;

			GROUP *pGroup;
			pGroup = GetGroup( ubGroupID ); 
			gpBattleGroup = pGroup;
			gubPBSectorX = gpBattleGroup->ubSectorX;
			gubPBSectorY = gpBattleGroup->ubSectorY;
			gubPBSectorZ = gpBattleGroup->ubSectorZ;

			gfEnterTacticalPlacementGUI = 1;
			goahead=1;
			readystage=1;
			ready_struct info; //send
			info.client_num = CLIENT_NUM;
			info.ready_stage = 1;
			info.status = 1; 		
			
			client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	
			status=0;//reset
			numready=0;
		
			SetCurrentWorldSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ ); //load
		}
		
		if(ubResult==3)
		{
			lockui(1);//unlock ui paused while wainting for loaders

			ready_struct info;
			info.client_num = CLIENT_NUM;
			readystage=0;
			numready=0;
			info.ready_stage = 2;//done placing mercs
			info.status=1;
			client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}

		if(ubResult==4) //overide waiting on merc placement
		{
			numready=0;

			ready_struct info; //send
			info.client_num = CLIENT_NUM;
			info.ready_stage = 4;//all done placing mercs, kill all
			info.status=1;
			gMsgBox.bHandled = MSG_BOX_RETURN_OK;
			status=0;
			KillTacticalPlacementGUI(); //kill
			ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPClientMessage[13]);

			client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	
		goahead = 0;
		numready = 0;
		readystage = 0;
		status = 0;
	}
}

void requestFILE_TRANSFER_SETTINGS(void)
{
	client->RPC("requestFILE_TRANSFER_SETTINGS","", 0, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

// OJW - 20090430
// give clients a choice to accept the security risk
void allowDownloadCallback( UINT8 ubResult )
{
	if (ubResult == 2)
	{
		// yes
		// begin downloading of files
		setID = fltClient.SetupReceive(&transferCBClient, false, serverAddr);

		char buffer[3];
		sprintf(buffer, "%i", setID);

		client->RPC("receiveSETID", (const char*) buffer, (int)sizeof(char*), HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
	else
	{
		// no
		// gracefully disconnect to the main menu
		client_disconnect();
		guiPendingScreen = MP_JOIN_SCREEN;
	}
}

// THIS METHOD IS CALLED FROM THE SERVER WHENEVER A NEW CLIENT CONNECTS
void requestSETID(RPCParameters *rpcParameters)
{
	// WANNE: FILE TRANSFER
	if (!is_server)
	{
		if (is_connected) // added this here for version disconnections
		{
			// We did not recieved the files, get them!
			if (!fClientReceivedAllFiles)
			{
				serverAddr = rpcParameters->sender;
				
				SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };
				DoMessageBox( MSG_BOX_BASIC_STYLE , MPClientMessage[64] , guiCurrentScreen, MSG_BOX_FLAG_YESNO | MSG_BOX_FLAG_USE_CENTERING_RECT, allowDownloadCallback, &CenteringRect );
			}
		}
	}
}

void requestSETTINGS(void)
{
	client_info cl_name;
	strcpy(cl_name.client_name , cClientName);
	cl_name.team = TEAM;
	cl_name.cl_edge = cStartingSectorEdge;

	// OJW - 20090507
	// send client version to server
	strcpy(cl_name.client_version,MPVERSION);
	client->RPC("requestSETTINGS",(const char*)&cl_name, (int)sizeof(client_info)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

// OJW: FILE TRANSFER: Clients get notified of other clients transfer progress
void recieveDOWNLOADSTATUS(RPCParameters *rpcParameters)
{
	progress_struct* prog = (progress_struct*)rpcParameters->input;
	int i = prog->client_num - 1;

	if (client_downloading[i] != prog->downloading)
	{
		if (prog->downloading == 0)
			ScreenMsg( FONT_RED, MSG_MPSYSTEM, (is_server? MPServerMessage[11] : MPClientMessage[61]), client_names[i]); // message client has recieved all files
		else
			ScreenMsg( FONT_RED, MSG_MPSYSTEM, (is_server? MPServerMessage[12] : MPClientMessage[62]), client_names[i]);// send message client has started downloading files
	}

	client_downloading[i] = prog->downloading;
	client_progress[i] = prog->progress;
	
	fDrawCharacterList = true;
}

// WANNE: FILE TRANSFER: Get executable Directory from Server. This is used to get corret file location on client side
void recieveFILE_TRANSFER_SETTINGS (RPCParameters *rpcParameters)
{
	if (!is_server && recieved_transfer_settings == 0)
	{		
		filetransfersettings_struct* fts = (filetransfersettings_struct*)rpcParameters->input;

		gCurrentTransferBytes = 0;
		gTotalTransferBytes = fts->totalTransferBytes;

		// Now get directory
		strcpy( server_fileTransferDirectoryPath, fts->fileTransferDirectory );
		vfs::Path profileRoot = vfs::Path(gzFileTransferDirectory) + vfs::Path(server_fileTransferDirectoryPath);

		/////////////////////////////////////////////////////////////////////
		SGP_TRYCATCH_RETHROW( ja2::mp::InitializeMultiplayerProfile(profileRoot), L"" );
		/////////////////////////////////////////////////////////////////////

		recieved_transfer_settings = 1;
		serverSyncClientsDirectory = fts->syncClientsDirectory;

		// We sync our MP game dir from the server
		if (fts->syncClientsDirectory == 1)
		{
			// profile is setup, nothing to do here
		}
	}
}

void recieveSETTINGS (RPCParameters *rpcParameters) //recive settings from server
{
	int startingEdge = MP_EDGE_NORTH;

	settings_struct* cl_lan = (settings_struct*)rpcParameters->input;

	char szDefault[30];
	sprintf(szDefault, "%s",cl_lan->client_name);

	// OJW - 20081204
	// get complete client data from the server
	memcpy( client_edges, cl_lan->client_edges , sizeof(int) * 5);
	memcpy( client_teams, cl_lan->client_teams , sizeof(int) * 4);

	if(!recieved_settings && strcmp(cl_lan->client_name, cClientName)==0)
	{
		// This settings packet contains information and settings specifically for us
		recieved_settings=1;

		// save the settings so if we need to re-apply them after a reinitialisation ( after file transfer ) we can
		memcpy ( &gMPServerSettings , cl_lan , sizeof(settings_struct) );

		CLIENT_NUM=cl_lan->client_num;//assign client number from server

		netbTeam = (CLIENT_NUM)+5;
		ubID_prefix = gTacticalStatus.Team[ netbTeam ].bFirstID;//over here now

		memcpy( client_names , cl_lan->client_names, sizeof( char ) * 4 * 30 );
		
		strcpy(client_names[cl_lan->client_num-1],szDefault);

		// OJW - 20081204
		strcpy(cServerName,cl_lan->server_name);
		gRandomMercs = cl_lan->randomMercs;
		gRandomStartingEdge = cl_lan->randomStartingEdge;

		cStartingSectorEdge = cl_lan->startingSectorEdge;

		if(gRandomMercs)
		{
			// copy the random merc list locally
			memcpy(random_mercs,cl_lan->random_mercs,sizeof(int) * 7);
		}

		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[2] );
		

		cMaxClients=cl_lan->maxClients;
		memcpy( cKitBag, cl_lan->kitBag, sizeof(char)*100);
		
		int cnt;
		int numnum = 0;
		int kitnum = 0;
		char tempstring[4];
		memset( &kit, 0, sizeof( int )*20 );
	
		if (strcmp(cKitBag, "") != 0)
		{
			for(cnt=0;  cnt < 100; cnt++)
			{
				char tempc = cKitBag[cnt];
				
				if( _strnicmp(&tempc, "[",1) == 0)
				{
					
					continue;
				}
				
				else if( _strnicmp(&tempc, ",",1) == 0)
				{
					numnum=0;

					kit[kitnum]=atoi(tempstring);

					memset( &tempstring, 0, sizeof( char )*4 );

					kitnum++;
					continue;
				}
			
				else if( _strnicmp(&tempc, "]",1) == 0)
				{
					kit[kitnum]=atoi(tempstring);
					break;
				}
				else
				{
				strncpy(&tempstring[numnum],&tempc,1);
				numnum++;
				}
			}
		}

		cDamageMultiplier=cl_lan->damageMultiplier;
		cSameMercAllowed=cl_lan->sameMercAllowed;
		
		gsMercArriveSectorX=cl_lan->gsMercArriveSectorX;
		gsMercArriveSectorY=cl_lan->gsMercArriveSectorY;

		// WANNE - BMP: We have to initialize the map size here!!
		InitializeWorldSize(gsMercArriveSectorX, gsMercArriveSectorY, 0);

		cGameType = cl_lan->gameType;
		cDisableMorale=cl_lan->disableMorale;
		ChangeSelectedMapSector( gsMercArriveSectorX, gsMercArriveSectorY, 0 );
		CHAR16 str[128];
		GetSectorIDString( gsMercArriveSectorX, gsMercArriveSectorY, 0, str, TRUE );
	
		// WANNE - MP: If time set to 0, then no time turns
		INT32 secs_per_tick;
		if (cl_lan->secondsPerTick == 0)
		{
			gGameOptions.fTurnTimeLimit=FALSE;
			secs_per_tick=1;
		}
		else
		{
			gGameOptions.fTurnTimeLimit=cl_lan->sofTurnTimeLimit;
			secs_per_tick=cl_lan->secondsPerTick;
		}
		
		PLAYER_TEAM_TIMER_SEC_PER_TICKS=secs_per_tick;

		INT32 clstarting_balance=cl_lan->startingCash;//set starting balance

		if(LaptopSaveInfo.iCurrentBalance<clstarting_balance)
		{
			AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), clstarting_balance-LaptopSaveInfo.iCurrentBalance );
		}
		else
		{
			AddTransactionToPlayersBook( TRANSACTION_FEE, 0, GetWorldTotalMin(), clstarting_balance-LaptopSaveInfo.iCurrentBalance );
		}

		gGameOptions.fGunNut=cl_lan->sofGunNut;	
		gGameOptions.ubGameStyle=cl_lan->soubGameStyle;
		gGameOptions.ubDifficultyLevel=cl_lan->soubDifficultyLevel;
		gGameOptions.fNewTraitSystem = cl_lan->soubSkillTraits;
		gGameOptions.fIronManMode=cl_lan->sofIronManMode;
		gGameOptions.ubBobbyRay=cl_lan->soubBobbyRay;

		// Set Bobby Ray "Under Construction"?
		if(!cl_lan->disableBobbyRay)
		{
			SetBookMark( BOBBYR_BOOKMARK );
			LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;
		}

		// Enable "AIM" and "MERC" only if random merc is false!
		if (!cl_lan->randomMercs)
		{
			SetBookMark( AIM_BOOKMARK );
			SetBookMark( MERC_BOOKMARK );
		}

		if(!cl_lan->disableMercEquipment)
			cAllowMercEquipment = 1;

		cMaxMercs = cl_lan->maxMercs;		
		cReportHiredMerc = cl_lan->reportHiredMerc;

		// WANNE: Removed
		/*
		ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, MPClientMessage[24],str,cMaxClients,cMaxMercs,cGameType,cSameMercAllowed,cDamageMultiplier,gGameOptions.fTurnTimeLimit,secs_per_tick,cl_lan->disableBobbyRay,cl_lan->disableMercEquipment,cDisableMorale,0);		
		*/
	
		// WANNE - MP: I just added the NUM_SEC_IN_DAY so the game starts at Day 1 instead of Day 0
		gGameExternalOptions.iGameStartingTime= NUM_SEC_IN_DAY + int(cl_lan->startingTime*3600);
		// WANNE - MP: In mulitplayer the hired merc arrive immediatly, so iFirstArrivalDelay must be set to 0
		// This also fixed the bug of the wrong hired hours!
		gGameExternalOptions.iFirstArrivalDelay = 0;

		// Disable Reinforcements
		gGameExternalOptions.gfAllowReinforcements				= false;
		gGameExternalOptions.gfAllowReinforcementsOnlyInCity	= false;

		// WANNE: The new improved interrupt system (IIS) does not work with multiplayer, so disable it
		gGameExternalOptions.fImprovedInterruptSystem			= false;

		// Disable Real-Time Mode
		// SANDRO - huh? real-time sneak is in preferences
		//gGameExternalOptions.fAllowRealTimeSneak = false;
		
		gGameSettings.fOptions[TOPTION_ALLOW_REAL_TIME_SNEAK] = false;
		gGameExternalOptions.fQuietRealTimeSneak = false;

				// WANNE: Take the settings from the ja2_options.ini
		// WANNE: Enable fast loading
		/*gGameExternalOptions.fDisableLaptopTransition = true;
		gGameExternalOptions.fFastWWWSitesLoading = true;
		gGameExternalOptions.fDisableStrategicTransition = true;*/

		// WANNE: fix HOT DAY in night at arrival by night.
		// Explanation: If game starting time + first arrival delay < 07:00 (111600) -> we arrive before the sun rises or
		// if game starting time + first arrival delay >= 21:00 (162000) -> we arrive after the sun goes down
		if( (gGameExternalOptions.iGameStartingTime + gGameExternalOptions.	iFirstArrivalDelay) < 111600 ||
			(gGameExternalOptions.iGameStartingTime + gGameExternalOptions.iFirstArrivalDelay >= 162000))
		{ 
			// Default: Night
			gubEnvLightValue = 12; 
		}
		else
		{
			// Default: Day
			gubEnvLightValue = 3;
		}

		LightSetBaseLevel(gubEnvLightValue); 

		InitNewGameClock( );
					
		cWeaponReadyBonus = cl_lan->weaponReadyBonus;
		cDisableSpectatorMode = cl_lan->disableSpectatorMode;
		
		// -----------------
		// RW: Inventory
		switch (cl_lan->inventoryAttachment)
		{
			case 0:	// Old/Old
				gGameOptions.ubInventorySystem = INVENTORY_OLD;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
				break;
			case 1:	// New/Old
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
				break;
			case 2:	// New/New
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_NEW;
				break;
		}

		gGameOptions.ubSquadSize = 6;

		// Server forces us to play with new Inventory, but NIV is not allowed on the client,
		// because of wrong resolution or other stuff
		if ( UsingNewInventorySystem() == true && !IsNIVModeValid(true) )
		{
			SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };
			DoMessageBox( MSG_BOX_BASIC_STYLE , MPClientMessage[69] , guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT, InvalidClientSettingsOkBoxCallback, &CenteringRect );
		}
		else
		{
			// WANNE - MP: We have to re-initialize the correct interface
			if((UsingNewInventorySystem() == true))
			{
				InitNewInventorySystem();
				InitializeSMPanelCoordsNew();
				InitializeInvPanelCoordsNew();
			}
			else
			{
				gGameOptions.ubInventorySystem = INVENTORY_OLD;
				InitOldInventorySystem();
				InitializeSMPanelCoordsOld();
				InitializeInvPanelCoordsOld();
			}
			
			// WANNE - MP: We also have to reinitialize the merc profiles because
			// they depend on the inventory!
			LoadMercProfiles();

			InitializeFaceGearGraphics();

			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[26],cl_lan->client_num,szDefault );
			
			fDrawCharacterList = true; // set the character list to be redrawn
	
			strcpy(client_names[cl_lan->client_num-1],szDefault);

			// OJW - 20091024 - extract random table
			if (!is_server)
				memcpy(guiPreRandomNums,cl_lan->random_table,sizeof(UINT32)*MAX_PREGENERATED_NUMS);			

			// WANNE: Turn on airspace mode (to switch maps) for the server!
			if (is_server)			
				TurnOnAirSpaceMode();			
		}
	}
	else 
	{
		fDrawCharacterList = true; // set the character list to be redrawn

		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[26],cl_lan->client_num,szDefault );
			
		strcpy(client_names[cl_lan->client_num-1],szDefault);				
	}
}

void reapplySETTINGS()
{
	// reapply some settings that would be lost if a game is re-initialised
	// after joining ( from downloading files )

	// Stuff from connect_client()
	//**********************
	//here some nifty little tweaks
	LaptopSaveInfo.guiNumberOfMercPaymentsInDays += 20;
	LaptopSaveInfo.gubLastMercIndex = LAST_MERC_ID;
	
	LaptopSaveInfo.ubLastMercAvailableId = 7;
	gGameExternalOptions.fEnableSlayForever	=1;
	LaptopSaveInfo.gubPlayersMercAccountStatus = 4;
		
	// WANNE: This should fix the bug playing a "tilt" sound and showing the mini laptop graphic on the screen, when opening the laptop / option screen from map screen
	gfDontStartTransitionFromLaptop = TRUE;

	gMercProfiles[ 57 ].sSalary = 2000;
	gMercProfiles[ 58 ].sSalary = 1500;
	gMercProfiles[ 59 ].sSalary = 600;
	gMercProfiles[ 60 ].sSalary = 500;
	gMercProfiles[ 64 ].sSalary = 1500;
	gMercProfiles[ 72 ].sSalary = 1000;
	gMercProfiles[ 148 ].sSalary = 100;
	gMercProfiles[ 68 ].sSalary = 2200; //iggy;

	// Stuff from recieveSETTINGS
	gsMercArriveSectorX=gMPServerSettings.gsMercArriveSectorX;
	gsMercArriveSectorY=gMPServerSettings.gsMercArriveSectorY;

	cGameType=gMPServerSettings.gameType;
	cDisableMorale=gMPServerSettings.disableMorale;
	ChangeSelectedMapSector( gsMercArriveSectorX, gsMercArriveSectorY, 0 );
	CHAR16 str[128];
	GetSectorIDString( gsMercArriveSectorX, gsMercArriveSectorY, 0, str, TRUE );
	
	// WANNE - MP: If time set to 0, then no time turns
	INT32 secs_per_tick;
	if (gMPServerSettings.secondsPerTick == 0)
	{
		gGameOptions.fTurnTimeLimit=FALSE;
		secs_per_tick=1;
	}
	else
	{
		gGameOptions.fTurnTimeLimit=gMPServerSettings.sofTurnTimeLimit;
		secs_per_tick=gMPServerSettings.secondsPerTick;
	}
	
	PLAYER_TEAM_TIMER_SEC_PER_TICKS=secs_per_tick;

	INT32 clstarting_balance=gMPServerSettings.startingCash;//set starting balance

	if(LaptopSaveInfo.iCurrentBalance<clstarting_balance)
	{
		AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), clstarting_balance-LaptopSaveInfo.iCurrentBalance );
	}
	else
	{
		AddTransactionToPlayersBook( TRANSACTION_FEE, 0, GetWorldTotalMin(), clstarting_balance-LaptopSaveInfo.iCurrentBalance );
	}

	gGameOptions.fGunNut=gMPServerSettings.sofGunNut;	
	gGameOptions.ubGameStyle=gMPServerSettings.soubGameStyle;
	gGameOptions.ubDifficultyLevel=gMPServerSettings.soubDifficultyLevel;
	gGameOptions.fNewTraitSystem = gMPServerSettings.soubSkillTraits;
	gGameOptions.fIronManMode=gMPServerSettings.sofIronManMode;
	gGameOptions.ubBobbyRay=gMPServerSettings.soubBobbyRay;

	// Set Bobby Ray "Under Construction"?
	if(!gMPServerSettings.disableBobbyRay)
	{
		SetBookMark( BOBBYR_BOOKMARK );
		LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;
	}

	// Enable "AIM" and "MERC" only if random merc is false!
	if (!gMPServerSettings.randomMercs)
	{
		SetBookMark( AIM_BOOKMARK );
		SetBookMark( MERC_BOOKMARK );
	}

	if(!gMPServerSettings.disableMercEquipment)
		cAllowMercEquipment = 1;

	cMaxMercs = gMPServerSettings.maxMercs;	
	cReportHiredMerc = gMPServerSettings.reportHiredMerc;

	// WANNE - MP: I just added the NUM_SEC_IN_DAY so the game starts at Day 1 instead of Day 0
	gGameExternalOptions.iGameStartingTime= NUM_SEC_IN_DAY + int(gMPServerSettings.startingTime*3600);

	// WANNE - MP: In mulitplayer the hired merc arrive immediatly, so iFirstArrivalDelay must be set to 0
	// This also fixed the bug of the wrong hired hours!
	gGameExternalOptions.iFirstArrivalDelay = 0;

	// Disable Reinforcements
	gGameExternalOptions.gfAllowReinforcements				= false;
	gGameExternalOptions.gfAllowReinforcementsOnlyInCity	= false;

	// WANNE: The new improved interrupt system (IIS) does not work with multiplayer, so disable it
	gGameExternalOptions.fImprovedInterruptSystem			= false;

	// Disable Real-Time Mode
	// SANDRO - real-time sneak is in preferences
	//gGameExternalOptions.fAllowRealTimeSneak = false;
	gGameSettings.fOptions[TOPTION_ALLOW_REAL_TIME_SNEAK] = false;
	gGameExternalOptions.fQuietRealTimeSneak = false;

		// WANNE: Take the settings from the ja2_options.ini
	// WANNE: Enable fast loading
	/*gGameExternalOptions.fDisableLaptopTransition = true;
	gGameExternalOptions.fFastWWWSitesLoading = true;
	gGameExternalOptions.fDisableStrategicTransition = true;*/

	// WANNE: fix HOT DAY in night at arrival by night.
	// Explanation: If game starting time + first arrival delay < 07:00 (111600) -> we arrive before the sun rises or
	// if game starting time + first arrival delay >= 21:00 (162000) -> we arrive after the sun goes down
	if( (gGameExternalOptions.iGameStartingTime + gGameExternalOptions.	iFirstArrivalDelay) < 111600 ||
		(gGameExternalOptions.iGameStartingTime + gGameExternalOptions.iFirstArrivalDelay >= 162000))
	{ 
		// Default: Night
		gubEnvLightValue = 12; 
	}
	else
	{
		// Default: Day
		gubEnvLightValue = 3;
	}

	LightSetBaseLevel(gubEnvLightValue); 

	InitNewGameClock( );
				
	cWeaponReadyBonus = gMPServerSettings.weaponReadyBonus;
	cDisableSpectatorMode = gMPServerSettings.disableSpectatorMode;
	
	// -----------------
	// RW: Inventory
	switch (gMPServerSettings.inventoryAttachment)
	{
		case 0:	// Old/Old
			gGameOptions.ubInventorySystem = INVENTORY_OLD;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
			break;
		case 1:	// New/Old
			gGameOptions.ubInventorySystem = INVENTORY_NEW;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
			break;
		case 2:	// New/New
			gGameOptions.ubInventorySystem = INVENTORY_NEW;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_NEW;
			break;
	}

	gGameOptions.ubSquadSize = 6;

	// WANNE - MP: We have to re-initialize the correct interface
	if((UsingNewInventorySystem() == true) && IsNIVModeValid(true))
	{
		InitNewInventorySystem();
		InitializeSMPanelCoordsNew();
		InitializeInvPanelCoordsNew();
	}
	else
	{
		gGameOptions.ubInventorySystem = INVENTORY_OLD;
		InitOldInventorySystem();
		InitializeSMPanelCoordsOld();
		InitializeInvPanelCoordsOld();
	}

	// WANNE - MP: We also have to reinitialize the merc profiles because
	// they depend on the inventory!
	LoadMercProfiles();

	InitializeFaceGearGraphics();
	
	fDrawCharacterList = true; // set the character list to be redrawn
}

void recieveTEAMCHANGE( RPCParameters *rpcParameters )
{
	teamchange_struct* cl_lan = (teamchange_struct*)rpcParameters->input;

	if (!can_teamchange())
	{
		// error
		ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"An error occured in recieveTEAMCHANGE that should not occur");
	}
	else
	{
		// redraw the character list on the map screen
		fTeamPanelDirty = true; 
		fDrawCharacterList = true;
		client_teams[cl_lan->client_num-1] = cl_lan->newteam;
		if (cl_lan->client_num == CLIENT_NUM)
		{
			TEAM = cl_lan->newteam;
		}
	}
}

void recieveEDGECHANGE( RPCParameters *rpcParameters )
{
	edgechange_struct* cl_lan = (edgechange_struct*)rpcParameters->input;

	if (!can_edgechange())
	{
		// error
		ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"An error occured in recieveEDGECHANGE that should not occur");
	}
	else
	{
		// redraw the character list on the map screen
		fTeamPanelDirty = true;
		fDrawCharacterList = true;
		client_edges[cl_lan->client_num-1] = cl_lan->newedge;
		if (cl_lan->client_num == CLIENT_NUM)
		{
			// store the setting locally
			cStartingSectorEdge = cl_lan->newedge;
		}
	}
}

void recieveMAPCHANGE( RPCParameters *rpcParameters )
{
	mapchange_struct* cl_lan = (mapchange_struct*)rpcParameters->input;

	if (!is_client || allowlaptop)
	{
		// error
		ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"An error occured in recieveMAPCHANGE that should not occur");
	}
	else
	{
		// copy new map settings locally
		gsMercArriveSectorX=cl_lan->gsMercArriveSectorX;
		gsMercArriveSectorY=cl_lan->gsMercArriveSectorY;

		ChangeSelectedMapSector( gsMercArriveSectorX, gsMercArriveSectorY, 0 );

		// WANNE - BMP:
		InitializeWorldSize( gsMercArriveSectorX, gsMercArriveSectorY, 0 );

		gGameExternalOptions.iGameStartingTime= NUM_SEC_IN_DAY + int(cl_lan->startingTime*3600);

		CHAR16 str[128];
		GetSectorIDString( gsMercArriveSectorX, gsMercArriveSectorY, 0, str, TRUE );

		// notify clients of map change in console
		ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, MPClientMessage[46],str);
	}
}

// 20091002 - OJW - Explosives
void send_grenade (OBJECTTYPE *pGameObj, float dLifeLength, float xPos, float yPos, float zPos, float xForce, float yForce, float zForce, UINT32 sTargetGridNo, UINT8 ubOwner, UINT8 ubActionCode, UINT32 uiActionData, INT32 iRealObjectID , bool bIsThrownGrenade)
{
	ubOwner = MPEncodeSoldierID(ubOwner); // translate our soldier to the "network" version

	SOLDIERTYPE* pSoldier = MercPtrs[ubOwner];
	if (pSoldier != NULL)
	{
		if ((pSoldier->bTeam == 1 && is_server) || IsOurSoldier(pSoldier))
		{
			physics_object gren;
			gren.dForceX = xForce;
			gren.dForceY = yForce;
			gren.dForceZ = zForce;
			gren.dX = xPos;
			gren.dY = yPos;
			gren.dZ = zPos;
			gren.dLifeSpan = dLifeLength;
			gren.ubActionCode = ubActionCode;
			gren.uiActionData = uiActionData;
			gren.ubID = ubOwner;
			gren.usItem = pGameObj->usItem;
			gren.sTargetGridNo = sTargetGridNo;
			gren.ubID = ubOwner;
			gren.RealObjectID = iRealObjectID;
			gren.IsThrownGrenade = bIsThrownGrenade;
			gren.uiPreRandomIndex = guiPreRandomIndex;

#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - send_grenade ( usItem : %i , sGridNo : %i , ubSoldierID : %i , uiPreRandomIndex : %i )\n",gren.usItem, gren.sTargetGridNo , gren.ubID , guiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
			gfMPDebugOutputRandoms = true;
#endif

			client->RPC("sendGRENADE",(const char*)&gren, (int)sizeof(physics_object)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}
}

void recieveGRENADE (RPCParameters *rpcParameters)
{
	physics_object* gren = (physics_object*)rpcParameters->input;

	gren->ubID = MPDecodeSoldierID(gren->ubID);

	SOLDIERTYPE* pThrower = MercPtrs[ gren->ubID ];
	if (pThrower != NULL)
	{
		guiPreRandomIndex = gren->uiPreRandomIndex;
		
		// grenade wasnt thrown by one of our guys, so we should do it on the client
		if (!IsOurSoldier(pThrower) && (pThrower->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recieveGRENADE ( usItem : %i , sGridNo : %i , ubSoldierID : %i , uiPreRandomIndex : %i )\n",gren->usItem, gren->sTargetGridNo , gren->ubID , guiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
			gfMPDebugOutputRandoms = true;
#endif

			// this is a bit of a hack until we do the inventory sync
			OBJECTTYPE* newObj = new OBJECTTYPE();
			CreateItem( gren->usItem, 99, newObj );
			OBJECTTYPE::CopyToOrCreateAt(&pThrower->pTempObject, newObj);
			// this will create a grenade and launch it
			INT32 i = CreatePhysicalObject( pThrower->pTempObject , gren->dLifeSpan , gren->dX , gren->dY , gren->dZ , gren->dForceX , gren->dForceY , gren->dForceZ , pThrower->ubID , gren->ubActionCode , gren->uiActionData, false);
			// save extra state info so we can check and feed it result later
			ObjectSlots[ i ].mpRealObjectID = gren->RealObjectID;
			ObjectSlots[ i ].mpTeam = pThrower->bTeam;
			ObjectSlots[ i ].mpIsFromRemoteClient = true;
			ObjectSlots[ i ].mpHaveClientResult = false;
			ObjectSlots[ i ].mpWasDud = false;

			// Do grenade animation (todo fix this for mortars)
			if (gren->IsThrownGrenade)
			{
				{
					Assert(pThrower->pThrowParams == NULL);

					// not a mem leak
					// will be freed in AdjustToNextAnimationFrame(SOLDIERTYPE*), case 461
					pThrower->pThrowParams = (THROW_PARAMS*) malloc(sizeof(THROW_PARAMS));
					pThrower->pThrowParams->dForceX = gren->dForceX;
					pThrower->pThrowParams->dForceY = gren->dForceY;
					pThrower->pThrowParams->dForceZ = gren->dForceZ;
					pThrower->pThrowParams->dLifeSpan = gren->dLifeSpan;
					pThrower->pThrowParams->dX = gren->dX;
					pThrower->pThrowParams->dY = gren->dY;
					pThrower->pThrowParams->dZ = gren->dZ;
					pThrower->pThrowParams->ubActionCode = gren->ubActionCode;
					pThrower->pThrowParams->uiActionData = gren->uiActionData;
				}

				HandleSoldierThrowItem( pThrower, gren->sTargetGridNo );
			}
		}
	}
	else
	{
#ifdef JA2BETAVERSION
		char tmpMsg[128];
		sprintf(tmpMsg,"ERROR! - Invalid Soldier pointer from ubID %i in recieveGRENADE()",gren->ubID);
		//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
		MPDebugMsg(tmpMsg);
#endif
	}
}

// we send a grenade result out to the clients as it may have been a fizzer
void send_grenade_result (float xPos, float yPos, float zPos, INT32 sGridNo, UINT8 ubOwnerID, INT32 iRealObjectID, bool bIsDud)
{
	ubOwnerID = MPEncodeSoldierID(ubOwnerID); // translate our soldier to the "network" version

	SOLDIERTYPE* pSoldier = MercPtrs[ubOwnerID];
	if (pSoldier != NULL)
	{
		if ((pSoldier->bTeam == 1 && is_server) || IsOurSoldier(pSoldier))
		{
			grenade_result gres;
			gres.dX = xPos;
			gres.dY = yPos;
			gres.dZ = zPos;
			gres.sGridNo = sGridNo;
			gres.ubOwnerID = ubOwnerID;
			gres.RealObjectID = iRealObjectID;
			gres.bWasDud = bIsDud;
			gres.uiPreRandomIndex = guiPreRandomIndex;

#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - send_grenade_result ( RealObjectID : %i , sGridNo : %i , ubSoldierID : %i , uiPreRandomIndex : %i )\n",gres.RealObjectID, gres.sGridNo , gres.ubOwnerID , guiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
			gfMPDebugOutputRandoms = true;
#endif

			client->RPC("sendGRENADERESULT",(const char*)&gres, (int)sizeof(grenade_result)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}
}

void recieveGRENADERESULT (RPCParameters *rpcParameters)
{
	grenade_result* gres = (grenade_result*)rpcParameters->input;

	gres->ubOwnerID = MPDecodeSoldierID(gres->ubOwnerID);

	SOLDIERTYPE* pThrower = MercPtrs[ gres->ubOwnerID ];
	if (pThrower != NULL)
	{
		// grenade wasnt thrown by one of our guys, so we should do it on the client
		if (!IsOurSoldier(pThrower) && (pThrower->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recieveGRENADERESULT ( RealObjectID : %i , sGridNo : %i , ubSoldierID : %i , uiPreRandomIndex : %i )\n",gres->RealObjectID, gres->sGridNo , gres->ubOwnerID , gres->uiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
			gfMPDebugOutputRandoms = true;
#endif

			bool bFound = false;
			INT32 usCnt;
			// loop through and find the local object we assigned for the remote grenade
			for( usCnt=0; usCnt<NUM_OBJECT_SLOTS; usCnt++ )
			{
				if (ObjectSlots[ usCnt ].mpTeam == pThrower->bTeam && ObjectSlots[ usCnt ].mpRealObjectID == gres->RealObjectID)
				{
					bFound = true;
					break;
				}
			}

			if (bFound)
			{
				// override the local predictions with the ones from the client that threw it
				ObjectSlots[ usCnt ].mpHaveClientResult = true;
				ObjectSlots[ usCnt ].mpWasDud = gres->bWasDud;
				ObjectSlots[ usCnt ].Position.x = gres->dX;
				ObjectSlots[ usCnt ].Position.y = gres->dY;
				ObjectSlots[ usCnt ].Position.z = gres->dZ;
				ObjectSlots[ usCnt ].sGridNo = gres->sGridNo;

				HandleArmedObjectImpact( &ObjectSlots[ usCnt ] );

				guiPreRandomIndex = gres->uiPreRandomIndex; // do this here because it should be in the same sequence as the sending computer which sends the grenade result at the end of HandleArmedObjectImpact()
			}
			else
			{
#ifdef JA2BETAVERSION
				char tmpMsg1[128];
				sprintf(tmpMsg1,"ERROR! - Couldnt find a local PhysicsObject for the RealObjectID %i sent remotely from Team %i in recievePLANTEXPLOSIVE()",gres->RealObjectID, pThrower->bTeam );
				//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
				MPDebugMsg(tmpMsg1);
#endif
			}
		}
	}
	else
	{
#ifdef JA2BETAVERSION
		char tmpMsg[128];
		sprintf(tmpMsg,"ERROR! - Invalid Soldier pointer from ubID %i in recieveGRENADERESULT()",gres->ubOwnerID);
		//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
		MPDebugMsg(tmpMsg);
#endif
	}
}

void send_plant_explosive (UINT8 ubID,UINT16 usItem,UINT8 ubItemStatus,UINT16 usFlags, UINT32 sGridNo,UINT8 ubLevel, UINT32 uiWorldItemIndex)
{
	explosive_obj exp;

	exp.sGridNo = sGridNo;
	exp.ubID = MPEncodeSoldierID(ubID);
	exp.usItem = usItem;
	exp.ubItemStatus = ubItemStatus;
	exp.usFlags = usFlags;
	exp.uiWorldIndex = uiWorldItemIndex;
	exp.ubLevel = ubLevel;
	exp.bDetonatorType = gWorldItems[ uiWorldItemIndex ].object[0]->data.misc.bDetonatorType;
	if (exp.bDetonatorType == BOMB_REMOTE)
		exp.bDelayFreq = gWorldItems[ uiWorldItemIndex ].object[0]->data.misc.bFrequency;
	else
		exp.bDelayFreq = gWorldItems[ uiWorldItemIndex ].object[0]->data.misc.bDelay;

#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"MP - send_plant_explosive ( usItem : %i , sGridNo : %i , ubSoldierID : %i , uiPreRandomIndex : %i , uiWorldItemIndex : %i )\n",exp.usItem, exp.sGridNo , exp.ubID , guiPreRandomIndex , uiWorldItemIndex );
	MPDebugMsg(tmpMPDbgString);
#endif

	client->RPC("sendPLANTEXPLOSIVE",(const char*)&exp, (int)sizeof(explosive_obj)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recievePLANTEXPLOSIVE (RPCParameters *rpcParameters)
{
	explosive_obj* exp = (explosive_obj*)rpcParameters->input;

	exp->ubID = MPDecodeSoldierID( exp->ubID );

	SOLDIERTYPE* pSoldier = MercPtrs[ exp->ubID ];
	if (pSoldier != NULL)
	{
		// explosive wasnt planted on our client, so we should do it on the client
		if (!IsOurSoldier(pSoldier) && (pSoldier->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recievePLANTEXPLOSIVE ( usItem : %i , sGridNo : %i , ubSoldierID : %i , uiPreRandomIndex : %i )\n",exp->usItem, exp->sGridNo , exp->ubID , guiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
#endif

			// this is a bit of a hack until we do the inventory sync
			OBJECTTYPE* newObj = new OBJECTTYPE();
			CreateItem( exp->usItem, exp->ubItemStatus, newObj );
			INT32 iNewItemIndex;
			OBJECTTYPE* pObj = AddItemToPoolAndGetIndex( exp->sGridNo, newObj, VISIBLE, exp->ubLevel, exp->usFlags,0, exp->ubID ,&iNewItemIndex);
			// need to save Item Type metadata agaist the world item
			(*pObj)[0]->data.misc.ubBombOwner = exp->ubID + 2; // this is a hack the designers put into the game, storing the side as well (which isnt relevant in MP, but still have to do it)
			(*pObj)[0]->data.misc.usBombItem = exp->usItem;
			(*pObj)[0]->data.misc.bDetonatorType = exp->bDetonatorType;
			if (exp->bDetonatorType == BOMB_REMOTE)
				(*pObj)[0]->data.misc.bFrequency = exp->bDelayFreq;
			else
				(*pObj)[0]->data.misc.bDelay = exp->bDelayFreq;

			// save old clients WorldID if we can
			// loop through world bombs and find the one linked to the item we just created
			UINT32 uiCount;
			bool bFound = false;
			for(uiCount=0; uiCount < guiNumWorldBombs; uiCount++)
			{
				if ( gWorldBombs[ uiCount ].fExists == TRUE && gWorldBombs[ uiCount ].iItemIndex == iNewItemIndex)
				{
					bFound = true;
					gWorldBombs[uiCount].iMPWorldItemIndex = exp->uiWorldIndex;
					gWorldBombs[uiCount].ubMPTeamIndex = pSoldier->bTeam;
					gWorldBombs[uiCount].bIsFromRemotePlayer = true;
					break;
				}
			}

			if (!bFound)
			{
#ifdef JA2BETAVERSION
				// this is a local failure really and will probably NEVER happen
				char tmpMsg1[128];
				sprintf(tmpMsg1,"ERROR! - Couldnt link our local WorldBomb to the ID sent remotely from Team %i in recievePLANTEXPLOSIVE()", pSoldier->bTeam );
				//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
				MPDebugMsg(tmpMsg1);
#endif
			}
		}
	}
	else
	{
#ifdef JA2BETAVERSION
		char tmpMsg[128];
		sprintf(tmpMsg,"ERROR! - Invalid Soldier pointer from ubID %i in recievePLANTEXPLOSIVE()",exp->ubID);
		//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
		MPDebugMsg(tmpMsg);
#endif
	}
}

void send_detonate_explosive (UINT32 uiWorldIndex, UINT8 ubID)
{
	ubID = MPEncodeSoldierID(ubID);

	SOLDIERTYPE* pSoldier = MercPtrs[ubID];
	if (pSoldier != NULL)
	{
		// explosive detonated on this client, notify the other clients
		if ((pSoldier->bTeam == 1 && is_server) || IsOurSoldier(pSoldier))
		{
			// find the appropriate world bomb for the world item
			UINT8 uiBombIndex = -1;
			UINT32 uiCount;
			for(uiCount=0; uiCount < guiNumWorldBombs; uiCount++)
			{
				if (gWorldBombs[uiCount].iItemIndex == uiWorldIndex)
				{
					uiBombIndex = uiCount;
					break;
				}
			}

			if (uiBombIndex > -1)
			{
				detonate_struct det;
				det.ubID = ubID;
										
				if ( gWorldBombs[ uiBombIndex ].bIsFromRemotePlayer ) 
				{
					// it is possible for players from other teams to set off a bomb that does not belong to them if they fail disarming it
					// but we must send the ID for the world item of the bomb that all the other clients recognise
					det.ubMPTeamIndex = gWorldBombs[ uiBombIndex ].ubMPTeamIndex;
					det.uiWorldItemIndex = gWorldBombs[ uiBombIndex ].iMPWorldItemIndex;
				}
				else
				{
					// it is a bomb that originated on our client
					det.uiWorldItemIndex = uiWorldIndex;
					det.ubMPTeamIndex = pSoldier->bTeam;
				}
				det.uiPreRandomIndex = guiPreRandomIndex;

#ifdef JA2BETAVERSION
				CHAR tmpMPDbgString[512];
				sprintf(tmpMPDbgString,"MP - send_detonate_explosive ( MPTeam : %i , uiWorldIndex : %i , uiPreRandomIndex : %i )\n",det.ubMPTeamIndex, det.uiWorldItemIndex , det.uiPreRandomIndex );
				MPDebugMsg(tmpMPDbgString);
				gfMPDebugOutputRandoms = true;
#endif

				client->RPC("sendDETONATEEXPLOSIVE",(const char*)&det, (int)sizeof(detonate_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			}
			else
			{
				char tmpMsg1[128];
				sprintf(tmpMsg1,"ERROR! - Cant find a local WorldBomb for WorldIndex (locally) %i in send_detonate_explosive()",uiWorldIndex);
				//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
				MPDebugMsg(tmpMsg1);
			}
		}
	}
}

void recieveDETONATEEXPLOSIVE (RPCParameters *rpcParameters)
{
	detonate_struct* det = (detonate_struct*)rpcParameters->input;

	det->ubID = MPDecodeSoldierID(det->ubID);

	SOLDIERTYPE* pSoldier = MercPtrs[det->ubID];
	if (pSoldier != NULL)
	{
		// if explosive detonation didnt originate from this client then its need to be performed here
		if (pSoldier->bTeam != netbTeam && (pSoldier->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recieveDETONATEEXPLOSIVE ( MPTeam : %i , uiWorldIndex : %i , uiPreRandomIndex : %i , ubID : %i )\n",det->ubMPTeamIndex, det->uiWorldItemIndex , det->uiPreRandomIndex , det->ubID );
			MPDebugMsg(tmpMPDbgString);
			gfMPDebugOutputRandoms = true;
#endif

			guiPreRandomIndex = det->uiPreRandomIndex; // syncronise random number generator

			UINT32 uiCount;
			UINT32 ubWorldIndexToCheck = -1;
			bool bFound = false;
			for(uiCount=0; uiCount < guiNumWorldBombs; uiCount++)
			{
				// we could be recieving a message that a player from another team has detonated our bomb (while disarming), in this case we would check the local ids
				// otherwise we check MPCreatorID's like normal
				ubWorldIndexToCheck = (det->ubMPTeamIndex == netbTeam ? gWorldBombs[ uiCount ].iItemIndex : gWorldBombs[ uiCount ].iMPWorldItemIndex);
				if ( gWorldBombs[ uiCount ].fExists == TRUE && 
					 ubWorldIndexToCheck == det->uiWorldItemIndex &&
					 (gWorldBombs[ uiCount ].ubMPTeamIndex == det->ubMPTeamIndex || det->ubMPTeamIndex == netbTeam) )
				{
					bFound = true;
					AddBombToQueue(uiCount, guiBaseJA2Clock, TRUE); // blow up now :)
					break;
				}
			}

			if (!bFound)
			{
#ifdef JA2BETAVERSION
				char tmpMsg1[128];
				sprintf(tmpMsg1,"ERROR! - Cant find a local WorldBomb for remote WorldIndex %i from Team %i in recieveDETONATEEXPLOSIVE()",det->uiWorldItemIndex,det->ubMPTeamIndex );
				//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
				MPDebugMsg(tmpMsg1);
#endif
			}
		}
	}
}

void send_disarm_explosive(UINT32 sGridNo, UINT32 uiWorldItem, UINT8 ubID)
{
	ubID = MPEncodeSoldierID(ubID);

	SOLDIERTYPE* pSoldier = MercPtrs[ubID];
	if (pSoldier != NULL)
	{
		// explosive disarmed on this client, notify the other clients
		if ((pSoldier->bTeam == 1 && is_server) || IsOurSoldier(pSoldier))
		{
			// find the appropriate world bomb for the world item
			UINT8 uiBombIndex = -1;
			UINT32 uiCount;
			for(uiCount=0; uiCount < guiNumWorldBombs; uiCount++)
			{
				if (gWorldBombs[uiCount].iItemIndex == uiWorldItem)
				{
					uiBombIndex = uiCount;
					break;
				}
			}

			if (uiBombIndex > -1)
			{
				disarm_struct disarm;
				disarm.ubID = ubID;
				if ( gWorldBombs[ uiBombIndex ].bIsFromRemotePlayer )
				{
					// it is possible for players from other teams to defuse a bomb in the world
					// but we must send the ID for the world item of the bomb that all the other clients recognise
					disarm.ubMPTeamIndex = gWorldBombs[ uiBombIndex ].ubMPTeamIndex;
					disarm.uiWorldItemIndex = gWorldBombs[ uiBombIndex ].iMPWorldItemIndex;
				}
				else
				{
					disarm.ubMPTeamIndex = pSoldier->bTeam;
					disarm.uiWorldItemIndex = uiWorldItem;
				}
				disarm.sGridNo = sGridNo;
				disarm.uiPreRandomIndex = guiPreRandomIndex;

	#ifdef JA2BETAVERSION
				CHAR tmpMPDbgString[512];
				sprintf(tmpMPDbgString,"MP - send_disarm_explosive ( MPTeam : %i , uiWorldIndex : %i , uiPreRandomIndex : %i , sGridNo : %i )\n",disarm.ubMPTeamIndex, disarm.uiWorldItemIndex , disarm.uiPreRandomIndex );
				MPDebugMsg(tmpMPDbgString);
				gfMPDebugOutputRandoms = true;
	#endif

				client->RPC("sendDISARMEXPLOSIVE",(const char*)&disarm, (int)sizeof(disarm_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			}
			else
			{
				char tmpMsg1[128];
				sprintf(tmpMsg1,"ERROR! - Cant find a local WorldBomb for WorldIndex (locally) %i in send_disarm_explosive()",uiWorldItem);
				//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
				MPDebugMsg(tmpMsg1);
			}
		}
	}
}

void recieveDISARMEXPLOSIVE (RPCParameters *rpcParameters)
{
	disarm_struct* disarm = (disarm_struct*)rpcParameters->input; 

	disarm->ubID = MPDecodeSoldierID(disarm->ubID);

	SOLDIERTYPE* pSoldier = MercPtrs[disarm->ubID];
	if (pSoldier != NULL)
	{
		// if explosive disarm didnt originate from this client then its need to be performed here
		if (pSoldier->bTeam != netbTeam && (pSoldier->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recieveDISARMEXPLOSIVE ( MPTeam : %i , uiWorldItemIndex : %i , uiPreRandomIndex : %i , ubID : %i , sGridNo : %i )\n",disarm->ubMPTeamIndex, disarm->uiWorldItemIndex , disarm->uiPreRandomIndex , disarm->ubID , disarm->sGridNo );
			MPDebugMsg(tmpMPDbgString);
			gfMPDebugOutputRandoms = true;
#endif

			guiPreRandomIndex = disarm->uiPreRandomIndex; // syncronise random number generator

			UINT32 uiCount;
			UINT32 ubWorldIndexToCheck = -1;
			bool bFound = false;
			for(uiCount=0; uiCount < guiNumWorldBombs; uiCount++)
			{
				ubWorldIndexToCheck = (disarm->ubMPTeamIndex == netbTeam ? gWorldBombs[ uiCount ].iItemIndex : gWorldBombs[ uiCount ].iMPWorldItemIndex);
				if ( gWorldBombs[ uiCount ].fExists == TRUE && 
					 disarm->uiWorldItemIndex == ubWorldIndexToCheck &&
					 (gWorldBombs[ uiCount ].ubMPTeamIndex == disarm->ubMPTeamIndex || disarm->ubMPTeamIndex == netbTeam) )
				{
					bFound = true;
					// print out a screen message if it was our bomb
					if (disarm->ubMPTeamIndex == netbTeam)
					{
						SOLDIERTYPE * pBombOwner = MercPtrs[gWorldItems[ gWorldBombs[ uiCount ].iItemIndex ].soldierID];
						if (pBombOwner != NULL)
						{
							ScreenMsg( FONT_LTBLUE , MSG_MPSYSTEM , MPClientMessage[71], pBombOwner->name, pSoldier->name);
						}
					}

					// removing from the item pool will remove world item and world bomb
					UINT8 ubLevel = gWorldItems[ gWorldBombs[ uiCount ].iItemIndex ].ubLevel;
					RemoveItemFromPool( disarm->sGridNo , gWorldBombs[ uiCount ].iItemIndex, ubLevel );
					break;
				}
			}

			if (!bFound)
			{
#ifdef JA2BETAVERSION
				char tmpMsg1[128];
				sprintf(tmpMsg1,"ERROR! - Cant find a local WorldBomb for remote WorldIndex %i from Team %i in recieveDISARMEXPLOSIVE()",disarm->uiWorldItemIndex,disarm->ubMPTeamIndex );
				//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
				MPDebugMsg(tmpMsg1);
#endif
			}
		}
	}
}

void send_spreadeffect ( INT32 sGridNo, UINT8 ubRadius, UINT16 usItem, UINT8 ubOwner, BOOLEAN fSubsequent, INT8 bLevel, INT32 iSmokeEffectID )
{
	spreadeffect_struct sef;

	sef.sGridNo = sGridNo;
	sef.ubRadius = ubRadius;
	sef.usItem = usItem;
	sef.ubOwner = MPEncodeSoldierID(ubOwner);
	sef.fSubsequent = fSubsequent;
	sef.bLevel = bLevel;
	sef.iSmokeEffectID = iSmokeEffectID;
	sef.uiPreRandomIndex = guiPreRandomIndex;

#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"MP - send_spreadeffect ( sGridNo : %i , ubRadius : %i , usItem : %i , ubOwner : %i , fSubsequent : %i , bLevel : %i , iSmokeEffectID : %i , uiPreRandomIndex : %i )\n",sef.sGridNo, sef.ubRadius ,sef.usItem, sef.ubOwner, sef.fSubsequent, sef.bLevel, sef.iSmokeEffectID, sef.uiPreRandomIndex );
	MPDebugMsg(tmpMPDbgString);
#endif

	client->RPC("sendSPREADEFFECT",(const char*)&sef, (int)sizeof(spreadeffect_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveSPREADEFFECT (RPCParameters *rpcParameters)
{
	spreadeffect_struct* sef = (spreadeffect_struct*)rpcParameters->input;

	sef->ubOwner = MPDecodeSoldierID(sef->ubOwner);

	SOLDIERTYPE* pSoldier = MercPtrs[sef->ubOwner];
	if (pSoldier != NULL)
	{

		// spread effect didnt originate from us
		if (!IsOurSoldier(pSoldier) && (pSoldier->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recieveSPREADEFFECT ( sGridNo : %i , ubRadius : %i , usItem : %i , ubOwner : %i , fSubsequent : %i , bLevel : %i , iSmokeEffectID : %i , uiPreRandomIndex : %i )\n",sef->sGridNo, sef->ubRadius ,sef->usItem, sef->ubOwner, sef->fSubsequent, sef->bLevel, sef->iSmokeEffectID, sef->uiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
#endif

			guiPreRandomIndex = sef->uiPreRandomIndex; // syncronise random number generator

			// translate SmokeEffectID
			if (sef->iSmokeEffectID >= 0)
			{
				UINT32 uiCount;
				bool bFound = false;
				for(uiCount=0; uiCount < guiNumSmokeEffects; uiCount++)
				{
					if ( gSmokeEffectData[ uiCount ].fAllocated == TRUE && gSmokeEffectData[ uiCount ].iMPTeamIndex == pSoldier->bTeam && gSmokeEffectData[ uiCount ].iMPSmokeEffectID == sef->iSmokeEffectID)
					{
						bFound = true;
						SpreadEffect( sef->sGridNo , sef->ubRadius , sef->usItem , sef->ubOwner , sef->fSubsequent , sef->bLevel , uiCount , TRUE);
						break;
					}
				}

				if (!bFound)
				{
#ifdef JA2BETAVERSION
					char tmpMsg1[128];
					sprintf(tmpMsg1,"ERROR! - Cant find a local SmokeEffectID for remote ID %i from team %i in recieveSPREADEFFECT()",sef->iSmokeEffectID, pSoldier->bTeam);
					//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
					MPDebugMsg(tmpMsg1);
#endif
				}
			}
			else
			{
				SpreadEffect( sef->sGridNo , sef->ubRadius , sef->usItem , sef->ubOwner , sef->fSubsequent , sef->bLevel , sef->iSmokeEffectID , TRUE);
			}
		}
	}
	else
	{
#ifdef JA2BETAVERSION
		char tmpMsg2[128];
		sprintf(tmpMsg2,"ERROR! - Invalid Soldier pointer from ubID %i in recieveSPREADEFFECT()",sef->ubOwner);
		//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
		MPDebugMsg(tmpMsg2);
#endif
	}
}

void send_newsmokeeffect(INT32 sGridNo, UINT16 usItem, INT8 bLevel, UINT8 ubOwner, INT32 iSmokeEffectID)
{
	// i'm reusing this struct, the parameters are essentially the same
	spreadeffect_struct sef;

	sef.sGridNo = sGridNo;
	sef.usItem = usItem;
	sef.ubOwner = MPEncodeSoldierID(ubOwner);
	sef.bLevel = bLevel;
	sef.iSmokeEffectID = iSmokeEffectID;
	sef.uiPreRandomIndex = guiPreRandomIndex;

#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"MP - send_newsmokeeffect ( sGridNo : %i , usItem : %i , ubOwner : %i , bLevel : %i , iSmokeEffectID : %i , uiPreRandomIndex : %i )\n",sef.sGridNo, sef.usItem, sef.ubOwner, sef.bLevel, sef.iSmokeEffectID, sef.uiPreRandomIndex );
	MPDebugMsg(tmpMPDbgString);
#endif

	client->RPC("sendNEWSMOKEEFFECT",(const char*)&sef, (int)sizeof(spreadeffect_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveNEWSMOKEEFFECT (RPCParameters *rpcParameters)
{
	spreadeffect_struct* sef = (spreadeffect_struct*)rpcParameters->input;

	// translate any of our soldier ids back to the correct local copy
	sef->ubOwner = MPDecodeSoldierID(sef->ubOwner);

	SOLDIERTYPE* pSoldier = MercPtrs[sef->ubOwner];
	if (pSoldier != NULL)
	{
		// new smoke effect didnt originate from us
		if (!IsOurSoldier(pSoldier) && (pSoldier->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recieveNEWSMOKEEFFECT ( sGridNo : %i , usItem : %i , ubOwner : %i , bLevel : %i , iSmokeEffectID : %i , uiPreRandomIndex : %i )\n",sef->sGridNo, sef->usItem, sef->ubOwner, sef->bLevel, sef->iSmokeEffectID, sef->uiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
#endif

			guiPreRandomIndex = sef->uiPreRandomIndex;

			// start new smoke effect
			INT32 iNewSmokeIndex = NewSmokeEffect( sef->sGridNo , sef->usItem , sef->bLevel , sef->ubOwner , TRUE );
			
			// attach remote id to local smoke effect
			gSmokeEffectData[iNewSmokeIndex].iMPTeamIndex = pSoldier->bTeam;
			gSmokeEffectData[iNewSmokeIndex].iMPSmokeEffectID = sef->iSmokeEffectID;
		}
	}
	else
	{
#ifdef JA2BETAVERSION
		char tmpMsg[128];
		sprintf(tmpMsg,"ERROR! - Invalid Soldier pointer from ubID %i in recieveNEWSMOKEEFFECT()",sef->ubOwner);
		//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
		MPDebugMsg(tmpMsg);
#endif
	}
}

void send_gasdamage( SOLDIERTYPE * pSoldier, UINT16 usExplosiveClassID, INT16 sSubsequent, BOOLEAN fRecompileMovementCosts, INT16 sWoundAmt, INT16 sBreathAmt, UINT8 ubOwner )
{
	explosiondamage_struct exp;
	exp.ubDamageFunc = 1;
	exp.ubSoldierID = MPEncodeSoldierID(pSoldier->ubID);
	exp.usExplosiveClassID = usExplosiveClassID;
	exp.sSubsequent = sSubsequent;
	exp.fRecompileMovementCosts = fRecompileMovementCosts;
	exp.sWoundAmt = sWoundAmt;
	exp.sBreathAmt = sBreathAmt;
	exp.ubAttackerID = MPEncodeSoldierID(ubOwner);
	exp.uiPreRandomIndex = guiPreRandomIndex;

#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"MP - send_gasdamage ( ubSoldierID : %i , usExplosiveClassID : %i , sSubsequent : %i , recompileMoveCosts : %i , sWoundAmt : %i , sBreathAmt : %i , ubOwner : %i )\n", exp.ubSoldierID , usExplosiveClassID , sSubsequent , fRecompileMovementCosts , sWoundAmt , sBreathAmt , exp.ubAttackerID );
	MPDebugMsg(tmpMPDbgString);
#endif

	client->RPC("sendEXPLOSIONDAMAGE",(const char*)&exp, (int)sizeof(explosiondamage_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void send_explosivedamage( UINT8 ubPerson, UINT8 ubOwner, INT32 sBombGridNo, INT16 sWoundAmt, INT16 sBreathAmt, UINT32 uiDist, UINT16 usItem, INT16 sSubsequent )
{
	explosiondamage_struct exp;
	exp.ubDamageFunc = 2;
	exp.ubSoldierID = MPEncodeSoldierID(ubPerson);
	exp.usItem = usItem;
	exp.uiDist = uiDist;
	exp.sSubsequent = sSubsequent;
	exp.sBombGridNo = sBombGridNo;
	exp.sWoundAmt = sWoundAmt;
	exp.sBreathAmt = sBreathAmt;
	exp.ubAttackerID = MPEncodeSoldierID(ubOwner);
	exp.uiPreRandomIndex = guiPreRandomIndex;

#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"MP - send_explosivedamage ( ubPerson : %i , ubOwner : %i , sBombGridNo : %i , sWoundAmt : %i , sBreathAmt : %i , uiDist : %i , usItem : %i , sSubs : %i )\n",ubPerson, ubOwner , sBombGridNo , sWoundAmt , sBreathAmt , uiDist , usItem , sSubsequent );
	MPDebugMsg(tmpMPDbgString);
#endif

	client->RPC("sendEXPLOSIONDAMAGE",(const char*)&exp, (int)sizeof(explosiondamage_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveEXPLOSIONDAMAGE (RPCParameters *rpcParameters)
{
	explosiondamage_struct* exp = (explosiondamage_struct*)rpcParameters->input;

	exp->ubSoldierID = MPDecodeSoldierID(exp->ubSoldierID);
	exp->ubAttackerID = MPDecodeSoldierID(exp->ubAttackerID);


	SOLDIERTYPE* pSoldier = MercPtrs[exp->ubSoldierID];
	if (pSoldier != NULL)
	{

		// damage isnt for our merc (or we wouldve handled it locally) or it is for an AI but we are NOT the server
		if (!IsOurSoldier(pSoldier) && (pSoldier->bTeam != 1 || !is_server))
		{
#ifdef JA2BETAVERSION
			CHAR tmpMPDbgString[512];
			sprintf(tmpMPDbgString,"MP - recieveEXPLOSIONDAMAGE ( ubDamageFunc : %i , ubSoldierID : %i , ubAttackerID : %i , usItem : %i , usExplosiveClassID : %i , sWoundAmt : %i , sBreathAmt : %i , uiDist : %i , sSubs : %i , sBombGridNo : %i , uiPreRandomIndex : %i )\n", exp->ubDamageFunc , exp->ubSoldierID , exp->ubAttackerID , exp->usItem , exp->usExplosiveClassID , exp->sWoundAmt , exp->sBreathAmt , exp->uiDist , exp->sSubsequent , exp->sBombGridNo , exp->uiPreRandomIndex );
			MPDebugMsg(tmpMPDbgString);
#endif

			guiPreRandomIndex = exp->uiPreRandomIndex;

			if (exp->ubDamageFunc == 1)
			{
				//EXPLOSIVETYPE* pExplosive = &(Explosive[ exp->usExplosiveClassID ] );
				//DishOutGasDamage(pSoldier, pExplosive, exp->sSubsequent , exp->fRecompileMovementCosts , exp->sWoundAmt , exp->sBreathAmt , exp->ubAttackerID , TRUE );

				// can use DishOutGasDamage() as it is dependant on the local state of the gas cloud which is not always in sync
				// but we have the definite results of damage on a merc, so :
				pSoldier->SoldierTakeDamage( ANIM_STAND, exp->sWoundAmt, exp->sBreathAmt, TAKE_DAMAGE_GAS, NOBODY, NOWHERE, 0, TRUE );
			}
			else if (exp->ubDamageFunc == 2)
			{
				DamageSoldierFromBlast( exp->ubSoldierID , exp->ubAttackerID , exp->sBombGridNo , exp->sWoundAmt , exp->sBreathAmt , exp->uiDist , exp->usItem , exp->sSubsequent , TRUE);
			}
		}
	}
	else
	{
#ifdef JA2BETAVERSION
		char tmpMsg[128];
		sprintf(tmpMsg,"ERROR! - Invalid Soldier pointer from ubID %i in recieveEXPLOSIONDAMAGE()",exp->ubAttackerID);
		//ScreenMsg(FONT_RED,MSG_MPSYSTEM,tmpMsg);
		MPDebugMsg(tmpMsg);
#endif
	}
}


void send_bullet(  BULLET * pBullet,UINT16 usHandItem )
{	
	netb_struct netb;
	netb.net_bullet=*pBullet;
	netb.usHandItem=usHandItem;

	//ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"Sent Bullet Id: %d",pBullet->iBullet);

	if(pBullet->ubTargetID < 20)netb.net_bullet.ubTargetID = netb.net_bullet.ubTargetID+ubID_prefix;
	if(pBullet->ubFirerID < 20)netb.net_bullet.ubFirerID = netb.net_bullet.ubFirerID+ubID_prefix;
				
	
	client->RPC("sendBULLET",(const char*)&netb, (int)sizeof(netb_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void recieveBULLET(RPCParameters *rpcParameters)
{
		netb_struct* netb = (netb_struct*)rpcParameters->input;

		INT32 net_iBullet=netb->net_bullet.iBullet;

		SOLDIERTYPE * pFirer=MercPtrs[ netb->net_bullet.ubFirerID ];

		INT8 bTeam=pFirer->bTeam;

		INT32 iBullet;
		BULLET * pBullet;

		iBullet = CreateBullet( netb->net_bullet.ubFirerID, 0, netb->net_bullet.usFlags,netb->usHandItem );
		
		if (iBullet == -1)
		{

#ifdef BETAVERSION
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"Failed to create bullet");		
#endif

		}

		//add bullet to bullet table for translation
		bTable[bTeam][net_iBullet].remote_id = net_iBullet;
		bTable[bTeam][net_iBullet].local_id = iBullet;
		
		pBullet = GetBulletPtr( iBullet );

		//ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"Created Bullet Id: %d",iBullet);		

		pBullet->fCheckForRoof=netb->net_bullet.fCheckForRoof;
		pBullet->qIncrX=netb->net_bullet.qIncrX;
		pBullet->qIncrY=netb->net_bullet.qIncrY;
		pBullet->qIncrZ=netb->net_bullet.qIncrZ;
		pBullet->sHitBy=netb->net_bullet.sHitBy;
		pBullet->ddHorizAngle=netb->net_bullet.ddHorizAngle;
		pBullet->fAimed=netb->net_bullet.fAimed;
		pBullet->ubItemStatus=netb->net_bullet.ubItemStatus;
		pBullet->qCurrX=netb->net_bullet.qCurrX;
		pBullet->qCurrY=netb->net_bullet.qCurrY;
		pBullet->qCurrZ=netb->net_bullet.qCurrZ;
		pBullet->iImpact=netb->net_bullet.iImpact;
		pBullet->iRange=netb->net_bullet.iRange;
		pBullet->sTargetGridNo=netb->net_bullet.sTargetGridNo;
		pBullet->bStartCubesAboveLevelZ=netb->net_bullet.bStartCubesAboveLevelZ;
		pBullet->bEndCubesAboveLevelZ=netb->net_bullet.bEndCubesAboveLevelZ;
		pBullet->iDistanceLimit=netb->net_bullet.iDistanceLimit;

		FireBullet( pFirer, pBullet, FALSE );
}

void send_changestate (EV_S_CHANGESTATE * SChangeState)
{
	EV_S_CHANGESTATE	new_state;

	memcpy( &new_state , SChangeState, sizeof( EV_S_CHANGESTATE ));

	if(new_state.usSoldierID < 20)
		new_state.usSoldierID = new_state.usSoldierID+ubID_prefix;
	
	client->RPC("sendSTATE",(const char*)&new_state, (int)sizeof(EV_S_CHANGESTATE)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveSTATE(RPCParameters *rpcParameters)
{
	EV_S_CHANGESTATE*	new_state = (EV_S_CHANGESTATE*)rpcParameters->input;

	SOLDIERTYPE * pSoldier=MercPtrs[ new_state->usSoldierID ];

	if(pSoldier->bActive)
	{
		// Start first AID
		if(new_state->usNewState==START_AID)
		{
			pSoldier->EVENT_InternalSetSoldierPosition( new_state->sXPos, new_state->sYPos ,FALSE, FALSE, FALSE );
			pSoldier->ChangeSoldierStance( ANIM_CROUCH );
			pSoldier->EVENT_SetSoldierDirection(	new_state->usNewDirection );

		}
		// Start cutting fence
		else if (new_state->usNewState==CUTTING_FENCE)
		{
			// The usTargetGridNo holds the GridNo of the fence tile
			pSoldier->sTargetGridNo = new_state->usTargetGridNo;

			pSoldier->EVENT_InternalSetSoldierPosition( new_state->sXPos, new_state->sYPos ,FALSE, FALSE, FALSE );
			pSoldier->ChangeSoldierStance( ANIM_CROUCH );
			pSoldier->EVENT_SetSoldierDirection(	new_state->usNewDirection );
		}

		pSoldier->EVENT_InitNewSoldierAnim( new_state->usNewState, new_state->usStartingAniCode, new_state->fForce );
	}
}

void send_death( SOLDIERTYPE *pSoldier )
{
	death_struct nDeath;
	nDeath.soldier_id=pSoldier->ubID;
	nDeath.attacker_id=pSoldier->ubAttackerID;

	// Translate soldier id for other clients if the soldier was one of ours
	if(pSoldier->ubID<20)nDeath.soldier_id=nDeath.soldier_id+ubID_prefix;
	
	// if soldier died from bleeding
	if(pSoldier->ubAttackerID==NULL || pSoldier->ubAttackerID == NOBODY)
	{
		if (pSoldier->ubPreviousAttackerID != NOBODY && pSoldier->ubPreviousAttackerID != NULL)
			nDeath.attacker_id = pSoldier->ubPreviousAttackerID;
		else if (pSoldier->ubNextToPreviousAttackerID != NOBODY && pSoldier->ubNextToPreviousAttackerID != NULL)
			nDeath.attacker_id = pSoldier->ubNextToPreviousAttackerID;
	}

	SOLDIERTYPE * pAttacker=MercPtrs[ nDeath.attacker_id ];
	INT8 pA_bTeam;
	CHAR16	pA_name[ 10 ];
	INT8 pS_bTeam;
	CHAR16	pS_name[ 10 ];

	// OJW - 20081222
	// save stats
	if(pAttacker)
	{
		// if attacker was one of our own mercs, use the last hostile attacker as the killer if there is one
		if (pAttacker->bTeam == pSoldier->bTeam && pSoldier->ubPreviousAttackerID != NULL && pSoldier->ubPreviousAttackerID != NOBODY)
		{
			pAttacker=MercPtrs[ pSoldier->ubPreviousAttackerID ];
			// check if the new attacker was also a friendly...
			if (pAttacker->bTeam == pSoldier->bTeam && pSoldier->ubNextToPreviousAttackerID != NULL && pSoldier->ubNextToPreviousAttackerID != NOBODY)
				pAttacker=MercPtrs[ pSoldier->ubNextToPreviousAttackerID ];
			// if its still a friendly, use the original attacker id...for posterity
			// guy must snore too loudly if all his mates wanna kill him :)
			if (pAttacker->bTeam == pSoldier->bTeam && pSoldier->ubAttackerID != NULL && pSoldier->ubAttackerID != NOBODY)
				pAttacker = MercPtrs[ pSoldier->ubAttackerID ];

			nDeath.attacker_id = pAttacker->ubID;
		}
	
		// Translate attacker id for other clients if attacker was one of ours
		if(pAttacker->ubID<20)nDeath.attacker_id=pAttacker->ubID+ubID_prefix;

		pA_bTeam=pAttacker->bTeam;
		memcpy(pA_name,pAttacker->name,sizeof(CHAR16)*10);

		if (pA_bTeam==1 && cGameType==MP_TYPE_COOP)
		{
			// CO-OP Kill by an enemy ai
			pA_bTeam = 5; // the server subtracts 1 from these numbers to score in scoreboard, AI is index 4
		}
		else
		{
			// Any mode, kill by a players merc
			if(pA_bTeam>5)
				pA_bTeam=pA_bTeam-5;

			if(pA_bTeam==0)
				pA_bTeam= CLIENT_NUM;
		}
	}

	if(pSoldier)
	{
		pS_bTeam=pSoldier->bTeam;
		memcpy(pS_name,pSoldier->name,sizeof(CHAR16)*10);
		if(pS_bTeam==1 && cGameType==MP_TYPE_COOP) 
		{
			// CO-OP Death of an enemy ai
			pS_bTeam = 5; // the server subtracts 1 from these numbers to score in scoreboard, AI is index 4
		}
		else
		{
			// Any mode death of a players merc
			if(pS_bTeam>5)
				pS_bTeam=pS_bTeam-5;

			if(pS_bTeam==0)
				pS_bTeam=CLIENT_NUM;
		}
	}

	nDeath.attacker_team = pA_bTeam;
	nDeath.soldier_team = pS_bTeam;

	// notify other clients of death
	client->RPC("sendDEATH",(const char*)&nDeath, (int)sizeof(death_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	
	// print kill notice to screen	
	if (pSoldier->bTeam==1)  
		ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, MPClientMessage[67]);	
	else  
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[28],pS_name,(pS_bTeam),client_names[(pS_bTeam-1)],pA_name,(pA_bTeam),client_names[(pA_bTeam-1)] );

#ifdef JA2BETAVERSION
	char s_name[10];
	char a_name[10];
	WideCharToMultiByte(CP_UTF8,0,pS_name,-1, s_name,10,NULL,NULL);
	WideCharToMultiByte(CP_UTF8,0,pA_name,-1, a_name,10,NULL,NULL);
	
	if (pSoldier->bTeam==1) 
		MPDebugMsg( String ( "MPDEBUG SEND - Enemy AI #%d was killed by ('%s' - %d) (client %d - '%s')\n",nDeath.soldier_id,a_name,nDeath.attacker_id,pA_bTeam,client_names[pA_bTeam-1]) );
	else if (pAttacker->bTeam==1) 
		MPDebugMsg( String ( "MPDEBUG SEND - '%s' (client %d - '%S') was killed by '%s' (client %d - '%s')\n",s_name,pS_bTeam,client_names[(pS_bTeam-1)],a_name,pA_bTeam,"Queens Army") );
	else 
		MPDebugMsg( String ( "MPDEBUG SEND - '%s' (client %d - '%S') was killed by '%s' (client %d - '%s')\n",s_name,pS_bTeam,client_names[(pS_bTeam-1)],a_name,pA_bTeam,client_names[(pA_bTeam-1)]) );
#endif
}

void recieveDEATH (RPCParameters *rpcParameters)
{
	death_struct* nDeath = (death_struct*)rpcParameters->input;
	SOLDIERTYPE * pSoldier=MercPtrs[ nDeath->soldier_id ];

	UINT16 ubAttackerID;
	if((nDeath->attacker_id >= ubID_prefix) && (nDeath->attacker_id < (ubID_prefix+6)))
		ubAttackerID = (nDeath->attacker_id - ubID_prefix);
	else
		ubAttackerID = nDeath->attacker_id;

	SOLDIERTYPE * pAttacker=MercPtrs[ ubAttackerID ];
	INT8 pA_bTeam;
	CHAR16	pA_name[ 10 ];
	INT8 pS_bTeam;
	CHAR16	pS_name[ 10 ];

	if(pAttacker)
	{
		pA_bTeam=pAttacker->bTeam;
		memcpy(pA_name,pAttacker->name,sizeof(CHAR16)*10);
		
		if(pA_bTeam>5)
			pA_bTeam=pA_bTeam-5;
		if(pA_bTeam==0)
			pA_bTeam=CLIENT_NUM;
	}

	if(pSoldier)
	{
		pS_bTeam=pSoldier->bTeam;
		memcpy(pS_name,pSoldier->name,sizeof(CHAR16)*10);
		
		if(pS_bTeam>5)
			pS_bTeam=pS_bTeam-5;
		if(pS_bTeam==0)
			pS_bTeam=CLIENT_NUM;
	}
			
	if(pSoldier->bActive)
	{
		pSoldier->usAnimState=50;

		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"made merc corpse/dead");	
		#endif

		RemoveManAsTarget(pSoldier);
		TurnSoldierIntoCorpse( pSoldier, TRUE, TRUE );
	

		if ( CheckForEndOfBattle( FALSE ) )
		{
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[72]);
		}

		if (pSoldier->bTeam==1)  
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, MPClientMessage[67]);	
		else 
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[28],pS_name,(pS_bTeam),client_names[(pS_bTeam-1)],pA_name,(pA_bTeam),client_names[(pA_bTeam-1)] );
	}
	else
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, L"merc already corpse/dead");	
		#endif

		if (pSoldier->bTeam==1) 
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, MPClientMessage[67]);	
		
		CheckForEndOfBattle( FALSE );
	}

#ifdef JA2BETAVERSION
	char s_name[10];
	char a_name[10];
	WideCharToMultiByte(CP_UTF8,0,pS_name,-1, s_name,10,NULL,NULL);
	WideCharToMultiByte(CP_UTF8,0,pA_name,-1, a_name,10,NULL,NULL);
	
	if (pSoldier->bTeam==1) 
		MPDebugMsg( String ( "MPDEBUG RECV - Enemy AI #%d was killed by ('%s' - #%d) (client %d - '%s')\n",nDeath->soldier_id,a_name,nDeath->attacker_id,pA_bTeam,client_names[pA_bTeam-1]) );
	else if (pAttacker->bTeam==1) 
		MPDebugMsg( String ( "MPDEBUG RECV - '%s' (client %d - '%s') was killed by '%s' (client %d - '%s')\n",s_name,pS_bTeam,client_names[(pS_bTeam-1)],a_name,pA_bTeam,"Queens Army") );
	else 
		MPDebugMsg( String ( "MPDEBUG RECV - '%s' (client %d - '%s') was killed by '%s' (client %d - '%s')\n",s_name,pS_bTeam,client_names[(pS_bTeam-1)],a_name,pA_bTeam,client_names[(pA_bTeam-1)]) );
#endif
}

void send_hitstruct(EV_S_STRUCTUREHIT	*	SStructureHit)
{
	EV_S_STRUCTUREHIT struct_hit;
	memcpy( &struct_hit , SStructureHit, sizeof( EV_S_STRUCTUREHIT ));
	if(SStructureHit->ubAttackerID <20)struct_hit.ubAttackerID = SStructureHit->ubAttackerID+ubID_prefix;
			
	client->RPC("sendhitSTRUCT",(const char*)&struct_hit, (int)sizeof(EV_S_STRUCTUREHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void send_hitwindow(EV_S_WINDOWHIT * SWindowHit)
{
	EV_S_WINDOWHIT window_hit;
	memcpy( &window_hit , SWindowHit, sizeof( EV_S_WINDOWHIT ));
	
	if(SWindowHit->ubAttackerID <20)
		window_hit.ubAttackerID = SWindowHit->ubAttackerID+ubID_prefix;
			
	client->RPC("sendhitWINDOW",(const char*)&window_hit, (int)sizeof(EV_S_WINDOWHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void send_miss(EV_S_MISS * SMiss)
{
	EV_S_MISS shot_miss;
	memcpy( &shot_miss , SMiss, sizeof( EV_S_MISS ));
	
	if(SMiss->ubAttackerID <20)
		shot_miss.ubAttackerID = SMiss->ubAttackerID+ubID_prefix;
			
	client->RPC("sendMISS",(const char*)&shot_miss, (int)sizeof(EV_S_MISS)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recievehitSTRUCT  (RPCParameters *rpcParameters)
{
	EV_S_STRUCTUREHIT* struct_hit = (EV_S_STRUCTUREHIT*)rpcParameters->input;
	SOLDIERTYPE *pSoldier = MercPtrs[ struct_hit->ubAttackerID ];
	INT8 bTeam=pSoldier->bTeam;
	INT32 iBullet = bTable[bTeam][struct_hit->iBullet].local_id;

	if(struct_hit->fStopped)
		StopBullet( iBullet );
	
	StructureHit( iBullet, struct_hit->usWeaponIndex, struct_hit->bWeaponStatus, struct_hit->ubAttackerID, struct_hit->sXPos, struct_hit->sYPos, struct_hit->sZPos, struct_hit->usStructureID, struct_hit->iImpact, struct_hit->fStopped );
	
	if(struct_hit->fStopped)
		RemoveBullet(iBullet);
}

void recievehitWINDOW  (RPCParameters *rpcParameters)
{
	EV_S_WINDOWHIT* window_hit = (EV_S_WINDOWHIT*)rpcParameters->input;
	WindowHit( window_hit->sGridNo, window_hit->usStructureID, window_hit->fBlowWindowSouth, window_hit->fLargeForce );
}

void recieveMISS  (RPCParameters *rpcParameters)
{
	EV_S_MISS* shot_miss = (EV_S_MISS*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ shot_miss->ubAttackerID ];
	INT8 bTeam=pSoldier->bTeam;
	INT32 iBullet = bTable[bTeam][shot_miss->iBullet].local_id;

	ShotMiss( shot_miss->ubAttackerID, iBullet );
}

BOOLEAN check_status (void)// any 'enemies' and clients left to fight ??
{
	SOLDIERTYPE *pSoldier;
	int cnt;
	int soldiers= 0 ;
	int numActiveSides = 0;
	
	int dm_teams[4] = {0 , 0 , 0 , 0};

	for(int x=0;x < MAXTEAMS; x++)
	{
		soldiers=0;

		for(cnt = gTacticalStatus.Team[ x ].bFirstID;cnt <= gTacticalStatus.Team[ x ].bLastID; cnt++)
		{
			pSoldier = MercPtrs[ cnt ];
			if(pSoldier->stats.bLife >= OKLIFE && pSoldier->bActive && pSoldier->bInSector)
			{
				soldiers++;
			}
		}

		if(soldiers>0)
		{
			gTacticalStatus.Team[ x ].bTeamActive=1;
			gTacticalStatus.Team[ x ].bMenInSector=soldiers;

			if (cGameType==MP_TYPE_TEAMDEATMATCH && (x==0 || x>5))
			{
				// store the number of active DM teams, and number of players per team still alive
				int cl_num = CLIENT_NUM; // 1 based
				if (x>5) cl_num = x - 5;
				dm_teams[ client_teams[ cl_num - 1 ] ]++;
			}
			else if (cGameType != MP_TYPE_TEAMDEATMATCH)
			{
				// count number of active teams
				numActiveSides++;
			}
		}
		else
		{
			gTacticalStatus.Team[ x ].bTeamActive=0;
			gTacticalStatus.Team[x].bMenInSector=0;
		}
	}

	if( (gTacticalStatus.Team[ 0 ].bTeamActive == 0) && wiped==0)//server's team has been knocked out
	{		
		wiped=1;
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[40] );
		if(!cDisableSpectatorMode)
		{
			gTacticalStatus.uiFlags |= SHOW_ALL_MERCS;//hayden
			ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, MPClientMessage[41] );
		}
		else 
		{
			ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPClientMessage[73]);
		}

		teamwiped();
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[42] );
	}

	if (cGameType == MP_TYPE_DEATHMATCH)
	{
		// check game end for DeathMatch
		return (numActiveSides > 1);
	}
	else if (cGameType == MP_TYPE_TEAMDEATMATCH)
	{
		// check game end for Team Deathmatch
		// count how many active deathmatch teams are alive (two players could be alive but might be on the same TEAM)
		for (int i=0; i < 4; i++)
		{
			if (dm_teams[i] > 0)
			{
				numActiveSides++;
			}
		}

		return (numActiveSides > 1);
	}
	else if (cGameType == MP_TYPE_COOP)
	{
		// check for game end for CO-OP
		// If any player team is alive && the number of enemies > 0 then continue game (true), else quit (false) 
		return ((gTacticalStatus.Team[ 0 ].bTeamActive==1 || gTacticalStatus.Team[ 6 ].bTeamActive==1 || gTacticalStatus.Team[ 7 ].bTeamActive==1 || gTacticalStatus.Team[ 8 ].bTeamActive==1 || gTacticalStatus.Team[ 9 ].bTeamActive==1  )&& NumEnemyInSector() > 0);
	}

	// dont stop the game by default
	return true;	
}

void UpdateSoldierToNetwork ( SOLDIERTYPE *pSoldier )
{
	//this send stats to other clients at intervals
	UINT8 id = pSoldier->ubID;
	UINT32 time = GetJA2Clock();

	if(id < 20 || (is_server && id <120))
	{
		if(pSoldier->usLastUpdateTime==0)
		{
			pSoldier->usLastUpdateTime = time;
		}
		if((time - (pSoldier->usLastUpdateTime)) > 2000 && pSoldier->stats.bLife!=0)
		{
			pSoldier->usLastUpdateTime = time;

			EV_S_UPDATENETWORKSOLDIER SUpdateNetworkSoldier;

			SUpdateNetworkSoldier.usSoldierID=pSoldier->ubID;
			
			if(pSoldier->ubID < 20)
				SUpdateNetworkSoldier.usSoldierID=pSoldier->ubID+ubID_prefix;
			
			SUpdateNetworkSoldier.sAtGridNo=pSoldier->sGridNo;
			SUpdateNetworkSoldier.bBreath=pSoldier->bBreath;
			SUpdateNetworkSoldier.ubDirection=pSoldier->ubDirection;

			SUpdateNetworkSoldier.bLife=pSoldier->stats.bLife;
			SUpdateNetworkSoldier.bBleeding=pSoldier->bBleeding;
			SUpdateNetworkSoldier.ubNewStance= gAnimControl[ pSoldier->usAnimState ].ubEndHeight;
			
			if((gTacticalStatus.ubTopMessageType == PLAYER_TURN_MESSAGE || gTacticalStatus.ubTopMessageType == PLAYER_INTERRUPT_MESSAGE || ((gTacticalStatus.ubTopMessageType == COMPUTER_INTERRUPT_MESSAGE || gTacticalStatus.ubTopMessageType == COMPUTER_TURN_MESSAGE )&& is_server)) && (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT))//update progress bar, not supporting coop yet...
			{
				SUpdateNetworkSoldier.usTactialTurnLimitCounter = gTacticalStatus.usTactialTurnLimitCounter;
				SUpdateNetworkSoldier.usTactialTurnLimitMax = gTacticalStatus.usTactialTurnLimitMax;
			}
			else
				SUpdateNetworkSoldier.usTactialTurnLimitCounter = 9999;
			
			client->RPC("updatenetworksoldier",(const char*)&SUpdateNetworkSoldier, (int)sizeof(EV_S_UPDATENETWORKSOLDIER)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}
}

void UpdateSoldierFromNetwork  (RPCParameters *rpcParameters)
{
	EV_S_UPDATENETWORKSOLDIER* SUpdateNetworkSoldier = (EV_S_UPDATENETWORKSOLDIER*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ SUpdateNetworkSoldier->usSoldierID ];
	pSoldier->bBreath=SUpdateNetworkSoldier->bBreath;
	pSoldier->stats.bLife=SUpdateNetworkSoldier->bLife;

	INT16  sCellX, sCellY;
	sCellX = CenterX( SUpdateNetworkSoldier->sAtGridNo );
	sCellY = CenterY( SUpdateNetworkSoldier->sAtGridNo );	

	if( pSoldier->sGridNo != SUpdateNetworkSoldier->sAtGridNo)
	{
		pSoldier->EVENT_InternalSetSoldierPosition( sCellX, sCellY ,FALSE, FALSE, FALSE );//new syncing call to correct network lag/drift
	}

	if(pSoldier->ubDirection != SUpdateNetworkSoldier->ubDirection)
	{
		pSoldier->EVENT_SetSoldierDesiredDirection( SUpdateNetworkSoldier->ubDirection );
	}
	if(gAnimControl[ pSoldier->usAnimState ].ubEndHeight != SUpdateNetworkSoldier->ubNewStance && pSoldier->bCollapsed != TRUE)
	{
		pSoldier->ChangeSoldierStance( SUpdateNetworkSoldier->ubNewStance );
	}
		
	pSoldier->bBleeding=SUpdateNetworkSoldier->bBleeding;

	if( (SUpdateNetworkSoldier->usTactialTurnLimitCounter != 9999) && (gTacticalStatus.ubTopMessageType != PLAYER_TURN_MESSAGE) && (gTacticalStatus.ubTopMessageType != PLAYER_INTERRUPT_MESSAGE))
	{
		gTacticalStatus.usTactialTurnLimitCounter = SUpdateNetworkSoldier->usTactialTurnLimitCounter;
		gTacticalStatus.usTactialTurnLimitMax = SUpdateNetworkSoldier->usTactialTurnLimitMax;
	}
}


void kick_player (void)
{
	if(is_server)
	{		
		CHAR16 Cmsg[255];

		if (cMaxClients == 2)
			swprintf(Cmsg, MPClientMessage[74], client_names[1],"<?>","<?>");
		else if (cMaxClients == 3)
			swprintf(Cmsg, MPClientMessage[74], client_names[1],client_names[2],"<?>");
		else 
			swprintf(Cmsg, MPClientMessage[74], client_names[1],client_names[2],client_names[3]);
		
		SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
		DoMessageBox( MSG_BOX_BASIC_STYLE, Cmsg,  guiCurrentScreen, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS | MSG_BOX_FLAG_USE_CENTERING_RECT, kick_callback,  &CenterRect );
	}
	else	
		ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[22] );
	
}

void kick_callback (UINT8 ubResult)
{	
	if (is_server)
	{
		// Pressed '1'
		if(ubResult ==1)
		{
			// WANNE: Nothing should happen			
		}
		else 
		{
			if (ubResult <= cMaxClients)
			{
				kickR kick;
				kick.ubResult=ubResult+5;
				
				client->RPC("Snull_team",(const char*)&kick, (int)sizeof(kickR)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

				// If the team that should be kicked has the turn, give the turn to the server
				if (gTacticalStatus.ubCurrentTeam == kick.ubResult)
				{					
					EndTurn(0);
				}
			}
			else
			{
				// The client to which we should give the turn doe not exists. Do nothinig!
			}
		}		
	}
}

void null_team (RPCParameters *rpcParameters)
{
	kickR* kick = (kickR*)rpcParameters->input;
	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[29],(kick->ubResult-5),client_names[kick->ubResult-6] );
	int fID = gTacticalStatus.Team[ kick->ubResult ].bFirstID;
	int lID = gTacticalStatus.Team[ kick->ubResult ].bLastID;
	
	if(kick->ubResult==netbTeam)
		fID=0,lID=19;
	
	int cnt;
	for ( cnt=fID ; cnt <= lID; cnt++ )
	{
		TacticalRemoveSoldier( cnt );
	}

	if (kick->ubResult==netbTeam)
	{		
		gTacticalStatus.uiFlags |= SHOW_ALL_MERCS;//hayden
		ScreenMsg( FONT_YELLOW, MSG_MPSYSTEM, MPClientMessage[41] );
	}	
}

void overide_turn (void)
{
	if(is_server)
	{
		//manual overide command for server
		CHAR16 Cmsg[255];
		
		if (cMaxClients == 2)
			swprintf(Cmsg, MPClientMessage[30], client_names[1],"<?>","<?>");
		else if (cMaxClients == 3)
			swprintf(Cmsg, MPClientMessage[30], client_names[1],client_names[2],"<?>");
		else 
			swprintf(Cmsg, MPClientMessage[30], client_names[1],client_names[2],client_names[3]);
			
		SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
		
		DoMessageBox( MSG_BOX_BASIC_STYLE, Cmsg,  guiCurrentScreen, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS | MSG_BOX_FLAG_USE_CENTERING_RECT | MSG_BOX_FLAG_OK, turn_callback,  &CenterRect );
	}
	else	
		ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[22] );
}

void turn_callback (UINT8 ubResult)
{
	if(is_server)
	{				
		// Pressed '1'
		if(ubResult ==1)
		{
			// WANNE: Nothing should happen. Do not give the turn to the server!
			//EndTurn( 0 );
		}
		else 
		{
			if (ubResult <= cMaxClients)
			{
				ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[31],ubResult );
	
				if(!( gTacticalStatus.uiFlags & INCOMBAT ))
				{
					gTacticalStatus.uiFlags |= INCOMBAT;
				}

				EndTurn( ubResult+5 );
			}
			else
			{
				// The client to which we should give the turn doe not exists. Do nothinig!
			}
		}
	}
}

void send_fireweapon (EV_S_FIREWEAPON  *SFireWeapon)
{
	EV_S_FIREWEAPON sFire;

	if(SFireWeapon->usSoldierID < 20)
		sFire.usSoldierID = (SFireWeapon->usSoldierID)+ubID_prefix;
	else
		sFire.usSoldierID = SFireWeapon->usSoldierID;

	sFire.sTargetGridNo=SFireWeapon->sTargetGridNo;
	sFire.bTargetCubeLevel=SFireWeapon->bTargetCubeLevel;
	sFire.bTargetLevel=SFireWeapon->bTargetLevel;

	client->RPC("sendFIREW",(const char*)&sFire, (int)sizeof(EV_S_FIREWEAPON)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieve_fireweapon (RPCParameters *rpcParameters)
{
	EV_S_FIREWEAPON* SFireWeapon = (EV_S_FIREWEAPON*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ SFireWeapon->usSoldierID ];

	pSoldier->sTargetGridNo = SFireWeapon->sTargetGridNo;
	pSoldier->bTargetLevel = SFireWeapon->bTargetLevel;
	pSoldier->bTargetCubeLevel = SFireWeapon->bTargetCubeLevel;
	FireWeapon( pSoldier, SFireWeapon->sTargetGridNo  );
}

void send_door ( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fNoAnimations )
{
	if((is_server && pSoldier->ubID<120) || (!is_server && is_client && pSoldier->ubID<20) || (!is_server && !is_client) )
	{
		doors sDoor;
		sDoor.ubID=pSoldier->ubID;
		sDoor.sGridNo=sGridNo;
		sDoor.fNoAnimations=fNoAnimations;
		
		client->RPC("sendDOOR",(const char*)&sDoor, (int)sizeof(doors)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieve_door (RPCParameters *rpcParameters)
{
	doors* sDoor = (doors*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ sDoor->ubID ];
	BOOLEAN fNoAnimations = FALSE;

	if ( !AllMercsLookForDoor( sDoor->sGridNo, FALSE ) )//check for los
	{
		fNoAnimations = TRUE;
	}

	HandleDoorChangeFromGridNo( pSoldier, sDoor->sGridNo, fNoAnimations );
}

void recieveCHATMSG(RPCParameters* rpcParameters)
{
	chat_msg* cmsg = (chat_msg*)rpcParameters->input;

	if (cGameType==MP_TYPE_TEAMDEATMATCH && cmsg->bToAll == false)
	{
		// If Team deathmatch && msg is allies only
		if (client_teams[cmsg->client_num-1] == TEAM)
		{
			// only display on an ally client
			ChatLogMessage( CHAT_FONT_COLOR, MSG_CHAT, cmsg->msg);
		}
	}
	else
	{
		// display to all clients
		ChatLogMessage( CHAT_FONT_COLOR, MSG_CHAT, cmsg->msg);
	}
}

/// OJW - 20081223
// recieveDISCONNECT - Handle disconnection
void recieveDISCONNECT(RPCParameters* rpcParameters)
{
	// for starters - we shouldnt get a message for ourselves :)
	int cl_num = (int) *rpcParameters->input; // cl_num starts at 1

	wchar_t szPlayerName[30];
	memset(szPlayerName,0,30*sizeof(wchar_t));
	mbstowcs( szPlayerName,client_names[cl_num-1],30);
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[47], szPlayerName );

	// clear our records from this client
	memset(client_names[cl_num-1],NULL,sizeof(char)*30);
	memset(&client_ready[cl_num-1],0,sizeof(int));
	memset(&client_teams[cl_num-1],0,sizeof(int));

	if (guiCurrentScreen == MAP_SCREEN && !(gTacticalStatus.uiFlags & INCOMBAT))
	{
		// in the map screen and not in combat
		// refresh player list to remove from the game
		fDrawCharacterList = true; // set the character list to be redrawn
		fTeamPanelDirty = true; // redraw the background
	}
	else if (guiCurrentScreen == GAME_SCREEN) // <TODO> get a more valid check that the game is in progress here
	{
		// in tactical screen and in combat
		// kill the dead clients mercs out of the game

		UINT8 iNetbTeam = (cl_num)+5;
		UINT8 iubID_prefix = gTacticalStatus.Team[ iNetbTeam ].bFirstID;//over here now

		// kill any alive soldiers for the disconnected team
		SOLDIERTYPE *pTeamSoldier;
		INT32				cnt = 0;

		for ( pTeamSoldier = Menptr, cnt = 0; cnt < TOTAL_SOLDIERS; pTeamSoldier++, cnt++ )
		{
			if ( pTeamSoldier->bActive && pTeamSoldier->bInSector  && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_DEAD ) )
			{
				// Checkf for any more bacguys
				if ( !pTeamSoldier->aiData.bNeutral && (pTeamSoldier->bTeam == iNetbTeam ) )
				{
					// KIll......
					pTeamSoldier->SoldierTakeDamage( ANIM_CROUCH, pTeamSoldier->stats.bLife, 100, TAKE_DAMAGE_BLOODLOSS, NOBODY, NOWHERE, 0, TRUE );
				}
			}
		}

		// if it was that teams turn then end it
		if(is_server)
		{
			if(gTacticalStatus.ubCurrentTeam==iNetbTeam)EndTurn( iNetbTeam+1 );	
		}
	}
}

// OJW - 20090507
// this function stores a reason from the server that we were disconnected
void recieveDISCONNECTREASON(RPCParameters *rpcParameters )
{
	CHAR16* reason = (CHAR16*)rpcParameters->input;
	wcscpy(gszDisconnectReason,reason);

	is_connected=false;
	auto_retry = false;
}

void disconnected_callback(UINT8 ubResult)
{
	if (iDisconnectedScreen == MAP_SCREEN)
	{
		// clean up all resources and exit from map to main menu
		RequestTriggerExitFromMapscreen(MAP_EXIT_TO_MAINMENU);
		// game is restarted in HandleExitsFromMapScreen()
	}
	else if (iDisconnectedScreen == GAME_SCREEN)
	{
		// clean up all resources and exit from tactical to main menu
		LeaveTacticalScreen(MAINMENU_SCREEN);
	}
	else if (iDisconnectedScreen == OPTIONS_SCREEN)
	{
		// Re-initialise the game
		ReStartingGame();
		// clean up all resources and exit from options to main menu
		SetOptionsExitScreen( MAINMENU_SCREEN );
	}
	else if (iDisconnectedScreen == LAPTOP_SCREEN)
	{
		// Re-initialise the game
		ReStartingGame();
		// clean up all resources and exit from laptop to main menu
		SetPendingNewScreen(MAINMENU_SCREEN); // Laptop screen is always cleaned up on screen change in gameloop
	}
	else if (iDisconnectedScreen == MP_CONNECT_SCREEN)
	{
		// Re-initialise the game
		ReStartingGame();
		// else dont clean "everything" but still exit to main menu
		SetPendingNewScreen(MP_JOIN_SCREEN);
	}
	else
	{
		// Re-initialise the game
		ReStartingGame();
		// else dont clean "everything" but still exit to main menu
		SetPendingNewScreen(MAINMENU_SCREEN);
	}
}

// Gracefully handle self-disconnection of the client by Dropout
void HandleClientConnectionLost()
{
	if (guiCurrentScreen != MP_SCORE_SCREEN)
	{
		// cleanup client
		client_disconnect();
		auto_retry = false;

		if (is_server)
			server_disconnect();

		// connection lost, let user know via popup then quit to main menu
		iDisconnectedScreen = guiCurrentScreen;
		SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };

		if (wcscmp(gszDisconnectReason,L"")==0)
		{
			UINT32 giMPHMessageBox = DoMessageBox(	MSG_BOX_BASIC_STYLE,	MPClientMessage[48],	guiCurrentScreen, ( UINT16 ) ( MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT ),disconnected_callback,	&CenteringRect );
		}
		else
		{
			UINT32 giMPHMessageBox = DoMessageBox(	MSG_BOX_BASIC_STYLE,	gszDisconnectReason,	guiCurrentScreen, ( UINT16 ) ( MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT ),disconnected_callback,	&CenteringRect );
		}

	}
	else
	{
		// Tell the score screen it can continue
		gfMPSScoreScreenCanContinue = TRUE;
	}
}

void sendRT(void)
{
	if(!requested)
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[32] );
		requested=true;
		real_struct rData;
		rData.bteam=netbTeam;

		client->RPC("sendREAL",(const char*)&rData, (int)sizeof(real_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void gotoRT(RPCParameters *rpcParameters)
{
	getReal=true;//MAY NOT BE NEEDED ANY MORE

	gTacticalStatus.bConsNumTurnsNotSeen = 0;
	gTacticalStatus.ubCurrentTeam = OUR_TEAM;
	guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
	ExitCombatMode();
	fInterfacePanelDirty = DIRTYLEVEL2;

	if ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[34] );
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[33] );
	}
	
	getReal=false;
}

void startCombat(UINT8 ubStartingTeam)
{
	sc_struct data;
	
	if(ubStartingTeam==0)
		ubStartingTeam=netbTeam;

	data.ubStartingTeam=ubStartingTeam;

	client->RPC("startCOMBAT",(const char*)&data, (int)sizeof(sc_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void teamwiped ( void )
{
	isOwnTeamWipedOut = true;

	sc_struct data;
	data.ubStartingTeam=netbTeam;

	client->RPC("sendWIPE",(const char*)&data, (int)sizeof(sc_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

	if (is_server)
	{
		// end the co-op game if all player teams have wiped
		if (cGameType==MP_TYPE_COOP)
		{
			iTeamsWiped++;

			if (iTeamsWiped >= cMaxClients)
				game_over();
		}
	}
}

void recieve_wipe (RPCParameters *rpcParameters)
{
	sc_struct* data = (sc_struct*)rpcParameters->input;
	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[75], data->ubStartingTeam );
	if(is_server)
	{
		if(gTacticalStatus.ubCurrentTeam==data->ubStartingTeam)EndTurn( data->ubStartingTeam+1 );	

		// end the co-op game if all player teams have wiped
		if (cGameType==MP_TYPE_COOP)
		{
			iTeamsWiped++;
			if (iTeamsWiped >= cMaxClients)
				game_over();
		}
	}
}

void send_heal (SOLDIERTYPE *pSoldier )
{
	heal data;
	data.ubID=pSoldier->ubID;
	data.bLife=pSoldier->stats.bLife;
	data.bBleeding=pSoldier->bBleeding;

	client->RPC("sendHEAL",(const char*)&data, (int)sizeof(heal)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieve_heal (RPCParameters *rpcParameters)
{
	heal* data = (heal*)rpcParameters->input;

	UINT16 healed;

	if((data->ubID >= ubID_prefix) && (data->ubID < (ubID_prefix+6)))
		healed = (data->ubID - ubID_prefix);
	else
		healed = data->ubID;

	SOLDIERTYPE *pSoldier = MercPtrs[ healed ];
	pSoldier->bBleeding=data->bBleeding;
	pSoldier->stats.bLife=data->bLife;

#ifdef BETAVERSION
	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"healing..." );
#endif
}

void requestAIint(SOLDIERTYPE *pSoldier )
{
	AIint data;
	data.ubID=pSoldier->ubID;
	data.bteam=netbTeam;

#ifdef BETAVERSION
	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"interrupt requested" );
#endif
		
	client->RPC("rINT",(const char*)&data, (int)sizeof(AIint)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void awardINT (RPCParameters *rpcParameters)
{
	AIint* data= (AIint*)rpcParameters->input;

	SOLDIERTYPE *pSoldier = MercPtrs[ data->ubID ];

	StartInterrupt();

#ifdef BETAVERSION
	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"interrupt awarded" );
#endif

}

void game_over()
{
	// wait 3 seconds then notify all clients
	// reset
	isOwnTeamWipedOut = false;
	
	is_game_over = true;
	iScoreScreenTime = guiBaseJA2NoPauseClock + 5000;
}

// OJW - note: i dont use the internal timer callback for this because
// death notices use it and i want all those to go through normally
// so all clients have time to receive death notice of the last merc
// which creates victory condition

void send_gameover()
{
	// handle the user calling the wrong function first
	if (!is_game_over)
	{
		game_over(); // start the event
		return;
	}

	// stop the event from firing again
	is_game_over = false;
	iScoreScreenTime = 0;

	// notify all the clients that the game is over
	client->RPC("sendGAMEOVER",(const char*)&CLIENT_NUM, (int)sizeof(CLIENT_NUM)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveGAMEOVER(RPCParameters *rpcParameters)
{
	player_stats* data= (player_stats*)rpcParameters->input;
	memcpy(gMPPlayerStats,data,sizeof(player_stats)*5);

	// fire the score screen
	StartScoreScreen();
}

//***************************
//*** client connection*****
//*************************

void connect_client ( void )
{
	if(!is_client)
	{
		ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, MPClientMessage[0] );
			
		client = RakNetworkFactory::GetRakPeerInterface();
		bool b = client->Startup(1,30,&SocketDescriptor(), 1);

		//RPC's
		REGISTER_STATIC_RPC(client, recievePATH);
		REGISTER_STATIC_RPC(client, recieveSTANCE);
		REGISTER_STATIC_RPC(client, recieveDIR);
		REGISTER_STATIC_RPC(client, recieveFIRE);
		REGISTER_STATIC_RPC(client, recieveHIT);
		REGISTER_STATIC_RPC(client, recieveHIRE);
		REGISTER_STATIC_RPC(client, recieveDISMISS);
		REGISTER_STATIC_RPC(client, recieveguiPOS);
		REGISTER_STATIC_RPC(client, recieveguiDIR);
		REGISTER_STATIC_RPC(client, recieveEndTurn);
		REGISTER_STATIC_RPC(client, recieveAI);
		REGISTER_STATIC_RPC(client, recieveSTOP);
		REGISTER_STATIC_RPC(client, recieveINTERRUPT);
		REGISTER_STATIC_RPC(client, recieveREADY);
		REGISTER_STATIC_RPC(client, recieveGUI);
		REGISTER_STATIC_RPC(client, recieveSETTINGS);
		REGISTER_STATIC_RPC(client, recieveDOWNLOADSTATUS);
		REGISTER_STATIC_RPC(client, recieveFILE_TRANSFER_SETTINGS);
		REGISTER_STATIC_RPC(client, recieveTEAMCHANGE);
		REGISTER_STATIC_RPC(client, recieveEDGECHANGE);
		REGISTER_STATIC_RPC(client, recieveMAPCHANGE);
		REGISTER_STATIC_RPC(client, recieveBULLET);
		REGISTER_STATIC_RPC(client, recieveGRENADE);
		REGISTER_STATIC_RPC(client, recieveGRENADERESULT);
		REGISTER_STATIC_RPC(client, recievePLANTEXPLOSIVE);
		REGISTER_STATIC_RPC(client, recieveDETONATEEXPLOSIVE);
		REGISTER_STATIC_RPC(client, recieveDISARMEXPLOSIVE);
		REGISTER_STATIC_RPC(client, recieveSPREADEFFECT);
		REGISTER_STATIC_RPC(client, recieveNEWSMOKEEFFECT);
		REGISTER_STATIC_RPC(client, recieveEXPLOSIONDAMAGE);
		REGISTER_STATIC_RPC(client, recieveSTATE);
		REGISTER_STATIC_RPC(client, recieveDEATH);
		REGISTER_STATIC_RPC(client, recievehitSTRUCT);
		REGISTER_STATIC_RPC(client, recievehitWINDOW);
		REGISTER_STATIC_RPC(client, recieveMISS);
		REGISTER_STATIC_RPC(client, resume_turn);
		REGISTER_STATIC_RPC(client, UpdateSoldierFromNetwork);
		REGISTER_STATIC_RPC(client, recieve_fireweapon);
		REGISTER_STATIC_RPC(client, recieve_door);
		REGISTER_STATIC_RPC(client, null_team);
		REGISTER_STATIC_RPC(client, gotoRT);
		REGISTER_STATIC_RPC(client, recieve_wipe);
		REGISTER_STATIC_RPC(client, recieve_heal);
		REGISTER_STATIC_RPC(client, awardINT);
		REGISTER_STATIC_RPC(client, recieveGAMEOVER);
		REGISTER_STATIC_RPC(client, recieveDISCONNECT);
		REGISTER_STATIC_RPC(client, recieveCHATMSG);
		REGISTER_STATIC_RPC(client, requestSETID);
		REGISTER_STATIC_RPC(client, recieveDISCONNECTREASON);
		//***
		
		if (b)
		{
			is_client=true;
		}
		else
		{ 
			ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[76]);
		}
			
		is_connected=false;
	}	
	
	//reconnect/connect
	if( !is_connected && !is_connecting)
	{
		gTacticalStatus.uiFlags&= (~SHOW_ALL_MERCS );

		memset( &readyteamreg , 0 , sizeof (int) * 10);
		//OJW - 20081204
		memset ( &client_names,NULL,sizeof(int)*4);
		memset ( &client_ready,0,sizeof(int)*4);
		memset ( &client_teams,0,sizeof(int)*4);

		if (!gRandomStartingEdge)
			memset ( &client_edges,0,sizeof(int)*5);

		if (gRandomMercs)
			memset (random_mercs,0,sizeof(int)*7);

		memset( gMPPlayerStats,0,sizeof(player_stats)*5);
		memset ( &client_downloading,0,sizeof(int)*4);
		memset ( &client_progress,0,sizeof(int)*4);
		memset( &gszDisconnectReason,0,sizeof(CHAR16)*255);

		// ----------------------------
		// Read from ja2_mp.ini
		// ----------------------------
		char serverIP[30];

		CIniReader iniReader(JA2MP_INI_FILENAME);
		strncpy(serverIP, iniReader.ReadString(JA2MP_INI_INITIAL_SECTION,JA2MP_SERVER_IP, ""), 30);
		strncpy(cClientName, iniReader.ReadString(JA2MP_INI_INITIAL_SECTION,JA2MP_CLIENT_NAME, ""), 30);
		strncpy(cGameDataSyncDirectory, iniReader.ReadString(JA2MP_INI_INITIAL_SECTION,JA2MP_FILE_TRANSFER_DIRECTORY, "MULTIPLAYER/Servers/My Server"), 100);

		vfs::PropertyContainer props;
		props.initFromIniFile(JA2MP_INI_FILENAME);
		UINT16 serverPort = (UINT16)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_SERVER_PORT, 60005);
			
		// ----------------------------
		// Save to global values
		// ----------------------------
		recieved_settings=0;
		recieved_transfer_settings=0;
		goahead = 0;
		numready = 0;
		readystage = 0;
		status = 0;
		is_game_started = false;
		TEAM = 0;
		isOwnTeamWipedOut = false;
		wiped=0;
		//disable cheating
		gubCheatLevel = 0;
		cMaxClients = 0;	//reset server only set settings.			
		cDamageMultiplier=0;
		cSameMercAllowed=0;
		cDisableSpectatorMode=0;

		if(is_server)
			strcpy(serverIP, "127.0.0.1" );

		//**********************
		//here some nifty little tweaks

		LaptopSaveInfo.guiNumberOfMercPaymentsInDays += 20;
		LaptopSaveInfo.gubLastMercIndex = LAST_MERC_ID;
		LaptopSaveInfo.ubLastMercAvailableId = 7;
		gGameExternalOptions.fEnableSlayForever	= 1;
		LaptopSaveInfo.gubPlayersMercAccountStatus = 4;
				
		// WANNE: This should fix the bug playing a "tilt" sound and showing the mini laptop graphic on the screen, when opening the laptop / option screen from map screen from map screen
		gfDontStartTransitionFromLaptop = TRUE;
										
		//**********************

		// WANNE: FILE TRANSFER: Build the absolut file transfer directory path for the client
		GetExecutableDirectory(client_executableDir);

		strcpy(client_fileTransferDirectoryPath, client_executableDir);
		strcat(client_fileTransferDirectoryPath, "\\");
		strcat(client_fileTransferDirectoryPath, cGameDataSyncDirectory);

		// WANNE: FILE TRANSFER
		client->AttachPlugin(&fltClient);
		client->SetSplitMessageProgressInterval(1);
		
		CHAR16 tmpMessage[512];
		swprintf( tmpMessage, MPClientMessage[1],serverIP );
		ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, tmpMessage); // we are connecting
		SetConnectScreenSubMessageW( tmpMessage );

		client->Connect(serverIP, serverPort, 0,0);
		is_connecting = true;	
	}
	else if (is_connecting)
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[4] );
	}
	else if (is_connected)
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[3] );
	}
}

void client_packet ( void )
{	
	Packet* p;

	if (is_client)
	{
		p = client->Receive();

		while(p)
		{
			// We got a packet, get the identifier with our handy function
			packetIdentifier = GetPacketIdentifier(p);
			
			// Check if this is a network message packet
			switch (packetIdentifier)
			{
				case ID_DISCONNECTION_NOTIFICATION:
					  // Connection lost normally
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_DISCONNECTION_NOTIFICATION");
					is_connected=false;
					//OJW - 20081223
					//Gracefully notify and disconnect the client
					client->DeallocatePacket(p); // HandleClientConnectionLost shuts down the client
					HandleClientConnectionLost();
					//OJW - 2009
					return;
					break;
				case ID_ALREADY_CONNECTED:
					// Connection lost normally
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_ALREADY_CONNECTED");
					break;
				case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_REMOTE_DISCONNECTION_NOTIFICATION");
					break;
				case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_REMOTE_CONNECTION_LOST");
					break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_REMOTE_NEW_INCOMING_CONNECTION");
					break;
				case ID_CONNECTION_ATTEMPT_FAILED:
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_CONNECTION_ATTEMPT_FAILED");
					is_connected=false;
					is_connecting=false;

					//OJW - 20081224
					CHAR16 msgString[512];
					// handle auto retry
					if (auto_retry && giNumTries > 0)
					{
						swprintf( msgString, MPClientMessage[49],giNumTries );
						ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, msgString); // we already tried once, let the user know we are retrying
						SetConnectScreenSubMessageW( msgString );
					}
					else
					{
						swprintf( msgString, MPClientMessage[50],giNumTries );
						ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, msgString); // we already tried once, let the user know we are retrying
						SetConnectScreenSubMessageW( msgString );
					}
					giNextRetryTime = guiBaseJA2NoPauseClock + 5000; // 5 seconds?
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					 // Sorry, the server is full.  I don't do anything here but
					// A real app should tell the user
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_NO_FREE_INCOMING_CONNECTIONS");
					break;
				case ID_CONNECTION_LOST:
					// Couldn't deliver a reliable packet - i.e. the other system was abnormally
					// terminated
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_CONNECTION_LOST");
					
					is_connected=false;
					//OJW - 20081223
					//Gracefully notify and disconnect the client
					client->DeallocatePacket(p); // HandleClientConnectionLost shuts down the client
					HandleClientConnectionLost();
					return;
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					// This tells the client they have connected
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_CONNECTION_REQUEST_ACCEPTED");
					is_connected=true;
					is_connecting=false;

					// WANNE: FILE TRANSFER: Send all the data that is needed for the file transfer to the client,
					// before the actual file transfer begins
					requestFILE_TRANSFER_SETTINGS();

					requestSETTINGS();
					break;
				case ID_NEW_INCOMING_CONNECTION:
					//tells server client has connected
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_NEW_INCOMING_CONNECTION");
					break;
				case ID_MODIFIED_PACKET:
					// Cheater!
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"ID_MODIFIED_PACKET");
					break;
				default:
					ScreenMsg( FONT_BEIGE, MSG_MPSYSTEM, L"** a packet has been recieved for which i dont know what to do... **");
					break;
			}

			// We're done with the packet, get more :)
			client->DeallocatePacket(p);
			p = client->Receive();
		}

		// OJW - 20081223
		if (is_game_over)
		{
			if (guiBaseJA2NoPauseClock >= iScoreScreenTime)
			{
				send_gameover();
			}
		}

		// OJW - 20090203
		// Using the built in callback functions didnt work, so doing manually here
		if (bClosingChatBoxToStartGame)
		{
			if (guiBaseJA2NoPauseClock >= iCCStartGameTime)
			{
				bClosingChatBoxToStartGame = false;
				StartBattleChatBoxClosedCallback();
			}
		}
	}
}

// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}


void client_disconnect (void)
{
	if(is_client)
	{
		client->DetachPlugin(&fltClient);

		client->Shutdown(300);
		is_client = false;
		is_connected=false;
		is_connecting=false;
		
		fileTransferProgress = 0;
		allowlaptop=false;

		TEAM=0;	
		
		// clear local client cache
		memset(client_names,0,sizeof(char)*4*30);
		memset(client_edges,0,sizeof(int)*5);
		memset(client_ready,0,sizeof(int)*4);
		memset(client_teams,0,sizeof(int)*4);
		memset(gMPPlayerStats,0,sizeof(player_stats)*5);
		memset(random_mercs,0,sizeof(int)*7);
		memset ( &client_downloading,0,sizeof(int)*4);
		memset ( &client_progress,0,sizeof(int)*4);	

		// We're done with the network
		RakNetworkFactory::DestroyRakPeerInterface(client);
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[77]);

		#ifdef JA2BETAVERSION
			MPDebugMsg( "client_disconnect()\n" );
		#endif
	}
	else
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[78]);
	}
}

//OJW - 20081204 - send a starting edge change to all the clients
void send_edgechange(int newedge)
{
	// <TODO> remove this godawful hack with a proper game status check
	if (can_edgechange())
	{
		edgechange_struct lan;

		lan.client_num = CLIENT_NUM;
		lan.newedge = newedge;

		client->RPC("sendEDGECHANGE",(const char*)&lan, (int)sizeof(edgechange_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

		// redraw the character list on the map screen
		fDrawCharacterList = true;
		fTeamPanelDirty = true;
		client_edges[CLIENT_NUM-1] =newedge;
		cStartingSectorEdge = newedge;
	}
	else
	{
		ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, gszMPMapscreenText[3]);
	}
}

bool can_edgechange()
{
	return (is_game_started != 1 && client_ready[CLIENT_NUM-1] == 0 && !allowlaptop && !gRandomStartingEdge);
}

//OJW - 20081204 - send a starting team change to all the clients
void send_teamchange(int newteam)
{
	// <TODO> remove this godawful hack with a proper game status check
	if (can_teamchange())
	{
		teamchange_struct lan;

		lan.client_num = CLIENT_NUM;
		lan.newteam = newteam;

		client->RPC("sendTEAMCHANGE",(const char*)&lan, (int)sizeof(teamchange_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

		// redraw the character list on the map screen
		fDrawCharacterList = true;
		fTeamPanelDirty = true;
		client_teams[lan.client_num-1] = lan.newteam;
		TEAM = newteam;
	}
	else
	{
		ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, gszMPMapscreenText[4]);
	}
}

bool can_teamchange()
{
	bool isGeneralTeamchangeValid = (is_game_started != 1 && client_ready[CLIENT_NUM-1] == 0 && !allowlaptop);
	
	if (isGeneralTeamchangeValid && cGameType == MP_TYPE_TEAMDEATMATCH)
		return TRUE;
	else
		return FALSE;
}

// 20081222 - OJW
void StartScoreScreen( void )
{
	// pause game
	
	// set main screen as score screen
	LeaveTacticalScreen( MP_SCORE_SCREEN );
}

void ChatCallback( UINT8 ubResult )
{
	if (ubResult == MSG_BOX_RETURN_OK && wcscmp(gszChatBoxInputString,L"") > 0)
	{
		chat_msg cmsg;
		wchar_t szPlayerName[30];
		memset(szPlayerName,0,30*sizeof(wchar_t));
		mbstowcs( szPlayerName,cClientName,30);

		cmsg.bToAll = gbChatSendToAll;
		if (cGameType==MP_TYPE_TEAMDEATMATCH && !cmsg.bToAll)
			swprintf(cmsg.msg,MPClientMessage[56], szPlayerName, gszChatBoxInputString);
		else
			swprintf(cmsg.msg,MPClientMessage[52], szPlayerName, gszChatBoxInputString);
		cmsg.client_num = CLIENT_NUM;

		// notify all of the chat message
		client->RPC("sendCHATMSG",(const char*)&cmsg, (int)sizeof(chat_msg)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}

	memset(gszMsgBoxInputString,0,sizeof(gszMsgBoxInputString));
}

void OpenChatMsgBox( void )
{
	DoChatBox((guiCurrentScreen == GAME_SCREEN? true : false),gzMPChatboxText[1],guiCurrentScreen,ChatCallback,NULL);
}