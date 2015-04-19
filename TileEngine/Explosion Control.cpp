#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
#include "TileEngine All.h"
#include "end game.h"
#include "Morale.h"
#else
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "debug.h"
//#include "soldier control.h"
#include "weapons.h"
#include "handle items.h"
#include "worlddef.h"
#include "worldman.h"
#include "rotting corpses.h"
#include "tile cache.h"
#include "isometric utils.h"
#include "animation control.h"
#include "utilities.h"
#include "game clock.h"
#include "soldier create.h"
#include "renderworld.h"
#include "soldier add.h"
#include "explosion control.h"
#include "tile animation.h"
#include "sound control.h"
#include "weapons.h"
#include "handle items.h"
#include "world items.h"
#include "structure wrap.h"
#include "tiledef.h"
#include "tiledat.h"
#include "interactive tiles.h"
#include "SaveLoadMap.h"
#include "Message.h"
#include "Random.h"
#include "smokeeffects.h"
#include "handle ui.h"
#include "pathai.h"
#include "campaign Types.h"
#include "strategicmap.h"
#include "strategic.h"
#include "Action Items.h"
#include "Soldier Profile.h"
#include "Quests.h"
#include "Interface Dialogue.h"
#include "LightEffects.h"
#include "AI.h"
#include "Soldier tile.h"
#include "lighting.h"
#include "Render Fun.h"
#include "Opplist.h"
#include "smell.h"
#include "GameSettings.h"
#include "Interface.h"
#include "end game.h"
#include "WorldDat.h"
#include "environment.h"
#include "Buildings.h"
#include "Keys.h"
#include "Morale.h"
#include "fov.h"
#include "Map Information.h"
#include "Soldier Functions.h"//dnl ch40 200909
#include "Text.h" // added by SANDRO
#include "campaign.h" // yet another one added
#include "CampaignStats.h"		// added by Flugente
#endif

#include "Soldier Macros.h"
#include "connect.h"
#include "debug control.h"

#include "LuaInitNPCs.h"
#include "Luaglobal.h"
#include "Item Statistics.h"


#ifdef JA2UB
#include "interface Dialogue.h"

#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "Dialogue Control.h"
#include "ub_config.h"
#endif


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
//DBrot: More Rooms
//BOOLEAN HookerInRoom( UINT8 ubRoom );
BOOLEAN HookerInRoom( UINT16 usRoom );

// MODULE FOR EXPLOSIONS

// Spreads the effects of explosions...
BOOLEAN ExpAffect( INT32 sBombGridNo, INT32 sGridNo, UINT32 uiDist, UINT16 usItem, UINT8 ubOwner, INT16 sSubsequent, BOOLEAN *pfMercHit, INT8 bLevel, INT32 iSmokeEffectID );

// Flashbang effect on soldier
UINT8 DetermineFlashbangEffect( SOLDIERTYPE *pSoldier, INT8 ubExplosionDir, BOOLEAN fInBuilding);

// HEADROCK HAM 5.1: Explosion Fragments launcher
void FireFragments( UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, UINT16 usItem, UINT8 ubDirection = DIRECTION_IRRELEVANT );

// Flugente: shoot a gun without anyone operating it (used for makeshift traps wih guns)
void FireFragmentsTrapGun( SOLDIERTYPE* pThrower, INT32 gridno, INT16 sZ, OBJECTTYPE* pObj, UINT8 ubDirection = NORTH );

extern INT8	gbSAMGraphicList[ MAX_NUMBER_OF_SAMS ];
extern	void AddToShouldBecomeHostileOrSayQuoteList( UINT8 ubID );
extern void RecompileLocalMovementCostsForWall( INT32 sGridNo, UINT8 ubOrientation );
void FatigueCharacter( SOLDIERTYPE *pSoldier );

#ifdef JA2UB
void HandleSeeingFortifiedDoor( UINT32 sGridNo );//Ja25 UB
#endif

#define NO_ALT_SOUND -1

EXPLOSION_DATA gExpAniData[ NUM_EXP_TYPES ] =
{
	//	Trans	Damage	Explosion			Alternative			 Explosion					Animation
	//	Key	 Key	 Sound				Explosion				Animation					Speed
	//	Frame	Frame	ID					SoundID				 Filename
	{0,	 0,		EXPLOSION_1,		EXPLOSION_ALT_BLAST_1,	"",						 0},
	{17,	3,		EXPLOSION_1,		EXPLOSION_ALT_BLAST_1,	"TILECACHE\\ZGRAV_D.STI",	80},
	{28,	5,		EXPLOSION_BLAST_2,	NO_ALT_SOUND,			"TILECACHE\\ZGRAV_C.STI",	80},
	{24,	5,		EXPLOSION_BLAST_2,	NO_ALT_SOUND,			"TILECACHE\\ZGRAV_B.STI",	80},
	{1,	 5,		EXPLOSION_1,		EXPLOSION_ALT_BLAST_1,	"TILECACHE\\shckwave.STI",	20},
	{1,	 18,	 AIR_ESCAPING_1,	 NO_ALT_SOUND,			"TILECACHE\\WAT_EXP.STI",	80},
	{1,	 18,	 AIR_ESCAPING_1,	 NO_ALT_SOUND,			"TILECACHE\\TEAR_EXP.STI",	80},
	{1,	 18,	 AIR_ESCAPING_1,	 NO_ALT_SOUND,			"TILECACHE\\TEAR_EXP.STI",	80},
	{1,	 18,	 AIR_ESCAPING_1,	 NO_ALT_SOUND,			"TILECACHE\\MUST_EXP.STI",	80}
};

//UINT8	ubTransKeyFrame[ NUM_EXP_TYPES ] =
//{
// 0,
// 17,
// 28,
// 24,
// 1,
// 1,
// 1,
// 1,
// 1,
//};
//
//UINT8	ubDamageKeyFrame[ NUM_EXP_TYPES ] =
//{
// 0,
// 3,
// 5,
// 5,
// 5,
// 18,
// 18,
// 18,
// 18,
//};
//
//
//UINT32 uiExplosionSoundID[ NUM_EXP_TYPES ] =
//{
// EXPLOSION_1,
// EXPLOSION_1,
// EXPLOSION_BLAST_2,	//LARGE
// EXPLOSION_BLAST_2,
// EXPLOSION_1,
// AIR_ESCAPING_1,
// AIR_ESCAPING_1,
// AIR_ESCAPING_1,
// AIR_ESCAPING_1,
//};
//
//
//CHAR8 zBlastFilenames[][70] =
//{
// "",
// "TILECACHE\\ZGRAV_D.STI",
// "TILECACHE\\ZGRAV_C.STI",
// "TILECACHE\\ZGRAV_B.STI",
// "TILECACHE\\shckwave.STI",
// "TILECACHE\\WAT_EXP.STI",
// "TILECACHE\\TEAR_EXP.STI",
// "TILECACHE\\TEAR_EXP.STI",
// "TILECACHE\\MUST_EXP.STI",
//};
//
//CHAR8 sBlastSpeeds[] =
//{
// 0,
// 80,
// 80,
// 80,
// 20,
// 80,
// 80,
// 80,
// 80,
//};

#define BOMB_QUEUE_DELAY (1000 + Random( 500 ) )

#define MAX_BOMB_QUEUE 40
ExplosionQueueElement	gExplosionQueue[MAX_BOMB_QUEUE];
UINT8			gubElementsOnExplosionQueue = 0;
BOOLEAN		 gfExplosionQueueActive = FALSE;

BOOLEAN		 gfExplosionQueueMayHaveChangedSight = FALSE;
UINT8			gubPersonToSetOffExplosions = NOBODY;

INT32			gsTempActionGridNo = NOWHERE;

extern UINT8 gubInterruptProvoker;

#define	NUM_EXPLOSION_SLOTS	 100

// GLOBAL FOR SMOKE LISTING
EXPLOSIONTYPE	gExplosionData[ NUM_EXPLOSION_SLOTS ];
UINT32		guiNumExplosions = 0;


INT32 GetFreeExplosion( void );
void RecountExplosions( void );
void GenerateExplosionFromExplosionPointer( EXPLOSIONTYPE *pExplosion );
void HandleBuldingDestruction( INT32 sGridNo, UINT8 ubOwner );

#ifdef JA2UB
//JA25 UB
void HavePersonAtGridnoStop( UINT32 sGridNo );
BOOLEAN ShouldThePlayerStopWhenWalkingOnBiggensActionItem( UINT8 ubRecordNum );
void HandleDestructionOfPowerGenFan();
BOOLEAN IsFanGraphicInSectorAtThisGridNo( UINT32 sGridNo );
void HandleExplosionsInTunnelSector( UINT32 sGridNo );
void HandleSwitchToOpenFortifiedDoor( UINT32 sGridNo );
void HandleSeeingPowerGenFan( UINT32 sGridNo );
#endif

// Flugente: for roof destruction
extern UINT8 gubMaterialArmour[];

// defines how much of a roofs tiles armour partially counts for neighbouring tiles - this is used in calculating the 'structural integrity' of a roof
#define WALLSTRENGTH_NEIGHBOURING_PER_TILE	0.75f

INT32 GetFreeExplosion( void )
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumExplosions; uiCount++)
	{
		if(( gExplosionData[uiCount].fAllocated==FALSE ) )
			return( (INT32)uiCount );
	}

	if( guiNumExplosions < NUM_EXPLOSION_SLOTS )
		return( (INT32) guiNumExplosions++ );

	return( -1 );
}

void RecountExplosions( void )
{
	INT32 uiCount;

	for(uiCount=guiNumExplosions-1; (uiCount >=0) ; uiCount--)
	{
		if( ( gExplosionData[uiCount].fAllocated ) )
		{
			guiNumExplosions=(UINT32)(uiCount+1);
			break;
		}
	}
}


extern void HandleLoyaltyForDemolitionOfBuilding( SOLDIERTYPE *pSoldier, INT16 sPointsDmg );

// GENERATE EXPLOSION
void InternalIgniteExplosion( UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, BOOLEAN fLocate, INT8 bLevel, UINT8 ubDirection, OBJECTTYPE * pObj )
{
#ifdef JA2BETAVERSION
	if (is_networked) {
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"InternalIgniteExplosion ( ubOwner : %i , sX : %i , sY : %i , sZ : %i , sGridNo : %i , usItem : %i , fLocate : %i , bLevel : %i , ubDirection : %i )\n",ubOwner, sX , sY , sZ , sGridNo , usItem , (int)fLocate , bLevel, ubDirection );
	MPDebugMsg(tmpMPDbgString);
	}
#endif

	EXPLOSION_PARAMS ExpParams ;

	// Callahan start
	// Double check that we are using an explosive!
	// Check if there is an explosive or an attacker

	if ( !( Item[ usItem ].usItemClass & IC_EXPLOSV ) && ubOwner == NOBODY )
	{
		return; // no explosive / no attacker
	}

	// The ammotype used. WARNING! This will only be != 0 if ubOwner != NOBODY !
	UINT16 ammotype = 0;
		
	// Okay, we either got an explosive or a real attacker to check for.
	// Let's check for the attacker first.
	if ( ubOwner != NOBODY )
	{
		OBJECTTYPE* pUsedGun = MercPtrs[ ubOwner ]->GetUsedWeapon( &MercPtrs [ ubOwner ]->inv[MercPtrs[ubOwner]->ubAttackingHand] );

		ammotype = (*pUsedGun)[0]->data.gun.ubGunAmmoType;

		if ( !( Item[ usItem ].usItemClass & IC_EXPLOSV ) && AmmoTypes[ammotype].explosionSize < 2 )
		{
			return; // no explosive and attackers gun is not fireing HE
		}

		// Flugente: campaign stats
		if ( MercPtrs[ ubOwner ]->bTeam == OUR_TEAM )
		{
			if ( Item[ usItem ].usItemClass & IC_EXPLOSV )
				gCampaignStats.AddConsumption(CAMPAIGN_CONSUMED_EXPLOSIVES, (FLOAT)(Item[usItem].ubWeight) );
		}
	}

	// Increment attack counter...
	if (gubElementsOnExplosionQueue == 0)
	{
		// single explosion, disable sight until the end, and set flag
		// to check sight at end of attack

		gTacticalStatus.uiFlags |= (DISALLOW_SIGHT | CHECK_SIGHT_AT_END_OF_ATTACK);
	}


	// gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Incrementing Attack: Explosion gone off, COunt now %d", gTacticalStatus.ubAttackBusyCount ) );


	// OK, go on!
	ExpParams.uiFlags	= EXPLOSION_FLAG_USEABSPOS;
	ExpParams.ubOwner	= ubOwner;

	// No explosive but an attacker with HE ammo.
	if ( !( Item[ usItem ].usItemClass & IC_EXPLOSV ) && ubOwner != NOBODY)
	{
		ExpParams.ubTypeID = (INT8)Explosive[Item[AmmoTypes[ammotype].highExplosive].ubClassIndex].ubAnimationID;		
		// return;
	}
	else // just normal explosives should get here
	{
		ExpParams.ubTypeID = (INT8)Explosive[ Item[ usItem ].ubClassIndex ].ubAnimationID;
	}
	// Callahan end

	// Flugente: bouncing grenades/jumping mines explode from about waist heigth, so we add 25 to sZ. However, on order for this to work on roofs, we have to add another 50
	// if on a roof, as heigth is only added if bLevel = 1 and sZ = 0
	if ( HasItemFlag(usItem, JUMP_GRENADE) )
	{
		sZ += 25;

		if ( bLevel )
			sZ += ROOF_LEVEL_HEIGHT;
	}

	ExpParams.sX	 = sX;
	ExpParams.sY	 = sY;
	ExpParams.sZ	 = sZ;
	ExpParams.sGridNo	= sGridNo;
	ExpParams.usItem	= usItem;
	ExpParams.fLocate	= fLocate;
	ExpParams.bLevel	= bLevel;

	GenerateExplosion( &ExpParams );

	// Flugente: if the explosion occured in a building, this might lower loyalty in town
	UINT16 tmp;
	if ( Item[ usItem ].usItemClass & IC_EXPLOSV && ubOwner != NOBODY && ubOwner < NUM_PROFILES && InARoom( sGridNo, &tmp ) )
	{
		HandleLoyaltyForDemolitionOfBuilding( MercPtrs[ubOwner], Explosive[ Item[ usItem ].ubClassIndex ].ubDamage );

		// Flugente: campaign stats
		gCurrentIncident.usIncidentFlags |= INCIDENT_BUILDINGS_DAMAGED;
	}

	// HEADROCK HAM 5.1: Launch fragments from the explosion.
	if (Explosive[ Item[ usItem ].ubClassIndex ].usNumFragments > 0 )
	{
		// HEADROCK HAM 5: Deactivated until the release of HAM 5.1.
		FireFragments( ubOwner, sX, sY, sZ, usItem, ubDirection );
	}

	// Flugente: Items can have secondary explosions
	HandleBuddyExplosions(ubOwner, sX, sY, sZ, sGridNo, usItem, fLocate, bLevel, ubDirection );

	// sevenfm: handle explosive items from attachments
	if( gGameExternalOptions.bAllowExplosiveAttachments )
		HandleAttachedExplosions(ubOwner, sX, sY, sZ, sGridNo, usItem, fLocate, bLevel, ubDirection, pObj );
	
	// sevenfm: add smoke effect if not in room and not underground, only for normal explosions
	if(!InARoom( sGridNo, &tmp ) && !gbWorldSectorZ && gGameExternalOptions.bAddSmokeAfterExplosion)
	{
		if( Explosive[ Item[ usItem ].ubClassIndex ].ubType == 0 )
		{
			NewSmokeEffect( sGridNo, SMALL_SMOKE, 0, NOBODY );
		}
	}
}


void IgniteExplosion( UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, INT8 bLevel, UINT8 ubDirection, OBJECTTYPE * pObj )
{
	InternalIgniteExplosion( ubOwner, sX, sY, sZ, sGridNo, usItem, TRUE, bLevel, ubDirection,  pObj );
}

void GenerateExplosion( EXPLOSION_PARAMS *pExpParams )
{
	EXPLOSIONTYPE	*pExplosion;
	UINT32	uiFlags;
	UINT8	ubOwner;
	UINT8	ubTypeID;
	INT16	sX;
	INT16	sY;
	INT16	sZ;
	INT32 sGridNo;
	UINT16	usItem;
	INT32	iIndex;
	INT8	bLevel;

	// Assign param values
	uiFlags	= pExpParams->uiFlags;
	ubOwner	= pExpParams->ubOwner;
	ubTypeID	= pExpParams->ubTypeID;
	sX		= pExpParams->sX;
	sY		= pExpParams->sY;
	sZ		= pExpParams->sZ;
	sGridNo	= pExpParams->sGridNo;
	usItem	= pExpParams->usItem;
	bLevel	= pExpParams->bLevel;


	{
		// GET AND SETUP EXPLOSION INFO IN TABLE....
		iIndex = GetFreeExplosion( );

		if ( iIndex == -1 )
		{
			return;
		}

		// OK, get pointer...
		pExplosion = &( gExplosionData[ iIndex ] );

		memset( pExplosion, 0, sizeof( EXPLOSIONTYPE ) );

		// Setup some data...
		memcpy( &(pExplosion->Params), pExpParams, sizeof( EXPLOSION_PARAMS ) );
		pExplosion->fAllocated = TRUE;
		pExplosion->iID = iIndex;

		GenerateExplosionFromExplosionPointer( pExplosion );
	}

	// ATE: Locate to explosion....
	if ( pExpParams->fLocate )
	{
		LocateGridNo( sGridNo );
	}
}


void GenerateExplosionFromExplosionPointer( EXPLOSIONTYPE *pExplosion )
{
	UINT32	uiFlags;
	UINT8	ubOwner;
	UINT8	ubTypeID;
	INT16	sX;
	INT16	sY;
	INT16	sZ;
	INT32 sGridNo;
	UINT16	usItem;
	UINT8	ubTerrainType;
	INT8	bLevel;
	UINT32	uiSoundID;

	ANITILE_PARAMS AniParams;

	// Assign param values
	uiFlags	= pExplosion->Params.uiFlags;
	ubOwner	= pExplosion->Params.ubOwner;
	ubTypeID	= pExplosion->Params.ubTypeID;
	sX		= pExplosion->Params.sX;
	sY		= pExplosion->Params.sY;
	sZ		= pExplosion->Params.sZ;
	sGridNo	= pExplosion->Params.sGridNo;
	usItem	= pExplosion->Params.usItem;
	bLevel	= pExplosion->Params.bLevel;

	// If Z value given is 0 and bLevel > 0, make z heigher
	if ( sZ == 0 && bLevel > 0 )
	{
		sZ = ROOF_LEVEL_HEIGHT;
	}

	pExplosion->iLightID = -1;

	// OK, if we are over water.... use water explosion...
	ubTerrainType = GetTerrainType( sGridNo );

	// Setup explosion!
	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );

	AniParams.sGridNo	= sGridNo;
	AniParams.ubLevelID	= ANI_TOPMOST_LEVEL;
	AniParams.sDelay	= gExpAniData[ ubTypeID ].sBlastSpeed; // Lesh: edit this line
	AniParams.sStartFrame = pExplosion->sCurrentFrame;
	AniParams.uiFlags	= ANITILE_CACHEDTILE | ANITILE_FORWARD | ANITILE_EXPLOSION;

	if ( TERRAIN_IS_WATER(ubTerrainType) )
	{
		// Change type to water explosion...
		ubTypeID = WATER_BLAST;
		AniParams.uiFlags |= ANITILE_ALWAYS_TRANSLUCENT;
	}


	if ( sZ < WALL_HEIGHT )
	{
		AniParams.uiFlags |= ANITILE_NOZBLITTER;
	}

	if ( uiFlags & EXPLOSION_FLAG_USEABSPOS )
	{
		AniParams.sX		 = sX;
		AniParams.sY		 = sY;
		AniParams.sZ		 = sZ;

		//AniParams.uiFlags		|= ANITILE_USEABSOLUTEPOS;
	}

	AniParams.ubKeyFrame1	= gExpAniData[ ubTypeID ].ubTransKeyFrame; // Lesh: edit this line
	AniParams.uiKeyFrame1Code = ANI_KEYFRAME_BEGIN_TRANSLUCENCY;

	if ( !( uiFlags & EXPLOSION_FLAG_DISPLAYONLY ) )
	{
		AniParams.ubKeyFrame2	= gExpAniData[ ubTypeID ].ubDamageKeyFrame; // Lesh: edit this line
		AniParams.uiKeyFrame2Code = ANI_KEYFRAME_BEGIN_DAMAGE;
	}
	AniParams.uiUserData = usItem;
	AniParams.ubUserData2 = ubOwner;
	AniParams.uiUserData3 = pExplosion->iID;


	strcpy( AniParams.zCachedFile, gExpAniData[ ubTypeID ].zBlastFilename ); // Lesh: edit this line

	// A little safety here, for just in case.	If it fails to create an explosion tile, don't increase the attack busy count.
	// But if it succeeds, do it here.	Don't futz with the count in other locations when it can be centralized!
	if (CreateAnimationTile( &AniParams ) )
	{
		gTacticalStatus.ubAttackBusyCount++;
		DebugAttackBusy( String( "Explosion started.	Incrementing attack busy, now %d\n", gTacticalStatus.ubAttackBusyCount ) );
	}

	//	set light source for flashbangs.... or...
	if ( pExplosion->Params.ubTypeID == FLASHBANG_EXP )
	{
		pExplosion->iLightID = LightSpriteCreate("FLSHBANG.LHT", 0 );
	}
	else
		// generic light
		// DO ONLY IF WE'RE AT A GOOD LEVEL
		if ( ubAmbientLightLevel >= MIN_AMB_LEVEL_FOR_MERC_LIGHTS )
		{
			pExplosion->iLightID = LightSpriteCreate("L-R04.LHT", 0 );
		}

		if( pExplosion->iLightID != -1 )
		{
			LightSpritePower	 ( pExplosion->iLightID, TRUE );
			LightSpriteRoofStatus( pExplosion->iLightID, pExplosion->Params.bLevel );
			LightSpritePosition	( pExplosion->iLightID, (INT16)(sX/CELL_X_SIZE), (INT16)(sY/CELL_Y_SIZE) );
		}

		// Lesh: sound randomization
		uiSoundID = gExpAniData[ ubTypeID ].uiExplosionSoundID;

		if ( gExpAniData[ ubTypeID ].uiAltExplosionSoundID != NO_ALT_SOUND )
		{
			// Randomize
			if ( Random( 2 ) == 0 )
			{
				uiSoundID = gExpAniData[ ubTypeID ].uiAltExplosionSoundID;
			}
		}
		// Lesh: sound randomization ends

		PlayJA2Sample( uiSoundID, RATE_11025, SoundVolume( HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );

}



void UpdateExplosionFrame( INT32 iIndex, INT16 sCurrentFrame )
{
	gExplosionData[ iIndex ].sCurrentFrame = sCurrentFrame;

	// Lesh: make sparkling effect
	if ( gExplosionData[iIndex].Params.ubTypeID == FLASHBANG_EXP )
	{
		if ( gExplosionData[iIndex].iLightID != -1 )
		{
			INT16 iX, iY;
			iX = (INT16) (gExplosionData[iIndex].Params.sX/CELL_X_SIZE + Random(3) - 1);
			iY = (INT16) (gExplosionData[iIndex].Params.sY/CELL_Y_SIZE + Random(3) - 1);
			LightSpritePosition( gExplosionData[iIndex].iLightID, iX, iY);
		}
	}
}

void RemoveExplosionData( INT32 iIndex )
{
	gExplosionData[ iIndex ].fAllocated = FALSE;

	if ( gExplosionData[ iIndex ].iLightID != -1 )
	{
		LightSpriteDestroy( gExplosionData[ iIndex ].iLightID );
	}

}


void HandleFencePartnerCheck( INT32 sStructGridNo )
{
	STRUCTURE *pFenceStructure, *pFenceBaseStructure;
	LEVELNODE *pFenceNode;
	INT8	bFenceDestructionPartner = -1;
	UINT32 uiFenceType;
	UINT16 usTileIndex;

	pFenceStructure = FindStructure( sStructGridNo, STRUCTURE_FENCE );

	if ( pFenceStructure )
	{
		// How does our explosion partner look?
		if ( pFenceStructure->pDBStructureRef->pDBStructure->bDestructionPartner < 0 )
		{
			// Find level node.....
			pFenceBaseStructure = FindBaseStructure( pFenceStructure );

			// Get LEVELNODE for struct and remove!
			pFenceNode = FindLevelNodeBasedOnStructure( pFenceBaseStructure->sGridNo, pFenceBaseStructure );

			// Get type from index...
			GetTileType( pFenceNode->usIndex, &uiFenceType );

			bFenceDestructionPartner = -1 * ( pFenceBaseStructure->pDBStructureRef->pDBStructure->bDestructionPartner );

			// Get new index
			GetTileIndexFromTypeSubIndex( uiFenceType, (INT8)( bFenceDestructionPartner ), &usTileIndex );

			//Set a flag indicating that the following changes are to go the the maps, temp file
			ApplyMapChangesToMapTempFile( TRUE );

			// Remove it!
			RemoveStructFromLevelNode( pFenceBaseStructure->sGridNo, pFenceNode );

			// Add it!
			AddStructToHead( pFenceBaseStructure->sGridNo, (UINT16)( usTileIndex ) );

			ApplyMapChangesToMapTempFile( FALSE );
		}
	}
}




