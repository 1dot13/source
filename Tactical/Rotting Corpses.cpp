	#include "builddefines.h"
	#include <stdio.h>
	#include <string.h>
	#include "wcheck.h"
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
	#include "strategicmap.h"
	#include "los.h"
	#include "opplist.h"
	#include "structure.h"
	#include "message.h"
	#include "Sound Control.h"
	#include "pathai.h"
	#include "Random.h"
	#include "dialogue control.h"
	#include "items.h"
	#include "smell.h"
	#include "World Items.h"
	#include "explosion control.h"
	#include "GameSettings.h"
	#include "Interface Items.h"
	#include "Soldier Profile.h"
	#include "Soldier macros.h"
	#include "Keys.h"
	#include "Render Fun.h"
	#include "strategic.h"
	#include "qarray.h"
	#include "Interface.h"
	#include "Music Control.h"
	#include "Campaign Types.h"	
	#include "text.h"		// added by Flugente
	#include "Vehicles.h"	// added by silversurfer
	#include "ai.h"			// added by Flugente
	#include "PreBattle Interface.h"	// added by Flugente
	#include "Strategic Town Loyalty.h"	// added by Flugente

#include "Animation Control.h"

#include "GameInitOptionsScreen.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

#define		CORPSE_INDEX_OFFSET		10000

// Flugente: these are ini values now
//#define		DELAY_UNTIL_ROTTING		( 1 * NUM_SEC_IN_DAY )
//#define		DELAY_UNTIL_ROTTING			( 1 * NUM_SEC_IN_DAY / 60 )
//#define		DELAY_UNTIL_DONE_ROTTING	( 3 * NUM_SEC_IN_DAY / 60 )

#define		MAX_NUM_CROWS					6

// From lighting
extern		SGPPaletteEntry	gpLightColors[3];
extern		UINT16 gusShadeLevels[16][3];


void MakeCorpseVisible( SOLDIERTYPE *pSoldier, ROTTING_CORPSE *pCorpse );


// When adding a corpse, add struct data...
CHAR8	zCorpseFilenames[ NUM_CORPSES ][70] =
{
	"",
	"ANIMS\\CORPSES\\S_D_JFK.STI",
	"ANIMS\\CORPSES\\S_D_BCK.STI",
	"ANIMS\\CORPSES\\S_D_FWD.STI",
	"ANIMS\\CORPSES\\S_D_DHD.STI",
	"ANIMS\\CORPSES\\S_D_PRN.STI",
	"ANIMS\\CORPSES\\S_D_WTR.STI",
	"ANIMS\\CORPSES\\S_D_FALL.STI",
	"ANIMS\\CORPSES\\S_D_FALLF.STI",

	"ANIMS\\CORPSES\\M_D_JFK.STI",
	"ANIMS\\CORPSES\\M_D_BCK.STI",
	"ANIMS\\CORPSES\\M_D_FWD.STI",
	"ANIMS\\CORPSES\\M_D_DHD.STI",
	"ANIMS\\CORPSES\\M_D_PRN.STI",
	"ANIMS\\CORPSES\\S_D_WTR.STI",
	"ANIMS\\CORPSES\\M_D_FALL.STI",
	"ANIMS\\CORPSES\\M_D_FALLF.STI",

	"ANIMS\\CORPSES\\F_D_JFK.STI",
	"ANIMS\\CORPSES\\F_D_BCK.STI",
	"ANIMS\\CORPSES\\F_D_FWD.STI",
	"ANIMS\\CORPSES\\F_D_DHD.STI",
	"ANIMS\\CORPSES\\F_D_PRN.STI",
	"ANIMS\\CORPSES\\S_D_WTR.STI",
	"ANIMS\\CORPSES\\F_D_FALL.STI",
	"ANIMS\\CORPSES\\F_D_FALLF.STI",

	// Civs....
	"ANIMS\\CORPSES\\M_DEAD1.STI",
	"ANIMS\\CORPSES\\K_DEAD2.STI",
	"ANIMS\\CORPSES\\H_DEAD2.STI",
	"ANIMS\\CORPSES\\FT_DEAD1.STI",
	"ANIMS\\CORPSES\\S_DEAD1.STI",
	"ANIMS\\CORPSES\\W_DEAD1.STI",
	"ANIMS\\CORPSES\\CP_DEAD1.STI",
	"ANIMS\\CORPSES\\M_DEAD2.STI",
	"ANIMS\\CORPSES\\K_DEAD1.STI",
	"ANIMS\\CORPSES\\H_DEAD1.STI",

	"ANIMS\\CORPSES\\FT_DEAD2.STI",
	"ANIMS\\CORPSES\\S_DEAD2.STI",
	"ANIMS\\CORPSES\\W_DEAD2.STI",
	"ANIMS\\CORPSES\\CP_DEAD2.STI",
	"ANIMS\\CORPSES\\CT_DEAD.STI",
	"ANIMS\\CORPSES\\CW_DEAD1.STI",
	"ANIMS\\CORPSES\\MN_DEAD2.STI",
	"ANIMS\\CORPSES\\I_DEAD1.STI",
	"ANIMS\\CORPSES\\L_DEAD1.STI",

	"ANIMS\\CORPSES\\P_DECOMP2.STI",
	"ANIMS\\CORPSES\\TK_WREK.STI",
	"ANIMS\\CORPSES\\TK2_WREK.STI",
	"ANIMS\\CORPSES\\HM_WREK.STI",
	"ANIMS\\CORPSES\\IC_WREK.STI",
	"ANIMS\\CORPSES\\QN_DEAD.STI",
	"ANIMS\\CORPSES\\J_DEAD.STI",
	"ANIMS\\CORPSES\\S_BURNT.STI",
	"ANIMS\\CORPSES\\S_EXPLD.STI",
	"ANIMS\\CORPSES\\CRYO_CORPSE.STI",
};


// When adding a corpse, add struct data...
CHAR8	zNoBloodCorpseFilenames[ NUM_CORPSES ][70] =
{
	"",
	"ANIMS\\CORPSES\\M_D_JFK_NB.STI",
	"ANIMS\\CORPSES\\S_D_BCK_NB.STI",
	"ANIMS\\CORPSES\\S_D_FWD_NB.STI",
	"ANIMS\\CORPSES\\S_D_DHD_NB.STI",
	"ANIMS\\CORPSES\\S_D_PRN_NB.STI",
	"ANIMS\\CORPSES\\S_D_WTR.STI",
	"ANIMS\\CORPSES\\S_D_FALL_NB.STI",
	"ANIMS\\CORPSES\\S_D_FALLF_NB.STI",

	"ANIMS\\CORPSES\\M_D_JFK_NB.STI",
	"ANIMS\\CORPSES\\M_D_BCK_NB.STI",
	"ANIMS\\CORPSES\\M_D_FWD_NB.STI",
	"ANIMS\\CORPSES\\M_D_DHD_NB.STI",
	"ANIMS\\CORPSES\\M_D_PRN_NB.STI",
	"ANIMS\\CORPSES\\S_D_WTR.STI",
	"ANIMS\\CORPSES\\M_D_FALL_NB.STI",
	"ANIMS\\CORPSES\\M_D_FALLF_NB.STI",

	"ANIMS\\CORPSES\\F_D_JFK_NB.STI",
	"ANIMS\\CORPSES\\F_D_BCK_NB.STI",
	"ANIMS\\CORPSES\\F_D_FWD_NB.STI",
	"ANIMS\\CORPSES\\F_D_DHD_NB.STI",
	"ANIMS\\CORPSES\\F_D_PRN_NB.STI",
	"ANIMS\\CORPSES\\S_D_WTR.STI",
	"ANIMS\\CORPSES\\F_D_FALL_NB.STI",
	"ANIMS\\CORPSES\\F_D_FALLF_NB.STI",

	// Civs....
	"ANIMS\\CORPSES\\M_DEAD1_NB.STI",
	"ANIMS\\CORPSES\\K_DEAD2_NB.STI",
	"ANIMS\\CORPSES\\H_DEAD2_NB.STI",
	"ANIMS\\CORPSES\\FT_DEAD1_NB.STI",
	"ANIMS\\CORPSES\\S_DEAD1_NB.STI",
	"ANIMS\\CORPSES\\W_DEAD1_NB.STI",
	"ANIMS\\CORPSES\\CP_DEAD1_NB.STI",
	"ANIMS\\CORPSES\\M_DEAD2_NB.STI",
	"ANIMS\\CORPSES\\K_DEAD1_NB.STI",
	"ANIMS\\CORPSES\\H_DEAD1_NB.STI",

	"ANIMS\\CORPSES\\FT_DEAD2_NB.STI",
	"ANIMS\\CORPSES\\S_DEAD2_NB.STI",
	"ANIMS\\CORPSES\\W_DEAD2_NB.STI",
	"ANIMS\\CORPSES\\CP_DEAD2_NB.STI",
	"ANIMS\\CORPSES\\CT_DEAD.STI",
	"ANIMS\\CORPSES\\CW_DEAD1.STI",
	"ANIMS\\CORPSES\\MN_DEAD2.STI",
	"ANIMS\\CORPSES\\I_DEAD1.STI",
	"ANIMS\\CORPSES\\L_DEAD1.STI",
	"ANIMS\\CORPSES\\P_DECOMP2.STI",

	"ANIMS\\CORPSES\\TK_WREK.STI",
	"ANIMS\\CORPSES\\TK2_WREK.STI",
	"ANIMS\\CORPSES\\HM_WREK.STI",
	"ANIMS\\CORPSES\\IC_WREK.STI",
	"ANIMS\\CORPSES\\QN_DEAD.STI",
	"ANIMS\\CORPSES\\J_DEAD.STI",
	"ANIMS\\CORPSES\\S_BURNT.STI",
	"ANIMS\\CORPSES\\S_EXPLD.STI",
	"ANIMS\\CORPSES\\CRYO_CORPSE.STI",
};

UINT8		gb4DirectionsFrom8[8] =
{
//	7,		// NORTH
	0,
	0,		// NE
	1,		// E
	0,		// SE
	2,		// S
	0,		// SW,
	3,		// W,
	0			// NW
};

UINT8		gb2DirectionsFrom8[8] =
{
	0,		// NORTH
	7,		// NE
	7,		// E
	7,		// SE
	0,		// S
	7,		// SW,
	7,		// W,
	7			// NW
};


BOOLEAN	gbCorpseValidForDecapitation[ NUM_CORPSES ] =
{
	0,
	0,
	1,
	1,
	1,
	1,
	1,
	1,
	1,

	0,
	1,
	1,
	1,
	1,
	1,
	1,
	1,

	0,
	1,
	1,
	1,
	1,
	1,
	1,
	1,

	// Civs....
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,

	1,
	1,
	1,
	1,
	1,			// Bloodcat - changed to 1 to allow skinning (works like decapitating)
	0,
	0,
	0,
	0,
	1,

	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};



INT8 gDecapitatedCorpse[ NUM_CORPSES ] =
{
	0,
	SMERC_JFK,
	SMERC_JFK,
	SMERC_JFK,
	SMERC_JFK,
	SMERC_JFK,
	SMERC_JFK,
	SMERC_JFK,
	SMERC_JFK,

	MMERC_JFK,
	MMERC_JFK,
	MMERC_JFK,
	MMERC_JFK,
	MMERC_JFK,
	MMERC_JFK,
	MMERC_JFK,
	MMERC_JFK,

	FMERC_JFK,
	FMERC_JFK,
	FMERC_JFK,
	FMERC_JFK,
	FMERC_JFK,
	FMERC_JFK,
	FMERC_JFK,
	FMERC_JFK,

	// Civs....
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,

	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,

	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};



std::vector<ROTTING_CORPSE> gRottingCorpse (MAX_ROTTING_CORPSES);
INT32						giNumRottingCorpse = 0;


BOOLEAN CreateCorpsePalette( ROTTING_CORPSE *pCorpse );
BOOLEAN				CreateCorpseShadedPalette( ROTTING_CORPSE *pCorpse, UINT32 uiBase, SGPPaletteEntry *pShadePal);

void ReduceAmmoDroppedByNonPlayerSoldiers( SOLDIERTYPE *pSoldier, INT32 iInvSlot );
void ReduceAttachmentsOnGunForNonPlayerChars(SOLDIERTYPE *pSoldier, OBJECTTYPE * pObj);

INT32 GetFreeRottingCorpse(void)
{
	for ( INT32 iCount = 0; iCount < giNumRottingCorpse; ++iCount )
	{
		if(( gRottingCorpse[iCount].fActivated == FALSE ) )
			return((INT32)iCount);
	}

	if(giNumRottingCorpse < MAX_ROTTING_CORPSES )
		return((INT32)giNumRottingCorpse++);

	return(-1);
}


void RecountRottingCorpses(void)
{
	if ( giNumRottingCorpse > 0 )
	{
		for ( INT32 uiCount = giNumRottingCorpse - 1; (uiCount >= 0); --uiCount )
		{
			if( ( gRottingCorpse[uiCount].fActivated == FALSE ) )
			{
				giNumRottingCorpse=(UINT32)(uiCount+1);
				break;
			}
		}
	}
}


UINT16 GetCorpseStructIndex( ROTTING_CORPSE_DEFINITION *pCorpseDef, BOOLEAN fForImage )
{
	INT8		bDirection;

	switch( pCorpseDef->ubType )
	{
	case QUEEN_MONSTER_DEAD:
	case BURNT_DEAD:
	case EXPLODE_DEAD:
	case CRYO_CORPSE:

		bDirection = 0;
		break;

	case ICECREAM_DEAD:
	case HUMMER_DEAD:

		// OK , these have 2 directions....
		bDirection = gb2DirectionsFrom8[ pCorpseDef->ubDirection ];
		if ( fForImage )
		{
			bDirection = gOneCDirection[ bDirection ];
		}
		break;

	case SMERC_FALL:
	case SMERC_FALLF:
	case MMERC_FALL:
	case MMERC_FALLF:
	case FMERC_FALL:
	case FMERC_FALLF:

		// OK , these have 4 directions....
		bDirection = gb4DirectionsFrom8[ pCorpseDef->ubDirection ];

//		if ( fForImage )
//		{
//			bDirection = gOneCDirection[ bDirection ];
//		}
		break;

	default:

		// Uses 8
		bDirection =	pCorpseDef->ubDirection;

		if ( fForImage )
		{
			bDirection = gOneCDirection[ bDirection ];
		}
		break;
	}

	return( bDirection );
}


