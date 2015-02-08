#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <memory.h>
	#include "Inventory Choosing.h"
	#include "animation data.h"
	#include "Items.h"
	#include "Random.h"
	#include "Weapons.h"
	#include "Strategic Status.h"
	#include "Campaign.h"
	#include "GameSettings.h"
	#include "AIInternals.h"
	#include "strategicmap.h"
	#include "Auto Resolve.h"
	#include "Map Screen Interface Map.h"
	#include "EnemyItemDrops.h"
	#include "strategic.h"
	#include "Game Clock.h"
	#include "message.h"
	#include "Tactical Save.h"	// added by Flugente
#endif

/*
#define ENEMYAMMODROPRATE       100 //Madd 50      // % of time enemies drop ammunition
#define ENEMYGRENADEDROPRATE    100 //Madd 25      // % of time enemies drop grenades
#define ENEMYEQUIPDROPRATE       50 //Madd 15      // % of stuff enemies drop equipment

// only 1/10th of what enemies drop...
#define MILITIAAMMODROPRATE      100 //Madd 5      // % of time enemies drop ammunition
#define MILITIAGRENADEDROPRATE	 100 //Madd 3      // % of time enemies drop grenades
#define MILITIAEQUIPDROPRATE      50 //Madd 2      // % of stuff enemies drop equipment

*/
int ENEMYAMMODROPRATE;       //100 //Madd 50      // % of time enemies drop ammunition
int ENEMYGRENADEDROPRATE;    //100 //Madd 25      // % of time enemies drop grenades
int ENEMYEQUIPDROPRATE;       //50 //Madd 15      // % of stuff enemies drop equipment

// only 1/10th of what enemies drop...
int MILITIAAMMODROPRATE;      //100 //Madd 5      // % of time enemies drop ammunition
int MILITIAGRENADEDROPRATE;	 //100 //Madd 3      // % of time enemies drop grenades
int MILITIAEQUIPDROPRATE;      //50 //Madd 2      // % of stuff enemies drop equipment

UINT16 itemMortar;
UINT16 itemGrenadeLauncher;
UINT16 itemLAW;
UINT16 itemRPG;

#define MAX_MORTARS_PER_TEAM			1			// one team can't randomly roll more than this many mortars per sector


UINT32 guiMortarsRolledByTeam = 0;

//
// WDS note 08/06/2008:  This is no longer used
// ARMY_GUN_CHOICE_TYPE gRegularArmyGunChoices[ARMY_GUN_LEVELS] =
// {	// INDEX		CLASS				 #CHOICES
// 	{ /* 0 - lo pistols			*/	2,	SW38,					DESERTEAGLE,		-1,						-1,					-1					},
// 	{ /* 1 - hi pistols			*/	2,	GLOCK_17,			BERETTA_93R,		-1,						-1,					-1					},
// 	{ /* 2 - lo SMG/shotgun	*/	2,	M870,					MP5K,						-1,						-1,					-1					},
// 	{ /* 3 - lo rifles			*/	1,	MINI14,				-1,							-1,						-1,					-1					},
// 	{ /* 4 - hi SMGs				*/	2,	MAC10,				COMMANDO,				-1,						-1,					-1					},
// 	{ /* 5 - med rifles  		*/	1,	G41,					-1,							-1,						-1,					-1					},
// 	{ /* 6 - sniper rifles	*/	1,	M24,					-1,							-1,						-1,					-1					},
// 	{	/* 7 - hi rifles			*/	2,	M14,					C7,							-1,						-1,					-1					},
// 	{ /* 8 - best rifle			*/	1,	FNFAL,				-1,							-1,						-1,					-1					},
// 	{ /* 9 - machine guns		*/	1,	MINIMI,				-1,							-1,						-1,					-1					},
// 	{ /* 10- rocket rifle		*/	2,	ROCKET_RIFLE,	MINIMI,							-1,						-1,					-1					},
// };


// Flugente: created separate gun choices for different soldier classes
ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[SOLDIER_GUN_CHOICE_SELECTIONS][ARMY_GUN_LEVELS];// =
//{	// INDEX		CLASS				 #CHOICES
//	{ /* 0 - lo pistols			*/	6,	SW38,					BARRACUDA,			DESERTEAGLE,	GLOCK_17,		M1911,	BERETTA_92F,-1,-1,-1,-1 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1		},
//	{ /* 1 - hi pist/shtgn	*/	6,	GLOCK_18,			BERETTA_93R,		P7M8,	M870,				M950		,FIVE7,-1,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1		},
//	{ /* 2 - lo SMGs/shtgn	*/	6,	 TYPE85,				THOMPSON,			MP5, MP53,					MP5K, MP5SD,-1,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
//	{ /* 3 - lo rifles    	*/	4,	MINI14,				SKS,						BENNELLI,						SPAS15,					-1		,-1,-1,-1,-1,-1		,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	},
//	{ /* 4 - hi SMGs				*/	7,	MAC10,				AKSU74,		MICRO_UZI,		COMMANDO,			M960,					UMP45		,MP5N,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1		},
//	{ /* 5 - med rifles  		*/	6,	AKM,					G3A3,						G41,					AK74,				M900	,AK103,-1,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1		},
//	{ /* 6 - sniper rifles	*/	6,	DRAGUNOV,			M24,						PSG,						VAL_SILENT,					BARRETT		,MSG90A1,-1,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	},
//	{	/* 7 - hi rifles			*/	7,	FAMAS,				M14,						AUG,					C7,				ENFIELD,		G36K,G36C,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1	},
//	{ /* 8 - best rifle			*/	4,	FNFAL,				P90,						G11,						TAR21,					-1,		-1,-1,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1		},
//	{ /* 9 - machine guns		*/	3,	MINIMI,				RPK74,					HK21E,				-1,					-1,-1,-1,-1,-1,-1	,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1		},
//	{ /* 10- rocket rifle		*/	5,	ROCKET_RIFLE,	AUTO_ROCKET_RIFLE ,	 RPK74,				HK21E,		MINIMI , -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
//};

// Flugente: created separate gun choices for different soldier classes
ARMY_GUN_CHOICE_TYPE gArmyItemChoices[SOLDIER_GUN_CHOICE_SELECTIONS][MAX_ITEM_TYPES];

void RandomlyChooseWhichItemsAreDroppable( SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass );
void EquipTank( SOLDIERCREATE_STRUCT *pp );

void ChooseKitsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bKitClass );
void ChooseMiscGearForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bMiscClass );
void ChooseBombsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bBombClass );
// Headrock: Added function definition for LBE chooser
void ChooseLBEsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bLBEClass );

UINT16 PickARandomItem(UINT8 typeIndex, INT8 bSoldierClass);
UINT16 PickARandomItem(UINT8 typeIndex, INT8 bSoldierClass, UINT8 maxCoolness);
UINT16 PickARandomItem(UINT8 typeIndex, INT8 bSoldierClass, UINT8 maxCoolness, BOOLEAN getMatchingCoolness);
UINT16 PickARandomAttachment(UINT8 typeIndex, INT8 bSoldierClass, UINT16 usBaseItem, UINT8 maxCoolness, BOOLEAN getMatchingCoolness);


void InitArmyGunTypes(void)
{
	ARMY_GUN_CHOICE_TYPE *pGunChoiceTable;
	UINT32 uiGunLevel;
	UINT32 uiChoice;
	UINT16 bItemNo;
	UINT16 ubWeapon;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitArmyGunTypes ");

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitArmyGunTypes set gun choice table");
	// depending on selection of the gun nut option
	//if (gGameOptions.fGunNut)
	//{
		// use table of extended gun choices
		pGunChoiceTable = &(gExtendedArmyGunChoices[SOLDIER_CLASS_NONE][0]);
	//}
	//else
	//{
	//	// use table of regular gun choices
	//	pGunChoiceTable = &(gRegularArmyGunChoices[0]);
	//}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitArmyGunTypes pick a gun type");
	// for each gun category
	for (uiGunLevel = 0; uiGunLevel <	ARMY_GUN_LEVELS; uiGunLevel++)
	{
		// choose one the of the possible gun choices to be used by the army for this game & store it
		uiChoice = Random(pGunChoiceTable[ uiGunLevel ].ubChoices);

		bItemNo = pGunChoiceTable[ uiGunLevel ].bItemNo[ uiChoice ];
		AssertMsg(bItemNo != -1, "Invalid army gun choice in table");
		gStrategicStatus.ubStandardArmyGunIndex[uiGunLevel] = (UINT8) bItemNo;
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitArmyGunTypes mark weapons as not dropped");
	// set all flags that track whether this weapon type has been dropped before to FALSE
	for (ubWeapon = 0; ubWeapon < MAXITEMS; ++ubWeapon)
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("InitArmyGunTypes marking weapons %d", ubWeapon));
		gStrategicStatus.fWeaponDroppedAlready[ubWeapon] = FALSE;
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitArmyGunTypes marking 38s as dropped");
	// avoid auto-drops for the gun class with the crappiest guns in it
	MarkAllWeaponsOfSameGunClassAsDropped( SW38 );

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitArmyGunTypes done");
}



INT8 GetWeaponClass( UINT16 usGun )
{
	UINT32		uiGunLevel, uiLoop;

	// always use the extended list since it contains all guns...
	if ( IsWeapon (usGun) )
	{
		for (uiGunLevel = 0; uiGunLevel <	ARMY_GUN_LEVELS; uiGunLevel++)
		{
			for ( uiLoop = 0; uiLoop < gExtendedArmyGunChoices[SOLDIER_CLASS_NONE][ uiGunLevel ].ubChoices; uiLoop++ )
			{
				if ( gExtendedArmyGunChoices[SOLDIER_CLASS_NONE][ uiGunLevel ].bItemNo[ uiLoop ] == usGun )
				{
					return( (INT8) uiGunLevel );
				}
			}
		}
	}
	return( -1 );
}



void MarkAllWeaponsOfSameGunClassAsDropped( UINT16 usWeapon )
{
	INT8 bGunClass;
	UINT32 uiLoop;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"MarkAllWeaponsOfSameGunClassAsDropped");


	// mark that item itself as dropped, whether or not it's part of a gun class
	gStrategicStatus.fWeaponDroppedAlready[ usWeapon ] = TRUE;

	bGunClass = GetWeaponClass( usWeapon );

	// if the gun belongs to a gun class (mortars, GLs, LAWs, etc. do not and are handled independently)
	if ( bGunClass != -1 )
	{
		// then mark EVERY gun in that class as dropped
		for ( uiLoop = 0; uiLoop < gExtendedArmyGunChoices[SOLDIER_CLASS_NONE][ bGunClass ].ubChoices; uiLoop++ )
		{
			gStrategicStatus.fWeaponDroppedAlready[ gExtendedArmyGunChoices[SOLDIER_CLASS_NONE][ bGunClass ].bItemNo[ uiLoop ] ] = TRUE;
		}
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"MarkAllWeaponsOfSameGunClassAsDropped done");
}