BOOLEAN ExplosiveDamageStructureAtGridNo( STRUCTURE * pCurrent, STRUCTURE **ppNextCurrent,  INT32 sGridNo, INT16 sWoundAmt, UINT32 uiDist, BOOLEAN *pfRecompileMovementCosts, BOOLEAN fOnlyWalls, BOOLEAN fSubSequentMultiTilesTransitionDamage, UINT8 ubOwner, INT8 bLevel )
{
#ifdef JA2BETAVERSION
	if (is_networked) {
		CHAR tmpMPDbgString[512];
		sprintf(tmpMPDbgString,"ExplosiveDamageStructureAtGridNo ( sGridNo : %i , sWoundAmt : %i , uiDist : %i , fRecompMoveCosts : %i , fOnlyWalls : %i , SubsMulTilTransDmg :  %i , ubOwner : %i , bLevel : %i )\n", sGridNo, sWoundAmt , uiDist, (int)*pfRecompileMovementCosts , (int)fOnlyWalls , (int)fSubSequentMultiTilesTransitionDamage , ubOwner , bLevel );
		MPDebugMsg(tmpMPDbgString);
	}
#endif

	INT16 sX, sY;
	STRUCTURE	*pBase, *pWallStruct, *pAttached, *pAttachedBase;
	LEVELNODE *pNode = NULL, *pNewNode = NULL, *pAttachedNode;
	INT32 sNewGridNo, sStructGridNo;
	INT16 sNewIndex, sSubIndex;
	UINT16 usObjectIndex, usTileIndex;
	UINT8	ubNumberOfTiles, ubLoop;
	DB_STRUCTURE_TILE ** ppTile;
	INT8 bDestructionPartner=-1;
	INT8	bDamageReturnVal;
	BOOLEAN fContinue;
	UINT32 uiTileType;
	INT32 sBaseGridNo;
	BOOLEAN fExplosive;

	// ATE: Check for O3 statue for special damage..
	// note we do this check every time explosion goes off in game, but it's
	// an effiecnent check...
	if ( DoesO3SectorStatueExistHere( sGridNo ) && uiDist <= 1 )
	{
		ChangeO3SectorStatue( TRUE );
		return( TRUE );
	}
#ifdef JA2UB	
	//JA25 UB
	//should we replace the mine entrance graphic
	if( IsMineEntranceInSectorI13AtThisGridNo( sGridNo ) && ubOwner == NOBODY )
	{
		//Yup, replace it
		ReplaceMineEntranceGraphicWithCollapsedEntrance();
	}

	//ja25 ub
	//Handle Explosions in the tunnel sectors
	HandleExplosionsInTunnelSector( sGridNo );
#endif
	
	// Get xy
	sX = CenterX( sGridNo );
	sY = CenterY( sGridNo );

	// ATE: Continue if we are only looking for walls
	if ( fOnlyWalls && !( pCurrent->fFlags & STRUCTURE_WALLSTUFF ) )
	{
		return( TRUE );
	}

	if ( bLevel > 0 )
	{
		return( TRUE );
	}

	// Is this a corpse?
	if ( ( pCurrent->fFlags & STRUCTURE_CORPSE ) && gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] && sWoundAmt > 10 )
	{
		// Spray corpse in a fine mist....
		if ( uiDist <= 1 )
		{
			// Remove corpse...
			VaporizeCorpse( sGridNo, pCurrent->usStructureID );
		}
	}
	else if ( !( pCurrent->fFlags & STRUCTURE_PERSON ) )
	{
		// Damage structure!
		if ( ( bDamageReturnVal = DamageStructure( pCurrent, (UINT8)sWoundAmt, STRUCTURE_DAMAGE_EXPLOSION, sGridNo, sX, sY, NOBODY ) ) != 0 )
		{
			fContinue = FALSE;
#ifdef JA2UB			
			//Ja25 ub
			//are we exploding the Fan in the power gen facility
			if( IsFanGraphicInSectorAtThisGridNo( sGridNo ) )
			{
				HandleDestructionOfPowerGenFan();
			}
#endif
			pBase = FindBaseStructure( pCurrent );

			sBaseGridNo = pBase->sGridNo;

			// if the structure is openable, destroy all items there
			if ( pBase->fFlags & STRUCTURE_OPENABLE && !(pBase->fFlags & STRUCTURE_DOOR ) )
			{
				RemoveAllUnburiedItems( pBase->sGridNo, bLevel );
			}

			fExplosive = ( ( pCurrent->fFlags & STRUCTURE_EXPLOSIVE ) != 0 );

			// Get LEVELNODE for struct and remove!
			pNode = FindLevelNodeBasedOnStructure( pBase->sGridNo, pBase );

			// ATE: if we have completely destroyed a structure,
			// and this structure should have a in-between explosion partner,
			// make damage code 2 - which means only damaged - the normal explosion
			// spreading will cause it do use the proper peices..
			if ( bDamageReturnVal == 1 && pBase->pDBStructureRef->pDBStructure->bDestructionPartner < 0 )
			{
				bDamageReturnVal = 2;
			}

			if ( bDamageReturnVal == 1 )
			{
				fContinue = TRUE;
			}
			// Check for a damaged looking graphic...
			else if ( bDamageReturnVal == 2 )
			{
				if ( pBase->pDBStructureRef->pDBStructure->bDestructionPartner < 0 )
				{
					// We swap to another graphic!
					// It's -ve and 1-based, change to +ve, 1 based
					bDestructionPartner = ( -1 * pBase->pDBStructureRef->pDBStructure->bDestructionPartner );

					GetTileType( pNode->usIndex, &uiTileType );

					fContinue = 2;
				}
			}

			if ( fContinue )
			{
				// Remove the beast!
				while ( (*ppNextCurrent) != NULL && (*ppNextCurrent)->usStructureID == pCurrent->usStructureID )
				{
					// the next structure will also be deleted so we had better
					// skip past it!
					(*ppNextCurrent) = (*ppNextCurrent)->pNext;
				}

				// Replace with explosion debris if there are any....
				// ( and there already sin;t explosion debris there.... )
				if ( pBase->pDBStructureRef->pDBStructure->bDestructionPartner > 0 )
				{
					// Alrighty add!

					// Add to every gridno structure is in
					ubNumberOfTiles = pBase->pDBStructureRef->pDBStructure->ubNumberOfTiles;
					ppTile = pBase->pDBStructureRef->ppTile;

					bDestructionPartner = pBase->pDBStructureRef->pDBStructure->bDestructionPartner;

					// OK, destrcution index is , as default, the partner, until we go over the first set of explsion
					// debris...
					if ( bDestructionPartner > 39 )
					{
						GetTileIndexFromTypeSubIndex( SECONDEXPLDEBRIS, (INT8)( bDestructionPartner - 40 ), &usTileIndex );
					}
					else
					{
						GetTileIndexFromTypeSubIndex( FIRSTEXPLDEBRIS, bDestructionPartner, &usTileIndex );
					}

					// Free all the non-base tiles; the base tile is at pointer 0
					for (ubLoop = BASE_TILE; ubLoop < ubNumberOfTiles; ubLoop++)
					{
						if ( !(ppTile[ ubLoop ]->fFlags & TILE_ON_ROOF ) )
						{
#if 0//dnl ch83 080114
							sStructGridNo = pBase->sGridNo + ppTile[ubLoop]->sPosRelToBase;
#else
							sStructGridNo = AddPosRelToBase(pBase->sGridNo, ppTile[ubLoop]);
#endif
							// there might be two structures in this tile, one on each level, but we just want to
							// delete one on each pass

							if ( !TypeRangeExistsInObjectLayer( sStructGridNo, FIRSTEXPLDEBRIS, SECONDEXPLDEBRIS, &usObjectIndex ) )
							{
								//Set a flag indicating that the following changes are to go the the maps, temp file
								ApplyMapChangesToMapTempFile( TRUE );

								AddObjectToHead( sStructGridNo, (UINT16)(usTileIndex + Random( 3 ) ) );

								ApplyMapChangesToMapTempFile( FALSE );
							}
						}
					}

					// IF we are a wall, add debris for the other side
					if ( pCurrent->fFlags & STRUCTURE_WALLSTUFF )
					{
						switch( pCurrent->ubWallOrientation )
						{
						case OUTSIDE_TOP_LEFT:
						case INSIDE_TOP_LEFT:

							sStructGridNo = NewGridNo( pBase->sGridNo, DirectionInc( SOUTH ) );
							if ( !TypeRangeExistsInObjectLayer( sStructGridNo, FIRSTEXPLDEBRIS, SECONDEXPLDEBRIS, &usObjectIndex ) )
							{
								//Set a flag indicating that the following changes are to go the the maps, temp file
								ApplyMapChangesToMapTempFile( TRUE );

								AddObjectToHead( sStructGridNo, (UINT16)(usTileIndex + Random( 3 ) ) );

								ApplyMapChangesToMapTempFile( FALSE );
							}
							break;

						case OUTSIDE_TOP_RIGHT:
						case INSIDE_TOP_RIGHT:

							sStructGridNo = NewGridNo( pBase->sGridNo, DirectionInc( EAST ) );
							if ( !TypeRangeExistsInObjectLayer( sStructGridNo, FIRSTEXPLDEBRIS, SECONDEXPLDEBRIS, &usObjectIndex ) )
							{
								//Set a flag indicating that the following changes are to go the the maps, temp file
								ApplyMapChangesToMapTempFile( TRUE );

								AddObjectToHead( sStructGridNo, (UINT16)(usTileIndex + Random( 3 ) ) );

								ApplyMapChangesToMapTempFile( FALSE );
							}
							break;
						}
					}
				}
				// Else look for fences, walk along them to change to destroyed peices...
				else if ( pCurrent->fFlags & STRUCTURE_FENCE )
				{
					// walk along based on orientation
					switch( pCurrent->ubWallOrientation )
					{
					case OUTSIDE_TOP_RIGHT:
					case INSIDE_TOP_RIGHT:

						sStructGridNo	= NewGridNo( pBase->sGridNo, DirectionInc( SOUTH ) );
						HandleFencePartnerCheck( sStructGridNo );
						sStructGridNo	= NewGridNo( pBase->sGridNo, DirectionInc( NORTH ) );
						HandleFencePartnerCheck( sStructGridNo );
						break;

					case OUTSIDE_TOP_LEFT:
					case INSIDE_TOP_LEFT:

						sStructGridNo = NewGridNo( pBase->sGridNo, DirectionInc( EAST ) );
						HandleFencePartnerCheck( sStructGridNo );
						sStructGridNo = NewGridNo( pBase->sGridNo, DirectionInc( WEST ) );
						HandleFencePartnerCheck( sStructGridNo );
						break;
					}
				}

				// OK, Check if this is a wall, then search and change other walls based on this
				if ( pCurrent->fFlags & STRUCTURE_WALLSTUFF )
				{
					// ATE
					// Remove any decals in tile....
					// Use tile database for this as apposed to stuct data
					RemoveAllStructsOfTypeRange( pBase->sGridNo, FIRSTWALLDECAL, FOURTHWALLDECAL );
					RemoveAllStructsOfTypeRange( pBase->sGridNo, FIFTHWALLDECAL, EIGTHWALLDECAL );
										
					// Alrighty, now do this
					// Get orientation
					// based on orientation, go either x or y dir
					// check for wall in both _ve and -ve directions
					// if found, replace!
					switch( pCurrent->ubWallOrientation )
					{
					case OUTSIDE_TOP_LEFT:
					case INSIDE_TOP_LEFT:

						// Move WEST
						sNewGridNo = NewGridNo( pBase->sGridNo, DirectionInc( WEST ) );

							pNewNode = GetWallLevelNodeAndStructOfSameOrientationAtGridNo( sNewGridNo, pCurrent->ubWallOrientation, &pWallStruct );

						if ( pNewNode != NULL )
						{
							if ( pWallStruct->fFlags & STRUCTURE_WALL )
							{
								if ( pCurrent->ubWallOrientation == OUTSIDE_TOP_LEFT )
								{
									sSubIndex = 48;
								}
								else
								{
									sSubIndex = 52;
								}

								// Replace!
								GetTileIndexFromTypeSubIndex( gTileDatabase[ pNewNode->usIndex ].fType, sSubIndex, (UINT16 *)&sNewIndex );

								//Set a flag indicating that the following changes are to go the the maps temp file
								ApplyMapChangesToMapTempFile( TRUE );

								RemoveStructFromLevelNode( sNewGridNo, pNewNode );
								AddWallToStructLayer( sNewGridNo, sNewIndex, TRUE );

								ApplyMapChangesToMapTempFile( FALSE );

							}
						}

						// Move in EAST
						sNewGridNo = NewGridNo( pBase->sGridNo, DirectionInc( EAST ) );

							pNewNode = GetWallLevelNodeAndStructOfSameOrientationAtGridNo( sNewGridNo, pCurrent->ubWallOrientation, &pWallStruct );

						if ( pNewNode != NULL )
						{
							if ( pWallStruct->fFlags & STRUCTURE_WALL )
							{
								if ( pCurrent->ubWallOrientation == OUTSIDE_TOP_LEFT )
								{
									sSubIndex = 49;
								}
								else
								{
									sSubIndex = 53;
								}

								// Replace!
								GetTileIndexFromTypeSubIndex( gTileDatabase[ pNewNode->usIndex ].fType, sSubIndex, (UINT16 *)&sNewIndex );

								//Set a flag indicating that the following changes are to go the the maps, temp file
								ApplyMapChangesToMapTempFile( TRUE );

								RemoveStructFromLevelNode( sNewGridNo, pNewNode );
								AddWallToStructLayer( sNewGridNo, sNewIndex, TRUE );

								ApplyMapChangesToMapTempFile( FALSE );
							}
						}

						// look for attached structures in same tile
						sNewGridNo = pBase->sGridNo;
						pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_LEFT_WALL );
						while (pAttached)
						{
							pAttachedBase = FindBaseStructure( pAttached );
							if (pAttachedBase)
							{
								// Remove the beast!
								while ( (*ppNextCurrent) != NULL && (*ppNextCurrent)->usStructureID == pAttachedBase->usStructureID )
								{
									// the next structure will also be deleted so we had better
									// skip past it!
									(*ppNextCurrent) = (*ppNextCurrent)->pNext;
								}

								pAttachedNode = FindLevelNodeBasedOnStructure( pAttachedBase->sGridNo, pAttachedBase );
								if (pAttachedNode)
								{
									ApplyMapChangesToMapTempFile( TRUE );
									RemoveStructFromLevelNode( pAttachedBase->sGridNo, pAttachedNode );
									ApplyMapChangesToMapTempFile( FALSE );
								}
								else
								{
									// error!
#ifdef JA2BETAVERSION
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
									break;
								}
							}
							else
							{
								// error!
#ifdef JA2BETAVERSION
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
								break;
							}
							// search for another, from the start of the list
							pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_LEFT_WALL );
						}

						// Move in SOUTH, looking for attached structures to remove
						sNewGridNo = NewGridNo( pBase->sGridNo, DirectionInc( SOUTH ) );
						pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_LEFT_WALL );
						while (pAttached)
						{
							pAttachedBase = FindBaseStructure( pAttached );
							if (pAttachedBase)
							{
								pAttachedNode = FindLevelNodeBasedOnStructure( pAttachedBase->sGridNo, pAttachedBase );
								if (pAttachedNode)
								{
									ApplyMapChangesToMapTempFile( TRUE );
									RemoveStructFromLevelNode( pAttachedBase->sGridNo, pAttachedNode );
									ApplyMapChangesToMapTempFile( FALSE );
								}
								else
								{
									// error!
#ifdef JA2BETAVERSION
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
									break;
								}
							}
							else
							{
								// error!
#ifdef JA2BETAVERSION
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
								break;
							}
							// search for another, from the start of the list
							pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_LEFT_WALL );
						}
						break;

					case OUTSIDE_TOP_RIGHT:
					case INSIDE_TOP_RIGHT:

						// Move in NORTH
						sNewGridNo = NewGridNo( pBase->sGridNo, DirectionInc( NORTH ) );

						pNewNode = GetWallLevelNodeAndStructOfSameOrientationAtGridNo( sNewGridNo, pCurrent->ubWallOrientation, &pWallStruct );

						if ( pNewNode != NULL )
						{
							if ( pWallStruct->fFlags & STRUCTURE_WALL )
							{
								if ( pCurrent->ubWallOrientation == OUTSIDE_TOP_RIGHT )
								{
									sSubIndex = 51;
								}
								else
								{
									sSubIndex = 55;
								}

								// Replace!
								GetTileIndexFromTypeSubIndex( gTileDatabase[ pNewNode->usIndex ].fType, sSubIndex, (UINT16 *)&sNewIndex );

								//Set a flag indicating that the following changes are to go the the maps, temp file
								ApplyMapChangesToMapTempFile( TRUE );

								RemoveStructFromLevelNode( sNewGridNo, pNewNode );
								AddWallToStructLayer( sNewGridNo, sNewIndex, TRUE );

								ApplyMapChangesToMapTempFile( FALSE );
							}
						}

						// Move in SOUTH
						sNewGridNo = NewGridNo( pBase->sGridNo, DirectionInc( SOUTH ) );

							pNewNode = GetWallLevelNodeAndStructOfSameOrientationAtGridNo( sNewGridNo, pCurrent->ubWallOrientation, &pWallStruct );

						if ( pNewNode != NULL )
						{
							if ( pWallStruct->fFlags & STRUCTURE_WALL )
							{
								if ( pCurrent->ubWallOrientation == OUTSIDE_TOP_RIGHT )
								{
									sSubIndex = 50;
								}
								else
								{
									sSubIndex = 54;
								}

								// Replace!
								GetTileIndexFromTypeSubIndex( gTileDatabase[ pNewNode->usIndex ].fType, sSubIndex, (UINT16 *)&sNewIndex );

								//Set a flag indicating that the following changes are to go the the maps, temp file
								ApplyMapChangesToMapTempFile( TRUE );

								RemoveStructFromLevelNode( sNewGridNo, pNewNode );
								AddWallToStructLayer( sNewGridNo, sNewIndex, TRUE );

								ApplyMapChangesToMapTempFile( FALSE );
							}
						}

						// looking for attached structures to remove in base tile
						sNewGridNo = pBase->sGridNo;
						pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_RIGHT_WALL );
						while (pAttached)
						{
							pAttachedBase = FindBaseStructure( pAttached );
							if (pAttachedBase)
							{
								pAttachedNode = FindLevelNodeBasedOnStructure( pAttachedBase->sGridNo, pAttachedBase );
								if (pAttachedNode)
								{
									ApplyMapChangesToMapTempFile( TRUE );
									RemoveStructFromLevelNode( pAttachedBase->sGridNo, pAttachedNode );
									ApplyMapChangesToMapTempFile( FALSE );
								}
								else
								{
									// error!
#ifdef JA2BETAVERSION
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
									break;
								}
							}
							else
							{
								// error!
#ifdef JA2BETAVERSION
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
								break;
							}
							// search for another, from the start of the list
							pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_RIGHT_WALL );
						}

						// Move in EAST, looking for attached structures to remove
						sNewGridNo = NewGridNo( pBase->sGridNo, DirectionInc( EAST ) );
						pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_RIGHT_WALL );
						while (pAttached)
						{
							pAttachedBase = FindBaseStructure( pAttached );
							if (pAttachedBase)
							{
								pAttachedNode = FindLevelNodeBasedOnStructure( pAttachedBase->sGridNo, pAttachedBase );
								if (pAttachedNode)
								{
									ApplyMapChangesToMapTempFile( TRUE );
									RemoveStructFromLevelNode( pAttachedBase->sGridNo, pAttachedNode );
									ApplyMapChangesToMapTempFile( FALSE );
								}
								else
								{
									// error!
#ifdef JA2BETAVERSION
									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
									break;
								}
							}
							else
							{
								// error!
#ifdef JA2BETAVERSION
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Problems removing structure attached to wall at %d", sNewGridNo );
#endif
								break;
							}
							// search for another, from the start of the list
							pAttached = FindStructure( sNewGridNo, STRUCTURE_ON_RIGHT_WALL );
						}

						break;
					}

					// CJC, Sept 16: if we destroy any wall of the brothel, make Kingpin's men hostile!
					if ( gWorldSectorX == 5 && gWorldSectorY == MAP_ROW_C && gbWorldSectorZ == 0 )
					{
						//DBrot: More Rooms
						//UINT8	ubRoom;
						UINT16 usRoom;
						BOOLEAN	fInRoom;

						fInRoom = InARoom( sGridNo, &usRoom );
						if ( !fInRoom )
						{
							// try to south
							fInRoom = InARoom( sGridNo + DirectionInc( SOUTH ) , &usRoom );
							if ( !fInRoom )
							{
								// try to east
								fInRoom = InARoom( sGridNo + DirectionInc( EAST ) , &usRoom );
							}
						}

						if ( fInRoom && IN_BROTHEL( usRoom ) )
						{
							CivilianGroupChangesSides( KINGPIN_CIV_GROUP );
						}
					}

				}

				// OK, we need to remove the water from the fountain
				// Lots of HARD CODING HERE :(
				// Get tile type
				GetTileType( pNode->usIndex, &uiTileType );
				// Check if we are a fountain!
				if ( _stricmp( gTilesets[ giCurrentTilesetID ].TileSurfaceFilenames[ uiTileType ], "fount1.sti" ) == 0 )
				{
					// Yes we are!
					// Remove water....
					ApplyMapChangesToMapTempFile( TRUE );
					GetTileIndexFromTypeSubIndex( uiTileType, 1, (UINT16 *)&sNewIndex );
					RemoveStruct( sBaseGridNo, sNewIndex );
					RemoveStruct( sBaseGridNo, sNewIndex );
					GetTileIndexFromTypeSubIndex( uiTileType, 2, (UINT16 *)&sNewIndex );
					RemoveStruct( sBaseGridNo, sNewIndex );
					RemoveStruct( sBaseGridNo, sNewIndex );
					GetTileIndexFromTypeSubIndex( uiTileType, 3, (UINT16 *)&sNewIndex );
					RemoveStruct( sBaseGridNo, sNewIndex );
					RemoveStruct( sBaseGridNo, sNewIndex );
					ApplyMapChangesToMapTempFile( FALSE );
				}


				// Remove any interactive tiles we could be over!
				BeginCurInteractiveTileCheck( INTILE_CHECK_SELECTIVE );

				if ( pCurrent->fFlags & STRUCTURE_WALLSTUFF )
				{
					RecompileLocalMovementCostsForWall( pBase->sGridNo, pBase->ubWallOrientation );
				}

				// Remove!
				//Set a flag indicating that the following changes are to go the the maps, temp file
				ApplyMapChangesToMapTempFile( TRUE );
				RemoveStructFromLevelNode( pBase->sGridNo, pNode );
				ApplyMapChangesToMapTempFile( FALSE );

				// OK, if we are to swap structures, do it now...
				if ( fContinue == 2 )
				{
					// We have a levelnode...
					// Get new index for new grpahic....
					GetTileIndexFromTypeSubIndex( uiTileType, bDestructionPartner, &usTileIndex );

					ApplyMapChangesToMapTempFile( TRUE );

					AddStructToHead( sBaseGridNo, usTileIndex );

					ApplyMapChangesToMapTempFile( FALSE );


				}

				// Rerender world!
				// Reevaluate world movement costs, reduncency!
				gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;
				// FOR THE NEXT RENDER LOOP, RE-EVALUATE REDUNDENT TILES
				InvalidateWorldRedundency( );
				SetRenderFlags(RENDER_FLAG_FULL);
				// Handle properly lighting the structure that was modified
				if( gbWorldSectorZ == 0 )
				{
					// ATE: make sure the lighting on the graphic gets updateds
					LightSetBaseLevel( GetTimeOfDayAmbientLightLevel() );
				}
				else
				{
					LightSetBaseLevel( LightGetAmbient() );
				}
				// Movement costs!
				( *pfRecompileMovementCosts ) = TRUE;

				{
					// Make secondary explosion if eplosive....
					if ( fExplosive )
					{
						InternalIgniteExplosion( ubOwner, CenterX( sBaseGridNo ), CenterY( sBaseGridNo ), 0, sBaseGridNo, STRUCTURE_EXPLOSION, FALSE, bLevel );
					}
				}

				if ( fContinue == 2 )
				{
					return( FALSE );
				}
			}

			// 2 is NO DAMAGE
			return( 2 );
		}
	}

	return( 1 );

}

STRUCTURE *gStruct;

void ExplosiveDamageGridNo( INT32 sGridNo, INT16 sWoundAmt, UINT32 uiDist, BOOLEAN *pfRecompileMovementCosts, BOOLEAN fOnlyWalls, INT8 bMultiStructSpecialFlag, BOOLEAN fSubSequentMultiTilesTransitionDamage, UINT8 ubOwner, INT8 bLevel )
{
#ifdef JA2BETAVERSION
	if (is_networked) {
		CHAR tmpMPDbgString[512];
		sprintf(tmpMPDbgString,"ExplosiveDamageGridNo ( sGridNo : %i , sWoundAmt : %i , uiDist : %i , fRecompileMoveCosts : %i , fOnlyWalls : %i , MultiStructSpecialFlag : %i ,fSubsequentMultiTilesTransDmg : %i , ubOwner : %i , bLevel : %i )\n", sGridNo, sWoundAmt ,uiDist, (int)*pfRecompileMovementCosts , (int)fOnlyWalls , bMultiStructSpecialFlag , (int)fSubSequentMultiTilesTransitionDamage , ubOwner , bLevel );
		MPDebugMsg(tmpMPDbgString);
	}
#endif

	STRUCTURE			*pCurrent, *pNextCurrent, *pStructure;
	STRUCTURE 			*pBaseStructure;
	INT16				sDesiredLevel;
	DB_STRUCTURE_TILE	**ppTile = NULL;
	UINT8				ubLoop, ubLoop2;
	INT32				sNewGridNo, sNewGridNo2, sBaseGridNo = NOWHERE;
	BOOLEAN				fToBreak = FALSE;
	BOOLEAN				fMultiStructure = FALSE;
	UINT8				ubNumberOfTiles = 0xff;
	BOOLEAN				fMultiStructSpecialFlag = FALSE;
	BOOLEAN				fExplodeDamageReturn = FALSE;

	// Based on distance away, damage any struct at this gridno
	// OK, loop through structures and damage!
	pCurrent	= gpWorldLevelData[ sGridNo ].pStructureHead;
	sDesiredLevel	= STRUCTURE_ON_GROUND;

	// This code gets a little hairy because
	// (1) we might need to destroy the currently-examined structure
	while (pCurrent != NULL)
	{
		// ATE: These are for the checks below for multi-structs....
		pBaseStructure = FindBaseStructure( pCurrent );

		if ( pBaseStructure )
		{
			sBaseGridNo = pBaseStructure->sGridNo;
			ubNumberOfTiles = pBaseStructure->pDBStructureRef->pDBStructure->ubNumberOfTiles;
			fMultiStructure = ( ( pBaseStructure->fFlags & STRUCTURE_MULTI ) != 0 );
			ppTile = (DB_STRUCTURE_TILE **) MemAlloc( sizeof( DB_STRUCTURE_TILE* ) * ubNumberOfTiles );

			memcpy( ppTile, pBaseStructure->pDBStructureRef->ppTile, sizeof( DB_STRUCTURE_TILE* ) * ubNumberOfTiles );

			if ( bMultiStructSpecialFlag == -1 )
			{
				// Set it!
				bMultiStructSpecialFlag = ( ( pBaseStructure->fFlags & STRUCTURE_SPECIAL ) != 0 );
			}

			if ( pBaseStructure->fFlags & STRUCTURE_EXPLOSIVE )
			{
				// ATE: Set hit points to zero....
				pBaseStructure->ubHitPoints = 0;
			}
		}
		else
		{
			fMultiStructure = FALSE;
			sBaseGridNo = 0;
			ubNumberOfTiles = 0;
			ppTile = 0;
		}

		pNextCurrent = pCurrent->pNext;
		gStruct = pNextCurrent;

		// Check level!
		if (pCurrent->sCubeOffset == sDesiredLevel )
		{
			fExplodeDamageReturn = ExplosiveDamageStructureAtGridNo( pCurrent, &pNextCurrent,	sGridNo, sWoundAmt, uiDist, pfRecompileMovementCosts, fOnlyWalls, 0, ubOwner, bLevel );

			// Are we overwriting damage due to multi-tile...?
			if ( fExplodeDamageReturn )
			{
				if ( fSubSequentMultiTilesTransitionDamage == 2)
				{
					fExplodeDamageReturn = 2;
				}
				else
				{
					fExplodeDamageReturn = 1;
				}
			}

			if ( !fExplodeDamageReturn )
			{
				fToBreak = TRUE;
			}
			//}

			// 0verhaul:  The following was combined with the previous code block.  I don't think they intended to execute this
			// part unless fExplodeDamageReturn was actually set.  When it was being executed, tossing a grenade just behind the
			// plane in Drassen, for instance, would cause an infinite recursion in this code.  The reason is that the plane's
			// armor is (amazingly enough) stronger than a grenade blast can even damage.  This code here seems to rely on the
			// structure in question being destroyed by the blast since it indiscriminently recurses on neighbors, creating a
			// ping pong on two adjacent parts of the plane.  Probably the reason this was not found before is that fExplodeDamageReturn
			// was uninitialized before and usually was non-zero.  Now it is initialized to false.

			// OK, for multi-structs...
			// AND we took damage...
			if ( fMultiStructure && !fOnlyWalls && fExplodeDamageReturn == 0 )
			{
				// ATE: Don't after first attack...
				if ( uiDist > 1 )
				{
					if ( pBaseStructure )
					{
						MemFree( ppTile );
					}
					return;
				}

				{

					for ( ubLoop = BASE_TILE; ubLoop < ubNumberOfTiles; ubLoop++)
					{
#if 0//dnl ch83 080114
						sNewGridNo = sBaseGridNo + ppTile[ubLoop]->sPosRelToBase;
#else
						sNewGridNo = AddPosRelToBase(sBaseGridNo, ppTile[ubLoop]);
#endif
						// look in adjacent tiles
						for ( ubLoop2 = 0; ubLoop2 < NUM_WORLD_DIRECTIONS; ubLoop2++ )
						{
							sNewGridNo2 = NewGridNo( sNewGridNo, DirectionInc( ubLoop2 ) );
							if ( sNewGridNo2 != sNewGridNo && sNewGridNo2 != sGridNo )
							{
								pStructure = FindStructure( sNewGridNo2, STRUCTURE_MULTI );
								if ( pStructure )
								{
									fMultiStructSpecialFlag = ( ( pStructure->fFlags & STRUCTURE_SPECIAL ) != 0 );

									if ( ( bMultiStructSpecialFlag == fMultiStructSpecialFlag ) )
									{
										// If we just damaged it, use same damage value....
										if ( fMultiStructSpecialFlag )
										{
											ExplosiveDamageGridNo( sNewGridNo2, sWoundAmt, uiDist, pfRecompileMovementCosts, fOnlyWalls, bMultiStructSpecialFlag, 1, ubOwner, bLevel );
										}
										else
										{
											ExplosiveDamageGridNo( sNewGridNo2, sWoundAmt, uiDist, pfRecompileMovementCosts, fOnlyWalls, bMultiStructSpecialFlag, 2, ubOwner, bLevel );
										}

										{
											InternalIgniteExplosion( ubOwner, CenterX( sNewGridNo2 ), CenterY( sNewGridNo2 ), 0, sNewGridNo2, RDX, FALSE, bLevel );
										}

										fToBreak = TRUE;
									}
								}
							}
						}
					}
				}
				if ( fToBreak )
				{
					break;
				}
			}
		}

		if ( pBaseStructure )
		{
			MemFree( ppTile );
		}

		pCurrent = pNextCurrent;
	}

}