INT32	AddRottingCorpse( ROTTING_CORPSE_DEFINITION *pCorpseDef )
{
	INT32					iIndex;
	ROTTING_CORPSE			*pCorpse;
	ANITILE_PARAMS			AniParams;
	UINT8					ubLevelID;
	STRUCTURE_FILE_REF		*pStructureFileRef = NULL;
	CHAR8					zFilename[150];
	DB_STRUCTURE_REF		*pDBStructureRef;
	UINT8					ubLoop;
	INT32					sTileGridNo;
	DB_STRUCTURE_TILE		**ppTile;
	UINT16					usStructIndex;
	UINT32					uiDirectionUseFlag;
	
	if (TileIsOutOfBounds(pCorpseDef->sGridNo))
	{
		return( -1 );
	}

	if ( pCorpseDef->ubType == NO_CORPSE )
	{
		return( -1 );
	}

	if( ( iIndex = GetFreeRottingCorpse() )==(-1) )
		return(-1);

	pCorpse = &gRottingCorpse[ iIndex ];

	// Copy elements in
	memcpy( pCorpse, pCorpseDef, sizeof( ROTTING_CORPSE_DEFINITION ) );
	
	uiDirectionUseFlag = ANITILE_USE_DIRECTION_FOR_START_FRAME;

	// If we are a special type...
	switch( pCorpseDef->ubType )
	{
		case SMERC_FALL:
		case SMERC_FALLF:
		case MMERC_FALL:
		case MMERC_FALLF:
		case FMERC_FALL:
		case FMERC_FALLF:

			uiDirectionUseFlag = ANITILE_USE_4DIRECTION_FOR_START_FRAME;

		// Flugente: remember if there was still a head attached to the guy
		case SMERC_JFK:
		case MMERC_JFK:
		case FMERC_JFK:

			 pCorpseDef->fHeadTaken = true;
	}

	if ( pCorpseDef->fHeadTaken )
		pCorpse->def.usFlags |= ROTTING_CORPSE_HEAD_TAKEN;

	if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		// OK, AS WE ADD, CHECK FOR TOD AND DECAY APPROPRIATELY
		if ( ((GetWorldTotalMin( ) - pCorpse->def.uiTimeOfDeath) >= gGameExternalOptions.usCorpseDelayUntilRotting) && (pCorpse->def.ubType < ROTTING_STAGE2) )
		{
			if ( pCorpse->def.ubType <= FMERC_FALLF )
			{
				// Rott!
				pCorpse->def.ubType = ROTTING_STAGE2;
			}
		}

		// If time of death is a few days, now, don't add at all!
		if ( ((GetWorldTotalMin( ) - pCorpse->def.uiTimeOfDeath) > gGameExternalOptions.usCorpseDelayUntilDoneRotting) )
		{
			return( -1 );
		}
	}

	// Check if on roof or not...
	if ( pCorpse->def.bLevel == 0 )
	{
		//ubLevelID = ANI_OBJECT_LEVEL;
		ubLevelID = ANI_STRUCT_LEVEL;
	}
	else
	{
		ubLevelID = ANI_ONROOF_LEVEL;
	}

	memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
	AniParams.sGridNo							= pCorpse->def.sGridNo;
	AniParams.ubLevelID						= ubLevelID;
	AniParams.sDelay							= (INT16)( 150 );
	AniParams.sStartFrame					= 0;
	AniParams.uiFlags							= ANITILE_CACHEDTILE | ANITILE_PAUSED | ANITILE_OPTIMIZEFORSLOWMOVING | ANITILE_ANIMATE_Z | ANITILE_ERASEITEMFROMSAVEBUFFFER | uiDirectionUseFlag;

	// use the x,y coordinates provided in the definition when adding a corpse
	if (pCorpse->def.usFlags |= ROTTING_CORPSE_USE_XY_PROVIDED)
	{
		AniParams.sX = pCorpse->def.dXPos;
		AniParams.sY = pCorpse->def.dYPos;
	}
	else
	{
		ConvertGridNoToCenterCellXY(pCorpse->def.sGridNo, &AniParams.sX, &AniParams.sY);
	}
		
	AniParams.sZ									= (INT16)pCorpse->def.sHeightAdjustment;
	AniParams.uiUserData3					= pCorpse->def.ubDirection;
	
	if ( !gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
	{
		strcpy( AniParams.zCachedFile, zNoBloodCorpseFilenames[ pCorpse->def.ubType ] );
	}
	else
	{
		strcpy( AniParams.zCachedFile, zCorpseFilenames[ pCorpse->def.ubType ] );
	}

	pCorpse->pAniTile = CreateAnimationTile( &AniParams );

	if ( pCorpse->pAniTile == NULL )
	{
		pCorpse->fActivated = FALSE;
		return( -1 );
	}

	// Set flag and index values
	pCorpse->pAniTile->pLevelNode->uiFlags		|= ( LEVELNODE_ROTTINGCORPSE );

	pCorpse->pAniTile->pLevelNode->ubShadeLevel					=	gpWorldLevelData[ pCorpse->def.sGridNo ].pLandHead->ubShadeLevel;
	pCorpse->pAniTile->pLevelNode->ubSumLights					=	gpWorldLevelData[ pCorpse->def.sGridNo ].pLandHead->ubSumLights;
	pCorpse->pAniTile->pLevelNode->ubMaxLights					=	gpWorldLevelData[ pCorpse->def.sGridNo ].pLandHead->ubMaxLights;
	pCorpse->pAniTile->pLevelNode->ubNaturalShadeLevel	=	gpWorldLevelData[ pCorpse->def.sGridNo ].pLandHead->ubNaturalShadeLevel;

	pCorpse->pAniTile->uiUserData								= iIndex;
	pCorpse->iID																= iIndex;

	// copy name of corpse definition...
	memcpy( &(pCorpse->name), &(pCorpseDef->name), sizeof(CHAR16) * 10 );
	pCorpse->name[9] = '\0';

	pCorpse->fActivated = TRUE;

	if ( Random( 100 ) > 50 )
	{
		pCorpse->fAttractCrowsOnlyWhenOnScreen = TRUE;
	}
	else
	{
		pCorpse->fAttractCrowsOnlyWhenOnScreen = FALSE;
	}

	pCorpse->iCachedTileID = pCorpse->pAniTile->sCachedTileID;

	if ( pCorpse->iCachedTileID == -1 )
	{
		DeleteAniTile( pCorpse->pAniTile );
		pCorpse->fActivated = FALSE;
		return( -1 );
	}

	// Get palette and create palettes and do substitutions
	if ( !CreateCorpsePalette( pCorpse ) )
	{
		DeleteAniTile( pCorpse->pAniTile );
		pCorpse->fActivated = FALSE;
		return( -1 );
	}

	SetRenderFlags(RENDER_FLAG_FULL);

	if ( pCorpse->def.usFlags & ROTTING_CORPSE_VEHICLE )
	{
		pCorpse->pAniTile->uiFlags |= ( ANITILE_FORWARD | ANITILE_LOOPING );

		// Turn off pause...
		pCorpse->pAniTile->uiFlags &= (~ANITILE_PAUSED);
	}

	InvalidateWorldRedundency( );

	// OK, loop through gridnos for corpse and remove blood.....

	// Get root filename... this removes path and extension
	// USed to find struct data fo rthis corpse...
	GetRootName( zFilename, AniParams.zCachedFile );

	// Add structure data.....
	CheckForAndAddTileCacheStructInfo( pCorpse->pAniTile->pLevelNode, pCorpse->def.sGridNo, pCorpse->def.bLevel, ( UINT16 ) ( pCorpse->iCachedTileID ), GetCorpseStructIndex( pCorpseDef, TRUE ) );

	pStructureFileRef = GetCachedTileStructureRefFromFilename( zFilename );

	if ( pStructureFileRef != NULL )
	{
		usStructIndex = GetCorpseStructIndex( pCorpseDef, TRUE );

		pDBStructureRef = &(pStructureFileRef->pDBStructureRef[ usStructIndex ] );

		for (ubLoop = 0; ubLoop < pDBStructureRef->pDBStructure->ubNumberOfTiles; ubLoop++)
		{
			ppTile = pDBStructureRef->ppTile;
			sTileGridNo = AddPosRelToBase(pCorpseDef->sGridNo, ppTile[ubLoop]);
			//Remove blood
			RemoveBlood( sTileGridNo, pCorpseDef->bLevel );
		}
	}

	// Flugente: update number of corpses in this sector
	if ( !gbWorldSectorZ )
	{
		SECTORINFO *pSectorInfo = &( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )] );

		if ( pSectorInfo )
			pSectorInfo->usNumCorpses = giNumRottingCorpse;
	}

	// OK, we're done!
	return( iIndex );
}


void FreeCorpsePalettes( ROTTING_CORPSE *pCorpse )
{
	// Free palettes
	MemFree( pCorpse->p8BPPPalette );
	MemFree( pCorpse->p16BPPPalette );

	for ( INT32 cnt = 0; cnt < NUM_CORPSE_SHADES; ++cnt )
	{
		if ( pCorpse->pShades[ cnt ] != NULL )
		{
			MemFree( pCorpse->pShades[ cnt ] );
			pCorpse->pShades[ cnt ] = NULL;
		}
	}
}


void RemoveCorpses( )
{
	for ( INT32 iCount = 0; iCount < giNumRottingCorpse; ++iCount )
	{
		if( ( gRottingCorpse[iCount].fActivated ) )
		{
			RemoveCorpse( iCount );
		}
	}

	giNumRottingCorpse = 0;
}

void RemoveCorpse( INT32 iCorpseID )
{
	// Remove!
	gRottingCorpse[ iCorpseID ].fActivated = FALSE;

	DeleteAniTile( gRottingCorpse[ iCorpseID ].pAniTile );

	FreeCorpsePalettes( &( gRottingCorpse[ iCorpseID ] ) );
}

BOOLEAN CreateCorpsePalette( ROTTING_CORPSE *pCorpse )
{
	CHAR8	zColFilename[ 100 ];
	INT8	bBodyTypePalette;
	SGPPaletteEntry							Temp8BPPPalette[ 256 ];

	pCorpse->p8BPPPalette = (SGPPaletteEntry *) MemAlloc( sizeof( SGPPaletteEntry ) * 256 );

	CHECKF( pCorpse->p8BPPPalette != NULL );

	bBodyTypePalette = GetBodyTypePaletteSubstitutionCode( NULL, pCorpse->def.ubBodyType, zColFilename );

	// If this corpse has cammo,
	if ( pCorpse->def.ubType == ROTTING_STAGE2 )
	{
		bBodyTypePalette = 0;
	}
	else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_CAMO_PALETTE )
	{
		strcpy( zColFilename, "ANIMS\\camo.COL" );
		bBodyTypePalette = 1;
	}
	else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_URBAN_CAMO_PALETTE )
	{
		strcpy( zColFilename, "ANIMS\\urban.col" );
		bBodyTypePalette = 1;
	}
	else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_DESERT_CAMO_PALETTE )
	{
		strcpy( zColFilename, "ANIMS\\desert.col" );
		bBodyTypePalette = 1;
	}
	else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_SNOW_CAMO_PALETTE )
	{
		strcpy( zColFilename, "ANIMS\\snow.col" );
		bBodyTypePalette = 1;
	}
	else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_STEALTH_PALETTE )
	{
		strcpy( zColFilename, "ANIMS\\stealth.col" );
		bBodyTypePalette = 1;
	}

	if ( bBodyTypePalette == -1	)
	{
		// Use palette from HVOBJECT, then use substitution for pants, etc
		memcpy( pCorpse->p8BPPPalette, gpTileCache[ pCorpse->iCachedTileID ].pImagery->vo->pPaletteEntry, sizeof( pCorpse->p8BPPPalette ) * 256 );

		// Substitute based on head, etc
		SetPaletteReplacement( pCorpse->p8BPPPalette, pCorpse->def.HeadPal );
		SetPaletteReplacement( pCorpse->p8BPPPalette, pCorpse->def.VestPal );
		SetPaletteReplacement( pCorpse->p8BPPPalette, pCorpse->def.PantsPal );
		SetPaletteReplacement( pCorpse->p8BPPPalette, pCorpse->def.SkinPal );
	}
	else if ( bBodyTypePalette == 0 )
	{
		// Use palette from hvobject
		memcpy( pCorpse->p8BPPPalette, gpTileCache[ pCorpse->iCachedTileID ].pImagery->vo->pPaletteEntry, sizeof( pCorpse->p8BPPPalette ) * 256 );
	}
	else
	{
		// Use col file
		if ( CreateSGPPaletteFromCOLFile( Temp8BPPPalette, zColFilename ) )
		{
			// Copy into palette
			memcpy( pCorpse->p8BPPPalette,		Temp8BPPPalette, sizeof( pCorpse->p8BPPPalette ) * 256 );
		}
		else
		{
			// Use palette from hvobject
			memcpy( pCorpse->p8BPPPalette, gpTileCache[ pCorpse->iCachedTileID ].pImagery->vo->pPaletteEntry, sizeof( pCorpse->p8BPPPalette ) * 256 );
		}
	}

	// -- BUILD 16BPP Palette from this
	pCorpse->p16BPPPalette = Create16BPPPalette( pCorpse->p8BPPPalette );

	CreateCorpsePaletteTables( pCorpse );

	return( TRUE );
}


