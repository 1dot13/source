#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <stdlib.h>
	#include "Morale.h"
	#include "Overhead.h"
	#include "Soldier Profile.h"
	#include "dialogue control.h"
	#include "Map Screen Interface.h"
	#include "message.h"
	#include "assignments.h"
	#include "Strategic Movement.h"
	#include "Strategic Status.h"
	#include "SkillCheck.h"
	#include "drugs and alcohol.h"
	#include "StrategicMap.h"
	#include "Debug.h"
	#include "Squads.h"
	#include "ai.h"
	#include "Campaign.h"
	#include "mapscreen.h"
	#include "Soldier macros.h"
	#include "Event Pump.h"
	// HEADROCK HAM 3.5: Added for facility effect on morale
	#include "Facilities.h"
	// addedd by SANDRO
	#include "GameSettings.h"
	#include "Isometric Utils.h"
	#include "Food.h"
	#include "Interface.h"			// added by Flugente
	#include "finances.h"			// added by Flugente for EXTENDED_CONTRACT_BY_1_DAY
	#include "Soldier Add.h"		// added by Flugente for MERC_TYPE__AIM_MERC
	#include "CampaignStats.h"		// added by Flugente for gCurrentIncident
#endif

#include "connect.h"
#include "fresh_header.h"

#include "Random.h"

#include "Merc Contract.h"

//#define MORALE_MOD_MAX 50		// morale *mod* range is -50 to 50, if you change this, check the decay formulas!

//#define	DRUG_EFFECT_MORALE_MOD			150
//#define	ALCOHOL_EFFECT_MORALE_MOD		160

//#define HOURS_BETWEEN_STRATEGIC_DECAY 3

//#define PHOBIC_LIMIT -20

// macros
#define SOLDIER_IN_SECTOR( pSoldier, sX, sY, bZ )		( !pSoldier->flags.fBetweenSectors && ( pSoldier->sSectorX == sX ) && ( pSoldier->sSectorY == sY ) && ( pSoldier->bSectorZ == bZ ) )



MoraleEvent gbMoraleEvent[NUM_MORALE_EVENTS] =
{
	// TACTICAL = Short Term Effect, STRATEGIC = Long Term Effect
	{	TACTICAL_MORALE_EVENT,			+4},	//	MORALE_KILLED_ENEMY
	{ TACTICAL_MORALE_EVENT,		-5},	//	MORALE_SQUADMATE_DIED,		// in same sector (not really squad)... IN ADDITION to strategic loss of morale
	{ TACTICAL_MORALE_EVENT,			-1},	//	MORALE_SUPPRESSED,				// up to 4 times per turn
	{ TACTICAL_MORALE_EVENT,			-2},	//	MORALE_AIRSTRIKE,
	{ TACTICAL_MORALE_EVENT,			+2},	//	MORALE_DID_LOTS_OF_DAMAGE,
	{ TACTICAL_MORALE_EVENT,			-3},	//	MORALE_TOOK_LOTS_OF_DAMAGE,
	{ STRATEGIC_MORALE_EVENT,			-5},	//	MORALE_KILLED_CIVILIAN,
	{ STRATEGIC_MORALE_EVENT,			+4},	//	MORALE_BATTLE_WON,
	{ STRATEGIC_MORALE_EVENT,			-5},	//	MORALE_RAN_AWAY,
	{ STRATEGIC_MORALE_EVENT,			+2},	//	MORALE_HEARD_BATTLE_WON,
	{ STRATEGIC_MORALE_EVENT,			-2},	//	MORALE_HEARD_BATTLE_LOST,
	{ STRATEGIC_MORALE_EVENT,			+5},	//	MORALE_TOWN_LIBERATED,
	{ STRATEGIC_MORALE_EVENT,			-5},	//	MORALE_TOWN_LOST,
	{ STRATEGIC_MORALE_EVENT,			+8},	//	MORALE_MINE_LIBERATED,
	{ STRATEGIC_MORALE_EVENT,			-8},	//	MORALE_MINE_LOST,
	{ STRATEGIC_MORALE_EVENT,			+3},	//	MORALE_SAM_SITE_LIBERATED,
	{ STRATEGIC_MORALE_EVENT,			-3},	//	MORALE_SAM_SITE_LOST,
	{ STRATEGIC_MORALE_EVENT,		-15},	//	MORALE_BUDDY_DIED,
	{ STRATEGIC_MORALE_EVENT,			+5},	//	MORALE_HATED_DIED,
	{ STRATEGIC_MORALE_EVENT,			-5},	//	MORALE_TEAMMATE_DIED,			// not in same sector
	{ STRATEGIC_MORALE_EVENT,			+5},	//	MORALE_LOW_DEATHRATE,
	{ STRATEGIC_MORALE_EVENT,			-5},	//	MORALE_HIGH_DEATHRATE,
	{ STRATEGIC_MORALE_EVENT,			+2},	//	MORALE_GREAT_MORALE,
	{ STRATEGIC_MORALE_EVENT,			-2},	//	MORALE_POOR_MORALE,
	{ TACTICAL_MORALE_EVENT,			-5},	//	MORALE_DRUGS_CRASH				// Flugente: lowered as drugs can have several components that have a backlash, causing the the morale to plummet drastical
	{ TACTICAL_MORALE_EVENT,		-10},	//	MORALE_ALCOHOL_CRASH
	{ STRATEGIC_MORALE_EVENT,		+15},	//	MORALE_MONSTER_QUEEN_KILLED
	{ STRATEGIC_MORALE_EVENT,		+25},	//	MORALE_DEIDRANNA_KILLED
	{ TACTICAL_MORALE_EVENT,			-1},	//	MORALE_CLAUSTROPHOBE_UNDERGROUND,
	{ TACTICAL_MORALE_EVENT,			-5},	//	MORALE_INSECT_PHOBIC_SEES_CREATURE,
	{ TACTICAL_MORALE_EVENT,			-1},	//	MORALE_NERVOUS_ALONE,
	{	STRATEGIC_MORALE_EVENT,		-5},	//	MORALE_MERC_CAPTURED,
	{ STRATEGIC_MORALE_EVENT,			-5},	//	MORALE_MERC_MARRIED,
	{ STRATEGIC_MORALE_EVENT,			+8},	//	MORALE_QUEEN_BATTLE_WON,
	{ STRATEGIC_MORALE_EVENT,			+5},	//	MORALE_SEX,
	// added by SANDRO
	{ STRATEGIC_MORALE_EVENT,			-1},	//	MORALE_HEAT_INTOLERANT_IN_DESERT,
	{ TACTICAL_MORALE_EVENT,			-1},	//	MORALE_PSYCHO_UNABLE_TO_PSYCHO,
	{ STRATEGIC_MORALE_EVENT,			+1},	//	MORALE_PACIFIST_GAIN_NONCOMBAT,
	{ TACTICAL_MORALE_EVENT,			+1},	//	MORALE_MALICIOUS_HIT,
	// added by Flugente
	{ TACTICAL_MORALE_EVENT,			1},		//MORALE_FOOD,
	{ TACTICAL_MORALE_EVENT,			5},		//MORALE_GOOD_FOOD,
	{ TACTICAL_MORALE_EVENT,			-1},	//MORALE_BAD_FOOD,
	{ TACTICAL_MORALE_EVENT,			-5},	//MORALE_LOATHSOME_FOOD,
	// added by anv
	{ STRATEGIC_MORALE_EVENT,			-5},	//MORALE_BUDDY_FIRED,
	{ STRATEGIC_MORALE_EVENT,			-8},	//MORALE_BUDDY_FIRED_ON_BAD_TERMS,
	{ STRATEGIC_MORALE_EVENT,			-8},	//MORALE_BUDDY_FIRED_EARLY
	{ STRATEGIC_MORALE_EVENT,			-2},	//MORALE_BAD_EQUIPMENT,
	{ STRATEGIC_MORALE_EVENT,			-3},	//MORALE_OWED_MONEY,
	{ STRATEGIC_MORALE_EVENT,			-3},	//MORALE_PLAYER_INACTIVE,
	{ STRATEGIC_MORALE_EVENT,			-1},	//MORALE_PREVENTED_MISBEHAVIOUR,
};

BOOLEAN gfSomeoneSaidMoraleQuote = FALSE;

BOOLEAN IsShowOffNearBy( SOLDIERTYPE * pSoldier ); // Added by SANDRO

INT8 GetMoraleModifier( SOLDIERTYPE * pSoldier )
{
	if (pSoldier->flags.uiStatusFlags & SOLDIER_PC)
	{
		if (pSoldier->aiData.bMorale > 50)
		{
			// give +1 at 55, +3 at 65, up to +5 at 95 and above
			return( (pSoldier->aiData.bMorale - 45) / 10 );
		}
		else
		{
			// give penalties down to -20 at 0 (-2 at 45, -4 by 40...)
			return( (pSoldier->aiData.bMorale - 50) * 2 / 5 );
		}
	}
	else
	{
		INT8 morale = 0;

		// use AI morale
		switch( pSoldier->aiData.bAIMorale )
		{
			case MORALE_HOPELESS:
				morale = -15;
			case MORALE_WORRIED:
				morale = -7;
			case MORALE_CONFIDENT:
				morale = -2;
			case MORALE_FEARLESS:
				morale = -5;
			default:
				morale = 0;
		}

		// Flugente: morale modifiers
		morale = max(morale, morale * pSoldier->GetMoraleModifier());

		return morale;
	}
}

void DecayTacticalMorale( SOLDIERTYPE * pSoldier )
{
	// decay the tactical morale modifier
	if (pSoldier->aiData.bTacticalMoraleMod != 0)
	{
		// decay the modifier!
		if (pSoldier->aiData.bTacticalMoraleMod > 0)
		{
			pSoldier->aiData.bTacticalMoraleMod = __max( 0, pSoldier->aiData.bTacticalMoraleMod - (8 - pSoldier->aiData.bTacticalMoraleMod / 10) );
		}
		else
		{
			pSoldier->aiData.bTacticalMoraleMod = __min( 0, pSoldier->aiData.bTacticalMoraleMod + (6 + pSoldier->aiData.bTacticalMoraleMod / 10) );
		}
	}
}

void DecayStrategicMorale( SOLDIERTYPE * pSoldier )
{
	// HEADROCK HAM 3.5: Strategic Morale Mod no longer normalizes to 0 by default. In fact, a local facility can
	// cause normalization to another value (positive or negative!), based on the activity that the character is
	// currently performing. It can literally reduce morale to ridiculously low amounts if you don't watch your
	// assignments in some sectors.

	// decay the modifier!
	if (pSoldier->aiData.bStrategicMoraleMod > 0)
	{
		pSoldier->aiData.bStrategicMoraleMod = __max( 0, pSoldier->aiData.bStrategicMoraleMod - (8 - (pSoldier->aiData.bStrategicMoraleMod / 10)) );
	}
	else
	{
		pSoldier->aiData.bStrategicMoraleMod = __min( 0, pSoldier->aiData.bStrategicMoraleMod + (6 + (pSoldier->aiData.bStrategicMoraleMod / 10)) );
	}
}

