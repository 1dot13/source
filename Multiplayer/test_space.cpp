#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "strategic.h"
#else
	#include "builddefines.h"
#include "Bullets.h"
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

#include "tactical placement gui.h"
#include "connect.h"
#include "network.h"

#pragma pack(1)

#include "text.h"
#include "Types.h"
#include "connect.h"
#include "message.h"
#include "Event pump.h"
#include "Soldier Init List.h"
#include "Overhead.h"
#include "weapons.h"
#include "Merc Hiring.h"
#include "soldier profile.h"

#include"laptop.h"
	#include "florist Order Form.h"
#include "prebattle interface.h"
#include "teamturns.h"
extern INT8 SquadMovementGroups[ ];

#include "test_space.h"
#include "soldier control.h"

bool ovh_advance;
bool ovh_ready;

#include "opplist.h"

//int cnt = 40;


void test_func2 (void)//now bound to "0" 
{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"test_func2 - function testing ground:" );


SOLDIERTYPE * pSoldier=MercPtrs[ 0 ];

PauseAITemporarily();

//RemoveOneOpponent( pSoldier );
//SOLDIERTYPE * pSoldier2=MercPtrs[ 127 ];
//
////pSoldier->AdjustNoAPToFinishMove( TRUE );
//
////pSoldier2->bVisible = 1;
//ManSeesMan(pSoldier,pSoldier2,pSoldier2->sGridNo,pSoldier2->pathing.bLevel,0,1);


/*
INT16  sCellX, sCellY;
			sCellX = CenterX( 22163 );
			sCellY = CenterY( 22163 );	

pSoldier->EVENT_InternalSetSoldierPosition( sCellX, sCellY ,FALSE, FALSE, FALSE );*/




//SOLDIERTYPE * pSoldier=MercPtrs[ 0 ];
//pSoldier->bLife=0;
//SoldierCollapse( pSoldier );
//SoldierTakeDamage( pFirer, ANIM_CROUCH, 1, 100, TAKE_DAMAGE_BLOODLOSS, NOBODY, NOWHERE, 0, TRUE );
//TurnSoldierIntoCorpse( pFirer, FALSE, FALSE );

//SOLDIERTYPE * pSoldier=MercPtrs[ 0 ];
////
//pSoldier->fNoAPToFinishMove = TRUE;
//
//
//fInterfacePanelDirty = DIRTYLEVEL2;



////EVENT_InitNewSoldierAnim(pSoldier,3,0,0); 
//pSoldier->ubDesiredHeight		 = ANIM_CROUCH;

//SOLDIERTYPE * pSoldier=MercPtrs[ 0 ];
//pSoldier->usAnimState=50;
//TurnSoldierIntoCorpse( pSoldier, TRUE, TRUE );

//gTacticalStatus.uiFlags |= SHOW_ALL_MERCS;
//BeginTeamTurn (0);



;

//gTacticalStatus.ubCurrentTeam = OUR_TEAM;
//guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
//ExitCombatMode();
//fInterfacePanelDirty = DIRTYLEVEL2;


//InitPlayerUIBar( FALSE );


//UIHandleLUIEndLock( NULL );
//if( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
//		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"combat turn based" );
//else
//		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"not" );


//SOLDIERTYPE * pSoldier=MercPtrs[ 0 ];
//
//		if ( ( gAnimControl[ pSoldier->usAnimState ].uiFlags &( ANIM_FIREREADY | ANIM_FIRE ) ) )
//		{
//			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Ready" );
//		}
//		else
//			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Not" );

////fInterfacePanelDirty = DIRTYLEVEL2;
////guiPendingOverrideEvent = LU_ENDUILOCK;
////guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
//
//
//
//					ClearIntList();
//					//guiPendingOverrideEvent = LU_ENDUILOCK;
//					guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
//					fInterfacePanelDirty  = DIRTYLEVEL2;
//					
//					if ( gusSelectedSoldier != NOBODY )
//					{
//					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,SETLOCATOR);
//					MercPtrs[ gusSelectedSoldier ]->DoMercBattleSound( BATTLE_SOUND_ATTN1 );
//					}
//					InitPlayerUIBar( 0 );
//HandleTacticalUI( );
//guiPendingOverrideEvent = LA_BEGINUIOURTURNLOCK;
//int n = 0;
//for(cnt=40+n ; cnt < 40+5+n ; cnt++)
//{
//
//QuickCreateProfileMerc( CIV_TEAM, cnt );jk
//
//RecruitRPC( cnt );
//Sleep(3000);
//}
//QuickCreateProfileMerc( CIV_TEAM, cnt );
//RecruitRPC( cnt );
//cnt++;


//HandleDoorChangeFromGridNo( pSoldier, 18451, FALSE );

//OBJECTTYPE		Object;
//for (cnt=0; cnt<20;cnt++)
//{
//CreateItem( 201, 100, &Object );
//
//AutoPlaceObject( pSoldier, &Object, TRUE );
//
//