BOOLEAN DamageSoldierFromBlast( UINT8 ubPerson, UINT8 ubOwner, INT32 sBombGridNo, INT16 sWoundAmt, INT16 sBreathAmt, UINT32 uiDist, UINT16 usItem, INT16 sSubsequent , BOOL fFromRemoteClient )
{
	// OJW - 20091028
	if (is_networked && is_client)
	{
		SOLDIERTYPE* pSoldier = MercPtrs[ubPerson];
		if (pSoldier != NULL)
		{
			// only the owner of a merc may send damage (as this takes into account equipped armor)
			if (IsOurSoldier(pSoldier) || (pSoldier->bTeam == 1 && is_server) && !fFromRemoteClient)
			{
				// let this function proceed, we will send damage towards the end
			}
			else if (!fFromRemoteClient)
			{
				// skip executing locally because we want the random number generator to be aligned
				// with the client that spawns set off the explosion/grenade/whatever
				return FALSE;
			}
		}
#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"DamageSoldierFromBlast ( ubPerson : %i , ubOwner : %i , sBombGridNo : %i , sWoundAmt : %i , sBreathAmt : %i , uiDist : %i , usItem : %i , sSubs : %i , fFromRemoteClient : %i )\n",ubPerson, ubOwner , sBombGridNo , sWoundAmt , sBreathAmt , uiDist , usItem , sSubsequent , fFromRemoteClient );
	MPDebugMsg(tmpMPDbgString);
#endif
	}


	SOLDIERTYPE *pSoldier;
	INT16 sNewWoundAmt = 0;
	UINT8 ubDirection;
	UINT8 ubSpecial = 0;
	BOOLEAN fInBuilding = InBuilding(sBombGridNo);
	BOOLEAN fFlashbang = Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_FLASHBANG;
	UINT16 usHalfExplosionRadius;

	pSoldier = MercPtrs[ ubPerson ];	// someone is here, and they're gonna get hurt

	if (!pSoldier->bActive || !pSoldier->bInSector || !pSoldier->stats.bLife )
		return( FALSE );

	if ( pSoldier->ubMiscSoldierFlags & SOLDIER_MISC_HURT_BY_EXPLOSION )
	{
		// don't want to damage the guy twice
		return( FALSE );
	}

	// Lesh: if flashbang
	// check if soldier is outdoor and situated farther that half explosion radius and not underground
	usHalfExplosionRadius = Explosive[Item[usItem].ubClassIndex].ubRadius / 2;
	if ( fFlashbang && !gbWorldSectorZ && !fInBuilding && uiDist > usHalfExplosionRadius )
	{
		// HEADROCK HAM 3.3: Flashbang at half distance causes up to 6 suppression points. Roughly equivalent of being
		// "lightly" shot at.
		if (gGameExternalOptions.usExplosionSuppressionEffect > 0)
		{
			pSoldier->ubSuppressionPoints += (PreRandom(6) * gGameExternalOptions.usExplosionSuppressionEffect) / 100;
		}
		// then no effect
		return( FALSE );
	}

	// Direction to center of explosion
	ubDirection = (UINT8)GetDirectionFromGridNo( sBombGridNo, pSoldier );

	// Increment attack counter...
	//	gTacticalStatus.ubAttackBusyCount++;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Incrementing Attack: Explosion dishing out damage, Count now %d", gTacticalStatus.ubAttackBusyCount ) );
	DebugAttackBusy( String("Explosion dishing out damage to %d\n", pSoldier->ubID) );

	sNewWoundAmt = sWoundAmt - __min( sWoundAmt, 35 ) * ArmourVersusExplosivesPercent( pSoldier ) / 100;
	if ( sNewWoundAmt < 0 )
	{
		sNewWoundAmt = 0;
	}
	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - STOMP traits
	else
	{
		if ( ubOwner < TOTAL_SOLDIERS && (MercPtrs[ ubOwner ] != NULL) && gGameOptions.fNewTraitSystem)
		{
			// Demolitions damage bonus with bombs and mines
			if ( HAS_SKILL_TRAIT( MercPtrs[ ubOwner ], DEMOLITIONS_NT ) &&
				Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_NORMAL && Item[usItem].usItemClass == IC_BOMB &&
				(!Item[usItem].attachment || Item[usItem].mine ))
			{
				sNewWoundAmt = (INT16)(((sNewWoundAmt * (100 + gSkillTraitValues.ubDEDamageOfBombsAndMines)) / 100) + 0.5);
			}
			// Heavy Weapons trait bonus damage to tanks
			if ( HAS_SKILL_TRAIT( MercPtrs[ ubOwner ], HEAVY_WEAPONS_NT ) && TANK( pSoldier ) &&
				Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_NORMAL )
			{
				sNewWoundAmt = (INT16)(((sNewWoundAmt * (100 + gSkillTraitValues.ubHWDamageTanksBonusPercent * NUM_SKILL_TRAITS( MercPtrs[ ubOwner ], HEAVY_WEAPONS_NT ))) / 100) + 0.5); // +30%
			}
			// Heavy Weapons trait bonus damage with rocket, grenade launchers and mortar
			else if ( HAS_SKILL_TRAIT( MercPtrs[ ubOwner ], HEAVY_WEAPONS_NT ) &&
				Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_NORMAL && 
				((Item[usItem].usItemClass == IC_BOMB && Item[usItem].attachment &&	!Item[usItem].mine ) || // mortar shells
				(Item[usItem].usItemClass == IC_GRENADE && (Item[usItem].glgrenade || Item[usItem].electronic) ) || // rockets for rocketlaunchers (I haven't found any other way)
				(Item[usItem].usItemClass == IC_LAUNCHER ) || Item[usItem].rocketlauncher || Item[usItem].singleshotrocketlauncher ) )
			{
				sNewWoundAmt = (INT16)(((sNewWoundAmt * (100 + gSkillTraitValues.ubHWDamageBonusPercentForHW * NUM_SKILL_TRAITS( MercPtrs[ ubOwner ], HEAVY_WEAPONS_NT ))) / 100) + 0.5); // +15%
			}
		}
		// adjust damage resistance of TANKS
		if ( TANK( pSoldier ) && gGameOptions.fNewTraitSystem )
		{
			sNewWoundAmt = (INT16)(sNewWoundAmt * (100 - gSkillTraitValues.bTanksDamageResistanceModifier) / 100);
			// another half of this for ordinary grenades
			if ( (( Item[usItem].usItemClass == IC_GRENADE ) || Item[usItem].glgrenade ) && !Item[usItem].electronic )
				sNewWoundAmt = (INT16)(sNewWoundAmt * (100 - (gSkillTraitValues.bTanksDamageResistanceModifier / 2)) / 100);
		}
		
		// Bodybuilding damage resistance
		if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, BODYBUILDING_NT ) )
			sNewWoundAmt = max( 1, (INT16)(sNewWoundAmt * (100 - gSkillTraitValues.ubBBDamageResistance) / 100)); 
				
		// Flugente: moved the damage calculation into a separate function
#ifdef ENABLE_ZOMBIES
		if ( pSoldier->GetDamageResistance(FALSE, FALSE) > 0 )
			sNewWoundAmt -= ((sNewWoundAmt * pSoldier->GetDamageResistance(FALSE, FALSE)) /100);
#else
		// Damage resistance for Militia
		if (pSoldier->ubSoldierClass == SOLDIER_CLASS_GREEN_MILITIA && gGameExternalOptions.bGreenMilitiaDamageResistance != 0)
			sNewWoundAmt -= ((sNewWoundAmt * gGameExternalOptions.bGreenMilitiaDamageResistance) /100);
		else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_REG_MILITIA && gGameExternalOptions.bRegularMilitiaDamageResistance != 0)
			sNewWoundAmt -= ((sNewWoundAmt * gGameExternalOptions.bRegularMilitiaDamageResistance) /100);
		else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA && gGameExternalOptions.bVeteranMilitiaDamageResistance != 0)
			sNewWoundAmt -= ((sNewWoundAmt * gGameExternalOptions.bVeteranMilitiaDamageResistance) /100);
		// bonus for enemy too
		else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR && gGameExternalOptions.sEnemyAdminDamageResistance != 0)
			sNewWoundAmt -= ((sNewWoundAmt * gGameExternalOptions.sEnemyAdminDamageResistance) /100);
		else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY && gGameExternalOptions.sEnemyRegularDamageResistance != 0)
			sNewWoundAmt -= ((sNewWoundAmt * gGameExternalOptions.sEnemyRegularDamageResistance) /100);
		else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE && gGameExternalOptions.sEnemyEliteDamageResistance != 0)
			sNewWoundAmt -= ((sNewWoundAmt * gGameExternalOptions.sEnemyEliteDamageResistance) /100);
#endif

		// we can loose stats due to being hit by the blast
		else if ( gGameOptions.fNewTraitSystem && Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_NORMAL && 
			!AM_A_ROBOT( pSoldier ) && !(pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER) && !TANK(pSoldier) &&
			sNewWoundAmt > 2 && sNewWoundAmt < pSoldier->stats.bLife )
		{
			if ( PreRandom( sNewWoundAmt ) > gSkillTraitValues.ubDamageNeededToLoseStats )
			{
				UINT8 ubStatLoss = PreRandom( sNewWoundAmt ) + 1;
				UINT8 ubPickStat = PreRandom( 20 );
				if (ubPickStat < 3 ) // 15% chance to lose Wisdom
				{
					if (ubStatLoss >= pSoldier->stats.bWisdom)
					{
						ubStatLoss = pSoldier->stats.bWisdom - 1;
					}
					if ( ubStatLoss > 0 )
					{
						pSoldier->stats.bWisdom -= ubStatLoss;
						pSoldier->ubCriticalStatDamage[DAMAGED_STAT_WISDOM] += ubStatLoss;

						if (pSoldier->ubProfile != NO_PROFILE)
						{
							gMercProfiles[ pSoldier->ubProfile ].bWisdom = pSoldier->stats.bWisdom;
						}

						if (pSoldier->name[0] && pSoldier->bVisible == TRUE)
						{
							// make stat RED for a while...
							pSoldier->timeChanges.uiChangeWisdomTime = GetJA2Clock();
							pSoldier->usValueGoneUp &= ~( WIS_INCREASE );

							if (ubStatLoss == 1)
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_WISDOM], pSoldier->GetName() );
							}
							else
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_WISDOM], pSoldier->GetName(), ubStatLoss );
							}
						}
					}
				}
				else if (ubPickStat < 7 ) // 20% chance to lose Dexterity
				{
					if (ubStatLoss >= pSoldier->stats.bDexterity)
					{
						ubStatLoss = pSoldier->stats.bDexterity - 1;
					}
					if ( ubStatLoss > 0 )
					{
						pSoldier->stats.bDexterity -= ubStatLoss;
						pSoldier->ubCriticalStatDamage[DAMAGED_STAT_DEXTERITY] += ubStatLoss;

						if (pSoldier->ubProfile != NO_PROFILE)
						{
							gMercProfiles[ pSoldier->ubProfile ].bDexterity = pSoldier->stats.bDexterity;
						}

						if (pSoldier->name[0] && pSoldier->bVisible == TRUE)
						{
							// make stat RED for a while...
							pSoldier->timeChanges.uiChangeDexterityTime = GetJA2Clock();
							pSoldier->usValueGoneUp &= ~( DEX_INCREASE );

							if (ubStatLoss == 1)
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_DEX], pSoldier->GetName() );
							}
							else
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_DEX], pSoldier->GetName(), ubStatLoss );
							}
						}
					}
				}
				else if (ubPickStat < 11 ) // 20% chance to lose Strength
				{
					if (ubStatLoss >= pSoldier->stats.bStrength)
					{
						ubStatLoss = pSoldier->stats.bStrength - 1;
					}
					if ( ubStatLoss > 0 )
					{
						pSoldier->stats.bStrength -= ubStatLoss;
						// added this for healing lost stats feature
						pSoldier->ubCriticalStatDamage[DAMAGED_STAT_STRENGTH] += ubStatLoss;

						if (pSoldier->ubProfile != NO_PROFILE)
						{
							gMercProfiles[ pSoldier->ubProfile ].bStrength = pSoldier->stats.bStrength;
						}

						if (pSoldier->name[0] && pSoldier->bVisible == TRUE)
						{
							// make stat RED for a while...
							pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
							pSoldier->usValueGoneUp &= ~( STRENGTH_INCREASE );

							if (ubStatLoss == 1)
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_STRENGTH], pSoldier->GetName() );
							}
							else
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_STRENGTH], pSoldier->GetName(), ubStatLoss );
							}
						}
					}
				}
				else if (ubPickStat < 15 ) // 20% chance to lose Agility
				{
					if (ubStatLoss >= pSoldier->stats.bAgility)
					{
						ubStatLoss = pSoldier->stats.bAgility - 1;
					}
					if ( ubStatLoss > 0 )
					{
						pSoldier->stats.bAgility -= ubStatLoss;
						// added this for healing lost stats feature
						pSoldier->ubCriticalStatDamage[DAMAGED_STAT_AGILITY] += ubStatLoss;

						if (pSoldier->ubProfile != NO_PROFILE)
						{
							gMercProfiles[ pSoldier->ubProfile ].bAgility = pSoldier->stats.bAgility;
						}

						if (pSoldier->name[0] && pSoldier->bVisible == TRUE)
						{
							// make stat RED for a while...
							pSoldier->timeChanges.uiChangeAgilityTime = GetJA2Clock();
							pSoldier->usValueGoneUp &= ~( AGIL_INCREASE );

							if (ubStatLoss == 1)
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_1_AGIL], pSoldier->GetName() );
							}
							else
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, Message[STR_LOSES_AGIL], pSoldier->GetName(), ubStatLoss );
							}
						}
					}
				}
				else if (ubPickStat < 18 ) // 15% chance to lose Health
				{
					if (ubStatLoss >= (pSoldier->stats.bLifeMax - OKLIFE))
					{
						ubStatLoss = pSoldier->stats.bLifeMax - OKLIFE - 1;
					}
					if ( ubStatLoss > sNewWoundAmt)
					{
						ubStatLoss = (UINT8)sNewWoundAmt;
					}
					if ( ubStatLoss > 0 )
					{
						pSoldier->stats.bLifeMax -= ubStatLoss;
						pSoldier->ubCriticalStatDamage[DAMAGED_STAT_HEALTH] += ubStatLoss;

						if (pSoldier->ubProfile != NO_PROFILE)
						{
							gMercProfiles[ pSoldier->ubProfile ].bLifeMax = pSoldier->stats.bLifeMax;
						}

						if (pSoldier->name[0] && pSoldier->bVisible == TRUE)
						{
							// make stat RED for a while...
							pSoldier->timeChanges.uiChangeDexterityTime = GetJA2Clock();
							pSoldier->usValueGoneUp &= ~( HEALTH_INCREASE );

							if (ubStatLoss == 1)
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_LOSES_ONE_POINT_MAX_HEALTH], pSoldier->GetName() );
							}
							else
							{
								ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_LOSES_X_POINTS_MAX_HEALTH], pSoldier->GetName(), ubStatLoss );
							}
						}
					}
				}
				else // 10% chance to be blinded
				{
					if (pSoldier->bBlindedCounter < ubStatLoss )
					{
						pSoldier->bBlindedCounter = ubStatLoss ;
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_BLINDED_BY_BLAST], pSoldier->GetName() );
					}
				}
				// SANDRO - new merc records - times stat damaged
				if ( ubStatLoss > 0 && pSoldier->ubProfile != NO_PROFILE )
					gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;
			}
		}

		sNewWoundAmt = max(1, sNewWoundAmt);
	}
	//////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_ZOMBIES
	// Nothing to do here
#else
	////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - option to make special NPCs stronger - damage resistance
	if (gGameExternalOptions.usSpecialNPCStronger > 0)
	{
		switch( pSoldier->ubProfile )
		{
			case CARMEN:
			case QUEEN:
			case JOE:
			case ANNIE:
			case CHRIS:
			case KINGPIN:
			case TIFFANY:
			case T_REX:
			case DRUGGIST:
			case GENERAL:
			case JIM:
			case JACK:
			case OLAF:
			case RAY:
			case OLGA:
			case TYRONE:
			case MIKE:
				sNewWoundAmt -= (sNewWoundAmt * gGameExternalOptions.usSpecialNPCStronger / 200);
				sNewWoundAmt = max( 1, sNewWoundAmt);
				break;
		}
	}
#endif
		
	// SANDRO - new merc records - times wounded (blasted by explosion)
	if ( ( sNewWoundAmt > 1 || sBreathAmt > 1000 ) && pSoldier->ubProfile != NO_PROFILE )
		gMercProfiles[ pSoldier->ubProfile ].records.usTimesWoundedBlasted++;
	////////////////////////////////////////////////////////////////////////////////////
	
	// Lesh: flashbang does affect on soldier or not - check it
	if ( (Item[usItem].usItemClass & IC_EXPLOSV) && fFlashbang )
	{
		ubSpecial = DetermineFlashbangEffect( pSoldier, ubDirection, fInBuilding);
	}

// HEADROCK HAM 3.3: Explosions cause suppression based on distance.
	if (gGameExternalOptions.usExplosionSuppressionEffect > 0)
	{
		pSoldier->ubSuppressionPoints += ((__max(0,((Explosive[Item[usItem].ubClassIndex].ubRadius * 3) - uiDist)))* gGameExternalOptions.usExplosionSuppressionEffect) / 100;
		if (fFlashbang && (gbWorldSectorZ || fInBuilding) && (UINT16)uiDist <= usHalfExplosionRadius)
		{
			pSoldier->ubSuppressionPoints += (15 * gGameExternalOptions.usExplosionSuppressionEffect) / 100;
		}
	}


	if (is_networked && is_client)
	{
		if (IsOurSoldier(pSoldier) || (pSoldier->bTeam == 1 && is_server) && !fFromRemoteClient)
		{
			// if it gets here then we can let the other clients know our merc took damage
			send_explosivedamage( ubPerson , ubOwner , sBombGridNo , sNewWoundAmt , sBreathAmt , uiDist , usItem , sSubsequent );
		}
	}
	
	// OJW - 20091028 - If from a remote client, use unadjusted damage amount
	pSoldier->EVENT_SoldierGotHit( usItem, (fFromRemoteClient ? sWoundAmt : sNewWoundAmt) , sBreathAmt, ubDirection, (INT16)uiDist, ubOwner, ubSpecial, ANIM_CROUCH, sSubsequent, sBombGridNo );
	
	pSoldier->ubMiscSoldierFlags |= SOLDIER_MISC_HURT_BY_EXPLOSION;

	if ( ubOwner != NOBODY && MercPtrs[ ubOwner ]->bTeam == gbPlayerNum && pSoldier->bTeam != gbPlayerNum )
	{
		ProcessImplicationsOfPCAttack( MercPtrs[ ubOwner ], &pSoldier, REASON_EXPLOSION );
	}

	return( TRUE );
}

BOOLEAN DishOutGasDamage( SOLDIERTYPE * pSoldier, EXPLOSIVETYPE * pExplosive, INT16 sSubsequent, BOOLEAN fRecompileMovementCosts, INT16 sWoundAmt, INT16 sBreathAmt, UINT8 ubOwner , BOOL fFromRemoteClient )
{
					// OJW - 20091028
	if (is_networked && is_client)
	{
		// only the owner of a merc may send damage (as this takes into account equipped gas mask)
		if (IsOurSoldier(pSoldier) || (pSoldier->bTeam == 1 && is_server) && !fFromRemoteClient)
		{
			// allow this function to proceed, we will send it later, when we are sure we take damage this turn and from this function call
		}
		else if (!fFromRemoteClient)
		{
			// skip executing locally because we want the random number generator to be aligned
			// with the client that spawns set off the explosion/grenade/whatever
			return FALSE;
		}
#ifdef JA2BETAVERSION
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"DishOutGasDamage ( ubSoldierID : %i , ubExplosiveType : %i , sSubsequent : %i , recompileMoveCosts : %i , sWoundAmt : %i , sBreathAmt : %i , ubOwner : %i , fRemote : %i)\n", pSoldier->ubID , pExplosive->ubType , sSubsequent , fRecompileMovementCosts , sWoundAmt , sBreathAmt , ubOwner , fFromRemoteClient );
	MPDebugMsg(tmpMPDbgString);
#endif
	}

	FLOAT fGasDamageModifier = 1.0;
	FLOAT fGasBreathDamageModifier = 1.0;
	INT8	bPosOfMask = NO_SLOT;

	if (!pSoldier->bActive || !pSoldier->bInSector || !pSoldier->stats.bLife || AM_A_ROBOT( pSoldier ) )
	{
		return( fRecompileMovementCosts );
	}

	if ( pExplosive->ubType == EXPLOSV_CREATUREGAS )
	{
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER )
		{
			// unaffected by own gas effects
			return( fRecompileMovementCosts );
		}
	}

	// no gas mask helps from creature attacks and fire
	if ( pExplosive->ubType == EXPLOSV_CREATUREGAS || pExplosive->ubType == EXPLOSV_BURNABLEGAS)
	{
		if ( sSubsequent && pSoldier->flags.fHitByGasFlags & HIT_BY_CREATUREGAS )
		{
			// already affected by creature gas this turn
			return( fRecompileMovementCosts );
		}
		// Who cares if he was affected already? Running through a gas cloud is not good for health so let him suffer!
		if ( /*sSubsequent &&*/ pSoldier->flags.fHitByGasFlags & HIT_BY_BURNABLEGAS )
		{
			// Already affected by burnable gas this turn. Lower damage value by ini setting.
			fGasDamageModifier = gItemSettings.fDamageHealthMoveModifierExplosive;
			fGasBreathDamageModifier = gItemSettings.fDamageBreathMoveModifierExplosive;
			// modify damage values
			sWoundAmt *= fGasDamageModifier;
			sBreathAmt *= fGasBreathDamageModifier;
			//return( fRecompileMovementCosts );
		}
	}
	else // 
		// ATE/CJC: gas stuff
	{
		if ( pExplosive->ubType == EXPLOSV_TEARGAS )
		{
			// zombies and robots are unaffected by tear gas
			if ( AM_A_ROBOT( pSoldier ) || pSoldier->IsZombie() )
			{
				return( fRecompileMovementCosts );
			}

			// ignore whether subsequent or not if hit this turn
			// Who cares if he was affected already? Running through a gas cloud is not good for health so let him suffer!
			if ( pSoldier->flags.fHitByGasFlags & HIT_BY_TEARGAS )
			{
				// Already affected by tear gas this turn. Lower damage value by ini setting.
				fGasDamageModifier = gItemSettings.fDamageHealthMoveModifierExplosive;
				fGasBreathDamageModifier = gItemSettings.fDamageBreathMoveModifierExplosive;
				//return( fRecompileMovementCosts );
			}
		}
		else if ( pExplosive->ubType == EXPLOSV_MUSTGAS )
		{
#ifdef ENABLE_ZOMBIES
			// zombies and robots are unaffected by mustard gas
			if ( AM_A_ROBOT( pSoldier ) || pSoldier->IsZombie() )
#else
			if ( AM_A_ROBOT( pSoldier ) )
#endif
			{
				return( fRecompileMovementCosts );
			}

			// Who cares if he was affected already? Running through a gas cloud is not good for health so let him suffer!
			if ( /*sSubsequent &&*/ pSoldier->flags.fHitByGasFlags & HIT_BY_MUSTARDGAS )
			{
				// Already affected by mustard gas this turn. Lower damage value by ini setting.
				fGasDamageModifier = gItemSettings.fDamageHealthMoveModifierExplosive;
				fGasBreathDamageModifier = gItemSettings.fDamageBreathMoveModifierExplosive;
				//return( fRecompileMovementCosts );
			}

		}
		else if(pExplosive->ubType == EXPLOSV_SMOKE)//dnl ch40 200909
		{
			// robots are unaffected by smoke
			if( AM_A_ROBOT(pSoldier) )
				return(fRecompileMovementCosts);

			// Who cares if he was affected already? Running through a gas cloud is not good for health so let him suffer!
			if ( pSoldier->flags.fHitByGasFlags & HIT_BY_SMOKEGAS )
			{
				// Already affected by smoke this turn. Lower damage value by ini setting.
				fGasDamageModifier = gItemSettings.fDamageHealthMoveModifierExplosive;
				fGasBreathDamageModifier = gItemSettings.fDamageBreathMoveModifierExplosive;
				//return( fRecompileMovementCosts );
			}
		}

		// modify damage values
		sWoundAmt *= fGasDamageModifier;
		sBreathAmt *= fGasBreathDamageModifier;

		bPosOfMask = FindGasMask(pSoldier);
		if(!DoesSoldierWearGasMask(pSoldier))//dnl ch40 200909
			bPosOfMask = NO_SLOT;
		if (	bPosOfMask == NO_SLOT || pSoldier->inv[ bPosOfMask ][0]->data.objectStatus < USABLE )
		{
			bPosOfMask = NO_SLOT;
		}
		//if ( pSoldier->inv[ HEAD1POS ].usItem == GASMASK && pSoldier->inv[ HEAD1POS ][0]->data.objectStatus >= USABLE )
		//{
		// bPosOfMask = HEAD1POS;
		//}
		//else if ( pSoldier->inv[ HEAD2POS ].usItem == GASMASK && pSoldier->inv[ HEAD2POS ][0]->data.objectStatus >= USABLE )
		//{
		// bPosOfMask = HEAD2POS;
		//}

		if ( bPosOfMask != NO_SLOT	)
		{
			if ( pSoldier->inv[ bPosOfMask ][0]->data.objectStatus < GASMASK_MIN_STATUS )
			{
				// GAS MASK reduces breath loss by its work% (it leaks if not at least 70%)
				sBreathAmt = ( sBreathAmt * ( 100 - pSoldier->inv[ bPosOfMask ][0]->data.objectStatus ) ) / 100;
				if ( sBreathAmt > 500 )
				{
					// if at least 500 of breath damage got through
					// the soldier within the blast radius is gassed for at least one
					// turn, possibly more if it's tear gas (which hangs around a while)
					pSoldier->flags.uiStatusFlags |= SOLDIER_GASSED;
				}

				if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC )
				{

					if ( sWoundAmt > 1 )
					{
						pSoldier->inv[ bPosOfMask ][0]->data.objectStatus =
							pSoldier->inv[ bPosOfMask ][0]->data.objectStatus - (INT8) Random( 4 );
						sWoundAmt = ( sWoundAmt * ( 100 -	pSoldier->inv[ bPosOfMask ][0]->data.objectStatus ) ) / 100;
					}
					else if ( sWoundAmt == 1 )
					{
						pSoldier->inv[ bPosOfMask ][0]->data.objectStatus =
							pSoldier->inv[ bPosOfMask ][0]->data.objectStatus - (INT8) Random( 2 );
					}
				}
			}
			else
			{
				sBreathAmt = 0;
				if ( sWoundAmt > 0 )
				{
					if ( sWoundAmt == 1 )
					{
						pSoldier->inv[ bPosOfMask ][0]->data.objectStatus =
							pSoldier->inv[ bPosOfMask ][0]->data.objectStatus - (INT8) Random( 2 );
					}
					else
					{
						// use up gas mask
						pSoldier->inv[ bPosOfMask ][0]->data.objectStatus =
							pSoldier->inv[ bPosOfMask ][0]->data.objectStatus - (INT8) Random( 4 );
					}
				}
				sWoundAmt = 0;
			}

		}
	}

	if ( sWoundAmt != 0 || sBreathAmt != 0 )
	{
		switch( pExplosive->ubType )
		{
		case EXPLOSV_CREATUREGAS:
			pSoldier->flags.fHitByGasFlags |= HIT_BY_CREATUREGAS;
			break;
		case EXPLOSV_TEARGAS:
			pSoldier->flags.fHitByGasFlags |= HIT_BY_TEARGAS;
			break;
		case EXPLOSV_MUSTGAS:		
			pSoldier->flags.fHitByGasFlags |= HIT_BY_MUSTARDGAS;
			break;
		case EXPLOSV_BURNABLEGAS:
			pSoldier->flags.fHitByGasFlags |= HIT_BY_BURNABLEGAS;
			break;
		case EXPLOSV_SMOKE://dnl ch40 200909
			pSoldier->flags.fHitByGasFlags |= HIT_BY_SMOKEGAS;
			break;
		default:
			break;
		}

		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"ExpControl pSoldier->flags.fHitByGasFlags: %d", pSoldier->flags.fHitByGasFlags );

		// a gas effect, take damage directly...
		pSoldier->SoldierTakeDamage( ANIM_STAND, sWoundAmt, 0, sBreathAmt, TAKE_DAMAGE_GAS, NOBODY, NOWHERE, 0, TRUE );

		if (is_networked && is_client)
		{
			// if it gets here we are supposed to send it.
			// let all the other clients know that our merc got gassed
			// and align them with our random number generator
			if (IsOurSoldier(pSoldier) || (pSoldier->bTeam == 1 && is_server) && !fFromRemoteClient)
			{
				send_gasdamage( pSoldier , pExplosive->uiIndex , sSubsequent , fRecompileMovementCosts , sWoundAmt , sBreathAmt , ubOwner );
			}
		}

		if ( pSoldier->stats.bLife >= CONSCIOUSNESS )
		{
			pSoldier->DoMercBattleSound( (INT8)( BATTLE_SOUND_HIT1 + Random( 2 ) ) );
		}

		if ( ubOwner != NOBODY && MercPtrs[ ubOwner ]->bTeam == gbPlayerNum && pSoldier->bTeam != gbPlayerNum )
		{
			ProcessImplicationsOfPCAttack( MercPtrs[ ubOwner ], &pSoldier, REASON_EXPLOSION );
		}
	}
	return( fRecompileMovementCosts );
}