void DecayTacticalMoraleModifiers( void )
{
	SOLDIERTYPE * pSoldier;
	UINT8 ubLoop, ubLoop2;
	BOOLEAN				fHandleNervous;

	ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pSoldier++ )
	{
		//if the merc is active, in Arulco
		// CJC: decay modifiers while asleep! or POW!
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE &&
															!(pSoldier->bAssignment == IN_TRANSIT ||
																	pSoldier->bAssignment == ASSIGNMENT_DEAD ) )
		{
			// only let morale mod decay if it is positive while merc is a POW
			if ( pSoldier->bAssignment == ASSIGNMENT_POW && pSoldier->aiData.bTacticalMoraleMod < 0 )
			{
				continue;
			}

			// Flugente: drugs can temporarily cause a merc get a new disability
			// therefore we change this routine
			BOOLEAN isClaustrophobic	= gMercProfiles[ pSoldier->ubProfile ].bDisability == CLAUSTROPHOBIC ? TRUE : FALSE;
			BOOLEAN isNervous			= gMercProfiles[ pSoldier->ubProfile ].bDisability == NERVOUS ? TRUE : FALSE;
			
			if ( MercUnderTheInfluence(pSoldier, DRUG_TYPE_CLAUSTROPHOBIC) )
				isClaustrophobic = TRUE;

			if ( MercUnderTheInfluence(pSoldier, DRUG_TYPE_NERVOUS) )
				isNervous = TRUE;

			if ( isClaustrophobic )
			{
				if ( pSoldier->bSectorZ > 0 )
				{
					// underground, no recovery... in fact, if tact morale is high, decay
					if ( pSoldier->aiData.bTacticalMoraleMod > gMoraleSettings.bModifiers[PHOBIC_LIMIT] )
					{
						HandleMoraleEvent( pSoldier, MORALE_CLAUSTROPHOBE_UNDERGROUND, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
					}
				}
			}

			if ( isNervous )
			{
				if ( pSoldier->aiData.bMorale < 50 )
				{
					if (pSoldier->ubGroupID != 0 && PlayerIDGroupInMotion( pSoldier->ubGroupID ))
 					{
						if ( NumberOfPeopleInSquad( pSoldier->bAssignment ) == 1 )
						{
							fHandleNervous = TRUE;
						}
						else
						{
							fHandleNervous = FALSE;
						}
					}
					else if ( pSoldier->bActive && pSoldier->bInSector )
					{
						if ( DistanceToClosestFriend( pSoldier ) > NERVOUS_RADIUS )
						{
							fHandleNervous = TRUE;
						}
						else
						{
							fHandleNervous = FALSE;
						}
					}
					else
					{
						// look for anyone else in same sector
						fHandleNervous = TRUE;
						for ( ubLoop2 = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; ubLoop2 <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop2++ )
						{
							if ( MercPtrs[ ubLoop2 ] != pSoldier && MercPtrs[ ubLoop2 ]->bActive && MercPtrs[ ubLoop2 ]->sSectorX == pSoldier->sSectorX && MercPtrs[ ubLoop2 ]->sSectorY == pSoldier->sSectorY && MercPtrs[ ubLoop2 ]->bSectorZ == pSoldier->bSectorZ )
							{
								// found someone!
								fHandleNervous = FALSE;
								break;
							}
						}
					}

					if ( fHandleNervous )
					{
						if ( pSoldier->aiData.bTacticalMoraleMod == gMoraleSettings.bModifiers[PHOBIC_LIMIT] )
						{
							// don't change morale
							//continue;
						}
						else
						{
							// alone, no recovery... in fact, if tact morale is high, decay
							if ( !(pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_PERSONALITY) )
							{
								TacticalCharacterDialogue( pSoldier, QUOTE_PERSONALITY_TRAIT );
								pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_PERSONALITY;
							}
							HandleMoraleEvent( pSoldier, MORALE_NERVOUS_ALONE, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );

							// Flugente: dynamic opinions
							HandleDynamicOpinionDisability( pSoldier );
						}
					}
				}
			}

			DecayTacticalMorale( pSoldier );
			RefreshSoldierMorale( pSoldier );
		}
	}
}

void DecayStrategicMoraleModifiers( void )
{
	SOLDIERTYPE * pSoldier;
	UINT8 ubLoop;

	ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( pSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pSoldier++ )
	{
		//if the merc is active, in Arulco
		// CJC: decay modifiers while asleep! or POW!
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE &&
															!(pSoldier->bAssignment == IN_TRANSIT ||
																pSoldier->bAssignment == ASSIGNMENT_DEAD ) )
		{
			// only let morale mod decay if it is positive while merc is a POW
			if ( pSoldier->bAssignment == ASSIGNMENT_POW && pSoldier->aiData.bStrategicMoraleMod < 0 )
			{
				continue;
			}

			DecayStrategicMorale( pSoldier );
			RefreshSoldierMorale( pSoldier );
		}
	}
}



void RefreshSoldierMorale( SOLDIERTYPE * pSoldier )
{
	INT32		iActualMorale;

	if ( pSoldier->flags.fMercAsleep )
	{
		// delay this till later!
		return;
	}

	// CJC, April 19, 1999: added up to 20% morale boost according to progress
	//iActualMorale = DEFAULT_MORALE + (INT32) pSoldier->aiData.bTeamMoraleMod + (INT32) pSoldier->aiData.bTacticalMoraleMod + (INT32) pSoldier->aiData.bStrategicMoraleMod + (INT32) (CurrentPlayerProgressPercentage() / 5);
	iActualMorale = gMoraleSettings.ubDefaultMorale + (INT32) pSoldier->aiData.bTeamMoraleMod + (INT32) pSoldier->aiData.bTacticalMoraleMod + (INT32) pSoldier->aiData.bStrategicMoraleMod + (INT32) (CurrentPlayerProgressPercentage() / 5);

	// ATE: Modify morale based on drugs....
	iActualMorale	+= ( ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ADRENALINE ] * gMoraleSettings.sDrugAndAlcoholModifiers[DRUG_EFFECT_MORALE_MOD] ) / 100 );
	iActualMorale	+= ( ( pSoldier->drugs.bDrugEffect[ DRUG_TYPE_ALCOHOL ] * gMoraleSettings.sDrugAndAlcoholModifiers[ALCOHOL_EFFECT_MORALE_MOD] ) / 100 );

	iActualMorale = __min( 100, iActualMorale );
	iActualMorale = __max( 0, iActualMorale );

	UINT8 ubMaxMorale = 100;
	// HEADROCK HAM 3.5: Local facilities may decrease the total morale allowed.
	for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; cnt++)
	{
		if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
		{
			if (cnt == (UINT16)pSoldier->sFacilityTypeOperated && // Soldier is operating this facility
				GetSoldierFacilityAssignmentIndex( pSoldier ) != -1) 
			{
				UINT8 ubFacilityType = (UINT8)cnt;
				UINT8 ubAssignmentType = (UINT8)GetSoldierFacilityAssignmentIndex( pSoldier );
				// Check this facility both for an assignment-specific AND ambient value. Use it if it's the lowest
				// encountered yet.
				ubMaxMorale = __min(ubMaxMorale, (UINT8)GetFacilityModifier(FACILITY_MAX_MORALE, ubFacilityType, ubAssignmentType ));
			}
			else // Soldier is not operating this facility
			{
				// Check this facility for an AMBIENT Maximum Morale limit. Use it if it's the lowest encountered yet.
				ubMaxMorale = __min(ubMaxMorale, (UINT8)GetFacilityModifier(FACILITY_MAX_MORALE, (UINT8)cnt, FAC_AMBIENT));
			}
		}
	}

	// Flugente: morale modifiers
	iActualMorale = iActualMorale * pSoldier->GetMoraleModifier();

	// Flugente: ubMaxMorale can now be influenced by our food situation
	if ( gGameOptions.fFoodSystem )
		FoodMaxMoraleModifiy(pSoldier, &ubMaxMorale);

	// Flugente: max morale can be lowered
	iActualMorale = min(iActualMorale, pSoldier->GetMoraleThreshold() );

	if (ubMaxMorale > 0 && iActualMorale > ubMaxMorale)
	{
		// Normalize to Max Morale
		iActualMorale = (iActualMorale + ubMaxMorale) / 2;
	}

	pSoldier->aiData.bMorale = (INT8) iActualMorale;

	// update mapscreen as needed
	fCharacterInfoPanelDirty = TRUE;
}

