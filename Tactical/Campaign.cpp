#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "builddefines.h"
	#include <wchar.h>
	#include <stdio.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	#include "MemMan.h"
	#include "Overhead Types.h"

	#include "random.h"
	#include "campaign.h"
	#include "dialogue control.h"
	#include "Map Screen Interface.h"
	#include "Message.h"
	#include "Game Clock.h"
	#include "Strategic Mines.h"
	#include "Strategic Status.h"
	#include "Sys Globals.h"
	#include "Text.h"
	#include "GameSettings.h"
	#include "Assignments.h"
	#include "Interface.h"
	#include "Game Event Hook.h"
	#include "Overhead.h"
	#include "meanwhile.h"
	#include "Quests.h"
	#include "Squads.h"
	#include "Soldier macros.h"
	#include "strategic.h"
	#include "strategicmap.h"
	#include "Town Militia.h"
	#include "Campaign Types.h"
	#include "Tactical Save.h"
	#include "Strategic AI.h"
	#include "interface Dialogue.h"
	#include "DynamicDialogue.h"
#endif

#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "mercs.h"
#include "ub_config.h"
#endif

#include "GameInitOptionsScreen.h"

#include "email.h"
#include "mercs.h"


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


extern	UINT8	gbPlayerNum;


#ifdef JA2TESTVERSION
// comment out to get rid of stat change msgs
//#define STAT_CHANGE_DEBUG
#endif

#ifdef STAT_CHANGE_DEBUG
STR16 wDebugStatStrings[]={
	L"",
	L"Life (Max)",
  L"Agility",
  L"Dexterity",
  L"Wisdom",
  L"Medical",
  L"Explosives",
  L"Mechanical",
	L"Marksmanship",
  L"Experience Level",
  L"Strength",
  L"Leadership",
};
#endif


// local prototypes
UINT8 CalcImportantSectorControl( void );
// HEADROCK HAM B1: New function to calculate how many important sectors there are in the game at all.
UINT8 CalcTotalImportantSectors( void );
UINT16 CountSurfaceSectorsVisited( void );
// HAEDROCK HAM B1: New function to calculate how many sectors CAN be visited, on the surface.
UINT16 TotalVisitableSurfaceSectors( void );



// give pSoldier usNumChances to improve ubStat.  If it's from training, it doesn't count towards experience level gain
void StatChange(SOLDIERTYPE *pSoldier, UINT8 ubStat, UINT16 usNumChances, UINT8 ubReason)
{
	if (pSoldier == NULL || pSoldier->bActive == FALSE)
		return;	// THIS SHOULD NEVER HAPPEN

	Assert(pSoldier != NULL);
	Assert(pSoldier->bActive);

	// ignore non-player soldiers
	if (!PTR_OURTEAM)
		return;

	// ignore anything without a profile
	if (pSoldier->ubProfile == NO_PROFILE)
		return;

	// ignore vehicles and robots
	if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT ) )
		return;

	if( pSoldier->bAssignment == ASSIGNMENT_POW )
	{
		ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"ERROR: StatChange: %s improving stats while POW! ubStat %d", pSoldier->GetName(), ubStat );
		return;
	}

	// no points earned while somebody is unconscious (for assist XPs, and such)
	if ( pSoldier->stats.bLife < CONSCIOUSNESS )
		return;


#ifdef TESTVERSION
	if ( gTacticalStatus.fStatChangeCheatOn )
	{
		usNumChances = 100;
	}
#endif

	ProcessStatChange( &( gMercProfiles[ pSoldier->ubProfile ] ), ubStat, usNumChances, ubReason );

	// Update stats....right away... ATE
	UpdateStats( pSoldier, ubReason );
}


// this is the equivalent of StatChange(), but for use with mercs not currently on player's team
// give pProfile usNumChances to improve ubStat.  If it's from training, it doesn't count towards experience level gain
void ProfileStatChange(MERCPROFILESTRUCT *pProfile, UINT8 ubStat, UINT16 usNumChances, UINT8 ubReason)
{
	// dead guys don't do nuthin' !
	if ( pProfile->bMercStatus == MERC_IS_DEAD )
		return;

	if ( pProfile->bLife < OKLIFE )
		return;

	ProcessStatChange( pProfile, ubStat, usNumChances, ubReason );

	// Update stats....right away... ATE
	ProfileUpdateStats( pProfile, ubReason );
}