BOOLEAN ExpAffect( INT32 sBombGridNo, INT32 sGridNo, UINT32 uiDist, UINT16 usItem, UINT8 ubOwner,  INT16 sSubsequent, BOOLEAN *pfMercHit, INT8 bLevel, INT32 iSmokeEffectID )
{
#ifdef JA2BETAVERSION
	if (is_networked) {
		CHAR tmpMPDbgString[512];
		sprintf(tmpMPDbgString,"ExpAffect ( sBombGridNo : %i , sGridNo : %i , uiDist : %i , usItem : %i , ubOwner : %i , sSubsequent : %i , fMercHit : %i , bLevel : %i , iSmokeEffectID : %i )\n",sBombGridNo, sGridNo , uiDist , usItem , ubOwner , sSubsequent , (int)*pfMercHit , bLevel , iSmokeEffectID );
		MPDebugMsg(tmpMPDbgString);
	}
#endif

	INT16 sWoundAmt = 0,sBreathAmt = 0, /* sNewWoundAmt = 0, sNewBreathAmt = 0, */ sStructDmgAmt;
	UINT8 ubPerson;
	SOLDIERTYPE *pSoldier;
	EXPLOSIVETYPE *pExplosive;
	INT16 sX, sY;
	BOOLEAN fRecompileMovementCosts = FALSE;
	BOOLEAN fSmokeEffect = FALSE;
	BOOLEAN fStunEffect	= FALSE;
	BOOLEAN fBlastEffect = TRUE;
	BOOLEAN fBloodEffect = FALSE;
	INT8 bSmokeEffectType = 0;
	INT32 sNewGridNo;
	ITEM_POOL * pItemPool, * pItemPoolNext;
	UINT32 uiRoll;

	//Init the variables
	sX = sY = -1;

	if ( sSubsequent == BLOOD_SPREAD_EFFECT )
	{
		fSmokeEffect	= FALSE;
		fBlastEffect	= FALSE;
		fBloodEffect	= TRUE;
	}
	else
	{
		// Turn off blast effect if some types of items...
		switch( Explosive[Item[usItem].ubClassIndex].ubType	)
		{
		case EXPLOSV_MUSTGAS:

			fSmokeEffect	= TRUE;
			bSmokeEffectType = MUSTARDGAS_SMOKE_EFFECT;
			fBlastEffect	= FALSE;
			break;

		case EXPLOSV_BURNABLEGAS:

			fSmokeEffect	= TRUE;
			bSmokeEffectType = BURNABLEGAS_SMOKE_EFFECT;
			fBlastEffect	= FALSE;
			break;

		case EXPLOSV_TEARGAS:

			fSmokeEffect	= TRUE;
			bSmokeEffectType = TEARGAS_SMOKE_EFFECT;
			fBlastEffect	= FALSE;
			break;

		case EXPLOSV_SMOKE:

			fSmokeEffect	= TRUE;
			bSmokeEffectType = NORMAL_SMOKE_EFFECT;
			fBlastEffect	= FALSE;
			break;

		case EXPLOSV_STUN:
			fStunEffect	= TRUE;
			break;

		case EXPLOSV_CREATUREGAS:

			fSmokeEffect	= TRUE;
			bSmokeEffectType = CREATURE_SMOKE_EFFECT;
			fBlastEffect	= FALSE;
			break;

		case EXPLOSV_SIGNAL_SMOKE:

			fSmokeEffect	= TRUE;
			bSmokeEffectType = SIGNAL_SMOKE_EFFECT;
			fBlastEffect	= FALSE;
			break;
		}
	}


	// OK, here we:
	// Get explosive data from table
	pExplosive = &(Explosive[ Item[ usItem ].ubClassIndex ] );

	uiRoll = PreRandom( 100 );

	// Calculate wound amount
	// HEADROCK HAM 3.6: Can now use negative modifier.
	INT16 newDamage = (INT16)GetModifiedExplosiveDamage( pExplosive->ubDamage, 0 );
	//INT16 newDamage = pExplosive->ubDamage + (INT16)(( pExplosive->ubDamage * gGameExternalOptions.ubExplosivesDamageMultiplier) / 100); //lal
	
	//DBrot: apply a modifier to confined explosions
	if(InARoom(sBombGridNo, NULL)){
		newDamage += (INT16)(newDamage * pExplosive->bIndoorModifier);
	}
	sWoundAmt = newDamage + (INT16) ( (newDamage * uiRoll) / 100 );

	// Calculate breath amount ( if stun damage applicable )
	INT16 newBreath = (INT16)GetModifiedExplosiveDamage( pExplosive->ubStunDamage, 1 );
	//INT16 newBreath = pExplosive->ubStunDamage + (INT16)(( pExplosive->ubStunDamage * gGameExternalOptions.ubExplosivesDamageMultiplier) / 100); //lal

	if(InARoom(sBombGridNo, NULL)){
		newBreath += (INT16)(newBreath * pExplosive->bIndoorModifier);
	}

	sBreathAmt = ( newBreath * 100 ) + (INT16) ( ( ( newBreath / 2 ) * 100 * uiRoll ) / 100 ) ;


	// ATE: Make sure guys get pissed at us!
	HandleBuldingDestruction( sGridNo ,ubOwner );


	if ( fBlastEffect )
	{
		// lower effects for distance away from center of explosion
		// If radius is 3, damage % is (100)/66/33/17
		// If radius is 5, damage % is (100)/80/60/40/20/10
		// If radius is 8, damage % is (100)/88/75/63/50/37/25/13/6

		if ( pExplosive->ubRadius == 0 )
		{
			// leave as is, has to be at range 0 here
		}
		else if (uiDist < pExplosive->ubRadius)
		{
			// if radius is 5, go down by 5ths ~ 20%
			sWoundAmt = sWoundAmt - (INT16)  (sWoundAmt * uiDist / pExplosive->ubRadius );
			sBreathAmt = sBreathAmt - (INT16) (sBreathAmt * uiDist / pExplosive->ubRadius );
		}
		else
		{
			// at the edge of the explosion, do half the previous damage
			sWoundAmt = (INT16) ( (sWoundAmt / pExplosive->ubRadius) / 2);
			sBreathAmt = (INT16) ( (sBreathAmt / pExplosive->ubRadius) / 2);
		}

		if (sWoundAmt < 0)
			sWoundAmt = 0;

		if (sBreathAmt < 0)
			sBreathAmt = 0;

		// damage structures
		if ( uiDist <= __max( 1, (UINT32) (newDamage / 30) ) )
		{
			if ( Item[ usItem ].usItemClass & IC_GRENADE )
			{
				sStructDmgAmt = sWoundAmt / 3;
			}
			else // most explosives
			{
				sStructDmgAmt = sWoundAmt;
			}

			// Flugente: roof collapse
			// Only do this at the source of the explosion (uiDist == 0).
			if ( !uiDist )
			{
				// walls are attributed to a gridno. As a result, sometimes the outer wall of a house is located at a gridno that does not 'belong' to the house.
				// This gridno will then have no roof over it, which HandleRoofDestruction(..) requires.
				// For this reason, we call it also at neighbouring gridnos.
				// If the explosion, however, is on a roof itself, no need for these shenanigans.
				if ( bLevel )
				{
					HandleRoofDestruction( sGridNo, sWoundAmt );
				}
				else
				{
					HandleRoofDestruction( sGridNo, sWoundAmt * 0.75f );
					HandleRoofDestruction( NewGridNo( sGridNo, DirectionInc( NORTH )), sWoundAmt * 0.75f );
					HandleRoofDestruction( NewGridNo( sGridNo, DirectionInc( EAST )), sWoundAmt * 0.75f );
					HandleRoofDestruction( NewGridNo( sGridNo, DirectionInc( WEST )), sWoundAmt * 0.75f );
					HandleRoofDestruction( NewGridNo( sGridNo, DirectionInc( SOUTH )), sWoundAmt * 0.75f );
				}
			}
			
			ExplosiveDamageGridNo( sGridNo, sStructDmgAmt, uiDist, &fRecompileMovementCosts, FALSE, -1, 0 , ubOwner, bLevel );

			// ATE: Look for damage to walls ONLY for next two gridnos
			sNewGridNo = NewGridNo( sGridNo, DirectionInc( NORTH ) );

			if ( GridNoOnVisibleWorldTile( sNewGridNo ) )
			{
				ExplosiveDamageGridNo( sNewGridNo, sStructDmgAmt, uiDist, &fRecompileMovementCosts, TRUE, -1, 0, ubOwner, bLevel );
			}

			// ATE: Look for damage to walls ONLY for next two gridnos
			sNewGridNo = NewGridNo( sGridNo, DirectionInc( WEST ) );

			if ( GridNoOnVisibleWorldTile( sNewGridNo ) )
			{
				ExplosiveDamageGridNo( sNewGridNo, sStructDmgAmt, uiDist, &fRecompileMovementCosts, TRUE, -1, 0, ubOwner, bLevel );
			}
		}

		// Add burn marks to ground randomly....
		if ( Random( 50 ) < 15 && uiDist == 1 )
		{
			//if ( !TypeRangeExistsInObjectLayer( sGridNo, FIRSTEXPLDEBRIS, SECONDEXPLDEBRIS, &usObjectIndex ) )
			//{
			// GetTileIndexFromTypeSubIndex( SECONDEXPLDEBRIS, (UINT16)( Random( 10 ) + 1 ), &usTileIndex );
			// AddObjectToHead( sGridNo, usTileIndex );

			// SetRenderFlags(RENDER_FLAG_FULL);

			//}
		}

		// NB radius can be 0 so cannot divide it by 2 here
		if (!fStunEffect && (uiDist * 2 <= pExplosive->ubRadius)	)
		{
			GetItemPool( sGridNo, &pItemPool, bLevel );

			while( pItemPool )
			{
				pItemPoolNext = pItemPool->pNext;

				if ( DamageItemOnGround( &(gWorldItems[ pItemPool->iItemIndex ].object), sGridNo, bLevel, (INT32) (sWoundAmt * 2), ubOwner ) )
				{
					// item was destroyed
					RemoveItemFromPool( sGridNo, pItemPool->iItemIndex, bLevel );
					// sevenfm: if no other bomb exists here
					CheckForBuriedBombsAndRemoveFlags( sGridNo, bLevel );
				}
				pItemPool = pItemPoolNext;
			}

			/*
			// Search for an explosive item in item pool
			while ( ( iWorldItem = GetItemOfClassTypeInPool( sGridNo, IC_EXPLOSV, bLevel ) ) != -1 )
			{
			// Get usItem
			usItem = gWorldItems[ iWorldItem ].object.usItem;

			DamageItem

			if ( CheckForChainReaction( usItem, gWorldItems[ iWorldItem ].object[0]->data.objectStatus, sWoundAmt, TRUE ) )
			{
			RemoveItemFromPool( sGridNo, iWorldItem, bLevel );

			// OK, Ignite this explosion!
			IgniteExplosion( NOBODY, sX, sY, 0, sGridNo, usItem, bLevel );
			}
			else
			{
			RemoveItemFromPool( sGridNo, iWorldItem, bLevel );
			}

			}

			// Remove any unburied items here!
			RemoveAllUnburiedItems( sGridNo, bLevel );
			*/
		}
	}
	else if ( fSmokeEffect )
	{
		// If tear gar, determine turns to spread.....
		if ( sSubsequent == ERASE_SPREAD_EFFECT )
		{
			RemoveSmokeEffectFromTile( sGridNo, bLevel );
		}
		else if ( sSubsequent != REDO_SPREAD_EFFECT )
		{
			AddSmokeEffectToTile( iSmokeEffectID, bSmokeEffectType, sGridNo, bLevel );
		}
	}
	else
	{
		// Drop blood ....
		// Get blood quantity....
		InternalDropBlood( sGridNo, 0, 0, (UINT8)(__max( ( MAXBLOODQUANTITY - ( uiDist * 2 ) ), 0 ) ), 1 );
	}

	if ( sSubsequent != ERASE_SPREAD_EFFECT && sSubsequent != BLOOD_SPREAD_EFFECT )
	{
		// if an explosion effect....
		if ( fBlastEffect )
		{
			// don't hurt anyone who is already dead & waiting to be removed
			if ( ( ubPerson = WhoIsThere2( sGridNo, bLevel ) ) != NOBODY )
			{
				DamageSoldierFromBlast( ubPerson, ubOwner, sBombGridNo, sWoundAmt, sBreathAmt, uiDist, usItem, sSubsequent );
			}

			if ( bLevel == 1 )
			{
				if ( ( ubPerson = WhoIsThere2(sGridNo, 0 ) ) != NOBODY )
				{
					if ( (sWoundAmt / 2) > 20 )
					{
						// debris damage!
						if ( (sBreathAmt / 2) > 20 )
						{
							DamageSoldierFromBlast( ubPerson, ubOwner, sBombGridNo, (INT16) Random( (sWoundAmt / 2) - 20 ), (INT16) Random( (sBreathAmt / 2) - 20 ), uiDist, usItem, sSubsequent );
						}
						else
						{
							DamageSoldierFromBlast( ubPerson, ubOwner, sBombGridNo, (INT16) Random( (sWoundAmt / 2) - 20 ), 1, uiDist, usItem, sSubsequent );
						}

					}

				}
			}
		}
		else
		{
			if ( ( ubPerson = WhoIsThere2(sGridNo, bLevel ) ) >= TOTAL_SOLDIERS )
			{
				return( fRecompileMovementCosts );
			}

			pSoldier = MercPtrs[ ubPerson ];	// someone is here, and they're gonna get hurt

			// silversurfer: Gas now only has an effect when the container had time to emit some. Initially it will do nothing.
			// This prevents the problem that we have to suffer two times without a chance to react (1st when the grenade hits our position, 2nd when our turn starts)
			if ( sSubsequent > 0 )
				fRecompileMovementCosts = DishOutGasDamage( pSoldier, pExplosive, sSubsequent, fRecompileMovementCosts, sWoundAmt, sBreathAmt, ubOwner );
			/*
			if (!pSoldier->bActive || !pSoldier->bInSector || !pSoldier->stats.bLife || AM_A_ROBOT( pSoldier ) )
			{
			return( fRecompileMovementCosts );
			}

			if ( pExplosive->ubType == EXPLOSV_CREATUREGAS )
			{
			if ( pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER )
			{
			// unaffected by own gas effects
			return( fRecompileMovementCosts );
			}
			if ( sSubsequent && pSoldier->flags.fHitByGasFlags & HIT_BY_CREATUREGAS )
			{
			// already affected by creature gas this turn
			return( fRecompileMovementCosts );
			}
			}
			else // no gas mask help from creature attacks
			// ATE/CJC: gas stuff
			{
			INT8 bPosOfMask = NO_SLOT;


			if ( pExplosive->ubType == EXPLOSV_TEARGAS )
			{
			// ignore whether subsequent or not if hit this turn
			if ( pSoldier->flags.fHitByGasFlags & HIT_BY_TEARGAS )
			{
			// already affected by creature gas this turn
			return( fRecompileMovementCosts );
			}
			}
			else if ( pExplosive->ubType == EXPLOSV_MUSTGAS )
			{
			if ( sSubsequent && pSoldier->flags.fHitByGasFlags & HIT_BY_MUSTARDGAS )
			{
			// already affected by creature gas this turn
			return( fRecompileMovementCosts );
			}

			}

			if ( sSubsequent && pSoldier->flags.fHitByGasFlags & HIT_BY_CREATUREGAS )
			{
			// already affected by creature gas this turn
			return( fRecompileMovementCosts );
			}


			if ( pSoldier->inv[ HEAD1POS ].usItem == GASMASK && pSoldier->inv[ HEAD1POS ][0]->data.objectStatus >= USABLE )
			{
			bPosOfMask = HEAD1POS;
			}
			else if ( pSoldier->inv[ HEAD2POS ].usItem == GASMASK && pSoldier->inv[ HEAD2POS ][0]->data.objectStatus >= USABLE )
			{
			bPosOfMask = HEAD2POS;
			}

			if ( bPosOfMask != NO_SLOT	)
			{
			if ( pSoldier->inv[ bPosOfMask ][0]->data.objectStatus < GASMASK_MIN_STATUS )
			{
			// GAS MASK reduces breath loss by its work% (it leaks if not at least 70%)
			sBreathAmt = ( sBreathAmt * ( 100 - pSoldier->inv[ bPosOfMask ][0]->data.objectStatus ) ) / 100;
			if ( sBreathAmt > 500 )
			{
			// if at least 500 of breath damage got through
			// the soldier within the blast radius is gassed for at least one
			// turn, possibly more if it's tear gas (which hangs around a while)
			pSoldier->flags.uiStatusFlags |= SOLDIER_GASSED;
			}

			if ( sWoundAmt > 1 )
			{
			pSoldier->inv[ bPosOfMask ][0]->data.objectStatus -= (INT8) Random( 4 );
			sWoundAmt = ( sWoundAmt * ( 100 -	pSoldier->inv[ bPosOfMask ][0]->data.objectStatus ) ) / 100;
			}
			else if ( sWoundAmt == 1 )
			{
			pSoldier->inv[ bPosOfMask ][0]->data.objectStatus -= (INT8) Random( 2 );
			}
			}
			else
			{
			sBreathAmt = 0;
			if ( sWoundAmt > 0 )
			{
			if ( sWoundAmt == 1 )
			{
			pSoldier->inv[ bPosOfMask ][0]->data.objectStatus -= (INT8) Random( 2 );
			}
			else
			{
			// use up gas mask
			pSoldier->inv[ bPosOfMask ][0]->data.objectStatus -= (INT8) Random( 4 );
			}
			}
			sWoundAmt = 0;
			}

			}
			}

			if ( sWoundAmt != 0 || sBreathAmt != 0 )
			{
			switch( pExplosive->ubType )
			{
			case EXPLOSV_CREATUREGAS:
			pSoldier->flags.fHitByGasFlags |= HIT_BY_CREATUREGAS;
			break;
			case EXPLOSV_TEARGAS:
			pSoldier->flags.fHitByGasFlags |= HIT_BY_TEARGAS;
			break;
			case EXPLOSV_MUSTGAS:
			pSoldier->flags.fHitByGasFlags |= HIT_BY_MUSTARDGAS;
			break;
			default:
			break;
			}
			// a gas effect, take damage directly...
			pSoldier->SoldierTakeDamage( ANIM_STAND, sWoundAmt, 0, sBreathAmt, TAKE_DAMAGE_GAS, NOBODY, NOWHERE, 0, TRUE );
			if ( pSoldier->stats.bLife >= CONSCIOUSNESS )
			{
			pSoldier->DoMercBattleSound( (INT8)( BATTLE_SOUND_HIT1 + Random( 2 ) ) );
			}
			}
			*/
		}

		(*pfMercHit) = TRUE;
	}

	return( fRecompileMovementCosts );

}

void GetRayStopInfo( UINT32 uiNewSpot, UINT8 ubDir, INT8 bLevel, BOOLEAN fSmokeEffect, INT32 uiCurRange, INT32 *piMaxRange, UINT8 *pubKeepGoing )
{
	INT8		 bStructHeight;
	UINT8	ubMovementCost;
	INT8	 Blocking, BlockingTemp;
	BOOLEAN		fTravelCostObs = FALSE;
	UINT32		uiRangeReduce;
   INT32 sNewGridNo;
	STRUCTURE * pBlockingStructure;
	BOOLEAN		fBlowWindowSouth = FALSE;
	BOOLEAN	fReduceRay = TRUE;

	ubMovementCost = gubWorldMovementCosts[ uiNewSpot ][ ubDir ][ bLevel ];

	if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
	{
		ubMovementCost = DoorTravelCost( NULL, uiNewSpot, ubMovementCost, FALSE, NULL );
		// If we have hit a wall, STOP HERE
		if (ubMovementCost >= TRAVELCOST_BLOCKED)
		{
			fTravelCostObs	= TRUE;
		}
	}
	else
	{
		// If we have hit a wall, STOP HERE
		if ( ubMovementCost == TRAVELCOST_WALL )
		{
			// We have an obstacle here..
			fTravelCostObs = TRUE;
		}
	}


	Blocking = GetBlockingStructureInfo( uiNewSpot, ubDir, 0, bLevel, &bStructHeight, &pBlockingStructure, TRUE );//dnl ch53 111009

	if ( pBlockingStructure )
	{
		if ( pBlockingStructure->fFlags & STRUCTURE_CAVEWALL )
		{
			// block completely!
			fTravelCostObs = TRUE;
		}
		else if ( pBlockingStructure->pDBStructureRef->pDBStructure->ubDensity <= 15 )
		{
			// not stopped
			fTravelCostObs = FALSE;
			fReduceRay = FALSE;
		}
	}

	// ATE: For smoke, don't let it go over roof....
	// 0verhaul:	Why not?
	if ( fSmokeEffect )
	{
		if ( bLevel )
		{
			STRUCTURE * pStructure;

			// Check for roof here....
			pStructure = FindStructure( uiNewSpot, STRUCTURE_ROOF );

			if ( pStructure == NULL )
			{
				// block completely!
				fTravelCostObs = TRUE;
			}
		}
	}
	if ( fTravelCostObs )
	{

		if ( fSmokeEffect )
		{
			if ( Blocking == BLOCKING_TOPRIGHT_OPEN_WINDOW || Blocking == BLOCKING_TOPLEFT_OPEN_WINDOW )
			{
				// If open, fTravelCostObs set to false and reduce range....
				fTravelCostObs = FALSE;
				// Range will be reduced below...
			}

			if ( fTravelCostObs )
			{
				// ATE: For windows, check to the west and north for a broken window, as movement costs
				// will override there...
				 sNewGridNo = NewGridNo( uiNewSpot, DirectionInc( WEST ) );

				 BlockingTemp = GetBlockingStructureInfo( sNewGridNo, ubDir, 0, bLevel, &bStructHeight, &pBlockingStructure, TRUE );
				if ( BlockingTemp == BLOCKING_TOPRIGHT_OPEN_WINDOW || BlockingTemp == BLOCKING_TOPLEFT_OPEN_WINDOW )
				{
					// If open, fTravelCostObs set to false and reduce range....
					fTravelCostObs = FALSE;
					// Range will be reduced below...
				}
				if ( pBlockingStructure && pBlockingStructure->pDBStructureRef->pDBStructure->ubDensity <= 15 )
				{
					fTravelCostObs = FALSE;
					fReduceRay = FALSE;
				}
			}

			if ( fTravelCostObs )
			{
				 sNewGridNo = NewGridNo( uiNewSpot, DirectionInc( NORTH ) );

				 BlockingTemp = GetBlockingStructureInfo( sNewGridNo, ubDir, 0, bLevel, &bStructHeight, &pBlockingStructure, TRUE );
				if ( BlockingTemp == BLOCKING_TOPRIGHT_OPEN_WINDOW || BlockingTemp == BLOCKING_TOPLEFT_OPEN_WINDOW )
				{
					// If open, fTravelCostObs set to false and reduce range....
					fTravelCostObs = FALSE;
					// Range will be reduced below...
				}
				if ( pBlockingStructure && pBlockingStructure->pDBStructureRef->pDBStructure->ubDensity <= 15 )
				{
					fTravelCostObs = FALSE;
					fReduceRay = FALSE;
				}
			}

		}
		else
		{
			// We are a blast effect....

			// ATE: explode windows!!!!
			if ( Blocking == BLOCKING_TOPLEFT_WINDOW || Blocking == BLOCKING_TOPRIGHT_WINDOW )
			{
				// Explode!
				if ( ubDir == SOUTH || ubDir == SOUTHEAST || ubDir == SOUTHWEST )
				{
					fBlowWindowSouth = TRUE;
				}

				if ( pBlockingStructure != NULL )
				{
					WindowHit( uiNewSpot, pBlockingStructure->usStructureID, fBlowWindowSouth, TRUE );
				}
			}

			// ATE: For windows, check to the west and north for a broken window, as movement costs
			// will override there...
			 sNewGridNo = NewGridNo( uiNewSpot, DirectionInc( WEST ) );

			 BlockingTemp = GetBlockingStructureInfo( sNewGridNo, ubDir, 0, bLevel, &bStructHeight, &pBlockingStructure , TRUE );
			if ( pBlockingStructure && pBlockingStructure->pDBStructureRef->pDBStructure->ubDensity <= 15 )
			{
				fTravelCostObs = FALSE;
				fReduceRay = FALSE;
			}
			if ( BlockingTemp == BLOCKING_TOPRIGHT_WINDOW || BlockingTemp == BLOCKING_TOPLEFT_WINDOW )
			{
				if ( pBlockingStructure != NULL )
				{
					WindowHit( sNewGridNo, pBlockingStructure->usStructureID, FALSE, TRUE );
				}
			}

			 sNewGridNo = NewGridNo( uiNewSpot, DirectionInc( NORTH ) );
			 BlockingTemp = GetBlockingStructureInfo( sNewGridNo, ubDir, 0, bLevel, &bStructHeight, &pBlockingStructure, TRUE );

			if ( pBlockingStructure && pBlockingStructure->pDBStructureRef->pDBStructure->ubDensity <= 15 )
			{
				fTravelCostObs = FALSE;
				fReduceRay = FALSE;
			}
			if ( BlockingTemp == BLOCKING_TOPRIGHT_WINDOW || BlockingTemp == BLOCKING_TOPLEFT_WINDOW )
			{
				if ( pBlockingStructure != NULL )
				{
					WindowHit( sNewGridNo, pBlockingStructure->usStructureID, FALSE, TRUE );
				}
			}
		}
	}

	// Have we hit things like furniture, etc?
	if ( Blocking != NOTHING_BLOCKING && !fTravelCostObs )
	{
		// ATE: Tall things should blaock all
		if ( bStructHeight == 4 )
		{
			(*pubKeepGoing) = FALSE;
		}
		else
		{
			// If we are smoke, reduce range variably....
			if ( fReduceRay )
			{
				if ( fSmokeEffect )
				{
					switch( bStructHeight )
					{
					case 3:
						uiRangeReduce = 2;
						break;

					case 2:

						uiRangeReduce = 1;
						break;

					default:

						uiRangeReduce = 0;
						break;
					}
				}
				else
				{
					uiRangeReduce = 2;
				}

				( *piMaxRange ) -= uiRangeReduce;
			}

			if ( uiCurRange <= (*piMaxRange) )
			{
				(*pubKeepGoing) = TRUE;
			}
			else
			{
				(*pubKeepGoing) = FALSE;
			}
		}
	}
	else
	{
		if ( fTravelCostObs )
		{
			( *pubKeepGoing ) = FALSE;
		}
		else
		{
			( *pubKeepGoing ) = TRUE;
		}
	}
}



