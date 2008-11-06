#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	//#include "soldier control.h"
	#include "soldier profile.h"
	#include "drugs and alcohol.h"
	#include "items.h"
	#include "drugs and alcohol.h"
	#include "morale.h"
	#include "points.h"
	#include "message.h"
#include "Random.h"
#include "Text.h"
#include "Interface.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


UINT8 ubDrugTravelRate[]			= { 4,	2 };
UINT8 ubDrugWearoffRate[]			= { 2,	2 };
UINT8 ubDrugEffect[]					= { 15, 8 };
UINT8 ubDrugSideEffect[]			= { 20, 10 };
UINT8 ubDrugSideEffectRate[]	= { 2,	1 };

INT32	giDrunkModifier[] =
{
	100,		// Sober
	75,			// Feeling good,
	65,			// Bporderline
	50,			// Drunk
	100,		// HungOver
};

#define HANGOVER_AP_REDUCE			5
#define HANGOVER_BP_REDUCE			200


UINT8 GetDrugType( UINT16 usItem )
{
	if ( usItem == ADRENALINE_BOOSTER )
	{
		return( DRUG_TYPE_ADRENALINE );
	}

	if ( usItem == REGEN_BOOSTER )
	{
		return( DRUG_TYPE_REGENERATION );
	}

	if ( usItem == ALCOHOL || usItem == WINE || usItem == BEER )
	{
		return( DRUG_TYPE_ALCOHOL );
	}


	return( NO_DRUG );
}