void ProcessStatChange(MERCPROFILESTRUCT *pProfile, UINT8 ubStat, UINT16 usNumChances, UINT8 ubReason)
{
	UINT32 uiCnt,uiEffLevel;
	INT16 sSubPointChange = 0;
	UINT16 usChance=0;
	UINT16 usSubpointsPerPoint;
	UINT16 usSubpointsPerLevel;
	INT8 bCurrentRating;
	UINT16 *psStatGainPtr;
	BOOLEAN fAffectedByWisdom = TRUE;

	Assert(pProfile != NULL);

	if ( pProfile->bEvolution == NO_EVOLUTION )
		return;     // No change possible, quit right away

	// if this is a Reverse-Evolving merc who attempting to train
	if ( ( ubReason == FROM_TRAINING ) && ( pProfile->bEvolution == DEVOLVE ) )
		return;	// he doesn't get any benefit, but isn't penalized either

	if (usNumChances == 0)
		return;
	
	usSubpointsPerPoint = SubpointsPerPoint(ubStat, pProfile->bExpLevel);
	usSubpointsPerLevel = SubpointsPerPoint(EXPERAMT, pProfile->bExpLevel);

	switch (ubStat)
	{
		case HEALTHAMT:
			bCurrentRating = pProfile->bLifeMax;
			psStatGainPtr = (UINT16 *)&(pProfile->sLifeGain);
			// NB physical stat checks not affected by wisdom, unless training is going on
			fAffectedByWisdom = FALSE;
		break;

		case AGILAMT:
			bCurrentRating = pProfile->bAgility;
			psStatGainPtr = (UINT16 *)&(pProfile->sAgilityGain);
			fAffectedByWisdom = FALSE;
		break;

		case DEXTAMT:
			bCurrentRating = pProfile->bDexterity;
			psStatGainPtr = (UINT16 *)&(pProfile->sDexterityGain);
			fAffectedByWisdom = FALSE;
		break;

		case WISDOMAMT:
			bCurrentRating = pProfile->bWisdom;
			psStatGainPtr = (UINT16 *)&(pProfile->sWisdomGain);
		break;

		case MEDICALAMT:
			bCurrentRating = pProfile->bMedical;
			psStatGainPtr = (UINT16 *)&(pProfile->sMedicalGain);
		break;

		case EXPLODEAMT:
			bCurrentRating = pProfile->bExplosive;
			psStatGainPtr = (UINT16 *)&(pProfile->sExplosivesGain);
		break;

		case MECHANAMT:
			bCurrentRating = pProfile->bMechanical;
			psStatGainPtr = (UINT16 *)&(pProfile->sMechanicGain);
		break;

		case MARKAMT:
			bCurrentRating = pProfile->bMarksmanship;
			psStatGainPtr = (UINT16 *)&(pProfile->sMarksmanshipGain);
		break;

		case EXPERAMT:
			bCurrentRating = pProfile->bExpLevel;
			psStatGainPtr = (UINT16 *)&(pProfile->sExpLevelGain);
		break;

		case STRAMT:
			bCurrentRating = pProfile->bStrength;
			psStatGainPtr = (UINT16 *)&(pProfile->sStrengthGain);
			fAffectedByWisdom = FALSE;
		break;

		case LDRAMT:
			bCurrentRating = pProfile->bLeadership;
			psStatGainPtr = (UINT16 *)&(pProfile->sLeadershipGain);
		break;

		default:
			// BETA message
			ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"ERROR: ProcessStatChange: Rcvd unknown ubStat %d", ubStat);
		return;
	}


	if (ubReason == FROM_TRAINING)
	{
		// training always affected by wisdom
		fAffectedByWisdom = TRUE;
	}
	
	// stats/skills of 0 can NEVER be improved!
	if ( bCurrentRating < 0 )
	{
		return;
	}

	// loop once for each chance to improve
	for (uiCnt = 0; uiCnt < usNumChances; uiCnt++)
	{
		if (pProfile->bEvolution != DEVOLVE)               // Evolves!
		{
			// if this is improving from a failure, and a successful roll would give us enough to go up a point
			if ((ubReason == FROM_FAILURE) && ((*psStatGainPtr + 1) >= usSubpointsPerPoint))
			{
				// can't improve any more from this statchange, because Ian don't want failures causin increases!
				break;
			}

			if (ubStat != EXPERAMT)
			{
				// NON-experience level changes, actual usChance depends on bCurrentRating
				// Base usChance is '100 - bCurrentRating'
				usChance = 100 - (bCurrentRating + (*psStatGainPtr / usSubpointsPerPoint));

				// prevent training beyond the training cap
				if ((ubReason == FROM_TRAINING) && (bCurrentRating + (*psStatGainPtr / usSubpointsPerPoint) >= gGameExternalOptions.ubTrainingSkillMax))
				{
					usChance = 0;
				}
			}
			else
			{
				// Experience level changes, actual usChance depends on level
				// Base usChance is '100 - (10 * current level)'
				usChance = 100 - 10 * (bCurrentRating + (*psStatGainPtr / usSubpointsPerPoint));
			}

			// if there IS a usChance, adjust it for high or low wisdom (50 is avg)
			if (usChance > 0 && fAffectedByWisdom)
			{
				usChance += (usChance * (pProfile->bWisdom + (pProfile->sWisdomGain / SubpointsPerPoint(WISDOMAMT, pProfile->bExpLevel)) - 50)) / 100;
			}

/*
			// if the stat is Marksmanship, and the guy is a hopeless shot
			if ((ubStat == MARKAMT) && (pProfile->bSpecialTrait == HOPELESS_SHOT))
			{
				usChance /= 5;		// MUCH slower to improve, divide usChance by 5
			}
*/

			// SANDRO - penalty for primitive people, they get lesser chance to gain point for certain skills
			if ( gGameOptions.fNewTraitSystem && (usChance > 10) && (ubStat != EXPERAMT) && (pProfile->bCharacterTrait == CHAR_TRAIT_PRIMITIVE) )
			{
				switch (ubStat)
				{
					case WISDOMAMT:
					case MEDICALAMT:
					case EXPLODEAMT:
					case MECHANAMT:
					case LDRAMT:
						usChance = max(1, (usChance - 10)); // -10% chance to gain the point
						break;
				}
			}

			// Buggler: more evolution rate choices
			if (pProfile->bEvolution == THREEQUARTER_EVOLUTION)
				usChance = max(1, usChance * 0.75);
			else if (pProfile->bEvolution == HALF_EVOLUTION)
				usChance =  max(1, usChance * 0.5);
			else if (pProfile->bEvolution == ONEQUARTER_EVOLUTION)
				usChance =  max(1, usChance * 0.25);


			// maximum possible usChance is 99%
			if (usChance > 99)
			{
					usChance = 99;
			}

			if (PreRandom(100) < usChance )
			{
				(*psStatGainPtr)++;
				sSubPointChange++;

				// as long as we're not dealing with exp_level changes (already added above!)
				// and it's not from training, and the exp level isn't max'ed out already
				if ((ubStat != EXPERAMT) && (ubReason != FROM_TRAINING))
				{
					uiEffLevel = pProfile->bExpLevel + (pProfile->sExpLevelGain / usSubpointsPerLevel);

					// if level is not at maximum
					if (uiEffLevel < MAXEXPLEVEL)
					{
						// if this is NOT improving from a failure, OR it would NOT give us enough to go up a level
						if ((ubReason != FROM_FAILURE) || ((pProfile->sExpLevelGain + 1) < usSubpointsPerLevel))
						{
							// all other stat changes count towards experience level changes (1 for 1 basis)
							pProfile->sExpLevelGain++;
						}
					}
				}
			}
		}
		else                          // Regresses!
		{
			// regression can happen from both failures and successes (but not training, checked above)

			if (ubStat != EXPERAMT)
			{
				// NON-experience level changes, actual usChance depends on bCurrentRating
				switch (ubStat)
				{
					case HEALTHAMT:
					case AGILAMT:
					case DEXTAMT:
					case WISDOMAMT:
					case STRAMT:
						// Base usChance is 'bCurrentRating - 1', since these must remain at 1-100
						usChance = bCurrentRating + (*psStatGainPtr / usSubpointsPerPoint) - 1;
						break;

					case MEDICALAMT:
					case EXPLODEAMT:
					case MECHANAMT:
					case MARKAMT:
					case LDRAMT:
						// Base usChance is 'bCurrentRating', these can drop to 0
						usChance = bCurrentRating + (*psStatGainPtr / usSubpointsPerPoint);
						break;
				}
			}
			else
			{
				// Experience level changes, actual usChance depends on level
				// Base usChance is '10 * (current level - 1)'
				usChance = 10 * (bCurrentRating + (*psStatGainPtr / usSubpointsPerPoint) - 1);

				// if there IS a usChance, adjust it for high or low wisdom (50 is avg)
				if (usChance > 0 && fAffectedByWisdom)
				{
					usChance -= (usChance * (pProfile->bWisdom + (pProfile->sWisdomGain / SubpointsPerPoint(WISDOMAMT, pProfile->bExpLevel)) - 50)) / 100;
				}

				// if there's ANY usChance, minimum usChance is 1% regardless of wisdom
				if (usChance < 1)
				{
					usChance = 1;
				}
			}

			if (usChance <= 0)
				usChance = 1;

			if (PreRandom(100) < usChance )
			{
				(*psStatGainPtr)--;
				sSubPointChange--;

				// as long as we're not dealing with exp_level changes (already added above!)
				// and it's not from training, and the exp level isn't max'ed out already
				if ((ubStat != EXPERAMT) && (ubReason != FROM_TRAINING))
				{
					uiEffLevel = pProfile->bExpLevel + (pProfile->sExpLevelGain / usSubpointsPerLevel );

					// if level is not at minimum
					if (uiEffLevel > 1)
					{
						// all other stat changes count towards experience level changes (1 for 1 basis)
						pProfile->sExpLevelGain--;
					}
				}
			}
		}
	}

#ifdef STAT_CHANGE_DEBUG
	if (sSubPointChange != 0)
	{
		// debug message
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"%s's %s changed by %d", pProfile->zNickname, wDebugStatStrings[ubStat], sSubPointChange );
	}
#endif


	// exclude training, that's not under our control
	if (ubReason != FROM_TRAINING)
	{
		// increment counters that track how often stat changes are being awarded
		pProfile->usStatChangeChances[ ubStat ] += usNumChances;
		pProfile->usStatChangeSuccesses[ ubStat ] += abs( sSubPointChange );
	}
}


// convert hired mercs' stats subpoint changes into actual point changes where warranted
void UpdateStats( SOLDIERTYPE *pSoldier, UINT8 ubReason )
{
	ProcessUpdateStats( &( gMercProfiles[ pSoldier->ubProfile ] ), pSoldier, ubReason );
}


// UpdateStats version for mercs not currently on player's team
void ProfileUpdateStats( MERCPROFILESTRUCT *pProfile, UINT8 ubReason )
{
	ProcessUpdateStats( pProfile, NULL, ubReason );
}