void SpreadEffect( INT32 sGridNo, UINT8 ubRadius, UINT16 usItem, UINT8 ubOwner, BOOLEAN fSubsequent, INT8 bLevel, INT32 iSmokeEffectID , BOOL fFromRemoteClient , BOOL fNewSmokeEffect  )
{
	if (is_networked && is_client)
	{
		SOLDIERTYPE* pAttacker = MercPtrs[ubOwner];
		if (pAttacker != NULL)
		{
			if (IsOurSoldier(pAttacker) || (pAttacker->bTeam == 1 && is_server))
			{
				// dont send SpreadEffect if it was just called from NewSmokeEffect - as now we sync that seperately
				if (!fNewSmokeEffect)
				{
					// let all the other clients know we are spawning this effect
					// and align them with our random number generator
					send_spreadeffect(sGridNo,ubRadius,usItem,ubOwner,fSubsequent,bLevel,iSmokeEffectID);
				}
			}
			else if (!fFromRemoteClient)
			{
				// skip executing locally because we want the random number generator to be aligned
				// with the client that spawns set off the explosion/grenade/whatever
				return;
			}

			// Flugente: campaign stats
			if ( Explosive[Item[usItem].ubClassIndex].ubType == EXPLOSV_MUSTGAS )
			{
				if ( IsOurSoldier(pAttacker) )
					gCurrentIncident.usIncidentFlags |= INCIDENT_MUSTARDGAS_PLAYERSIDE;
				else
					gCurrentIncident.usIncidentFlags |= INCIDENT_MUSTARDGAS_ENEMY;
			}
		}
#ifdef JA2BETAVERSION
		CHAR tmpMPDbgString[512];
		sprintf(tmpMPDbgString,"SpreadEffect ( sGridNo : %i , ubRadius : %i , usItem : %i , ubOwner : %i , fSubsequent : %i , bLevel : %i , iSmokeEffectID : %i , fFromRemote : %i , fNewSmoke : %i )\n",sGridNo, ubRadius , usItem , ubOwner , (int)fSubsequent  , bLevel , iSmokeEffectID , fFromRemoteClient , fNewSmokeEffect );
		MPDebugMsg(tmpMPDbgString);
		gfMPDebugOutputRandoms = true;
#endif
	}
		

	INT32 uiNewSpot, uiTempSpot, uiBranchSpot, cnt, branchCnt;
	INT32	uiTempRange, ubBranchRange;
	UINT8	ubDir,ubBranchDir, ubKeepGoing;
	INT16 sRange;
	BOOLEAN	fRecompileMovement = FALSE;
	BOOLEAN	fAnyMercHit = FALSE;
	BOOLEAN		fSmokeEffect = FALSE;

	switch( Explosive[Item[usItem].ubClassIndex].ubType	)
	{

	case EXPLOSV_MUSTGAS:
	case EXPLOSV_BURNABLEGAS:
	case EXPLOSV_TEARGAS:
	case EXPLOSV_SMOKE:
	case EXPLOSV_CREATUREGAS:
	case EXPLOSV_SIGNAL_SMOKE:

		fSmokeEffect = TRUE;
		break;
	}
/*if(is_networked)
{
	ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, L"explosives not coded in MP");
	return;
}*/
	// Set values for recompile region to optimize area we need to recompile for MPs
	gsRecompileAreaTop = sGridNo / WORLD_COLS;
	gsRecompileAreaLeft = sGridNo % WORLD_COLS;
	gsRecompileAreaRight = gsRecompileAreaLeft;
	gsRecompileAreaBottom = gsRecompileAreaTop;

	// multiply range by 2 so we can correctly calculate approximately round explosion regions
	sRange = ubRadius * 2;

	// first, affect main spot
	if ( ExpAffect( sGridNo, sGridNo, 0, usItem, ubOwner, fSubsequent, &fAnyMercHit, bLevel, iSmokeEffectID ) )
	{
		fRecompileMovement = TRUE;
	}


	for (ubDir = NORTH; ubDir <= NORTHWEST; ubDir++ )
	{
		uiTempSpot = sGridNo;

		uiTempRange = sRange;

		if (ubDir & 1)
		{
			cnt = 3;
		}
		else
		{
			cnt = 2;
		}
		while( cnt <= uiTempRange) // end of range loop
		{
			// move one tile in direction
			uiNewSpot = NewGridNo( uiTempSpot, DirectionInc( ubDir ) );

			// see if this was a different spot & if we should be able to reach
			// this spot
			if (uiNewSpot == uiTempSpot)
			{
				ubKeepGoing = FALSE;
			}
			else
			{
				// Check if struct is a tree, etc and reduce range...
				GetRayStopInfo( uiNewSpot, ubDir, bLevel, fSmokeEffect, cnt, &uiTempRange, &ubKeepGoing );
			}

			if (ubKeepGoing)
			{
				uiTempSpot = uiNewSpot;

				//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Explosion affects %d", uiNewSpot) );
				// ok, do what we do here...
       if ( ExpAffect( sGridNo, uiNewSpot, cnt / 2, usItem, ubOwner, fSubsequent, &fAnyMercHit, bLevel, iSmokeEffectID ) )
				{
					fRecompileMovement = TRUE;
				}

				// how far should we branch out here?
				ubBranchRange = (UINT8)( sRange - cnt );

				if ( ubBranchRange )
				{
					// ok, there's a branch here. Mark where we start this branch.
					uiBranchSpot = uiNewSpot;

					// figure the branch direction - which is one dir clockwise
					ubBranchDir = (ubDir + 1) % 8;

					if (ubBranchDir & 1)
					{
						branchCnt = 3;
					}
					else
					{
						branchCnt = 2;
					}

					while( branchCnt <= ubBranchRange) // end of range loop
					{
						ubKeepGoing	= TRUE;
						uiNewSpot = NewGridNo( uiBranchSpot, DirectionInc(ubBranchDir));

						if (uiNewSpot != uiBranchSpot)
						{
							// Check if struct is a tree, etc and reduce range...
							GetRayStopInfo( uiNewSpot, ubBranchDir, bLevel, fSmokeEffect, branchCnt, &ubBranchRange, &ubKeepGoing );

							if ( ubKeepGoing )
							{
								// ok, do what we do here
								//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Explosion affects %d", uiNewSpot) );
								if ( ExpAffect( sGridNo, uiNewSpot, (INT16)((cnt + branchCnt) / 2), usItem, ubOwner, fSubsequent, &fAnyMercHit, bLevel, iSmokeEffectID ) )
								{
									fRecompileMovement = TRUE;
								}
								uiBranchSpot = uiNewSpot;
							}
							//else
							{
								// check if it's ANY door, and if so, affect that spot so it's damaged
								//	if (RealDoorAt(uiNewSpot))
								//	{
								//		ExpAffect(sGridNo,uiNewSpot,cnt,ubReason,fSubsequent);
								//	}
								// blocked, break out of the the sub-branch loop
								//	break;
							}
						}

						if (ubBranchDir & 1)
						{
							branchCnt += 3;
						}
						else
						{
							branchCnt += 2;
						}

					}
				} // end of if a branch to do

			}
			else	 // at edge, or tile blocks further spread in that direction
			{
				break;
			}

			if (ubDir & 1)
			{
				cnt += 3;
			}
			else
			{
				cnt += 2;
			}
		}

	} // end of dir loop

	// Recompile movement costs...
	if ( fRecompileMovement )
	{
		INT16 sX, sY;

		// DO wireframes as well
		ConvertGridNoToXY( sGridNo, &sX, &sY );
		SetRecalculateWireFrameFlagRadius( sX, sY, ubRadius );
		CalculateWorldWireFrameTiles( FALSE );

		RecompileLocalMovementCostsInAreaWithFlags();
		RecompileLocalMovementCostsFromRadius( sGridNo, MAX_DISTANCE_EXPLOSIVE_CAN_DESTROY_STRUCTURES );

		// if anything has been done to change movement costs and this is a potential POW situation, check
		// paths for POWs
		if ( gWorldSectorX == 13 && gWorldSectorY == MAP_ROW_I )
		{
			DoPOWPathChecks();
		}

	}

	// do sight checks if something damaged or smoke stuff involved
	if ( fRecompileMovement || fSmokeEffect )
	{
		if ( gubElementsOnExplosionQueue )
		{
			gfExplosionQueueMayHaveChangedSight = TRUE;
		}
	}

	gsRecompileAreaTop = 0;
	gsRecompileAreaLeft = 0;
	gsRecompileAreaRight = 0;
	gsRecompileAreaBottom = 0;

	if (fAnyMercHit)
	{
		// reset explosion hit flag so we can damage mercs again
		for ( cnt = 0; cnt < (INT32)guiNumMercSlots; cnt++ )
		{
			if ( MercSlots[ cnt ] )
			{
				MercSlots[ cnt ]->ubMiscSoldierFlags &= ~SOLDIER_MISC_HURT_BY_EXPLOSION;
			}
		}
	}

	if ( fSubsequent != BLOOD_SPREAD_EFFECT )
	{
		MakeNoise( NOBODY, sGridNo, bLevel, gpWorldLevelData[sGridNo].ubTerrainID, (UINT8)Explosive[ Item [ usItem ].ubClassIndex ].ubVolume, NOISE_EXPLOSION );

	}

	gfMPDebugOutputRandoms = false;
}

void ToggleActionItemsByFrequency( INT8 bFrequency )
{
	UINT32	uiWorldBombIndex;
	OBJECTTYPE * pObj;

	// Go through all the bombs in the world, and look for remote ones
	for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
	{
		if (gWorldBombs[uiWorldBombIndex].fExists)
		{
			pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
			if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_REMOTE )
			{
				// Found a remote bomb, so check to see if it has the same frequency
				if ((*pObj)[0]->data.misc.bFrequency == bFrequency)
				{
					// toggle its active flag
					if ((*pObj).fFlags & OBJECT_DISABLED_BOMB)
					{
						(*pObj).fFlags &= (~OBJECT_DISABLED_BOMB);
					}
					else
					{
						(*pObj).fFlags |= OBJECT_DISABLED_BOMB;
					}
				}
			}
		}
	}
}

void TogglePressureActionItemsInGridNo( INT32 sGridNo )
{
	UINT32	uiWorldBombIndex;
	OBJECTTYPE * pObj;

	// Go through all the bombs in the world, and look for remote ones
	for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
	{
		if ( gWorldBombs[uiWorldBombIndex].fExists && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo == sGridNo )
		{
			pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
			if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_PRESSURE )
			{
				// Found a pressure item
				// toggle its active flag
				if ((*pObj).fFlags & OBJECT_DISABLED_BOMB)
				{
					(*pObj).fFlags &= (~OBJECT_DISABLED_BOMB);
				}
				else
				{
					(*pObj).fFlags |= OBJECT_DISABLED_BOMB;
				}
			}
		}
	}
}


void DelayedBillyTriggerToBlockOnExit( void )
{
	if ( WhoIsThere2( gsTempActionGridNo, 0 ) == NOBODY )
	{
		TriggerNPCRecord( BILLY, 6 );
	}
	else
	{
		// delay further!
		SetCustomizableTimerCallbackAndDelay( 1000, DelayedBillyTriggerToBlockOnExit, TRUE );
	}
}

void BillyBlocksDoorCallback( void )
{
	TriggerNPCRecord( BILLY, 6 );
}

BOOLEAN HookerInRoom( UINT16 usRoom )
{
	//DBrot: More Rooms
	UINT8		ubLoop;//, ubTempRoom;
	UINT16		usTempRoom;
	SOLDIERTYPE *	pSoldier;

	for ( ubLoop = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; ubLoop <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; ubLoop++ )
	{
		pSoldier = MercPtrs[ ubLoop ];

		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE && pSoldier->aiData.bNeutral && pSoldier->ubBodyType == MINICIV )
		{
			if ( InARoom( pSoldier->sGridNo, &usTempRoom ) && usTempRoom == usRoom )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

void PerformItemAction( INT32 sGridNo, OBJECTTYPE * pObj )
{

#ifdef LUA_EXPLOSION_CONTROL

	for (UINT32 i = ACTION_ITEM_OPEN_DOOR; i < NUM_ACTIONITEMS; /*= ACTION_ITEM_NEW;*/ i++ )
	{
		if ( (*pObj)[0]->data.misc.bActionValue == i ) 
			LetLuaPerformItemAction( i, sGridNo, 0 );
	}

#else

	STRUCTURE * pStructure;

	switch( (*pObj)[0]->data.misc.bActionValue )
	{
	case ACTION_ITEM_OPEN_DOOR:
		pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
		if (pStructure)
		{
			if (pStructure->fFlags & STRUCTURE_OPEN)
			{
				// it's already open - this MIGHT be an error but probably not
				// because we are basically just ensuring that the door is open
			}
			else
			{
				if (pStructure->fFlags & STRUCTURE_BASE_TILE)
				{
					HandleDoorChangeFromGridNo( NULL, sGridNo, FALSE );
				}
				else
				{
					HandleDoorChangeFromGridNo( NULL, pStructure->sBaseGridNo, FALSE );
				}
				gfExplosionQueueMayHaveChangedSight = TRUE;
			}
		}
		else
		{
			// error message here
#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Action item to open door in gridno %d but there is none!", sGridNo );
#endif
		}
		break;
	case ACTION_ITEM_CLOSE_DOOR:
		pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
		if (pStructure)
		{
			if (pStructure->fFlags & STRUCTURE_OPEN)
			{
				if (pStructure->fFlags & STRUCTURE_BASE_TILE)
				{
					HandleDoorChangeFromGridNo( NULL, sGridNo , FALSE );
				}
				else
				{
					HandleDoorChangeFromGridNo( NULL, pStructure->sBaseGridNo, FALSE );
				}
				gfExplosionQueueMayHaveChangedSight = TRUE;
			}
			else
			{
				// it's already closed - this MIGHT be an error but probably not
				// because we are basically just ensuring that the door is closed
			}
		}
		else
		{
			// error message here
#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Action item to close door in gridno %d but there is none!", sGridNo );
#endif
		}
		break;
	case ACTION_ITEM_TOGGLE_DOOR:
		pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
		if (pStructure)
		{
			if (pStructure->fFlags & STRUCTURE_BASE_TILE)
			{
				HandleDoorChangeFromGridNo( NULL, sGridNo, FALSE );
			}
			else
			{
				HandleDoorChangeFromGridNo( NULL, pStructure->sBaseGridNo , FALSE );
			}
			gfExplosionQueueMayHaveChangedSight = TRUE;
		}
		else
		{
			// error message here
#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Action item to toggle door in gridno %d but there is none!", sGridNo );
#endif
		}
		break;
	case ACTION_ITEM_UNLOCK_DOOR:
		{
			DOOR * pDoor;

			pDoor = FindDoorInfoAtGridNo( sGridNo );
			if ( pDoor )
			{
				pDoor->fLocked = FALSE;
			}
		}
		break;
	case ACTION_ITEM_TOGGLE_LOCK:
		{
			DOOR * pDoor;

			pDoor = FindDoorInfoAtGridNo( sGridNo );
			if ( pDoor )
			{
				if ( pDoor->fLocked )
				{
					pDoor->fLocked = FALSE;
				}
				else
				{
					pDoor->fLocked = TRUE;
				}
			}
		}
		break;
	case ACTION_ITEM_UNTRAP_DOOR:
		{
			DOOR * pDoor;

			pDoor = FindDoorInfoAtGridNo( sGridNo );
			if ( pDoor )
			{
				pDoor->ubTrapLevel = 0;
				pDoor->ubTrapID = NO_TRAP;
			}
		}
		break;
	case ACTION_ITEM_SMALL_PIT:
		Add3X3Pit( sGridNo );
		SearchForOtherMembersWithinPitRadiusAndMakeThemFall( sGridNo, 1 );
		break;
	case ACTION_ITEM_LARGE_PIT:
		Add5X5Pit( sGridNo );
		SearchForOtherMembersWithinPitRadiusAndMakeThemFall( sGridNo, 2 );
		break;
	case ACTION_ITEM_TOGGLE_ACTION1:
		ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 1 );
		break;
	case ACTION_ITEM_TOGGLE_ACTION2:
		ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 2 );
		break;
	case ACTION_ITEM_TOGGLE_ACTION3:
		ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 3 );
		break;
	case ACTION_ITEM_TOGGLE_ACTION4:
		ToggleActionItemsByFrequency( FIRST_MAP_PLACED_FREQUENCY + 4 );
		break;
	case ACTION_ITEM_TOGGLE_PRESSURE_ITEMS:
		TogglePressureActionItemsInGridNo( sGridNo );
		break;
	case ACTION_ITEM_ENTER_BROTHEL:
		// JA2Gold: Disable brothel tracking
		/*
		if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
		{
		UINT8	ubID;

		ubID = WhoIsThere2( sGridNo, 0 );
		if ( (ubID != NOBODY) && (MercPtrs[ ubID ]->bTeam == gbPlayerNum) )
		{
		if ( MercPtrs[ ubID ]->sOldGridNo == sGridNo + DirectionInc( SOUTH ) )
		{
		gMercProfiles[ MADAME ].bNPCData2++;

		SetFactTrue( FACT_PLAYER_USED_BROTHEL );
		SetFactTrue( FACT_PLAYER_PASSED_GOON );

		// If we for any reason trigger Madame's record 34 then we don't bother to do
		// anything else

		// Billy always moves back on a timer so that the player has a chance to sneak
		// someone else through

		// Madame's quote about female mercs should therefore not be made on a timer

		if ( gMercProfiles[ MADAME ].bNPCData2 > 2 )
		{
		// more than 2 entering brothel
		TriggerNPCRecord( MADAME, 35 );
		return;
		}

		if ( gMercProfiles[ MADAME ].bNPCData2 == gMercProfiles[ MADAME ].bNPCData )
		{
		// full # of mercs who paid have entered brothel
		// have Billy block the way again
		SetCustomizableTimerCallbackAndDelay( 2000, BillyBlocksDoorCallback, FALSE );
		//TriggerNPCRecord( BILLY, 6 );
		}
		else if ( gMercProfiles[ MADAME ].bNPCData2 > gMercProfiles[ MADAME ].bNPCData )
		{
		// more than full # of mercs who paid have entered brothel
		// have Billy block the way again?
		if ( CheckFact( FACT_PLAYER_FORCED_WAY_INTO_BROTHEL, 0 ) )
		{
		// player already did this once!
		TriggerNPCRecord( MADAME, 35 );
		return;
		}
		else
		{
		SetCustomizableTimerCallbackAndDelay( 2000, BillyBlocksDoorCallback, FALSE );
		SetFactTrue( FACT_PLAYER_FORCED_WAY_INTO_BROTHEL );
		TriggerNPCRecord( MADAME, 34 );
		}
		}

		if ( gMercProfiles[ MercPtrs[ ubID ]->ubProfile ].bSex == FEMALE )
		{
		// woman walking into brothel
		TriggerNPCRecordImmediately( MADAME, 33 );
		}

		}
		else
		{
		// someone wants to leave the brothel
		TriggerNPCRecord( BILLY, 5 );
		}

		}

		}
		*/
		break;
	case ACTION_ITEM_EXIT_BROTHEL:
		// JA2Gold: Disable brothel tracking
		/*
		if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
		{
		UINT8	ubID;

		ubID = WhoIsThere2( sGridNo, 0 );
		if ( (ubID != NOBODY) && (MercPtrs[ ubID ]->bTeam == gbPlayerNum) && MercPtrs[ ubID ]->sOldGridNo == sGridNo + DirectionInc( NORTH ) )
		{
		gMercProfiles[ MADAME ].bNPCData2--;
		if ( gMercProfiles[ MADAME ].bNPCData2 == 0 )
		{
		// reset paid #
		gMercProfiles[ MADAME ].bNPCData = 0;
		}
		// Billy should move back to block the door again
		gsTempActionGridNo = sGridNo;
		SetCustomizableTimerCallbackAndDelay( 1000, DelayedBillyTriggerToBlockOnExit, TRUE );
		}
		}
		*/
		break;
	case ACTION_ITEM_KINGPIN_ALARM:
		PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 5, SoundDir( sGridNo ) );
		CallAvailableKingpinMenTo( sGridNo );

		gTacticalStatus.fCivGroupHostile[ KINGPIN_CIV_GROUP ] = CIV_GROUP_HOSTILE;

		{
			UINT8	ubID, ubID2;
			BOOLEAN fEnterCombat = FALSE;

			for ( ubID = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; ubID <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; ubID++ )
			{
				if ( MercPtrs[ ubID ]->bActive && MercPtrs[ ubID ]->bInSector && MercPtrs[ ubID ]->ubCivilianGroup == KINGPIN_CIV_GROUP )
				{
					for ( ubID2 = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; ubID2 <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubID2++ )
					{
						if ( MercPtrs[ ubID ]->aiData.bOppList[ ubID2 ] == SEEN_CURRENTLY )
						{
							MakeCivHostile( MercPtrs[ ubID ], 2 );
							fEnterCombat = TRUE;
						}
					}
				}
			}

			if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
			{
				EnterCombatMode( CIV_TEAM );
			}
		}

		// now zap this object so it won't activate again
		(*pObj).fFlags &= (~OBJECT_DISABLED_BOMB);
		break;
	case ACTION_ITEM_SEX:
		// JA2Gold: Disable brothel sex Madd: Re-enabled

		if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			UINT8	ubID;
			OBJECTTYPE DoorCloser;
			INT16	sTeleportSpot;
			INT16	sDoorSpot;
			UINT8	ubDirection;
			//DBrot: More Rooms
			//UINT8	ubRoom, ubOldRoom;
			UINT16 usRoom, usOldRoom;

			ubID = WhoIsThere2( sGridNo, 0 );
			if ( (ubID != NOBODY) && (MercPtrs[ ubID ]->bTeam == gbPlayerNum) )
			{
				if ( InARoom( sGridNo, &usRoom ) && InARoom( MercPtrs[ ubID ]->sOldGridNo, &usOldRoom ) && usOldRoom != usRoom )
				{
					// also require there to be a miniskirt civ in the room
					if ( HookerInRoom( usRoom ) )
					{

						// stop the merc...
						MercPtrs[ ubID ]->EVENT_StopMerc( MercPtrs[ ubID ]->sGridNo, MercPtrs[ ubID ]->ubDirection );

						switch( sGridNo )
						{
						case 13414:
							sDoorSpot = 13413;
							sTeleportSpot = 13413;
							break;
						case 11174:
							sDoorSpot = 11173;
							sTeleportSpot = 11173;
							break;
						case 12290:
							sDoorSpot = 12290;
							sTeleportSpot = 12291;
							break;

						default:

							sDoorSpot = NOWHERE;
							sTeleportSpot = NOWHERE;


						}
						
						if (!TileIsOutOfBounds(sDoorSpot) && !TileIsOutOfBounds(sTeleportSpot) )
						{
							// close the door...
							DoorCloser[0]->data.misc.bActionValue = ACTION_ITEM_CLOSE_DOOR;
							PerformItemAction( sDoorSpot, &DoorCloser );

							// have sex
							HandleNPCDoAction( 0, NPC_ACTION_SEX, 0 );

							// move the merc outside of the room again
							sTeleportSpot = FindGridNoFromSweetSpotWithStructData( MercPtrs[ ubID ], STANDING, sTeleportSpot, 2, &ubDirection, FALSE );
							MercPtrs[ ubID ]->ChangeSoldierState( STANDING, 0, TRUE );
							TeleportSoldier( MercPtrs[ ubID ], sTeleportSpot, FALSE );

							HandleMoraleEvent( MercPtrs[ ubID ], MORALE_SEX, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
							FatigueCharacter( MercPtrs[ ubID ] );
							FatigueCharacter( MercPtrs[ ubID ] );
							FatigueCharacter( MercPtrs[ ubID ] );
							FatigueCharacter( MercPtrs[ ubID ] );
							DirtyMercPanelInterface( MercPtrs[ ubID ], DIRTYLEVEL1 );
						}
					}

				}
				break;

			}
		}

		break;
	case ACTION_ITEM_REVEAL_ROOM:
		{
			//DBrot: More Rooms
			//UINT8 ubRoom;
			UINT16 usRoom;
			if ( InAHiddenRoom( sGridNo, &usRoom ) )
			{
				RemoveRoomRoof( sGridNo, usRoom, NULL );
			}
		}
		break;
	case ACTION_ITEM_LOCAL_ALARM:
		MakeNoise( NOBODY, sGridNo, 0, gpWorldLevelData[sGridNo].ubTerrainID, 30, NOISE_SILENT_ALARM );
		break;
	case ACTION_ITEM_GLOBAL_ALARM:
		CallAvailableEnemiesTo( sGridNo );
		break;
	case ACTION_ITEM_BLOODCAT_ALARM:
		CallAvailableTeamEnemiesTo( sGridNo, CREATURE_TEAM );
		break;
	case ACTION_ITEM_KLAXON:
		PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 5, SoundDir( sGridNo ) );
		break;
	case ACTION_ITEM_MUSEUM_ALARM:
		PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 5, SoundDir( sGridNo ) );
		CallEldinTo( sGridNo );
		break;
#ifdef JA2UB
		case ACTION_ITEM_BIGGENS_BOMBS:

			if( ShouldThePlayerStopWhenWalkingOnBiggensActionItem( 17 ) )
			{
				HavePersonAtGridnoStop( sGridNo );

				//Make Biggens run for cover and then detonate the explosives
				TriggerNPCRecord( BIGGENS_UB , 17 );  //BIGGENS
			}

			break;
		case ACTION_ITEM_BIGGENS_WARNING:
			
			if( ShouldThePlayerStopWhenWalkingOnBiggensActionItem( 16 ) )
			{
				HavePersonAtGridnoStop( sGridNo );

				//Have Biggens spit out a warning about the bombs
				TriggerNPCRecord( BIGGENS_UB, 16 ); //BIGGENS
			}
			
			break;

		case ACTION_ITEM_SEE_FORTIFIED_DOOR:
			HandleSeeingFortifiedDoor( sGridNo );
			break;

		case ACTION_ITEM_OPEN_FORTIFED_DOOR:
			HandleSwitchToOpenFortifiedDoor( sGridNo );
			break;

		case ACTION_ITEM_SEE_POWER_GEN_FAN:
			
			//if the player is in the power plant
			if( gWorldSectorX == 13 && gWorldSectorY == 10 && gbWorldSectorZ == 0 )
			{
				HandleSeeingPowerGenFan( sGridNo );
			}
			else if( gWorldSectorX == 15 && gWorldSectorY == 12 && gbWorldSectorZ == 3 )
			{
				//The player is hitting the switch to launch the missles
				HandlePlayerHittingSwitchToLaunchMissles();
			}
			
			break;
#endif
	default:
		// error message here
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Action item with invalid action in gridno %d!", sGridNo );
#endif
		break;
	}
	
#endif


}

void AddBombToQueue( UINT32 uiWorldBombIndex, UINT32 uiTimeStamp, BOOL fFromRemoteClient )
{
	if (gubElementsOnExplosionQueue == MAX_BOMB_QUEUE)
	{
		return;
	}

	// 20091002 - OJW - MP Explosives
	if (is_networked && is_client)
	{
		/*if (gWorldBombs[uiWorldBombIndex].bIsFromRemotePlayer && !fFromRemoteClient)
		{
			return; 
		}
		else
		{
			// this is the world item index
			UINT32 iWorldIndex = gWorldBombs[uiWorldBombIndex].iItemIndex;
			WORLDITEM wi = gWorldItems[iWorldIndex];
			if (wi.fExists)
			{
				INT8 soldierID = wi.soldierID;
				if (soldierID == -1)
					soldierID = wi.object[0]->data.misc.ubBombOwner - 2; // undo the hack

				send_detonate_explosive(iWorldIndex,soldierID);
			}
		}*/
		UINT32 iWorldIndex = gWorldBombs[uiWorldBombIndex].iItemIndex;
		WORLDITEM wi = gWorldItems[iWorldIndex];
		if (wi.fExists)
		{
			INT8 soldierID = wi.soldierID; // bomb's owner
			if (soldierID == -1)
				soldierID = wi.object[0]->data.misc.ubBombOwner - 2; // undo the hack

			if (IsOurSoldier(gubPersonToSetOffExplosions) || IsOurSoldier(soldierID))
			{
				// we set off the bomb (could be failed disarm) or we own it, tell the other clients we are setting it off
				send_detonate_explosive(iWorldIndex,gubPersonToSetOffExplosions);
			}
			else if (gWorldBombs[uiWorldBombIndex].bIsFromRemotePlayer && !fFromRemoteClient)
			{
				return; // dont explode bombs which arent originating from our client unless we were told to
			}
		}
	}

	gExplosionQueue[gubElementsOnExplosionQueue].uiWorldBombIndex = uiWorldBombIndex;
	gExplosionQueue[gubElementsOnExplosionQueue].uiTimeStamp = uiTimeStamp;
	gExplosionQueue[gubElementsOnExplosionQueue].fExists = TRUE;
	if (!gfExplosionQueueActive)
	{
		// lock UI
		guiPendingOverrideEvent = LU_BEGINUILOCK;
		// disable sight
		gTacticalStatus.uiFlags |= DISALLOW_SIGHT;
	}
	gubElementsOnExplosionQueue++;
	gfExplosionQueueActive = TRUE;
}

// Flugente: activate everything connected to a tripwire in the surrounding if sGridNo on level bLevel with regard to the tripwire netwrok and hierarchy determined by ubFlag
BOOLEAN ActivateSurroundingTripwire( UINT8 ubID, INT32 sGridNo, INT8 bLevel, UINT32 ubFlag )
{
	UINT32	uiTimeStamp= GetJA2Clock();
	BOOLEAN	fFoundMine = FALSE;
		
	UINT8 feasibletripwiredirections[4] =
	{
		NORTH,
		EAST,
		SOUTH,
		WEST
	};

	// for every orientation
	for (UINT8 i = 0; i < 4; ++i)
	{
		UINT8 direction = feasibletripwiredirections[i];
		// get adjacent grid
		UINT32 adjgrid = NewGridNo( sGridNo, DirectionInc( direction ) );
		
		// if there is a bomb at that grid and level, and it isn't disabled
		for (UINT32 uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
		{
			if (gWorldBombs[uiWorldBombIndex].fExists && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo == adjgrid && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].ubLevel == bLevel )
			{
				OBJECTTYPE* pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
				if (!((*pObj).fFlags & OBJECT_DISABLED_BOMB))
				{
					// Flugente: we can either activate a bomb, or an action item that simulates a bomb - we have to check that first
					UINT16 usBombItem = pObj->usItem;
					if ( usBombItem == ACTION_ITEM )
						usBombItem = (*pObj)[0]->data.misc.usBombItem;

					// if item can be activated by tripwire, detonate it
					if ( Item[usBombItem].tripwireactivation == 1 )
					{
						// tripwire just gets activated
						if ( Item[usBombItem].tripwire == 1 )
						{
							// this is important - we have to check wether the wire has already been activated 
							if ( ( (*pObj)[0]->data.sObjectFlag & TRIPWIRE_ACTIVATED ) == 0 )
							{
								// determine this tripwire's flag
								UINT32 ubWireNetworkFlag = (*pObj)[0]->data.ubWireNetworkFlag;

								// check if a) tripwire belongs to the same tripwire network and b) its of the same or lower hierarchy level
								BOOLEAN samenetwork = FALSE;
								BOOLEAN sameorlowerhierarchy = FALSE;

								// the biggest flags are the hierarchy flags, so a simple comparison is enough here
								if ( ubWireNetworkFlag <= ubFlag )
									sameorlowerhierarchy = TRUE;
							
								if ( !sameorlowerhierarchy )
									continue;

								// test: do the 2 wires belong to the same 'side'?
								if ( ( (ubFlag & TRIPWIRE_NETWORK_OWNER_ENEMY) && (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_ENEMY) ) || ( (ubFlag & TRIPWIRE_NETWORK_OWNER_PLAYER) && (ubWireNetworkFlag & TRIPWIRE_NETWORK_OWNER_PLAYER) ) )
								{
									if ( ( (ubFlag & TRIPWIRE_NETWORK_NET_1) && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_1) ) ||
										( (ubFlag & TRIPWIRE_NETWORK_NET_2) && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_2) ) ||
										( (ubFlag & TRIPWIRE_NETWORK_NET_3) && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_3) ) ||
										( (ubFlag & TRIPWIRE_NETWORK_NET_4) && (ubWireNetworkFlag & TRIPWIRE_NETWORK_NET_4) ) )
										samenetwork = TRUE;
									else
										continue;
								}
								else
									continue;

								if ( samenetwork && sameorlowerhierarchy )
								{
									gubPersonToSetOffExplosions = ubID;

									/*if (pObj->usItem != ACTION_ITEM || (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP)
									{
										uiTimeStamp += BOMB_QUEUE_DELAY;
									}*/

									// put this bomb on the queue
									AddBombToQueue( uiWorldBombIndex, uiTimeStamp );
															
									if ( (*pObj)[0]->data.misc.usBombItem != NOTHING && Item[ (*pObj)[0]->data.misc.usBombItem ].usItemClass & IC_EXPLOSV )
									{
										fFoundMine = TRUE;
									}
																
									// Flugente hack: the tripwire will get removed in HandleExplosionQueue (it is still needed in there until the bomb gets called). 
									// Because of this, ActivateSurroundingTripwire would normally find this wire again, thus a loop would occur.
									// So we mark the wire with this flag
									(*pObj)[0]->data.sObjectFlag |= TRIPWIRE_ACTIVATED;

									// activate surrounding tripwires, unless tripwire is too much damaged and we are unlucky.. 
									if ( (*pObj)[0]->data.objectStatus > (INT16)Random(50) )
										fFoundMine = ActivateSurroundingTripwire(ubID, adjgrid, bLevel, ubWireNetworkFlag);
								}
							}
						}
						// bombs go off
						else
						{
							gubPersonToSetOffExplosions = ubID;

							// SANDRO - merc records
							// only if we blew up somebody not in our team(no achievement for blowing our guys :)), only if owner exists and have profile
							if ( (MercPtrs[ubID]->bTeam != gbPlayerNum) && ((*pObj)[0]->data.misc.ubBombOwner > 1) )
							{
								if ( MercPtrs[ ((*pObj)[0]->data.misc.ubBombOwner - 2) ]->ubProfile != NO_PROFILE && MercPtrs[ ((*pObj)[0]->data.misc.ubBombOwner - 2) ]->bTeam == gbPlayerNum ) 
									gMercProfiles[ MercPtrs[ ((*pObj)[0]->data.misc.ubBombOwner - 2) ]->ubProfile ].records.usExpDetonated++;
							}

							/*if (pObj->usItem != ACTION_ITEM || (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP)
							{
								uiTimeStamp += BOMB_QUEUE_DELAY;
							}*/

							// put this bomb on the queue
							AddBombToQueue( uiWorldBombIndex, uiTimeStamp );
							
							if ( (*pObj)[0]->data.misc.usBombItem != NOTHING && Item[ (*pObj)[0]->data.misc.usBombItem ].usItemClass & IC_EXPLOSV )
							{
								fFoundMine = TRUE;
							}
						}
					}
				}
			}
		}
	}


	return( fFoundMine );
}

