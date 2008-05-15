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
#endif

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"

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

unsigned char GetPacketIdentifier(Packet *p);
unsigned char packetIdentifier;

#include "messagebox.h"

#pragma pack(1)

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
#include "test_space.h"



extern INT8 SquadMovementGroups[ ];
RakPeerInterface *client;

typedef struct
{
	UINT8		ubProfileID;
	int		team;
	BOOLEAN fCopyProfileItemsOver;
	INT8 bTeam;

} send_hire_struct;

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

bullets_table bTable[11][50];

char client_names[4][30];

INT32 MAX_MERCS;

int TEAM;
//int OP_TEAM_2;
//int OP_TEAM_3;
//int OP_TEAM_4;



UINT8 netbTeam;
UINT8	ubID_prefix;

 bool is_connected=false;
 bool is_connecting=false;
 bool is_client=false;
 bool is_server=false;
 bool is_networked=false;

 bool requested=false;

int tcount;
int kit[20];
bool DISABLE_MORALE;
 int TESTING;
 bool allowlaptop=false;

 bool recieved_settings=false;

 bool getReal=false;

 //char CLIENT_NUM[30];
 int CLIENT_NUM;
 char SECT_EDGE[30];
char ckbag[100];
 char CLIENT_NAME[30];

 char SERVER_IP[30] ;
 char SERVER_PORT[30];

 int REPORT_NAME;
 int WEAPON_READIED_BONUS;

 int ENEMY_ENABLED=0;
 int CREATURE_ENABLED=0;
 int MILITIA_ENABLED=0;
 int CIV_ENABLED=0;

 int ALLOW_EQUIP;

 int SAME_MERC;
 int PLAYER_BSIDE;

int START_TEAM_TURN;

 bool goahead = 0;
 int numready = 0;
 int readystage = 0;
 bool status = 0;

 bool wiped;

 bool lockedgui = 0;

FLOAT DAMAGE_MULTIPLIER;

//int INTERRUPTS;
int MAX_CLIENTS;

UINT16 crate_usMapPos;	

INT16	crate_sGridX, crate_sGridY;

void overide_callback( UINT8 ubResult );
void kick_callback( UINT8 ubResult );
void turn_callback (UINT8 ubResult);


//*****************
//RPC sends and recieves:
//********************


void send_path (  SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  )
{
	if(pSoldier->ubID < 120)
	{
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Sending new path" );


		EV_S_SENDPATHTONETWORK SNetPath;

		if(pSoldier->ubID < 20)
			SNetPath.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		else
			SNetPath.usSoldierID = pSoldier->ubID;
	
		SNetPath.sAtGridNo=pSoldier->sGridNo;
		
		
			SNetPath.ubNewState=usMovementAnim;
			//if(fFromUI==255)SNetPath.ubNewState=255;

			SNetPath.usCurrentPathIndex=pSoldier->pathing.usPathIndex;
			memcpy(SNetPath.usPathData, pSoldier->pathing.usPathingData, sizeof(UINT16)*30);
			SNetPath.usPathDataSize=pSoldier->pathing.usPathDataSize;
			SNetPath.sDestGridNo=sDestGridNo;
			
//this stops soldier from advancing further than one square,and requests "clearance" for another after that... :)
		//if(pSoldier->flags.fIsSoldierDelayed==FALSE)
		//{
		//	pSoldier->flags.fIsSoldierDelayed=TRUE;
		//	request_ovh(pSoldier->ubID);
		//}

		
		client->RPC("sendPATH",(const char*)&SNetPath, (int)sizeof(EV_S_SENDPATHTONETWORK)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}

}

void recievePATH(RPCParameters *rpcParameters)
{

		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Recieving new path," );
					
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

		//if ( !( ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_MOVING | ANIM_SPECIALMOVE ) ) && pSoldier->fNoAPToFinishMove  ) && !pSoldier->fPauseAllAnimation  )
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
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"change stance: %d",ubDesiredStance );
	
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

void send_fire( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo )
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
		
		EV_S_WEAPONHIT* SWeaponHit = (EV_S_WEAPONHIT*)rpcParameters->input;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveHIT" );

		SOLDIERTYPE *pSoldier = MercPtrs[ SWeaponHit->usSoldierID ];
					UINT16 usSoldierID;
					UINT8 ubAttackerID;

					if((SWeaponHit->usSoldierID >= ubID_prefix) && (SWeaponHit->usSoldierID < (ubID_prefix+7))) // within our netbTeam range...
						usSoldierID = (SWeaponHit->usSoldierID - ubID_prefix);
					else
						usSoldierID = SWeaponHit->usSoldierID;

					if((SWeaponHit->ubAttackerID >= ubID_prefix) && (SWeaponHit->ubAttackerID < (ubID_prefix+7)))
						ubAttackerID = (SWeaponHit->ubAttackerID - ubID_prefix);
					else
						ubAttackerID = SWeaponHit->ubAttackerID;
	

	
		//AddGameEvent( S_WEAPONHIT, 0, &SWeaponHit );
		WeaponHit( usSoldierID, SWeaponHit->usWeaponIndex, SWeaponHit->sDamage, SWeaponHit->sBreathLoss, SWeaponHit->usDirection, SWeaponHit->sXPos, SWeaponHit->sYPos, SWeaponHit->sZPos, SWeaponHit->sRange, ubAttackerID, SWeaponHit->fHit, SWeaponHit->ubSpecial, SWeaponHit->ubLocation );

		if(SWeaponHit->fStopped)
		{
			INT8 bTeam=pSoldier->bTeam;
			INT32 iBullet = bTable[bTeam][SWeaponHit->iBullet].local_id;
					
			RemoveBullet(iBullet);
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"removed bullet" );

		
		}
		
}

