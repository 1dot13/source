#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "worlddef.h"
	#include "Random.h"
	#include "Smell.h"
	#include "worldman.h"
	#include "renderworld.h"
	#include "SaveLoadMap.h"
	#include "gamesettings.h"
	#include "message.h"
	#include "Isometric utils.h"
	#include "Map Information.h"
	#include "Game Clock.h"
	#include "Overhead.h"
	#include "debug control.h"
#endif

/*
 * Smell & Blood system
 *
 * Smell and blood trails decay as time passes.
 *
 *			 Decay Rate		Maximum Strength	Decay Time: Min Max (for biggest volume)
 *
 * Smell		1 per turn				31						 31	31
 * Blood	1 every 1-3 turns			7							7	21
 *
 * Smell has a much finer resolution so that creatures which track by smell
 * can do so effectively.
 */

/*
 * Time for some crazy-ass macros!
 * The smell byte is spit as follows:
 * O \
 * O	\
 * O	\ Smell
 * O	/ Strength (only on ground)
 * O	/
 * O /
 * O >	Type of blood on roof
 * O >	Type of smell/blood on ground
 *
 * The blood byte is split as follows:
 * O \
 * O	> Blood quantity on roof
 * O /
 * O \
 * O	> Blood quantity on ground
 * O /
 * O \	Blood decay
 * O /	time (roof and ground decay together)
 */

/*
 * In these defines,
 * s indicates the smell byte, b indicates the blood byte
 */

// LUT for which graphic to use based on strength
//															0	1,	2,	3,	4,	5,	6, 7
UINT8 ubBloodGraphicLUT [ ] = {	3, 3,	2,	2,	1,	1,	0, 0 };


#define SMELL_STRENGTH_MAX			63
#define BLOOD_STRENGTH_MAX			7
#define BLOOD_DELAY_MAX				3

#define SMELL_TYPE_BITS( s )		(s & 0x03)

#define BLOOD_ROOF_TYPE( s )		(s & 0x02)
#define BLOOD_FLOOR_TYPE( s )		(s & 0x01)

#define BLOOD_ROOF_STRENGTH( b )	(b & 0xE0)
#define BLOOD_FLOOR_STRENGTH( b )	( (b & 0x1C) >> 2 )
#define BLOOD_DELAY_TIME( b )		(b & 0x03)
#define NO_BLOOD_STRENGTH( b )		((b & 0xFC) == 0)

#define DECAY_SMELL_STRENGTH( s ) \
{ \
	UINT8 ubStrength = SMELL_STRENGTH( (s) ); \
	ubStrength--; \
	ubStrength = ubStrength << SMELL_TYPE_NUM_BITS; \
	(s) = SMELL_TYPE_BITS( (s) ) | ubStrength; \
}

// s = smell byte
// ns = new strength
// ntf = new type on floor
// Note that the first part of the macro is designed to
// preserve the type value for the blood on the roof
#define SET_SMELL( s, ns, ntf ) \
{ \
	(s) = (BLOOD_ROOF_TYPE( s )) | SMELL_TYPE( ntf ) | (ns << SMELL_TYPE_NUM_BITS); \
}

#define DECAY_BLOOD_DELAY_TIME( b ) \
{ \
	(b)--; \
}

#define SET_BLOOD_FLOOR_STRENGTH( b, nb ) \
{ \
	(b) = ( (nb) << 2 ) | ( (b) & 0xE3); \
}

#define SET_BLOOD_ROOF_STRENGTH( b, nb ) \
{ \
	(b) = BLOOD_FLOOR_STRENGTH( (nb) ) << 5 | ( (b) & 0x1F); \
}

#define DECAY_BLOOD_FLOOR_STRENGTH( b ) \
{ \
	UINT8 ubFloorStrength; \
	ubFloorStrength = BLOOD_FLOOR_STRENGTH( (b) ); \
	ubFloorStrength--; \
	SET_BLOOD_FLOOR_STRENGTH( b, ubFloorStrength ); \
}