BOOLEAN TurnSoldierIntoCorpse( SOLDIERTYPE *pSoldier, BOOLEAN fRemoveMerc, BOOLEAN fCheckForLOS )
{	
	if (TileIsOutOfBounds(pSoldier->sGridNo))
	{
		return( FALSE );
	}

 	ROTTING_CORPSE_DEFINITION		Corpse;
	UINT8												ubType;
	UINT32												cnt;
	UINT16											usItemFlags = 0; //WORLD_ITEM_DONTRENDER;
	INT32												iCorpseID;
	INT8												bVisible = -1;
	OBJECTTYPE									*pObj;
	UINT8						ubNumGoo;
	INT32 sNewGridNo;

	// ATE: Change to fix crash when item in hand
	if ( gpItemPointer != NULL && gpItemPointerSoldier == pSoldier )
	{
		CancelItemPointer( );
	}

	// Setup some values!
	memset( &Corpse, 0, sizeof( Corpse ) );
	Corpse.ubBodyType							= pSoldier->ubBodyType;
	Corpse.sGridNo								= pSoldier->sGridNo;		
	Corpse.dXPos									= pSoldier->dXPos;
	Corpse.dYPos									= pSoldier->dYPos;
	Corpse.bLevel									= pSoldier->pathing.bLevel;
	Corpse.ubProfile							= pSoldier->ubProfile;

///ddd{ for the enemy to be able to detect corpses
	if ( pSoldier->bTeam != gbPlayerNum )
		Corpse.ubAIWarningValue = 1;
		//def.ubAIWarningValue = 1; //not used value!
///ddd}
	
	if ( Corpse.bLevel > 0 )
	{
		Corpse.sHeightAdjustment			= (INT16)( pSoldier->sHeightAdjustment - WALL_HEIGHT );
	}

	SET_PALETTEREP_ID ( Corpse.HeadPal,		pSoldier->HeadPal );
	SET_PALETTEREP_ID ( Corpse.VestPal,		pSoldier->VestPal );
	SET_PALETTEREP_ID ( Corpse.SkinPal,		pSoldier->SkinPal );
	SET_PALETTEREP_ID ( Corpse.PantsPal,	pSoldier->PantsPal );

	int urban = pSoldier->urbanCamo + pSoldier->wornUrbanCamo;
	int jungle = pSoldier->bCamo + pSoldier->wornCamo;
	int desert = pSoldier->desertCamo + pSoldier->wornDesertCamo;
	int snow = pSoldier->snowCamo + pSoldier->wornSnowCamo;
	int total = urban + jungle + desert + snow;

	if ( GetWornStealth(pSoldier) >= 50 )
	{
		Corpse.usFlags |= ROTTING_CORPSE_USE_STEALTH_PALETTE;
	}
	else if ( total >= 50 )
	{
		// display camo depending on which is higher
		if ( jungle >= urban && jungle >= desert && jungle >= snow )
			Corpse.usFlags |= ROTTING_CORPSE_USE_CAMO_PALETTE;
		else if ( urban >= jungle && urban >= desert && urban >= snow )
			Corpse.usFlags |= ROTTING_CORPSE_USE_URBAN_CAMO_PALETTE;
		else if ( desert >= urban && desert >= jungle && desert >= snow )
			Corpse.usFlags |= ROTTING_CORPSE_USE_DESERT_CAMO_PALETTE;
		else if ( snow >= urban && snow >= desert && snow >= jungle )
			Corpse.usFlags |= ROTTING_CORPSE_USE_SNOW_CAMO_PALETTE;
	}

	// Flugente Zombies: Determine if a zombie can rise from this corpse
	switch ( gGameExternalOptions.sZombieRiseBehaviour )
	{	
		case 3:
			// a zombie might rise again
			if ( pSoldier->IsZombie() && Random( 2 ) > 0 )
				Corpse.usFlags |= ROTTING_CORPSE_NEVER_RISE_AGAIN;
			break;

		case 2:	
			// death tosses a coin...
			if ( Random( 2 ) > 0 )
				Corpse.usFlags |= ROTTING_CORPSE_NEVER_RISE_AGAIN;
			break;
			
		case 1:
			// no flag, the dead shall rise for eternity!
			break;

		case 0:
		default:
			// a killed zombie won't rise again
			if ( pSoldier->IsZombie() )
				Corpse.usFlags |= ROTTING_CORPSE_NEVER_RISE_AGAIN;
			break;
	}

	// if zombie and headshots are required, forbid rising if killed by headshot
	if ( pSoldier->IsZombie() && gGameExternalOptions.fZombieOnlyHeadShotsPermanentlyKill && (pSoldier->usSoldierFlagMask & SOLDIER_HEADSHOT) )
		Corpse.usFlags |= ROTTING_CORPSE_NEVER_RISE_AGAIN;

	// Flugente: copy name of soldier...
	memcpy( &(Corpse.name), &(pSoldier->name), sizeof(CHAR16) * 10 );
	Corpse.name[9] = '\0';
		
	// if this soldier's uniform was damaged (gunfire, blade attacks, explosions) then don't allow to take the uniform. We can't stay hidden if we're covered in blood :-)
	if ( pSoldier->usSoldierFlagMask & SOLDIER_DAMAGED_VEST )
		Corpse.usFlags |= ROTTING_CORPSE_NO_VEST;

	if ( pSoldier->usSoldierFlagMask & SOLDIER_DAMAGED_PANTS )
		Corpse.usFlags |= ROTTING_CORPSE_NO_PANTS;

	// Determine corpse type!
	ubType = (UINT8)gubAnimSurfaceCorpseID[ pSoldier->ubBodyType][ pSoldier->usAnimState ];

	Corpse.ubDirection	= pSoldier->ubDirection;

	// If we are a vehicle.... only use 1 direction....
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		Corpse.usFlags |= ROTTING_CORPSE_VEHICLE;

		if ( pSoldier->ubBodyType != ICECREAMTRUCK && pSoldier->ubBodyType != HUMVEE )
		{
			Corpse.ubDirection = NORTHWEST;
		}
		else
		{
			Corpse.ubDirection = gb2DirectionsFrom8[ Corpse.ubDirection ];
		}
	}

	if ( ubType == QUEEN_MONSTER_DEAD || ubType == BURNT_DEAD || ubType == EXPLODE_DEAD || ubType == CRYO_CORPSE )
	{
		Corpse.ubDirection = NORTHWEST;
	}

	// ATE: If bDirection, get opposite
//	if ( ubType == SMERC_FALLF || ubType == MMERC_FALLF || ubType == FMERC_FALLF )
	//{
	//	Corpse.bDirection = gOppositeDirection[ Corpse.bDirection ];
//	}

	// Set time of death
	Corpse.uiTimeOfDeath = GetWorldTotalMin( );

	// If corpse is not valid. make items visible
	if ( ubType == NO_CORPSE && pSoldier->bTeam != gbPlayerNum )
	{
		usItemFlags &= (~WORLD_ITEM_DONTRENDER );
	}
	
	// ATE: If the queen is killed, she should
	// make items visible because it ruins end sequence....
	if ( pSoldier->ubProfile == QUEEN || pSoldier->bTeam == gbPlayerNum )
	{
		bVisible = 1;
	}

	// Not for a robot...
	if ( AM_A_ROBOT( pSoldier ) )
	{

	}
	else if ( ubType == QUEEN_MONSTER_DEAD )
	{
		gTacticalStatus.fLockItemLocators = FALSE;

		UINT8 DiffLevel;
		if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
			DiffLevel = 1;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
			DiffLevel = 2;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
			DiffLevel = 3;
		else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
			DiffLevel = 4;	
		else
			DiffLevel = 1;

		ubNumGoo = 6 - ( DiffLevel - DIF_LEVEL_EASY );
		//ubNumGoo = 6 - ( gGameOptions.ubDifficultyLevel - DIF_LEVEL_EASY );
		
		sNewGridNo = pSoldier->sGridNo + ( WORLD_COLS * 2 );

		for ( cnt = 0; cnt < ubNumGoo; ++cnt )
		{
			CreateItem( JAR_QUEEN_CREATURE_BLOOD, 100, &gTempObject );

			AddItemToPool( sNewGridNo, &gTempObject, bVisible , pSoldier->pathing.bLevel, usItemFlags, -1 );
		}
	}
	else
	{
		// OK, Place what objects this guy was carrying on the ground!
		UINT32 invsize = pSoldier->inv.size();
		for ( cnt = 0; cnt < invsize; ++cnt )
		{
			pObj = &( pSoldier->inv[ cnt ] );

			if ( pObj->exists() == true )
			{
				// Check if it's supposed to be dropped
				// silversurfer: new option to drop items from CIV_TEAM regardless of "OBJECT_UNDROPPABLE" flag
				// the same applies for enemy soldiers if mild drop all has been enabled
				if ( !( (*pObj).fFlags & OBJECT_UNDROPPABLE ) || pSoldier->bTeam == gbPlayerNum 
					|| ( gGameExternalOptions.fCiviliansDropAll && pSoldier->bTeam == CIV_TEAM && !IsVehicle(pSoldier) )
					|| ( gGameExternalOptions.fEnemiesDropAllItems == 2 && pSoldier->bTeam == ENEMY_TEAM ) )
				{
					// and make sure that it really is a droppable item type
					// if ( !(Item[ pObj->usItem ].fFlags & ITEM_DEFAULT_UNDROPPABLE) )
					if ( !(Item[ pObj->usItem ].defaultundroppable ) )
					{
						ReduceAmmoDroppedByNonPlayerSoldiers( pSoldier, cnt );
						//if this soldier was an enemy
						// Kaiden: Added from UB's reveal all items after combat feature!
						// HEADROCK HAM B2.8: Now also reveals equipment dropped by militia, if requirement is met.
						if( pSoldier->bTeam == ENEMY_TEAM ||
							( gGameExternalOptions.ubMilitiaDropEquipment == 2 && pSoldier->bTeam == MILITIA_TEAM ) ||
							( gGameExternalOptions.ubMilitiaDropEquipment == 1 && pSoldier->bTeam == MILITIA_TEAM && Menptr[ pSoldier->ubAttackerID ].bTeam != OUR_TEAM ))
						{
							//add a flag to the item so when all enemies are killed, we can run through and reveal all the enemies items
							usItemFlags |= WORLD_ITEM_DROPPED_FROM_ENEMY;

							if ( Item[pObj->usItem].damageable && Item[pObj->usItem].usItemClass != IC_THROWING_KNIFE ) // Madd: drop crappier items from enemies on higher difficulty levels - note the quick fix for throwing knives
							{
								// silversurfer: externalized this
								//(*pObj)[0]->data.objectStatus -= (gGameOptions.ubDifficultyLevel - 1) * Random(20);
								// if mild drop all is enabled and the item was usually not allowed to drop we will reduce its status considerably
								if ( gGameExternalOptions.fEnemiesDropAllItems == 2 && pSoldier->bTeam == ENEMY_TEAM && (*pObj).fFlags & OBJECT_UNDROPPABLE )
								{
									(*pObj)[0]->data.objectStatus -= 60 + Random( zDiffSetting[gGameOptions.ubDifficultyLevel].usLootStatusModifier );
									(*pObj)[0]->data.objectStatus = min(max((*pObj)[0]->data.objectStatus,1),100); // never below 1% or above 100%
								}
								else
								{
									(*pObj)[0]->data.objectStatus -= Random( zDiffSetting[gGameOptions.ubDifficultyLevel].usLootStatusModifier );
									(*pObj)[0]->data.objectStatus = min(max((*pObj)[0]->data.objectStatus,1),100); // never below 1% or above 100%

								}
									(*pObj)[0]->data.sRepairThreshold = max(1, min(100, ((*pObj)[0]->data.objectStatus + 200)/3 ));
							}
						}

						if(UsingNewAttachmentSystem()==true)
						{
							ReduceAttachmentsOnGunForNonPlayerChars(pSoldier, pObj);
						}

						// HEADROCK HAM B2.8: Militia will drop items only if allowed.
						if (!(gGameExternalOptions.ubMilitiaDropEquipment == 0 && pSoldier->bTeam == MILITIA_TEAM ) &&
							!(gGameExternalOptions.ubMilitiaDropEquipment == 1 && pSoldier->bTeam == MILITIA_TEAM && Menptr[ pSoldier->ubAttackerID ].bTeam == OUR_TEAM ))
						{
							AddItemToPool( pSoldier->sGridNo, pObj, bVisible , pSoldier->pathing.bLevel, usItemFlags, -1 );
						}
					}
				}
			}
		}

		DropKeysInKeyRing( pSoldier, pSoldier->sGridNo, pSoldier->pathing.bLevel, bVisible, FALSE, 0, FALSE );

		// Flugente: even if we forbid militia from dropping their equipment, they will still drop what they took via sector inventory (this functions only drops what they took)
		pSoldier->DropSectorEquipment();
	}

	// Make team look for items
	AllSoldiersLookforItems( TRUE );

	//Madd: set warning value to signal other enemies
	if( pSoldier->bTeam == ENEMY_TEAM )
		Corpse.ubAIWarningValue = 20;

	// This should free up ABC for death codes
	if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_ATTACK )
	{
		DebugAttackBusy( "%%%%%%%%% Freeing up attacker because soldier became a corpse.\n");
		ReduceAttackBusyCount( );
	}

	//if we are to call TacticalRemoveSoldier after adding the corpse
	if( fRemoveMerc )
	{
		// If not a player, you can completely remove soldiertype
		// otherwise, just remove their graphic
		if ( pSoldier->bTeam != gbPlayerNum )
		{
			// Remove merc!
			// ATE: Remove merc slot first - will disappear if no corpse data found!
			TacticalRemoveSoldier( pSoldier->ubID );
		}
		else
		{
			pSoldier->RemoveSoldierFromGridNo( );
		}
	}

	if ( ubType == NO_CORPSE )
	{
		return( TRUE );
	}

	// Set type
	Corpse.ubType	= ubType;

	// Add corpse!
	iCorpseID = AddRottingCorpse( &Corpse );

	// If this is our guy......make visible...
	//if ( pSoldier->bTeam == gbPlayerNum )
	if ( iCorpseID != -1 )
	{
		MakeCorpseVisible( pSoldier, &( gRottingCorpse[ iCorpseID ] ) );
	}

	return( TRUE );
}


INT16 FindNearestRottingCorpse( SOLDIERTYPE *pSoldier )
{
	INT32		uiRange, uiLowestRange = 999999;
	INT32 sLowestGridNo = NOWHERE;
	ROTTING_CORPSE		*pCorpse;

	// OK, loop through our current listing of bodies
	for ( INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt )
	{
		pCorpse = &(gRottingCorpse[ cnt ] );

		if ( pCorpse->fActivated )
		{
			// Check rotting state
			if ( pCorpse->def.ubType == ROTTING_STAGE2 )
			{
				uiRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, pCorpse->def.sGridNo );

				if ( uiRange < uiLowestRange )
				{
					sLowestGridNo = pCorpse->def.sGridNo;
					uiLowestRange = uiRange;
				}
			}
		}
	}

	return( sLowestGridNo );
}

void AddCrowToCorpse( ROTTING_CORPSE *pCorpse )
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	INT8										ubBodyType = CROW;
	UINT8										iNewIndex;
	INT32 sGridNo;
	UINT8										ubDirection;
	SOLDIERTYPE							*pSoldier;
	//DBrot: More Rooms
	//UINT8										ubRoomNum;
	UINT16	usRoomNum;
	// No crows inside :(
	if ( InARoom( pCorpse->def.sGridNo, &usRoomNum ) )
	{
		return;
	}

	// Put him flying over corpse pisition
	MercCreateStruct.ubProfile		= NO_PROFILE;
	MercCreateStruct.sSectorX			= gWorldSectorX;
	MercCreateStruct.sSectorY			= gWorldSectorY;
	MercCreateStruct.bSectorZ			= gbWorldSectorZ;
	MercCreateStruct.ubBodyType		= ubBodyType;
	MercCreateStruct.ubDirection					= SOUTH;
	MercCreateStruct.bTeam							= CIV_TEAM;
	MercCreateStruct.sInsertionGridNo		= pCorpse->def.sGridNo;
	RandomizeNewSoldierStats( &MercCreateStruct );

	if ( TacticalCreateSoldier( &MercCreateStruct, &iNewIndex ) != NULL )
	{
		pSoldier = MercPtrs[ iNewIndex ];

		sGridNo =	FindRandomGridNoFromSweetSpot( pSoldier, pCorpse->def.sGridNo, 2, &ubDirection );
		
		if (!TileIsOutOfBounds(sGridNo))
		{
			pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
			pSoldier->usStrategicInsertionData = sGridNo;

			pSoldier->sInsertionGridNo		= sGridNo;
			pSoldier->sDesiredHeight			= 0;

			// Add to sector
			AddSoldierToSector( iNewIndex );

			// Change to fly animation
			//sGridNo =	FindRandomGridNoFromSweetSpot( pSoldier, pCorpse->def.sGridNo, 5, &ubDirection );
			//pSoldier->usUIMovementMode = CROW_FLY;
			//pSoldier->EVENT_GetNewSoldierPath( sGridNo, pSoldier->usUIMovementMode );

			// Setup action data to point back to corpse....
			pSoldier->aiData.uiPendingActionData1	=	pCorpse->iID;
			pSoldier->aiData.sPendingActionData2		=	pCorpse->def.sGridNo;

			pCorpse->def.bNumServicingCrows++;
		}
	}
}