BOOLEAN ApplyDrugs( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject )
{
	UINT8 ubDrugType;
	UINT8	ubKitPoints;
	INT8	bRegenPointsGained;
	UINT16	usItem;

	usItem = pObject->usItem;

	// If not a syringe, return

	ubDrugType = GetDrugType( usItem );

	// Determine what type of drug....
	if ( ubDrugType == NO_DRUG )
	{
		return( FALSE );
	}

	// do switch for Larry!!
	if ( pSoldier->ubProfile == LARRY_NORMAL )
	{
		pSoldier = SwapLarrysProfiles( pSoldier );
	}
	else if ( pSoldier->ubProfile == LARRY_DRUNK )
	{
		gMercProfiles[ LARRY_DRUNK ].bNPCData = 0;
	}

	if ( ubDrugType < NUM_COMPLEX_DRUGS )
	{

		// Add effects
		if ( ( pSoldier->drugs.bFutureDrugEffect[ ubDrugType ] + ubDrugEffect[ ubDrugType ] ) < 127 )
		{
			pSoldier->drugs.bFutureDrugEffect[ ubDrugType ]							+= ubDrugEffect[ ubDrugType ];
		}
		pSoldier->drugs.bDrugEffectRate[ ubDrugType ]								= ubDrugTravelRate[ ubDrugType ];

		// Increment times used during lifetime...
		// CAP!
		if ( ubDrugType == DRUG_TYPE_ADRENALINE )
		{
			if ( gMercProfiles[ pSoldier->ubProfile ].ubNumTimesDrugUseInLifetime != 255 )
			{
				gMercProfiles[ pSoldier->ubProfile ].ubNumTimesDrugUseInLifetime++;
			}
		}

		// Reset once we sleep...
		pSoldier->drugs.bTimesDrugUsedSinceSleep[ ubDrugType ]++;

		// Increment side effects..
		if ( ( pSoldier->drugs.bDrugSideEffect[ ubDrugType ] + ubDrugSideEffect[ ubDrugType ] ) < 127 )
		{
			pSoldier->drugs.bDrugSideEffect[ ubDrugType ]								+= ( ubDrugSideEffect[ ubDrugType ] );
		}
		// Stop side effects until were done....
		pSoldier->drugs.bDrugSideEffectRate[ ubDrugType ]						= 0;


		if ( ubDrugType == DRUG_TYPE_ALCOHOL )
		{
			// ATE: use kit points...
			if ( usItem == ALCOHOL )
			{
				ubKitPoints = 10;
			}
			else if ( usItem == WINE )
			{
				ubKitPoints = 20;
			}
			else
			{
				ubKitPoints = 100;
			}

			UseKitPoints( pObject, ubKitPoints, pSoldier );
		}
		else
		{
			// Remove the object....
			pObject->RemoveObjectsFromStack(1);

			// ATE: Make guy collapse from heart attack if too much stuff taken....
			if ( pSoldier->drugs.bDrugSideEffectRate[ ubDrugType ] > ( ubDrugSideEffect[ ubDrugType ] * 3 ) )
			{
				// Keel over...
				DeductPoints( pSoldier, 0, 10000 );

				// Permanently lower certain stats...
				pSoldier->stats.bWisdom	-= 5;
				pSoldier->stats.bDexterity -= 5;
				pSoldier->stats.bStrength	-= 5;

				if (pSoldier->stats.bWisdom < 1)
					pSoldier->stats.bWisdom = 1;
				if (pSoldier->stats.bDexterity < 1)
					pSoldier->stats.bDexterity = 1;
				if (pSoldier->stats.bStrength < 1)
					pSoldier->stats.bStrength = 1;

				// export stat changes to profile
				gMercProfiles[ pSoldier->ubProfile ].bWisdom	= pSoldier->stats.bWisdom;
				gMercProfiles[ pSoldier->ubProfile ].bDexterity = pSoldier->stats.bDexterity;
				gMercProfiles[ pSoldier->ubProfile ].bStrength	= pSoldier->stats.bStrength;

				// make those stats RED for a while...
				pSoldier->timeChanges.uiChangeWisdomTime = GetJA2Clock();
				pSoldier->usValueGoneUp &= ~( WIS_INCREASE );
				pSoldier->timeChanges.uiChangeDexterityTime = GetJA2Clock();
				pSoldier->usValueGoneUp &= ~( DEX_INCREASE );
				pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
				pSoldier->usValueGoneUp &= ~( STRENGTH_INCREASE );
			}
		}
	}
	else
	{
		if ( ubDrugType == DRUG_TYPE_REGENERATION )
		{
			// each use of a regen booster over 1, each day, reduces the effect
			bRegenPointsGained = REGEN_POINTS_PER_BOOSTER * (*pObject)[0]->data.objectStatus / 100;
			// are there fractional %s left over?
			if ( ( (*pObject)[0]->data.objectStatus % (100 / REGEN_POINTS_PER_BOOSTER ) ) != 0 )
			{
				// chance of an extra point
				if ( PreRandom( 100 / REGEN_POINTS_PER_BOOSTER ) < (UINT32) ( (*pObject)[0]->data.objectStatus % (100 / REGEN_POINTS_PER_BOOSTER ) ) )
				{
					bRegenPointsGained++;
				}
			}

			bRegenPointsGained -= pSoldier->bRegenBoostersUsedToday;
			if (bRegenPointsGained > 0)
			{
				// can't go above the points you get for a full boost
				pSoldier->bRegenerationCounter = __min( pSoldier->bRegenerationCounter + bRegenPointsGained, REGEN_POINTS_PER_BOOSTER );
			}
			pSoldier->bRegenBoostersUsedToday++;
		}

		// remove object
		pObject->RemoveObjectsFromStack(1);
	}

	if ( ubDrugType == DRUG_TYPE_ALCOHOL )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_DRANK_SOME ], pSoldier->name, ShortItemNames[ usItem ] );
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_MERC_TOOK_DRUG ], pSoldier->name );
	}

	// Dirty panel
	fInterfacePanelDirty = DIRTYLEVEL2;

	return( TRUE );
}

