#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
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
#endif

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

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

//new profiles by Jazz-------------------------------
AIM_PROFIL  gProfilesAIM[ NUM_PROFILES ];
MERC_PROFIL gProfilesMERC[ NUM_PROFILES ];
NPC_PROFIL  gProfilesNPC[ NUM_PROFILES ];
RPC_PROFIL  gProfilesRPC[ NUM_PROFILES ];
VEHICLE_PROFIL  gProfilesVehicle[ NUM_PROFILES ];
IMP_PROFIL gProfilesIMP[NUM_PROFILES];
//---------------------------------------------------

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

void RandomStartSalary();

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

BOOLEAN SaveNewSystemMercsToSaveGameFile( HWFILE hFile );
BOOLEAN LoadNewSystemMercsToSaveGameFile( HWFILE hFile );
void InitNewProfiles();


// WANNE: This method is not needed anymore. All the stuff is done in the LoadMercProfiles() method.
void InitNewProfiles()
{
	UINT32 uiProfileId = 0;
	
	UINT32 i = 0;
	
	UINT32 uiLoop;
	
	//new profiles by Jazz
	for( uiProfileId = 0; uiProfileId < NUM_PROFILES; uiProfileId++ )
	{	
		gProfilesAIM[ uiProfileId ].ProfilId = -1;
		gProfilesMERC[ uiProfileId ].ProfilId = -1;
		gProfilesNPC[ uiProfileId ].ProfilId = -1;
		gProfilesRPC[ uiProfileId ].ProfilId = -1;
		gProfilesVehicle[ uiProfileId ].ProfilId = -1;
		gProfilesIMP[ uiProfileId ].ProfilId = -1;
	}
	
	for( uiProfileId = 0; uiProfileId < 40; uiProfileId++ )
	{	
		gProfilesAIM[ uiProfileId ].ProfilId = uiProfileId;
	}
	
	for( uiProfileId = 40; uiProfileId < 51; uiProfileId++ )
	{	
		gProfilesMERC[ uiProfileId ].ProfilId = uiProfileId;
	}
	
	for( uiProfileId = FIRST_RPC; uiProfileId < FIRST_NPC; uiProfileId++ )
	{	
		gProfilesRPC[ uiProfileId ].ProfilId = uiProfileId;
	}
	
	for( uiProfileId = FIRST_NPC; uiProfileId < 160; uiProfileId++ )
	{	
		gProfilesNPC[ uiProfileId ].ProfilId = uiProfileId;
	}
	
	//IMP
	gProfilesIMP[ 51 ].ProfilId = 51; //IMP Male
	gProfilesIMP[ 52 ].ProfilId = 52; //IMP Male
	gProfilesIMP[ 53 ].ProfilId = 53; //IMP Male
	gProfilesIMP[ 54 ].ProfilId = 54; //IMP Female
	gProfilesIMP[ 55 ].ProfilId = 55; //IMP Female
	gProfilesIMP[ 56 ].ProfilId = 56; //IMP Female
	
	//Vehicle
	gProfilesVehicle[ 160 ].ProfilId = 160; //Hummer
	gProfilesVehicle[ 161 ].ProfilId = 161; //El Dorado
	gProfilesVehicle[ 162 ].ProfilId = 162; //Truck
	gProfilesVehicle[ 163 ].ProfilId = 163; //HELICOPTER
	gProfilesVehicle[ 164 ].ProfilId = 164; //TANK_CAR
	
	//NPC
	gProfilesNPC[ 169 ].ProfilId = 169; //none
	
	//MERC
	gProfilesMERC[ 165 ].ProfilId = 165; //Gaston
	gProfilesMERC[ 166 ].ProfilId = 166; //Stogie
	gProfilesMERC[ 167 ].ProfilId = 167; //Tex
	gProfilesMERC[ 168 ].ProfilId = 168; //Beggins
	
	for(uiLoop=0; uiLoop < 40; uiLoop++)
	{
		if ( gProfilesAIM[uiLoop].ProfilId == uiLoop )
		{
			AimMercArray[ i ]  = uiLoop;
		}
		i++;
	}	
			
	MAX_NUMBER_MERCS = 40;	
	START_MERC = 0;
	
	if ( gGameExternalOptions.fReadProfileDataFromXML == TRUE )
	{		
		for( uiProfileId = 0; uiProfileId < NUM_PROFILES; uiProfileId++ )
		{	
			gProfilesAIM[ uiProfileId ].ProfilId = -1;
			gProfilesMERC[ uiProfileId ].ProfilId = -1;
			gProfilesNPC[ uiProfileId ].ProfilId = -1;
			gProfilesRPC[ uiProfileId ].ProfilId = -1;
			gProfilesVehicle[ uiProfileId ].ProfilId = -1;
			gProfilesIMP[ uiProfileId ].ProfilId = -1;
		}
	}
		
	//------------------------------------------------------------------------
}