void HandleCrowLeave( SOLDIERTYPE *pSoldier )
{
	// Check if this crow is still referencing the same corpse...
	ROTTING_CORPSE* pCorpse = &(gRottingCorpse[pSoldier->aiData.uiPendingActionData1]);

	// Double check grindo...
	if ( pCorpse && pSoldier->aiData.sPendingActionData2 == pCorpse->def.sGridNo )
	{
		// We have a match
		// Adjust crow servicing count...
		pCorpse->def.bNumServicingCrows--;

		if ( pCorpse->def.bNumServicingCrows < 0 )
		{
			pCorpse->def.bNumServicingCrows = 0;
		}
	}
}


void HandleCrowFlyAway( SOLDIERTYPE *pSoldier )
{
	UINT8 ubDirection;
	INT32 sGridNo;

	// Set desired height
	pSoldier->sDesiredHeight			= 100;

	// Change to fly animation
	sGridNo =	FindRandomGridNoFromSweetSpot( pSoldier, pSoldier->sGridNo, 5, &ubDirection );
	pSoldier->usUIMovementMode = CROW_FLY;
	SendGetNewSoldierPathEvent( pSoldier, sGridNo, pSoldier->usUIMovementMode );
}


void HandleRottingCorpses( )
{
	ROTTING_CORPSE		*pCorpse;
	INT8							bNumCrows = 0;
	UINT32			uiChosenCorpseID;

	// Don't allow crows here if flags not set
	if ( !gTacticalStatus.fGoodToAllowCrows )
	{
		return;
	}

	if ( gbWorldSectorZ > 0 )
	{
		return;
	}

	// ATE: If it's too late, don't!
	if( NightTime() )
	{
		return;
	}

	// ATE: Check for multiple crows.....
	// Couint how many we have now...
	{
		UINT16 bLoop;
		SOLDIERTYPE * pSoldier;

		for ( bLoop=gTacticalStatus.Team[ CIV_TEAM ].bFirstID, pSoldier=MercPtrs[bLoop]; bLoop <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; bLoop++, pSoldier++)
		{
			if (pSoldier->bActive && pSoldier->bInSector && (pSoldier->stats.bLife >= OKLIFE) && !( pSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) )
			{
				if ( pSoldier->ubBodyType == CROW )
				{
					++bNumCrows;
				}
			}
		}
	}

	// Once population gets down to 0, we can add more again....
	if ( bNumCrows == 0 )
	{
		gTacticalStatus.fDontAddNewCrows = FALSE;
	}

	if ( gTacticalStatus.fDontAddNewCrows )
	{
		return;
	}

	if ( bNumCrows >= gTacticalStatus.ubNumCrowsPossible )
	{
		gTacticalStatus.fDontAddNewCrows = TRUE;
		return;
	}

	if ( gTacticalStatus.Team[ CREATURE_TEAM ].bTeamActive )
	{
		// don't add any crows while there are predators around
		return;
	}

	// Pick one to attact a crow...
	{
		uiChosenCorpseID = Random( giNumRottingCorpse );

		pCorpse = &(gRottingCorpse[ uiChosenCorpseID ] );

		if ( pCorpse->fActivated )
		{
			if ( !( pCorpse->def.usFlags & ROTTING_CORPSE_VEHICLE ) )
			{
				if ( pCorpse->def.ubType == ROTTING_STAGE2 )
				{
					 if ( GridNoOnScreen( pCorpse->def.sGridNo ) )
					 {
						 return;
					 }

					AddCrowToCorpse( pCorpse );
					AddCrowToCorpse( pCorpse );
				}
			}
		}
	}
}

void MakeCorpseVisible( SOLDIERTYPE *pSoldier, ROTTING_CORPSE *pCorpse )
{
	pCorpse->def.bVisible = 1;
	SetRenderFlags( RENDER_FLAG_FULL );
}


void AllMercsOnTeamLookForCorpse( ROTTING_CORPSE *pCorpse, INT8 bTeam )
{
	INT32					cnt;
	SOLDIERTYPE							*pSoldier;
	INT32 sGridNo;

	// If this cump is already visible, return
	if ( pCorpse->def.bVisible == 1 )
	{
		return;
	}

	if ( !pCorpse->fActivated )
	{
		return;
	}

	// IF IT'S THE SELECTED GUY, MAKE ANOTHER SELECTED!
	cnt = gTacticalStatus.Team[ bTeam ].bFirstID;

	sGridNo = pCorpse->def.sGridNo;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ bTeam ].bLastID; cnt++,pSoldier++ )
	{
		// ATE: Ok, lets check for some basic things here!		
		if ( pSoldier->stats.bLife >= OKLIFE && !TileIsOutOfBounds(pSoldier->sGridNo) && pSoldier->bActive && pSoldier->bInSector )
		{
			// and we can trace a line of sight to his x,y coordinates?
			// (taking into account we are definitely aware of this guy now)
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, pCorpse->def.bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
			{
				 MakeCorpseVisible( pSoldier, pCorpse );
				 return;
			}
		}
	}
}

void MercLooksForCorpses( SOLDIERTYPE *pSoldier )
{
	// Should we say disgust quote?
	if ( ( pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_ROTTINGCORPSE ) )
	{
		return;
	}

	if ( pSoldier->ubProfile == NO_PROFILE )
	{
		return;
	}

	if ( AM_AN_EPC( pSoldier ) )
	{
		return;
	}

	if ( QuoteExp[ pSoldier->ubProfile ].QuoteExpHeadShotOnly == 1 )
	{
		return;
	}

	// Every so often... do a corpse quote...
	if ( Random( 400 ) <= 2 )
	{
		INT32				sGridNo;
		ROTTING_CORPSE*		pCorpse;

		// Loop through all corpses....
		for ( INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt )
		{
			pCorpse = &(gRottingCorpse[ cnt ] );

			if ( !pCorpse->fActivated )
			{
				continue;
			}

			// Has this corpse rotted enough?
			if ( pCorpse->def.ubType == ROTTING_STAGE2 )
			{
				sGridNo = pCorpse->def.sGridNo;

				// and we can trace a line of sight to his x,y coordinates?
				// (taking into account we are definitely aware of this guy now)
				if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, pCorpse->def.bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
				{
					TacticalCharacterDialogue( pSoldier, QUOTE_HEADSHOT );

					pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_ROTTINGCORPSE;

					BeginMultiPurposeLocator( sGridNo, pCorpse->def.bLevel, FALSE );

					// Slide to...
					SlideToLocation( 0, sGridNo );

					return;
				}
			}
		}
	}
}

void RebuildAllCorpseShadeTables( )
{
	// Flugente 2015-08-19: atm this function is pointless, as nothing is ever done here... so I'm putting a stop to it 
	return;

	ROTTING_CORPSE*		pCorpse;

	// Loop through all corpses....
	for ( INT32 cnt = 0; cnt < giNumRottingCorpse; cnt++ )
	{
		pCorpse = &(gRottingCorpse[ cnt ] );

		// If this cump is already visible, continue
		if ( pCorpse->def.bVisible == 1 )
		{
			continue;
		}

		if ( !pCorpse->fActivated )
		{
			continue;
		}

		// Rebuild shades....

	}
}

UINT16 CreateCorpsePaletteTables( ROTTING_CORPSE *pCorpse )
{
	SGPPaletteEntry LightPal[256];

	// create the basic shade table
	for ( UINT32 uiCount = 0; uiCount < 256; ++uiCount )
	{
		// combine the rgb of the light color with the object's palette
		LightPal[uiCount].peRed=(UINT8)(__min((UINT16)pCorpse->p8BPPPalette[uiCount].peRed+(UINT16)gpLightColors[0].peRed, 255));
		LightPal[uiCount].peGreen=(UINT8)(__min((UINT16)pCorpse->p8BPPPalette[uiCount].peGreen+(UINT16)gpLightColors[0].peGreen, 255));
		LightPal[uiCount].peBlue=(UINT8)(__min((UINT16)pCorpse->p8BPPPalette[uiCount].peBlue+(UINT16)gpLightColors[0].peBlue, 255));
	}

	// build the shade tables
	CreateCorpseShadedPalette( pCorpse, 0, LightPal );

	// build neutral palette as well!
	// Set current shade table to neutral color

	return(TRUE);
}

BOOLEAN CreateCorpseShadedPalette( ROTTING_CORPSE *pCorpse, UINT32 uiBase, SGPPaletteEntry *pShadePal )
{
	pCorpse->pShades[uiBase] = Create16BPPPaletteShaded( pShadePal, gusShadeLevels[0][0], gusShadeLevels[0][1], gusShadeLevels[0][2], TRUE);

	for ( UINT32 uiCount = 1; uiCount < 16; ++uiCount )
	{
		pCorpse->pShades[uiBase+uiCount]=Create16BPPPaletteShaded( pShadePal, gusShadeLevels[uiCount][0], gusShadeLevels[uiCount][1], gusShadeLevels[uiCount][2], FALSE);
	}

	return(TRUE);
}

ROTTING_CORPSE  *FindCorpseBasedOnStructure( INT32 sGridNo, INT8 asLevel, STRUCTURE *pStructure )
{
	LEVELNODE					*pLevelNode;
	ROTTING_CORPSE		*pCorpse = NULL;
	
	// Flugente: if the corpse is on a roof, search there
	if ( asLevel )
		pLevelNode = gpWorldLevelData[sGridNo].pOnRoofHead;
	else
		pLevelNode = gpWorldLevelData[ sGridNo ].pStructHead;

	while( pLevelNode != NULL )
	{
		if (pLevelNode->pStructureData == pStructure )
		{
			break;
		}
		pLevelNode = pLevelNode->pNext;
	}

	if ( pLevelNode != NULL )
	{
		// Get our corpse....
		pCorpse = &( gRottingCorpse[ pLevelNode->pAniTile->uiUserData ] );
	}

	return( pCorpse );
}


void CorpseHit( INT32 sGridNo, INT8 asLevel, UINT16 usStructureID )
{
}

void VaporizeCorpse( INT32 sGridNo, INT8 asLevel, UINT16 usStructureID )
{
	STRUCTURE				*pStructure, *pBaseStructure;
	ROTTING_CORPSE	*pCorpse = NULL;
	INT32 sBaseGridNo;
	ANITILE_PARAMS		AniParams;

	pStructure = FindStructureByID( sGridNo, usStructureID );

	// Get base....
	pBaseStructure = FindBaseStructure( pStructure );

	// Find base gridno...
	sBaseGridNo = pBaseStructure->sGridNo;

	// Get corpse ID.....
	pCorpse = FindCorpseBasedOnStructure( sBaseGridNo, asLevel, pBaseStructure );

	if ( pCorpse == NULL )
	{
#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Vaporize corpse but corpse cannot be found at: %d", sBaseGridNo );
#endif
		return;
	}

	if ( pCorpse->def.usFlags & ROTTING_CORPSE_VEHICLE )
	{
		return;
	}

	if ( GridNoOnScreen( sBaseGridNo ) )
	{
		// Add explosion
		memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
		AniParams.sGridNo = sBaseGridNo;

		// Check if on roof or not...
		if ( pCorpse->def.bLevel == 0 )
			AniParams.ubLevelID = ANI_STRUCT_LEVEL;
		else
			AniParams.ubLevelID = ANI_ONROOF_LEVEL;

		AniParams.sDelay = (INT16)( 80 );
		AniParams.sStartFrame = 0;
		AniParams.uiFlags = ANITILE_CACHEDTILE | ANITILE_FORWARD;
		ConvertGridNoToCenterCellXY(sBaseGridNo, &AniParams.sX, &AniParams.sY);
		AniParams.sZ = (INT16)pCorpse->def.sHeightAdjustment;

		strcpy( AniParams.zCachedFile, "TILECACHE\\GEN_BLOW.STI" );
		CreateAnimationTile( &AniParams );

		// Remove....
		RemoveCorpse( pCorpse->iID );
		SetRenderFlags( RENDER_FLAG_FULL );

		// add disease
		if ( !gbWorldSectorZ )
		{
			SECTORINFO *pSectorInfo = &( SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )] );

			if ( pSectorInfo )
				pSectorInfo->fDiseasePoints = min( DISEASE_MAX_SECTOR, pSectorInfo->fDiseasePoints + DISEASE_PER_ROTTINGCORPSE );

			// the population doesn't like it when we blow chunks of people all over the place
			INT8 bTownId = GetTownIdForSector( gWorldSectorX, gWorldSectorY );

			// if NOT in a town
			if ( bTownId != BLANK_SECTOR )
			{
				UINT32 uiLoyaltyChange = LOYALTY_PENALTY_ROTTED_CORPSE;
				DecrementTownLoyalty( bTownId, uiLoyaltyChange );
			}
		}

		if ( pCorpse->def.bLevel == 0 )
		{
			// Set some blood......
			SpreadEffect( sBaseGridNo, (UINT8)((2)), 0, NOBODY, BLOOD_SPREAD_EFFECT, asLevel, -1 );
		}
	}

	// PLay a sound....
	PlayJA2Sample( (UINT32)( BODY_EXPLODE_1 ), RATE_11025, SoundVolume( HIGHVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
}

INT32 FindNearestAvailableGridNoForCorpse( ROTTING_CORPSE_DEFINITION *pDef, INT8 ubRadius )
{
	INT32 sSweetGridNo;
	INT16	sTop, sBottom;
	INT16	sLeft, sRight;
	INT16	cnt1, cnt2, cnt3;
	INT32	sGridNo;
	INT32	uiRange, uiLowestRange = 999999;
	INT32	sLowestGridNo=0;
	INT32	leftmost;
	BOOLEAN	fFound = FALSE;
	SOLDIERTYPE soldier;
	INT16	ubSaveNPCAPBudget;
	UINT8	ubSaveNPCDistLimit;
	STRUCTURE_FILE_REF * pStructureFileRef = NULL;
	CHAR8	zFilename[150];
	UINT8	ubBestDirection=0;
	BOOLEAN	fSetDirection	= FALSE;

	cnt3 = 0;

	// Get root filename... this removes path and extension
	// USed to find struct data fo rthis corpse...
	GetRootName( zFilename, zCorpseFilenames[pDef->ubType ] );

	pStructureFileRef = GetCachedTileStructureRefFromFilename( zFilename );

	sSweetGridNo = pDef->sGridNo;


	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.bTeam = 1;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + (WORLD_COLS * cnt1) + cnt2;
			if ( sGridNo >=0 && sGridNo < WORLD_MAX )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, 0 );//dnl ch50 071009

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if ( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) &&
				gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				if ( NewOKDestination( &soldier, sGridNo, TRUE, soldier.pathing.bLevel ) )
				{
					BOOLEAN fDirectionFound = FALSE;
					BOOLEAN	fCanSetDirection	= FALSE;

					// Check each struct in each direction
					if ( pStructureFileRef == NULL )
					{
						fDirectionFound = TRUE;
					}
					else
					{
						for ( cnt3 = 0; cnt3 < NUM_WORLD_DIRECTIONS; ++cnt3 )
						{
							if ( OkayToAddStructureToWorld( sGridNo, pDef->bLevel, &(pStructureFileRef->pDBStructureRef[gOneCDirection[ cnt3 ]]), INVALID_STRUCTURE_ID ) )
							{
								fDirectionFound = TRUE;
								fCanSetDirection = TRUE;
								break;
							}
						}
					}

					if ( fDirectionFound )
					{
						uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );

						if ( uiRange < uiLowestRange )
						{
							if ( fCanSetDirection )
							{
								ubBestDirection = (UINT8)cnt3;
								fSetDirection	= TRUE;
							}
							sLowestGridNo = sGridNo;
							uiLowestRange = uiRange;
							fFound = TRUE;
						}
					}

				}
			}
		}
	}
	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;
	if ( fFound )
	{
		if ( fSetDirection )
		{
			pDef->ubDirection = ubBestDirection;
		}

		return sLowestGridNo;
	}
	return NOWHERE;
}