#define DECAY_BLOOD_ROOF_STRENGTH( b ) \
{ \
	UINT8 ubRoofStrength; \
	ubRoofStrength = BLOOD_ROOF_STRENGTH( (b) ); \
	ubRoofStrength--; \
	SET_BLOOD_FLOOR_STRENGTH( b, ubRoofStrength ); \
}

#define SET_BLOOD_DELAY_TIME( b ) \
{ \
 (b) = BLOOD_DELAY_TIME( (UINT8) Random( BLOOD_DELAY_MAX ) + 1 ) | (b & 0xFC); \
}

#define SET_BLOOD_FLOOR_TYPE( s, ntg ) \
{ \
	(s) = BLOOD_FLOOR_TYPE( ntg ) | (s & 0xFE); \
}

#define SET_BLOOD_ROOF_TYPE( s, ntr ) \
{ \
	(s) = BLOOD_ROOF_TYPE( ntr ) | (s & 0xFD); \
}

void RemoveBlood( INT32 sGridNo, INT8 bLevel )
{
	//gpWorldLevelData[ sGridNo ].ubBloodInfo = 0;
	if (bLevel > 0)
		(gpWorldLevelData[sGridNo].ubBloodInfo) &= ~(0xE0);
	else
		(gpWorldLevelData[sGridNo].ubBloodInfo) &= ~(0x1C);

	gpWorldLevelData[ sGridNo ].uiFlags |= MAPELEMENT_REEVALUATEBLOOD;

	UpdateBloodGraphics( sGridNo, bLevel );
}

void DecaySmells( void )
{
	INT32					uiLoop;
	MAP_ELEMENT *		pMapElement;

	for ( uiLoop = 0, pMapElement = gpWorldLevelData; uiLoop < WORLD_MAX; ++uiLoop, ++pMapElement )
	{
		if (pMapElement->ubSmellInfo)
		{
			// decay smell strength!
			DECAY_SMELL_STRENGTH( pMapElement->ubSmellInfo );
			// if the strength left is 0, wipe the whole byte to clear the type
			if (SMELL_STRENGTH( pMapElement->ubSmellInfo ) == 0)
			{
				pMapElement->ubSmellInfo = 0;
			}
		}
	}
}

void DecayBlood()
{
	INT32					uiLoop;
	MAP_ELEMENT *		pMapElement;

	for (uiLoop = 0, pMapElement = gpWorldLevelData; uiLoop < WORLD_MAX; ++uiLoop, ++pMapElement)
	{
		if (pMapElement->ubBloodInfo)
		{
			// delay blood timer!
			DECAY_BLOOD_DELAY_TIME( pMapElement->ubBloodInfo );
			if (BLOOD_DELAY_TIME( pMapElement->ubBloodInfo ) == 0)
			{
				// Set re-evaluate flag
				pMapElement->uiFlags |= MAPELEMENT_REEVALUATEBLOOD;

				// reduce the floor blood strength if it is above zero
				if (BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo ) > 0)
				{
					DECAY_BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo )
					if (BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo ) == 0)
					{
						// delete the blood graphic on the floor!
						// then
						if (NO_BLOOD_STRENGTH( pMapElement->ubBloodInfo ))
						{
							// wipe the whole byte to zero
							pMapElement->ubBloodInfo = 0;
						}
					}
				}
				// reduce the roof blood strength if it is above zero
				if (BLOOD_ROOF_STRENGTH( pMapElement->ubBloodInfo ) > 0)
				{
					DECAY_BLOOD_ROOF_STRENGTH( pMapElement->ubBloodInfo )
					if (BLOOD_ROOF_STRENGTH( pMapElement->ubBloodInfo ) == 0)
					{
						// delete the blood graphic on the roof!
						if (NO_BLOOD_STRENGTH( pMapElement->ubBloodInfo ))
						{
							// wipe the whole byte to zero
							pMapElement->ubBloodInfo = 0;
						}
					}
				}

				// if any blood remaining, reset time
				if ( pMapElement->ubBloodInfo )
				{
					SET_BLOOD_DELAY_TIME( pMapElement->ubBloodInfo );
				}
			}
			// end of blood handling
		}

		// now go on to the next gridno
	}
}

