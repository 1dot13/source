#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "Language Defines.h"
	#include "HelpScreen.h"
	#include "GameSettings.h"
	#include "INIReader.h"
	#include "Soldier Profile.h"
	#include "XML.h"
	#include "Item Types.h"
	#include "Items.h"
#else
	#include "sgp.h"
	#include "jascreens.h"
	#include "laptop.h"
	#include "worlddef.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "fade screen.h"
	#include "gamescreen.h" 
	#include "email.h"
	#include "Game Clock.h"
	#include "soldier profile.h"
	#include "strategicmap.h"
	#include "game init.h"
	#include "animation data.h"
	#include "finances.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "music control.h"
	#include "strategic.h"
	#include "history.h"
	#include "merc entering.h"
	#include "Squads.h"
	#include "Campaign Init.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Mines.h"
	#include "gameloop.h"
	#include "Random.h"
	#include "Map Screen Interface.h"
	#include "Tactical Save.h"
	#include "Campaign Types.h"
	#include "Message.h"
	#include "Game Event Hook.h"
	#include "Strategic Movement.h"
	#include "Creature Spreading.h"
	#include "Quests.h"
	#include "Strategic AI.h"
	#include "sound control.h"
	#include "Laptopsave.h"
	#include "AimMembers.h"
	#include "dialogue control.h"
	#include "npc.h"
	#include "Init.h"
	#include "GameSettings.h"
	#include "INIReader.h"
	#include "Soldier Profile.h"
	#include "XML.h"
	#include "Item Types.h"
	#include "Items.h"
	#include "text.h"
#endif

#define	KAT_PROF1					"TABLEDATA\\Profile\\"

STR8 TABLICA_SEKTORY[]={
"A1","A2","A3","A4","A5","A6","A7","A8","A9","A10","A11","A12","A13","A14","A15","A16",
"B1","B2","B3","B4","B5","B6","B7","B8","B9","B10","B11","B12","B13","B14","B15","B16",
"C1","C2","C3","C4","C5","C6","C7","C8","C9","C10","C11","C12","C13","C14","C15","C16",
"D1","D2","D3","D4","D5","D6","D7","D8","D9","D10","D11","D12","D13","D14","D15","D16",
"E1","E2","E3","E4","E5","E6","E7","E8","E9","E10","E11","E12","E13","E14","E15","E16",
"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16",
"G1","G2","G3","G4","G5","G6","G7","G8","G9","G10","G11","G12","G13","G14","G15","G16",
"H1","H2","H3","H4","H5","H6","H7","H8","H9","H10","H11","H12","H13","H14","H15","H16",
"I1","I2","I3","I4","I5","I6","I7","I8","I9","I10","I11","I12","I13","I14","I15","I16",
"J1","J2","J3","J4","J5","J6","J7","J8","J9","J10","J11","J12","J13","J14","J15","J16",
"K1","K2","K3","K4","K5","K6","K7","K8","K9","K10","K11","K12","K13","K14","K15","K16",
"L1","L2","L3","L4","L5","L6","L7","L8","L9","L10","L11","L12","L13","L14","L15","L16",
"M1","M2","M3","M4","M5","M6","M7","M8","M9","M10","M11","M12","M13","M14","M15","M16",
"N1","N2","N3","N4","N5","N6","N7","N8","N9","N10","N11","N12","N13","N14","N15","N16",
"O1","O2","O3","O4","O5","O6","O7","O8","O9","O10","O11","O12","O13","O14","O15","O16",
"P1","P2","P3","P4","P5","P6","P7","P8","P9","P10","P11","P12","P13","P14","P15","P16"
};

// Temp function
void QuickSetupOfMercProfileItems( UINT32 uiCount, UINT8 ubProfileIndex );
BOOLEAN QuickGameMemberHireMerc( UINT8 ubCurrentSoldier );
void MERC_INIT ( STR8 Plik, INT32 ID, STR8 profil  );
extern UINT32 guiExitScreen;
extern UINT32 uiMeanWhileFlags;
extern BOOLEAN gfGamePaused;

extern UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ );
extern void InitVehicles( );

UINT8			gubScreenCount=0;

