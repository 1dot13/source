#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "SkillCheck.h"
	#include "Soldier Profile.h"
	#include "Random.h"
	#include "Items.h"
	#include "Dialogue Control.h"
	#include "Overhead.h"
	#include "Soldier macros.h"
	#include "Isometric Utils.h"
	#include "Morale.h"
	#include "drugs and alcohol.h"
	#include "strategicmap.h"
	// added by SANDRO
	#include "GameSettings.h"
	#include "Animation Data.h"
	#include "Soldier Control.h"
#endif

INT8 EffectiveStrength( SOLDIERTYPE * pSoldier )
{
	INT8	bBandaged;
	INT32	iEffStrength;

	// Effective strength is:
	// 1/2 full strength
	// plus 1/2 strength scaled according to how hurt we are
	bBandaged = pSoldier->stats.bLifeMax - pSoldier->stats.bLife - pSoldier->bBleeding;

	if (pSoldier->stats.bStrength > 0)
	{
		iEffStrength = pSoldier->stats.bStrength / 2;
		iEffStrength += (pSoldier->stats.bStrength / 2) * (pSoldier->stats.bLife + bBandaged / 2) / (pSoldier->stats.bLifeMax);
	}
	else
	{
		iEffStrength = 0;
	}

	// ATE: Make sure at least 2...
	iEffStrength = __max( iEffStrength, 2 );

	return( (INT8) iEffStrength );
}


INT8 EffectiveWisdom( SOLDIERTYPE * pSoldier )
{
	INT32	iEffWisdom;

	iEffWisdom = pSoldier->stats.bWisdom;

	iEffWisdom = EffectStatForBeingDrunk( pSoldier, iEffWisdom );

	return( (INT8) iEffWisdom );
}

INT8 EffectiveAgility( SOLDIERTYPE * pSoldier )
{
	INT32	iEffAgility;

	iEffAgility = pSoldier->stats.bAgility;

	iEffAgility = EffectStatForBeingDrunk( pSoldier, iEffAgility );

	if ( pSoldier->sWeightCarriedAtTurnStart > 100 )
	{
		iEffAgility = (iEffAgility * 100) / pSoldier->sWeightCarriedAtTurnStart;
	}

	return( (INT8) iEffAgility );
}


INT8 EffectiveMechanical( SOLDIERTYPE * pSoldier )
{
	INT32	iEffMechanical;

	iEffMechanical = pSoldier->stats.bMechanical;

	iEffMechanical = EffectStatForBeingDrunk( pSoldier, iEffMechanical );

	return( (INT8) iEffMechanical );
}


INT8 EffectiveExplosive( SOLDIERTYPE * pSoldier )
{
	INT32	iEffExplosive;

	iEffExplosive = pSoldier->stats.bExplosive;

	iEffExplosive = EffectStatForBeingDrunk( pSoldier, iEffExplosive );

	return( (INT8) iEffExplosive );
}


INT8 EffectiveMedical( SOLDIERTYPE * pSoldier )
{
	INT32	iEffMedical;

	iEffMedical = pSoldier->stats.bMedical;

	iEffMedical = EffectStatForBeingDrunk( pSoldier, iEffMedical );

	return( (INT8) iEffMedical );
}

INT8 EffectiveLeadership( SOLDIERTYPE * pSoldier )
{
	INT32	iEffLeadership;
	INT8	bDrunkLevel;

	iEffLeadership = pSoldier->stats.bLeadership;

	// if we are drunk, effect leader ship in a +ve way...
	bDrunkLevel = GetDrunkLevel( pSoldier );

	if ( bDrunkLevel == FEELING_GOOD )
	{
		iEffLeadership = ( iEffLeadership * 120 / 100 );
	}

	return( (INT8) iEffLeadership );
}

