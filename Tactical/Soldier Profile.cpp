	#include <stdio.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	#include "math.h"
	#include "worlddef.h"
	#include "renderworld.h"


	#include "Animation Control.h"
	#include "Animation Data.h"
	#include "Isometric Utils.h"
	#include "Event Pump.h"
	#include "Render Fun.h"
	#include "interface.h"
	#include "sysutil.h"
	#include "FileMan.h"
	#include "Random.h"
	#include "ai.h"
	#include "Interactive Tiles.h"
	#include "soldier ani.h"
	#include "english.h"
	#include "overhead.h"
	#include "Soldier Profile.h"
	#include "Game Clock.h"
	#include "assignments.h"
	#include "Dialogue Control.h"
	#include "soldier create.h"
	#include "soldier add.h"
	#include "opplist.h"
	#include "weapons.h"
	#include "Strategic Town Loyalty.h"
	#include "squads.h"
	#include "Tactical Save.h"
	#include "Quests.h"
	#include "aim.h"
	#include "Interface Dialogue.h"
	#include "GameSettings.h"
	#include "strategic town reputation.h"
	#include "interface utils.h"
	#include "Game Event Hook.h"
	#include "Map Information.h"
	#include "history.h"
	#include "personnel.h"
	#include "environment.h"
	#include "Player Command.h"
	#include "strategic.h"
	#include "strategicmap.h" // added by SANDRO
	#include "drugs and alcohol.h" // added by Flugente
	#include "Campaign.h"
	#include "LuaInitNPCs.h"		// added by Flugente

#include "aim.h"
#include "AimFacialIndex.h"
#include "mercs.h"

#ifdef JA2UB
#include "Ja25_Tactical.h"
#endif

#include "ub_config.h"
#include "XML.h"

#ifdef JA2UB
#else
	// anv: for playable Speck
	#include "Speck Quotes.h"
	#include "LaptopSave.h"
#endif

#include "connect.h"
#ifdef JA2EDITOR
	extern BOOLEAN gfProfileDataLoaded;
#endif

extern INT32 GetTheStateOfDepartedMerc(INT32 iId);

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

// Flugente: why do we require 6 different structures and arrays if all we keep track of is the profile type? A simple array is enough
UINT8 gProfileType[NUM_PROFILES];

BOOLEAN	gfPotentialTeamChangeDuringDeath = FALSE;


#define		MIN_BLINK_FREQ					3000
#define		MIN_EXPRESSION_FREQ			2000

#define		SET_PROFILE_GAINS2			500, 500, 500, 500, 500, 500, 500, 500, 500

MERCPROFILESTRUCT	gMercProfiles[ NUM_PROFILES ];
MERCPROFILEGEAR		gMercProfileGear[ NUM_PROFILES ][ NUM_MERCSTARTINGGEAR_KITS ];

extern UINT8 gubItemDroppableFlag[NUM_INV_SLOTS];

//Random Stats 
RANDOM_STATS_VALUES gRandomStatsValue[NUM_PROFILES];
void RandomStats();
void RandomGrowthModifiers();
void RandomStartSalary();

//Jenilee
extern void RandomizeMerc(UINT8 profile_id, MERCPROFILESTRUCT* merc, BOOL random_gear_kits);
extern void InitRandomMercs();
extern void ExitRandomMercs();

INT32 RandomAbsoluteRange( INT32 iValue, INT32 iMin, INT32 iMax, INT32 iRange, BOOLEAN fBellCurve );
INT8 RandomPercentRange( UINT8 iPRange, BOOLEAN fBellCurve );

INT8 gbSkillTraitBonus[NUM_SKILLTRAITS_OT] =
{
	0,	//NO_SKILLTRAIT
	25,	//LOCKPICKING
	15,	//HANDTOHAND
	15,	//ELECTRONICS
	15,	//NIGHTOPS
	12,	//THROWING
	15,	//TEACHING
	15,	//HEAVY_WEAPS
	0,	//AUTO_WEAPS
	15,	//STEALTHY
	0,	//AMBIDEXT
	0,	//THIEF				// UNUSED!
	30,	//MARTIALARTS
	30,	//KNIFING
	5,	//PROF_SNIPER
	0,	//CAMOUFLAGED
	//0,	//CAMOUFLAGED_URBAN
	//0,	//CAMOUFLAGED_DESERT
	//0,	//CAMOUFLAGED_SNOW
};


UINT8			gubBasicInventoryPositions[] = {
							HELMETPOS,
							VESTPOS,
							LEGPOS,
							HANDPOS,
							BIGPOCK1POS,
							BIGPOCK2POS,
							BIGPOCK3POS,
							BIGPOCK4POS
};

#define NUM_TERRORISTS 6

UINT8	gubTerrorists[NUM_TERRORISTS + 1] =
{
	DRUGGIST,
	SLAY,
	ANNIE,
	CHRIS,
	TIFFANY,
	T_REX,
	0
};

UINT8	gubNumTerrorists = 0;

#define NUM_TERRORIST_POSSIBLE_LOCATIONS 5

INT16	gsTerroristSector[NUM_TERRORISTS][NUM_TERRORIST_POSSIBLE_LOCATIONS][2] =
{
	// Elgin... preplaced
	{
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 }
	},
	// Slay
	{
		{ 9,	MAP_ROW_F },
		{ 14,	MAP_ROW_I },
		{ 1,	MAP_ROW_G },
		{ 2,	MAP_ROW_G },
		{ 8,	MAP_ROW_G }
	},
	// Matron
	{
		{ 14,	MAP_ROW_I },
		{ 6,	MAP_ROW_C },
		{ 2,	MAP_ROW_B },
		{ 11, MAP_ROW_L },
		{ 8,	MAP_ROW_G }
	},
	// Imposter
	{
		{ 1,	MAP_ROW_G },
		{ 9,	MAP_ROW_F },
		{	11,	MAP_ROW_L },
		{	8,	MAP_ROW_G },
		{ 2,	MAP_ROW_G }
	},
	// Tiffany
	{
		{ 14,	MAP_ROW_I },
		{ 2,	MAP_ROW_G },
		{ 14,	MAP_ROW_H },
		{	6,	MAP_ROW_C },
		{	2,	MAP_ROW_B }
	},
	// Rexall
	{
		{	9,	MAP_ROW_F },
		{ 14,	MAP_ROW_H },
		{ 2,	MAP_ROW_H },
		{ 1,	MAP_ROW_G },
		{ 2,	MAP_ROW_B }
	}
};

INT32 gsRobotGridNo;

#define NUM_ASSASSINS 6

UINT8 gubAssassins[NUM_ASSASSINS] =
{
	JIM,
	JACK,
	OLAF,
	RAY,
	OLGA,
	TYRONE
};

#define NUM_ASSASSIN_POSSIBLE_TOWNS 5

INT8 gbAssassinTown[NUM_ASSASSINS][NUM_ASSASSIN_POSSIBLE_TOWNS] =
{
	// Jim
	{ CAMBRIA, DRASSEN, ALMA, BALIME, GRUMM },
	// Jack
	{ CHITZENA,	ESTONI, ALMA, BALIME, GRUMM },
	// Olaf
	{ DRASSEN, ESTONI, ALMA, CAMBRIA, BALIME },
	// Ray
	{ CAMBRIA, OMERTA, BALIME, GRUMM, DRASSEN },
	// Olga
	{ CHITZENA, OMERTA, CAMBRIA, ALMA, GRUMM },
	// Tyrone
	{ CAMBRIA, BALIME, ALMA, GRUMM, DRASSEN },
};

UINT16 CalcCompetence( MERCPROFILESTRUCT * pProfile );
INT16 CalcMedicalDeposit( MERCPROFILESTRUCT * pProfile );
extern void HandleEndDemoInCreatureLevel( );
void DecideActiveTerrorists( void );

extern SOLDIERTYPE			*gpSMCurrentMerc;
extern BOOLEAN	gfRerenderInterfaceFromHelpText;

BOOLEAN LoadNewSystemMercsToSaveGameFile( HWFILE hFile )
{
	// read the old data from the save file and store it in the new array while you're at it
	UINT32	uiNumBytesRead;

	for ( int i = 0; i < NUM_PROFILES; ++i )
		gProfileType[i] = PROFILETYPE_NONE;

	typedef struct
	{
		UINT8 ProfilId;
	} TMP_PROFIL;

	TMP_PROFIL  tmpprofilewedontreallyneed[NUM_PROFILES];

	for ( int profiletype = PROFILETYPE_AIM; profiletype < PROFILETYPE_MAX; ++profiletype )
	{
		FileRead( hFile, &tmpprofilewedontreallyneed, sizeof( tmpprofilewedontreallyneed ), &uiNumBytesRead );
		if ( uiNumBytesRead != sizeof( tmpprofilewedontreallyneed ) )
		{
			return( FALSE );
		}

		for ( int i = 0; i < NUM_PROFILES; ++i )
		{
			if ( tmpprofilewedontreallyneed[i].ProfilId == i )
				gProfileType[i] = profiletype;
		}
	}
	
	// for whatever bizarre reason, this function is called way after the profiles themselves are loaded - so we now have to overwrite the types in there
	for ( int i = 0; i < NUM_PROFILES; ++i )
	{
		gMercProfiles[i].Type = gProfileType[i];
	}
	
	return( TRUE );
}

//Random stats
void RandomStats()
{
	UINT32 cnt;
	INT8 bBaseAttribute = 0;
	MERCPROFILESTRUCT * pProfile;
	UINT8 Exp = gGameExternalOptions.ubMercRandomExpRange;
	UINT8 Stats = gGameExternalOptions.ubMercRandomStatsRange;
	BOOLEAN Type = gGameExternalOptions.fMercRandomBellDistribution;

	// not randomizing
	if ( gGameExternalOptions.ubMercRandomStats == 0 )
		return;

	// full stats random for all soldierIDs
	else if ( gGameExternalOptions.ubMercRandomStats == 1 )
	{	
		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			pProfile = &(gMercProfiles[cnt]);
			// Buggler: +/- random range will be limited due to min/max allowed value
			pProfile->bExpLevel		= RandomAbsoluteRange( pProfile->bExpLevel, 1, 10, Exp, Type );
			pProfile->bLifeMax		= RandomAbsoluteRange( pProfile->bLifeMax, 1, 100, Stats, Type );
			pProfile->bLife			= pProfile->bLifeMax;
			pProfile->bAgility		= RandomAbsoluteRange( pProfile->bAgility, 1, 100, Stats, Type );
			pProfile->bDexterity	= RandomAbsoluteRange( pProfile->bDexterity, 1, 100, Stats, Type );
			pProfile->bStrength		= RandomAbsoluteRange( pProfile->bStrength, 1, 100, Stats, Type );
			pProfile->bLeadership	= RandomAbsoluteRange( pProfile->bLeadership, 1, 100, Stats, Type );
			pProfile->bWisdom		= RandomAbsoluteRange( pProfile->bWisdom, 1, 100, Stats, Type );
			pProfile->bMarksmanship	= RandomAbsoluteRange( pProfile->bMarksmanship, 1, 100, Stats, Type );
			pProfile->bMechanical	= RandomAbsoluteRange( pProfile->bMechanical, 1, 100, Stats, Type );
			pProfile->bExplosive	= RandomAbsoluteRange( pProfile->bExplosive, 1, 100, Stats, Type );
			pProfile->bMedical		= RandomAbsoluteRange( pProfile->bMedical, 1, 100, Stats, Type );
		}
	}

	// partial stats random based on XML stat tag
	else if ( gGameExternalOptions.ubMercRandomStats == 2 )
	{
		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			if ( gRandomStatsValue[cnt].Enabled )
			{
				pProfile = &(gMercProfiles[cnt]);
				
				if ( gRandomStatsValue[cnt].RandomExpLevel == TRUE )
					pProfile->bExpLevel		= RandomAbsoluteRange( pProfile->bExpLevel, 1, 10, Exp, Type );

				if ( gRandomStatsValue[cnt].RandomLife == TRUE )
				{
					pProfile->bLifeMax		= RandomAbsoluteRange( pProfile->bLifeMax, 1, 100, Stats, Type );
					pProfile->bLife			= pProfile->bLifeMax;
				}

				if ( gRandomStatsValue[cnt].RandomAgility == TRUE )
					pProfile->bAgility		= RandomAbsoluteRange( pProfile->bAgility, 1, 100, Stats, Type );
				
				if ( gRandomStatsValue[cnt].RandomDexterity == TRUE )
					pProfile->bDexterity	= RandomAbsoluteRange( pProfile->bDexterity, 1, 100, Stats, Type );
					
				if ( gRandomStatsValue[cnt].RandomStrength == TRUE )
					pProfile->bStrength		= RandomAbsoluteRange( pProfile->bStrength, 1, 100, Stats, Type );
					
				if ( gRandomStatsValue[cnt].RandomLeadership == TRUE )
					pProfile->bLeadership	= RandomAbsoluteRange( pProfile->bLeadership, 1, 100, Stats, Type );
					
				if ( gRandomStatsValue[cnt].RandomWisdom == TRUE )
					pProfile->bWisdom		= RandomAbsoluteRange( pProfile->bWisdom, 1, 100, Stats, Type );
					
				if ( gRandomStatsValue[cnt].RandomMarksmanship == TRUE )
					pProfile->bMarksmanship	= RandomAbsoluteRange( pProfile->bMarksmanship, 1, 100, Stats, Type );
					
				if ( gRandomStatsValue[cnt].RandomMechanical == TRUE )
					pProfile->bMechanical	= RandomAbsoluteRange( pProfile->bMechanical, 1, 100, Stats, Type );
					
				if ( gRandomStatsValue[cnt].RandomExplosive == TRUE )	
					pProfile->bExplosive	= RandomAbsoluteRange( pProfile->bExplosive, 1, 100, Stats, Type );
					
				if ( gRandomStatsValue[cnt].RandomMedical == TRUE )
					pProfile->bMedical		= RandomAbsoluteRange( pProfile->bMedical, 1, 100, Stats, Type );
			}
		}
	}

	// Buggler: tweaked Jazz's random code
	else if ( gGameExternalOptions.ubMercRandomStats == 3 )
	{
		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			if ( gRandomStatsValue[cnt].Enabled )
			{
				pProfile = &(gMercProfiles[cnt]);

				if ( gRandomStatsValue[cnt].RandomExpLevel == TRUE )
					pProfile->bExpLevel		= RandomAbsoluteRange( pProfile->bExpLevel, 1, 10, Exp, Type );

				bBaseAttribute = gRandomStatsValue[cnt].BaseAttribute + ( 4 * pProfile->bExpLevel );

				if ( gRandomStatsValue[cnt].RandomLife == TRUE )
				{
					pProfile->bLifeMax = (bBaseAttribute + Random( 9 ) + Random( 8 ));
					pProfile->bLife = pProfile->bLifeMax;
				}

				if ( gRandomStatsValue[cnt].RandomAgility == TRUE )
				{
					pProfile->bAgility = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomDexterity == TRUE )
				{
					pProfile->bDexterity = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomStrength == TRUE )
				{
					pProfile->bStrength = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomLeadership == TRUE )
				{
					pProfile->bLeadership = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomWisdom == TRUE )
				{
					pProfile->bWisdom = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomMarksmanship == TRUE )
				{
					pProfile->bMarksmanship = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomMechanical == TRUE )
				{
					pProfile->bMechanical = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomExplosive == TRUE )
				{
					pProfile->bExplosive = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}

				if ( gRandomStatsValue[cnt].RandomMedical == TRUE )
				{
					pProfile->bMedical = (bBaseAttribute + Random( 9 ) + Random( 8 ));
				}
			}
		}
	}
	// Jenilee: 4th method (see also RandomMerc.cpp)
	else if (gGameExternalOptions.ubMercRandomStats == 4)
	{
		InitRandomMercs();

		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			if (IsProfileIdAnAimOrMERCMerc(cnt)) //affect only AIM and MERC mercs
			{
				pProfile = &(gMercProfiles[cnt]);
				RandomizeMerc(cnt, pProfile, gGameExternalOptions.fMercRandomGearKits);
			}
		}

		ExitRandomMercs();	}
}