void ChangeStat( MERCPROFILESTRUCT *pProfile, SOLDIERTYPE *pSoldier, UINT8 ubStat, INT16 sPtsChanged, UINT8 ubReason = 0 )
{
	// this function changes the stat a given amount...
	INT16 *psStatGainPtr = NULL;
	INT8 *pbStatPtr = NULL;
	INT8 *pbSoldierStatPtr = NULL;
	INT8 *pbStatDeltaPtr = NULL;
	UINT32 *puiStatTimerPtr = NULL;
	BOOLEAN fChangeTypeIncrease;
	BOOLEAN fChangeSalary;
	UINT32 uiLevelCnt;
	UINT8 ubMercMercIdValue = 0;
	UINT16 usIncreaseValue = 0;
	UINT16 usSubpointsPerPoint;
	INT8 bDamagedStatToRaise = -1; // added by SANDRO 

	usSubpointsPerPoint = SubpointsPerPoint(ubStat, pProfile->bExpLevel );

	// build ptrs to appropriate profiletype stat fields
	switch( ubStat )
	{
	case HEALTHAMT:
    psStatGainPtr = &(pProfile->sLifeGain);
		pbStatDeltaPtr = &(pProfile->bLifeDelta);
		pbStatPtr = &(pProfile->bLifeMax);
    break;

  case AGILAMT:
    psStatGainPtr = &(pProfile->sAgilityGain);
		pbStatDeltaPtr = &(pProfile->bAgilityDelta);
		pbStatPtr = &(pProfile->bAgility);
    break;

  case DEXTAMT:
    psStatGainPtr = &(pProfile->sDexterityGain);
		pbStatDeltaPtr = &(pProfile->bDexterityDelta);
		pbStatPtr = &(pProfile->bDexterity);
    break;

  case WISDOMAMT:
    psStatGainPtr = &(pProfile->sWisdomGain);
		pbStatDeltaPtr = &(pProfile->bWisdomDelta);
    pbStatPtr = &(pProfile->bWisdom);
		break;

  case MEDICALAMT:
    psStatGainPtr = &(pProfile->sMedicalGain);
		pbStatDeltaPtr = &(pProfile->bMedicalDelta);
    pbStatPtr = &(pProfile->bMedical);
		break;

  case EXPLODEAMT:
    psStatGainPtr = &(pProfile->sExplosivesGain);
		pbStatDeltaPtr = &(pProfile->bExplosivesDelta);
    pbStatPtr = &(pProfile->bExplosive);
		break;

  case MECHANAMT:
    psStatGainPtr = &(pProfile->sMechanicGain);
		pbStatDeltaPtr = &(pProfile->bMechanicDelta);
    pbStatPtr = &(pProfile->bMechanical);
		break;

  case MARKAMT:
    psStatGainPtr = &(pProfile->sMarksmanshipGain);
		pbStatDeltaPtr = &(pProfile->bMarksmanshipDelta);
    pbStatPtr = &(pProfile->bMarksmanship);
		break;

  case EXPERAMT:
    psStatGainPtr = &(pProfile->sExpLevelGain);
		pbStatDeltaPtr = &(pProfile->bExpLevelDelta);
    pbStatPtr = &(pProfile->bExpLevel);
		break;

	case STRAMT:
    psStatGainPtr = &(pProfile->sStrengthGain);
		pbStatDeltaPtr = &(pProfile->bStrengthDelta);
    pbStatPtr = &(pProfile->bStrength);
		break;

	case LDRAMT:
    psStatGainPtr = &(pProfile->sLeadershipGain);
		pbStatDeltaPtr = &(pProfile->bLeadershipDelta);
    pbStatPtr = &(pProfile->bLeadership);
		break;
	}


	// if this merc is currently on the player's team
	if (pSoldier != NULL)
	{
		// build ptrs to appropriate soldiertype stat fields
		// SANDRO - added a variable for stat repair
		switch( ubStat )
		{
		case HEALTHAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bLifeMax );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeHealthTime);
			usIncreaseValue = HEALTH_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_HEALTH;
			break;

		case AGILAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bAgility );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeAgilityTime);
			usIncreaseValue = AGIL_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_AGILITY;
			break;

		case DEXTAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bDexterity );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeDexterityTime);
			usIncreaseValue = DEX_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_DEXTERITY;
			break;

		case WISDOMAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bWisdom );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeWisdomTime);
			usIncreaseValue = WIS_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_WISDOM;
			break;

		case MEDICALAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bMedical );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeMedicalTime);
			usIncreaseValue = MED_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_MEDICAL;
			break;

		case EXPLODEAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bExplosive );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeExplosivesTime);
			usIncreaseValue = EXP_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_EXPLOSIVES;
			break;

		case MECHANAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bMechanical );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeMechanicalTime);
			usIncreaseValue = MECH_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_MECHANICAL;
			break;

		case MARKAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bMarksmanship );
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeMarksmanshipTime);
			usIncreaseValue = MRK_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_MARKSMANSHIP;
			break;

		case EXPERAMT:
			pbSoldierStatPtr = &(pSoldier->stats.bExpLevel);
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeLevelTime );
			usIncreaseValue = LVL_INCREASE;
			break;

		case STRAMT:
			pbSoldierStatPtr = &(pSoldier->stats.bStrength);
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeStrengthTime);
			usIncreaseValue = STRENGTH_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_STRENGTH;
			break;

		case LDRAMT:
			pbSoldierStatPtr = &( pSoldier->stats.bLeadership);
			puiStatTimerPtr = &( pSoldier->timeChanges.uiChangeLeadershipTime);
			usIncreaseValue = LDR_INCREASE;
			bDamagedStatToRaise = DAMAGED_STAT_LEADERSHIP;
			break;
		}
	}

	// ptrs set up, now handle
	// if the stat needs to change
	if ( sPtsChanged != 0 )
	{
		// if a stat improved
		if ( sPtsChanged > 0 )
		{
			fChangeTypeIncrease = TRUE;
		}
		else
		{
			fChangeTypeIncrease = FALSE;
		}

		// update merc profile stat
		*pbStatPtr += sPtsChanged;

		// if this merc is currently on the player's team (DON'T count increases earned outside the player's employ)
		if (pSoldier != NULL)
		{
			// also note the delta (how much this stat has changed since start of game)
			*pbStatDeltaPtr += sPtsChanged;
		}

		// reduce gain to the unused subpts only
		*psStatGainPtr = ( *psStatGainPtr ) % usSubpointsPerPoint;


		// if the guy is employed by player
		if (pSoldier != NULL)
		{
			// transfer over change to soldiertype structure
			*pbSoldierStatPtr = *pbStatPtr;

			// SANDRO - reduce damaged stat if this stat was increased normally
			if ( fChangeTypeIncrease && (bDamagedStatToRaise != -1) )
			{
				INT16 ptstolower = sPtsChanged;
				UINT8 oldctrpts = pSoldier->ubCriticalStatDamage[ bDamagedStatToRaise ];

				if (pSoldier->ubCriticalStatDamage[ bDamagedStatToRaise ] > 0)
				{
					pSoldier->ubCriticalStatDamage[ bDamagedStatToRaise ] = max( 0, (pSoldier->ubCriticalStatDamage[ bDamagedStatToRaise ] - ptstolower));

					ptstolower -= oldctrpts - pSoldier->ubCriticalStatDamage[ bDamagedStatToRaise ];
				}
								
				if ( bDamagedStatToRaise == DAMAGED_STAT_STRENGTH && pSoldier->usStarveDamageStrength > 0 )
					pSoldier->usStarveDamageStrength = max(0, pSoldier->usStarveDamageStrength - ptstolower);
				else if ( bDamagedStatToRaise == DAMAGED_STAT_HEALTH && pSoldier->usStarveDamageHealth > 0 )
					pSoldier->usStarveDamageHealth = max(0, pSoldier->usStarveDamageHealth - ptstolower);
			}

			// if it's a level gain, or sometimes for other stats
			// ( except health; not only will it sound silly, but
			// also we give points for health on sector traversal and this would
			// probaby mess up battle handling too )
			if ( (ubStat != HEALTHAMT) && ( (ubStat == EXPERAMT) || Random( 100 ) < 25 ) )
			//if ( (ubStat != EXPERAMT) && (ubStat != HEALTHAMT) && ( Random( 100 ) < 25 ) )
			{
				// Pipe up with "I'm getting better at this!"
				TacticalCharacterDialogueWithSpecialEventEx( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_DISPLAY_STAT_CHANGE, fChangeTypeIncrease, sPtsChanged, ubStat );

				//Madd: option to make mercs quiet during training / doctoring / repairing
				if ( ((pSoldier->bAssignment == TRAIN_BY_OTHER || pSoldier->bAssignment == TRAIN_TEAMMATE || pSoldier->bAssignment == TRAIN_SELF || 
							pSoldier->bAssignment == FACILITY_STAFF || pSoldier->bAssignment == TRAIN_TOWN || pSoldier->bAssignment == TRAIN_MOBILE
							|| pSoldier->bAssignment == TRAIN_WORKERS ) 
							&& !gGameSettings.fOptions[TOPTION_QUIET_TRAINING]) ||
					 (IS_REPAIR(pSoldier->bAssignment) && !gGameSettings.fOptions[TOPTION_QUIET_REPAIRING]) ||
					 (IS_DOCTOR(pSoldier->bAssignment) && !gGameSettings.fOptions[TOPTION_QUIET_DOCTORING]))

					 TacticalCharacterDialogue( pSoldier, QUOTE_EXPERIENCE_GAIN );
			}
			else
			{
				CHAR16 wTempString[ 128 ];

				// tell player about it
				BuildStatChangeString( wTempString, pSoldier->GetName(), fChangeTypeIncrease, sPtsChanged, ubStat );
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, wTempString );
			}

			// update mapscreen soldier info panel
			fCharacterInfoPanelDirty = TRUE;

			// remember what time it changed at, it's displayed in a different color for a while afterwards
			*puiStatTimerPtr = GetJA2Clock();

			if( fChangeTypeIncrease )
			{
				pSoldier->usValueGoneUp |= usIncreaseValue;
			}
			else
			{
				pSoldier->usValueGoneUp &= ~( usIncreaseValue );
			}

			fInterfacePanelDirty = DIRTYLEVEL2;
		}


		// special handling for LIFEMAX
		if( ubStat == HEALTHAMT )
		{
			// adjust current health by the same amount as max health
			pProfile->bLife += sPtsChanged;

			// don't let this kill a guy or knock him out!!!
			if (pProfile->bLife < OKLIFE)
			{
				pProfile->bLife = OKLIFE;
			}

			// if the guy is employed by player
			if (pSoldier != NULL)
			{
				// adjust current health by the same amount as max health
				pSoldier->stats.bLife += sPtsChanged;

				// don't let this kill a guy or knock him out!!!
				if (pSoldier->stats.bLife < OKLIFE)
				{
					pSoldier->stats.bLife = OKLIFE;
				}
			}
		}

		// special handling for EXPERIENCE LEVEL
		// merc salaries increase if level goes up (but they don't go down if level drops!)
		if( (ubStat == EXPERAMT) && fChangeTypeIncrease)
		{
			// if the guy is employed by player
			if (pSoldier != NULL)
			{
				switch (pSoldier->ubWhatKindOfMercAmI)
				{
					case MERC_TYPE__AIM_MERC:
						// A.I.M.
						pSoldier->flags.fContractPriceHasIncreased = TRUE;
						fChangeSalary = TRUE;
						break;

					case MERC_TYPE__MERC:
						// M.E.R.C.
						ubMercMercIdValue = pSoldier->ubProfile;

						/*
						// Biff's profile id ( 40 ) is the base
						ubMercMercIdValue -= BIFF;

						// offset for the 2 profiles of Larry (we only have one email for Larry..but 2 profile entries
						if( ubMercMercIdValue >= ( LARRY_DRUNK - BIFF ) )
						{
							ubMercMercIdValue--;
						}
						*/

						//
						// Send special E-mail
						//

						//	DEF: 03/06/99 Now sets an event that will be processed later in the day
						//	ubEmailOffset = MERC_UP_LEVEL_BIFF + MERC_UP_LEVEL_LENGTH_BIFF * ( ubMercMercIdValue );
						//	AddEmail( ubEmailOffset, MERC_UP_LEVEL_LENGTH_BIFF, SPECK_FROM_MERC, GetWorldTotalMin() );

						AddStrategicEvent( EVENT_MERC_MERC_WENT_UP_LEVEL_EMAIL_DELAY, GetWorldTotalMin( ) + 60 + Random( 60 ), ubMercMercIdValue );

						fChangeSalary = TRUE;
						break;

					default:
						// others don't increase salary
						fChangeSalary = FALSE;
						break;
				}
			}
			else	// not employed by player
			{
				// only AIM and M.E.R.C.s update stats when not on player's team, and both of them DO change salary
				fChangeSalary = TRUE;
			}

			if (fChangeSalary)
			{
				// increase all salaries and medical deposits, once for each level gained
				for (uiLevelCnt = 0; uiLevelCnt < (UINT32) sPtsChanged; ++uiLevelCnt)
				{
					pProfile->sSalary				= (INT16) CalcNewSalary(pProfile->sSalary,					fChangeTypeIncrease, MAX_DAILY_SALARY);
					pProfile->uiWeeklySalary		=		  CalcNewSalary(pProfile->uiWeeklySalary,			fChangeTypeIncrease, MAX_LARGE_SALARY);
					pProfile->uiBiWeeklySalary		=	      CalcNewSalary(pProfile->uiBiWeeklySalary,			fChangeTypeIncrease, MAX_LARGE_SALARY);
					pProfile->sTrueSalary			= (INT16) CalcNewSalary(pProfile->sTrueSalary,				fChangeTypeIncrease, MAX_DAILY_SALARY);
					pProfile->sMedicalDepositAmount = (INT16) CalcNewSalary(pProfile->sMedicalDepositAmount,	fChangeTypeIncrease, MAX_DAILY_SALARY);

					//if (pSoldier != NULL)
						// DON'T increase the *effective* medical deposit, it's already been paid out
						// pSoldier->usMedicalDeposit = pProfile->sMedicalDepositAmount;
				}
			}
		}

		// Flugente: dynamic opinions
		if ( FROM_TRAINING == ubReason )
			HandleDynamicOpinionTeaching( pSoldier, ubStat );
	}
}