INT8 EffectiveExpLevel( SOLDIERTYPE * pSoldier )
{
	INT32	iEffExpLevel;
	INT8	bDrunkLevel;
	INT32	iExpModifier[] =
		{ 0,	// SOBER
			0,	// Feeling good
		-1,	// Borderline
		-2,	// Drunk
		0,		// Hung
		};

	iEffExpLevel = pSoldier->stats.bExpLevel;

	// SANDRO - STOMP traits - Squadleader bonus to effective level
	if ( gGameOptions.fNewTraitSystem && IS_MERC_BODY_TYPE( pSoldier ))
	{
		iEffExpLevel += (gSkillTraitValues.ubSLEffectiveLevelInRadius * GetSquadleadersCountInVicinity( pSoldier, TRUE, FALSE ));
	}

	bDrunkLevel = GetDrunkLevel( pSoldier );

	iEffExpLevel = iEffExpLevel + iExpModifier[ bDrunkLevel ];

	if (pSoldier->ubProfile != NO_PROFILE)
	{
		if ( (gMercProfiles[ pSoldier->ubProfile ].bDisability == CLAUSTROPHOBIC) && pSoldier->bActive && pSoldier->bInSector && gbWorldSectorZ > 0)
		{
			// claustrophobic!
			iEffExpLevel -= 2;
		}
		else if ( (gMercProfiles[ pSoldier->ubProfile ].bDisability == FEAR_OF_INSECTS) && MercIsInTropicalSector( pSoldier ) )
		{
			// SANDRO - fear of insects, and we are in tropical sector
			iEffExpLevel -= 1;
		}
	}
	
	if (iEffExpLevel > 10)
	{
		// can't go over 10 - SANDRO
		return( 10 );
	}
	else if (iEffExpLevel < 1)
	{
		// can't go below 1
		return( 1 );
	}
	else
	{
		return( (INT8) iEffExpLevel );
	}
}

INT8 EffectiveMarksmanship( SOLDIERTYPE * pSoldier )
{
	INT32	iEffMarksmanship;

	iEffMarksmanship = pSoldier->stats.bMarksmanship;

	iEffMarksmanship = EffectStatForBeingDrunk( pSoldier, iEffMarksmanship );

	return( (INT8) iEffMarksmanship );
}

INT8 EffectiveDexterity( SOLDIERTYPE * pSoldier )
{
	INT32	iEffDexterity;

	iEffDexterity = pSoldier->stats.bDexterity;

	iEffDexterity = EffectStatForBeingDrunk( pSoldier, iEffDexterity );

	return( (INT8) iEffDexterity );
}



UINT8 GetPenaltyForFatigue( SOLDIERTYPE *pSoldier )
{
	UINT8 ubPercentPenalty;

	if			( pSoldier->bBreathMax >= 85 )	ubPercentPenalty =	0;
	else if ( pSoldier->bBreathMax >= 70 )	ubPercentPenalty =	10;
	else if ( pSoldier->bBreathMax >= 50 )	ubPercentPenalty =	25;
	else if ( pSoldier->bBreathMax >= 30 )	ubPercentPenalty =	50;
	else if ( pSoldier->bBreathMax >= 15 )	ubPercentPenalty =	75;
	else if ( pSoldier->bBreathMax >	0 )	ubPercentPenalty =	90;
	else																		ubPercentPenalty = 100;

	return( ubPercentPenalty );
}

void ReducePointsForFatigue( SOLDIERTYPE *pSoldier, UINT16 *pusPoints )
{
	*pusPoints -= (*pusPoints * GetPenaltyForFatigue( pSoldier )) / 100;
}

INT32 GetSkillCheckPenaltyForFatigue( SOLDIERTYPE *pSoldier, INT32 iSkill )
{
	// use only half the full effect of fatigue for skill checks
	return( ( (iSkill * GetPenaltyForFatigue( pSoldier ) ) / 100) / 2 );
}