// Flugente: A special function for tripwire gun traps. Search if pObj has a gun attached. If so, fire a shot from that gun in a specific direction. Afterwards place the gun on the ground
void CheckAndFireTripwireGun( OBJECTTYPE* pObj, INT32 sGridNo, INT8 bLevel, UINT8 ubId, UINT8 ubDirection )
{
	if ( !pObj )
		return;

	// we actually found a gun.
	// if the gun has ammo and is not jammed, fire it
	if ( (*pObj)[0]->data.gun.ubGunShotsLeft > 0 && (*pObj)[0]->data.gun.bGunAmmoStatus > 0 )
	{
		// Increment attack counter...
		if (gubElementsOnExplosionQueue == 0)
		{
			// single explosion, disable sight until the end, and set flag
			// to check sight at end of attack

			gTacticalStatus.uiFlags |= (DISALLOW_SIGHT | CHECK_SIGHT_AT_END_OF_ATTACK);
		}

		FireFragmentsTrapGun( MercPtrs[ubId], sGridNo, 0, pObj, ubDirection );

		// this is important... if not set, the game will remain in a loop
		gTacticalStatus.ubAttackBusyCount = 0;
	}
				
	// add this gun to the floor
	AddItemToPool( sGridNo, pObj, 1, bLevel, 0, -1 );
}

void HandleExplosionQueue( void )
{
	UINT32	uiIndex;
	UINT32	uiWorldBombIndex;
	UINT32	uiCurrentTime;
	INT32 sGridNo;
	OBJECTTYPE * pObj;
	UINT8		 ubLevel;
	BOOLEAN fAttFound = FALSE;

	if ( !gfExplosionQueueActive )
	{
		return;
	}

	uiCurrentTime = GetJA2Clock();
	// WDS 07/25/2008 - Avoid error where gWorldItems and/or gWorldBombs is nil
	if (gWorldBombs && !gWorldItems.empty()) {//dnl ch75 271013
	for ( uiIndex = 0; uiIndex < gubElementsOnExplosionQueue; uiIndex++ )
	{
		if ( gExplosionQueue[ uiIndex ].fExists && uiCurrentTime >= gExplosionQueue[ uiIndex ].uiTimeStamp )
		{
			// Set off this bomb now!

			// Preliminary assignments:
			uiWorldBombIndex = gExplosionQueue[ uiIndex ].uiWorldBombIndex;
			pObj = &( gWorldItems[ gWorldBombs[ uiWorldBombIndex ].iItemIndex ].object );
			sGridNo = gWorldItems[ gWorldBombs[ uiWorldBombIndex ].iItemIndex ].sGridNo;
			ubLevel = gWorldItems[ gWorldBombs[ uiWorldBombIndex ].iItemIndex ].ubLevel;

			if (pObj->usItem == ACTION_ITEM && (*pObj)[0]->data.misc.bActionValue != ACTION_ITEM_BLOW_UP)
			{
				PerformItemAction( sGridNo, pObj );
			}
			else if ( (*pObj)[0]->data.misc.usBombItem == TRIP_KLAXON )
			{
				PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 5, SoundDir( sGridNo ) );
				CallAvailableEnemiesTo( sGridNo );
				//RemoveItemFromPool( sGridNo, gWorldBombs[ uiWorldBombIndex ].iItemIndex, 0 );
			}
			else if ( (*pObj)[0]->data.misc.usBombItem == TRIP_FLARE )
			{				
				// sevenfm: changed pObj->usItem to Item[pObj->usItem].ubClassIndex as it should be correct explosives index
				// NewLightEffect( sGridNo, (UINT8)Explosive[pObj->usItem].ubDuration, (UINT8)Explosive[pObj->usItem].ubStartRadius );
				NewLightEffect( sGridNo, (UINT8)Explosive[ Item[pObj->usItem].ubClassIndex ].ubDuration, (UINT8)Explosive[ Item[pObj->usItem].ubClassIndex ].ubStartRadius );

				RemoveItemFromPool( sGridNo, gWorldBombs[ uiWorldBombIndex ].iItemIndex, ubLevel );
			}
			// Flugente: handle tripwire gun traps here...
			// tripwire gets called and activated in ActivateSurroundingTripwire
			else if ( Item[pObj->usItem].tripwire == 1 )
			{
				OBJECTTYPE newtripwireObject;
				CreateItem( pObj->usItem, (*pObj)[0]->data.objectStatus, &newtripwireObject );
				
				// search for attached guns
				BOOLEAN fgunfound = FALSE;
				OBJECTTYPE* pAttGun = NULL;
				// check all attachments
				attachmentList::iterator iterend = (*pObj)[0]->attachments.end();
				for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
				{
					if ( iter->exists() && Item[iter->usItem].usItemClass == IC_GUN )
					{
						fgunfound = TRUE;
						pAttGun = &(*iter);
						break;
					}
				}

				if ( fgunfound && pAttGun && pAttGun->exists() )
				{
					// Flugente 2012-06-16: I ran into a weird bug. If activating multiple guntraps, an error would occur when placing a gun on the floor. 
					// The problem is the function GetFreeWorldItemIndex( ) in AddItemToWorld() in World Items.cpp.
					// It gets a free index for the item that should be placed. If there is no free index, it creates a bigger array, copies the current array into it, and then deletes the old index.
					// For some reason still unknown to me, this corrupts the pObj-pointer. It also corrupts the pAttGun-pointer, this is the reason I create a new object.
					// I had to move some functions here to take care of that. It would be good if someone with more knowledge could take a look at this.

					// fire with this gun, if possible. Afterwards place it on the floor
					OBJECTTYPE object(*pAttGun);
					CheckAndFireTripwireGun( &object, sGridNo, ubLevel, (*pObj)[0]->data.misc.ubBombOwner, (*pObj)[0]->data.ubDirection );
				} else {
					// sevenfm: blow attached items with tripwireactivation = TRUE
					// no preplaced (owner=NOBODY) tripwire with explosive attachments allowed
					if ( gGameExternalOptions.bAllowExplosiveAttachments && (*pObj)[0]->data.misc.ubBombOwner > 1 )
					{
						fAttFound=HandleAttachedExplosions( (UINT8) ((*pObj)[0]->data.misc.ubBombOwner - 2), CenterX( sGridNo ), CenterY( sGridNo ), 0, 
										sGridNo, (*pObj)[0]->data.misc.usBombItem, FALSE, ubLevel, (*pObj)[0]->data.ubDirection, pObj);
				}
				}
				
				// this is important: delete the tripwire, otherwise we get into an infinite loop if there are two piecs of tripwire....
				RemoveItemFromPool( sGridNo, gWorldBombs[ uiWorldBombIndex ].iItemIndex, ubLevel );
						
				// if no other bomb exists here
				CheckForBuriedBombsAndRemoveFlags( sGridNo, ubLevel );
				
				// delete the flag, otherwise wire will only work once
				(newtripwireObject)[0]->data.sObjectFlag &= ~TRIPWIRE_ACTIVATED;

				// now add a tripwire item to the floor, simulating that activating tripwire deactivates it
				AddItemToPool( sGridNo, &newtripwireObject, 1, ubLevel, 0, -1 );
			}
			// Flugente: handle tripwire gun traps here...
			// tripwire gets called and activated in ActivateSurroundingTripwire
			// now for action item tripwire
			else if ( pObj->usItem == ACTION_ITEM && (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP && Item[(*pObj)[0]->data.misc.usBombItem].tripwire == 1 )
			{
				OBJECTTYPE newtripwireObject;
				CreateItem( (*pObj)[0]->data.misc.usBombItem, (*pObj)[0]->data.objectStatus, &newtripwireObject );
																
				// this is important: delete the tripwire, otherwise we get into an infinite loop if there are two piecs of tripwire....
				RemoveItemFromPool( sGridNo, gWorldBombs[ uiWorldBombIndex ].iItemIndex, ubLevel );
						
				// if no other bomb exists here
				CheckForBuriedBombsAndRemoveFlags( sGridNo, ubLevel );
				
				// delete the flag, otherwise wire will only work once
				(newtripwireObject)[0]->data.sObjectFlag &= ~TRIPWIRE_ACTIVATED;

				// now add a tripwire item to the floor, simulating that activating tripwire deactivates it
				AddItemToPool( sGridNo, &newtripwireObject, 1, ubLevel, 0, -1 );
			}
			else if ( (*pObj)[0]->data.ubWireNetworkFlag & ANY_ARTILLERY_FLAG )
			{
				UINT8 cnt = 0;
				if ( (*pObj)[0]->data.ubWireNetworkFlag & ARTILLERY_STRIKE_COUNT_1 )	cnt += 1;
				if ( (*pObj)[0]->data.ubWireNetworkFlag & ARTILLERY_STRIKE_COUNT_2 )	cnt += 2;
				if ( (*pObj)[0]->data.ubWireNetworkFlag & ARTILLERY_STRIKE_COUNT_4 )	cnt += 4;

					// determine gridno to attack - smoke signal required. Otherwise, it is assumed the radio operator ordered the bombing of his OWN position
					// if we cannot even find a radio operator, all bets are off - target a random gridno
					// the usual +/- 2 shenanigans
					UINT16 usOwner = NOBODY;
					if ( (*pObj)[0]->data.misc.ubBombOwner > 1 )
						usOwner = (*pObj)[0]->data.misc.ubBombOwner - 2;

					INT32 sTargetGridNo = NOWHERE;
					if ( GetRandomSignalSmokeGridNo( &sTargetGridNo ) || GetRadioOperatorSignal( usOwner, &sTargetGridNo ) || (sTargetGridNo = RandomGridNo( )) )
					{
						for ( UINT8 i = 0; i < cnt; ++i)
							ArtilleryStrike( pObj->usItem, usOwner, sGridNo, sTargetGridNo );
					}

					// not needed anymore
				RemoveItemFromPool( sGridNo, gWorldBombs[ uiWorldBombIndex ].iItemIndex, ubLevel );
			}
			else
			{
				gfExplosionQueueMayHaveChangedSight = TRUE;

				// We have to remove the item first to prevent the explosion from detonating it
				// a second time :-)
				RemoveItemFromPool( sGridNo, gWorldBombs[ uiWorldBombIndex ].iItemIndex, ubLevel );

				// make sure no one thinks there is a bomb here any more!
				CheckForBuriedBombsAndRemoveFlags( sGridNo, ubLevel);
				// BOOM!

				// bomb objects only store the SIDE who placed the bomb! :-(
				if ( (*pObj)[0]->data.misc.ubBombOwner > 1 )
				{
					IgniteExplosion( (UINT8) ((*pObj)[0]->data.misc.ubBombOwner - 2), CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, (*pObj)[0]->data.misc.usBombItem, ubLevel, (*pObj)[0]->data.ubDirection, pObj);
				}
				else
				{
					// pre-placed
					IgniteExplosion( NOBODY, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, (*pObj)[0]->data.misc.usBombItem, ubLevel, (*pObj)[0]->data.ubDirection );
				}
			}
/*			if ( FindWorldItemForBuriedBombInGridNo(sGridNo, ubLevel) != -1 )
			{
				gpWorldLevelData[sGridNo].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;
				gpWorldLevelData[sGridNo].uiFlags |= MAPELEMENT_ENEMY_MINE_PRESENT;
			}*/

			// Bye bye bomb
			gExplosionQueue[ uiIndex ].fExists = FALSE;
		}
	}
	}

	// See if we can reduce the # of elements on the queue that we have recorded
	// Easier to do it at this time rather than in the loop above
	while ( gubElementsOnExplosionQueue > 0 && gExplosionQueue[ gubElementsOnExplosionQueue - 1 ].fExists == FALSE )
	{
		gubElementsOnExplosionQueue--;
	}

	if ( gubElementsOnExplosionQueue == 0 && (gubPersonToSetOffExplosions == NOBODY || gTacticalStatus.ubAttackBusyCount == 0) )
	{
		// turn off explosion queue

		// re-enable sight
		gTacticalStatus.uiFlags &= (~DISALLOW_SIGHT);

		if ( gubPersonToSetOffExplosions != NOBODY && !(MercPtrs[ gubPersonToSetOffExplosions ]->flags.uiStatusFlags & SOLDIER_PC) )
		{
			FreeUpNPCFromPendingAction( MercPtrs[ gubPersonToSetOffExplosions ] );
		}

		if (gfExplosionQueueMayHaveChangedSight)
		{
			UINT8 ubLoop;
			SOLDIERTYPE * pTeamSoldier;

			// set variable so we may at least have someone to resolve interrupts vs
			gubInterruptProvoker = gubPersonToSetOffExplosions;
			AllTeamsLookForAll( TRUE );

			// call fov code
			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive && pTeamSoldier->bInSector )
				{
					RevealRoofsAndItems( pTeamSoldier, TRUE, FALSE, pTeamSoldier->pathing.bLevel, FALSE );
				}
			}

			gfExplosionQueueMayHaveChangedSight = FALSE;
			gubPersonToSetOffExplosions = NOBODY;
		}

		// unlock UI
		//UnSetUIBusy( (UINT8)gusSelectedSoldier );
		// OJW - 20091028 - fix explosion UI lock bug on unoriginating clients
		if ( !(gTacticalStatus.uiFlags & INCOMBAT) || gTacticalStatus.ubCurrentTeam == gbPlayerNum || (is_networked && gTacticalStatus.ubCurrentTeam != 1) )
		{
			// don't end UI lock when it's a computer turn
			guiPendingOverrideEvent = LU_ENDUILOCK;
		}

		gfExplosionQueueActive = FALSE;
	}

}

void DecayBombTimers( void )
{
	UINT32	uiWorldBombIndex;
	UINT32	uiTimeStamp;
	OBJECTTYPE * pObj;

	uiTimeStamp = GetJA2Clock();
		
	// Go through all the bombs in the world, and look for timed ones
	for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
	{
		if (gWorldBombs[uiWorldBombIndex].fExists)
		{
			pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
			if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_TIMED && !((*pObj).fFlags & OBJECT_DISABLED_BOMB) )
			{
				// Found a timed bomb, so decay its delay value and see if it goes off
				(*pObj)[0]->data.misc.bDelay--;
				if ((*pObj)[0]->data.misc.bDelay == 0)
				{
					// ATE: CC black magic....
					if ( (*pObj)[0]->data.misc.ubBombOwner > 1 )
					{
						gubPersonToSetOffExplosions = (UINT8) ((*pObj)[0]->data.misc.ubBombOwner - 2);
						// SANDRO - merc records - detonating explosives
						if ( MercPtrs[ gubPersonToSetOffExplosions ]->ubProfile != NO_PROFILE && MercPtrs[ gubPersonToSetOffExplosions ]->bTeam == gbPlayerNum )
						{
							gMercProfiles[ MercPtrs[ gubPersonToSetOffExplosions ]->ubProfile ].records.usExpDetonated++;
						}
					}
					else
					{
						gubPersonToSetOffExplosions = NOBODY;
					}

					
					// put this bomb on the queue
					AddBombToQueue( uiWorldBombIndex, uiTimeStamp );

					if (pObj->usItem != ACTION_ITEM || (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP)
					{
						uiTimeStamp += BOMB_QUEUE_DELAY;
					}
				}
			}
		}
	}

	// Flugente: we have to check every inventory for armed bombs and do the countdown for them, too
	// Flugente: new stuff: we can now also arm bombs in our inventory, and detonate/defuse those bombs remotely
	// So we have to look at every item in every inventory in this sector
	for (UINT32 cnt = 0; cnt < guiNumMercSlots; ++cnt )
	{
		SOLDIERTYPE* pSoldier = MercSlots[ cnt ];

		if ( pSoldier != NULL )
		{
			if ( pSoldier->bInSector && pSoldier->bActive )
			{
				INT8 invsize = (INT8)pSoldier->inv.size();								// remember inventorysize, so we don't call size() repeatedly
							  
				for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)							// ... for all items in our inventory ...
			    {
					// ... if Item is a bomb ...
					if (pSoldier->inv[bLoop].exists() == true && ( Item[pSoldier->inv[bLoop].usItem].usItemClass & (IC_BOMB|IC_GRENADE) ) )
					{
						OBJECTTYPE * pObj = &(pSoldier->inv[bLoop]);					// ... get pointer for this item ...

						if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_TIMED )
						{
							// Found a timed bomb, so decay its delay value and see if it goes off
							(*pObj)[0]->data.misc.bDelay--;
							if ((*pObj)[0]->data.misc.bDelay == 0)
							{
								// ATE: CC black magic....
								if ( (*pObj)[0]->data.misc.ubBombOwner > 1 )
								{
									gubPersonToSetOffExplosions = (UINT8) ((*pObj)[0]->data.misc.ubBombOwner - 2);
									// SANDRO - merc records - detonating explosives
									if ( MercPtrs[ gubPersonToSetOffExplosions ]->ubProfile != NO_PROFILE && MercPtrs[ gubPersonToSetOffExplosions ]->bTeam == gbPlayerNum )
									{
										gMercProfiles[ MercPtrs[ gubPersonToSetOffExplosions ]->ubProfile ].records.usExpDetonated++;
									}
								}
								else
								{
									gubPersonToSetOffExplosions = NOBODY;
								}

								// ignite explosions manually - this item is not in the WorldBombs-structure, so we can't add it to the queue
								IgniteExplosion( gubPersonToSetOffExplosions, pSoldier->sX, pSoldier->sY, (INT16) (gpWorldLevelData[pSoldier->sGridNo].sHeight), pSoldier->sGridNo, pObj->usItem, pSoldier->pathing.bLevel, pSoldier->ubDirection, pObj );

								DeleteObj( pObj );
							}
						}
					}
				}
			}
		}
	}		
}