// pSoldier may be NULL!
void ProcessUpdateStats( MERCPROFILESTRUCT *pProfile, SOLDIERTYPE *pSoldier, UINT8 ubReason )
{
	// this function will run through the soldier's profile and update their stats based on any accumulated gain pts.
	UINT8 ubStat = 0;
	INT16 *psStatGainPtr = NULL;
	INT8 *pbStatPtr = NULL;
	INT8 *pbSoldierStatPtr = NULL;
	INT8 bMinStatValue;
	INT8 bMaxStatValue;
	UINT16 usSubpointsPerPoint;
	INT16 sPtsChanged;


	// if hired, not back at AIM
	if ( pSoldier != NULL )
	{
		// ATE: if in the midst of an attack, if in the field, delay all stat changes until the check made after the 'attack'...
		if ( ( gTacticalStatus.ubAttackBusyCount > 0 ) && pSoldier->bInSector && ( gTacticalStatus.uiFlags & INCOMBAT ) )
			return;

		// ignore non-player soldiers
		if (!PTR_OURTEAM)
			return;

		// ignore anything without a profile
		if (pSoldier->ubProfile == NO_PROFILE)
			return;

		// ignore vehicles and robots
		if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT ) )
			return;

		// delay increases while merc is dying
		if (pSoldier->stats.bLife < OKLIFE)
			return;

		// ignore POWs - shouldn't ever be getting this far
		if( pSoldier->bAssignment == ASSIGNMENT_POW )
		{
			return;
		}
	}
	else
	{
		// dead guys don't do nuthin' !
		if ( pProfile->bMercStatus == MERC_IS_DEAD )
			return;

		if ( pProfile->bLife < OKLIFE )
			return;
	}


	// check every attribute, skill, and exp.level, too
	for( ubStat = FIRST_CHANGEABLE_STAT; ubStat <= LAST_CHANGEABLE_STAT; ubStat++ )
	{
		// set default min & max, subpoints/pt.
		bMinStatValue = 1;
		bMaxStatValue = MAX_STAT_VALUE;
		usSubpointsPerPoint = SubpointsPerPoint(ubStat, pProfile->bExpLevel);

		// build ptrs to appropriate profiletype stat fields
		switch( ubStat )
		{
			case HEALTHAMT:
      psStatGainPtr = &(pProfile->sLifeGain);
			pbStatPtr = &(pProfile->bLifeMax);

			bMinStatValue = OKLIFE;
      break;

    case AGILAMT:
      psStatGainPtr = &(pProfile->sAgilityGain);
			pbStatPtr = &(pProfile->bAgility);
      break;

    case DEXTAMT:
      psStatGainPtr = &(pProfile->sDexterityGain);
			pbStatPtr = &(pProfile->bDexterity);
      break;

    case WISDOMAMT:
      psStatGainPtr = &(pProfile->sWisdomGain);
      pbStatPtr = &(pProfile->bWisdom);
			break;

    case MEDICALAMT:
      psStatGainPtr = &(pProfile->sMedicalGain);
      pbStatPtr = &(pProfile->bMedical);

			bMinStatValue = 0;
			break;

    case EXPLODEAMT:
      psStatGainPtr = &(pProfile->sExplosivesGain);
      pbStatPtr = &(pProfile->bExplosive);

			bMinStatValue = 0;
			break;

    case MECHANAMT:
      psStatGainPtr = &(pProfile->sMechanicGain);
      pbStatPtr = &(pProfile->bMechanical);

			bMinStatValue = 0;
			break;

    case MARKAMT:
      psStatGainPtr = &(pProfile->sMarksmanshipGain);
      pbStatPtr = &(pProfile->bMarksmanship);

			bMinStatValue = 0;
			break;

    case EXPERAMT:
      psStatGainPtr = &(pProfile->sExpLevelGain);
      pbStatPtr = &(pProfile->bExpLevel);

			bMaxStatValue = MAXEXPLEVEL;
			break;

		case STRAMT:
      psStatGainPtr = &(pProfile->sStrengthGain);
      pbStatPtr = &(pProfile->bStrength);
			break;

		case LDRAMT:
      psStatGainPtr = &(pProfile->sLeadershipGain);
      pbStatPtr = &(pProfile->bLeadership);

			bMinStatValue = 0; // Snap: Since leadership can now be 0...
			break;
		}


		// if this merc is currently on the player's team
		if (pSoldier != NULL)
		{
			// build ptrs to appropriate soldiertype stat fields
			switch( ubStat )
			{
				case HEALTHAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bLifeMax );
				break;

			case AGILAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bAgility );
				break;

			case DEXTAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bDexterity );
				break;

			case WISDOMAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bWisdom );
				break;

			case MEDICALAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bMedical );
				break;

			case EXPLODEAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bExplosive );
				break;

			case MECHANAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bMechanical );
				break;

			case MARKAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bMarksmanship );
				break;

			case EXPERAMT:
				pbSoldierStatPtr = &(pSoldier->stats.bExpLevel);
				break;

			case STRAMT:
				pbSoldierStatPtr = &(pSoldier->stats.bStrength);
				break;

			case LDRAMT:
				pbSoldierStatPtr = &( pSoldier->stats.bLeadership);
				break;
			}
		}


		// ptrs set up, now handle

		// Calc how many full points worth of stat changes we have accumulated in this stat (positive OR negative!)
		// NOTE: for simplicity, this hopes nobody will go up more than one level at once, which would change the subpoints/pt
		sPtsChanged = ( *psStatGainPtr ) / usSubpointsPerPoint;

		// gone too high or too low?..handle the fact
		if( (*pbStatPtr + sPtsChanged) > bMaxStatValue )
		{
			// reduce change to reach max value and reset stat gain ptr
			sPtsChanged = bMaxStatValue - *pbStatPtr;
			*psStatGainPtr = 0;
		}
		else
		if( (*pbStatPtr + sPtsChanged) < bMinStatValue )
		{
			// reduce change to reach min value and reset stat gain ptr
			sPtsChanged = bMinStatValue - *pbStatPtr;
			*psStatGainPtr = 0;
		}


		// if the stat needs to change
		if ( sPtsChanged != 0 )
		{
			// Otherwise, use normal stat increase stuff...
			ChangeStat( pProfile, pSoldier, ubStat, sPtsChanged, ubReason );
		}
	}
}


void HandleAnyStatChangesAfterAttack( void )
{
  INT32 cnt;
	SOLDIERTYPE *pSoldier;

	// must check everyone on player's team, not just the shooter
	for ( cnt = 0, pSoldier = MercPtrs[ 0 ]; cnt <= gTacticalStatus.Team[ MercPtrs[ 0 ]->bTeam ].bLastID; cnt++,pSoldier++)
	{
		if (pSoldier->bActive)
		{
			ProcessUpdateStats( &( gMercProfiles[ pSoldier->ubProfile ] ), pSoldier );
		}
	}
}


