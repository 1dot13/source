#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "GameSettings.h"
#else
	#include "laptop.h"
	#include "CharProfile.h"
	#include "Utilities.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "GameSettings.h"
	#include "IMPVideoObjects.h"
	#include "IMP MainPage.h"
	#include "IMP Personality Entrance.h"
	#include "IMP Skill Trait.h"
	#include "IMP Attribute Selection.h"
	#include "IMP Portraits.h"
	#include "IMP Compile Character.h"
	#include "Soldier Profile Type.h"
	#include "Soldier Profile.h"
	#include "Animation Data.h"
	#include "random.h"
	#include "LaptopSave.h"
	// These 4 added - SANDRO
	#include "IMP Character Trait.h"
	#include "IMP Disability Trait.h"
	#include "IMP Color Choosing.h"
	#include "IMP Minor Trait.h"
#endif

#include "IMP Confirm.h"

// how many times should a 'die' be rolled for skills of the same type?

#define HOW_MANY_ROLLS_FOR_SAME_SKILL_CHECK 20

INT32 AttitudeList[ ATTITUDE_LIST_SIZE ];
INT32 iLastElementInAttitudeList = 0;

INT32 SkillsList[ ATTITUDE_LIST_SIZE ];
INT32 BackupSkillsList[ ATTITUDE_LIST_SIZE ];
INT32 iLastElementInSkillsList = 0;

INT32 PersonalityList[ ATTITUDE_LIST_SIZE ];
INT32 iLastElementInPersonalityList = 0;

extern BOOLEAN fLoadingCharacterForPreviousImpProfile;

// positions of the face x and y for eyes and mouth for the 10 portraits

INT16 sFacePositions[ NUMBER_OF_PLAYER_PORTRAITS ][ 4 ];
/*{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
};

STR8 pPlayerSelectedFaceFileNames[ NUMBER_OF_PLAYER_PORTRAITS ]=
{
	"Faces\\200.sti",
	"Faces\\201.sti",
	"Faces\\202.sti",
	"Faces\\203.sti",
	"Faces\\204.sti",
	"Faces\\205.sti",
	"Faces\\206.sti",
	"Faces\\207.sti",
	"Faces\\208.sti",
	"Faces\\209.sti",
	"Faces\\210.sti",
	"Faces\\211.sti",
	"Faces\\212.sti",
	"Faces\\213.sti",
	"Faces\\214.sti",
	"Faces\\215.sti",
};

STR8 pPlayerSelectedBigFaceFileNames[ NUMBER_OF_PLAYER_PORTRAITS ]=
{
	"Faces\\BigFaces\\200.sti",
	"Faces\\BigFaces\\201.sti",
	"Faces\\BigFaces\\202.sti",
	"Faces\\BigFaces\\203.sti",
	"Faces\\BigFaces\\204.sti",
	"Faces\\BigFaces\\205.sti",
	"Faces\\BigFaces\\206.sti",
	"Faces\\BigFaces\\207.sti",
	"Faces\\BigFaces\\208.sti",
	"Faces\\BigFaces\\209.sti",
	"Faces\\BigFaces\\210.sti",
	"Faces\\BigFaces\\211.sti",
	"Faces\\BigFaces\\212.sti",
	"Faces\\BigFaces\\213.sti",
	"Faces\\BigFaces\\214.sti",
	"Faces\\BigFaces\\215.sti",
};
*/

// function declarations
void SelectMercFace( void );
void SetMercSkinAndHairColors( void );
BOOLEAN ShouldThisMercHaveABigBody( void );





