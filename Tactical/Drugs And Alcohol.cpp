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
	#include "Food.h"			// added by Flugente
	#include "Animation data.h"	// added by Flugente for SoldierBodyTypes
	#include "CampaignStats.h"	// added by Flugente
	#include "DynamicDialogue.h"// added by Flugente
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;

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

BOOLEAN ApplyDrugs_New( SOLDIERTYPE *pSoldier, UINT16 usItem, UINT16 uStatusUsed )
{
	// If not a drug, return
	if ( !Item[usItem].drugtype || !uStatusUsed || !pSoldier )
		return(FALSE);

	UINT32 drugused = Item[usItem].drugtype;

	// to stop Larry from getting stoned via unsanitary bandages etc., note whether this is a 'real' drug
	BOOL complainworthyeffects = FALSE;

	// we might not use up the entire item, so reduce effects accordingly
	FLOAT effectivepercentage = uStatusUsed / 100.0;

	// if this alcohol, alcohol resistance can lower the effects
	if ( Item[usItem].alcohol > 0.0f )
	{
		effectivepercentage = effectivepercentage * ((100.0 - pSoldier->GetBackgroundValue( BG_RESI_ALCOHOL )) / 100.0);

		FLOAT weight = pSoldier->GetBodyWeight( );

		// the alcohol amounts in the xml are intended for a person with weight 80. We thus have to alter the effective value
		if ( weight > 0.0f )
		{
			effectivepercentage *= 80.0f / weight;
		}
	}

	// we now add drug, disease, personality and disability effects
	// every effect has a chance of happening (not entering a chance, so 0, always results in a effect for xml editing simplicity reasons)
	
	// add effects
	std::vector<DRUG_EFFECT> vec_drug = NewDrug[drugused].drug_effects;

	std::vector<DRUG_EFFECT>::iterator drug_effects_itend = vec_drug.end( );
	for ( std::vector<DRUG_EFFECT>::iterator drug_effects_it = vec_drug.begin( ); drug_effects_it != drug_effects_itend; ++drug_effects_it )
	{
		if ( !(*drug_effects_it).chance || Chance( (*drug_effects_it).chance ) )
		{
			// if we are already under influence of this effect, merge them, otherwise just add effect
			if ( pSoldier->newdrugs.duration[(*drug_effects_it).effect] )
			{
				INT32 effectsum_existing	= pSoldier->newdrugs.duration[(*drug_effects_it).effect] * pSoldier->newdrugs.size[(*drug_effects_it).effect];
				INT32 effectsum_new			= (*drug_effects_it).duration * (*drug_effects_it).size;

				INT32 effectsum_total = effectsum_existing + effectsum_new * effectivepercentage;

				UINT16 newduration = (pSoldier->newdrugs.duration[(*drug_effects_it).effect] + (*drug_effects_it).duration) / 2;

				if ( newduration )
				{
					INT32 newsize = effectsum_total / newduration;

					pSoldier->newdrugs.duration[(*drug_effects_it).effect]	= newduration;
					pSoldier->newdrugs.size[(*drug_effects_it).effect]		= newsize;
				}
			}
			else
			{
				pSoldier->newdrugs.duration[(*drug_effects_it).effect]	= (*drug_effects_it).duration;
				pSoldier->newdrugs.size[(*drug_effects_it).effect] = (*drug_effects_it).size * effectivepercentage;
			}

			complainworthyeffects = TRUE;
		}
	}

	// add diseases
	std::vector<DISEASE_EFFECT> vec_disease = NewDrug[drugused].disease_effects;

	std::vector<DISEASE_EFFECT>::iterator disease_effects_itend = vec_disease.end( );
	for ( std::vector<DISEASE_EFFECT>::iterator disease_effects_it = vec_disease.begin( ); disease_effects_it != disease_effects_itend; ++disease_effects_it )
	{
		if ( !(*disease_effects_it).chance || Chance( (*disease_effects_it).chance ) )
		{
			pSoldier->AddDiseasePoints( (*disease_effects_it).disease, (*disease_effects_it).size * effectivepercentage );
		}
	}

	// add disability
	std::vector<DISABILITY_EFFECT> vec_disability = NewDrug[drugused].disability_effects;

	std::vector<DISABILITY_EFFECT>::iterator disability_effects_itend = vec_disability.end( );
	for ( std::vector<DISABILITY_EFFECT>::iterator disability_effects_it = vec_disability.begin( ); disability_effects_it != disability_effects_itend; ++disability_effects_it )
	{
		if ( !(*disability_effects_it).chance || Chance( (*disability_effects_it).chance ) )
		{
			if ( pSoldier->newdrugs.drugdisability == (*disability_effects_it).disability )
			{
				pSoldier->newdrugs.drugdisability_duration += (*disability_effects_it).duration * effectivepercentage;
			}
			else
			{
				pSoldier->newdrugs.drugdisability = (*disability_effects_it).disability;
				pSoldier->newdrugs.drugdisability_duration = (*disability_effects_it).duration * effectivepercentage;
			}
		}
	}

	// add personality
	std::vector<PERSONALITY_EFFECT> vec_personality = NewDrug[drugused].personality_effects;

	std::vector<PERSONALITY_EFFECT>::iterator personality_effects_itend = vec_personality.end( );
	for ( std::vector<PERSONALITY_EFFECT>::iterator personality_effects_it = vec_personality.begin( ); personality_effects_it != personality_effects_itend; ++personality_effects_it )
	{
		if ( !(*personality_effects_it).chance || Chance( (*personality_effects_it).chance ) )
		{
			if ( pSoldier->newdrugs.drugpersonality == (*personality_effects_it).personality )
			{
				pSoldier->newdrugs.drugpersonality_duration += (*personality_effects_it).duration * effectivepercentage;
			}
			else
			{
				pSoldier->newdrugs.drugpersonality = (*personality_effects_it).personality;
				pSoldier->newdrugs.drugpersonality_duration = (*personality_effects_it).duration * effectivepercentage;
			}
		}
	}
	
	if ( complainworthyeffects )
	{
		// do switch for Larry!!
		if ( pSoldier->ubProfile == LARRY_NORMAL )
		{
			SwapToProfile( pSoldier, LARRY_DRUNK );

			gMercProfiles[LARRY_NORMAL].bNPCData = LARRY_FALLS_OFF_WAGON;
		}
		else if ( pSoldier->ubProfile == LARRY_DRUNK )
		{
			// NB store all drunkenness info in LARRY_NORMAL profile (to use same values)
			// so long as he keeps consuming, keep number above level at which he cracked						
			gMercProfiles[LARRY_NORMAL].bNPCData += (INT8)Random( 5 );

			// allow value to keep going up to 24 (about 2 days since we subtract Random( 2 ) when he has no access )
			gMercProfiles[LARRY_NORMAL].bNPCData = __min( gMercProfiles[LARRY_NORMAL].bNPCData, 24 );
			gMercProfiles[LARRY_NORMAL].bNPCData = __max( gMercProfiles[LARRY_NORMAL].bNPCData, LARRY_FALLS_OFF_WAGON );
		}

		if ( NewDrug[drugused].opinionevent )
		{
			HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_ADDICT, TRUE, TRUE );
		}
	}
	
	if ( Item[usItem].alcohol > 0.0f )
	{
		FLOAT weight = pSoldier->GetBodyWeight( );

		// the alcohol amounts in the xml are intended for a person with weight 80. We thus have to alter the effective value
		if ( weight > 0.0f )
		{
			// added promille = alcohol added (g) / (weight of person (kg) * 0.7)
			FLOAT addedpromille = (Item[usItem].alcohol * effectivepercentage) / (weight * 0.7);

			pSoldier->newdrugs.drinkstaken += addedpromille;
		}

		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_DRANK_SOME], pSoldier->GetName( ), ShortItemNames[usItem] );

		HandleDynamicOpinionTeamDrinking( pSoldier );
	}
	else
	{
		// set flag: we are on non-alcoholic drugs
		pSoldier->usSoldierFlagMask |= SOLDIER_DRUGGED;

		if ( complainworthyeffects && gMercProfiles[pSoldier->ubProfile].ubNumTimesDrugUseInLifetime != 255 )
		{
			gMercProfiles[pSoldier->ubProfile].ubNumTimesDrugUseInLifetime++;
		}

		if ( Item[usItem].cigarette )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_MERC_TOOK_CIGARETTE], pSoldier->GetName( ), ShortItemNames[usItem] );
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[MSG_MERC_TOOK_DRUG], pSoldier->GetName( ), ShortItemNames[usItem] );
		}
	}

	// Dirty panel
	fInterfacePanelDirty = DIRTYLEVEL2;

	return TRUE;
}