INT32 SkillCheck( SOLDIERTYPE * pSoldier, INT8 bReason, INT8 bChanceMod )
{
	INT32	iSkill;
	INT32	iChance, iReportChance;
	INT32	iRoll, iMadeItBy;
	INT8	bSlot;
	INT32	iLoop;
	SOLDIERTYPE * pTeamSoldier;
	INT8	bBuddyIndex;
	BOOLEAN fForceDamnSound = FALSE;

	iReportChance = -1;

	switch (bReason)
	{
		case LOCKPICKING_CHECK:
		case ELECTRONIC_LOCKPICKING_CHECK:

		fForceDamnSound = TRUE;

			iSkill = EffectiveMechanical( pSoldier );
			if (iSkill == 0)
			{
				break;
			}
			// adjust skill based on wisdom (knowledge)
			iSkill = iSkill * (EffectiveWisdom( pSoldier ) + 100) / 200;
			// and dexterity (clumsy?)
			iSkill = iSkill * (EffectiveDexterity( pSoldier ) + 100) / 200;
			// factor in experience
			iSkill = iSkill + EffectiveExpLevel( pSoldier ) * 3;

			// SANDRO - STOMP traits - Technician trait lockpicking bonus
			if ( gGameOptions.fNewTraitSystem )
			{
				iSkill += gSkillTraitValues.bCheckModifierLockpicking; // reduce skill value for untrained mercs

				// Simply reduce the skill value for this as usual
				if (bReason == ELECTRONIC_LOCKPICKING_CHECK)
				{
					iSkill /= 2;
				}
				// then add a flat bonus (undivided by the electronic lock penalty above)
				if ( HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ))
				{
					iSkill += gSkillTraitValues.usTELockpickingBonus * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT );
				}
			}
			else // original code
			{
				if (HAS_SKILL_TRAIT( pSoldier, LOCKPICKING_OT ) )
				{
					// if we specialize in picking locks...
					iSkill += gbSkillTraitBonus[LOCKPICKING_OT] * NUM_SKILL_TRAITS( pSoldier, LOCKPICKING_OT );
				}

				if (bReason == ELECTRONIC_LOCKPICKING_CHECK && !(HAS_SKILL_TRAIT( pSoldier, ELECTRONICS_OT)) )
				{
					// if we are unfamiliar with electronics...
					iSkill /= 2;
				}

			}
			// adjust chance based on status of kit
			bSlot = FindLocksmithKit( pSoldier );
			if (bSlot == NO_SLOT)
			{
				// this should never happen, but might as well check...
				iSkill = 0;
			}
			iSkill = iSkill * pSoldier->inv[bSlot][0]->data.objectStatus / 100;
			break;
		case ATTACHING_DETONATOR_CHECK:
		case ATTACHING_REMOTE_DETONATOR_CHECK:
			iSkill = EffectiveExplosive( pSoldier );
			if (iSkill == 0)
			{
				break;
			}
			iSkill = (iSkill * 3 + EffectiveDexterity( pSoldier ) ) / 4;

			// SANDRO - STOMP traits - Demolitions trait detonator attaching bonus
			if ( gGameOptions.fNewTraitSystem )
			{
				iSkill += gSkillTraitValues.bCheckModifierAttachDetonators; // reduce skill value for untrained mercs

				if ( bReason == ATTACHING_REMOTE_DETONATOR_CHECK )
				{
					iSkill /= 2;
				}
				// add a flat bonus instead of eliminating the penalty above
				if ( HAS_SKILL_TRAIT( pSoldier, DEMOLITIONS_NT ))
				{
					iSkill += gSkillTraitValues.ubDEAttachDetonatorCheckBonus;
				}
			}
			else // original code
			{
				if ( bReason == ATTACHING_REMOTE_DETONATOR_CHECK && !(HAS_SKILL_TRAIT( pSoldier, ELECTRONICS_OT )) )
				{
					iSkill /= 2;
				}
			}
			break;
		case PLANTING_BOMB_CHECK:
		case PLANTING_REMOTE_BOMB_CHECK:
			iSkill = EffectiveExplosive( pSoldier ) * 7;
			iSkill += EffectiveWisdom( pSoldier ) * 2;
			iSkill += EffectiveExpLevel( pSoldier ) * 10;
			iSkill = iSkill / 10; // bring the value down to a percentage

			// SANDRO - STOMP traits - Demolitions trait planting bombs bonus
			if ( gGameOptions.fNewTraitSystem )
			{
				iSkill += gSkillTraitValues.bCheckModifierSetExplosives; // reduce skill for untrained mercs

				if ( bReason == PLANTING_REMOTE_BOMB_CHECK )
				{
					iSkill = (iSkill * 3) / 4;
				}
				// add a flat bonus instead of eliminating the penalty above
				if ( HAS_SKILL_TRAIT( pSoldier, DEMOLITIONS_NT ))
				{
					iSkill += gSkillTraitValues.ubDEPlantAndRemoveBombCheckBonus;
				}
			}
			else // original code
			{
				if ( bReason == PLANTING_REMOTE_BOMB_CHECK && !(HAS_SKILL_TRAIT( pSoldier, ELECTRONICS_OT)) )
				{
					// deduct only a bit...
					iSkill = (iSkill * 3) / 4;
				}
			}

			// Ok, this is really damn easy, so skew the values...
			// e.g. if calculated skill is 84, skewed up to 96
			// 51 to 84
			// 22 stays as is
			iSkill = (iSkill + 100 * (iSkill / 25) ) / (iSkill / 25 + 1);
			break;

		case DISARM_TRAP_CHECK:

			fForceDamnSound = TRUE;

			// SANDRO - normal(exploding) traps should be based at least a little on mechanical skill as well
			if ( gGameOptions.fNewTraitSystem)
			{ 
				// It is impossible without both skills
				if ( (EffectiveMechanical( pSoldier ) <= 0) || (EffectiveExplosive( pSoldier ) <= 0) )
				{
					iSkill = 0;
					break;
				}
				else
				{
					iSkill = EffectiveExplosive( pSoldier ) * 5;
					iSkill += EffectiveMechanical( pSoldier ) * 2;
				}
			}
			else
			{
				iSkill = EffectiveExplosive( pSoldier ) * 7;
				if ( iSkill == 0 )
				{
					break;
				}
			}

			iSkill += EffectiveDexterity( pSoldier ) * 2;
			iSkill += EffectiveExpLevel( pSoldier ) * 10;
			iSkill = iSkill / 10; // bring the value down to a percentage

			// SANDRO - STOMP traits - Demolitions trait removing traps bonus
			if ( gGameOptions.fNewTraitSystem )
			{
				iSkill += gSkillTraitValues.bCheckModifierDisarmExplosiveTraps; // -15% for untrained mercs

				if (HAS_SKILL_TRAIT( pSoldier, DEMOLITIONS_NT ))
					iSkill += gSkillTraitValues.ubDEPlantAndRemoveBombCheckBonus; // +50% bonus for Demolitions
			}

			// penalty based on poor wisdom
			iSkill -= (100 - EffectiveWisdom( pSoldier ) ) / 5;
			break;

		case DISARM_ELECTRONIC_TRAP_CHECK:

			fForceDamnSound = TRUE;

			// SANDRO - electronic traps are based more on mechanical skill
			if ( gGameOptions.fNewTraitSystem)
			{ 
				// It is impossible without both skills
				if ( (EffectiveMechanical( pSoldier ) <= 0) || (EffectiveExplosive( pSoldier ) <= 0) )
				{
					iSkill = 0;
					break;
				}
				else
				{
					iSkill = EffectiveExplosive( pSoldier ) * 2;
					iSkill += EffectiveMechanical( pSoldier ) * 5;
				}
			}
			else
			{
				iSkill = __max( EffectiveMechanical( pSoldier ) , EffectiveExplosive( pSoldier ) ) * 7;
				if ( iSkill == 0 )
				{
					break;
				}
			}
			iSkill += EffectiveDexterity( pSoldier ) * 2;
			iSkill += EffectiveExpLevel( pSoldier ) * 10;
			iSkill = iSkill / 10; // bring the value down to a percentage
			// penalty based on poor wisdom
			iSkill -= (100 - EffectiveWisdom( pSoldier ) ) / 5;

			// SANDRO - STOMP traits - Technician trait removing traps bonus
			if ( gGameOptions.fNewTraitSystem )
			{
				iSkill += gSkillTraitValues.bCheckModifierDisarmElectronicTraps; // penalty for untrained mercs

				iSkill = (iSkill * 3) / 4; // original penalty

				// add a flat bonus instead of eliminating the penalty above
				if ( HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ))
				{
					iSkill += gSkillTraitValues.usTEDisarmElTrapBonus * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT );
				}
			}
			else // original code
			{
				if ( !(HAS_SKILL_TRAIT( pSoldier, ELECTRONICS_OT)) )
				{
					iSkill = (iSkill * 3) / 4;
				}
			}
			break;

		case OPEN_WITH_CROWBAR:
			// Add for crowbar...
			iSkill = EffectiveStrength( pSoldier ) + 20;
		fForceDamnSound = TRUE;
			break;

		case SMASH_DOOR_CHECK:
			iSkill = EffectiveStrength( pSoldier );
			// SANDRO - STOMP traits - Martial Arts trait kick doors bonus
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, MARTIAL_ARTS_NT ))
			{
				iSkill += gSkillTraitValues.ubMAChanceToCkickDoors * NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT );
			}
			break;
		case UNJAM_GUN_CHECK:
			iSkill = 30 + EffectiveMechanical( pSoldier ) / 2;
			// SANDRO - STOMP traits - Technician trait unjamming guns bonus
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ))
			{
				iSkill += gSkillTraitValues.ubTEUnjamGunBonus * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT );
			}
			break;
		case NOTICE_DART_CHECK:
			// only a max of ~20% chance
			iSkill = EffectiveWisdom( pSoldier ) / 10 + EffectiveExpLevel( pSoldier );
			break;
		case LIE_TO_QUEEN_CHECK:
			// competitive check vs the queen's wisdom and leadership... poor guy!
			iSkill = 50 * ( EffectiveWisdom( pSoldier ) + EffectiveLeadership( pSoldier ) ) / ( gMercProfiles[ QUEEN ].bWisdom + gMercProfiles[ QUEEN ].bLeadership );
			// squadleader has bonus for this, muhehe - SANDRO
			if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, SQUADLEADER_NT ))
			{
				iSkill += (20 * NUM_SKILL_TRAITS( pSoldier, SQUADLEADER_NT ));
			}

			break;
		case ATTACHING_SPECIAL_ITEM_CHECK:
		case ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK:
			iSkill = EffectiveMechanical( pSoldier );
			if (iSkill == 0)
			{
				break;
			}
			// adjust skill based on wisdom (knowledge)
			iSkill = iSkill * (EffectiveWisdom( pSoldier ) + 100) / 200;
			// and dexterity (clumsy?)
			iSkill = iSkill * (EffectiveDexterity( pSoldier ) + 100) / 200;
			// factor in experience
			iSkill = iSkill + EffectiveExpLevel( pSoldier ) * 3;

			// SANDRO - STOMP traits - Technician trait attaching special items bonus
			if ( gGameOptions.fNewTraitSystem )
			{
				iSkill += gSkillTraitValues.bCheckModifierAttachSpecialItems; // penalty for untrained mercs

				if ( bReason == ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK )
				{
					iSkill /= 2;
				}
				// add a flat bonus instead of eliminating the penalty above
				if ( HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ))
				{
					iSkill += gSkillTraitValues.usTEAttachingItemsBonus * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT );
				}
			}
			else // original code
			{
				if (bReason == ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK && !(HAS_SKILL_TRAIT( pSoldier, ELECTRONICS_OT)) )
				{
					// if we are unfamiliar with electronics...
					iSkill /= 2;
				}
			}
			break;
		default:
			iSkill = 0;
			break;
	}

	iSkill -= GetSkillCheckPenaltyForFatigue( pSoldier, iSkill );

	iChance = iSkill + bChanceMod;

	////////////////////////////////////////////////////////////////////////
	// SANDRO - STOMP - character traits and disabilities modifiers

	//added heat intolerant penalty
	if ( MercIsHot( pSoldier ) )
	{
		iChance -= 15;
	}
	// also added a small penalty for fear of insects in tropical sectors
	else if ( (gMercProfiles[ pSoldier->ubProfile ].bDisability == FEAR_OF_INSECTS) && MercIsInTropicalSector( pSoldier ))
	{
		// fear of insects, and we are in tropical sector
		iChance -= 5;
	}

	// character traits influence
	// Sociable - better performance in groups
	if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_SOCIABLE )
	{	
		INT8 bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_SOCIABLE );
		if ( bNumMercs > 2 )
			iChance += 5;
		else if ( bNumMercs > 0 )
			iChance += 2;
	}
	// Loner - better performance when alone
	else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_LONER )
	{	
		INT8 bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_LONER );
		if ( bNumMercs == 0 )
			iChance += 5;
		else if ( bNumMercs <= 1 )
			iChance += 2;
	}
	// Aggressive - penalty for actions needing patience
	else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_AGGRESSIVE )
	{	
		switch ( bReason )
		{
			case LOCKPICKING_CHECK:
			case ELECTRONIC_LOCKPICKING_CHECK:
			case PLANTING_BOMB_CHECK:
			case PLANTING_REMOTE_BOMB_CHECK:
			case DISARM_TRAP_CHECK:
			case DISARM_ELECTRONIC_TRAP_CHECK:
			case ATTACHING_SPECIAL_ITEM_CHECK:
			case ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK:
				iChance -= 10;
				break;
		}
	}
	// Phlegmatic - bonus for actions needing patience
	else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_PHLEGMATIC )
	{	
		switch ( bReason )
		{
			case LOCKPICKING_CHECK:
			case ELECTRONIC_LOCKPICKING_CHECK:
			case PLANTING_BOMB_CHECK:
			case PLANTING_REMOTE_BOMB_CHECK:
			case DISARM_TRAP_CHECK:
			case DISARM_ELECTRONIC_TRAP_CHECK:
			case ATTACHING_SPECIAL_ITEM_CHECK:
			case ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK:
				iChance += 5;
				break;
		}
	}
	// Show-off - better performance if some babes around to impress
	else if ( gMercProfiles[ pSoldier->ubProfile ].bCharacterTrait == CHAR_TRAIT_SHOWOFF )
	{	
		INT8 bNumMercs = CheckMercsNearForCharTraits( pSoldier->ubProfile, CHAR_TRAIT_SHOWOFF );
		if ( bNumMercs > 1 )
			iChance += 5;
		else if ( bNumMercs > 0 )
			iChance += 2;
	}
	//////////////////////////////////////////////////////////////////////

	// silversurfer: moved this down here where it belongs
	// if this wasn't the last check a merc with certain disability modifiers would never completely fail 
	// to do something and would never do his speech telling that he can't do this
	switch (bReason)
	{
		case LOCKPICKING_CHECK:
		case ELECTRONIC_LOCKPICKING_CHECK:
		case DISARM_TRAP_CHECK:
		case DISARM_ELECTRONIC_TRAP_CHECK:
		case OPEN_WITH_CROWBAR:
		case SMASH_DOOR_CHECK:
		case ATTACHING_SPECIAL_ITEM_CHECK:
		case ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK:
			// for lockpicking and smashing locks, if the chance isn't reasonable
			// we set it to 0 so they can never get through the door if they aren't
			// good enough
			//CHRISL: with this set to 30, some "hard" to open doors become basically impossible.  Case in point, Drassen "dentist office" which imposes a
			//	-75 penalty on lockpicking/forcing chances.  You find this near the start of the game but would need an 80 STR + Crowbar to open.  It's
			//	impossible to force with just strength or lockpicking skills, regardless of how skilled you are.
			if (iChance < 15)
			{
				iChance = 0;
				break;
			}
			// else fall through
		default:
			iChance += GetMoraleModifier( pSoldier );
			break;
	}

	if (iChance > 99)
	{
		iChance = 99;
	}
	else if (iChance < 0)
	{
		iChance = 0;
	}

	iRoll = PreRandom( 100 );
	iMadeItBy = iChance - iRoll;
	if (iMadeItBy < 0)
	{
		if ( (pSoldier->bLastSkillCheck == bReason) && (pSoldier->sGridNo == pSoldier->sSkillCheckGridNo) )
		{
			pSoldier->ubSkillCheckAttempts++;
			if (pSoldier->ubSkillCheckAttempts > 2)
			{
				if (iChance == 0)
				{
					if ( MercIsHot( pSoldier ) && pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__PLAYER_CHARACTER) // SANDRO - added this
					{
						TacticalCharacterDialogue( pSoldier, QUOTE_PERSONALITY_TRAIT );
					}
					// do we realize that we just can't do this?
					if ( (100 - (pSoldier->ubSkillCheckAttempts - 2) * 20) < EffectiveWisdom( pSoldier ) )
					{
						// say "I can't do this" quote
						TacticalCharacterDialogue( pSoldier, QUOTE_DEFINITE_CANT_DO );
						return( iMadeItBy );
					}
				}
			}
		}
		else
		{
			pSoldier->bLastSkillCheck = bReason;
			pSoldier->ubSkillCheckAttempts = 1;
			pSoldier->sSkillCheckGridNo = pSoldier->sGridNo;
		}

		if ( fForceDamnSound || Random( 100 ) < 40 )
		{
			switch( bReason )
			{
				case UNJAM_GUN_CHECK:
				case NOTICE_DART_CHECK:
				case LIE_TO_QUEEN_CHECK:
					// silent check
					break;
				default:
					pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
					break;
			}
		}

	}
	else
	{
		// A buddy might make a positive comment based on our success;
		// Increase the chance for people with higher skill and for more difficult tasks
		iChance = 15 + iSkill / 20 + (-bChanceMod) / 20;
		if (iRoll < iChance)
		{
			// If a buddy of this merc is standing around nearby, they'll make a positive comment.
			iLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
			for ( pTeamSoldier = MercPtrs[ iLoop ]; iLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; iLoop++,pTeamSoldier++ )
			{
				if ( OK_INSECTOR_MERC( pTeamSoldier ) )
				{
					bBuddyIndex = WhichBuddy( pTeamSoldier->ubProfile, pSoldier->ubProfile );
					if (bBuddyIndex >= 0 && SpacesAway( pSoldier->sGridNo, pTeamSoldier->sGridNo ) < 15)
					{
						switch( bBuddyIndex )
						{
							case 0:
								// buddy #1 did something good!
								TacticalCharacterDialogue( pTeamSoldier, QUOTE_BUDDY_1_GOOD );
								break;
							case 1:
								// buddy #2 did something good!
								TacticalCharacterDialogue( pTeamSoldier, 	QUOTE_BUDDY_2_GOOD );
								break;
							case 2:
								// learn to like buddy did something good!
								TacticalCharacterDialogue( pTeamSoldier, QUOTE_LEARNED_TO_LIKE_WITNESSED );
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}
	return( iMadeItBy );
}


INT8 CalcTrapDetectLevel( SOLDIERTYPE * pSoldier, BOOLEAN fExamining )
{
	// return the level of trap which the guy is able to detect

	INT8 bDetectLevel;

	// formula: 1 pt for every exp_level
	//	 plus 1 pt for every 40 explosives
	//	 less 1 pt for every 20 wisdom MISSING

	bDetectLevel = EffectiveExpLevel( pSoldier );
	bDetectLevel += (EffectiveExplosive( pSoldier ) / 40);
	bDetectLevel -= ((100 - EffectiveWisdom( pSoldier ) ) / 20);

	// STOMP traits - Technician trait detecting traps bonus - SANDRO
	if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, TECHNICIAN_NT ))
	{
		bDetectLevel += (gSkillTraitValues.ubTEChanceToDetectTrapsBonus * NUM_SKILL_TRAITS( pSoldier, TECHNICIAN_NT )); // +2 trap level per trait
	}

	// if the examining flag is true, this isn't just a casual glance
	// and the merc should have a higher chance
	if (fExamining)
	{
		bDetectLevel += (INT8) PreRandom(bDetectLevel / 3 + 2);
	}
	// SANDRO - optimists just don't even think about the traps, randomly ignoring this all
	else if ( gGameOptions.fNewTraitSystem && gMercProfiles[pSoldier->ubProfile].bCharacterTrait == CHAR_TRAIT_OPTIMIST )
	{
		if ( Chance(50) )
			bDetectLevel = 1;
		else
			bDetectLevel -= 2;
	}

	// if substantially bleeding, or still in serious shock, randomly lower value
	if ((pSoldier->bBleeding > 20) || (pSoldier->aiData.bShock > 1))
	{
		bDetectLevel -= (INT8) PreRandom(3);
	}

	if (bDetectLevel < 1)
	{
		bDetectLevel = 1;
	}

	return( bDetectLevel );
}