void send_hire( UINT8 iNewIndex, UINT8 ubCurrentSoldier, INT16 iTotalContractLength, BOOLEAN fCopyProfileItemsOver)
{
		
		{
		
			send_hire_struct sHireMerc;

			sHireMerc.ubProfileID=ubCurrentSoldier;
			//sHireMerc.iTotalContractLength=iTotalContractLength;
			sHireMerc.team=TEAM;
			sHireMerc.fCopyProfileItemsOver=fCopyProfileItemsOver;

			
			sHireMerc.bTeam=netbTeam;

			SOLDIERTYPE *pSoldier = MercPtrs[ iNewIndex ];
			pSoldier->ubStrategicInsertionCode=(atoi(SECT_EDGE)); // this sets the param read from the ini for your starting sector edge...
			
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[44], pSoldier->name);

			
			AddCharacterToAnySquad( pSoldier );
			AddSoldierToSector( pSoldier->ubID ); //add g\hired merc to sector so can access sector inv.
			
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



			
				//if (pSoldier->ubID==0)
				//{
				//			///create crate apon first hire
				////crate_usMapPos = MAPROWCOLTOPOS( crate_sGridY, crate_sGridX );
				//crate_usMapPos =  ChooseMapEdgepoint(pSoldier->ubStrategicInsertionCode);

				////crate_usMapPos	= ChooseMapEdgepoint(atoi(SECT_EDGE));
				//AddStructToUnLoadedMapTempFile( crate_usMapPos, SECONDOSTRUCT3, gsMercArriveSectorX, gsMercArriveSectorY, 0 );
				//}
				
	

	

			client->RPC("sendHIRE",(const char*)&sHireMerc, (int)sizeof(send_hire_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
}

void recieveHIRE(RPCParameters *rpcParameters)
{
	
	send_hire_struct* sHireMerc = (send_hire_struct*)rpcParameters->input;

	

	SOLDIERTYPE	*pSoldier;
	UINT8		iNewIndex;
	UINT8		ubCount=0;

	
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	BOOLEAN fReturn = FALSE;
	
	

	memset( &MercCreateStruct, 0, sizeof( MercCreateStruct ) );
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
	if(!SAME_MERC)gMercProfiles[ pSoldier->ubProfile ].bMercStatus = MERC_WORKING_ELSEWHERE;
	pSoldier->bSide=0; //default coop only
	if(PLAYER_BSIDE==0)//all vs all only
	{
		pSoldier->bSide=1;
	//if(MercCreateStruct.bTeam==6)pSoldier->bSide=OP_TEAM_1;
	//	if(MercCreateStruct.bTeam==7)pSoldier->bSide=OP_TEAM_2;
	//		if(MercCreateStruct.bTeam==8)pSoldier->bSide=OP_TEAM_3;
	//	if(MercCreateStruct.bTeam==9)pSoldier->bSide=OP_TEAM_4;
		
	}
	if(PLAYER_BSIDE==1) //allow teams
	{
			if(sHireMerc->team != TEAM)pSoldier->bSide=1;

	}


	AddSoldierToSector( iNewIndex );

	
	if(REPORT_NAME==1)
	{	
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[5],MercCreateStruct.bTeam-5,client_names[MercCreateStruct.bTeam-6],pSoldier->name );
	}
	else 
	{
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[6],MercCreateStruct.bTeam-5,client_names[MercCreateStruct.bTeam-6] );
	}


}

void send_gui_pos(SOLDIERTYPE *pSoldier,  FLOAT dNewXPos, FLOAT dNewYPos)
{
		
		{
			
			gui_pos gnPOS;

		
	

			gnPOS.usSoldierID = (pSoldier->ubID)+ubID_prefix;
			
			gnPOS.dNewXPos = dNewXPos;
			gnPOS.dNewYPos = dNewYPos;
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"guiPOS: %f , %f", dNewXPos,dNewYPos);
		

			client->RPC("sendguiPOS",(const char*)&gnPOS, (int)sizeof(gui_pos)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	
}
void recieveguiPOS(RPCParameters *rpcParameters)
{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveguiPOS" );
		gui_pos* gnPOS = (gui_pos*)rpcParameters->input;

		SOLDIERTYPE *pSoldier = MercPtrs[ gnPOS->usSoldierID ];

		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"guiPOS: %f , %f", gnPOS->dNewXPos,gnPOS->dNewYPos);
	
		INT16 sNewGridNo;

    	sNewGridNo = GETWORLDINDEXFROMWORLDCOORDS(gnPOS->dNewXPos, gnPOS->dNewYPos );
		pSoldier->usStrategicInsertionData=sNewGridNo;
		pSoldier->ubStrategicInsertionCode=INSERTION_CODE_GRIDNO;
		pSoldier->sInsertionGridNo = pSoldier->usStrategicInsertionData;


		pSoldier->EVENT_SetSoldierPosition( gnPOS->dNewXPos, gnPOS->dNewYPos );
		
}

void send_gui_dir(SOLDIERTYPE *pSoldier, UINT16	usNewDirection)
{
		
		{
			
			gui_dir gnDIR;

		

			gnDIR.usSoldierID = (pSoldier->ubID)+ubID_prefix;
			gnDIR.usNewDirection = usNewDirection;
			


			client->RPC("sendguiDIR",(const char*)&gnDIR, (int)sizeof(gui_dir)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	
}

void recieveguiDIR(RPCParameters *rpcParameters)
{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveguiDIR" );
		gui_dir* gnDIR = (gui_dir*)rpcParameters->input;

		SOLDIERTYPE *pSoldier = MercPtrs[ gnDIR->usSoldierID ];
		
		pSoldier->EVENT_SetSoldierDirection( gnDIR->usNewDirection );
		//pSoldier->ubInsertionDirection = pSoldier->bDirection;
}




void send_EndTurn( UINT8 ubNextTeam )
{
		
		{
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"sendEndTurn" );
		
			if(ubNextTeam==0)
			{
				ubNextTeam=netbTeam;
			}
		
	
		turn_struct tStruct;

		tStruct.tsnetbTeam = netbTeam;
		tStruct.tsubNextTeam = ubNextTeam;
		

		client->RPC("sendEndTurn",(const char*)&tStruct, (int)sizeof(turn_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}


}

void recieveEndTurn(RPCParameters *rpcParameters)
{
		turn_struct* tStruct = (turn_struct*)rpcParameters->input;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveEndTurn" );
		UINT8 sender_bTeam;
		UINT8 ubTeam;
		sender_bTeam=tStruct->tsnetbTeam;
		ubTeam=tStruct->tsubNextTeam;
	
	if(is_server || sender_bTeam==6)
	{
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
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"send_AI" );

		//SOLDIERCREATE_STRUCT aaa = *pCreateStruct;
	
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
		memcpy( new_standard_data.PADDINGSLOTS, send_inv->standard_data.PADDINGSLOTS, sizeof( INT8 ) * 14 );
		new_standard_data.bPatrolCnt = send_inv->standard_data.bPatrolCnt;
		new_standard_data.bSectorZ = send_inv->standard_data.bSectorZ;
		new_standard_data.bStrength = send_inv->standard_data.bStrength;
		new_standard_data.bTeam = send_inv->standard_data.bTeam;
		new_standard_data.bUseGivenVehicleID = send_inv->standard_data.bUseGivenVehicleID;
		new_standard_data.bWisdom = send_inv->standard_data.bWisdom;
		//new_standard_data.ef1 = send_inv->standard_data.ef1;
		//new_standard_data.ef2 = send_inv->standard_data.ef2;
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
		memcpy( new_standard_data.sPatrolGrid, send_inv->standard_data.sPatrolGrid, sizeof( INT16 ) * MAXPATROLGRIDS );
		new_standard_data.sSectorX = send_inv->standard_data.sSectorX;
		new_standard_data.sSectorY = send_inv->standard_data.sSectorY;
		new_standard_data.ubCivilianGroup = send_inv->standard_data.ubCivilianGroup;
		new_standard_data.ubProfile = send_inv->standard_data.ubProfile;
		new_standard_data.ubScheduleID = send_inv->standard_data.ubScheduleID;
		new_standard_data.ubSoldierClass = send_inv->standard_data.ubSoldierClass;
		memcpy( new_standard_data.VestPal , send_inv->standard_data.VestPal, sizeof( PaletteRepID ));
				

		for(int x=0;x<55;x++)
		{
			//OBJECTTYPE		Object;
			//new_standard_data.Inv[x]

			//new_standard_data.Inv[x].fFlags = send_inv->slot[x].fFlags;
			if(send_inv->slot[x].usItem != 0)CreateItems( send_inv->slot[x].usItem, 100, send_inv->slot[x].ubNumberOfObjects, &new_standard_data.Inv[x] );
			//new_standard_data.Inv[x] = Object;
		}


		new_standard_data.fPlayerPlan=1;

	
    	TacticalCreateSoldier( &new_standard_data, &iNewIndex );
		pSoldier = &Menptr[iNewIndex];
		pSoldier->flags.uiStatusFlags |= SOLDIER_PC;

		AddSoldierToSector( iNewIndex );
		//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"recieveAI" );
	

	
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
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[7],numready,MAX_CLIENTS );
					}
					else
					{
				    info.status = 0; 
					status=0;
					numready = numready-1;
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[8],numready,MAX_CLIENTS );
					}	
				}
				
				
		
					if(is_server && numready == MAX_CLIENTS) //all ready. and server tells all to load...
					{
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[9] );

					goahead=1;
					readystage=1;

					info.ready_stage = 1;
					info.status = 1; 					
					}	
			
				
			
		client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

		if(is_server && numready == MAX_CLIENTS)
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
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[10], info->client_num,client_names[info->client_num-1],numready,MAX_CLIENTS );
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[9] );
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
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[10], info->client_num,client_names[info->client_num-1],numready,MAX_CLIENTS );
		
		}
		else
		{
		numready = numready-1;
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[11], info->client_num,client_names[info->client_num-1],numready,MAX_CLIENTS );
		
		}
		
		if(is_server && numready == MAX_CLIENTS) //all ready. and server tells all to load...and loads himself
		{
			goahead=1;
			readystage=1;
			send_ready();
			start_battle();
			
		}
	}
	else if(info->ready_stage==36)//server allows laptop access
	{
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[36] );
		allowlaptop=1;
	}

}

