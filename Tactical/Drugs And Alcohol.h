#ifndef __DRUGS_AND_ALCOHOL_H
#define __DRUGS_AND_ALCOHOL_H

#include "soldier control.h"

#define	SOBER										0
#define	FEELING_GOOD								1
#define	BORDERLINE									2
#define DRUNK										3
#define	HUNGOVER									4 

#define REGEN_POINTS_PER_BOOSTER					4
#define LIFE_GAIN_PER_REGEN_POINT					10

extern	UINT8	gbPlayerNum;

enum {
	DRUG_TYPE_ADRENALINE = 0,
	DRUG_TYPE_ALCOHOL,
	DRUG_TYPE_REGENERATION,
	DRUG_TYPE_RESISTANCE,
	DRUG_TYPE_STRENGTH,
	DRUG_TYPE_AGILITY,		// 5
	DRUG_TYPE_DEXTERITY,
	DRUG_TYPE_WISDOM,
	DRUG_TYPE_PERCEPTION,	
	DRUG_TYPE_PSYCHO,	
	DRUG_TYPE_NERVOUS,		// 10
	DRUG_TYPE_CLAUSTROPHOBIC,
	DRUG_TYPE_HEATINTOLERANT,
	DRUG_TYPE_FEAROFINSECTS,
	DRUG_TYPE_FORGETFUL,
	DRUG_TYPE_BLIND,		// 15
	DRUG_TYPE_KNOCKOUT,
	DRUG_TYPE_VISION,
	DRUG_TYPE_TUNNELVISION,
	DRUG_TYPE_LIFEDAMAGE
};

// Flugente, 12-04-21
// These flags determine the different types of 'pure' drugs that we have. Each one has specific effects and sideeffects
// Note that a consumable can be a combination of several different drugs
#define DRUG_ADRENALINE			(1 << DRUG_TYPE_ADRENALINE)			//0x00000001	//1
#define DRUG_ALCOHOL			(1 << DRUG_TYPE_ALCOHOL)			//0x00000002	//2
#define DRUG_REGENERATION		(1 << DRUG_TYPE_REGENERATION)		//0x00000004	//4
#define DRUG_DAMAGERESISTANCE	(1 << DRUG_TYPE_RESISTANCE)			//0x00000008	//8
#define DRUG_STRENGTH			(1 << DRUG_TYPE_STRENGTH)			//0x00000010	//16
#define DRUG_AGILITY			(1 << DRUG_TYPE_AGILITY)			//0x00000020	//32
#define DRUG_DEXTERITY   		(1 << DRUG_TYPE_DEXTERITY)			//0x00000040	//64
#define DRUG_WISDOM   			(1 << DRUG_TYPE_WISDOM)				//0x00000080	//128
#define DRUG_PERCEPTION			(1 << DRUG_TYPE_PERCEPTION)			//0x00000100	//256
#define DRUG_PSYCHO				(1 << DRUG_TYPE_PSYCHO)				//0x00000200	//512
#define DRUG_NERVOUS			(1 << DRUG_TYPE_NERVOUS)			//0x00000400	//1024
#define DRUG_CLAUSTROPHOBIC		(1 << DRUG_TYPE_CLAUSTROPHOBIC)		//0x00000800	//2048
#define DRUG_HEATINTOLERANT		(1 << DRUG_TYPE_HEATINTOLERANT)		//0x00001000	//4096
#define DRUG_FEAROFINSECTS		(1 << DRUG_TYPE_FEAROFINSECTS)		//0x00002000	//8192
#define DRUG_FORGETFUL			(1 << DRUG_TYPE_FORGETFUL)			//0x00004000	//16384
#define DRUG_BLIND 				(1 << DRUG_TYPE_BLIND)				//0x00008000	//32768
#define DRUG_KNOCKOUT 			(1 << DRUG_TYPE_KNOCKOUT)			//0x00010000	//65536
#define DRUG_VISION 	        (1 << DRUG_TYPE_VISION)				//0x00020000	//131072
#define DRUG_TUNNELVISION 	    (1 << DRUG_TYPE_TUNNELVISION)		//0x00040000	//262144
#define DRUG_LIFEDAMAGE			(1 << DRUG_TYPE_LIFEDAMAGE)			//0x00080000	//524288
#define DRUG_MISC_7 	        0x00100000	//1048576
#define DRUG_MISC_8 	    	0x00200000	//2097152
#define DRUG_MISC_9 	    	0x00400000	//4194304
#define DRUG_MISC_10 	    	0x00800000	//8388608
#define DRUG_MISC_11 	    	0x01000000	//16777216
#define DRUG_MISC_12 	    	0x02000000	//33554432
#define DRUG_MISC_13 	    	0x04000000	//67108864
#define DRUG_MISC_14 	    	0x08000000	//134217728
#define DRUG_MISC_15 	    	0x10000000	//268435456
#define DRUG_MISC_16 	    	0x20000000	//536870912
#define DRUG_MISC_17 	    	0x40000000	//1073741824
#define DRUG_MISC_18 	    	0x80000000	//2147483648

typedef struct
{
	UINT8		ubType;					// type of drug: bit field
	UINT8		ubDrugTravelRate;		
	UINT8		ubDrugWearoffRate;
	UINT8		ubDrugEffect;		
	UINT8		ubDrugSideEffect;		
	UINT8		ubDrugSideEffectRate;
	UINT8		ubMoralBacklash;
} DRUGTYPE;

//GLOBALS
extern DRUGTYPE Drug[DRUG_TYPE_MAX];

BOOLEAN ApplyDrugs( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject );

void HandleEndTurnDrugAdjustments( SOLDIERTYPE *pSoldier );
void HandleAPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *pubPoints );
void HandleBPEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT16 *psPoints );
void HandleDamageResistanceEffectDueToDrugs( SOLDIERTYPE *pSoldier, INT32 *psPoints );


INT8 GetDrugEffect( SOLDIERTYPE *pSoldier, UINT8 ubDrugType );
INT8 GetDrugSideEffect( SOLDIERTYPE *pSoldier, UINT8 ubDrugType );
INT8 GetDrugAllEffect( SOLDIERTYPE *pSoldier, UINT8 ubDrugType );
INT8 GetDrunkLevel( SOLDIERTYPE *pSoldier );
INT32 EffectStatForBeingDrunk( SOLDIERTYPE *pSoldier, INT32 iStat );
BOOLEAN MercUnderTheInfluence( SOLDIERTYPE *pSoldier );
BOOLEAN MercUnderTheInfluence( SOLDIERTYPE *pSoldier, UINT8 aDrugType );


#endif