void MERC_INIT ( STR8 Plik, INT32 ID, STR8 profil  )
{
MERCPROFILESTRUCT * pProfile;
	INT32 i,b,war, war2;
    BOOLEAN Wyb,Wyb2;
   char	uiFSize[30];
	CHAR8 *		lpcBuffer;
	CHAR16 temp[30];
	STR16 pNameString;
	UINT32 cnt;
	STR8 ttt; 
	UINT32 uiLoop, uiLoop2;
	
	
	
if (FileExists(Plik))
	{

	   CIniReader iniReader(Plik);

       pProfile = &(gMercProfiles[ID]);
       		
				

war		= iniReader.ReadBoolean(profil,"CivilianGroup",-1);

   	switch( war )
	{
		case  0: pProfile->ubCivilianGroup = NON_CIV_GROUP; break;
		case  1: pProfile->ubCivilianGroup = REBEL_CIV_GROUP; break;
		case  2: pProfile->ubCivilianGroup = KINGPIN_CIV_GROUP; break;
		case  3: pProfile->ubCivilianGroup = SANMONA_ARMS_GROUP; break;
		case  4: pProfile->ubCivilianGroup = ANGELS_GROUP; break;
		case  5: pProfile->ubCivilianGroup = BEGGARS_CIV_GROUP; break;
		case  6: pProfile->ubCivilianGroup = TOURISTS_CIV_GROUP; break;
		case  7: pProfile->ubCivilianGroup = ALMA_MILITARY_CIV_GROUP; break;
		case  8: pProfile->ubCivilianGroup = DOCTORS_CIV_GROUP; break;
		case  9: pProfile->ubCivilianGroup = COUPLE1_CIV_GROUP; break;
		case 10: pProfile->ubCivilianGroup = HICKS_CIV_GROUP; break;
		case 11: pProfile->ubCivilianGroup = WARDEN_CIV_GROUP; break;
		case 12: pProfile->ubCivilianGroup = JUNKYARD_CIV_GROUP; break;
		case 13: pProfile->ubCivilianGroup = FACTORY_KIDS_GROUP; break;
		case 14: pProfile->ubCivilianGroup = QUEENS_CIV_GROUP; break;
		case 15: pProfile->ubCivilianGroup = UNNAMED_CIV_GROUP_15; break;
		case 16: pProfile->ubCivilianGroup = UNNAMED_CIV_GROUP_16; break;
		case 17: pProfile->ubCivilianGroup = UNNAMED_CIV_GROUP_17; break;
		case 18: pProfile->ubCivilianGroup = UNNAMED_CIV_GROUP_18; break;
		case 19: pProfile->ubCivilianGroup = UNNAMED_CIV_GROUP_19; break;
		case 20: pProfile->ubCivilianGroup = GRUPA_GROUP_1; break;
		case 21: pProfile->ubCivilianGroup = GRUPA_GROUP_2; break;
		case 22: pProfile->ubCivilianGroup = GRUPA_GROUP_3; break;
		case 23: pProfile->ubCivilianGroup = GRUPA_GROUP_4; break;
		case 24: pProfile->ubCivilianGroup = GRUPA_GROUP_5; break;
		case 25: pProfile->ubCivilianGroup = GRUPA_GROUP_6; break;
		case 26: pProfile->ubCivilianGroup = GRUPA_GROUP_7; break;
		case 27: pProfile->ubCivilianGroup = GRUPA_GROUP_8; break;
		case 28: pProfile->ubCivilianGroup = GRUPA_GROUP_9; break;
		case 29: pProfile->ubCivilianGroup = GRUPA_GROUP_10; break;
		case 30: pProfile->ubCivilianGroup = GRUPA_GROUP_11; break;
		case 31: pProfile->ubCivilianGroup = GRUPA_GROUP_12; break;
		case 32: pProfile->ubCivilianGroup = GRUPA_GROUP_13; break;
		case 33: pProfile->ubCivilianGroup = GRUPA_GROUP_14; break;
		case 34: pProfile->ubCivilianGroup = GRUPA_GROUP_15; break;
		case 35: pProfile->ubCivilianGroup = GRUPA_GROUP_16; break;
		case 36: pProfile->ubCivilianGroup = GRUPA_GROUP_17; break;
		case 37: pProfile->ubCivilianGroup = GRUPA_GROUP_18; break;
		case 38: pProfile->ubCivilianGroup = GRUPA_GROUP_19; break;
		case 39: pProfile->ubCivilianGroup = GRUPA_GROUP_20; break;
		
		case 40: pProfile->ubCivilianGroup = GRUPA_GROUP_21; break;
		case 41: pProfile->ubCivilianGroup = GRUPA_GROUP_22; break;
		case 42: pProfile->ubCivilianGroup = GRUPA_GROUP_23; break;
		case 43: pProfile->ubCivilianGroup = GRUPA_GROUP_24; break;
		case 44: pProfile->ubCivilianGroup = GRUPA_GROUP_25; break;
		case 45: pProfile->ubCivilianGroup = GRUPA_GROUP_26; break;
		case 46: pProfile->ubCivilianGroup = GRUPA_GROUP_27; break;
		case 47: pProfile->ubCivilianGroup = GRUPA_GROUP_28; break;
		case 48: pProfile->ubCivilianGroup = GRUPA_GROUP_29; break;
		case 49: pProfile->ubCivilianGroup = GRUPA_GROUP_30; break;
		case 50: pProfile->ubCivilianGroup = GRUPA_GROUP_31; break;
		case 51: pProfile->ubCivilianGroup = GRUPA_GROUP_32; break;
		case 52: pProfile->ubCivilianGroup = GRUPA_GROUP_33; break;
		case 53: pProfile->ubCivilianGroup = GRUPA_GROUP_34; break;
		case 54: pProfile->ubCivilianGroup = GRUPA_GROUP_35; break;
		case 55: pProfile->ubCivilianGroup = GRUPA_GROUP_36; break;
		case 56: pProfile->ubCivilianGroup = GRUPA_GROUP_37; break;
		case 57: pProfile->ubCivilianGroup = GRUPA_GROUP_38; break;
		case 58: pProfile->ubCivilianGroup = GRUPA_GROUP_39; break;
		case 59: pProfile->ubCivilianGroup = GRUPA_GROUP_40; break;		
		case -1:break;	
		default:break;
	}

Wyb2		= iniReader.ReadBoolean(profil,"GEN2",FALSE);


if (Wyb2 == FALSE) 
	{
       
    war		= iniReader.ReadInteger(profil,"Life",-1);
	if (war > -1 ) { pProfile->bLife           = war; } else;
	
	war		= iniReader.ReadInteger(profil,"LifeMax",-1);
	if (war > -1 ) { pProfile->bLifeMax        = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Agility",-1);
	if (war > -1 ) { pProfile->bAgility        = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Leadership",-1);
	if (war > -1 ) { pProfile->bLeadership     = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Dexterity",-1);
	if (war > -1 ) { pProfile->bDexterity      = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Strength",-1);
	if (war > -1 ) { pProfile->bStrength       = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Wisdom",-1);
	if (war > -1 ) { pProfile->bWisdom         = war; } else;
	
	war		= iniReader.ReadInteger(profil,"ExpLevel",-1);
	if (war > -1 ) { pProfile->bExpLevel       = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Marksmanship",-1);
	if (war > -1 ) { pProfile->bMarksmanship   = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Medical",-1);
	if (war > -1 ) { pProfile->bMedical        = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Mechanical",-1);
	if (war > -1 ) { pProfile->bMechanical     = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Explosive",-1);
	if (war > -1 ) { pProfile->bExplosive      = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Scientific",-1);
	if (war > -1 ) { pProfile->bScientific     = war; } else;
	
	}
	
	else if (Wyb2 == TRUE) 
	{
	war		= iniReader.ReadInteger(profil,"GEN2_ExpLevel",1);
	//war2		= iniReader.ReadInteger(profil,"GEN2_BaseAttribute",49);
			INT8 bBaseAttribute;
   	switch( war )
	{
		case 1: 
			
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		//bp.bAttitude = (INT8) Random( MAXATTITUDES );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
		
			break;
		case 2: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;
		case 3: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;
		case 4: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;	
		case 5: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;	
		case 6: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;	
		case 7: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;	
		case 8: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;	
		case 9: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;
		case 10: 
		
		pProfile->bExpLevel = war;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
			
			break;		
			
		default:
		
		pProfile->bExpLevel = 1;	
		bBaseAttribute = 49 + ( 4 * pProfile->bExpLevel );
		
		pProfile->bLifeMax			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLife	  			= pProfile->bLifeMax;
		pProfile->bAgility			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bDexterity		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMarksmanship		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMedical			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bMechanical		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bExplosive		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bLeadership		= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bStrength			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));
		pProfile->bWisdom			= (INT8)(bBaseAttribute + Random( 9 ) + Random( 8 ));	
		
			break;
	}
	
	
	
	}
	

    war		= iniReader.ReadInteger(profil,"Sex",-1);
       	switch( war )
	{
		case 0:
			pProfile->bSex =0; //M
			break;
		case 1:
			pProfile->bSex =1; //K
			break;
		case -1:
			break;	
		default:
			break;
	}
   	
	war		= iniReader.ReadInteger(profil,"BodyType",-1);  	
   	switch( war )
	{
		case 0: pProfile->ubBodyType = REGMALE; break;
		case 1: pProfile->ubBodyType = BIGMALE; break;
		case 2: pProfile->ubBodyType = STOCKYMALE; break;
		case 3: pProfile->ubBodyType = REGFEMALE; break;
		case 4: pProfile->ubBodyType = ADULTFEMALEMONSTER; break;		
		case 5: pProfile->ubBodyType = AM_MONSTER; break;					
		case 6: pProfile->ubBodyType = YAF_MONSTER; break;					
		case 7: pProfile->ubBodyType = YAM_MONSTER; break;					
		case 8: pProfile->ubBodyType = LARVAE_MONSTER; break;	
		case 9: pProfile->ubBodyType = INFANT_MONSTER; break;	
		case 10: pProfile->ubBodyType = QUEENMONSTER; break;									
	    case 11: pProfile->ubBodyType = FATCIV; break;													
		case 12: pProfile->ubBodyType = MANCIV; break;	
		case 13: pProfile->ubBodyType = DRESSCIV; break;	
		case 14: pProfile->ubBodyType = HATKIDCIV; break;				
		case 15: pProfile->ubBodyType = KIDCIV; break;	
		case 16: pProfile->ubBodyType = COW; break;	
		case 17: pProfile->ubBodyType = CROW; break;
		case 18: pProfile->ubBodyType = ROBOTNOWEAPON; break;		
		case 19: pProfile->ubBodyType = BLOODCAT; break;	
		case 20: pProfile->ubBodyType = HUMVEE; break;		
		case 21: pProfile->ubBodyType = ELDORADO; break;	
		case 22: pProfile->ubBodyType = ICECREAMTRUCK; break;	
		case 23: pProfile->ubBodyType = JEEP; break;						
		case -1: break;	
		default: break;
	}

	war		= iniReader.ReadInteger(profil,"Head",-1);
   	switch( war )
	{
		case 0: SET_PALETTEREP_ID ( pProfile->HAIR,"BROWNHEAD"); break;
		case 1: SET_PALETTEREP_ID ( pProfile->HAIR,"BLACKHEAD"); break;
		case 2: SET_PALETTEREP_ID ( pProfile->HAIR,"WHITEHEAD"); break;
		case 3: SET_PALETTEREP_ID ( pProfile->HAIR,"BLONDEHEAD"); break;
		case 4: SET_PALETTEREP_ID ( pProfile->HAIR,"REDHEAD"); break;		
		case -1: break;	
		default: break;
	}


Wyb		= iniReader.ReadBoolean(profil,"GEN1",FALSE);
if (Wyb == FALSE) {
   war		= iniReader.ReadInteger(profil,"VEST",-1);
   	switch( war )
	{
		case 0:SET_PALETTEREP_ID ( pProfile->VEST,"WHITEVEST");break;
		case 1:SET_PALETTEREP_ID ( pProfile->VEST,"BLACKSHIRT");break;
		case 2:SET_PALETTEREP_ID ( pProfile->VEST,"PURPLESHIRT");break;
		case 3:SET_PALETTEREP_ID ( pProfile->VEST,"BLUEVEST");break;
		case 4:SET_PALETTEREP_ID ( pProfile->VEST,"BROWNVEST");break;
		case 5:SET_PALETTEREP_ID ( pProfile->VEST,"JEANVEST");break;
		case 6: SET_PALETTEREP_ID ( pProfile->VEST,"REDVEST"); break;
		case 7: SET_PALETTEREP_ID ( pProfile->VEST,"GYELLOWSHIRT"); break;
        case 8: SET_PALETTEREP_ID ( pProfile->VEST, "GREENVEST"  ); break;
		case 9: SET_PALETTEREP_ID ( pProfile->VEST, "greyVEST"  ); break;
		case 10:SET_PALETTEREP_ID( pProfile->VEST,"YELLOWVEST"  ); break;
		case -1: break;
		default: break;
	}
 
    war		= iniReader.ReadInteger(profil,"PANTS",-1); 
   	switch( war )
	{
		case 0: SET_PALETTEREP_ID ( pProfile->PANTS,"TANPANTS"); break;
		case 1: SET_PALETTEREP_ID ( pProfile->PANTS,"BEIGEPANTS"); break;
		case 2: SET_PALETTEREP_ID ( pProfile->PANTS,"GREENPANTS"); break;
		case 3: SET_PALETTEREP_ID ( pProfile->PANTS,"JEANPANTS"); break;
		case -1: break;	
		default: break;
	}
	
	}
	
	else if (Wyb == TRUE)
	
	{
	
	   	switch( Random(11) )
	{
		case 0:SET_PALETTEREP_ID ( pProfile->VEST,"WHITEVEST");break;
		case 1:SET_PALETTEREP_ID ( pProfile->VEST,"BLACKSHIRT");break;
		case 2:SET_PALETTEREP_ID ( pProfile->VEST,"PURPLESHIRT");break;
		case 3:SET_PALETTEREP_ID ( pProfile->VEST,"BLUEVEST");break;
		case 4:SET_PALETTEREP_ID ( pProfile->VEST,"BROWNVEST");break;
		case 5:SET_PALETTEREP_ID ( pProfile->VEST,"JEANVEST");break;
		case 6:SET_PALETTEREP_ID ( pProfile->VEST,"REDVEST");break;
		case 7:SET_PALETTEREP_ID ( pProfile->VEST,"GYELLOWSHIRT");break;	
        case 8: SET_PALETTEREP_ID( pProfile->VEST,"GREENVEST"  ); break;
		case 9: SET_PALETTEREP_ID( pProfile->VEST,"greyVEST"  ); break;
		case 10:SET_PALETTEREP_ID( pProfile->VEST,"YELLOWVEST"  ); break;

	}
 
   	switch( Random (4) )
	{
		case 0:	SET_PALETTEREP_ID ( pProfile->PANTS,"TANPANTS"); break;
		case 1:	SET_PALETTEREP_ID ( pProfile->PANTS,"BEIGEPANTS"); break;
		case 2: SET_PALETTEREP_ID ( pProfile->PANTS,"GREENPANTS");break;
		case 3: SET_PALETTEREP_ID ( pProfile->PANTS,"JEANPANTS"); break;	
	}
	
	}
    
 
  war		= iniReader.ReadInteger(profil,"SKIN",-1);  	
   	switch( war )
	{
		case 0: SET_PALETTEREP_ID ( pProfile->SKIN,"PINKSKIN"); break;
		case 1: SET_PALETTEREP_ID ( pProfile->SKIN,"TANSKIN"); break;
		case 2: SET_PALETTEREP_ID ( pProfile->SKIN,"DARKSKIN"); break;
		case 3: SET_PALETTEREP_ID ( pProfile->SKIN,"BLACKSKIN"); break;
		case -1: break;	
		default: break;
	}
       

	war		= iniReader.ReadInteger(profil,"FaceIndex",-1); 
	if (war > -1 ) { pProfile->ubFaceIndex   =  war; } else;
   
	war		= iniReader.ReadInteger(profil,"EyesX",-1); 
	if (war > -1 ) { pProfile->usEyesX       =  war; } else;
	 
	war		= iniReader.ReadInteger(profil,"EyesY",-1); 
	if (war > -1 ) { pProfile->usEyesY       =  war; } else;
	 
	war		= iniReader.ReadInteger(profil,"MouthX",-1); 
	if (war > -1 ) { pProfile->usMouthX      =  war; } else;
	
	war		= iniReader.ReadInteger(profil,"MouthY",-1); 
	if (war > -1 ) { pProfile->usMouthY      =  war; } else;
			
	war		= iniReader.ReadInteger(profil,"BlinkFrequency",-1); 
	if (war > -1 ) { pProfile->uiBlinkFrequency      =  war; } else;

	war		= iniReader.ReadInteger(profil,"ExpressionFrequency",-1); 
	if (war > -1 ) { pProfile->uiExpressionFrequency      =  war; } else;

			
	war		= iniReader.ReadInteger(profil,"SkillTrait1",-1); 
	switch( war )
	{
		case 0: pProfile->bSkillTrait =NO_SKILLTRAIT; break;
		case 1: pProfile->bSkillTrait =LOCKPICKING; break;
		case 2: pProfile->bSkillTrait =HANDTOHAND; break;
		case 3: pProfile->bSkillTrait =ELECTRONICS; break;		
		case 4: pProfile->bSkillTrait =NIGHTOPS; break;		
		case 5: pProfile->bSkillTrait =THROWING; break;		
		case 6: pProfile->bSkillTrait =TEACHING; break;		
		case 7: pProfile->bSkillTrait =HEAVY_WEAPS; break;		
		case 8: pProfile->bSkillTrait =AUTO_WEAPS; break;		
		case 9: pProfile->bSkillTrait =STEALTHY; break;		
		case 10: pProfile->bSkillTrait =AMBIDEXT; break;		
		case 11: pProfile->bSkillTrait =THIEF; break;		
		case 12: pProfile->bSkillTrait =MARTIALARTS; break;
		case 13: pProfile->bSkillTrait =KNIFING; break;		
		case 14: pProfile->bSkillTrait =ONROOF; break;
		case 15: pProfile->bSkillTrait =CAMOUFLAGED; break;		
		case 16: pProfile->bSkillTrait =CAMOUFLAGED_URBAN; break;		
		case 17: pProfile->bSkillTrait =CAMOUFLAGED_DESERT; break;		
		case 18: pProfile->bSkillTrait =CAMOUFLAGED_SNOW; break;
		case 19: pProfile->bSkillTrait =NUM_SKILLTRAITS; break;	
		case -1: break;			
		default: break;
	}
	
war		= iniReader.ReadInteger(profil,"SkillTrait2",-1); 
	switch( war )
	{
		case 0:
			pProfile->bSkillTrait2 =NO_SKILLTRAIT;
			break;
		case 1:
			pProfile->bSkillTrait2 =LOCKPICKING;
			break;
		case 2:
			pProfile->bSkillTrait2 =HANDTOHAND;
			break;
		case 3:
			pProfile->bSkillTrait2 =ELECTRONICS;
			break;		
		case 4:
			pProfile->bSkillTrait2 =NIGHTOPS;
			break;		
		case 5:
			pProfile->bSkillTrait2 =THROWING;
			break;		
		case 6:
			pProfile->bSkillTrait2 =TEACHING;
			break;		
		case 7:
			pProfile->bSkillTrait2 =HEAVY_WEAPS;
			break;		
		case 8:
			pProfile->bSkillTrait2 =AUTO_WEAPS;
			break;		
		case 9:
			pProfile->bSkillTrait2 =STEALTHY;
			break;		
		case 10:
			pProfile->bSkillTrait2 =AMBIDEXT;
			break;		
		case 11:
			pProfile->bSkillTrait2 =THIEF;
			break;		
		case 12:
			pProfile->bSkillTrait2 =MARTIALARTS;
			break;
		case 13:
			pProfile->bSkillTrait2 =KNIFING;
			break;		
		case 14:
			pProfile->bSkillTrait2 =ONROOF;
			break;
		case 15:
			pProfile->bSkillTrait2 =CAMOUFLAGED;
			break;		
		case 16:
			pProfile->bSkillTrait2 =CAMOUFLAGED_URBAN;
			break;		
		case 17:
			pProfile->bSkillTrait2 =CAMOUFLAGED_DESERT;
			break;		
		case 18:
			pProfile->bSkillTrait2 =CAMOUFLAGED_SNOW;
			break;	
		case 19:
			pProfile->bSkillTrait2 =NUM_SKILLTRAITS;
			break;			
		case -1:
			break;				
		default:
			break;
	}
	
  // PersonalityTrait;
   war		= iniReader.ReadInteger(profil,"PersonalityTrait",-1); 
   	switch( war )
	{
		case 0: pProfile->bPersonalityTrait =NO_PERSONALITYTRAIT; break;
		case 1: pProfile->bPersonalityTrait =HEAT_INTOLERANT; break;
		case 2: pProfile->bPersonalityTrait =NERVOUS; break;
		case 3: pProfile->bPersonalityTrait =CLAUSTROPHOBIC; break;
		case 4: pProfile->bPersonalityTrait =NONSWIMMER; break;
		case 5: pProfile->bPersonalityTrait =FEAR_OF_INSECTS; break;
		case 6: pProfile->bPersonalityTrait =FORGETFUL; break;
		case 7: pProfile->bPersonalityTrait =PSYCHO; break;
		case -1: break;	
		default: break;
	} 
	
	
	//Attitudes;
   war		= iniReader.ReadInteger(profil,"Attitude",-1); 
   	switch( war )
	{
		case 0:
			pProfile->bAttitude =ATT_NORMAL;
			break;
		case 1:
			pProfile->bAttitude =ATT_FRIENDLY;
			break;
		case 2:
			pProfile->bAttitude =ATT_LONER;
			break;
		case 3:
			pProfile->bAttitude =ATT_OPTIMIST;
			break;
		case 4:
			pProfile->bAttitude =ATT_PESSIMIST;
			break;
		case 5:
			pProfile->bAttitude =ATT_AGGRESSIVE;
			break;
		case 6:
			pProfile->bAttitude =ATT_ARROGANT;
			break;
		case 7:
			pProfile->bAttitude =ATT_BIG_SHOT;
			break;
		case 8:
			pProfile->bAttitude =ATT_ASSHOLE;
			break;
		case 9:
			pProfile->bAttitude =ATT_COWARD;
			break;
		case 10:
			pProfile->bAttitude =NUM_ATTITUDES;
			break;
		case -1:
			break;	
		default:
			break;
	} 
	
 
	// SexistLevels
   war		= iniReader.ReadInteger(profil,"Sexist",-1); 
   	switch( war )
	{
		case 0:
			pProfile->bSexist =NOT_SEXIST;
			break;
		case 1:
			pProfile->bSexist =SOMEWHAT_SEXIST;
			break;
		case 2:
			pProfile->bSexist =VERY_SEXIST;
			break;
		case 3:
			pProfile->bSexist =GENTLEMAN;
			break;
		case -1:
			break;	
		default:
			break;
	}

	//CharacterEvolution;
   war		= iniReader.ReadInteger(profil,"Evolution",-1); 
   	switch( war )
	{
		case 0:
			pProfile->bEvolution =NORMAL_EVOLUTION;
			break;
		case 1:
			pProfile->bEvolution =NO_EVOLUTION;
			break;
		case 2:
			pProfile->bEvolution =DEVOLVE;
			break;
		case -1:
			break;	
		default:
			break;
	}

   war		= iniReader.ReadInteger(profil,"MedicalDeposit",-1); 
	if (war == 0 ) { pProfile->bMedicalDeposit = false; }
	else if (war == 1 ) { pProfile->bMedicalDeposit = true;  } 
	else if (war == -1 ) ;

war		= iniReader.ReadInteger(profil,"BaseMorale",-1); 
	if (war > -1 ) { pProfile->bBaseMorale      =  war; } else;
		
war		= iniReader.ReadInteger(profil,"Salary",-1); 
	if (war > -1 ) { pProfile->sSalary      =  war; } else;
	
war		= iniReader.ReadInteger(profil,"WeeklySalary",-1); 
	if (war > -1 ) { pProfile->uiWeeklySalary      =  war; } else;
	
war		= iniReader.ReadInteger(profil,"Bi-WeeklySalary",-1); 
	if (war > -1 ) { pProfile->uiBiWeeklySalary      =  war; } else;
	
war		= iniReader.ReadInteger(profil,"NeedForSleep",-1); 
	if (war > -1 ) { pProfile->ubNeedForSleep      =  war; } else;
	
war		= iniReader.ReadInteger(profil,"DeathRate",-1); 
	if (war > -1 ) { pProfile->bDeathRate      =  war; } else;
	
war		= iniReader.ReadInteger(profil,"Friendly",-1); 
	if (war > -1 ) { pProfile->bFriendlyOrDirectDefaultResponseUsedRecently      =  war; } else;
	
war		= iniReader.ReadInteger(profil,"Recruit",-1); 
	if (war > -1 ) { pProfile->bRecruitDefaultResponseUsedRecently      =  war; } else;
	
war		= iniReader.ReadInteger(profil,"Threaten",-1); 
	if (war > -1 ) { pProfile->bThreatenDefaultResponseUsedRecently      =  war; } else;



//PROFILE_MISC_FLAG_RECRUITED
//PROFILE_MISC_FLAG_HAVESEENCREATURE
//PROFILE_MISC_FLAG_FORCENPCQUOTE
//PROFILE_MISC_FLAG_WOUNDEDBYPLAYER
//PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS
//PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE
//PROFILE_MISC_FLAG_EPCACTIVE
//PROFILE_MISC_FLAG_ALREADY_USED_ITEMS

war		= iniReader.ReadInteger(profil,"MISC_FLAG1",-1); 
   	switch( war )
	{
		case 0:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_RECRUITED;
			break;
		case 1:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_HAVESEENCREATURE;
			break;
		case 2:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_FORCENPCQUOTE;
			break;
		case 3:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_WOUNDEDBYPLAYER;
			break;
		case 4:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS;
			break;
		case 5:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_SAID_HOSTILE_QUOTE;
			break;
		case 6:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_EPCACTIVE;
			break;	
		case 7:
			pProfile->ubMiscFlags=PROFILE_MISC_FLAG_ALREADY_USED_ITEMS;
			break;			
		case -1:
			break;	
		default:
			break;
		}	

//PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR
//PROFILE_MISC_FLAG2_LEFT_COUNTRY
//PROFILE_MISC_FLAG2_BANDAGED_TODAY
//PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE
//PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE
//PROFILE_MISC_FLAG2_MARRIED_TO_HICKS
//PROFILE_MISC_FLAG2_ASKED_BY_HICKS

war		= iniReader.ReadInteger(profil,"MISC_FLAG2",-1); 
   	switch( war )
	{
		case 0:
			pProfile->ubMiscFlags2=PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR;
			break;
		case 1:
			pProfile->ubMiscFlags2=PROFILE_MISC_FLAG2_LEFT_COUNTRY;
			break;
		case 2:
			pProfile->ubMiscFlags2=PROFILE_MISC_FLAG2_BANDAGED_TODAY;
			break;
		case 3:
			pProfile->ubMiscFlags2=PROFILE_MISC_FLAG2_SAID_FIRSTSEEN_QUOTE;
			break;
		case 4:
			pProfile->ubMiscFlags2=PROFILE_MISC_FLAG2_NEEDS_TO_SAY_HOSTILE_QUOTE;
			break;
		case 5:
			pProfile->ubMiscFlags2=PROFILE_MISC_FLAG2_MARRIED_TO_HICKS;
			break;
		case 6:
			pProfile->ubMiscFlags2=PROFILE_MISC_FLAG2_ASKED_BY_HICKS;
			break;		
		case -1:
			break;	
		default:
			break;
		}	

//PROFILE_MISC_FLAG3_PLAYER_LEFT_MSG_FOR_MERC_AT_AIM
//PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE
//PROFILE_MISC_FLAG3_PLAYER_HAD_CHANCE_TO_HIRE
//PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL
//PROFILE_MISC_FLAG3_NPC_PISSED_OFF
//PROFILE_MISC_FLAG3_MERC_MERC_IS_DEAD_AND_QUOTE_SAID
//PROFILE_MISC_FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH
//PROFILE_MISC_FLAG3_GOODGUY	

war		= iniReader.ReadInteger(profil,"MISC_FLAG3",-1); 
   	switch( war )
	{
		case 0:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_PLAYER_LEFT_MSG_FOR_MERC_AT_AIM;
			break;
		case 1:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE;
			break;
		case 2:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_PLAYER_HAD_CHANCE_TO_HIRE;
			break;
		case 3:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL;
			break;
		case 4:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_NPC_PISSED_OFF;
			break;
		case 5:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_MERC_MERC_IS_DEAD_AND_QUOTE_SAID;
			break;
		case 6:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_TOWN_DOESNT_CARE_ABOUT_DEATH;
			break;
		case 7:
			pProfile->ubMiscFlags3=PROFILE_MISC_FLAG3_GOODGUY;
			break;
			
		case -1:
			break;	
		default:
			break;
		}	

		
			//hand
	war		= iniReader.ReadInteger(profil,"HAND",-1); 
		if (war > -1 ) 
			{ 		
				gMercProfiles[ ID ].inv[ HANDPOS ] = war; 
				war		= iniReader.ReadInteger(profil,"HAND_Status",-1); 
				gMercProfiles[ ID ].bInvStatus[ HANDPOS ] = war;
				war		= iniReader.ReadInteger(profil,"HAND_Number",-1); 	
				gMercProfiles[ ID ].bInvNumber[ HANDPOS ] = war;	
			} else;
		
		
		//second hand
		war		= iniReader.ReadInteger(profil,"SECOND_HAND",-1); 
		if (war > -1 ) 
			{ 	
				gMercProfiles[ ID ].inv[ SECONDHANDPOS ] = war;
				war		= iniReader.ReadInteger(profil,"SECOND_HAND_Status",-1); 
				gMercProfiles[ ID ].bInvStatus[ SECONDHANDPOS ] = war;
				war		= iniReader.ReadInteger(profil,"SECOND_HAND_Number",-1); 
				gMercProfiles[ ID ].bInvNumber[ SECONDHANDPOS ] = war;
			} else;
		

		//BIGPOCK1POS
	war		= iniReader.ReadInteger(profil,"BIGPOCK1",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ BIGPOCK1POS ] = war;
			war		= iniReader.ReadInteger(profil,"BIGPOCK1_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ BIGPOCK1POS ] = war;	
			war		= iniReader.ReadInteger(profil,"BIGPOCK1_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ BIGPOCK1POS ] = war;
		} else;
		
		
		//BIGPOCK2POS
	war		= iniReader.ReadInteger(profil,"BIGPOCK2",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ BIGPOCK2POS ] = war;
			war		= iniReader.ReadInteger(profil,"BIGPOCK2_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ BIGPOCK2POS ] = war;	
			war		= iniReader.ReadInteger(profil,"BIGPOCK2_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ BIGPOCK2POS ] = war;
		} else;
		
		//BIGPOCK3POS
	war		= iniReader.ReadInteger(profil,"BIGPOCK3",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ BIGPOCK3POS ] = war;
			war		= iniReader.ReadInteger(profil,"BIGPOCK3_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ BIGPOCK3POS ] = war;	
			war		= iniReader.ReadInteger(profil,"BIGPOCK3_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ BIGPOCK3POS ] = war;
		} else;
		
		//BIGPOCK4POS
	war		= iniReader.ReadInteger(profil,"BIGPOCK4",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ BIGPOCK4POS ] = war;
			war		= iniReader.ReadInteger(profil,"BIGPOCK4_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ BIGPOCK4POS ] = war;	
			war		= iniReader.ReadInteger(profil,"BIGPOCK4_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ BIGPOCK4POS ] = war;
		} else;	
		
		
		//SMALLPOCK1POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK1",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK1POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK1_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK1POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK1_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK1POS ] = war;
		} else;	
		
		//SMALLPOCK2POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK2",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK2POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK2_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK2POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK2_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK2POS ] = war;
		} else;	
	
	
		//SMALLPOCK3POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK3",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK3POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK3_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK3POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK3_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK3POS ] = war;
		} else;	
		
		//SMALLPOCK4POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK4",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK4POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK4_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK4POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK4_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK4POS ] = war;
		} else;	
		
		//SMALLPOCK5POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK5",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK5POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK5_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK5POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK5_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK5POS ] = war;
		} else;	
	
	
		//SMALLPOCK6POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK6",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK6POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK6_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK6POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK6_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK6POS ] = war;
		} else;	
		
		
		//SMALLPOCK7POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK7",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK7POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK7_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK7POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK7_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK7POS ] = war;
		} else;	
		
		//SMALLPOCK8POS
	war		= iniReader.ReadInteger(profil,"SMALLPOCK8",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ SMALLPOCK8POS ] = war;
			war		= iniReader.ReadInteger(profil,"SMALLPOCK8_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ SMALLPOCK8POS ] = war;	
			war		= iniReader.ReadInteger(profil,"SMALLPOCK8_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ SMALLPOCK8POS ] = war;
		} else;	
		
		
		//HELMETPOS
	war		= iniReader.ReadInteger(profil,"HELMET",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ HELMETPOS ] = war;
			war		= iniReader.ReadInteger(profil,"HELMET_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ HELMETPOS ] = war;	
			war		= iniReader.ReadInteger(profil,"HELMET_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ HELMETPOS ] = war;
		} else;	
		
		//VESTPOS
	war		= iniReader.ReadInteger(profil,"VEST2",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ VESTPOS ] = war;
			war		= iniReader.ReadInteger(profil,"VEST2_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ VESTPOS ] = war;	
			war		= iniReader.ReadInteger(profil,"VEST2_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ VESTPOS ] = war;
		} else;	
		
		//LEGPOS
	war		= iniReader.ReadInteger(profil,"LEG",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ LEGPOS ] = war;
			war		= iniReader.ReadInteger(profil,"LEG_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ LEGPOS ] = war;	
			war		= iniReader.ReadInteger(profil,"LEG_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ LEGPOS ] = war;
		} else;	
		
		//HEAD1POS
	war		= iniReader.ReadInteger(profil,"HEAD1",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ HEAD1POS ] = war;
			war		= iniReader.ReadInteger(profil,"HEAD1_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ HEAD1POS ] = war;	
			war		= iniReader.ReadInteger(profil,"HEAD1_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ HEAD1POS ] = war;
		} else;	
		
		//HEAD2POS
	war		= iniReader.ReadInteger(profil,"HEAD2",-1); 
	if (war > -1 ) 
		{ 		
			gMercProfiles[ ID ].inv[ HEAD2POS ] = war;
			war		= iniReader.ReadInteger(profil,"HEAD2_Status",-1); 
			gMercProfiles[ ID ].bInvStatus[ HEAD2POS ] = war;	
			war		= iniReader.ReadInteger(profil,"HEAD2_Number",-1); 
			gMercProfiles[ ID ].bInvNumber[ HEAD2POS ] = war;
		} else;	
		
		
		//	iBalance;
		//wartosc 0 - 1000000000
	war		= iniReader.ReadInteger(profil,"Balance",-1);
	if (war > -1 ) { pProfile->iBalance      = war; } else;
		
		//1-16
	war		= iniReader.ReadInteger(profil,"SectorX",0);
	if (war > 0 ) { gMercProfiles[ ID ].sSectorX = war; } else;
		//1-16
	war		= iniReader.ReadInteger(profil,"SectorY",0);
	if (war > 0 ) { gMercProfiles[ ID ].sSectorY = war; } else;
		//0-3
	war		= iniReader.ReadInteger(profil,"SectorZ", -1);
	if (war > -1 ) { gMercProfiles[ ID ].bSectorZ = war; } else;

//-----------------------------------------------nowe-----------------------------	
	war		= iniReader.ReadInteger(profil,"Race", -1);
	if (war > -1 ) { gMercProfiles[ ID ].bRace = war; } else;
	
	war		= iniReader.ReadInteger(profil,"Nationality", -1);
	if (war > -1 ) { gMercProfiles[ ID ].bNationality = war; } else;
	
	
	war		= iniReader.ReadInteger(profil,"VocalVolume", -1);
	   	switch( war )
	{
		case 0:
			pProfile->bVocalVolume =LOWVOLUME;
			break;
		case 1:
			pProfile->bVocalVolume =MIDVOLUME;
			break;
		case 2:
			pProfile->bVocalVolume =HIGHVOLUME;
			break;
		case 3:
			pProfile->bVocalVolume =BTNVOLUME;
			break;	
		case -1:
			break;	
		default:
			break;
	}
	
//----------------------------	


	}

}


void InitNPCs( void )
{
	MERCPROFILESTRUCT * pProfile;
	STR8 nazwa; 
	INT i;
	CHAR8						zFileName[255];
	

	// add the pilot at a random location!
	pProfile = &(gMercProfiles[ SKYRIDER ]);
	switch( Random( 4 ) )
	{
		case 0:
			pProfile->sSectorX = 15;
			pProfile->sSectorY = MAP_ROW_B;
			pProfile->bSectorZ = 0;
			break;
		case 1:
			pProfile->sSectorX = 14;
			pProfile->sSectorY = MAP_ROW_E;
			pProfile->bSectorZ = 0;
			break;
		case 2:
			pProfile->sSectorX = 12;
			pProfile->sSectorY = MAP_ROW_D;
			pProfile->bSectorZ = 0;
			break;
		case 3:
			pProfile->sSectorX = 16;
			pProfile->sSectorY = MAP_ROW_C;
			pProfile->bSectorZ = 0;
			break;
	}
	
		switch ( gGameOptions.ubDifficultyLevel)
	{
		case DIF_LEVEL_EASY:
		
		    if ( gGameOptions.fGunNut )  nazwa ="prof_TonsOfGuns"; else  nazwa ="prof_NormalGuns";
				
				INT i;
				CHAR8						zFileName[255];
				
				for( i = 0; i < 500; i++ )
				{	
				sprintf( zFileName, "TABLEDATA\\Profile\\Prof_Novice\\prof%03d.ini", i );			
				MERC_INIT (zFileName,i,nazwa);			
				}
				
			break;
			
		case DIF_LEVEL_MEDIUM:
			if ( gGameOptions.fGunNut )  nazwa ="prof_TonsOfGuns"; else  nazwa ="prof_NormalGuns";
				
				
				for( i = 0; i < 500; i++ )
				{	
				sprintf( zFileName, "TABLEDATA\\Profile\\Prof_Experienced\\prof%03d.ini", i );			
				MERC_INIT (zFileName,i,nazwa);			
				}
					
			break;
		
		case DIF_LEVEL_HARD:
			if ( gGameOptions.fGunNut )  nazwa ="prof_TonsOfGuns"; else  nazwa ="prof_NormalGuns";
				
				
				for( i = 0; i < 500; i++ )
				{	
				sprintf( zFileName, "TABLEDATA\\Profile\\Prof_Expert\\prof%03d.ini", i );			
				MERC_INIT (zFileName,i,nazwa);			
				}	
			
			break;
			
		case DIF_LEVEL_INSANE:
			if ( gGameOptions.fGunNut )  nazwa ="prof_TonsOfGuns"; else  nazwa ="prof_NormalGuns";
				
				
				for( i = 0; i < 500; i++ )
				{	
				sprintf( zFileName, "TABLEDATA\\Profile\\Prof_Insane\\prof%03d.ini", i );			
				MERC_INIT (zFileName,i,nazwa);			
				}
						
			break;
			
		default:

			break;
	}
	
	#ifdef JA2TESTVERSION
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"Skyrider in %c %d", 'A' + pProfile->sSectorY - 1, pProfile->sSectorX );
	#endif
	// use alternate map, with Skyrider's shack, in this sector
	SectorInfo[ SECTOR( pProfile->sSectorX, pProfile->sSectorY ) ].uiFlags |= SF_USE_ALTERNATE_MAP;


	// set up Madlab's secret lab (he'll be added when the meanwhile scene occurs)

	switch( Random( 4 ) )
	{
		case 0:
			// use alternate map in this sector
			SectorInfo[ SECTOR( 7, MAP_ROW_H ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
			break;
		case 1:
			SectorInfo[ SECTOR( 16, MAP_ROW_H ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
			break;
		case 2:
			SectorInfo[ SECTOR( 11, MAP_ROW_I ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
			break;
		case 3:
			SectorInfo[ SECTOR( 4, MAP_ROW_E ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
			break;
	}
	
	// add Micky in random location

	pProfile = &(gMercProfiles[MICKY]);
	switch( Random( 5 ) )
	{
		case 0:
			pProfile->sSectorX = 9;
			pProfile->sSectorY = MAP_ROW_G;
			pProfile->bSectorZ = 0;
			break;
		case 1:
			pProfile->sSectorX = 13;
			pProfile->sSectorY = MAP_ROW_D;
			pProfile->bSectorZ = 0;
			break;
		case 2:
			pProfile->sSectorX = 5;
			pProfile->sSectorY = MAP_ROW_C;
			pProfile->bSectorZ = 0;
			break;
		case 3:
			pProfile->sSectorX = 2;
			pProfile->sSectorY = MAP_ROW_H;
			pProfile->bSectorZ = 0;
			break;
		case 4:
			pProfile->sSectorX = 6;
			pProfile->sSectorY = MAP_ROW_C;
			pProfile->bSectorZ = 0;
			break;
	}



	#ifdef JA2TESTVERSION
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"%s in %c %d", pProfile->zNickname, 'A' + pProfile->sSectorY - 1, pProfile->sSectorX );
	#endif

	// use alternate map in this sector
	//SectorInfo[ SECTOR( pProfile->sSectorX, pProfile->sSectorY ) ].uiFlags |= SF_USE_ALTERNATE_MAP;

  gfPlayerTeamSawJoey = FALSE;


	if ( gGameOptions.ubGameStyle == STYLE_SCIFI  && gGameExternalOptions.fEnableCrepitus )
	{
		// add Bob
		pProfile = &(gMercProfiles[BOB]);
		pProfile->sSectorX = 8;
		pProfile->sSectorY = MAP_ROW_F;
		pProfile->bSectorZ = 0;

		// add Gabby in random location
		pProfile = &(gMercProfiles[GABBY]);
		switch( Random( 2 ) )
		{
			case 0:
				pProfile->sSectorX = 11;
				pProfile->sSectorY = MAP_ROW_H;
				pProfile->bSectorZ = 0;
				break;
			case 1:
				pProfile->sSectorX = 4;
				pProfile->sSectorY = MAP_ROW_I;
				pProfile->bSectorZ = 0;
				break;
		}

		#ifdef JA2TESTVERSION
			ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"%s in %c %d", pProfile->zNickname, 'A' + pProfile->sSectorY - 1, pProfile->sSectorX );
		#endif

		// use alternate map in this sector
		SectorInfo[ SECTOR( pProfile->sSectorX, pProfile->sSectorY ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
	}
	else
	{ //not scifi, so use alternate map in Tixa's b1 level that doesn't have the stairs going down to the caves.
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( 9, 10, 1 ); //j9_b1
		if( pSector )
		{
			pSector->uiFlags |= SF_USE_ALTERNATE_MAP;
		}
	}

	// init hospital variables
	giHospitalTempBalance = 0;
	giHospitalRefund = 0;
	gbHospitalPriceModifier = 0;

	// set up Devin so he will be placed ASAP
	gMercProfiles[ DEVIN ].bNPCData = 3;
}

void InitBloodCatSectors()
{
	INT32 i,ie,im,ih,in;
	INT8 ST_ILOSC, ST_ILOSC2;
	STR8 S; 
	//Hard coded table of bloodcat populations.  We don't have
	//access to the real population (if different) until we physically 
	//load the map.  If the real population is different, then an error
	//will be reported.
	for( i = 0; i < 255; i++ )
	{
		SectorInfo[ i ].bBloodCats = -1;
	}
	
	/*
	SectorInfo[ SEC_A15	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_B4	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_B16	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_C3	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_C8	].bBloodCatPlacements = 13;
	SectorInfo[ SEC_C11	].bBloodCatPlacements = 7;
	SectorInfo[ SEC_D4	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_D9	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_E11	].bBloodCatPlacements = 10;
	SectorInfo[ SEC_E13	].bBloodCatPlacements = 14;
	SectorInfo[ SEC_F3	].bBloodCatPlacements = 13;
	SectorInfo[ SEC_F5	].bBloodCatPlacements = 7;
	SectorInfo[ SEC_F7	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_F12	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_F14	].bBloodCatPlacements = 14;
	SectorInfo[ SEC_F15	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_G6	].bBloodCatPlacements = 7;
	SectorInfo[ SEC_G10	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_G12	].bBloodCatPlacements = 11;
	SectorInfo[ SEC_H5	].bBloodCatPlacements = 9;
	SectorInfo[ SEC_I4	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_I15	].bBloodCatPlacements = 8;
	SectorInfo[ SEC_J6	].bBloodCatPlacements = 11;
	SectorInfo[ SEC_K3	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_K6	].bBloodCatPlacements = 14;
	SectorInfo[ SEC_K10	].bBloodCatPlacements = 12;
	SectorInfo[ SEC_K14	].bBloodCatPlacements = 14;
	
	*/

	

CIniReader iniReader("TABLEDATA\\BloodCats.ini");

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY: //50%
			for( ie = 0; ie < 255; ie++ )
			{
				S = TABLICA_SEKTORY [ie];
				ST_ILOSC		= iniReader.ReadInteger(S,"BloodCat_Easy",-1);	
				ST_ILOSC2		= iniReader.ReadInteger(S,"BloodCatPlacements",-1);
				SectorInfo[ ie ].bBloodCatPlacements = ST_ILOSC2;
				SectorInfo[ ie ].bBloodCats = ST_ILOSC;
			}

			break;

		case DIF_LEVEL_MEDIUM: //75%
		for( im = 0; im < 255; im++ )
			{
				S = TABLICA_SEKTORY [im];
				ST_ILOSC		= iniReader.ReadInteger(S,"BloodCat_Medium",-1);	
				ST_ILOSC2		= iniReader.ReadInteger(S,"BloodCatPlacements",-1);
				SectorInfo[ im ].bBloodCatPlacements = ST_ILOSC2;
				SectorInfo[ im ].bBloodCats = ST_ILOSC;
			} 
			break;

		case DIF_LEVEL_HARD: //100%
		for( ih = 0; ih < 255; ih++ )
			{
				S = TABLICA_SEKTORY [ih];
				ST_ILOSC		= iniReader.ReadInteger(S,"BloodCat_Hard",-1);	
				ST_ILOSC2		= iniReader.ReadInteger(S,"BloodCatPlacements",-1);
				SectorInfo[ ih ].bBloodCatPlacements = ST_ILOSC2;
				SectorInfo[ ih ].bBloodCats = ST_ILOSC;
			} 
			break;

		case DIF_LEVEL_INSANE: //150%
			for( in = 0; in < 255; in++ )
			{
				S = TABLICA_SEKTORY [in];
				ST_ILOSC		= iniReader.ReadInteger(S,"BloodCat_Insane",-1);	
				ST_ILOSC2		= iniReader.ReadInteger(S,"BloodCatPlacements",-1);
				SectorInfo[ in ].bBloodCatPlacements = ST_ILOSC2;
				SectorInfo[ in ].bBloodCats = ST_ILOSC;
			} 
			break;
	}

}


void InitStrategicLayer( void )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitStrategicLayer");

	// Clear starategic layer!
	SetupNewStrategicGame();
	InitQuestEngine();

	//Setup a new campaign via the enemy perspective.
	InitNewCampaign();
	// Init Squad Lists
	InitSquads();
	// Init vehicles
	InitVehicles( );
	// init town loyalty
	InitTownLoyalty();
	// init the mine management system
	InitializeMines();
	// initialize map screen flags
	InitMapScreenFlags();
	// initialize NPCs, select alternate maps, etc
	InitNPCs();
	// init Skyrider and his helicopter
	InitializeHelicopter();
	//Clear out the vehicle list
	ClearOutVehicleList();

	InitBloodCatSectors();

	InitializeSAMSites();

	// make Orta, Tixa, SAM sites not found
	InitMapSecrets();


	// free up any leave list arrays that were left allocated
	ShutDownLeaveList( );
	// re-set up leave list arrays for dismissed mercs
	InitLeaveList( );

	// reset time compression mode to X0 (this will also pause it)
	SetGameTimeCompressionLevel( TIME_COMPRESS_X0 );

	// select A9 Omerta as the initial selected sector
	ChangeSelectedMapSector(  gGameExternalOptions.SektorX4,  gGameExternalOptions.SektorY4, 0 );

	// Reset these flags or mapscreen could be disabled and cause major headache.
	fDisableDueToBattleRoster = FALSE;
	fDisableMapInterfaceDueToBattle = FALSE;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitStrategicLayer done");
}

void ShutdownStrategicLayer()
{
	DeleteAllStrategicEvents();
	RemoveAllGroups();
	TrashUndergroundSectorInfo();
	DeleteCreatureDirectives();
	KillStrategicAI();
}

BOOLEAN InitNewGame( BOOLEAN fReset )
{
	INT32		iStartingCash;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame");
//	static fScreenCount = 0;

	if( fReset )
	{
		gubScreenCount = 0;
		return( TRUE );
	}

	// reset meanwhile flags
	uiMeanWhileFlags = 0;

	// Reset the selected soldier
	gusSelectedSoldier = NOBODY;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: loading merc profiles");
	if( gubScreenCount == 0 )
	{		
		if( !LoadMercProfiles() )
			return(FALSE);
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: init arms dealers");
	//Initialize the Arms Dealers and Bobby Rays inventory
	if( gubScreenCount == 0 )
	{
		//Init all the arms dealers inventory
		InitAllArmsDealers();
		InitBobbyRayInventory();
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: clearing messages");
	// clear tactical 
	ClearTacticalMessageQueue( );

	// clear mapscreen messages
	FreeGlobalMessageList();

	// IF our first time, go into laptop!
	if ( gubScreenCount == 0 )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame: first time: init laptop");
		//Init the laptop here
		InitLaptopAndLaptopScreens();

		InitStrategicLayer();

		// Set new game flag
		SetLaptopNewGameFlag( );

		// this is for the "mercs climbing down from a rope" animation, NOT Skyrider!!
		ResetHeliSeats( );

		// Setup two new messages!
		AddPreReadEmail(OLD_ENRICO_1,OLD_ENRICO_1_LENGTH,MAIL_ENRICO,  GetWorldTotalMin() );
		AddPreReadEmail(OLD_ENRICO_2,OLD_ENRICO_2_LENGTH,MAIL_ENRICO,  GetWorldTotalMin() );
		AddPreReadEmail(RIS_REPORT,RIS_REPORT_LENGTH,RIS_EMAIL,  GetWorldTotalMin() );
		AddPreReadEmail(OLD_ENRICO_3,OLD_ENRICO_3_LENGTH,MAIL_ENRICO,  GetWorldTotalMin() );
		AddEmail(IMP_EMAIL_INTRO,IMP_EMAIL_INTRO_LENGTH,CHAR_PROFILE_SITE,  GetWorldTotalMin(), -1);
		//AddEmail(ENRICO_CONGRATS,ENRICO_CONGRATS_LENGTH,MAIL_ENRICO, GetWorldTotalMin() );
		if(gGameExternalOptions.fMercDayOne)
		{
			AddEmail(MERC_INTRO, MERC_INTRO_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin( ), -1 );
		}



		// ATE: Set starting cash....
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:

				iStartingCash = gGameExternalOptions.iStartingCashNovice;
				//iStartingCash	= 45000;
				break;

			case DIF_LEVEL_MEDIUM:

				iStartingCash = gGameExternalOptions.iStartingCashExperienced;
				//iStartingCash	= 35000;
				break;

			case DIF_LEVEL_HARD:

				iStartingCash = gGameExternalOptions.iStartingCashExpert;
				//iStartingCash	= 30000;
				break;

			case DIF_LEVEL_INSANE:

				iStartingCash = gGameExternalOptions.iStartingCashInsane; 
				// iStartingCash	= 15000;
				break;

			default:
				Assert(0);
				return( FALSE );
		}

		// Setup initial money
 		AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), iStartingCash );


		{
			UINT32	uiDaysTimeMercSiteAvailable = Random( 2 ) + 1;

			// schedule email for message from spec at 7am 3 days in the future
			AddFutureDayStrategicEvent( EVENT_DAY3_ADD_EMAIL_FROM_SPECK, 60*7, 0, uiDaysTimeMercSiteAvailable );
		}

#ifdef CRIPPLED_VERSION
		{
			UINT32 cnt;

			//loop through the first 20 AIM mercs and set them to be away
			for( cnt = 0; cnt < 20; cnt++)
			{
				gMercProfiles[ cnt ].bMercStatus = MERC_WORKING_ELSEWHERE;
				gMercProfiles[ cnt ].uiDayBecomesAvailable = 14;		// 14 days should be ok considering crippled version only goes to day 7
			}
		}

		//Add an event to check for the end of the crippled version 
		AddEveryDayStrategicEvent( EVENT_CRIPPLED_VERSION_END_GAME_CHECK, 0, 0 );
#endif


		SetLaptopExitScreen( INIT_SCREEN );
		SetPendingNewScreen(LAPTOP_SCREEN);
		gubScreenCount = 1;

		//Set the fact the game is in progress
		gTacticalStatus.fHasAGameBeenStarted = TRUE;

		return( TRUE );
	}

	/*
	if( ( guiExitScreen == MAP_SCREEN ) && ( LaptopSaveInfo.gfNewGameLaptop ) )
	{
		SetLaptopExitScreen( GAME_SCREEN );
		return( TRUE );
	}
*/
	if ( gubScreenCount == 1 )
	{
		// OK , FADE HERE
		//BeginFade( INIT_SCREEN, 35, FADE_OUT_REALFADE, 5 );
		//BeginFade( INIT_SCREEN, 35, FADE_OUT_VERSION_FASTER, 25 );
		//BeginFade( INIT_SCREEN, 35, FADE_OUT_VERSION_SIDE, 0 );


		gubScreenCount = 2;
		return( TRUE );
	}

/*
	if ( gubScreenCount == 2 )
	{
			
		if ( !SetCurrentWorldSector( 9, 1, 0 ) )
		{

		}

		SetLaptopExitScreen( MAP_SCREEN );

		FadeInGameScreen( );

		EnterTacticalScreen( );

		if( gfAtLeastOneMercWasHired == TRUE )
		{  
			gubScreenCount = 3;
		}
		else
		{
			
		}

		return( TRUE );
	}

	*/

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewGame done");
	return( TRUE );
}


BOOLEAN AnyMercsHired( )
{
	INT32 cnt;
	SOLDIERTYPE		*pTeamSoldier;
	INT16				  bLastTeamID;

	// Find first guy availible in team
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;

  // look for all mercs on the same team, 
  for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pTeamSoldier++)
	{	
		if ( pTeamSoldier->bActive )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}
 

void QuickStartGame( )
{
	INT32		cnt;
	UINT16	usVal;
	UINT8 ub1 = 0, ub2 = 0;

	for ( cnt = 0; cnt < 3; cnt++ )
	{
		if ( cnt == 0 )
		{
			usVal = (UINT16)Random( 40 );

			QuickSetupOfMercProfileItems( cnt, (UINT8)usVal );
			QuickGameMemberHireMerc( (UINT8)usVal );
		}
		else if ( cnt == 1 )
		{
			do
			{
				usVal = (UINT16)Random( 40 );
			}
			while( usVal != ub1 );

			QuickSetupOfMercProfileItems( cnt, (UINT8)usVal );
			QuickGameMemberHireMerc( (UINT8)usVal );
		}
		else if ( cnt == 2 )
		{
			do
			{
				usVal = (UINT16)Random( 40 );
			} 
			while( usVal != ub1 && usVal != ub2 );

			QuickSetupOfMercProfileItems( cnt, (UINT8)usVal );
			QuickGameMemberHireMerc( (UINT8)usVal );
		}

	}
}


// TEMP FUNCTION!
void QuickSetupOfMercProfileItems( UINT32 uiCount, UINT8 ubProfileIndex )
{
	// Quickly give some guys we hire some items
 
	if ( uiCount == 0 )
	{
		//CreateGun( GLOCK_17, &(pSoldier->inv[ HANDPOS ] ) );
		//gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = HAND_GRENADE;
		//gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		//gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 3;
		
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = C7;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK1POS ] = CAWS;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK1POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK1POS ] = 1;

		gMercProfiles[ ubProfileIndex ].bSkillTrait = MARTIALARTS;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK3POS ] = KEY_2;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK3POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK5POS ] = LOCKSMITHKIT;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK5POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK5POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK3POS ] = MEDICKIT;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK3POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK4POS ] = SHAPED_CHARGE;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK4POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK4POS ] = 1;


		// TEMP!
		// make carman's opinion of us high!
		gMercProfiles[ 78 ].bMercOpinion[ ubProfileIndex ] = 25; 

	}
	else if ( uiCount == 1 )
	{
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = CAWS;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK3POS ] = KEY_1;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK3POS ] = 1;


	}
	else if ( uiCount == 2 )
	{
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = GLOCK_17;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ HANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SECONDHANDPOS ] = 5;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SECONDHANDPOS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SECONDHANDPOS ] = 1;


		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK1POS ] = SILENCER;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK1POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK1POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK2POS ] = SNIPERSCOPE;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK2POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK2POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK3POS ] = LASERSCOPE;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK3POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK3POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK5POS ] = BIPOD;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK5POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK5POS ] = 1;

		gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK6POS ] = LOCKSMITHKIT;
		gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK6POS ] = 100;
		gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK6POS ] = 1;

	}
	else
	{
		gMercProfiles[ ubProfileIndex ].inv[ HANDPOS ] = (UINT8)Random(30);
		gMercProfiles[ ubProfileIndex ].bInvNumber[ HANDPOS ] = 1;


	}

	gMercProfiles[ ubProfileIndex ].inv[ HELMETPOS ] = KEVLAR_HELMET;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ HELMETPOS ] = 100;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ HELMETPOS ] = 1;

	gMercProfiles[ ubProfileIndex ].inv[ VESTPOS ] = KEVLAR_VEST;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ VESTPOS ] = 100;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ VESTPOS ] = 1;

	gMercProfiles[ ubProfileIndex ].inv[ BIGPOCK2POS ] = RDX;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ BIGPOCK2POS ] = 10;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ BIGPOCK2POS ] = 1;
	
	gMercProfiles[ ubProfileIndex ].inv[ SMALLPOCK4POS ] = HAND_GRENADE;
	gMercProfiles[ ubProfileIndex ].bInvStatus[ SMALLPOCK4POS ] = 100;
	gMercProfiles[ ubProfileIndex ].bInvNumber[ SMALLPOCK4POS ] = 4;

	// Give special items to some NPCs
	//gMercProfiles[ 78 ].inv[ SMALLPOCK4POS ] = TERRORIST_INFO;
	//gMercProfiles[ 78 ].bInvStatus[ SMALLPOCK4POS ] = 100;
	//gMercProfiles[ 78 ].bInvNumber[ SMALLPOCK4POS ] = 1;

}


