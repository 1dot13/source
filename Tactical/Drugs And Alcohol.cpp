#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "soldier control.h"
	#include "soldier profile.h"
	#include "drugs and alcohol.h"
	#include "items.h"
	#include "morale.h"
	#include "points.h"
	#include "message.h"
	#include "GameSettings.h" // SANDRO - had to add this, dammit!
#include "Random.h"
#include "Text.h"
#include "Interface.h"
#include "Food.h"	// added by Flugente
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


/*UINT8 ubDrugTravelRate[]			= { 4,	2 };
UINT8 ubDrugWearoffRate[]			= { 2,	2 };
UINT8 ubDrugEffect[]				= { 15, 8 };
UINT8 ubDrugSideEffect[]			= { 20, 10 };
UINT8 ubDrugSideEffectRate[]		= { 2,	1 };*/

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


BOOLEAN ApplyDrugs( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject )
{
	UINT32  ubDrugType;
	INT8	bRegenPointsGained;
	UINT16	usItem;

	usItem = pObject->usItem;

	// Determine what type of drug....
	ubDrugType = Item[usItem].drugtype;

	// If not a drug, return
	if ( ubDrugType == 0 )
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

	BOOLEAN consumeitem = TRUE;
	// if item is also a food item, don't use it up here, it will be consumed in ApplyFood, which will be called afterwards
	UINT32 foodtype = Item[pObject->usItem].foodtype;

	// if not a food item, nothing to see here
	if ( gGameOptions.fFoodSystem && foodtype > 0  )
		consumeitem = FALSE;

	// set flag: we are on drugs
	pSoldier->bSoldierFlagMask |= SOLDIER_DRUGGED;

	// Flugente: we have to check for every single type of drug ( a drug applied may consist of several 'pure' drug types)	
	for (UINT8 i = DRUG_TYPE_ADRENALINE; i < DRUG_TYPE_MAX; ++i)
	{
		UINT32 drugtestflag = (1 << i);			// comparing with this flag will determine the drug

		// we do not actually test for DRUG_REGENERATION, because that is checked afterwards separately
		if ( (drugtestflag & DRUG_REGENERATION ) != 0 )
			continue;
		
		if ( (ubDrugType & drugtestflag) != 0 )
		{
			// Add effects
			if ( ( pSoldier->drugs.bFutureDrugEffect[ i ] + Drug[i].ubDrugEffect ) < 127 )
			{
				pSoldier->drugs.bFutureDrugEffect[ i ]	+= Drug[i].ubDrugEffect;
			}
			pSoldier->drugs.bDrugEffectRate[ i ]		= Drug[i].ubDrugTravelRate;

			// Reset once we sleep...
			pSoldier->drugs.bTimesDrugUsedSinceSleep[ i ]++;

			// Increment side effects..
			if ( ( pSoldier->drugs.bDrugSideEffect[ i ] + Drug[i].ubDrugSideEffect ) < 127 )
			{
				pSoldier->drugs.bDrugSideEffect[ i ]	+= ( Drug[i].ubDrugSideEffect );
			}
			// Stop side effects until were done....
			pSoldier->drugs.bDrugSideEffectRate[ i ]	= 0;

			// ATE: Make guy collapse from heart attack if too much stuff taken....
			if ( pSoldier->drugs.bDrugSideEffectRate[ i ] > ( Drug[i].ubDrugSideEffect * 3 ) )
			{
				// Keel over...
				DeductPoints( pSoldier, 0, 10000 );

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - if our stat is damaged through facility event, make it healable
				// Permanently lower certain stats...
				if ( gGameOptions.fNewTraitSystem )
				{
					// WISDOM decrease
					if ( pSoldier->stats.bWisdom > 5 )
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_WISDOM ] += 5;
						pSoldier->stats.bWisdom -= 5;
					}
					else
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_WISDOM ] += (pSoldier->stats.bWisdom - 1);
						pSoldier->stats.bWisdom = 1;
					}
					// DEXTERITY decrease
					if ( pSoldier->stats.bDexterity > 5 )
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_DEXTERITY ] += 5;
						pSoldier->stats.bDexterity -= 5;
					}
					else
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_DEXTERITY ] += (pSoldier->stats.bDexterity - 1);
						pSoldier->stats.bDexterity = 1;
					}
					// STRENGTH decrease
					if ( pSoldier->stats.bStrength > 5 )
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_STRENGTH ] += 5;
						pSoldier->stats.bStrength -= 5;
					}
					else
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_STRENGTH ] += (pSoldier->stats.bStrength - 1);
						pSoldier->stats.bStrength = 1;
					}
					// AGILITY decrease
					if ( pSoldier->stats.bAgility > 5 )
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_AGILITY ] += 5;
						pSoldier->stats.bAgility -= 5;
					}
					else
					{
						pSoldier->ubCriticalStatDamage[ DAMAGED_STAT_AGILITY ] += (pSoldier->stats.bAgility - 1);
						pSoldier->stats.bAgility = 1;
					}
				}
				else // old system
				{
					pSoldier->stats.bAgility	= __max(1, pSoldier->stats.bWisdom-5);
					pSoldier->stats.bDexterity	= __max(1, pSoldier->stats.bDexterity-5);
					pSoldier->stats.bStrength	= __max(1, pSoldier->stats.bStrength-5);
					pSoldier->stats.bAgility	= __max(1, pSoldier->stats.bAgility-5);
					
					// make those stats RED for a while...
					// SANDRO - we don't need to do this with new system, as we simply show all damaged stats in red until healed
					pSoldier->timeChanges.uiChangeWisdomTime = GetJA2Clock();
					pSoldier->usValueGoneUp &= ~( WIS_INCREASE );
					pSoldier->timeChanges.uiChangeDexterityTime = GetJA2Clock();
					pSoldier->usValueGoneUp &= ~( DEX_INCREASE );
					pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
					pSoldier->usValueGoneUp &= ~( STRENGTH_INCREASE );
					pSoldier->timeChanges.uiChangeAgilityTime = GetJA2Clock();
					pSoldier->usValueGoneUp &= ~( AGIL_INCREASE );
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				// export stat changes to profile
				gMercProfiles[ pSoldier->ubProfile ].bWisdom	= pSoldier->stats.bWisdom;
				gMercProfiles[ pSoldier->ubProfile ].bDexterity = pSoldier->stats.bDexterity;
				gMercProfiles[ pSoldier->ubProfile ].bStrength	= pSoldier->stats.bStrength;
				gMercProfiles[ pSoldier->ubProfile ].bAgility	= pSoldier->stats.bAgility;

				// SANDRO - merc records - stat damaged
				gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;
			}
		}
	}

	if ( (ubDrugType & DRUG_REGENERATION) != 0 )
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

	// increase drug counter if not alcoholic drug
	if ( usItem != ALCOHOL && usItem != WINE && usItem != BEER )
	{
		if ( gMercProfiles[ pSoldier->ubProfile ].ubNumTimesDrugUseInLifetime != 255 )
		{
			gMercProfiles[ pSoldier->ubProfile ].ubNumTimesDrugUseInLifetime++;
		}
	}

	// ATE: use kit points...
	if ( consumeitem )
	{
		if ( usItem == ALCOHOL )
			UseKitPoints( pObject, 10, pSoldier );
		else if ( usItem == WINE )
			UseKitPoints( pObject, 20, pSoldier );
		else if ( usItem == BEER )
			UseKitPoints( pObject, 100, pSoldier );
		else
		{
			// remove object
			pObject->RemoveObjectsFromStack(1);
		}
	}
		
	if ( (ubDrugType & DRUG_ALCOHOL) != 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_DRANK_SOME ], pSoldier->name, ShortItemNames[ usItem ] );
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_MERC_TOOK_DRUG ], pSoldier->name );
	}

	// Dirty panel
	fInterfacePanelDirty = DIRTYLEVEL2;

	if ( !consumeitem )
		ApplyFood( pSoldier, pObject, TRUE, TRUE );

	return( TRUE );
}