// SANDRO - changed this a bit
void UpdateSoldierMorale( SOLDIERTYPE * pSoldier, INT8 bMoraleEvent )
{
	MERCPROFILESTRUCT *		pProfile;
	INT32									iMoraleModTotal;
	UINT8 ubType;
	INT8 bMoraleMod = 0;

	if ( !pSoldier->bActive || ( pSoldier->stats.bLife < CONSCIOUSNESS ) ||
		( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) || AM_AN_EPC( pSoldier ) )
	{
		return;
	}

	if ( ( pSoldier->bAssignment == ASSIGNMENT_DEAD ) ||
			( pSoldier->bAssignment == ASSIGNMENT_POW ) ||
			( pSoldier->bAssignment == IN_TRANSIT ) )
	{
		return;
	}


	if ( pSoldier->ubProfile == NO_PROFILE )
	{
		return;
	}
	
	if(cDisableMorale && is_networked)
	{
		return;
	}//hayden

	ubType = gbMoraleEvent[bMoraleEvent].ubType;
	bMoraleMod = gMoraleSettings.bValues[bMoraleEvent];//gbMoraleEvent[bMoraleEvent].bChange;

	pProfile = &(gMercProfiles[ pSoldier->ubProfile ]);

	if (bMoraleMod > 0)
	{
		if ( gGameOptions.fNewTraitSystem )
		{
			// SANDRO - STOMP traits 
			// Squadleader's bonus to morale 
			if ( IS_MERC_BODY_TYPE( pSoldier ) && (pSoldier->bTeam == ENEMY_TEAM || pSoldier->bTeam == MILITIA_TEAM || pSoldier->bTeam == gbPlayerNum) )
			{
				bMoraleMod += (gSkillTraitValues.ubSLMoraleGainBonus * GetSquadleadersCountInVicinity( pSoldier, FALSE, FALSE ));
			}
			// Check for character traits
			INT8 bNumMercs = 0;
			switch( pProfile->bCharacterTrait )
			{
				case CHAR_TRAIT_SOCIABLE:
					bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_SOCIABLE );
					if ( bNumMercs == 0 )
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_SOCIABLE_NO_MERCS_NEARBY];// -= 5;
					else if ( bNumMercs == 1 )
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_SOCIABLE_ONE_MERC_NEARBY];// -= 2;
				break;
				case CHAR_TRAIT_LONER:
					bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_LONER );
					if ( bNumMercs > 1 )
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_LONER_MORE_MERCS_NEARBY];// -= 5;
					else if ( bNumMercs == 1 )
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_LONER_ONE_MERC_NEARBY];// -= 2;
				break;
				case CHAR_TRAIT_OPTIMIST:
					bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OPTIMIST];//1;
					break;
				case CHAR_TRAIT_AGGRESSIVE:
					switch ( bMoraleEvent )
					{
						case MORALE_KILLED_ENEMY:
						case MORALE_DID_LOTS_OF_DAMAGE:
						case MORALE_MONSTER_QUEEN_KILLED:
						case MORALE_DEIDRANNA_KILLED:
							bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_AGRESSIVE_VIOLENT_ACTION];//1;
							break;
					}
					break;
				case CHAR_TRAIT_PACIFIST:
					switch ( bMoraleEvent )
					{
						case MORALE_KILLED_ENEMY:
						case MORALE_DID_LOTS_OF_DAMAGE:
						case MORALE_BATTLE_WON:
						case MORALE_HEARD_BATTLE_WON:
						case MORALE_HATED_DIED:
						case MORALE_MONSTER_QUEEN_KILLED:
						case MORALE_DEIDRANNA_KILLED:
						case MORALE_QUEEN_BATTLE_WON:
							bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_PACIFIST_VIOLENT_ACTION];//-= 5;
							break;
					}
					break;
			}
			if ( IsShowOffNearBy( pSoldier ) )
			{
				bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_SHOWOFF_AROUND];//-= 2;
			}

		}
		else if ( bMoraleMod != MORALE_PSYCHO_UNABLE_TO_PSYCHO && bMoraleMod != MORALE_MALICIOUS_HIT)
		{
			switch( pProfile->bAttitude )
			{
				case ATT_OPTIMIST:
					bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OT_OPTIMIST_GOOD_EVENT];//1;
					break;
				case ATT_AGGRESSIVE:
					bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OT_AGGRESSIVE_GOOD_EVENT];//1;
					break;
				case ATT_PESSIMIST:
					bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OT_PESSIMIST_GOOD_EVENT];//-= 1;
					break;
				default:
					break;
			}
		}

		if (bMoraleMod < 0)
		{
			// can't change a positive event into a negative one!
			bMoraleMod = 0;
		}
	}
	else
	{
		if ( gGameOptions.fNewTraitSystem )
		{
			// SANDRO - STOMP traits - squadleader's bonus to morale 
			if ( IS_MERC_BODY_TYPE( pSoldier ) && (pSoldier->bTeam == ENEMY_TEAM || pSoldier->bTeam == MILITIA_TEAM || pSoldier->bTeam == gbPlayerNum) )
			{
				switch ( bMoraleEvent ) // certain thing are not supported by our squadleader
				{
					case MORALE_HATED_DIED:
					case MORALE_MERC_MARRIED:
					case MORALE_SEX:
					case MORALE_PSYCHO_UNABLE_TO_PSYCHO:
					case MORALE_MALICIOUS_HIT:
						break;
					default:
						bMoraleMod += (gSkillTraitValues.ubSLMoraleLossReduction * GetSquadleadersCountInVicinity( pSoldier, FALSE, FALSE ));
						break;
				}
			}
			// Check for character traits
			if ( pProfile->bCharacterTrait == CHAR_TRAIT_OPTIMIST )
			{
				bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OPTIMIST];//1;
			}
			// Fearless character does not suffer morale loss for these so much
			else if( pProfile->bCharacterTrait == CHAR_TRAIT_DAUNTLESS )
			{
				switch ( bMoraleEvent )
				{
					case MORALE_SQUADMATE_DIED:
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_DAUNTLESS_SQUADMATE_DIED];//3;
						break;
					case MORALE_SUPPRESSED:
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_DAUNTLESS_SUPPRESSED];//3;
						break;
					case MORALE_TOOK_LOTS_OF_DAMAGE:
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_DAUNTLESS_TOOK_LOTS_OF_DAMAGE];//3;
						break;
					case MORALE_TEAMMATE_DIED:
						bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_DAUNTLESS_TEAMMATE_DIED];//2;
						break;
				}
			}
		}
		else if ( bMoraleMod != MORALE_PSYCHO_UNABLE_TO_PSYCHO )
		{
			switch( pProfile->bAttitude )
			{
				case ATT_OPTIMIST:
					bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OT_OPTIMIST_BAD_EVENT];//1;
					break;
				case ATT_PESSIMIST:
					bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OT_PESSIMIST_BAD_EVENT];//-= 1;
					break;
				case ATT_COWARD:
					bMoraleMod += gMoraleSettings.bModifiers[MORALE_MODIFIER_OT_COWARD_BAD_EVENT];//-= 2;
				default:
					break;
			}
		}

		// Flugente: what the heck? why is morale lowered or increased depending on our height? This doesn't make any sense. AT ALL.
		/*if (pSoldier->pathing.bLevel == 1)
		{
			bMoraleMod--;
		}
		else if (pSoldier->pathing.bLevel > 5)
		{
			bMoraleMod++;
		}*/

		if (bMoraleMod > 0)
		{
			// can't change a negative event into a positive one!
			bMoraleMod = 0;
		}
	}
	// apply change!
	if (ubType == TACTICAL_MORALE_EVENT)
	{
		iMoraleModTotal = (INT32) pSoldier->aiData.bTacticalMoraleMod + (INT32) bMoraleMod;
		iMoraleModTotal = __min( iMoraleModTotal, gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
		iMoraleModTotal = __max( iMoraleModTotal, -gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
		pSoldier->aiData.bTacticalMoraleMod = (INT8) iMoraleModTotal;
	}
	else if ( gTacticalStatus.fEnemyInSector && !pSoldier->bInSector ) // delayed strategic
	{
		iMoraleModTotal = (INT32) pSoldier->bDelayedStrategicMoraleMod + (INT32) bMoraleMod;
		iMoraleModTotal = __min( iMoraleModTotal, gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
		iMoraleModTotal = __max( iMoraleModTotal, -gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
		pSoldier->bDelayedStrategicMoraleMod = (INT8) iMoraleModTotal;
	}
	else // strategic
	{
		iMoraleModTotal = (INT32) pSoldier->aiData.bStrategicMoraleMod + (INT32) bMoraleMod;
		iMoraleModTotal = __min( iMoraleModTotal, gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
		iMoraleModTotal = __max( iMoraleModTotal, -gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
		pSoldier->aiData.bStrategicMoraleMod = (INT8) iMoraleModTotal;
	}

	RefreshSoldierMorale( pSoldier );

	if ( !pSoldier->flags.fMercAsleep )
	{
		if ( !gfSomeoneSaidMoraleQuote )
		{
			// Check if we're below a certain value and warn
			if ( pSoldier->aiData.bMorale < 35 )
			{
				// Have we said this quote yet?
				if ( !(	pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_LOW_MORAL ) )
				{
					gfSomeoneSaidMoraleQuote = TRUE;

					// ATE: Amde it a DELAYED QUOTE - will be delayed by the dialogue Q until it's our turn...
					DelayedTacticalCharacterDialogue( pSoldier, QUOTE_STARTING_TO_WHINE );
					pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_LOW_MORAL;
				}
			}
		}
	}

	// Reset flag!
	if ( pSoldier->aiData.bMorale > 65 )
	{
		pSoldier->usQuoteSaidFlags &= (~SOLDIER_QUOTE_SAID_LOW_MORAL );
	}

}


void HandleMoraleEventForSoldier( SOLDIERTYPE * pSoldier, INT8 bMoraleEvent )
{
	// SANDRO - changed this to send the event forward
	UpdateSoldierMorale( pSoldier, bMoraleEvent ); //, gbMoraleEvent[bMoraleEvent].ubType, gbMoraleEvent[bMoraleEvent].bChange );
}


void HandleMoraleEvent( SOLDIERTYPE *pSoldier, INT8 bMoraleEvent, INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT8 ubLoop;
	SOLDIERTYPE *					pTeamSoldier;
	MERCPROFILESTRUCT *		pProfile;

	gfSomeoneSaidMoraleQuote = FALSE;

	// NOTE: Many morale events are NOT attached to a specific player soldier at all!
	// Those that do need it have Asserts on a case by case basis below
	if (pSoldier == NULL)
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Handling morale event %d at X=%d, Y=%d,Z=%d", bMoraleEvent, sMapX, sMapY, bMapZ ) );
	}
	else
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Handling morale event %d for %S at X=%d, Y=%d, Z=%d", bMoraleEvent, pSoldier->GetName(), sMapX, sMapY, bMapZ ) );
	}


	switch( bMoraleEvent )
	{
		case MORALE_KILLED_ENEMY:
		case MORALE_DID_LOTS_OF_DAMAGE:
		case MORALE_DRUGS_CRASH:
		case MORALE_ALCOHOL_CRASH:
		case MORALE_SUPPRESSED:
		case MORALE_TOOK_LOTS_OF_DAMAGE:
		case MORALE_HIGH_DEATHRATE:
		case MORALE_SEX:
			// needs specific soldier!
			Assert( pSoldier );
			// affects the soldier only
			HandleMoraleEventForSoldier( pSoldier, bMoraleEvent );
			break;

		case MORALE_CLAUSTROPHOBE_UNDERGROUND:
		case MORALE_INSECT_PHOBIC_SEES_CREATURE:
		case MORALE_NERVOUS_ALONE:
		case MORALE_HEAT_INTOLERANT_IN_DESERT: // SANDRO added this one
			// needs specific soldier!
			Assert( pSoldier );
			// affects the soldier only, should be ignored if tactical morale mod is -20 or less
			// SANDRO - really?? I think it also shouldn't go lower if strategic morale mod is less than -20
			if ( pSoldier->aiData.bTacticalMoraleMod > gMoraleSettings.bModifiers[PHOBIC_LIMIT] && pSoldier->aiData.bStrategicMoraleMod > gMoraleSettings.bModifiers[PHOBIC_LIMIT] )
			{
				HandleMoraleEventForSoldier( pSoldier, bMoraleEvent );
			}
			break;

		case MORALE_BATTLE_WON:
			// affects everyone to varying degrees
			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive )
				{
					if ( SOLDIER_IN_SECTOR( pTeamSoldier, sMapX, sMapY, bMapZ ) )
					{
						HandleMoraleEventForSoldier( pTeamSoldier, MORALE_BATTLE_WON );
					}
					// SANDRO - Assertive people don't care about actions of others
					else if ( !gGameOptions.fNewTraitSystem || gMercProfiles[pTeamSoldier->ubProfile].bCharacterTrait != CHAR_TRAIT_ASSERTIVE )
					{
						HandleMoraleEventForSoldier( pTeamSoldier, MORALE_HEARD_BATTLE_WON );
					}
				}
			}
			break;
		case MORALE_RAN_AWAY:
			// affects everyone to varying degrees
			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive )
				{
					// CJC: adding to SOLDIER_IN_SECTOR check special stuff because the old sector values might
					// be appropriate (because in transit going out of that sector!)

					if ( SOLDIER_IN_SECTOR( pTeamSoldier, sMapX, sMapY, bMapZ ) || ( pTeamSoldier->flags.fBetweenSectors && ((pTeamSoldier->ubPrevSectorID % 16) + 1) == sMapX && ((pTeamSoldier->ubPrevSectorID / 16) + 1) == sMapY && ( pTeamSoldier->bSectorZ == bMapZ ) ) )
					{
						if ( gGameOptions.fNewTraitSystem )
						{
							// Flugente: if we have the covert trait and are covert, we might simply be returning from a reconnaissance mission in enemy territory. No need for a morale drop in this case
							if ( !HAS_SKILL_TRAIT( pTeamSoldier, COVERT_NT ) || ( (pTeamSoldier->usSoldierFlagMask & (SOLDIER_COVERT_CIV|SOLDIER_COVERT_SOLDIER)) == 0) )
							{
								// SANDRO - no penalty for pacifists to run away
								if ( gMercProfiles[pTeamSoldier->ubProfile].bCharacterTrait != CHAR_TRAIT_PACIFIST )
									HandleMoraleEventForSoldier( pTeamSoldier, MORALE_RAN_AWAY );
								// Double morale drop for aggressive people
								if  ( gMercProfiles[pTeamSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
									HandleMoraleEventForSoldier( pTeamSoldier, MORALE_RAN_AWAY );
							}
						}
						else
						{
							switch ( gMercProfiles[ pTeamSoldier->ubProfile ].bAttitude )
							{
								case ATT_AGGRESSIVE:
									// double the penalty - these guys REALLY hate running away
									HandleMoraleEventForSoldier( pTeamSoldier, MORALE_RAN_AWAY );
									HandleMoraleEventForSoldier( pTeamSoldier, MORALE_RAN_AWAY );
									break;
								case ATT_COWARD:
									// no penalty - cowards are perfectly happy to avoid fights!
									break;
								default:
									HandleMoraleEventForSoldier( pTeamSoldier, MORALE_RAN_AWAY );
									break;
							}
						}
					}
					// SANDRO - Assertive people don't care about actions of others
					else if ( !gGameOptions.fNewTraitSystem || gMercProfiles[pTeamSoldier->ubProfile].bCharacterTrait != CHAR_TRAIT_ASSERTIVE )
					{
						HandleMoraleEventForSoldier( pTeamSoldier, MORALE_HEARD_BATTLE_LOST );
					}
				}
			}
			break;

		case MORALE_TOWN_LIBERATED:
		case MORALE_TOWN_LOST:
		case MORALE_MINE_LIBERATED:
		case MORALE_MINE_LOST:
		case MORALE_SAM_SITE_LIBERATED:
		case MORALE_SAM_SITE_LOST:
		case MORALE_KILLED_CIVILIAN:
		case MORALE_LOW_DEATHRATE:
		case MORALE_HEARD_BATTLE_WON:
		case MORALE_HEARD_BATTLE_LOST:
		case MORALE_MONSTER_QUEEN_KILLED:
		case MORALE_DEIDRANNA_KILLED:
			// affects everyone, everywhere
			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive )
				{
					if ( gGameOptions.fNewTraitSystem && bMoraleEvent != MORALE_DEIDRANNA_KILLED)
					{
						if ( !SOLDIER_IN_SECTOR( pTeamSoldier, sMapX, sMapY, bMapZ ) && ( gMoraleSettings.bValues[bMoraleEvent] > 0 ) && //( gbMoraleEvent[bMoraleEvent].bChange > 0 ) && 
							gMercProfiles[pTeamSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_ASSERTIVE )
						{
							// No morale gain for assertive people from actions of others
						}
						else
						{
							HandleMoraleEventForSoldier( pTeamSoldier, bMoraleEvent );
						}
					}
					else
					{
						HandleMoraleEventForSoldier( pTeamSoldier, bMoraleEvent );
					}
				}
			}
			break;

		case MORALE_POOR_MORALE:
		case MORALE_GREAT_MORALE:
		case MORALE_AIRSTRIKE:
			// affects every in sector
			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive && SOLDIER_IN_SECTOR( pTeamSoldier, sMapX, sMapY, bMapZ ) )
				{
					HandleMoraleEventForSoldier( pTeamSoldier, bMoraleEvent );
				}
			}
			break;

		case MORALE_MERC_CAPTURED:
			// needs specific soldier! (for reputation, not here)
			Assert( pSoldier );

			// affects everyone
			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive )
				{
					HandleMoraleEventForSoldier( pTeamSoldier, bMoraleEvent );
				}
			}
			break;
		case MORALE_TEAMMATE_DIED:
			// needs specific soldier!
			Assert( pSoldier );

			// affects everyone, in sector differently than not, extra bonuses if it's a buddy or hated merc
			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE)
				{
					pProfile = &(gMercProfiles[ pTeamSoldier->ubProfile ]);

					if (HATED_MERC( pProfile, pSoldier->ubProfile ))
					{
						// yesss!
						HandleMoraleEventForSoldier( pTeamSoldier, MORALE_HATED_DIED );
					}
					else
					{
						if ( SOLDIER_IN_SECTOR( pTeamSoldier, sMapX, sMapY, bMapZ ) )
						{
							// mate died in my sector!	tactical morale mod
							HandleMoraleEventForSoldier( pTeamSoldier, MORALE_SQUADMATE_DIED );

							// SANDRO - Squadleaders death cost a lot of morale
							if( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SQUADLEADER_NT ))
							{
								for ( UINT8 i = 0; i < (gSkillTraitValues.ubSLDeathMoralelossMultiplier * NUM_SKILL_TRAITS( pSoldier, SQUADLEADER_NT )); i++ )
								{
									HandleMoraleEventForSoldier( pTeamSoldier, MORALE_SQUADMATE_DIED );
								}
							}
						}

						// this is handled for everyone even if in sector, as it's a strategic morale mod
						HandleMoraleEventForSoldier( pTeamSoldier, MORALE_TEAMMATE_DIED );

						// SANDRO - Squadleaders death cost a lot of morale
						if( gGameOptions.fNewTraitSystem && (NUM_SKILL_TRAITS( pSoldier, SQUADLEADER_NT ) > 1))
						{
							for ( UINT8 i = 0; i < gSkillTraitValues.ubSLDeathMoralelossMultiplier; i++ )
							{
								HandleMoraleEventForSoldier( pTeamSoldier, MORALE_SQUADMATE_DIED );
							}
						}
						
						if (BUDDY_MERC( pProfile, pSoldier->ubProfile ))
						{
							// oh no!	buddy died!
							HandleMoraleEventForSoldier( pTeamSoldier, MORALE_BUDDY_DIED );
						}
					}
				}
			}
			break;

		case MORALE_MERC_MARRIED:
			// female mercs get unhappy based on how sexist they are (=hate men)
			// gentlemen males get unhappy too

			ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ ubLoop ]; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++, pTeamSoldier++ )
			{
				if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE )
				{
					if ( WhichHated( pTeamSoldier->ubProfile, pSoldier->ubProfile ) != -1 )
					{
						// we hate 'em anyways
						continue;
					}

					if ( gMercProfiles[ pTeamSoldier->ubProfile ].bSex == FEMALE )
					{
						switch( gMercProfiles[ pTeamSoldier->ubProfile ].bSexist )
						{
							case SOMEWHAT_SEXIST:
								HandleMoraleEventForSoldier( pTeamSoldier, MORALE_MERC_MARRIED );
								break;
							case VERY_SEXIST:
								// handle TWICE!
								HandleMoraleEventForSoldier( pTeamSoldier, MORALE_MERC_MARRIED );
								HandleMoraleEventForSoldier( pTeamSoldier, MORALE_MERC_MARRIED );
								break;
							default:
								break;
						}
					}
					else
					{
						switch( gMercProfiles[ pTeamSoldier->ubProfile ].bSexist )
						{
							case GENTLEMAN:
								HandleMoraleEventForSoldier( pTeamSoldier, MORALE_MERC_MARRIED );
								break;
							default:
								break;
						}
					}

				}
			}
			break;
		 // added by SANDRO
		case MORALE_PSYCHO_UNABLE_TO_PSYCHO:
		case MORALE_PACIFIST_GAIN_NONCOMBAT: // added by SANDRO
		case MORALE_MALICIOUS_HIT: // added by SANDRO
			Assert( pSoldier );
			HandleMoraleEventForSoldier( pSoldier, bMoraleEvent );
			break;

			// added by Flugente
		case MORALE_FOOD:
		case MORALE_GOOD_FOOD:
		case MORALE_BAD_FOOD:
		case MORALE_LOATHSOME_FOOD:
			Assert( pSoldier );
			HandleMoraleEventForSoldier( pSoldier, bMoraleEvent );
			break;

		case MORALE_BUDDY_FIRED:
		case MORALE_BUDDY_FIRED_EARLY:
		case MORALE_BUDDY_FIRED_ON_BAD_TERMS:
		case MORALE_BAD_EQUIPMENT:
		case MORALE_OWED_MONEY:
		case MORALE_PLAYER_INACTIVE_DAYS:
		case MORALE_PREVENTED_MISBEHAVIOUR:
			Assert( pSoldier );
			HandleMoraleEventForSoldier( pSoldier, bMoraleEvent );
			break;

		default:
			// debug message
			ScreenMsg( MSG_FONT_RED, MSG_BETAVERSION, L"Invalid morale event type = %d.	AM/CC-1", bMoraleEvent );
			break;
	}


	// some morale events also impact the player's reputation with the mercs back home
	switch( bMoraleEvent )
	{
		case MORALE_HIGH_DEATHRATE:
			ModifyPlayerReputation(REPUTATION_HIGH_DEATHRATE);
			break;
		case MORALE_LOW_DEATHRATE:
			ModifyPlayerReputation(REPUTATION_LOW_DEATHRATE);
			break;
		case MORALE_POOR_MORALE:
			ModifyPlayerReputation(REPUTATION_POOR_MORALE);
			break;
		case MORALE_GREAT_MORALE:
			ModifyPlayerReputation(REPUTATION_GREAT_MORALE);
			break;
		case MORALE_BATTLE_WON:
			ModifyPlayerReputation(REPUTATION_BATTLE_WON);
			break;
		case MORALE_RAN_AWAY:
		case MORALE_HEARD_BATTLE_LOST:
			ModifyPlayerReputation(REPUTATION_BATTLE_LOST);
			break;
		case MORALE_TOWN_LIBERATED:
			ModifyPlayerReputation(REPUTATION_TOWN_WON);
			break;
		case MORALE_TOWN_LOST:
			ModifyPlayerReputation(REPUTATION_TOWN_LOST);
			break;
		case MORALE_TEAMMATE_DIED:
			// impact depends on that dude's level of experience
			ModifyPlayerReputation((UINT8) (pSoldier->stats.bExpLevel * REPUTATION_SOLDIER_DIED));
			break;
		case MORALE_MERC_CAPTURED:
			// impact depends on that dude's level of experience
			ModifyPlayerReputation((UINT8) (pSoldier->stats.bExpLevel * REPUTATION_SOLDIER_CAPTURED));
			break;
		case MORALE_KILLED_CIVILIAN:
			ModifyPlayerReputation(REPUTATION_KILLED_CIVILIAN);
			break;
		case MORALE_MONSTER_QUEEN_KILLED:
			ModifyPlayerReputation(REPUTATION_KILLED_MONSTER_QUEEN);
			break;
		case MORALE_DEIDRANNA_KILLED:
			ModifyPlayerReputation(REPUTATION_KILLED_DEIDRANNA);
			break;
		case MORALE_BAD_EQUIPMENT:
			ModifyPlayerReputation(REPUTATION_MERC_COMPLAIN_EQUIPMENT);
			break;
		case MORALE_OWED_MONEY:
			ModifyPlayerReputation(REPUTATION_MERC_OWED_MONEY);
			break;
		case MORALE_PLAYER_INACTIVE_DAYS:
			ModifyPlayerReputation(REPUTATION_PLAYER_IS_INACTIVE);
			break;

		default:
			// no reputation impact
			break;
	}
}