void HandleEndTurnDrugAdjustments( SOLDIERTYPE *pSoldier )
{
	INT32 cnt, cnt2;
	INT32	iNumLoops;
//	INT8 bBandaged;

	for ( cnt = 0; cnt < NUM_COMPLEX_DRUGS; cnt++ )
	{
		// If side effect aret is non-zero....
		if ( pSoldier->drugs.bDrugSideEffectRate[ cnt ] > 0 )
		{
			// Subtract some...
			pSoldier->drugs.bDrugSideEffect[ cnt ] -= pSoldier->drugs.bDrugSideEffectRate[ cnt ];

			// If we're done, we're done!
			if ( pSoldier->drugs.bDrugSideEffect[ cnt ] <= 0 )
			{
				pSoldier->drugs.bDrugSideEffect[ cnt ] = 0;
				fInterfacePanelDirty	= DIRTYLEVEL1;
			}
		}

		// IF drug rate is -ve, it's being worn off...
		if ( pSoldier->drugs.bDrugEffectRate[ cnt ] < 0 )
		{
			pSoldier->drugs.bDrugEffect[ cnt ]				-= ( -1 * pSoldier->drugs.bDrugEffectRate[ cnt ] );

			// Have we run out?
			if ( pSoldier->drugs.bDrugEffect[ cnt ] <= 0 )
			{
				pSoldier->drugs.bDrugEffect[ cnt ] = 0;

		// Dirty panel
		fInterfacePanelDirty = DIRTYLEVEL2;

				// Start the bad news!
				pSoldier->drugs.bDrugSideEffectRate[ cnt ] = ubDrugSideEffectRate[ cnt ];

				// The drug rate is 0 now too
				pSoldier->drugs.bDrugEffectRate[ cnt ]		= 0;

				// Once for each 'level' of crash....
				iNumLoops = ( pSoldier->drugs.bDrugSideEffect[ cnt ] / ubDrugSideEffect[ cnt ] ) + 1;

				for ( cnt2 = 0; cnt2 < iNumLoops; cnt2++ )
				{
					// OK, give a much BIGGER morale downer
					if ( cnt == DRUG_TYPE_ALCOHOL )
					{
						HandleMoraleEvent( pSoldier, MORALE_ALCOHOL_CRASH, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
					}
					else
					{
						HandleMoraleEvent( pSoldier, MORALE_DRUGS_CRASH, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ	);
					}
				}
			}
		}

		// Add increase ineffect....
		if ( pSoldier->drugs.bDrugEffectRate[ cnt ] > 0 )
		{
			// Seap some in....
			pSoldier->drugs.bFutureDrugEffect[ cnt ] -= pSoldier->drugs.bDrugEffectRate[ cnt ];
			pSoldier->drugs.bDrugEffect[ cnt ]				+= pSoldier->drugs.bDrugEffectRate[ cnt ];

			// Refresh morale w/ new drug value...
			RefreshSoldierMorale( pSoldier );

			// Check if we need to stop 'adding'
			if ( pSoldier->drugs.bFutureDrugEffect[ cnt ] <= 0 )
			{
				pSoldier->drugs.bFutureDrugEffect[ cnt ] = 0;
				// Change rate to -ve..
				pSoldier->drugs.bDrugEffectRate[ cnt ]		= -ubDrugWearoffRate[ cnt ];
			}
		}
	}

	if ( pSoldier->bRegenerationCounter > 0)
	{
//		bBandaged = BANDAGED( pSoldier );

		// increase life
		pSoldier->stats.bLife = __min( pSoldier->stats.bLife + LIFE_GAIN_PER_REGEN_POINT, pSoldier->stats.bLifeMax );

		if ( pSoldier->stats.bLife == pSoldier->stats.bLifeMax )
		{
			pSoldier->bBleeding = 0;
		}
		else if ( pSoldier->bBleeding + pSoldier->stats.bLife > pSoldier->stats.bLifeMax )
		{
			// got to reduce amount of bleeding
			pSoldier->bBleeding = (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);
		}

		// decrement counter
		pSoldier->bRegenerationCounter--;
	}
}

INT8 GetDrugEffect( SOLDIERTYPE *pSoldier, UINT8 ubDrugType	)
{
	return( pSoldier->drugs.bDrugEffect[ ubDrugType ] );
}


INT8 GetDrugSideEffect( SOLDIERTYPE *pSoldier, UINT8 ubDrugType )
{
	// If we have a o-positive effect
	if ( pSoldier->drugs.bDrugEffect[ ubDrugType ] > 0 )
	{
		return( 0 );
	}
	else
	{
		return( pSoldier->drugs.bDrugSideEffect[ ubDrugType ] );
	}
}

void HandleAPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *pubPoints )
{
	INT8	bDrunkLevel;
	INT16	sPoints = (*pubPoints);

	// Are we in a side effect or good effect?
	if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ADRENALINE ] )
	{
		// Adjust!
		sPoints += pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ADRENALINE ];
	}
	else if ( pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_ADRENALINE ] )
	{
		// Adjust!
		sPoints -= pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_ADRENALINE ];

		if ( sPoints < APBPConstants[APBPConstants[APBPConstants[AP_MINIMUM]]] )
		{
			sPoints = APBPConstants[APBPConstants[APBPConstants[AP_MINIMUM]]];
		}
	}

	bDrunkLevel = GetDrunkLevel( pSoldier );

	if ( bDrunkLevel == HUNGOVER )
	{
		// Reduce....
		sPoints -= HANGOVER_AP_REDUCE;

		if ( sPoints < APBPConstants[APBPConstants[APBPConstants[AP_MINIMUM]]] )
		{
			sPoints = APBPConstants[APBPConstants[APBPConstants[AP_MINIMUM]]];
		}
	}

	(*pubPoints) = sPoints;
}


void HandleBPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *psPointReduction )
{
	INT8 bDrunkLevel;

	// Are we in a side effect or good effect?
	if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ADRENALINE ] )
	{
		// Adjust!
		(*psPointReduction) -= ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ADRENALINE ] * APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL] );
	}
	else if ( pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_ADRENALINE ] )
	{
		// Adjust!
		(*psPointReduction) += ( pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_ADRENALINE ] * APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL] );
	}

	bDrunkLevel = GetDrunkLevel( pSoldier );

	if ( bDrunkLevel == HUNGOVER )
	{
		// Reduce....
		(*psPointReduction) += HANGOVER_BP_REDUCE;
	}
}


INT8 GetDrunkLevel( SOLDIERTYPE *pSoldier )
{
	INT8 bNumDrinks;

	// If we have a -ve effect ...
	if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ALCOHOL ] == 0 && pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_ALCOHOL ] == 0 )
	{
		return( SOBER );
	}

	if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ALCOHOL ] == 0 && pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_ALCOHOL ] != 0 )
	{
		return( HUNGOVER );
	}

	// Calculate how many dinks we have had....
	bNumDrinks = ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ALCOHOL ] / ubDrugEffect[ DRUG_TYPE_ALCOHOL ] );

	if ( bNumDrinks <= 3 )
	{
		return( FEELING_GOOD );
	}
	else if ( bNumDrinks <= 4 )
	{
		return( BORDERLINE );
	}
	else
	{
		return( DRUNK );
	}
}


INT32 EffectStatForBeingDrunk( SOLDIERTYPE *pSoldier, INT32 iStat )
{
	return( ( iStat * giDrunkModifier[ GetDrunkLevel( pSoldier ) ] / 100 ) );
}


BOOLEAN MercUnderTheInfluence( SOLDIERTYPE *pSoldier )
{
	// Are we in a side effect or good effect?
	if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ADRENALINE ] )
	{
		return( TRUE );
	}
	else if ( pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_ADRENALINE ] )
	{
		return( TRUE );
	}

	if ( GetDrunkLevel( pSoldier ) != SOBER )
	{
		return( TRUE );
	}

	return( FALSE );
}