void SetOffBombsByFrequency( UINT8 ubID, INT8 bFrequency )
{
	UINT32	uiWorldBombIndex;
	UINT32	uiTimeStamp;
	OBJECTTYPE * pObj;

	uiTimeStamp = GetJA2Clock();

	// Flugente: The remote detonator can now detonate _or_ defuse bombs. 
	// In order for a bomb do be detonated, it must have a remote detonator attached. If a 'detonate' command was clicked, bFrequency will be out of 1 - 4.
	// In order for a bomb do be detonated, it must have a remote defuse attached. If a 'defuse' command was clicked, bFrequency will be out of 5 - 8.
	BOOLEAN fDetonate = TRUE;

	// if bFrequency is in 5-8, it must have been a 'defuse' command. Change for correct frequency;
	if ( bFrequency > 4  && bFrequency < 9)
	{
		fDetonate = FALSE;
		bFrequency -= 4;
	}

	// Go through all the bombs in the world, and look for remote ones
	for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
	{
		if (gWorldBombs[uiWorldBombIndex].fExists)
		{
			pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
			if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_REMOTE && !((*pObj).fFlags & OBJECT_DISABLED_BOMB) )
			{
				// Found a remote bomb, so check to see if it has the same frequency
				if ( fDetonate )	// detonate bombs
				{
					// Found a remote bomb, so check to see if it has the same frequency
					if ((*pObj)[0]->data.misc.bFrequency == bFrequency)
					{					
						// SANDRO - added merc records and some exp
						if ( ((*pObj)[0]->data.misc.ubBombOwner) > 1 )
						{
							if ( MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)]->ubProfile != NO_PROFILE &&
								MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)]->bTeam == gbPlayerNum )
							{
								gMercProfiles[MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)]->ubProfile].records.usExpDetonated++;

								StatChange( MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)], EXPLODEAMT, ( 5 ), FALSE );					
							}
						}

						gubPersonToSetOffExplosions = ubID;

						// put this bomb on the queue
						AddBombToQueue( uiWorldBombIndex, uiTimeStamp );
						if (pObj->usItem != ACTION_ITEM || (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP)
						{
							uiTimeStamp += BOMB_QUEUE_DELAY;
						}
					}
				}
				else	// defuse bombs
				{
					// check for frequency
					if ((*pObj)[0]->data.bDefuseFrequency == bFrequency)
					{
						// check for a defuse
						if ( HasAttachmentOfClass(pObj, AC_DEFUSE) )
						{
							(*pObj)[0]->data.bTrap = 0;

							if ( (*pObj).fFlags & OBJECT_KNOWN_TO_BE_TRAPPED )
								pObj->fFlags &= ~( OBJECT_KNOWN_TO_BE_TRAPPED );

							if ( (*pObj).fFlags & OBJECT_ARMED_BOMB )
								pObj->fFlags &= ~( OBJECT_ARMED_BOMB );

							if ( !((*pObj).fFlags & OBJECT_DISABLED_BOMB) )
								pObj->fFlags |= OBJECT_DISABLED_BOMB ;

							INT32 sGridNo = gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo;
							UINT8 ubLevel = gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].ubLevel;

							// OJW - 20091029 - disarm explosives
							if (is_networked && is_client)
								send_disarm_explosive( sGridNo , gWorldBombs[uiWorldBombIndex].iItemIndex, ubID );

							// set back ubWireNetworkFlag and bDefuseFrequency, but not the direction... bomb is still aimed, it is just turned off
							(*pObj)[0]->data.ubWireNetworkFlag = 0;
							(*pObj)[0]->data.bDefuseFrequency = 0;
							(*pObj)[0]->data.misc.bDelay = 0;
							(*pObj)[0]->data.misc.bFrequency = 0;
							(*pObj)[0]->data.misc.ubBombOwner = NOBODY;
							(*pObj)[0]->data.misc.bDetonatorType = 0;
						
							//create a new item: copy the old item
							OBJECTTYPE newbombitem( *pObj );

							// place item on the floor
							AddItemToPool( sGridNo, &newbombitem, 1, ubLevel, 0, -1 );

							// remove old item
							RemoveItemFromPool( sGridNo, gWorldBombs[uiWorldBombIndex].iItemIndex, ubLevel );

							// TODO remove bomb from queue...
						}
					}
				}
			}
		}
	}

	// Flugente: new stuff: we can now also arm bombs in our inventory, and detonate/defuse those bombs remotely
	// So we have to look at every item in every inventory in this sector
	for (UINT32 cnt = 0; cnt < guiNumMercSlots; ++cnt )
	{
		SOLDIERTYPE* pSoldier = MercSlots[ cnt ];

		if ( pSoldier != NULL )
		{
			if ( pSoldier->bInSector && pSoldier->bActive )
			{
				INT8 invsize = (INT8)pSoldier->inv.size();								// remember inventorysize, so we don't call size() repeatedly
							  
				for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)							// ... for all items in our inventory ...
			    {
					// ... if Item is a bomb ...
					if (pSoldier->inv[bLoop].exists() == true && ( Item[pSoldier->inv[bLoop].usItem].usItemClass & (IC_BOMB) ) )
					{
						OBJECTTYPE * pObj = &(pSoldier->inv[bLoop]);					// ... get pointer for this item ...

						if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_REMOTE )
						{
							// Found a remote bomb, so check to see if it has the same frequency
							if ( fDetonate )	// detonate bombs
							{
								// Found a remote bomb, so check to see if it has the same frequency
								if ((*pObj)[0]->data.misc.bFrequency == bFrequency)
								{					
									// SANDRO - added merc records and some exp
									if ( ((*pObj)[0]->data.misc.ubBombOwner) > 1 )
									{
										if ( MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)]->ubProfile != NO_PROFILE &&
											MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)]->bTeam == gbPlayerNum )
										{
											gMercProfiles[MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)]->ubProfile].records.usExpDetonated++;

											StatChange( MercPtrs[((*pObj)[0]->data.misc.ubBombOwner - 2)], EXPLODEAMT, ( 5 ), FALSE );					
										}
									}

									gubPersonToSetOffExplosions = ubID;

									// ignite explosions manually - this item is not in the WorldBobms-structure, so we can't add it to the queue
									IgniteExplosion( ubID, pSoldier->sX, pSoldier->sY, (INT16) (gpWorldLevelData[pSoldier->sGridNo].sHeight), pSoldier->sGridNo, pObj->usItem, pSoldier->pathing.bLevel, pSoldier->ubDirection, pObj );

									DeleteObj( pObj );
								}
							}
							else	// defuse bombs
							{
								// check for frequency
								if ((*pObj)[0]->data.bDefuseFrequency == bFrequency)
								{
									// check for a defuse
									if ( HasAttachmentOfClass(pObj, AC_DEFUSE) )
									{
										(*pObj)[0]->data.bTrap = 0;

										if ( (*pObj).fFlags & OBJECT_KNOWN_TO_BE_TRAPPED )
											pObj->fFlags &= ~( OBJECT_KNOWN_TO_BE_TRAPPED );

										if ( (*pObj).fFlags & OBJECT_ARMED_BOMB )
											pObj->fFlags &= ~( OBJECT_ARMED_BOMB );
																				
										// set back ubWireNetworkFlag and bDefuseFrequency, but not the direction... bomb is still aimed, it is just turned off
										(*pObj)[0]->data.ubWireNetworkFlag = 0;
										(*pObj)[0]->data.bDefuseFrequency = 0;
										(*pObj)[0]->data.misc.bDetonatorType = 0;
										(*pObj)[0]->data.misc.bDelay = 0;
										(*pObj)[0]->data.misc.bFrequency = 0;
										(*pObj)[0]->data.bTrap = 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void SetOffPanicBombs( UINT8 ubID, INT8 bPanicTrigger )
{
	// need to turn off gridnos & flags in gTacticalStatus
	gTacticalStatus.sPanicTriggerGridNo[ bPanicTrigger ] = NOWHERE;	
	if ( ( TileIsOutOfBounds(gTacticalStatus.sPanicTriggerGridNo[0])) &&
		( TileIsOutOfBounds(gTacticalStatus.sPanicTriggerGridNo[1])) &&
		( TileIsOutOfBounds(gTacticalStatus.sPanicTriggerGridNo[2])) )
	{
		gTacticalStatus.fPanicFlags &= ~(PANIC_TRIGGERS_HERE);
	}

	switch( bPanicTrigger )
	{
	case 0:
		SetOffBombsByFrequency( ubID, PANIC_FREQUENCY );
		gTacticalStatus.fPanicFlags &= ~(PANIC_BOMBS_HERE);
		break;

	case 1:
		SetOffBombsByFrequency( ubID, PANIC_FREQUENCY_2 );
		break;

	case 2:
		SetOffBombsByFrequency( ubID, PANIC_FREQUENCY_3 );
		break;

	default:
		break;

	}

	if ( gTacticalStatus.fPanicFlags )
	{
		// find a new "closest one"
		MakeClosestEnemyChosenOne();
	}
}

BOOLEAN SetOffBombsInGridNo( UINT8 ubID, INT32 sGridNo, BOOLEAN fAllBombs, INT8 bLevel )
{
	UINT32	uiWorldBombIndex;
	UINT32	uiTimeStamp;
	OBJECTTYPE * pObj;
	BOOLEAN	fFoundMine = FALSE;

	uiTimeStamp = GetJA2Clock();

	// Go through all the bombs in the world, and look for mines at this location
	for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
	{
		if (gWorldBombs[uiWorldBombIndex].fExists && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo == sGridNo && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].ubLevel == bLevel )
		{
			pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
			if (!((*pObj).fFlags & OBJECT_DISABLED_BOMB))
			{
				if (fAllBombs || (*pObj)[0]->data.misc.bDetonatorType == BOMB_PRESSURE)
				{
					// Snap: if we do set off our own trap (e.g. by trying to disarm it), we pay!
					/*if (!fAllBombs && MercPtrs[ ubID ]->bTeam != gbPlayerNum )
					{
					// ignore this unless it is a mine, etc which would have to have been placed by the
					// player, seeing as how the others are all marked as known to the AI.
					if ( !(Item[pObj->usItem].mine || pObj->usItem == TRIP_FLARE || pObj->usItem == TRIP_KLAXON ) )
					{
					continue;
					}
					}

					// player and militia ignore bombs set by player
					if ( (*pObj)[0]->data.misc.ubBombOwner > 1 && (MercPtrs[ ubID ]->bTeam == gbPlayerNum || MercPtrs[ ubID ]->bTeam == MILITIA_TEAM) )
					{
					continue;

					}*/

					if (pObj->usItem == SWITCH)
					{						
						// send out a signal to detonate other bombs, rather than this which
						// isn't a bomb but a trigger
						SetOffBombsByFrequency( ubID, (*pObj)[0]->data.misc.bFrequency );
					}
					// Flugente: a tripwire activates all other tripwires in connection, and detonates all bombs in connection that are tripwire-activated
					else if ( Item[pObj->usItem].tripwire == 1 || (pObj->usItem == ACTION_ITEM && (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP && Item[(*pObj)[0]->data.misc.usBombItem].tripwire == 1 ) )
					{
						gubPersonToSetOffExplosions = ubID;

						/*if (pObj->usItem != ACTION_ITEM || (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP)
						{
							uiTimeStamp += BOMB_QUEUE_DELAY;
						}*/

						// put this bomb on the queue
						AddBombToQueue( uiWorldBombIndex, uiTimeStamp );
						
						if ( (*pObj)[0]->data.misc.usBombItem != NOTHING && Item[ (*pObj)[0]->data.misc.usBombItem ].usItemClass & IC_EXPLOSV )
						{
							fFoundMine = TRUE;
						}
												
						// Flugente hack: the tripwire will get removed in HandleExplosionQueue (it is still needed in there until the bomb gets called). 
						// Because of this, ActivateSurroundingTripwire would normally find this wire again, thus a loop would occur.
						// So we mark the wire with this flag
						(*pObj)[0]->data.sObjectFlag |= TRIPWIRE_ACTIVATED;

						// activate surrounding tripwires and tripwire-activated mines, unless tripwire is too much damaged and we are unlucky.. 
						if ( (*pObj)[0]->data.objectStatus > (INT16)Random(50) )
							fFoundMine = ActivateSurroundingTripwire(ubID, sGridNo, bLevel, (*pObj)[0]->data.ubWireNetworkFlag);
					}
					else
					{
						gubPersonToSetOffExplosions = ubID;

						// SANDRO - merc records
						// only if we blew up somebody not in our team(no achievement for blowing our guys :)), only if owner exists and have profile
						if ( (MercPtrs[ubID]->bTeam != gbPlayerNum) && ((*pObj)[0]->data.misc.ubBombOwner > 1) )
						{
							if ( MercPtrs[ ((*pObj)[0]->data.misc.ubBombOwner - 2) ]->ubProfile != NO_PROFILE && MercPtrs[ ((*pObj)[0]->data.misc.ubBombOwner - 2) ]->bTeam == gbPlayerNum ) 
								gMercProfiles[ MercPtrs[ ((*pObj)[0]->data.misc.ubBombOwner - 2) ]->ubProfile ].records.usExpDetonated++;
						}

						/*if (pObj->usItem != ACTION_ITEM || (*pObj)[0]->data.misc.bActionValue == ACTION_ITEM_BLOW_UP)
						{
							uiTimeStamp += BOMB_QUEUE_DELAY;
						}*/

						// put this bomb on the queue
						AddBombToQueue( uiWorldBombIndex, uiTimeStamp );

						if ( (*pObj)[0]->data.misc.usBombItem != NOTHING && Item[ (*pObj)[0]->data.misc.usBombItem ].usItemClass & IC_EXPLOSV )
						{
							fFoundMine = TRUE;
						}

					}
				}
			}
		}
	}
	return( fFoundMine );
}

void ActivateSwitchInGridNo( UINT8 ubID, INT32 sGridNo )
{
	UINT32	uiWorldBombIndex;
	OBJECTTYPE * pObj;

	// Go through all the bombs in the world, and look for mines at this location
	for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
	{
		if (gWorldBombs[uiWorldBombIndex].fExists && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo == sGridNo)
		{
			pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );

			if ( pObj->usItem == SWITCH && ( !((*pObj).fFlags & OBJECT_DISABLED_BOMB) ) && (*pObj)[0]->data.misc.bDetonatorType == BOMB_SWITCH)
			{
				// send out a signal to detonate other bombs, rather than this which
				// isn't a bomb but a trigger

				// first set attack busy count to 0 in case of a lingering a.b.c. problem...
				// No, not a good idea.
				// gTacticalStatus.ubAttackBusyCount = 0;

				SetOffBombsByFrequency( ubID, (*pObj)[0]->data.misc.bFrequency );
			}
		}
	}
}

BOOLEAN SaveExplosionTableToSaveGameFile( HWFILE hFile )
{
	UINT32 uiNumBytesWritten;
	UINT32 uiExplosionCount=0;
	UINT32 uiCnt;


	//
	// Explosion queue Info
	//


	//Write the number of explosion queues
	FileWrite( hFile, &gubElementsOnExplosionQueue, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		FileClose( hFile );
		return( FALSE );
	}


	//loop through and add all the explosions
	for( uiCnt=0; uiCnt< MAX_BOMB_QUEUE; uiCnt++)
	{
		FileWrite( hFile, &gExplosionQueue[ uiCnt ], sizeof( ExplosionQueueElement ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( ExplosionQueueElement ) )
		{
			FileClose( hFile );
			return( FALSE );
		}
	}


	//
	// Explosion Data
	//

	//loop through and count all the active explosions
	uiExplosionCount = 0;
	for( uiCnt=0; uiCnt< NUM_EXPLOSION_SLOTS; uiCnt++)
	{
		if( gExplosionData[ uiCnt ].fAllocated )
		{
			uiExplosionCount++;
		}
	}

	//Save the number of explosions
	FileWrite( hFile, &uiExplosionCount, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		FileClose( hFile );
		return( FALSE );
	}



	//loop through and count all the active explosions
	for( uiCnt=0; uiCnt< NUM_EXPLOSION_SLOTS; uiCnt++)
	{
		if( gExplosionData[ uiCnt ].fAllocated )
		{
			FileWrite( hFile, &gExplosionData[ uiCnt ], sizeof( EXPLOSIONTYPE ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( EXPLOSIONTYPE ) )
			{
				FileClose( hFile );
				return( FALSE );
			}
		}
	}




	return( TRUE );
}



BOOLEAN LoadExplosionTableFromSavedGameFile( HWFILE hFile )
{
	UINT32 uiNumBytesRead;
	UINT32 uiCnt;


	//
	// Explosion Queue
	//

	//Clear the Explosion queue
	memset( gExplosionQueue, 0, sizeof( ExplosionQueueElement ) * MAX_BOMB_QUEUE );

	//Read the number of explosions queue's
	FileRead( hFile, &gubElementsOnExplosionQueue, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return( FALSE );
	}


	//loop through read all the active explosions fro the file
	for( uiCnt=0; uiCnt<MAX_BOMB_QUEUE; uiCnt++)
	{
		FileRead( hFile, &gExplosionQueue[ uiCnt ], sizeof( ExplosionQueueElement ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( ExplosionQueueElement ) )
		{
			return( FALSE );
		}
	}



	//
	// Explosion Data
	//

	//Load the number of explosions
	FileRead( hFile, &guiNumExplosions, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return( FALSE );
	}


	//loop through and load all the active explosions
	for( uiCnt=0; uiCnt< guiNumExplosions; uiCnt++)
	{
		FileRead( hFile, &gExplosionData[ uiCnt ], sizeof( EXPLOSIONTYPE ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( EXPLOSIONTYPE ) )
		{
			return( FALSE );
		}
		gExplosionData[ uiCnt ].iID = uiCnt;
		gExplosionData[ uiCnt ].iLightID = -1;

		GenerateExplosionFromExplosionPointer( &gExplosionData[ uiCnt ] );
	}

	return( TRUE );
}




BOOLEAN DoesSAMExistHere( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, INT32 sGridNo )
{
	INT32 cnt;
	INT16 sSectorNo;

	// ATE: If we are belwo, return right away...
	if ( sSectorZ != 0 )
	{
		return( FALSE );
	}

	sSectorNo = SECTOR( sSectorX, sSectorY );

	for ( cnt = 0; cnt < NUMBER_OF_SAMS; cnt++ )
	{
		// Are we i nthe same sector...
		if ( pSamList[ cnt ] == sSectorNo )
		{
			// Are we in the same gridno?
			if ( pSamGridNoAList[ cnt ] == sGridNo || pSamGridNoBList[ cnt ] == sGridNo )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


void UpdateAndDamageSAMIfFound( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, INT32 sGridNo, UINT8 ubDamage )
{
	INT16 sSectorNo;

	// OK, First check if SAM exists, and if not, return
	if ( !DoesSAMExistHere( sSectorX, sSectorY, sSectorZ, sGridNo ) )
	{
		return;
	}

	// Damage.....
	sSectorNo = CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY );

	INT8 statusbefore = StrategicMap[ sSectorNo ].bSAMCondition;

	if ( StrategicMap[ sSectorNo ].bSAMCondition >= ubDamage )
	{
		StrategicMap[ sSectorNo ].bSAMCondition -= ubDamage;
	}
	else
	{
		StrategicMap[ sSectorNo ].bSAMCondition = 0;
	}

	// SAM site may have been put out of commission...
	UpdateAirspaceControl( );

	// Flugente:  campaign stats: if SAM was functional before and isn't anymore, note this
	if ( statusbefore >= MIN_CONDITION_FOR_SAM_SITE_TO_WORK && StrategicMap[ sSectorNo ].bSAMCondition < MIN_CONDITION_FOR_SAM_SITE_TO_WORK )
		gCurrentIncident.usIncidentFlags |= INCIDENT_SAMSITE_SABOTAGED;

	// ATE: GRAPHICS UPDATE WILL GET DONE VIA NORMAL EXPLOSION CODE.....
}


void UpdateSAMDoneRepair( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ	)
{
	INT32 cnt;
	INT16 sSectorNo;
	BOOLEAN	fInSector = FALSE;
	UINT16	usGoodGraphic, usDamagedGraphic;

	// ATE: If we are below, return right away...
	if ( sSectorZ != 0 )
	{
		return;
	}

	if ( sSectorX == gWorldSectorX && sSectorY == gWorldSectorY && sSectorZ == gbWorldSectorZ )
	{
		fInSector = TRUE;
	}


	sSectorNo = SECTOR( sSectorX, sSectorY );

	for ( cnt = 0; cnt < NUMBER_OF_SAMS; cnt++ )
	{
		// Are we i nthe same sector...
		if ( pSamList[ cnt ] == sSectorNo )
		{
			// get graphic.......
			GetTileIndexFromTypeSubIndex( EIGHTISTRUCT, (UINT16)( gbSAMGraphicList[ cnt ] ), &usGoodGraphic );

			// Damaged one ( current ) is 2 less...
			usDamagedGraphic = usGoodGraphic - 2;

			// First gridno listed is base gridno....

			// if this is loaded....
			if ( fInSector )
			{
				// Update graphic.....
				// Remove old!
				ApplyMapChangesToMapTempFile( TRUE );

				RemoveStruct( pSamGridNoAList[ cnt ], usDamagedGraphic );
				AddStructToHead( pSamGridNoAList[ cnt ], usGoodGraphic );

				ApplyMapChangesToMapTempFile( FALSE );
			}
			else
			{
				// We add temp changes to map not loaded....
				// Remove old
				RemoveStructFromUnLoadedMapTempFile( pSamGridNoAList[ cnt ], usDamagedGraphic, sSectorX, sSectorY, (UINT8)sSectorZ );
				// Add new
				AddStructToUnLoadedMapTempFile( pSamGridNoAList[ cnt ], usGoodGraphic, sSectorX, sSectorY, (UINT8)sSectorZ );
			}
		}
	}

	// SAM site may have been put back into working order...
	UpdateAirspaceControl( );
}


// loop through civ team and find
// anybody who is an NPC and
// see if they get angry
void HandleBuldingDestruction( INT32 sGridNo, UINT8 ubOwner )
{
	SOLDIERTYPE *	pSoldier;
	UINT8		cnt;

	if ( ubOwner == NOBODY )
	{
		return;
	}

	if ( MercPtrs[ ubOwner ]->bTeam != gbPlayerNum )
	{
		return;
	}

	cnt = gTacticalStatus.Team[ CIV_TEAM ].bFirstID;
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; cnt++ ,pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife && pSoldier->aiData.bNeutral )
		{
			if ( pSoldier->ubProfile != NO_PROFILE )
			{
				// ignore if the player is fighting the enemy here and this is a good guy
				if ( gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector > 0 && (gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) )
				{
					continue;
				}

				if ( DoesNPCOwnBuilding( pSoldier, sGridNo ) )
				{
					MakeNPCGrumpyForMinorOffense( pSoldier, MercPtrs[ ubOwner ] );
				}
			}
		}
	}
}

INT32 FindActiveTimedBomb( void )
{
	UINT32	uiWorldBombIndex;
	UINT32	uiTimeStamp;
	OBJECTTYPE * pObj;

	uiTimeStamp = GetJA2Clock();

	// Go through all the bombs in the world, and look for timed ones
	for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
	{
		if (gWorldBombs[uiWorldBombIndex].fExists)
		{
			pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
			if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_TIMED && !((*pObj).fFlags & OBJECT_DISABLED_BOMB) )
			{
				return( gWorldBombs[uiWorldBombIndex].iItemIndex );
			}
		}
	}

	return( -1 );
}

BOOLEAN ActiveTimedBombExists( void )
{
	if ( gfWorldLoaded )
	{
		return( FindActiveTimedBomb() != -1 );
	}
	else
	{
		return( FALSE );
	}
}

void RemoveAllActiveTimedBombs( void )
{
	INT32 iItemIndex;

	do
	{
		iItemIndex = FindActiveTimedBomb();
		if (iItemIndex != -1 )
		{
			RemoveItemFromWorld( iItemIndex );
		}
	} while( iItemIndex != -1 );

}

UINT8 DetermineFlashbangEffect( SOLDIERTYPE *pSoldier, INT8 ubExplosionDir, BOOLEAN fInBuilding)
{
	INT8 bNumTurns;
	UINT16 usHeadItem1, usHeadItem2;

	bNumTurns	= FindNumTurnsBetweenDirs(pSoldier->ubDirection, ubExplosionDir);
	usHeadItem1 = pSoldier->inv[ HEAD1POS ].usItem;
	usHeadItem2 = pSoldier->inv[ HEAD2POS ].usItem;

	// if soldier got in explosion area check if he is affected by flash
	// if soldier wears sun goggles OR grenade behind him OR
	//	(he is not underground AND it is day AND he is outdoor)
	if ( (usHeadItem1 == SUNGOGGLES || usHeadItem2 == SUNGOGGLES) || (bNumTurns > 1) ||
		(!gbWorldSectorZ && !NightTime() && !fInBuilding) )
	{
		// soldier didn't see flash or wears protective sungogles or outdoor at day, so he is only deafened
		return ( FIRE_WEAPON_DEAFENED );
	}

	return ( FIRE_WEAPON_BLINDED_AND_DEAFENED );
}


// HEADROCK HAM 5.1: This handles launching fragments out of an explosion. The number of fragments is read from
// the Explosives.XML file, and they each have a set amount of damage and range as well. They are currently
// fired at completely random trajectories.
void FireFragments( UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, UINT16 usItem, UINT8 ubDirection )
{
	UINT16 usNumFragments = Explosive[Item[usItem].ubClassIndex].usNumFragments;
	UINT16 ubFragRange = Explosive[Item[usItem].ubClassIndex].ubFragRange;

	AssertMsg( ubFragRange > 0 , "Fragmentation data lacks range property!" );

	// on some maps, the floor height is not 0 (example is Drassen D13). We have to account for that with an offset
	INT32 gridno = GETWORLDINDEXFROMWORLDCOORDS( sY, sX );
	UINT32 z_offset = CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[gridno].sHeight );

	for (UINT16 x = 0; x < usNumFragments; ++x)
	{
		FLOAT dRandomX = 0;
		FLOAT dRandomY = 0;
		FLOAT dRandomZ = 0;

		// if explosive is directional, calculation of frags is different
		if ( Item[usItem].directional == TRUE )
		{
			// Flugente: if item is a directional explosive, determine in what direction the frags should fly
			INT16 degree = (45 + ubDirection * 45) % 360;											// modulo 360 to prevent nonsense from nonsensical input
			INT16 horizontalarc = Explosive[Item[usItem].ubClassIndex].ubHorizontalDegree % 360;	// modulo 360 to prevent nonsense from nonsensical input
			INT16 halfhorizontalarc = (INT16)(horizontalarc / 2);
			INT16 sLowHorizontalD = (360 + degree - halfhorizontalarc) % 360;	
			INT16 dRandomDegreeH = (sLowHorizontalD + Random(horizontalarc) ) % 360;
		
			// transform the degree into our coordinates
			if ( dRandomDegreeH < 90 )
			{
				dRandomX = (FLOAT)dRandomDegreeH / 45.0f;
				dRandomY = 0.0;
			}
			else if ( dRandomDegreeH < 180 )
			{
				dRandomX = (FLOAT)2.0;
				dRandomY = (FLOAT)(dRandomDegreeH - 90 ) / 45.0f;
			}
			else if ( dRandomDegreeH < 270 )
			{
				dRandomX = (FLOAT)(270 - dRandomDegreeH) / 45.0f;
				dRandomY = (FLOAT)2.0;
			}
			else 
			{
				dRandomX = 0.0;
				dRandomY = (FLOAT)(360 - dRandomDegreeH) / 45.0f;
			}

			// X and Y now need to be distributed, at the moment they are on a circle
			// project into [-1.0, 1.0]
			dRandomX -= 1.0f;
			dRandomY -= 1.0f;

			// vertical stuff
			INT16 verticalarc = Explosive[Item[usItem].ubClassIndex].ubVerticalDegree % 180;	// modulo 180 to prevent nonsense from nonsensical input
			INT16 halfverticalarc = (INT16)(verticalarc / 2);
			INT16 sLowVerticalD = (90 - halfverticalarc) % 180;
	
			INT16 dRandomDegreeV = sLowVerticalD + Random(verticalarc);

			dRandomZ = ((FLOAT)(dRandomDegreeV) / 90.0f) - 1.0f;
		}
		else
		{
			dRandomX = ((FLOAT)Random(2000) / 1000.0f) - 1.0f;
			dRandomY = ((FLOAT)Random(2000) / 1000.0f) - 1.0f;
			dRandomZ = ((FLOAT)Random(2000) / 1000.0f) - 1.0f;
		}

		FLOAT dDeltaX = (dRandomX * ubFragRange);
		FLOAT dDeltaY = (dRandomY * ubFragRange);
		FLOAT dDeltaZ = ((dRandomZ * 25.6f) * 50 );

		// silversurfer: changed that from 10 to 1 because ubFragRange is already in tiles * 10. A Claymore for example has ubFragRange = 100. Multiplying again by 10 means 100 tiles range!
		FLOAT dRangeMultiplier = 1; // Arbitrary, but gives good results.

		FLOAT dEndX = (FLOAT)(sX + (dDeltaX * dRangeMultiplier));
		FLOAT dEndY = (FLOAT)(sY + (dDeltaY * dRangeMultiplier));
		FLOAT dEndZ = (FLOAT)(sZ + (dDeltaZ * dRangeMultiplier));

		// Add some randomness to the start coordinates as well, so that not all fragments fly from the same point
		// in space.
		FLOAT dStartX = (FLOAT)sX + (dRandomX * ((FLOAT)Random(4)+1.0f));
		FLOAT dStartY = (FLOAT)sY + (dRandomY * ((FLOAT)Random(4)+1.0f));
		FLOAT dStartZ = (FLOAT)sZ + (dRandomZ * ((FLOAT)Random(4)+1.0f));

		FireFragmentGivenTarget( ubOwner, dStartX, dStartY, dStartZ + z_offset, dEndX, dEndY, dEndZ + z_offset, usItem );
	}
}

// Flugente: shoot a gun without anyone operating it (used for makeshift traps wih guns)
void FireFragmentsTrapGun( SOLDIERTYPE* pThrower, INT32 gridno, INT16 sZ, OBJECTTYPE* pObj, UINT8 ubDirection )
{
	if ( !pObj )
		return;

	if ( !pThrower )
		return;

	// at the moment always only 1 bullet
	UINT16 usNumFragments = 1;
	UINT16 ubFragRange = GunRange( pObj, NULL );

	// deviation arcs. A gun fired by tripping a wire isn't exactly precise
	INT16 horizontalarc = 5;
	INT16 verticalarc	= 0;

	INT16 sX = CenterX(gridno);
	INT16 sY = CenterY(gridno);

	AssertMsg( ubFragRange > 0 , "Fragmentation data lacks range property!" );

	// on some maps, the floor height is not 0 (example is Drassen D13). We have to account for that with an offset
	UINT32 z_offset = CONVERT_PIXELS_TO_HEIGHTUNITS( gpWorldLevelData[gridno].sHeight );

	for (UINT16 x = 0; x < usNumFragments; ++x)
	{
		FLOAT dRandomX = 0;
		FLOAT dRandomY = 0;
		FLOAT dRandomZ = 0;
				
		// Flugente: if item is a directional explosive, determine in what direction the frags should fly
		INT16 degree = (45 + ubDirection * 45) % 360;											// modulo 360 to prevent nonsense from nonsensical input
		//INT16 horizontalarc = horizontalarc % 360;	// modulo 360 to prevent nonsense from nonsensical input
		INT16 halfhorizontalarc = (INT16)(horizontalarc / 2);
		INT16 sLowHorizontalD = (360 + degree - halfhorizontalarc) % 360;	
		INT16 dRandomDegreeH = (sLowHorizontalD + Random(horizontalarc) ) % 360;
		
		// transform the degree into our coordinates
		if ( dRandomDegreeH < 90 )
		{
			dRandomX = (FLOAT)dRandomDegreeH / 45.0f;
			dRandomY = 0.0;
		}
		else if ( dRandomDegreeH < 180 )
		{
			dRandomX = (FLOAT)2.0;
			dRandomY = (FLOAT)(dRandomDegreeH - 90 ) / 45.0f;
		}
		else if ( dRandomDegreeH < 270 )
		{
			dRandomX = (FLOAT)(270 - dRandomDegreeH) / 45.0f;
			dRandomY = (FLOAT)2.0;
		}
		else 
		{
			dRandomX = 0.0;
			dRandomY = (FLOAT)(360 - dRandomDegreeH) / 45.0f;
		}

		// X and Y now need to be distributed, at the moment they are on a circle
		// project into [-1.0, 1.0]
		dRandomX -= 1.0f;
		dRandomY -= 1.0f;

		// vertical stuff
		//INT16 verticalarc = verticalarc % 180;	// modulo 180 to prevent nonsense from nonsensical input
		INT16 halfverticalarc = (INT16)(verticalarc / 2);
		INT16 sLowVerticalD = (90 - halfverticalarc) % 180;
	
		INT16 dRandomDegreeV = sLowVerticalD + Random(verticalarc);
		
		dRandomZ = ((FLOAT)(dRandomDegreeV) / 90.0f) - 1.0f;

		FLOAT dDeltaX = (dRandomX * ubFragRange);
		FLOAT dDeltaY = (dRandomY * ubFragRange);
		FLOAT dDeltaZ = ((dRandomZ * 25.6f) * 50 );

		// silversurfer: changed that from 10 to 1 because ubFragRange is already in tiles * 10. A Claymore for example has ubFragRange = 100. Multiplying again by 10 means 100 tiles range!
		FLOAT dRangeMultiplier = 1; // Arbitrary, but gives good results.

		FLOAT dEndX = (FLOAT)(sX + (dDeltaX * dRangeMultiplier));
		FLOAT dEndY = (FLOAT)(sY + (dDeltaY * dRangeMultiplier));
		FLOAT dEndZ = (FLOAT)(sZ + (dDeltaZ * dRangeMultiplier));

		FireBulletGivenTargetTrapOnly( pThrower, pObj, gridno, 150 + z_offset, dEndX, dEndY, dEndZ + z_offset, 100 );
	}
}

#ifdef JA2UB

//-- UB

void HavePersonAtGridnoStop( UINT32 sGridNo )
{
	UINT8	ubID;

	//Sewe if there is a person at the gridno
	ubID = WhoIsThere2( sGridNo, 0 );

	//is it a valid person
	if ( (ubID != NOBODY) && (MercPtrs[ ubID ]->bTeam == gbPlayerNum) )
	{
		SOLDIERTYPE *pSoldier = MercPtrs[ ubID ];

		//Stop the merc
		pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );
	}
}

//JA25 UB
BOOLEAN ShouldThePlayerStopWhenWalkingOnBiggensActionItem( UINT8 ubRecordNum )
{
	SOLDIERTYPE *pSoldier=NULL;

	pSoldier = FindSoldierByProfileID( BIGGENS_UB, TRUE ); //BIGGENS

	//if biggens hasnt said the quote before, or is on the players team
	if( HasNpcSaidQuoteBefore( BIGGENS_UB, ubRecordNum ) || ( pSoldier != NULL || gMercProfiles[ BIGGENS_UB ].bLife <= 0 ) ) //BIGGENS
	{
		return( FALSE );
	}
	else
	{
		return( TRUE );
	}
}

// This function checks if we should replace the fan graphic
BOOLEAN IsFanGraphicInSectorAtThisGridNo( UINT32 sGridNo )
{
	// First check current sector......
	if( gWorldSectorX == 13 && gWorldSectorY == MAP_ROW_J && gbWorldSectorZ == 0 )
	{
		//if this is the right gridno
		/*if( sGridNo == 10978 || 
				sGridNo == 10979 || 
				sGridNo == 10980 || 
				sGridNo == 10818 || 
				sGridNo == 10819 || 
				sGridNo == 10820 || 
				sGridNo == 10658 || 
				sGridNo == 10659 || 
				sGridNo == 10660 )
				*/
		if( sGridNo == gGameUBOptions.FanGridNo[0] || 
				sGridNo == gGameUBOptions.FanGridNo[1] || 
				sGridNo == gGameUBOptions.FanGridNo[2] || 
				sGridNo == gGameUBOptions.FanGridNo[3] || 
				sGridNo == gGameUBOptions.FanGridNo[4] || 
				sGridNo == gGameUBOptions.FanGridNo[5] || 
				sGridNo == gGameUBOptions.FanGridNo[6] || 
				sGridNo == gGameUBOptions.FanGridNo[7] || 
				sGridNo == gGameUBOptions.FanGridNo[8] )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

void HandleDestructionOfPowerGenFan()
{
	UINT8 ubShadeLevel=0;
	INT8	bID;

	//if we have already destroyed the fan
	if( gJa25SaveStruct.ubHowPlayerGotThroughFan == PG__PLAYER_BLEW_UP_FAN_TO_GET_THROUGH )
	{
		//leave
		return;
	}

	//if we have already been in here
	if( gJa25SaveStruct.ubStateOfFanInPowerGenSector == PGF__BLOWN_UP )
	{
		return;
	}

	//Remeber that the player blew up the fan
	gJa25SaveStruct.ubStateOfFanInPowerGenSector = PGF__BLOWN_UP;

	//Remeber how the player got through
	HandleHowPlayerGotThroughFan();

	//Since the player is making LOTS of noise, add more enemies to the tunnel sector
//	AddEnemiesToJa25TunnelMaps();

	if ( gGameUBOptions.HandleAddingEnemiesToTunnelMaps == TRUE )
        {
        	HandleAddingEnemiesToTunnelMaps();
	}

	//Make sure to apply these changes to the map
	ApplyMapChangesToMapTempFile( TRUE );

	//Add an exit grid to the map
	AddExitGridForFanToPowerGenSector();

	//done with the changes
	ApplyMapChangesToMapTempFile( FALSE );

	//Stop the fan sound
	HandleRemovingPowerGenFanSound();


	//
	// Have a qualified merc say a quote
	//
	//Get a random qualified merc to say the quote
	bID = RandomSoldierIdFromNewMercsOnPlayerTeam();
	if( bID != -1 )
	{
		DelayedMercQuote( Menptr[ bID ].ubProfile, QUOTE_ACCEPT_CONTRACT_RENEWAL, GetWorldTotalSeconds() + 2 );
	}
}

void HandleExplosionsInTunnelSector( UINT32 sGridNo )
{
	//if this isnt the tunnel sectors
	if( !( gWorldSectorX == 14 && ( gWorldSectorY == MAP_ROW_J || gWorldSectorY == MAP_ROW_K ) && gbWorldSectorZ == 1 ) )
	{
		//get the fuck out...
		return;
	}

	//Since the enemy will hear explosions in the tunnel, remember the player made a noise
	gJa25SaveStruct.uiJa25GeneralFlags |= JA_GF__DID_PLAYER_MAKE_SOUND_GOING_THROUGH_TUNNEL_GATE;
}


void HandleSeeingFortifiedDoor( UINT32 sGridNo )
{
	INT32 sID=0;

	//if this isnt the First level of the complex
	if( !( gWorldSectorX == 15 && gWorldSectorY == MAP_ROW_K && gbWorldSectorZ == 1 ) )
	{
		//get the fuck out...
		return;
	}

	//if the player has already seen it
	if( gJa25SaveStruct.uiJa25GeneralFlags & JA_GF__PLAYER_HAS_SEEN_FORTIFIED_DOOR )
	{
		//get out
		return;
	}

	//Remeber that we have said the quote
	gJa25SaveStruct.uiJa25GeneralFlags |= JA_GF__PLAYER_HAS_SEEN_FORTIFIED_DOOR;

	//find out whos is the one walking across the trap
	sID = WhoIsThere2( sGridNo, 0 );
	if( sID != NOBODY && IsSoldierQualifiedMerc( &Menptr[ sID ] ) )
	{
	}
	else
	{
		//Get a random merc to say quote
		sID = RandomSoldierIdFromNewMercsOnPlayerTeam();
	}

	if( sID != -1 )
	{
		//say the quote
		TacticalCharacterDialogue( &Menptr[ sID ], QUOTE_LENGTH_OF_CONTRACT );
	}
}

void HandleSwitchToOpenFortifiedDoor( UINT32 sGridNo )
{
	INT8 bID;

	//if the door is already opened
	if( gJa25SaveStruct.ubStatusOfFortifiedDoor == FD__OPEN )
	{
		return;
	}

	//remeber that the switch to open the forified door on level 1, has been pulled
	gJa25SaveStruct.ubStatusOfFortifiedDoor = FD__OPEN;

	bID = RandomSoldierIdFromNewMercsOnPlayerTeam();

	if( bID != -1 )
	{
		TacticalCharacterDialogue( &Menptr[ bID ], QUOTE_COMMENT_BEFORE_HANG_UP );
	}
}

void HandleSeeingPowerGenFan( UINT32 sGridNo )
{
//	INT8 bID;
	UINT8 ubPerson;
	BOOLEAN fFanIsStopped;
	BOOLEAN	fFanHasBeenStopped;
	SOLDIERTYPE *pSoldier;
	SOLDIERTYPE *pOtherSoldier;
	INT32	cnt;
	BOOLEAN	fSaidQuote=FALSE;

	//if the fan has already been seen
	if( IsJa25GeneralFlagSet( JA_GF__PLAYER_SEEN_FAN_BEFORE ) ) 
	{
		//get out
		return;
	}

	fFanIsStopped = ( gJa25SaveStruct.ubStateOfFanInPowerGenSector == PGF__STOPPED );
	fFanHasBeenStopped = IsJa25GeneralFlagSet( JA_GF__POWER_GEN_FAN_HAS_BEEN_STOPPED );

	//Get the person who is at the gridno
	ubPerson = WhoIsThere2( sGridNo, 0 );

	if( ubPerson != NOBODY )
	{
		pSoldier = &Menptr[ ubPerson ];

		//if the fan is stopped And is this merc is a qualified merc but Not a power gen fan qualified merc?
		if( IsSoldierQualifiedMerc( pSoldier ) && fFanIsStopped )
		{
			//Have the merc say the quote
			TacticalCharacterDialogue( pSoldier, QUOTE_HATE_MERC_2_ON_TEAM_WONT_RENEW );
			fSaidQuote = TRUE;
		}
		else if( IsSoldierQualifiedMercForSeeingPowerGenFan( pSoldier ) )
		{
			//Have the merc say the quote
			TacticalCharacterDialogue( pSoldier, QUOTE_HATE_MERC_2_ON_TEAM_WONT_RENEW );
			fSaidQuote = TRUE;
		}
		else
		{
			//see if there is another merc that is close by to say the quote
			cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
			for ( pOtherSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pOtherSoldier++)
			{
				//if the soldier is in the sector
				if( pOtherSoldier->bActive && pOtherSoldier->bInSector && ( pOtherSoldier->stats.bLife >= CONSCIOUSNESS ) )
				{
					INT16 sDistanceAway;

					// if the soldier isnt that far away AND he is qualified merc
					sDistanceAway = PythSpacesAway( pSoldier->sGridNo, pOtherSoldier->sGridNo );

					if( sDistanceAway <= 5 && ( !InARoom( pOtherSoldier->sGridNo, NULL ) || pOtherSoldier->pathing.bLevel ) &&
						( ( IsSoldierQualifiedMerc( pOtherSoldier ) && fFanIsStopped ) ||
							( IsSoldierQualifiedMercForSeeingPowerGenFan( pOtherSoldier ) ) ) )
					{
						//Have the merc say the quote
						TacticalCharacterDialogue( pOtherSoldier, QUOTE_HATE_MERC_2_ON_TEAM_WONT_RENEW );
						fSaidQuote = TRUE;
						break;
					}
				}
			}
		}
	}

	//if the quote was said, dont say it again
	if( fSaidQuote )
	{
		//remeber that the fan has been seen
		SetJa25GeneralFlag( JA_GF__PLAYER_SEEN_FAN_BEFORE );
	}
}
#endif

void HandleBuddyExplosions(UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, BOOLEAN fLocate, INT8 bLevel, UINT8 ubDirection )
{
	// Flugente: Items can have secondary explosions
	if ( Item[usItem].usBuddyItem )
	{
		if ( Item[Item[usItem].usBuddyItem].flare )
		{
			if( !sZ || !FindBuilding(sGridNo) )
			{
				// Add a light effect...
				NewLightEffect( sGridNo, (UINT8)Explosive[Item[Item[usItem].usBuddyItem].ubClassIndex].ubDuration , (UINT8)Explosive[Item[Item[usItem].usBuddyItem].ubClassIndex].ubStartRadius );
			}
		}
		else if ( Item[Item[usItem].usBuddyItem ].usItemClass & (IC_GRENADE|IC_BOMB) )
		{
			IgniteExplosion( ubOwner, sX, sY, sZ, sGridNo, Item[usItem].usBuddyItem, bLevel, ubDirection );						
		}
	}
}

// sevenfm: handle explosive items from attachments
BOOLEAN HandleAttachedExplosions(UINT8 ubOwner, INT16 sX, INT16 sY, INT16 sZ, INT32 sGridNo, UINT16 usItem, BOOLEAN fLocate, INT8 bLevel, UINT8 ubDirection, OBJECTTYPE * pObj)
{
	BOOLEAN binderFound = FALSE;
	BOOLEAN detonator = FALSE;
	BOOLEAN fAttFound = FALSE;
	attachmentList::iterator iterend;
	attachmentList::iterator iter;
	UINT8 direction;
	
	if(pObj==NULL)
		return FALSE;

	binderFound = FindBinderAttachment ( pObj );
	detonator = CheckExplosiveTypeAsDetonator( Explosive[ Item[ usItem ].ubClassIndex ].ubType );

	// search for attached explosives
	iterend = (*pObj)[0]->attachments.end();
	for (iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
	{
		if ( iter->exists() && Item[iter->usItem].usItemClass & (IC_GRENADE|IC_BOMB) )
		{ 			
			// no need for binder if both item and attachment are tripwire-activated
			if( ( Item[pObj->usItem].tripwireactivation && Item[iter->usItem].tripwireactivation ) ||
				( binderFound && detonator && Explosive[Item[iter->usItem].ubClassIndex].ubVolatility > 0 ) )
			{
				if(Item[iter->usItem].directional && ubDirection == DIRECTION_IRRELEVANT)
					direction=Random(8);
				else
					direction=ubDirection;				
				if( Item[iter->usItem].uiIndex == TRIP_KLAXON )
				{
					PlayJA2Sample( KLAXON_ALARM, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 5, SoundDir( sGridNo ) );
					// CallAvailableEnemiesTo( sGridNo );
					MakeNoise( NOBODY, sGridNo, bLevel, gpWorldLevelData[ sGridNo ].ubTerrainID, (UINT8)Explosive[ Item[iter->usItem].ubClassIndex ].ubVolume, NOISE_EXPLOSION );
				} else if( Item[iter->usItem].uiIndex == TRIP_FLARE )
				{
					NewLightEffect( sGridNo, (UINT8)Explosive[ Item[iter->usItem].ubClassIndex ].ubDuration, (UINT8)Explosive[iter->usItem].ubStartRadius );
				} else
				{
					IgniteExplosion( ubOwner, sX, sY, sZ, sGridNo, Item[iter->usItem].uiIndex, bLevel, direction , NULL );
			}
				fAttFound = TRUE;
		}
		}
		if ( binderFound && detonator && gGameExternalOptions.bAllowSpecialExplosiveAttachments && iter->exists() && Item[iter->usItem].usItemClass & IC_MISC )
		{
			if(Item[iter->usItem].gascan)
			{
				IgniteExplosion( ubOwner, sX, sY, sZ, sGridNo, GAS_EXPLOSION, bLevel, DIRECTION_IRRELEVANT , NULL );
				fAttFound = TRUE;
			}
			if(Item[iter->usItem].alcohol)
			{
				IgniteExplosion( ubOwner, sX, sY, sZ, sGridNo, MOLOTOV_EXPLOSION, bLevel, DIRECTION_IRRELEVANT , NULL );
				fAttFound = TRUE;
			}
			if(Item[iter->usItem].marbles)
			{
				IgniteExplosion( ubOwner, sX, sY, sZ, sGridNo, FRAG_EXPLOSION, bLevel, DIRECTION_IRRELEVANT , NULL );
				fAttFound = TRUE;
		}
	}
	}
	return fAttFound;
}

void CheckForBuriedBombsAndRemoveFlags( INT32 sGridNo, INT8 bLevel )
{
	if ( FindWorldItemForBuriedBombInGridNo(sGridNo, bLevel) == -1 )
	{
		// make sure no one thinks there is a bomb here any more!
		if ( gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT )
		{
			RemoveBlueFlag( sGridNo, bLevel );
		}
		gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_ENEMY_MINE_PRESENT);
	}
}

// sevenfm: calculate total average volatility of item+attachments
UINT16 CalcTotalVolatility(OBJECTTYPE * pObj)
{
	BOOLEAN binderFound = FALSE;
	BOOLEAN detonator = FALSE;
	attachmentList::iterator iterend;
	attachmentList::iterator iter;
	UINT16 totalVolatility;
	UINT8 num;
	UINT16 usItem;
	UINT16 classIndex;
	
	if(pObj==NULL)
		return 0;

	usItem = pObj->usItem;
	classIndex = Item[ usItem ].ubClassIndex;
	
	totalVolatility = Explosive[ classIndex ].ubVolatility;
	num = 1;

	binderFound = FindBinderAttachment ( pObj );
	detonator = CheckExplosiveTypeAsDetonator( Explosive[ classIndex ].ubType );

	// search for attached explosives
	iterend = (*pObj)[0]->attachments.end();
	for (iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
	{
		if ( iter->exists() && Item[iter->usItem].usItemClass & (IC_GRENADE|IC_BOMB) )
		{ 			
			// no need for binder if both item and attachment are tripwire-activated
			if( ( Item[usItem].tripwireactivation && Item[iter->usItem].tripwireactivation ) ||
				( binderFound && detonator && Explosive[Item[iter->usItem].ubClassIndex].ubVolatility > 0 ) )
			{
				totalVolatility += Explosive[Item[iter->usItem].ubClassIndex].ubVolatility;
				num++;
			}
		}
	}
	return totalVolatility / num;
}

BOOLEAN FindBinderAttachment (OBJECTTYPE * pObj)
{
	attachmentList::iterator iterend;
	attachmentList::iterator iter;

	// check all attachments, search for ELASTIC or DUCT_TAPE;
	iterend = (*pObj)[0]->attachments.end();
	for (iter = (*pObj)[0]->attachments.begin(); iter != iterend; ++iter) 
	{
		if ( iter->exists() && Item[iter->usItem].usItemClass == IC_MISC )
		{
			if(Item[iter->usItem].uiIndex == ELASTIC || Item[iter->usItem].uiIndex == DUCT_TAPE )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOLEAN CheckExplosiveTypeAsDetonator(UINT16 ubType)
{
	// attached explosives are allowed only for EXPLOSV_NORMAL, EXPLOSV_STUN and EXPLOSV_FLASHBANG types
	return ( ubType == EXPLOSV_NORMAL || ubType == EXPLOSV_STUN || ubType == EXPLOSV_FLASHBANG );
}

typedef std::pair<INT32, UINT8> gridnoarmourpair;
typedef std::vector< gridnoarmourpair > gridnoarmourvector;
typedef std::map<UINT32, gridnoarmourvector> gridnoarmournetworkmap;

void SoldierDropThroughRoof( SOLDIERTYPE* pSoldier, INT32 sGridNo )
{
	if ( !pSoldier || TileIsOutOfBounds( sGridNo ) )
		return;

	pSoldier->SetSoldierHeight( 0.0 );
	TeleportSoldier( pSoldier, sGridNo, TRUE );
	pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );

	// if we play the animation of falling roof tiles over this, it will look like the merc fell really painful (there's even blood)
	SoldierCollapse( pSoldier );

	// take damage
	UINT32 damage = 15 + Random( 5 ) + Random( 23 );
	pSoldier->SoldierTakeDamage( ANIM_CROUCH, damage, 0, damage * 100, TAKE_DAMAGE_FALLROOF, NOBODY, NOWHERE, 0, TRUE );
}

gridnoarmourvector GetConnectedRoofGridnoArmours( INT32 sGridNo )
{
	gridnoarmourvector vec;

	// this shouldn't even happen
	if ( TileIsOutOfBounds( sGridNo ) || !IsRoofPresentAtGridNo( sGridNo ) )
		return vec;

	UINT8 armour = 0;
	STRUCTURE* pStruct = FindStructure( sGridNo, (STRUCTURE_WALL) );

	if ( pStruct )
		armour = gubMaterialArmour[pStruct->pDBStructureRef->pDBStructure->ubArmour];

	vec.push_back( std::make_pair( sGridNo, armour ) );

	// starting from sGridNo, we inspect neighbouring gridnos to detect tiles of the same roof, and detect wether there is a wall below them
	// if there is indeed a wall, note its armour level
	// if we already intersected a roof by deleting roof tiles, this function will only detect the connected roof part

	UINT32 cnt = 0;
	while ( cnt < vec.size( ) )
	{
		INT32 currentgridno = vec[cnt++].first;

		// determine all neighbouring gridnos. If they have a roof, add them to the list
		{
			INT32 nextgridno = currentgridno + 1;
			if ( !TileIsOutOfBounds( nextgridno ) && IsRoofPresentAtGridNo( nextgridno ) )
			{
				UINT8 armour = 0;
				STRUCTURE* pStruct = FindStructure( nextgridno, (STRUCTURE_WALL) );

				if ( pStruct )
					armour = gubMaterialArmour[pStruct->pDBStructureRef->pDBStructure->ubArmour];

				gridnoarmourpair pair( nextgridno, armour );

				if ( std::find( vec.begin( ), vec.end( ), pair ) == vec.end( ) )
					vec.push_back( pair );
			}
		}

		{
			INT32 nextgridno = currentgridno - 1;
			if ( !TileIsOutOfBounds( nextgridno ) && IsRoofPresentAtGridNo( nextgridno ) )
			{
				UINT8 armour = 0;
				STRUCTURE* pStruct = FindStructure( nextgridno, (STRUCTURE_WALL) );

				if ( pStruct )
					armour = gubMaterialArmour[pStruct->pDBStructureRef->pDBStructure->ubArmour];

				std::pair<INT32, UINT8> pair( nextgridno, armour );

				if ( std::find( vec.begin( ), vec.end( ), pair ) == vec.end( ) )
					vec.push_back( pair );
			}
		}

		{
			INT32 nextgridno = currentgridno + WORLD_COLS;
			if ( !TileIsOutOfBounds( nextgridno ) && IsRoofPresentAtGridNo( nextgridno ) )
			{
				UINT8 armour = 0;
				STRUCTURE* pStruct = FindStructure( nextgridno, (STRUCTURE_WALL) );

				if ( pStruct )
					armour = gubMaterialArmour[pStruct->pDBStructureRef->pDBStructure->ubArmour];

				gridnoarmourpair pair( nextgridno, armour );

				if ( std::find( vec.begin( ), vec.end( ), pair ) == vec.end( ) )
					vec.push_back( pair );
			}
		}

		{
			INT32 nextgridno = currentgridno - WORLD_COLS;
			if ( !TileIsOutOfBounds( nextgridno ) && IsRoofPresentAtGridNo( nextgridno ) )
			{
				UINT8 armour = 0;
				STRUCTURE* pStruct = FindStructure( nextgridno, (STRUCTURE_WALL) );

				if ( pStruct )
					armour = gubMaterialArmour[pStruct->pDBStructureRef->pDBStructure->ubArmour];

				gridnoarmourpair pair( nextgridno, armour );

				if ( std::find( vec.begin( ), vec.end( ), pair ) == vec.end( ) )
					vec.push_back( pair );
			}
		}
	}

	return vec;
}

// after roof tiles have collapsed, the connected roof tile network can degenerate into several unconnected networks
// we now collect all these networks
gridnoarmournetworkmap GetConnectedRoofNetworks( gridnoarmourvector& arNetwork )
{
	gridnoarmournetworkmap map;

	// we need a copy of the thing
	gridnoarmourvector networkcpy = arNetwork;

	UINT16 networkcounter = 0;

	// at first, each nod is its own network. Ignore nodes that have already been destroyed (those that don't have a roof anymore)
	for ( gridnoarmourvector::iterator it = networkcpy.begin( ); it != networkcpy.end( ); ++it )
	{
		gridnoarmourpair pair = (*it);

		if ( IsRoofPresentAtGridNo( pair.first ) )
		{
			gridnoarmourvector vect = GetConnectedRoofGridnoArmours( pair.first );

			// delete all nodes further down in networkcpy that are also present in vect
			for ( gridnoarmourvector::iterator tmpit = vect.begin( ); tmpit != vect.end( ); ++tmpit )
			{
				networkcpy.erase( std::remove( it + 1, networkcpy.end( ), (*tmpit) ), networkcpy.end( ) );
			}

			map[networkcounter++] = vect;

			if ( it == networkcpy.end( ) )
				break;
		}
	}

	return map;
}

// for a given network, return the same network with updated armour values: wall armour partially extends to neighbouring nodes
gridnoarmourvector GetArmourSharedRoofNetwork( gridnoarmourvector& arNetwork )
{
	// we return a new network
	gridnoarmourvector vec = arNetwork;

	BOOLEAN fChangeApplied = TRUE;

	while ( fChangeApplied )
	{
		fChangeApplied = FALSE;

		gridnoarmourvector::iterator itend = vec.end( );
		for ( gridnoarmourvector::iterator it1 = vec.begin( ); it1 != itend; ++it1 )
		{
			gridnoarmourpair& pair1 = (*it1);

			if ( !IsRoofPresentAtGridNo( pair1.first ) )
				continue;

			// loop over all other nodes
			for ( gridnoarmourvector::iterator it2 = vec.begin( ); it2 != itend; ++it2 )
			{
				gridnoarmourpair& pair2 = (*it2);

				if ( !IsRoofPresentAtGridNo( pair2.first ) )
					continue;

				if ( PythSpacesAway( pair1.first, pair2.first ) == 1 )
				{
					UINT8 armour1 = pair1.second;
					UINT8 armour2 = pair2.second;

					UINT8 sharedarmour1 = max( armour1, armour2 * WALLSTRENGTH_NEIGHBOURING_PER_TILE );
					UINT8 sharedarmour2 = max( armour1 * WALLSTRENGTH_NEIGHBOURING_PER_TILE, armour2 );

					if ( armour1 < sharedarmour1 )
					{
						pair1.second = sharedarmour1;
						fChangeApplied = TRUE;
					}

					if ( armour2 < sharedarmour2 )
					{
						pair2.second = sharedarmour2;
						fChangeApplied = TRUE;
					}
				}
			}
		}
	}

	return vec;
}


// handle destroying if a single roof tile
void RoofDestruction( INT32 sGridNo )
{
	SOLDIERTYPE* pSoldier = NULL;

	if ( TileIsOutOfBounds( sGridNo ) || !IsRoofPresentAtGridNo( sGridNo ) )
		return;

	// any items on this roof drop to the floor below
	// if there is already a structure here, we need to drop the gear next to it...
	INT32 geardropoffgridno = sGridNo;
	if ( !IsLocationSittableExcludingPeople( geardropoffgridno, FALSE ) )
	{
		for ( INT32 i = -1; i <= 1; ++i )
		{
			for ( INT32 j = -1; j <= 1; ++j )
			{
				INT32 tmpgridno = sGridNo + i + j * WORLD_COLS;

				// if this gridno has place for gear, and is in the same room as the original sGridNo (or both are not in a room)
				if ( IsLocationSittableExcludingPeople( tmpgridno, FALSE ) && gusWorldRoomInfo[sGridNo] == gusWorldRoomInfo[tmpgridno] )
				{
					geardropoffgridno = tmpgridno;
					break;
				}
			}
		}
	}

	MoveItemPools( sGridNo, geardropoffgridno, 1, 0 );

	// if there is anybody at the floor level, damage them with the debris
	// we have to do this BEFORE people drop down. It would otherwise be possible for a falling person to avoid this damage, depending on order of collapsing and possible gridno-shifts.
	UINT16 ubId = WhoIsThere2( sGridNo, 0 );
	if ( ubId != NOBODY )
	{
		pSoldier = MercPtrs[ubId];

		pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );

		// if we play the animation of falling roof tiles over this, it will look like the merc fell really painful (there's even blood)
		SoldierCollapse( pSoldier );

		// take damage
		UINT32 damage = 10 + Random( 3 ) + Random( 10 );
		pSoldier->SoldierTakeDamage( ANIM_CROUCH, damage, 0, damage * 100, TAKE_DAMAGE_FALLROOF, NOBODY, NOWHERE, 0, TRUE );
	}

	// if there is a person here, drop them to the ground...
	ubId = WhoIsThere2( sGridNo, 1 );
	if ( ubId != NOBODY )
	{
		pSoldier = MercPtrs[ubId];

		INT32 soldierdropoffgridno = sGridNo;
		if ( !IsLocationSittable( soldierdropoffgridno, FALSE ) )
		{
			for ( INT32 i = -1; i <= 1; ++i )
			{
				for ( INT32 j = -1; j <= 1; ++j )
				{
					INT32 tmpgridno = sGridNo + i + j * WORLD_COLS;

					// if this gridno has place for gear, and is in the same room as the original sGridNo (or both are not in a room)
					if ( IsLocationSittable( tmpgridno, FALSE ) && gusWorldRoomInfo[sGridNo] == gusWorldRoomInfo[tmpgridno] )
					{
						soldierdropoffgridno = tmpgridno;
						break;
					}
				}
			}
		}

		SoldierDropThroughRoof( pSoldier, soldierdropoffgridno );
	}

	// there can be non-roof structures on roofs. In order to collapse those, check for all structures who are above a certain height, and destroy them
	STRUCTURE* pStruct = GetTallestStructureOnGridno( sGridNo, 1 );
	while ( pStruct )
	{
		LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

		if ( pNode )
		{
			if ( RemoveOnRoofStruct( sGridNo, pNode->usIndex ) )
				pStruct = GetTallestStructureOnGridno( sGridNo, 1 );
			else
				break;
		}
		else
			break;
	}

	// very important: if applying changes to map files is turned on, adding corpses will corrupt the map (only noticeable if you reload it)
	// for this reason we turn this off and on again
	ApplyMapChangesToMapTempFile( FALSE );

	// if there is a corpse here, drop that down as well
	ShiftCorpse( sGridNo , 1);

	ApplyMapChangesToMapTempFile( TRUE );

	// play an animation of falling roof tiles
	// if we merely collected the tiles that collapse and then call them all together, it would be possible to have animations for multi-tile collapses
	static UINT16 usRoofCollapseExplosionIndex = 1727;
	if ( HasItemFlag( usRoofCollapseExplosionIndex, ROOF_COLLAPSE_ITEM ) || GetFirstItemWithFlag( &usRoofCollapseExplosionIndex, ROOF_COLLAPSE_ITEM ) )
	{
		InternalIgniteExplosion( NOBODY, CenterX( sGridNo ), CenterY( sGridNo ), 0, sGridNo, usRoofCollapseExplosionIndex, FALSE, 0 );
	}

	RemoveAllRoofsOfTypeRangeAdjustSaveFile( sGridNo, FIRSTTEXTURE, WIREFRAMES );
}

// damage, or even destroy, a roof tile
// return true if it roof was destroyed
BOOLEAN DamageRoof( INT32 sGridNo, INT16 sDamage )
{
	// only if there is significant damage done
	if ( sDamage < 1 || TileIsOutOfBounds( sGridNo ) || !IsRoofPresentAtGridNo( sGridNo ) )
		return FALSE;

	// sadly, pStruct->ubHitPoints of roof structures is shadowed by sBaseGridNo, so we cannot use it
	// pStruct->pDBStructureRef->pDBStructure->ubHitPoints is always 0 for roof tiles, so we cannot use that either
	// therefore roofs have no HP pool, and we have to outright destroy them
	// should that ever be corrected, comment this part back in
	// I could jut write something into pStruct->pDBStructureRef->pDBStructure->ubHitPoints, but knowing the code the 0 will also be used somewhere else - if you try that, be careful!
	/*STRUCTURE* pStruct = FindStructure( sGridNo, STRUCTURE_ROOF );

	if ( pStruct )
	{
		// if damage exceeds hitpoints, destroy the thing
		if ( (UINT8)sDamage >= pStruct->pDBStructureRef->pDBStructure->ubHitPoints )
		{
			RoofDestruction( sGridNo );
			return TRUE;
		}
		// otherwise just damage it
		else
		{
			pStruct->pDBStructureRef->pDBStructure->ubHitPoints = (UINT8)max( 1, pStruct->ubHitPoints - sDamage );
		}
	}
	
	return FALSE;*/

	// just destroy the thing
	RoofDestruction( sGridNo );

	return TRUE;
}

void HandleRoofDestruction( INT32 sGridNo, INT16 sDamage )
{
	// only if there is significant damage done
	if ( sDamage < 1 || !gGameExternalOptions.fRoofCollapse  || TileIsOutOfBounds( sGridNo ) || !IsRoofPresentAtGridNo( sGridNo ) )
		return;

	ApplyMapChangesToMapTempFile( TRUE );

	gridnoarmourvector floorarmourvector = GetConnectedRoofGridnoArmours( sGridNo );

	INT16 bestarmour = 0;
	gridnoarmourvector::iterator itend = floorarmourvector.end( );
	for ( gridnoarmourvector::iterator it = floorarmourvector.begin( ); it != itend; ++it )
		bestarmour = max( bestarmour, (INT16)(*it).second );

	for ( gridnoarmourvector::iterator it = floorarmourvector.begin( ); it != itend; ++it )
	{
		INT32 sNewGridno = (*it).first;

		INT16 distance = PythSpacesAway( sGridNo, sNewGridno );

		// for formula reasons, distance is at least 1
		distance = max( distance, 1);

		// only remove tile if enough damage has been done
		// it might be necessary to tweak the damage formula here
		// armour above 127 is deemed indestructable
		if ( bestarmour < 127 && sDamage > distance * bestarmour )
		{
			if ( DamageRoof( sNewGridno, sDamage - distance * bestarmour ) )
				(*it).second = 0;
		}
	}
		
	// Nodes might have been removed, and the roof might have degenerated into several networks
	gridnoarmournetworkmap roofnetworkmap = GetConnectedRoofNetworks( floorarmourvector );

	// for each remaining node, determine the distance to the closest node with a wall-connection inside the remaining network. If the distance is high enough, the roof will come down
	gridnoarmournetworkmap::iterator roofnetworkitend = roofnetworkmap.end( );
	for ( gridnoarmournetworkmap::iterator roofnetworkit = roofnetworkmap.begin( ); roofnetworkit != roofnetworkitend; ++roofnetworkit )
	{
		// sadly the calculating time is somewhat high if there are many nodes connected to the roof, like Alma prison.
		// therefore this part is commented out. Can be commented in once the speed issues have been resolved
		// properly done, this would simulat the 'structural integrity' of the building
		/*// for each remaining node, determine the distance to the closest node with a wall-connection inside the remaining network. If the distance is high enough, the roof will come down
		gridnoarmourvector roofarmoursharednetwork = GetArmourSharedRoofNetwork( (*roofnetworkit).second );

		for ( gridnoarmourvector::iterator it = roofarmoursharednetwork.begin( ); it != roofarmoursharednetwork.end( ); ++it )
		{
		gridnoarmourpair pair = (*it);

		// if it does not have a roof, ignore
		if ( !IsRoofPresentAtGridNo( pair.first ) )
		{
		pair.second = 0;
		continue;
		}

		if ( pair.second < 1 )
		{
		if ( DamageRoof(  pair.first, 255 ) )
		pair.second = 0;
		}
		}*/

		// for now, determine the best armour for each remaining network, and collapse it if there is no armou - and thus no wall connection - left
		gridnoarmourvector roofnetwork = (*roofnetworkit).second;

		UINT8 bestarmour = 0;
		for ( gridnoarmourvector::iterator it = roofnetwork.begin( ); it != roofnetwork.end( ); ++it )
			bestarmour = max( bestarmour, (*it).second );

		// if a network has no wall connection at all, collapse the entire thing
		if ( bestarmour < 1 )
		{
			for ( gridnoarmourvector::iterator it = roofnetwork.begin( ); it != roofnetwork.end( ); ++it )
			{
				if ( DamageRoof( (*it).first, 255 ) )
					(*it).second = 0;
			}
		}
	}

	ApplyMapChangesToMapTempFile( FALSE );

	// FOR THE NEXT RENDER LOOP, RE-EVALUATE REDUNDENT TILES
	InvalidateWorldRedundency( );
	SetRenderFlags( RENDER_FLAG_FULL );
}