void HourlyMoraleUpdate( void )
{
	INT8									bMercID, bOtherID;
	INT8									bActualTeamOpinion;
	INT8									bTeamMoraleModChange, bTeamMoraleModDiff;
	INT8									bOpinion=-1;
	INT32									iTotalOpinions;
	INT8									bNumTeamMembers;
	INT8									bHighestTeamLeadership = 0;
	INT8									bLastTeamID;
	SOLDIERTYPE *					pSoldier;
	SOLDIERTYPE *					pOtherSoldier;
	MERCPROFILESTRUCT *		pProfile;
	BOOLEAN								fSameGroupOnly;
	static INT8						bStrategicMoraleUpdateCounter = 0;
	BOOLEAN								fFoundHated = FALSE;
	INT8									bHated;

	bMercID = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

	// loop through all mercs to calculate their morale
	for ( pSoldier = MercPtrs[ bMercID ]; bMercID <= bLastTeamID; ++bMercID,pSoldier++)
	{
		//if the merc is active, in Arulco, and conscious, not POW
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE &&
																!(pSoldier->bAssignment == IN_TRANSIT ||
																pSoldier->flags.fMercAsleep == TRUE ||
																pSoldier->bAssignment == ASSIGNMENT_DEAD ||
																pSoldier->bAssignment == ASSIGNMENT_POW) )
		{
			// calculate the guy's opinion of the people he is with
			pProfile = &(gMercProfiles[ pSoldier->ubProfile ]);

			// if we're moving
			if (pSoldier->ubGroupID != 0 && PlayerIDGroupInMotion( pSoldier->ubGroupID ))
			{
				// we only check our opinions of people in our squad
				fSameGroupOnly = TRUE;
			}
			else
			{
				fSameGroupOnly = FALSE;
			}
			fFoundHated = FALSE;

			// reset counts to calculate average opinion
			iTotalOpinions = 0;
			bNumTeamMembers = 0;

			// let people with high leadership affect their own morale
			bHighestTeamLeadership = EffectiveLeadership( pSoldier );

			// loop through all other mercs
			bOtherID = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pOtherSoldier = MercPtrs[ bOtherID ]; bOtherID <= bLastTeamID; bOtherID++,pOtherSoldier++)
			{
				// skip past ourselves and all inactive mercs
				if (bOtherID != bMercID && pOtherSoldier->bActive && pOtherSoldier->ubProfile != NO_PROFILE &&
					!(pOtherSoldier->bAssignment == IN_TRANSIT ||
						pOtherSoldier->flags.fMercAsleep == TRUE ||
						pOtherSoldier->bAssignment == ASSIGNMENT_DEAD ||
						pOtherSoldier->bAssignment == ASSIGNMENT_POW))
				{
					if (fSameGroupOnly)
					{
						// all we have to check is the group ID
						if (pSoldier->ubGroupID != pOtherSoldier->ubGroupID)
						{
							continue;
						}
					}
					else
					{
						// check to see if the location is the same
						if (pOtherSoldier->sSectorX != pSoldier->sSectorX ||
							pOtherSoldier->sSectorY != pSoldier->sSectorY ||
								pOtherSoldier->bSectorZ != pSoldier->bSectorZ)
						{
							continue;
						}

						// if the OTHER soldier is in motion then we don't do anything!
						if (pOtherSoldier->ubGroupID != 0 && PlayerIDGroupInMotion( pOtherSoldier->ubGroupID ))
						{
							continue;
						}
					}

					bOpinion = SoldierRelation( pSoldier, pOtherSoldier);
										
					if (bOpinion == HATED_OPINION)
					{
						bHated = WhichHated( pSoldier->ubProfile, pOtherSoldier->ubProfile );
						if ( bHated >= 5 )
						{
							// learn to hate which has become full-blown hatred, full strength
							fFoundHated = TRUE;
							break;
						}
						else
						{
							// scale according to how close to we are to snapping
							//KM : Divide by 0 error found.	Wrapped into an if statement.
							if( pProfile->bHatedTime[ bHated ] )
							{
								bOpinion = ((INT32) bOpinion) * (pProfile->bHatedTime[ bHated ] - pProfile->bHatedCount[ bHated ]) / pProfile->bHatedTime[ bHated ];
							}

							if ( pProfile->bHatedCount[ bHated ] <= pProfile->bHatedTime[ bHated ] / 2 )
							{
								// Augh, we're teamed with someone we hate!	We HATE this!!	Ignore everyone else!
								fFoundHated = TRUE;
								break;
							}
							// otherwise just mix this opinion in with everyone else...
						}
					}
					iTotalOpinions += bOpinion;
					++bNumTeamMembers;
					if ( EffectiveLeadership( pOtherSoldier ) > bHighestTeamLeadership)
					{
						bHighestTeamLeadership = EffectiveLeadership( pOtherSoldier );
					}
				}
			}

			if (fFoundHated)
			{
				// If teamed with someone we hated, team opinion is automatically minimum
				bActualTeamOpinion = HATED_OPINION;
			}
			else if (bNumTeamMembers > 0)
			{
				bActualTeamOpinion = (INT8) (iTotalOpinions / bNumTeamMembers);
				// give bonus/penalty for highest leadership value on team
				bActualTeamOpinion += (bHighestTeamLeadership - 50) / 10;
			}
			else // alone
			{
				bActualTeamOpinion = 0;
			}

			// reduce to a range of HATED through BUDDY
			if (bActualTeamOpinion > BUDDY_OPINION)
			{
				bActualTeamOpinion = BUDDY_OPINION;
			}
			else if (bActualTeamOpinion < HATED_OPINION)
			{
				bActualTeamOpinion = HATED_OPINION;
			}

			// shift morale from team by ~10%

			// this should range between -75 and +75
			bTeamMoraleModDiff = bActualTeamOpinion - pSoldier->aiData.bTeamMoraleMod;
			if (bTeamMoraleModDiff > 0)
			{
				bTeamMoraleModChange = 1 + bTeamMoraleModDiff / 10;
			}
			else if (bTeamMoraleModDiff < 0)
			{
				bTeamMoraleModChange = -1 + bTeamMoraleModDiff / 10;
			}
			else
			{
				bTeamMoraleModChange = 0;
			}
			// SANDRO - morale is going down faster if not fighting for malicious characters
			if ( gGameOptions.fNewTraitSystem && pProfile->bCharacterTrait == CHAR_TRAIT_MALICIOUS )
			{
				bTeamMoraleModChange += gMoraleSettings.bModifiers[MORALE_MODIFIER_MALICIOUS_HOURLY_DECAY];//-= 1;
			}
			pSoldier->aiData.bTeamMoraleMod += bTeamMoraleModChange;
			pSoldier->aiData.bTeamMoraleMod = __min( pSoldier->aiData.bTeamMoraleMod, gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
			pSoldier->aiData.bTeamMoraleMod = __max( pSoldier->aiData.bTeamMoraleMod, -gMoraleSettings.bModifiers[MORALE_MOD_MAX] );

			// New, December 3rd, 1998, by CJC --
			// If delayed strategic modifier exists then incorporate it in strategic mod
			if ( pSoldier->bDelayedStrategicMoraleMod )
			{
				pSoldier->aiData.bStrategicMoraleMod += pSoldier->bDelayedStrategicMoraleMod;
				pSoldier->bDelayedStrategicMoraleMod = 0;
				pSoldier->aiData.bStrategicMoraleMod = __min( pSoldier->aiData.bStrategicMoraleMod, gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
				pSoldier->aiData.bStrategicMoraleMod = __max( pSoldier->aiData.bStrategicMoraleMod, -gMoraleSettings.bModifiers[MORALE_MOD_MAX] );
			}

			// refresh the morale value for the soldier based on the recalculated team modifier
			RefreshSoldierMorale( pSoldier );
		}
	}

	++bStrategicMoraleUpdateCounter;

	if ( bStrategicMoraleUpdateCounter == gMoraleSettings.ubHoursBetweenStrategicDelay )
	{
		DecayStrategicMoraleModifiers();
		bStrategicMoraleUpdateCounter = 0;
	}
}