void send_loaded (void)
{
		ready_struct info;
		info.client_num = CLIENT_NUM;
		info.ready_stage = 1;//done loading level
		info.status=1;

		numready++;
		if(numready==MAX_CLIENTS && is_server)
		{
			lockui(1);//unlock ui
			readystage=0;
			numready=0;

					info.ready_stage = 2;//done placing mercs
					info.status=1;

					//client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}

		client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void send_donegui ( UINT8 ubResult )
{
	if(ubResult==1 && status==0)return;//avoid double remove callback response from final message box removal
		ready_struct info;
		info.client_num = CLIENT_NUM;
		if(ubResult==1)DialogRemoved(1);//cleanup msgbox after not ready
		if(status==0)//now ready
		{
			status=1;
			numready++;
		info.ready_stage = 3;//done placing mercs
		info.status=1;
			
			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, MPClientMessage[12],  guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT, send_donegui,  &CenterRect );


			if(numready==MAX_CLIENTS && is_server)//all done
			{
				
				numready=0;
				status=0;
				info.ready_stage = 4;//done placing mercs
				info.status=1;
				gMsgBox.bHandled = MSG_BOX_RETURN_OK;
				KillTacticalPlacementGUI(); //send and kill
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, MPClientMessage[13]);
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
		if(numready==MAX_CLIENTS && is_server)
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
		if(numready==MAX_CLIENTS && is_server)//all done
		{
						numready=0;

					ready_struct info;
					info.client_num = CLIENT_NUM;
					info.ready_stage = 4;//all done placing mercs, kill all
					info.status=1;
					gMsgBox.bHandled = MSG_BOX_RETURN_OK;
					status=0;
					KillTacticalPlacementGUI();
					ScreenMsg( FONT_LTBLUE, MSG_CHAT, MPClientMessage[13]);

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
		ScreenMsg( FONT_LTBLUE, MSG_CHAT, MPClientMessage[13]);
		numready=0;
		status=0;
	}

}
void allowlaptop_callback ( UINT8 ubResult )
{

	if(ubResult==2)
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[36] );
	allowlaptop=1;

				ready_struct info;
				info.client_num = CLIENT_NUM;
				info.ready_stage=36;
				info.status=1;

	client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

	}
}