BOOLEAN IsValidDecapitationCorpse( ROTTING_CORPSE *pCorpse )
{
	if ( (pCorpse->def.usFlags & ROTTING_CORPSE_HEAD_TAKEN) )
	{
		return( FALSE );
	}

	return( gbCorpseValidForDecapitation[ pCorpse->def.ubType ] );
}


ROTTING_CORPSE *GetCorpseAtGridNo( INT32 sGridNo, INT8 bLevel )
{
	STRUCTURE				*pStructure, *pBaseStructure;
	INT32 sBaseGridNo;

	pStructure = FindStructure( sGridNo, STRUCTURE_CORPSE );

	if ( pStructure != NULL )
	{
		// Get base....
		pBaseStructure = FindBaseStructure( pStructure );

		if ( pBaseStructure != NULL )
		{
			// Find base gridno...
			sBaseGridNo = pBaseStructure->sGridNo;

			if (!TileIsOutOfBounds(sBaseGridNo))
			{
				return(FindCorpseBasedOnStructure(sBaseGridNo, bLevel, pBaseStructure));
			}			
		}
	}

	return( NULL );
}


BOOLEAN DecapitateCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel )
{
	ROTTING_CORPSE *pCorpse;
	ROTTING_CORPSE_DEFINITION CorpseDef;
	UINT16 usHeadIndex = HEAD_1;
	static UINT16 usBloodCatSkinIndex = 232;

	pCorpse = GetCorpseAtGridNo( sGridNo, bLevel );

	if ( pCorpse == NULL )
		return FALSE;
		
	if ( IsValidDecapitationCorpse( pCorpse ) )
	{
		// Decapitate.....
		// Copy corpse definition...
		memcpy( &CorpseDef, &(pCorpse->def), sizeof( ROTTING_CORPSE_DEFINITION ) );
				
		// Add new one...
		CorpseDef.ubType = gDecapitatedCorpse[ CorpseDef.ubType ];
				
		pCorpse->def.usFlags |= ROTTING_CORPSE_HEAD_TAKEN;
		CorpseDef.fHeadTaken = TRUE;
		
		if ( CorpseDef.ubType != 0 )
		{
			// Remove old one...
			RemoveCorpse( pCorpse->iID );

			INT32 iCorpseID = AddRottingCorpse( &CorpseDef );

			if ( iCorpseID != -1 )
			{
				gRottingCorpse[ iCorpseID ].def.usFlags |= ROTTING_CORPSE_HEAD_TAKEN;
			}
		}

		// Add head item.....

		// Pick the head based on profile type...
		switch( pCorpse->def.ubProfile )
		{
			case 83:
				usHeadIndex = HEAD_2;
				break;

			case 111:
				usHeadIndex = HEAD_3;
				break;

			case 64:
				usHeadIndex = HEAD_4;
				break;

			case 112:
				usHeadIndex = HEAD_5;
				break;

			case 82:
				usHeadIndex = HEAD_6;
				break;

			case 110:
				usHeadIndex = HEAD_7;
				break;

		}

		if (  pCorpse->def.ubType == BLOODCAT_DEAD )
		{
			if ( HasItemFlag(usBloodCatSkinIndex, SKIN_BLOODCAT) || GetFirstItemWithFlag(&usBloodCatSkinIndex, SKIN_BLOODCAT) )
				usHeadIndex = usBloodCatSkinIndex;
		}

		if ( usHeadIndex > 0 )
		{
			CreateItem( usHeadIndex, 100, &gTempObject );
			AddItemToPool( sGridNo, &gTempObject, INVISIBLE, bLevel, 0, 0 );

			// All teams lok for this...
			NotifySoldiersToLookforItems( );

			return TRUE;
		}
		else
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_HEAD_ITEM] );
	}
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_DECAPITATION] );

	return FALSE;
}

void ShiftCorpse( INT32 sGridNo, INT8 bLevel )
{
	for ( INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt )
	{
		ROTTING_CORPSE* pCorpse = &(gRottingCorpse[cnt]);

		if ( pCorpse && pCorpse->fActivated && pCorpse->def.ubType != 0 && pCorpse->def.sGridNo == sGridNo && pCorpse->def.bLevel == 1 )
		{
			ROTTING_CORPSE_DEFINITION CorpseDef;

			// Copy corpse definition...
			memcpy( &CorpseDef, &(pCorpse->def), sizeof(ROTTING_CORPSE_DEFINITION) );

			// Remove old one...
			RemoveCorpse( pCorpse->iID );

			// move corpse to the other level
			CorpseDef.bLevel = 1 - bLevel;

			AddRottingCorpse( &CorpseDef );
		}
	}
}

// Flugente: can this corpse be gutted?
BOOLEAN IsValidGutCorpse( ROTTING_CORPSE *pCorpse )
{
	if ( pCorpse->def.ubType == ROTTING_STAGE2 || (pCorpse->def.usFlags & ROTTING_CORPSE_GUTTED) )
		return( FALSE );

	return( pCorpse->def.ubType == BLOODCAT_DEAD || pCorpse->def.ubType == COW_DEAD );
}

// Flugente: gut a corpse
BOOLEAN GutCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo,  INT8 bLevel )
{
	ROTTING_CORPSE *pCorpse = GetCorpseAtGridNo( sGridNo, bLevel );

	if ( pCorpse == NULL )
		return FALSE;
		
	// can this thing be gutted?
	if ( IsValidGutCorpse( pCorpse ) )
	{
		// numbers for the meat acquired by gutting. These values respond to the standard GameDir values, if they have other values in the xml, no problem, we'll search for them
		static UINT16 usBloodCatMeatIndex = 1566;
		static UINT16 usCowMeatIndex = 1565;
		UINT16 meatindex = 0;

		pCorpse->def.usFlags |= ROTTING_CORPSE_GUTTED;

		if ( pCorpse->def.ubType == BLOODCAT_DEAD )
		{
			if ( HasItemFlag(usBloodCatMeatIndex, MEAT_BLOODCAT) || GetFirstItemWithFlag(&usBloodCatMeatIndex, MEAT_BLOODCAT) )
				meatindex = usBloodCatMeatIndex;
		}
		else
		{
			if ( HasItemFlag( usCowMeatIndex, MEAT_COW ) || GetFirstItemWithFlag( &usCowMeatIndex, MEAT_COW ) )
				meatindex = usCowMeatIndex;
		}

		if ( meatindex > 0 )
		{
			CreateItem( meatindex, 100, &gTempObject );
			AddItemToPool( sGridNo, &gTempObject, INVISIBLE, bLevel, 0, 0 );

			// All teams lok for this...
			NotifySoldiersToLookforItems( );

			return TRUE;
		}
		else
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_MEAT_ITEM] );
	}
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_GUTTING] );

	return FALSE;
}

// Flugente: can clothes be taken off of this corpse?
BOOLEAN IsValidStripCorpse( ROTTING_CORPSE *pCorpse )
{
	if ( pCorpse->def.ubType == ROTTING_STAGE2 || ((pCorpse->def.usFlags & ROTTING_CORPSE_NO_VEST) && (pCorpse->def.usFlags & ROTTING_CORPSE_NO_PANTS)) )
		return( FALSE );

	if ( pCorpse->def.ubType == BLOODCAT_DEAD )
		return TRUE;

	return( CorpseOkToDress(pCorpse) );
}

// Flugente: take the clothes off a corpse
BOOLEAN StripCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo,  INT8 bLevel )
{
	ROTTING_CORPSE *pCorpse = GetCorpseAtGridNo( sGridNo, bLevel );

	if ( pCorpse == NULL || !pSoldier )
		return FALSE;

	// can this thing be stripped?
	if ( IsValidStripCorpse( pCorpse ) )
	{
		if ( pCorpse->def.ubType == BLOODCAT_DEAD )
		{
			UINT16 peltitem = 232;
			if ( !HasItemFlag( peltitem, SKIN_BLOODCAT ) )
				GetFirstItemWithFlag( &peltitem, SKIN_BLOODCAT );

			// status depends on whether the cat pellet has holes in it, and whether we might have a bit of knowledge about skinning
			INT16 status = 70;
			if ( ( pCorpse->def.usFlags & ROTTING_CORPSE_NO_VEST ) )
				status -= 40;
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MELEE_NT ) )
				status += 5;
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, DOCTOR_NT ) )
				status += 5;

			CreateItem( peltitem, status + Random(20), &gTempObject );
			if ( !AutoPlaceObject( pSoldier, &gTempObject, FALSE ) )
				AddItemToPool( pSoldier->sGridNo, &gTempObject, VISIBLE, bLevel, 0, -1 );
		}
		else
		{
			// determine which clothes to spawn
			if ( !( pCorpse->def.usFlags & ROTTING_CORPSE_NO_VEST ) )
			{
				UINT16 vestitem = 0;
				if ( GetFirstClothesItemWithSpecificData( &vestitem, pCorpse->def.VestPal, "blank" ) )
				{
					CreateItem( vestitem, 100, &gTempObject );
					if ( !AutoPlaceObject( pSoldier, &gTempObject, FALSE ) )
						AddItemToPool( pSoldier->sGridNo, &gTempObject, VISIBLE, bLevel, 0, -1 );
				}
				else
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_NO_CLOTHES_ITEM] );
			}

			if ( !( pCorpse->def.usFlags & ROTTING_CORPSE_NO_PANTS ) )
			{
				UINT16 pantsitem = 0;
				if ( GetFirstClothesItemWithSpecificData( &pantsitem, "blank", pCorpse->def.PantsPal ) )
				{
					CreateItem( pantsitem, 100, &gTempObject );
					if ( !AutoPlaceObject( pSoldier, &gTempObject, FALSE ) )
						AddItemToPool( pSoldier->sGridNo, &gTempObject, VISIBLE, bLevel, 0, -1 );
				}
				else
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCovertTextStr[STR_COVERT_NO_CLOTHES_ITEM] );
			}
		}

		// we took the clothes, mark this
		pCorpse->def.usFlags |= (ROTTING_CORPSE_NO_VEST|ROTTING_CORPSE_NO_PANTS);

		return TRUE;
	}
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_CLOTHESFOUND] );

	return FALSE;
}

// Flugente: can this corpse be carried?
BOOLEAN IsValidTakeCorpse( ROTTING_CORPSE *pCorpse )
{
	if ( pCorpse->def.ubType >= BLOODCAT_DEAD || pCorpse->def.ubType == NO_CORPSE )
		return( FALSE );

	return( TRUE );
}

// Flugente: take a corpse into your hand, thereby removing it from the field
BOOLEAN TakeCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel )
{
	ROTTING_CORPSE *pCorpse = GetCorpseAtGridNo( sGridNo, bLevel );

	if ( pCorpse == NULL || !pSoldier )
		return FALSE;
	
	// can this corpse be picked up?
	if ( IsValidTakeCorpse( pCorpse ) )
	{
		// is there a 'corpse' item?
		UINT16 corpseitem = 0;
		if ( GetFirstItemWithFlag(&corpseitem, CORPSE)  )
		{
			// we have to make sure this can be placed in our hands, as corpses can only be carried in hands.
			// At this point, we will have a knife in our first hand, so check if our second hand is free
			if ( !(pSoldier->inv[SECONDHANDPOS].exists()) )
			{
				CreateItem( corpseitem, 100, &gTempObject );

				if(CanItemFitInPosition(pSoldier, &gTempObject, SECONDHANDPOS, FALSE))
				{
					// determine Body type
					if ( pCorpse->def.ubBodyType == BIGMALE || pCorpse->def.ubBodyType == STOCKYMALE || pCorpse->def.ubBodyType == FATCIV )
						gTempObject[0]->data.sObjectFlag |= CORPSE_M_BIG;
					else if ( pCorpse->def.ubBodyType == REGFEMALE || pCorpse->def.ubBodyType == MINICIV || pCorpse->def.ubBodyType == DRESSCIV )
						gTempObject[0]->data.sObjectFlag |= CORPSE_F;

					if ( pCorpse->def.usFlags & ROTTING_CORPSE_HEAD_TAKEN || pCorpse->def.fHeadTaken )
						gTempObject[0]->data.sObjectFlag |= CORPSE_NO_HEAD;

					if ( pCorpse->def.usFlags & ROTTING_CORPSE_NO_VEST )
						gTempObject[0]->data.sObjectFlag |= CORPSE_NO_VEST;

					if ( pCorpse->def.usFlags & ROTTING_CORPSE_NO_PANTS )
						gTempObject[0]->data.sObjectFlag |= CORPSE_NO_PANTS;

					if ( pCorpse->def.usFlags & ROTTING_CORPSE_NEVER_RISE_AGAIN )
						gTempObject[0]->data.sObjectFlag |= CORPSE_NO_ZOMBIE_RISE;
				
					// now we have to get the correct flags for the object from the corpse, so that upon recreating the corpse, it looks the same
					UINT8 headpal = 0, skinpal = 0, vestpal = 0, pantspal = 0;
					GetPaletteRepIndexFromID(pCorpse->def.HeadPal, &headpal);
					GetPaletteRepIndexFromID(pCorpse->def.SkinPal, &skinpal);
					GetPaletteRepIndexFromID(pCorpse->def.VestPal, &vestpal);
					GetPaletteRepIndexFromID(pCorpse->def.PantsPal, &pantspal);

					// sevenfm: reuse object data to store non-standard palette values
					UINT32 uiFlag = 0;
					uiFlag += (headpal & 0xff);
					uiFlag += (skinpal & 0xff) << 8;
					uiFlag += (vestpal & 0xff) << 16;
					uiFlag += (pantspal & 0xff) << 24;
					gTempObject[0]->data.ubWireNetworkFlag = uiFlag;
					// sevenfm: store palette flags
					gTempObject[0]->data.sRepairThreshold = pCorpse->def.usFlags & (ROTTING_CORPSE_USE_STEALTH_PALETTE | ROTTING_CORPSE_USE_CAMO_PALETTE | ROTTING_CORPSE_USE_URBAN_CAMO_PALETTE | ROTTING_CORPSE_USE_DESERT_CAMO_PALETTE | ROTTING_CORPSE_USE_SNOW_CAMO_PALETTE);

					switch ( headpal )
					{
					case 0:
						gTempObject[0]->data.sObjectFlag |= CORPSE_HAIR_BLOND;
						break;
					case 1:
						gTempObject[0]->data.sObjectFlag |= CORPSE_HAIR_BROWN;
						break;
					case 2:
						gTempObject[0]->data.sObjectFlag |= CORPSE_HAIR_BLACK;
						break;
					case 3:
						gTempObject[0]->data.sObjectFlag |= CORPSE_HAIR_WHITE;
						break;				
					case 4:
					default:
						// the default value (which is also used for red hair) is nothing. Upon spawning a corpse from an item, we assume that it has red hair if none of the above flags is found
						break;
					}

					switch ( skinpal )
					{
					case 13:
						gTempObject[0]->data.sObjectFlag |= CORPSE_SKIN_PINK;
						break;
					case 14:
						gTempObject[0]->data.sObjectFlag |= CORPSE_SKIN_TAN;
						break;
					case 15:
						gTempObject[0]->data.sObjectFlag |= CORPSE_SKIN_DARK;
						break;
					case 16:
					default:
						gTempObject[0]->data.sObjectFlag |= CORPSE_SKIN_BLACK;
						break;
					}

					switch ( vestpal )
					{
					case 17:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_BROWN;
						break;
					case 18:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_grey;
						break;
					case 19:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_GREEN;
						break;
					case 20:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_JEAN;
						break;
					case 21:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_RED;
						break;
					case 22:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_BLUE;
						break;
					case 23:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_YELLOW;
						break;
					case 24:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_WHITE;
						break;
					case 25:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_BLACK;
						break;
					case 26:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_GYELLOW;
						break;
					case 27:
					default:
						gTempObject[0]->data.sObjectFlag |= CORPSE_VEST_PURPLE;
						break;
					}

					switch ( pantspal )
					{
					case 7:
						gTempObject[0]->data.sObjectFlag |= CORPSE_PANTS_GREEN;
						break;
					case 8:
						gTempObject[0]->data.sObjectFlag |= CORPSE_PANTS_JEAN;
						break;
					case 9:
						gTempObject[0]->data.sObjectFlag |= CORPSE_PANTS_TAN;
						break;
					case 10:
						gTempObject[0]->data.sObjectFlag |= CORPSE_PANTS_BLACK;
						break;
					case 11:
						gTempObject[0]->data.sObjectFlag |= CORPSE_PANTS_BLUE;
						break;
					case 12:
					default:
						gTempObject[0]->data.sObjectFlag |= CORPSE_PANTS_BEIGE;
						break;
					}

					gTempObject.MoveThisObjectTo(pSoldier->inv[SECONDHANDPOS], ALL_OBJECTS, pSoldier, SECONDHANDPOS);

					if ( pCorpse->def.ubType != 0 )
					{
						// Remove corpse...
						RemoveCorpse( pCorpse->iID );
					}
				}

				return TRUE;
			}
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_FREEHAND] );
		}
		else
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_CORPSE_ITEM] );
	}
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_NO_TAKING] );

	return FALSE;
}

