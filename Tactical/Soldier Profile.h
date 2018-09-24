#ifndef __SOLDER_PROFILE_H
#define __SOLDER_PROFILE_H

#include "Soldier Control.h"
#include "Soldier Profile Type.h"
#include "Merc Hiring.h"

typedef struct
{
	UINT16 uiIndex;
	BOOLEAN Enabled;
	INT8 BaseAttribute;
	BOOLEAN RandomExpLevel;
	BOOLEAN RandomLife;
	BOOLEAN RandomAgility;
	BOOLEAN RandomDexterity;
	BOOLEAN RandomStrength;
	BOOLEAN RandomLeadership;
	BOOLEAN RandomWisdom;
	BOOLEAN RandomMarksmanship;
	BOOLEAN RandomMechanical;
	BOOLEAN RandomExplosive;
	BOOLEAN RandomMedical;
	BOOLEAN RandomScientific;
} RANDOM_STATS_VALUES;

extern RANDOM_STATS_VALUES gRandomStatsValue[NUM_PROFILES];
extern void RandomStats ();

extern INT8 gbSkillTraitBonus[NUM_SKILLTRAITS_OT];
extern UINT8 gubTerrorists[];
extern INT16 gsTerroristSector[][5][2];
extern BOOLEAN	gfPotentialTeamChangeDuringDeath;

extern MERCPROFILESTRUCT gMercProfiles[ NUM_PROFILES ];

extern MERCPROFILEGEAR gMercProfileGear[ NUM_PROFILES ][ NUM_MERCSTARTINGGEAR_KITS ];


#define AIM_AND_MERC_MERCS		51		// A.I.M. is 0-39, M.E.R.C.s are 40-50

//use this to sort out the above define
BOOLEAN IsProfileIdAnAimOrMERCMerc( UINT8 ubProfileID );

//enums for the mercs
enum ProfileType
{
	PROFILETYPE_NONE = 0,
	PROFILETYPE_AIM,
	PROFILETYPE_MERC,
	PROFILETYPE_RPC,
	PROFILETYPE_NPC,
	PROFILETYPE_VEHICLE,
	PROFILETYPE_IMP,

	PROFILETYPE_MAX
};

// Flugente: why do we require 6 different structures and arrays if all we keep track of is the profile type? A simple array is enough (and only needed during loading anyway)
extern UINT8 gProfileType[NUM_PROFILES];

extern BOOLEAN LoadNewSystemMercsToSaveGameFile( HWFILE hFile );
//---------------------------------------------------

enum NPCIDs
{
	VICKI = 4,// anv: check when hiring Speck
	BUNS = 17,
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
#ifdef JA2UB
	MIGUEL = 58,  	//Ja25:		Was 57
#else
	MIGUEL = 57,
#endif
	CARLOS,
	IRA,
	DIMITRI,

	DEVIN,	// 61
	ROBOT,
	HAMOUS,
	SLAY,
#ifdef JA2UB
//	RPC65,
#else
	RPC65,
#endif
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
#ifdef JA2UB
	NPC165,
	NPC166,
	NPC167,
	NPC168,
	NPC169,
#else
	GASTON,
	STOGIE,
	TEX,
	BIGGENS,
	NPC169,
#endif	
	NPC170 = NPC169 + 84,
	SPECK_PLAYABLE = 191,// anv
	
	// Those 4 only available in JA2 113 and not in JA2 UB!
	JOHN_MERC = 195,
	ELIO = 196,
	JUAN = 197,
	WAHAN = 198,

	BUNS_CHAOTIC = 215,
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
BOOLEAN MercIsHot( SOLDIERTYPE * pSoldier ); // added by SANDRO
BOOLEAN MercIsInTropicalSector( SOLDIERTYPE * pSoldier ); // added by SANDRO

void StartSomeMercsOnAssignment( void );

void DecideOnAssassin( void );

extern BOOLEAN gubNumTerrorists;

void MakeRemainingTerroristsTougher( void );
void MakeRemainingAssassinsTougher( void );
SOLDIERTYPE* SwapToProfile( SOLDIERTYPE * pSoldier, UINT8 ubDestProfile );

BOOLEAN DoesNPCOwnBuilding( SOLDIERTYPE *pSoldier, INT32 sGridNo );

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
	INT8		bCharacterTrait; // added by SANDRO
	INT8		bDisability;
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
	// changed by SANDRO
	INT8		bOldSkillTrait;
	INT8		bOldSkillTrait2;
	// added by SANDRO
	INT8		bNewSkillTrait1;
	INT8		bNewSkillTrait2;
	INT8		bNewSkillTrait3;
	INT8		bNewSkillTrait4;
	INT8		bNewSkillTrait5;
	INT8		bNewSkillTrait6;
	INT8		bNewSkillTrait7;
	INT8		bNewSkillTrait8;
	INT8		bNewSkillTrait9;
	INT8		bNewSkillTrait10;
	INT8		bNewSkillTrait11;
	INT8		bNewSkillTrait12;
	INT8		bNewSkillTrait13;
	INT8		bNewSkillTrait14;
	INT8		bNewSkillTrait15;
	INT8		bNewSkillTrait16;
	INT8		bNewSkillTrait17;
	INT8		bNewSkillTrait18;
	INT8		bNewSkillTrait19;
	INT8		bNewSkillTrait20;
	INT8		bNewSkillTrait21;
	INT8		bNewSkillTrait22;
	INT8		bNewSkillTrait23;
	INT8		bNewSkillTrait24;
	INT8		bNewSkillTrait25;
	INT8		bNewSkillTrait26;
	INT8		bNewSkillTrait27;
	INT8		bNewSkillTrait28;
	INT8		bNewSkillTrait29;
	INT8		bNewSkillTrait30;

	UINT8		bBuddy[5];
	UINT8		bLearnToLike;
	INT8		bLearnToLikeTime;

	UINT8		bHated[5];
	INT8		bHatedTime[5];
	UINT8		bLearnToHate;
	INT8		bLearnToHateTime;

	INT8		bRace;
	INT8		bNationality;
	INT8		bAppearance;
	INT8		bAppearanceCareLevel;
	INT8		bRefinement;
	INT8		bRefinementCareLevel;
	INT8		bHatedNationality;
	INT8		bHatedNationalityCareLevel;
	INT8		bRacist;
	UINT8		bSexist;

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

	INT8 bMercOpinion[NUMBER_OF_OPINIONS];
	
	UINT32 Type;
	
	UINT16 sSectorX;
	UINT16 sSectorY;
	INT8	bSectorZ;
	UINT8 ubCivilianGroup;
	INT8 bTown;
	INT8 bTownAttachment;

	// Flugente: backgrounds
	UINT16 usBackground;

	UINT32 usVoiceIndex;

} TEMPPROFILETYPE;

extern TEMPPROFILETYPE tempProfiles[NUM_PROFILES+1];

extern BOOLEAN WriteMercProfiles();
extern BOOLEAN WriteMercOpinions();

void OverwriteMercProfileWithXMLData( UINT32 uiLoop );
void OverwriteMercOpinionsWithXMLData( UINT32 uiLoop );

// SANDRO - added functions
INT8 CheckMercsNearForCharTraits( UINT8 ubProfileID, INT8 bCharTraitID );
INT8 ProfileHasSkillTrait( INT32 ubProfileID, INT8 bSkillTrait );

#endif