void start_battle ( void )
{ 
if(!is_client)
{
ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[14] );
}
else if(!allowlaptop && is_server)
{
			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, MPClientMessage[35],  guiCurrentScreen, MSG_BOX_FLAG_YESNO | MSG_BOX_FLAG_USE_CENTERING_RECT, allowlaptop_callback,  &CenterRect );

}
else if(allowlaptop)
{
	
	if ( NumberOfMercsOnPlayerTeam() ==0)
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[15] );
	}
	else if(goahead==1)
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

	UINT32	i;
	for(i=0; i<4;i++)
	{	
		CHAR16 name[255];
		int nm = mbstowcs( name, client_names[i], sizeof (char)*30 );
		//copy in client specified name for the player turn bar :)
		if(nm)
		{
			CHAR16 string[255];
			memcpy(string,TeamTurnString[ (i+6) ], sizeof( CHAR16) * 255 );
		

			CHAR16 full[255];
			swprintf(full, L"%s - '%s'",string,name);

			memcpy( TeamTurnString[ (i+6) ] , full, sizeof( CHAR16) * 255 );
		}
	}

	SetCurrentWorldSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ );
	}
	else
	{
	send_ready();
	}
}
else if(!allowlaptop && is_client && !is_server)
{
   ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[16] );
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
		/*memset( pObject, 0, sizeof( OBJECTTYPE ) * usNumberOfItems );*/
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
				//memcpy( &pObject[ uiCount ], &Object, sizeof( OBJECTTYPE ) );//fixed for NIV
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
		//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"send_stop" );
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
			//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"skipped" );
			return;
		}
		stop_struct.sXPos=SStopMerc->sXPos;
		stop_struct.sYPos=SStopMerc->sYPos;
		client->RPC("sendSTOP",(const char*)&stop_struct, (int)sizeof(EV_S_STOP_MERC)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieveSTOP (RPCParameters *rpcParameters)
{
	//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"recieve_stop" );
	
	EV_S_STOP_MERC* SStopMerc =(EV_S_STOP_MERC*)rpcParameters->input;
	
	SOLDIERTYPE *pSoldier = MercPtrs[ SStopMerc->usSoldierID ];

		//INT16  sCellX, sCellY;
		//sCellX = CenterX( SStopMerc->sGridNo );
		//sCellY = CenterY( SStopMerc->sGridNo );	

		
	pSoldier->EVENT_InternalSetSoldierPosition( SStopMerc->sXPos, SStopMerc->sYPos,FALSE, FALSE, FALSE );
		pSoldier->EVENT_SetSoldierDirection( SStopMerc->ubDirection );
		//pSoldier->HaultSoldierFromSighting(TRUE);
		pSoldier->AdjustNoAPToFinishMove( SStopMerc->fset );
		//EVENT_StopMerc( pSoldier, SStopMerc->sGridNo, SStopMerc->bDirection );
		//pSoldier->AdjustNoAPToFinishMove( TRUE );		

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
	
	//if(gubOutOfTurnPersons>0)ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Sent Int: bTeam-%d ubID-%d", INT.bTeam,INT.ubID );
	
	gTacticalStatus.ubCurrentTeam=INT.bTeam;
	
	client->RPC("sendINTERRUPT",(const char*)&INT, (int)sizeof(INT_STRUCT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);


}

void recieveINTERRUPT (RPCParameters *rpcParameters)
{
	INT_STRUCT* INT = (INT_STRUCT*)rpcParameters->input;
	

		if(INT->bTeam==netbTeam)
			{
				INT->bTeam=0;
				INT->ubID=INT->ubID - ubID_prefix;

				for(int i=0; i <= INT->gubOutOfTurnPersons; i++)
				{
					if((INT->gubOutOfTurnOrder[i] >= ubID_prefix) && (INT->gubOutOfTurnOrder[i] < (ubID_prefix+5)))
					{
						INT->gubOutOfTurnOrder[i]=INT->gubOutOfTurnOrder[i]-ubID_prefix;
					}
				}
				memcpy(gubOutOfTurnOrder,INT->gubOutOfTurnOrder, sizeof(UINT8) * MAXMERCS);
				gubOutOfTurnPersons = INT->gubOutOfTurnPersons;
					
			}

		if(	INT->bTeam !=0)
			{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[17] );
			//stop moving merc who was interrupted and init UI bar
			SOLDIERTYPE* pMerc = MercPtrs[ INT->ubID ];
			//AdjustNoAPToFinishMove( pMerc, TRUE );	
			pMerc->HaultSoldierFromSighting(TRUE);
			//pMerc->fTurningFromPronePosition = FALSE;// hmmm ??
			FreezeInterfaceForEnemyTurn();
			InitEnemyUIBar( 0, 0 );
			fInterfacePanelDirty = DIRTYLEVEL2;
			AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ INT->bTeam ] );
			gTacticalStatus.fInterruptOccurred = TRUE;
			
			}
		else
			{
				//it for us ! :)
				if(INT->gubOutOfTurnPersons==0)//indicates finished interrupt maybe can just call end interrupt
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"old int finish" );
	//prob never get here again, and can be removed later

				}
				else //start our interrupt turn
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[37] );

					SOLDIERTYPE* pSoldier = MercPtrs[ INT->ubID ];
					SOLDIERTYPE* pOpponent = MercPtrs[ INT->Interrupted];
					ManSeesMan(pSoldier,pOpponent,pOpponent->sGridNo,pOpponent->pathing.bLevel,2,1);
					StartInterrupt();
				}
			}

	
		//if(INT->gubOutOfTurnPersons > 0)ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Recieved Int: bTeam-%d ubID-%d", INT->bTeam,INT->ubID );
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
	
	if(INT->bTeam==netbTeam)//may need working
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[18] );

		for(int i=0; i <= INT->gubOutOfTurnPersons; i++)
			{
				if((INT->gubOutOfTurnOrder[i] >= ubID_prefix) && (INT->gubOutOfTurnOrder[i] < (ubID_prefix+5)))
				{
					INT->gubOutOfTurnOrder[i]=INT->gubOutOfTurnOrder[i]-ubID_prefix;
				}
			}

		memcpy(gubOutOfTurnOrder,INT->gubOutOfTurnOrder, sizeof(UINT8) * MAXMERCS);
		gubOutOfTurnPersons = INT->gubOutOfTurnPersons;



		EndInterrupt( INT->fMarkInterruptOccurred );
	}
}

void grid_display ( void ) //print mouse coordinates, helpfull for crate placement.
{
	INT16	sGridX, sGridY;
	UINT16 usMapPos;

	GetMouseXY( &sGridX, &sGridY );
	usMapPos = MAPROWCOLTOPOS( sGridY, sGridX );
	
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[19] );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[20], sGridX, sGridY );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[21], usMapPos );
}

void mp_help(void)
{
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[2]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[3]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[4]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[5]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[6]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[7]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[8]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[9]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[10]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[11]);
	


}

void mp_help2(void)
{
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[12]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[13]);

	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[14]);
	
	
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[15]);

	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[16]);
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, MPHelp[11]);

}

void manual_overide (void)//now bound to "7"
{
	if(is_server)
	{
	//manual overide command for server
		const STR16 msg = MPClientMessage[23];

			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, msg,  guiCurrentScreen, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS | MSG_BOX_FLAG_USE_CENTERING_RECT, overide_callback,  &CenterRect );

	}
	else	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[22] );
	
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
					ScreenMsg( FONT_LTBLUE, MSG_CHAT, MPClientMessage[13]);

					client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

			}
		

		goahead = 0;
		numready = 0;
		readystage = 0;
		status = 0;
	
	

	}
}

