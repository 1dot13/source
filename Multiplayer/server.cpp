#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "FileListTransfer.h"
#include "FileListTransferCBInterface.h"
#include "FileOperations.h"
#include "SuperFastHash.h"
#include "RakAssert.h"
#include "IncrementalReadInterface.h"
#include "BitStream.h"
#include "RakSleep.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <list>
#include "connect.h"
#include "types.h"
#include "gamesettings.h"
#include "message.h"
#include "FileMan.h"
#include "IniReader.h"
#include <vfs/Core/vfs.h>
#include "transfer_rules.h"
#include "MPJoinScreen.h"
#include "game init.h"
#include "text.h"
#include "network.h"
#include "message.h"
#include "overhead.h"
#include "fresh_header.h"
#include "Debug Control.h"
#include "MPXmlTeams.hpp"

extern CHAR16 gzFileTransferDirectory[100];

// WANNE: FILE TRANSFER
unsigned int setID;

// Sender progress notification
class ServerFileListProgress : public FileListProgress
{
	virtual void OnFilePush(const char *fileName, unsigned int fileLengthBytes, unsigned int offset, unsigned int bytesBeingSent, bool done, SystemAddress targetSystem)
	{
		// WANNE: Removed output in strategy log screen, because otherwise we do not see chat messages easily
		/*
		if (done)
		{
			char* filename = ExtractFilename((char*)fileName);
			ScreenMsg( FONT_RED, MSG_MPSYSTEM, MPServerMessage[10], filename);
		}
		*/
	}

	char *ExtractFilename(char *pathname) 
	{
		char *s;

		if ((s=strrchr(pathname, '\\')) != NULL) s++;
		else if ((s=strrchr(pathname, '/')) != NULL) s++;
		else if ((s=strrchr(pathname, ':')) != NULL) s++;
		else s = pathname;
		return s;
	}
} serverFileListProgress;

// ------------------------------
// Global Server Variables from ja2_mp.ini
// ------------------------------
char gKitBag[100];
UINT8 gMaxMercs;
UINT8 gDisableMorale;
UINT8 gReportHiredMerc;
UINT8 gDisableBobbyRay;
UINT8 gDisableMercEquipment;
UINT8 gSameMercAllowed;
float gDamageMultiplier;
UINT8 gMaxClients ;
UINT8 gGameType;
UINT8 gDifficultyLevel;
UINT8 gSkillTraits;
UINT8 gSyncGameDirectory;
INT32 gSecondsPerTick;
INT32 gStartingCash;
float gStartingTime;
UINT8 gWeaponReadyBonus;
UINT8 gInventoryAttachment;
UINT8 gDisableSpectatorMode;
UINT8 gMaxEnemiesEnabled;
// ------------------------------

UINT16 nc; //number of open connection
UINT16 ncr; //number of ready confirmed connections
//something keep record of ready connections ..
int mercs_ready[255];

unsigned char SGetPacketIdentifier(Packet *p);
unsigned char SpacketIdentifier;

RakPeerInterface *server;

// WANNE: FILE TRANSFER
FileListTransfer fltServer;	// flt1
IncrementalReadInterface incrementalReadInterface;
FileList fileList;
// OJW - 20090405
long fileListTotalBytes=0;

int numreadyteams;
int readyteamreg[10];

bool Sawarded;

SystemAddress blank;

typedef struct
{
	SystemAddress address;
	int cl_number;
	
}client_data;

client_data client_d[4];
int client_mercteam[4] = { 0 , 1 , 2 , 3 }; // random index of random_merc_teams per player

bool inline can_joingame();

char *ReplaceCharactersInString_Server(char *str, char *orig, char *rep)
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

int f_rec_num(int mode, SystemAddress sender)//from client data
{
	int x;
	client_data cl_record;
	for ( x=0; x<4;x++)
	{
		cl_record = client_d[x];

		if(mode==0)//find empty slot for new record
		{
			if(cl_record.address.binaryAddress==0)
				return(x);
		}
		if(mode==1)//wipe clean all
		{
			client_d[x].address.binaryAddress=0;
			client_d[x].address.port=0;
			client_d[x].cl_number=0;
		}
		if(mode==2)//clear one record
		{
			if(cl_record.address.binaryAddress==sender.binaryAddress && cl_record.address.port==sender.port)
			{
				client_d[x].address.port=0;
				client_d[x].cl_number=0;
				client_d[x].address.binaryAddress=0;
				return(254);
			}

		}
		// OJW - 090212 - look up client number
		if (mode == 3)
		{
			if(cl_record.address.binaryAddress==sender.binaryAddress && cl_record.address.port==sender.port)
			{
				return (x);
			}
		}
	}
	if(mode == 0)//'no free slots'
	{
	ScreenMsg( FONT_RED, MSG_MPSYSTEM, L"Client Record Error, Restart Server, and Report Error." );
	return (255);
	}
	return(254);
}