BOOLEAN QuickGameMemberHireMerc( UINT8 ubCurrentSoldier )
{
	MERC_HIRE_STRUCT HireMercStruct;

	memset(&HireMercStruct, 0, sizeof(MERC_HIRE_STRUCT));

	HireMercStruct.ubProfileID = ubCurrentSoldier;

	HireMercStruct.sSectorX = gsMercArriveSectorX;
	HireMercStruct.sSectorY = gsMercArriveSectorY;
	HireMercStruct.fUseLandingZoneForArrival = TRUE;

	HireMercStruct.fCopyProfileItemsOver =	TRUE;
	HireMercStruct.ubInsertionCode				= INSERTION_CODE_CHOPPER;

	HireMercStruct.iTotalContractLength = 7;

	//specify when the merc should arrive
	HireMercStruct.uiTimeTillMercArrives = 0;

	//if we succesfully hired the merc
	if( !HireMerc( &HireMercStruct ) )
	{
		return(FALSE);
	}
	
	//add an entry in the finacial page for the hiring of the merc
	AddTransactionToPlayersBook(HIRED_MERC, ubCurrentSoldier, GetWorldTotalMin(), -(INT32) gMercProfiles[ubCurrentSoldier].uiWeeklySalary );
	
	if( gMercProfiles[ ubCurrentSoldier ].bMedicalDeposit )
	{
		//add an entry in the finacial page for the medical deposit
		AddTransactionToPlayersBook(MEDICAL_DEPOSIT, ubCurrentSoldier, GetWorldTotalMin(), -(gMercProfiles[ubCurrentSoldier].sMedicalDepositAmount) );
	}

	//add an entry in the history page for the hiring of the merc
	AddHistoryToPlayersLog( HISTORY_HIRED_MERC_FROM_AIM, ubCurrentSoldier, GetWorldTotalMin(), -1, -1 );

	return(TRUE);
}