void RandomGrowthModifiers()
{
	UINT32 cnt;
	MERCPROFILESTRUCT * pProfile;
	BOOLEAN useBellCurve = TRUE;

	 if (gGameExternalOptions.fMercRandomGrowthModifiers == TRUE) // rftr todo - add settings
	{
		for (cnt = 0; cnt < NUM_PROFILES; cnt++)
		{
			pProfile = &(gMercProfiles[cnt]);

			if (pProfile->bGrowthModifierExpLevel > 0)
				pProfile->bGrowthModifierExpLevel		= RandomAbsoluteRange( pProfile->bGrowthModifierExpLevel, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierExpLevel < 0)
				pProfile->bGrowthModifierExpLevel		= RandomAbsoluteRange( pProfile->bGrowthModifierExpLevel, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierLife > 0)
				pProfile->bGrowthModifierLife			= RandomAbsoluteRange( pProfile->bGrowthModifierLife, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierLife < 0)
				pProfile->bGrowthModifierLife			= RandomAbsoluteRange( pProfile->bGrowthModifierLife, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierAgility > 0)
				pProfile->bGrowthModifierAgility		= RandomAbsoluteRange( pProfile->bGrowthModifierAgility, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierAgility < 0)
				pProfile->bGrowthModifierAgility		= RandomAbsoluteRange( pProfile->bGrowthModifierAgility, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierDexterity > 0)
				pProfile->bGrowthModifierDexterity		= RandomAbsoluteRange( pProfile->bGrowthModifierDexterity, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierDexterity < 0)
				pProfile->bGrowthModifierDexterity		= RandomAbsoluteRange( pProfile->bGrowthModifierDexterity, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierStrength > 0)
				pProfile->bGrowthModifierStrength		= RandomAbsoluteRange( pProfile->bGrowthModifierStrength, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierStrength < 0)
				pProfile->bGrowthModifierStrength		= RandomAbsoluteRange( pProfile->bGrowthModifierStrength, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierLeadership > 0)
				pProfile->bGrowthModifierLeadership		= RandomAbsoluteRange( pProfile->bGrowthModifierLeadership, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierLeadership < 0)
				pProfile->bGrowthModifierLeadership		= RandomAbsoluteRange( pProfile->bGrowthModifierLeadership, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierWisdom > 0)
				pProfile->bGrowthModifierWisdom			= RandomAbsoluteRange( pProfile->bGrowthModifierWisdom, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierWisdom < 0)
				pProfile->bGrowthModifierWisdom			= RandomAbsoluteRange( pProfile->bGrowthModifierWisdom, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierMarksmanship > 0)
				pProfile->bGrowthModifierMarksmanship	= RandomAbsoluteRange( pProfile->bGrowthModifierMarksmanship, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierMarksmanship < 0)
				pProfile->bGrowthModifierMarksmanship	= RandomAbsoluteRange( pProfile->bGrowthModifierMarksmanship, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierMechanical > 0)
				pProfile->bGrowthModifierMechanical		= RandomAbsoluteRange( pProfile->bGrowthModifierMechanical, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierMechanical < 0)
				pProfile->bGrowthModifierMechanical		= RandomAbsoluteRange( pProfile->bGrowthModifierMechanical, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierExplosive > 0)
				pProfile->bGrowthModifierExplosive		= RandomAbsoluteRange( pProfile->bGrowthModifierExplosive, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierExplosive < 0)
				pProfile->bGrowthModifierExplosive		= RandomAbsoluteRange( pProfile->bGrowthModifierExplosive, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );

			if (pProfile->bGrowthModifierMedical > 0)
				pProfile->bGrowthModifierMedical		= RandomAbsoluteRange( pProfile->bGrowthModifierMedical, 0, 500, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
			else if (pProfile->bGrowthModifierMedical < 0)
				pProfile->bGrowthModifierMedical		= RandomAbsoluteRange( pProfile->bGrowthModifierMedical, -500, 0, gGameExternalOptions.iMercRandomGrowthModifiersRange, useBellCurve );
		}
	}
}

void RandomStartSalary()
{
	UINT32 cnt;
	MERCPROFILESTRUCT * pProfile;

	// Buggler: random starting salary
	if ( gGameExternalOptions.fMercRandomStartSalary == TRUE )
	{
		UINT8 SalaryPercentMod = gGameExternalOptions.ubMercRandomStartSalaryPercentMod;
		BOOLEAN Type = gGameExternalOptions.fMercRandomBellDistribution;
		FLOAT SalaryMod;

		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			pProfile = &(gMercProfiles[cnt]);
			SalaryMod =  1 + (FLOAT) RandomPercentRange( SalaryPercentMod, Type ) / 100;
			// random non-zero salary 
			if ( pProfile->sSalary |= 0 )
				pProfile->sSalary = RoundOffSalary( (UINT32)( pProfile->sSalary * SalaryMod ) );
			if ( pProfile->uiWeeklySalary |= 0 )
				pProfile->uiWeeklySalary = RoundOffSalary( (UINT32)( pProfile->uiWeeklySalary * SalaryMod ) );
			if ( pProfile->uiBiWeeklySalary |= 0 )
				pProfile->uiBiWeeklySalary = RoundOffSalary( (UINT32)( pProfile->uiBiWeeklySalary * SalaryMod ) );
			if ( pProfile->sTrueSalary |= 0 )
				pProfile->sTrueSalary = RoundOffSalary( (UINT32)( pProfile->sTrueSalary * SalaryMod ) );
		}
	}
}

INT32 RandomAbsoluteRange( INT32 iValue, INT32 iMin, INT32 iMax, INT32 iRange, BOOLEAN fBellCurve )
{
	INT32 iRValue;
	
	if (iMin > iMax)
		Assert(0);
	
	//no random if value at or out of random limits
	if ( iValue <= iMin || iValue >= iMax )
		return( iValue );

	//reduce random range due to the min limits
	if ( iRange > iValue - iMin )
		iRange = iValue - iMin;
	
	//reduce random range due to the max limits
	if ( iRange > iMax - iValue )
		iRange = iMax - iValue;

	//bell curve random distribution
	if ( fBellCurve )
		iRValue = iValue - iRange + Random( iRange + 1 ) + Random( iRange + 1 );
	
	//uniform random distribution
	else
		iRValue = iValue - iRange + Random( 2 * iRange + 1 );

	//calculated random value
	return( iRValue );
}

INT8 RandomPercentRange( UINT8 iPRange, BOOLEAN fBellCurve )
{
	INT8 bRPValue;

	if (iPRange < 0 || iPRange > 100 )
		Assert(0);

	//bell curve random distribution
	if ( fBellCurve )
		bRPValue = Random ( iPRange + 1 ) + Random ( iPRange + 1 ) - iPRange;
	
	//uniform random distribution
	else
		bRPValue = Random ( 2 * iPRange + 1 ) - iPRange;
		
	//percent range -100 to 100
	return( bRPValue );
}

// WANNE - BMP: DONE!
BOOLEAN LoadMercProfiles(void)
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"LoadMercProfiles");
//	FILE *fptr;
	HWFILE fptr;
	STR8 pFileName = "BINARYDATA\\Prof.dat";
#ifdef JA2UB
	STR8 pFileName_UB = "BINARYDATA\\JA25PROF.DAT";
#endif
	STR8 pFileName1_Normal = "BINARYDATA\\Prof_Novice_NormalGuns.dat";
	STR8 pFileName2_Normal = "BINARYDATA\\Prof_Experienced_NormalGuns.dat";
	STR8 pFileName3_Normal = "BINARYDATA\\Prof_Expert_NormalGuns.dat";
	STR8 pFileName4_Normal = "BINARYDATA\\Prof_Insane_NormalGuns.dat";

	STR8 pFileName1_Tons = "BINARYDATA\\Prof_Novice_TonsOfGuns.dat";
	STR8 pFileName2_Tons = "BINARYDATA\\Prof_Experienced_TonsOfGuns.dat";
	STR8 pFileName3_Tons = "BINARYDATA\\Prof_Expert_TonsOfGuns.dat";
	STR8 pFileName4_Tons = "BINARYDATA\\Prof_Insane_TonsOfGuns.dat";

	UINT32 uiLoop, uiLoop2;//, uiLoop3;
	UINT16 usItem;//, usNewGun, usAmmo, usNewAmmo;
		
	if (gGameExternalOptions.fUseDifficultyBasedProfDat == TRUE)
	{
		switch ( gGameOptions.ubDifficultyLevel)
		{
			case DIF_LEVEL_EASY:
				if ( gGameOptions.fGunNut )
					fptr = FileOpen(pFileName1_Tons, FILE_ACCESS_READ, FALSE );
				else
					fptr = FileOpen(pFileName1_Normal, FILE_ACCESS_READ, FALSE );
				break;
			case DIF_LEVEL_MEDIUM:
				if ( gGameOptions.fGunNut )
					fptr = FileOpen(pFileName2_Tons, FILE_ACCESS_READ, FALSE );
				else
					fptr = FileOpen(pFileName2_Normal, FILE_ACCESS_READ, FALSE );
				break;
			case DIF_LEVEL_HARD:
				if ( gGameOptions.fGunNut )
					fptr = FileOpen(pFileName3_Tons, FILE_ACCESS_READ, FALSE );
				else
					fptr = FileOpen(pFileName3_Normal, FILE_ACCESS_READ, FALSE );
				break;
			case DIF_LEVEL_INSANE:
				if ( gGameOptions.fGunNut )
					fptr = FileOpen(pFileName4_Tons, FILE_ACCESS_READ, FALSE );
				else
					fptr = FileOpen(pFileName4_Normal, FILE_ACCESS_READ, FALSE );
				break;
			default:
				fptr = FileOpen(pFileName, FILE_ACCESS_READ, FALSE );
				break;
		}
	}
	else
	{

#ifdef JA2UB
		fptr = FileOpen(pFileName_UB, FILE_ACCESS_READ, FALSE );   //ub
		if( !fptr )
		{
			fptr = FileOpen(pFileName, FILE_ACCESS_READ, FALSE );   //ja
		}
#else
		fptr = FileOpen(pFileName, FILE_ACCESS_READ, FALSE );
#endif

	}
	
	if( !fptr )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to LoadMercProfiles from file %s", pFileName) );
		return(FALSE);
	}

	// Reset
	for( int uiProfileId = 0; uiProfileId < NUM_PROFILES; ++uiProfileId )
	{
		gProfileType[uiProfileId] = PROFILETYPE_NONE;
	}

	int profileIndex = -1;

	for(uiLoop=0; uiLoop< NUM_PROFILES; ++uiLoop)
	{
		++profileIndex;

		// Changed by ADB, rev 1513
		//if( !gMercProfiles[uiLoop].Load(fptr, true))
		//if( !gMercProfiles[uiLoop].Load(fptr, true, true, true))
		if( uiLoop < 170 && !gMercProfiles[uiLoop].Load(fptr, true, true, true))
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Read Merc Profiles from File %d %s",uiLoop, pFileName) );
			FileClose( fptr );
			return(FALSE);
		}

		/// Flugente: until the introduction of a separate varriable for the voiceset, the voice was identical with the slot
		// as we are loading from a .dat file, we always have to account for that
		{
			gMercProfiles[uiLoop].usVoiceIndex = uiLoop;
		}
		
		// WANNE: For the new WF merc, there is no entry in prof.dat, so we have to reset some flags manually!		
		if (uiLoop >= 170)
		{
			gMercProfiles[uiLoop].initialize();
			
			// Flugente: as this data is not in the xml, we set dummy values
			if ( gMercProfiles[uiLoop].ubApproachVal[0] == 0 &&
				gMercProfiles[uiLoop].ubApproachVal[1] == 0 &&
				gMercProfiles[uiLoop].ubApproachVal[2] == 0 &&
				gMercProfiles[uiLoop].ubApproachVal[3] == 0 )
			{
				for ( int i = 0; i < 4; ++i )
				{
					// with ubApproachVal of 50 and ubApproachMod of 100, approach value towards an NPC will be the merc's effective leadership, which seems like a reasonable default
					gMercProfiles[uiLoop].ubApproachVal[i] = 50;

					for ( int j = 0; j < 3; ++j )
					{
						gMercProfiles[uiLoop].ubApproachMod[j][i] = 100;
					}
				}
			}
		}

		// WANNE - BMP: DONE!
		//<SB> convert old MERCPROFILESTRUCT to new MERCPROFILESTRUCT
		gMercProfiles[uiLoop].sGridNo = gMercProfiles[uiLoop]._old_sGridNo;
		gMercProfiles[uiLoop].sPreCombatGridNo = gMercProfiles[uiLoop]._old_sGridNo;
		//</SB>

		/* silversurfer: READ_PROFILE_DATA_FROM_XML from JA2.Options.ini should always take precedence.
		CHRISL: For now, we should only overwrite prof.dat in the new inventory system.  Old system should still use
		prof.dat until we're sure we want to replace it with the xml file.
			Because the new WF mercs don't have entries in the prof*.dat files, we need to always load their equipment from
			MercStaringGear.xml, regardless of the inventory system we're going to use.*/
		if( gGameExternalOptions.fReadProfileDataFromXML || UsingNewInventorySystem() == true || uiLoop >= 170 )
		{
			// Start by resetting all profile inventory values to 0
			gMercProfiles[uiLoop].clearInventory();
			gMercProfiles[uiLoop].ubInvUndroppable = 0;
			// Next, go through and assign everything but lbe gear
			for(uiLoop2=INV_START_POS; uiLoop2<NUM_INV_SLOTS; uiLoop2++)
			{
				if(gMercProfileGear[uiLoop][0].inv[uiLoop2] != NONE)
				{
					gMercProfiles[uiLoop].inv[uiLoop2] = gMercProfileGear[uiLoop][0].inv[uiLoop2];
					gMercProfiles[uiLoop].bInvStatus[uiLoop2] = gMercProfileGear[uiLoop][0].iStatus[uiLoop2];
					if(uiLoop2 > 5)
						gMercProfiles[uiLoop].bInvNumber[uiLoop2] = gMercProfileGear[uiLoop][0].iNumber[uiLoop2];
					else
						gMercProfiles[uiLoop].bInvNumber[uiLoop2] = 1;
				}
				//CHRISL: Moved outside first condition we we set ubInvUndroppable regardless of having an item
				if(gMercProfileGear[uiLoop][0].iDrop[uiLoop2] == 0 && uiLoop > 56){
					gMercProfiles[uiLoop].ubInvUndroppable |= gubItemDroppableFlag[uiLoop2];
				}
			}
			// Last, go through and assign LBE items.  Only needed for new inventory system
			if((UsingNewInventorySystem() == true))
			{
				for(uiLoop2=0; uiLoop2<5; uiLoop2++)
				{
					UINT32 uiLoop3 = uiLoop2 + VESTPOCKPOS;
					if(gMercProfileGear[uiLoop][0].lbe[uiLoop2] != NONE){
						gMercProfiles[uiLoop].inv[uiLoop3] = gMercProfileGear[uiLoop][0].lbe[uiLoop2];
						gMercProfiles[uiLoop].bInvStatus[uiLoop3] = gMercProfileGear[uiLoop][0].lStatus[uiLoop2];
						gMercProfiles[uiLoop].bInvNumber[uiLoop3] = 1;
					}
				}
			}
		}

		//if the Dialogue exists for the merc, allow the merc to be hired
		if ( DialogueDataFileExistsForProfile( gMercProfiles[uiLoop].usVoiceIndex, 0, FALSE, NULL ) )
		{
			gMercProfiles[uiLoop].bMercStatus = 0;
		}
		else
			gMercProfiles[uiLoop].bMercStatus = MERC_HAS_NO_TEXT_FILE;

		// if the merc has a medical deposit
		if( gMercProfiles[ uiLoop ].bMedicalDeposit )
		{
			gMercProfiles[uiLoop].sMedicalDepositAmount = CalcMedicalDeposit( &gMercProfiles[uiLoop]);
		}
		else
			gMercProfiles[uiLoop].sMedicalDepositAmount = 0;

		// ATE: New, face display indipendent of ID num now
		// Setup face index value
		// Default is the ubCharNum
		gMercProfiles[uiLoop].ubFaceIndex = (UINT8)uiLoop;

		//ATE: Calculate some inital attractiveness values for buddy's inital equipment...
		// Look for gun and armour
		gMercProfiles[uiLoop].bMainGunAttractiveness		= -1;
		gMercProfiles[uiLoop].bArmourAttractiveness			= -1;

		UINT32 invsize = gMercProfiles[ uiLoop ].inv.size();
		for ( uiLoop2 = 0; uiLoop2 < invsize; ++uiLoop2 )
		{
			usItem = gMercProfiles[uiLoop].inv[ uiLoop2 ];

			if ( usItem != NOTHING )
			{
				// Check if it's a gun
				if ( Item[ usItem ].usItemClass & IC_GUN )
				{
					gMercProfiles[uiLoop].bMainGunAttractiveness = Weapon[ usItem ].ubDeadliness;
				}

				// If it's armour
				if ( Item[ usItem ].usItemClass & IC_ARMOUR )
				{
					gMercProfiles[uiLoop].bArmourAttractiveness = min(128,Armour[ Item[ usItem ].ubClassIndex ].ubProtection);
				}
			}
		}	

		//These variables to get loaded in
		gMercProfiles[ uiLoop ].fUseProfileInsertionInfo = FALSE;
		gMercProfiles[ uiLoop ].sGridNo = 0;

		// ARM: this is also being done inside the profile editor, but put it here too, so this project's code makes sense
		gMercProfiles[ uiLoop ].bHatedCount[0]	= gMercProfiles[ uiLoop ].bHatedTime[0];
		gMercProfiles[ uiLoop ].bHatedCount[1]	= gMercProfiles[ uiLoop ].bHatedTime[1];
		gMercProfiles[ uiLoop ].bHatedCount[2]	= gMercProfiles[ uiLoop ].bHatedTime[2];
		gMercProfiles[ uiLoop ].bHatedCount[3]	= gMercProfiles[ uiLoop ].bHatedTime[3];
		gMercProfiles[ uiLoop ].bHatedCount[4]	= gMercProfiles[ uiLoop ].bHatedTime[4];
		gMercProfiles[ uiLoop ].bLearnToHateCount = gMercProfiles[ uiLoop ].bLearnToHateTime;
		gMercProfiles[ uiLoop ].bLearnToLikeCount = gMercProfiles[ uiLoop ].bLearnToLikeTime;

		if (gGameExternalOptions.fReadProfileDataFromXML)
		{
			// HEADROCK PROFEX: Overwrite data read from PROF.DAT with data read from XML
			OverwriteMercProfileWithXMLData( uiLoop );
			OverwriteMercOpinionsWithXMLData( uiLoop );
		}
		
		//tais: new tag in gearkit that sets an absolute price for gearkit that will override item value and price modifier if it's a sensible value between 0 and 32000
		if(gMercProfileGear[uiLoop][0].AbsolutePrice >= 0 && gMercProfileGear[uiLoop][0].AbsolutePrice <= 32000)
		{
			gMercProfiles[ uiLoop ].usOptionalGearCost = gMercProfileGear[uiLoop][0].AbsolutePrice;
		}
		else
		{
			//tais: moved initial price calculation down to this spot because PROFEX overwrites my gearkit prices
			//with old binary file optional gear prices which got ported into MercProfiles.xml
			gMercProfiles[ uiLoop ].usOptionalGearCost = 0;
			UINT16 tempGearCost = 0;
			for ( uiLoop2 = 0; uiLoop2< invsize; ++uiLoop2 )
			{
				if ( gMercProfiles[ uiLoop ].inv[ uiLoop2 ] != NOTHING )
				{
					//get the item
					usItem = gMercProfiles[ uiLoop ].inv[ uiLoop2 ];

					// for an item stack, we obviously need to account fot the number of items
					// for single items, the number is not always set, so just to be sure...
					// initialise to first gearkit
					int number = 1;
					if ( gMercProfileGear[uiLoop][0].iNumber.size( ) >= uiLoop2 )
						number = max( 1, gMercProfileGear[uiLoop][0].iNumber[uiLoop2] );

					//add the cost
					tempGearCost += (Item[ usItem ].usPrice * number);
				}
			}
			//tais: added optional price modifier for gearkits, reads the xml tag mPriceMod from MercStartingGear.xml
			if(gMercProfileGear[uiLoop][0].PriceModifier != 0 &&
				gMercProfileGear[uiLoop][0].PriceModifier <= 200 &&
				gMercProfileGear[uiLoop][0].PriceModifier >= -100)
			{
				FLOAT mod;
				mod = (FLOAT) (gMercProfileGear[uiLoop][0].PriceModifier + 100) / 100.0f;
				gMercProfiles[ uiLoop ].usOptionalGearCost = (UINT16)(tempGearCost * mod);
			}
			else
			{
				gMercProfiles[ uiLoop ].usOptionalGearCost = tempGearCost;
			}
		}

		// ----- WANNE.PROFILE: New Profile Loading - BEGIN
		if ( gGameExternalOptions.fReadProfileDataFromXML == FALSE )
		{	
			// WANNE: If all 4 (Friendly, Direct, Threaten, Recruit) are not set, set them to 100.
			// These 4 values cannot be edit in Proedit if Merc Index >= 75.
			// If we set these to 100 (which means it is equal the leadership of the merc), we fix the bug that the 4 UB Merc (Stogie, Tex, Gaston, Biggins) cannot recruit NPCs or do special quests).
			if (uiLoop >= FIRST_NPC)
			{
				if (gMercProfiles[ uiLoop ].usApproachFactor[0] == 0 &&
					gMercProfiles[ uiLoop ].usApproachFactor[1] == 0 &&
					gMercProfiles[ uiLoop ].usApproachFactor[2] == 0 &&
					gMercProfiles[ uiLoop ].usApproachFactor[3] == 0)
				{
					gMercProfiles[ uiLoop ].usApproachFactor[0] = 100;
					gMercProfiles[ uiLoop ].usApproachFactor[1] = 100;
					gMercProfiles[ uiLoop ].usApproachFactor[2] = 100;
					gMercProfiles[ uiLoop ].usApproachFactor[3] = 100;
				}
			}
			
			// AIM und MERC ( 0 - 51 )
			if (uiLoop < 51)
			{
				// AIM
				if (uiLoop < 40)
				{
					gProfileType[uiLoop] = PROFILETYPE_AIM;
				}
				// MERC
				else
				{
					gProfileType[uiLoop] = PROFILETYPE_MERC;
				}			
			}
			// IMP (51 - 56)
			else if (uiLoop >= 51 && uiLoop < FIRST_RPC)
			{
				gProfileType[uiLoop] = PROFILETYPE_IMP;
			}
			else
			{
				// Vehicle, MERC (default 1.13) or RPC?
				if (uiLoop >= 165 && uiLoop <= 168)
				{
					switch (gMercProfiles[ uiLoop ].ubBodyType)
					{
						// MERC
						case REGMALE:
						case BIGMALE:
						case REGFEMALE:
							gProfileType[uiLoop] = PROFILETYPE_MERC;
							break;
						// Vehicle
						case HUMVEE:
						case TANK_NW:
						case TANK_NE:
						case ELDORADO:
						case ICECREAMTRUCK:
						case JEEP:
						case COMBAT_JEEP:
							gProfileType[uiLoop] = PROFILETYPE_VEHICLE;
							break;
						// Make it an RPC
						default:
							gProfileType[uiLoop] = PROFILETYPE_RPC;
							break;
					}
				}
				else
				{
					// Last Index -> NPC
					if (uiLoop == 169)
					{
						gProfileType[uiLoop] = PROFILETYPE_NPC;
					}
					// NPC, RPC or Vehicle
					else
					{
						switch (gMercProfiles[ uiLoop ].ubBodyType)
						{
							// Vehicle
							case HUMVEE:
							case TANK_NW:
							case TANK_NE:
							case ELDORADO:
							case ICECREAMTRUCK:
							case JEEP:
							case COMBAT_JEEP:
								gProfileType[uiLoop] = PROFILETYPE_VEHICLE;
								break;
							// RPC or NPC
							default:
								// RPC
								if (uiLoop >= FIRST_RPC && uiLoop < FIRST_NPC)
								{
									gProfileType[uiLoop] = PROFILETYPE_RPC;
								}
								// NPC
								else
								{
									gProfileType[uiLoop] = PROFILETYPE_NPC;
								}
								break;
						}
					}
				}
			}
		}
	}
	
	// ----- WANNE.PROFILE: New Profile Loading - END

	// SET SOME DEFAULT LOCATIONS FOR STARTING NPCS
	
	// Reset
	for (int i = 0; i < NUM_PROFILES; i++)
	{
		AimMercArray[i] = -1;
	}

	FileClose( fptr );
	//WriteMercStartingGearStats();
	