void HandleSnitchCheck( void )
{
	UINT16									bMercID, bOtherID;
	INT8									bOpinion = -1;
	UINT16									bLastTeamID;
	SOLDIERTYPE*							pSoldier;
	SOLDIERTYPE*							pOtherSoldier;
	MERCPROFILESTRUCT*						pProfile;
	BOOLEAN									fSameGroupOnly;
	// anv: save merc id and his negative morale event for snitches
	std::vector<SnitchEvent>				snitcheventvector;

	bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;

	// loop through all mercs to calculate their morale
	for ( pSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pSoldier++ )
	{
		//if the merc is active, in Arulco, not POW, not a vehicle
		if ( pSoldier && pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE &&
			!(pSoldier->bAssignment == IN_TRANSIT ||
			pSoldier->bAssignment == ASSIGNMENT_DEAD ||
			pSoldier->bAssignment == ASSIGNMENT_POW ||
			pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) )
		{
			// calculate the guy's opinion of the people he is with
			pProfile = &(gMercProfiles[pSoldier->ubProfile]);

			// if we're moving
			if ( pSoldier->ubGroupID != 0 && PlayerIDGroupInMotion( pSoldier->ubGroupID ) )
			{
				// we only check our opinions of people in our squad
				fSameGroupOnly = TRUE;
			}
			else
			{
				fSameGroupOnly = FALSE;
			}
			// loop through all other mercs
			bOtherID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
			for ( pOtherSoldier = MercPtrs[bOtherID]; bOtherID <= bLastTeamID; ++bOtherID, pOtherSoldier++ )
			{
				// skip past ourselves and all inactive mercs
				if ( bOtherID != bMercID && pOtherSoldier && pOtherSoldier->bActive && pOtherSoldier->ubProfile != NO_PROFILE &&
					!(pOtherSoldier->bAssignment == IN_TRANSIT ||
					pOtherSoldier->bAssignment == ASSIGNMENT_DEAD ||
					pOtherSoldier->bAssignment == ASSIGNMENT_POW ||
					pOtherSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) )
				{
					if ( fSameGroupOnly )
					{
						// all we have to check is the group ID
						if ( pSoldier->ubGroupID != pOtherSoldier->ubGroupID )
						{
							continue;
						}
					}
					else
					{
						// check to see if the location is the same
						if ( pOtherSoldier->sSectorX != pSoldier->sSectorX ||
							pOtherSoldier->sSectorY != pSoldier->sSectorY ||
							pOtherSoldier->bSectorZ != pSoldier->bSectorZ )
						{
							continue;
						}

						// if the OTHER soldier is in motion then we don't do anything!
						if ( pOtherSoldier->ubGroupID != 0 && PlayerIDGroupInMotion( pOtherSoldier->ubGroupID ) )
						{
							continue;
						}
					}
					bOpinion = SoldierRelation( pSoldier, pOtherSoldier );
					if ( bOpinion <= gSkillTraitValues.bSNTMercOpinionAboutMercTreshold )
					{
						RememberSnitchableEvent( pSoldier->ubProfile, pOtherSoldier->ubProfile, fSameGroupOnly, SNITCH_HATED_PERSON, snitcheventvector );
					}
				}
			}

			// check death rate vs. merc's tolerance once/day (ignores buddies!)
			if ( MercThinksDeathRateTooHigh( pSoldier->ubProfile ) )
			{
				// too high, inform
				RememberSnitchableEvent( pSoldier->ubProfile, NO_PROFILE, fSameGroupOnly, SNITCH_DEATH_RATE, snitcheventvector );
			}

			// check his morale vs. his morale tolerance once/day (ignores buddies!)
			if ( MercThinksHisMoraleIsTooLow( pSoldier ) )
			{
				// too low, inform
				RememberSnitchableEvent( pSoldier->ubProfile, NO_PROFILE, fSameGroupOnly, SNITCH_LOW_MORALE, snitcheventvector );
			}

			// check his opinion about player's reputation
			if ( MercThinksBadReputationTooHigh( pSoldier->ubProfile ) )
			{
				// too high, inform
				RememberSnitchableEvent( pSoldier->ubProfile, NO_PROFILE, fSameGroupOnly, SNITCH_REPUTATION, snitcheventvector );
			}

			// check his opinion about player activity
			if ( MercThinksPlayerIsInactiveTooLong( pSoldier->ubProfile ) )
			{
				// player inactive, inform
				RememberSnitchableEvent( pSoldier->ubProfile, NO_PROFILE, fSameGroupOnly, SNITCH_PROGRESS, snitcheventvector );
			}

			// check if player owes him money
			if ( MercIsOwedTooMuchMoney( pSoldier->ubProfile ) )
			{
				// player owes him money, inform
				RememberSnitchableEvent( pSoldier->ubProfile, NO_PROFILE, fSameGroupOnly, SNITCH_OWED_MONEY, snitcheventvector );
			}

			// check if contract is running out, and will not renew
			// but only if he hasn't another contract signed, so there's still a chance to renew if player reacts properly
			if ( ContractIsGoingToExpireSoon( pSoldier ) )
			{
				if ( (pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC) )
				{
					// Only do this if they don't want to renew.....
					if ( !(pSoldier->flags.fSignedAnotherContract) && !WillMercRenew( pSoldier, FALSE ) )
					{
						RememberSnitchableEvent( pSoldier->ubProfile, NO_PROFILE, fSameGroupOnly, SNITCH_GONNA_QUIT, snitcheventvector );
					}
				}
			}
		}
	}

	// anv: handle snitches informing player about reasons for morale drops
	HandleSnitchesReports( snitcheventvector );
}

void HandleSnitchesReports( std::vector<SnitchEvent>& aVec )
{
	UINT8 bSnitchID;
	SOLDIERTYPE *pSnitch;
	BOOLEAN fSleepingSnitch = FALSE;

	UINT16 size = aVec.size( );
	for ( UINT16 bCounter = 0; bCounter < size; ++bCounter )
	{
		SnitchEvent& event = aVec[bCounter];

		if ( event.ubEventType < NUM_SNITCH_EVENTS )
		{
			bSnitchID = event.ubSnitchID;
			pSnitch = FindSoldierByProfileID( bSnitchID, TRUE );

			if ( pSnitch == NULL )
				continue;

			if ( !(pSnitch->bActive) )
				continue;

			if ( pSnitch->flags.fMercAsleep )
				fSleepingSnitch = TRUE;

			// snitch introduction
			if ( fSleepingSnitch )
				TacticalCharacterDialogueWithSpecialEvent( pSnitch, 0, DIALOGUE_SPECIAL_EVENT_SLEEP, 0, 0 );

			SnitchTacticalCharacterDialogue( pSnitch, 0, SNITCH_INTRODUCTION, NO_PROFILE, NO_PROFILE );

			// process all reports by the same snitch in row
			for ( UINT16 bCounter2 = bCounter; bCounter2 < size; ++bCounter2 )
			{
				SnitchEvent& event2 = aVec[bCounter2];

				if ( event2.ubEventType < NUM_SNITCH_EVENTS && event2.ubSnitchID == bSnitchID )
				{
					// check if relevant mercs are well
					SOLDIERTYPE *pSoldier, *pOtherSoldier;
					pSoldier = FindSoldierByProfileID( event2.ubTargetProfile, TRUE );
					if ( pSoldier == NULL || !(pSoldier->bActive) )
						continue;

					if ( event2.ubEventType == SNITCH_HATED_PERSON )
					{
						pOtherSoldier = FindSoldierByProfileID( event2.ubSecondaryTargetProfile, TRUE );
						if ( pOtherSoldier == NULL || !(pOtherSoldier->bActive) )
							continue;
					}

					// say this info
					SnitchTacticalCharacterDialogue( pSnitch, event2.ubEventType, event2.ubEventType, event2.ubTargetProfile, event2.ubSecondaryTargetProfile );

					// clear info
					event2.ubEventType = NUM_SNITCH_EVENTS;

					// Flugente: dynamic opinions
					AddOpinionEvent( event2.ubTargetProfile, pSnitch->ubProfile, OPINIONEVENT_SNITCHSOLDMEOUT );
				}
			}

			if ( fSleepingSnitch )
				TacticalCharacterDialogueWithSpecialEvent( FindSoldierByProfileID( bSnitchID, TRUE ), 0, DIALOGUE_SPECIAL_EVENT_SLEEP, 1, 0 );

			// clear info
			event.ubEventType = NUM_SNITCH_EVENTS;
		}
	}
}