UINT32 CalcNewSalary(UINT32 uiOldSalary, BOOLEAN fIncrease, UINT32 uiMaxLimit)
{
  UINT32 uiNewSalary;
  FLOAT SalaryMultiplier = (FLOAT) ((gGameExternalOptions.gMercLevelUpSalaryIncreasePercentage / 100) + 1);

	// if he was working for free, it's still free!
	if (uiOldSalary == 0)
	{
		return(0);
	}

  if (fIncrease)
	{
    uiNewSalary = (UINT32) (uiOldSalary * SalaryMultiplier);
	}
  else
	{
    uiNewSalary = (UINT32) (uiOldSalary / SalaryMultiplier);
	}

  // round it off to a reasonable multiple
  uiNewSalary = RoundOffSalary(uiNewSalary);

	// let's set some reasonable limits here, lest it get silly one day
	if (uiNewSalary > uiMaxLimit)
		uiNewSalary = uiMaxLimit;

	if (uiNewSalary < 5)
		uiNewSalary = 5;


  return(uiNewSalary);
}


UINT32 RoundOffSalary(UINT32 uiSalary)
{
	UINT32 uiMultiple;


	// determine what multiple value the salary should be rounded off to
	if      (uiSalary <=   250) uiMultiple =    5;
	else if (uiSalary <=   500) uiMultiple =   10;
	else if (uiSalary <=  1000) uiMultiple =   25;
	else if (uiSalary <=  2000) uiMultiple =   50;
	else if (uiSalary <=  5000) uiMultiple =  100;
	else if (uiSalary <= 10000) uiMultiple =  500;
	else if (uiSalary <= 25000) uiMultiple = 1000;
	else if (uiSalary <= 50000) uiMultiple = 2000;
	else												uiMultiple = 5000;


	// if the salary doesn't divide evenly by the multiple
	if (uiSalary % uiMultiple)
	{
		// then we have to make it so, as Picard would say <- We have to wonder how much Alex gets out
		// and while we're at it, we round up to next higher multiple if halfway
		uiSalary = ((uiSalary + (uiMultiple / 2)) / uiMultiple) * uiMultiple;
	}

	return(uiSalary);
}


UINT16 SubpointsPerPoint(UINT8 ubStat, INT8 bExpLevel)
{
	UINT16 usSubpointsPerPoint;

	UINT16 HEALTH_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usHealthSubpointsToImprove;
	UINT16 STRENGTH_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usStrengthSubpointsToImprove;
	UINT16 DEXTERITY_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usDexteritySubpointsToImprove;
	UINT16 AGILITY_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usAgilitySubpointsToImprove;
	UINT16 WISDOM_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usWisdomSubpointsToImprove;
	UINT16 MARKSMANSHIP_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usMarksmanshipSubpointsToImprove;
	UINT16 MEDICAL_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usMedicalSubpointsToImprove;
	UINT16 MECHANICAL_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usMechanicalSubpointsToImprove;
	UINT16 LEADERSHIP_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usLeadershipSubpointsToImprove;
	UINT16 EXPLOSIVES_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usExplosivesSubpointsToImprove;
	UINT16 LEVEL_SUBPOINTS_TO_IMPROVE = gGameExternalOptions.usLevelSubpointsToImprove;
	// figure out how many subpoints this type of stat needs to change
  switch (ubStat)
  {
	  // Attributes
    case HEALTHAMT:
		usSubpointsPerPoint = HEALTH_SUBPOINTS_TO_IMPROVE;
		break;
    case AGILAMT:
		usSubpointsPerPoint = AGILITY_SUBPOINTS_TO_IMPROVE;
		break;
    case DEXTAMT:
		usSubpointsPerPoint = DEXTERITY_SUBPOINTS_TO_IMPROVE;
		break;
    case WISDOMAMT:
		usSubpointsPerPoint = WISDOM_SUBPOINTS_TO_IMPROVE;
		break;
	case STRAMT:
		usSubpointsPerPoint = STRENGTH_SUBPOINTS_TO_IMPROVE;
		break;
		
	  // Skills
    case MEDICALAMT:
		usSubpointsPerPoint = MEDICAL_SUBPOINTS_TO_IMPROVE;
		break;
    case EXPLODEAMT:
		usSubpointsPerPoint = EXPLOSIVES_SUBPOINTS_TO_IMPROVE;
		break;
    case MECHANAMT:
		usSubpointsPerPoint = MECHANICAL_SUBPOINTS_TO_IMPROVE;
		break;
    case MARKAMT:
		usSubpointsPerPoint = MARKSMANSHIP_SUBPOINTS_TO_IMPROVE;
		break;
	case LDRAMT:
		usSubpointsPerPoint = LEADERSHIP_SUBPOINTS_TO_IMPROVE;
		break;

	  // Experience
    case EXPERAMT:
		usSubpointsPerPoint = LEVEL_SUBPOINTS_TO_IMPROVE * bExpLevel;
		break;

    default:
			// BETA message
      ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"SubpointsPerPoint: ERROR - Unknown ubStat %d", ubStat);
      return(100);
  }

	return(usSubpointsPerPoint);
}


// handles stat changes for mercs not currently working for the player
void HandleUnhiredMercImprovement( MERCPROFILESTRUCT *pProfile )
{
	UINT8 ubNumStats;
	UINT8 ubStat;
	UINT16 usNumChances;

	ubNumStats = LAST_CHANGEABLE_STAT - FIRST_CHANGEABLE_STAT + 1;

	// if he's working on another job
	if (pProfile->bMercStatus == MERC_WORKING_ELSEWHERE)
	{
		// Lesh: changed chance from 20 to 70
		// if he did't do anything interesting today
		if (Random(100) < 70)
		{
			// no chance to change today
			return;
		}

		// it's real on the job experience, counts towards experience

		// all stats (including experience itself) get an equal chance to improve
		// 80 wisdom gives 8 rolls per stat per day, 10 stats, avg success rate 40% = 32pts per day,
		// so about 10 working days to hit lvl 2.  This seems high, but mercs don't actually "work" that often, and it's twice
		// as long to hit level 3.  If we go lower, attribs & skills will barely move.
		// Lesh: changed wisdom divided by 7 insead of 10
		usNumChances = ( pProfile->bWisdom / 7 );
		for (ubStat = FIRST_CHANGEABLE_STAT; ubStat <= LAST_CHANGEABLE_STAT; ubStat++)
		{
			ProfileStatChange( pProfile, ubStat, usNumChances, FALSE );
		}
	}
	else
	{
		// if the merc just takes it easy (high level or stupid mercs are more likely to)
		if (((INT8) Random(10) < pProfile->bExpLevel) || ((INT8) Random(100) > pProfile->bWisdom))
		{
			// no chance to change today
			return;
		}

		// it's just practise/training back home
		do
		{
			// pick ONE stat at random to focus on (it may be beyond training cap, but so what, too hard to weed those out)
			ubStat = (UINT8) (FIRST_CHANGEABLE_STAT + Random(ubNumStats));
			// except experience - can't practise that!
		} while (ubStat == EXPERAMT);

		// try to improve that one stat
		ProfileStatChange( pProfile, ubStat, ( UINT16 ) ( pProfile->bWisdom / 2 ), FROM_TRAINING );
	}

	ProfileUpdateStats( pProfile );
}