//--------------
/*
CHAR8						fileName[255];

	//JA25 UB
for( int i = 0; i < NUM_PROFILES; i++ )
	{
	strcpy(fileName, "TABLEDATA\\Profile\\prof03.xml");
	
	//sprintf( fileName, "TABLEDATA\\Profile\\prof%03d.xml", i );
	
	if ( FileExists(fileName) )
	{
		ReadInMercProfiles(fileName,FALSE,i,TRUE);
	}

	}
*/
//--------------

	// ---------------

	UINT16 maxAIMProfiles = 0;
	if (!gGameExternalOptions.fReadProfileDataFromXML)
		maxAIMProfiles = 40;
	else
		maxAIMProfiles = NUM_PROFILES;

	UINT32 x = 0;
	UINT32 i = 0;
	MAX_NUMBER_MERCS = 0;		
	START_MERC = 0;

	// Find all AIM merc and add them in the AimMercArray which is used for the AIM webpage
	for (i = 0; i < maxAIMProfiles; i++)
	{
		if (gAimAvailability[i].ProfilId != 255)
		{
			AimMercArray[ x ] = gAimAvailability[i].ProfilId;
			x++;
			MAX_NUMBER_MERCS++;
		}
	}

	// ---------------
		
	RandomStats (); //random stats by Jazz

	RandomGrowthModifiers();
	
	// Buggler: random starting salary
	RandomStartSalary ();

	// decide which terrorists are active
	DecideActiveTerrorists();

	// initialize mercs' status
	if(!is_networked)StartSomeMercsOnAssignment( );

	// initial recruitable mercs' reputation in each town
	InitializeProfilesForTownReputation( );

	#ifdef JA2EDITOR
	gfProfileDataLoaded = TRUE;
	#endif

	// no better place..heh?.. will load faces for profiles that are 'extern'.....won't have soldiertype instances
	InitalizeStaticExternalNPCFaces( );

	// car portrait values
	LoadCarPortraitValues( );


	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"LoadMercProfiles done");
	return(TRUE);
}

#define MAX_ADDITIONAL_TERRORISTS 5 // instead of 4

void DecideActiveTerrorists( void )
{
	UINT8 ubLoop, ubLoop2;
	UINT8		ubTerrorist;
	UINT8		ubNumAdditionalTerrorists, ubNumTerroristsAdded = 0;
	UINT32	uiChance, uiLocationChoice;
	BOOLEAN	fFoundSpot;
	INT16		sTerroristPlacement[MAX_ADDITIONAL_TERRORISTS][2] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} }; // added one here

	#ifdef CRIPPLED_VERSION
		return;
	#endif

	// one terrorist will always be Elgin
	// determine how many more terrorists - 2 to 4 more

	// using this stochastic process(!), the chances for terrorists are:
	// EASY:		3, 9%			4, 42%		5, 49%
	// MEDIUM:	3, 25%		4, 50%		5, 25%
	// HARD:		3, 49%		4, 42%		5, 9%
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			uiChance = 70;
			break;
		case DIF_LEVEL_INSANE:
			uiChance = 30;
			break;
		default:
			uiChance = 50;
			break;
	}

	// add at least 2 more
	ubNumAdditionalTerrorists = 2;
	for (ubLoop = 0; ubLoop < (MAX_ADDITIONAL_TERRORISTS - 3); ubLoop++) // -3 instead of -2  because we increased MAX_ADDITIONAL_TERRORISTS above by 1
	{
		if (Random( 100 ) < uiChance)
		{
			ubNumAdditionalTerrorists++;
		}
	}

	/////////////////////////////////////////////////////
	// Added, so with ENABLE_ALL_TERRORISTS you really get all of them  (5 + Charlie)
	if ( gGameExternalOptions.fEnableAllTerrorists )
		ubNumAdditionalTerrorists = 5;
	/////////////////////////////////////////////////////

	// ifdefs added by CJC
	#ifdef JA2TESTVERSION
		ubNumAdditionalTerrorists = 5;
	#endif

	// silversurfer: this was causing an infinite loop at game start when someone manually placed terrorists using ProEdit and thus 
	// causing them to become invalid for random location choosing
//	while ( ubNumTerroristsAdded < ubNumAdditionalTerrorists )
//	{

		ubLoop = 1; // start at beginning of array (well, after Elgin)

		// NB terrorist ID of 0 indicates end of array
		while ( ubNumTerroristsAdded < ubNumAdditionalTerrorists && gubTerrorists[ ubLoop ] != 0 )
		{

			ubTerrorist = gubTerrorists[ ubLoop ];

			// place randomly if not yet placed
			if ( gMercProfiles[ ubTerrorist ].sSectorX == 0 )
			{
				// random 40% chance of adding this terrorist if not yet placed or place all
				if ( ( Random( 100 ) < 40 ) || gGameExternalOptions.fEnableAllTerrorists )
				{
					//fFoundSpot = FALSE;
					// Since there are 5 spots per terrorist and a maximum of 5 terrorists, we
					// are guaranteed to be able to find a spot for each terrorist since there
					// aren't enough other terrorists to use up all the spots for any one
					// terrorist
					do
					{
						fFoundSpot = TRUE;

						// pick a random spot, see if it's already been used by another terrorist
						uiLocationChoice = Random( NUM_TERRORIST_POSSIBLE_LOCATIONS );
						for (ubLoop2 = 0; ubLoop2 < ubNumTerroristsAdded; ubLoop2++)
						{
							if (sTerroristPlacement[ubLoop2][0] == gsTerroristSector[ubLoop][uiLocationChoice][0] )
							{
								if (sTerroristPlacement[ubLoop2][1] == gsTerroristSector[ubLoop][uiLocationChoice][1] )
								{
									// WANNE: Fix a vanilla bug: Due to a logic bug multiple terrorists could end up in the same sector.
									// Fixed by Tron (Straciatella): Revision: 6932
									fFoundSpot = FALSE;
									break;
									//continue;
								}
							}
						}
						//fFoundSpot = TRUE;
					} while( !fFoundSpot );

					// place terrorist!
					gMercProfiles[ ubTerrorist ].sSectorX = gsTerroristSector[ ubLoop ][ uiLocationChoice ][ 0 ];
					gMercProfiles[ ubTerrorist ].sSectorY = gsTerroristSector[ ubLoop ][ uiLocationChoice ][ 1 ];
					gMercProfiles[ ubTerrorist ].bSectorZ = 0;
					sTerroristPlacement[ ubNumTerroristsAdded ][ 0 ] = gMercProfiles[ ubTerrorist ].sSectorX;
					sTerroristPlacement[ ubNumTerroristsAdded ][ 1 ] = gMercProfiles[ ubTerrorist ].sSectorY;
					ubNumTerroristsAdded++;
				}
			}
			else
			{
				// this terrorist has a fixed location in MercProfiles.xml
				sTerroristPlacement[ ubNumTerroristsAdded ][ 0 ] = gMercProfiles[ ubTerrorist ].sSectorX;
				sTerroristPlacement[ ubNumTerroristsAdded ][ 1 ] = gMercProfiles[ ubTerrorist ].sSectorY;
				ubNumTerroristsAdded++;
			}
			ubLoop++;
		}

		// start over if necessary
//	}

	// set total terrorists outstanding in Carmen's info byte
	gMercProfiles[ 78 ].bNPCData = 1 + ubNumTerroristsAdded;  //ubNumAdditionalTerrorists; 
	// silversurfer: only use the number of terrorist assigned through this function
	// If someone manually placed a terrorist in an invalid sector it is his problem that he will not be able to solve the quest.

	// store total terrorists
	gubNumTerrorists = 1 + ubNumTerroristsAdded;  //ubNumAdditionalTerrorists;
}

