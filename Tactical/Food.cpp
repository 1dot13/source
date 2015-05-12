#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <math.h>
	#include "sgp.h"
	#include "soldier profile.h"
	#include "Food.h"
	#include "items.h"
	#include "morale.h"
	#include "points.h"
	#include "message.h"
	#include "GameSettings.h" // SANDRO - had to add this, dammit!
	#include "Random.h"
	#include "Text.h"
	#include "Interface.h"
	#include "Dialogue Control.h"
	#include "Sound Control.h"
	#include "Assignments.h"
	#include "Overhead.h"
	#include "Isometric Utils.h"
	#include "Campaign Types.h"
	#include "Drugs And Alcohol.h"
	#include "environment.h"
	#include "WorldDat.h"
	#include "Facilities.h"
	#include "Soldier macros.h"
	#include "strategicmap.h"
	#include "DynamicDialogue.h"			// added by Flugente
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

FOODOPINIONS FoodOpinions[NUM_PROFILES];

extern MoraleEvent gbMoraleEvent[NUM_MORALE_EVENTS];

extern BOOLEAN GetSectorFlagStatus( INT16 sMapX, INT16 sMapY, UINT8 bMapZ, UINT32 uiFlagToSet );
extern BOOLEAN IsVehicle(SOLDIERTYPE *pSoldier);

extern SECTOR_EXT_DATA	SectorExternalData[256][4];

#define FOOD_BAD_THRESHOLD							0.5f		// must be > 0 !!!!!

#define FOOD_MORALE_DRINK_TO_FOOD_RATIO				1.0f
#define FOOD_FACILITY_WATER_FACTOR					3.0f		// in a facility that serves food, only the food value is specified in the xml. drink value is food value multiplied by this

#define FOOD_POW_MULTIPLICATOR						1.2f		// multiplicator to the water we get when POWs (so many times our water digestion rate, so we will barely survive)
#define FOOD_WATER_POISONOUS_TEMPERATURE			20000.0f	// termperature value of water taken out of swamps etc.

// these midifiers are applied separately for both food and water
// apart from the ubStatDamageChance values, be careful not to set any modifiers below -50 or above 0 unless you know what you are doing!!!
FoodMoraleMod FoodMoraleMods[NUM_FOOD_MORALE_TYPES] =
{
	{ 100000,	-8,		+3,		-20,	-35,	2},		//	FOOD_STUFFED
	{ 7500,		-5,		+2,		-5,		-5,		0},		//	FOOD_EXTREMELY_FULL
	{ 5000,		0,		+1,		-2,		-2,		0},		//	FOOD_VERY_FULL
	{ 2500,		2,		+1,		-1,		0,		0},		//	FOOD_FULL
	{ 1000,		5,		0,		0,		0,		0},		//	FOOD_SLIGHTLY_FULL

	{ 0,		0,		0,		0,		0,		0},		//	FOOD_NORMAL

	{ -1000,	0,		0,		0,		0,		0},		//	FOOD_LOW
	{ -2500,	-2,		0,		0,		0,		0},		//	FOOD_EVEN_LOWER
	{ -5000,	-8,		+1,		-10,	-18,	5},		//	FOOD_VERY_LOW
	{ -7500,	-15,	+2,		-20,	-15,	25},	//	FOOD_DANGER
	{ -8750,	-25,	+2,		-35,	-30,	75},	//	FOOD_DESPERATE
	{ -10000,	-40,	+3,		-45,	-45,	100},	//	FOOD_STARVING
};

// Flugente: LootFragg had a nice idea: food intake should be non-linear.
// The idea is that for a starving person, a piece of food raises the food percentage more than it would for somebody overfed.
// I thus created the 'food integral'. The points we get from food depend on the situation we are in when we eat something
// As a result, we dont have to stuff unholy amounts of food in when we ar elow on food, but overfeeding gets harder.
// It is thus more reasonable to eat when you are hungry instead of stuffing like a bear before hibernation.

FLOAT FoodEfficiency_Primitive( FLOAT aVal )
{
	return aVal - aVal*aVal/FOOD_HALF_RANGE + aVal*aVal*aVal/(3*FOOD_HALF_RANGE*FOOD_HALF_RANGE);
}

FLOAT FoodIntegral( INT32 a, INT32 b )
{
	if ( a >= b )
		return 0.0;

	if ( a < FOOD_MIN ) a = FOOD_MIN;
	if ( b > FOOD_MAX ) b = FOOD_MAX;

	return ( FoodEfficiency_Primitive( (FLOAT)b ) - FoodEfficiency_Primitive( (FLOAT)a ) );
}

void AddFoodpoints( INT32& arCurrentFood, INT32 aVal )
{
	// the amount of points we get depends on our current food situation and the amount of food points
	INT32 intake = FoodIntegral( arCurrentFood, arCurrentFood + aVal );

	arCurrentFood = min(arCurrentFood + intake, FOOD_MAX);
	arCurrentFood = max(arCurrentFood, FOOD_MIN);
}