// handles possible death of mercs not currently working for the player
void HandleUnhiredMercDeaths( INT32 iProfileID )
{
	UINT8 ubMaxDeaths;
	INT16 sChance;
	MERCPROFILESTRUCT *pProfile = &(gMercProfiles[ iProfileID ]);

	// if the player has never yet had the chance to hire this merc
	if ( !( pProfile->ubMiscFlags3 & PROFILE_MISC_FLAG3_PLAYER_HAD_CHANCE_TO_HIRE) )
	{
		// then we're not allowed to kill him (to avoid really pissing off player by killing his very favorite merc)
		return;
	}
	
	ubMaxDeaths = zDiffSetting[gGameOptions.ubDifficultyLevel].iMaxMercDeaths;

	// if we've already hit the limit in this game, skip these checks
	if (gStrategicStatus.ubUnhiredMercDeaths >= ubMaxDeaths)
	{
		return;
	}

	// calculate this merc's (small) chance to get killed today (out of 1000)
	sChance = 10 - pProfile->bExpLevel;

	// SANDRO - certain traits makes us less likely to get killed
	if ( gGameOptions.fNewTraitSystem )
	{	
		if (pProfile->bDisability != NO_DISABILITY)
		{
			sChance += 1;
		}
		if ( ProfileHasSkillTrait( iProfileID, SQUADLEADER_NT ) > 0 )
			sChance -= ProfileHasSkillTrait( iProfileID, SQUADLEADER_NT );
		if ( ProfileHasSkillTrait( iProfileID, TECHNICIAN_NT ) > 0 )
			sChance -= ProfileHasSkillTrait( iProfileID, TECHNICIAN_NT );
		if ( ProfileHasSkillTrait( iProfileID, DOCTOR_NT ) > 0 )
			sChance -= ProfileHasSkillTrait( iProfileID, DOCTOR_NT );
		if ( ProfileHasSkillTrait( iProfileID, STEALTHY_NT ) > 0 )
			sChance -= ProfileHasSkillTrait( iProfileID, STEALTHY_NT );
		if ( ProfileHasSkillTrait( iProfileID, BODYBUILDING_NT ) > 0 )
			sChance -= ProfileHasSkillTrait( iProfileID, BODYBUILDING_NT );
		if ( ProfileHasSkillTrait( iProfileID, SCOUTING_NT ) > 0 )
			sChance -= ProfileHasSkillTrait( iProfileID, SCOUTING_NT );
	}
	else
	{		
		switch (pProfile->bDisability)
		{
			case FORGETFUL:
			case NERVOUS:
			case PSYCHO:
				// these guys are somewhat more likely to get killed (they have "problems")
				sChance += 2;
				break;
		}
		// stealthy guys are slightly less likely to get killed (they're careful)
		if ( ProfileHasSkillTrait( iProfileID, STEALTHY_OT ) > 0 )
			sChance -= ProfileHasSkillTrait( iProfileID, STEALTHY_OT );
	}

	if ((INT16) PreRandom(1000) < sChance)
	{
		// this merc gets Killed In Action!!!
		pProfile->bMercStatus = MERC_IS_DEAD;
		pProfile->uiDayBecomesAvailable = 0;

		// keep count of how many there have been
		gStrategicStatus.ubUnhiredMercDeaths++;

		//send an email as long as the merc is from aim
#ifdef JA2UB
		//ja25 ub	
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTDONE || gGameUBOptions.LaptopQuestEnabled == FALSE )
	{
		if ( gProfilesAIM[ iProfileID ].ProfilId == iProfileID && gGameUBOptions.fDeadMerc == TRUE )  //new profiles by Jazz
			//send an email to the player telling the player that a merc died
			AddEmailWithSpecialData(206, MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH, AIM_SITE, GetWorldTotalMin(), 0, iProfileID, TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT, TYPE_E_AIM_L1 );
	}
#else
	//	if( iProfileID < BIFF )
		if ( gProfilesAIM[ iProfileID ].ProfilId == iProfileID )  //new profiles by Jazz
		{
			//send an email to the player telling the player that a merc died
			AddEmailWithSpecialData(MERC_DIED_ON_OTHER_ASSIGNMENT, MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH, AIM_SITE, GetWorldTotalMin(), 0, iProfileID, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
		}
#endif
	}
}


// These HAVE to total 100% at all times!!!
//#define PROGRESS_PORTION_KILLS	25
//#define PROGRESS_PORTION_CONTROL	25
//#define PROGRESS_PORTION_INCOME	50
//Lalien: Removed due to externalization


// returns a number between 0-100, this is an estimate of how far a player has progressed through the game
UINT8 CurrentPlayerProgressPercentage(void)
{
#ifdef JA2UB
	INT8	bFurthestSectorPlayerOwns=-1; //JA25 UB
	UINT8 ubCurrentProgress;
#else
	
	UINT32 uiCurrentIncome;
	UINT32 uiPossibleIncome;
	UINT16 usCurrentProgress;
 	UINT16 ubKillsPerPoint;
 	UINT16 usKillsProgress;
 	UINT16 usControlProgress;
 	UINT16 usVisitProgress;
	// HEADROCK HAM 3: Added a separate variable for Income Progress, 
	// to enable comparing the results from each progress aspect SEPARATELY.
	UINT16 usIncomeProgress;
	// HEADROCK HAM 3: And another variable to contain the highest result so far.
	UINT16 usHighestProgress;
	// HEADROCK HAM 3: Four variables to hold the maximum attainable progress from each aspect.
	UINT16 usMaxKillsProgress;
	UINT16 usMaxIncomeProgress;
	UINT16 usMaxControlProgress;
	UINT16 usMaxVisitProgress;
#endif

#ifdef JA2UB	
	//Get the furthest sector the player owns
	bFurthestSectorPlayerOwns = GetTheFurthestSectorPlayerOwns();
	//JA25 UB
	switch( bFurthestSectorPlayerOwns )
	{
		//initial sector
		case SEC_H7:
			ubCurrentProgress = 44;
			break;

		case SEC_H8:
			ubCurrentProgress = 45;
			break;

		//guard post
		case SEC_H9:
			ubCurrentProgress = 55;
			break;

		//field
		case SEC_H10:
			ubCurrentProgress = 58;
			break;

		//field
		case SEC_I9:
			ubCurrentProgress = 60;
			break;

		//first part of town
		case SEC_I10:
			ubCurrentProgress = 63;
			break;

		//second part of town
		case SEC_I11:
			ubCurrentProgress = 65;
			break;

		//field
		case SEC_I12:
			ubCurrentProgress = 68;
			break;

		//Abondoned mine
		case SEC_I13:
			ubCurrentProgress = 70;
			break;

		// cave under abondoned mine
/*	case SEC_I13_1:
			ubCurrentProgress = 72;
			break;
*/
		//field
		case SEC_J11:
			ubCurrentProgress = 70;
			break;

		//field
		case SEC_J12:
			ubCurrentProgress = 70;
			break;

		//power gen plant
		case SEC_J13:
			ubCurrentProgress = 75;
			break;
/*
			//power gen plant, sub level
		case JA25_J13_1:
			ubCurrentProgress = 75;
			break;

		//first part of tunnel
		case JA25_J14_1:
			ubCurrentProgress = 80;
			break;

		//second part of tunnel
		case JA25_K14_1:
			ubCurrentProgress = 82;
			break;

		//ground level of complex
		case JA25_K15:
			ubCurrentProgress = 90;
			break;

		//initial sector of complex
		case JA25_K15_1:
			ubCurrentProgress = 85;
			break;

		// 2nd level down of complex
		case JA25_K15_2:
			ubCurrentProgress = 95;
			break;

		//2nd last sector
		case JA25_L15_2:
			ubCurrentProgress = 98;
			break;

		//last sector
		case JA25_L15_3:
			ubCurrentProgress = 100;
			break;
*/
		default:

			// OK, use percentage complete from map...
			//Assert( 0 );
			//ubCurrentProgress = SectorInfo[ bFurthestSectorPlayerOwns ].ubCurrentProgressValue;
			ubCurrentProgress = 50;
			break;
	}

	return(ubCurrentProgress);
#else
	if( gfEditMode )
		return 0;

	// HEADROCK HAM 3: If the alternate progress calculation is used, all four INI settings should be set to 100,
	// otherwise progress cannot ever reach 100...
	if (gGameExternalOptions.fAlternateProgressCalculation)
	{
		usMaxKillsProgress = 100;
		usMaxIncomeProgress = 100;
		usMaxControlProgress = 100;
		usMaxVisitProgress = 100;
	}
	// Else, set to INI-read values. Note that the rest of the function now reads these instead of referring
	// to the long variable names.
	else
	{
		usMaxKillsProgress = gGameExternalOptions.ubGameProgressPortionKills;
		usMaxIncomeProgress = gGameExternalOptions.ubGameProgressPortionIncome;
		usMaxControlProgress = gGameExternalOptions.ubGameProgressPortionControl;
		usMaxVisitProgress = gGameExternalOptions.ubGameProgressPortionVisited;
	}


	// figure out the player's current mine income
	uiCurrentIncome = PredictIncomeFromPlayerMines(FALSE);

	// figure out the player's potential mine income
	uiPossibleIncome = CalcMaxPlayerIncomeFromMines();

	// either of these indicates a critical failure of some sort
	
	// HEADROCK HAM 3.6: No need to assert this. Max Income can potentially be 0 in modded games.
	// Assert(uiPossibleIncome > 0);

	// HEADROCK HAM 3.6: It is now possible, with the help of facilities, 
	// to make more money from mines than normally possible. 
	// This assertion check is now obsolete.
	//Assert(uiCurrentIncome <= uiPossibleIncome); 
	uiCurrentIncome = __min(uiPossibleIncome, uiCurrentIncome);

	// for a rough guess as to how well the player is doing,
	// we'll take the current mine income / potential mine income as a percentage


	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// HEADROCK HAM 3
	// Several changes have been made here to accomodate a new type of progress generation.
	// With this alternate system, the program determines the current progress from each of the
	// four aspects SEPARATELY. It then compares all of them, and only the HIGHEST one determines
	// our current progress. Please note that while some changes have been made, the ORIGINAL
	// progress control works EXACTLY THE SAME AS IT ALWAYS DID.
	//
	/////////////////////////////////////////////////////////////////////////////////////////////

	//Kris:  Make sure you don't divide by zero!!!
	if( uiPossibleIncome > 0)
	{
		usIncomeProgress = (UINT8) ((uiCurrentIncome * usMaxIncomeProgress) / uiPossibleIncome);
	}
	else
	{
		usIncomeProgress = 0;
	}

	// kills per point depends on difficulty, and should match the ratios of starting enemy populations (730/1050/1500)
	// HEADROCK HAM 3: Externalized all four Kills-per-point ratios.
	ubKillsPerPoint = zDiffSetting[gGameOptions.ubDifficultyLevel].iNumKillsPerProgressPoint;

	usKillsProgress = gStrategicStatus.usPlayerKills / ubKillsPerPoint;
	if (usKillsProgress > usMaxKillsProgress)
	{
		usKillsProgress = usMaxKillsProgress;
	}

	// 19 sectors in mining towns + 3 wilderness SAMs each count double.  Balime & Meduna are extra and not required
	// HEADROCK HAM B1: Changed the next line, adding a call to a new function. This allows the weight of Sector
	// Control to be altered in JA2_OPTIONS.INI (Previously damaged the game's progress if set over 25... So I've 
	// made this MOD-Friendly :D )
	// BTW, Balime and Meduna _ARE_ required. The function doesn't differentiate! Such carelessness. Tsk tsk tsk.

	usControlProgress = usMaxControlProgress * CalcImportantSectorControl() / CalcTotalImportantSectors();
	if (usControlProgress > usMaxControlProgress)
	{
		usControlProgress = usMaxControlProgress;
	}

	// WDS: Adding more ways to progress in the game
	// Get a ratio of sectors visited to the total number of sectors
	// HEADROCK HAM B1: Fixed this so it doesn't count sectors that can't be visited. Allows progress to go to
	// 100 even if the map has some unvisitable sectors (heh, doesn't it always?)
	usVisitProgress = CountSurfaceSectorsVisited() * usMaxVisitProgress / TotalVisitableSurfaceSectors();

	// HEADROCK HAM 3: This bit is ugly for now, unless someone can optimize it for me. 
	// When the "Alternate Progress Calculation" is activated, the program selects only the HIGHEST of the 
	// progress controls, and sets that to be the current progress, disregarding any advances in the three
	// other fields.

	if (gGameExternalOptions.fAlternateProgressCalculation)
	{
		usHighestProgress = __max(usKillsProgress, usControlProgress);
		usHighestProgress = __max(usHighestProgress, usIncomeProgress);
		usHighestProgress = __max(usHighestProgress, usVisitProgress);
		usCurrentProgress = usHighestProgress;
	}
	// Else, add them all up as normal (original progress calculation). This replaces all those lines I removed
	// above where progress was summed along the way.
	else
	{
		usCurrentProgress = usKillsProgress + usControlProgress + usIncomeProgress + usVisitProgress;
	}

	// Add a static amount of points, as declared in the INI file.
	// HEADROCK HAM 3.6: Allow negative modifiers!
	usCurrentProgress = __max(0, (INT16)(usCurrentProgress + gGameExternalOptions.bGameProgressModifier));

	// And failsafes here. I'm not 100% sure about these though: I've never personally seen progress 
	// values go over 100, and I don't think they SHOULD... Can the game handle values > 100? Should it?
	usCurrentProgress = __min(100, usCurrentProgress);
	usCurrentProgress = __max(0, usCurrentProgress);
	// No less than 0, or the minimum set in the INI file.
	UINT32 uiMinProgress = __max(gGameExternalOptions.ubGameProgressMinimum, 0);
	usCurrentProgress = __max(uiMinProgress, usCurrentProgress);


	return((UINT8)usCurrentProgress);
	
#endif
}