void RememberSnitchableEvent( UINT8 ubTargetProfile, UINT8 ubSecondaryTargetProfile, BOOLEAN fSameGroupOnly, UINT8 ubEventType, std::vector<SnitchEvent>& aVec )
{
	UINT16 bSnitchID;
	INT16 sSnitchingChance = 0;
	UINT8 ubSnitchProfile;
	SOLDIERTYPE * pSnitch;
	SOLDIERTYPE * pSoldier;
	SOLDIERTYPE * pOtherSoldier;

	pSoldier = FindSoldierByProfileID( ubTargetProfile, FALSE );
	pOtherSoldier = FindSoldierByProfileID( ubSecondaryTargetProfile, FALSE );

	// loop through all other mercs
	bSnitchID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	for ( pSnitch = MercPtrs[bSnitchID]; bSnitchID <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++bSnitchID, pSnitch++ )
	{
		ubSnitchProfile = pSnitch->ubProfile;
		// skip past ourselves and all inactive mercs
		if ( ProfileHasSkillTrait( ubSnitchProfile, SNITCH_NT ) &&
			ubSnitchProfile != ubTargetProfile && ubSnitchProfile != ubSecondaryTargetProfile
			&& pSnitch->bActive && ubSnitchProfile != NO_PROFILE &&
			!(pSnitch->bAssignment == IN_TRANSIT ||
			pSnitch->bAssignment == ASSIGNMENT_DEAD ||
			pSnitch->bAssignment == ASSIGNMENT_POW) )
		{
			if ( fSameGroupOnly )
			{
				// all we have to check is the group ID
				if ( pSoldier->ubGroupID != pSnitch->ubGroupID )
				{
					continue;
				}
			}
			else
			{
				// check to see if the location is the same
				if ( pSnitch->sSectorX != pSoldier->sSectorX ||
					pSnitch->sSectorY != pSoldier->sSectorY ||
					pSnitch->bSectorZ != pSoldier->bSectorZ )
				{
					continue;
				}

				// if snitch is in motion then we don't do anything!
				if ( pSnitch->ubGroupID != 0 && PlayerIDGroupInMotion( pSnitch->ubGroupID ) )
				{
					continue;
				}
			}
			// calculate chances of snitching

			// decreased if target doesn't like snitch, increased if snitch doesn't like him
			sSnitchingChance = gSkillTraitValues.ubSNTBaseChance;
			sSnitchingChance = (INT16)(sSnitchingChance +
				SoldierRelation( pSoldier, pSnitch )*gSkillTraitValues.fSNTMercOpinionAboutSnitchBonusModifier +
				SoldierRelation( pSnitch, pSoldier )*gSkillTraitValues.fSNTSnitchOpinionAboutMercBonusModifier +
				gMercProfiles[ubSnitchProfile].bLeadership*gSkillTraitValues.fSNTSnitchLeadershipBonusModifer);
			if ( pSoldier->bAssignment == pSnitch->bAssignment )
			{
				sSnitchingChance += gSkillTraitValues.bSNTSameAssignmentBonus;
			}
			if ( gMercProfiles[pSnitch->ubProfile].bCharacterTrait == CHAR_TRAIT_SOCIABLE )
			{
				sSnitchingChance += gSkillTraitValues.bSNTSociableMercBonus;
			}
			if ( gMercProfiles[pSnitch->ubProfile].bCharacterTrait == CHAR_TRAIT_LONER )
			{
				sSnitchingChance += gSkillTraitValues.bSNTLonerMercBonus;
			}
			if ( ubEventType == SNITCH_HATED_PERSON )
			{
				if ( gSkillTraitValues.bSNTMercOpinionAboutMercTreshold != HATED_OPINION )
				{
					sSnitchingChance = (INT16)(sSnitchingChance *
						(1.0 - (FLOAT)(SoldierRelation( pSoldier, pOtherSoldier ) - HATED_OPINION) /
						(FLOAT)(gSkillTraitValues.bSNTMercOpinionAboutMercTreshold - HATED_OPINION)));
				}
			}
			if ( gMercProfiles[pSnitch->ubProfile].bDisability == DEAF )
			{
				sSnitchingChance /= 2;
			}
			sSnitchingChance = max( 0, sSnitchingChance );
			if ( Random( 100 ) < (UINT8)sSnitchingChance )
			{
				//information gathered succesfully, remember event to report it later
				SnitchEvent event;

				event.ubEventType = ubEventType;
				event.ubSnitchID = ubSnitchProfile;
				event.ubTargetProfile = ubTargetProfile;
				event.ubSecondaryTargetProfile = ubSecondaryTargetProfile;

				aVec.push_back( event );
			}
		}
	}
}

void DailyMoraleUpdate(SOLDIERTYPE *pSoldier)
{
	if ( pSoldier->ubProfile == NO_PROFILE )
	{
		return;
	}

	// CJC: made per hour now
/*
	// decay the merc's strategic morale modifier
	if (pSoldier->aiData.bStrategicMoraleMod != 0)
	{
		// decay the modifier!
		DecayStrategicMorale( pSoldier );

		// refresh the morale value for the soldier based on the recalculated modifier
		RefreshSoldierMorale( pSoldier );
	}
*/

	// check death rate vs. merc's tolerance once/day (ignores buddies!)
	if ( MercThinksDeathRateTooHigh( pSoldier->ubProfile ) )
	{
		// too high, morale takes a hit
		HandleMoraleEvent( pSoldier, MORALE_HIGH_DEATHRATE, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
	}

	// check his morale vs. his morale tolerance once/day (ignores buddies!)
	if ( MercThinksHisMoraleIsTooLow( pSoldier ) )
	{
		// too low, morale sinks further (merc's in a funk and things aren't getting better)
		HandleMoraleEvent( pSoldier, MORALE_POOR_MORALE, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
	}
	else if ( pSoldier->aiData.bMorale >= 75 )
	{
		// very high morale, merc is cheerleading others
		HandleMoraleEvent( pSoldier, MORALE_GREAT_MORALE, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
	}

}

// Added by SANDRO
BOOLEAN IsShowOffNearBy( SOLDIERTYPE * pSoldier )
{
	UINT32					uiLoop;
	SOLDIERTYPE *		pTeammate;
	BOOLEAN				fOneException = FALSE;
	BOOLEAN				fYesHeIs = FALSE;

	if (!pSoldier)
	{
		return( FALSE );
	}
	if( !(pSoldier->bActive) || !(pSoldier->bInSector) )
	{
		return( FALSE );	
	}

	for ( uiLoop = gTacticalStatus.Team[ pSoldier->bTeam ].bFirstID; uiLoop < gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; uiLoop++)
	{
		pTeammate = MercPtrs[ uiLoop ];
		if ( pTeammate == NULL )
		{
			continue;
		}
		if ( pTeammate == pSoldier )
		{
			continue;
		}
		// Are we actually here?
		if ( !(pTeammate->bActive) || !(pTeammate->bInSector) || ( pTeammate->flags.uiStatusFlags & SOLDIER_VEHICLE ) || (pTeammate->bAssignment == VEHICLE ) )
		{
			// is nowhere around!
			continue;
		}
		// Are we from our team an dalive?
		if ( pTeammate->bTeam == pSoldier->bTeam && pTeammate->stats.bLife >= OKLIFE && 
			gMercProfiles[ pTeammate->ubProfile ].bCharacterTrait == CHAR_TRAIT_SHOWOFF && PythSpacesAway( pSoldier->sGridNo, pTeammate->sGridNo ) <= 15)
		{
			if ( (pSoldier->ubBodyType <= STOCKYMALE && pTeammate->ubBodyType <= STOCKYMALE) || 
				(pSoldier->ubBodyType == REGFEMALE && pTeammate->ubBodyType == REGFEMALE) ) 
			{
				// phlegmatic character can ignore one
				if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_PHLEGMATIC && !fOneException)
				{
					fOneException = TRUE;			
				}
				else
				{
					fYesHeIs = TRUE;
				}
			}
		}
	}

	return( fYesHeIs );
}

// Flugente: calculate A's opinion of B
INT8	SoldierRelation( SOLDIERTYPE* pSoldierA, SOLDIERTYPE* pSoldierB)
{
	INT8 bOpinion = 0;

	if ( !OKToCheckOpinion(pSoldierA->ubProfile) || !OKToCheckOpinion(pSoldierB->ubProfile) )
		return 0;

	MERCPROFILESTRUCT*	pProfile = &(gMercProfiles[ pSoldierA->ubProfile ]);
	
	bOpinion = pProfile->bMercOpinion[ pSoldierB->ubProfile ];

	// Flugente: evaluate appearance/refinement/hated nationalities
					
	// some people loathe ugly people and like beautiful people. It's a mean world.
	// we also handle sexism here
	switch ( gMercProfiles[ pSoldierB->ubProfile ].bAppearance )
	{
	case APPEARANCE_UGLY:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance * 2;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance * 4;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[ pSoldierB->ubProfile ].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism * 2;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism * 4;
			}
		}
		break;
	case APPEARANCE_HOMELY:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion -= gGameExternalOptions.sMoraleModAppearance * 2;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[ pSoldierB->ubProfile ].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion -= gGameExternalOptions.sMoraleModSexism * 2;
			}
		}
		break;
	case APPEARANCE_ATTRACTIVE:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance * 2;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[ pSoldierB->ubProfile ].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism * 2;
			}
		}
		break;
	case APPEARANCE_BABE:
		{
			if ( pProfile->bAppearanceCareLevel == CARELEVEL_SOME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance * 2;
			else if ( pProfile->bAppearanceCareLevel == CARELEVEL_EXTREME )
				bOpinion += gGameExternalOptions.sMoraleModAppearance * 4;

			// some people are sexists
			if ( pProfile->bSexist && gMercProfiles[ pSoldierB->ubProfile ].bSex != pProfile->bSex )
			{
				if ( pProfile->bSexist == SOMEWHAT_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism * 2;
				else if ( pProfile->bSexist == VERY_SEXIST )
					bOpinion += gGameExternalOptions.sMoraleModSexism * 4;
			}
		}
		break;
	}

	// some people care about how distuingished other people are. Malus if on different ends of the spectrum, a small bonus if on the same and its really important to the person
	// also give a malus if the other person is a slob or snob and we are average but care extremely (we don't like people who behave differently)
	// if we don't care, doesn't matter
	if ( pProfile->bRefinementCareLevel == CARELEVEL_NONE )
	{
		// nothing to do...
	}
	// if we care somewhat, malus on slob/snob
	else if ( pProfile->bRefinementCareLevel == CARELEVEL_SOME )
	{
		if ( pProfile->bRefinement * gMercProfiles[pSoldierB->ubProfile].bRefinement == 2 )
			bOpinion -= gGameExternalOptions.sMoraleModRefinement;
	}
	// if we care extremely, reward for similarity, malus otherwise
	else //if( pProfile->bRefinementCareLevel == CARELEVEL_EXTREME )
	{
		if ( pProfile->bRefinement * gMercProfiles[pSoldierB->ubProfile].bRefinement == 2 )
			bOpinion -= 2 * gGameExternalOptions.sMoraleModRefinement;
		else if ( pProfile->bRefinement * gMercProfiles[pSoldierB->ubProfile].bRefinement == 0 )
			bOpinion -= gGameExternalOptions.sMoraleModRefinement;
		else 
			bOpinion += gGameExternalOptions.sMoraleModRefinement;
	}
	
	// some people hate other nationalities (do not mix up with racism, which uses bRace)
	if ( pProfile->bHatedNationality > -1 && gMercProfiles[ pSoldierB->ubProfile ].bNationality == pProfile->bHatedNationality )
	{
		if ( pProfile->bHatedNationalityCareLevel == CARELEVEL_SOME )
			bOpinion -= gGameExternalOptions.sMoraleModHatedNationality;
		else if ( pProfile->bHatedNationalityCareLevel == CARELEVEL_EXTREME )
			bOpinion -= gGameExternalOptions.sMoraleModHatedNationality * 2;
	}

	// some people are racists
	if ( pProfile->bRacist && gMercProfiles[ pSoldierB->ubProfile ].bRace != pProfile->bRace )
	{
		if ( pProfile->bRacist == RACIST_SOME )
			bOpinion -= gGameExternalOptions.sMoraleModRacism;
		else if ( pProfile->bRacist == RACIST_VERY )
			bOpinion -= gGameExternalOptions.sMoraleModRacism * 2;
	}
										
	// Flugente: backgrounds
	if ( pSoldierA->GetBackgroundValue( BG_DISLIKEBG ) && pSoldierA->GetBackgroundValue( BG_DISLIKEBG ) == -pSoldierB->GetBackgroundValue( BG_DISLIKEBG ) )
	{
		bOpinion -= 2;
	}

	if ( pSoldierA->HasBackgroundFlag( BACKGROUND_XENOPHOBIC ) && pSoldierB->ubProfile != NO_PROFILE && gMercProfiles[pSoldierA->ubProfile].usBackground != gMercProfiles[pSoldierB->ubProfile].usBackground )
		bOpinion -= gGameExternalOptions.sMoraleModXenophobicBackGround;

	// Flugente: dynamic opinions
	if ( gGameExternalOptions.fDynamicOpinions )
	{
		for ( UINT8 opinionevent = OPINIONEVENT_FRIENDLYFIRE; opinionevent < OPINIONEVENT_MAX; ++opinionevent )
		{
			bOpinion += GetDynamicOpinion( pSoldierA->ubProfile, pSoldierB->ubProfile, opinionevent );
		}
	}

	// reasonable values
	bOpinion = min( BUDDY_OPINION, bOpinion );
	bOpinion = max( HATED_OPINION, bOpinion );
	
	return bOpinion;
}