// Flugente: create a corpse from an object and place it in the world
BOOLEAN AddCorpseFromObject(OBJECTTYPE* pObj, INT32 sGridNo, INT8 bLevel )
{
	if ( !pObj || !HasItemFlag(pObj->usItem, CORPSE) )
		return FALSE;

	ROTTING_CORPSE_DEFINITION		Corpse;

	if ( (*pObj)[0]->data.sObjectFlag & CORPSE_M_BIG )
	{
		if ( (*pObj)[0]->data.sObjectFlag & CORPSE_NO_HEAD )
			Corpse.ubType = MMERC_JFK;
		else
			Corpse.ubType = MMERC_BCK;

		Corpse.ubBodyType = BIGMALE;
	}
	else if ( (*pObj)[0]->data.sObjectFlag & CORPSE_F )
	{
		if ( (*pObj)[0]->data.sObjectFlag & CORPSE_NO_HEAD )
			Corpse.ubType = FMERC_JFK;
		else
			Corpse.ubType = FMERC_BCK;

		Corpse.ubBodyType = REGFEMALE;
	}
	else //if ( (*pObj)[0]->data.sObjectFlag & CORPSE_M_SMALL )
	{
		if ( (*pObj)[0]->data.sObjectFlag & CORPSE_NO_HEAD )
			Corpse.ubType = SMERC_JFK;
		else
			Corpse.ubType = SMERC_BCK;

		Corpse.ubBodyType = REGMALE;
	}

	INT16 sX, sY;
	ConvertGridNoToCenterCellXY(sGridNo, &sX, &sY);

	Corpse.sGridNo = sGridNo;
	Corpse.dXPos = sX;
	Corpse.dYPos = sY;

	Corpse.sHeightAdjustment = 0;

	UINT32 uiFlag = (*pObj)[0]->data.ubWireNetworkFlag;
	INT16 sCamoFlag = (*pObj)[0]->data.sRepairThreshold;

	if (uiFlag > 0)
	{
		// sevenfm: set values directly from stored IDs
		UINT8 headpal = (uiFlag & 0xff);
		UINT8 skinpal = ((uiFlag >> 8) & 0xff);
		UINT8 vestpal = ((uiFlag >> 16) & 0xff);
		UINT8 pantspal = ((uiFlag >> 24) & 0xff);

		SET_PALETTEREP_ID(Corpse.HeadPal, gpPalRep[headpal].ID);
		SET_PALETTEREP_ID(Corpse.SkinPal, gpPalRep[skinpal].ID);
		SET_PALETTEREP_ID(Corpse.VestPal, gpPalRep[vestpal].ID);
		SET_PALETTEREP_ID(Corpse.PantsPal, gpPalRep[pantspal].ID);

		// also restore camo status
		if (sCamoFlag > 0)
		{
			Corpse.usFlags |= sCamoFlag;
		}
	}
	else
	{
		// sevenfm: use default way for standard palette
		// check the objects flagmask and set the corpse palette IDs accordingly

		// Hair
		if ((*pObj)[0]->data.sObjectFlag & CORPSE_HAIR_BROWN)
			SET_PALETTEREP_ID(Corpse.HeadPal, "BROWNHEAD");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_HAIR_BLACK)
			SET_PALETTEREP_ID(Corpse.HeadPal, "BLACKHEAD");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_HAIR_WHITE)
			SET_PALETTEREP_ID(Corpse.HeadPal, "WHITEHEAD");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_HAIR_BLOND)
			SET_PALETTEREP_ID(Corpse.HeadPal, "BLONDHEAD");
		else
			SET_PALETTEREP_ID(Corpse.HeadPal, "REDHEAD");

		// Skin
		if ((*pObj)[0]->data.sObjectFlag & CORPSE_SKIN_PINK)
			SET_PALETTEREP_ID(Corpse.SkinPal, "PINKSKIN");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_SKIN_TAN)
			SET_PALETTEREP_ID(Corpse.SkinPal, "TANSKIN");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_SKIN_DARK)
			SET_PALETTEREP_ID(Corpse.SkinPal, "DARKSKIN");
		else
			SET_PALETTEREP_ID(Corpse.SkinPal, "BLACKSKIN");

		// Vest
		if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_BROWN)
			SET_PALETTEREP_ID(Corpse.VestPal, "BROWNVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_grey)
			SET_PALETTEREP_ID(Corpse.VestPal, "greyVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_GREEN)
			SET_PALETTEREP_ID(Corpse.VestPal, "GREENVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_JEAN)
			SET_PALETTEREP_ID(Corpse.VestPal, "JEANVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_RED)
			SET_PALETTEREP_ID(Corpse.VestPal, "REDVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_BLUE)
			SET_PALETTEREP_ID(Corpse.VestPal, "BLUEVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_YELLOW)
			SET_PALETTEREP_ID(Corpse.VestPal, "YELLOWVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_WHITE)
			SET_PALETTEREP_ID(Corpse.VestPal, "WHITEVEST");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_BLACK)
			SET_PALETTEREP_ID(Corpse.VestPal, "BLACKSHIRT");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_VEST_GYELLOW)
			SET_PALETTEREP_ID(Corpse.VestPal, "GYELLOWSHIRT");
		else
			SET_PALETTEREP_ID(Corpse.VestPal, "PURPLESHIRT");

		// Pants
		if ((*pObj)[0]->data.sObjectFlag & CORPSE_PANTS_GREEN)
			SET_PALETTEREP_ID(Corpse.PantsPal, "GREENPANTS");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_PANTS_JEAN)
			SET_PALETTEREP_ID(Corpse.PantsPal, "JEANPANTS");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_PANTS_TAN)
			SET_PALETTEREP_ID(Corpse.PantsPal, "TANPANTS");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_PANTS_BLACK)
			SET_PALETTEREP_ID(Corpse.PantsPal, "BLACKPANTS");
		else if ((*pObj)[0]->data.sObjectFlag & CORPSE_PANTS_BLUE)
			SET_PALETTEREP_ID(Corpse.PantsPal, "BLUEPANTS");
		else
			SET_PALETTEREP_ID(Corpse.PantsPal, "BEIGEPANTS");
	}
	
	Corpse.ubDirection = NORTH;
	Corpse.uiTimeOfDeath = GetWorldTotalMin();

	Corpse.usFlags = 0;		// no flags here, at least not the new ones

	Corpse.bLevel = bLevel;

	Corpse.bVisible = 1;
	Corpse.bNumServicingCrows = 0;
	Corpse.ubProfile = NO_PROFILE;
	Corpse.fHeadTaken = FALSE;
	Corpse.ubAIWarningValue = 20;

	// Flugente: use zombie name (it's the only way this will ever be relevant again anyway)
	swprintf( Corpse.name, TacticalStr[ ZOMBIE_TEAM_MERC_NAME ] );
	Corpse.name[9] = '\0';
	
	INT32 iCorpseID = AddRottingCorpse( &Corpse );

	if ( iCorpseID != -1 )
	{		
		AllMercsOnTeamLookForCorpse(&( gRottingCorpse[ iCorpseID ] ), OUR_TEAM);

		if ( (*pObj)[0]->data.sObjectFlag & CORPSE_NO_HEAD )
			gRottingCorpse[ iCorpseID ].def.usFlags |= ROTTING_CORPSE_HEAD_TAKEN;

		if ( (*pObj)[0]->data.sObjectFlag & CORPSE_NO_VEST )
			gRottingCorpse[ iCorpseID ].def.usFlags |= ROTTING_CORPSE_NO_VEST;

		if ( (*pObj)[0]->data.sObjectFlag & CORPSE_NO_PANTS )
			gRottingCorpse[ iCorpseID ].def.usFlags |= ROTTING_CORPSE_NO_PANTS;

		if ( (*pObj)[0]->data.sObjectFlag & CORPSE_NO_ZOMBIE_RISE )
			gRottingCorpse[ iCorpseID ].def.usFlags |= ROTTING_CORPSE_NEVER_RISE_AGAIN;

		return TRUE;
	}
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szCorpseTextStr[STR_CORPSE_INVALID_CORPSE_ID] );

	return FALSE;
}


void GetBloodFromCorpse( SOLDIERTYPE *pSoldier )
{
	ROTTING_CORPSE *pCorpse;
	INT8						bObjSlot;
	//OBJECTTYPE			Object = {};

	// OK, get corpse
	pCorpse = &( gRottingCorpse[ pSoldier->aiData.uiPendingActionData4 ] );

	bObjSlot = FindObj( pSoldier, JAR );

	// What kind of corpse ami I?
	switch( pCorpse->def.ubType )
	{
		case ADULTMONSTER_DEAD:
		case INFANTMONSTER_DEAD:
			// Can get creature blood....
			CreateItem( JAR_CREATURE_BLOOD, 100, &gTempObject );
			break;

	case QUEEN_MONSTER_DEAD:
			CreateItem( JAR_QUEEN_CREATURE_BLOOD, 100, &gTempObject );
			break;

		default:
			CreateItem( JAR_HUMAN_BLOOD, 100, &gTempObject );
			break;
	}

	if ( bObjSlot != NO_SLOT )
	{
		SwapObjs( pSoldier, bObjSlot, &gTempObject, TRUE );
	}
}

void ReduceAmmoDroppedByNonPlayerSoldiers( SOLDIERTYPE *pSoldier, INT32 iInvSlot )
{
	Assert( pSoldier );
	Assert( ( iInvSlot >= 0 ) && ( iInvSlot < (INT32)pSoldier->inv.size() ) );

	// if not a player soldier
	if ( pSoldier->bTeam != gbPlayerNum )
	{
		OBJECTTYPE *pObj = &( pSoldier->inv[ iInvSlot ] );

		// if it's ammo
		if ( Item[ pObj->usItem ].usItemClass == IC_AMMO
		&& Magazine[Item[pObj->usItem].ubClassIndex].ubMagType != AMMO_BOX
		&& Magazine[Item[pObj->usItem].ubClassIndex].ubMagType != AMMO_CRATE)
		{
			//don't drop all the clips, just a random # of them between 1 and how many there are
			pObj->ubNumberOfObjects = ( UINT8 ) ( 1 + Random( pObj->ubNumberOfObjects ) );
			// recalculate the weight
			//ADB ubWeight has been removed, see comments in OBJECTTYPE
			//pObj->ubWeight = CalculateObjectWeight( pObj );
		}
	}
}

void ReduceAttachmentsOnGunForNonPlayerChars(SOLDIERTYPE *pSoldier, OBJECTTYPE * pObj){
	
	//Not meant for use in OAS.
	Assert(UsingNewAttachmentSystem()==true);
		
	//If this item has any attachments, is not from a player, and is overwriteable. It's also only for guns.
	if((*pObj)[0]->AttachmentListSize() > 0 && pSoldier->bTeam != gbPlayerNum && !((*pObj).fFlags & OBJECT_NO_OVERWRITE) && Item[pObj->usItem].usItemClass == IC_GUN && !(gGameExternalOptions.fEnemiesDropAllItems == 1) )
	{
		UINT8 slotCount = 0;
		for(std::list<OBJECTTYPE>::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter, ++slotCount)
		{
			//A few conditions under which we will not delete this attachment
			if(!iter->exists())
				continue;

			//Loop to find out if this attachment is a default one, if so, we don't want to remove it.
			int i;
			for(i = 0; i < MAX_DEFAULT_ATTACHMENTS && Item[pObj->usItem].defaultattachments[i] != iter->usItem; i++){}

			// with mild drop all attachments of undropable items need to be damaged as well as the base item
			if( gGameExternalOptions.fEnemiesDropAllItems == 2 && pSoldier->bTeam == ENEMY_TEAM && (*pObj).fFlags & OBJECT_UNDROPPABLE )
			{
				(*iter)[0]->data.objectStatus -= 60 + Random( zDiffSetting[gGameOptions.ubDifficultyLevel].usLootStatusModifier );
				(*iter)[0]->data.objectStatus = min(max((*iter)[0]->data.objectStatus,1),100); // never below 1% or above 100%
			}

			if(Item[pObj->usItem].defaultattachments[i] == iter->usItem)
				continue;

			//Erase this attachment or not?
			if(!Chance(gGameExternalOptions.usAttachmentDropRate))
			{
				(*pObj).RemoveAttachment(&(*iter), 0, 0, pSoldier, 0, 1);
				iter = (*pObj)[0]->attachments.begin();
				advance(iter, slotCount);
			}
		}
	}
}

void LookForAndMayCommentOnSeeingCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubLevel )
{
	ROTTING_CORPSE *pCorpse;
	INT8			bToleranceThreshold = 0;
	INT32			cnt;
	SOLDIERTYPE		*pTeamSoldier;

	if ( QuoteExp[ pSoldier->ubProfile ].QuoteExpHeadShotOnly == 1 )
	{
		return;
	}

	pCorpse = GetCorpseAtGridNo( sGridNo, ubLevel );

	if ( pCorpse == NULL )
	{
		return;
	}

	if ( pCorpse->def.ubType != ROTTING_STAGE2 )
	{
		return;
	}

	// If servicing qrows, tolerance is now 1
	if ( pCorpse->def.bNumServicingCrows > 0 )
	{
		++bToleranceThreshold;
	}

	// Check tolerance
	if ( pSoldier->bCorpseQuoteTolerance <= bToleranceThreshold )
	{
		// Say quote...
		TacticalCharacterDialogue( pSoldier, QUOTE_HEADSHOT );

		BeginMultiPurposeLocator( sGridNo, ubLevel, FALSE );

		// Reset values....
		pSoldier->bCorpseQuoteTolerance = (INT8)( Random(3) + 1 );

		// 50% chance of adding 1 to other mercs....
		if ( Random( 2 ) == 1 )
		{
			// IF IT'S THE SELECTED GUY, MAKE ANOTHER SELECTED!
			cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

			// look for all mercs on the same team,
			for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
			{
				// ATE: Ok, lets check for some basic things here!				
				if ( pTeamSoldier->stats.bLife >= OKLIFE && !TileIsOutOfBounds(pTeamSoldier->sGridNo) && pTeamSoldier->bActive && pTeamSoldier->bInSector )
				{
					pTeamSoldier->bCorpseQuoteTolerance++;
				}
			}
		}
	}
}

INT32 GetGridNoOfCorpseGivenProfileID( UINT8 ubProfileID )
{
	ROTTING_CORPSE*	pCorpse;

	// Loop through all corpses....
	for ( INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt )
	{
		pCorpse = &(gRottingCorpse[ cnt ] );

		if ( pCorpse->fActivated )
		{
			if ( pCorpse->def.ubProfile == ubProfileID )
			{
				return( pCorpse->def.sGridNo );
			}
		}
	}

	return( NOWHERE );
}

void DecayRottingCorpseAIWarnings( void )
{
	ROTTING_CORPSE*	pCorpse;

	for ( INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt )
	{
		pCorpse = &(gRottingCorpse[ cnt ] );

		if ( pCorpse->fActivated && pCorpse->def.ubAIWarningValue > 0 )
		{
			pCorpse->def.ubAIWarningValue--;
		}
	}
}

UINT8 GetNearestRottingCorpseAIWarning( INT32 sGridNo )
{
	ROTTING_CORPSE* pCorpse;
	UINT8			ubHighestWarning = 0;

	for ( INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt )
	{
		pCorpse = &(gRottingCorpse[ cnt ] );

		if ( pCorpse->fActivated && pCorpse->def.ubAIWarningValue > 0 )
		{
			if ( PythSpacesAway( sGridNo, pCorpse->def.sGridNo ) <= CORPSE_WARNING_DIST )
			{
				if ( pCorpse->def.ubAIWarningValue > ubHighestWarning )
				{
					ubHighestWarning = pCorpse->def.ubAIWarningValue;
				}
			}
		}
	}

	return( ubHighestWarning );
}

ROTTING_CORPSE* GetRottingCorpse( INT16 aNum )
{
	if ( aNum < giNumRottingCorpse )
		return &(gRottingCorpse[aNum]);

	return NULL;
}

std::vector<INT16> GetCorpseIDsNearGridNo( INT32 sGridNo, INT8 bLevel, INT8 sRadius )
{
	std::vector<INT16> idvec;

	ROTTING_CORPSE* pCorpse;

	for ( INT32 cnt = 0; cnt < giNumRottingCorpse; ++cnt )
	{
		pCorpse = &(gRottingCorpse[cnt]);

		if ( pCorpse->fActivated && pCorpse->def.bLevel == bLevel )
		{
			if ( PythSpacesAway( sGridNo, pCorpse->def.sGridNo ) <= sRadius )
			{
				idvec.push_back(cnt);
			}
		}
	}

	return idvec;
}

extern UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ );
INT16 gsZombieRaiseSoundNum = -1;

// Flugente Zombies: resurrect zombies
void RaiseZombies( void )
{
	if ( gGameSettings.fOptions[TOPTION_ZOMBIES] )
	{
		// if gGameExternalOptions.fZombieSpawnWaves is true, zombies will spawn from all corpses (while there is still room for more), creating a wave of zombies. with lots of bodies	lying around, this can be a lot.
		// if GameExternalOptions.fZombieSpawnWaves is false, each zombie can spawn randomly, you will get zombies on msot turns, but they won't spawn a whole horde at once
		if ( !gGameExternalOptions.fZombieSpawnWaves || ( gGameExternalOptions.fZombieSpawnWaves && (INT8) ( Random( 100 ) ) > 100 - gGameExternalOptions.sZombieRiseWaveFrequency ) )
		{
			ROTTING_CORPSE *	pCorpse;
			BOOLEAN				zombieshaverisen = FALSE;

			// silversurfer: We need to check if we are above or below ground or we will modify the wrong sector counters!
			// above ground
			if (!gbWorldSectorZ)
			{
				SECTORINFO *pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

				for ( INT32 cnt = giNumRottingCorpse - 1; cnt >= 0; --cnt )
				{
					if ( pSector && pSector->ubNumCreatures < gGameExternalOptions.ubGameMaximumNumberOfCreatures )			// ... if there is still room for more zombies (zombies count as creatures until a separate ZOMBIE_TEAM is implemented)...
					{
						pCorpse = &(gRottingCorpse[ cnt ] );

						// if zombies should spawn individually, roll for every corpse individually
						if ( gGameExternalOptions.fZombieSpawnWaves || ( !gGameExternalOptions.fZombieSpawnWaves && (INT8) ( Random( 100 ) ) > 100 - gGameExternalOptions.sZombieRiseWaveFrequency ) )
						{
							if ( pCorpse->fActivated && !(pCorpse->def.usFlags & (ROTTING_CORPSE_HEAD_TAKEN|ROTTING_CORPSE_NEVER_RISE_AGAIN) ) )	// ... if corpse is active, and still has a head and can rise again...
							{
								if ( !TileIsOutOfBounds(pCorpse->def.sGridNo)  )											// ... if corpse is on existing coordinates ...
								{					
									if ( WhoIsThere2( pCorpse->def.sGridNo, pCorpse->def.bLevel ) == NOBODY )				// ... if nobody else is on that position ...
									{
										UINT16 recanimstate = STANDING;

										if ( CorpseOkToSpawnZombie( pCorpse, &recanimstate ) )								// ... a zombie can be created from this corpse, in the corresponding animstate ...
										{								
											zombieshaverisen = TRUE;
											CreateZombiefromCorpse( pCorpse, recanimstate );

											pSector->ubNumCreatures++;
											pSector->ubCreaturesInBattle++;

											RemoveCorpse( cnt );
										}
									}
								}
							}
						}
					}
					else
					{
						// if there is no more room, we can skip this
						break;
					}
				}
			}
			else // below ground
			{
				UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

				for ( INT32 cnt = giNumRottingCorpse - 1; cnt >= 0; --cnt )
				{
					if ( pSector && pSector->ubNumCreatures < gGameExternalOptions.ubGameMaximumNumberOfCreatures )			// ... if there is still room for more zombies (zombies count as creatures until a separate ZOMBIE_TEAM is implemented)...
					{
						pCorpse = &(gRottingCorpse[ cnt ] );

						// if zombies should spawn individually, roll for every corpse individually
						if ( gGameExternalOptions.fZombieSpawnWaves || ( !gGameExternalOptions.fZombieSpawnWaves && (INT8) ( Random( 100 ) ) > 100 - gGameExternalOptions.sZombieRiseWaveFrequency ) )
						{
							if ( pCorpse->fActivated && !(pCorpse->def.usFlags & (ROTTING_CORPSE_HEAD_TAKEN|ROTTING_CORPSE_NEVER_RISE_AGAIN) ) )	// ... if corpse is active, and still has a head and can rise again...
							{
								if ( !TileIsOutOfBounds(pCorpse->def.sGridNo)  )											// ... if corpse is on existing coordinates ...
								{					
									if ( WhoIsThere2( pCorpse->def.sGridNo, pCorpse->def.bLevel ) == NOBODY )				// ... if nobody else is on that position ...
									{
										UINT16 recanimstate = STANDING;

										if ( CorpseOkToSpawnZombie( pCorpse, &recanimstate ) )								// ... a zombie can be created from this corpse, in the corresponding animstate ...
										{								
											zombieshaverisen = TRUE;
											CreateZombiefromCorpse( pCorpse, recanimstate );

											pSector->ubNumCreatures++;
											pSector->ubCreaturesInBattle++;

											RemoveCorpse( cnt );
										}
									}
								}
							}
						}
					}
					else
					{
						// if there is no more room, we can skip this
						break;
					}
				}
			}

			if ( zombieshaverisen )
			{
				SetRenderFlags( RENDER_FLAG_FULL );

#ifdef JA2TESTVERSION
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"A wave of zombies is created");
#endif
				CHAR8	zFilename[512];
				// prepare zombie raise sound
				if (gsZombieRaiseSoundNum < 0)
				{
					gsZombieRaiseSoundNum = 0;
					do
					{
						gsZombieRaiseSoundNum++;
						sprintf(zFilename, "Sounds\\Misc\\ZombieRaise%d.ogg", gsZombieRaiseSoundNum);
					} while (FileExists(zFilename));
					gsZombieRaiseSoundNum--;
				}
				if (gsZombieRaiseSoundNum > 0)
				{
					sprintf(zFilename, "Sounds\\Misc\\ZombieRaise%d.ogg", Random(gsZombieRaiseSoundNum) + 1);
					if (FileExists(zFilename))
					{
						//PlayJA2SampleFromFile(zFilename, RATE_11025, SoundVolume(MIDVOLUME, this->sGridNo), 1, SoundDir(this->sGridNo));
						PlayJA2SampleFromFile(zFilename, RATE_11025, MIDVOLUME, 1, MIDDLEPAN);
					}
				}
				else
				{
					// Play default sound
					PlayJA2SampleFromFile("Sounds\\zombie1.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN);
				}

				UseCreatureMusic(TRUE); // Madd: music when zombies rise
#ifdef NEWMUSIC
				GlobalSoundID  = MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalTensor[gbWorldSectorZ];
				if ( MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalTensor[gbWorldSectorZ] != -1 )
					SetMusicModeID( MUSIC_TACTICAL_ENEMYPRESENT, MusicSoundValues[ SECTOR( gWorldSectorX, gWorldSectorY ) ].SoundTacticalTensor[gbWorldSectorZ] );
				else
#endif
				SetMusicMode( MUSIC_TACTICAL_ENEMYPRESENT );

				// Flugente: additional dialogue
				AdditionalTacticalCharacterDialogue_AllInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, NO_PROFILE, ADE_ZOMBIESRISE );

				// alert the creatures
				HandleInitialRedAlert( CREATURE_TEAM, FALSE );

				// we need this so that the battle is properly recognized in strategic
				{
					//Set up the location as well as turning on the blit flag.
					gubPBSectorX = (UINT8)gWorldSectorX;
					gubPBSectorY = (UINT8)gWorldSectorY;
					gubPBSectorZ = (UINT8)gbWorldSectorZ;
					gfBlitBattleSectorLocator = TRUE;
				}

				if ( GetEnemyEncounterCode() == NO_ENCOUNTER_CODE )
					SetEnemyEncounterCode( ZOMBIE_ATTACK_CODE );

				// idea... potentially scrap this part and use a simplified form of void CreatureAttackTown_OtherCreatures(...) ?

				//gubSectorIDOfCreatureAttack = SECTOR( gWorldSectorX, gWorldSectorY );
			}
		}
	}
}