void requestSETTINGS(void)
{
	client_info cl_name;
	//cl_name.client_num=CLIENT_NUM;
	strcpy(cl_name.client_name , CLIENT_NAME);
	cl_name.team=TEAM;
	//cl_name.cl_ops[1]=OP_TEAM_2;
	//cl_name.cl_ops[2]=OP_TEAM_3;
	//cl_name.cl_ops[3]=OP_TEAM_4;
	cl_name.cl_edge=atoi(SECT_EDGE);
	

	client->RPC("requestSETTINGS",(const char*)&cl_name, (int)sizeof(client_info)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}




void recieveSETTINGS (RPCParameters *rpcParameters) //recive settings from server
{

	settings_struct* cl_lan = (settings_struct*)rpcParameters->input;

				char szDefault[30];
				sprintf(szDefault, "%s",cl_lan->client_name);

		if(!recieved_settings)
		{
			recieved_settings=1;

			CLIENT_NUM=cl_lan->client_num;//assign client number from server

			netbTeam = (CLIENT_NUM)+5;
			ubID_prefix = gTacticalStatus.Team[ netbTeam ].bFirstID;//over here now

			memcpy( client_names , cl_lan->client_names, sizeof( char ) * 4 * 30 );
			
			strcpy(client_names[cl_lan->client_num-1],szDefault);

		
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[2] );
			

			MAX_CLIENTS=cl_lan->max_clients;
			memcpy( ckbag, cl_lan->kitbag, sizeof(char)*100);
			
			int cnt;
			int numnum = 0;
			int kitnum = 0;
			char tempstring[4];
			memset( &kit, 0, sizeof( int )*20 );
		
			for(cnt=0;  cnt < 100; cnt++)
			{
				char tempc = ckbag[cnt];
				
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
				//temp = atoi(tempstring);
				}
			}


			DAMAGE_MULTIPLIER=cl_lan->damage_multiplier;
			SAME_MERC=cl_lan->same_merc;
			

			gsMercArriveSectorX=cl_lan->gsMercArriveSectorX;
			gsMercArriveSectorY=cl_lan->gsMercArriveSectorY;

			PLAYER_BSIDE=cl_lan->gsPLAYER_BSIDE;
			DISABLE_MORALE=cl_lan->emorale;
			ChangeSelectedMapSector( gsMercArriveSectorX, gsMercArriveSectorY, 0 );
			CHAR16 str[128];
			GetSectorIDString( gsMercArriveSectorX, gsMercArriveSectorY, 0, str, TRUE );
			//new  ---------
			gGameOptions.fTurnTimeLimit=cl_lan->sofTurnTimeLimit;
			INT32 secs_per_tick=cl_lan->secs_per_tick;
			PLAYER_TEAM_TIMER_SEC_PER_TICKS=secs_per_tick;

			INT32 clstarting_balance=cl_lan->starting_balance;//set starting balance

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
			
			gGameOptions.fIronManMode=cl_lan->sofIronManMode;
			gGameOptions.ubBobbyRay=cl_lan->soubBobbyRay;

			//gGameOptions.ubInventorySystem=cl_lan->sofNewInv;

			if(!cl_lan->soDis_Bobby)LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;

			if(!cl_lan->soDis_Equip)
				ALLOW_EQUIP=TRUE;

			MAX_MERCS=cl_lan->gsMAX_MERCS;
			TESTING=cl_lan->TESTING;
			REPORT_NAME=cl_lan->gsREPORT_NAME;

			ScreenMsg( FONT_YELLOW, MSG_CHAT, MPClientMessage[24],str,MAX_CLIENTS,MAX_MERCS,PLAYER_BSIDE,SAME_MERC,DAMAGE_MULTIPLIER,gGameOptions.fTurnTimeLimit,secs_per_tick,cl_lan->soDis_Bobby,cl_lan->soDis_Equip,DISABLE_MORALE,TESTING);
			if(TESTING)	ScreenMsg( FONT_WHITE, MSG_CHAT, MPClientMessage[25] );
		
			// WANNE - MP: I just added the NUM_SEC_IN_DAY so the game starts at Day 1 instead of Day 0
			gGameExternalOptions.iGameStartingTime= NUM_SEC_IN_DAY + int(cl_lan->TIME*3600);
			// WANNE - MP: In mulitplayer the hired merc arrive immediatly, so iFirstArrivalDelay must be set to 0
			// This also fixed the bug of the wrong hired hours!
			gGameExternalOptions.iFirstArrivalDelay = 0;

			// WANNE: fix HOT DAY in night at arrival by night.
			// Explanation: If game starting time + first arrival delay < 07:00 (111600) -> we arrive before the sun rises or
			// if game starting time + first arrival delay >= 21:00 (162000) -> we arrive after the sun goes down
			if( (gGameExternalOptions.iGameStartingTime + gGameExternalOptions.	iFirstArrivalDelay) < 111600 ||
				(gGameExternalOptions.iGameStartingTime + gGameExternalOptions.iFirstArrivalDelay >= 162000))
			{ 
				gubEnvLightValue = 12; 
				LightSetBaseLevel(gubEnvLightValue); 
			} 

			InitNewGameClock( );
						
			WEAPON_READIED_BONUS=cl_lan->WEAPON_READIED_BONUS;

			
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[26],cl_lan->client_num,szDefault,cl_lan->cl_edge,cl_lan->team );
//		if(PLAYER_BSIDE==0)ScreenMsg(FONT_LTGREEN,MSG_CHAT,MPClientMessage[27],cl_lan->cl_ops[0],cl_lan->cl_ops[1],cl_lan->cl_ops[2],cl_lan->cl_ops[3]);
		//ScreenMsg(FONT_LTGREEN,MSG_CHAT,MPClientMessage[27],cl_lan->team);
	
				strcpy(client_names[cl_lan->client_num-1],szDefault);				

		}
		else 
		{
			
			
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[26],cl_lan->client_num,szDefault,cl_lan->cl_edge,cl_lan->team );
		//	ScreenMsg(FONT_LTGREEN,MSG_CHAT,MPClientMessage[27],cl_lan->team);	
				strcpy(client_names[cl_lan->client_num-1],szDefault);				

		}

}


void send_bullet(  BULLET * pBullet,UINT16 usHandItem )
{	
	netb_struct netb;
	netb.net_bullet=*pBullet;
	netb.usHandItem=usHandItem;

	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Sent Bullet Id: %d",pBullet->iBullet);

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
			ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Failed to create bullet");		
		}
//add bullet to bullet table for translation

		bTable[bTeam][net_iBullet].remote_id = net_iBullet;
		bTable[bTeam][net_iBullet].local_id = iBullet;

		pBullet = GetBulletPtr( iBullet );

		//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Created Bullet Id: %d",iBullet);		

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

		if(new_state.usSoldierID < 20)new_state.usSoldierID = new_state.usSoldierID+ubID_prefix;
	
			

	client->RPC("sendSTATE",(const char*)&new_state, (int)sizeof(EV_S_CHANGESTATE)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"sent state");		


}

void recieveSTATE(RPCParameters *rpcParameters)
{

	EV_S_CHANGESTATE*	new_state = (EV_S_CHANGESTATE*)rpcParameters->input;


	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved state");	
	

	SOLDIERTYPE * pSoldier=MercPtrs[ new_state->usSoldierID ];

	if(pSoldier->bActive)
		pSoldier->EVENT_InitNewSoldierAnim( new_state->usNewState, new_state->usStartingAniCode, new_state->fForce );
//AddGameEvent( S_CHANGESTATE, 0, &SChangeState );
	//someday ;)
}