UINT8 HighestPlayerProgressPercentage(void)
{
	if( gfEditMode )
		return 0;

	return(gStrategicStatus.ubHighestProgress);
}

// monitors the highest level of progress that player has achieved so far (checking hourly),
// as opposed to his immediate situation (which may be worse if he's suffered a setback).
void HourlyProgressUpdate(void)
{
	UINT8 ubCurrentProgress;

	ubCurrentProgress = CurrentPlayerProgressPercentage();

	// if this is new high, remember it as that
	if (ubCurrentProgress > gStrategicStatus.ubHighestProgress)
	{
		// CJC:  note when progress goes above certain values for the first time

		// at 35% start the Madlab quest
#ifdef JA2UB
// no UB
#else
		if ( ubCurrentProgress >= gGameExternalOptions.ubGameProgressStartMadlabQuest && gStrategicStatus.ubHighestProgress < gGameExternalOptions.ubGameProgressStartMadlabQuest )
		{
			HandleScientistAWOLMeanwhileScene();
		}
#endif
		// at 50% make Mike available to the strategic AI
		if ( ubCurrentProgress >= gGameExternalOptions.ubGameProgressMikeAvailable && gStrategicStatus.ubHighestProgress < gGameExternalOptions.ubGameProgressMikeAvailable )
		{
			if( gubFact[FACT_MIKE_AVAILABLE_TO_ARMY] < 2 )
			{
				SetFactTrue( FACT_MIKE_AVAILABLE_TO_ARMY );
			}
		}

		// at 70% add Iggy to the world
		if ( ubCurrentProgress >= gGameExternalOptions.ubGameProgressIggyAvaliable && gStrategicStatus.ubHighestProgress < gGameExternalOptions.ubGameProgressIggyAvaliable )
		{
			if ( gubFact[ FACT_IGGY_AVAILABLE_TO_ARMY ] < 2 )
			{
				gMercProfiles[ IGGY ].sSectorX = gModSettings.ubAddIggySectorX; //5
				gMercProfiles[ IGGY ].sSectorY = gModSettings.ubAddIggySectorY; //MAP_ROW_C
				gMercProfiles[ IGGY ].bSectorZ = gModSettings.ubAddIggySectorZ; //0
			}
		}

		// Flugente: on certain progress levels, the queen decides to initiate major attacks
		if ( ubCurrentProgress >= gGameExternalOptions.ubGameProgressOffensiveStage1 && gStrategicStatus.ubHighestProgress < gGameExternalOptions.ubGameProgressOffensiveStage1 )
		{
			if ( !CheckFact( FACT_GLOBAL_OFFENSIVE_1_ORDERED , 0) )
				ExecuteStrategicAIAction( NPC_ACTION_GLOBAL_OFFENSIVE_1, 0, 0 );
		}

		if ( ubCurrentProgress >= gGameExternalOptions.ubGameProgressOffensiveStage2 && gStrategicStatus.ubHighestProgress < gGameExternalOptions.ubGameProgressOffensiveStage2 )
		{
			if ( !CheckFact( FACT_GLOBAL_OFFENSIVE_2_ORDERED, 0 ) )
				ExecuteStrategicAIAction( NPC_ACTION_GLOBAL_OFFENSIVE_2, 0, 0 );
		}

		gStrategicStatus.ubHighestProgress = ubCurrentProgress;

		// debug message
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"New player progress record: %d%%", gStrategicStatus.ubHighestProgress );
	}
}



#ifdef JA2TESTVERSION
void TestDumpStatChanges(void)
{
  UINT32 uiProfileId;
	UINT8 ubStat;
  CHAR8 zPrintFileName[60];
  FILE *FDump;
  MERCPROFILESTRUCT *pProfile;
	BOOLEAN fMercUsed;
	CHAR8 cEvolutionChars[3] = { '+', '=', '-' };
	UINT32 uiTotalSuccesses[ 12 ];
	UINT32 uiTotalChances[ 12 ];


	// clear totals
	memset( uiTotalSuccesses, 0, sizeof( uiTotalSuccesses ) );
	memset( uiTotalChances,   0, sizeof( uiTotalChances ) );

  // open output file
 	strcpy(zPrintFileName, "C:\\Temp\\StatChanges.TXT");
  FDump = fopen(zPrintFileName, "wt");

  if (FDump == NULL)
    return;


	// print headings
	fprintf(FDump, "   NAME   SRV EVL ");
	fprintf(FDump, "---HEALTH-- --AGILITY-- -DEXTERITY- ---WISDOM-- --MEDICAL-- --EXPLOSIV- --MECHANIC- --MARKSMAN- -EXP.LEVEL- --STRENGTH- -LEADERSHIP");
	fprintf(FDump, "\n");


  // loop through profiles
	for (uiProfileId = 0; uiProfileId < NUM_PROFILES; uiProfileId++)
  {
    pProfile = &(gMercProfiles[uiProfileId]);

		fMercUsed = FALSE;

		// see if this guy should be printed at all (only mercs actually used are dumped)
		for( ubStat = FIRST_CHANGEABLE_STAT; ubStat <= LAST_CHANGEABLE_STAT; ubStat++ )
		{
			if ( pProfile->usStatChangeChances[ ubStat ] > 0 )
			{
				fMercUsed = TRUE;
				break;
			}
    }

		if (fMercUsed)
		{
			// print nickname
			fprintf(FDump, "%-10ls ", pProfile->zNickname);
			// print days served
			fprintf(FDump, "%3d ", pProfile->usTotalDaysServed);
			// print evolution type
			fprintf(FDump, "%c ", cEvolutionChars[ pProfile->bEvolution ]);

			// now print all non-zero stats
			for( ubStat = FIRST_CHANGEABLE_STAT; ubStat <= LAST_CHANGEABLE_STAT; ubStat++ )
			{
				if ( pProfile->usStatChangeChances[ ubStat ] > 0 )
				{
					// print successes/chances
					fprintf(FDump, " %5d/%-5d", pProfile->usStatChangeSuccesses[ ubStat ], pProfile->usStatChangeChances[ ubStat ]);
				}
				else
				{
					//
					fprintf(FDump, "            ");
				}

				uiTotalSuccesses[ ubStat ] += pProfile->usStatChangeSuccesses[ ubStat ];
				uiTotalChances[ ubStat ]   += pProfile->usStatChangeChances[ ubStat ];
			}

			// newline
			fprintf(FDump, "\n");
		}
  }


	// print totals:
	fprintf(FDump, "TOTAL        ");

	for( ubStat = FIRST_CHANGEABLE_STAT; ubStat <= LAST_CHANGEABLE_STAT; ubStat++ )
	{
		fprintf(FDump, " %5d/%-5d", uiTotalSuccesses[ ubStat ], uiTotalChances[ ubStat ]);
	}

	fprintf(FDump, "\n");


  fclose(FDump);
}
#endif