// Flugente: create a zombie from a corpse
void CreateZombiefromCorpse( ROTTING_CORPSE *	pCorpse, UINT16 usAnimState )
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;

	MercCreateStruct.ubProfile			= NO_PROFILE;
	MercCreateStruct.bTeam				= CREATURE_TEAM;	// should be ZOMBIE_TEAM, once that is properly implemented
	MercCreateStruct.sInsertionGridNo	= pCorpse->def.sGridNo;
	MercCreateStruct.sSectorX			= gWorldSectorX;
	MercCreateStruct.sSectorY			= gWorldSectorY;
	MercCreateStruct.bSectorZ			= gbWorldSectorZ;
	MercCreateStruct.ubBodyType			= pCorpse->def.ubBodyType;
	MercCreateStruct.ubDirection		= pCorpse->def.ubDirection;
	MercCreateStruct.fOnRoof			= pCorpse->def.bLevel > 0 ? TRUE : FALSE;
			
	// add important stats according to difficulty level
	switch( gGameExternalOptions.sZombieDifficultyLevel )
	{
		case 4:
			MercCreateStruct.bLifeMax		= (INT8)( 70 + Random( 30 ) );
			MercCreateStruct.bLife			= MercCreateStruct.bLifeMax;
			MercCreateStruct.bAgility		= (INT8)( 50 + Random( 10 ) );
			MercCreateStruct.bDexterity		= (INT8)( 60 + Random( 15 ) );
			MercCreateStruct.bStrength		= (INT8)( 80 + Random( 20 ) );
			break;
				
		case 3:
			MercCreateStruct.bLifeMax		= (INT8)( 60 + Random( 20 ) );
			MercCreateStruct.bLife			= MercCreateStruct.bLifeMax;
			MercCreateStruct.bAgility		= (INT8)( 40 + Random( 10 ) );
			MercCreateStruct.bDexterity		= (INT8)( 45 + Random( 10 ) );
			MercCreateStruct.bStrength		= (INT8)( 60 + Random( 20 ) );
			break;

		case 2:
			MercCreateStruct.bLifeMax		= (INT8)( 45 + Random( 15 ) );
			MercCreateStruct.bLife			= MercCreateStruct.bLifeMax;
			MercCreateStruct.bAgility		= (INT8)( 30 + Random(  5 ) );
			MercCreateStruct.bDexterity		= (INT8)( 30 + Random( 10 ) );
			MercCreateStruct.bStrength		= (INT8)( 45 + Random( 20 ) );
			break;

		case 1:
		default:
			MercCreateStruct.bLifeMax		= (INT8)( 35 + Random( 10 ) );
			MercCreateStruct.bLife			= MercCreateStruct.bLifeMax;
			MercCreateStruct.bAgility		= (INT8)( 15 + Random(  5 ) );
			MercCreateStruct.bDexterity		= (INT8)( 15 + Random(  5 ) );
			MercCreateStruct.bStrength		= (INT8)( 30 + Random( 20 ) );
			break;
	}

	// FIX: something's wrong with the FATCIV-bodytype when dying... this is to ensure they die absolutely fast
	//if ( MercCreateStruct.ubBodyType == FATCIV )
		//MercCreateStruct.bLifeMax		= OKLIFE;
			
	MercCreateStruct.bExpLevel		= 1;
	MercCreateStruct.bMarksmanship	= 1;
	MercCreateStruct.bMedical		= 1;
	MercCreateStruct.bMechanical	= 1;
	MercCreateStruct.bExplosive		= 1;
	MercCreateStruct.bLeadership	= 1;
	MercCreateStruct.bWisdom		= 1;
	MercCreateStruct.bMorale		= 90;
	MercCreateStruct.bAIMorale		= MORALE_FEARLESS;
			
	SET_PALETTEREP_ID ( MercCreateStruct.HeadPal,		pCorpse->def.HeadPal );
	SET_PALETTEREP_ID ( MercCreateStruct.PantsPal,		pCorpse->def.PantsPal );
	SET_PALETTEREP_ID ( MercCreateStruct.VestPal,		pCorpse->def.VestPal );
	SET_PALETTEREP_ID ( MercCreateStruct.SkinPal,		pCorpse->def.SkinPal );
																								
	MercCreateStruct.fVisible			= TRUE;

	INT8							iNewIndex;
	if ( TacticalCreateSoldier( &MercCreateStruct, (UINT8 *)&iNewIndex ) )
	{
		/*	certain values have to be set afterwards - the alternative would be to edit each and every function that gets called from TacticalCreateSoldier() subsequently and
		*	make an exception for zombies every time...
		*/
		SOLDIERTYPE* pNewSoldier = MercPtrs[ (UINT8)iNewIndex ];
			
		pNewSoldier->bActionPoints			= 60;
		pNewSoldier->bInitialActionPoints	= 60;
		pNewSoldier->sBreathRed				= 0;

		pNewSoldier->ubInsertionDirection	= pCorpse->def.ubDirection;

		//pNewSoldier->sHeightAdjustment		= pCorpse->def.sHeightAdjustment;
		pNewSoldier->sDesiredHeight			= 3;

		pNewSoldier->ubDesiredHeight		= 3;		// this forces pNewSoldier to rise up to crouching position

		pNewSoldier->ubSoldierClass			= SOLDIER_CLASS_ZOMBIE;
		pNewSoldier->aiData.bOrders			= SEEKENEMY;
		pNewSoldier->aiData.bAttitude		= AGGRESSIVE;

		pNewSoldier->bBleeding				= 0;

		if (   !memcmp( pCorpse->name, TacticalStr[ CIV_TEAM_MINER_NAME ], sizeof(pCorpse->name) ) 
			|| !memcmp( pCorpse->name, TacticalStr[ MILITIA_TEAM_MERC_NAME ], sizeof(pCorpse->name) )
			|| !memcmp( pCorpse->name, TacticalStr[ CREATURE_TEAM_MERC_NAME ], sizeof(pCorpse->name) ) 
			|| !memcmp( pCorpse->name, TacticalStr[ CIV_TEAM_MERC_NAME ], sizeof(pCorpse->name) ) 
			|| !memcmp( pCorpse->name, TacticalStr[ ZOMBIE_TEAM_MERC_NAME ], sizeof(pCorpse->name) ) 
			|| !memcmp( pCorpse->name, TacticalStr[ ENEMY_TEAM_MERC_NAME ], sizeof(pCorpse->name) ) )
		{
			swprintf( pNewSoldier->name, TacticalStr[ ZOMBIE_TEAM_MERC_NAME ] );
		}
		else
		{
			memcpy( &(pNewSoldier->name), &(pCorpse->name), sizeof(CHAR16) * 10 );
			pNewSoldier->name[9] = '\0';
		}
			
		// add skills according to difficulty level
		switch( gGameExternalOptions.sZombieDifficultyLevel )
		{
			case 4:
				if ( gGameOptions.fNewTraitSystem )
				{
				pNewSoldier->stats.ubSkillTraits[0] = MARTIAL_ARTS_NT;
				pNewSoldier->stats.ubSkillTraits[1] = MARTIAL_ARTS_NT;
				pNewSoldier->stats.ubSkillTraits[2] = ATHLETICS_NT;
				}
				else
				{
				pNewSoldier->stats.ubSkillTraits[0] = HANDTOHAND_OT;
				pNewSoldier->stats.ubSkillTraits[1] = MARTIALARTS_OT;
				}
				break;
				
			case 3:
				if ( gGameOptions.fNewTraitSystem )
				{
				pNewSoldier->stats.ubSkillTraits[0] = MARTIAL_ARTS_NT;
				pNewSoldier->stats.ubSkillTraits[1] = ATHLETICS_NT;
				pNewSoldier->stats.ubSkillTraits[2] = NO_SKILLTRAIT_NT;
				}
				else
				{
				pNewSoldier->stats.ubSkillTraits[0] = HANDTOHAND_OT;
				pNewSoldier->stats.ubSkillTraits[1] = NO_SKILLTRAIT_OT;
				}
				break;

			case 2:
				if ( gGameOptions.fNewTraitSystem )
				{
				pNewSoldier->stats.ubSkillTraits[0] = MARTIAL_ARTS_NT;
				pNewSoldier->stats.ubSkillTraits[1] = NO_SKILLTRAIT_NT;
				pNewSoldier->stats.ubSkillTraits[2] = NO_SKILLTRAIT_NT;
				}
				else
				{
				pNewSoldier->stats.ubSkillTraits[0] = NO_SKILLTRAIT_OT;
				pNewSoldier->stats.ubSkillTraits[1] = NO_SKILLTRAIT_OT;
				}
				break;

			case 1:
			default:
				if ( gGameOptions.fNewTraitSystem )
				{
				pNewSoldier->stats.ubSkillTraits[0] = NO_SKILLTRAIT_NT;
				pNewSoldier->stats.ubSkillTraits[1] = NO_SKILLTRAIT_NT;
				pNewSoldier->stats.ubSkillTraits[2] = NO_SKILLTRAIT_NT;
				}
				else
				{
				pNewSoldier->stats.ubSkillTraits[0] = NO_SKILLTRAIT_OT;
				pNewSoldier->stats.ubSkillTraits[1] = NO_SKILLTRAIT_OT;
				}
				break;
		}
					
		AddSoldierToSectorNoCalculateDirectionUseAnimation( iNewIndex, usAnimState, 0 );

		// If this corpse has camo, use palette from hvobject
		if ( pCorpse->def.ubType == ROTTING_STAGE2 )
		{
			memcpy( pNewSoldier->p8BPPPalette, gpTileCache[ pCorpse->iCachedTileID ].pImagery->vo->pPaletteEntry, sizeof( pCorpse->p8BPPPalette ) * 256 );
		}
		else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_CAMO_PALETTE )
		{
			pNewSoldier->bCamo = gGameExternalOptions.bCamoKitArea;
			pNewSoldier->wornCamo = __max(0, ( 65 - gGameExternalOptions.bCamoKitArea ) );
		}
		else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_URBAN_CAMO_PALETTE )
		{
			pNewSoldier->urbanCamo = gGameExternalOptions.bCamoKitArea;
			pNewSoldier->wornUrbanCamo = __max(0, ( 65 - gGameExternalOptions.bCamoKitArea ) );
		}
		else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_DESERT_CAMO_PALETTE )
		{
			pNewSoldier->desertCamo = gGameExternalOptions.bCamoKitArea;
			pNewSoldier->wornDesertCamo = __max(0, ( 65 - gGameExternalOptions.bCamoKitArea ) );
		}
		else if ( pCorpse->def.usFlags & ROTTING_CORPSE_USE_SNOW_CAMO_PALETTE )
		{
			pNewSoldier->snowCamo = gGameExternalOptions.bCamoKitArea;
			pNewSoldier->wornSnowCamo = __max(0, ( 65 - gGameExternalOptions.bCamoKitArea ) );
		}

		// Reload palettes....
		if ( pNewSoldier->bInSector )
		{
			pNewSoldier->CreateSoldierPalettes( );
		}

		// Set a pending animation to change stance first...
		//SendChangeSoldierStanceEvent( pNewSoldier, ANIM_CROUCH );

		// search for armour and equip if found
		if ( gGameExternalOptions.fZombieRiseWithArmour )
		{
			BOOLEAN fHelmetFound = FALSE;
			BOOLEAN fVestFound = FALSE;
			BOOLEAN fPantsFound = FALSE;

			ITEM_POOL * pItemPool, * pItemPoolNext;

			GetItemPool( pCorpse->def.sGridNo, &pItemPool, pCorpse->def.bLevel );

			while( pItemPool && ( !fHelmetFound || !fVestFound ||!fPantsFound ) )
			{
				pItemPoolNext = pItemPool->pNext;

				OBJECTTYPE* pObj = &(gWorldItems[ pItemPool->iItemIndex ].object);

				if ( Item[ pObj->usItem ].usItemClass  == IC_ARMOUR )
				{
						switch (Armour[Item[pObj->usItem].ubClassIndex].ubArmourClass)
						{
							case ARMOURCLASS_HELMET:
								{
									if( !fHelmetFound && AutoPlaceObject(pNewSoldier, pObj, FALSE))
									{
										// remove item from the ground
										RemoveItemFromPool( pCorpse->def.sGridNo, pItemPool->iItemIndex, pCorpse->def.bLevel );
										fHelmetFound = TRUE;
									}
									break;
								}
							case ARMOURCLASS_VEST:
								{
									if( !fVestFound && AutoPlaceObject(pNewSoldier, pObj, FALSE))
									{
										// remove item from the ground
										RemoveItemFromPool( pCorpse->def.sGridNo, pItemPool->iItemIndex, pCorpse->def.bLevel );
										fVestFound = TRUE;
									}
									break;
								}
							case ARMOURCLASS_LEGGINGS:
								{
									if( !fPantsFound && AutoPlaceObject(pNewSoldier, pObj, FALSE))
									{
										// remove item from the ground
										RemoveItemFromPool( pCorpse->def.sGridNo, pItemPool->iItemIndex, pCorpse->def.bLevel );
										fPantsFound = TRUE;
									}
									break;
								}
							default:
								break;
						}
				}

				pItemPool = pItemPoolNext;
			}
		}

		// Change to standing,unless we can getup with an animation
		pNewSoldier->EVENT_InitNewSoldierAnim( STANDING, 0, TRUE );
		pNewSoldier->BeginSoldierGetup( );
			
		// So we can see them!
		AllTeamsLookForAll(ALLOW_INTERRUPTS);
	}
}

// Flugente Zombies: returns true if a zombie can be raised from this corpse, and returns the correct pAnimState for the new zombie
BOOLEAN CorpseOkToSpawnZombie( ROTTING_CORPSE *	pCorpse, UINT16* pAnimState )
{
	BOOLEAN canbezombie = FALSE;
	*pAnimState = FALLBACKHIT_STOP;

	switch ( pCorpse->def.ubType )
	{
		case SMERC_BCK:	
		case SMERC_DHD:
		case SMERC_FALL:
		case MMERC_BCK:	
		case MMERC_DHD:
		case MMERC_FALL:
		case FMERC_BCK:
		case FMERC_DHD:
		case FMERC_FALL:
		case ROTTING_STAGE2:
			canbezombie = TRUE;
			*pAnimState = FALLBACKHIT_STOP;
			break;

		case SMERC_FWD:
		case SMERC_PRN:
		case SMERC_FALLF:
		case MMERC_FWD:
		case MMERC_PRN:
		case MMERC_FALLF:
		case FMERC_FWD:
		case FMERC_PRN:
		case FMERC_FALLF:
			canbezombie = TRUE;
			*pAnimState = FALLFORWARD_HITDEATH_STOP;
			break;

		case M_DEAD1:				
			canbezombie = TRUE;
			*pAnimState = GENERIC_HIT_DEATH;
			break;

		case M_DEAD2:		
			canbezombie = TRUE;
			*pAnimState = CIV_DIE2;
			break;

		case H_DEAD1:
		case K_DEAD1:
		case FT_DEAD1:
		case S_DEAD1:
		case W_DEAD1:
		case C_DEAD1:
		case H_DEAD2:
		case K_DEAD2:
		case FT_DEAD2:
		case S_DEAD2:
		case W_DEAD2:
		case C_DEAD2:
			canbezombie = TRUE;
			*pAnimState = STANDING;
			break;

		default:
			break;
	}

	return( canbezombie );
}

// Flugente: can we take the clothes of this corpse?
// calling this with NULL for soldier will give a general answer for any bodytype
BOOLEAN CorpseOkToDress( ROTTING_CORPSE* pCorpse )
{
	if ( !pCorpse )
		return FALSE;

	switch ( pCorpse->def.ubType )
	{
		case SMERC_JFK:
		case SMERC_BCK:
		case SMERC_FWD:
		case SMERC_DHD:
		case SMERC_PRN:
		case SMERC_WTR:
		case SMERC_FALL:
		case SMERC_FALLF:
		case M_DEAD1:
		case M_DEAD2:
		case H_DEAD1:
		case H_DEAD2:
		case S_DEAD1:
		case S_DEAD2:
		case C_DEAD1:
		case C_DEAD2:
		case MMERC_JFK:
		case MMERC_BCK:
		case MMERC_FWD:
		case MMERC_DHD:
		case MMERC_PRN:
		case MMERC_WTR:
		case MMERC_FALL:
		case MMERC_FALLF:
		case FT_DEAD1:
		case FT_DEAD2:			
		case FMERC_JFK:
		case FMERC_BCK:
		case FMERC_FWD:
		case FMERC_DHD:
		case FMERC_PRN:
		case FMERC_WTR:
		case FMERC_FALL:
		case FMERC_FALLF:
		case W_DEAD1:
		case W_DEAD2:
			return TRUE;
			break;

		default:
			break;
	}

	return FALSE;
}

// Flugente: how rotten is this corpse? values from 0 to 1, 1 as soon as it is rotting
FLOAT GetCorpseRotFactor( ROTTING_CORPSE* pCorpse )
{
	if ( !pCorpse )
		return 0.0f;

	if ( pCorpse->def.ubType == ROTTING_STAGE2 )
		return 1.0f;

	return (FLOAT)(min(gGameExternalOptions.usCorpseDelayUntilRotting, GetWorldTotalMin() - pCorpse->def.uiTimeOfDeath)) / gGameExternalOptions.usCorpseDelayUntilRotting;
}