void HandleEndTurnDrugAdjustments_New( SOLDIERTYPE *pSoldier )
{
	// some effects are handled here
	if ( pSoldier->newdrugs.size[DRUG_EFFECT_HP] )
	{
		// note the current hp
		INT8 oldlife = pSoldier->stats.bLife;

		// increase life
		pSoldier->stats.bLife = __min( pSoldier->stats.bLife + pSoldier->newdrugs.size[DRUG_EFFECT_HP], pSoldier->stats.bLifeMax );

		//SANDRO - Insta-healable injury reduction
		if ( pSoldier->newdrugs.size[DRUG_EFFECT_HP] > 0 )
		{
			pSoldier->iHealableInjury = max( 0, (pSoldier->iHealableInjury - (100 * pSoldier->newdrugs.size[DRUG_EFFECT_HP])) );
		}

		if ( pSoldier->stats.bLife == pSoldier->stats.bLifeMax )
		{
			pSoldier->bBleeding = 0;
			pSoldier->iHealableInjury = 0;
		}
		else if ( pSoldier->bBleeding + pSoldier->stats.bLife > pSoldier->stats.bLifeMax )
		{
			// got to reduce amount of bleeding
			pSoldier->bBleeding = (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);
		}

		// display health change next time we are in tactical
		pSoldier->flags.fDisplayDamage = TRUE;
		pSoldier->sDamage -= pSoldier->stats.bLife - oldlife;
	}

	pSoldier->bExtraStrength += pSoldier->newdrugs.size[DRUG_EFFECT_STR];
	pSoldier->bExtraDexterity += pSoldier->newdrugs.size[DRUG_EFFECT_DEX];
	pSoldier->bExtraAgility += pSoldier->newdrugs.size[DRUG_EFFECT_AGI];
	pSoldier->bExtraWisdom += pSoldier->newdrugs.size[DRUG_EFFECT_WIS];

	BOOLEAN fStillDrugged = FALSE;

	// as time progresses, effects wear off
	for ( UINT8 i = 0; i < DRUG_EFFECT_MAX; ++i )
	{
		pSoldier->newdrugs.duration[i] = max( 0, pSoldier->newdrugs.duration[i] - 1 );

		if ( !pSoldier->newdrugs.duration[i] )
			pSoldier->newdrugs.size[i] = 0;
		else
			fStillDrugged = TRUE;
	}

	pSoldier->newdrugs.drugpersonality_duration = max( 0, pSoldier->newdrugs.drugpersonality_duration - 1 );

	if ( !pSoldier->newdrugs.drugpersonality_duration )
		pSoldier->newdrugs.drugpersonality = 0;
	else
		fStillDrugged = TRUE;

	pSoldier->newdrugs.drugdisability_duration = max( 0, pSoldier->newdrugs.drugdisability_duration - 1 );

	if ( !pSoldier->newdrugs.drugdisability_duration )
		pSoldier->newdrugs.drugdisability = 0;
	else
		fStillDrugged = TRUE;

	if ( !fStillDrugged )
	{
		pSoldier->usSoldierFlagMask &= ~SOLDIER_DRUGGED;

		fInterfacePanelDirty = DIRTYLEVEL1;
	}
}