BOOLEAN ApplyFood( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject, BOOLEAN fForce, BOOLEAN fForceFromDrugs )
{
	// static variables to remember the last food someone was forced to eat
	static UINT8 lasteater = 0;
	static UINT16 lastitem = 0;

	// how did this even happen?
	if ( !pSoldier || !pObject || !(pObject->exists() ) || (*pObject)[0]->data.objectStatus < 1 )
		return( FALSE);

	// dont feed our machines
	if ( pSoldier->ubProfile == ROBOT || IsVehicle(pSoldier) )
		return( FALSE);

	UINT32 foodtype = Item[pObject->usItem].foodtype;

	// if not a food item, nothing to see here
	if ( foodtype == 0 || foodtype > FOOD_TYPE_MAX )
		return( FALSE);
		
	// workaround: canteens with 1% status are treated as 'empty'. They cannot be consumed, but refilled
	if ( Item[pObject->usItem].canteen == TRUE && (*pObject)[0]->data.objectStatus == 1 )
		return( FALSE);

	// do we eat or drink this stuff?
	UINT8 type = AP_EAT;
	if ( Food[foodtype].bDrinkPoints > Food[foodtype].bFoodPoints )
		type = AP_DRINK;

	// return if we don't have enough APs
	if ( !fForceFromDrugs && !EnoughPoints( pSoldier, APBPConstants[type], 0, TRUE ) )
	{
		return( FALSE );
	}
	 
	// check if we are willing to eat this: if we're filled, the merc refuses
	if ( !fForceFromDrugs && ( ( Food[foodtype].bFoodPoints > 0 && pSoldier->bFoodLevel > FoodMoraleMods[FOOD_MERC_REFUSAL].bThreshold ) || Food[foodtype].bFoodPoints <= 0 ) && ( ( Food[foodtype].bDrinkPoints > 0 && pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_MERC_REFUSAL].bThreshold ) || Food[foodtype].bDrinkPoints <= 0 ) )
	{
		// Say quote!
		TacticalCharacterDialogue( pSoldier, 61 );

		return( FALSE);
	}

	// we have to determine wether the food is rotten, that might influence its condition and poison us
	FLOAT foodcondition = (*pObject)[0]->data.bTemperature / OVERHEATING_MAX_TEMPERATURE;
	
	// food in bad condition is harmful!
	if ( !fForceFromDrugs && foodcondition < FOOD_BAD_THRESHOLD )
	{
		// if we can choose to reject a food and haven't yet done so with this type of bad food, do so. Works only once
		if ( !fForce && ( lasteater != pSoldier->ubID || lastitem != pObject->usItem) )
		{
			lasteater = pSoldier->ubID;
			lastitem = pObject->usItem;

			// notification
			if ( type == AP_EAT )
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_DONOTWANT_EAT], pSoldier->GetName(), Item[pObject->usItem].szItemName );
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_DONOTWANT_DRINK], pSoldier->GetName(), Item[pObject->usItem].szItemName );

			// Say quote!
			TacticalCharacterDialogue( pSoldier, 61 );

			return( FALSE);
		}

		// determine the max nutritional value
		INT32 maxpts = max(Food[foodtype].bFoodPoints, Food[foodtype].bDrinkPoints);

		// divide by 100 (poison sum is in [0, 99]
		// multiply with 0.25 for more reasonable values
		INT8 poisonadd = (INT8)(maxpts * (1.0 - foodcondition) * 0.025 );

		// added a max threshold to food poisoning
		poisonadd = min(poisonadd, gGameExternalOptions.usFoodMaxPoisoning),

		pSoldier->AddPoison(poisonadd);

		// we might get a disease from this...
		FLOAT modifier = 1.0f - 2 * foodcondition;
		HandlePossibleInfection( pSoldier, NULL, type == AP_EAT ? INFECTION_TYPE_BADFOOD : INFECTION_TYPE_BADWATER, modifier );
	}

	FLOAT conditionmodifier = 0.5f * (1.0f + sqrt(foodcondition));

	FLOAT percentualsize = min(Food[foodtype].ubPortionSize, (*pObject)[0]->data.objectStatus) / 100.0f;

	INT32 foodpts  = (INT32) (Food[foodtype].bFoodPoints  * percentualsize * conditionmodifier );
	INT32 drinkpts = (INT32) (Food[foodtype].bDrinkPoints * percentualsize * conditionmodifier );

	// eat it!
	AddFoodpoints(pSoldier->bFoodLevel, foodpts);
	AddFoodpoints(pSoldier->bDrinkLevel, drinkpts);

	/////////////////// MORALE //////////////////////
	// morale can now be defined for each profile and food individually - default value is always 0
	INT8 moralemod = 0;
	if ( pSoldier->ubProfile != NO_PROFILE )
		moralemod = FoodOpinions[pSoldier->ubProfile].sFoodOpinion[foodtype];

	// if food is rotting, we get a morale penalty
	if ( foodcondition < FOOD_BAD_THRESHOLD )
		moralemod -= (FOOD_BAD_THRESHOLD - foodcondition) * 10;
	
	if ( moralemod > 0 )
	{
		while ( moralemod > 0 && moralemod >= gMoraleSettings.bValues[MORALE_GOOD_FOOD] )
		{
			HandleMoraleEvent( pSoldier, MORALE_GOOD_FOOD, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
			moralemod -= gMoraleSettings.bValues[MORALE_GOOD_FOOD];
		}

		while ( moralemod > 0 && moralemod >= gMoraleSettings.bValues[MORALE_FOOD] )
		{
			HandleMoraleEvent( pSoldier, MORALE_FOOD, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
			moralemod -= gMoraleSettings.bValues[MORALE_FOOD];
		}
	}
	else if ( moralemod < 0 )
	{
		while ( moralemod < 0 && moralemod <= gMoraleSettings.bValues[MORALE_LOATHSOME_FOOD] )
		{
			HandleMoraleEvent( pSoldier, MORALE_LOATHSOME_FOOD, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
			moralemod -= gMoraleSettings.bValues[MORALE_LOATHSOME_FOOD];
		}

		while ( moralemod < 0 && moralemod <= gMoraleSettings.bValues[MORALE_BAD_FOOD] )
		{
			HandleMoraleEvent( pSoldier, MORALE_BAD_FOOD, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
			moralemod -= gMoraleSettings.bValues[MORALE_BAD_FOOD];
		}
	}
	/////////////////// MORALE //////////////////////

	// notification
	if ( type == AP_EAT )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_ATE], pSoldier->GetName(), Item[pObject->usItem].szItemName );
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_DRANK], pSoldier->GetName(), Item[pObject->usItem].szItemName );

	// Flugente: if this guy has the disease, infect object
	if ( pSoldier->sDiseasePoints[0] > 0 )
		(*pObject)[0]->data.sObjectFlag |= INFECTED;
	// if object is infected, infect the victim
	else if ( (*pObject)[0]->data.sObjectFlag & INFECTED && gGameExternalOptions.fDiseaseContaminatesItems )
		pSoldier->Infect( 0 );
	
	// now remove a portion of the food item (or the whole item altogether)
	UINT16 ptsconsumed = UseKitPoints( pObject, Food[foodtype].ubPortionSize, pSoldier );

	INT32 sBPAdjustment = 0;
	// if the food is more of a drink, we also restore breath points
	if ( Food[foodtype].bDrinkPoints > Food[foodtype].bFoodPoints )
	{
		sBPAdjustment = 2 * ptsconsumed * -(100 - pSoldier->bBreath);
	}

	DeductPoints( pSoldier, APBPConstants[type], sBPAdjustment );

	// let it be known that we are eating
	if ( pSoldier->bTeam == gbPlayerNum && gGameExternalOptions.fFoodEatingSounds )
	{
		if ( type == AP_EAT )
		{
			// Play sound
			PlayJA2SampleFromFile( "Sounds\\eat1.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		}
		else
		{
			if ( gMercProfiles[ pSoldier->ubProfile ].bSex == MALE )
			{
				  PlayJA2Sample( DRINK_CANTEEN_MALE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
			}
			else
			{
				  PlayJA2Sample( DRINK_CANTEEN_FEMALE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
			}
		}
	}

	return( TRUE );
}

void GetFoodSituation( SOLDIERTYPE *pSoldier, UINT8* pFoodSituation, UINT8* pWaterSituation )
{
	*pFoodSituation = FOOD_NORMAL;
	*pWaterSituation = FOOD_NORMAL;

	if ( !pSoldier )
		return;

	for ( UINT8 i = FOOD_STUFFED; i < NUM_FOOD_MORALE_TYPES; ++i )
	{
		if ( pSoldier->bFoodLevel <= FoodMoraleMods[i].bThreshold )
			*pFoodSituation = i;

		if ( pSoldier->bDrinkLevel <= FoodMoraleMods[i].bThreshold )
			*pWaterSituation = i;
	}
}

void FoodMaxMoraleModifiy( SOLDIERTYPE *pSoldier, UINT8* pubMaxMorale )
{
	if ( !pSoldier )
		return;

	UINT8 foodsituation;
	UINT8 watersituation;
	GetFoodSituation( pSoldier, &foodsituation, &watersituation );

	INT8 foodmod  = FoodMoraleMods[foodsituation].bMoraleModifier;
	INT8 watermod = FoodMoraleMods[watersituation].bMoraleModifier;

	(*pubMaxMorale) = max(1, (*pubMaxMorale) + foodmod + watermod);
}

void FoodNeedForSleepModifiy( SOLDIERTYPE *pSoldier, UINT8* pubNeedForSleep )
{
	if ( !pSoldier )
		return;

	UINT8 foodsituation;
	UINT8 watersituation;
	GetFoodSituation( pSoldier, &foodsituation, &watersituation );
	
	(*pubNeedForSleep) = max(1, (INT16)((*pubNeedForSleep) + FoodMoraleMods[foodsituation].bSleepModifier + FoodMoraleMods[watersituation].bSleepModifier ));
}

void ReducePointsForHunger( SOLDIERTYPE *pSoldier, UINT32 *pusPoints )
{
	UINT8 foodsituation;
	UINT8 watersituation;
	GetFoodSituation( pSoldier, &foodsituation, &watersituation );

	INT8 foodmod  = FoodMoraleMods[foodsituation].bAssignmentEfficiencyModifier;
	INT8 watermod = FoodMoraleMods[watersituation].bAssignmentEfficiencyModifier;

	*pusPoints = (UINT32)((*pusPoints) * (100 + foodmod + watermod)/100);
}

void ReduceBPRegenForHunger( SOLDIERTYPE *pSoldier, INT32 *psPoints )
{
	UINT8 foodsituation;
	UINT8 watersituation;
	GetFoodSituation( pSoldier, &foodsituation, &watersituation );

	INT8 foodmod  = FoodMoraleMods[foodsituation].bBreathRegenModifier;
	INT8 watermod = FoodMoraleMods[watersituation].bBreathRegenModifier;

	*psPoints = (INT32)((*psPoints) * (100 + foodmod + watermod)/100);
}

void HourlyFoodSituationUpdate( SOLDIERTYPE *pSoldier )
{
	if ( !pSoldier )
		return;

	// determine our current activity level
	FLOAT activitymodifier = gGameExternalOptions.sFoodDigestionOnDuty;
	if ( pSoldier->flags.fMercAsleep == TRUE )
		activitymodifier = gGameExternalOptions.sFoodDigestionSleep;
	else if ( !pSoldier->bInSector )
	{
		if( pSoldier->bAssignment == VEHICLE )
			activitymodifier = gGameExternalOptions.sFoodDigestionTravelVehicle;
		else
			activitymodifier = gGameExternalOptions.sFoodDigestionTravel;
	}
	else if ( pSoldier->bAssignment > DOCTOR )
		activitymodifier = gGameExternalOptions.sFoodDigestionAssignment;
	else if ( (gTacticalStatus.uiFlags & INCOMBAT) )
		activitymodifier = gGameExternalOptions.sFoodDigestionCombat;
		
	// for some odd reason, the time isn't even needed here, so we just use 0 :-)
	INT8 sectortemperaturemod = SectorTemperature( 0, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );

	// if we are heat intolerant, increase modifier
	if ( sectortemperaturemod > 0 && (gMercProfiles[ pSoldier->ubProfile ].bDisability == HEAT_INTOLERANT || MercUnderTheInfluence(pSoldier, DRUG_TYPE_HEATINTOLERANT) ) )
		++sectortemperaturemod;

	FLOAT  temperaturemodifier  = (FLOAT)(3 + sectortemperaturemod)/3;
	
	FLOAT specialfoodmodifier  = 100.0 + pSoldier->GetBackgroundValue( BG_PERC_FOOD );
	FLOAT specialdrinkmodifier = 100.0 + pSoldier->GetBackgroundValue( BG_PERC_WATER );

	if ( HAS_SKILL_TRAIT( pSoldier, SURVIVAL_NT ) )
	{
		specialfoodmodifier  += gSkillTraitValues.sSVFoodConsumption;
		specialdrinkmodifier += gSkillTraitValues.sSVDrinkConsumption;
	}

	for ( int i = 0; i < NUM_DISEASES; ++i )
	{
		specialfoodmodifier  += Disease[i].sFoodModifier  * pSoldier->GetDiseaseMagnitude( i );
		specialdrinkmodifier += Disease[i].sDrinkModifier * pSoldier->GetDiseaseMagnitude( i );
	}

	specialfoodmodifier /= 100.0;
	specialdrinkmodifier /= 100.0;
	
	// due to digestion, reduce our food and drink levels
	pSoldier->bFoodLevel  = max(pSoldier->bFoodLevel  - (INT32) (specialfoodmodifier  * activitymodifier * gGameExternalOptions.usFoodDigestionHourlyBaseFood), FOOD_MIN);
	pSoldier->bDrinkLevel = max(pSoldier->bDrinkLevel - (INT32) (specialdrinkmodifier * activitymodifier * temperaturemodifier * gGameExternalOptions.usFoodDigestionHourlyBaseDrink), FOOD_MIN);

	// there is a chance that we take damage to our health and strength stats if we are starving (or insanely obese :-) )
	UINT8 foodsituation;
	UINT8 watersituation;
	GetFoodSituation( pSoldier, &foodsituation, &watersituation );

	// we do this separately. first for food
	UINT8 statdamagechance = FoodMoraleMods[foodsituation].ubStatDamageChance;
	
	if ( statdamagechance > 0 )
	{
		// these reductions can be healed, but only if we are in a sufficient food situation again
		// damage strength
		if ( Random(100) < statdamagechance )
		{
			UINT8 numberofreduces = 1;
			// if starving, we lose stats a LOT faster
			if ( foodsituation == FOOD_STARVING )
				numberofreduces += Random(2);
						
			INT8 oldval = pSoldier->stats.bStrength;
			pSoldier->stats.bStrength = max(1, pSoldier->stats.bStrength - numberofreduces);
			pSoldier->usStarveDamageStrength += oldval - pSoldier->stats.bStrength;

			// Update Profile
			gMercProfiles[ pSoldier->ubProfile ].bStrength	= pSoldier->stats.bStrength;
			gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

			// make stat RED for a while...
			pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
			pSoldier->usValueGoneUp &= ~( STRENGTH_INCREASE );

			if ( foodsituation < FOOD_NORMAL )
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_STR_DAMAGE_FOOD_TOO_MUCH], pSoldier->GetName() );
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_STR_DAMAGE_FOOD_TOO_LESS], pSoldier->GetName() );
		}

		// damage health
		if ( Random(100) < statdamagechance )
		{
			UINT8 numberofreduces = 1;
			// if starving, we lose stats a LOT faster
			if ( foodsituation == FOOD_STARVING )
				numberofreduces += 1 + 2 * Random(2);

			INT8 oldlife = pSoldier->stats.bLife;

			pSoldier->stats.bLifeMax = max(2, pSoldier->stats.bLifeMax - numberofreduces);
			pSoldier->stats.bLife = min(pSoldier->stats.bLife, pSoldier->stats.bLifeMax);
			pSoldier->bBleeding = min(pSoldier->bBleeding, pSoldier->stats.bLifeMax);

			// adjust poison values
			pSoldier->bPoisonSum = min(pSoldier->bPoisonSum, pSoldier->stats.bLifeMax);
			pSoldier->bPoisonLife = min(pSoldier->bPoisonLife, pSoldier->bPoisonSum);
			pSoldier->bPoisonBleeding = min(pSoldier->bPoisonBleeding, pSoldier->bBleeding);

			pSoldier->usStarveDamageHealth += oldlife - pSoldier->stats.bLifeMax;

			// Update Profile
			gMercProfiles[ pSoldier->ubProfile ].bLifeMax	= pSoldier->stats.bLifeMax;
			gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

			if ( foodsituation < FOOD_NORMAL )
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_HEALTH_DAMAGE_FOOD_TOO_MUCH], pSoldier->GetName() );
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_HEALTH_DAMAGE_FOOD_TOO_LESS], pSoldier->GetName() );

			// if we fall below OKLIFE, we start bleeding...
			// Reason for this is that 
			if ( pSoldier->stats.bLife < OKLIFE )
			{
				pSoldier->bBleeding = max(1, pSoldier->stats.bLife - 1);
				pSoldier->stats.bLife = 1;

				// Update Profile
				gMercProfiles[ pSoldier->ubProfile ].bLifeMax	= pSoldier->stats.bLifeMax;
				pSoldier->usStarveDamageHealth += oldlife - pSoldier->stats.bLifeMax;

				return;
			}

			// make stat RED for a while...
			pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
			pSoldier->usValueGoneUp &= ~( HEALTH_INCREASE );						
		}
	}

	// now for water
	statdamagechance = FoodMoraleMods[watersituation].ubStatDamageChance;
	
	if ( statdamagechance > 0 )
	{
		// these reductions can be healed, but only if we are in a sufficient food situation again
		// damage strength
		if ( Random(100) < statdamagechance )
		{
			UINT8 numberofreduces = 1;
			// if starving, we lose stats a LOT faster
			// SANDRO - shouldn't it be "watersituation" here?
			//if ( foodsituation == FOOD_STARVING )
			if ( watersituation == FOOD_STARVING )
				numberofreduces += Random(2);
						
			INT8 oldval = pSoldier->stats.bStrength;
			pSoldier->stats.bStrength = max(1, pSoldier->stats.bStrength - numberofreduces);
			pSoldier->usStarveDamageStrength += oldval - pSoldier->stats.bStrength;

			// Update Profile
			gMercProfiles[ pSoldier->ubProfile ].bStrength	= pSoldier->stats.bStrength;
			gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;

			// make stat RED for a while...
			pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
			pSoldier->usValueGoneUp &= ~( STRENGTH_INCREASE );

			if ( watersituation < FOOD_NORMAL )
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_STR_DAMAGE_DRINK_TOO_MUCH], pSoldier->GetName() );
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_STR_DAMAGE_DRINK_TOO_LESS], pSoldier->GetName() );
		}

		// damage health
		if ( Random(100) < statdamagechance )
		{
			UINT8 numberofreduces = 1;
			// if starving, we lose stats a LOT faster
			// SANDRO - shouldn't it be "watersituation" here?
			//if ( foodsituation == FOOD_STARVING )
			if ( watersituation == FOOD_STARVING )
				numberofreduces += 1 + 2 * Random(2);

			INT8 oldlife = pSoldier->stats.bLife;

			pSoldier->stats.bLifeMax = max(2, pSoldier->stats.bLifeMax - numberofreduces);
			pSoldier->stats.bLife = min(pSoldier->stats.bLife, pSoldier->stats.bLifeMax);
			pSoldier->bBleeding = min(pSoldier->bBleeding, pSoldier->stats.bLifeMax);

			// adjust poison values
			pSoldier->bPoisonSum = min(pSoldier->bPoisonSum, pSoldier->stats.bLifeMax);
			pSoldier->bPoisonLife = min(pSoldier->bPoisonLife, pSoldier->bPoisonSum);
			pSoldier->bPoisonBleeding = min(pSoldier->bPoisonBleeding, pSoldier->bBleeding);

			pSoldier->usStarveDamageHealth += oldlife - pSoldier->stats.bLifeMax;

			// Update Profile
			gMercProfiles[ pSoldier->ubProfile ].bLifeMax	= pSoldier->stats.bLifeMax;
			gMercProfiles[ pSoldier->ubProfile ].records.usTimesStatDamaged++;
			
			if ( watersituation < FOOD_NORMAL )
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_HEALTH_DAMAGE_DRINK_TOO_MUCH], pSoldier->GetName() );
			else
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFoodTextStr[STR_FOOD_HEALTH_DAMAGE_DRINK_TOO_LESS], pSoldier->GetName() );

			// if we fall below OKLIFE, we start bleeding...
			// Reason for this is that 
			if ( pSoldier->stats.bLife < OKLIFE )
			{
				pSoldier->bBleeding = max(1, pSoldier->stats.bLife - 1);
				pSoldier->stats.bLife = 1;

				// Update Profile
				gMercProfiles[ pSoldier->ubProfile ].bLifeMax	= pSoldier->stats.bLifeMax;
				pSoldier->usStarveDamageHealth += oldlife - pSoldier->stats.bLifeMax;

				return;
			}

			// make stat RED for a while...
			pSoldier->timeChanges.uiChangeStrengthTime = GetJA2Clock();
			pSoldier->usValueGoneUp &= ~( HEALTH_INCREASE );						
		}
	}
}