void DecayBloodAndSmells( UINT32 uiTime )
{
	UINT32					uiCheckTime;

	if ( !gfWorldLoaded )
	{
		return;
	}

	// period between checks, in game seconds
	switch( giTimeCompressMode )
	{
		// in time compression, let this happen every 5 REAL seconds
		case TIME_COMPRESS_5MINS: // rate of 300 seconds per real second
			uiCheckTime = 5 * 300;
			break;
		case TIME_COMPRESS_30MINS: // rate of 1800 seconds per real second
			uiCheckTime = 5 * 1800;
			break;
		case TIME_COMPRESS_60MINS: // rate of 3600 seconds per real second
		case TIME_SUPER_COMPRESS: // should not be used but just in frigging case...
			uiCheckTime = 5 * 3600;
			break;
		default: // not compressing
			uiCheckTime = 100;
			break;
	}

	// ok so "uiDecayBloodLastUpdate" is a bit of a misnomer now
	if ( ( uiTime - gTacticalStatus.uiDecayBloodLastUpdate ) > uiCheckTime )
	{
		gTacticalStatus.uiDecayBloodLastUpdate = uiTime;
		DecayBlood();
		DecaySmells();
	}
}

void DropSmell( SOLDIERTYPE * pSoldier )
{
	MAP_ELEMENT *		pMapElement;
	UINT8				ubOldSmell;
	UINT8				ubOldStrength;
	UINT8				ubSmell;
	UINT8				ubStrength;

	/*
	*	Here we are creating a new smell on the ground.	If there is blood in
	*	the tile, it overrides dropping smells of any type
	*/

	if (pSoldier->pathing.bLevel == 0)
	{
		pMapElement = &(gpWorldLevelData[pSoldier->sGridNo]);
		if (pMapElement->ubBloodInfo)
		{
			// blood here, don't drop any smell
			return;
		}

		if (pSoldier->aiData.bNormalSmell > pSoldier->aiData.bMonsterSmell)
		{
			ubStrength = pSoldier->aiData.bNormalSmell - pSoldier->aiData.bMonsterSmell;
			ubSmell = HUMAN;
		}
		else
		{
			ubStrength = pSoldier->aiData.bMonsterSmell - pSoldier->aiData.bNormalSmell;
			if (ubStrength == 0)
			{
				// don't drop any smell
				return;
			}
			ubSmell = CREATURE_ON_FLOOR;
		}

		if (pMapElement->ubSmellInfo)
		{
			// smell already exists here; check to see if it's the same or not

			ubOldSmell = SMELL_TYPE( pMapElement->ubSmellInfo );
			ubOldStrength = SMELL_STRENGTH( pMapElement->ubSmellInfo );
			if (ubOldSmell == ubSmell)
			{
				// same smell; increase the strength to the bigger of the two strengths,
				// plus 1/5 of the smaller
				ubStrength = __max( ubStrength, ubOldStrength ) + __min( ubStrength, ubOldStrength ) / 5;
				ubStrength = __max( ubStrength, SMELL_STRENGTH_MAX );
			}
			else
			{
				// different smell; we muddy the smell by reducing the smell strength
				if (ubOldStrength > ubStrength)
				{
					ubOldStrength -= ubStrength / 3;
					SET_SMELL( pMapElement->ubSmellInfo, ubOldStrength, ubOldSmell );
				}
				else
				{
					ubStrength -= ubOldStrength / 3;
					if (ubStrength > 0)
					{
						SET_SMELL( pMapElement->ubSmellInfo, ubStrength, ubSmell );
					}
					else
					{
						// smell reduced to 0 - wipe all info on it!
						pMapElement->ubSmellInfo = 0;
					}
				}
			}
		}
		else
		{
			// the simple case, dropping a smell in a location where there is none
			SET_SMELL( pMapElement->ubSmellInfo, ubStrength, ubSmell );
		}
	}
	// otherwise skip dropping smell
}