// Flugente: dynamic opinions
void AddOpinionEvent( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent )
{
	if ( usProfileA == NO_PROFILE || usProfileA == NO_PROFILE )
		return;

	if ( usEvent >= OPINIONEVENT_MAX )
		return;

	// we add a flag signifiying that an event happened today
	switch ( usEvent )
	{
	case OPINIONEVENT_FRIENDLYFIRE:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_FRIENDLYFIRE;	break;
	case OPINIONEVENT_SNITCHSOLDMEOUT:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_SNITCHSOLDMEOUT;	break;
	case OPINIONEVENT_SNITCHINTERFERENCE:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_INTERFERENCE;	break;
	case OPINIONEVENT_FRIENDSWITHHATED:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_FRIENDSWITHHATED;	break;
	case OPINIONEVENT_CONTRACTEXTENSION:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_CONTRACTEXTENSION;	break;
	case OPINIONEVENT_ORDEREDRETREAT:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_ORDEREDRETREAT;	break;
	case OPINIONEVENT_CIVKILLER:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_CIVKILLER;	break;
	case OPINIONEVENT_SLOWSUSDOWN:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] |= OPINIONFLAG_STAGE1_SLOWSUSDOWN;	break;

	case OPINIONEVENT_NOSHARINGFOOD:				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_NOSHARINGFOOD;	break;
	case OPINIONEVENT_ANNOYINGDISABILITY:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_ANNOYINGDISABILITY;	break;
	case OPINIONEVENT_ADDICT:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_ADDICT;	break;
	case OPINIONEVENT_THIEF:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_THIEF;	break;
	case OPINIONEVENT_WORSTCOMMANDEREVER:			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_WORSTCOMMANDEREVER;	break;
	case OPINIONEVENT_RICHGUY:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_RICHGUY;	break;
	case OPINIONEVENT_BETTERGEAR:					gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_BETTERGEAR;	break;
	case OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS:	gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] |= OPINIONFLAG_STAGE1_YOUMOUNTEDAGUNONMYBREASTS;	break;

	case OPINIONEVENT_BANDAGED:						gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_BANDAGED;	break;

		// drinking flags are a bit different - drinking can apply the GOOD or BAD flag, and they cancel each other out.
		// GOOD on GOOD causes SUPER, which cannot be removed (and stops this for today)
		// BAD on BAD causes WORSE, which cannot be removed (and stops this for today)
	case OPINIONEVENT_DRINKBUDDIES_GOOD:
		// if we do not already have the SUPER flag...
		if ( !(gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & (OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER | OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE)) )
		{
			// if we already have GOOD, get SUPER
			if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD )
			{
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER;
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD;
			}
			// if we have BAD, cancel it out
			else if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD )
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD;
			// if we have nothing, get GOOD
			else
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD;
		}
		break;

	case OPINIONEVENT_DRINKBUDDIES_BAD:
		// if we do not already have the SUPER flag...
		if ( !(gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & (OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER | OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE)) )
		{			
			// if we already have BAD, get WORSE
			if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD )
			{
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE;
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD;
			}
			// if we have GOOD, cancel it out
			else if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD )
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] &= ~OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD;
			// if we have nothing, get BAD
			else
				gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] |= OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD;
		}
		break;
		
	default:		break;
	}
}

// get usProfileA's opinion of usProfileB concerning usEvent
INT8 GetDynamicOpinion( UINT8 usProfileA, UINT8 usProfileB, UINT8 usEvent )
{
	INT32 opinion = 0;

	if ( usProfileA == NO_PROFILE || usProfileA == NO_PROFILE )
		return opinion;

	if ( usEvent >= OPINIONEVENT_MAX )
		return opinion;

	// count how many relevant flags are set
	UINT8 numflags = 0;
	switch ( usEvent )
	{
	case OPINIONEVENT_FRIENDLYFIRE:
		if (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_FRIENDLYFIRE)	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_FRIENDLYFIRE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_FRIENDLYFIRE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_FRIENDLYFIRE )	++numflags;
		break;

	case OPINIONEVENT_SNITCHSOLDMEOUT:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_SNITCHSOLDMEOUT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_SNITCHSOLDMEOUT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_SNITCHSOLDMEOUT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_SNITCHSOLDMEOUT )	++numflags;
		break;

	case OPINIONEVENT_SNITCHINTERFERENCE:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_INTERFERENCE )	++numflags;
		if (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_INTERFERENCE)	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_INTERFERENCE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_INTERFERENCE )	++numflags;
		break;

	case OPINIONEVENT_FRIENDSWITHHATED:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_FRIENDSWITHHATED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_FRIENDSWITHHATED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_FRIENDSWITHHATED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_FRIENDSWITHHATED )	++numflags;
		break;

	case OPINIONEVENT_CONTRACTEXTENSION:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_CONTRACTEXTENSION )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_CONTRACTEXTENSION )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_CONTRACTEXTENSION )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_CONTRACTEXTENSION )	++numflags;
		break;

	case OPINIONEVENT_ORDEREDRETREAT:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_ORDEREDRETREAT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_ORDEREDRETREAT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_ORDEREDRETREAT )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_ORDEREDRETREAT )	++numflags;
		break;

	case OPINIONEVENT_CIVKILLER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_CIVKILLER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_CIVKILLER )	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_CIVKILLER)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_CIVKILLER)	++numflags;
		break;

	case OPINIONEVENT_SLOWSUSDOWN:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE1_SLOWSUSDOWN)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE2_SLOWSUSDOWN)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE3_SLOWSUSDOWN)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][0] & OPINIONFLAG_STAGE4_SLOWSUSDOWN)	++numflags;
		break;

	case OPINIONEVENT_NOSHARINGFOOD:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_NOSHARINGFOOD)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_NOSHARINGFOOD)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_NOSHARINGFOOD)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_NOSHARINGFOOD)	++numflags;
		break;

	case OPINIONEVENT_ANNOYINGDISABILITY:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_ANNOYINGDISABILITY)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_ANNOYINGDISABILITY)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_ANNOYINGDISABILITY)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_ANNOYINGDISABILITY)	++numflags;
		break;

	case OPINIONEVENT_ADDICT:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_ADDICT)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_ADDICT)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_ADDICT)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_ADDICT)	++numflags;
		break;

	case OPINIONEVENT_THIEF:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_THIEF)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_THIEF)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_THIEF)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_THIEF)	++numflags;
		break;

	case OPINIONEVENT_WORSTCOMMANDEREVER:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_WORSTCOMMANDEREVER)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_WORSTCOMMANDEREVER)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_WORSTCOMMANDEREVER)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_WORSTCOMMANDEREVER)	++numflags;
		break;

	case OPINIONEVENT_RICHGUY:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_RICHGUY)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_RICHGUY)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_RICHGUY)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_RICHGUY)	++numflags;
		break;

	case OPINIONEVENT_BETTERGEAR:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_BETTERGEAR)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_BETTERGEAR)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_BETTERGEAR)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_BETTERGEAR)	++numflags;
		break;

	case OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS:
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE1_YOUMOUNTEDAGUNONMYBREASTS)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE2_YOUMOUNTEDAGUNONMYBREASTS)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE3_YOUMOUNTEDAGUNONMYBREASTS)	++numflags;
		if  (gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][1] & OPINIONFLAG_STAGE4_YOUMOUNTEDAGUNONMYBREASTS)	++numflags;
		break;

	case OPINIONEVENT_BANDAGED:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_BANDAGED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_BANDAGED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_BANDAGED )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_BANDAGED )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_GOOD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_GOOD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_GOOD )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_SUPER:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_SUPER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_SUPER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_SUPER )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_SUPER )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_BAD:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_BAD )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_BAD )	++numflags;
		break;

	case OPINIONEVENT_DRINKBUDDIES_WORSE:
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE1_DRINKBUDDIES_WORSE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE2_DRINKBUDDIES_WORSE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE3_DRINKBUDDIES_WORSE )	++numflags;
		if ( gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][2] & OPINIONFLAG_STAGE4_DRINKBUDDIES_WORSE )	++numflags;
		break;

	default:
		break;
	}

	// event opinion is number of times this happened times opinion modifer
	opinion = numflags * gMoraleSettings.bDynamicOpinionModifiers[usEvent];

	// cut it down to INT8
	return (INT8)opinion;
}

// daily rollover of opinions
void HandleDynamicOpinions( )
{
	for ( UINT8 usProfile = 0; usProfile < NUM_PROFILES; ++usProfile )
	{
		// each profile has its opinions on everyone else renewed
		RolloverDynamicOpinions( usProfile );
	}

	SOLDIERTYPE*		pSoldier	= NULL;
	UINT16				bMercID		= gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID	= gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE &&
			!(pSoldier->bAssignment == IN_TRANSIT ||
			pSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			// or each profile, check wether everyone else is a friend of someone else we hate
			CheckForFriendsofHated( pSoldier );

			// get annoyed on mercs that receive a lot more money than we do
			HandleDynamicOpinionWageJealousy( pSoldier );
		}
	}
}

// a day has passed, 'age' opinions 
void RolloverDynamicOpinions( UINT8 usProfileA )
{
	for ( UINT8 usProfileB = 0; usProfileB < NUM_PROFILES; ++usProfileB )
	{
		for ( UINT8 i = 0; i < OPINION_FLAGMASKS_NUMBER; ++i )
		{
			// events that are at stage 4 are forgotten
			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i] &= ~OPINIONFLAG_STAGE4_ALL;

			// all other events move up one stage
			gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i] = gMercProfiles[usProfileA].usDynamicOpinionFlagmask[usProfileB][i] << 1;
		}
	}
}

// check wether other people are friends with someone else we hate. All persons must be in Arulco
void CheckForFriendsofHated( SOLDIERTYPE* pSoldier )
{
	INT8									bMercID, bOtherID, bThirdID;
	INT8									bOpinion = -1;
	INT8									bSecondOpinion = -1;
	INT8									bLastTeamID;
	SOLDIERTYPE*							pOtherSoldier;
	SOLDIERTYPE*							pThirdSoldier;

	bMercID = pSoldier->ubID;
	bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	
	// loop through all other mercs
	bOtherID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	for ( pOtherSoldier = MercPtrs[bOtherID]; bOtherID <= bLastTeamID; ++bOtherID, pOtherSoldier++ )
	{
		// skip past ourselves and all inactive mercs
		if ( bOtherID != bMercID && pOtherSoldier->bActive && pOtherSoldier->ubProfile != NO_PROFILE &&
			!(pOtherSoldier->bAssignment == IN_TRANSIT ||
			pOtherSoldier->bAssignment == ASSIGNMENT_DEAD ) )
		{
			bOpinion = SoldierRelation( pSoldier, pOtherSoldier );

			// we cannot simply check for HATED_OPINION here - this very feature makes opinions not so easy anymore. Simply check for a range
			if ( bOpinion < -20 )
			{
				// there is someone in our team that we hate. We dislike his friends somewhat, purely because they like our foe
				// loop through all other mercs
				bThirdID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
				for ( pThirdSoldier = MercPtrs[bThirdID]; bThirdID <= bLastTeamID; ++bThirdID, pThirdSoldier++ )
				{
					// skip past ourselves and all inactive mercs
					if ( bThirdID != bMercID && bThirdID != bOtherID && pThirdSoldier->bActive && pThirdSoldier->ubProfile != NO_PROFILE &&
						!(pThirdSoldier->bAssignment == IN_TRANSIT ||
						pThirdSoldier->bAssignment == ASSIGNMENT_DEAD) )
					{
						bSecondOpinion = SoldierRelation( pThirdSoldier, pOtherSoldier );

						if ( bSecondOpinion > 20 )
						{
							// this guy is friends with someone we hate! We dislike him a bit for that
							AddOpinionEvent( pSoldier->ubProfile, pThirdSoldier->ubProfile, OPINIONEVENT_FRIENDSWITHHATED );
						}
					}
				}
			}
		}
	}
}