void HourlyFoodAutoDigestion( SOLDIERTYPE *pSoldier )
{
	if ( !pSoldier )
		return;

	// don't eat if not necessary ( note that if the play decides to eat manually, he can achieve better results. This is intended to award micro-management)
	if ( pSoldier->bFoodLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold && pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold )
		return;

	// if we're a prisoner, we can't feed ourself, and the player can't do that either. Instead the army provides food (not much and of bad quality)
	if (pSoldier->bAssignment == ASSIGNMENT_POW)
	{
		INT16 powwater   = gGameExternalOptions.usFoodDigestionHourlyBaseDrink * gGameExternalOptions.sFoodDigestionAssignment * FOOD_POW_MULTIPLICATOR;
		INT16 powfoodadd = powwater * gGameExternalOptions.usFoodDigestionHourlyBaseFood / max(1, gGameExternalOptions.usFoodDigestionHourlyBaseDrink);

		// if we're thirsty or hungry, and this is nutritious, consume it
		if ( pSoldier->bDrinkLevel < FoodMoraleMods[FOOD_VERY_LOW].bThreshold  )
			AddFoodpoints(pSoldier->bDrinkLevel, powwater);

		if ( pSoldier->bFoodLevel < FoodMoraleMods[FOOD_VERY_LOW].bThreshold )
			AddFoodpoints(pSoldier->bDrinkLevel, powfoodadd);
	}
	else
	{
		// no eating if not able to!
		if ( pSoldier->flags.fMercAsleep == TRUE || pSoldier->stats.bLife < OKLIFE )
			return;

		// In certain facilities, we can also eat
		BOOLEAN eatinginfacility = FALSE;
		for (UINT16 cnt = 0; cnt < NUM_FACILITY_TYPES; ++cnt)
		{
			// Is this facility here?
			if (gFacilityLocations[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)][cnt].fFacilityHere)
			{
				// Does it allow training militia?
				if (gFacilityTypes[cnt].AssignmentData[FAC_FOOD].sCantinaFoodModifier > 0)
				{
					if (cnt == (UINT16)pSoldier->sFacilityTypeOperated && // Soldier is operating this facility
						GetSoldierFacilityAssignmentIndex( pSoldier ) != -1) 
					{
						eatinginfacility = TRUE;
						INT16 cantinafoodadd = gFacilityTypes[cnt].AssignmentData[FAC_FOOD].sCantinaFoodModifier;
						INT16 cantinawater   = cantinafoodadd * FOOD_FACILITY_WATER_FACTOR;

						// if we're thirsty or hungry, and this is nutritious, consume it. When in a cantina, we are willing to eat a bit more
						if ( pSoldier->bDrinkLevel < FoodMoraleMods[FOOD_MERC_STOP_FACILITY].bThreshold  )
							AddFoodpoints(pSoldier->bDrinkLevel, cantinawater);

						if ( pSoldier->bFoodLevel < FoodMoraleMods[FOOD_MERC_STOP_FACILITY].bThreshold )
							AddFoodpoints(pSoldier->bFoodLevel, cantinafoodadd);
					}
				}
			}
		}

		// search inventory for food, and eat it
		if ( !eatinginfacility )
		{
			SoldierAutoFillCanteens(pSoldier);

			EatFromInventory( pSoldier, FALSE );
		}

		// dynamic opinions: if we're still really hungry  an someone in this sector has food, we get a lwoer opinion of him, as he obviously doesn't share
		HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_NOSHARINGFOOD, FALSE, FALSE );
	}	
}