BOOLEAN SaveNewSystemMercsToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	FileWrite( hFile, &gProfilesAIM, sizeof( gProfilesAIM), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gProfilesAIM ) )
	{
		return( FALSE );
	}
	
	FileWrite( hFile, &gProfilesMERC, sizeof( gProfilesMERC), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gProfilesMERC ) )
	{
		return( FALSE );
	}
	
	FileWrite( hFile, &gProfilesNPC, sizeof( gProfilesNPC), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gProfilesNPC ) )
	{
		return( FALSE );
	}
	
	FileWrite( hFile, &gProfilesRPC, sizeof( gProfilesRPC), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gProfilesRPC ) )
	{
		return( FALSE );
	}
	
	FileWrite( hFile, &gProfilesVehicle, sizeof( gProfilesVehicle), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gProfilesVehicle ) )
	{
		return( FALSE );
	}
	
	FileWrite( hFile, &gProfilesIMP, sizeof( gProfilesIMP), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gProfilesIMP ) )
	{
		return( FALSE );
	}	
	return( TRUE );
}

BOOLEAN LoadNewSystemMercsToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;

	FileRead( hFile, &gProfilesAIM, sizeof( gProfilesAIM), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gProfilesAIM ) )
	{
		return( FALSE );
	}
	
	FileRead( hFile, &gProfilesMERC, sizeof( gProfilesMERC), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gProfilesMERC ) )
	{
		return( FALSE );
	}
	
	FileRead( hFile, &gProfilesNPC, sizeof( gProfilesNPC), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gProfilesNPC ) )
	{
		return( FALSE );
	}
	
	FileRead( hFile, &gProfilesRPC, sizeof( gProfilesRPC), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gProfilesRPC ) )
	{
		return( FALSE );
	}
	
	FileRead( hFile, &gProfilesVehicle, sizeof( gProfilesVehicle), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gProfilesVehicle ) )
	{
		return( FALSE );
	}
	
	FileRead( hFile, &gProfilesIMP, sizeof( gProfilesIMP), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gProfilesIMP ) )
	{
		return( FALSE );
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

	// not randomizing
	if ( gGameExternalOptions.ubMercRandomStats == 0 )
		return;

	// full stats random for all soldierIDs
	else if ( gGameExternalOptions.ubMercRandomStats == 1 )
	{	
		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			pProfile = &(gMercProfiles[cnt]);
			// Buggler: +/- random range will be limited due to proximity to min/max allowed value, slightly different for EXP as can ignore 0
			pProfile->bExpLevel += Random( 2 * min( Exp, min( 9 - pProfile->bExpLevel, pProfile->bExpLevel - 1 ) ) + 1 ) - min( Exp, min( 9 - pProfile->bExpLevel, pProfile->bExpLevel - 1 ) );
			pProfile->bLifeMax += Random( 2 * min( Stats, min( 100 - pProfile->bLifeMax, max( 0, pProfile->bLifeMax - 1  ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bLifeMax, max( 0, pProfile->bLifeMax - 1 ) ) );
			pProfile->bLife = pProfile->bLifeMax;
			pProfile->bAgility += Random( 2 * min( Stats, min( 100 - pProfile->bAgility, max( 0, pProfile->bAgility - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bAgility, max( 0, pProfile->bAgility - 1 ) ) );
			pProfile->bDexterity += Random( 2 * min( Stats, min( 100 - pProfile->bDexterity, max( 0, pProfile->bDexterity - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bDexterity, max( 0, pProfile->bDexterity - 1 ) ) );
			pProfile->bStrength += Random( 2 * min( Stats, min( 100 - pProfile->bStrength, max( 0, pProfile->bStrength - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bStrength, max( 0, pProfile->bStrength - 1 ) ) );
			pProfile->bLeadership += Random( 2 * min( Stats, min( 100 - pProfile->bLeadership, max( 0, pProfile->bLeadership - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bLeadership, max( 0, pProfile->bLeadership - 1 ) ) );
			pProfile->bWisdom += Random( min( Stats, 2 * min( 100 - pProfile->bWisdom, max( 0, pProfile->bWisdom - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bWisdom, max( 0, pProfile->bWisdom - 1 ) ) );
			pProfile->bMarksmanship += Random( 2 * min( Stats, min( 100 - pProfile->bMarksmanship, max( 0, pProfile->bMarksmanship - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bMarksmanship, max( 0, pProfile->bMarksmanship - 1 ) ) );
			pProfile->bMechanical += Random( 2 * min( Stats, min( 100 - pProfile->bMechanical, max( 0, pProfile->bMechanical - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bMechanical, max( 0, pProfile->bMechanical - 1 ) ) );
			pProfile->bExplosive += Random( 2 * min( Stats, min( 100 - pProfile->bExplosive, max( 0, pProfile->bExplosive - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bExplosive, max( 0, pProfile->bExplosive - 1 ) ) );
			pProfile->bMedical += Random( 2 * min( Stats, min( 100 - pProfile->bMedical, max( 0, pProfile->bMedical - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bMedical, max( 0, pProfile->bMedical - 1 ) ) );
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
					pProfile->bExpLevel += Random( 2 * min( Exp, min( 9 - pProfile->bExpLevel, pProfile->bExpLevel - 1 ) ) + 1 ) - min( Exp, min( 9 - pProfile->bExpLevel, pProfile->bExpLevel - 1 ) );

				if ( gRandomStatsValue[cnt].RandomLife == TRUE )
					pProfile->bLifeMax += Random( 2 * min( Stats, min( 100 - pProfile->bLifeMax, max( 0, pProfile->bLifeMax - 1  ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bLifeMax, max( 0, pProfile->bLifeMax - 1 ) ) );
					pProfile->bLife = pProfile->bLifeMax;

				if ( gRandomStatsValue[cnt].RandomAgility == TRUE )
					pProfile->bAgility += Random( 2 * min( Stats, min( 100 - pProfile->bAgility, max( 0, pProfile->bAgility - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bAgility, max( 0, pProfile->bAgility - 1 ) ) );
				
				if ( gRandomStatsValue[cnt].RandomDexterity == TRUE )
					pProfile->bDexterity += Random( 2 * min( Stats, min( 100 - pProfile->bDexterity, max( 0, pProfile->bDexterity - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bDexterity, max( 0, pProfile->bDexterity - 1 ) ) );
					
				if ( gRandomStatsValue[cnt].RandomStrength == TRUE )
					pProfile->bStrength += Random( 2 * min( Stats, min( 100 - pProfile->bStrength, max( 0, pProfile->bStrength - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bStrength, max( 0, pProfile->bStrength - 1 ) ) );
					
				if ( gRandomStatsValue[cnt].RandomLeadership == TRUE )
					pProfile->bLeadership += Random( 2 * min( Stats, min( 100 - pProfile->bLeadership, max( 0, pProfile->bLeadership - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bLeadership, max( 0, pProfile->bLeadership - 1 ) ) );
					
				if ( gRandomStatsValue[cnt].RandomWisdom == TRUE )
					pProfile->bWisdom += Random( min( Stats, 2 * min( 100 - pProfile->bWisdom, max( 0, pProfile->bWisdom - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bWisdom, max( 0, pProfile->bWisdom - 1 ) ) );
					
				if ( gRandomStatsValue[cnt].RandomMarksmanship == TRUE )
					pProfile->bMarksmanship += Random( 2 * min( Stats, min( 100 - pProfile->bMarksmanship, max( 0, pProfile->bMarksmanship - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bMarksmanship, max( 0, pProfile->bMarksmanship - 1 ) ) );
					
				if ( gRandomStatsValue[cnt].RandomMechanical == TRUE )
					pProfile->bMechanical += Random( 2 * min( Stats, min( 100 - pProfile->bMechanical, max( 0, pProfile->bMechanical - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bMechanical, max( 0, pProfile->bMechanical - 1 ) ) );
					
				if ( gRandomStatsValue[cnt].RandomExplosive == TRUE )	
					pProfile->bExplosive += Random( 2 * min( Stats, min( 100 - pProfile->bExplosive, max( 0, pProfile->bExplosive - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bExplosive, max( 0, pProfile->bExplosive - 1 ) ) );
					
				if ( gRandomStatsValue[cnt].RandomMedical == TRUE )
					pProfile->bMedical += Random( 2 * min( Stats, min( 100 - pProfile->bMedical, max( 0, pProfile->bMedical - 1 ) ) ) + 1 ) - min( Stats, min( 100 - pProfile->bMedical, max( 0, pProfile->bMedical - 1 ) ) );
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
					pProfile->bExpLevel += Random( 2 * min( Exp, min( 9 - pProfile->bExpLevel, pProfile->bExpLevel - 1 ) ) + 1 ) - min( Exp, min( 9 - pProfile->bExpLevel, pProfile->bExpLevel - 1 ) );

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
}

void RandomStartSalary()
{
	UINT32 cnt;
	MERCPROFILESTRUCT * pProfile;

	// Buggler: random starting salary
	if ( gGameExternalOptions.fMercRandomStartSalary == TRUE )
	{
		UINT8 SalaryPercentMod = gGameExternalOptions.ubMercRandomStartSalaryPercentMod;
		FLOAT SalaryMod;

		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			pProfile = &(gMercProfiles[cnt]);
			SalaryMod =  1 + ( (FLOAT) Random ( 2 * SalaryPercentMod + 1 ) - (FLOAT) SalaryPercentMod ) / 100;
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
	
	// ----- WANNE.PROFILE: New Profile Loading - BEGIN
	//InitNewProfiles();
	// ----- WANNE.PROFILE: New Profile Loading - END


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
	for( int uiProfileId = 0; uiProfileId < NUM_PROFILES; uiProfileId++ )
	{	
		gProfilesAIM[ uiProfileId ].ProfilId = -1;
		gProfilesMERC[ uiProfileId ].ProfilId = -1;
		gProfilesNPC[ uiProfileId ].ProfilId = -1;
		gProfilesRPC[ uiProfileId ].ProfilId = -1;
		gProfilesVehicle[ uiProfileId ].ProfilId = -1;
		gProfilesIMP[ uiProfileId ].ProfilId = -1;
	}

	int profileIndex = -1;

	for(uiLoop=0; uiLoop< NUM_PROFILES; uiLoop++)
	{
		profileIndex++;

		// Changed by ADB, rev 1513
		//if( !gMercProfiles[uiLoop].Load(fptr, true))
		//if( !gMercProfiles[uiLoop].Load(fptr, true, true, true))
		if( uiLoop < 170 && !gMercProfiles[uiLoop].Load(fptr, true, true, true))
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("FAILED to Read Merc Profiles from File %d %s",uiLoop, pFileName) );
			FileClose( fptr );
			return(FALSE);
		}

		
		
		// WANNE: For the new WF merc, there is no entry in prof.dat, so we have to reset some flags manually!		
		if (uiLoop >= 170)
		{
			gMercProfiles[uiLoop].ubMiscFlags = 0;
			gMercProfiles[uiLoop].ubMiscFlags2 = 0;
			gMercProfiles[uiLoop].ubMiscFlags3 = 0;

			gMercProfiles[uiLoop].uiTotalCostToDate = 0;
			gMercProfiles[uiLoop].iMercMercContractLength = 0;
		}

		// WANNE - BMP: DONE!
		//<SB> convert old MERCPROFILESTRUCT to new MERCPROFILESTRUCT
		gMercProfiles[uiLoop].sGridNo = gMercProfiles[uiLoop]._old_sGridNo;
		gMercProfiles[uiLoop].sPreCombatGridNo = gMercProfiles[uiLoop]._old_sGridNo;
		//</SB>

		/* CHRISL: For now, we should only overwrite prof.dat in the new inventory system.  Old system should still use
		prof.dat until we're sure we want to replace it with the xml file.
			Because the new WF mercs don't have entries in the prof*.dat files, we need to always load their equipment from
			MercStaringGear.xml, regardless of the inventory system we're going to use.*/
		if(UsingNewInventorySystem() == true || uiLoop >= 170 )
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
		if( DialogueDataFileExistsForProfile( (UINT8)uiLoop, 0, FALSE, NULL ) )
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

		for ( uiLoop2 = 0; uiLoop2 < gMercProfiles[uiLoop].inv.size(); uiLoop2++ )
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
			for ( uiLoop2 = 0; uiLoop2< gMercProfiles[ uiLoop ].inv.size(); uiLoop2++ )
			{
				if ( gMercProfiles[ uiLoop ].inv[ uiLoop2 ] != NOTHING )
				{
					//get the item
					usItem = gMercProfiles[ uiLoop ].inv[ uiLoop2 ];

					//add the cost
					tempGearCost += Item[ usItem ].usPrice;
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
					gProfilesAIM[uiLoop].ProfilId = uiLoop;
				}
				// MERC
				else
				{
					gProfilesMERC[uiLoop].ProfilId = uiLoop;
				}			
			}
			// IMP (51 - 56)
			else if (uiLoop >= 51 && uiLoop < FIRST_RPC)
			{
				gProfilesIMP[uiLoop].ProfilId = uiLoop;
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
							gProfilesMERC[uiLoop].ProfilId = uiLoop;
							break;
						// Vehicle
						case HUMVEE:
						case TANK_NW:
						case TANK_NE:
						case ELDORADO:
						case ICECREAMTRUCK:
						case JEEP:
							gProfilesVehicle[uiLoop].ProfilId = uiLoop;
							break;
						// Make it an RPC
						default:
							gProfilesRPC[uiLoop].ProfilId = uiLoop;
							break;
					}
				}
				else
				{
					// Last Index -> NPC
					if (uiLoop == 169)
					{
						gProfilesNPC[uiLoop].ProfilId = uiLoop;
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
								gProfilesVehicle[uiLoop].ProfilId = uiLoop;
								break;
							// RPC or NPC
							default:
								// RPC
								if (uiLoop >= FIRST_RPC && uiLoop < FIRST_NPC)
								{
									gProfilesRPC[uiLoop].ProfilId = uiLoop;
								}
								// NPC
								else
								{
									gProfilesNPC[uiLoop].ProfilId = uiLoop;
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
	UINT32 uiCnt;
	MERCPROFILESTRUCT *pProfile;
	UINT32 uiChance;

	// some randomly picked A.I.M. mercs will start off "on assignment" at the beginning of each new game
	for( uiCnt = 0; uiCnt < NUM_PROFILES; uiCnt++) // AIM_AND_MERC_MERCS
	{
		//new profiles by Jazz
		if ( gProfilesAIM[ uiCnt ].ProfilId == (UINT8)uiCnt || gProfilesMERC[ uiCnt ].ProfilId == (UINT8)uiCnt )
		{
		if( !IsProfileIdAnAimOrMERCMerc( (UINT8)uiCnt ) )
		{
			continue;
		}

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

	if (fPlayerMercsOnly)
	{
		ubLoopLimit = gTacticalStatus.Team[0].bLastID;
	}
	else
	{
		ubLoopLimit = MAX_NUM_SOLDIERS;
	}

	for (ubLoop = 0, pSoldier = MercPtrs[0]; ubLoop < ubLoopLimit; ubLoop++, pSoldier++)
	{
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
	MercCreateStruct.bBodyType					= pSoldier->ubBodyType;
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
			for(cnt = 0; cnt < (UINT32)BODYPOSFINAL; cnt++)
			{
				pNewSoldier->inv[cnt] = pSoldier->inv[cnt];
			}
			// Next, direct copy of the gunsling and knife pockets
			pNewSoldier->inv[GUNSLINGPOCKPOS] = pSoldier->inv[GUNSLINGPOCKPOS];
			pNewSoldier->inv[KNIFEPOCKPOS] = pSoldier->inv[KNIFEPOCKPOS];
			// Then, try to autoplace everything else
			for(cnt = BIGPOCKSTART; cnt < pNewSoldier->inv.size(); cnt++ )
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
	else if ( ubCharNum == DYNAMO && gubQuest[ QUEST_FREE_DYNAMO ] == QUESTINPROGRESS )
	{
		EndQuest( QUEST_FREE_DYNAMO, pSoldier->sSectorX, pSoldier->sSectorY );
	}
	// SANDRO - give exp and records quest point, if finally recruiting Miguel
	else if ( ubCharNum == MIGUEL )
	{
		GiveQuestRewardPoint( pSoldier->sSectorX, pSoldier->sSectorY, 6, MIGUEL );
	}

	// Flugente: people recruited in Arulco are known to the enemy as civilians or even soldiers. So they will be covert when recruited. Of course, this is not for the rebels...
	if ( ubCharNum == DEVIN || ubCharNum == HAMOUS || ubCharNum == SLAY || ubCharNum == VINCE || ubCharNum == MADDOG || ubCharNum == MICKY )
	{
		pNewSoldier->bSoldierFlagMask |= (SOLDIER_COVERT_CIV|SOLDIER_COVERT_NPC_SPECIAL);
	}
	else if ( ubCharNum == IGGY || ubCharNum == CONRAD )
	{
		pNewSoldier->bSoldierFlagMask |= (SOLDIER_COVERT_SOLDIER|SOLDIER_COVERT_NPC_SPECIAL);
	}

	// handle town loyalty adjustment
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
	if ( ubCharNum == IRA )
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

	// Flugente: people recruited in Arulco are known to the enemy as civilians or even soldiers. So they will be covert when recruited. Of course, this is not for the rebels...
	pNewSoldier->bSoldierFlagMask |= (SOLDIER_COVERT_CIV|SOLDIER_COVERT_NPC_SPECIAL);

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

	// O< check if this is the only guy in the sector....
	if ( gusSelectedSoldier == pSoldier->ubID )
	{
		gusSelectedSoldier = NOBODY;
	}

	// OK, UN set recruit flag..
	gMercProfiles[ ubCharNum ].ubMiscFlags &= (~PROFILE_MISC_FLAG_EPCACTIVE);

	// update sector values to current

	// check to see if this person should disappear from the map after this
	if ( (ubCharNum == JOHN || ubCharNum == MARY) && pSoldier->sSectorX == 13 && pSoldier->sSectorY == MAP_ROW_B && pSoldier->bSectorZ == 0 )
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
	}

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

	for (bLoop = 0; bLoop < 3; bLoop++)
	{
		if ( pProfile->bBuddy[bLoop] == ubBuddy )
		{
			return( bLoop );
		}
	}
	return( -1 );
}

INT8 WhichHated( UINT8 ubCharNum, UINT8 ubHated )
{
	MERCPROFILESTRUCT *	pProfile;
	INT8								bLoop;

	pProfile = &( gMercProfiles[ ubCharNum ] );

	for (bLoop = 0; bLoop < 3; bLoop++)
	{
		if ( pProfile->bHated[bLoop] == ubHated )
		{
			return( bLoop );
		}
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
	UINT32 uiCount;
	SOLDIERTYPE *pSoldier = NULL;
	MERCPROFILESTRUCT * pProfile;
	BOOLEAN				fDoCopy = FALSE;

	for( uiCount=0; uiCount < guiNumMercSlots; uiCount++)
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
					//if ( pSoldier->ubProfile < FIRST_RPC || pSoldier->ubProfile >= GASTON )
					//new profiles by Jazz
					if ( gProfilesIMP[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile || gProfilesAIM[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile || gProfilesMERC[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile)
					{
						fDoCopy = TRUE;
					}
				}
				else
				{
					//if ( pSoldier->ubProfile >= FIRST_RPC && pSoldier->ubProfile < GASTON )
					//new profiles by Jazz
					if ( gProfilesRPC[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile || gProfilesNPC[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile || gProfilesVehicle[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile )
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
	for(ubCnt=0; ubCnt< 3; ubCnt++)
	{
		//see if the merc has a buddy on the team
		bBuddyID = gMercProfiles[ ubMercID ].bBuddy[ubCnt];

		//If its not a valid 'buddy'
		if( bBuddyID < 0 )
			continue;

		if( IsMercOnTeam( bBuddyID ) )
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
	if ( pSoldier->ubProfile != NO_PROFILE && (gMercProfiles[ pSoldier->ubProfile ].bDisability == HEAT_INTOLERANT || MercUnderTheInfluence(pSoldier, DRUG_TYPE_HEATINTOLERANT) ) && !pSoldier->MercInWater())
	{
		if ((pSoldier->bSectorZ > 0) || (guiEnvWeather & ( WEATHER_FORECAST_SHOWERS | WEATHER_FORECAST_THUNDERSHOWERS )))
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

SOLDIERTYPE * SwapLarrysProfiles( SOLDIERTYPE * pSoldier )
{
	UINT8	ubSrcProfile;
	UINT8	ubDestProfile;
	MERCPROFILESTRUCT * pNewProfile;

	ubSrcProfile = pSoldier->ubProfile;
	if ( ubSrcProfile == LARRY_NORMAL)
	{
		ubDestProfile = LARRY_DRUNK;
	}
	else if ( ubSrcProfile == LARRY_DRUNK )
	{
		ubDestProfile = LARRY_NORMAL;
	}
	else
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
	pNewProfile->records.usItemsCombined = gMercProfiles[ ubSrcProfile ].records.usItemsStolen;
	pNewProfile->records.usMercsBandaged = gMercProfiles[ ubSrcProfile ].records.usMercsBandaged;
	pNewProfile->records.usSurgeriesMade = gMercProfiles[ ubSrcProfile ].records.usSurgeriesMade;
	pNewProfile->records.usNPCsDiscovered = gMercProfiles[ ubSrcProfile ].records.usNPCsDiscovered;
	pNewProfile->records.usSectorsDiscovered = gMercProfiles[ ubSrcProfile ].records.usSectorsDiscovered;
	pNewProfile->records.usMilitiaTrained = gMercProfiles[ ubSrcProfile ].records.usMilitiaTrained;
	pNewProfile->records.ubQuestsHandled = gMercProfiles[ ubSrcProfile ].records.ubQuestsHandled;

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
	pNewProfile->ubMiscFlags3 = gMercProfiles[ ubSrcProfile ].ubMiscFlags3;
	pNewProfile->ubDaysOfMoraleHangover = gMercProfiles[ ubSrcProfile ].ubDaysOfMoraleHangover;
	pNewProfile->ubNumTimesDrugUseInLifetime = gMercProfiles[ ubSrcProfile ].ubNumTimesDrugUseInLifetime;
	pNewProfile->uiPrecedentQuoteSaid = gMercProfiles[ ubSrcProfile ].uiPrecedentQuoteSaid;
	pNewProfile->sPreCombatGridNo = gMercProfiles[ ubSrcProfile ].sPreCombatGridNo;

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
	pSoldier->stats.bWisdom =				pNewProfile->bWisdom + pNewProfile->bWisdomDelta;
	pSoldier->stats.bExpLevel =			pNewProfile->bExpLevel + pNewProfile->bExpLevelDelta;
	pSoldier->stats.bLeadership =		pNewProfile->bLeadership + pNewProfile->bLeadershipDelta;

	pSoldier->stats.bMarksmanship =	pNewProfile->bMarksmanship + pNewProfile->bMarksmanshipDelta;
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
	else
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

	//if( ubProfileID < BIFF || ( ubProfileID >= BIFF && ubProfileID <= BUBBA ) || ubProfileID >= GASTON )
	if( gProfilesAIM[ ubProfileID ].ProfilId == ubProfileID || gProfilesMERC[ ubProfileID ].ProfilId == ubProfileID )
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

		gMercProfiles[ uiLoop ].bEvolution = tempProfiles[ uiLoop ].bEvolution ;
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
		
					//None
					if ( tempProfiles[uiLoop].Type == 0 )
					{
						//Reset
						gMercProfiles[ uiLoop ].sSectorX = 0;
						gMercProfiles[ uiLoop ].sSectorY = 0;
						gMercProfiles[ uiLoop ].bSectorZ = 0;
						gMercProfiles[ uiLoop ].bTown = 0;
						gMercProfiles[ uiLoop ].bTownAttachment = 0;
					}
					
					//AIM
					if ( tempProfiles[uiLoop].Type == 1 )
					{
						gProfilesAIM[ uiLoop ].ProfilId = uiLoop;
						
						//Reset
						gMercProfiles[ uiLoop ].sSectorX = 0;
						gMercProfiles[ uiLoop ].sSectorY = 0;
						gMercProfiles[ uiLoop ].bSectorZ = 0;
						gMercProfiles[ uiLoop ].bTown = 0;
						gMercProfiles[ uiLoop ].bTownAttachment = 0;
					}
					
					//MERC
					if ( tempProfiles[uiLoop].Type == 2 )
					{
						gProfilesMERC[ uiLoop ].ProfilId = uiLoop;
						
						//Reset
						gMercProfiles[ uiLoop ].sSectorX = 0;
						gMercProfiles[ uiLoop ].sSectorY = 0;
						gMercProfiles[ uiLoop ].bSectorZ = 0;
						gMercProfiles[ uiLoop ].bTown = 0;
						gMercProfiles[ uiLoop ].bTownAttachment = 0;
					}
					
					//RPC
					if ( tempProfiles[uiLoop].Type == 3 )
					{
						gProfilesRPC[ uiLoop ].ProfilId = uiLoop;
					}
					
					//NPC
					if ( tempProfiles[uiLoop].Type == 4 )
					{
						gProfilesNPC[ uiLoop ].ProfilId = uiLoop;
					}
					
					//Vehicle
					if ( tempProfiles[uiLoop].Type == 5 )
					{
						gProfilesVehicle[ uiLoop ].ProfilId = uiLoop;
					}
					
					//IMP
					if ( tempProfiles[uiLoop].Type == 6 )
					{
						gProfilesIMP[ uiLoop ].ProfilId = uiLoop;
						
						//Reset
						gMercProfiles[ uiLoop ].sSectorX = 0;
						gMercProfiles[ uiLoop ].sSectorY = 0;
						gMercProfiles[ uiLoop ].bSectorZ = 0;
						gMercProfiles[ uiLoop ].bTown = 0;
						gMercProfiles[ uiLoop ].bTownAttachment = 0;
					}
		
}

void OverwriteMercOpinionsWithXMLData( UINT32 uiLoop )
{
	UINT8 cnt;

	for (cnt=0; cnt< NUMBER_OF_OPINIONS; ++cnt )
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
	if (!pSoldier)
	{
		return( -1 );
	}
	if( !(pSoldier->bActive) || !(pSoldier->bInSector) )
	{
		return( -1 );	
	}

	for ( uiLoop = gTacticalStatus.Team[ pSoldier->bTeam ].bFirstID; uiLoop < gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; uiLoop++)
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
						if( gMercProfiles[ pTeammate->ubProfile ].bCharacterTrait != CHAR_TRAIT_MALICIOUS )
							bNumber++;
						break;
					// Loner can tolerate one other loner around
					case CHAR_TRAIT_LONER:
						if ( gMercProfiles[ pTeammate->ubProfile ].bCharacterTrait == CHAR_TRAIT_LONER && !fOnlyOneException )
						{
							fOnlyOneException = TRUE;	
						}
						else
						{	
							bNumber++;				
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
									bNumber--;
								else 
									bNumber++;
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
									bNumber--;
								else 
									bNumber++;
							}
						}
						break;
					default:
						bNumber++;
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
		if ( gSkillTraitValues.fAllowSpecialMercTraitsException && ubProfileID == gSkillTraitValues.ubSpecialMercID)
		{
			bMaxTraits++;
			bMaxMajorTraits++;
		}
		
		for ( INT8 bCnt = 0; bCnt < bMaxTraits; bCnt++ )
		{
			if ( TwoStagedTrait(bSkillTrait) )
			{
				if ( gMercProfiles[ubProfileID].bSkillTraits[ bCnt ] == bSkillTrait )
				{
					bNumTraits++;
					bNumMajorTraitsCounted++;
				}
				else if ( TwoStagedTrait(gMercProfiles[ubProfileID].bSkillTraits[ bCnt ]) )
				{
					bNumMajorTraitsCounted++;
				}
				// if we exceeded the allowed number of major traits, ignore the rest of them
				if ( bNumMajorTraitsCounted >= bMaxMajorTraits )
				{
					break;
				}
			}
			else
			{
				if ( gMercProfiles[ubProfileID].bSkillTraits[ bCnt ] == bSkillTrait )
				{
					bNumTraits++;
				}
			}
		}
		// cannot have more than 1 rade of minor traits or 2 grades of a major traits
		if( bSkillTrait > DOCTOR_NT && bSkillTrait != COVERT_NT )
			return ( min(1, bNumTraits) );
		else
			return ( min(2, bNumTraits) );
	}
	else
	{
		if (gMercProfiles[ubProfileID].bSkillTraits[ 0 ] == bSkillTrait)
			bNumTraits++;
		if (gMercProfiles[ubProfileID].bSkillTraits[ 1 ] == bSkillTrait)
			bNumTraits++;	

		// Electronics, Ambidextrous and Camouflaged can only be of one grade
		if( bSkillTrait == ELECTRONICS_OT || 
			 bSkillTrait == AMBIDEXT_OT ||
			  bSkillTrait == CAMOUFLAGED_OT )
			return ( min(1, bNumTraits) );
		else
			return ( bNumTraits );
	}
}
