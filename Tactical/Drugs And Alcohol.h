#ifndef __DRUGS_AND_ALCOHOL_H
#define __DRUGS_AND_ALCOHOL_H

#include "Soldier Control.h"

typedef struct
{
	UINT8		effect;
	UINT16		duration;
	INT16		size;
	UINT8		chance;
} DRUG_EFFECT;

typedef struct
{
	UINT8		disease;
	INT32		size;
	UINT8		chance;
} DISEASE_EFFECT;

typedef struct
{
	UINT8		disability;
	UINT16		duration;
	UINT8		chance;
} DISABILITY_EFFECT;

typedef struct
{
	UINT8		personality;
	UINT16		duration;
	UINT8		chance;
} PERSONALITY_EFFECT;

typedef struct
{
	UINT8		uiIndex;
	CHAR16		szName[80];							// name, used for display
	BOOLEAN		opinionevent;

	std::vector<DRUG_EFFECT>		drug_effects;
	std::vector<DISEASE_EFFECT>		disease_effects;
	std::vector<DISABILITY_EFFECT>	disability_effects;
	std::vector<PERSONALITY_EFFECT>	personality_effects;
} DRUG;

//GLOBALS
#define NEW_DRUGS_MAX			100

extern DRUG NewDrug[NEW_DRUGS_MAX];

enum {
	SOBER = 0,
	FEELING_GOOD,
	BORDERLINE,
	DRUNK,
	HUNGOVER,
};

#define LARRY_FALLS_OFF_WAGON 8

BOOLEAN ApplyDrugs_New( SOLDIERTYPE *pSoldier, UINT16 usItem, UINT16 uStatusUsed );

void HandleEndTurnDrugAdjustments_New( SOLDIERTYPE *pSoldier );

INT8 GetDrunkLevel( SOLDIERTYPE *pSoldier );

// does a merc have a disability/personality, or is he under drugs that simulate this?
BOOLEAN DoesMercHaveDisability( const SOLDIERTYPE *pSoldier, UINT8 aVal );
BOOLEAN DoesMercHavePersonality( SOLDIERTYPE *pSoldier, UINT8 aVal );

//void HandleEndTurnDrugAdjustments( SOLDIERTYPE *pSoldier );
void HandleAPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *pubPoints );
void HandleBPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *psPoints );

INT32 EffectStatForBeingDrunk( SOLDIERTYPE *pSoldier, INT32 iStat );
BOOLEAN MercDruggedOrDrunk( SOLDIERTYPE *pSoldier );
BOOLEAN MercDrugged( SOLDIERTYPE *pSoldier );

void HourlyDrugUpdate();

#endif