// eat stuff from the inventory. if fcanteensonly = TRUE, only drink from canteen items
void EatFromInventory( SOLDIERTYPE *pSoldier, BOOLEAN fcanteensonly )
{
	if ( !pSoldier )
		return;

	// don't eat if not necessary (note that if the player decides to eat manually, he can achieve better results. This is intended to award micro-management)
	if ( pSoldier->bFoodLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold && pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold )
		return;

	// search for food in our inventory
	INT8 invsize = (INT8)pSoldier->inv.size();									// remember inventorysize, so we don't call size() repeatedly

	// on the first loop, we omit food in bad condition, and refillable canteens and canned food
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)							// ... for all items in our inventory ...
	{
		// ... if Item exists and is food ...
		if (pSoldier->inv[bLoop].exists() == true && Item[pSoldier->inv[bLoop].usItem].foodtype > 0 )
		{
			OBJECTTYPE * pObj = &(pSoldier->inv[bLoop]);						// ... get pointer for this item ...

			if ( pObj != NULL )													// ... if pointer is not obviously useless ...
			{
				UINT32 foodtype = Item[pObj->usItem].foodtype;

				// if fcanteensonly is TRUE, omit everything that is not a canteen
				if ( fcanteensonly )
				{
					if ( Item[pObj->usItem].canteen == FALSE )
						continue;
				}
				else
				{
					// omit non-degrading food (save it for later!)
					if ( Food[foodtype].usDecayRate <= 0.0f )
						continue;

					// omit bad food (we don't like that and will eat it only if we have to)
					FLOAT foodcondition = (*pObj)[0]->data.bTemperature / OVERHEATING_MAX_TEMPERATURE;
					if ( foodcondition < FOOD_BAD_THRESHOLD )
						continue;

					if ( Item[pObj->usItem].canteen == TRUE )
						continue;
				}

				// if we're thirsty or hungry, and this is nutritious, consume it
				if ( ( pSoldier->bDrinkLevel < FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold && Food[foodtype].bDrinkPoints > 0 ) || ( pSoldier->bFoodLevel < FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold && Food[foodtype].bFoodPoints > 0 ) )
				{
					while ( (*pObj)[0]->data.objectStatus > 1 )
					{
						// if food is also a drug, ApplyDrugs will also call ApplyFood
						if ( Item[pObj->usItem].drugtype > 0 )
							ApplyDrugs( pSoldier, pObj );
						else
							ApplyFood( pSoldier, pObj, TRUE, FALSE );		// cannot reject to eat this, we chose to eat this ourself!

						// if we're full, finish
						if ( pSoldier->bFoodLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold && pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold )
							return;
					}
				}					
			}
		}
	}

	// second loop: consume anything to feed
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)							// ... for all items in our inventory ...
	{
		// ... if Item exists and is food ...
		if (pSoldier->inv[bLoop].exists() == true && Item[pSoldier->inv[bLoop].usItem].foodtype > 0 )
		{
			OBJECTTYPE * pObj = &(pSoldier->inv[bLoop]);						// ... get pointer for this item ...

			if ( pObj != NULL )													// ... if pointer is not obviously useless ...
			{
				UINT32 foodtype = Item[pObj->usItem].foodtype;
				
				// if fcanteensonly is TRUE, omit everything that is not a canteen
				if ( fcanteensonly )
				{
					if ( Item[pObj->usItem].canteen == FALSE )
						continue;
				}

				// if we're thirsty or hungry, and this is nutritious, consume it
				if ( ( pSoldier->bDrinkLevel < FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold && Food[foodtype].bDrinkPoints > 0 ) || ( pSoldier->bFoodLevel < FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold && Food[foodtype].bFoodPoints > 0 ) )
				{
					while ( (*pObj)[0]->data.objectStatus > 1 )
					{
						// if food is also a drug, ApplyDrugs will also call ApplyFood
						if ( Item[pObj->usItem].drugtype > 0 )
							ApplyDrugs( pSoldier, pObj );
						else
							ApplyFood( pSoldier, pObj, TRUE, FALSE );		// cannot reject to eat this, we chose to eat this ourself!

						// if we're full, finish
						if ( ( pSoldier->bFoodLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold || Food[foodtype].bFoodPoints == 0 ) && ( pSoldier->bDrinkLevel > FoodMoraleMods[FOOD_MERC_START_CONSUME].bThreshold|| Food[foodtype].bDrinkPoints == 0 ) )
							return;
					}
				}
			}
		}
	}
}