void HandleDynamicOpinionOnContractExtension( UINT8 ubCode, UINT8 usProfile )
{
	if ( usProfile == NO_PROFILE )
		return;

	if ( ubCode == EXTENDED_CONTRACT_BY_1_DAY || ubCode == EXTENDED_CONTRACT_BY_1_WEEK || ubCode == EXTENDED_CONTRACT_BY_2_WEEKS )
	{
		INT16 id = GetSoldierIDFromMercID( usProfile );
		if ( id > -1 )
		{
			SOLDIERTYPE* pSoldierWhoGotPaid = MercPtrs[id];

			// only for AIM mercs
			if ( pSoldierWhoGotPaid->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
				return;

			// determine the remaining length of his contract BEFORE it go renewed
			INT32 oldcontract = pSoldierWhoGotPaid->iEndofContractTime;
			if ( ubCode == EXTENDED_CONTRACT_BY_1_DAY )
				oldcontract -= 1440;
			else if ( ubCode == EXTENDED_CONTRACT_BY_1_WEEK )
				oldcontract -= 7 * 1440;
			else if ( ubCode == EXTENDED_CONTRACT_BY_2_WEEKS )
				oldcontract -= 14 * 1440;

			// someones contract got extended. Other mercs who have less time on their contract will be annoyed by this, as they feel they shoul be paid first
			SOLDIERTYPE*		pSoldier = NULL;
			UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
			UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
			for ( pSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pSoldier++ )
			{
				if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE && pSoldier->ubProfile != usProfile &&
					!(pSoldier->bAssignment == IN_TRANSIT ||
					pSoldier->bAssignment == ASSIGNMENT_DEAD) )
				{
					// only for AIM mercs
					if ( pSoldier->ubProfile == NO_PROFILE || pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC )
						continue;

					if ( pSoldier->iEndofContractTime < oldcontract )
					{
						// this guy got paid at a point where we had less time than he did! Favouritism!
						AddOpinionEvent( pSoldier->ubProfile, usProfile, OPINIONEVENT_CONTRACTEXTENSION );
					}
				}
			}
		}
	}
}

void HandleDynamicOpinionCivKill( SOLDIERTYPE* pSoldier )
{
	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			if ( (gMercProfiles[pTeamSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) )
			{
				AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_CIVKILLER );
			}
		}
	}
}

void HandleDynamicOpinionSlowdown( SOLDIERTYPE* pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
		return;
		
	INT32 iPercentEncumbranceOffended = CalculateCarriedWeight( pSoldier );
	if ( iPercentEncumbranceOffended > 100 )
	{
		// everyone else on the same squad gets annoyed, if they aren't overencumbered themselves, or at least not as much as the offender
		SOLDIERTYPE*		pTeamSoldier = NULL;
		UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
		UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
		for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
		{
			if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile && pTeamSoldier->bAssignment == pSoldier->bAssignment &&
				!(pTeamSoldier->bAssignment == IN_TRANSIT ||
				pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
			{
				INT32 iPercentEncumbrance = CalculateCarriedWeight( pTeamSoldier );

				if ( iPercentEncumbrance < 100 || iPercentEncumbrance < 0.7 * iPercentEncumbranceOffended )
				{
					AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_SLOWSUSDOWN );
				}
			}
		}
	}
}

void HandleDynamicOpinionWageJealousy( SOLDIERTYPE* pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
		return;

	if ( !pSoldier->iTotalContractLength || (pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC && pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__MERC && pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT) )
		return;

	// determine our mean daily wage
	UINT32 meanwage = gMercProfiles[pSoldier->ubProfile].uiTotalCostToDate / pSoldier->iTotalContractLength;

	UINT8 explevel = pSoldier->stats.bExpLevel;

	// hu?
	if ( !explevel )
		return;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			if ( !pTeamSoldier->iTotalContractLength || (pTeamSoldier->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC && pTeamSoldier->ubWhatKindOfMercAmI != MERC_TYPE__MERC && pTeamSoldier->ubWhatKindOfMercAmI != MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT) )
				continue;

			// their wage
			UINT32 theirmeanwage = gMercProfiles[pTeamSoldier->ubProfile].uiTotalCostToDate / pTeamSoldier->iTotalContractLength;

			// adjust this for experience levels
			FLOAT explevelfactor = gGameExternalOptions.fDynamicWageFactor * pTeamSoldier->stats.bExpLevel / explevel;

			if ( theirmeanwage > explevelfactor * meanwage )
			{
				AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_RICHGUY );
			}
		}
	}
}

void HandleDynamicOpinionDisability( SOLDIERTYPE* pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
		return;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// everybody other merc in the same sector gets annoyed
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_ANNOYINGDISABILITY );
		}
	}
}

void HandleDynamicOpinionAddict( SOLDIERTYPE* pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
		return;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_ADDICT );
		}
	}
}

void HandleDynamicOpinionThief( SOLDIERTYPE* pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
		return;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// everybody other merc in the same sector gets annoyed
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_THIEF );
		}
	}
}

void HandleDynamicOpinionFoodSharing( SOLDIERTYPE* pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
		return;

	BOOLEAN fCheckFood = (pSoldier->bFoodLevel < FoodMoraleMods[FOOD_MERC_START_SHOW_HUNGER_SYMBOL].bThreshold);
	BOOLEAN fCheckDrink = (pSoldier->bDrinkLevel < FoodMoraleMods[FOOD_MERC_START_SHOW_HUNGER_SYMBOL].bThreshold);

	//  no hunger - no problem
	if ( !fCheckFood && !fCheckDrink )
		return;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// everybody other merc in the same sector gets annoyed
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			if ( HasFoodInInventory( pTeamSoldier, fCheckFood, fCheckDrink ) )
			{
				AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_NOSHARINGFOOD );
			}
		}
	}
}

void HandleDynamicOpinionGear( SOLDIERTYPE* pSoldier )
{
	// only in combat squads
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE || pSoldier->bAssignment >= ON_DUTY )
		return;

	UINT8 highestcoolness = HighestInventoryCoolness( pSoldier );

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// everybody other merc in the same sector gets annoyed
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ &&
			pTeamSoldier->bAssignment < ON_DUTY &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			if ( HighestInventoryCoolness( pTeamSoldier ) > highestcoolness + 2 )
			{
				AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_BETTERGEAR );
			}
		}
	}
}

void HandleDynamicOpinionBattleLosses()
{
	UINT32 badstuff = 0;

	for ( UINT16 i = 0; i < CAMPAIGNHISTORY_SD_CIV; ++i )
	{
		badstuff += 5 * gCurrentIncident.usKills[i];
		badstuff += gCurrentIncident.usWounds[i];
		badstuff += 4 * gCurrentIncident.usPrisoners[i];
	}

	if ( badstuff > 100 )
	{
		// this was a disaster (Ignoring of how high the enemies losses were to create drama :-) )! Someone needs to be blamed!
		SOLDIERTYPE*		pScapeGoat	 = NULL;
		FLOAT				bestdommanderrating = 0.0f;
		SOLDIERTYPE*		pTeamSoldier = NULL;
		UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
		UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
		for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
		{
			// everybody other merc in the same sector gets annoyed
			if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && 
				pTeamSoldier->sSectorX == SECTORX( gCurrentIncident.usSector ) && pTeamSoldier->sSectorY == SECTORY( gCurrentIncident.usSector ) && pTeamSoldier->bSectorZ == gCurrentIncident.usLevel &&
				pTeamSoldier->bAssignment < ON_DUTY &&
				!(pTeamSoldier->bAssignment == IN_TRANSIT ||
				pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
			{
				FLOAT commanderrating = pTeamSoldier->stats.bExpLevel + NUM_SKILL_TRAITS( pTeamSoldier, SQUADLEADER_NT );
				
				if ( commanderrating > bestdommanderrating )
				{
					bestdommanderrating = commanderrating;

					pScapeGoat = pTeamSoldier;
				}
			}
		}

		// we've found someone competent. Let's all blame him for this disaster!
		if ( bestdommanderrating > 0.0f )
		{
			bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
			for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
			{
				// everybody other merc in the same sector gets annoyed
				if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE &&
					pTeamSoldier->ubProfile != pScapeGoat->ubProfile &&
					pTeamSoldier->sSectorX == SECTORX( gCurrentIncident.usSector ) && pTeamSoldier->sSectorY == SECTORY( gCurrentIncident.usSector ) && pTeamSoldier->bSectorZ == gCurrentIncident.usLevel &&
					pTeamSoldier->bAssignment < ON_DUTY &&
					!(pTeamSoldier->bAssignment == IN_TRANSIT ||
					pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
				{
					AddOpinionEvent( pTeamSoldier->ubProfile, pScapeGoat->ubProfile, OPINIONEVENT_WORSTCOMMANDEREVER );
				}
			}
		}
	}
}

void HandleDynamicOpinionRetreat()
{	
	// this was a disaster (Ignoring of how high the enemies losses were to create drama :-) )! Someone needs to be blamed!
	SOLDIERTYPE*		pScapeGoat = NULL;
	FLOAT				bestdommanderrating = 0.0f;
	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// everybody other merc in the same sector gets annoyed
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE &&
			pTeamSoldier->sSectorX == gWorldSectorX && pTeamSoldier->sSectorY == gWorldSectorY && (pTeamSoldier->bSectorZ == gbWorldSectorZ) &&
			pTeamSoldier->bAssignment < ON_DUTY &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			FLOAT commanderrating = pTeamSoldier->stats.bExpLevel + NUM_SKILL_TRAITS( pTeamSoldier, SQUADLEADER_NT );

			if ( commanderrating > bestdommanderrating )
			{
				bestdommanderrating = commanderrating;

				pScapeGoat = pTeamSoldier;
			}
		}
	}

	// we've found someone competent. Let's all blame him for this disaster!
	if ( bestdommanderrating > 0.0f )
	{
		bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
		for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
		{
			// everybody other merc in the same sector gets annoyed
			if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE &&
				pTeamSoldier->ubProfile != pScapeGoat->ubProfile &&
				pTeamSoldier->sSectorX == gWorldSectorX && pTeamSoldier->sSectorY == gWorldSectorY && (pTeamSoldier->bSectorZ == gbWorldSectorZ) &&
				pTeamSoldier->bAssignment < ON_DUTY &&
				!(pTeamSoldier->bAssignment == IN_TRANSIT ||
				pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
			{
				AddOpinionEvent( pTeamSoldier->ubProfile, pScapeGoat->ubProfile, OPINIONEVENT_ORDEREDRETREAT );
			}
		}
	}
}

void HandleDynamicOpinionTeamDrinking( SOLDIERTYPE* pSoldier )
{
	// need to be drunk for this
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE || !MercUnderTheInfluence( pSoldier, DRUG_TYPE_ALCOHOL ) )
		return;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// everybody other merc in the same sector can get updated if they are drugged too
		if ( pTeamSoldier->bActive && pTeamSoldier->ubProfile != NO_PROFILE && pTeamSoldier->ubProfile != pSoldier->ubProfile &&
			pTeamSoldier->sSectorX == pSoldier->sSectorX && pTeamSoldier->sSectorY == pSoldier->sSectorY && pTeamSoldier->bSectorZ == pSoldier->bSectorZ &&
			MercUnderTheInfluence( pTeamSoldier, DRUG_TYPE_ALCOHOL ) &&
			!(pTeamSoldier->bAssignment == IN_TRANSIT ||
			pTeamSoldier->bAssignment == ASSIGNMENT_DEAD) )
		{
			// both mercs drink together, they opinion either improve or worsen
			if ( Chance( 67 ) )
				AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_GOOD );
			else
				AddOpinionEvent( pTeamSoldier->ubProfile, pSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_BAD );

			if ( Chance( 67 ) )
				AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_GOOD );
			else
				AddOpinionEvent( pSoldier->ubProfile, pTeamSoldier->ubProfile, OPINIONEVENT_DRINKBUDDIES_BAD );
		}
	}
}

UINT8 HighestInventoryCoolness( SOLDIERTYPE* pSoldier )
{
	UINT8 coolness = 0;

	if ( !pSoldier )
		return coolness;

	// search for food in our inventory
	INT8 invsize = (INT8)pSoldier->inv.size( );									// remember inventorysize, so we don't call size() repeatedly
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )							// ... for all items in our inventory ...
	{
		// ... if Item exists and is food ...
		if ( pSoldier->inv[bLoop].exists( ) )
		{
			coolness = max( coolness, Item[pSoldier->inv[bLoop].usItem].ubCoolness );
		}
	}

	return coolness;
}