INT8 GetDrunkLevel( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->usSoldierFlagMask2 & SOLDIER_HUNGOVER )
	{
		return HUNGOVER;
	}
	
	if ( pSoldier->newdrugs.drinkstaken <= 0.01 )
	{
		return SOBER;
	}
	else if ( pSoldier->newdrugs.drinkstaken <= 0.7 )
	{
		return FEELING_GOOD;
	}
	else if ( pSoldier->newdrugs.drinkstaken <= 2.0 )
	{
		return BORDERLINE;
	}
	
	return DRUNK;
}

// does a merc have a disability/personality, or is he under drugs that simulate this?
BOOLEAN DoesMercHaveDisability( SOLDIERTYPE *pSoldier, UINT8 aVal )
{
	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		if ( gMercProfiles[pSoldier->ubProfile].bDisability == aVal )
			return TRUE;
		
		if ( pSoldier->newdrugs.drugdisability == aVal )
			return TRUE;

		// Flugente: if disease with severe limitations is active, we can have multiple disabilities
		if ( gGameExternalOptions.fDisease
			&& gGameExternalOptions.fDiseaseSevereLimitations
			&& pSoldier->usDisabilityFlagMask & ( 1 << (aVal - 1) ) )
			return TRUE;
	}

	return FALSE;
}