void HourlyFoodUpdate( void )
{
	INT8									bMercID, bLastTeamID;
	SOLDIERTYPE *							pSoldier = NULL;

	bMercID = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

	// loop through all mercs to calculate their morale
	for ( pSoldier = MercPtrs[ bMercID ]; bMercID <= bLastTeamID; ++bMercID, ++pSoldier)
	{
		//if the merc is active, and in Arulco
		if ( pSoldier && pSoldier->bActive && !AM_AN_EPC(pSoldier) && pSoldier->ubProfile != ROBOT && !IsVehicle(pSoldier) && !(pSoldier->bAssignment == IN_TRANSIT || pSoldier->bAssignment == ASSIGNMENT_DEAD ) )
		{			
			// digestion
			HourlyFoodSituationUpdate( pSoldier );

			// if hungry, eat something automatically from the inventory
			HourlyFoodAutoDigestion( pSoldier );
		}
	}
}

UINT8	GetWaterQuality(INT16 asMapX, INT16 asMapY, INT8 asMapZ)
{
	UINT8 waterquality = WATER_NONE;

	// for now, we assume that only sectors on the surface have access to water
	UINT8 ubSectorId = SECTOR(asMapX, asMapY);
	if (asMapZ == 0 && ubSectorId >= 0 && ubSectorId < 256  )
	{
		waterquality = SectorExternalData[ubSectorId][asMapZ].usWaterType;
	}

	return waterquality;
}