// use UNASSIGNED_SYSTEM_ADDRESS instead of rpcParameters->sender to send it back to yourself (the sender)
// there is very little in here dependant on the game engine and originally started out as an independant dedicated server .exe, and could if go ther again ... hayden.
//********* RPC SECTION ************

void sendPATH(RPCParameters *rpcParameters)
{
	server->RPC("recievePATH",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

// OJW - 20090405
void sendDOWNLOADSTATUS(RPCParameters *rpcParameters)
{
	server->RPC("recieveDOWNLOADSTATUS",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendSTANCE(RPCParameters *rpcParameters)
{
	server->RPC("recieveSTANCE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendDIR(RPCParameters *rpcParameters)
{
	server->RPC("recieveDIR",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendFIRE(RPCParameters *rpcParameters)
{
	server->RPC("recieveFIRE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendHIT(RPCParameters *rpcParameters)
{
	EV_S_WEAPONHIT* hit = (EV_S_WEAPONHIT*)rpcParameters->input;
	
	int team = MercPtrs[ hit->ubAttackerID ]->bTeam;
	
	// AI
	if (team == 1) 
		team = 4;
	// Client
	else if (team >= 6) 
		team -= 6;
	else if (team == 0) 
		team = CLIENT_NUM-1; // this case should not be possible, including as a precaution

	gMPPlayerStats[team].hits++;

	server->RPC("recieveHIT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendDISMISS(RPCParameters *rpcParameters)
{
	server->RPC("recieveDISMISS",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendHIRE(RPCParameters *rpcParameters)
{
	server->RPC("recieveHIRE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendguiPOS(RPCParameters *rpcParameters)
{
	server->RPC("recieveguiPOS",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendguiDIR(RPCParameters *rpcParameters)
{
	server->RPC("recieveguiDIR",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendEndTurn(RPCParameters *rpcParameters)
{
	server->RPC("recieveEndTurn",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendAI(RPCParameters *rpcParameters)
{
	server->RPC("recieveAI",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendSTOP(RPCParameters *rpcParameters)
{
	server->RPC("recieveSTOP",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendINTERRUPT(RPCParameters *rpcParameters)
{
	server->RPC("recieveINTERRUPT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendREADY(RPCParameters *rpcParameters)
{
	server->RPC("recieveREADY",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendGUI(RPCParameters *rpcParameters)
{
	server->RPC("recieveGUI",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendBULLET(RPCParameters *rpcParameters)
{
	server->RPC("recieveBULLET",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendGRENADE(RPCParameters *rpcParameters)
{
	server->RPC("recieveGRENADE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendGRENADERESULT(RPCParameters *rpcParameters)
{
	server->RPC("recieveGRENADERESULT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendPLANTEXPLOSIVE(RPCParameters *rpcParameters)
{
	server->RPC("recievePLANTEXPLOSIVE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendDETONATEEXPLOSIVE(RPCParameters *rpcParameters)
{
	server->RPC("recieveDETONATEEXPLOSIVE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendDISARMEXPLOSIVE(RPCParameters *rpcParameters)
{
	server->RPC("recieveDISARMEXPLOSIVE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendSPREADEFFECT(RPCParameters *rpcParameters)
{
	server->RPC("recieveSPREADEFFECT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendNEWSMOKEEFFECT(RPCParameters *rpcParameters)
{
	server->RPC("recieveNEWSMOKEEFFECT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendEXPLOSIONDAMAGE(RPCParameters *rpcParameters)
{
	server->RPC("recieveEXPLOSIONDAMAGE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendSTATE(RPCParameters *rpcParameters)
{
	server->RPC("recieveSTATE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendDEATH(RPCParameters *rpcParameters)
{
	// the master copy of the scoreboard is kept on the server
	death_struct* nDeath = (death_struct*)rpcParameters->input;

	// Save Stats on the server side
	gMPPlayerStats[nDeath->soldier_team-1].deaths++;
	gMPPlayerStats[nDeath->attacker_team-1].kills++;
	
	// get the client number of the client sending the message
	int iCLnum = f_rec_num(3,rpcParameters->sender)+1;

	server->RPC("recieveDEATH",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);

#ifdef JA2BETAVERSION
	wchar_t ateam[5];
	wchar_t steam[5];
	wchar_t clnum[5];
	_itow(nDeath->attacker_team,ateam,10);
	_itow(nDeath->soldier_team,steam,10);
	_itow(iCLnum,clnum,10);
	ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, L"DEBUG: Soldier Killed : Attacking team %s , Soldier Team %s, Sender %s",ateam,steam,clnum);
	char logmsg[100];
	sprintf(logmsg, "MP DEBUG: Soldier Killed #%i : Attacking team %i , Soldier Team %i, Sender %i\n",nDeath->soldier_id,nDeath->attacker_team,nDeath->soldier_team,iCLnum);
	MPDebugMsg( logmsg );
#endif
}
void sendhitSTRUCT(RPCParameters *rpcParameters)
{
	EV_S_STRUCTUREHIT* miss = (EV_S_STRUCTUREHIT*)rpcParameters->input;
	
	SOLDIERTYPE* pAttacker = MercPtrs[ miss->ubAttackerID ];
	if (pAttacker != NULL)
	{
		int team = MercPtrs[ miss->ubAttackerID ]->bTeam;
		
		// AI
		if (team == 1) 
			team = 4;
		// Clients
		else if (team >= 6) 
			team -= 6;
		else if (team == 0) 
			team = CLIENT_NUM-1; // this case should not be possible, including as a precaution

		gMPPlayerStats[team].misses++;
	}

	server->RPC("recievehitSTRUCT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendhitWINDOW(RPCParameters *rpcParameters)
{
	EV_S_WINDOWHIT* miss = (EV_S_WINDOWHIT*)rpcParameters->input;
	

	SOLDIERTYPE* pAttacker = MercPtrs[ miss->ubAttackerID ];
	if (pAttacker != NULL)
	{
		int team = MercPtrs[ miss->ubAttackerID ]->bTeam;
		
		// AI
		if (team == 1) 
			team = 4;
		// Clients
		else if (team >= 6) 
			team -= 6;
		else if (team == 0) 
			team = CLIENT_NUM-1; // this case should not be possible, including as a precaution

		gMPPlayerStats[team].misses++;
	}

	server->RPC("recievehitWINDOW",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendMISS(RPCParameters *rpcParameters)
{
	EV_S_MISS* miss = (EV_S_MISS*)rpcParameters->input;

	SOLDIERTYPE* pAttacker = MercPtrs[ miss->ubAttackerID ];
	if (pAttacker != NULL)
	{
		int team = MercPtrs[ miss->ubAttackerID ]->bTeam;
		
		// AI
		if (team == 1) 
			team = 4;
		// Clients
		else if (team >= 6) 
			team -= 6;
		else if (team == 0) 
			team = CLIENT_NUM-1; // this case should not be possible, including as a precaution

		gMPPlayerStats[team].misses++;
	}

	server->RPC("recieveMISS",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void updatenetworksoldier(RPCParameters *rpcParameters)
{
	server->RPC("UpdateSoldierFromNetwork",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void Snull_team(RPCParameters *rpcParameters)
{
	server->RPC("null_team",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendFIREW(RPCParameters *rpcParameters)
{
	server->RPC("recieve_fireweapon",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendDOOR(RPCParameters *rpcParameters)
{
	server->RPC("recieve_door",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void endINTERRUPT(RPCParameters *rpcParameters)
{
	server->RPC("resume_turn",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendWIPE(RPCParameters *rpcParameters)
{
	server->RPC("recieve_wipe",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendHEAL(RPCParameters *rpcParameters)
{
	server->RPC("recieve_heal",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

// OJW - edge and team changes
void sendEDGECHANGE(RPCParameters *rpcParameters)
{
	server->RPC("recieveEDGECHANGE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendTEAMCHANGE(RPCParameters *rpcParameters)
{
	server->RPC("recieveTEAMCHANGE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void requestSETID(SystemAddress addr)
{
	server->RPC("requestSETID","", 0, HIGH_PRIORITY, RELIABLE, 0, addr, false, 0, UNASSIGNED_NETWORK_ID,0);
}

void receiveSETID(RPCParameters *rpcParameters)
{
	setID = atoi((const char *)rpcParameters->input);

	// WANNE: FILE TRANSFER: Send the files to the client
	fltServer.Send(&fileList,server,rpcParameters->sender,setID,MEDIUM_PRIORITY,0,false, &incrementalReadInterface, 5000);
}

void startCOMBAT(RPCParameters *rpcParameters)
{
	if(!( gTacticalStatus.uiFlags & INCOMBAT ))
	
	{

		gTacticalStatus.uiFlags |= INCOMBAT;

		sc_struct* data = (sc_struct*)rpcParameters->input;
		EndTurn( data->ubStartingTeam );
	}

}

void sendREAL(RPCParameters *rpcParameters)
{
	real_struct* rData = (real_struct*)rpcParameters->input;

	if(readyteamreg[rData->bteam]==0)
	{
		readyteamreg[rData->bteam]=1;//register vote, to prevent double voting ;p~ //hayden
		numreadyteams++;

		int numactiveteams=0;
		int b;
		for(int i=6;i<=LAST_TEAM;i++)
		{
			if(i==6)
				b=0;
			else 
				b=i;

			if(gTacticalStatus.Team[ b ].bTeamActive)
				numactiveteams++;
		}

		//check # clients ready for realtime
		if (numreadyteams >= numactiveteams)
		{
			//if all send notification for realtime changeover
			//ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, L"Switching to realtime..." );
			numreadyteams=0;
			memset( &readyteamreg , 0 , sizeof (int) * 10);

			server->RPC("gotoRT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}
}

// 20081222 - OJW
void sendGAMEOVER(RPCParameters *rpcParameters)
{
	// ignore the RPCParams and send the server side scoreboard
	server->RPC("recieveGAMEOVER",(const char*)gMPPlayerStats, sizeof(gMPPlayerStats)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendCHATMSG(RPCParameters *rpcParameters)
{
	// ignore the RPCParams and send the server side scoreboard
	server->RPC("recieveCHATMSG",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

// OJW - 20081223
// fix client disconnecting mid game, allowing the game to proceed
void HandleDisconnect(SystemAddress sender)
{
	// find the CLIENT_NUM of the player
	int x;
	client_data cl_record;

	for ( x=0; x<4;x++)
	{
		cl_record = client_d[x];
		if(cl_record.address.binaryAddress==sender.binaryAddress && cl_record.address.port==sender.port)
		{
			// notify all the clients of the disconnect
			server->RPC("recieveDISCONNECT",(const char*)&cl_record.cl_number , sizeof(int)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			f_rec_num(2,sender); // remove from server's client list
			break;
		}
	}
}

// OJW - 20081218
// shuffle an integer array
//<TODO> remove shuffledList and put directly into arr[]
void rSortArray(int* arr, int len)
{
	std::list<int> tmpList;
	std::list<int> shuffledList;
	std::list<int>::iterator Iter;
	int i=0;

	// add all our array items
	for(i=0; i<len; i++)
		tmpList.push_front(arr[i]);

	// shuffle the items
	while(tmpList.size())
	{
		int iRandPos =  rand() % tmpList.size();
		for(Iter = tmpList.begin(); iRandPos>0; iRandPos--, Iter++);
		
		shuffledList.push_front(*Iter);
		tmpList.erase(Iter);
	}

	// add all our elements
	for(i=0; i<len; i++)
	{
		arr[i] = shuffledList.back();
		shuffledList.pop_back();
	}
}

// WANNE: FILE TRANSFER: Send all the settings the client needs to know for the file transfer before file transfer starts
void requestFILE_TRANSFER_SETTINGS(RPCParameters *rpcParameters)
{
	SystemAddress sender = rpcParameters->sender;//get senders address

	filetransfersettings_struct fts;

	fts.syncClientsDirectory = gSyncGameDirectory;
	strcpy(fts.fileTransferDirectory, s_ServerId.getServerId(vfs::Path(gzFileTransferDirectory)).utf8().c_str());
	strcpy(fts.serverName, cServerName);
	fts.totalTransferBytes = fileListTotalBytes;

	// OJW - 200907819 - Only send to the client that asked for it
	server->RPC("recieveFILE_TRANSFER_SETTINGS",(const char*)&fts, (int)sizeof(filetransfersettings_struct)*8, HIGH_PRIORITY, RELIABLE, 0, sender, false, 0, UNASSIGNED_NETWORK_ID,0);
}

//************************* //UNASSIGNED_SYSTEM_ADDRESS
//START INTERNAL SERVER
//*************************
//void send_settings (void)//send server settings to client
void requestSETTINGS(RPCParameters *rpcParameters )
{
	// dont generate or send settings to a new user if they are about to be disconnected
	// because no more players can join the the game
	if (can_joingame())
	{
		client_info* clinf = (client_info*)rpcParameters->input;

		// OJW - 20090507
		// Disconnect if version is wrong
		if (strcmp(clinf->client_version,MPVERSION)!=0)
		{
			CHAR16 verErrMsg[255];
			swprintf(verErrMsg,255, MPClientMessage[66], clinf->client_version,MPVERSION);

			// send disconnect reason only to this client
			server->RPC("recieveDISCONNECTREASON",(const char*)&verErrMsg, (int)sizeof(CHAR16)*255*8, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, false, 0, UNASSIGNED_NETWORK_ID,0);

			ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"CONNECTION REJECTED - CLIENT HAS WRONG VERSION");
			// disconnect this client
			server->CloseConnection(rpcParameters->sender, true);
			return;
		}
		
		//server assigned client numbers - hayden.
		SystemAddress sender = rpcParameters->sender;//get senders address
		int bslot = f_rec_num(0,blank);//get empty record slot
		client_d[bslot].address=sender; //record clients address
		int new_cl_num = bslot+1;//client number to assign
		client_d[bslot].cl_number=new_cl_num; //record clients number

		settings_struct lan;
		
		lan.client_num = new_cl_num; //new server assigned number
		strcpy(lan.client_name , clinf->client_name);

		lan.randomStartingEdge = gRandomStartingEdge;
		lan.randomMercs = gRandomMercs;

		lan.maxClients = gMaxClients;
		memcpy(lan.kitBag , gKitBag,sizeof (char)*100);
		lan.damageMultiplier = gDamageMultiplier;

		lan.sameMercAllowed = gSameMercAllowed;
		lan.gsMercArriveSectorX = gsMercArriveSectorX;
		lan.gsMercArriveSectorY = gsMercArriveSectorY;

		lan.enemyEnabled = gEnemyEnabled;
		lan.creatureEnabled = gCreatureEnabled;
		lan.militiaEnabled = gMilitiaEnabled;
		lan.civEnabled = gCivEnabled;

		lan.gameType = gGameType;

		lan.disableMorale = gDisableMorale;
		lan.reportHiredMerc = gReportHiredMerc;
		lan.secondsPerTick = gSecondsPerTick;

		// WANNE.MP: Check
		lan.soubBobbyRay = BR_AWESOME;
		lan.sofGunNut = TRUE;	
		lan.soubGameStyle = STYLE_REALISTIC;
		lan.soubDifficultyLevel = gDifficultyLevel;
		lan.soubSkillTraits = gSkillTraits;
		lan.sofTurnTimeLimit = TRUE;
		lan.sofIronManMode = FALSE;
		lan.startingCash = gStartingCash;
		
		// Old/Old
		if (gInventoryAttachment == INVENTORY_OLD)
		{			
			lan.inventoryAttachment = 0;	
		}
		else
		{
			// New/Old
			if (gGameOptions.ubAttachmentSystem == ATTACHMENT_OLD)
			{				
				lan.inventoryAttachment = 1; // New/Old
			}
			// New/New
			else
			{				
				lan.inventoryAttachment = 2;	// New/New
			}
		}		
		
		lan.disableBobbyRay=gDisableBobbyRay;
		lan.disableMercEquipment=gDisableMercEquipment;

		lan.maxMercs = gMaxMercs;
		
		memcpy( lan.client_names , client_names, sizeof( char ) * 4 * 30 );
		lan.team=clinf->team;
		// OJW - 20090530 - fix teams not initialised properly
		client_teams[ lan.client_num - 1 ] = lan.team;
		
		// OJW - 20081218
		if (gRandomStartingEdge)
		{
			// Get the edge from the randomized "client_edges"
			lan.startingSectorEdge = client_edges[lan.client_num-1];
		}
		else
		{
			// WANNE: on DM, each client should get a unique starting edge per default
			if (gGameType == MP_TYPE_DEATHMATCH || gGameType == MP_TYPE_TEAMDEATMATCH)
			{
				client_edges[0] = MP_EDGE_NORTH;	// client 1
				client_edges[1] = MP_EDGE_SOUTH;	// client 2
				client_edges[2] = MP_EDGE_EAST;		// client 3
				client_edges[3] = MP_EDGE_WEST;		// client 4

				lan.startingSectorEdge = client_edges[lan.client_num-1];
			}
			else
			{
				lan.startingSectorEdge=clinf->cl_edge;
			}
		}

		// OJW - 20081223
		if (gRandomMercs)
		{			
			mpTeams.SerializeProfiles(lan.random_mercs);
		}

		lan.startingTime = gStartingTime;
		lan.weaponReadyBonus = gWeaponReadyBonus;
		lan.inventoryAttachment = gInventoryAttachment;
		lan.disableSpectatorMode = gDisableSpectatorMode;

		// OJW - 20081204
		strcpy(lan.server_name , cServerName);
		memcpy(lan.client_edges,client_edges,sizeof(int)*5);
		memcpy(lan.client_teams,client_teams,sizeof(int)*4);

		// OJW - 20091024 - send servers random table
		memcpy(lan.random_table,guiPreRandomNums,sizeof(UINT32)*MAX_PREGENERATED_NUMS);

		// OJW - 20090507
		// send server version to client
		strcpy(lan.server_version,MPVERSION);

		server->RPC("recieveSETTINGS",(const char*)&lan, (int)sizeof(settings_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

		// WANNE: FILE TRANSFER: A client connected -> start the file transfer!
		if (gSyncGameDirectory)
			requestSETID(rpcParameters->sender);
	}
}

// added 081201 by Owen , allow the server to change the map while its still not in laptop mode
void send_mapchange(void)
{
	if (is_server  && !allowlaptop)
	{
		mapchange_struct lan;

		lan.gsMercArriveSectorX=gsMercArriveSectorX;
		lan.gsMercArriveSectorY=gsMercArriveSectorY;
		lan.startingTime = gStartingTime;

		server->RPC("recieveMAPCHANGE",(const char*)&lan, (int)sizeof(mapchange_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
	else
	{
		ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPServerMessage[45]);
	}
}

// OJW 090212
bool inline can_joingame()
{
	return !allowlaptop;
}

// Allow server to disconnect incoming clients after the game has started
void CheckIncomingConnection(Packet* p)
{
	// some clients might reconnect after disconnecting, either after the laptop is unlocked
	// or after the game has started
	// we dont want to allow this as thier game will be out of state
	if (!can_joingame())
	{
		ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, L"CONNECTION REJECTED - GAME HAS STARTED");
		// disconnect this client, no need to notify them as they will know if they disconnected
		// before receiving a settings packet that they were not allowed to join
		server->CloseConnection(p->systemAddress, true);
	}
}

void AddFilesToSendList()
{
	// we cannot just iterate over "*/*" as we would get ALL files and we don't want to send all files
	// instead we only iterate over the files in the "_MULTIPLAYER" profile
	vfs::CProfileStack *PS = getVFS()->getProfileStack();
	vfs::CVirtualProfile *prof = PS->getProfile("_MULTIPLAYER");
	if(prof != PS->topProfile())
	{
		// there is not supposed to be another profile?
		// output error message
		return;
	}
	CTransferRules transferRules;
	transferRules.initFromTxtFile("transfer_rules.txt");
	vfs::IBaseLocation* loc = prof->getLocation("");
	SGP_THROW_IFFALSE(loc != NULL, "MP profile was successfully created, but the root directory is not included");
	vfs::IBaseLocation::Iterator it = loc->begin();
	int i=0;
	for(; !it.end(); it.next(), i++)
	{
		vfs::Path const& valid_path = it.value()->getPath();
		if(transferRules.applyRule(valid_path()) == CTransferRules::ACCEPT)
		{
			// transfer only those files that are not on the ignore list
			vfs::tReadableFile* rfile = vfs::tReadableFile::cast(it.value());
			if(!rfile)
			{
				continue;
			}
			vfs::size_t fsize = rfile->getSize();
			fileListTotalBytes += (long)fsize;
			if( (fsize>0) && rfile->openRead())
			{
				std::vector<vfs::Byte> data(fsize,0);
				rfile->read(&data[0], fsize);
				rfile->close();
				fileList.AddFile(vfs::String::as_utf8(valid_path()).c_str(),&data[0], fsize,fsize,FileListNodeContext(0,0), false);
			}
		}
	}	
}

void start_server (void)
{
	if(!is_server)
	{	
		f_rec_num(1,blank);//wipe clean
				
		// ----------------------------
		// Read from ja2_mp.ini
		// ----------------------------

		CIniReader iniReader(JA2MP_INI_FILENAME);	// Wird nur für Strings gebraucht
		strncpy(cServerName, iniReader.ReadString(JA2MP_INI_INITIAL_SECTION, JA2MP_SERVER_NAME, "My JA2 Server"), 30 );				
		strncpy(gKitBag, iniReader.ReadString(JA2MP_INI_INITIAL_SECTION,JA2MP_KIT_BAG, ""), 100);
		
		vfs::PropertyContainer props;
		props.initFromIniFile(JA2MP_INI_FILENAME);
		UINT16 serverPort = (UINT16)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_SERVER_PORT, 60005);		
		UINT8 maxClients = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_MAX_CLIENTS, 4);										
		UINT8 sameMercAllowed = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_SAME_MERC, 1);
		UINT8 civEnabled = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_CIV_ENABLED, 0);
		UINT8 gameType = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_GAME_MODE, 0);
		UINT8 difficultyLevel = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_DIFFICULT_LEVEL, 3);
		UINT8 skillTraits = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_NEW_TRAITS, 0);
		UINT8 randomMercs = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_RANDOM_MERCS, 0);
		UINT8 randomStartingEdge = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_RANDOM_EDGES, 0);		
		UINT8 damageSelection = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_DAMAGE_MULTIPLIER, 1);
		UINT8 maxEnemiesEnabled = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_OVERRIDE_MAX_AI, 0);
		UINT8 syncGameDirectory = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_SYNC_CLIENTS_MP_DIR, 1);
		UINT8 reportHiredMerc = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_REPORT_NAME, 1);
		UINT8 startingCashSelection = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_STARTING_BALANCE, 25000);
		UINT8 timeTurnsSelection = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_TIMED_TURN_SECS_PER_TICK, 2);
		UINT8 disableBobbyRay = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_DISABLE_BOBBY_RAYS, 0);
		UINT8 maxMercs = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_MAX_MERCS, 6);
		UINT8 timeSelection = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_TIME, 1);
		UINT8 inventoryAttachment = (UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_ALLOW_CUSTOM_NIV, 0);

		// ----------------------------
		// Save to global values
		// ----------------------------		
		gMaxClients = maxClients;
		gMaxEnemiesEnabled = 0;
		gDisableMorale = 0;
		gSyncGameDirectory = syncGameDirectory;										
		gReportHiredMerc = reportHiredMerc;			
		gDisableBobbyRay = disableBobbyRay;
		gDisableMercEquipment = 0;		// Disable AIM and MERC equipment				
		gMaxMercs = maxMercs;		
		gGameType = gameType;
		gDifficultyLevel = difficultyLevel + 1;
		gSkillTraits = skillTraits;
		gEnemyEnabled = 0;
		gCreatureEnabled = 0;
		gMilitiaEnabled = 0;
		gCivEnabled = 0;
		gRandomMercs = randomMercs;
		gRandomStartingEdge = randomStartingEdge;
		gWeaponReadyBonus = 0;
		gDisableSpectatorMode = 0;
		gInventoryAttachment = inventoryAttachment;

		if (gRandomStartingEdge)
		{
			// create random starting edges
			int spawns[5] = { 0 , 1 , 2 , 3, 9 };	// 9 == Center
			
			// Randomize spawns
			rSortArray(spawns,5);
			memcpy(client_edges,spawns,sizeof(int)*5);
		}

		if (gRandomMercs)
		{
			// randomly sort team indexes to give client
			// one of four random merc teams
			rSortArray(client_mercteam,4);
			mpTeams.HandleServerStarted();
		}

		if(gGameType == MP_TYPE_COOP)//only enable ai during coop
		{
			gEnemyEnabled = 1;				// always enable enemies in co-op
			gMilitiaEnabled = 0;			// always disable militia
			gCivEnabled = civEnabled;
			gMaxEnemiesEnabled = maxEnemiesEnabled;				
		}

		// random_mercs implies same_merc
		gSameMercAllowed = gRandomMercs ? 1 : sameMercAllowed;
				
		switch (damageSelection)
		{
			case 0:	// Very Low
				gDamageMultiplier = 0.2f;
				break;
			case 1:	// Low
				gDamageMultiplier = 0.7f;
				break;
			case 2:	// Normal
				gDamageMultiplier = 1.0f;
				break;
		}									

		switch (timeTurnsSelection)
		{
			case 0:		// Never
				gSecondsPerTick = 0;
				break;
			case 1:		// Slow
				gSecondsPerTick = 5;
				break;
			case 2:		// Medium
				gSecondsPerTick = 100;
				break;
			case 3:		// Fast
				gSecondsPerTick = 400;
				break;
		}

		switch (startingCashSelection)
		{
			case 0:	// Low
				gStartingCash = 5000;
				break;
			case 1:	// Medium
				gStartingCash = 50000;
				break;
			case 2:	// High
				gStartingCash = 100000;
				break;
			case 3:	// Unlimited
				gStartingCash = 999999999;
				break;
		}
		
		switch (timeSelection)
		{
			case 0:	// Morning
				gStartingTime = 7.00f;
				break;
			case 1:	// Afternoon
				gStartingTime = 13.00f;
				break;
			case 2:	// Night
				gStartingTime = 2.00;
				break;
		}
					
		//**********************

		ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, MPServerMessage[0] );

		server=RakNetworkFactory::GetRakPeerInterface();
		
		// WANNE: Set higher timeout than default (30 seconds)
		server->SetTimeoutTime(120000, UNASSIGNED_SYSTEM_ADDRESS);	// 120 Seconds


		bool b = server->Startup(gMaxClients, 30, &SocketDescriptor(serverPort,0), 1);

		server->SetMaximumIncomingConnections((gMaxClients));
		server->SetOccasionalPing(true);

		//RPC's
		REGISTER_STATIC_RPC(server, sendPATH);
		REGISTER_STATIC_RPC(server, sendDOWNLOADSTATUS);
		REGISTER_STATIC_RPC(server, sendSTANCE);
		REGISTER_STATIC_RPC(server, sendDIR);
		REGISTER_STATIC_RPC(server, sendFIRE);
		REGISTER_STATIC_RPC(server, sendHIT);
		REGISTER_STATIC_RPC(server, sendHIRE);
		REGISTER_STATIC_RPC(server, sendDISMISS);
		REGISTER_STATIC_RPC(server, sendguiPOS);
		REGISTER_STATIC_RPC(server, sendguiDIR);
		REGISTER_STATIC_RPC(server, sendEndTurn);
		REGISTER_STATIC_RPC(server, sendAI);
		REGISTER_STATIC_RPC(server, sendSTOP);
		REGISTER_STATIC_RPC(server, sendINTERRUPT);
		REGISTER_STATIC_RPC(server, sendREADY);
		REGISTER_STATIC_RPC(server, sendGUI);
		REGISTER_STATIC_RPC(server, sendBULLET);
		REGISTER_STATIC_RPC(server, sendGRENADE);
		REGISTER_STATIC_RPC(server, sendGRENADERESULT);
		REGISTER_STATIC_RPC(server, sendPLANTEXPLOSIVE);
		REGISTER_STATIC_RPC(server, sendDETONATEEXPLOSIVE);
		REGISTER_STATIC_RPC(server, sendDISARMEXPLOSIVE);
		REGISTER_STATIC_RPC(server, sendSPREADEFFECT);
		REGISTER_STATIC_RPC(server, sendNEWSMOKEEFFECT);
		REGISTER_STATIC_RPC(server, sendEXPLOSIONDAMAGE);
		REGISTER_STATIC_RPC(server, requestSETTINGS);
		REGISTER_STATIC_RPC(server, requestFILE_TRANSFER_SETTINGS);
		REGISTER_STATIC_RPC(server, sendSTATE);
		REGISTER_STATIC_RPC(server, sendDEATH);
		REGISTER_STATIC_RPC(server, sendhitSTRUCT);
		REGISTER_STATIC_RPC(server, sendhitWINDOW);
		REGISTER_STATIC_RPC(server, sendMISS);		
		REGISTER_STATIC_RPC(server, updatenetworksoldier);
		REGISTER_STATIC_RPC(server, Snull_team);
		REGISTER_STATIC_RPC(server, sendFIREW);
		REGISTER_STATIC_RPC(server, sendDOOR);
		REGISTER_STATIC_RPC(server, endINTERRUPT);
		REGISTER_STATIC_RPC(server, sendREAL);
		REGISTER_STATIC_RPC(server, startCOMBAT);
		REGISTER_STATIC_RPC(server, sendWIPE);
		REGISTER_STATIC_RPC(server, sendHEAL);
		REGISTER_STATIC_RPC(server, sendEDGECHANGE);
		REGISTER_STATIC_RPC(server, sendTEAMCHANGE);
		REGISTER_STATIC_RPC(server, sendGAMEOVER);
		REGISTER_STATIC_RPC(server, sendCHATMSG);
		REGISTER_STATIC_RPC(server, receiveSETID);		

		if (b)
		{
			ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, MPServerMessage[1]);			
			is_server = true;

			// WANNE: FILE TRANSFER
			server->AttachPlugin(&fltServer);
			server->SetSplitMessageProgressInterval(1);
			fltServer.SetCallback(&serverFileListProgress);

			fileListTotalBytes=0;
			if (gSyncGameDirectory == 1)
			{
				AddFilesToSendList();
			}

			connect_client();//connect client to server
		}
		else
		{
			ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPServerMessage[4]);
		}
	}
	else
	{
		ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, MPServerMessage[3]);
	}
}


// recieve and process server info packets

void server_packet ( void )
{
	
	Packet* p;

	if (is_server)
	{

	p = server->Receive();

	while(p)
	{
			//continue; // Didn't get any packets

		// We got a packet, get the identifier with our handy function
		SpacketIdentifier = SGetPacketIdentifier(p);
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"packet recieved");
		// Check if this is a network message packet
		switch (SpacketIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION://client disconnected purposefullly
				// Connection lost normally
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_DISCONNECTION_NOTIFICATION");
				HandleDisconnect(p->systemAddress);//clear record
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_ALREADY_CONNECTED");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_REMOTE_DISCONNECTION_NOTIFICATION");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_REMOT/MING_CONNECTION");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_CONNECTION_ATTEMPT_FAILED");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_NO_FREE_INCOMING_CONNECTIONS");
				break;
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_CONNECTION_LOST");//client dropped
				HandleDisconnect(p->systemAddress);//clear record
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_CONNECTION_REQUEST_ACCEPTED");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				//tells server client has connected
				#ifdef JA2BETAVERSION
					ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_NEW_INCOMING_CONNECTION");
				#endif
				// make sure they can connect
				CheckIncomingConnection(p);
				//send_settings();//send off server set settings

				break;
			case ID_MODIFIED_PACKET:
				// Cheater!
				#ifdef JA2BETAVERSION
					ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"ID_MODIFIED_PACKET");
				#endif
				break;
			default:
				#ifdef JA2BETAVERSION	
					ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, L"** a packet has been recieved for which i dont know what to do... **");
				#endif
				break;
		}


		// We're done with the packet, get more :)
		server->DeallocatePacket(p);
		p = server->Receive();
	}
	}
}
// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char SGetPacketIdentifier(Packet *p)
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

void server_disconnect (void)
{
	if(is_server)
	{
		server->DetachPlugin(&fltServer);
	server->Shutdown(300);
	is_server = false;
	fileList.Clear();
	// We're done with the network
	RakNetworkFactory::DestroyRakPeerInterface(server);
	ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, MPServerMessage[6]);
	}
	else
	{
	ScreenMsg( FONT_ORANGE, MSG_MPSYSTEM, MPServerMessage[7]);
	}
}