BOOLEAN DoesMercHavePersonality( SOLDIERTYPE *pSoldier, UINT8 aVal )
{
	// personalities are new trait system only!
	if ( !gGameOptions.fNewTraitSystem )
		return FALSE;

	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		if ( gMercProfiles[pSoldier->ubProfile].bCharacterTrait == aVal )
			return TRUE;

		if ( pSoldier->newdrugs.drugpersonality == aVal )
			return TRUE;
	}

	return FALSE;
}

void HandleAPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *pubPoints )
{
	*pubPoints += pSoldier->newdrugs.size[DRUG_EFFECT_AP];
	
	if ( GetDrunkLevel( pSoldier ) == HUNGOVER )
	{
		// Reduce....
		*pubPoints -= HANGOVER_AP_REDUCE;

		if ( *pubPoints < APBPConstants[AP_MINIMUM] )
		{
			*pubPoints = APBPConstants[AP_MINIMUM];
		}
	}
}

void HandleBPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *psPointReduction )
{
	*psPointReduction -= pSoldier->newdrugs.size[DRUG_EFFECT_BP];
	
	if ( GetDrunkLevel( pSoldier ) == HUNGOVER )
	{
		// Reduce....
		(*psPointReduction) += HANGOVER_BP_REDUCE;
	}
}

INT32 EffectStatForBeingDrunk( SOLDIERTYPE *pSoldier, INT32 iStat )
{
	return( ( iStat * giDrunkModifier[ GetDrunkLevel( pSoldier ) ] / 100 ) );
}

BOOLEAN MercDruggedOrDrunk( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->newdrugs.drinkstaken )
		return TRUE;

	if ( pSoldier->usSoldierFlagMask & SOLDIER_DRUGGED )
		return TRUE;

	return FALSE;
}

BOOLEAN MercDrugged( SOLDIERTYPE *pSoldier )
{
	return (pSoldier->usSoldierFlagMask & SOLDIER_DRUGGED);
}

void HourlyDrugUpdate( )
{
	for ( UINT16 ubID = gTacticalStatus.Team[OUR_TEAM].bFirstID; ubID <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++ubID )
	{
		// every hour, we lower our alcohol counter
		if ( MercPtrs[ubID]->newdrugs.drinkstaken > 0.0f )
		{
			MercPtrs[ubID]->newdrugs.drinkstaken = max( 0.0, MercPtrs[ubID]->newdrugs.drinkstaken - 0.15f );

			if ( MercPtrs[ubID]->newdrugs.drinkstaken <= 0.0f )
			{
				MercPtrs[ubID]->usSoldierFlagMask2 &= ~SOLDIER_HUNGOVER;
			}
		}
	}
}