// a function that tries to fill up all canteens in this sector
void SectorFillCanteens( void )
{
	// no functionality if not in tactical or in combat, or nobody is here
	// can be called from a messagebox, thus the check for MSG_BOX_SCREEN
	if ( (guiCurrentScreen != GAME_SCREEN && guiCurrentScreen != MSG_BOX_SCREEN) || (gTacticalStatus.uiFlags & INCOMBAT) || gusSelectedSoldier == NOBODY )
		return;

	// determine if there are any patches of water in this sector.
	// If so, fill up all refillable water containers (= canteens) (there is no way to check if this is actually fresh water, we just assume it is)
	// If not, see if there is a water drum, and fill up the canteens from that one
	UINT8 waterquality = GetWaterQuality(gWorldSectorX, gWorldSectorY, gbWorldSectorZ);

	// search for a water drum
	BOOLEAN waterdrumfound = FALSE;
	OBJECTTYPE* pWaterDrum = GetUsableWaterDrumInSector();
	if ( pWaterDrum && (pWaterDrum->exists()) )
		waterdrumfound = TRUE;

	// drink from sector if water is ok, or it is poisonous but we haven't found a useable water drum
	if ( waterquality == WATER_DRINKABLE || (waterquality == WATER_POISONOUS && !waterdrumfound) )
	{
		// the temperature of the water in this sector (temperature reflects the quality)
		FLOAT addtemperature = OVERHEATING_MAX_TEMPERATURE;

		// if the water in this sector is poisoned, we add a different temperature - resulting in worsening of the item's decay status
		if ( waterquality == WATER_POISONOUS )
			addtemperature = FOOD_WATER_POISONOUS_TEMPERATURE;

		// first step: fill all canteens in inventories
		INT8									bMercID, bLastTeamID;
		SOLDIERTYPE *							pSoldier = NULL;

		bMercID = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

		// loop through all mercs
		for ( pSoldier = MercPtrs[ bMercID ]; bMercID <= bLastTeamID; ++bMercID, pSoldier++)
		{
			//if the merc is in this sector
			if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE && pSoldier->bInSector && ( pSoldier->sSectorX == gWorldSectorX ) && ( pSoldier->sSectorY == gWorldSectorY ) && ( pSoldier->bSectorZ == gbWorldSectorZ) )
			{
				INT8 invsize = (INT8)pSoldier->inv.size();									// remember inventorysize, so we don't call size() repeatedly

				for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)								// ... for all items in our inventory ...
				{
					// ... if Item exists and is canteen (that can have drink points) ...
					if (pSoldier->inv[bLoop].exists() == true && Item[pSoldier->inv[bLoop].usItem].canteen && Food[Item[pSoldier->inv[bLoop].usItem].foodtype].bDrinkPoints > 0)
					{
						OBJECTTYPE* pObj = &(pSoldier->inv[bLoop]);							// ... get pointer for this item ...

						if ( pObj != NULL )													// ... if pointer is not obviously useless ...
						{
							for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)				// ... there might be multiple items here (item stack), so for each one ...
							{
								UINT16 status = (*pObj)[i]->data.objectStatus;
								UINT16 statusmmissing = max(0, 100 - status);
								FLOAT temperature = (*pObj)[i]->data.bTemperature;
																
								(*pObj)[i]->data.objectStatus = 100;						// refill canteen
								(*pObj)[i]->data.bTemperature = (status * temperature + statusmmissing * addtemperature)/100;
							}
						}
					}
				}

				// it would be pretty pointless to fill our canteens and then not to drink from them even though we are hungry. If there is an unlimited water source in this sector, drink from our 
				// freshly filled canteens. Thus calling this function repeatedly will cause us to drink till we're full, and restore our canteens to full level
				if ( gGameOptions.fFoodSystem )
					EatFromInventory( pSoldier, TRUE );
			}
		}

		// second step: fill canteens in sector
		for( UINT32 uiCount = 0; uiCount < guiNumWorldItems; ++uiCount )				// ... for all items in the world ...
		{
			if( gWorldItems[ uiCount ].fExists )										// ... if item exists ...
			{
				// ... if Item exists and is a canteen (only those are refillable) ...
				if ( Item[gWorldItems[ uiCount ].object.usItem].canteen && Food[Item[gWorldItems[ uiCount ].object.usItem].foodtype].bDrinkPoints > 0)
				{
					OBJECTTYPE* pObj = &(gWorldItems[ uiCount ].object);				// ... get pointer for this item ...

					if ( pObj != NULL )													// ... if pointer is not obviously useless ...
					{
						for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)				// ... there might be multiple items here (item stack), so for each one ...
						{
							UINT16 status = (*pObj)[i]->data.objectStatus;
							UINT16 statusmmissing = max(0, 100 - status);
							FLOAT temperature = (*pObj)[i]->data.bTemperature;
																
							(*pObj)[i]->data.objectStatus = 100;						// refill canteen
							(*pObj)[i]->data.bTemperature = (status * temperature + statusmmissing * addtemperature)/100;
						}
					}
				}
			}
		}
	}
	else if ( waterdrumfound )
	{
		INT32 drumsize = Food[Item[pWaterDrum->usItem].foodtype].bDrinkPoints;

		// first step: fill all canteens in inventories
		INT8									bMercID, bLastTeamID;
		SOLDIERTYPE *							pSoldier = NULL;

		bMercID = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

		// loop through all mercs
		for ( pSoldier = MercPtrs[ bMercID ]; bMercID <= bLastTeamID; ++bMercID, pSoldier++)
		{
			//if the merc is in this sector
			if ( pSoldier->bActive && pSoldier->ubProfile != NO_PROFILE && pSoldier->bInSector && ( pSoldier->sSectorX == gWorldSectorX ) && ( pSoldier->sSectorY == gWorldSectorY ) && ( pSoldier->bSectorZ == gbWorldSectorZ) )
			{
				INT8 invsize = (INT8)pSoldier->inv.size();								// remember inventorysize, so we don't call size() repeatedly

				for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)							// ... for all items in our inventory ...
				{
					// ... if Item exists and is canteen and is NOT a water drum...
					if (pSoldier->inv[bLoop].exists() == true && Item[pSoldier->inv[bLoop].usItem].canteen && (Food[Item[pSoldier->inv[bLoop].usItem].foodtype].bDrinkPoints > 0) && !HasItemFlag(pSoldier->inv[bLoop].usItem, (WATER_DRUM)))
					{
						OBJECTTYPE* pObj = &(pSoldier->inv[bLoop]);							// ... get pointer for this item ...

						if ( pObj != NULL && pObj->exists() )								// ... if pointer is not obviously useless ...
						{
							INT32 canteensize = Food[Item[pObj->usItem].foodtype].bDrinkPoints;

							for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)			// ... there might be multiple items here (item stack), so for each one ...
							{
								if ( (*pObj)[i]->data.objectStatus < 100 )				// ... and status is > 1 (1 means that it is empty, but still usable)
								{
									INT32 ptsneeded		 = (INT32)((100 - (*pObj)[i]->data.objectStatus)		   * canteensize / 100);
									INT32 ptsinwaterdrum = (INT32)(((*pWaterDrum)[i]->data.objectStatus - 1) * drumsize    / 100);	// -1 because 1% is the status of an empty water drum

									if ( ptsneeded < ptsinwaterdrum )
									{
										(*pObj)[i]->data.objectStatus = 100;
										
										(*pWaterDrum)[i]->data.objectStatus = max(1, (INT16)((100 * ( ptsinwaterdrum - ptsneeded )) / drumsize) );
									}
									else
									{
										(*pObj)[i]->data.objectStatus += (INT16)((100 * ptsinwaterdrum) / canteensize);

										(*pWaterDrum)[i]->data.objectStatus = 1;																				
									}

									(*pObj)[i]->data.bTemperature = ((*pObj)[i]->data.bTemperature + (*pWaterDrum)[0]->data.bTemperature) / 2;	// water now has mixed freshness

									if ( (*pWaterDrum)[0]->data.objectStatus == 1 )
									{
										// get a new water drum!
										pWaterDrum = GetUsableWaterDrumInSector();
										if ( !pWaterDrum || !(pWaterDrum->exists()) )
											return;

										drumsize = Food[Item[pWaterDrum->usItem].foodtype].bDrinkPoints;
									}
								}
							}
						}
					}
				}
			}
		}

		// second step: fill canteens in sector
		for( UINT32 uiCount = 0; uiCount < guiNumWorldItems; ++uiCount )				// ... for all items in the world ...
		{
			if( gWorldItems[ uiCount ].fExists )										// ... if item exists ...
			{
				// ... if Item exists and is a canteen (only those are refillable) ...
				if ( Item[gWorldItems[ uiCount ].object.usItem].canteen )
				{
					OBJECTTYPE* pObj = &(gWorldItems[ uiCount ].object);				// ... get pointer for this item ...

					if ( pObj != NULL && pObj->exists() )								// ... if pointer is not obviously useless ...
					{
						if ( (Food[pObj->usItem].bDrinkPoints > 0) && !HasItemFlag(pObj->usItem, (WATER_DRUM)) )					// ... if item is NOT a water drum...
						{
							INT32 canteensize = Food[Item[pObj->usItem].foodtype].bDrinkPoints;

							for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)			// ... there might be multiple items here (item stack), so for each one ...
							{
								if ( (*pObj)[i]->data.objectStatus < 100 )				// ... and status is > 1 (1 means that it is empty, but still usable)
								{
									INT32 ptsneeded		 = (INT32)((100 - (*pObj)[i]->data.objectStatus)		   * canteensize / 100);
									INT32 ptsinwaterdrum = (INT32)(((*pWaterDrum)[i]->data.objectStatus - 1) * drumsize    / 100);	// -1 because 1% is the status of an empty water drum

									if ( ptsneeded < ptsinwaterdrum )
									{
										(*pObj)[i]->data.objectStatus = 100;
										
										(*pWaterDrum)[i]->data.objectStatus = max(1, (INT16)((100 * ( ptsinwaterdrum - ptsneeded )) / drumsize) );
									}
									else
									{
										(*pObj)[i]->data.objectStatus += (INT16)((100 * ptsinwaterdrum) / canteensize);

										(*pWaterDrum)[i]->data.objectStatus = 1;																				
									}

									(*pObj)[0]->data.bTemperature = ((*pObj)[0]->data.bTemperature + (*pWaterDrum)[0]->data.bTemperature) / 2;	// water now has mixed freshness

									if ( (*pWaterDrum)[i]->data.objectStatus == 1 )
									{
										// get a new water drum!
										pWaterDrum = GetUsableWaterDrumInSector();
										if ( !pWaterDrum || !(pWaterDrum->exists()) )
											return;

										drumsize = Food[Item[pWaterDrum->usItem].foodtype].bDrinkPoints;
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

OBJECTTYPE* GetUsableWaterDrumInSector( void )
{
	for( UINT32 uiCount = 0; uiCount < guiNumWorldItems; ++uiCount )				// ... for all items in the world ...
	{
		if( gWorldItems[ uiCount ].fExists )										// ... if item exists ...
		{
			// ... if Item exists and is a canteen (only those are refillable) ...
			if ( Item[gWorldItems[ uiCount ].object.usItem].canteen )
			{
				OBJECTTYPE* pObj = &(gWorldItems[ uiCount ].object);				// ... get pointer for this item ...

				if ( pObj != NULL && pObj->exists() )								// ... if pointer is not obviously useless ...
				{
					if ( HasItemFlag(pObj->usItem, (WATER_DRUM)) )					// ... if item is a water drum...
					{
						for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)			// ... there might be multiple items here (item stack), so for each one ...
						{
							if ( (*pObj)[i]->data.objectStatus > 1 )				// ... and status is > 1 (1 means that it is empty, but still usable)
							{
								return( pObj );
							}
						}
					}
				}
			}
		}
	}

	return( NULL );
}

// soldier refills canteen while auto-consuming. Only clean sector water souces are consumed, and sector inventory is not touched (sector is likely not loaded)
void SoldierAutoFillCanteens(SOLDIERTYPE *pSoldier)
{
	// no functionality if in combat, invalid/travelling/asleep/non-profile soldier
	if ( (gTacticalStatus.uiFlags & INCOMBAT) || !pSoldier || !pSoldier->bActive || pSoldier->flags.fMercAsleep || pSoldier->ubProfile == NO_PROFILE )
		return;

	// determine if there are any patches of water in this sector.
	// If so, fill up all refillable water containers (= canteens)
	UINT8 waterquality = GetWaterQuality(pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ);

	// drink from sector only if water is ok - this happens automatically, so if we use poisoned water, we will slowly poison ourselves without the player noticing
	if ( waterquality == WATER_DRINKABLE )
	{
		// the temperature of the water in this sector (temperature reflects the quality)
		FLOAT addtemperature = OVERHEATING_MAX_TEMPERATURE;

		// first step: fill all canteens in inventories	
		INT8 invsize = (INT8)pSoldier->inv.size();									// remember inventorysize, so we don't call size() repeatedly
		for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)								// ... for all items in our inventory ...
		{
			// ... if Item exists and is canteen (that can have drink points) ...
			if (pSoldier->inv[bLoop].exists() == true && Item[pSoldier->inv[bLoop].usItem].canteen && Food[Item[pSoldier->inv[bLoop].usItem].foodtype].bDrinkPoints > 0)
			{
				OBJECTTYPE* pObj = &(pSoldier->inv[bLoop]);							// ... get pointer for this item ...

				if ( pObj != NULL )													// ... if pointer is not obviously useless ...
				{
					for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)				// ... there might be multiple items here (item stack), so for each one ...
					{
						UINT16 status = (*pObj)[i]->data.objectStatus;
						UINT16 statusmmissing = max(0, 100 - status);
						FLOAT temperature = (*pObj)[i]->data.bTemperature;
																
						(*pObj)[i]->data.objectStatus = 100;						// refill canteen
						(*pObj)[i]->data.bTemperature = (status * temperature + statusmmissing * addtemperature)/100;
					}
				}
			}
		}
	}
}

BOOLEAN HasFoodInInventory( SOLDIERTYPE *pSoldier, BOOLEAN fCheckFood, BOOLEAN fCheckDrink )
{
	if ( !pSoldier )
		return FALSE;

	if ( !fCheckFood && !fCheckDrink )
		return TRUE;

	// search for food in our inventory
	INT8 invsize = (INT8)pSoldier->inv.size( );									// remember inventorysize, so we don't call size() repeatedly
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )							// ... for all items in our inventory ...
	{
		// ... if Item exists and is food ...
		if ( pSoldier->inv[bLoop].exists( ) && Item[pSoldier->inv[bLoop].usItem].foodtype > 0 )
		{
			if ( fCheckFood && Food[Item[pSoldier->inv[bLoop].usItem].foodtype].bFoodPoints )
				return TRUE;

			if ( fCheckDrink && Food[Item[pSoldier->inv[bLoop].usItem].foodtype].bDrinkPoints )
			{
				if ( Item[pSoldier->inv[bLoop].usItem].canteen )
				{
					// empty canteens retain 1% status, so check ether something is in them
					if ( pSoldier->inv[bLoop][0]->data.objectStatus > 1 )
						return TRUE;
				}
				else
					return TRUE;
			}
		}
	}

	return FALSE;
}