//This function is called when the game is REstarted.  Things that need to be reinited are placed in here
void ReStartingGame()
{
	UINT16	cnt;

	//Pause the game
	gfGamePaused = TRUE;

	//Reset the sectors
	gWorldSectorX = gWorldSectorY = 0;
	gbWorldSectorZ = -1;

	SoundStopAll( );

	//we are going to restart a game so initialize the variable so we can initialize a new game
	InitNewGame( TRUE );

	//Deletes all the Temp files in the Maps\Temp directory
	InitTacticalSave( TRUE );

	//Loop through all the soldier and delete them all
	for( cnt=0; cnt< TOTAL_SOLDIERS; cnt++)
	{	
		TacticalRemoveSoldier( cnt );
	}

	// Re-init overhead...
	InitOverhead( );

	//Reset the email list
	ShutDownEmailList();

	//Reinit the laptopn screen variables
	InitLaptopAndLaptopScreens();
	LaptopScreenInit();

	//Reload the Merc profiles
	LoadMercProfiles( );
	

	// Reload quote files
	ReloadAllQuoteFiles();

	//Initialize the ShopKeeper Interface ( arms dealer inventory, etc. )
	ShopKeeperScreenInit();

	//Delete the world info
	TrashWorld();

	//Init the help screen system
	InitHelpScreenSystem();

	EmptyDialogueQueue( );

	if ( InAirRaid( ) )
	{
		EndAirRaid( );
	}	

#ifdef JA2TESTVERSION
	//Reset so we can use the 'cheat key' to start with mercs
	TempHiringOfMercs( 0, TRUE );
#endif

	//Make sure the game starts in the TEAM panel ( it wasnt being reset )
	gsCurInterfacePanel = TEAM_PANEL;

	//Delete all the strategic events
	DeleteAllStrategicEvents();

	//This function gets called when ur in a game a click the quit to main menu button, therefore no game is in progress
	gTacticalStatus.fHasAGameBeenStarted = FALSE;

	// Reset timer callbacks
	gpCustomizableTimerCallback = NULL;

	gubCheatLevel = STARTING_CHEAT_LEVEL;
}