//INT16 sCellX, sCellY;
////ConvertGridNoToCellXY( 13576, &sCellX, &sCellY );
//
//	sCellX = CenterX( 13576 );
//	sCellY = CenterY( 13576 );
//
//	//pSoldier->dXPos = sCellX;
//	//pSoldier->dYPos = sCellY;
//
//	//pSoldier->sX = (INT16)sCellX;
//	//pSoldier->sY = (INT16)sCellY;
//	EVENT_InitNewSoldierAnim( pSoldier, 6, 0, FALSE );

	//EVENT_SetSoldierPositionForceDelete( pSoldier, (FLOAT)sCellX, (FLOAT)sCellY );

//HaultSoldierFromSighting( pSoldier, 1 );
//pSoldier->sScheduledStop=7961;
////
////EVENT_SetSoldierPosition( pSoldier, 1140, 1170 );
//AdjustNoAPToFinishMove( pSoldier, TRUE );
//pSoldier->fTurningFromPronePosition = FALSE;
//
////HaultSoldierFromSighting( pSoldier, 1 );




//UINT16 usPathData[30];
//usPathData[0]=3;
//usPathData[1]=5;
//usPathData[2]=7;
//
//pSoldier->sFinalDestination = 8446;
//
//pSoldier->usPathDataSize=3;
//pSoldier->usPathIndex=0;
//
//memcpy(pSoldier->usPathingData,usPathData,sizeof(UINT16)*30);


//INT16 sNewGridNo;

//sNewGridNo = NewGridNo( (UINT16)pSoldier->sGridNo, DirectionInc( (UINT8)pSoldier->usPathingData[ pSoldier->usPathDataSize ] ) );

//pSoldier->sFinalDestination = sNewGridNo;

//EVENT_InitNewSoldierAnim( pSoldier, 0, 0, FALSE );



//ovh_advance=true;
//
//
//	CHAR16 string[255];
//	memcpy(string,TeamTurnString[ 7 ], sizeof( CHAR16) * 255 );
//
//	CHAR16 name[255];
//    mbstowcs( name, CLIENT_NAME, sizeof (char)*30 );
//
//	CHAR16 full[255];
//	swprintf(full, L"%s - '%s'",string,name);
////
////
//	memcpy( TeamTurnString[ 7 ] , full, sizeof( CHAR16) * 255 );


}



//BOOLEAN       fMadeCorpse;
//	 SOLDIERTYPE * pSoldier=MercPtrs[ 0 ];
//pSoldier->bLife = 0;
//    HandleSoldierDeath( pSoldier, &fMadeCorpse );

//guiPendingOverrideEvent = LA_BEGINUIOURTURNLOCK;
//
//guiPendingOverrideEvent = CA_MERC_SHOOT;
//
//guiPendingOverrideEvent = G_GETTINGITEM;
//
//guiPendingOverrideEvent = LU_ON_TERRAIN;
//
//guiPendingOverrideEvent = ET_ON_TERRAIN;
//
//guiPendingOverrideEvent = A_CHANGE_TO_MOVE;











//HandleTacticalUI( );



		//CHAR8	string[60];
	//	wcscpy( string, "GetVideoObject" );
		//sprintf( string, "hello");
	/*	char szDefault[255];
		sprintf(szDefault, "%s","hello");	
			
				
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"%S has connected.",szDefault );*/
	
	//manual overide
	//manual_overide();
	//allow_bullet=1;
	//

	//if(1)
	//{
	//	stage=0;
	//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"1" );
	//	//FireBulletGivenTarget( MercPtrs[0], 875, 995, 1, 11, -74, 0, 0 );
	//	
	//	
	//	INT32 iBullet;
	//	BULLET * pBullet;

	//	iBullet = CreateBullet( 0, 0, 0,11 );
	//	if (iBullet == -1)
	//	{
	//		ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Failed to create bullet");		
	//	}
	//	pBullet = GetBulletPtr( iBullet );

	//	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Created Bullet");		

	//	pBullet->fCheckForRoof=0;
	//	pBullet->qIncrX=-917769;
	//	pBullet->qIncrY=-507159;
	//	pBullet->qIncrZ=-537679;
	//	pBullet->sHitBy=-30;
	//	pBullet->ddHorizAngle=-2.5959375990628013;
	//	pBullet->fAimed=1;
	//	pBullet->ubItemStatus=0;
	//	pBullet->qCurrX=1231159031;
	//	pBullet->qCurrY=1221083881;
	//	pBullet->qCurrZ=182963121;
	//	pBullet->iImpact=28;
	//	pBullet->iRange=200;
	//	pBullet->sTargetGridNo=15929;
	//	pBullet->bStartCubesAboveLevelZ=2;
	//	pBullet->bEndCubesAboveLevelZ=0;
	//	pBullet->iDistanceLimit=328;


	//	SOLDIERTYPE * pFirer=MercPtrs[ 0 ];

	//	//FireBullet( pFirer, pBullet, FALSE );
	//	if(is_client)send_bullet( pBullet, 11 );//hayden


//}
	//else
	//{
	//	stage=1;
	//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"0" );
	//	FireBulletGivenTarget( MercPtrs[0], 875, 995, 1, 11, -74, 0, 0 );
	//}