//Chooses equipment based on the relative equipment level (0-4) with best being 4.  It allocates a range
//of equipment to choose from.
//NOTE:  I'm just winging it for the decisions on which items that different groups can have.  Basically,
// there are variations, so a guy at a certain level may get a better gun and worse armour or vice versa.
void GenerateRandomEquipment( SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass, INT8 bEquipmentRating )
{
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GenerateRandomEquipment"));
	OBJECTTYPE *pItem;
	//general rating information
	INT8 bRating = 0;
	//numbers of items
	INT8 bAmmoClips = 0;
	INT8 bGrenades = 0;
	BOOLEAN fAttachment = FALSE;
	//item levels
	INT8 bWeaponClass = 0;
	INT8 bHelmetClass = 0;
	INT8 bVestClass = 0;
	INT8 bLeggingClass = 0;
	INT8 bAttachClass = 0;
	INT8 bGrenadeClass = 0;
	INT8 bKnifeClass = 0;
	INT8 bKitClass = 0;
	INT8 bMiscClass = 0;
	INT8 bBombClass = 0;
	// Headrock: Added Zeroed LBE integer
	INT8 bLBEClass = 0;
	//special weapons
	BOOLEAN fMortar = FALSE;
	BOOLEAN fGrenadeLauncher = FALSE;
	BOOLEAN fLAW = FALSE;
	BOOLEAN fRPG = FALSE;
	UINT32 i;
	INT8 bEquipmentModifier;
	UINT8 ubMaxSpecialWeaponRoll;


	Assert( pp );

	// kids don't get anything 'cause they don't have any weapon animations and the rest is inappropriate
	if ( ( pp->bBodyType == HATKIDCIV ) || ( pp->bBodyType == KIDCIV ) )
	{
		return;
	}


	if ( ( pp->bBodyType == TANK_NE ) || ( pp->bBodyType == TANK_NW ) )
	{
		EquipTank( pp );
		return;
	}


	Assert( ( bSoldierClass >= SOLDIER_CLASS_NONE ) && ( bSoldierClass <= SOLDIER_CLASS_ELITE_MILITIA ) || bSoldierClass == SOLDIER_CLASS_TANK );
	Assert( ( bEquipmentRating >= 0 ) && ( bEquipmentRating <= 4 ) );


	// equipment level is modified by 1/10 of the difficulty percentage, -5, so it's between -5 to +5
	// (on normal, this is actually -4 to +4, easy is -5 to +3, and hard is -3 to +5)
	//bEquipmentModifier = bEquipmentRating + ( ( CalcDifficultyModifier( bSoldierClass ) / 10 ) - 5 );

	// Lesh 1.13 mod notes
	// My idea of enemy choice balancing is based on bEquipmentModifier, which is influenced by
	// game progress and difficulty. You can see that I throw out bEquipmentRating from the formulae.
	// This variable has a very big influence to result. Also as you can see below I commented
	// out such constants as BAD_ADMINISTRATOR_EQUIPMENT_RATING. They have big influence too,
	// maybe because of enemy choices table was incorrectly assembled. For example:
	// BAD_ADMINISTRATOR_EQUIPMENT_RATING is equal 1 and BAD_ELITE_EQUIPMENT_RATING is equal 5.
	// So in the same time when admins hold crappy weapons, elites get stuff up to nearly 6 level
	// in Drassen counter-attack. In this situation it is very hard to make balanced xml-file(IMHO).
	// Made in optional form

	// SANDRO - new behaviour of progress setting
	bEquipmentModifier = bEquipmentRating + ( ( CalcDifficultyModifier( bSoldierClass ) / 10 ) - 5 );
	switch( gGameOptions.ubProgressSpeedOfItemsChoices )
	{
		case ITEM_PROGRESS_VERY_SLOW:
			// On "Very Slow", at the beginning of the game, the modifier is -5 and goes up to -1 at +-100% game progress
			bEquipmentModifier -= (1 + (4 - (HighestPlayerProgressPercentage() / 25) ));
			break;
		case ITEM_PROGRESS_SLOW:
			// On "Slow", it's -3 until hit at least 10% game progress, then -2 till 40%, then -1 till 80%
			if (HighestPlayerProgressPercentage() < 10 )
				bEquipmentModifier -= 3;
			else if (HighestPlayerProgressPercentage() < 40 )
				bEquipmentModifier -= 2;
			else if (HighestPlayerProgressPercentage() < 80 )
				bEquipmentModifier -= 1;
			break;
		case ITEM_PROGRESS_NORMAL:
			// Nothing needed here
			break;
		case ITEM_PROGRESS_FAST:
			// On "Fast", it's just +1 at the beginning and rise up through out the game
			if (HighestPlayerProgressPercentage() < 25 )
				bEquipmentModifier += 1;
			else if (HighestPlayerProgressPercentage() < 60 )
				bEquipmentModifier += 2;
			else if (HighestPlayerProgressPercentage() < 90 )
				bEquipmentModifier += 3;
			else
				bEquipmentModifier += 4;
			break;
		case ITEM_PROGRESS_VERY_FAST:
			// "Very Fast" just make everyone loaded with top gear quickly
			if (HighestPlayerProgressPercentage() < 10 )
				bEquipmentModifier += 2;
			else if (HighestPlayerProgressPercentage() < 25 )
				bEquipmentModifier += 3;
			else if (HighestPlayerProgressPercentage() < 50 )
				bEquipmentModifier += 4;
			else if (HighestPlayerProgressPercentage() < 80 )
				bEquipmentModifier += 5;
			else
				bEquipmentModifier += 6;
			break;
		default:
			break;
	}

	switch( bSoldierClass )
	{
		case SOLDIER_CLASS_NONE:
			// ammo is here only so that civilians with pre-assigned ammo will get some clips for it!
			bAmmoClips = (INT8)(2 + Random( 2 ));

			// civilians get nothing, anyone who should get something should be preassigned by Linda
			break;

		// SANDRO - I've split the item choosing below, so the item choices for militia can be
		// handled differently than for enemies.
		case SOLDIER_CLASS_ADMINISTRATOR:
			//if ( gGameOptions.fSlowProgressForEnemyItemsChoice )
			//	bRating = bEquipmentModifier - ( bSoldierClass == SOLDIER_CLASS_ADMINISTRATOR );
			//else
			bRating = BAD_ADMINISTRATOR_EQUIPMENT_RATING + bEquipmentModifier;

			// Equipment quality modifier
			if ( gGameExternalOptions.sEnemyAdminEquipmentQualityModifier != 0 )
				bRating += gGameExternalOptions.sEnemyAdminEquipmentQualityModifier;

			bRating = (INT8)max( MIN_EQUIPMENT_CLASS, min( MAX_EQUIPMENT_CLASS, bRating ) );

			bWeaponClass = bRating;
			//Headrocktest, remove for release
			bLBEClass = bRating;

			//Note:  in some cases the class of armour and/or helmet won't be high enough to make
			//			 the lowest level.
			bVestClass = bRating;
			bHelmetClass = bRating;
			// no leggings

			if( Chance( 30 ) )
				bKnifeClass = bRating;

			bAmmoClips = (INT8)(3 + Random( 2 ));

			if( bRating >= GOOD_ADMINISTRATOR_EQUIPMENT_RATING )
			{
				bAmmoClips++;

				bKitClass = bRating;
				bMiscClass = bRating;
				// Headrock: Low Level LBEs possible on Administrators:
				bLBEClass = bRating;
			}

			if( bRating >= GREAT_ADMINISTRATOR_EQUIPMENT_RATING )
			{
				bGrenades = 1, bGrenadeClass = bRating;
			}

			if( ( bRating > MIN_EQUIPMENT_CLASS ) && bRating < MAX_EQUIPMENT_CLASS )
			{ //Randomly decide if there is to be an upgrade of guns vs armour (one goes up, the other down)
				switch( Random( 5 ) )
				{
					case 0:	bWeaponClass++, bVestClass--;					break;	//better gun, worse armour
					case 1:	bWeaponClass--, bVestClass++;					break;	//worse gun, better armour
				}
			}
			break;
		case SOLDIER_CLASS_GREEN_MILITIA:

			bRating = BAD_ADMINISTRATOR_EQUIPMENT_RATING + bEquipmentModifier;

			///////////////////////////////////////////////////////////////////////////////////////////
			// SANDRO - Militia equipment quality bonus
			if ( gGameExternalOptions.bGreenMilitiaEquipmentQualityModifier != 0 )
				bRating += gGameExternalOptions.bGreenMilitiaEquipmentQualityModifier;
			///////////////////////////////////////////////////////////////////////////////////////////

			bRating = (INT8)max( MIN_EQUIPMENT_CLASS, min( MAX_EQUIPMENT_CLASS, bRating ) );

			bWeaponClass = bRating;
			//Headrocktest, remove for release
			//bLBEClass = bRating;

			//Note:  in some cases the class of armour and/or helmet won't be high enough to make
			//			 the lowest level.
			bVestClass = bRating;
			bHelmetClass = bRating;

			if( Chance( 15 ) )
				bKnifeClass = bRating;

			bAmmoClips = (INT8)(3 + Random( 2 ));

			if( bRating >= GOOD_ADMINISTRATOR_EQUIPMENT_RATING )
			{
				bAmmoClips++;

				bMiscClass = bRating;
			}

			if( bRating >= GREAT_ADMINISTRATOR_EQUIPMENT_RATING )
			{
				bGrenades = 1, bGrenadeClass = bRating;
			}

			if( ( bRating > MIN_EQUIPMENT_CLASS ) && bRating < MAX_EQUIPMENT_CLASS )
			{ //Randomly decide if there is to be an upgrade of guns vs armour (one goes up, the other down)
				switch( Random( 5 ) )
				{
					case 0:	bWeaponClass++, bVestClass--;					break;	//better gun, worse armour
					case 1:	bWeaponClass--, bVestClass++;					break;	//worse gun, better armour
				}
			}
			break;

		case SOLDIER_CLASS_ARMY:
			//army guys tend to have a broad range of equipment
			//if ( gGameOptions.fSlowProgressForEnemyItemsChoice )
			//	bRating = bEquipmentModifier - ( bSoldierClass == SOLDIER_CLASS_ARMY );
			//else
				bRating = BAD_ARMY_EQUIPMENT_RATING + bEquipmentModifier;

			// Equipment quality modifier
			if ( gGameExternalOptions.sEnemyRegularEquipmentQualityModifier != 0 )
				bRating += gGameExternalOptions.sEnemyRegularEquipmentQualityModifier;
				
			bRating = (INT8)max( MIN_EQUIPMENT_CLASS, min( MAX_EQUIPMENT_CLASS, bRating ) );

			bWeaponClass = bRating;
			bVestClass = bRating;
			bHelmetClass = bRating;
			bGrenadeClass = bRating;
			// Headrock: Added LBE set to Coolness Rating
			bLBEClass = bRating;
			//WarmSteel - attachments don't need to be as high a class, controversional and might be better to externalize?
			bAttachClass = bRating*3/8;


			if( ( bRating >= GOOD_ARMY_EQUIPMENT_RATING ) && Chance(33) )
			{
				fAttachment = TRUE;
				bAttachClass = bRating*5/8;
			}

			bAmmoClips = (INT8)(3 + Random( 2 ));

			if( bRating >= AVERAGE_ARMY_EQUIPMENT_RATING )
			{
				bGrenades = (INT8)Random( 2 );
				bKitClass = bRating;
				bMiscClass = bRating;
			}

			if( bRating >= GOOD_ARMY_EQUIPMENT_RATING )
			{
				bGrenades++;
			}

			if( bRating >= GREAT_ARMY_EQUIPMENT_RATING )
			{
				bGrenades++;

				bLeggingClass = bRating;

				if ( Chance( 25 ) )
				{
					bBombClass = bRating;
				}
			}

			if( Chance( 35 ) )
				bKnifeClass = bRating;

			// Headrock: Chance for soldier to carry better LBE
			//if( Chance( 50 ) )
			//	bLBEClass++;

			if( ( bRating > MIN_EQUIPMENT_CLASS ) && bRating < MAX_EQUIPMENT_CLASS )
			{
				switch( Random( 7 ) )
				{
					case 3:	bWeaponClass++, bVestClass--;		break;	//better gun, worse armour
					case 4: bWeaponClass--, bVestClass++;		break;	//worse gun, better armour
					case 5: bVestClass++, bHelmetClass--;		break;	//better armour, worse helmet
					case 6: bVestClass--, bHelmetClass++;		break;	//worse armour, better helmet
				}
			}

			// if well-enough equipped, 1/5 chance of something really special
			if( ( bRating >= GREAT_ARMY_EQUIPMENT_RATING ) && ( Random( 100 ) < 20 ) )
			{
				//give this man a special weapon!  No mortars if underground, however
				ubMaxSpecialWeaponRoll = ( !IsAutoResolveActive() && ( gbWorldSectorZ != 0 ) ) ? 6 : 7;
				switch ( Random ( ubMaxSpecialWeaponRoll ) )
				{
					case 0:
					case 1:
					case 2:
						if ( pp->bExpLevel >= 3 )
						{
							//grenade launcher
							fGrenadeLauncher = TRUE;
							bGrenades = 3 + (INT8)(Random( 3 )); //3-5
						}
						break;

					case 3:
					case 4:
						if ( pp->bExpLevel >= 4 )
						{
							// LAW rocket launcher
							fLAW = TRUE;
						}
						break;
					case 5:
						if ( pp->bExpLevel >= 5 )
						{
							// RPG rocket launcher
							fRPG = TRUE;
							bGrenades = 2 + (INT8)(Random( 3 )); //2-4
							bGrenadeClass = RPG_GRENADE_CLASS;
						}
						break;

					case 6:
						// one per team maximum!
						if ( ( pp->bExpLevel >= 5 ) && ( guiMortarsRolledByTeam < MAX_MORTARS_PER_TEAM ) )
						{
							//mortar
							fMortar = TRUE;
							guiMortarsRolledByTeam++;

							// the grenades will actually represent mortar shells in this case
							bGrenades = 2 + (INT8)(Random( 3 )); //2-4
							bGrenadeClass = MORTAR_GRENADE_CLASS;
						}
						break;
				}
			}
			break;
		case SOLDIER_CLASS_REG_MILITIA:
			
			bRating = BAD_ARMY_EQUIPMENT_RATING + bEquipmentModifier;

			///////////////////////////////////////////////////////////////////////////////////////////
			// SANDRO - Militia equipment quality bonus
			if ( gGameExternalOptions.bRegularMilitiaEquipmentQualityModifier != 0 )
				bRating += gGameExternalOptions.bRegularMilitiaEquipmentQualityModifier;
			///////////////////////////////////////////////////////////////////////////////////////////

			bRating = (INT8)max( MIN_EQUIPMENT_CLASS, min( MAX_EQUIPMENT_CLASS, bRating ) );

			bWeaponClass = bRating;
			bVestClass = bRating;
			bHelmetClass = bRating;
			bGrenadeClass = bRating;
			//WarmSteel - attachments don't need to be as high a class, controversional and might be better to externalize?
			bAttachClass = bRating*3/8;


			if( ( bRating >= GOOD_ARMY_EQUIPMENT_RATING ) && Chance(33) )
			{
				fAttachment = TRUE;
				bAttachClass = bRating*5/8;
			}

			bAmmoClips = (INT8)(3 + Random( 2 ));

			if( bRating >= AVERAGE_ARMY_EQUIPMENT_RATING )
			{
				bGrenades = (INT8)Random( 2 );
				bKitClass = bRating;
				bMiscClass = bRating;
			}

			if( bRating >= GOOD_ARMY_EQUIPMENT_RATING )
			{
				bGrenades++;
			}

			if( bRating >= GREAT_ARMY_EQUIPMENT_RATING )
			{
				bGrenades++;

				bLeggingClass = bRating;

				if ( Chance( 25 ) )
				{
					bBombClass = bRating;
				}
			}

			if( Chance( 20 ) )
				bKnifeClass = bRating;

			if( ( bRating > MIN_EQUIPMENT_CLASS ) && bRating < MAX_EQUIPMENT_CLASS )
			{
				switch( Random( 7 ) )
				{
					case 3:	bWeaponClass++, bVestClass--;		break;	//better gun, worse armour
					case 4: bWeaponClass--, bVestClass++;		break;	//worse gun, better armour
					case 5: bVestClass++, bHelmetClass--;		break;	//better armour, worse helmet
					case 6: bVestClass--, bHelmetClass++;		break;	//worse armour, better helmet
				}
			}

			// if well-enough equipped, 1/5 chance of something really special
			if( ( bRating >= GREAT_ARMY_EQUIPMENT_RATING ) && ( Random( 100 ) < 20 ) )
			{
				//give this man a special weapon!  No mortars if underground, however
				ubMaxSpecialWeaponRoll = ( !IsAutoResolveActive() && ( gbWorldSectorZ != 0 ) ) ? 6 : 7;
				switch ( Random ( ubMaxSpecialWeaponRoll ) )
				{
					case 0:
					case 1:
					case 2:
						if ( pp->bExpLevel >= 3 )
						{
							//grenade launcher
							fGrenadeLauncher = TRUE;
							bGrenades = 3 + (INT8)(Random( 3 )); //3-5
						}
						break;

					case 3:
					case 4:
						if ( pp->bExpLevel >= 4 )
						{
							// LAW rocket launcher
							fLAW = TRUE;
						}
						break;
					case 5:
						if ( pp->bExpLevel >= 5 )
						{
							// RPG rocket launcher
							fRPG = TRUE;
							bGrenades = 2 + (INT8)(Random( 3 )); //2-4
							bGrenadeClass = RPG_GRENADE_CLASS;
						}
						break;
				}
			}
			break;

		case SOLDIER_CLASS_ELITE:
			//if ( gGameOptions.fSlowProgressForEnemyItemsChoice )
			//	bRating = bEquipmentModifier - ( bSoldierClass == SOLDIER_CLASS_ELITE );
			//else
				bRating = BAD_ELITE_EQUIPMENT_RATING + bEquipmentModifier;

			// Equipment quality modifier
			if ( gGameExternalOptions.sEnemyEliteEquipmentQualityModifier != 0 )
				bRating += gGameExternalOptions.sEnemyEliteEquipmentQualityModifier;

			bRating = (INT8)max( MIN_EQUIPMENT_CLASS, min( MAX_EQUIPMENT_CLASS, bRating ) );

			bWeaponClass = bRating;
			bHelmetClass = bRating;
			bVestClass = bRating;
			bLeggingClass = bRating;
			bAttachClass = bRating*7/8;
			bGrenadeClass = bRating;
			bKitClass = bRating;
			bMiscClass = bRating;

			// Headrock: Elite LBEs
			bLBEClass = bRating;

			if ( Chance( 25 ) )
			{
				bBombClass = bRating;
			}

			bAmmoClips = (INT8)(3 + Random( 2 ));
			bGrenades = (INT8)(2 + Random( 3 ));

			if( ( bRating >= AVERAGE_ELITE_EQUIPMENT_RATING ) && Chance(75) )
			{
				fAttachment = TRUE;
				bAttachClass = bRating;
			}

			if( Chance( 35 ) )
				bKnifeClass = bRating;

			if( ( bRating > MIN_EQUIPMENT_CLASS ) && bRating < MAX_EQUIPMENT_CLASS )
			{
				UINT32 uiRange = ((UsingNewInventorySystem() == false)) ? Random(11) : Random(12);
				switch( uiRange )
				{
					case 4:		bWeaponClass++, bVestClass--;		break;
					case 5:		bWeaponClass--, bVestClass--;		break;
					case 6:		bVestClass++, bHelmetClass--;		break;
					case 7:		bGrenades += 2;						break;
					case 8:		bHelmetClass++;						break;
					case 9:		bVestClass++;						break;
					case 10:	bWeaponClass++;						break;
					case 11:	bLBEClass++;						break;
				}
			}

			// if well-enough equipped, 1/3 chance of something really special
			if( ( bRating >= GOOD_ELITE_EQUIPMENT_RATING ) && ( Random( 100 ) < 33 ) )
			{
				//give this man a special weapon!  No mortars if underground, however
				ubMaxSpecialWeaponRoll = ( !IsAutoResolveActive() && ( gbWorldSectorZ != 0 ) ) ? 6 : 7;
				switch ( Random ( ubMaxSpecialWeaponRoll ) )
				{
					case 0:
					case 1:
					case 2:
						//grenade launcher
						fGrenadeLauncher = TRUE;
						bGrenades = 4 + (INT8)(Random( 4 )); //4-7
						break;
					case 3:
					case 4:
						// LAW rocket launcher
						fLAW = TRUE;
						break;
					case 5:
						// RPG rocket launcher
						fRPG = TRUE;
						bGrenades = 3 + (INT8)(Random( 5 )); //3-7
						bGrenadeClass = RPG_GRENADE_CLASS;
						break;
					case 6:
						// one per team maximum!
						if ( guiMortarsRolledByTeam < MAX_MORTARS_PER_TEAM )
						{
							//mortar
							fMortar = TRUE;
							guiMortarsRolledByTeam++;

							// the grenades will actually represent mortar shells in this case
							bGrenades = 3 + (INT8)(Random( 5 )); //3-7
							bGrenadeClass = MORTAR_GRENADE_CLASS;
						}
						break;
				}
			}
			break;
		case SOLDIER_CLASS_ELITE_MILITIA:
			
			bRating = BAD_ELITE_EQUIPMENT_RATING + bEquipmentModifier;

			///////////////////////////////////////////////////////////////////////////////////////////
			// SANDRO - Militia equipment quality bonus
			if ( gGameExternalOptions.bVeteranMilitiaEquipmentQualityModifier != 0 )
				bRating += gGameExternalOptions.bVeteranMilitiaEquipmentQualityModifier;
			///////////////////////////////////////////////////////////////////////////////////////////

			bRating = (INT8)max( MIN_EQUIPMENT_CLASS, min( MAX_EQUIPMENT_CLASS, bRating ) );

			bWeaponClass = bRating;
			bHelmetClass = bRating;
			bVestClass = bRating;
			bLeggingClass = bRating;
			bAttachClass = bRating*7/8;
			bGrenadeClass = bRating;
			bKitClass = bRating;
			bMiscClass = bRating;

			if ( Chance( 25 ) )
			{
				bBombClass = bRating;
			}

			bAmmoClips = (INT8)(3 + Random( 2 ));
			bGrenades = (INT8)(2 + Random( 3 ));
			
			if( ( bRating >= AVERAGE_ELITE_EQUIPMENT_RATING ) && Chance(75) )
			{
				fAttachment = TRUE;
				bAttachClass = bRating;
			}

			if( Chance( 25 ) )
				bKnifeClass = bRating;

			if( ( bRating > MIN_EQUIPMENT_CLASS ) && bRating < MAX_EQUIPMENT_CLASS )
			{
				UINT32 uiRange = ((UsingNewInventorySystem() == false)) ? Random(10) : Random(11);
				switch( uiRange )
				{
					case 4:		bWeaponClass++, bVestClass--;		break;
					case 5:		bWeaponClass--, bVestClass--;		break;
					case 6:		bVestClass++, bHelmetClass--;		break;
					case 7:		bGrenades += 2;						break;
					case 8:		bHelmetClass++;						break;
					case 9:		bVestClass++;						break;
					case 10:	bWeaponClass++;						break;
					//case 11:	bLBEClass++;						break;
				}
			}

			// if well-enough equipped, 1/3 chance of something really special
			if( ( bRating >= GOOD_ELITE_EQUIPMENT_RATING ) && ( Random( 100 ) < 60 ) )
			{
				//give this man a special weapon!  No mortars if underground, however
				ubMaxSpecialWeaponRoll = ( !IsAutoResolveActive() && ( gbWorldSectorZ != 0 ) ) ? 4 : 5;
				switch ( Random ( ubMaxSpecialWeaponRoll ) )
				{
					case 0:
						//grenade launcher
						fGrenadeLauncher = TRUE;
						bGrenades = 4 + (INT8)(Random( 4 )); //4-7
						break;
					case 1:
					case 2:
						// LAW rocket launcher
						fLAW = TRUE;
						break;
					case 3:
						// RPG rocket launcher
						fRPG = TRUE;
						bGrenades = 3 + (INT8)(Random( 5 )); //3-7
						bGrenadeClass = RPG_GRENADE_CLASS;
						break;
					case 4:
						// one per team maximum!
						if ( guiMortarsRolledByTeam < MAX_MORTARS_PER_TEAM )
						{
							//mortar
							fMortar = TRUE;
							guiMortarsRolledByTeam++;

							// the grenades will actually represent mortar shells in this case
							bGrenades = 3 + (INT8)(Random( 5 )); //3-7
							bGrenadeClass = MORTAR_GRENADE_CLASS;
						}
						else
						{
							// RPG rocket launcher
							fRPG = TRUE;
							bGrenades = 3 + (INT8)(Random( 5 )); //3-7
							bGrenadeClass = RPG_GRENADE_CLASS;
						}
						break;
				}
			}
			break;

#ifdef ENABLE_ZOMBIES
		case SOLDIER_CLASS_ZOMBIE:
			// zombies get no items at all...
			break;
#endif
	}

	UINT32 invsize = pp->Inv.size();
	for( i = 0; i < invsize; ++i )
	{
		//clear items, but only if they have write status.
		if( !(pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE) )
		{
			DeleteObj(&pp->Inv[ i ]);
			//ADB this is ok, delete only inits it
			pp->Inv[ i ].fFlags |= OBJECT_UNDROPPABLE;
		}
		else
		{ //check to see what kind of item is here.  If we find a gun, for example, it'll make the
			//bWeaponClass negative to signify that a gun has already been specified, and later
			//code will use that to determine that and to pick ammo for it.
			pItem = &pp->Inv[ i ];
			if( !pItem )
				continue;
			switch( Item[ pItem->usItem ].usItemClass )
			{
				case IC_GUN:
					if ( !Item[pItem->usItem].rocketlauncher )
					{
						bWeaponClass *= -1;
					}
					else	// rocket launcher!
					{
						fLAW = FALSE;
					}
					(*pItem)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;
					break;
				case IC_AMMO:
					bAmmoClips = 0;
					break;
				case IC_BLADE:
				case IC_THROWING_KNIFE:
					bKnifeClass = 0;
					break;
				case IC_LAUNCHER:
					(*pItem)[0]->data.gun.ubGunState |= GS_CARTRIDGE_IN_CHAMBER;
					fGrenadeLauncher = FALSE;
					if (fMortar || fRPG)
					{
						bGrenadeClass = 0;
						bGrenades = 0;
					}
					fMortar = FALSE;
					fRPG = FALSE;
					break;
				case IC_ARMOUR:
					if( i == HELMETPOS )
						bHelmetClass = 0;
					else if( i == VESTPOS )
						bVestClass = 0;
					else if( i == LEGPOS )
						bLeggingClass = 0;
					break;
				case IC_GRENADE:
					bGrenades = 0;
					bGrenadeClass = 0;
					break;
				case IC_MEDKIT:
				case IC_KIT:
					bKitClass = 0;
					break;
				case IC_MISC:
					bMiscClass = 0;
				case IC_BOMB:
					bBombClass = 0;
					break;
				// Headrock: Added failsafe for LBEs
				case IC_LBEGEAR:
					bLBEClass = 0;
					break;
			}
		}
	}



	// special: militia shouldn't drop bombs
	if ( !( SOLDIER_CLASS_ENEMY( bSoldierClass ) ) )
	{
		bBombClass = 0;
	}


	//Now actually choose the equipment!
	ChooseWeaponForSoldierCreateStruct( pp, bWeaponClass, bAmmoClips, bAttachClass, fAttachment );
	// Headrock: This is where the program calls LBE choosing
	if ( SOLDIER_CLASS_ENEMY( bSoldierClass )) // BUT ONLY FOR ENEMIES, NOT MILITIA - SANDRO
	{
		if (Chance(50))	// Make EDB gear rare
			ChooseLBEsForSoldierCreateStruct( pp, bLBEClass );
	}
	ChooseSpecialWeaponsForSoldierCreateStruct( pp, bKnifeClass, fGrenadeLauncher, fLAW, fMortar, fRPG );
	ChooseGrenadesForSoldierCreateStruct( pp, bGrenades, bGrenadeClass, fGrenadeLauncher );
	ChooseArmourForSoldierCreateStruct( pp, bHelmetClass, bVestClass, bLeggingClass );
	ChooseFaceGearForSoldierCreateStruct( pp );
	if (Chance(65))
	{ ChooseKitsForSoldierCreateStruct( pp, bKitClass ); }	// Kits not so often - SANDRO
	ChooseMiscGearForSoldierCreateStruct( pp, bMiscClass );
	ChooseBombsForSoldierCreateStruct( pp, bBombClass );
	//ADB why is this here twice?
	// Headrock: This is where the program calls LBE choosing
	//ChooseLBEsForSoldierCreateStruct( pp, bLBEClass ); // SANDRO - removed duplicated 
	ChooseLocationSpecificGearForSoldierCreateStruct( pp );
	RandomlyChooseWhichItemsAreDroppable( pp, bSoldierClass );

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("GenerateRandomEquipment done"));
}


//When using the class values, they should all range from 0-11, 0 meaning that there will be no
//selection for that particular type of item, and 1-11 means to choose an item if possible.  1 is
//the worst class of item, while 11 is the best.

void ChooseWeaponForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bWeaponClass,
																				 INT8 bAmmoClips, INT8 bAttachClass, BOOLEAN fAttachment )
{
	//INVTYPE *pItem;
	UINT16 i;
	//UINT16 usRandom;
	UINT16 usGunIndex = 0;
	UINT16 usAmmoIndex = 0;
	UINT16 usAttachIndex = 0;
	UINT16 usAttachIndex2 = 0;
	UINT16 usScopeIndex = 0;
	UINT8 ubChanceStandardAmmo;
	INT8 bStatus;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseWeaponForSoldierCreateStruct");

	// Choose weapon:
	// WEAPONS are very important, and are therefore handled differently using special pre-generated tables.
	// It was requested that enemies use only a small subset of guns with a lot duplication of the same gun type.

	// if gun was pre-selected (rcvd negative weapon class) and needs ammo
	if( bWeaponClass < 0 && bAmmoClips )
	{
		//Linda has added a specific gun to the merc's inventory, but no ammo.  So, we
		//will choose ammunition that works with the gun.
		UINT16 invsize = pp->Inv.size();
		for( i = 0; i < invsize; ++i )
		{
			if( Item[ pp->Inv[ i ].usItem ].usItemClass == IC_GUN && pp->Inv[ i ].exists() == true)
			{
				usGunIndex = pp->Inv[ i ].usItem;
				ubChanceStandardAmmo = 100 - (bWeaponClass * -9);		// weapon class is negative!
				usAmmoIndex = RandomMagazine( usGunIndex, ubChanceStandardAmmo, max(Item[usGunIndex].ubCoolness, HighestPlayerProgressPercentage() / 10 + 3), pp->ubSoldierClass);
		
				if ( usAmmoIndex <= 0 )
					usAmmoIndex = DefaultMagazine(usGunIndex);

				pp->Inv[ i ][0]->data.gun.ubGunAmmoType = Magazine[Item[usAmmoIndex].ubClassIndex].ubAmmoType;
				pp->Inv[ i ][0]->data.gun.usGunAmmoItem = usAmmoIndex;

				if ( Item[usGunIndex].fingerprintid )
				{
					pp->Inv[ i ][0]->data.ubImprintID = (NO_PROFILE + 1);
				}

				break;
			}
		}
		if( bAmmoClips && usAmmoIndex )
		{
			CreateItems( usAmmoIndex, 100, bAmmoClips, &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}

		return;
	}

	if (bWeaponClass < 1)
		return; //empty handed / pre-selected

	// reduce anything over 11 to 11
	if (bWeaponClass > ARMY_GUN_LEVELS)
	{
		bWeaponClass = ARMY_GUN_LEVELS;
	}


	// the weapon class here ranges from 1 to 11, so subtract 1 to get a gun level
	usGunIndex = SelectStandardArmyGun( (UINT8) (bWeaponClass - 1), pp->ubSoldierClass );

	// Flugente: random items
	UINT16 newitemfromrandom = 0;
	if ( GetItemFromRandomItem(usGunIndex, &newitemfromrandom) )
		usGunIndex = newitemfromrandom;

	// if item is still random, use default gun -> GLOCK 17
	if ( Item[usGunIndex].randomitem > 0 )
		usGunIndex = GLOCK_17;

	//WarmSteel - Removed the attachment check here. why check twice anyway?
	//Now, we have chosen all of the correct items.  Now, we will assign them into the slots.
	//Because we are dealing with enemies, automatically give them full ammo in their weapon.
	switch( pp->ubSoldierClass )
	{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_ARMY:
		case SOLDIER_CLASS_GREEN_MILITIA:
		case SOLDIER_CLASS_REG_MILITIA:
			//Admins/Troops: 60-75% + 1% every 4% progress
			bStatus = (INT8)(60 + Random( 16 ));
			bStatus += (INT8)(HighestPlayerProgressPercentage() / 4);
			bStatus = (INT8)min( 100, bStatus );
			break;
		case SOLDIER_CLASS_ELITE:
		case SOLDIER_CLASS_ELITE_MILITIA:
			//85-90% +  1% every 10% progress
			bStatus = (INT8)(85 + Random( 6 ));
			bStatus += (INT8)(HighestPlayerProgressPercentage() / 10);
			bStatus = (INT8)min( 100, bStatus );
			break;

#ifdef ENABLE_ZOMBIES
		case SOLDIER_CLASS_ZOMBIE:
			bStatus = (INT8)(1);	// if a zombie gets anything, it'll be broken
#endif
		default:
			bStatus = (INT8)(50 + Random( 51 ) );
			break;
	}
	// don't allow it to be lower than marksmanship, we don't want it to affect their chances of hitting
	bStatus = (INT8)max( pp->bMarksmanship, bStatus );


	CreateItem( usGunIndex, bStatus, &(pp->Inv[ HANDPOS ]) );
	pp->Inv[ HANDPOS ].fFlags |= OBJECT_UNDROPPABLE;

	// Rocket Rifles must come pre-imprinted, in case carrier gets killed without getting a shot off
	if ( Item[usGunIndex].fingerprintid )
	{
		pp->Inv[ HANDPOS ][0]->data.ubImprintID = (NO_PROFILE + 1);
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseWeaponForSoldierCreateStruct: Gun Created");
	if( !(pp->Inv[ HANDPOS ].fFlags & OBJECT_NO_OVERWRITE) )
	{ //slot not locked, so add attachments to it
		if(UsingNewAttachmentSystem()==false){
			if ( Weapon[usGunIndex].ubWeaponType == GUN_SN_RIFLE )
			{
				usScopeIndex = PickARandomAttachment(SCOPE, pp->ubSoldierClass, usGunIndex,bAttachClass,TRUE);
				if ( usScopeIndex == 0 )
				{
					// find any one that works
					usScopeIndex = PickARandomAttachment(SCOPE, pp->ubSoldierClass, usGunIndex,bAttachClass,FALSE);
				}
			}

			if( ValidItemAttachment(&(pp->Inv[ HANDPOS ]),usScopeIndex,TRUE,FALSE) )
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: Create Scope %d",usScopeIndex));
				CreateItem( usScopeIndex, 100, &gTempObject );
				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
				pp->Inv[ HANDPOS ].AttachObject( NULL, &gTempObject, FALSE );
			} else {
				usScopeIndex = 0;
			}

			//Choose attachment
			if( bAttachClass && ( fAttachment ))
				usAttachIndex = PickARandomAttachment(ATTACHMENTS, pp->ubSoldierClass, usGunIndex,bAttachClass,FALSE);

			if( ValidItemAttachment(&(pp->Inv[ HANDPOS ]),usAttachIndex,TRUE,FALSE))
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: Create Attachment %d",usAttachIndex));
				CreateItem( usAttachIndex, 100, &gTempObject );
				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
				pp->Inv[ HANDPOS ].AttachObject( NULL, &gTempObject, FALSE );
			} else {
				usAttachIndex = 0;
			}

			//check for chance of second attachment
			if ( ( bAttachClass - Item[usAttachIndex].ubCoolness ) > 0 && Random(2) )
				usAttachIndex2 = PickARandomAttachment(ATTACHMENTS, pp->ubSoldierClass, usGunIndex,bAttachClass,FALSE);


			if( ValidItemAttachment(&(pp->Inv[ HANDPOS ]),usAttachIndex2,TRUE,FALSE))
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: Create 2nd Attachment %d",usAttachIndex2));
				CreateItem( usAttachIndex2, 100, &gTempObject );
				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
				pp->Inv[ HANDPOS ].AttachObject( NULL, &gTempObject, FALSE );
			} else {
				usAttachIndex2 = 0;
			}
		} else {
			//The total pool of coolness we have for all the attachments. If fAttachment is true, the chance of getting an attachment is boosted.
			INT8 iMiscAttachmentChance = 40 + (fAttachment * 20);
			//Add a value equal to the avarage amount of attachments that will be on this gun.
			//Because the guns scope does not subtract from this value, -1.
			INT16 iAttachmentCoolnessPool = (bAttachClass * (gGameExternalOptions.iMaxEnemyAttachments-1) * iMiscAttachmentChance) / 100;

			BOOLEAN fFoundValidAttachment = FALSE;
			BOOLEAN fRandomPassed = FALSE;
			BOOLEAN fGetMatchingCoolness = FALSE;
			UINT16	usTryCount = 0;

			//When using NAS, we want more attachments.
			if ( Weapon[usGunIndex].ubWeaponType == GUN_SN_RIFLE )
			{
				usScopeIndex = PickARandomAttachment(SCOPE, pp->ubSoldierClass, usGunIndex,bAttachClass,TRUE);
				if ( usScopeIndex == 0 )
				{
					// find any one that works
					usScopeIndex = PickARandomAttachment(SCOPE, pp->ubSoldierClass, usGunIndex,bAttachClass,FALSE);
				}
			//Guns should have a fairly good chance of having a scope. Even when they're not sniper rifles.
			//They're likely to be crappier, though.
			} else if (Chance(75) && fAttachment){
				usScopeIndex = PickARandomAttachment(SCOPE, pp->ubSoldierClass, usGunIndex,bAttachClass-1,FALSE);
			}

			if( ValidItemAttachmentSlot(&(pp->Inv[ HANDPOS ]),usScopeIndex, TRUE,FALSE) )
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: Create Scope %d",usScopeIndex));
				CreateItem( usScopeIndex, 100, &gTempObject );
				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
				pp->Inv[ HANDPOS ].AttachObject( NULL, &gTempObject, FALSE );
			}

			//Loop for the maximum amount of attachments (-1 because we can already have a scope by here)
			//cnt = -1 because we start with ++ing it
			for(INT16 cnt = -1; cnt < (gGameExternalOptions.iMaxEnemyAttachments-1); ){
				usTryCount++;
				usAttachIndex = 0;

				//Our coolness pool was depleted, we won't find any more attachments anyway.
				if(iAttachmentCoolnessPool <= 0)
					break;

				//Only move on to the next attachment if 1: we found a good one. 2: randomness decided this attachment won't be. 3: We've exceeded the maximum number of tries.
				if(fFoundValidAttachment || !fRandomPassed || usTryCount > 20){
					cnt++;
					usTryCount = 0;
					fFoundValidAttachment = FALSE;

					//Determine if this attachments deserves to be.
					fRandomPassed = Chance(iMiscAttachmentChance);
					//Sometimes allow lower grade attachments.
					fGetMatchingCoolness = Chance(70);
				}

				if (fRandomPassed)
					usAttachIndex = PickARandomAttachment(ATTACHMENTS, pp->ubSoldierClass, usGunIndex,bAttachClass,fGetMatchingCoolness);

				if(!usAttachIndex){
					continue;
				}

				if( ValidItemAttachmentSlot(&(pp->Inv[ HANDPOS ]),usAttachIndex, TRUE,FALSE))
				{
					fFoundValidAttachment = TRUE;
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: Created Attachment %d, num: %d",usAttachIndex, cnt));
					CreateItem( usAttachIndex, 100, &gTempObject );
					gTempObject.fFlags |= OBJECT_UNDROPPABLE;
					pp->Inv[ HANDPOS ].AttachObject( NULL, &gTempObject, FALSE );
					iAttachmentCoolnessPool -= Item[usAttachIndex].ubCoolness;
				}
			}
		}
	}
	

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: Set bullets"));
	//set bullets = to magsize including any attachments (c-mag adapters, etc)
	pp->Inv[ HANDPOS ][0]->data.gun.ubGunShotsLeft = GetMagSize(&pp->Inv[ HANDPOS ]);

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: choose ammo"));
	if( bAmmoClips )
	{ //We have a gun, so choose ammo clips

		// check default ammo first...
		usAmmoIndex = DefaultMagazine( usGunIndex );
		if ( AmmoTypes[Magazine[ Item[ usAmmoIndex ].ubClassIndex ].ubAmmoType].standardIssue )
			ubChanceStandardAmmo = 80;
		else
			ubChanceStandardAmmo = 100 - (bWeaponClass * 9);

		//switch( Magazine[ Item[ usAmmoIndex ].ubClassIndex ].ubAmmoType )
		//{
		//	case AMMO_AP:
		//	case AMMO_SUPER_AP:
		//		// assault rifle, rocket rifle (etc) - high chance of having AP ammo
		//		ubChanceStandardAmmo = 80;
		//		break;
		//	default:
		//		ubChanceStandardAmmo = 100 - (bWeaponClass * 9);
		//		break;
		//}

		usAmmoIndex = RandomMagazine( &pp->Inv[HANDPOS], ubChanceStandardAmmo, max(Item[usGunIndex].ubCoolness, HighestPlayerProgressPercentage() / 10 + 3 ), pp->ubSoldierClass);

		if ( usAmmoIndex <= 0 )
			usAmmoIndex = DefaultMagazine(usGunIndex);

		pp->Inv[ HANDPOS ][0]->data.gun.ubGunAmmoType = Magazine[Item[usAmmoIndex].ubClassIndex].ubAmmoType;
		pp->Inv[ HANDPOS ][0]->data.gun.usGunAmmoItem = usAmmoIndex;
	}

	//Ammo
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: create ammo"));
	if( bAmmoClips && usAmmoIndex )
	{
		//Madd: ensure a minimum # of bullets to make sure enemies don't run out and run away
		while ( ( (bAmmoClips - 1) * Weapon[usGunIndex].ubMagSize ) < 20 ) // each soldier should have at least 20 bullets, ie: 2 9mm 15rd clips, 3 7rd shotgun shells, 4 6rd speedloaders, etc.
		{
			bAmmoClips++;
		}

		CreateItems( usAmmoIndex, 100, bAmmoClips, &gTempObject );
		gTempObject.fFlags |= OBJECT_UNDROPPABLE;
		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("ChooseWeaponForSoldierCreateStruct: done"));
}

void ChooseGrenadesForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bGrenades, INT8 bGrenadeClass, BOOLEAN fGrenadeLauncher )
{
	INT16 sNumPoints;
	UINT16 usItem;
	UINT8 ubBaseQuality;
	UINT8 ubQualityVariation;
	//numbers of each type the player will get!
	UINT8 count = 0;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseGrenadesForSoldierCreateStruct");

	//determine how many *points* the enemy will get to spend on grenades...
	sNumPoints = bGrenades * bGrenadeClass;

	//no points, no grenades!
	if( !sNumPoints )
		return;

	// special mortar shell handling
	if (bGrenadeClass == MORTAR_GRENADE_CLASS)
	{
		// 0verhaul:  itemMortar can be 0 if the previous function
		// 1) failed to find a Mortar that the soldier would want to use (due to XML prefs)
		// 2) randomly chose not to supply a Mortar (always possible, even if choices exist).
		// Since we should not go beyond this point in the case where Mortar Rounds are desired,
		// return here in any case
		if (itemMortar > 0 )
		{
			usItem = PickARandomLaunchable ( itemMortar );
			if ( usItem > 0 )
			{
				CreateItems( usItem, (INT8) (80 + Random(21)), bGrenades, &gTempObject );
				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
				PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
			}
		}

		return;
	}

	// special rpg rocket handling
	if (bGrenadeClass == RPG_GRENADE_CLASS)
	{
		// 0verhaul:  itemRPG can be 0 if the previous function
		// 1) failed to find an RPG that the soldier would want to use (due to XML prefs)
		// 2) randomly chose not to supply an RPG (always possible, even if choices exist).
		// Since we should not go beyond this point in the case where RPG grenades are desired,
		// return here in any case
		if (itemRPG > 0 )
		{
			usItem = PickARandomLaunchable ( itemRPG );
			if ( usItem > 0 )
			{
				for ( int i = 0; i < bGrenades; i++ )
				{
					CreateItem( usItem, (INT8) (70 + Random(31)), &gTempObject ); 
					gTempObject.fFlags |= OBJECT_UNDROPPABLE;
					PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
				}

			}
		}
		return;
	}

	Assert( bGrenadeClass <= 11 );

	//determine the quality of grenades.  The elite guys get the best quality, while the others
	//get progressively worse.
	ubBaseQuality = (UINT8)min( 45 + bGrenadeClass * 5, 90 );
	ubQualityVariation = 101 - ubBaseQuality;

	// Madd: GL guys don't get hand grenades anymore
	if ( itemGrenadeLauncher > 0 )
	{
		//do this for every 1-2 grenades so that we can get more variety
		while ( bGrenades > 0 )
		{
			count = Random(3);
			if ( count > bGrenades )
				count = bGrenades;

			usItem = PickARandomLaunchable ( itemGrenadeLauncher );
			if ( usItem > 0 && count > 0 )
			{
				CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )), count, &gTempObject );
				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
				PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
			}
			bGrenades -= count;
		}

		return;
	}


	//Madd: screw the original code; it's impossible to externalize and too complicated anyway
	//do this for every 1-2 grenades so that we can get more variety
	while ( bGrenades > 0 )
	{
		count = Random(3);
		if ( count > bGrenades )
			count = bGrenades;

		usItem = PickARandomItem ( GRENADE, pp->ubSoldierClass, bGrenadeClass, FALSE );
		if ( usItem > 0 && count > 0 )
		{
			CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )), count, &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}
		bGrenades -= count;
	}
	return;

#ifdef obsoleteCode
	////now, purchase the grenades.
	//while( sNumPoints > 0 )
	//{
	//	if( sNumPoints >= 20 )
	//	{ //Choose randomly between mustard and regular
	//		if( Random( 2 ) && !fGrenadeLauncher )
	//			ubNumMustard++, sNumPoints -= 10;
	//		else
	//			ubNumReg++, sNumPoints -= 9;
	//	}

	//	if( sNumPoints >= 10 )
	//	{ //Choose randomly between any
	//		switch( Random( 7 ) )
	//		{
	//			case 0:	if ( !fGrenadeLauncher )
	//							{
	//								ubNumMustard++;		sNumPoints -= 10;	break;
	//							}
	//							// if grenade launcher, pick regular instead
	//			case 1: ubNumReg++;				sNumPoints -= 9;		break;
	//			case 2: if ( !fGrenadeLauncher )
	//							{
	//								ubNumMini++;			sNumPoints -= 7;		break;
	//							}
	//							// if grenade launcher, pick tear instead
	//			case 3: ubNumTear++;			sNumPoints -= 6;		break;
	//			case 4: ubNumStun++;			sNumPoints -= 5;		break;
	//			case 5: ubNumSmoke++;			sNumPoints -= 4;		break;
	//			case 6: if (!fGrenadeLauncher )
	//					{
	//						ubNumFlare++;			sNumPoints -= 3;
	//					}
	//					break;
	//		}
	//	}

	//	// JA2 Gold: don't make mini-grenades take all points available, and add chance of break lights
	//	if( sNumPoints >= 1 && sNumPoints < 10 )
	//	{
	//		switch( Random( 10 ) )
	//		{
	//			case 0:
	//			case 1:
	//			case 2:
	//				ubNumSmoke++;
	//				sNumPoints -= 4;
	//				break;
	//			case 3:
	//			case 4:
	//				ubNumTear++;
	//				sNumPoints -= 6;
	//				break;
	//			case 5:
	//			case 6:
	//				if (!fGrenadeLauncher)
	//				{
	//					ubNumFlare++;
	//					sNumPoints -= 3;
	//				}
	//				break;
	//			case 7:
	//			case 8:
	//				ubNumStun++;
	//				sNumPoints -= 5;
	//				break;
	//			case 9:
	//				if (!fGrenadeLauncher)
	//				{
	//					ubNumMini++;
	//					sNumPoints -= 7;
	//				}
	//				break;
	//		}
	//	}
	//	/*
	//	if( usNumPoints >= 1 && usNumPoints < 10 )
	//	{ //choose randomly between either stun or tear, (normal with rare chance)
	//		switch( Random( 10 ) )
	//		{
	//			case 0:
	//			case 1:
	//			case 2:
	//			case 3:
	//				ubNumSmoke++;
	//				if( usNumPoints > 4 )
	//					usNumPoints -= 4;
	//				else
	//					usNumPoints = 0;
	//				break;
	//			case 4:
	//			case 5:
	//			case 6:
	//				ubNumTear++;
	//				if( usNumPoints > 6 )
	//					usNumPoints -= 6;
	//				else
	//					usNumPoints = 0;
	//				break;
	//			case 7:
	//			case 8:
	//				ubNumStun++;
	//				if( usNumPoints > 5 )
	//					usNumPoints -= 5;
	//				else
	//					usNumPoints = 0;
	//				break;
	//			case 9:
	//				ubNumMini++;
	//				usNumPoints = 0;
	//				break;
	//		}
	//	}
	//	*/
	//}


	////Create the grenades and add them to the soldier

	//if( ubNumSmoke )
	//{
	//	if ( fGrenadeLauncher )
	//	{
	//		usItem = GL_SMOKE_GRENADE;
	//	}
	//	else
	//	{
	//		usItem = SMOKE_GRENADE;
	//	}
	//	CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumSmoke, &gTempObject );
	//	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//}
	//if( ubNumTear )
	//{
	//	if ( fGrenadeLauncher )
	//	{
	//		usItem = GL_TEARGAS_GRENADE;
	//	}
	//	else
	//	{
	//		usItem = TEARGAS_GRENADE;
	//	}
	//	CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumTear, &gTempObject );
	//	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//}
	//if( ubNumStun )
	//{
	//	if ( fGrenadeLauncher )
	//	{
	//		usItem = GL_STUN_GRENADE;
	//	}
	//	else
	//	{
	//		usItem = STUN_GRENADE;
	//	}
	//	CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumStun, &gTempObject );
	//	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//}
	//if( ubNumReg )
	//{
	//	if ( fGrenadeLauncher )
	//	{
	//		usItem = GL_HE_GRENADE;
	//	}
	//	else
	//	{
	//		usItem = HAND_GRENADE;
	//	}
	//	CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumReg, &gTempObject );
	//	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//}

	//if( ubNumMini )
	//{
	//	CreateItems( MINI_GRENADE, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumMini, &gTempObject );
	//	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//}
	//if( ubNumMustard )
	//{
	//	CreateItems( MUSTARD_GRENADE, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumMustard, &gTempObject );
	//	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//}
	//if( ubNumFlare )
	//{
	//	CreateItems( BREAK_LIGHT, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumFlare, &gTempObject );
	//	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//}
#endif //obsoleteCode
}

void ChooseArmourForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bHelmetClass, INT8 bVestClass, INT8 bLeggingsClass )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseArmourForSoldierCreateStruct");
	//UINT16 i;
	//INVTYPE *pItem;
	//UINT16 usRandom;
	UINT16 usItem = 0, usHelmetItem = 0, usVestItem = 0, usLeggingsItem = 0;
	//UINT16 usNumMatches;
	//INT8 bOrigVestClass = bVestClass;
	INT8 i;

	//tais: always get any armor... smeagol doesnt like naked people..
	if(gGameExternalOptions.fSoldiersWearAnyArmour)
	{
		if(bHelmetClass < 1) bHelmetClass = 1;
		//search for a non-empty class with items we need
		for(i=bHelmetClass;i<=10;i++)
		{
			usHelmetItem = PickARandomItem(HELMET, pp->ubSoldierClass, i );
			//if we find a non-empty class change to that and break
			if(usHelmetItem > 0)
			{
				bHelmetClass = i;
				break;
			}
		}
		if(bVestClass < 1) bVestClass = 1;
		//search for a non-empty class with items we need
		for(i=bVestClass;i<=10;i++)
		{
			usVestItem = PickARandomItem(VEST, pp->ubSoldierClass, i );
			//if we find a non-empty class change to that and break
			if(usVestItem > 0)
			{
				bVestClass = i;
				break;
			}
		}
		if(bLeggingsClass < 1) bLeggingsClass = 1;
		//search for a non-empty class with items we need
		for(i=bLeggingsClass;i<=10;i++)
		{
			usLeggingsItem = PickARandomItem(LEGS, pp->ubSoldierClass, i );
			//if we find a non-empty class change to that and break
			if(usLeggingsItem > 0)
			{
				bLeggingsClass = i;
				break;
			}
		}
	}

	//Madd: added minimum protection of 10 for armours to be used by enemies

	//Choose helmet
	if( bHelmetClass )
	{
		if(!gGameExternalOptions.fSoldiersWearAnyArmour) usHelmetItem = PickARandomItem(HELMET, pp->ubSoldierClass, bHelmetClass );
		if ( usHelmetItem > 0 && Item[usHelmetItem].usItemClass == IC_ARMOUR && !(pp->Inv[ HELMETPOS ].fFlags & OBJECT_NO_OVERWRITE) && Armour[ Item[usHelmetItem].ubClassIndex ].ubArmourClass == ARMOURCLASS_HELMET )
		{
			CreateItem( usHelmetItem, (INT8)(70+Random(31)), &(pp->Inv[ HELMETPOS ]) );
			pp->Inv[ HELMETPOS ].fFlags |= OBJECT_UNDROPPABLE;

			// roll to see if he gets an attachment, too.  Higher chance the higher his entitled helmet class is
			if (( INT8 ) Random( 100 ) < ( 15 * ( bHelmetClass - Item[usHelmetItem].ubCoolness ) ) )
			{
				UINT16 usAttachment = PickARandomAttachment(ARMOURATTACHMENT, pp->ubSoldierClass, usHelmetItem, bHelmetClass, FALSE);
				if ( usAttachment > 0 )
				{
					CreateItem( usAttachment, (INT8)(70+Random(31)), &gTempObject );
					gTempObject.fFlags |= OBJECT_UNDROPPABLE;
					pp->Inv[ HELMETPOS ].AttachObject( NULL, &gTempObject, FALSE );
				}
			}

		}

#ifdef obsoleteCode
		//usNumMatches = 0;
		//while( bHelmetClass && !usNumMatches )
		//{ //First step is to count the number of helmets in the helmet class range.  If we
		//	//don't find one, we keep lowering the class until we do.
		//	for( i = 0; i < MAXITEMS; i++ )
		//	{
		//		if ( Item[i].usItemClass  == 0 )
		//			break;
		//		pItem = &Item[ i ];
		//		// NOTE: This relies on treated armor to have a coolness of 0 in order for enemies not to be equipped with it
		//		if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness == bHelmetClass )
		//		{
		//			if( Armour[ pItem->ubClassIndex ].ubArmourClass == ARMOURCLASS_HELMET && Armour[ pItem->ubClassIndex ].ubProtection >= 10 )
		//				usNumMatches++;
		//		}
		//	}
		//	if( !usNumMatches )
		//		bHelmetClass--;
		//}
		//if( usNumMatches )
		//{ //There is a helmet that we can choose.
		//	usRandom = (UINT16)Random( usNumMatches );
		//	for( i = 0; i < MAXITEMS; i++ )
		//	{
		//		if ( Item[i].usItemClass  == 0 )
		//			break;
		//		pItem = &Item[ i ];
		//		if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness == bHelmetClass )
		//		{
		//			if( Armour[ pItem->ubClassIndex ].ubArmourClass == ARMOURCLASS_HELMET && Armour[ pItem->ubClassIndex ].ubProtection >= 10 )
		//			{
		//				if( usRandom )
		//					usRandom--;
		//				else
		//				{
		//					if( !(pp->Inv[ HELMETPOS ].fFlags & OBJECT_NO_OVERWRITE) )
		//					{
		//						CreateItem( i, (INT8)(70+Random(31)), &(pp->Inv[ HELMETPOS ]) );
		//						pp->Inv[ HELMETPOS ].fFlags |= OBJECT_UNDROPPABLE;
		//					}
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}
#endif //obsoleteCode
	}

	//Choose vest
	if( bVestClass )
	{
		if(!gGameExternalOptions.fSoldiersWearAnyArmour) usVestItem = PickARandomItem(VEST, pp->ubSoldierClass, bVestClass );
		if ( usVestItem > 0 && Item[usVestItem].usItemClass == IC_ARMOUR && !(pp->Inv[ VESTPOS ].fFlags & OBJECT_NO_OVERWRITE) && Armour[ Item[usVestItem].ubClassIndex ].ubArmourClass == ARMOURCLASS_VEST )
		{
			CreateItem( usVestItem, (INT8)(70+Random(31)), &(pp->Inv[ VESTPOS ]) );
			pp->Inv[ VESTPOS ].fFlags |= OBJECT_UNDROPPABLE;

			// roll to see if he gets a CERAMIC PLATES, too.  Higher chance the higher his entitled vest class is
			if (( INT8 ) Random( 100 ) < ( 15 * ( bVestClass - Item[usVestItem].ubCoolness ) ) )
			{
				UINT16 usAttachment = PickARandomAttachment(ARMOURATTACHMENT, pp->ubSoldierClass, usVestItem, bVestClass, FALSE);
				if ( usAttachment > 0 )
				{
					CreateItem( usAttachment, (INT8)(70+Random(31)), &gTempObject );
					gTempObject.fFlags |= OBJECT_UNDROPPABLE;
					pp->Inv[ VESTPOS ].AttachObject( NULL, &gTempObject, FALSE );
				}
			}

		}

#ifdef obsoleteCode
		//usNumMatches = 0;
		//while( bVestClass && !usNumMatches )
		//{ //First step is to count the number of armours in the armour class range.  If we
		//	//don't find one, we keep lowering the class until we do.
		//	for( i = 0; i < MAXITEMS; i++ )
		//	{
		//		if ( Item[i].usItemClass  == 0 )
		//			break;
		//		// these 3 have a non-zero coolness, and would otherwise be selected, so skip them
		//		if ( Item[i].leatherjacket)
		//			continue;

		//		pItem = &Item[ i ];
		//		// Madd: don't pick crappy armour like t-shirts, must be a minimum protection of ten
		//		// NOTE: This relies on treated armor to have a coolness of 0 in order for enemies not to be equipped with it
		//		if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness == bVestClass )
		//		{
		//			if( Armour[ pItem->ubClassIndex ].ubArmourClass == ARMOURCLASS_VEST && Armour[pItem->ubClassIndex].ubProtection >= 10 )
		//				usNumMatches++;
		//		}
		//	}
		//	if( !usNumMatches )
		//		bVestClass--;
		//}
		//if( usNumMatches )
		//{ //There is an armour that we can choose.
		//	usRandom = (UINT16)Random( usNumMatches );
		//	for( i = 0; i < MAXITEMS; i++ )
		//	{
		//		if ( Item[i].usItemClass  == 0 )
		//			break;

		//		pItem = &Item[ i ];
		//		if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness == bVestClass )
		//		{
		//			if ( Item[i].leatherjacket )
		//				continue;

		//			// Madd: don't pick crappy armour like t-shirts, must be a minimum protection of ten
		//			if( Armour[ pItem->ubClassIndex ].ubArmourClass == ARMOURCLASS_VEST && Armour[ pItem->ubClassIndex ].ubProtection >= 10 )
		//			{
		//				if( usRandom )
		//					usRandom--;
		//				else
		//				{
		//					if( !(pp->Inv[ VESTPOS ].fFlags & OBJECT_NO_OVERWRITE) )
		//					{
		//						CreateItem( i, (INT8)(70+Random(31)), &(pp->Inv[ VESTPOS ]) );
		//						pp->Inv[ VESTPOS ].fFlags |= OBJECT_UNDROPPABLE;

		//						// roll to see if he gets a CERAMIC PLATES, too.  Higher chance the higher his entitled vest class is
		//						if (( INT8 ) Random( 100 ) < ( 15 * ( bOrigVestClass - pItem->ubCoolness ) ) )
		//						{
		//							UINT16 usAttachment = PickARandomItem(ARMOURATTACHMENT, bOrigVestClass);
		//							if ( usAttachment > 0 && ValidAttachment(usAttachment, i ) )
		//							{
		//								CreateItem( usAttachment, (INT8)(70+Random(31)), &gTempObject );
		//								gTempObject.fFlags |= OBJECT_UNDROPPABLE;
		//								AttachObject( NULL, &(pp->Inv[ VESTPOS ]), &gTempObject );
		//							}
		//						}
		//					}
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}
#endif //obsoleteCode
	}
	//Choose Leggings
	if( bLeggingsClass )
	{
		if(!gGameExternalOptions.fSoldiersWearAnyArmour) usLeggingsItem = PickARandomItem(LEGS, pp->ubSoldierClass, bLeggingsClass );
		if ( usLeggingsItem > 0 && Item[usLeggingsItem].usItemClass == IC_ARMOUR && !(pp->Inv[ LEGPOS ].fFlags & OBJECT_NO_OVERWRITE) && Armour[ Item[usLeggingsItem].ubClassIndex ].ubArmourClass == ARMOURCLASS_LEGGINGS )
		{
			CreateItem( usLeggingsItem, (INT8)(70+Random(31)), &(pp->Inv[ LEGPOS ]) );
			pp->Inv[ LEGPOS ].fFlags |= OBJECT_UNDROPPABLE;

			// roll to see if he gets an attachment, too.  Higher chance the higher his entitled Leggings class is
			if (( INT8 ) Random( 100 ) < ( 15 * ( bLeggingsClass - Item[usLeggingsItem].ubCoolness ) ) )
			{
				UINT16 usAttachment = PickARandomAttachment(ARMOURATTACHMENT, pp->ubSoldierClass, usLeggingsItem, bLeggingsClass, FALSE);
				if ( usAttachment > 0 )
				{
					CreateItem( usAttachment, (INT8)(70+Random(31)), &gTempObject );
					gTempObject.fFlags |= OBJECT_UNDROPPABLE;
					pp->Inv[ LEGPOS ].AttachObject( NULL, &gTempObject, FALSE);
				}
			}

		}


#ifdef obsoleteCode
		//usNumMatches = 0;
		//while( bLeggingsClass && !usNumMatches )
		//{ //First step is to count the number of Armours in the Armour class range.  If we
		//	//don't find one, we keep lowering the class until we do.
		//	for( i = 0; i < MAXITEMS; i++ )
		//	{
		//		if ( Item[i].usItemClass  == 0 )
		//			break;
		//		pItem = &Item[ i ];
		//		// NOTE: This relies on treated armor to have a coolness of 0 in order for enemies not to be equipped with it
		//		if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness == bLeggingsClass )
		//		{
		//			if( Armour[ pItem->ubClassIndex ].ubArmourClass == ARMOURCLASS_LEGGINGS && Armour[ pItem->ubClassIndex ].ubProtection >= 10 )
		//				usNumMatches++;
		//		}
		//	}
		//	if( !usNumMatches )
		//		bLeggingsClass--;
		//}
		//if( usNumMatches )
		//{ //There is a legging that we can choose.
		//	usRandom = (UINT16)Random( usNumMatches );
		//	for( i = 0; i < MAXITEMS; i++ )
		//	{
		//		if ( Item[i].usItemClass  == 0 )
		//			break;
		//		pItem = &Item[ i ];
		//		if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness == bLeggingsClass )
		//		{
		//			if( Armour[ pItem->ubClassIndex ].ubArmourClass == ARMOURCLASS_LEGGINGS && Armour[ pItem->ubClassIndex ].ubProtection >= 10 )
		//			{
		//				if( usRandom )
		//					usRandom--;
		//				else
		//				{
		//					if( !(pp->Inv[ LEGPOS ].fFlags & OBJECT_NO_OVERWRITE) )
		//					{
		//						CreateItem( i, (INT8)(70+Random(31)), &(pp->Inv[ LEGPOS ]) );
		//						pp->Inv[ LEGPOS ].fFlags |= OBJECT_UNDROPPABLE;
		//						break;
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
#endif //obsoleteCode
	}
}

void ChooseSpecialWeaponsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bKnifeClass, BOOLEAN fGrenadeLauncher, BOOLEAN fLAW, BOOLEAN fMortar, BOOLEAN fRPG )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseSpecialWeaponsForSoldierCreateStruct");
	//UINT16 i;
	//INVTYPE *pItem;
	//UINT16 usRandom;
	UINT16 usKnifeIndex = 0;

	itemGrenadeLauncher = 0;
	itemLAW = 0;
	itemRPG = 0;
	itemMortar=0;

#ifdef obsoleteCode
	//Choose knife
	//while( bKnifeClass && !usNumMatches )
	//{ //First step is to count the number of weapons in the weapon class range.  If we
	//	//don't find one, we keep lowering the class until we do.
	//	for( i = 0; i < MAXITEMS; i++ )
	//	{
	//		if ( Item[i].usItemClass  == 0 )
	//			break;
	//		pItem = &Item[ i ];
	//		if( ( pItem->usItemClass == IC_BLADE || pItem->usItemClass == IC_THROWING_KNIFE ) && pItem->ubCoolness == bKnifeClass )
	//		{
	//			usNumMatches++;
	//		}
	//	}
	//	if( !usNumMatches )
	//		bKnifeClass--;
	//}
	//if( usNumMatches )
	//{ //There is a knife that we can choose.
	//	usRandom = (UINT16)Random( usNumMatches );
	//	for( i = 0; i < MAXITEMS; i++ )
	//	{
	//		if ( Item[i].usItemClass  == 0 )
	//			break;
	//		pItem = &Item[ i ];
	//		if( ( pItem->usItemClass == IC_BLADE || pItem->usItemClass == IC_THROWING_KNIFE ) && pItem->ubCoolness == bKnifeClass )
	//		{
	//			if( usRandom )
	//				usRandom--;
	//			else
	//			{
	//				usKnifeIndex = i;
	//				break;
	//			}
	//		}
	//	}
	//}
#endif //obsoleteCode

	if ( bKnifeClass )
	{
		usKnifeIndex = PickARandomItem(KNIVES, pp->ubSoldierClass, bKnifeClass);

		if( usKnifeIndex > 0 )
		{
			CreateItem( usKnifeIndex, (INT8)(70 + Random( 31 )), &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}
	}


////test code
//		CreateItem( 347, (INT8)(50 + Random( 51 )), &gTempObject );
//		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
//
//		CreateItem( 298, (INT8)(50 + Random( 51 )), &gTempObject );
//		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
//
//		CreateItem( 298, (INT8)(50 + Random( 51 )), &gTempObject );
//		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );

	if ( IsGrenadeLauncherAttached(&pp->Inv[HANDPOS]) ) //Madd: if there's a grenade launcher attached to the gun, then use that instead of giving him another one
	{
		itemGrenadeLauncher = GetAttachedGrenadeLauncher(&pp->Inv[HANDPOS]);
	}
	else if (fGrenadeLauncher)
	{
		itemGrenadeLauncher = PickARandomItem ( GRENADELAUNCHER, pp->ubSoldierClass );
		// give grenade launcher
		if ( itemGrenadeLauncher > 0 )
		{
			CreateItem( itemGrenadeLauncher, (INT8)(50 + Random( 51 )), &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}
	}

	if (fLAW)
	{
		// give rocket launcher
		itemLAW = PickARandomItem (SINGLESHOTROCKETLAUNCHER, pp->ubSoldierClass );
		if ( itemLAW > 0 )
		{
			CreateItem( itemLAW, (INT8)(50 + Random( 51 )), &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}
	}
	if (fRPG)
	{
		// give rpg
		itemRPG = PickARandomItem (ROCKETLAUNCHER, pp->ubSoldierClass);
		if ( itemRPG > 0 )
		{
			CreateItem( itemRPG, (INT8)(50 + Random( 51 )), &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}
	}

	if (fMortar)
	{
		// make sure we're not distributing them underground!
		Assert( IsAutoResolveActive() || ( gbWorldSectorZ == 0 ) );

		// give mortar
		itemMortar = PickARandomItem (MORTARLAUNCHER, pp->ubSoldierClass);
		if ( itemMortar > 0 )
		{
			CreateItem( itemMortar, (INT8)(50 + Random( 51 )), &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}
	}


}


void ChooseFaceGearForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseFaceGearForSoldierCreateStruct");
	INT32 i;
	INT8 bDifficultyRating = CalcDifficultyModifier( pp->ubSoldierClass );
	UINT16 usItem=0;

	if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y && StrategicMap[ TIXA_SECTOR_X + TIXA_SECTOR_Y * MAP_WORLD_X ].fEnemyControlled )
	{ //Tixa is a special case that is handled differently.
		return;
	}

	//Look for any face item in the big pocket positions (the only place they can be added in the editor)
	//If any are found, then don't assign any
	// CHRISL: Change static inventory pocket definition to dynamic
	for( i = BIGPOCKSTART; i < BIGPOCKFINAL; i++ )
	{
		if( Item[ pp->Inv[ i ].usItem ].usItemClass == IC_FACE )
		{
			return;
		}
	}

	//KM: (NEW)
	//Note the lack of overwritable item checking here.  This is because faceitems are not
	//supported in the editor, hence they can't have this status.
	switch( pp->ubSoldierClass )
	{
		case SOLDIER_CLASS_ELITE:
		case SOLDIER_CLASS_ELITE_MILITIA:
			if ( Chance( bDifficultyRating ) )
			{
				//All elites get a gasmask and either night goggles or uv goggles.
				if( Chance( 75 ) )
				{
					usItem = PickARandomItem(GASMASKS, pp->ubSoldierClass);
					if ( usItem > 0 )
					{
						CreateItem( usItem, (INT8)(70+Random(31)), &(pp->Inv[ HEAD1POS ]) );
						pp->Inv[ HEAD1POS ].fFlags |= OBJECT_UNDROPPABLE;
					}
				}
				else
				{
					usItem = PickARandomItem(HEARINGAIDS, pp->ubSoldierClass);
					if ( usItem > 0 )
					{
						CreateItem( usItem, (INT8)(70+Random(31)), &(pp->Inv[ HEAD1POS ]) );
						pp->Inv[ HEAD1POS ].fFlags |= OBJECT_UNDROPPABLE;
					}
				}
			}
			if ( Chance( bDifficultyRating / 3 ) )
			{
				if( Chance( 75 ) )
				{
					usItem = PickARandomItem(NVGLOW, pp->ubSoldierClass);
					if ( usItem > 0 )
					{
						CreateItem( usItem, (INT8)(70+Random(31)), &(pp->Inv[ HEAD2POS ]) );
						pp->Inv[ HEAD2POS ].fFlags |= OBJECT_UNDROPPABLE;
					}
				}
				else
				{
					usItem = PickARandomItem(NVGHIGH, pp->ubSoldierClass);
					if ( usItem > 0 )
					{
						CreateItem( usItem, (INT8)(70+Random(31)), &(pp->Inv[ HEAD2POS ]) );
						pp->Inv[ HEAD2POS ].fFlags |= OBJECT_UNDROPPABLE;
					}
				}
			}
			break;
		case SOLDIER_CLASS_ARMY:
		case SOLDIER_CLASS_REG_MILITIA:
			if( Chance( bDifficultyRating / 2 ) )
			{ //chance of getting a face item
				if( Chance( 50 ) )
				{
					usItem = PickARandomItem(GASMASKS, pp->ubSoldierClass);
					if ( usItem > 0 )
					{
						CreateItem( usItem, (INT8)(70+Random(31)), &(pp->Inv[ HEAD1POS ]) );
						pp->Inv[ HEAD1POS ].fFlags |= OBJECT_UNDROPPABLE;
					}
				}
				else
				{
					usItem = PickARandomItem(NVGLOW, pp->ubSoldierClass);
					if ( usItem > 0 )
					{
						CreateItem( usItem, (INT8)(70+Random(31)), &(pp->Inv[ HEAD1POS ]) );
						pp->Inv[ HEAD1POS ].fFlags |= OBJECT_UNDROPPABLE;
					}
				}
			}
			if( Chance( bDifficultyRating / 3 ) )
			{ //chance of getting a extended ear
				usItem = PickARandomItem(HEARINGAIDS, pp->ubSoldierClass);
				if ( usItem > 0 )
				{
					CreateItem( usItem, (INT8)(70+Random(31)), &(pp->Inv[ HEAD2POS ]) );
					pp->Inv[ HEAD2POS ].fFlags |= OBJECT_UNDROPPABLE;
				}
			}
			break;
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_GREEN_MILITIA:
#ifdef ENABLE_ZOMBIES
		case SOLDIER_CLASS_ZOMBIE:
#endif
			break;

	}
}



void ChooseKitsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bKitClass )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseKitsForSoldierCreateStruct");
	//UINT16 i;
	//INVTYPE *pItem;
	//UINT16 usRandom;
	//UINT16 usNumMatches = 0;
	UINT16 usKitItem = 0;

	//Madd: specify kit items in xml instead

#ifdef obsoleteCode
	// we want these mostly to be first aid and medical kits, and for those kit class doesn't matter, they're always useful
	//if ( Chance( 50 ) )
	//{
	//	usKitItem = FIRSTAIDKIT;
	//}
	//else
	//if ( Chance( 25 ) )
	//{
	//	usKitItem = MEDICKIT;
	//}
	//else
	//{
	//	// count how many non-medical KITS are eligible ( of sufficient or lower coolness)
	//	for( i = 0; i < MAXITEMS; i++ )
	//	{
	//		if ( Item[i].usItemClass  == 0 )
	//			break;
	//		pItem = &Item[ i ];
	//		// skip toolkits
	//		if( ( pItem->usItemClass == IC_KIT ) && ( pItem->ubCoolness > 0 ) && pItem->ubCoolness <= bKitClass && ( !Item[i].toolkit ) )
	//		{
	//			usNumMatches++;
	//		}
	//	}

	//	// if any are eligible, pick one of them at random
	//	if( usNumMatches )
	//	{
	//		usRandom = (UINT16)Random( usNumMatches );
	//		for( i = 0; i < MAXITEMS; i++ )
	//		{
	//			if ( Item[i].usItemClass  == 0 )
	//				break;
	//			pItem = &Item[ i ];
	//			// skip toolkits
	//			if( ( pItem->usItemClass == IC_KIT ) && ( pItem->ubCoolness > 0 ) && pItem->ubCoolness <= bKitClass && ( !Item[i].toolkit ) )
	//			{
	//				if( usRandom )
	//					usRandom--;
	//				else
	//				{
	//					usKitItem = i;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
#endif //obsoleteCode

	// if we still want more medical and first aid kits, we can just add extras of those item #s to the array
	usKitItem = PickARandomItem( KIT, pp->ubSoldierClass, bKitClass, FALSE );
	if ( usKitItem > 0 )
	{
		CreateItem( usKitItem, (INT8)(80 + Random( 21 )), &gTempObject );
		gTempObject.fFlags |= OBJECT_UNDROPPABLE;
		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	}
}


void ChooseMiscGearForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bMiscClass )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseMiscGearForSoldierCreateStruct");
	//UINT16 i;
	//INVTYPE *pItem;
	UINT16 usRandom;

	//Madd: let's do this a couple times, so we can have > 1 misc item per soldier
	usRandom = Random(3);

	if ( usRandom )
	{
		UINT16 usItem = 0;
		for ( UINT16 i=0;i<usRandom;i++ )
		{
			usItem = PickARandomItem (MISCITEMS, pp->ubSoldierClass, bMiscClass, FALSE);
			if ( usItem > 0 )
			{
				int bStatus = 80 + Random( 21 );
				if ( Item[usItem].usItemClass == IC_MONEY )
					bStatus = 1 + Random( 20 );

				CreateItem( usItem, (INT8)(bStatus), &gTempObject );
				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
				PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
			}
		}
	}

	// Madd: code above is externalized
#ifdef obsoleteCode
	//// not all of these are IC_MISC, some are IC_PUNCH (not covered anywhere else)
	//INT32 iMiscItemsList[] =
	//{
	//	CANTEEN,
	//	CANTEEN,
	//	CANTEEN,
	//	CANTEEN,
	//	ALCOHOL,
	//	ALCOHOL,
	//	ADRENALINE_BOOSTER,
	//	ADRENALINE_BOOSTER,
	//	REGEN_BOOSTER,
	//	BRASS_KNUCKLES,
	//	CHEWING_GUM,
	//	CIGARS,
	//	GOLDWATCH,
	//	-1
	//};


	//// count how many are eligible
	//i = 0;
	//while ( iMiscItemsList[ i ] != -1 )
	//{
	//	pItem = &Item[ iMiscItemsList [ i ] ];
	//	if( ( pItem->ubCoolness > 0 ) && ( pItem->ubCoolness <= bMiscClass ) )
	//	{
	//		// exclude REGEN_BOOSTERs unless Sci-Fi flag is on
	//		if ( ( iMiscItemsList [ i ] != REGEN_BOOSTER ) || ( gGameOptions.ubGameStyle == STYLE_SCIFI ) )
	//		{
	//			usNumMatches++;
	//		}
	//	}

	//	i++;
	//}


	//// if any are eligible, pick one of them at random
	//if( usNumMatches )
	//{
	//	usRandom = (UINT16)Random( usNumMatches );

	//	i = 0;
	//	while ( iMiscItemsList[ i ] != -1 )
	//	{
	//		pItem = &Item[ iMiscItemsList [ i ] ];
	//		if( ( pItem->ubCoolness > 0 ) && ( pItem->ubCoolness <= bMiscClass ) )
	//		{
	//			// exclude REGEN_BOOSTERs unless Sci-Fi flag is on
	//			if ( ( iMiscItemsList [ i ] != REGEN_BOOSTER ) || ( gGameOptions.ubGameStyle == STYLE_SCIFI ) )
	//			{
	//				if( usRandom )
	//					usRandom--;
	//				else
	//				{
	//					CreateItem( ( UINT16 ) iMiscItemsList[ i ], (INT8)(80 + Random( 21 )), &gTempObject );
	//					gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//					PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//					break;
	//				}
	//			}
	//		}

	//		i++;
	//	}
	//}
#endif //obsoleteCode
}


void ChooseBombsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bBombClass )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseBombsForSoldierCreateStruct");
	//UINT16 i;
	//INVTYPE *pItem;
	//UINT16 usRandom;
	UINT16 usItem = 0;

	usItem = PickARandomItem( BOMB, pp->ubSoldierClass, bBombClass, FALSE );
	if ( usItem > 0 )
	{
		CreateItem( usItem, (INT8)(80 + Random( 21 )), &gTempObject );
		gTempObject.fFlags |= OBJECT_UNDROPPABLE;
		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	}

	// Madd: externalized again
#ifdef obsoleteCode
	//// count how many are eligible
	//for( i = 0; i < MAXITEMS; i++ )
	//{
	//	if ( Item[i].usItemClass  == 0 )
	//		break;
	//	pItem = &Item[ i ];
	//	if( ( pItem->usItemClass == IC_BOMB ) && ( pItem->ubCoolness > 0 ) && ( pItem->ubCoolness <= bBombClass ) )
	//	{
	//		usNumMatches++;
	//	}
	//}


	//// if any are eligible, pick one of them at random
	//if( usNumMatches )
	//{
	//	usRandom = (UINT16)Random( usNumMatches );
	//	for( i = 0; i < MAXITEMS; i++ )
	//	{
	//		if ( Item[i].usItemClass  == 0 )
	//			break;

	//		pItem = &Item[ i ];
	//		if( ( pItem->usItemClass == IC_BOMB ) && ( pItem->ubCoolness > 0 ) && ( pItem->ubCoolness <= bBombClass ) )
	//		{
	//			if( usRandom )
	//				usRandom--;
	//			else
	//			{
	//				CreateItem( i, (INT8)(80 + Random( 21 )), &gTempObject );
	//				gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	//				PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	//				break;
	//			}
	//		}
	//	}
	//}
#endif //obsoleteCode
}

// Headrock: Added a function to randomly create LBEs
void ChooseLBEsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bLBEClass )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseLBEsForSoldierCreateStruct");
	//UINT16 i;
	//INVTYPE *pItem;
	//UINT16 usRandom;
	UINT16 usItem = 0;

	// CHRISL: If we're using the old inventory system, just return
	if((UsingNewInventorySystem() == false))
		return;

	usItem = PickARandomItem( LBE, pp->ubSoldierClass, bLBEClass, FALSE );
	if ( usItem > 0 )
	{
		CreateItem( usItem, (INT8)(80 + Random( 21 )), &gTempObject );
		gTempObject.fFlags |= OBJECT_UNDROPPABLE;
		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
	}
}


void ChooseLocationSpecificGearForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"ChooseLocationSpecificGearForSoldierCreateStruct");
	UINT16 usItem = 0;
	// If this is Tixa and the player doesn't control Tixa then give all enemies gas masks,
	// but somewhere on their person, not in their face positions
	if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y && StrategicMap[ TIXA_SECTOR_X + TIXA_SECTOR_Y * MAP_WORLD_X ].fEnemyControlled )
	{
		//dnl ch40 041009
		usItem = PickARandomItem(GASMASKS, pp->ubSoldierClass);
		if(Random(100) > 80)
			usItem = NOTHING;
		if(usItem > 0)
		{
			CreateItem(usItem, (INT8)(90+Random(10)), &gTempObject);
			PlaceObjectInSoldierCreateStruct(pp, &gTempObject);
		}
	}
}