void MakeRemainingTerroristsTougher( void )
{
	UINT8					ubRemainingTerrorists = 0, ubLoop;
	UINT16				usNewItem, usOldItem;
	UINT8					ubRemainingDifficulty;

	for ( ubLoop = 0; ubLoop < NUM_TERRORISTS; ubLoop++ )
	{
		if ( gMercProfiles[ gubTerrorists[ ubLoop ] ].bMercStatus != MERC_IS_DEAD && gMercProfiles[ gubTerrorists[ ubLoop ] ].sSectorX != 0 && gMercProfiles[ gubTerrorists[ ubLoop ] ].sSectorY != 0 )
		{
#ifdef JA2UB
//no Ub
#else
			if ( gubTerrorists[ ubLoop ] == SLAY )
			{
				if ( FindSoldierByProfileID( SLAY, TRUE ) != NULL )
				{
					// Slay on player's team, doesn't count towards remaining terrorists
					continue;
				}
			}
#endif
			ubRemainingTerrorists++;
		}
	}

	ubRemainingDifficulty = (60 / gubNumTerrorists) * (gubNumTerrorists - ubRemainingTerrorists);

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_MEDIUM:
			ubRemainingDifficulty = (ubRemainingDifficulty * 13) / 10;
			break;
		case DIF_LEVEL_HARD:
			ubRemainingDifficulty = (ubRemainingDifficulty * 16) / 10;
			break;
		case DIF_LEVEL_INSANE:
			ubRemainingDifficulty = (ubRemainingDifficulty * 19) / 10;
			break;
		default:
			break;
	}

	if ( ubRemainingDifficulty < 14 )
	{
		// nothing
		return;
	}
	else if ( ubRemainingDifficulty < 28 )
	{
		// mini grenade
		usOldItem = NOTHING;
		usNewItem = MINI_GRENADE;
	}
	else if ( ubRemainingDifficulty < 42)
	{
		// hand grenade
		usOldItem = MINI_GRENADE;
		usNewItem = HAND_GRENADE;
	}
	else if ( ubRemainingDifficulty < 56)
	{
		// mustard
		usOldItem = HAND_GRENADE;
		usNewItem = MUSTARD_GRENADE;
	}
	else if ( ubRemainingDifficulty < 70)
	{
		// LAW
		usOldItem = MUSTARD_GRENADE;
		usNewItem = ROCKET_LAUNCHER;
	}
	else
	{
		// LAW and hand grenade
		usOldItem = NOTHING;
		usNewItem = HAND_GRENADE;
	}

	CreateItem(usNewItem, 100, &gTempObject);

	for ( ubLoop = 0; ubLoop < NUM_TERRORISTS; ubLoop++ )
	{
		if ( gMercProfiles[ gubTerrorists[ ubLoop ] ].bMercStatus != MERC_IS_DEAD && gMercProfiles[ gubTerrorists[ ubLoop ] ].sSectorX != 0 && gMercProfiles[ gubTerrorists[ ubLoop ] ].sSectorY != 0 )
		{
		
#ifdef JA2UB
// no UB
#else
			if ( gubTerrorists[ ubLoop ] == SLAY )
			{
				if ( FindSoldierByProfileID( SLAY, TRUE ) != NULL )
				{
					// Slay on player's team, doesn't count towards remaining terrorists
					continue;
				}
			}
#endif
			if ( usOldItem != NOTHING )
			{
				RemoveObjectFromSoldierProfile( gubTerrorists[ ubLoop ], usOldItem );
			}
			PlaceObjectInSoldierProfile( gubTerrorists[ ubLoop ], &gTempObject );
		}
	}
}

void DecideOnAssassin( void )
{
	UINT8		ubAssassinPossibility[NUM_ASSASSINS] = { NO_PROFILE, NO_PROFILE, NO_PROFILE, NO_PROFILE, NO_PROFILE, NO_PROFILE };
	UINT8		ubAssassinsPossible = 0;
	UINT8 ubLoop, ubLoop2;
	UINT8		ubTown;

	#ifdef CRIPPLED_VERSION
		return;
	#endif

	ubTown = GetTownIdForSector( gWorldSectorX, gWorldSectorY );

	for ( ubLoop = 0; ubLoop < NUM_ASSASSINS; ubLoop++ )
	{
		// make sure alive and not placed already
		if ( gMercProfiles[ gubAssassins[ ubLoop ] ].bMercStatus != MERC_IS_DEAD && gMercProfiles[ gubAssassins[ ubLoop ] ].sSectorX == 0 && gMercProfiles[ gubAssassins[ ubLoop ] ].sSectorY == 0 )
		{
			// check this merc to see if the town is a possibility
			for ( ubLoop2 = 0; ubLoop2 < NUM_ASSASSIN_POSSIBLE_TOWNS; ubLoop2++ )
			{
				if ( gbAssassinTown[ ubLoop ][ ubLoop2 ] == ubTown )
				{
					ubAssassinPossibility[ ubAssassinsPossible ] = gubAssassins[ ubLoop ];
					ubAssassinsPossible++;
				}
			}
		}
	}

	if ( ubAssassinsPossible != 0 )
	{
		ubLoop = ubAssassinPossibility[ Random( ubAssassinsPossible ) ];
		gMercProfiles[ ubLoop ].sSectorX = gWorldSectorX;
		gMercProfiles[ ubLoop ].sSectorY = gWorldSectorY;
		AddStrategicEvent( EVENT_REMOVE_ASSASSIN, GetWorldTotalMin() + 60 * ( 3 + Random( 3 ) ), ubLoop );
	}

}

void MakeRemainingAssassinsTougher( void )
{
	UINT8					ubRemainingAssassins = 0, ubLoop;
	UINT16				usNewItem, usOldItem;
	UINT8					ubRemainingDifficulty;

	for ( ubLoop = 0; ubLoop < NUM_ASSASSINS; ubLoop++ )
	{
		if ( gMercProfiles[ gubAssassins[ ubLoop ] ].bMercStatus != MERC_IS_DEAD	)
		{
			ubRemainingAssassins++;
		}
	}

	ubRemainingDifficulty = (60 / NUM_ASSASSINS) * (NUM_ASSASSINS - ubRemainingAssassins);

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_MEDIUM:
			ubRemainingDifficulty = (ubRemainingDifficulty * 13) / 10;
			break;
		case DIF_LEVEL_HARD:
			ubRemainingDifficulty = (ubRemainingDifficulty * 16) / 10;
			break;
		case DIF_LEVEL_INSANE:
			ubRemainingDifficulty = (ubRemainingDifficulty * 19) / 10;
			break;
		default:
			break;
	}

	if ( ubRemainingDifficulty < 14 )
	{
		// nothing
		return;
	}
	else if ( ubRemainingDifficulty < 28 )
	{
		// mini grenade
		usOldItem = NOTHING;
		usNewItem = MINI_GRENADE;
	}
	else if ( ubRemainingDifficulty < 42)
	{
		// hand grenade
		usOldItem = MINI_GRENADE;
		usNewItem = HAND_GRENADE;
	}
	else if ( ubRemainingDifficulty < 56)
	{
		// mustard
		usOldItem = HAND_GRENADE;
		usNewItem = MUSTARD_GRENADE;
	}
	else if ( ubRemainingDifficulty < 70)
	{
		// LAW
		usOldItem = MUSTARD_GRENADE;
		usNewItem = ROCKET_LAUNCHER;
	}
	else
	{
		// LAW and hand grenade
		usOldItem = NOTHING;
		usNewItem = HAND_GRENADE;
	}

	CreateItem(usNewItem, 100, &gTempObject);
	for ( ubLoop = 0; ubLoop < NUM_ASSASSINS; ubLoop++ )
	{
		if ( gMercProfiles[ gubAssassins[ ubLoop ] ].bMercStatus != MERC_IS_DEAD )
		{
			if ( usOldItem != NOTHING )
			{
				RemoveObjectFromSoldierProfile( gubAssassins[ ubLoop ], usOldItem );
			}
			PlaceObjectInSoldierProfile( gubAssassins[ ubLoop ], &gTempObject );
		}
	}
}

void StartSomeMercsOnAssignment(void)
{
	MERCPROFILESTRUCT *pProfile;
	UINT32 uiChance;

	// some randomly picked A.I.M. mercs will start off "on assignment" at the beginning of each new game
	for( UINT32 uiCnt = 0; uiCnt < NUM_PROFILES; ++uiCnt) // AIM_AND_MERC_MERCS
	{
		if ( IsProfileIdAnAimOrMERCMerc( (UINT8)uiCnt ) )
		{
		pProfile = &(gMercProfiles[ uiCnt ]);
#ifdef JA2UB		
		//Make sure stigie and Gaston are available at the start of the game
/*		if( uiCnt == 59 || uiCnt == 58 )
		{
			pProfile->bMercStatus = MERC_OK;
			pProfile->uiDayBecomesAvailable = 0;
			pProfile->uiPrecedentQuoteSaid = 0;
			pProfile->ubDaysOfMoraleHangover = 0;

			continue;
		}
*/
		//if the merc is dead, dont modify anything
		if( pProfile->bMercStatus == MERC_IS_DEAD )
		{
			continue;
		}

		// calc chance to start on assignment
		uiChance = 3 * pProfile->bExpLevel; //5 Ja25 UB
#else
		uiChance = 5 * pProfile->bExpLevel;
#endif

		// tais: disable mercs being on assignment (this check is just for at the start of the campaign)
		if (Random(100) < uiChance && gGameExternalOptions.fMercsOnAssignment < 1)
		{
			pProfile->bMercStatus = MERC_WORKING_ELSEWHERE;
			pProfile->uiDayBecomesAvailable = 1 + Random(6 + (pProfile->bExpLevel / 2) );		// 1-(6 to 11) days
		}
		else
		{
			pProfile->bMercStatus = MERC_OK;
			pProfile->uiDayBecomesAvailable = 0;
		}

		pProfile->uiPrecedentQuoteSaid = 0;
		pProfile->ubDaysOfMoraleHangover = 0;
		}
	}
}


void SetProfileFaceData( UINT8 ubCharNum, UINT8 ubFaceIndex, UINT16 usEyesX, UINT16 usEyesY, UINT16 usMouthX, UINT16 usMouthY )
{
	gMercProfiles[ ubCharNum ].ubFaceIndex = ubFaceIndex;
	gMercProfiles[ ubCharNum ].usEyesX			= usEyesX;
	gMercProfiles[ ubCharNum ].usEyesY			= usEyesY;
	gMercProfiles[ ubCharNum ].usMouthX		= usMouthX;
	gMercProfiles[ ubCharNum ].usMouthY		= usMouthY;
}

UINT16 CalcCompetence( MERCPROFILESTRUCT * pProfile )
{
	UINT32 uiStats, uiSkills, uiActionPoints, uiSpecialSkills;
	UINT16 usCompetence;


	// count life twice 'cause it's also hit points
	// mental skills are halved 'cause they're actually not that important within the game
	uiStats = ((2 * pProfile->bLifeMax) + pProfile->bStrength + pProfile->bAgility + pProfile->bDexterity + ((pProfile->bLeadership + pProfile->bWisdom) / 2)) / 3;

	// marksmanship is very important, count it double
	uiSkills = (UINT32) ((2	* (pow((double)pProfile->bMarksmanship, 3) / 10000)) +
												1.5 *	(pow((double)pProfile->bMedical, 3) / 10000) +
															(pow((double)pProfile->bMechanical, 3) / 10000) +
															(pow((double)pProfile->bExplosive, 3) / 10000));

	// action points
	uiActionPoints = 5 + (((10 * pProfile->bExpLevel +
													3 * pProfile->bAgility	+
													2 * pProfile->bLifeMax	+
													2 * pProfile->bDexterity) + 20) / 40);


	// count how many he has, don't care what they are
	uiSpecialSkills = 0;
	for ( INT8 bCnt = 0; bCnt < 30; bCnt++ )
	{
		if ( pProfile->bSkillTraits[bCnt] != 0 )
			uiSpecialSkills++;
	}

	usCompetence = (UINT16) ((pow((double)pProfile->bExpLevel, 0.2) * uiStats * uiSkills * (uiActionPoints - 6) * (1 + (0.05 * (FLOAT)uiSpecialSkills))) / 1000);

	// this currently varies from about 10 (Flo) to 1200 (Gus)
	return(usCompetence);
}

INT16 CalcMedicalDeposit( MERCPROFILESTRUCT * pProfile )
{
	UINT16 usDeposit;

	// this rounds off to the nearest hundred
	usDeposit = (((5 * CalcCompetence(pProfile)) + 50) / 100) * 100;

	return(usDeposit);
}

SOLDIERTYPE * FindSoldierByProfileID( UINT8 ubProfileID, BOOLEAN fPlayerMercsOnly )
{
	UINT8 ubLoop, ubLoopLimit;
	SOLDIERTYPE * pSoldier;

	// sevenfm: fix for last soldier in player team
	if (fPlayerMercsOnly)
	{
		ubLoopLimit = gTacticalStatus.Team[0].bLastID + 1;
	}
	else
	{
		ubLoopLimit = MAX_NUM_SOLDIERS;
	}

	for (ubLoop = 0; ubLoop < ubLoopLimit; ubLoop++)
	{
		pSoldier = MercPtrs[ubLoop];

		if (pSoldier->bActive && pSoldier->ubProfile == ubProfileID)
		{
			return( pSoldier );
		}
	}
	return( NULL );
}