void InternalDropBlood( INT32 sGridNo, INT8 bLevel, UINT8 ubType, UINT8 ubStrength, INT8 bVisible )
{
	CHAR tmpMPDbgString[512];
	sprintf(tmpMPDbgString,"InternalDropBlood ( %i , %i , %i , %i , %i )\n",sGridNo, bLevel , ubType , ubStrength , bVisible );
	MPDebugMsg(tmpMPDbgString);

	MAP_ELEMENT *		pMapElement;
	UINT8						ubOldStrength=0;
	UINT8						ubNewStrength=0;
	UINT8	bOverTerrainType;

	/*
	* Dropping some blood;
	* We can check the type of blood by consulting the type in the smell byte
	*/

	bOverTerrainType = GetTerrainType( sGridNo);
	// If we are in water...
	if ( TERRAIN_IS_WATER( bOverTerrainType) )
	{
		return;
	}

	// ATE: Send warning if dropping blood nowhere....	
	if (TileIsOutOfBounds(sGridNo))
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Attempting to drop blood NOWHERE" );
#endif
		return;
	}

	// ensure max strength is okay
	ubStrength = __min( ubStrength, BLOOD_STRENGTH_MAX );

	pMapElement = &(gpWorldLevelData[ sGridNo ]);
	if ( bLevel == 0 )
	{
		// dropping blood on ground
		ubOldStrength = BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo );
		if (ubOldStrength > 0)
		{
			// blood already there... we'll leave the decay time as it is
			// sevenfm: blood floor type stored in smell byte!
			//if (BLOOD_FLOOR_TYPE( pMapElement->ubBloodInfo ) == ubType)
			if (BLOOD_FLOOR_TYPE(pMapElement->ubSmellInfo) == ubType)
			{
				// combine blood strengths!
				ubNewStrength = __min( ( ubOldStrength + ubStrength ), BLOOD_STRENGTH_MAX );

				SET_BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo, ubNewStrength );
			}
			else
			{
				// replace the existing blood if more is being dropped than exists
				if (ubStrength > ubOldStrength )
				{
					// replace!
					SET_BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo, ubStrength );
				}
				// else we don't drop anything at all
			}
		}
		else
		{
			// no blood on the ground yet, so drop this amount!
			// set decay time
			SET_BLOOD_DELAY_TIME( pMapElement->ubBloodInfo );
			SET_BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo, ubStrength );
			// NB blood floor type stored in smell byte!
			SET_BLOOD_FLOOR_TYPE( pMapElement->ubSmellInfo, ubType );
		}
	}
	else
	{
		// dropping blood on roof
		ubOldStrength = BLOOD_ROOF_STRENGTH( pMapElement->ubBloodInfo );
		if (ubOldStrength > 0)
		{
			// blood already there... we'll leave the decay time as it is
			if (BLOOD_ROOF_TYPE( pMapElement->ubSmellInfo ) == ubType)
			{
				// combine blood strengths!
				ubNewStrength = __max( ubOldStrength, ubStrength ) + 1;
				// make sure the strength is legal
				ubNewStrength = __max( ubNewStrength, BLOOD_STRENGTH_MAX );
				SET_BLOOD_ROOF_STRENGTH( pMapElement->ubBloodInfo, ubNewStrength );
			}
			else
			{
				// replace the existing blood if more is being dropped than exists
				if (ubStrength > ubOldStrength )
				{
					// replace!
					SET_BLOOD_ROOF_STRENGTH( pMapElement->ubBloodInfo, ubStrength );
				}
				// else we don't drop anything at all
			}
		}
		else
		{
			// no blood on the roof yet, so drop this amount!
			// set decay time
			SET_BLOOD_DELAY_TIME( pMapElement->ubBloodInfo );
			SET_BLOOD_ROOF_STRENGTH( pMapElement->ubBloodInfo, ubNewStrength );
			SET_BLOOD_ROOF_TYPE( pMapElement->ubSmellInfo, ubType );
		}
	}

	// Turn on flag...
	pMapElement->uiFlags |= MAPELEMENT_REEVALUATEBLOOD;

	if ( bVisible != -1 )
	{
		UpdateBloodGraphics( sGridNo, bLevel );
	}
}