BOOLEAN PlaceObjectInSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, OBJECTTYPE *pObject )
{
	INT8 i;

	if( FitsInSmallPocket(pObject) == true ) {
		// CHRISL: Change static inventory pocket definition to dynamic
		//try to get it into a small pocket first
		for( i = SMALLPOCKSTART; i < SMALLPOCKFINAL; i++ )
		{
			if( pp->Inv[ i ].exists() == false && !(pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE) )
			{
				pp->Inv[ i ] = *pObject;
				return TRUE;
			}
		}
	}

	//NPCs skip medium pockets

	// CHRISL: Change static inventory pocket definition to dynamic
	for( i = BIGPOCKSTART; i < BIGPOCKFINAL; i++ )
	{ //no space free in small pockets, so put it into a large pocket.
		if( pp->Inv[ i ].exists() == false && !(pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE) )
		{
			pp->Inv[ i ] = *pObject;
			return TRUE;
		}
	}
	return FALSE;
}

void RandomlyChooseWhichItemsAreDroppable( SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass )
{
	UINT32 i;
	INT32 j;
//	UINT16 usRandomNum;
	UINT32 uiItemClass;
	UINT8 ubNumMatches = 0;
	UINT16 usItem;
	UINT8 ubAmmoDropRate;
	UINT8 ubGrenadeDropRate;
	UINT8 ubOtherDropRate;
	BOOLEAN fWeapon = FALSE;
	BOOLEAN fGrenades = FALSE;	// this includes all  grenades!
	BOOLEAN fAmmo = FALSE;
	BOOLEAN fArmour = FALSE;
	BOOLEAN fKnife = FALSE;
	BOOLEAN fKit = FALSE;
	BOOLEAN fFace = FALSE;
	BOOLEAN fMisc = FALSE;
	UINT32 uiRandomValue = 0;

	//Madd
//if ( gGameSettings.fOptions[TOPTION_DROP_ALL] ) // EXTERNILIZED TO THE INI INSTEAD - SANDRO
if ( gGameOptions.fEnemiesDropAllItems )
{
	ENEMYAMMODROPRATE = 100;
	ENEMYGRENADEDROPRATE = 100;
	ENEMYEQUIPDROPRATE = 100;	// WANNE: Changed from 50 to 100, because DROP ALL should mean DROP ALL!
	MILITIAAMMODROPRATE = 100;
	MILITIAGRENADEDROPRATE = 100;
	MILITIAEQUIPDROPRATE = 100;   // WANNE: Changed from 50 to 100, because DROP ALL should mean DROP ALL!
}
else
{
	// Default random drop
	if (gGameExternalOptions.ubEnemiesItemDrop == 0)
	{
		ENEMYAMMODROPRATE = 50;
		ENEMYGRENADEDROPRATE = 25;
		ENEMYEQUIPDROPRATE = 15;
		MILITIAAMMODROPRATE = 5;
		MILITIAGRENADEDROPRATE = 3;
		MILITIAEQUIPDROPRATE = 2;
	}
	// Get drop rate from XML-Files
	else
	{
		// Reset, because it is not used anymore
		ENEMYAMMODROPRATE = -1;
		ENEMYGRENADEDROPRATE = -1;
		ENEMYEQUIPDROPRATE = -1;
		MILITIAAMMODROPRATE = -1;
		MILITIAGRENADEDROPRATE = -1;
		MILITIAEQUIPDROPRATE = -1;
	}
}

#ifdef obsoleteCode
/*
	//40% of soldiers will have droppable items.
	usRandomNum = (UINT16)Random( 1000 );
	if( usRandomNum >= 400 )
		return;
	//so now the number is 0-399.  This is kind of like a D&D die roll where
	//various numbers drop different items, or even more than one item.  At this
	//point, we don't care if the enemy has anything in the slot that is made droppable.
	//Any items containing the OBJECT_NO_OVERWRITE slot is rejected for droppable
	//consideration.

	if( usRandomNum < 32 ) //3.2% of dead bodies present the possibility of several items (0-5 items : avg 3).
	{ //31 is the magic number that allows all 5 item classes to be dropped!
		if( usRandomNum & 16 )
			fWeapon = TRUE;
		if( usRandomNum & 8 )
			fAmmo = TRUE;
		if( usRandomNum & 4 )
			fGrenades = TRUE;
		if( usRandomNum & 2 )
			fArmour = TRUE;
		if( usRandomNum & 1 )
			fMisc = TRUE;
	}
	else if( usRandomNum < 100 ) //6.8% chance of getting 2-3 different items.
	{ //do a more generalized approach to dropping items.
		switch( usRandomNum / 10 )
		{
			case 3:	fWeapon = TRUE;											fAmmo = TRUE;																break;
			case 4:	fWeapon = TRUE;	fGrenades = TRUE;																							break;
			case 5:									fGrenades = TRUE;																fMisc = TRUE;	break;
			case 6:									fGrenades = TRUE;								fArmour = TRUE;								break;
			case 7:																			fAmmo = TRUE;	fArmour = TRUE;								break;
			case 8:																			fAmmo = TRUE;	fArmour = TRUE;	fMisc = TRUE;	break;
			case 9:									fGrenades = TRUE;	fAmmo = TRUE;									fMisc = TRUE;	break;
		}
	}
	else
	{
		switch( usRandomNum / 50 ) //30% chance of getting 1-2 items (no weapons)
		{
			case 2:									fGrenades = TRUE;																							break;
			case 3:																			fAmmo = TRUE;																break;
			case 4:																									fArmour = TRUE;									break;
			case 5:																																		fMisc = TRUE;	break;
			case 6:																			fAmmo = TRUE;									fMisc = TRUE;	break;
			case 7:									fGrenades = TRUE;	fAmmo = TRUE;																break;
		}
	}

	fKnife = (Random(3)) ? FALSE : TRUE;
*/
#endif //obsoleteCode


	if ( SOLDIER_CLASS_MILITIA( bSoldierClass ) )
	{
		// militia (they drop much less stuff)
		ubAmmoDropRate		= MILITIAAMMODROPRATE;
		ubGrenadeDropRate = MILITIAGRENADEDROPRATE;
		ubOtherDropRate = MILITIAEQUIPDROPRATE;
	}
	else
	{
		// enemy army
		ubAmmoDropRate  = ENEMYAMMODROPRATE;
		ubGrenadeDropRate = ENEMYGRENADEDROPRATE;
		ubOtherDropRate = ENEMYEQUIPDROPRATE;
	}

	// only enemy soldiers use auto-drop system!
	// don't use the auto-drop system in auto-resolve: player won't see what's being used & enemies will often win & keep'em
	// HEADROCK HAM B2.8: Militia now drop all their equipment... IF not killed by player (see TurnSoldierIntoCorpse() )
	if ( (SOLDIER_CLASS_ENEMY( bSoldierClass ) || ( gGameExternalOptions.ubMilitiaDropEquipment > 0 && SOLDIER_CLASS_MILITIA( bSoldierClass ) )) && !IsAutoResolveActive() )
	{
		// SPECIAL handling for weapons: we'll always drop a weapon type that has never been dropped before
		UINT32 invsize = pp->Inv.size();
		for( i = 0; i < invsize; ++i )
		{
			usItem = pp->Inv[ i ].usItem;
//TODO: someday maybe 			pp->Inv[i].fFlags &= ~ITEM_REPAIRABLE;

			//if ( gGameSettings.fOptions[TOPTION_DROP_ALL]  ) // EXTERNILIZED TO THE INI INSTEAD - SANDRO
			if ( gGameOptions.fEnemiesDropAllItems )
			{
				// WANNE: "Drop all" should mean "Drop all"
				/*
				if ( Item[ pp->Inv[ i ].usItem ].usItemClass == IC_FACE )
				{
					if( Random(100) < ubOtherDropRate ) // way too many gas masks, nvg and extended ears getting dropped!
						pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
				}
				else
				*/

				pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
			}
			else
			{
				// Default random drop
				if (gGameExternalOptions.ubEnemiesItemDrop == 0)
				{
						// if it's a weapon (monster parts included - they won't drop due to checks elsewhere!)
					if ( (usItem > NONE) && (usItem < gMAXITEMS_READ) ) // Madd -- this should be ok set to maxitems instead of max_Weapons
					{
						// and we're allowed to change its flags
						if(! (pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE ))
						{
							// and it's never been dropped before in this game
							if (!gStrategicStatus.fWeaponDroppedAlready[usItem])
							{
								// mark it as droppable, and remember we did so.  If the player never kills this particular dude, oh well,
								// tough luck, he missed his chance for an easy reward, he'll have to wait til next time and need some luck...
								pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;

								MarkAllWeaponsOfSameGunClassAsDropped( usItem );
							}
						}
					}
				}
			}
		}
	}

	//if ( gGameSettings.fOptions[TOPTION_DROP_ALL]  ) // EXTERNILIZED TO THE INI INSTEAD - SANDRO
	if ( gGameOptions.fEnemiesDropAllItems )
		return;



	// WANNE: Randomly choose which type of items should be dropped
	if (gGameExternalOptions.ubEnemiesItemDrop == 0)
	{
		if( Random(100) < ubAmmoDropRate )
			fAmmo = TRUE;

		if( Random(100) < ubOtherDropRate )
			fWeapon = TRUE;

		if( Random(100) < ubOtherDropRate )
			fArmour = TRUE;

		if( Random(100) < ubOtherDropRate )
			fKnife = TRUE;

		if( Random(100) < ubGrenadeDropRate )
			fGrenades = TRUE;

		if( Random(100) < ubOtherDropRate )
			fKit = TRUE;

		if( Random(100) < (UINT32)(ubOtherDropRate / 3) )
			fFace = TRUE;

		if( Random(100) < ubOtherDropRate )
			fMisc = TRUE;


		//Now, that the flags are set for each item, we now have to search through the item slots to
		//see if we can find a matching item, however, if we find any items in a particular class that
		//have the OBJECT_NO_OVERWRITE flag set, we will not make any items droppable for that class
		//because the editor would have specified it already.
		if( fAmmo )
		{
			// now drops ALL ammo found, not just the first slot
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_AMMO )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						continue;
					else
					{
						pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
					}
				}
			}
		}

		if( fWeapon )
		{
			ubNumMatches = 0;
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_GUN || uiItemClass == IC_LAUNCHER )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						break;
					else
						++ubNumMatches;
				}
			}
			if ( ubNumMatches > 0 )
			{
				for( i = 0; i < invsize; ++i )
				{
					uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
					if( uiItemClass == IC_GUN || uiItemClass == IC_LAUNCHER )
					{
						if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
							break;
						else if( !Random( ubNumMatches-- )  )
						{
							pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
							break;
						}
					}
				}
			}
		}

		if( fArmour )
		{
			ubNumMatches = 0;
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_ARMOUR )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						break;
					else
						++ubNumMatches;
				}
			}
			if ( ubNumMatches > 0 )
			{
				for( i = 0; i < invsize; ++i )
				{
					uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
					if( uiItemClass == IC_ARMOUR )
					{
						if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
							break;
						else if( !Random( ubNumMatches-- ) )
						{
							pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
							break;
						}
					}
				}
			}
		}

		if( fKnife)
		{
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				// drops FIRST knife found
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_BLADE || uiItemClass == IC_THROWING_KNIFE )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						break;
					else
					{
						pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
						break;
					}
				}
			}
		}

		// note that they'll only drop ONE TYPE of grenade if they have multiple types (very common)
		if( fGrenades )
		{
			ubNumMatches = 0;
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_GRENADE )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						break;
					else
						++ubNumMatches;
				}
			}
			if ( ubNumMatches > 0 )
			{
				for( i = 0; i < invsize; ++i )
				{
					uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
					if( uiItemClass == IC_GRENADE )
					{
						if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
							break;
						else if( !Random( ubNumMatches-- )  )
						{
							pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
							break;
						}
					}
				}
			}
		}

		if( fKit )
		{
			ubNumMatches = 0;
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_MEDKIT || uiItemClass == IC_KIT )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						break;
					else
						ubNumMatches++;
				}
			}
			if ( ubNumMatches > 0 )
			{
				for( i = 0; i < invsize; ++i )
				{
					uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
					if( uiItemClass == IC_MEDKIT || uiItemClass == IC_KIT )
					{
						if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
							break;
						else if( !Random( ubNumMatches-- )  )
						{
							pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
							break;
						}
					}
				}
			}
		}

		if( fFace )
		{
			ubNumMatches = 0;
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_FACE )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						break;
					else
						++ubNumMatches;
				}
			}
			if ( ubNumMatches > 0 )
			{
				for( i = 0; i < invsize; ++i )
				{
					uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
					if( uiItemClass == IC_FACE )
					{
						if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
							break;
						else if( !Random( ubNumMatches-- ) )
						{
							pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
							break;
						}
					}
				}
			}
		}

		if( fMisc )
		{
			ubNumMatches = 0;
			UINT32 invsize = pp->Inv.size();
			for( i = 0; i < invsize; ++i )
			{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
				if( uiItemClass == IC_MISC )
				{
					if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
						break;
					else
						++ubNumMatches;
				}
			}
			if ( ubNumMatches > 0 )
			{
				for( i = 0; i < invsize; ++i )
				{
					uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;
					if( uiItemClass == IC_MISC )
					{
						if( pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE )
							break;
						else if( !Random( ubNumMatches-- ) )
						{
							pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
							break;
						}
					}
				}
			}
		}
	}
	// WANNE: Drop-rate from different XML-Files
	else if (gGameExternalOptions.ubEnemiesItemDrop == 1)
	{
		// Loop through the enemy inter
		UINT32 invsize = pp->Inv.size();
		for( i = 0; i < invsize; ++i )
		{
				uiItemClass = Item[ pp->Inv[ i ].usItem ].usItemClass;

				// We are allowed to change the object and it is not the first (nothing) object
				//if(! (pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE ) && pp->Inv[ i ].usItem != 0)
				if( pp->Inv[ i ].exists() == true)
				{
					// Weapon
					if( uiItemClass == IC_GUN)
					{
						// Find matching weaponType in the XML
						for (j = 0; j < MAX_DROP_ITEMS; ++j)
						{
							// We have no more weapon items->exit from loop
							if (j > 0 && gEnemyWeaponDrops[j].uiIndex == 0)
								break;

							// We found the matching weapon type
							if (Weapon[ Item[ pp->Inv[ i ].usItem ].ubClassIndex ].ubWeaponType == gEnemyWeaponDrops[j].ubWeaponType)
							{
								uiRandomValue = Random(100);

								if (uiRandomValue == 0)
									uiRandomValue++;

								// militia
								if ( SOLDIER_CLASS_MILITIA( bSoldierClass ) )
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyWeaponDrops[j].ubMilitiaDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}
								// enemy
								else
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyWeaponDrops[j].ubEnemyDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}

								break;
							}
						}
					}
					// Ammo
					else if (uiItemClass == IC_AMMO)
					{
						// Find matching ammo in the XML
						for (j = 0; j < MAX_DROP_ITEMS; ++j)
						{
							// We have no more ammo items->exit from loop
							if (j > 0 && gEnemyAmmoDrops[j].uiIndex == 0)
								break;

							// We found the matching ammo type
							if (Magazine[ Item[ pp->Inv[ i ].usItem ].ubClassIndex ].ubAmmoType == gEnemyAmmoDrops[j].uiType)
							{
								uiRandomValue = Random(100);

								if (uiRandomValue == 0)
									++uiRandomValue;

								// militia
								if ( SOLDIER_CLASS_MILITIA( bSoldierClass ) )
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyAmmoDrops[j].ubMilitiaDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}
								else
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyAmmoDrops[j].ubEnemyDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}

								break;
							}
						}
					}
					// Explosive
					else if (uiItemClass == IC_GRENADE || uiItemClass == IC_BOMB)
					{
						// Find matching explosive in the XML
						for (j = 0; j < MAX_DROP_ITEMS; j++)
						{
							// We have no more explosive items->exit from loop
							if (j > 0 && gEnemyExplosiveDrops[j].uiIndex == 0)
								break;

							// We found the matching explosive type
							if (Explosive[Item[ pp->Inv[ i ].usItem ].ubClassIndex].ubType == gEnemyExplosiveDrops[j].ubType)
							{
								uiRandomValue = Random(100);

								if (uiRandomValue == 0)
									uiRandomValue++;

								// militia
								if ( SOLDIER_CLASS_MILITIA( bSoldierClass ) )
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyExplosiveDrops[j].ubMilitiaDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}
								// enemy
								else
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyExplosiveDrops[j].ubEnemyDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}

								break;
							}
						}
					}
					// Armour
					else if (uiItemClass == IC_ARMOUR)
					{
						// Find matching armour in the XML
						for (j = 0; j < MAX_DROP_ITEMS; j++)
						{
							// We have no more armour items->exit from loop
							if (j > 0 && gEnemyArmourDrops[j].uiIndex == 0)
								break;

							// We found the matching armour type
							if (Armour[ Item[ pp->Inv[ i ].usItem ].ubClassIndex ].ubArmourClass == gEnemyArmourDrops[j].ubArmourClass)
							{
								uiRandomValue = Random(100);

								if (uiRandomValue == 0)
									++uiRandomValue;

								// militia
								if ( SOLDIER_CLASS_MILITIA( bSoldierClass ) )
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyArmourDrops[j].ubMilitiaDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}
								// enemy
								else
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyArmourDrops[j].ubEnemyDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}

								break;
							}
						}
					}
					// Misc
					else if (uiItemClass == IC_BLADE || uiItemClass == IC_MEDKIT || uiItemClass == IC_KIT ||
						uiItemClass == IC_THROWING_KNIFE || uiItemClass == IC_LAUNCHER ||
						uiItemClass == IC_APPLIABLE || uiItemClass == IC_FACE || uiItemClass == IC_TENTACLES ||
						uiItemClass == IC_THROWN || uiItemClass == IC_PUNCH || uiItemClass == IC_KEY ||
						uiItemClass == IC_MISC || uiItemClass == IC_MONEY
						
						// WANNE: Fix by Roadkill: Also drop LBE Gear
						|| uiItemClass == IC_LBEGEAR || uiItemClass == IC_BELTCLIP						
						)
					{
						// Find matching armour in the XML
						for (j = 0; j < MAX_DROP_ITEMS; j++)
						{
							// We have no more armour items->exit from loop
							if (j > 0 && gEnemyMiscDrops[j].uiIndex == 0)
								break;

							// We found the matching armour type
							if (Item[ pp->Inv[ i ].usItem ].usItemClass == gEnemyMiscDrops[j].usItemClass)
							{
								uiRandomValue = Random(100);

								if (uiRandomValue == 0)
									uiRandomValue++;

								// militia
								if ( SOLDIER_CLASS_MILITIA( bSoldierClass ) )
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyMiscDrops[j].ubMilitiaDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}
								// enemy
								else
								{
									// Drop the item!
									if (uiRandomValue <= gEnemyMiscDrops[j].ubEnemyDropRate)
									{
										pp->Inv[ i ].fFlags &= ~OBJECT_UNDROPPABLE;
									}
								}

								break;
							}
						}
					}
				}
			}
	}
}


void AssignCreatureInventory( SOLDIERTYPE *pSoldier )
{
	UINT32 uiChanceToDrop = 0;
	BOOLEAN fMaleCreature = FALSE;
	BOOLEAN fBloodcat = FALSE;

	// all creature items in this first section are only offensive/defensive placeholders, and
	// never get dropped, because they're not real items!
	switch( pSoldier->ubBodyType)
	{
		case ADULTFEMALEMONSTER:
			CreateItem( CREATURE_OLD_FEMALE_CLAWS,	100, &(pSoldier->inv[HANDPOS]) );
			CreateItem( CREATURE_OLD_FEMALE_HIDE,		100, &(pSoldier->inv[HELMETPOS]) );
			CreateItem( CREATURE_OLD_FEMALE_HIDE,		100, &(pSoldier->inv[VESTPOS]) );
			CreateItem( CREATURE_OLD_FEMALE_HIDE,		100, &(pSoldier->inv[LEGPOS]) );
			uiChanceToDrop = 30;
			break;
		case AM_MONSTER:
			CreateItem( CREATURE_OLD_MALE_CLAWS,		100, &(pSoldier->inv[HANDPOS]) );
			CreateItem( CREATURE_OLD_MALE_SPIT,			100, &(pSoldier->inv[SECONDHANDPOS]) );
			CreateItem( CREATURE_OLD_MALE_HIDE,			100, &(pSoldier->inv[HELMETPOS]) );
			CreateItem( CREATURE_OLD_MALE_HIDE,			100, &(pSoldier->inv[VESTPOS]) );
			CreateItem( CREATURE_OLD_MALE_HIDE,			100, &(pSoldier->inv[LEGPOS]) );
			uiChanceToDrop = 30;
			fMaleCreature = TRUE;
			break;
		case YAF_MONSTER:
			CreateItem( CREATURE_YOUNG_FEMALE_CLAWS,		100, &(pSoldier->inv[HANDPOS]) );
			CreateItem( CREATURE_YOUNG_FEMALE_HIDE,			100, &(pSoldier->inv[HELMETPOS]) );
			CreateItem( CREATURE_YOUNG_FEMALE_HIDE,			100, &(pSoldier->inv[VESTPOS]) );
			CreateItem( CREATURE_YOUNG_FEMALE_HIDE,			100, &(pSoldier->inv[LEGPOS]) );
			uiChanceToDrop = 15;
			break;
		case YAM_MONSTER:
			CreateItem( CREATURE_YOUNG_MALE_CLAWS,		100, &(pSoldier->inv[HANDPOS]) );
			CreateItem( CREATURE_YOUNG_MALE_SPIT,			100, &(pSoldier->inv[SECONDHANDPOS]) );
			CreateItem( CREATURE_YOUNG_MALE_HIDE,			100, &(pSoldier->inv[HELMETPOS]) );
			CreateItem( CREATURE_YOUNG_MALE_HIDE,			100, &(pSoldier->inv[VESTPOS]) );
			CreateItem( CREATURE_YOUNG_MALE_HIDE,			100, &(pSoldier->inv[LEGPOS]) );
			uiChanceToDrop = 15;
			fMaleCreature = TRUE;
			break;
		case INFANT_MONSTER:
			CreateItem( CREATURE_INFANT_SPIT,			100, &(pSoldier->inv[HANDPOS]) );
			CreateItem( CREATURE_INFANT_HIDE,			100, &(pSoldier->inv[HELMETPOS]) );
			CreateItem( CREATURE_INFANT_HIDE,			100, &(pSoldier->inv[VESTPOS]) );
			CreateItem( CREATURE_INFANT_HIDE,			100, &(pSoldier->inv[LEGPOS]) );
			uiChanceToDrop = 5;
			break;
		case LARVAE_MONSTER:
			uiChanceToDrop = 0;
			break;
		case QUEENMONSTER:
			CreateItem( CREATURE_QUEEN_SPIT,			100, &(pSoldier->inv[HANDPOS]) );
			CreateItem( CREATURE_QUEEN_TENTACLES, 100, &(pSoldier->inv[SECONDHANDPOS]) );
			CreateItem( CREATURE_QUEEN_HIDE,			100, &(pSoldier->inv[HELMETPOS]) );
			CreateItem( CREATURE_QUEEN_HIDE,			100, &(pSoldier->inv[VESTPOS]) );
			CreateItem( CREATURE_QUEEN_HIDE,			100, &(pSoldier->inv[LEGPOS]) );
			// she can't drop anything, because the items are unreachable anyways (she's too big!)
			uiChanceToDrop = 0;
			break;
		case BLOODCAT:
			CreateItem( BLOODCAT_CLAW_ATTACK,		100, &(pSoldier->inv[HANDPOS]) );
			CreateItem( BLOODCAT_BITE,		100, &(pSoldier->inv[SECONDHANDPOS]) );
			fBloodcat = TRUE;
			uiChanceToDrop = 30;
			break;

		default:
			AssertMsg( FALSE, String( "Invalid creature bodytype %d", pSoldier->ubBodyType ) );
			return;
	}

	// decide if the creature will drop any REAL bodyparts
	if (Random(100) < uiChanceToDrop)
	{
		CreateItem( (UINT16)(fBloodcat ? BLOODCAT_CLAWS : CREATURE_PART_CLAWS), (INT8) (80 + Random(21)), &(pSoldier->inv[BIGPOCK1POS]) );
	}

	if (Random(100) < uiChanceToDrop)
	{
		CreateItem( (UINT16)(fBloodcat ? BLOODCAT_TEETH : CREATURE_PART_FLESH), (INT8) (80 + Random(21)), &(pSoldier->inv[BIGPOCK2POS]) );
	}

	// as requested by ATE, males are more likely to drop their "organs" (he actually suggested this, I'm serious!)
	if (fMaleCreature)
	{
		// increase chance by 50%
		uiChanceToDrop += (uiChanceToDrop / 2);
	}

	if (Random(100) < uiChanceToDrop)
	{
		CreateItem( (UINT16)(fBloodcat ? BLOODCAT_PELT : CREATURE_PART_ORGAN), (INT8) (80 + Random(21)), &(pSoldier->inv[BIGPOCK3POS]) );
	}
}