void CreateACharacterFromPlayerEnteredStats( void )
{

		// Kaiden: Seems like as good a place as any to stash this function call to
		// ensure that these lists don't get overwritten or Nulled due to the amount
		// of changes and revisions that have been made to personalities and attitudes.
	CreatePlayersPersonalitySkillsAndAttitude();

	// copy over full name
	wcscpy( gMercProfiles[ LaptopSaveInfo.iIMPIndex ].zName, pFullName );

	// the nickname
	wcscpy( gMercProfiles[ LaptopSaveInfo.iIMPIndex ].zNickname, pNickName );

	// gender
	if ( fCharacterIsMale == TRUE )
	{
		// male
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bSex = MALE;
	}
	else
	{
		// female
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bSex = FEMALE;
	}


	// attributes
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bLifeMax		= ( INT8 )iHealth;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bLife		= ( INT8 )iHealth;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bAgility	= ( INT8 )iAgility;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bStrength	= ( INT8 )iStrength;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bDexterity	= ( INT8 )iDexterity;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bWisdom	 = ( INT8 )iWisdom;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bLeadership = ( INT8 )iLeadership;


		// skills
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bMarksmanship = ( INT8 )iMarksmanship;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bMedical		= ( INT8 )iMedical;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bMechanical	= ( INT8 )iMechanical;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bExplosive	= ( INT8 )iExplosives;


	// personality
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bDisability = ( INT8 )iPersonality;

	// attitude
	// SANDRO - decide here, if we use the default attitude or the new so called Character Trait for new traits
	if ( gGameOptions.fNewTraitSystem)
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bCharacterTrait = ( INT8 )iAttitude;
	else
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bAttitude = ( INT8 )iAttitude;

	// WDS: Advanced start 
	//gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bExpLevel = gGameExternalOptions.ubIMPStartingLevel;
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bExpLevel = StartingLevelChosen(); // We now choose the starting level on IMP creation - SANDRO

	// set time away
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bMercStatus = 0;

	// face
	SelectMercFace( );

	//  Option for badass added - SANDRO
	if (bBadAssSelected())
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].uiBodyTypeSubFlags = 1;


	return;

}


BOOLEAN DoesCharacterHaveAnAttitude( void )
{

	// simply checks if caracter has an attitude other than normal
	switch( iAttitude )
	{
		case ATT_LONER:
		case ATT_PESSIMIST:
		case ATT_ARROGANT:
		case ATT_BIG_SHOT:
		case ATT_ASSHOLE:
		case ATT_COWARD:
			return( TRUE );
		default:
			return( FALSE );
	}
}


BOOLEAN DoesCharacterHaveAPersoanlity( void )
{


	if( iPersonality != NO_DISABILITY )
	{
		// yep
	return ( TRUE );
	}
	else
	{
		// nope
		return ( FALSE );
	}


	// Kaiden: Following is the original UB section:
	// The fist return( FALSE ); Statement was the only
	// thing uncommented.
/*
	// only one we can get is PSYCHO, and that is not much of a penalty
	return( FALSE );

	// simply checks if caracter has a personality other than normal
	if( iPersonality != NO_DISABILITY )
	{
		// yep
	return ( TRUE );
	}
	else
	{
		// nope
		return ( FALSE );
	}
	*/


}

void CreatePlayerAttitude( void )
{
	// DLETED UNUSED PART OF THE CODE - SANDRO

	iAttitude =	iChosenCharacterTrait();
}


void AddAnAttitudeToAttitudeList( INT8 bAttitude )
{
	// adds an attitude to attitude list

	if( iLastElementInAttitudeList < ATTITUDE_LIST_SIZE)
	{
		// add element
		AttitudeList[ iLastElementInAttitudeList ] = ( INT32 )bAttitude;

		// increment attitude list counter
		iLastElementInAttitudeList++;
	}

	return;
}


void AddSkillToSkillList( INT8 bSkill )
{
	// adds a skill to skills list

	if( iLastElementInSkillsList < ATTITUDE_LIST_SIZE)
	{
		// add element
		SkillsList[ iLastElementInSkillsList ] = ( INT32 )bSkill;

		// increment attitude list counter
		iLastElementInSkillsList++;
	}

	return;
}

// Kaiden : Added to clear the list when first visiting the IMP homepage,
// Or for each visit there after.
void ClearAllSkillsList( void )
{
	INT32		iLoop;

	for ( iLoop = 0; iLoop < ATTITUDE_LIST_SIZE; iLoop++ ) // SANDRO - better clear everything to be sure
	{
		SkillsList[ iLoop ] = 0;
	}
	iLastElementInSkillsList = 0;
	return;
}



void RemoveSkillFromSkillsList( INT32 iIndex )
{
	INT32		iLoop;

	// remove a skill from the index given and shorten the list
	if ( iIndex < iLastElementInSkillsList )
	{
		memset( BackupSkillsList, 0, ATTITUDE_LIST_SIZE * sizeof( INT32 ) );

		// use the backup array to create a version of the array without
		// this index
		for ( iLoop = 0; iLoop < iIndex; iLoop++ )
		{
			BackupSkillsList[ iLoop ] = SkillsList[ iLoop ];
		}
		for ( iLoop = iIndex + 1; iLoop < iLastElementInSkillsList; iLoop++ )
		{
			BackupSkillsList[ iLoop - 1 ] = SkillsList[ iLoop ];
		}
		// now copy this over to the skills list
		memcpy( SkillsList, BackupSkillsList, ATTITUDE_LIST_SIZE * sizeof( INT32 ) );

		// reduce recorded size by 1
		iLastElementInSkillsList--;
	}
}