void HandleEndTurnDrugAdjustments( SOLDIERTYPE *pSoldier )
{
	INT32 cnt, cnt2;
	INT32	iNumLoops;

	// if were not on drugs, nothing to do here
	if ( ( pSoldier->bSoldierFlagMask & SOLDIER_DRUGGED ) == 0 )
		return;

	// We test for every 'pure' drug separately
	for (cnt = DRUG_TYPE_ADRENALINE; cnt < DRUG_TYPE_MAX; ++cnt)
	{
		UINT32 drugtestflag = (1 << cnt);			// comparing with this flag will determine the drug

		// we do not actually test for DRUG_REGENERATION, because that is checked afterwards separately
		if ( (drugtestflag & DRUG_REGENERATION ) != 0 )
			continue;
		
		// If side effect aret is non-zero....
		if ( pSoldier->drugs.bDrugSideEffectRate[ cnt ] > 0 )
		{
			// Subtract some...
			pSoldier->drugs.bDrugSideEffect[ cnt ] -= pSoldier->drugs.bDrugSideEffectRate[ cnt ];

			// If we're done, we're done!
			if ( pSoldier->drugs.bDrugSideEffect[ cnt ] <= 0 )
			{
				pSoldier->drugs.bDrugSideEffect[ cnt ] = 0;
				pSoldier->drugs.bDrugSideEffectRate[ cnt ] = 0;
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
				pSoldier->drugs.bDrugSideEffectRate[ cnt ] = Drug[cnt].ubDrugSideEffectRate;

				// The drug rate is 0 now too
				pSoldier->drugs.bDrugEffectRate[ cnt ]		= 0;

				// morale downer only if side effect exists and this effect is allowed for a drug
				if ( Drug[cnt].ubDrugSideEffect > 0 && Drug[cnt].ubMoralBacklash > 0 )
				{
					// Once for each 'level' of crash....
					iNumLoops = ( pSoldier->drugs.bDrugSideEffect[ cnt ] / Drug[cnt].ubDrugSideEffect ) + 1;

					for ( cnt2 = 0; cnt2 < iNumLoops; ++cnt2 )
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
		}

		// Add increase ineffect....
		if ( pSoldier->drugs.bDrugEffectRate[ cnt ] > 0 )
		{
			// Seap some in....
			pSoldier->drugs.bFutureDrugEffect[ cnt ] -= pSoldier->drugs.bDrugEffectRate[ cnt ];
			pSoldier->drugs.bDrugEffect[ cnt ]		 += pSoldier->drugs.bDrugEffectRate[ cnt ];

			// Refresh morale w/ new drug value...
			RefreshSoldierMorale( pSoldier );

			// Check if we need to stop 'adding'
			if ( pSoldier->drugs.bFutureDrugEffect[ cnt ] <= 0 )
			{
				pSoldier->drugs.bFutureDrugEffect[ cnt ] = 0;
				// Change rate to -ve..
				pSoldier->drugs.bDrugEffectRate[ cnt ]		= -Drug[cnt].ubDrugWearoffRate;
			}
		}
	}

	if ( pSoldier->bRegenerationCounter > 0)
	{
//		bBandaged = BANDAGED( pSoldier );

		// increase life
		pSoldier->bPoisonLife = max(pSoldier->bPoisonSum, pSoldier->bPoisonLife + pSoldier->stats.bLife - __min( pSoldier->stats.bLife + LIFE_GAIN_PER_REGEN_POINT, pSoldier->stats.bLifeMax ) );
		pSoldier->stats.bLife = __min( pSoldier->stats.bLife + LIFE_GAIN_PER_REGEN_POINT, pSoldier->stats.bLifeMax );
		
		//SANDRO - Insta-healable injury reduction
		if( pSoldier->iHealableInjury > 0 )
		{
			pSoldier->iHealableInjury = max(0, (pSoldier->iHealableInjury - (100 * LIFE_GAIN_PER_REGEN_POINT)));
		}

		if ( pSoldier->stats.bLife == pSoldier->stats.bLifeMax )
		{
			pSoldier->bBleeding = 0;
			pSoldier->bPoisonBleeding = 0;
			pSoldier->iHealableInjury = 0;
		}
		else if ( pSoldier->bBleeding + pSoldier->stats.bLife > pSoldier->stats.bLifeMax )
		{
			// got to reduce amount of bleeding
			pSoldier->bBleeding = (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);
			pSoldier->bPoisonBleeding = min(pSoldier->bPoisonBleeding, (pSoldier->stats.bLifeMax - pSoldier->stats.bLife));
		}

		// decrement counter
		pSoldier->bRegenerationCounter--;
	}

	// Flugente: always do the following checks. Thereby, if the effect runs out, our stats will be back to normal
	// only do the checks for the player team, as soldiers do not have a gMercProfiles
	if  ( pSoldier->bTeam == gbPlayerNum)
	{
		//////////////// STRENGTH ////////////////
		pSoldier->bExtraStrength	= pSoldier->drugs.bDrugEffect[ DRUG_TYPE_STRENGTH ] - pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_STRENGTH ];

		//////////////// DEXTERITY ////////////////
		pSoldier->bExtraDexterity	= pSoldier->drugs.bDrugEffect[ DRUG_TYPE_DEXTERITY ] - pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_DEXTERITY ];

		//////////////// AGILITY ////////////////
		pSoldier->bExtraAgility		= pSoldier->drugs.bDrugEffect[ DRUG_TYPE_AGILITY ] - pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_AGILITY ];

		//////////////// WISDOM ////////////////
		pSoldier->bExtraWisdom		= pSoldier->drugs.bDrugEffect[ DRUG_TYPE_WISDOM ] - pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_WISDOM ];

		// if our sideeffect count is 1 (which should occur a while AFTER we took the drug), we suddenly become blind for a few turns...
		if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_BLIND ] == 0 && pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_BLIND ] == 1 )
		{
			pSoldier->bBlindedCounter = 3;
		}

		// if our sideeffect count is 1 (which should occur a while AFTER we took the drug), we get a heart-attack and get knocked out...
		if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_KNOCKOUT ] == 0 && pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_KNOCKOUT ] == 1 )
		{
			// Keel over...
			DeductPoints( pSoldier, 0, 20000 );
		}

		// if we have a life damaging effect, deduct life points
		if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_LIFEDAMAGE ] == 0 && pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_LIFEDAMAGE ] > 0 )
		{
			if ( pSoldier->stats.bLife > OKLIFE )
			{
				INT8 lifepointdamage = pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_LIFEDAMAGE ];
				
				INT8 applieddamage = pSoldier->stats.bLife;

				pSoldier->stats.bLife = max(OKLIFE - 1, pSoldier->stats.bLife - lifepointdamage);

				applieddamage -= pSoldier->stats.bLife;

				pSoldier->iHealableInjury += (applieddamage * 100);
			}
		}

		// if we took an antidote, reduce poisoning
		if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_CUREPOISON ] > 0 )
		{
			if (  pSoldier->bPoisonSum > 0 )
			{
				if ( pSoldier->bPoisonBleeding > 0 )
				{
					pSoldier->bPoisonBleeding--;
				}
				else if ( pSoldier->bPoisonLife > 0 )
				{
					pSoldier->bPoisonLife--;
				}

				pSoldier->bPoisonSum--;
			}
		}
	}

	// if all drug effects have ended, delete flag
	if ( !MercUnderTheInfluence(pSoldier) )
		pSoldier->bSoldierFlagMask &= ~SOLDIER_DRUGGED;
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

void HandleDamageResistanceEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT32 *psPointReduction )
{
	// Are we in a side effect or good effect?
	if ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_RESISTANCE ] )
	{
		// Adjust!
		(*psPointReduction) += pSoldier->drugs.bDrugEffect[ DRUG_TYPE_RESISTANCE ];
	}
	else if ( pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_RESISTANCE ] )
	{
		// Adjust!
		(*psPointReduction) -= pSoldier->drugs.bDrugSideEffect[ DRUG_TYPE_RESISTANCE ];
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
	bNumDrinks = ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ALCOHOL ] / Drug[DRUG_TYPE_ALCOHOL].ubDrugEffect );

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
	for (UINT8 cnt = DRUG_TYPE_ADRENALINE; cnt < DRUG_TYPE_MAX; ++cnt)
	{
		// Are we in a side effect or good effect?
		if ( pSoldier->drugs.bDrugEffect[ cnt ] )
		{
			return( TRUE );
		}
		else if ( pSoldier->drugs.bDrugSideEffect[ cnt ] )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

BOOLEAN MercUnderTheInfluence( SOLDIERTYPE *pSoldier, UINT8 aDrugType )
{
	// in case of wrong inout, stay safe
	if ( aDrugType >= DRUG_TYPE_MAX )
		return( FALSE );

	// Are we in a side effect or good effect?
	if ( pSoldier->drugs.bDrugEffect[ aDrugType ] )
	{
		return( TRUE );
	}
	else if ( pSoldier->drugs.bDrugSideEffect[ aDrugType ] )
	{
		return( TRUE );
	}

	return( FALSE );
}