void ReplaceExtendedGuns( SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass )
{
	UINT32				uiLoop, uiLoop2;
	INT8					bWeaponClass;
	UINT16				usItem, usNewGun, usAmmo, usNewAmmo;

	UINT32 invsize = pp->Inv.size();
	for ( uiLoop = 0; uiLoop < invsize; ++uiLoop )
	{
		usItem = pp->Inv[ uiLoop ].usItem;

		if ( ( Item[ usItem ].usItemClass & IC_GUN ) && ExtendedGunListGun( usItem ) )
		{
			if ( bSoldierClass == SOLDIER_CLASS_NONE )
			{
				usNewGun = StandardGunListReplacement( usItem );
			}
			else
			{
				bWeaponClass = GetWeaponClass( usItem );
				AssertMsg( bWeaponClass != -1, String( "Gun %d does not have a match in the extended gun array", usItem ) );
				usNewGun = SelectStandardArmyGun( bWeaponClass, bSoldierClass );
			}

			if ( usNewGun != NOTHING )
			{
				//We are creating a new gun, but the new gun needs the old gun's attachments
				OBJECTTYPE* pObj = &(pp->Inv[ uiLoop ]);
				CreateItem( usNewGun, (*pObj)[0]->data.gun.bGunStatus, &gTempObject );
				gTempObject.fFlags = (*pObj).fFlags;

				//init attachments
				InitItemAttachments(pObj);

				for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
					if(iter->exists()){
						gTempObject.AttachObject(0, &(*iter), FALSE);
					}
				}

				//copy it over
				*pObj = gTempObject;


				// must search through inventory and replace ammo accordingly
				UINT32 invsize = pp->Inv.size();
				for ( uiLoop2 = 0; uiLoop2 < invsize; ++uiLoop2 )
				{
					usAmmo = pp->Inv[ uiLoop2 ].usItem;
					if ( (Item[ usAmmo ].usItemClass & IC_AMMO) )
					{
						usNewAmmo = FindReplacementMagazineIfNecessary( usItem, usAmmo, usNewGun );
						if (usNewAmmo != NOTHING )
						{
							// found a new magazine, replace...
							CreateItems( usNewAmmo, 100, pp->Inv[ uiLoop2 ].ubNumberOfObjects, &( pp->Inv[ uiLoop2 ] ) );
						}
					}
				}
			}
		}
	}
}



UINT16 SelectStandardArmyGun( UINT8 uiGunLevel, INT8 bSoldierClass )
{
	ARMY_GUN_CHOICE_TYPE *pGunChoiceTable;
	int uiChoice;
	int usGunIndex;

	// pick the standard army gun for this weapon class from table
//	usGunIndex = gStrategicStatus.ubStandardArmyGunIndex[uiGunLevel];

	// decided to randomize it afterall instead of repeating the same weapon over and over

	// depending on selection of the gun nut option
	//if (gGameOptions.fGunNut)
	//{
		// use table of extended gun choices

		// Flugente: if accessing with wrong soldier class, or not using different selection choices, take default one
		if ( bSoldierClass >= SOLDIER_GUN_CHOICE_SELECTIONS || bSoldierClass < SOLDIER_CLASS_NONE || !gGameExternalOptions.fSoldierClassSpecificItemTables )
			bSoldierClass = SOLDIER_CLASS_NONE;

		pGunChoiceTable = &(gExtendedArmyGunChoices[bSoldierClass][0]);
	//}
	//else
	//{
	//	// use table of regular gun choices
	//	pGunChoiceTable = &(gRegularArmyGunChoices[0]);
	//}

	// choose one the of the possible gun choices
	usGunIndex = -1;

	BOOLEAN isnight = NightTime();

	while (usGunIndex == -1)
	{
		uiChoice = Random(pGunChoiceTable[ uiGunLevel ].ubChoices);
		usGunIndex = pGunChoiceTable[ uiGunLevel ].bItemNo[ uiChoice ];

		if (!ItemIsLegal(usGunIndex)) //Madd: check for tons of guns
			usGunIndex = -1;

		//Check to avoid an endless loop looking for "normal" guns
		if (usGunIndex == -1)
		{
			//Madd: there better be something from the original JA2 guns here somewhere (biggunlist=0)!
			int numTries = 0;
			//Try 5 more times...
			while (numTries < 5 && usGunIndex == -1)
			{
				uiChoice = Random(pGunChoiceTable[ uiGunLevel ].ubChoices);
				usGunIndex = pGunChoiceTable[ uiGunLevel ].bItemNo[ uiChoice ];

				if (!ItemIsLegal(usGunIndex)) //Madd: check for tons of guns
					usGunIndex = -1;

				numTries++;
			}

			if (usGunIndex == -1) //We still haven't found one!  Start just looping through the guns then
			{
				for (int i=0;i<pGunChoiceTable[uiGunLevel].ubChoices;i++)
				{
					usGunIndex = pGunChoiceTable[ uiGunLevel ].bItemNo[ i ];

					// Flugente: ignore this item if we aren't allowed to pick it at this time of day
					if ( ( isnight && Item[usGunIndex].usItemChoiceTimeSetting == 1 ) || ( !isnight && Item[usGunIndex].usItemChoiceTimeSetting == 2 ) )
						continue;

					if (!ItemIsLegal(usGunIndex))
						usGunIndex = -1;
					else
						break;
				}
			}

			if ( usGunIndex == -1 )
			{
				//Still nothing?  Then he gets a glock
				usGunIndex = GLOCK_17;
			}

		}

	}

	Assert(usGunIndex);

	return(usGunIndex);
}



void EquipTank( SOLDIERCREATE_STRUCT *pp )
{
	// tanks get special equipment, and they drop nothing (MGs are hard-mounted & non-removable)

	// main cannon
	CreateItem( TANK_CANNON, ( INT8 )( 80 + Random( 21 ) ), &( pp->Inv[ HANDPOS ]) );
	pp->Inv[ HANDPOS ].fFlags |= OBJECT_UNDROPPABLE;

	// machine gun
	CreateItems( MINIMI, ( INT8 )( 80 + Random( 21 ) ), 1, &gTempObject );
	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );

	// tanks don't deplete shells or ammo...
	CreateItems( TANK_SHELL, 100, 1, &gTempObject );
	gTempObject.fFlags |= OBJECT_UNDROPPABLE;
	PlaceObjectInSoldierCreateStruct( pp, &gTempObject );

	// armour equal to spectra all over (for vs explosives)
	CreateItem( SPECTRA_VEST, 100, &(pp->Inv[ VESTPOS ]) );
	pp->Inv[ VESTPOS ].fFlags |= OBJECT_UNDROPPABLE;
	CreateItem( SPECTRA_HELMET, 100, &(pp->Inv[ HELMETPOS ]) );
	pp->Inv[ HELMETPOS ].fFlags |= OBJECT_UNDROPPABLE;
	CreateItem( SPECTRA_LEGGINGS, 100, &(pp->Inv[ LEGPOS ]) );
	pp->Inv[ LEGPOS ].fFlags |= OBJECT_UNDROPPABLE;

}



void ResetMortarsOnTeamCount( void )
{
	guiMortarsRolledByTeam = 0;
}

UINT16 PickARandomItem(UINT8 typeIndex, INT8 bSoldierClass)
{
	return PickARandomItem(typeIndex, bSoldierClass, 100,FALSE);
}
UINT16 PickARandomItem(UINT8 typeIndex, INT8 bSoldierClass, UINT8 maxCoolness)
{
	return PickARandomItem(typeIndex, bSoldierClass, maxCoolness,TRUE);
}
UINT16 PickARandomItem(UINT8 typeIndex, INT8 bSoldierClass, UINT8 maxCoolness, BOOLEAN getMatchingCoolness)
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("PickARandomItem: typeIndex = %d, maxCoolness = %d, getMatchingCoolness = %d",typeIndex,maxCoolness,getMatchingCoolness));

	UINT16 usItem = 0;
	UINT32 uiChoice;
	UINT16 defaultItem = 0;
	BOOLEAN pickItem = FALSE;

	// Flugente: if accessing with wrong soldier class, or not using different selection choices, take default one
	if ( bSoldierClass >= SOLDIER_GUN_CHOICE_SELECTIONS || bSoldierClass < SOLDIER_CLASS_NONE || !gGameExternalOptions.fSoldierClassSpecificItemTables )
		bSoldierClass = SOLDIER_CLASS_NONE;

	if ( gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices <= 0 )
		return 0;

	BOOLEAN isnight = NightTime();

	// check up to 10 times for an item with a matching coolness
	for (int i=0; i < 10; ++i)
	{
		//if we've already tried more times then there are items + 1, limit the looping to speed up the game, and just plain give up
		if ( i > gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices )
			break;

		// a chance for nothing!
		uiChoice = Random(gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices + (int) ( gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices / 3 ));

		if ( uiChoice >= gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices )
		{
			if ( !getMatchingCoolness )
				return 0;
			else
				uiChoice = Random(gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices);
		}
		usItem = gArmyItemChoices[bSoldierClass][ typeIndex ].bItemNo[ uiChoice ];

		// Flugente: ignore this item if we aren't allowed to pick it at this time of day
		if ( ( isnight && Item[usItem].usItemChoiceTimeSetting == 1 ) || ( !isnight && Item[usItem].usItemChoiceTimeSetting == 2 ) )
			continue;

		// Flugente: random items
		UINT16 newitemfromrandom = 0;
		if ( GetItemFromRandomItem(usItem, &newitemfromrandom) )
			usItem = newitemfromrandom;

		pickItem = FALSE;

		if (usItem >= 0 && Item[usItem].ubCoolness <= maxCoolness && ItemIsLegal(usItem))
		{
			// On day
			if (DayTime() == TRUE)
			{
				// Only pick items, that have not a negative day vision bonus range
				// So we only pick normal items and day items (sun googles, ...)
				if (Item[usItem].dayvisionrangebonus >= 0 )
				{
					pickItem = TRUE;
				}
			}
			// At night
			else
			{
				// Only pick items, that have not a negative night vision bonus range
				// So we only pick normal items and night items (NVG, ...)
				if (Item[usItem].nightvisionrangebonus >= 0 )
				{
					pickItem = TRUE;
				}
			}

			// Flugente: if item is still random, don't pick it
			if ( Item[usItem].randomitem > 0 )
				pickItem = FALSE;
		}


		//Madd: quickfix: don't use NVGs during the day, and no sungoggles at night either
		//if ( usItem >= 0 && Item[usItem].ubCoolness <= maxCoolness && ItemIsLegal(usItem) && (( DayTime() && Item[usItem].nightvisionrangebonus == 0 ) || ( NightTime() && Item[usItem].dayvisionrangebonus == 0 )))

		if (pickItem == TRUE)
		{
			// pick a default item in case we don't find anything with a matching coolness, but pick the coolest item we can find
			if ( defaultItem == 0 || Item[usItem].ubCoolness > Item[defaultItem].ubCoolness )
				defaultItem = usItem;

			// found something with the right coolness
			if ( Item[usItem].ubCoolness == maxCoolness || !getMatchingCoolness )
				return usItem;
		}
	}

	// couldn't find anything with the exact matching coolness, so return the best item we did find
	if (ItemIsLegal(defaultItem))
		return defaultItem;
	else
		return 0;
}
UINT16 PickARandomAttachment(UINT8 typeIndex, INT8 bSoldierClass, UINT16 usBaseItem, UINT8 maxCoolness, BOOLEAN getMatchingCoolness)
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("PickARandomAttachment: baseItem = %d, typeIndex = %d, maxCoolness = %d, getMatchingCoolness = %d",usBaseItem, typeIndex,maxCoolness,getMatchingCoolness));

	UINT16 usItem = 0;
	UINT32 uiChoice;
	UINT16 defaultItem = 0;

	// Flugente: if accessing with wrong soldier class, or not using different selection choices, take default one
	if ( bSoldierClass >= SOLDIER_GUN_CHOICE_SELECTIONS || bSoldierClass < SOLDIER_CLASS_NONE || !gGameExternalOptions.fSoldierClassSpecificItemTables )
		bSoldierClass = SOLDIER_CLASS_NONE;

//	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("PickARandomAttachment: # choices = %d", gArmyItemChoices[ typeIndex ].ubChoices ));
	if ( gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices <= 0 )
		return 0;

	BOOLEAN isnight = NightTime();

	// check up to 10 times for an item with a matching coolness
	for (int i=0; i < 50; i++)
	{
		//if we've already tried more times then there are items + 1, limit the looping to speed up the game, and just plain give up
		if ( i > gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices )
			break;

		uiChoice = Random(gArmyItemChoices[bSoldierClass][ typeIndex ].ubChoices);
		usItem = gArmyItemChoices[bSoldierClass][ typeIndex ].bItemNo[ uiChoice ];

		// Flugente: ignore this item if we aren't allowed to pick it at this time of day
		if ( ( isnight && Item[usItem].usItemChoiceTimeSetting == 1 ) || ( !isnight && Item[usItem].usItemChoiceTimeSetting == 2 ) )
			continue;

		BOOLEAN fDefaultAttachment = FALSE;
		for(UINT8 cnt = 0; cnt < MAX_DEFAULT_ATTACHMENTS; cnt++){
			if(Item[usBaseItem].defaultattachments[cnt] == 0)
				break;

			if(Item[usBaseItem].defaultattachments[cnt] == usItem){
				fDefaultAttachment = TRUE;
			}
		}
		if ( usItem >= 0 && Item[usItem].ubCoolness <= maxCoolness && ValidAttachment(usItem,usBaseItem) && !fDefaultAttachment)
		{
			// pick a default item in case we don't find anything with a matching coolness, but pick the coolest item we can find
			if ( defaultItem == 0 || Item[usItem].ubCoolness > Item[defaultItem].ubCoolness )
				defaultItem = usItem;

			// found something with the right coolness
			if ( (Item[usItem].ubCoolness >= maxCoolness-2) || !getMatchingCoolness )
			{
//				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("PickARandomAttachment: baseItem = %d, usItem = %d",usBaseItem, usItem));
				if (ItemIsLegal(usItem))
					return usItem;
			}
		}
	}

	// couldn't find anything with the exact matching coolness, so return the best item we did find
//	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("PickARandomAttachment: baseItem = %d, defaultItem = %d",usBaseItem, defaultItem));
	if (ItemIsLegal(defaultItem))
		return defaultItem;
	else
		return 0;
}