SOLDIERTYPE *ChangeSoldierTeam( SOLDIERTYPE *pSoldier, UINT8 ubTeam )
{
	UINT8										ubID;
	SOLDIERTYPE							*pNewSoldier = NULL;
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	UINT32									cnt;
	INT32										sOldGridNo;

	UINT8										ubOldID;
	UINT32									uiOldUniqueId;

	UINT32									uiSlot;
	SOLDIERTYPE							*pGroupMember;

	BOOLEAN								success;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("ChangeSoldierTeam"));

	if (gfInTalkPanel)
	{
		DeleteTalkingMenu();
	}

	// Save merc id for this guy...
	ubID = pSoldier->ubID;

	ubOldID = ubID;
	uiOldUniqueId = pSoldier->uiUniqueSoldierIdValue;

	sOldGridNo = pSoldier->sGridNo;

	// Remove him from the game!
	InternalTacticalRemoveSoldier( ubID, FALSE );

	// Create a new one!
	MercCreateStruct.bTeam							= ubTeam;
	MercCreateStruct.ubProfile					= pSoldier->ubProfile;
	MercCreateStruct.ubBodyType					= pSoldier->ubBodyType;
	MercCreateStruct.sSectorX						= pSoldier->sSectorX;
	MercCreateStruct.sSectorY						= pSoldier->sSectorY;
	MercCreateStruct.bSectorZ						= pSoldier->bSectorZ;
	MercCreateStruct.sInsertionGridNo		= pSoldier->sGridNo;
	MercCreateStruct.ubDirection					= pSoldier->ubDirection;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		MercCreateStruct.ubProfile					= NO_PROFILE;
		MercCreateStruct.fUseGivenVehicle		= TRUE;
		MercCreateStruct.bUseGivenVehicleID	= pSoldier->bVehicleID;
	}

	if ( ubTeam == gbPlayerNum )
	{
		MercCreateStruct.fPlayerMerc = TRUE;
	}

	if ( ubTeam == MILITIA_TEAM )
	{
		MercCreateStruct.ubSoldierClass = pSoldier->ubSoldierClass;
	}

	if ( TacticalCreateSoldier( &MercCreateStruct, &ubID ) )
	{
		pNewSoldier = MercPtrs[ ubID ];

		// Copy vital stats back!
		pNewSoldier->stats.bLife													= pSoldier->stats.bLife;
		pNewSoldier->stats.bLifeMax												= pSoldier->stats.bLifeMax;
		pNewSoldier->stats.bAgility												= pSoldier->stats.bAgility;
		pNewSoldier->stats.bLeadership										= pSoldier->stats.bLeadership;
		pNewSoldier->stats.bDexterity											= pSoldier->stats.bDexterity;
		pNewSoldier->stats.bStrength											= pSoldier->stats.bStrength;
		pNewSoldier->stats.bWisdom												= pSoldier->stats.bWisdom;
		pNewSoldier->stats.bExpLevel											= pSoldier->stats.bExpLevel;
		pNewSoldier->stats.bMarksmanship									= pSoldier->stats.bMarksmanship;
		pNewSoldier->stats.bMedical												= pSoldier->stats.bMedical;
		pNewSoldier->stats.bMechanical										= pSoldier->stats.bMechanical;
		pNewSoldier->stats.bExplosive											= pSoldier->stats.bExplosive;
		pNewSoldier->stats.bScientific										= pSoldier->stats.bScientific;
		pNewSoldier->bLastRenderVisibleValue				= pSoldier->bLastRenderVisibleValue;
		pNewSoldier->bVisible												= pSoldier->bVisible;
		// added by SANDRO - insta-healable injury zero on soldier creation
		pNewSoldier->iHealableInjury = pSoldier->iHealableInjury; 
		
		// WANNE: Fix a vanilla bug: When a soldier changed team (e.g. getting hostile), he lost his camouflage.
		// Fixed by Tron (Stracciatella): Revision: 7055
		pNewSoldier->bCamo													= pSoldier->bCamo;
		if (pNewSoldier->bCamo != 0)
		{
			pNewSoldier->CreateSoldierPalettes();
		}

		// 0verhaul:  Need to pass certain flags over.  COWERING is one of them.  Others to be determined.
		
		// copy uiStatusFlags, etc. - hayden :)
		if (is_networked)
		{
			pNewSoldier->flags.uiStatusFlags = pSoldier->flags.uiStatusFlags;
			pNewSoldier->ubProfile = pSoldier->ubProfile;
		}
		else
		{
			pNewSoldier->flags.uiStatusFlags |= pSoldier->flags.uiStatusFlags & (SOLDIER_COWERING | SOLDIER_MUTE | SOLDIER_GASSED);
		}

		
		if ( ubTeam == gbPlayerNum )
		{
			pNewSoldier->bVisible= 1;
		}

		//CHRISL: Rather then resorting the profile, which recreates all the items, what if we simply try and sort the
		//	objects that are already attached to the RPC we're hiring?
		//if(UsingNewInventorySystem() == true)
		{
			// Start by direct copy of all BODYPOS items (armor, hands, head and LBE)
			for(cnt = 0; cnt < (UINT32)BODYPOSFINAL; ++cnt)
			{
				pNewSoldier->inv[cnt] = pSoldier->inv[cnt];
			}

			// Next, direct copy of the gunsling and knife pockets
			pNewSoldier->inv[GUNSLINGPOCKPOS] = pSoldier->inv[GUNSLINGPOCKPOS];
			pNewSoldier->inv[KNIFEPOCKPOS] = pSoldier->inv[KNIFEPOCKPOS];
			// Then, try to autoplace everything else
			UINT32 invsize = pNewSoldier->inv.size();
			for(cnt = BIGPOCKSTART; cnt < invsize; ++cnt )
			{
				if(pSoldier->inv[cnt].exists() == true)
				{
					success = PlaceInAnyPocket(pNewSoldier, &pSoldier->inv[cnt], FALSE);
					if(!success)
					{
						PlaceObject( pNewSoldier, cnt, &pSoldier->inv[cnt] );
					}
				}
			}
		}

		// OK, loop through all active merc slots, change
		// Change ANY attacker's target if they were once on this guy.....
		for ( uiSlot = 0; uiSlot < guiNumMercSlots; uiSlot++ )
		{
			pGroupMember = MercSlots[ uiSlot ];

			if ( pGroupMember != NULL )
			{
				if ( pGroupMember->ubTargetID == pSoldier->ubID )
				{
					pGroupMember->ubTargetID = pNewSoldier->ubID;
				}
			}
		}


		// Set insertion gridNo
		pNewSoldier->sInsertionGridNo								= sOldGridNo;

		if ( gfPotentialTeamChangeDuringDeath )
		{
			HandleCheckForDeathCommonCode( pSoldier );
		}

		if ( gfWorldLoaded &&	pSoldier->bInSector
		//pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->bSectorZ == gbWorldSectorZ
		)
		{
			AddSoldierToSectorNoCalculateDirectionUseAnimation( ubID, pSoldier->usAnimState, pSoldier->usAniCode );
			HandleSight(pNewSoldier, SIGHT_LOOK | SIGHT_RADIO);
		}

		// fix up the event queue...
	//	ChangeSoldierIDInQueuedEvents( ubOldID, uiOldUniqueId, pNewSoldier->ubID, pNewSoldier->uiUniqueSoldierIdValue );

		if ( pNewSoldier->ubProfile != NO_PROFILE )
		{
			if ( ubTeam == gbPlayerNum )
			{
				gMercProfiles[ pNewSoldier->ubProfile ].ubMiscFlags |= PROFILE_MISC_FLAG_RECRUITED;
			}
			else
			{
				gMercProfiles[ pNewSoldier->ubProfile ].ubMiscFlags &= (~PROFILE_MISC_FLAG_RECRUITED);
			}
		}

	}

	// AT the low level check if this poor guy is in inv panel, else
	// remove....
	if ( gsCurInterfacePanel == SM_PANEL && gpSMCurrentMerc == pSoldier )
	{
		// Switch....
		SetCurrentInterfacePanel( TEAM_PANEL );
	}

	return( pNewSoldier );
}


BOOLEAN RecruitRPC( UINT8 ubCharNum )
{
	SOLDIERTYPE *pSoldier, *pNewSoldier;

	// Get soldier pointer
	pSoldier = FindSoldierByProfileID( ubCharNum, FALSE );

	if (!pSoldier)
	{
		return( FALSE );
	}

	bool first_time_recruited = GetTheStateOfDepartedMerc(ubCharNum) == -1;

	// OK, set recruit flag..
	gMercProfiles[ ubCharNum ].ubMiscFlags |= PROFILE_MISC_FLAG_RECRUITED;

	// Add this guy to our team!
	pNewSoldier = ChangeSoldierTeam( pSoldier, gbPlayerNum );

	if (!pNewSoldier)
	{
		return( FALSE );
	}

	if ( ubCharNum == SLAY )
	{
		if(gGameExternalOptions.fEnableSlayForever == TRUE)
		{
			if(gMercProfiles[ ubCharNum ].sSalary == 0)
				gMercProfiles[ ubCharNum ].sSalary = gMercProfiles[ 7 ].sSalary;
			if(gMercProfiles[ ubCharNum ].uiWeeklySalary == 0)
				gMercProfiles[ ubCharNum ].uiWeeklySalary = gMercProfiles[ 7 ].uiWeeklySalary;
			if(gMercProfiles[ ubCharNum ].uiBiWeeklySalary == 0)
				gMercProfiles[ ubCharNum ].uiBiWeeklySalary = gMercProfiles[ 7 ].uiBiWeeklySalary;
			pNewSoldier->iTotalContractLength = 7;
		}
		// slay will leave in a week
		pNewSoldier->iEndofContractTime = GetWorldTotalMin() + ( 7 * 24 * 60 );

		KickOutWheelchair( pNewSoldier );
	}
	else if ( ubCharNum == DYNAMO && gubQuest[ QUEST_FREE_DYNAMO ] == QUESTINPROGRESS && first_time_recruited)
	{
		EndQuest( QUEST_FREE_DYNAMO, pSoldier->sSectorX, pSoldier->sSectorY );
	}
	// SANDRO - give exp and records quest point, if finally recruiting Miguel
	else if ( ubCharNum == MIGUEL && first_time_recruited)
	{
		GiveQuestRewardPoint( pSoldier->sSectorX, pSoldier->sSectorY, 6, MIGUEL );
	}

	if ( gGameOptions.fNewTraitSystem )
	{
		// Flugente: people recruited in Arulco are known to the enemy as civilians or even soldiers. So they will be covert when recruited. Of course, this is not for the rebels...
		if ( ubCharNum == DEVIN || ubCharNum == HAMOUS || ubCharNum == SLAY || ubCharNum == VINCE || ubCharNum == MADDOG || ubCharNum == MICKY )
		{
			pNewSoldier->usSoldierFlagMask |= (SOLDIER_COVERT_CIV|SOLDIER_COVERT_NPC_SPECIAL);
		}
		else if ( ubCharNum == IGGY || ubCharNum == CONRAD )
		{
			pNewSoldier->usSoldierFlagMask |= (SOLDIER_COVERT_SOLDIER|SOLDIER_COVERT_NPC_SPECIAL);
		}
	}

	// handle town loyalty adjustment
	if(first_time_recruited)
		HandleTownLoyaltyForNPCRecruitment( pNewSoldier );

	// Try putting them into the current squad
	if ( AddCharacterToSquad( pNewSoldier, (INT8)CurrentSquad( ) ) == FALSE )
	{
		AddCharacterToAnySquad( pNewSoldier );
	}

	ResetDeadSquadMemberList( pNewSoldier->bAssignment );

	DirtyMercPanelInterface( pNewSoldier, DIRTYLEVEL2 );

	if ( pNewSoldier->inv[ HANDPOS ].exists() == false )
	{
		// empty handed - swap in first available weapon
		INT8		bSlot;

		bSlot = FindObjClass( pNewSoldier, IC_WEAPON );
		if ( bSlot != NO_SLOT )
		{
//			if ( Item[ pNewSoldier->inv[ bSlot ].usItem ].fFlags & ITEM_TWO_HANDED )
			if ( Item[ pNewSoldier->inv[ bSlot ].usItem ].twohanded )
			{
				if ( bSlot != SECONDHANDPOS && pNewSoldier->inv[ SECONDHANDPOS ].exists() == true )
				{
					// need to move second hand item out first
					AutoPlaceObject( pNewSoldier, &(pNewSoldier->inv[ SECONDHANDPOS ]), FALSE );
				}
			}
			// swap item to hand
			SwapObjs( pNewSoldier, bSlot, HANDPOS, TRUE );
		}
	}
#ifdef JA2UB
// no Ja25 UB
#else
	if ( ubCharNum == IRA && first_time_recruited)
	{
		// trigger 0th PCscript line
		TriggerNPCRecord( IRA, 0 );
	}
#endif
	// Set whatkind of merc am i
	pNewSoldier->ubWhatKindOfMercAmI = MERC_TYPE__NPC;

	if ( ubCharNum == SLAY && gGameExternalOptions.fEnableSlayForever == TRUE )
	{
		pNewSoldier->ubWhatKindOfMercAmI = MERC_TYPE__AIM_MERC;
	}

	//
	//add a history log that tells the user that a npc has joined the team
	//
	// ( pass in pNewSoldier->sSectorX cause if its invalid, -1, n/a will appear as the sector in the history log )
	AddHistoryToPlayersLog( HISTORY_RPC_JOINED_TEAM, pNewSoldier->ubProfile, GetWorldTotalMin(), pNewSoldier->sSectorX, pNewSoldier->sSectorY );


	//remove the merc from the Personnel screens departed list ( if they have never been hired before, its ok to call it )
	RemoveNewlyHiredMercFromPersonnelDepartedList( pSoldier->ubProfile );
#ifdef JA2UB	
	//If this is a special NPC, play a quote from the team mates
	HandlePlayingQuoteWhenHiringNpc( pNewSoldier->ubProfile );
#endif

	// Flugente: additional dialogue
	AdditionalTacticalCharacterDialogue_AllInSectorRadiusCall( ubCharNum, ADE_DIALOGUE_RPC_RECRUIT_SUCCESS, ubCharNum );
	
	// Flugente: external scripts might have extra functionality on recruiting someone
	if (first_time_recruited)
		LuaRecruitRPCAdditionalHandling( ubCharNum );

	return( TRUE );
}

BOOLEAN RecruitEPC( UINT8 ubCharNum )
{
	SOLDIERTYPE *pSoldier, *pNewSoldier;

	// Get soldier pointer
	pSoldier = FindSoldierByProfileID( ubCharNum, FALSE );

	if (!pSoldier)
	{
		return( FALSE );
	}

	// OK, set recruit flag..
	gMercProfiles[ ubCharNum ].ubMiscFlags |= PROFILE_MISC_FLAG_EPCACTIVE;

	gMercProfiles[ ubCharNum ].ubMiscFlags3 &= ~PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE;

	// Add this guy to our team!
	pNewSoldier = ChangeSoldierTeam( pSoldier, gbPlayerNum );
	pNewSoldier->ubWhatKindOfMercAmI = MERC_TYPE__EPC;

	// Try putting them into the current squad
	if ( AddCharacterToSquad( pNewSoldier, (INT8)CurrentSquad( ) ) == FALSE )
	{
		AddCharacterToAnySquad( pNewSoldier );
	}

	ResetDeadSquadMemberList( pNewSoldier->bAssignment );

	DirtyMercPanelInterface( pNewSoldier, DIRTYLEVEL2 );
	// Make the interface panel dirty..
	// This will dirty the panel next frame...
	gfRerenderInterfaceFromHelpText = TRUE;


	// If we are a robot, look to update controller....
	if ( pNewSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		pNewSoldier->UpdateRobotControllerGivenRobot(	);
	}

	// Set whatkind of merc am i
	pNewSoldier->ubWhatKindOfMercAmI = MERC_TYPE__EPC;

	// Flugente: people recruited in Arulco are known to the enemy as civilians or even soldiers. So they will be covert when recruited. Of course, this is not for the rebels/vehicles/robots
	if ( gGameOptions.fNewTraitSystem && !(pNewSoldier->flags.uiStatusFlags & (SOLDIER_ROBOT | SOLDIER_VEHICLE)) )
	{
		pNewSoldier->usSoldierFlagMask |= (SOLDIER_COVERT_CIV | SOLDIER_COVERT_NPC_SPECIAL);
	}

	UpdateTeamPanelAssignments( );

	return( TRUE );
}


BOOLEAN UnRecruitEPC( UINT8 ubCharNum )
{
	SOLDIERTYPE *pSoldier, *pNewSoldier;

	// Get soldier pointer
	pSoldier = FindSoldierByProfileID( ubCharNum, FALSE );

	if (!pSoldier)
	{
		return( FALSE );
	}

	if (pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__EPC)
	{
		return( FALSE );
	}

	if ( pSoldier->bAssignment < ON_DUTY )
	{
		ResetDeadSquadMemberList( pSoldier->bAssignment );
	}

	// Rmeove from squad....
	RemoveCharacterFromSquads( pSoldier );

	//	Add to our 'departed' list
	AddCharacterToOtherList( pSoldier );

	// O< check if this is the only guy in the sector....
	if ( gusSelectedSoldier == pSoldier->ubID )
	{
		gusSelectedSoldier = NOBODY;
	}

	// OK, UN set recruit flag..
	gMercProfiles[ ubCharNum ].ubMiscFlags &= (~PROFILE_MISC_FLAG_EPCACTIVE);

	// update sector values to current

	//Buggler: code only works on map reload so consolidate to HandleEarlyMorningEvents
	// check to see if this person should disappear from the map after this
	/*if ( (ubCharNum == JOHN || ubCharNum == MARY) && pSoldier->sSectorX == 13 && 
		pSoldier->sSectorY == MAP_ROW_B && pSoldier->bSectorZ == 0 )
	{
		gMercProfiles[ ubCharNum ].sSectorX = 0;
		gMercProfiles[ ubCharNum ].sSectorY = 0;
		gMercProfiles[ ubCharNum ].bSectorZ = 0;
	}
	else
	{
		gMercProfiles[ ubCharNum ].sSectorX = pSoldier->sSectorX;
		gMercProfiles[ ubCharNum ].sSectorY = pSoldier->sSectorY;
		gMercProfiles[ ubCharNum ].bSectorZ = pSoldier->bSectorZ;
	}*/

	gMercProfiles[ ubCharNum ].sSectorX = pSoldier->sSectorX;
	gMercProfiles[ ubCharNum ].sSectorY = pSoldier->sSectorY;
	gMercProfiles[ ubCharNum ].bSectorZ = pSoldier->bSectorZ;

	// how do we decide whether or not to set this?
	gMercProfiles[ ubCharNum ].ubMiscFlags3 |= PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE;
	gMercProfiles[ ubCharNum ].usStrategicInsertionData = pSoldier->sGridNo;
	gMercProfiles[ ubCharNum ].fUseProfileInsertionInfo = TRUE;
	gMercProfiles[ ubCharNum ].ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;

	// Add this guy to CIV team!
	pNewSoldier = ChangeSoldierTeam( pSoldier, CIV_TEAM );

	UpdateTeamPanelAssignments( );

	return( TRUE );
}