void DropBlood( SOLDIERTYPE * pSoldier, UINT8 ubStrength, INT8 bVisible )
{
	UINT8						ubType;
	/*
	* Dropping some blood;
	* We can check the type of blood by consulting the type in the smell byte
	*/

	// figure out the type of blood that we're dropping
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER )
	{
		if ( pSoldier->pathing.bLevel == 0 )
			ubType = CREATURE_ON_FLOOR;
		else
			ubType = CREATURE_ON_ROOF;
	}
	else
	{
		ubType = HUMAN;
	}

	InternalDropBlood( pSoldier->sGridNo, pSoldier->pathing.bLevel, ubType, ubStrength, bVisible );
}

void UpdateBloodGraphics( INT32 sGridNo, INT8 bLevel )
{
	MAP_ELEMENT *		pMapElement;
	INT8				bValue;
	UINT16				usIndex, usNewIndex;

	// OK, based on level, type, display graphics for blood
	pMapElement = &(gpWorldLevelData[ sGridNo ]);

	// CHECK FOR BLOOD OPTION
	if ( !gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
	{
		return;
	}

	if ( pMapElement->uiFlags & MAPELEMENT_REEVALUATEBLOOD )
	{
		// Turn off flag!
		pMapElement->uiFlags &= ( ~MAPELEMENT_REEVALUATEBLOOD );

		// Ground
		if ( bLevel == 0 )
		{
			bValue = BLOOD_FLOOR_STRENGTH( pMapElement->ubBloodInfo );

			// OK, remove tile graphic if one exists....
			if ( TypeRangeExistsInObjectLayer( sGridNo, HUMANBLOOD, CREATUREBLOOD, &usIndex ) )
			{
				//This has been removed and it is handled by the ubBloodInfo level when restoring a saved game.
				//Set a flag indicating that the following changes are to go the the maps temp file
				//ApplyMapChangesToMapTempFile( TRUE );

				// Remove
				RemoveObject( sGridNo, usIndex );

				//ApplyMapChangesToMapTempFile( FALSE );

				// sevenfm: update rendering
				pMapElement->uiFlags |= MAPELEMENT_REDRAW;
				SetRenderFlags(RENDER_FLAG_FULL);
			}

			// OK, pick new one. based on strength and randomness

			if ( bValue > 0 )
			{
				usIndex = (UINT16)( ( Random( 4 ) * 4 ) + ubBloodGraphicLUT[ bValue ] );

				if ( BLOOD_FLOOR_TYPE( pMapElement->ubSmellInfo )	== 0 )
				{
					GetTileIndexFromTypeSubIndex( HUMANBLOOD, (UINT16)(usIndex + 1), &usNewIndex );
				}
				else
				{
					GetTileIndexFromTypeSubIndex( CREATUREBLOOD, (UINT16)(usIndex + 1), &usNewIndex );
				}

				//This has been removed and it is handled by the ubBloodInfo level when restoring a saved game.
				//Set a flag indicating that the following changes are to go the the maps temp file
				//ApplyMapChangesToMapTempFile( TRUE );

				// Add!
				AddObjectToHead( sGridNo, usNewIndex );

				//ApplyMapChangesToMapTempFile( FALSE );

				// Update rendering!
				pMapElement->uiFlags|=MAPELEMENT_REDRAW;
				SetRenderFlags(RENDER_FLAG_MARKED);
			}
		}
		// Roof
		else
		{

		}
	}
}