void send_death( SOLDIERTYPE *pSoldier )
{
	death_struct nDeath;
	nDeath.soldier_id=pSoldier->ubID;
	nDeath.attacker_id=pSoldier->ubAttackerID;
		if(pSoldier->ubAttackerID==NULL)nDeath.attacker_id=pSoldier->ubPreviousAttackerID;
		if(pSoldier->ubID<20)nDeath.soldier_id=nDeath.soldier_id+ubID_prefix;
		
	client->RPC("sendDEATH",(const char*)&nDeath, (int)sizeof(death_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		
	SOLDIERTYPE * pAttacker=MercPtrs[ nDeath.attacker_id ];
	INT8 pA_bTeam;
	CHAR16	pA_name[ 10 ];
	INT8 pS_bTeam;
	CHAR16	pS_name[ 10 ];
	if(pAttacker)
	{
	pA_bTeam=pAttacker->bTeam;
	memcpy(pA_name,pAttacker->name,sizeof(CHAR16)*10);
	if(pA_bTeam>5)pA_bTeam=pA_bTeam-5;
	if(pA_bTeam==0)pA_bTeam=CLIENT_NUM;
	}
	if(pSoldier)
	{
	pS_bTeam=pSoldier->bTeam;
	memcpy(pS_name,pSoldier->name,sizeof(CHAR16)*10);
	if(pS_bTeam>5)pS_bTeam=pS_bTeam-5;
	if(pS_bTeam==0)pS_bTeam=CLIENT_NUM;
	}
	if (pSoldier->bTeam==1)  ScreenMsg( FONT_YELLOW, MSG_CHAT, L"You Killed An Enemy AI");	
	else  ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[28],pS_name,(pS_bTeam),client_names[(pS_bTeam-1)],pA_name,(pA_bTeam),client_names[(pA_bTeam-1)] );
	//ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[28],(pS_bTeam), pS_name,client_names[(pS_bTeam-1)],pA_name,(pA_bTeam),client_names[(pA_bTeam-1)] );

}

void recieveDEATH (RPCParameters *rpcParameters)
{

	death_struct* nDeath = (death_struct*)rpcParameters->input;
	SOLDIERTYPE * pSoldier=MercPtrs[ nDeath->soldier_id ];

	UINT16 ubAttackerID;
					if((nDeath->attacker_id >= ubID_prefix) && (nDeath->attacker_id < (ubID_prefix+5)))
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
	if(pA_bTeam>5)pA_bTeam=pA_bTeam-5;
	if(pA_bTeam==0)pA_bTeam=CLIENT_NUM;
	}
	if(pSoldier)
	{
	pS_bTeam=pSoldier->bTeam;
	memcpy(pS_name,pSoldier->name,sizeof(CHAR16)*10);
	if(pS_bTeam>5)pS_bTeam=pS_bTeam-5;
	if(pS_bTeam==0)pS_bTeam=CLIENT_NUM;
	}
			

	if(pSoldier->bActive)
	{
		pSoldier->usAnimState=50;
		ScreenMsg( FONT_YELLOW, MSG_CHAT, L"made merc corpse/dead");	
		TurnSoldierIntoCorpse( pSoldier, TRUE, TRUE );
			if ( CheckForEndOfBattle( FALSE ) )
			{
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"All over red rover...");
			}
		if (pSoldier->bTeam==1)  ScreenMsg( FONT_YELLOW, MSG_CHAT, L"An Enemy AI was killed...");	
		else ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[28],pS_name,(pS_bTeam),client_names[(pS_bTeam-1)],pA_name,(pA_bTeam),client_names[(pA_bTeam-1)] );
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[28],(pS_bTeam),pS_name,client_names[(pS_bTeam-1)],pA_name,(pA_bTeam),client_names[(pA_bTeam-1)] );

	}
	else
	{
		ScreenMsg( FONT_YELLOW, MSG_CHAT, L"merc allready corpse/dead");	
		if (pSoldier->bTeam==1)  ScreenMsg( FONT_YELLOW, MSG_CHAT, L"An Enemy AI was killed...");	
		CheckForEndOfBattle( FALSE );
	}

}