INT32	FindSkillInSkillsList( INT32 iSkill )
{
	INT32		iLoop;

	for ( iLoop = 0; iLoop < iLastElementInSkillsList; iLoop++ )
	{
		if ( SkillsList[ iLoop ] == iSkill )
		{
			return( iLoop );
		}
	}

	return( -1 );
}

void ValidateSkillsList( void )
{
	INT32	iIndex;//, iValue;
	MERCPROFILESTRUCT * pProfile;

	// remove from the generated traits list any traits that don't match
	// the character's skills
	pProfile = &(gMercProfiles[ LaptopSaveInfo.iIMPIndex ]);
	if ( pProfile->bMechanical == 0 )
	{
		// without mechanical, electronics is useless
		// Electronics/Technician check - SANDRO
		if ( gGameOptions.fNewTraitSystem )
		{
			iIndex = FindSkillInSkillsList( TECHNICIAN_NT ); 
			if ( iIndex != -1 )
				pProfile->bMechanical = 1;
		}
		else
		{
			iIndex = FindSkillInSkillsList( ELECTRONICS_OT );
			if ( iIndex != -1 )
				pProfile->bMechanical = 1;
			
			iIndex = FindSkillInSkillsList( LOCKPICKING_OT );
			if ( iIndex != -1 )
				pProfile->bMechanical = 1;
		}

	}

	// SANDRO - added to give 1 medical skill to doctors
	if ( pProfile->bMedical == 0 && gGameOptions.fNewTraitSystem )
	{
		// without medical, Doctor trait is useless
		iIndex = FindSkillInSkillsList( DOCTOR_NT ); 
		if ( iIndex != -1 )
			pProfile->bMedical = 1;
	}

	// SANDRO - added to give 1 explosive skill to dmolitions
	if ( pProfile->bExplosive == 0 && gGameOptions.fNewTraitSystem )
	{
		iIndex = FindSkillInSkillsList( DEMOLITIONS_NT ); 
		if ( iIndex != -1 )
			pProfile->bExplosive = 1;
	}

	// SANDRO - added to give 1 Leadership skill to Squadleaders
	if ( pProfile->bLeadership == 0 && gGameOptions.fNewTraitSystem )
	{
		// without medical, Doctor trait is useless
		iIndex = FindSkillInSkillsList( SQUADLEADER_NT ); 
		if ( iIndex != -1 )
			pProfile->bLeadership = 1;
	}

	if ( pProfile->bMarksmanship == 0 )
	{
		// without marksmanship, the following traits are useless:
		// auto weapons, heavy weapons, sniper, ranger, gunslinger

		if ( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO 
		{
			iIndex = FindSkillInSkillsList( AUTO_WEAPONS_NT );
			if( iIndex != -1 )
				pProfile->bMarksmanship = 1;

			iIndex = FindSkillInSkillsList( HEAVY_WEAPONS_NT );
			if( iIndex != -1 )
				pProfile->bMarksmanship = 1;

			iIndex = FindSkillInSkillsList( SNIPER_NT );
			if( iIndex != -1 )
				pProfile->bMarksmanship = 1;

			iIndex = FindSkillInSkillsList( RANGER_NT );
			if( iIndex != -1 )
				pProfile->bMarksmanship = 1;

			iIndex = FindSkillInSkillsList( GUNSLINGER_NT );
			if( iIndex != -1 )
				pProfile->bMarksmanship = 1;
		}
		else
		{
			iIndex = FindSkillInSkillsList( AUTO_WEAPS_OT );
			if( iIndex != -1 )
			{
				//RemoveSkillFromSkillsList( iIndex );
				//iIndex = FindSkillInSkillsList( AUTO_WEAPS_OT );
				pProfile->bMarksmanship = 1;
			}
			iIndex = FindSkillInSkillsList( HEAVY_WEAPS_OT );
			if( iIndex != -1 )
			{
				//RemoveSkillFromSkillsList( iIndex );
				//iIndex = FindSkillInSkillsList( HEAVY_WEAPONS_NT );
				pProfile->bMarksmanship = 1;
			}
		}
	}
}

void CreatePlayerSkills( void )
{
	ValidateSkillsList();

	// Kaiden: This section was added in it's place:
	// SANDRO - Note: this is actually used only for old trait system
	if( iLastElementInSkillsList > 0 )
	{
		iSkillA = SkillsList[ 0 ];
	}
	if( iLastElementInSkillsList > 1 )
	{
		iSkillB = SkillsList[ 1 ];
	}
}


void AddAPersonalityToPersonalityList( INT8 bPersonlity )
{
	// CJC, Oct 26 98: prevent personality list from being generated
	// because no dialogue was written to support PC personality quotes

	// BUT we can manage this for PSYCHO okay

	//Kaiden: But we're going to try it anyway screw dialoge.
	// Commenting out the below if test

	//if ( bPersonlity != PSYCHO )
	//{
	//	return;
	//}

	// will add a persoanlity to persoanlity list
	if( iLastElementInPersonalityList < ATTITUDE_LIST_SIZE)
	{
		// add element
		PersonalityList[ iLastElementInPersonalityList ] = ( INT32 )bPersonlity;

		// increment attitude list counter
		iLastElementInPersonalityList++;
	}

	return;
}

void CreatePlayerPersonality( void )
{
	// DELETED UNUSED PART OF THE CODE - SANDRO

	iPersonality = iChosenDisabilityTrait();
	
	return;
}

void CreatePlayersPersonalitySkillsAndAttitude( void )
{
	// creates personality, skills and attitudes from curretly built list

	// personality
	CreatePlayerPersonality( );

	// skills are now created later after stats have been chosen
	//CreatePlayerSkills( );

	// attitude
	CreatePlayerAttitude( );

	return;
}


void ResetSkillsAttributesAndPersonality( void )
{

	// reset count of skills attributes and personality

	iLastElementInPersonalityList = 0;

	iLastElementInSkillsList = 0;

	iLastElementInAttitudeList = 0;

}


void ResetIncrementCharacterAttributes( void )
{
	// this resets any increments due to character generation

	// attributes
	iAddStrength = 0;
	iAddDexterity = 0;
	iAddWisdom = 0;
	iAddAgility = 0;
	iAddHealth = 0;
	iAddLeadership = 0;

	// skills
	iAddMarksmanship = 0;
	iAddExplosives = 0;
	iAddMedical = 0;
	iAddMechanical = 0;

}

void SelectMercFace( void )
{
/*
UINT32 cnt;
		
		for ( cnt = 0; cnt < MAX_NEW_IMP_PORTRAITS; cnt++ )
		{
			sFacePositions[ cnt ][ 0 ] = 0;
			sFacePositions[ cnt ][ 1 ] = 0;
			sFacePositions[ cnt ][ 2 ] = 0;
			sFacePositions[ cnt ][ 3 ] = 0;
		}
		*/
	// this procedure will select the approriate face for the merc and save offsets

	// grab face filename
//	strcpy( gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubUnusedFaceFileName , pPlayerSelectedFaceFileNames[ iPortraitNumber ]);


			if( fCharacterIsMale )
			{
			//gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubFaceIndex = 200 + ( UINT8 )iPortraitNumber;
			
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubFaceIndex = ( UINT8 )iPortraitNumber;

			// eyes
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usEyesX = sFacePositions[ gIMPMaleValues[iPortraitNumber].uiEyeXPositions ][ 0 ];
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usEyesY = sFacePositions[ gIMPMaleValues[iPortraitNumber].uiEyeYPositions ][ 1 ];

			// mouth
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usMouthX = sFacePositions[ gIMPMaleValues[iPortraitNumber].uiMouthXPositions ][ 2 ];
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usMouthY = sFacePositions[ gIMPMaleValues[iPortraitNumber].uiMouthYPositions ][ 3 ];
			}
			else
			{
			//gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubFaceIndex = 200 + ( UINT8 )iPortraitNumber;
			
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubFaceIndex = ( UINT8 )iPortraitNumber;

			// eyes
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usEyesX = sFacePositions[ gIMPFemaleValues[iPortraitNumber].uiEyeXPositions ][ 0 ];
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usEyesY = sFacePositions[ gIMPFemaleValues[iPortraitNumber].uiEyeYPositions ][ 1 ];

			// mouth
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usMouthX = sFacePositions[ gIMPFemaleValues[iPortraitNumber].uiMouthXPositions ][ 2 ];
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usMouthY = sFacePositions[ gIMPFemaleValues[iPortraitNumber].uiMouthYPositions ][ 3 ];

	
			}

	// now the offsets
	/*
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubFaceIndex = 200 + ( UINT8 )iPortraitNumber;

	// eyes
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usEyesX = sFacePositions[ iPortraitNumber ][ 0 ];
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usEyesY = sFacePositions[ iPortraitNumber ][ 1 ];

	// mouth
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usMouthX = sFacePositions[ iPortraitNumber ][ 2 ];
	gMercProfiles[ LaptopSaveInfo.iIMPIndex ].usMouthY = sFacePositions[ iPortraitNumber ][ 3 ];
	*/
	// set merc skins and hair color
	SetMercSkinAndHairColors( );
	return;
}

void SetMercSkinAndHairColors( void )
{
	// DELETED UNUSED PART OF THE CODE - SANDRO

	// skin strings
	STR sSkinStrings[]={
		"PINKSKIN",
		"TANSKIN",
		"DARKSKIN",
		"BLACKSKIN",
	};

	// the hair colors
	STR sHairStrings[]={
		"BROWNHEAD",
		"BLACKHEAD",
		"WHITEHEAD",
		"BLONDHEAD",
		"REDHEAD",
	};

	// shirts
	STR sShirtStrings[]={
		"WHITEVEST",
		"GYELLOWSHIRT",
		"YELLOWVEST",
		"greyVEST",
		"BROWNVEST",
		"PURPLESHIRT",
		"BLUEVEST",
		"JEANVEST",
		"GREENVEST",
		"REDVEST",
		"BLACKSHIRT",
	};//shirts


	// shirts
	STR sPantStrings[]={
		"BLUEPANTS",
		"BLACKPANTS",
		"JEANPANTS",
		"TANPANTS",
		"BEIGEPANTS",
		"GREENPANTS",
	};//pants

	// DELETED UNUSED PART OF THE CODE - SANDRO

	strcpy( gMercProfiles[ LaptopSaveInfo.iIMPIndex ].HAIR, sHairStrings[ iChosenHair() ] );
	strcpy( gMercProfiles[ LaptopSaveInfo.iIMPIndex ].SKIN, sSkinStrings[ iChosenSkin() ] );
	strcpy( gMercProfiles[ LaptopSaveInfo.iIMPIndex ].PANTS, sPantStrings[ iChosenPants() ] );
	strcpy( gMercProfiles[ LaptopSaveInfo.iIMPIndex ].VEST, sShirtStrings[ iChosenShirt() ] );
}


void HandleMercStatsForChangesInFace( )
{
	if ( fLoadingCharacterForPreviousImpProfile )
	{
		return;
	}

	//add the skills (major) to the skills list
	AddSelectedSkillsToSkillsList();

	if ( gGameOptions.fNewTraitSystem ) // SANDRO - add also minor traits
		AddSelectedMinorTraitsToSkillsList();

	// now figure out skills
	CreatePlayerSkills();

	// body type
	if ( fCharacterIsMale	)
	{
	// male
		// big or regular
		// Madd - don't override the skills - override the body type instead
		// this should not happen any more, we don't even offer the martial arts to big mercs - SANDRO
		//if( ShouldThisMercHaveABigBody() && iSkillA != MARTIALARTS_OT && iSkillB != MARTIALARTS_OT )
		if( ShouldThisMercHaveABigBody() ) // still added a safety check - SANDRO
		{
			if (!gGameOptions.fNewTraitSystem) // SANDRO - traits
			{
				if (iSkillA == MARTIALARTS_OT )
					iSkillA = HANDTOHAND_OT;
				if (iSkillB == MARTIALARTS_OT )
					iSkillB = HANDTOHAND_OT;
			}

			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubBodyType = BIGMALE;
		}
		else
		{
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubBodyType = REGMALE;
		}
	}
	else
	{
		// female
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].ubBodyType = REGFEMALE;

		if (!gGameOptions.fNewTraitSystem) // SANDRO - traits
		{
			if( iSkillA == MARTIALARTS_OT )
				iSkillA = HANDTOHAND_OT;
			if( iSkillB == MARTIALARTS_OT )
				iSkillB = HANDTOHAND_OT;
		}
	}

	if (gGameOptions.fNewTraitSystem)
	{
		for ( UINT8 ubCnt = 0; ubCnt < gSkillTraitValues.ubMaxNumberOfTraits; ubCnt++ )
		{
			gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bSkillTraits[ ubCnt ] = ( INT8 )SkillsList[ ubCnt ];
		}
	}
	else
	{
		// skill trait
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bSkillTraits[0] =	( INT8 )iSkillA;
		gMercProfiles[ LaptopSaveInfo.iIMPIndex ].bSkillTraits[1] = ( INT8 )iSkillB;
	}
}

BOOLEAN ShouldThisMercHaveABigBody( void )
{
	// We can now choose this ourselves - SANDRO
	return( bBigBodySelected() );
}