////////////////// Flugente: militia equipment feature ///////////////////////////////////
// determines how good an item is
UINT32 ItemFitness( OBJECTTYPE* pObj, UINT8 idx )
{
	UINT32 value = 0;

	if ( Item[ pObj->usItem ].usItemClass & IC_ARMOUR )
	{
		value = Armour[ Item[ pObj->usItem ].ubClassIndex ].ubProtection * Armour[ Item[ pObj->usItem ].ubClassIndex ].ubCoverage * (*pObj)[idx]->data.objectStatus;
	}
	else if ( Item[ pObj->usItem ].usItemClass & IC_FACE )
	{
		if ( Item[ pObj->usItem ].gasmask )
			value = (*pObj)[idx]->data.objectStatus;
		else if ( Item[ pObj->usItem ].hearingrangebonus )
		{
			value = Item[ pObj->usItem ].hearingrangebonus;
		}
		else
		{
			value = (*pObj)[idx]->data.objectStatus * max(0, max( Item[ pObj->usItem ].nightvisionrangebonus, Item[ pObj->usItem ].dayvisionrangebonus ) );
		}
	}
	else if ( Item[ pObj->usItem ].usItemClass & (IC_BLADE|IC_PUNCH) )
	{
		value = GetDamage(pObj);

		value *= (*pObj)[idx]->data.objectStatus;
	}
	else if ( Item[ pObj->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
	{
		// value is damage * range * firing rate, modified by number of attachments and accuracy
		value = GetDamage(pObj) * GunRange(pObj, NULL) * max(1, GetAutofireShotsPerFiveAPs(pObj));

		value = value * (10 + (UINT32)GetGunAccuracy(pObj)) / 10;

		value = value * (10 + (*pObj)[0]->AttachmentListSize()) / 10;

		value *= (*pObj)[idx]->data.objectStatus;
	}
	// default
	else
	{
		value = (*pObj)[idx]->data.objectStatus * Item[pObj->usItem].ubCoolness;
	}

	return value;
}

// how much ammo does this item need?
UINT32 GetNeededTotalAmmo( UINT16 usItem )
{
	// determine what amount of ammo is deemed 'enough' for this gun
	return max(gGameExternalOptions.usMilitiaAmmo_Min, min(gGameExternalOptions.usMilitiaAmmo_Max, gGameExternalOptions.usMilitiaAmmo_OptimalMagCount * Weapon[ usItem ].ubMagSize));
}

// special version of ItemFitness() that takes ammo count into consideration
UINT32 ItemFitness_WithAmmo( OBJECTTYPE* pObj, UINT8 idx, UINT32 uiBullets )
{
	UINT32 optimalammo = GetNeededTotalAmmo(pObj->usItem);
	return (ItemFitness(pObj, idx) * min(uiBullets, optimalammo) ) / max(1, optimalammo);
}

// retreives the next free slot in pWorldItem (we want to keep it short and not add too many items) 
UINT32 GetNextFreeIndex( std::vector<WORLDITEM>& pWorldItem, UINT32 uiMaxCount, UINT32 uiCount )//dnl ch75 271013
{
	for( ; uiCount < uiMaxCount; ++uiCount )
	{
		if( !pWorldItem[ uiCount ].fExists )
			return uiCount;
	}

	return -1;
}

// These are the things we are searching for
enum {
	SI_HELMET = 0,
	SI_VEST,
	SI_LEGS,
	SI_SIGHT,				// for sunglasses and NVGs
	SI_FACE2,
	SI_FACE_SPARESIGHT,		// for sunglasses at night an NVGs during the day - militia take those at well, otherwise mobiles are ill equipped when the night begins/ends
	SI_GASMASK,
	SI_MELEE,
	SI_GRENADE,
	SI_GUN,
	SI_LAUNCHER,

	SI_MAX,
} searchItem;

// this struct is used to store the location of the best item of a category, we use it to later easily retrieve it from the sector inventory
struct ItemSearchStruct {
	ItemSearchStruct::ItemSearchStruct() : found(FALSE), done(FALSE), pos(0), slot(0), soldierslot(0), val(0) {}
	BOOLEAN found;			// has any item been found for this?
	BOOLEAN done;			// are we done evaluating this item?
	UINT32 pos;				// position in pWorldItem of this object
	UINT8 slot;				// slot of the specific object on the position in pWorldItem
	UINT16 soldierslot;		// slot on which this item should be equipped
	UINT32 val;				// the fitness value of this object
};

// evaluate an object an remember it if it is the best so far
void EvaluateObjForItem( std::vector<WORLDITEM>& pWorldItem, OBJECTTYPE* pObj, UINT32 uiCount, ItemSearchStruct* pSi )//dnl ch75 271013
{
	// safety first
	if ( pWorldItem.empty() || !pObj || !pSi )
		// THIS SHOULDN'T HAPPEN!
		return;

	for(UINT8 i = 0; i < pObj->ubNumberOfObjects; ++i)
	{
		UINT32 fitness = ItemFitness(pObj, i);
		if ( !pSi->found || fitness > pSi->val )
		{
			pSi->found	= TRUE;
			pSi->pos	= uiCount;
			pSi->slot	= i;
			pSi->val	= fitness;
		}
	}
}

// special version of EvaluateObjForItem(): we take the existing ammo count into consideration
void EvaluateObjForItem_WithAmmo( std::vector<WORLDITEM>& pWorldItem, OBJECTTYPE* pObj, UINT32 uiCount, ItemSearchStruct* pSi, UINT32 uiBullets )//dnl ch75 271013
{
	// safety first
	if ( pWorldItem.empty() || !pObj || !pSi )
		// THIS SHOULDN'T HAPPEN!
		return;

	for(UINT8 i = 0; i < pObj->ubNumberOfObjects; ++i)
	{
		UINT32 fitness = ItemFitness_WithAmmo(pObj, i, uiBullets);
		if ( !pSi->found || fitness > pSi->val )
		{
			pSi->found	= TRUE;
			pSi->pos	= uiCount;
			pSi->slot	= i;
			pSi->val	= fitness;
		}
	}
}

// forward declaration for default parameter
// if pSi has an entry, move gun from pWorldItem into pp
void SearchItemRetrieval( std::vector<WORLDITEM>& pWorldItem, ItemSearchStruct* pSi, SOLDIERCREATE_STRUCT *pp, UINT8 usTake = 1 );//dnl ch75 271013

// if pSi has an entry, move gun from pWorldItem into pp
void SearchItemRetrieval( std::vector<WORLDITEM>& pWorldItem, ItemSearchStruct* pSi, SOLDIERCREATE_STRUCT *pp, UINT8 usTake )//dnl ch75 271013
{
	if ( pSi->found && !pSi->done )
	{
		UINT8 usRealTake = min(usTake, pWorldItem[ pSi->pos ].object.ubNumberOfObjects);
		pWorldItem[ pSi->pos ].object.MoveThisObjectTo(pp->Inv[ pSi->soldierslot ], usRealTake );

		for ( UINT8 i = 0; i < usRealTake; ++i )
			(pp->Inv[ pSi->soldierslot ])[i]->data.sObjectFlag |= TAKEN_BY_MILITIA;

		if ( pWorldItem[ pSi->pos ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN )
			(pp->Inv[ pSi->soldierslot ])[0]->data.sObjectFlag |= TAKEN_BY_MILITIA_TABOO_GREEN;
		if ( pWorldItem[ pSi->pos ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE )
			(pp->Inv[ pSi->soldierslot ])[0]->data.sObjectFlag |= TAKEN_BY_MILITIA_TABOO_BLUE;
		
		if ( pWorldItem[ pSi->pos ].object.ubNumberOfObjects < 1 )
		{
			// account for items with invalid gridnos...
			if ( pWorldItem[ pSi->pos ].sGridNo != NOWHERE )
				RemoveItemFromPool(pWorldItem[ pSi->pos ].sGridNo, (pSi->pos), pWorldItem[ pSi->pos ].ubLevel);

			// setting this to false can lead to cases where we 'forget' items without a valid gridno - though I am unsure why.
			//pWorldItem[ pSi->pos ].fExists = FALSE;
		}
	}

	pSi->done = TRUE;
}

typedef std::map<UINT8, UINT32>	AmmoType_BulletCountMap;					// used to count how many bullets of an ammotype we have
typedef std::map<UINT8, AmmoType_BulletCountMap> Calibre_BulletCountMap;	// this map stores a map containing all the different ammotypes and how many bulltes we have for them

struct LauncherHelpStruct {
	LauncherHelpStruct::LauncherHelpStruct() : fNeedsAmmo(TRUE), ammocount(0) {}
	BOOLEAN fNeedsAmmo;		// the launcher isn't singleshot (LAW), it needs ammo
	UINT16	ammocount;		// so much ammo found for this
};

typedef std::map<UINT16, LauncherHelpStruct> LauncherHelpMap;

// how many bullets does this map have? (used for one caliber each)
UINT32 GetTotalCalibreAmmo( AmmoType_BulletCountMap* pAmmoBulletMap )
{
	UINT32 bullets = 0;
	AmmoType_BulletCountMap::iterator ammotype_itend = pAmmoBulletMap->end();
	for( AmmoType_BulletCountMap::iterator ammotype_it = pAmmoBulletMap->begin(); ammotype_it != ammotype_itend; ++ammotype_it )
	{
		bullets += (*ammotype_it).second;
	}

	return bullets;
}

// are there enough bullets for the gun? 
BOOLEAN EnoughBulletsForGun( UINT16 usItem, AmmoType_BulletCountMap* pAmmoBulletMap )
{
	if ( !pAmmoBulletMap || !(Item[usItem].usItemClass & IC_GUN) )
		return FALSE;

	// for now, we are satisfied if we have enough bullets for at least 2 magazines/at least 10 bullets/max 200 bullets. This could be externalised...
	if ( GetTotalCalibreAmmo(pAmmoBulletMap) >= GetNeededTotalAmmo(usItem) )
		return TRUE;

	return FALSE;
}

// spawn fitting ammo for pObj into pp
void SpawnFittingAmmo(SOLDIERCREATE_STRUCT *pp, OBJECTTYPE* pObj, UINT8 ammotype, UINT32 numberofbullets )
{
	if ( !pObj || numberofbullets == 0 || !(Item[pObj->usItem].usItemClass & IC_GUN) )
		return;

	UINT16 magsize = GetMagSize(pObj);
	UINT16 magitem = FindReplacementMagazine( Weapon[ pObj->usItem ].ubCalibre, magsize, ammotype );

	if ( !magsize || !magitem )
		return;

	UINT16 deductedbullets = 0;

	if ( (*pObj)[0]->data.gun.ubGunShotsLeft == 0 )
	{
		(*pObj)[0]->data.gun.ubGunAmmoType = ammotype;
		(*pObj)[0]->data.gun.usGunAmmoItem = magitem;

		deductedbullets = min(magsize, numberofbullets);
		numberofbullets	-= deductedbullets;

		(*pObj)[0]->data.gun.ubGunShotsLeft = deductedbullets;
	}

	while ( numberofbullets > 0 )
	{
		deductedbullets = min(magsize, numberofbullets);
		numberofbullets	-= deductedbullets;

		// create ammo in inventory
		CreateAmmo(magitem, &gTempObject, deductedbullets);
		gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
		PlaceObjectInSoldierCreateStruct( pp, &gTempObject );

		// if we have as many bullets as we wanted, get out of here
		if ( !numberofbullets )
			return;
	}
}

void MoveOneMilitiaEquipmentSet(INT16 sSourceX, INT16 sSourceY, INT16 sTargetX, INT16 sTargetY, INT8 bSoldierClass)
{
	BOOLEAN fReturn					= FALSE;
	UINT32 uiTotalNumberOfRealItems = 0;
	UINT32 uiNumOriginalItems		= 0;
	std::vector<WORLDITEM> pWorldItem;//dnl ch75 271013
	SOLDIERCREATE_STRUCT tmp;
	UINT32 uiCount					= 0;
	INT32 dummygridno				= NOWHERE;

	if ( !gGameExternalOptions.fMilitiaUseSectorInventory )
		return;

	TakeMilitiaEquipmentfromSector(sSourceX, sSourceY, 0, &tmp, bSoldierClass);

	// open target sector inv
	if( ( gWorldSectorX == sTargetX )&&( gWorldSectorY == sTargetY ) && (gbWorldSectorZ == 0 ) )
	{
		uiTotalNumberOfRealItems = guiNumWorldItems;
		pWorldItem = gWorldItems;
	}
	else
	{
		// not loaded, load
		// get total number, visable and invisible
		fReturn = GetNumberOfWorldItemsFromTempItemFile( sTargetX, sTargetY, 0, &( uiTotalNumberOfRealItems ), TRUE );
		Assert( fReturn );

		if( uiTotalNumberOfRealItems > 0 )
		{
			// allocate space for the list
			pWorldItem.resize(uiTotalNumberOfRealItems);//dnl ch75 271013

			// now load into mem
			LoadWorldItemsFromTempItemFile(  sTargetX,  sTargetY, 0, pWorldItem );
		}
	}

	uiNumOriginalItems = uiTotalNumberOfRealItems;

	// we note the last item existing in the inventory (but not ammo, as we delete those). We use this to assess how much we really need to increase the inventory
	UINT32 existingitemsfound = 0;

	for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )				// ... for all items in the world ...
	{
		if( pWorldItem[ uiCount ].fExists )										// ... if item exists ...
		{
			++existingitemsfound;
			dummygridno = pWorldItem[ uiCount ].sGridNo;
		}
	}

	if ( dummygridno == NOWHERE )
	{
		dummygridno = RandomGridNo();

		if ( dummygridno == NOWHERE )
			return;
	}

	/////////////////////////////// ADD ITEMS FROM STRUCT TO SECTOR /////////////////////////////////////////////////////
	// We will now add the items from tmp to the target sector
	// first, determine how many items we added, and add an increased copy of pWorldItem
	UINT16 numnewitems = 0;
	UINT8 invsize = tmp.Inv.size();
	for (UINT8 i = 0; i < invsize; ++i)
	{
		if ( tmp.Inv[ i ].exists() )
			++numnewitems;
	}

	// new total size needed is number of existing items found + number of needed ammo crate objects, but at least uiTotalNumberOfRealItems:
	UINT32 uiNewInvSize = max(uiTotalNumberOfRealItems, existingitemsfound + numnewitems);

	// create a bigger inventory wit big enough size
	std::vector<WORLDITEM> pWorldItem_tmp(uiNewInvSize);//dnl ch75 271013

	// copy over old inventory
	for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )
	{
		pWorldItem_tmp[uiCount] = pWorldItem[uiCount];
	}

	uiCount = 0;

	// now add the items from the struct
	for (UINT8 i = 0; i < invsize; ++i)
	{
		// only add existing items that were taken by militia (it can happen that we generate fitting ammo, do not take that)
		if ( tmp.Inv[ i ].exists() && tmp.Inv[ i ][0]->data.sObjectFlag & TAKEN_BY_MILITIA )
		{
			uiCount =  GetNextFreeIndex( pWorldItem_tmp, uiNewInvSize, uiCount );

			if ( uiCount < 0 )
			{
				// Error! this shouldn't happen!
				return;
			}
			
			pWorldItem_tmp[uiCount].fExists										= TRUE;
			pWorldItem_tmp[uiCount].sGridNo										= dummygridno;
			pWorldItem_tmp[uiCount].ubLevel										= 0;
			pWorldItem_tmp[uiCount].usFlags										= WORLD_ITEM_REACHABLE;
			pWorldItem_tmp[uiCount].bVisible									= 1;
			pWorldItem_tmp[uiCount].bRenderZHeightAboveLevel					= 0;
			pWorldItem_tmp[uiCount].soldierID									= -1;
			pWorldItem_tmp[uiCount].object										= tmp.Inv[ i ];
		}
	}
		
	// use the new map
	uiTotalNumberOfRealItems = uiNewInvSize;
	pWorldItem = pWorldItem_tmp;
	/////////////////////////////// ADD ITEMS FROM STRUCT TO SECTOR /////////////////////////////////////////////////////
		
	// save the changed intentory
	// open sector inv
	if( ( gWorldSectorX == sTargetX )&&( gWorldSectorY == sTargetY ) && (gbWorldSectorZ == 0 ) )
	{
		guiNumWorldItems = uiTotalNumberOfRealItems;
		gWorldItems = pWorldItem;
	}
	else
	{
		//Save the Items to the the file
		SaveWorldItemsToTempItemFile( sTargetX, sTargetY, 0, uiTotalNumberOfRealItems, pWorldItem );
	}
}

void MoveMilitiaEquipment(INT16 sSourceX, INT16 sSourceY, INT16 sTargetX, INT16 sTargetY, UINT8 usElites, UINT8 usRegulars, UINT8 usGreens)
{
	if ( !gGameExternalOptions.fMilitiaUseSectorInventory )
		return;

	// atm there is no class-specific selection, but that might change in the future
	for (UINT8 i = 0; i < usElites; ++i)
	{
		MoveOneMilitiaEquipmentSet( sSourceX, sSourceY, sTargetX, sTargetY, SOLDIER_CLASS_ELITE_MILITIA);
	}

	for (UINT8 i = 0; i < usRegulars; ++i)
	{
		MoveOneMilitiaEquipmentSet( sSourceX, sSourceY, sTargetX, sTargetY, SOLDIER_CLASS_REG_MILITIA);
	}

	for (UINT8 i = 0; i < usGreens; ++i)
	{
		MoveOneMilitiaEquipmentSet( sSourceX, sSourceY, sTargetX, sTargetY, SOLDIER_CLASS_GREEN_MILITIA);
	}
}

// This function loads the sector inventory of a given sector and fills the inventory of a SOLDIERCREATE_STRUCT according to the game settings. The soldier class is a parameter, but is currently not used
// This function is rather lengthy, this is due to the fact that we have to loop over the entire sector inventory mulitple times. Below is a small documentation. If you change anything, please update it - Flugente

// We loop over the inventory 3 times. I'll briefly explain why. After each loop we do a few other things - like taking items or doing further evaluation
// Note that any items we checked have to be reachable AND not be 'taboo' to the militia.

// Preparation:			load correct sector inventory
//						init selection structures
//						decide which item categories to look for
//						delete existing soldier inventory, but account for gear already selected by this function (failsafe)
//						early exit if no items are found in sector at all

// 1st loop:			Count all ammo in ammo and guns, evaluate all armour/face items/hand grenades/melee items/guns/launchers
//						We do not yet evaluate guns - we do that later, when we know what ammo exists
//						We store ammo in a map that also stores caliber and ammotype
//						We store launchers in a structure that stores their item, wether they need ammo, and how much ammo we found
//						We evaluate objects via EvaluateObjForItem(...). If you are unsatisfied with militia selections, try to improve this funtion first

// 1st loop afterwork:	If we are unable to determine a valid GridNo, something is wrong here, abort
//						Definite decision on armour/face items/melee items/hand grenades

// 2nd loop:			Evaluate all guns, taking ammo into account (we know how much ammo there is, as we wrote that into our map)
//						We know know what launchers that need ammo we have here, thus we can test bombs and grenades on wether they are such ammo. Note this in the launcher map

// 2nd loop afterwork:	We now know all launchers and their ammo, we decide which launcher to take
//						Decide on a gun, we can now do that as we know all ammo
//						If we have selected a gun, but do not take ammo from the inventory, spawn ammo in the traditional way

// 3rd loop:			If we ever decide to pick up a gun, we will have done so by now. We will look for fitting ammo in all ammo/guns, spawn correct magazines in our inventory, and remove ammo from found objects
//						As we have decided on which launcher we want, we evaluate all launchers of this type and take if afterwards. We also move enough ammo into our inventory.
//						If we created a new mag from the gun we took, try to add this to an empty inventory slot, if none is found, enlarge the inventory afterwards

// 3rd loop afterwork:	Take launcher
//						Add mag if created and it couldn't be added to empty slot earlier by enlarging the inventory

// Exit:				Save changed inventory
void TakeMilitiaEquipmentfromSector( INT16 sMapX, INT16 sMapY, INT8 sMapZ, SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass )
{
	BOOLEAN fReturn					= FALSE;
	UINT32 uiTotalNumberOfRealItems = 0;
	std::vector<WORLDITEM> pWorldItem;//dnl ch75 271013
	UINT32 uiCount					= 0;
	INT32 dummygridno				= NOWHERE;					// this gridno will be the new position of items we create (ammo crates)
	BOOLEAN fNightTime				= NightTime();	
	ItemSearchStruct si[SI_MAX];								// this struct stores the necessary data on the items we find during our search
	Calibre_BulletCountMap calibrebulletountmap;				// a struct that saves all ammo we will find
	UINT16 numcrates				= 0;
	BOOLEAN fNewMagCreated			= FALSE;
	OBJECTTYPE newAmmoObj;
	BOOLEAN fSearchForAmmo			= TRUE;
	UINT16 usLauncherItem			= 0;
	UINT8 usLauncherAmmoLeftToTake	= 0;
	UINT16 usSelectedGunBulletsNeeded = 0;						// how many bullets do we want for our gun
	UINT16 usSelectedGunBulletCount = 0;						// how many bullets have we already taken for our selected gun
	UINT16 usTabooFlag = 0;
	
	if ( !gGameExternalOptions.fMilitiaUseSectorInventory )
		return;

	// depending on our bSoldierClass, we may be forbidden from taking items the player has flagged
	// note that in the current implmentation, items with the WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN-flag will have the higher flags as well, but that might change in the future
	if ( bSoldierClass == SOLDIER_CLASS_ELITE_MILITIA )
		usTabooFlag = WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ELITE;
	else if ( bSoldierClass == SOLDIER_CLASS_REG_MILITIA )
		usTabooFlag = WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE;
	else
		usTabooFlag = WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN;

	///////////////////////////////// Preparation /////////////////////////////////////////////////////////

	// Preparation:			load correct sector inventory
	//						init selection structures
	//						decide which item categories to look for
	//						delete existing soldier inventory, but account for gear already selected by this function (failsafe)
	//						early exit if no items are found in sector at all
		
	// open sector inv
	if( ( gWorldSectorX == sMapX )&&( gWorldSectorY == sMapY ) && (gbWorldSectorZ == sMapZ ) )
	{
		uiTotalNumberOfRealItems = guiNumWorldItems;
		pWorldItem = gWorldItems;
	}
	else
	{
		// not loaded, load
		// get total number, visable and invisible
		fReturn = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, ( INT8 )( sMapZ ), &( uiTotalNumberOfRealItems ), FALSE );
		Assert( fReturn );

		if( uiTotalNumberOfRealItems > 0 )
		{
			// allocate space for the list
			pWorldItem.resize(uiTotalNumberOfRealItems);//dnl ch75 271013

			if ( !uiTotalNumberOfRealItems )
				return;

			// now load into mem
			LoadWorldItemsFromTempItemFile(  sMapX,  sMapY, ( INT8 ) ( sMapZ ), pWorldItem );
		}
	}

	// this vector isn't used anywhere - it simply allows us to easily view the contents of the inventory in debug mode
#ifdef JA2TESTVERSION
	std::vector<std::pair<BOOLEAN, UINT16> > overviewvector;
	for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )
	{
		overviewvector.push_back( std::pair<BOOLEAN, UINT16>(pWorldItem[ uiCount ].fExists, pWorldItem[ uiCount ].object.usItem) );
	}
#endif
		
	// initialize soldier slots
	si[SI_HELMET].soldierslot			= HELMETPOS;
	si[SI_VEST].soldierslot				= VESTPOS;
	si[SI_LEGS].soldierslot				= LEGPOS;
	si[SI_SIGHT].soldierslot			= HEAD1POS;
	si[SI_FACE2].soldierslot			= HEAD2POS;
	si[SI_FACE_SPARESIGHT].soldierslot	= MEDPOCK1POS;
	si[SI_GASMASK].soldierslot			= MEDPOCK2POS;
	si[SI_MELEE].soldierslot			= BIGPOCK1POS;
	si[SI_GRENADE].soldierslot			= BIGPOCK2POS;
	si[SI_GUN].soldierslot				= HANDPOS;
	si[SI_LAUNCHER].soldierslot			= BIGPOCK3POS;

	// depending on gamesettings, we only have to search for some items
	si[SI_HELMET].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Armour;
	si[SI_VEST].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Armour;
	si[SI_LEGS].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Armour;
	si[SI_SIGHT].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Face;
	si[SI_FACE2].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Face;
	si[SI_FACE_SPARESIGHT].done	= !gGameExternalOptions.fMilitiaUseSectorInventory_Face;
	si[SI_GASMASK].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Face;
	si[SI_MELEE].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Melee;
	si[SI_GRENADE].done			= !gGameExternalOptions.fMilitiaUseSectorInventory_Grenade;
	si[SI_GUN].done				= !gGameExternalOptions.fMilitiaUseSectorInventory_Gun;
	si[SI_LAUNCHER].done		= !gGameExternalOptions.fMilitiaUseSectorInventory_Launcher;
	fSearchForAmmo				= (gGameExternalOptions.fMilitiaUseSectorInventory_Gun && gGameExternalOptions.fMilitiaUseSectorInventory_Ammo);	// we only search for ammo if we also search for guns

	LauncherHelpMap launcherhelpmap;
		
	// erase existing soldier inventory, if any
	UINT8 invsize = pp->Inv.size();
	for ( UINT8 i = 0; i < invsize; ++i )
	{ 
		//clear items, but only if they have write status.
		if( !(pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE) )
		{
			if ( Item[pp->Inv[ i ].usItem].usItemClass & IC_ARMOUR && gGameExternalOptions.fMilitiaUseSectorInventory_Armour )
			{
				if ( pp->Inv[ i ][0]->data.sObjectFlag & TAKEN_BY_MILITIA )
					si[SI_HELMET].done = si[SI_VEST].done = si[SI_LEGS].done = TRUE;
				else
					DeleteObj(&pp->Inv[ i ]);
			}
			else if ( Item[pp->Inv[ i ].usItem].usItemClass & IC_FACE && gGameExternalOptions.fMilitiaUseSectorInventory_Face )
			{
				if ( pp->Inv[ i ][0]->data.sObjectFlag & TAKEN_BY_MILITIA )
					si[SI_SIGHT].done = si[SI_FACE2].done = si[SI_GASMASK].done = si[SI_FACE_SPARESIGHT].done = TRUE;
				else
					DeleteObj(&pp->Inv[ i ]);
			}			
			else if ( Item[pp->Inv[ i ].usItem].usItemClass & (IC_BLADE|IC_PUNCH) && gGameExternalOptions.fMilitiaUseSectorInventory_Melee )
			{
				if ( pp->Inv[ i ][0]->data.sObjectFlag & TAKEN_BY_MILITIA )
					si[SI_MELEE].done = TRUE;
				else
					DeleteObj(&pp->Inv[ i ]);
			}
			else if ( Item[pp->Inv[ i ].usItem].usItemClass & IC_GRENADE && gGameExternalOptions.fMilitiaUseSectorInventory_Grenade )
			{
				if ( pp->Inv[ i ][0]->data.sObjectFlag & TAKEN_BY_MILITIA )
					si[SI_GRENADE].done = TRUE;
				else
					DeleteObj(&pp->Inv[ i ]);
			}
			else if ( Item[pp->Inv[ i ].usItem].usItemClass & (IC_GUN|IC_AMMO) && gGameExternalOptions.fMilitiaUseSectorInventory_Gun )
			{
				if ( pp->Inv[ i ][0]->data.sObjectFlag & TAKEN_BY_MILITIA )
				{
					si[SI_GUN].done = TRUE;
					fSearchForAmmo = FALSE;
				}
				else
					DeleteObj(&pp->Inv[ i ]);
			}
			else if ( Item[pp->Inv[ i ].usItem].usItemClass & IC_LAUNCHER && gGameExternalOptions.fMilitiaUseSectorInventory_Launcher )
			{
				if ( pp->Inv[ i ][0]->data.sObjectFlag & TAKEN_BY_MILITIA )
					si[SI_LAUNCHER].done = TRUE;
				else
					DeleteObj(&pp->Inv[ i ]);
			}
		}
	}

	if ( uiTotalNumberOfRealItems == 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Militia found no items to equip, uses harsh langugage instead!" );
		return;
	}

	///////////////////////////////// Preparation /////////////////////////////////////////////////////////
			
	///////////////////////////////// 1st loop /////////////////////////////////////////////////////////

	// 1st loop:			Count all ammo in ammo and guns, evaluate all armour/face items/hand grenades/melee items/guns/launchers
	//						We do not yet evaluate guns - we do that later, when we know what ammo exists
	//						We store ammo in a map that also stores caliber and ammotype
	//						We store launchers in a structure that stores their item, wether they need ammo, and how much ammo we found
	//						We evaluate objects via EvaluateObjForItem(...). If you are unsatisgied with militia selections, try to improve this funtion first

	for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )				// ... for all items in the world ...
	{
		if( pWorldItem[ uiCount ].fExists )										// ... if item exists ...
		{
			dummygridno = pWorldItem[ uiCount ].sGridNo;

			OBJECTTYPE* pObj = &(pWorldItem[ uiCount ].object);			// ... get pointer for this item ...

			if ( pObj != NULL && pObj->exists() )												// ... if pointer is not obviously useless ...
			{
				// this would be the place where we check wether the militia is allowed to pick up an item depending on its soldierclass

				// test wether item is reachable and useable by militia
				if ( (pWorldItem[ uiCount ].usFlags & WORLD_ITEM_REACHABLE) && !(pWorldItem[ uiCount ].usFlags & usTabooFlag) )
				{
					// armour
					if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_ARMOUR && (!si[SI_HELMET].done || !si[SI_VEST].done || !si[SI_LEGS].done ) )
					{
						switch( Armour[ Item[ pWorldItem[ uiCount ].object.usItem ].ubClassIndex ].ubArmourClass )
						{
						case( ARMOURCLASS_HELMET ):
								EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_HELMET] );
							break;
						case( ARMOURCLASS_VEST ):
								EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_VEST] );
							break;
						case( ARMOURCLASS_LEGGINGS ):
								EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_LEGS] );
							break;
						}
					}
					// face gear
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_FACE && (!si[SI_SIGHT].done || !si[SI_FACE2].done || !si[SI_FACE_SPARESIGHT].done || !si[SI_GASMASK].done) )
					{
						// gasmasks are reserved for a special slot and will only be worn if we do not have 2 face items. items that increase our vision (NVGs adn sungooggles) get to slot 1, everything else in 2
						if ( Item[ pWorldItem[ uiCount ].object.usItem ].gasmask )
							EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_GASMASK] );
						else if ( Item[ pWorldItem[ uiCount ].object.usItem ].nightvisionrangebonus > 0 )
						{
							if ( fNightTime )
								EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_SIGHT] );
							else
								EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_FACE_SPARESIGHT] );
						}
						else if ( Item[ pWorldItem[ uiCount ].object.usItem ].dayvisionrangebonus > 0 )
						{
							if ( !fNightTime )
								EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_SIGHT] );
							else
								EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_FACE_SPARESIGHT] );
						}
						else 
							EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_FACE2] );
						
					}
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & (IC_BLADE|IC_PUNCH) && !si[SI_MELEE].done )
					{
						EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_MELEE] );
					}
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_GRENADE && !si[SI_GRENADE].done )
					{
						// if this item can be thrown, its a hand grenade, thus we will evaluate it
						if ( Item[ pWorldItem[ uiCount ].object.usItem ].ubCursor == TOSSCURS )
							EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_GRENADE] );
					}
					// ammo
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_AMMO && fSearchForAmmo )
					{
						UINT16 usItem = pWorldItem[ uiCount ].object.usItem;
						UINT16 usMagIndex = Item[usItem].ubClassIndex;

						// we count ammo and then remove it. We will later add it again in the form of ammo crates
						UINT8 calibre = Magazine[usMagIndex].ubCalibre;
						UINT8 ammotype = Magazine[usMagIndex].ubAmmoType;
						UINT32 newammo = 0;
						for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)	
						{
							newammo += (*pObj)[i]->data.ubShotsLeft;
						}

						// add ammo to our map
						if ( calibrebulletountmap[calibre][ammotype] )
							calibrebulletountmap[calibre][ammotype] += newammo;
						else
							calibrebulletountmap[calibre][ammotype] = newammo;
					}
					// gun
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_GUN && !si[SI_GUN].done )
					{
						// we count ammo and then remove it. We will later add it again in the form of ammo crates
						UINT8 calibre = Weapon[ pWorldItem[ uiCount ].object.usItem ].ubCalibre;
						// guns in a stack can have different ammo, thus he need to add it individually
						for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)	
						{
							UINT8 ammotype = (*pObj)[i]->data.gun.ubGunAmmoType;

							if ( (*pObj)[i]->data.gun.ubGunShotsLeft )
							{
								// add ammo to our map
								if ( calibrebulletountmap[calibre][ammotype] )
									calibrebulletountmap[calibre][ammotype] += (*pObj)[i]->data.gun.ubGunShotsLeft;
								else
									calibrebulletountmap[calibre][ammotype] = (*pObj)[i]->data.gun.ubGunShotsLeft;
							}
						}												
					}
					// launcher
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_LAUNCHER && !si[SI_LAUNCHER].done )
					{
						// add launcher type to our launcher map
						LauncherHelpMap::iterator fnd = launcherhelpmap.find( pWorldItem[ uiCount ].object.usItem );
						if ( fnd == launcherhelpmap.end() )
						{
							LauncherHelpStruct tmp;
							tmp.fNeedsAmmo = !Item[pWorldItem[ uiCount ].object.usItem].singleshotrocketlauncher;
							launcherhelpmap[ pWorldItem[ uiCount ].object.usItem ] = tmp;
						}
					}
				}
			}
		}
	}

	///////////////////////////////// 1st loop /////////////////////////////////////////////////////////

	///////////////////////////////// 1st loop afterwork /////////////////////////////////////////////////////////

	// 1st loop afterwork:	If we are unable to determine a valid GridNo, soemthing is wrong here, abort
	//						Definite decision on armour/face items/melee items/hand grenades
				
	// if the dummy GridNo is still NOWHERE, we did not find a single reachable item. If getting a random gridno also fails, better get out of here
	if ( dummygridno == NOWHERE )
	{
		dummygridno = RandomGridNo();

		if ( dummygridno == NOWHERE )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Militia found no items to equip, uses harsh langugage instead!" );
			return;
		}
	}

	SearchItemRetrieval( pWorldItem, &si[SI_HELMET], pp );
	SearchItemRetrieval( pWorldItem, &si[SI_VEST], pp );
	SearchItemRetrieval( pWorldItem, &si[SI_LEGS], pp );
	SearchItemRetrieval( pWorldItem, &si[SI_SIGHT], pp );
	SearchItemRetrieval( pWorldItem, &si[SI_FACE2], pp );
	SearchItemRetrieval( pWorldItem, &si[SI_FACE_SPARESIGHT], pp );

	// special: of there is a free face slot and we have a gasmask, equip it there
	if ( si[SI_GASMASK].found )
	{
		if ( !si[SI_SIGHT].found )
			si[SI_GASMASK].soldierslot = si[SI_SIGHT].soldierslot;
		else if ( !si[SI_FACE2].found )
			si[SI_GASMASK].soldierslot = si[SI_FACE2].soldierslot;
	}

	SearchItemRetrieval( pWorldItem, &si[SI_GASMASK], pp );
	SearchItemRetrieval( pWorldItem, &si[SI_MELEE], pp );
	SearchItemRetrieval( pWorldItem, &si[SI_GRENADE], pp, 2 );	// if on a stack, we are willing to take 2 grenades at once

	///////////////////////////////// 1st loop afterwork /////////////////////////////////////////////////////////

	///////////////////////////////// 2nd loop /////////////////////////////////////////////////////////

	// Second loop:		Evaluate all guns, taking ammo into account (we know how much ammo there is, as we wrote that into our map)
	//					We know know what launchers that need ammo we have here, thus we can test bombs and grenades on wether they are such ammo. Note this in the launcher map

	for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )				// ... for all items in the world ...
	{
		if( pWorldItem[ uiCount ].fExists )										// ... if item exists ...
		{
			OBJECTTYPE* pObj = &(pWorldItem[ uiCount ].object);			// ... get pointer for this item ...

			if ( pObj != NULL && pObj->exists() )												// ... if pointer is not obviously useless ...
			{
				// this would be the place where we check wether the militia is allowed to pick up an item depending on its soldierclass

				// test wether item is reachable 
				if ( (pWorldItem[ uiCount ].usFlags & WORLD_ITEM_REACHABLE) && !(pWorldItem[ uiCount ].usFlags & usTabooFlag) )
				{
					// only if we are still looking for a gun
					if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_GUN && !si[SI_GUN].done )
					{
						UINT8 calibre = Weapon[ pWorldItem[ uiCount ].object.usItem ].ubCalibre;

						Calibre_BulletCountMap::iterator calibremap_it = calibrebulletountmap.find( calibre );

						if ( fSearchForAmmo )
						{
							// only evaluate of ammo is found!
							if ( calibremap_it != calibrebulletountmap.end() )
							{
								UINT32 bullets = GetTotalCalibreAmmo( &((*calibremap_it).second) );

								EvaluateObjForItem_WithAmmo( pWorldItem, pObj, uiCount, &si[SI_GUN], bullets );
							}
						}
						else
						{
							EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_GUN] );
						}
					}
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & (IC_GRENADE|IC_BOMB) && !si[SI_LAUNCHER].done )
					{
						// if this is a valid launchable for any launcher we found, update their potential ammocount
						LauncherHelpMap::iterator itend = launcherhelpmap.end();
						for (LauncherHelpMap::iterator it = launcherhelpmap.begin(); it != itend; ++it)
						{
							if ( (*it).second.fNeedsAmmo && ValidLaunchable( pWorldItem[ uiCount ].object.usItem, (*it).first ) )
								(*it).second.ammocount += pWorldItem[ uiCount ].object.ubNumberOfObjects;
						}
					}
				}
			}			
		}
	}

	///////////////////////////////// 2nd loop /////////////////////////////////////////////////////////

	///////////////////////////////// 2nd loop afterwork /////////////////////////////////////////////////////////

	// 2nd loop afterwork:	We now know all launchers and their ammo, we decide which launcher to take
	//						Decide on a gun, we can now do that as we know all ammo
	//						If we have selected a gun, but do not take ammo from the inventory, spawn ammo in the traditional way

	// we investigate the launchers we found, and their ammo.
	if ( gGameExternalOptions.fMilitiaUseSectorInventory_Launcher && !si[SI_LAUNCHER].done )
	{
		LauncherHelpMap::iterator itend = launcherhelpmap.end();
		for (LauncherHelpMap::iterator it = launcherhelpmap.begin(); it != itend; ++it)
		{
			// we pick the first launcher that needs no ammo or has enough ammo
			if ( !(*it).second.fNeedsAmmo || (*it).second.ammocount > 3 )
			{
				usLauncherItem = (*it).first;
				usLauncherAmmoLeftToTake = (*it).second.ammocount ? 3 : 0;
				break;
			}
		}
	}
		
	// now that we know which gun to take, do so
	// if found a gun, check wether we have enough ammo for it
	if ( !si[SI_GUN].done )
	{
		if ( si[SI_GUN].found )
		{
			UINT8 calibre = Weapon[ pWorldItem[ si[SI_GUN].pos ].object.usItem ].ubCalibre;

			Calibre_BulletCountMap::iterator calibremap_it = calibrebulletountmap.find( calibre );
			if ( !fSearchForAmmo || calibremap_it != calibrebulletountmap.end() )
			{
				// do we have enough ammo form that calibre?
				if ( !fSearchForAmmo || EnoughBulletsForGun( pWorldItem[ si[SI_GUN].pos ].object.usItem, &((*calibremap_it).second) ) )
				{
					usSelectedGunBulletsNeeded = fSearchForAmmo ? GetNeededTotalAmmo( pWorldItem[ si[SI_GUN].pos ].object.usItem ) : 0;
					usSelectedGunBulletCount   = fSearchForAmmo ? pWorldItem[ si[SI_GUN].pos ].object[0]->data.gun.ubGunShotsLeft : 0;

					// take this gun
					SearchItemRetrieval( pWorldItem, &si[SI_GUN], pp );

					// empty the gun, create a mag
					if ( !fSearchForAmmo )
					{
						UINT16 bulletstomove = (pp->Inv[ HANDPOS ])[0]->data.gun.ubGunShotsLeft;
						if ( bulletstomove )
						{
							(pp->Inv[ HANDPOS ])[0]->data.gun.ubGunShotsLeft = 0;
							UINT8 ammotype = (pp->Inv[ HANDPOS ])[0]->data.gun.ubGunAmmoType;
							UINT16 magsize = GetMagSize( &(pp->Inv[ HANDPOS ] ) );
							UINT16 magitem = FindReplacementMagazine( Weapon[ pp->Inv[ HANDPOS ].usItem ].ubCalibre, magsize, ammotype );
											
							// create ammo in inventory
							CreateAmmo(magitem, &newAmmoObj, bulletstomove);
							fNewMagCreated = TRUE;
						}
					}
				}
				else
				{
					usSelectedGunBulletsNeeded = fSearchForAmmo ? GetNeededTotalAmmo( pWorldItem[ si[SI_GUN].pos ].object.usItem ) : 0;
					usSelectedGunBulletCount   = fSearchForAmmo ? pWorldItem[ si[SI_GUN].pos ].object[0]->data.gun.ubGunShotsLeft : 0;

					// take this gun
					SearchItemRetrieval( pWorldItem, &si[SI_GUN], pp );

					// empty the gun, create a mag
					if ( !fSearchForAmmo )
					{
						UINT16 bulletstomove = (pp->Inv[ HANDPOS ])[0]->data.gun.ubGunShotsLeft;
						if ( bulletstomove )
						{
							(pp->Inv[ HANDPOS ])[0]->data.gun.ubGunShotsLeft = 0;
							UINT8 ammotype = (pp->Inv[ HANDPOS ])[0]->data.gun.ubGunAmmoType;
							UINT16 magsize = GetMagSize( &(pp->Inv[ HANDPOS ] ) );
							UINT16 magitem = FindReplacementMagazine( Weapon[ pp->Inv[ HANDPOS ].usItem ].ubCalibre, magsize, ammotype );
											
							// create ammo in inventory
							CreateAmmo(magitem, &newAmmoObj, bulletstomove);
							fNewMagCreated = TRUE;
						}
					}
				}
			}
			// no ammo at all... this is bad
			else
			{
				si[SI_GUN].done = TRUE;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Militia found no gun to equip, uses harsh langugage instead!" );
			}
		}
		// we didn't find any gun at all. Now what?
		else
		{
			si[SI_GUN].done = TRUE;
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Militia found no gun to equip, uses harsh langugage instead!" );
		}
	}
		 
	// if we selected a gun but do not select ammo, we have to spawn fitting ammo the traditional way
	if ( gGameExternalOptions.fMilitiaUseSectorInventory_Gun && !gGameExternalOptions.fMilitiaUseSectorInventory_Ammo)
	{
		UINT16 usGunIndex = pp->Inv[ HANDPOS ].usItem;
		if ( usGunIndex )
		{
			// check default ammo first...
			UINT16 usAmmoIndex = DefaultMagazine( usGunIndex );
			UINT8 ubChanceStandardAmmo = 100;
			if ( AmmoTypes[Magazine[ Item[ usAmmoIndex ].ubClassIndex ].ubAmmoType].standardIssue )
				ubChanceStandardAmmo = 80;
			else
				ubChanceStandardAmmo = 100 - (Random(9) * 9);

			usAmmoIndex = RandomMagazine( &pp->Inv[HANDPOS], ubChanceStandardAmmo, max(Item[usGunIndex].ubCoolness, HighestPlayerProgressPercentage() / 10 + 3 ), pp->ubSoldierClass);

			if ( usAmmoIndex <= 0 )
				usAmmoIndex = DefaultMagazine(usGunIndex);

			//Madd: ensure a minimum # of bullets to make sure enemies don't run out and run away
			UINT8 numberofmags = 3 + Random(2);
			while ( ( (numberofmags - 1) * Weapon[usGunIndex].ubMagSize ) < 20 ) // each soldier should have at least 20 bullets, ie: 2 9mm 15rd clips, 3 7rd shotgun shells, 4 6rd speedloaders, etc
				++numberofmags;

			CreateItems( usAmmoIndex, 100, numberofmags, &gTempObject );
			gTempObject.fFlags |= OBJECT_UNDROPPABLE;
			PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		}
	}

	///////////////////////////////// 2nd loop afterwork /////////////////////////////////////////////////////////

	///////////////////////////////// 3rd loop /////////////////////////////////////////////////////////

	// Third loop:		If we ever decide to pick up a gun, we will have done so now. We will look for fitting ammo in all ammo/guns, spawn correct magazines in our inventory, and remove ammo from found objects
	//					As we have decided on which launcher we want, we evaluate all launchers of this type and take if afterwards. We also move enough ammo into our inventory.
	//					If we created a new mag from the gun we took, try to add this to an empty invetory slot, if none is found, enlarge the inventory afterwards

	// 3rd loop: take ammo and grenade launcher grenades
	for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )				// ... for all items in the world ...
	{
		if( pWorldItem[ uiCount ].fExists )										// ... if item exists ...
		{
			OBJECTTYPE* pObj = &(pWorldItem[ uiCount ].object);			// ... get pointer for this item ...

			if ( pObj != NULL && pObj->exists() )												// ... if pointer is not obviously useless ...
			{
				// test wether item is reachable 
				if ( (pWorldItem[ uiCount ].usFlags & WORLD_ITEM_REACHABLE) && !(pWorldItem[ uiCount ].usFlags & usTabooFlag) )
				{
					// look for fitting ammo
					if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_GUN && fSearchForAmmo && usSelectedGunBulletCount < usSelectedGunBulletsNeeded )
					{
						if ( Weapon[ pWorldItem[ uiCount ].object.usItem ].ubCalibre == Weapon[ pp->Inv[ HANDPOS ].usItem ].ubCalibre )
						{
							// guns in a stack can have different ammo, thus we need to add it individually
							for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)	
							{
								UINT8 ammotype = (*pObj)[i]->data.gun.ubGunAmmoType;
								
								UINT32 addammo = min(usSelectedGunBulletsNeeded - usSelectedGunBulletCount, (*pObj)[i]->data.gun.ubGunShotsLeft);

								SpawnFittingAmmo( pp, &(pp->Inv[ HANDPOS ]), ammotype, addammo );

								(*pObj)[i]->data.gun.ubGunShotsLeft -= addammo;
								usSelectedGunBulletCount += addammo;
							}
						}
					}
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_AMMO && fSearchForAmmo && usSelectedGunBulletCount < usSelectedGunBulletsNeeded )
					{
						UINT16 usItem = pWorldItem[ uiCount ].object.usItem;
						UINT16 usMagIndex = Item[usItem].ubClassIndex;
						UINT8 calibre = Magazine[usMagIndex].ubCalibre;
						
						// if caliber fits, add ammo to inventory, remove bullets and update bullet counter
						if ( calibre == Weapon[ pp->Inv[ HANDPOS ].usItem ].ubCalibre )
						{
							UINT8 ammotype = Magazine[usMagIndex].ubAmmoType;

							UINT32 newammo = 0;
							for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)
								newammo += (*pObj)[i]->data.ubShotsLeft;

							UINT32 addammo = min((UINT32)(usSelectedGunBulletsNeeded - usSelectedGunBulletCount), newammo);//dnl ch75 271013

							SpawnFittingAmmo( pp, &(pp->Inv[ HANDPOS ]), ammotype, addammo );

							usSelectedGunBulletCount += addammo;

							// remove ammo from stack
							for(INT16 i = 0; i < pObj->ubNumberOfObjects; ++i)
							{
								if ( !addammo )
									break;

								UINT32 takeammo = min(addammo, (*pObj)[i]->data.ubShotsLeft);
								addammo -= takeammo;
								(*pObj)[i]->data.ubShotsLeft -= takeammo;

								if ( !(*pObj)[i]->data.ubShotsLeft )
								{
									pWorldItem[uiCount].object.RemoveObjectAtIndex( i );

									// we removed an item from the stack - set back the loop variable so we don't miss an object
									--i;
								}
							}
							
							if ( pWorldItem[ uiCount ].object.ubNumberOfObjects < 1 )
							{
								if ( pWorldItem[ uiCount ].sGridNo != NOWHERE )
									RemoveItemFromPool(pWorldItem[ uiCount ].sGridNo, uiCount, pWorldItem[ uiCount ].ubLevel);

								pWorldItem[uiCount].fExists = FALSE;
							}
						}
					}
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & IC_LAUNCHER && !si[SI_LAUNCHER].done )
					{
						// if launcher is of the type we want, evaluate it
						if ( pWorldItem[ uiCount ].object.usItem == usLauncherItem )
						{
							EvaluateObjForItem( pWorldItem, pObj, uiCount, &si[SI_LAUNCHER] );
						}
					}
					else if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass & (IC_GRENADE|IC_BOMB) && !si[SI_LAUNCHER].done && usLauncherAmmoLeftToTake )
					{
						// if launcher is of the type we want, evaluate it
						if ( ValidLaunchable(pWorldItem[ uiCount ].object.usItem, usLauncherItem) )
						{
							// take item and reduce amount left to take
							UINT8 totake = min(usLauncherAmmoLeftToTake, pWorldItem[ uiCount ].object.ubNumberOfObjects);
							usLauncherAmmoLeftToTake = max(0, usLauncherAmmoLeftToTake - totake);
							
							pWorldItem[ uiCount ].object.MoveThisObjectTo(gTempObject, totake);

							gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;

							PlaceObjectInSoldierCreateStruct( pp, &gTempObject );
		
							if ( pWorldItem[ uiCount ].object.ubNumberOfObjects < 1 )
							{
								if ( pWorldItem[ uiCount ].sGridNo != NOWHERE )
									RemoveItemFromPool(pWorldItem[ uiCount ].sGridNo, uiCount, pWorldItem[ uiCount ].ubLevel);

								pWorldItem[uiCount].fExists = FALSE;
							}
						}
					}
				}
			}
			// found an empty slot - we can put the magazine we created here
			else if ( fNewMagCreated  )
			{
				pWorldItem[uiCount].fExists										= TRUE;
				pWorldItem[uiCount].sGridNo										= dummygridno;
				pWorldItem[uiCount].ubLevel										= 0;
				pWorldItem[uiCount].usFlags										= WORLD_ITEM_REACHABLE;
				pWorldItem[uiCount].bVisible									= 1;
				pWorldItem[uiCount].bRenderZHeightAboveLevel					= 0;
				pWorldItem[uiCount].soldierID									= -1;
				pWorldItem[uiCount].object										= newAmmoObj;

				fNewMagCreated = FALSE;
			}
		}
	}

	///////////////////////////////// 3rd loop /////////////////////////////////////////////////////////

	///////////////////////////////// 3rd loop afterwork /////////////////////////////////////////////////////////

	// 3rd loop afterwork:	Take launcher
	//						Add mag if created adn it couldn't be added to empty slot earlier by enlarging the inventory

	SearchItemRetrieval( pWorldItem, &si[SI_LAUNCHER], pp );	

	// it can happen that we create a new mag item (that happens if we search for guns but not ammo, and the gun we selected has ammo)
	// if we didn't find an empty slot earlier, we still have to add the item, so we increase the sector inventory size by 1 to make room
	if ( fNewMagCreated )
	{
		// new total size needed is number of existing items found + number of needed ammo crate objects, but at least uiTotalNumberOfRealItems:
		UINT32 uiNewInvSize = uiTotalNumberOfRealItems + 1;

		// create a bigger inventory wit big enough size
		std::vector<WORLDITEM> pWorldItem_tmp(uiNewInvSize);//dnl ch75 271013

		// copy over old inventory
		for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )
		{
			pWorldItem_tmp[uiCount] = pWorldItem[uiCount];
		}

		pWorldItem_tmp[uiCount].fExists										= TRUE;
		pWorldItem_tmp[uiCount].sGridNo										= dummygridno;
		pWorldItem_tmp[uiCount].ubLevel										= 0;
		pWorldItem_tmp[uiCount].usFlags										= WORLD_ITEM_REACHABLE;
		pWorldItem_tmp[uiCount].bVisible									= 1;
		pWorldItem_tmp[uiCount].bRenderZHeightAboveLevel					= 0;
		pWorldItem_tmp[uiCount].soldierID									= -1;
		pWorldItem_tmp[uiCount].object										= newAmmoObj;

		fNewMagCreated = FALSE;

		// use the new map
		uiTotalNumberOfRealItems = uiNewInvSize;
		pWorldItem = pWorldItem_tmp;
	}
	///////////////////////////////// 3rd loop afterwork /////////////////////////////////////////////////////////

	///////////////////////////////// Exit /////////////////////////////////////////////////////////

	// Exit:				Save changed inventory

#ifdef JA2TESTVERSION
	std::vector<std::pair<BOOLEAN, UINT16> > overviewvector2;
	for( uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )
	{
		overviewvector2.push_back( std::pair<BOOLEAN, UINT16>(pWorldItem[ uiCount ].fExists, pWorldItem[ uiCount ].object.usItem) );
	}
#endif

	// save the changed inventory
	// open sector inv
	if( ( sMapX == gWorldSectorX )&&( gWorldSectorY == sMapY ) && (gbWorldSectorZ == sMapZ ) )
	{
		guiNumWorldItems = uiTotalNumberOfRealItems;
		gWorldItems = pWorldItem;
	}
	else
	{
		//Save the Items to the the file
		SaveWorldItemsToTempItemFile( sMapX, sMapY, (INT8)sMapZ, uiTotalNumberOfRealItems, pWorldItem );
	}

	///////////////////////////////// Exit /////////////////////////////////////////////////////////
}

////////////////// Flugente: militia equipment feature ///////////////////////////////////