INT8 WhichBuddy( UINT8 ubCharNum, UINT8 ubBuddy )
{
	MERCPROFILESTRUCT *	pProfile;
	INT8								bLoop;

	pProfile = &( gMercProfiles[ ubCharNum ] );

	for (bLoop = 0; bLoop < 5; bLoop++)
	{
		if ( pProfile->bBuddy[bLoop] == ubBuddy )
		{
			return( bLoop );
		}
	}
	if( pProfile->bLearnToLike == ubBuddy && pProfile->bLearnToLikeCount == 0 )
	{
		return( 5 );
	}
	return( -1 );
}

INT8 WhichHated( UINT8 ubCharNum, UINT8 ubHated )
{
	MERCPROFILESTRUCT *	pProfile;
	INT8								bLoop;

	pProfile = &( gMercProfiles[ ubCharNum ] );

	for (bLoop = 0; bLoop < 5; bLoop++)
	{
		if ( pProfile->bHated[bLoop] == ubHated )
		{
			return( bLoop );
		}
	}
	if( pProfile->bLearnToHate == ubHated && pProfile->bLearnToHateCount == 0 )
	{
		return( 5 );
	}
	return( -1 );
}


BOOLEAN IsProfileATerrorist( UINT8 ubProfile )
{
	if ( ubProfile == 83 || ubProfile == 111 ||
			ubProfile == 64 || ubProfile == 112 ||
			ubProfile == 82 || ubProfile == 110 )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN IsProfileAHeadMiner( UINT8 ubProfile )
{
	if ( ubProfile == 106 || ubProfile == 148 ||
			ubProfile == 156 || ubProfile == 157 ||
			ubProfile == 158 )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


void UpdateSoldierPointerDataIntoProfile( BOOLEAN fPlayerMercs )
{
	SOLDIERTYPE *pSoldier = NULL;
	MERCPROFILESTRUCT * pProfile;
	BOOLEAN				fDoCopy = FALSE;

	for( UINT32 uiCount=0; uiCount < guiNumMercSlots; ++uiCount)
	{
		pSoldier = MercSlots[ uiCount ];

		if ( pSoldier != NULL )
		{
			if ( pSoldier->ubProfile != NO_PROFILE )
			{
				fDoCopy = FALSE;

				// If we are above player mercs
				if ( fPlayerMercs )
				{
					if ( gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_AIM || 
						gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_MERC ||
						gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_IMP )
					{
						fDoCopy = TRUE;
					}
				}
				else
				{
					if ( gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_RPC ||
						gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_NPC ||
						gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_VEHICLE )
					{
						fDoCopy = TRUE;
					}
				}

				if ( fDoCopy )
				{
					// get profile...
					pProfile = &( gMercProfiles[ pSoldier->ubProfile ] );

					// Copy....
					pProfile->bLife 										= pSoldier->stats.bLife;
					pProfile->bLifeMax									= pSoldier->stats.bLifeMax;
					pProfile->bAgility									= pSoldier->stats.bAgility;
					pProfile->bLeadership								= pSoldier->stats.bLeadership;
					pProfile->bDexterity								= pSoldier->stats.bDexterity;
					pProfile->bStrength									= pSoldier->stats.bStrength;
					pProfile->bWisdom										= pSoldier->stats.bWisdom;
					pProfile->bExpLevel									= pSoldier->stats.bExpLevel;
					pProfile->bMarksmanship							= pSoldier->stats.bMarksmanship;
					pProfile->bMedical									= pSoldier->stats.bMedical;
					pProfile->bMechanical								= pSoldier->stats.bMechanical;
					pProfile->bExplosive								= pSoldier->stats.bExplosive;
					pProfile->bScientific								= pSoldier->stats.bScientific;
				}
			}
		}
	}
}



BOOLEAN DoesMercHaveABuddyOnTheTeam( UINT8 ubMercID )
{
	UINT8	ubCnt;
	UINT8	bBuddyID;

	// loop through the list of people the merc is buddies with
	for(ubCnt=0; ubCnt< 6; ubCnt++)
	{
		//see if the merc has a buddy on the team
		if( ubCnt<5 )
		{
			bBuddyID = gMercProfiles[ ubMercID ].bBuddy[ubCnt];
		}
		else
		{
			bBuddyID = gMercProfiles[ ubMercID ].bLearnToLike;
			// ignore learn to like, if he's not a buddy yet
			if( gMercProfiles[ ubMercID ].bLearnToLikeCount > 0 )
				continue;
		}

		//If its not a valid 'buddy'
		if( bBuddyID == NUM_PROFILES )
			continue;

		if( IsMercOnTeam( bBuddyID, FALSE, FALSE ) )
		{
			if( !IsMercDead( bBuddyID ) )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

BOOLEAN MercIsHot( SOLDIERTYPE * pSoldier )
{
	// SANDRO - added argument
	// Flugente: drugs can temporarily cause a merc to be heat intolerant
	if ( !pSoldier->MercInWater( ) && DoesMercHaveDisability( pSoldier, HEAT_INTOLERANT ) )
	{
		if ( (pSoldier->bSectorZ > 0) || (SectorInfo[SECTOR( pSoldier->sSectorX, pSoldier->sSectorY )].usWeather == WEATHER_FORECAST_RAIN || SectorInfo[SECTOR( pSoldier->sSectorX, pSoldier->sSectorY )].usWeather == WEATHER_FORECAST_THUNDERSHOWERS) )
		{
			// cool underground or raining
			return( FALSE );
		}
		else if ( IsSectorDesert( pSoldier->sSectorX, pSoldier->sSectorY ) ) // is desert
		//if ( SectorTemperature( GetWorldMinutesInDay(), pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ ) > 1 )
		{
			return( TRUE );
		}
		else if ( IsSectorTropical( pSoldier->sSectorX, pSoldier->sSectorY ) ) // is tropical
		{
			return( TRUE );
		}
	}
	return( FALSE );
}
// SANDRO - added function here
BOOLEAN MercIsInTropicalSector( SOLDIERTYPE * pSoldier )
{
	if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->bSectorZ <= 0 && IsSectorTropical(pSoldier->sSectorX, pSoldier->sSectorY) )
	{
		return( TRUE );
	}
	else
		return( FALSE );
}

SOLDIERTYPE* SwapToProfile( SOLDIERTYPE * pSoldier, UINT8 ubDestProfile )
{
	MERCPROFILESTRUCT* pNewProfile;
	UINT8 ubSrcProfile = pSoldier->ubProfile;

	// need a stop criteria...
	if ( FALSE )
	{
		// I don't think so!
		return( pSoldier );
	}

	pNewProfile = &gMercProfiles[ ubDestProfile ];
	pNewProfile->ubMiscFlags2 = gMercProfiles[ ubSrcProfile ].ubMiscFlags2;
	pNewProfile->ubMiscFlags = gMercProfiles[ ubSrcProfile ].ubMiscFlags;
	pNewProfile->sSectorX = gMercProfiles[ ubSrcProfile ].sSectorX;
	pNewProfile->sSectorY = gMercProfiles[ ubSrcProfile ].sSectorY;
	pNewProfile->uiDayBecomesAvailable = gMercProfiles[ ubSrcProfile ].uiDayBecomesAvailable;
	/////////////////////////////////////////////////////////////////////////////////////
	// SANDRO - experimental - more specific statistics of mercs
	pNewProfile->records.usKillsElites = gMercProfiles[ ubSrcProfile ].records.usKillsElites;
	pNewProfile->records.usKillsRegulars = gMercProfiles[ ubSrcProfile ].records.usKillsRegulars;
	pNewProfile->records.usKillsAdmins = gMercProfiles[ ubSrcProfile ].records.usKillsAdmins;
	pNewProfile->records.usKillsCreatures = gMercProfiles[ ubSrcProfile ].records.usKillsCreatures;
	pNewProfile->records.usKillsZombies = gMercProfiles[ ubSrcProfile ].records.usKillsZombies;
	pNewProfile->records.usKillsTanks = gMercProfiles[ ubSrcProfile ].records.usKillsTanks;
	pNewProfile->records.usKillsOthers = gMercProfiles[ ubSrcProfile ].records.usKillsOthers;
	pNewProfile->records.usAssistsMercs = gMercProfiles[ ubSrcProfile ].records.usAssistsMercs;
	pNewProfile->records.usAssistsMilitia = gMercProfiles[ ubSrcProfile ].records.usAssistsMilitia;
	pNewProfile->records.usAssistsOthers = gMercProfiles[ ubSrcProfile ].records.usAssistsOthers;
	pNewProfile->records.usShotsFired = gMercProfiles[ ubSrcProfile ].records.usShotsFired;
	pNewProfile->records.usMissilesLaunched = gMercProfiles[ ubSrcProfile ].records.usMissilesLaunched;
	pNewProfile->records.usGrenadesThrown = gMercProfiles[ ubSrcProfile ].records.usGrenadesThrown;
	pNewProfile->records.usKnivesThrown = gMercProfiles[ ubSrcProfile ].records.usKnivesThrown;
	pNewProfile->records.usBladeAttacks = gMercProfiles[ ubSrcProfile ].records.usBladeAttacks;
	pNewProfile->records.usHtHAttacks = gMercProfiles[ ubSrcProfile ].records.usHtHAttacks;
	pNewProfile->records.usShotsHit = gMercProfiles[ ubSrcProfile ].records.usShotsHit;
	pNewProfile->records.usBattlesTactical = gMercProfiles[ ubSrcProfile ].records.usBattlesTactical;
	pNewProfile->records.usBattlesAutoresolve = gMercProfiles[ ubSrcProfile ].records.usBattlesAutoresolve;
	pNewProfile->records.usBattlesRetreated = gMercProfiles[ ubSrcProfile ].records.usBattlesRetreated;
	pNewProfile->records.usAmbushesExperienced = gMercProfiles[ ubSrcProfile ].records.usAmbushesExperienced;
	pNewProfile->records.usLargestBattleFought = gMercProfiles[ ubSrcProfile ].records.usLargestBattleFought;
	pNewProfile->records.usTimesWoundedShot = gMercProfiles[ ubSrcProfile ].records.usTimesWoundedShot;
	pNewProfile->records.usTimesWoundedStabbed = gMercProfiles[ ubSrcProfile ].records.usTimesWoundedStabbed;
	pNewProfile->records.usTimesWoundedPunched = gMercProfiles[ ubSrcProfile ].records.usTimesWoundedPunched;
	pNewProfile->records.usTimesWoundedBlasted = gMercProfiles[ ubSrcProfile ].records.usTimesWoundedBlasted;
	pNewProfile->records.usTimesStatDamaged = gMercProfiles[ ubSrcProfile ].records.usTimesStatDamaged;
	pNewProfile->records.usFacilityAccidents = gMercProfiles[ ubSrcProfile ].records.usFacilityAccidents;
	pNewProfile->records.usLocksPicked = gMercProfiles[ ubSrcProfile ].records.usLocksPicked;
	pNewProfile->records.usLocksBreached = gMercProfiles[ ubSrcProfile ].records.usLocksBreached;
	pNewProfile->records.usTrapsRemoved = gMercProfiles[ ubSrcProfile ].records.usTrapsRemoved;
	pNewProfile->records.usExpDetonated = gMercProfiles[ ubSrcProfile ].records.usExpDetonated;
	pNewProfile->records.usItemsRepaired = gMercProfiles[ ubSrcProfile ].records.usItemsRepaired;
	pNewProfile->records.usItemsCombined = gMercProfiles[ ubSrcProfile ].records.usItemsCombined;
	pNewProfile->records.usItemsStolen = gMercProfiles[ ubSrcProfile ].records.usItemsStolen;
	pNewProfile->records.usMercsBandaged = gMercProfiles[ ubSrcProfile ].records.usMercsBandaged;
	pNewProfile->records.usSurgeriesMade = gMercProfiles[ ubSrcProfile ].records.usSurgeriesMade;
	pNewProfile->records.usNPCsDiscovered = gMercProfiles[ ubSrcProfile ].records.usNPCsDiscovered;
	pNewProfile->records.usSectorsDiscovered = gMercProfiles[ ubSrcProfile ].records.usSectorsDiscovered;
	pNewProfile->records.usMilitiaTrained = gMercProfiles[ ubSrcProfile ].records.usMilitiaTrained;
	pNewProfile->records.ubQuestsHandled = gMercProfiles[ ubSrcProfile ].records.ubQuestsHandled;
	pNewProfile->records.usInterrogations = gMercProfiles[ubSrcProfile].records.usInterrogations;
	pNewProfile->records.usDamageTaken = gMercProfiles[ubSrcProfile].records.usDamageTaken;
	pNewProfile->records.usDamageDealt = gMercProfiles[ubSrcProfile].records.usDamageDealt;
	pNewProfile->records.usTimesInfected = gMercProfiles[ubSrcProfile].records.usTimesInfected;
	pNewProfile->records.usPointsHealed = gMercProfiles[ubSrcProfile].records.usPointsHealed;

	/////////////////////////////////////////////////////////////////////////////////////
	pNewProfile->usTotalDaysServed = gMercProfiles[ ubSrcProfile ].usTotalDaysServed;
	pNewProfile->bResigned = gMercProfiles[ ubSrcProfile ].bResigned;
	pNewProfile->bActive = gMercProfiles[ ubSrcProfile ].bActive;
	pNewProfile->fUseProfileInsertionInfo = gMercProfiles[ ubSrcProfile ].fUseProfileInsertionInfo;
	pNewProfile->sGridNo = gMercProfiles[ ubSrcProfile ].sGridNo;
	pNewProfile->ubQuoteActionID = gMercProfiles[ ubSrcProfile ].ubQuoteActionID;
	pNewProfile->ubLastQuoteSaid = gMercProfiles[ ubSrcProfile ].ubLastQuoteSaid;
	pNewProfile->ubStrategicInsertionCode = gMercProfiles[ ubSrcProfile ].ubStrategicInsertionCode;
	pNewProfile->bMercStatus = gMercProfiles[ ubSrcProfile ].bMercStatus;
	pNewProfile->bSectorZ = gMercProfiles[ ubSrcProfile ].bSectorZ;
	pNewProfile->usStrategicInsertionData = gMercProfiles[ ubSrcProfile ].usStrategicInsertionData;
	pNewProfile->sTrueSalary = gMercProfiles[ ubSrcProfile ].sTrueSalary;

	// the goodguy property is permanent and shouldn't carry over, so check for that
	BOOLEAN goodguy = ( (gMercProfiles[ubDestProfile].ubMiscFlags3) & PROFILE_MISC_FLAG3_GOODGUY ) ? TRUE : FALSE;

	pNewProfile->ubMiscFlags3 = gMercProfiles[ ubSrcProfile ].ubMiscFlags3;

	pNewProfile->ubMiscFlags3 &= ~PROFILE_MISC_FLAG3_GOODGUY;
	if ( goodguy )
		pNewProfile->ubMiscFlags3 |= PROFILE_MISC_FLAG3_GOODGUY;

	pNewProfile->ubDaysOfMoraleHangover = gMercProfiles[ ubSrcProfile ].ubDaysOfMoraleHangover;
	pNewProfile->ubNumTimesDrugUseInLifetime = gMercProfiles[ ubSrcProfile ].ubNumTimesDrugUseInLifetime;
	pNewProfile->uiPrecedentQuoteSaid = gMercProfiles[ ubSrcProfile ].uiPrecedentQuoteSaid;
	pNewProfile->sPreCombatGridNo = gMercProfiles[ ubSrcProfile ].sPreCombatGridNo;
	pNewProfile->ubSnitchExposedCooldown = gMercProfiles[ubSrcProfile].ubSnitchExposedCooldown;

// CJC: this is causing problems so just skip the transfer of exp...
/*
	pNewProfile->sLifeGain = gMercProfiles[ ubSrcProfile ].sLifeGain;
	pNewProfile->sAgilityGain = gMercProfiles[ ubSrcProfile ].sAgilityGain;
	pNewProfile->sDexterityGain = gMercProfiles[ ubSrcProfile ].sDexterityGain;
	pNewProfile->sStrengthGain = gMercProfiles[ ubSrcProfile ].sStrengthGain;
	pNewProfile->sLeadershipGain = gMercProfiles[ ubSrcProfile ].sLeadershipGain;
	pNewProfile->sWisdomGain = gMercProfiles[ ubSrcProfile ].sWisdomGain;
	pNewProfile->sExpLevelGain = gMercProfiles[ ubSrcProfile ].sExpLevelGain;
	pNewProfile->sMarksmanshipGain = gMercProfiles[ ubSrcProfile ].sMarksmanshipGain;
	pNewProfile->sMedicalGain = gMercProfiles[ ubSrcProfile ].sMedicalGain;
	pNewProfile->sMechanicGain = gMercProfiles[ ubSrcProfile ].sMechanicGain;
	pNewProfile->sExplosivesGain = gMercProfiles[ ubSrcProfile ].sExplosivesGain;

	pNewProfile->bLifeDelta = gMercProfiles[ ubSrcProfile ].bLifeDelta;
	pNewProfile->bAgilityDelta = gMercProfiles[ ubSrcProfile ].bAgilityDelta;
	pNewProfile->bDexterityDelta = gMercProfiles[ ubSrcProfile ].bDexterityDelta;
	pNewProfile->bStrengthDelta = gMercProfiles[ ubSrcProfile ].bStrengthDelta;
	pNewProfile->bLeadershipDelta = gMercProfiles[ ubSrcProfile ].bLeadershipDelta;
	pNewProfile->bWisdomDelta = gMercProfiles[ ubSrcProfile ].bWisdomDelta;
	pNewProfile->bExpLevelDelta = gMercProfiles[ ubSrcProfile ].bExpLevelDelta;
	pNewProfile->bMarksmanshipDelta = gMercProfiles[ ubSrcProfile ].bMarksmanshipDelta;
	pNewProfile->bMedicalDelta = gMercProfiles[ ubSrcProfile ].bMedicalDelta;
	pNewProfile->bMechanicDelta = gMercProfiles[ ubSrcProfile ].bMechanicDelta;
	pNewProfile->bExplosivesDelta = gMercProfiles[ ubSrcProfile ].bExplosivesDelta;
	*/

	pNewProfile->sLifeGain = gMercProfiles[ubSrcProfile].sLifeGain;
	pNewProfile->sAgilityGain = gMercProfiles[ubSrcProfile].sAgilityGain;
	pNewProfile->sDexterityGain = gMercProfiles[ubSrcProfile].sDexterityGain;
	pNewProfile->sStrengthGain = gMercProfiles[ubSrcProfile].sStrengthGain;
	pNewProfile->sWisdomGain = gMercProfiles[ubSrcProfile].sWisdomGain;
	pNewProfile->sExpLevelGain = gMercProfiles[ubSrcProfile].sExpLevelGain;

	pNewProfile->bLifeDelta = gMercProfiles[ubSrcProfile].bLifeDelta;
	pNewProfile->bAgilityDelta = gMercProfiles[ubSrcProfile].bAgilityDelta;
	pNewProfile->bDexterityDelta = gMercProfiles[ubSrcProfile].bDexterityDelta;
	pNewProfile->bStrengthDelta = gMercProfiles[ubSrcProfile].bStrengthDelta;
	pNewProfile->bWisdomDelta = gMercProfiles[ubSrcProfile].bWisdomDelta;
	pNewProfile->bExpLevelDelta = gMercProfiles[ubSrcProfile].bExpLevelDelta;

	pNewProfile->bLife = gMercProfiles[ubSrcProfile].bLife;
	pNewProfile->bAgility = gMercProfiles[ubSrcProfile].bAgility;
	pNewProfile->bDexterity = gMercProfiles[ubSrcProfile].bDexterity;
	pNewProfile->bStrength = gMercProfiles[ubSrcProfile].bStrength;
	pNewProfile->bWisdom = gMercProfiles[ubSrcProfile].bWisdom;
	pNewProfile->bExpLevel = gMercProfiles[ubSrcProfile].bExpLevel;


	pNewProfile->bInvStatus = gMercProfiles[ ubSrcProfile ].bInvStatus;
	pNewProfile->bInvNumber = gMercProfiles[ ubSrcProfile ].bInvNumber;
	pNewProfile->inv = gMercProfiles[ ubSrcProfile ].inv;
	memcpy( pNewProfile->bMercTownReputation , gMercProfiles[ ubSrcProfile ].bMercTownReputation , sizeof( UINT8 ) * 20 );

	// remove face
	DeleteSoldierFace( pSoldier );

	pSoldier->ubProfile = ubDestProfile;

	// create new face
	pSoldier->iFaceIndex = InitSoldierFace( pSoldier );

	// replace profile in group
	ReplaceSoldierProfileInPlayerGroup( pSoldier->ubGroupID, ubSrcProfile, ubDestProfile );

	pSoldier->stats.bStrength =			pNewProfile->bStrength + pNewProfile->bStrengthDelta;
	pSoldier->stats.bDexterity =		pNewProfile->bDexterity + pNewProfile->bDexterityDelta;
	pSoldier->stats.bAgility =			pNewProfile->bAgility + pNewProfile->bAgilityDelta;
	pSoldier->stats.bWisdom =			pNewProfile->bWisdom + pNewProfile->bWisdomDelta;
	pSoldier->stats.bExpLevel =			pNewProfile->bExpLevel + pNewProfile->bExpLevelDelta;
	pSoldier->stats.bLeadership =		pNewProfile->bLeadership + pNewProfile->bLeadershipDelta;

	pSoldier->stats.bMarksmanship =		pNewProfile->bMarksmanship + pNewProfile->bMarksmanshipDelta;
	pSoldier->stats.bMechanical =		pNewProfile->bMechanical + pNewProfile->bMechanicDelta;
	pSoldier->stats.bMedical =			pNewProfile->bMedical + pNewProfile->bMedicalDelta;
	pSoldier->stats.bExplosive =		pNewProfile->bExplosive + pNewProfile->bExplosivesDelta;
	
	if ( pSoldier->ubProfile == LARRY_DRUNK )
	{
		SetFactTrue( FACT_LARRY_CHANGED );

#ifdef JA2UB
#else
		// anv: make speck whine about it immediately if on team
		if( !IsSpeckComAvailable() )
		{
			TacticalCharacterDialogue( FindSoldierByProfileID( SPECK_PLAYABLE , TRUE ), SPECK_PLAYABLE_QUOTE_LARRY_RELAPSED );
			// don't bring this up again
			LaptopSaveInfo.uiSpeckQuoteFlags |= SPECK_QUOTE__ALREADY_TOLD_PLAYER_THAT_LARRY_RELAPSED;
		}
#endif
	}
	else if ( pSoldier->ubProfile == LARRY_NORMAL )
	{
		SetFactFalse( FACT_LARRY_CHANGED );
	}

	DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );

	return( pSoldier );
}


BOOLEAN DoesNPCOwnBuilding( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	//DBrot: More Rooms
	//UINT8 ubRoomInfo;
	UINT16 usRoomInfo;

	// Get room info
	usRoomInfo = gusWorldRoomInfo[ sGridNo ];

	if ( usRoomInfo == NO_ROOM )
	{
	return( FALSE );
	}

	// Are we an NPC?
	if ( pSoldier->bTeam != CIV_TEAM )
	{
	return( FALSE );
	}

	// OK, check both ranges
	if ( usRoomInfo >= gMercProfiles[ pSoldier->ubProfile ].usRoomRangeStart[ 0 ] &&
		usRoomInfo <= gMercProfiles[ pSoldier->ubProfile ].usRoomRangeEnd[ 0 ] )
	{
	 return( TRUE );
	}

	if ( usRoomInfo >= gMercProfiles[ pSoldier->ubProfile ].usRoomRangeStart[ 1 ] &&
		usRoomInfo <= gMercProfiles[ pSoldier->ubProfile ].usRoomRangeEnd[ 1 ] )
	{
	 return( TRUE );
	}

	return( FALSE );
}

BOOLEAN IsProfileIdAnAimOrMERCMerc( UINT8 ubProfileID )
{
	if ( gMercProfiles[ ubProfileID ].Type == PROFILETYPE_AIM || gMercProfiles[ubProfileID].Type == PROFILETYPE_MERC )
	{
		return( TRUE );
	}

	return( FALSE );
}

void OverwriteMercProfileWithXMLData( UINT32 uiLoop )
{
		//////////////////////////////////////////////////////////////////////////////////
		//
		// HEADROCK PROFEX: Profile Externalization
		//
		// This is a complete hack which is meant for temporary use until a better system
		// can be implemented. This bit OVERWRITES data accumulated so far, by drawing
		// new data from XML. This allows making PROEDIT obsolete.
		//
		//////////////////////////////////////////////////////////////////////////////////

		wcscpy(gMercProfiles[ uiLoop ].zName, tempProfiles[ uiLoop ].zName) ;
		wcscpy(gMercProfiles[ uiLoop ].zNickname, tempProfiles[ uiLoop ].zNickname) ;

		gMercProfiles[ uiLoop ].ubFaceIndex = tempProfiles[ uiLoop ].ubFaceIndex ;
		gMercProfiles[ uiLoop ].usEyesX = tempProfiles[ uiLoop ].usEyesX ;
		gMercProfiles[ uiLoop ].usEyesY = tempProfiles[ uiLoop ].usEyesY ;
		gMercProfiles[ uiLoop ].usMouthX = tempProfiles[ uiLoop ].usMouthX ;
		gMercProfiles[ uiLoop ].usMouthY = tempProfiles[ uiLoop ].usMouthY ;
		gMercProfiles[ uiLoop ].uiEyeDelay = tempProfiles[ uiLoop ].uiEyeDelay ;
		gMercProfiles[ uiLoop ].uiMouthDelay = tempProfiles[ uiLoop ].uiMouthDelay ;
		gMercProfiles[ uiLoop ].uiBlinkFrequency = tempProfiles[ uiLoop ].uiBlinkFrequency ;
		gMercProfiles[ uiLoop ].uiExpressionFrequency = tempProfiles[ uiLoop ].uiExpressionFrequency ;

		strcpy(gMercProfiles[ uiLoop ].PANTS, tempProfiles[ uiLoop ].PANTS) ;
		strcpy(gMercProfiles[ uiLoop ].VEST, tempProfiles[ uiLoop ].VEST) ;
		strcpy(gMercProfiles[ uiLoop ].SKIN, tempProfiles[ uiLoop ].SKIN) ;
		strcpy(gMercProfiles[ uiLoop ].HAIR, tempProfiles[ uiLoop ].HAIR) ;

		gMercProfiles[ uiLoop ].bSex = tempProfiles[ uiLoop ].bSex ;
		gMercProfiles[ uiLoop ].ubBodyType = tempProfiles[ uiLoop ].ubBodyType ;
		gMercProfiles[ uiLoop ].uiBodyTypeSubFlags = tempProfiles[ uiLoop ].uiBodyTypeSubFlags ;

		///////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - old/new traits
		if (gGameOptions.fNewTraitSystem)
		{
			if (tempProfiles[ uiLoop ].bCharacterTrait >= NUM_CHAR_TRAITS || tempProfiles[ uiLoop ].bCharacterTrait < CHAR_TRAIT_NORMAL )
				gMercProfiles[ uiLoop ].bCharacterTrait = CHAR_TRAIT_NORMAL;
			else
				gMercProfiles[ uiLoop ].bCharacterTrait = tempProfiles[ uiLoop ].bCharacterTrait ;
		}
		else
		{
			if (tempProfiles[ uiLoop ].bAttitude >= NUM_ATTITUDES || tempProfiles[ uiLoop ].bAttitude < ATT_NORMAL )
				gMercProfiles[ uiLoop ].bAttitude = ATT_NORMAL;
			else
				gMercProfiles[ uiLoop ].bAttitude = tempProfiles[ uiLoop ].bAttitude ;
		}

		if (tempProfiles[ uiLoop ].bDisability >= NUM_DISABILITIES || tempProfiles[ uiLoop ].bDisability < NO_DISABILITY )
			gMercProfiles[ uiLoop ].bDisability = NO_DISABILITY;
		else
			gMercProfiles[ uiLoop ].bDisability = tempProfiles[ uiLoop ].bDisability ;
		///////////////////////////////////////////////////////////////////////////////////////////
		gMercProfiles[ uiLoop ].ubNeedForSleep = tempProfiles[ uiLoop ].ubNeedForSleep ;

		gMercProfiles[ uiLoop ].bReputationTolerance = tempProfiles[ uiLoop ].bReputationTolerance ;
		gMercProfiles[ uiLoop ].bDeathRate = tempProfiles[ uiLoop ].bDeathRate ;

		gMercProfiles[ uiLoop ].bLifeMax = tempProfiles[ uiLoop ].bLifeMax ;
		gMercProfiles[ uiLoop ].bLife = tempProfiles[ uiLoop ].bLife ;
		gMercProfiles[ uiLoop ].bStrength = tempProfiles[ uiLoop ].bStrength ;
		gMercProfiles[ uiLoop ].bAgility = tempProfiles[ uiLoop ].bAgility ;
		gMercProfiles[ uiLoop ].bDexterity = tempProfiles[ uiLoop ].bDexterity ;
		gMercProfiles[ uiLoop ].bWisdom = tempProfiles[ uiLoop ].bWisdom ;
		gMercProfiles[ uiLoop ].bMarksmanship = tempProfiles[ uiLoop ].bMarksmanship ;
		gMercProfiles[ uiLoop ].bExplosive = tempProfiles[ uiLoop ].bExplosive ;
		gMercProfiles[ uiLoop ].bLeadership = tempProfiles[ uiLoop ].bLeadership ;
		gMercProfiles[ uiLoop ].bMedical = tempProfiles[ uiLoop ].bMedical ;
		gMercProfiles[ uiLoop ].bMechanical = tempProfiles[ uiLoop ].bMechanical ;
		gMercProfiles[ uiLoop ].bExpLevel = tempProfiles[ uiLoop ].bExpLevel ;

		gMercProfiles[uiLoop]._old_bEvolution = 0;
		//////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - Check old/new traits and repair possible errors
		if (gGameOptions.fNewTraitSystem)
		{
			gMercProfiles[ uiLoop ].bSkillTraits[0] = tempProfiles[ uiLoop ].bNewSkillTrait1;
			gMercProfiles[ uiLoop ].bSkillTraits[1] = tempProfiles[ uiLoop ].bNewSkillTrait2;
			gMercProfiles[ uiLoop ].bSkillTraits[2] = tempProfiles[ uiLoop ].bNewSkillTrait3;
			gMercProfiles[ uiLoop ].bSkillTraits[3] = tempProfiles[ uiLoop ].bNewSkillTrait4;
			gMercProfiles[ uiLoop ].bSkillTraits[4] = tempProfiles[ uiLoop ].bNewSkillTrait5;
			gMercProfiles[ uiLoop ].bSkillTraits[5] = tempProfiles[ uiLoop ].bNewSkillTrait6;
			gMercProfiles[ uiLoop ].bSkillTraits[6] = tempProfiles[ uiLoop ].bNewSkillTrait7;
			gMercProfiles[ uiLoop ].bSkillTraits[7] = tempProfiles[ uiLoop ].bNewSkillTrait8;
			gMercProfiles[ uiLoop ].bSkillTraits[8] = tempProfiles[ uiLoop ].bNewSkillTrait9;
			gMercProfiles[ uiLoop ].bSkillTraits[9] = tempProfiles[ uiLoop ].bNewSkillTrait10;
			gMercProfiles[ uiLoop ].bSkillTraits[10] = tempProfiles[ uiLoop ].bNewSkillTrait11;
			gMercProfiles[ uiLoop ].bSkillTraits[11] = tempProfiles[ uiLoop ].bNewSkillTrait12;
			gMercProfiles[ uiLoop ].bSkillTraits[12] = tempProfiles[ uiLoop ].bNewSkillTrait13;
			gMercProfiles[ uiLoop ].bSkillTraits[13] = tempProfiles[ uiLoop ].bNewSkillTrait14;
			gMercProfiles[ uiLoop ].bSkillTraits[14] = tempProfiles[ uiLoop ].bNewSkillTrait15;
			gMercProfiles[ uiLoop ].bSkillTraits[15] = tempProfiles[ uiLoop ].bNewSkillTrait16;
			gMercProfiles[ uiLoop ].bSkillTraits[16] = tempProfiles[ uiLoop ].bNewSkillTrait17;
			gMercProfiles[ uiLoop ].bSkillTraits[17] = tempProfiles[ uiLoop ].bNewSkillTrait18;
			gMercProfiles[ uiLoop ].bSkillTraits[18] = tempProfiles[ uiLoop ].bNewSkillTrait19;
			gMercProfiles[ uiLoop ].bSkillTraits[19] = tempProfiles[ uiLoop ].bNewSkillTrait20;
			gMercProfiles[ uiLoop ].bSkillTraits[20] = tempProfiles[ uiLoop ].bNewSkillTrait21;
			gMercProfiles[ uiLoop ].bSkillTraits[21] = tempProfiles[ uiLoop ].bNewSkillTrait22;
			gMercProfiles[ uiLoop ].bSkillTraits[22] = tempProfiles[ uiLoop ].bNewSkillTrait23;
			gMercProfiles[ uiLoop ].bSkillTraits[23] = tempProfiles[ uiLoop ].bNewSkillTrait24;
			gMercProfiles[ uiLoop ].bSkillTraits[24] = tempProfiles[ uiLoop ].bNewSkillTrait25;
			gMercProfiles[ uiLoop ].bSkillTraits[25] = tempProfiles[ uiLoop ].bNewSkillTrait26;
			gMercProfiles[ uiLoop ].bSkillTraits[26] = tempProfiles[ uiLoop ].bNewSkillTrait27;
			gMercProfiles[ uiLoop ].bSkillTraits[27] = tempProfiles[ uiLoop ].bNewSkillTrait28;
			gMercProfiles[ uiLoop ].bSkillTraits[28] = tempProfiles[ uiLoop ].bNewSkillTrait29;
			gMercProfiles[ uiLoop ].bSkillTraits[29] = tempProfiles[ uiLoop ].bNewSkillTrait30;

			// if we are out of skill number, make it no trait, otherwise set our trait right
			for ( INT8 bCnt = 0; bCnt < 30; bCnt++ )
			{
				if ( gMercProfiles[ uiLoop ].bSkillTraits[ bCnt ] >= NUM_SKILLTRAITS_NT || gMercProfiles[ uiLoop ].bSkillTraits[ bCnt ] < NO_SKILLTRAIT_NT )
				{
					gMercProfiles[ uiLoop ].bSkillTraits[ bCnt ] = 0 ;
				}
			}
		}
		else
		{
			gMercProfiles[ uiLoop ].bSkillTraits[0] = tempProfiles[ uiLoop ].bOldSkillTrait;
			gMercProfiles[ uiLoop ].bSkillTraits[1] = tempProfiles[ uiLoop ].bOldSkillTrait2;

			// if we have two same traits and cannot aquire expert level, ignore the second one
			if( ( gMercProfiles[ uiLoop ].bSkillTraits[0] == ELECTRONICS_OT && gMercProfiles[ uiLoop ].bSkillTraits[1] == ELECTRONICS_OT ) ||
				( gMercProfiles[ uiLoop ].bSkillTraits[0] == AMBIDEXT_OT && gMercProfiles[ uiLoop ].bSkillTraits[1] == AMBIDEXT_OT ) ||
				( gMercProfiles[ uiLoop ].bSkillTraits[0] == CAMOUFLAGED_OT && gMercProfiles[ uiLoop ].bSkillTraits[1] == CAMOUFLAGED_OT ) )
			{
				gMercProfiles[ uiLoop ].bSkillTraits[1] = 0 ;
			}

			// if we are out of skill number, make it no trait, otherwise set our trait right
			for ( INT8 bCnt = 0; bCnt < 30; bCnt++ )
			{
				if ( gMercProfiles[ uiLoop ].bSkillTraits[ bCnt ] >= NUM_SKILLTRAITS_OT || gMercProfiles[ uiLoop ].bSkillTraits[ bCnt ] < NO_SKILLTRAIT_OT )
				{
					gMercProfiles[ uiLoop ].bSkillTraits[ bCnt ] = 0 ;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////

		memcpy( &(gMercProfiles[ uiLoop ].bBuddy), &(tempProfiles[ uiLoop ].bBuddy), 5 * sizeof (UINT8));
		gMercProfiles[ uiLoop ].bLearnToLike = tempProfiles[ uiLoop ].bLearnToLike ;
		gMercProfiles[ uiLoop ].bLearnToLikeTime = tempProfiles[ uiLoop ].bLearnToLikeTime ;

		memcpy( &(gMercProfiles[ uiLoop ].bHated), &(tempProfiles[ uiLoop ].bHated), 5 * sizeof (UINT8));
		memcpy( &(gMercProfiles[ uiLoop ].bHatedTime), &(tempProfiles[ uiLoop ].bHatedTime), 5 * sizeof (INT8));
		gMercProfiles[ uiLoop ].bLearnToHate = tempProfiles[ uiLoop ].bLearnToHate ;
		gMercProfiles[ uiLoop ].bLearnToHateTime = tempProfiles[ uiLoop ].bLearnToHateTime ;

		gMercProfiles[ uiLoop ].bRace						= tempProfiles[ uiLoop ].bRace;
		gMercProfiles[ uiLoop ].bNationality				= tempProfiles[ uiLoop ].bNationality;
		gMercProfiles[ uiLoop ].bAppearance					= tempProfiles[ uiLoop ].bAppearance;
		gMercProfiles[ uiLoop ].bAppearanceCareLevel		= tempProfiles[ uiLoop ].bAppearanceCareLevel;
		gMercProfiles[ uiLoop ].bRefinement					= tempProfiles[ uiLoop ].bRefinement;
		gMercProfiles[ uiLoop ].bRefinementCareLevel		= tempProfiles[ uiLoop ].bRefinementCareLevel;
		gMercProfiles[ uiLoop ].bHatedNationality			= tempProfiles[ uiLoop ].bHatedNationality;
		gMercProfiles[ uiLoop ].bHatedNationalityCareLevel	= tempProfiles[ uiLoop ].bHatedNationalityCareLevel;
		gMercProfiles[ uiLoop ].bRacist						= tempProfiles[ uiLoop ].bRacist;
		gMercProfiles[ uiLoop ].bSexist						= tempProfiles[ uiLoop ].bSexist;
		
		gMercProfiles[ uiLoop ].sSalary = tempProfiles[ uiLoop ].sSalary ;
		gMercProfiles[ uiLoop ].uiWeeklySalary = tempProfiles[ uiLoop ].uiWeeklySalary ;
		gMercProfiles[ uiLoop ].uiBiWeeklySalary = tempProfiles[ uiLoop ].uiBiWeeklySalary ;
		gMercProfiles[ uiLoop ].bMedicalDeposit = tempProfiles[ uiLoop ].bMedicalDeposit ;
		gMercProfiles[ uiLoop ].sMedicalDepositAmount = tempProfiles[ uiLoop ].sMedicalDepositAmount ;
		gMercProfiles[ uiLoop ].usOptionalGearCost = tempProfiles[ uiLoop ].usOptionalGearCost ;

		gMercProfiles[ uiLoop ].bArmourAttractiveness = tempProfiles[ uiLoop ].bArmourAttractiveness ;
		gMercProfiles[ uiLoop ].bMainGunAttractiveness = tempProfiles[ uiLoop ].bMainGunAttractiveness ;

		memcpy( &(gMercProfiles[ uiLoop ].usApproachFactor), &(tempProfiles[ uiLoop ].usApproachFactor), 4 * sizeof (UINT16));
		if (tempProfiles[ uiLoop ].fGoodGuy)
		{
			gMercProfiles[ uiLoop ].ubMiscFlags3 |= PROFILE_MISC_FLAG3_GOODGUY;
		}
		
		gMercProfiles[ uiLoop ].sSectorX = tempProfiles[uiLoop].sSectorX;
		gMercProfiles[ uiLoop ].sSectorY = tempProfiles[uiLoop].sSectorY;
		gMercProfiles[ uiLoop ].bSectorZ = tempProfiles[uiLoop].bSectorZ;
		gMercProfiles[ uiLoop ].ubCivilianGroup = tempProfiles[uiLoop].ubCivilianGroup;
		gMercProfiles[ uiLoop ].bTown = tempProfiles[uiLoop].bTown;
		gMercProfiles[ uiLoop ].bTownAttachment = tempProfiles[uiLoop].bTownAttachment;
		gMercProfiles[ uiLoop ].usBackground = tempProfiles[uiLoop].usBackground;
		gMercProfiles[ uiLoop ].usVoiceIndex = tempProfiles[uiLoop].usVoiceIndex;
		gMercProfiles[ uiLoop ].Type = tempProfiles[uiLoop].Type;

		gProfileType[uiLoop] = gMercProfiles[uiLoop].Type;
				
		switch ( tempProfiles[uiLoop].Type )
		{
		case PROFILETYPE_RPC:
		case PROFILETYPE_NPC:
		case PROFILETYPE_VEHICLE:
			break;

		case PROFILETYPE_NONE:
		case PROFILETYPE_AIM:
		case PROFILETYPE_MERC:		
		case PROFILETYPE_IMP:
		default:
			//Reset
			gMercProfiles[uiLoop].sSectorX = 0;
			gMercProfiles[uiLoop].sSectorY = 0;
			gMercProfiles[uiLoop].bSectorZ = 0;
			gMercProfiles[uiLoop].bTown = 0;
			gMercProfiles[uiLoop].bTownAttachment = 0;
			break;
		}
}

void OverwriteMercOpinionsWithXMLData( UINT32 uiLoop )
{
	for ( UINT8 cnt=0; cnt< NUMBER_OF_OPINIONS; ++cnt )
	{
		gMercProfiles[ uiLoop ].bMercOpinion[cnt] = tempProfiles[ uiLoop ].bMercOpinion[cnt] ;
	}
}

// SANDRO - added function
INT8 CheckMercsNearForCharTraits( UINT8 ubProfileID, INT8 bCharTraitID )
{
	INT8						bNumber = 0;
	UINT32					uiLoop;
	SOLDIERTYPE *		pSoldier;
	SOLDIERTYPE *		pTeammate;
	BOOLEAN				fOnlyOneException = FALSE;

	pSoldier = FindSoldierByProfileID( ubProfileID, FALSE );
	if (!pSoldier || !( pSoldier->bActive ) || !( pSoldier->bInSector ) )
	{
		return( -1 );
	}

	for ( uiLoop = gTacticalStatus.Team[ pSoldier->bTeam ].bFirstID; uiLoop <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; uiLoop++)
	{
		pTeammate = MercPtrs[ uiLoop ];
		if ( pTeammate == NULL )
		{
			continue;
		}
		// Are we actually here?
		if ( !(pTeammate->bActive) || !(pTeammate->bInSector) || ( pTeammate->flags.uiStatusFlags & SOLDIER_VEHICLE ) || (pTeammate->bAssignment == VEHICLE ) )
		{
			// is nowhere around!
			continue;
		}
		if ( pTeammate == pSoldier )
		{
			continue;
		}
		// Are we from our team an dalive?
		if ( pTeammate->bTeam == pSoldier->bTeam && pTeammate->stats.bLife >= OKLIFE )
		{
			// Are we close enough?
			if (PythSpacesAway( pSoldier->sGridNo, pTeammate->sGridNo ) <= 20)
			{
				switch ( bCharTraitID )
				{
					// Sociable - we are OK with everyone except those wicked malicious guys
					case CHAR_TRAIT_SOCIABLE:
						if ( !DoesMercHavePersonality( pTeammate, CHAR_TRAIT_MALICIOUS ) )
							++bNumber;
						break;
					// Loner can tolerate one other loner around
					case CHAR_TRAIT_LONER:
						if ( !fOnlyOneException && DoesMercHavePersonality( pTeammate, CHAR_TRAIT_LONER ) )
						{
							fOnlyOneException = TRUE;	
						}
						else
						{	
							++bNumber;
						}
						break;
					// Show-off, depends on gender and appearance of others
					case CHAR_TRAIT_SHOWOFF:
						// If we are male
						if ( gMercProfiles[ pSoldier->ubProfile ].ubBodyType <= STOCKYMALE )
						{
							if ( gMercProfiles[ pTeammate->ubProfile ].ubBodyType == REGFEMALE )
							{
								// Count as two if babe around!
								if ( gMercProfiles[ pTeammate->ubProfile ].bAppearance == APPEARANCE_BABE )
									bNumber += 2;
								// However remove one if ugly one
								else if ( gMercProfiles[ pTeammate->ubProfile ].bAppearance == APPEARANCE_UGLY )
									--bNumber;
								else 
									++bNumber;
							}
						}
						// If we are female
						else if ( gMercProfiles[ pSoldier->ubProfile ].ubBodyType == REGFEMALE )
						{
							if ( gMercProfiles[ pTeammate->ubProfile ].ubBodyType <= STOCKYMALE )
							{
								// Count as two if babe around!
								if ( gMercProfiles[ pTeammate->ubProfile ].bAppearance == APPEARANCE_BABE )
									bNumber += 2;
								// However remove one if ugly one
								else if ( gMercProfiles[ pTeammate->ubProfile ].bAppearance == APPEARANCE_UGLY )
									--bNumber;
								else 
									++bNumber;
							}
						}
						break;
					default:
						++bNumber;
						break;
				}
			}
		}
	}

	return( bNumber );
}

INT8 ProfileHasSkillTrait( INT32 ubProfileID, INT8 bSkillTrait )
{
	INT8 bNumTraits = 0;
	INT8 bNumMajorTraitsCounted = 0;
	INT8 bMaxTraits = gSkillTraitValues.ubMaxNumberOfTraits;
	INT8 bMaxMajorTraits = gSkillTraitValues.ubNumberOfMajorTraitsAllowed;

	// check old/new traits
	if (gGameOptions.fNewTraitSystem)
	{
		// exception for special merc
		//if ( gSkillTraitValues.fAllowSpecialMercTraitsException && ubProfileID == gSkillTraitValues.ubSpecialMercID)
		//{
		//	bMaxTraits++;
		//	bMaxMajorTraits++;
		//}
		
		for ( INT8 bCnt = 0; bCnt < bMaxTraits; ++bCnt )
		{
			if ( gMercProfiles[ubProfileID].bSkillTraits[bCnt] == bSkillTrait )
				++bNumTraits;
				
			if ( MajorTrait( gMercProfiles[ubProfileID].bSkillTraits[bCnt] ) )
				++bNumMajorTraitsCounted;

			// if we exceeded the allowed number of major traits, ignore the rest of them
			if ( bNumMajorTraitsCounted > bMaxMajorTraits )
				break;
		}

		if ( TwoStagedTrait( bSkillTrait ) )
			return (min( 2, bNumTraits ));

		return (min( 1, bNumTraits ));
	}
	else
	{
		if (gMercProfiles[ubProfileID].bSkillTraits[ 0 ] == bSkillTrait)
			++bNumTraits;

		if (gMercProfiles[ubProfileID].bSkillTraits[ 1 ] == bSkillTrait)
			++bNumTraits;	

		// Electronics, Ambidextrous and Camouflaged can only be of one grade
		if( bSkillTrait == ELECTRONICS_OT || 
			 bSkillTrait == AMBIDEXT_OT ||
			  bSkillTrait == CAMOUFLAGED_OT )
			return ( min(1, bNumTraits) );
		
		return ( bNumTraits );
	}
}
