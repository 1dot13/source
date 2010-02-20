#ifndef __SOLDER_PROFILE_H
#define __SOLDER_PROFILE_H

#include "Soldier Control.h"
#include "Soldier Profile Type.h"
#include "Merc Hiring.h"

extern INT8 gbSkillTraitBonus[NUM_SKILLTRAITS];
extern UINT8 gubTerrorists[];
extern INT16 gsTerroristSector[][5][2];
extern BOOLEAN	gfPotentialTeamChangeDuringDeath;

extern MERCPROFILESTRUCT gMercProfiles[ NUM_PROFILES ];
extern MERCPROFILEGEAR gMercProfileGear[ NUM_PROFILES ];


#define AIM_AND_MERC_MERCS		51		// A.I.M. is 0-39, M.E.R.C.s are 40-50

//use this to sort out the above define
BOOLEAN IsProfileIdAnAimOrMERCMerc( UINT8 ubProfileID );
//enums for the mercs 
enum NPCIDs
{
	BIFF = 40,
	HAYWIRE,
	GASKET,
	RAZOR,
	FLO,
	GUMPY,
	LARRY_NORMAL,
	LARRY_DRUNK,
	COUGAR,
	NUMB,
	BUBBA,

	MIGUEL = 57,
	CARLOS,
	IRA,
	DIMITRI,

	DEVIN,	// 61
	ROBOT,
	HAMOUS,
	SLAY,
	RPC65,
	DYNAMO,
	SHANK,
	IGGY,
	VINCE,
	CONRAD,

	RPC71,	// 71
	MADDOG,
	DARREL,
	PERKO,
	QUEEN,
	AUNTIE,
	ENRICO, //GABBY, //ENRICO,
	CARMEN,
	JOE,
	STEVE,

	RAT,		// 81
	ANNIE,
	CHRIS,
	BOB,
	BRENDA,
	KINGPIN,
	DARREN,
	MARIA,
	ANGEL,
	JOEY,

	TONY,		// 91
	FRANK,
	SPIKE,
	DAMON,
	KYLE,
	MICKY,
	SKYRIDER,
	PABLO,
	SAL,
	FATHER,

	FATIMA,	// 101
	WARDEN,
	GORDON,
	GABBY, //ENRICO, //GABBY,
	ERNEST,
	FRED,
	MADAME,
	YANNI,
	MARTHA,
	TIFFANY,

	T_REX,	// 111
	DRUGGIST,	// ELGIN
	JAKE,
	PACOS,
	GERARD,
	SKIPPER,
	HANS,
	JOHN,
	MARY,
	GENERAL,

	SERGEANT,	// 121
	ARMAND,
	LORA,
	FRANZ,
	HOWARD,
	SAM,
	ELDIN,
	ARNIE,
	TINA,
	FREDO,

	WALTER,	// 131
	JENNY,
	BILLY,
	BREWSTER,
	ELLIOT,
	DEREK,
	OLIVER,
	WALDO,
	DOREEN,
	JIM,

	JACK,		// 141
	OLAF,
	RAY,
	OLGA,
	TYRONE,
	MADLAB,
	KEITH,
	MATT,
	MIKE,
	DARYL,

	HERVE,	// 151
	PETER,
	ALBERTO,
	CARLO,
	MANNY,
	OSWALD,
	CALVIN,
	CARL,
	SPECK,
	
	PROF_HUMMER,

	PROF_ELDERODO,	// 161
	PROF_ICECREAM,
	PROF_HELICOPTER,
	NPC164,
	GASTON,
	STOGIE,
	TEX,
	BIGGENS,
	NPC169,
} ;

BOOLEAN LoadMercProfiles(void);

// This function will overload the default for the face index ( which is the ubCharNum )
void SetProfileFaceData( UINT8 ubCharNum, UINT8 ubFaceIndex, UINT16 usEyesX, UINT16 usEyesY, UINT16 usMouthX, UINT16 usMouthY );
SOLDIERTYPE * FindSoldierByProfileID( UINT8 ubProfileID, BOOLEAN fPlayerMercsOnly );