void send_hitstruct(EV_S_STRUCTUREHIT	*	SStructureHit)
{
	EV_S_STRUCTUREHIT struct_hit;
	memcpy( &struct_hit , SStructureHit, sizeof( EV_S_STRUCTUREHIT ));
	if(SStructureHit->ubAttackerID <20)struct_hit.ubAttackerID = struct_hit.ubAttackerID+ubID_prefix;
			

	client->RPC("sendhitSTRUCT",(const char*)&struct_hit, (int)sizeof(EV_S_STRUCTUREHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void send_hitwindow(EV_S_WINDOWHIT * SWindowHit)
{
		EV_S_WINDOWHIT window_hit;
	memcpy( &window_hit , SWindowHit, sizeof( EV_S_WINDOWHIT ));
	if(SWindowHit->ubAttackerID <20)window_hit.ubAttackerID = window_hit.ubAttackerID+ubID_prefix;
			

	client->RPC("sendhitWINDOW",(const char*)&window_hit, (int)sizeof(EV_S_WINDOWHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void send_miss(EV_S_MISS * SMiss)
{

		EV_S_MISS shot_miss;
	memcpy( &shot_miss , SMiss, sizeof( EV_S_MISS ));
	if(SMiss->ubAttackerID <20)shot_miss.ubAttackerID = shot_miss.ubAttackerID+ubID_prefix;
			

	client->RPC("sendMISS",(const char*)&shot_miss, (int)sizeof(EV_S_MISS)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recievehitSTRUCT  (RPCParameters *rpcParameters)
{
	EV_S_STRUCTUREHIT* struct_hit = (EV_S_STRUCTUREHIT*)rpcParameters->input;
		
		SOLDIERTYPE *pSoldier = MercPtrs[ struct_hit->ubAttackerID ];
		INT8 bTeam=pSoldier->bTeam;
		INT32 iBullet = bTable[bTeam][struct_hit->iBullet].local_id;

	StructureHit( iBullet, struct_hit->usWeaponIndex, struct_hit->bWeaponStatus, struct_hit->ubAttackerID, struct_hit->sXPos, struct_hit->sYPos, struct_hit->sZPos, struct_hit->usStructureID, struct_hit->iImpact, struct_hit->fStopped );
	if(struct_hit->fStopped)RemoveBullet(iBullet);
	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved structure hit");
}
void recievehitWINDOW  (RPCParameters *rpcParameters)
{
	EV_S_WINDOWHIT* window_hit = (EV_S_WINDOWHIT*)rpcParameters->input;



	WindowHit( window_hit->sGridNo, window_hit->usStructureID, window_hit->fBlowWindowSouth, window_hit->fLargeForce );

	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved window hit");
}
void recieveMISS  (RPCParameters *rpcParameters)
{
	EV_S_MISS* shot_miss = (EV_S_MISS*)rpcParameters->input;

		SOLDIERTYPE *pSoldier = MercPtrs[ shot_miss->ubAttackerID ];
		INT8 bTeam=pSoldier->bTeam;
		INT32 iBullet = bTable[bTeam][shot_miss->iBullet].local_id;


	ShotMiss( shot_miss->ubAttackerID, iBullet );
	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved shot miss");	
}

void cheat_func(void)
{
	if(TESTING)
	{
	ScreenMsg( FONT_YELLOW, MSG_CHAT, L"gTacticalStatus.uiFlags |= SHOW_ALL_MERCS");	
	gTacticalStatus.uiFlags |= SHOW_ALL_MERCS;
	}
}

BOOLEAN check_status (void)// any 'enemies' and clients left to fight ??
{
	SOLDIERTYPE *pSoldier;
	int cnt;
	int soldiers= 0 ;
	


				for(int x=0;x < MAXTEAMS; x++)
				{

					for(cnt = gTacticalStatus.Team[ x ].bFirstID;cnt <= gTacticalStatus.Team[ x ].bLastID; cnt++)
					{
						pSoldier = MercPtrs[ cnt ];
						if(pSoldier->stats.bLife!=0 && pSoldier->bActive && pSoldier->bInSector)
						{
							soldiers++;
						}
					}
					if(soldiers>0)
					{
						gTacticalStatus.Team[ x ].bTeamActive=1;
						gTacticalStatus.Team[x].bMenInSector=soldiers;
					
						soldiers=0 ;
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
							ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[40] );
							gTacticalStatus.uiFlags |= SHOW_ALL_MERCS;//hayden
							ScreenMsg( FONT_YELLOW, MSG_CHAT, MPClientMessage[41] );
							teamwiped();
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, MPClientMessage[42] );

				}
				if((gTacticalStatus.Team[ 0 ].bTeamActive==1 || gTacticalStatus.Team[ 6 ].bTeamActive==1 || gTacticalStatus.Team[ 7 ].bTeamActive==1 || gTacticalStatus.Team[ 0 ].bTeamActive==1 || gTacticalStatus.Team[ 9 ].bTeamActive==1  )&& NumEnemyInSector() > 0)return(TRUE);
				else return(FALSE);

		
					
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
			//ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"update: %d ",time );
			

			EV_S_UPDATENETWORKSOLDIER SUpdateNetworkSoldier;

			SUpdateNetworkSoldier.usSoldierID=pSoldier->ubID;
			if(pSoldier->ubID < 20)SUpdateNetworkSoldier.usSoldierID=pSoldier->ubID+ubID_prefix;

			SUpdateNetworkSoldier.sAtGridNo=pSoldier->sGridNo;
			SUpdateNetworkSoldier.bBreath=pSoldier->bBreath;
			SUpdateNetworkSoldier.bLife=pSoldier->stats.bLife;
			SUpdateNetworkSoldier.bBleeding=pSoldier->bBleeding;
			SUpdateNetworkSoldier.ubDirection=pSoldier->ubDirection;

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
				//ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"sync ubid:%d grid %d to %d",pSoldier->ubID,pSoldier->sGridNo,SUpdateNetworkSoldier->sAtGridNo  );
			}

			if(pSoldier->ubDirection != SUpdateNetworkSoldier->ubDirection)
			{
				pSoldier->EVENT_SetSoldierDesiredDirection( SUpdateNetworkSoldier->ubDirection );
				//ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"sync ubid:%d dir %d to %d",pSoldier->ubID, pSoldier->ubDirection, SUpdateNetworkSoldier->ubDirection  );
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
	//manual overide command for server
		const STR16 msg = L"Choose Client Number to Kick:";

			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, msg,  guiCurrentScreen, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS | MSG_BOX_FLAG_USE_CENTERING_RECT, kick_callback,  &CenterRect );

	}
	else	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[22] );
	
}
void kick_callback (UINT8 ubResult)
{
	
	kickR kick;
	kick.ubResult=ubResult+5;
	
	//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"kicking client " );
	client->RPC("Snull_team",(const char*)&kick, (int)sizeof(kickR)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void null_team (RPCParameters *rpcParameters)
{
	kickR* kick = (kickR*)rpcParameters->input;
	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[29],(kick->ubResult-5),client_names[kick->ubResult-6] );
	int fID = gTacticalStatus.Team[ kick->ubResult ].bFirstID;
	int lID = gTacticalStatus.Team[ kick->ubResult ].bLastID;

	if(kick->ubResult==netbTeam)fID=0,lID=19;
	int cnt;
	for ( cnt=fID ; cnt <= lID; cnt++ )
	{
		TacticalRemoveSoldier( cnt );
	}
	
}

void overide_turn (void)
{
	if(is_server)
	{
	//manual overide command for server
		const STR16 msg = MPClientMessage[30];

			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, msg,  guiCurrentScreen, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS | MSG_BOX_FLAG_USE_CENTERING_RECT, turn_callback,  &CenterRect );

	}
	else	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[22] );
	
}

void turn_callback (UINT8 ubResult)
{

	if(is_server)
	{
		ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, MPClientMessage[31],ubResult );
	
	if(!( gTacticalStatus.uiFlags & INCOMBAT ))
	{
		gTacticalStatus.uiFlags |= INCOMBAT;
	}
	if(ubResult ==1)EndTurn( 0 );		
	else EndTurn( ubResult+5 );
	}
}

void send_fireweapon (EV_S_FIREWEAPON  *SFireWeapon)
{
		EV_S_FIREWEAPON sFire;
		//sFire.usSoldierID=SFireWeapon->usSoldierID;

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
				//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"recieve_fireweapon" );
				//SFireWeapon.usSoldierID			= pSoldier->ubID;
				////SFireWeapon.uiUniqueId			= pSoldier->uiUniqueSoldierIdValue;
				//SFireWeapon.sTargetGridNo		= pSoldier->sTargetGridNo;
				//SFireWeapon.bTargetLevel		= pSoldier->bTargetLevel;
				//SFireWeapon.bTargetCubeLevel= pSoldier->bTargetCubeLevel;
				//AddGameEvent( S_FIREWEAPON, 0, &SFireWeapon );

	SOLDIERTYPE *pSoldier = MercPtrs[ SFireWeapon->usSoldierID ];

				pSoldier->sTargetGridNo = SFireWeapon->sTargetGridNo;
				pSoldier->bTargetLevel = SFireWeapon->bTargetLevel;
				pSoldier->bTargetCubeLevel = SFireWeapon->bTargetCubeLevel;
				FireWeapon( pSoldier, SFireWeapon->sTargetGridNo  );

}

void send_door ( SOLDIERTYPE *pSoldier, INT16 sGridNo, BOOLEAN fNoAnimations )
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
	HandleDoorChangeFromGridNo( pSoldier, sDoor->sGridNo, sDoor->fNoAnimations );

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

	sc_struct data;
	data.ubStartingTeam=netbTeam;

	client->RPC("sendWIPE",(const char*)&data, (int)sizeof(sc_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void recieve_wipe (RPCParameters *rpcParameters)

{
	sc_struct* data = (sc_struct*)rpcParameters->input;
	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"Team #%d is wiped out.", data->ubStartingTeam );
if(is_server)
{
	if(gTacticalStatus.ubCurrentTeam==data->ubStartingTeam)EndTurn( data->ubStartingTeam+1 );	
}
}

//***************************
//*** client connection*****
//*************************

void connect_client ( void )
{
	
		if(!is_client)
		{
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[0] );

			
							
		
			client=RakNetworkFactory::GetRakPeerInterface();
			bool b = client->Startup(1,30,&SocketDescriptor(), 1);

			//RPC's
			REGISTER_STATIC_RPC(client, recievePATH);
			REGISTER_STATIC_RPC(client, recieveSTANCE);
			REGISTER_STATIC_RPC(client, recieveDIR);
			REGISTER_STATIC_RPC(client, recieveFIRE);
			REGISTER_STATIC_RPC(client, recieveHIT);
			REGISTER_STATIC_RPC(client, recieveHIRE);
			REGISTER_STATIC_RPC(client, recieveguiPOS);
			REGISTER_STATIC_RPC(client, recieveguiDIR);
			REGISTER_STATIC_RPC(client, recieveEndTurn);
			REGISTER_STATIC_RPC(client, recieveAI);
			REGISTER_STATIC_RPC(client, recieveSTOP);
			REGISTER_STATIC_RPC(client, recieveINTERRUPT);
			REGISTER_STATIC_RPC(client, recieveREADY);
			REGISTER_STATIC_RPC(client, recieveGUI);
			REGISTER_STATIC_RPC(client, recieveSETTINGS);
			REGISTER_STATIC_RPC(client, recieveBULLET);
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
			//***
			
		if (b)
		{
			//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client started, waiting for connections...");
			is_client=true;
			/*repo=0;*/
		}
		else
		{ 
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client failed to start.  Terminating.");
			
		}
			
		is_connected=false;

	
		
			
			
			
		}	
		
		//reconnect/connect
		if( !is_connected && !is_connecting)
		{

			recieved_settings=0;
			goahead = 0;
			numready = 0;
			readystage = 0;
			status = 0;
			gTacticalStatus.uiFlags&= (~SHOW_ALL_MERCS );
			memset( &readyteamreg , 0 , sizeof (int) * 10);

			//retrieve settings from Ja2_mp.ini
			char ip[30];
			char port[30];
//			char client_number[30];
			char sector_edge[30];
			wiped=0;
			
			//disable cheating
			gubCheatLevel = 0;

			//char c_x[30];
			
		
			char clname[30];



			MAX_CLIENTS=0;//reset server only set settings.
			//INTERRUPTS=0;
			DAMAGE_MULTIPLIER=0;
			SAME_MERC=0;

			GetPrivateProfileString( "Ja2_mp Settings","SERVER_IP", "", ip, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SERVER_PORT", "", port, MAX_PATH, "..\\Ja2_mp.ini" );
		//	GetPrivateProfileString( "Ja2_mp Settings","CLIENT_NUM", "", client_number, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SECTOR_EDGE", "", sector_edge, MAX_PATH, "..\\Ja2_mp.ini" );


			//GetPrivateProfileString( "Ja2_mp Settings","CRATE_X", "", c_x, MAX_PATH, "..\\Ja2_mp.ini" );
		
			
			GetPrivateProfileString( "Ja2_mp Settings","CLIENT_NAME", "", clname, MAX_PATH, "..\\Ja2_mp.ini" );

		

	

			char op1[30];
			//char op2[30];
			//char op3[30];
			//char op4[30];

			GetPrivateProfileString( "Ja2_mp Settings","TEAM", "", op1, MAX_PATH, "..\\Ja2_mp.ini" );
			//GetPrivateProfileString( "Ja2_mp Settings","OP_TEAM_2", "", op2, MAX_PATH, "..\\Ja2_mp.ini" );
			//GetPrivateProfileString( "Ja2_mp Settings","OP_TEAM_3", "", op3, MAX_PATH, "..\\Ja2_mp.ini" );
			//GetPrivateProfileString( "Ja2_mp Settings","OP_TEAM_4", "", op4, MAX_PATH, "..\\Ja2_mp.ini" );
			/*char stt[30];
			GetPrivateProfileString( "Ja2_mp Settings","START_TEAM_TURN", "", stt, MAX_PATH, "..\\Ja2_mp.ini" );*/

			
			TEAM=atoi(op1);
				//OP_TEAM_2=atoi(op2);
				//	OP_TEAM_3=atoi(op3);
				//		OP_TEAM_4=atoi(op4);

			strcpy( CLIENT_NAME, clname);

			
			if(is_server)strcpy( SERVER_IP , "127.0.0.1" );
			else strcpy( SERVER_IP , ip );

			strcpy( SERVER_PORT, port );
			
			/*if (is_server) strcpy( CLIENT_NUM, "1" );
			else strcpy( CLIENT_NUM, client_number );*/
			
			strcpy( SECT_EDGE, sector_edge);
			
			//START_TEAM_TURN=atoi(stt);
			//crate_sGridX = atoi(c_x);
//			crate_sGridY = atoi(c_y);

			/*netbTeam = (CLIENT_NUM)+5;
			ubID_prefix = gTacticalStatus.Team[ netbTeam ].bFirstID;*/




			//**********************
			//here some nifty little tweaks

				LaptopSaveInfo.guiNumberOfMercPaymentsInDays += 20;
				LaptopSaveInfo.gubLastMercIndex = LAST_MERC_ID;
				
				LaptopSaveInfo.ubLastMercAvailableId = 7;
				
				extern BOOLEAN gfTemporaryDisablingOfLoadPendingFlag;
				gfTemporaryDisablingOfLoadPendingFlag = TRUE;
				SetBookMark( AIM_BOOKMARK );
				SetBookMark( BOBBYR_BOOKMARK );
				//SetBookMark( IMP_BOOKMARK );
				SetBookMark( MERC_BOOKMARK );

				gMercProfiles[ 57 ].sSalary = 2000;
				gMercProfiles[ 58 ].sSalary = 1500;
				gMercProfiles[ 59 ].sSalary = 600;
				gMercProfiles[ 60 ].sSalary = 500;
				gMercProfiles[ 64 ].sSalary = 1500;
				gMercProfiles[ 72 ].sSalary = 1000;
				gMercProfiles[ 148 ].sSalary = 100;

							
			//**********************
			
			
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[1],SERVER_IP);
			client->Connect(SERVER_IP, atoi(SERVER_PORT), 0,0);
			is_connecting=true;
			
		}
	
		else if (is_connecting)
		{
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[4] );
		}
		else if (is_connected)
		{
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, MPClientMessage[3] );
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
			//continue; // Didn't get any packets

		// We got a packet, get the identifier with our handy function
		packetIdentifier = GetPacketIdentifier(p);
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"packet recieved");
		// Check if this is a network message packet
		switch (packetIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION:
				  // Connection lost normally
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_DISCONNECTION_NOTIFICATION");
				is_connected=false;
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_ALREADY_CONNECTED");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_REMOTE_DISCONNECTION_NOTIFICATION");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_REMOTE_NEW_INCOMING_CONNECTION");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_CONNECTION_ATTEMPT_FAILED");
				is_connected=false;
				is_connecting=false;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				 // Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_NO_FREE_INCOMING_CONNECTIONS");
				break;
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_CONNECTION_LOST");
				is_connected=false;
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_CONNECTION_REQUEST_ACCEPTED");
				is_connected=true;
				is_connecting=false;
				requestSETTINGS();
				//request_settings();//ask server for game settings...
				break;
				case ID_NEW_INCOMING_CONNECTION:
				//tells server client has connected
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_NEW_INCOMING_CONNECTION");
				break;
			case ID_MODIFIED_PACKET:
				// Cheater!
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_MODIFIED_PACKET");
				break;
			default:
					
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"** a packet has been recieved for which i dont know what to do... **");
				break;
		}


		// We're done with the packet, get more :)
		client->DeallocatePacket(p);
		p = client->Receive();
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
	client->Shutdown(300);
	is_client = false;
	is_connected=false;
	is_connecting=false;
	
	allowlaptop=false;

			

	// We're done with the network
	RakNetworkFactory::DestroyRakPeerInterface(client);
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client disconnected and shutdown");
	}
	else
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client is not running");
	}
}