void AwardExperienceBonusToActiveSquad( UINT8 ubExpBonusType )
{
	UINT16 usXPs = 0;
	UINT8 ubGuynum;
	SOLDIERTYPE *pSoldier;


	Assert ( ubExpBonusType < NUM_EXP_BONUS_TYPES );

	switch ( ubExpBonusType )
	{
		case EXP_BONUS_MINIMUM:		usXPs =   25;			break;
		case EXP_BONUS_SMALL:			usXPs =   50;			break;
		case EXP_BONUS_AVERAGE:		usXPs =  100;			break;
		case EXP_BONUS_LARGE:			usXPs =  200;			break;
		case EXP_BONUS_MAXIMUM:		usXPs =  400;			break;
	}

	// to do: find guys in sector on the currently active squad, those that are conscious get this amount in XPs
	for ( ubGuynum = gTacticalStatus.Team[ gbPlayerNum ].bFirstID, pSoldier = MercPtrs[ ubGuynum ];
				ubGuynum <= gTacticalStatus.Team[ gbPlayerNum ].bLastID;
				ubGuynum++, pSoldier++ )
	{
		if ( pSoldier->bActive && pSoldier->bInSector && IsMercOnCurrentSquad( pSoldier ) && ( pSoldier->stats.bLife >= CONSCIOUSNESS ) &&
				 !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) )
		{
			StatChange( pSoldier, EXPERAMT, usXPs, FALSE );
		}
	}
}



void BuildStatChangeString( STR16 wString, STR16 wName, BOOLEAN fIncrease, INT16 sPtsChanged, UINT8 ubStat )
{
	UINT8 ubStringIndex;


	Assert( sPtsChanged != 0 );
	Assert( ubStat >= FIRST_CHANGEABLE_STAT );
	Assert( ubStat <= LAST_CHANGEABLE_STAT );

	// if just a 1 point change
	if ( abs( sPtsChanged ) == 1 )
	{
		// use singular
		ubStringIndex = 2;
	}
	else
	{
		ubStringIndex = 3;
		// use plural
	}

	if ( ubStat == EXPERAMT )
	{
		// use "level/levels instead of point/points
		ubStringIndex += 2;
	}

	swprintf( wString, L"%s %s %d %s %s", wName, sPreStatBuildString[ fIncrease ? 1 : 0 ], abs( sPtsChanged ),
					sPreStatBuildString[ ubStringIndex ], sStatGainStrings[ ubStat - FIRST_CHANGEABLE_STAT ] );
}



UINT8 CalcImportantSectorControl( void )
{
	UINT8 ubMapX, ubMapY;
	UINT8	ubSectorControlPts = 0;


	for ( ubMapX = 1; ubMapX < MAP_WORLD_X - 1; ubMapX++ )
	{
		for ( ubMapY = 1; ubMapY < MAP_WORLD_Y - 1; ubMapY++ )
		{
			// if player controlled
			if ( StrategicMap[ CALCULATE_STRATEGIC_INDEX( ubMapX, ubMapY ) ].fEnemyControlled == FALSE )
			{
				// towns where militia can be trained and SAM sites are important sectors
				if ( MilitiaTrainingAllowedInSector( ubMapX, ubMapY, 0 ) )
				{
					ubSectorControlPts++;

					// SAM sites count double - they have no income, but have significant air control value
					if ( IsThisSectorASAMSector( ubMapX, ubMapY, 0 ) )
					{
						ubSectorControlPts++;
					}
				}
			}
		}
	}

	return( ubSectorControlPts );
}

// HEADROCK HAM B1: function to calculate how many "important sectors" there actually are in the game. This is
// used to fix the weight of strategic control importance on game progress, which was until now not really 
// moddable.

UINT8 CalcTotalImportantSectors( void )
{
	UINT8 ubMapX, ubMapY;
	UINT8	ubSectorControlPts = 0;


	for ( ubMapX = 1; ubMapX < MAP_WORLD_X - 1; ubMapX++ )
	{
		for ( ubMapY = 1; ubMapY < MAP_WORLD_Y - 1; ubMapY++ )
		{
			// towns where militia can be trained and SAM sites are important sectors
			if ( MilitiaTrainingAllowedInSector( ubMapX, ubMapY, 0 ) )
			{
				ubSectorControlPts++;

				// SAM sites count double - they have no income, but have significant air control value
				if ( IsThisSectorASAMSector( ubMapX, ubMapY, 0 ) )
				{
					ubSectorControlPts++;
				}
			}
		}
	}

	return( ubSectorControlPts );
}

// WDS: Adding more ways to progress in the game
// Count how many surface sectors the player has visited
UINT16 CountSurfaceSectorsVisited( void )
{
	UINT8 ubMapX, ubMapY;
	UINT16	ubSectorsVisited = 0;


	for ( ubMapX = 1; ubMapX < MAP_WORLD_X - 1; ubMapX++ )
	{
		for ( ubMapY = 1; ubMapY < MAP_WORLD_Y - 1; ubMapY++ )
		{
			if( GetSectorFlagStatus( ubMapX, ubMapY, 0, SF_ALREADY_VISITED ) == TRUE )
				++ubSectorsVisited;
		}
	}

	return( ubSectorsVisited );
}

// HEADROCK: Count number of VISITABLE sectors
UINT16 TotalVisitableSurfaceSectors( void )
{
	UINT8 ubMapX, ubMapY;
	UINT16	ubVisitableSectors = 0;
	
	for ( ubMapX = 1; ubMapX < MAP_WORLD_X - 1; ubMapX++ )
	{
		for ( ubMapY = 1; ubMapY < MAP_WORLD_Y - 1; ubMapY++ )
		{
			if ( !SectorIsImpassable( SECTOR( ubMapX, ubMapY ) ) )
				++ubVisitableSectors;
		}
	}

	return( ubVisitableSectors );
}

void MERCMercWentUpALevelSendEmail( UINT8 ubMercMercIdValue )
{
#ifdef JA2UB
//JA25 UB
#else
	UINT8 ubEmailOffset = 0;
	int iMsgLength = 0;
	
	UINT8 pMerc = 0;
	UINT8 iMerc = 0;
	UINT8 oMerc = 0;
	
	// Read from EmailMercAvailable.xml
	if ( ReadXMLEmail == TRUE )
	{		
	oMerc = ubMercMercIdValue;				
	iMerc = oMerc * 1;
	
	if ( oMerc != 0 )
		pMerc = oMerc + 1;
	else
		pMerc = 0;
	if ( gProfilesMERC[ubMercMercIdValue].ProfilId == ubMercMercIdValue )
		if( IsSpeckComAvailable() )// anv: only send level up email if Speck is available at website
			AddEmailTypeXML( pMerc, iMerc, iMerc, GetWorldTotalMin(), -1 , TYPE_EMAIL_MERC_LEVEL_UP);
	}
	else
	{
	// Read from Email.edt and sender (nickname) from MercProfiles.xml
	// WANNE: TODO: Tex, Biggins, Stoggy and Gaston have special handling because they are the new MERC merc in 1.13
	// There is no letter template in Email.edt. We have them hardcoded in the source code.
	if (ubMercMercIdValue == 124 || ubMercMercIdValue == 125 || ubMercMercIdValue == 126 || ubMercMercIdValue == 127)
	{
		// Gaston
		if (ubMercMercIdValue == 124)
		{
			ubEmailOffset = MERC_UP_LEVEL_BIFF;
			iMsgLength = MERC_UP_LEVEL_GASTON;
		}
		// Stogie
		else if (ubMercMercIdValue == 125)
		{
			ubEmailOffset = MERC_UP_LEVEL_BIFF;
			iMsgLength = MERC_UP_LEVEL_STOGIE;
		}
		// Tex
		else if (ubMercMercIdValue == 126)
		{
			ubEmailOffset = MERC_UP_LEVEL_BIFF;
			iMsgLength = MERC_UP_LEVEL_TEX;
		}
		// Biggens
		else if (ubMercMercIdValue == 127)
		{
			ubEmailOffset = MERC_UP_LEVEL_BIFF;
			iMsgLength = MERC_UP_LEVEL_BIGGENS;
		}
	}
	else
	{
		iMsgLength = MERC_UP_LEVEL_LENGTH_BIFF;
		ubEmailOffset = MERC_UP_LEVEL_BIFF + MERC_UP_LEVEL_LENGTH_BIFF * ( ubMercMercIdValue ); 
	}

	AddEmail( ubEmailOffset, iMsgLength, SPECK_FROM_MERC, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT_NAME_MERC);
	
	}
#endif
}