BOOLEAN RecruitRPC( UINT8 ubCharNum );

BOOLEAN RecruitEPC( UINT8 ubCharNum );
BOOLEAN UnRecruitEPC( UINT8 ubCharNum );

INT8 WhichBuddy( UINT8 ubCharNum, UINT8 ubBuddy );
INT8 WhichHated( UINT8 ubCharNum, UINT8 ubHated );

SOLDIERTYPE *ChangeSoldierTeam( SOLDIERTYPE *pSoldier, UINT8 ubTeam );

BOOLEAN IsProfileATerrorist( UINT8 ubProfile );
BOOLEAN IsProfileAHeadMiner( UINT8 ubProfile );

void UpdateSoldierPointerDataIntoProfile( BOOLEAN fPlayerMercs );


// Returns true if a buddy of the merc is on team
BOOLEAN DoesMercHaveABuddyOnTheTeam( UINT8 ubMercID );

void StartSomeMercsOnAssignment( void );

void DecideOnAssassin( void );

extern BOOLEAN gubNumTerrorists;

void MakeRemainingTerroristsTougher( void );
void MakeRemainingAssassinsTougher( void );
SOLDIERTYPE * SwapLarrysProfiles( SOLDIERTYPE * pSoldier );

BOOLEAN DoesNPCOwnBuilding( SOLDIERTYPE *pSoldier, INT16 sGridNo );

// HEADROCK PROFEX: Temporary array for merc profile data, read from XML
typedef struct
{
	CHAR16 zName[NAME_LENGTH];
	CHAR16 zNickname[ NICKNAME_LENGTH ];
	UINT8		ubFaceIndex;
	UINT16	usEyesX;
	UINT16	usEyesY;
	UINT16	usMouthX;
	UINT16	usMouthY;
	UINT32	uiEyeDelay;
	UINT32	uiMouthDelay;
	UINT32	uiBlinkFrequency;
	UINT32	uiExpressionFrequency;

	PaletteRepID		PANTS;
	PaletteRepID		VEST;
	PaletteRepID		SKIN;
	PaletteRepID		HAIR;

	INT8		bSex;
	UINT8		ubBodyType;
	UINT32		uiBodyTypeSubFlags;

	INT8		bAttitude;
	INT8		bPersonalityTrait;
	UINT8		ubNeedForSleep;

	INT8		bReputationTolerance;
	INT8		bDeathRate;

	INT8		bLifeMax;
	INT8		bLife;
	INT8		bStrength;
	INT8		bAgility;
	INT8		bDexterity;	
	INT8		bWisdom;
	INT8		bMarksmanship;
	INT8		bExplosive;
	INT8		bLeadership;
	INT8		bMedical;
	INT8		bMechanical;

	INT8		bExpLevel;

	INT8		bEvolution;
	INT8		bSkillTrait;
	INT8		bSkillTrait2;

	INT8		bBuddy[5];
	INT8		bLearnToLike;
	INT8		bLearnToLikeTime;

	INT8		bHated[5];
	INT8		bHatedTime[5];
	INT8		bLearnToHate;
	INT8		bLearnToHateTime;

	INT16		sSalary;
	UINT32		uiWeeklySalary;
	UINT32		uiBiWeeklySalary;
	INT8		bMedicalDeposit;
	UINT16		sMedicalDepositAmount;
	UINT16		usOptionalGearCost;

	INT8		bArmourAttractiveness;
	INT8		bMainGunAttractiveness;

	// This boolean DOES NOT EXIST in gMercProfiles - it is part of a flag set called ubMiscFlags. This code
	// reads the boolean and applies the flag to that flagset, if true.
	BOOLEAN		fGoodGuy;

	UINT16		usApproachFactor[4];

	INT8 bMercOpinion[75];

} TEMPPROFILETYPE;

extern TEMPPROFILETYPE tempProfiles[NUM_PROFILES+1];

extern BOOLEAN WriteMercProfiles();
extern BOOLEAN WriteMercOpinions();

void OverwriteMercProfileWithXMLData( UINT32 uiLoop );
void OverwriteMercOpinionsWithXMLData( UINT32 uiLoop );

#endif
