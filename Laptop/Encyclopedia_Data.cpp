#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "Laptop All.h"
	#include "laptop.h"
	#include "aim.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "WordWrap.h"
	#include "Text.h"
	#include "Multi Language Graphic Utils.h"
	#include "Encrypted File.h"
	#include "Soldier Profile.h"
#endif	

#include "Encyclopedia_Data.h"
#include "Encyclopedia.h"
#include "Quests.h"
#include "Tactical Save.h"

#define MAX_FILTR_LOCATION_BUTTONS 11
#define FILTR_INVENTORY_BUTTONS 5
#define FILTR_BUTTONS_CHARACTER 8
#define NO_FILTR -1
#define NONE_FILTR 0

INT32 	guiFiltrPageButton[MAX_FILTR_LOCATION_BUTTONS] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
INT32  	guiFiltrPageButtonImage = -1;

INT32 	guiFiltrInventoryButton[FILTR_INVENTORY_BUTTONS] = { -1,-1,-1,-1,-1 };
INT32  	guiFiltrInventoryButtonImage = -1;
MOUSE_REGION	gSelectedEncyclopediaInventoryTextRegion[ 4 ];
void SelectInventoryFiltrButtonsRegionCallBack(GUI_BUTTON * btn, INT32 reason );

INT32 	guiFiltrAddInventoryButton[11] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
INT32  	guiFiltrAddInventoryButtonImage = -1;
MOUSE_REGION	gSelectedEncyclopediaAddInventoryTextRegion[ 11 ];

UINT32 	gFiltrBox = -1;
INT32 	guiEncyclopediaLocationPageButton[3] = { -1,-1,-1 };
INT32  	guiEncyclopediaLocationPageButtonImage = -1;
INT32 	guiEncyclopediaPageButton[3] = { -1,-1,-1 };
INT32  	guiEncyclopediaiPageButtonImage =  -1;
UINT32 	guiEncyclopediaPopUp;
INT32 	ID = 0;
INT32 	IDimage = 1;
INT32 	MaxImages = 1;
INT32 	IDNewLocation = 0;
UINT32 	LocationID = 0;
INT32 	MaxLocation = 0;
INT32 	ENCYCLOPEDIA_PAGE =	2;
BOOLEAN	fFirstTimeInEncyclopediaLocation = TRUE;
UINT32 FiltrID = 0;
BOOLEAN ResetVal = FALSE;

UINT32 IDPageEncyData;
void InitData ( BOOLEAN bInit );

#define		ENCYCLOPEDIA_LOCATION_PAGE1_X					(LAPTOP_SCREEN_UL_X + 100)
#define		ENCYCLOPEDIA_LOCATION_PAGE1_Y					(LAPTOP_SCREEN_WEB_UL_Y + 357)
#define		ENCYCLOPEDIA_LOCATION_FILTER_Y					(LAPTOP_SCREEN_WEB_UL_Y + 10)
#define		ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP				FONT_MCOLOR_DKWHITE
#define		ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN			138
#define		ENCYCLOPEDIA_LOCATION_PAGE_FONT					FONT10ARIAL
#define		ENCYCLOPEDIA_LOCATION_PAGE_GAP					ENCYCLOPEDIA_LOCATION_BOTTOM_BUTTON_START_WIDTH + 25
#define		ENCYCLOPEDIA_LOCATION_BOTTOM_BUTTON_START_WIDTH	75
#define		ENCYCLOPEDIA_LOCATION_BOX_X						LAPTOP_SCREEN_UL_X + (500-ENCYCLOPEDIA_LOCATION_BOX_WIDTH)/2
#define		ENCYCLOPEDIA_LOCATION_BOX_Y						iScreenHeightOffset + 260 + LAPTOP_SCREEN_WEB_DELTA_Y
#define		ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP			4
#define		ENCYCLOPEDIA_LOCATION_BOX_WIDTH					400//309
#define		ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH			400//296
#define		ENCYCLOPEDIA_LOCATION_BOX_SECTION_HEIGHT		9
#define		ENCYCLOPEDIA_LOCATION_BOX_FONT					FONT10ARIAL
#define		ENCYCLOPEDIA_LOCATION_BOX_COLOR					FONT_MCOLOR_WHITE
#define		ENCYCLOPEDIA_LOCATION_BOX_DESC_X				ENCYCLOPEDIA_LOCATION_BOX_X + 8
#define		ENCYCLOPEDIA_LOCATION_PANEL_HEIGHT				0//52
#define		ENCYCLOPEDIA_LOCATION_PANEL_Y					ENCYCLOPEDIA_LOCATION_BOX_Y + 6
#define		ENCYCLOPEDIA_LOCATION_BOX_DESC_Y				ENCYCLOPEDIA_LOCATION_PANEL_Y + ENCYCLOPEDIA_LOCATION_PANEL_HEIGHT + 5

#define ENCYCLOPEDIA_IMAGE_X LAPTOP_SCREEN_UL_X
#define ENCYCLOPEDIA_IMAGE_Y iScreenHeightOffset + LAPTOP_SCREEN_WEB_DELTA_Y
#define ENCYCLOPEDIA_FILTR_BUTTON_GAP	16
void SelectEncyclopediaLocationPageRegionCallBack(GUI_BUTTON * btn, INT32 reason );
void SelectEncyclopediaLocationRegionCallBack(GUI_BUTTON * btn, INT32 reason );
void SelectFiltrButtonsRegionCallBack(GUI_BUTTON * btn, INT32 reason );

//void BtnEncyclopediaTogglesCallback( GUI_BUTTON *btn, INT32 reason );
void RenderButtonDisabled();
void RenderButtonDisabled2();
void RenderButtonDisabled3();
void RenderButtonDisabled4();
void RenderBoxDisabledButton();
BOOLEAN  RenderMap();
void CopyToTemp ( ENCYCLOPEDIA_LOCATION *Ency, BOOLEAN bFiltr, INT32 sort, INT32 TypFiltr, BOOLEAN ShowBox );
void ResetTemp();
void UnLoadMenuButtons ();

#define MAX_NUMBER_OF_OPTION_TOGGLES 5

//UINT32	guiEncyclopediaToggles[ MAX_NUMBER_OF_OPTION_TOGGLES ];
MOUSE_REGION	gSelectedEncyclopediaTextRegion[ MAX_FILTR_LOCATION_BUTTONS ];


BOOLEAN bEncyclopediaLocation = FALSE;
BOOLEAN bEncyclopediaCharacter = FALSE;
BOOLEAN bEncyclopediaInventory = FALSE;
BOOLEAN bEncyclopediaQuests  = FALSE;
BOOLEAN bBriefingRoom  = FALSE;
BOOLEAN bBriefingRoomSpecialMission  = FALSE;
BOOLEAN bRenderFiltrBox[5];

#define MAX_MISSION_BUTTONS 2

BOOLEAN bSoundButtons = FALSE;
UINT32 	guiSoundButtons[ MAX_MISSION_BUTTONS ];
UINT32	uiSoundSampleBR;
INT32  	guiSoundButtonsImage = -1;
MOUSE_REGION	gSelectedSoundButtonTextRegion[ MAX_MISSION_BUTTONS ];
void SelectSoundButtonsRegionCallBack(GUI_BUTTON * btn, INT32 reason );

BOOLEAN LoadEncyclopediaFromLoadGameFile( HWFILE hFile );
BOOLEAN SaveEncyclopediaToSaveGameFile( HWFILE hFile );

BOOLEAN DisplayEncyclopediaLocationText()
{
	return(TRUE);
}

void GameInitEncyclopediaLocation()
{

}

void ResetTemp()
{
UINT32 i,NUM_TEMP;

  if ( bEncyclopediaLocation == TRUE )
		NUM_TEMP = NUM_SECTOR;
  else if ( bEncyclopediaCharacter == TRUE )
		NUM_TEMP = NUM_PROFILES;
  else if ( bEncyclopediaInventory == TRUE )
		NUM_TEMP = MAXITEMS;
  else if ( bEncyclopediaQuests == TRUE )
		NUM_TEMP = MAX_QUESTS;
  else if ( bBriefingRoom == TRUE || bBriefingRoomSpecialMission  == TRUE)
		NUM_TEMP = NUM_SECTOR;
  else
		NUM_TEMP = NUM_SECTOR;
		
  	for(i=0; i<NUM_SECTOR; i++)
	{
			gEncyclopediaDataTemp[i].uiIndex = i;
			
			//strncpy(gEncyclopediaLocationTextTemp[i].szFile, '\0',MAX_ENCYCLOPEDIA_CHARS);
			//strncpy(gEncyclopediaLocationTextTemp[i].szFile2, '\0',MAX_ENCYCLOPEDIA_CHARS);
			//strncpy(gEncyclopediaLocationTextTemp[i].szFile3, '\0',MAX_ENCYCLOPEDIA_CHARS);
			//strncpy(gEncyclopediaLocationTextTemp[i].szFile4, '\0',MAX_ENCYCLOPEDIA_CHARS);
			
			gEncyclopediaDataTemp[i].Name[ENCYCLOPEDIA_NAME_SIZE]  = '\0';
			gEncyclopediaDataTemp[i].sDesc1[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc2[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc3[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc4[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc5[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc6[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc7[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc8[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc9[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc10[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc11[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc12[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc13[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc14[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc15[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc16[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc17[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc18[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc19[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sDesc20[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			
			gEncyclopediaDataTemp[i].sImageDesc1[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sImageDesc2[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sImageDesc3[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].sImageDesc4[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			
			gEncyclopediaDataTemp[i].sCode[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			
			gEncyclopediaDataTemp[i].sImagePositionX[0] = 0;
			gEncyclopediaDataTemp[i].sImagePositionY[0] = 0;
					
			gEncyclopediaDataTemp[i].sImagePositionX[1] = 0;
			gEncyclopediaDataTemp[i].sImagePositionY[1] = 0;		
					
			gEncyclopediaDataTemp[i].sImagePositionX[2] = 0;
			gEncyclopediaDataTemp[i].sImagePositionY[2] = 0;
					
			gEncyclopediaDataTemp[i].sImagePositionX[3] = 0;
			gEncyclopediaDataTemp[i].sImagePositionY[3] = 0;
			
			gEncyclopediaDataTemp[i].Hidden = FALSE;
			gEncyclopediaDataTemp[i].SectorX = -1;
			gEncyclopediaDataTemp[i].SectorY = -1;
			gEncyclopediaDataTemp[i].SectorZ = -1;
			
			gEncyclopediaDataTemp[i].MaxPages = 0;
			gEncyclopediaDataTemp[i].MaxImages = -1;
			
			//Only locations
			gEncyclopediaDataTemp[i].LocType = -1;
	
			//Only profiles
			gEncyclopediaDataTemp[i].IDProfile = -1;
			gEncyclopediaDataTemp[i].Filtr = FALSE;
			gEncyclopediaDataTemp[i].enableDesc = FALSE;
			gEncyclopediaDataTemp[i].subGroup1 = -1;
			gEncyclopediaDataTemp[i].bvisible = FALSE;	
			
			//Inventory
			gEncyclopediaDataTemp[i].InventoryClass = -1;
						
			//Quest
			gEncyclopediaDataTemp[i].QuestID = -1;
			gEncyclopediaDataTemp[i].pQuests = -1;
			gEncyclopediaDataTemp[i].SpecialQuestImage = FALSE;
			//BeforeImage[MAX_ENCYCLOPEDIA_CHARS];
			//AfterImage[MAX_ENCYCLOPEDIA_CHARS];
			gEncyclopediaDataTemp[i].ImagePositionQX[0] = 0;
			gEncyclopediaDataTemp[i].ImagePositionQY[0] = 0;
			gEncyclopediaDataTemp[i].ImagePositionQX[1] = 0;
			gEncyclopediaDataTemp[i].ImagePositionQY[1] = 0;
			gEncyclopediaDataTemp[i].ImageDescBefore[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			gEncyclopediaDataTemp[i].ImageDescAfter[ENCYCLOPEDIA_DECRIPTION_SIZE] = '\0';
			
			gEncyclopediaDataTemp[i].MissionID = i;
			gEncyclopediaDataTemp[i].NextMission = -1;
			
			gEncyclopediaDataTemp[i].CheckMission = 0;	
	
			
	}
}

void CopyToTemp ( ENCYCLOPEDIA_LOCATION *Ency, BOOLEAN bFiltr, INT32 sort, INT32 TypFiltr, BOOLEAN ShowBox )
{
	UINT32 i, NUM_TEMP;
	INT32 IDClass;
	BOOLEAN bBoxShow;
	ID = 0;
	IDimage = 1;
	MaxImages = 1;
	IDNewLocation = 0;
	LocationID = 0;
	MaxLocation = 0;
    bBoxShow = TRUE;
	ResetVal = FALSE;
	
  if ( bEncyclopediaLocation == TRUE )
		NUM_TEMP = NUM_SECTOR;
  else if ( bEncyclopediaCharacter == TRUE )
		NUM_TEMP = NUM_PROFILES;
  else if ( bEncyclopediaInventory == TRUE )
		NUM_TEMP = MAXITEMS;
  else if ( bEncyclopediaQuests == TRUE )
		NUM_TEMP = MAX_QUESTS;
  else if ( bBriefingRoom == TRUE || bBriefingRoomSpecialMission  == TRUE )
		NUM_TEMP = NUM_SECTOR;
  else 
		NUM_TEMP = NUM_SECTOR;
		
	// test quests
  	for(i=0; i<23; i++)
	{
	  //gubQuest[ i ] = QUESTINPROGRESS;
      //gubQuest[ i ] = QUESTDONE;
	}
		
  	for(i=0; i<NUM_TEMP; i++)
	{
	
		gEncyclopediaDataTemp[i].uiIndex = i;
		Ency[i].uiIndex = i;
			
			
		if ( bBriefingRoomSpecialMission  == TRUE )
		{
			gEncyclopediaDataTemp[MaxLocation].IDProfile = -1;
			gEncyclopediaDataTemp[MaxLocation].subGroup1 = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorX = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorY = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorZ = -1;
			Ency[i].IDProfile = -1;
			Ency[i].subGroup1 = -1;
			Ency[i].SectorX = -1;
			Ency[i].SectorY = -1;
			Ency[i].SectorZ = -1;	
			Ency[i].InventoryClass = -1;
			
			//Quests
			Ency[i].QuestID = -1;
			Ency[i].pQuests = -1;
			Ency[i].SpecialQuestImage = FALSE;
			Ency[i].ImagePositionQX[0] = 0;
			Ency[i].ImagePositionQY[0] = 0;
			Ency[i].ImagePositionQX[1] = 0;
			Ency[i].ImagePositionQY[1] = 0;
		}
		else if ( bBriefingRoom == TRUE )
		{
			gEncyclopediaDataTemp[MaxLocation].IDProfile = -1;
			gEncyclopediaDataTemp[MaxLocation].subGroup1 = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorX = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorY = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorZ = -1;
			Ency[i].IDProfile = -1;
			Ency[i].subGroup1 = -1;
			Ency[i].SectorX = -1;
			Ency[i].SectorY = -1;
			Ency[i].SectorZ = -1;	
			Ency[i].InventoryClass = -1;
			
			//Quests
			Ency[i].QuestID = -1;
			Ency[i].pQuests = -1;
			Ency[i].SpecialQuestImage = FALSE;
			Ency[i].ImagePositionQX[0] = 0;
			Ency[i].ImagePositionQY[0] = 0;
			Ency[i].ImagePositionQX[1] = 0;
			Ency[i].ImagePositionQY[1] = 0;
			
			//Mission ID
			gEncyclopediaDataTemp[MaxLocation].MissionID = Ency[i].MissionID;
			gEncyclopediaDataTemp[MaxLocation].NextMission = Ency[i].NextMission;
			gEncyclopediaDataTemp[MaxLocation].CheckMission = Ency[i].CheckMission;
		/*	
			if ( saveMissionData[i].CheckMission == MISSIONEND || saveMissionData[i].CheckMission == MISSIONNOSTARTED ) 
			{
				saveMissionData[i].CheckMission = MISSIONNOSTARTED;
				Ency[i].Hidden = TRUE;
			}
			else 
			{
				saveMissionData[i].CheckMission = MISSIONNOSTARTED;
				Ency[i].Hidden = FALSE; 
			}
		*/	
		
		}
		else if ( bEncyclopediaQuests == TRUE )	
		{
			gEncyclopediaDataTemp[MaxLocation].IDProfile = -1;
			gEncyclopediaDataTemp[MaxLocation].subGroup1 = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorX = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorY = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorZ = -1;
			Ency[i].IDProfile = -1;
			Ency[i].subGroup1 = -1;
			Ency[i].SectorX = -1;
			Ency[i].SectorY = -1;
			Ency[i].SectorZ = -1;	
			Ency[i].InventoryClass = -1;
			Ency[i].QuestID = i;
			
			if ( gubQuest[ Ency[i].QuestID ] == QUESTINPROGRESS || gubQuest[ Ency[i].QuestID ] == QUESTDONE ) 
				Ency[i].Hidden = TRUE;
			else 
				Ency[i].Hidden = FALSE; 
			
		}
		//Profiles
		else if ( bEncyclopediaLocation == TRUE )
		{
			gEncyclopediaDataTemp[MaxLocation].IDProfile = -1;
			gEncyclopediaDataTemp[MaxLocation].subGroup1 = -1;
			Ency[i].IDProfile = -1;
			Ency[i].subGroup1 = -1;
			Ency[i].InventoryClass = -1;
			//Quests
			Ency[i].QuestID = -1;
			Ency[i].pQuests = -1;
			Ency[i].SpecialQuestImage = FALSE;
			Ency[i].ImagePositionQX[0] = 0;
			Ency[i].ImagePositionQY[0] = 0;
			Ency[i].ImagePositionQX[1] = 0;
			Ency[i].ImagePositionQY[1] = 0;
			
			if( GetSectorFlagStatus( Ency[i].SectorX, Ency[i].SectorY, ( UINT8 )Ency[i].SectorZ, SF_ALREADY_VISITED ) == TRUE )
				Ency[i].Hidden = TRUE;
			else 
				Ency[i].Hidden = FALSE; 
		}
		else if ( bEncyclopediaCharacter == TRUE )
		{
			gEncyclopediaDataTemp[MaxLocation].SectorX = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorY = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorZ = -1;
			Ency[i].SectorX = -1;
			Ency[i].SectorY = -1;
			Ency[i].SectorZ = -1;	
			Ency[i].InventoryClass = -1;
			
			//Quests
			Ency[i].QuestID = -1;
			Ency[i].pQuests = -1;
			Ency[i].SpecialQuestImage = FALSE;
			Ency[i].ImagePositionQX[0] = 0;
			Ency[i].ImagePositionQY[0] = 0;
			Ency[i].ImagePositionQX[1] = 0;
			Ency[i].ImagePositionQY[1] = 0;
			
			//gEncyclopediaDataTemp[MaxLocation].IDProfile = i;
			
			Ency[i].IDProfile = i;		
				
			if ( Ency[i].LocType == 0 && gProfilesVehicle[ i ].ProfilId == i ) Ency[i].LocType = 5;
			if ( Ency[i].LocType == 0 && gProfilesAIM[ i ].ProfilId == i ) Ency[i].LocType = 1;
			if ( Ency[i].LocType == 0 && gProfilesMERC[ i ].ProfilId == i ) Ency[i].LocType = 2;
			if ( Ency[i].LocType == 0 && gProfilesNPC[ i ].ProfilId == i ) Ency[i].LocType = 4;
			if ( Ency[i].LocType == 0 && gProfilesRPC[ i ].ProfilId == i ) Ency[i].LocType = 3; 
			if ( Ency[i].LocType == 0 && gProfilesIMP[ i ].ProfilId == i ) Ency[i].LocType = 6;
			
			if ( Ency[i].LocType == 3 ) Ency[i].LocType = 4; //RPC -> NPC
			
			Ency[i].subGroup1 = 0; // set none
			
			if ( CheckNPCIsEPC ( Ency[i].IDProfile ) == TRUE || Ency[i].bvisible == TRUE && gProfilesNPC[ Ency[i].IDProfile ].ProfilId == Ency[i].IDProfile )
			{
				Ency[i].subGroup1 = 1; //set EPC
				Ency[i].bvisible = TRUE;	
			}
			else if ( CheckNPCIsRPC ( Ency[i].IDProfile ) == TRUE || Ency[i].bvisible == TRUE && gProfilesRPC[ Ency[i].IDProfile ].ProfilId == Ency[i].IDProfile )
			{
				Ency[i].subGroup1 = 3; // set RPC
				Ency[i].bvisible = TRUE;	
			}
			
			/*
			else if ( Ency[i].bvisible == FALSE )
			{
				Ency[i].subGroup1 = 0; // set none
				Ency[i].bvisible = TRUE;	
			}
			*/	

			gEncyclopediaDataTemp[MaxLocation].subGroup1 = Ency[i].subGroup1;
		}
		else if ( bEncyclopediaInventory == TRUE )
		{
			gEncyclopediaDataTemp[MaxLocation].subGroup1 = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorX = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorY = -1;
			gEncyclopediaDataTemp[MaxLocation].SectorZ = -1;
			Ency[i].SectorX = -1;
			Ency[i].SectorY = -1;
			Ency[i].SectorZ = -1;	
			Ency[i].subGroup1 = -1;
			Ency[i].IDProfile = -1;
			
			//Quests
			Ency[i].QuestID = -1;
			Ency[i].pQuests = -1;
			Ency[i].SpecialQuestImage = FALSE;
			Ency[i].ImagePositionQX[0] = 0;
			Ency[i].ImagePositionQY[0] = 0;
			Ency[i].ImagePositionQX[1] = 0;
			Ency[i].ImagePositionQY[1] = 0;
			
			if ( Item[i].usItemClass == IC_GUN )
			{
				if (Item[i].newinv && UsingNewInventorySystem() == true )
				{
					IDClass = Item[i].ubClassIndex;
					Ency[i].LocType = Weapon[IDClass].ubWeaponType;
					Ency[i].InventoryClass = Item[i].usItemClass;
				}
				else
				{
					IDClass = Item[i].ubClassIndex;
					Ency[i].LocType = Weapon[IDClass].ubWeaponType;
					Ency[i].InventoryClass = Item[i].usItemClass;
				}
			}	
			else if ( Item[i].usItemClass == IC_ARMOUR )
			{
				if (Item[i].newinv && UsingNewInventorySystem() == true )
				{
					IDClass = Item[i].ubClassIndex;
					Ency[i].InventoryClass = Item[i].usItemClass;	
					Ency[i].LocType = Armour[IDClass].ubArmourClass;
				}
				else
				{
					IDClass = Item[i].ubClassIndex;
					Ency[i].InventoryClass = Item[i].usItemClass;	
					Ency[i].LocType = Armour[IDClass].ubArmourClass;
				}
			}
			else if ( Item[i].usItemClass == IC_BLADE || Item[i].usItemClass == IC_THROWING_KNIFE || Item[i].usItemClass == IC_PUNCH ||
			          Item[i].usItemClass == IC_BOMB || Item[i].usItemClass == IC_GRENADE || Item[i].usItemClass == IC_MEDKIT ||
					  Item[i].usItemClass == IC_KIT || Item[i].usItemClass == IC_FACE || Item[i].usItemClass == IC_LBEGEAR || 
					  Item[i].usItemClass == IC_MISC )
			{
				if (Item[i].newinv && UsingNewInventorySystem() == true )
				{
					//IDClass = Item[i].ubClassIndex;
					Ency[i].LocType = Item[i].usItemClass;
					Ency[i].InventoryClass = TypFiltr;
				}
				else
				{
					//IDClass = Item[i].ubClassIndex;
					Ency[i].LocType = Item[i].usItemClass;
					Ency[i].InventoryClass = TypFiltr;
				}
			}		
			else if ( Item[i].usItemClass == IC_AMMO )
			{
				if (Item[i].newinv && UsingNewInventorySystem() == true )
				{
				IDClass = Item[i].ubClassIndex;
				//Ency[i].LocType = Weapon[IDClass].ubWeaponType;
				//Ency[i].InventoryClass = Item[i].usItemClass;
			
		// Weapon has the correct filter ammo weapon type
		  // if (Magazine[ Item[ i ].ubClassIndex ].ubCalibre == Weapon[i].ubCalibre)
			//{
				//if (Weapon[IDClass].ubWeaponType == sort)
				//{
					//Weapon has correct magazine size
					//if(Weapon[Item[i].ubClassIndex].ubMagSize == Magazine[Item[i].ubClassIndex].ubMagSize )
					//{
						Ency[i].LocType = Item[i].usItemClass; //Weapon[IDClass].ubWeaponType;
						Ency[i].InventoryClass = Item[i].usItemClass;
					//}
			//	}
			//}
			//	IDClass = Item[i].ubClassIndex;
			//	Ency[i].InventoryClass = Item[i].usItemClass;		
			//	Ency[i].LocType = Magazine[IDClass].ubCalibre; //Armour[IDClass].ubArmourClass;	
				}
				else
				{
					IDClass = Item[i].ubClassIndex;
					Ency[i].LocType = Item[i].usItemClass; //Weapon[IDClass].ubWeaponType;
					Ency[i].InventoryClass = Item[i].usItemClass;
				}
			}
		}		
		
		if ( Item[i].usItemClass == IC_NONE || Item[i].usItemClass == IC_TENTACLES ) Ency[i].Hidden = FALSE;
		
		//if ( Ency[i].Hidden == TRUE && Ency[i].LocType == sort && !bFiltr ) Ency[i].Filtr = FALSE;
		//if ( Ency[i].Hidden == TRUE && bFiltr == TRUE && Ency[i].LocType == sort ) Ency[i].Filtr = TRUE;
		//if ( Ency[i].LocType == 0 ) Ency[i].Filtr = TRUE;
		
		if ( Ency[i].Hidden == TRUE ) gEncyclopediaDataTemp[MaxLocation].LocType = Ency[i].LocType;
			
		if ( Ency[i].Hidden == TRUE && ( Ency[i].LocType == sort || bFiltr == TRUE ) && ( Ency[i].subGroup1 == TypFiltr || Ency[i].InventoryClass == TypFiltr || TypFiltr == NO_FILTR )) //&& Ency[i].Filtr != FALSE ) 
		{	

			bBoxShow = FALSE;
			
			gEncyclopediaDataTemp[MaxLocation].InventoryClass = Ency[i].InventoryClass;
			
			gEncyclopediaDataTemp[MaxLocation].enableDesc = Ency[i].enableDesc;
			
			strncpy(gEncyclopediaDataTemp[MaxLocation].szFile, Ency[i].szFile,MAX_ENCYCLOPEDIA_CHARS);
			strncpy(gEncyclopediaDataTemp[MaxLocation].szFile2, Ency[i].szFile2,MAX_ENCYCLOPEDIA_CHARS);
			strncpy(gEncyclopediaDataTemp[MaxLocation].szFile3, Ency[i].szFile3,MAX_ENCYCLOPEDIA_CHARS);
			strncpy(gEncyclopediaDataTemp[MaxLocation].szFile4, Ency[i].szFile4,MAX_ENCYCLOPEDIA_CHARS);
			
			strncpy(gEncyclopediaDataTemp[MaxLocation].BeforeImage, Ency[i].BeforeImage,MAX_ENCYCLOPEDIA_CHARS);
			strncpy(gEncyclopediaDataTemp[MaxLocation].AfterImage, Ency[i].AfterImage,MAX_ENCYCLOPEDIA_CHARS);
			
			strncpy(gEncyclopediaDataTemp[MaxLocation].sSounds, Ency[i].sSounds,MAX_ENCYCLOPEDIA_CHARS);
			
			wcscpy(gEncyclopediaDataTemp[MaxLocation].ImageDescBefore, Ency[i].ImageDescBefore);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].ImageDescAfter, Ency[i].ImageDescAfter);
			
			wcscpy(gEncyclopediaDataTemp[MaxLocation].Name, Ency[i].Name);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc1, Ency[i].sDesc1);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc2, Ency[i].sDesc2);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc3, Ency[i].sDesc3);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc4, Ency[i].sDesc4);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc5, Ency[i].sDesc5);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc6, Ency[i].sDesc6);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc7, Ency[i].sDesc7);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc8, Ency[i].sDesc8);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc9, Ency[i].sDesc9);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc10, Ency[i].sDesc10);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc11, Ency[i].sDesc11);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc12, Ency[i].sDesc12);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc13, Ency[i].sDesc13);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc14, Ency[i].sDesc14);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc15, Ency[i].sDesc15);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc16, Ency[i].sDesc16);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc17, Ency[i].sDesc17);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc18, Ency[i].sDesc18);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc19, Ency[i].sDesc19);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sDesc20, Ency[i].sDesc20);
			
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sImageDesc1, Ency[i].sImageDesc1);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sImageDesc2, Ency[i].sImageDesc2);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sImageDesc3, Ency[i].sImageDesc3);
			wcscpy(gEncyclopediaDataTemp[MaxLocation].sImageDesc4, Ency[i].sImageDesc4);
			
			gEncyclopediaDataTemp[MaxLocation].sImagePositionX[0] = Ency[i].sImagePositionX[0];
			gEncyclopediaDataTemp[MaxLocation].sImagePositionY[0] = Ency[i].sImagePositionY[0];
					
			gEncyclopediaDataTemp[MaxLocation].sImagePositionX[1] = Ency[i].sImagePositionX[1];
			gEncyclopediaDataTemp[MaxLocation].sImagePositionY[1] = Ency[i].sImagePositionY[1];		
					
			gEncyclopediaDataTemp[MaxLocation].sImagePositionX[2] = Ency[i].sImagePositionX[2];
			gEncyclopediaDataTemp[MaxLocation].sImagePositionY[2] = Ency[i].sImagePositionY[2];
					
			gEncyclopediaDataTemp[MaxLocation].sImagePositionX[3] = Ency[i].sImagePositionX[3];
			gEncyclopediaDataTemp[MaxLocation].sImagePositionY[3] = Ency[i].sImagePositionY[3];
			
			gEncyclopediaDataTemp[MaxLocation].Filtr = Ency[i].Filtr;
		//	Ency[i].Filtr = FALSE;

			//if ( bEncyclopediaCharacter == FALSE )
			
			gEncyclopediaDataTemp[MaxLocation].bvisible = Ency[i].bvisible;
			gEncyclopediaDataTemp[MaxLocation].Hidden = Ency[i].Hidden;	
				
			//Quests
			gEncyclopediaDataTemp[MaxLocation].QuestID = Ency[i].QuestID;
			gEncyclopediaDataTemp[MaxLocation].pQuests = Ency[i].pQuests;
			gEncyclopediaDataTemp[MaxLocation].SpecialQuestImage = Ency[i].SpecialQuestImage;
			gEncyclopediaDataTemp[MaxLocation].ImagePositionQX[0] = Ency[i].ImagePositionQX[0];
			gEncyclopediaDataTemp[MaxLocation].ImagePositionQY[0] = Ency[i].ImagePositionQY[0];
			gEncyclopediaDataTemp[MaxLocation].ImagePositionQX[1] = Ency[i].ImagePositionQX[1];
			gEncyclopediaDataTemp[MaxLocation].ImagePositionQY[1] = Ency[i].ImagePositionQY[1];
			
			if ( gEncyclopediaDataTemp[MaxLocation].SpecialQuestImage == TRUE )
				gEncyclopediaDataTemp[MaxLocation].MaxImages = 0;
			else
				gEncyclopediaDataTemp[MaxLocation].MaxImages = Ency[i].MaxImages;
			
			gEncyclopediaDataTemp[MaxLocation].MaxPages = Ency[i].MaxPages;
			
			//Profiles
			if ( bEncyclopediaLocation == TRUE )
				{
					gEncyclopediaDataTemp[MaxLocation].SectorX = Ency[i].SectorX;
					gEncyclopediaDataTemp[MaxLocation].SectorY = Ency[i].SectorY;
					gEncyclopediaDataTemp[MaxLocation].SectorZ = Ency[i].SectorZ;
				}
			else if ( bEncyclopediaCharacter == TRUE )
				{
					gEncyclopediaDataTemp[MaxLocation].IDProfile = Ency[i].IDProfile; //Ency[i].uiIndex;
				}
			MaxLocation++;
			
		}
	}
	
	if ( bBoxShow == TRUE && ShowBox == TRUE ) 
	{
		ResetVal = TRUE;
		//DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, pSectorPageText[ 4 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
	}
}
/*
void InitLocationFiltrButtons()
{
		guiEncyclopediaToggles[ 0 ] = CreateCheckBoxButton( LAPTOP_SCREEN_UL_X + 15, LAPTOP_SCREEN_WEB_UL_Y  + 250,
															"INTERFACE\\OptionsCheckBoxes_12x12.sti", 
															MSYS_PRIORITY_HIGH+10,
															BtnEncyclopediaTogglesCallback );
		MSYS_SetBtnUserData( guiEncyclopediaToggles[ 0 ], 0, 0 );
		
		guiEncyclopediaToggles[ 1 ] = CreateCheckBoxButton( LAPTOP_SCREEN_UL_X + 30, LAPTOP_SCREEN_WEB_UL_Y  + 250,
															"INTERFACE\\OptionsCheckBoxes_12x12.sti", 
															MSYS_PRIORITY_HIGH+10,
															BtnEncyclopediaTogglesCallback );
		MSYS_SetBtnUserData( guiEncyclopediaToggles[ 1 ], 0, 1 );
		
		guiEncyclopediaToggles[ 2 ] = CreateCheckBoxButton( LAPTOP_SCREEN_UL_X + 15, LAPTOP_SCREEN_WEB_UL_Y  + 264,
															"INTERFACE\\OptionsCheckBoxes_12x12.sti", 
															MSYS_PRIORITY_HIGH+10,
															BtnEncyclopediaTogglesCallback );
		MSYS_SetBtnUserData( guiEncyclopediaToggles[ 2 ], 0, 2 );
		
		guiEncyclopediaToggles[ 3 ] = CreateCheckBoxButton( LAPTOP_SCREEN_UL_X + 30, LAPTOP_SCREEN_WEB_UL_Y  + 264,
															"INTERFACE\\OptionsCheckBoxes_12x12.sti", 
															MSYS_PRIORITY_HIGH+10,
															BtnEncyclopediaTogglesCallback );
		MSYS_SetBtnUserData( guiEncyclopediaToggles[ 3 ], 0, 3 );
		
		SetRegionFastHelpText( &gSelectedEncyclopediaTextRegion[ 0 ], pEncyclopediaHelpText[LOCATION_CITIES] );
		SetButtonFastHelpText( guiEncyclopediaToggles[ 0 ], pEncyclopediaHelpText[LOCATION_CITIES]  );	
		
		SetRegionFastHelpText( &gSelectedEncyclopediaTextRegion[ 1 ], pEncyclopediaHelpText[LOCATION_SAM] );
		SetButtonFastHelpText( guiEncyclopediaToggles[ 1 ], pEncyclopediaHelpText[LOCATION_SAM]  );	
		
		SetRegionFastHelpText( &gSelectedEncyclopediaTextRegion[ 2 ], pEncyclopediaHelpText[LOCATION_OTHER] );
		SetButtonFastHelpText( guiEncyclopediaToggles[ 2 ], pEncyclopediaHelpText[LOCATION_OTHER]  );	
		
		SetRegionFastHelpText( &gSelectedEncyclopediaTextRegion[ 3 ], pEncyclopediaHelpText[LOCATION_MINES] );
		SetButtonFastHelpText( guiEncyclopediaToggles[ 3 ], pEncyclopediaHelpText[LOCATION_MINES]  );	

}
*/

void InitSoundButtons()
{
	UINT16	usPosY, i;
	guiSoundButtonsImage =	LoadButtonImage("BriefingRoom\\BUTTONF.sti", -1,0,-1,1,-1 );

	usPosY = ENCYCLOPEDIA_LOCATION_FILTER_Y;
	for(i=0; i<MAX_MISSION_BUTTONS; i++)
	{
		guiSoundButtons[i] = CreateIconAndTextButton( guiSoundButtonsImage, pOtherButtonsText[i], ENCYCLOPEDIA_LOCATION_PAGE_FONT,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP, DEFAULT_SHADOW,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														ENCYCLOPEDIA_LOCATION_PAGE1_X-92, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, SelectSoundButtonsRegionCallBack);
		SetButtonCursor(guiSoundButtons[i], CURSOR_WWW);
		MSYS_SetBtnUserData( guiSoundButtons[i], 0, i);
		usPosY += ENCYCLOPEDIA_FILTR_BUTTON_GAP;
		
		SetRegionFastHelpText( &gSelectedSoundButtonTextRegion[ i ], pOtherButtonsHelpText[i] );
		SetButtonFastHelpText( guiSoundButtons[ i ], pOtherButtonsHelpText[i]  );	
	}
}

void InitLocationFiltrButtons()
{
	UINT16	usPosX, usPosY, i;
	guiFiltrPageButtonImage =	LoadButtonImage("ENCYCLOPEDIA\\BUTTONF.sti", -1,0,-1,1,-1 );

	usPosX = ENCYCLOPEDIA_LOCATION_PAGE1_X-92;
	usPosY = ENCYCLOPEDIA_LOCATION_FILTER_Y;

	for(i=0; i<MAX_FILTR_LOCATION_BUTTONS; i++)
	{
		guiFiltrPageButton[i] = CreateIconAndTextButton( guiFiltrPageButtonImage, pEncyclopediaSkrotyText[i], ENCYCLOPEDIA_LOCATION_PAGE_FONT,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP, DEFAULT_SHADOW,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, SelectFiltrButtonsRegionCallBack);
		SetButtonCursor(guiFiltrPageButton[i], CURSOR_WWW);
		MSYS_SetBtnUserData( guiFiltrPageButton[i], 0, i);
		usPosY += ENCYCLOPEDIA_FILTR_BUTTON_GAP;
		
		SetRegionFastHelpText( &gSelectedEncyclopediaTextRegion[ i ], pEncyclopediaHelpText[i] );
		SetButtonFastHelpText( guiFiltrPageButton[ i ], pEncyclopediaHelpText[i]  );	
	}
}

void InitCharacterFiltrButtons()
{
	UINT16	usPosX, usPosY, i;
	guiFiltrPageButtonImage =	LoadButtonImage("ENCYCLOPEDIA\\BUTTONF.sti", -1,0,-1,1,-1 );

	usPosX = ENCYCLOPEDIA_LOCATION_PAGE1_X-92;
	usPosY = ENCYCLOPEDIA_LOCATION_FILTER_Y;
	
	for(i=0; i<FILTR_BUTTONS_CHARACTER; i++)
	{
		guiFiltrPageButton[i] = CreateIconAndTextButton( guiFiltrPageButtonImage, pEncyclopediaShortCharacterText[i], ENCYCLOPEDIA_LOCATION_PAGE_FONT,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP, DEFAULT_SHADOW,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, SelectFiltrButtonsRegionCallBack);
		SetButtonCursor(guiFiltrPageButton[i], CURSOR_WWW);
		MSYS_SetBtnUserData( guiFiltrPageButton[i], 0, i);
		usPosY += ENCYCLOPEDIA_FILTR_BUTTON_GAP;
		
		SetRegionFastHelpText( &gSelectedEncyclopediaTextRegion[ i ], pEncyclopediaHelpCharacterText[i] );
		SetButtonFastHelpText( guiFiltrPageButton[ i ], pEncyclopediaHelpCharacterText[i]  );	
	}
	
	//DisableButton( guiFiltrPageButton[6] );	
}

void InitInventoryFiltrButtons()
{
	UINT16	usPosX, usPosY, i;
	guiFiltrInventoryButtonImage =	LoadButtonImage("ENCYCLOPEDIA\\BUTTONF.sti", -1,0,-1,1,-1 );

	usPosX = ENCYCLOPEDIA_LOCATION_PAGE1_X-92;
	usPosY = ENCYCLOPEDIA_LOCATION_FILTER_Y;
	
	for(i=0; i<FILTR_INVENTORY_BUTTONS; i++)
	{
		guiFiltrInventoryButton[i] = CreateIconAndTextButton( guiFiltrInventoryButtonImage, pEncyclopediaShortInventoryText[i], ENCYCLOPEDIA_LOCATION_PAGE_FONT,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP, DEFAULT_SHADOW,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, SelectFiltrButtonsRegionCallBack);
		SetButtonCursor(guiFiltrInventoryButton[i], CURSOR_WWW);
		MSYS_SetBtnUserData( guiFiltrInventoryButton[i], 0, i);
		usPosY += ENCYCLOPEDIA_FILTR_BUTTON_GAP;
		
		SetRegionFastHelpText( &gSelectedEncyclopediaInventoryTextRegion[ i ], pEncyclopediaShortInventoryText[i+5] );
		SetButtonFastHelpText( guiFiltrInventoryButton[ i ], pEncyclopediaShortInventoryText[i+5]  );	
	}
}

//Add inventory buttons
void InitAddInventoryFiltrButtons( UINT32 Butons, UINT32 Type, BOOLEAN bDisableButton)
{
	UINT16	usPosX, usPosY,i,iHelp;
	guiFiltrAddInventoryButtonImage =	LoadButtonImage("ENCYCLOPEDIA\\BUTTONF.sti", -1,0,-1,1,-1 );
	
	//ButtonsID = Butons;

	usPosX = ENCYCLOPEDIA_LOCATION_PAGE1_X-40;
	usPosY = ENCYCLOPEDIA_LOCATION_PAGE1_Y+22;
	
	for(i=0; i<Butons; i++)
	{
		iHelp = i + Type;
	
		guiFiltrAddInventoryButton[i] = CreateIconAndTextButton( guiFiltrAddInventoryButtonImage, BoxFilter[iHelp], ENCYCLOPEDIA_LOCATION_PAGE_FONT,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP, DEFAULT_SHADOW,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, SelectInventoryFiltrButtonsRegionCallBack);
		SetButtonCursor(guiFiltrAddInventoryButton[i], CURSOR_WWW);
		MSYS_SetBtnUserData( guiFiltrAddInventoryButton[i], 0, i);
		
	    usPosX += ENCYCLOPEDIA_FILTR_BUTTON_GAP + 25;	
		SetRegionFastHelpText( &gSelectedEncyclopediaAddInventoryTextRegion[ i ], BobbyRFilter[iHelp] );
		SetButtonFastHelpText( guiFiltrAddInventoryButton[ i ], BobbyRFilter[iHelp] );	
	}
	
	if ( bDisableButton == TRUE ) DisableButton( guiFiltrAddInventoryButton[0] );	
	
	RenderBoxDisabledButton();

}

BOOLEAN  RenderFiltrBox()
{
  VOBJECT_DESC	VObjectDesc;
  char fileName[500];

    VObjectDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE;	
	
	strcpy(fileName, "ENCYCLOPEDIA\\BOX.sti");
	strcpy(VObjectDesc.ImageFile, fileName);
	CHECKF(AddVideoObject(&VObjectDesc,&gFiltrBox));
	BltVideoObjectFromIndex( FRAME_BUFFER, gFiltrBox, 0 , ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, ENCYCLOPEDIA_LOCATION_BOX_Y + 13 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, VO_BLT_SRCTRANSPARENCY, NULL );

	return(TRUE);
}

void InitData ( BOOLEAN bInit)
{

  IDimage = 1;
  MaxImages = 1;
  LocationID = 0;
  ID = 0;
  IDNewLocation = 0;
  MaxLocation = 0;
  ENCYCLOPEDIA_PAGE = 0;
  
  bRenderFiltrBox[0] = FALSE;
  bRenderFiltrBox[1] = FALSE;
  bRenderFiltrBox[2] = FALSE;
  bRenderFiltrBox[3] = FALSE;
  
  
  ResetTemp();
  
  if ( bEncyclopediaLocation == TRUE )
		CopyToTemp (gEncyclopediaLocationData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bEncyclopediaCharacter == TRUE )
		CopyToTemp (gEncyclopediaProfilesData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bEncyclopediaInventory == TRUE )
		CopyToTemp (gEncyclopediaInventoryData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bEncyclopediaQuests == TRUE )
		CopyToTemp (gEncyclopediaQuestsData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bBriefingRoom == TRUE )
		CopyToTemp (gBriefingRoomData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bBriefingRoomSpecialMission  == TRUE )
		CopyToTemp (gBriefingRoomSpecialMissionData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  
	//InitEncyclopediaDefaults();
	
	if ( bEncyclopediaLocation == TRUE )
		InitLocationFiltrButtons();
	
	if ( bEncyclopediaCharacter == TRUE )
		InitCharacterFiltrButtons();
		
	if ( bEncyclopediaInventory == TRUE )
		InitInventoryFiltrButtons();
	
    if ( bBriefingRoomSpecialMission  == TRUE || bBriefingRoom == TRUE ) bSoundButtons = TRUE; else bSoundButtons = FALSE;
	
	if ( bSoundButtons == TRUE )
		InitSoundButtons();
		
	ResetVal = bInit;

}

BOOLEAN EnterEncyclopediaLocation()
{
  UINT16	usPosX, usPosY, i;
  /*
  IDimage = 1;
  MaxImages = 1;
  LocationID = 0;
  ID = 0;
  IDNewLocation = 0;
  MaxLocation = 0;
  ENCYCLOPEDIA_PAGE = 0;
  
  bRenderFiltrBox[0] = FALSE;
  bRenderFiltrBox[1] = FALSE;
  bRenderFiltrBox[2] = FALSE;
  bRenderFiltrBox[3] = FALSE;
  
  
  ResetTemp();
  
  if ( bEncyclopediaLocation == TRUE )
		CopyToTemp (gEncyclopediaLocationData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bEncyclopediaCharacter == TRUE )
		CopyToTemp (gEncyclopediaProfilesData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bEncyclopediaInventory == TRUE )
		CopyToTemp (gEncyclopediaInventoryData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bEncyclopediaQuests == TRUE )
		CopyToTemp (gEncyclopediaQuestsData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bBriefingRoom == TRUE )
		CopyToTemp (gBriefingRoomData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  else if ( bBriefingRoomSpecialMission  == TRUE )
		CopyToTemp (gBriefingRoomSpecialMissionData, TRUE, NONE_FILTR , NO_FILTR, FALSE );
  
	InitEncyclopediaDefaults();
	
	if ( bEncyclopediaLocation == TRUE )
		InitLocationFiltrButtons();
	
	if ( bEncyclopediaCharacter == TRUE )
		InitCharacterFiltrButtons();
		
	if ( bEncyclopediaInventory == TRUE )
		InitInventoryFiltrButtons();
	
    if ( bBriefingRoomSpecialMission  == TRUE || bBriefingRoom == TRUE ) bSoundButtons = TRUE; else bSoundButtons = FALSE;
	
	if ( bSoundButtons == TRUE )
		InitSoundButtons();
		
	//if ( bEncyclopediaQuests == TRUE )
	//	InitQuestsFiltrButtons();
	*/
  
	InitData (ResetVal);
	
	InitEncyclopediaDefaults();
	
	guiEncyclopediaLocationPageButtonImage =	LoadButtonImage("ENCYCLOPEDIA\\BottomButtons2.sti", -1,0,-1,1,-1 );

	usPosX = ENCYCLOPEDIA_LOCATION_PAGE1_X+15;
	usPosY = ENCYCLOPEDIA_LOCATION_PAGE1_Y;

	for(i=0; i<3; i++)
	{
		guiEncyclopediaLocationPageButton[i] = CreateIconAndTextButton( guiEncyclopediaLocationPageButtonImage, pSectorPageText[i], ENCYCLOPEDIA_LOCATION_PAGE_FONT,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP, DEFAULT_SHADOW,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, SelectEncyclopediaLocationRegionCallBack);
		SetButtonCursor(guiEncyclopediaLocationPageButton[i], CURSOR_WWW);
		MSYS_SetBtnUserData( guiEncyclopediaLocationPageButton[i], 0, i);
		usPosX += ENCYCLOPEDIA_LOCATION_PAGE_GAP;
	}
	
	usPosX=0;
	//Load graphic for buttons
	guiEncyclopediaiPageButtonImage =	LoadButtonImage("ENCYCLOPEDIA\\BottomButtons2.sti", -1,0,-1,1,-1 );

	usPosX = ENCYCLOPEDIA_LOCATION_PAGE1_X-44;
	usPosY = ENCYCLOPEDIA_LOCATION_PAGE1_Y-130;

	for(i=0; i<3; i++)
	{
		guiEncyclopediaPageButton[i] = CreateIconAndTextButton( guiEncyclopediaiPageButtonImage, pLocationPageText[i], ENCYCLOPEDIA_LOCATION_PAGE_FONT,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_UP, DEFAULT_SHADOW,
														ENCYCLOPEDIA_LOCATION_PAGE_COLOR_DOWN, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														usPosX, usPosY, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, SelectEncyclopediaLocationPageRegionCallBack);
		SetButtonCursor(guiEncyclopediaPageButton[i], CURSOR_WWW);
		MSYS_SetBtnUserData( guiEncyclopediaPageButton[i], 0, i);

		usPosX += ENCYCLOPEDIA_LOCATION_PAGE_GAP+60;
	}
	
	fFirstTimeInEncyclopediaLocation = FALSE;

	RenderEncyclopediaLocation(FALSE);
	
	RenderButtonDisabled();
	
	DisableButton( guiEncyclopediaPageButton[0] );	
	DisableButton( guiEncyclopediaLocationPageButton[0] );	
	
	if ( gEncyclopediaDataTemp[LocationID].CheckMission == MISSIONSTART || gEncyclopediaDataTemp[LocationID].CheckMission == MISSIONEND )				
		DisableButton ( guiSoundButtons[1] );
	else if ( gEncyclopediaDataTemp[LocationID].CheckMission == MISSIONNOSTARTED )	
		EnableButton ( guiSoundButtons[1] );
	
	if ( MaxLocation == -1 || MaxLocation == 0 || MaxLocation == 1 ) 
		DisableButton( guiEncyclopediaLocationPageButton[2] );	
		
	RenderMap();
 
	return(TRUE);
}

void SetFiltrButtons()
{
UINT32 i;

		for(i=0; i<11; i++)
		{
			guiFiltrAddInventoryButton[i] = -1;
		}
		
		gFiltrBox = -1; 
		guiFiltrAddInventoryButtonImage = -1;
}

void UnloadInventoryFilterBox( UINT32 ButtonID )
{
    UINT16	i;
	
	//if ( bEncyclopediaInventory == TRUE )
	//{		
		if(guiFiltrAddInventoryButtonImage != -1)
		{
			UnloadButtonImage( guiFiltrAddInventoryButtonImage );
			guiFiltrAddInventoryButtonImage = -1;
		}
	
		for(i=0; i<ButtonID; i++)
		{
			if(guiFiltrAddInventoryButton[i] != -1)
			{
				RemoveButton( guiFiltrAddInventoryButton[i] );
				MSYS_RemoveRegion( &gSelectedEncyclopediaAddInventoryTextRegion[i]);
			}
		}
	//}
	if ( gFiltrBox != -1 )
		DeleteVideoObjectFromIndex (gFiltrBox);	
}

void UnLoadMenuButtons ()
{
UINT16	i;

	if ( bEncyclopediaLocation == TRUE )
	{
	
		if ( guiFiltrPageButtonImage != -1 )
		UnloadButtonImage( guiFiltrPageButtonImage );
	
		for(i=0; i<MAX_FILTR_LOCATION_BUTTONS; i++)
		{
			RemoveButton( guiFiltrPageButton[i] );
			MSYS_RemoveRegion( &gSelectedEncyclopediaTextRegion[i]);
		}
	}
	
	if ( bEncyclopediaCharacter == TRUE )
	{
	
		if ( guiFiltrPageButtonImage != -1 )
		UnloadButtonImage( guiFiltrPageButtonImage );
	
		for(i=0; i<FILTR_BUTTONS_CHARACTER; i++)
		{
			RemoveButton( guiFiltrPageButton[i] );
			MSYS_RemoveRegion( &gSelectedEncyclopediaTextRegion[i]);
		}
	}
	
	if ( bEncyclopediaInventory == TRUE )
	{
	
		if ( guiFiltrInventoryButtonImage != -1 )
		UnloadButtonImage( guiFiltrInventoryButtonImage );
	
		for(i=0; i<FILTR_INVENTORY_BUTTONS; i++)
		{	
			RemoveButton( guiFiltrInventoryButton[i] );
			MSYS_RemoveRegion( &gSelectedEncyclopediaInventoryTextRegion[i]);
		}
		
	}

	/*
	if ( bBriefingRoom == TRUE )
	{
		UnloadButtonImage( guiFiltrBriefingRoomButtonImage );
	
		for(i=0; i<FILTR_BRIEFING_ROOM_BUTTONS; i++)
		{	
			RemoveButton( guiFiltrBriefingRoomButton[i] );
			MSYS_RemoveRegion( &gSelectedEncyclopediaBriefingRoomTextRegion[i]);
		}
		
	}
	*/
	
	/*
	if ( bEncyclopediaQuests == TRUE )
	{
		UnloadButtonImage( guiFiltrQuestsButtonImage );
	
		for(i=0; i<FILTR_QUESTS_BUTTONS; i++)
		{	
			RemoveButton( guiFiltrQuestsButton[i] );
			MSYS_RemoveRegion( &gSelectedEncyclopediaQuestsTextRegion[i]);
		}
		
	}
	*/

}

void ExitEncyclopediaLocation()
{
UINT16	i;
	RemoveEncyclopediaDefaults();
	
	if (guiEncyclopediaiPageButtonImage != -1 )
	UnloadButtonImage( guiEncyclopediaiPageButtonImage );
	
	if (guiEncyclopediaLocationPageButtonImage != -1 )
	UnloadButtonImage( guiEncyclopediaLocationPageButtonImage );
	
	UnLoadMenuButtons ();
	
	
		if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
		if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
		if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
		if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
		
		bRenderFiltrBox[0] = FALSE;
		bRenderFiltrBox[1] = FALSE;
		bRenderFiltrBox[2] = FALSE;
		bRenderFiltrBox[3] = FALSE;

		
	/*
	if ( bEncyclopediaLocation == TRUE )
	{
		MSYS_RemoveRegion( &gSelectedEncyclopediaTextRegion[0]);
		MSYS_RemoveRegion( &gSelectedEncyclopediaTextRegion[1]);
		MSYS_RemoveRegion( &gSelectedEncyclopediaTextRegion[2]);
		MSYS_RemoveRegion( &gSelectedEncyclopediaTextRegion[3]);
		RemoveButton( guiEncyclopediaToggles[ 0 ] );
		RemoveButton( guiEncyclopediaToggles[ 1 ] );
		RemoveButton( guiEncyclopediaToggles[ 2 ] );
		RemoveButton( guiEncyclopediaToggles[ 3 ] );
	}
	*/
	
	for(i=0; i<3; i++)
 		RemoveButton( guiEncyclopediaPageButton[i] );
		
	for(i=0; i<3; i++)
 		RemoveButton( guiEncyclopediaLocationPageButton[i] );
		
	if ( bSoundButtons == TRUE )
	{
		if ( guiSoundButtonsImage != -1 )
		{
			UnloadButtonImage( guiSoundButtonsImage );
		
			for(i=0; i<MAX_MISSION_BUTTONS; i++)
			{
				RemoveButton( guiSoundButtons[i] );
				MSYS_RemoveRegion( &gSelectedSoundButtonTextRegion[i]);
			}
		}
	}
	

	bSoundButtons = FALSE;
		
	bEncyclopediaLocation = FALSE;
	bEncyclopediaCharacter = FALSE;
	bEncyclopediaInventory = FALSE;
	bEncyclopediaQuests  = FALSE;
	bBriefingRoom  = FALSE;
	bBriefingRoomSpecialMission = FALSE;
	
}

void HandleEncyclopediaLocation()
{

}

void RenderButtonDisabled()
{
	if ( gEncyclopediaDataTemp[LocationID].sDesc2 == '\0' && gEncyclopediaDataTemp[LocationID].sDesc3 == '\0' ) 
	{
		DisableButton( guiEncyclopediaPageButton[0] );
		DisableButton( guiEncyclopediaPageButton[2] );
	}
	else
	{
		EnableButton( guiEncyclopediaPageButton[2] );
	}
}

void RenderButtonDisabled2()
{
	DisableButton( guiEncyclopediaPageButton[0] );
	EnableButton( guiEncyclopediaPageButton[2] );
}

void RenderButtonDisabled3()
{
	DisableButton( guiEncyclopediaLocationPageButton[0] );
	
	if ( MaxLocation == -1 || MaxLocation == 0 || MaxLocation == 1 ) 
		DisableButton( guiEncyclopediaLocationPageButton[2] );
	else
		EnableButton( guiEncyclopediaLocationPageButton[2] );
}

void RenderButtonDisabled4()
{
	DisableButton( guiEncyclopediaPageButton[0] );
	DisableButton( guiEncyclopediaPageButton[2] );
}

void RenderBoxDisabledButton()
{
	MaxImages = 1;
	MaxLocation = 0;
	IDimage = 1;
	
	DisableButton( guiEncyclopediaLocationPageButton[0] );
	//DisableButton( guiEncyclopediaLocationPageButton[1] );
	DisableButton( guiEncyclopediaLocationPageButton[2] );
		
	DisableButton( guiEncyclopediaPageButton[0] );	
	DisableButton( guiEncyclopediaPageButton[1] );
	DisableButton( guiEncyclopediaPageButton[2] );
}

BOOLEAN  RenderMap()
{
  VOBJECT_DESC	VObjectDesc;
  char fileName[500];

    VObjectDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE;	
	
	if ( gEncyclopediaDataTemp[LocationID].SpecialQuestImage == FALSE ) 
	{
	
	if (IDimage == 1 )
		strcpy(fileName, gEncyclopediaDataTemp[LocationID].szFile);
	if (IDimage == 2 )
		strcpy(fileName, gEncyclopediaDataTemp[LocationID].szFile2);
	if (IDimage == 3 )
		strcpy(fileName, gEncyclopediaDataTemp[LocationID].szFile3);
	if (IDimage == 4 )
		strcpy(fileName, gEncyclopediaDataTemp[LocationID].szFile4);
	if (IDimage == -1 || IDimage > 4 )
		strcpy(fileName, gEncyclopediaDataTemp[LocationID].szFile);
		
	if (IDimage == -1 || IDimage > 4 ) IDimage = 1;
	
	if ( IDimage > 0 && gEncyclopediaDataTemp[LocationID].MaxImages != -1 && (bEncyclopediaLocation == TRUE || bEncyclopediaCharacter == TRUE || bEncyclopediaInventory == TRUE || bEncyclopediaQuests == TRUE || bBriefingRoom == TRUE || bBriefingRoomSpecialMission == TRUE ) )
	{
		strcpy(VObjectDesc.ImageFile, fileName);
		CHECKF(AddVideoObject(&VObjectDesc,&gEncyclopediaDataTemp[LocationID].uiIndex));
		//BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_LOCATION_BOX_X + 50 ,  ENCYCLOPEDIA_LOCATION_BOX_Y - 210, VO_BLT_SRCTRANSPARENCY, NULL );
		BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_IMAGE_X + gEncyclopediaDataTemp[LocationID].sImagePositionX[IDimage-1] ,  ENCYCLOPEDIA_IMAGE_Y + gEncyclopediaDataTemp[LocationID].sImagePositionY[IDimage-1], VO_BLT_SRCTRANSPARENCY, NULL );
	}
	
	}
	
	if ( gEncyclopediaDataTemp[LocationID].SpecialQuestImage == TRUE && bEncyclopediaQuests == TRUE && gubQuest[ gEncyclopediaDataTemp[ LocationID ].QuestID ] == QUESTINPROGRESS )
	{
		strcpy(fileName, gEncyclopediaDataTemp[LocationID].BeforeImage);
		strcpy(VObjectDesc.ImageFile, fileName);
		CHECKF(AddVideoObject(&VObjectDesc,&gEncyclopediaDataTemp[LocationID].uiIndex));
		//BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_LOCATION_BOX_X + 50 ,  ENCYCLOPEDIA_LOCATION_BOX_Y - 210, VO_BLT_SRCTRANSPARENCY, NULL );
		BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_IMAGE_X + gEncyclopediaDataTemp[LocationID].ImagePositionQX[0] ,  ENCYCLOPEDIA_IMAGE_Y + gEncyclopediaDataTemp[LocationID].ImagePositionQY[0], VO_BLT_SRCTRANSPARENCY, NULL );
	}
	else if ( gEncyclopediaDataTemp[LocationID].SpecialQuestImage == TRUE && gubQuest[ gEncyclopediaDataTemp[ LocationID ].QuestID ] == QUESTDONE ) 
	{
		strcpy(fileName, gEncyclopediaDataTemp[LocationID].AfterImage);
		strcpy(VObjectDesc.ImageFile, fileName);
		CHECKF(AddVideoObject(&VObjectDesc,&gEncyclopediaDataTemp[LocationID].uiIndex));
		//BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_LOCATION_BOX_X + 50 ,  ENCYCLOPEDIA_LOCATION_BOX_Y - 210, VO_BLT_SRCTRANSPARENCY, NULL );
		BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_IMAGE_X + gEncyclopediaDataTemp[LocationID].ImagePositionQX[1] ,  ENCYCLOPEDIA_IMAGE_Y + gEncyclopediaDataTemp[LocationID].ImagePositionQY[1], VO_BLT_SRCTRANSPARENCY, NULL );
	}
	/*
	else if ( gEncyclopediaDataTemp[LocationID].MaxImages != -10 && bEncyclopediaCharacter == TRUE )
	{
		strcpy(VObjectDesc.ImageFile, fileName);
		CHECKF(AddVideoObject(&VObjectDesc,&gEncyclopediaDataTemp[LocationID].uiIndex));
		BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_IMAGE_X + gEncyclopediaDataTemp[LocationID].sImagePositionX[IDimage] ,  ENCYCLOPEDIA_IMAGE_Y + gEncyclopediaDataTemp[LocationID].sImagePositionY[IDimage], VO_BLT_SRCTRANSPARENCY, NULL );
	//	BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_LOCATION_BOX_X + 150 ,  ENCYCLOPEDIA_LOCATION_BOX_Y - 190, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	else if ( gEncyclopediaDataTemp[LocationID].MaxImages != -10 && bEncyclopediaInventory == TRUE )
	{
		strcpy(VObjectDesc.ImageFile, fileName);
		CHECKF(AddVideoObject(&VObjectDesc,&gEncyclopediaDataTemp[LocationID].uiIndex));
		BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_IMAGE_X + gEncyclopediaDataTemp[LocationID].sImagePositionX[IDimage] ,  ENCYCLOPEDIA_IMAGE_Y + gEncyclopediaDataTemp[LocationID].sImagePositionY[IDimage], VO_BLT_SRCTRANSPARENCY, NULL );
	//	BltVideoObjectFromIndex( FRAME_BUFFER, gEncyclopediaDataTemp[LocationID].uiIndex, 0 , ENCYCLOPEDIA_LOCATION_BOX_X + 150 ,  ENCYCLOPEDIA_LOCATION_BOX_Y - 190, VO_BLT_SRCTRANSPARENCY, NULL );
	}
	*/
	return(TRUE);
}

void RenderEncyclopediaLocation( BOOLEAN bHidden )
{
  UINT16	i;
  UINT8		ubNumLines=11; 
  UINT16	usPosY;
  //UINT8			ubFontHeight;
  UINT16	usStringPixLength;
  CHAR16	zString[512]; 
  CHAR16	zString2[512];

	DrawEncyclopediaDefaults();
	
	DisplayEncyclopediaLocationText();
	
	//ubFontHeight = (UINT8)GetFontHeight(ENCYCLOPEDIA_LOCATION_BOX_FONT);
	
	ENCYCLOPEDIA_PAGE = gEncyclopediaDataTemp[LocationID].MaxPages-1;
	
	MaxImages = gEncyclopediaDataTemp[LocationID].MaxImages;//-1;
	
	//Location name
	if (bEncyclopediaLocation == TRUE  && ResetVal == FALSE)//&& gEncyclopediaDataTemp[LocationID].SectorY != -1 || gEncyclopediaDataTemp[LocationID].SectorX != -1 || gEncyclopediaDataTemp[LocationID].SectorZ != -1 )
	{
	swprintf( zString, L" ( %c%d )", 'A' + gEncyclopediaDataTemp[LocationID].SectorY - 1, gEncyclopediaDataTemp[LocationID].SectorX );
	wcscpy( zString2,gEncyclopediaDataTemp[LocationID].Name );
	wcscat( zString2,zString );
	}
	
	//Profile name
	if (bEncyclopediaCharacter == TRUE && gEncyclopediaDataTemp[LocationID].IDProfile != -1  && ResetVal == FALSE)
	{
	//	if ( gEncyclopediaDataTemp[gEncyclopediaDataTemp[LocationID].IDProfile].Name[0] != 0 )
		///swprintf( zString, L"%s ( %s )", gMercProfiles[gEncyclopediaDataTemp[LocationID].IDProfile].zName, gMercProfiles[ gEncyclopediaDataTemp[LocationID].IDProfile ].zNickname );
	//		swprintf( zString, gEncyclopediaDataTemp[gEncyclopediaDataTemp[LocationID].IDProfile].Name );
	///	else	
		swprintf( zString, gMercProfiles[gEncyclopediaDataTemp[LocationID].IDProfile].zName );
		wcscpy( zString2,zString );
	}
	
	//Inventory name
	if ( bEncyclopediaInventory == TRUE || bBriefingRoom == TRUE || bEncyclopediaQuests == TRUE || bBriefingRoomSpecialMission == TRUE && ResetVal == FALSE )
	{
		swprintf( zString, gEncyclopediaDataTemp[LocationID].Name );
		wcscpy( zString2,zString );
	}
	
	ShadowVideoSurfaceRect( FRAME_BUFFER, ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, ENCYCLOPEDIA_LOCATION_BOX_Y + 13 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 227 , ENCYCLOPEDIA_LOCATION_BOX_X + ENCYCLOPEDIA_LOCATION_BOX_WIDTH + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, ENCYCLOPEDIA_LOCATION_BOX_Y + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP-42);

	ShadowVideoSurfaceRect( FRAME_BUFFER, ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, ENCYCLOPEDIA_LOCATION_BOX_Y + 13 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50 , ENCYCLOPEDIA_LOCATION_BOX_X + ENCYCLOPEDIA_LOCATION_BOX_WIDTH + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, ENCYCLOPEDIA_LOCATION_BOX_Y + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP-20);
	
	
	//Location name
	if (bEncyclopediaLocation == TRUE && ResetVal == FALSE) //&& gEncyclopediaDataTemp[LocationID].IDProfile != -1 )
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X, ENCYCLOPEDIA_LOCATION_BOX_Y-225, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, FONT14ARIAL, ENCYCLOPEDIA_LOCATION_BOX_COLOR, zString2, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

	//Inventory name
	if (bEncyclopediaInventory == TRUE || bBriefingRoom == TRUE || bEncyclopediaQuests == TRUE && ResetVal == FALSE ) 
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X, ENCYCLOPEDIA_LOCATION_BOX_Y-225, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, FONT14ARIAL, ENCYCLOPEDIA_LOCATION_BOX_COLOR, zString2, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

	//Profile name
	if ( bEncyclopediaCharacter == TRUE && ResetVal == FALSE )	
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X, ENCYCLOPEDIA_LOCATION_BOX_Y-225, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, FONT14ARIAL, ENCYCLOPEDIA_LOCATION_BOX_COLOR, zString2, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	
	//Reset
    if ( ResetVal == TRUE )
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X, ENCYCLOPEDIA_LOCATION_BOX_Y-225, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, FONT14ARIAL, ENCYCLOPEDIA_LOCATION_BOX_COLOR, pSectorPageText[ 4 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	
	/*
	//Type 
	if ( bEncyclopediaInventory == TRUE && bEncyclopediaLocation == FALSE )
	{
		//Guns
			 if ( gEncyclopediaDataTemp[LocationID].LocType == NOT_GUN ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[0] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_PISTOL ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[1] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_M_PISTOL ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[2] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_SMG ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[3] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_RIFLE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[4] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_SN_RIFLE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[5] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_AS_RIFLE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[6] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_LMG ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[7] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == GUN_SHOTGUN ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[8] );
		//Armour
		else if ( gEncyclopediaDataTemp[LocationID].LocType == ARMOURCLASS_HELMET ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[21] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == ARMOURCLASS_VEST ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[22] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == ARMOURCLASS_LEGGINGS ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[23] );
		else if ( gEncyclopediaDataTemp[LocationID].LocType == ARMOURCLASS_PLATE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[24] );
		//Misc
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_BLADE && gEncyclopediaDataTemp[LocationID].LocType == IC_BLADE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[25] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_THROWING_KNIFE && gEncyclopediaDataTemp[LocationID].LocType == IC_THROWING_KNIFE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[26] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_PUNCH ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[27] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_GRENADE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[28] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_BOMB ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[29] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_MEDKIT ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[30] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_KIT ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[31] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_FACE ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[32] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_LBEGEAR ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[33] );
		else if ( gEncyclopediaDataTemp[LocationID].InventoryClass == IC_MISC ) swprintf( zString, L"%s %s", pSectorPageText[3],BobbyRFilter[34] );
		else  swprintf( zString, L"%s ---", pSectorPageText[3] );
							
		wcscpy( zString2,zString );
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 65, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, zString2, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);
	}
	*/
	
	if ( bEncyclopediaLocation == TRUE  && ResetVal == FALSE)
	{
	swprintf( zString, L"%s %s", pSectorPageText[3],pEncyclopediaTypeText[gEncyclopediaDataTemp[LocationID].LocType] );
	wcscpy( zString2,zString );
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 65, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, zString2, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);
	}
	
	//Reset
	if ( ResetVal == TRUE )
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 65, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, L"", FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);
	
	if ( gEncyclopediaDataTemp[LocationID].SpecialQuestImage == FALSE && ResetVal == FALSE)
	{
	
	if ( IDimage == 1 )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sImageDesc1, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if (IDimage == 2 && gEncyclopediaDataTemp[LocationID].sImageDesc2 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sImageDesc2, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if (IDimage == 3 && gEncyclopediaDataTemp[LocationID].sImageDesc3 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sImageDesc3, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if (IDimage == 4 && gEncyclopediaDataTemp[LocationID].sImageDesc4 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sImageDesc4, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	}
	
	if ( ResetVal == FALSE && gEncyclopediaDataTemp[LocationID].SpecialQuestImage == TRUE && gubQuest[ gEncyclopediaDataTemp[ LocationID ].QuestID ] == QUESTINPROGRESS  )
	{
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].ImageDescBefore, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	}
	else 	if ( gEncyclopediaDataTemp[LocationID].SpecialQuestImage == TRUE && gubQuest[ gEncyclopediaDataTemp[ LocationID ].QuestID ] == QUESTDONE )
	{
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].ImageDescAfter, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	}
	
	//Reset
	if ( ResetVal == TRUE )
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP + 3, ENCYCLOPEDIA_LOCATION_BOX_Y + 18 + ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP - 50, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, L"", FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	if ( ID == 0 )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc1, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 1 && gEncyclopediaDataTemp[LocationID].sDesc2 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc2, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 2 && gEncyclopediaDataTemp[LocationID].sDesc3 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc3, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 3 && gEncyclopediaDataTemp[LocationID].sDesc4 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc4, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 4 && gEncyclopediaDataTemp[LocationID].sDesc5 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc5, ENCYCLOPEDIA_LOCATION_BOX_FONT);	
	else if (ID == 5 && gEncyclopediaDataTemp[LocationID].sDesc6 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc6, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 6 && gEncyclopediaDataTemp[LocationID].sDesc7 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc7, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 7 && gEncyclopediaDataTemp[LocationID].sDesc8 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc8, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 8 && gEncyclopediaDataTemp[LocationID].sDesc9 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc9, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 9 && gEncyclopediaDataTemp[LocationID].sDesc10 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc10, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 10 && gEncyclopediaDataTemp[LocationID].sDesc11 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc11, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 11 && gEncyclopediaDataTemp[LocationID].sDesc12 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc12, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 12 && gEncyclopediaDataTemp[LocationID].sDesc13 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc13, ENCYCLOPEDIA_LOCATION_BOX_FONT);	
	else if (ID == 13 && gEncyclopediaDataTemp[LocationID].sDesc14 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc14, ENCYCLOPEDIA_LOCATION_BOX_FONT);	
	else if (ID == 14 && gEncyclopediaDataTemp[LocationID].sDesc15 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc15, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 15 && gEncyclopediaDataTemp[LocationID].sDesc16 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc16, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 16 && gEncyclopediaDataTemp[LocationID].sDesc17 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc17, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 17 && gEncyclopediaDataTemp[LocationID].sDesc18 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc18, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 18 && gEncyclopediaDataTemp[LocationID].sDesc19 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc19, ENCYCLOPEDIA_LOCATION_BOX_FONT);
	else if (ID == 19 && gEncyclopediaDataTemp[LocationID].sDesc20 != '\0' )
		usStringPixLength = StringPixLength( gEncyclopediaDataTemp[LocationID].sDesc20, ENCYCLOPEDIA_LOCATION_BOX_FONT);
		
	//reset
	if ( ResetVal == TRUE )
	usStringPixLength = StringPixLength( L"", ENCYCLOPEDIA_LOCATION_BOX_FONT);
		
	usPosY = ENCYCLOPEDIA_LOCATION_BOX_Y + 13;
	
	//draw top line of the popup background
	ShadowVideoSurfaceRect( FRAME_BUFFER, ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, usPosY+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, ENCYCLOPEDIA_LOCATION_BOX_X + ENCYCLOPEDIA_LOCATION_BOX_WIDTH+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, usPosY + ENCYCLOPEDIA_LOCATION_BOX_SECTION_HEIGHT+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP-1);

	//draw mid section of the popup background
	usPosY += ENCYCLOPEDIA_LOCATION_BOX_SECTION_HEIGHT;
	for(i=0; i< 10; i++)
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, ENCYCLOPEDIA_LOCATION_BOX_X+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, usPosY+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, ENCYCLOPEDIA_LOCATION_BOX_X + ENCYCLOPEDIA_LOCATION_BOX_WIDTH+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP, usPosY + ENCYCLOPEDIA_LOCATION_BOX_SECTION_HEIGHT+ENCYCLOPEDIA_LOCATION_BOX_SHADOW_GAP-1);
		usPosY += ENCYCLOPEDIA_LOCATION_BOX_SECTION_HEIGHT;
	}
		
	if ( ResetVal == FALSE )  {
	if (ID == 0 )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc1, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 1 && gEncyclopediaDataTemp[LocationID].sDesc2 != '\0')
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc2, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 2 && gEncyclopediaDataTemp[LocationID].sDesc3 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc3, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 3 && gEncyclopediaDataTemp[LocationID].sDesc4 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc4, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 4 && gEncyclopediaDataTemp[LocationID].sDesc5 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc5, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 5 && gEncyclopediaDataTemp[LocationID].sDesc6 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc6, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 6 && gEncyclopediaDataTemp[LocationID].sDesc7 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc7, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 7 && gEncyclopediaDataTemp[LocationID].sDesc8 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc8, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 8 && gEncyclopediaDataTemp[LocationID].sDesc9 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc9, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 9 && gEncyclopediaDataTemp[LocationID].sDesc10 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc10, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 10 && gEncyclopediaDataTemp[LocationID].sDesc11 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc11, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 11 && gEncyclopediaDataTemp[LocationID].sDesc12 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc12, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 12 && gEncyclopediaDataTemp[LocationID].sDesc13 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc13, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 13 && gEncyclopediaDataTemp[LocationID].sDesc14 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc14, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 14 && gEncyclopediaDataTemp[LocationID].sDesc15 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc15, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 15 && gEncyclopediaDataTemp[LocationID].sDesc16 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc16, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 16 && gEncyclopediaDataTemp[LocationID].sDesc17 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc17, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 17 && gEncyclopediaDataTemp[LocationID].sDesc18 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc18, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 18 && gEncyclopediaDataTemp[LocationID].sDesc19 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc19, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	else if ( ID == 19 && gEncyclopediaDataTemp[LocationID].sDesc20 != '\0' )
		DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, gEncyclopediaDataTemp[LocationID].sDesc20, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	}
	//Reset
	if ( ResetVal == TRUE )
	DisplayWrappedString(ENCYCLOPEDIA_LOCATION_BOX_DESC_X, ENCYCLOPEDIA_LOCATION_BOX_DESC_Y + 13, ENCYCLOPEDIA_LOCATION_BOX_TEXT_WIDTH, 6, ENCYCLOPEDIA_LOCATION_BOX_FONT, ENCYCLOPEDIA_LOCATION_BOX_COLOR, L"", FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	
	
	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	
	if ( ENCYCLOPEDIA_PAGE == -1 || ENCYCLOPEDIA_PAGE == 0 )
		RenderButtonDisabled4();
		
	if ( bHidden == TRUE && ( MaxImages == 0 || MaxImages == -1 || MaxImages == 1 ) ) //|| gEncyclopediaDataTemp[0].MaxImages == 1 ) )
		DisableButton( guiEncyclopediaPageButton[1] );
	else if ( bHidden == FALSE && MaxImages > 1 )
		EnableButton( guiEncyclopediaPageButton[1] );
		
	//Reset
	if ( ResetVal == TRUE )	
	DisableButton( guiEncyclopediaPageButton[1] );
		
	if ( gEncyclopediaDataTemp[LocationID].CheckMission == MISSIONSTART || gEncyclopediaDataTemp[LocationID].CheckMission == MISSIONEND )				
		DisableButton ( guiSoundButtons[1] );
	else if ( gEncyclopediaDataTemp[LocationID].CheckMission == MISSIONNOSTARTED )	
		EnableButton ( guiSoundButtons[1] );
		
	//if ( bRenderFiltrBox == TRUE ) RenderFiltrBox();
	
	//if ( ResetVal == TRUE ) 
	//DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, pSectorPageText[ 4 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);

  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
  
}

void SelectEncyclopediaLocationPageRegionCallBack(GUI_BUTTON * btn, INT32 reason )
{ 
	UINT8	ubRetValue = (UINT8)MSYS_GetBtnUserData( btn, 0 );
	
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			
			if ( ubRetValue == 2 )
			{
				ID++;
				
				if ( ID == ENCYCLOPEDIA_PAGE || ID > ENCYCLOPEDIA_PAGE ) DisableButton( guiEncyclopediaPageButton[2] );
				if ( ID > 0 ) EnableButton( guiEncyclopediaPageButton[0] );
				
				RenderEncyclopediaLocation(FALSE);
				RenderMap();
			}
			else if ( ubRetValue == 0 )
			{
				ID--;
				//if ( ID < 0 ) ID = 0;
				if ( ID == 0 ) DisableButton( guiEncyclopediaPageButton[0] );
				if ( ID < ENCYCLOPEDIA_PAGE ) EnableButton( guiEncyclopediaPageButton[2] );
				RenderEncyclopediaLocation(FALSE);
				RenderMap();
			}
			else if ( ubRetValue == 1 )
			{
				IDimage++;
				
				if ( IDimage > MaxImages || IDimage == -1 ) IDimage = 1;
	
				RenderEncyclopediaLocation(FALSE);
				RenderMap();
			}
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}

void SelectEncyclopediaLocationRegionCallBack(GUI_BUTTON * btn, INT32 reason )
{ 
	UINT8	ubRetValue = (UINT8)MSYS_GetBtnUserData( btn, 0 );
	
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			if ( ubRetValue == 2 )
			{
				IDNewLocation++;
				ID = 0;
				
				LocationID = IDNewLocation;
				
				if ( IDNewLocation == MaxLocation - 1 || IDNewLocation == -1 ) DisableButton( guiEncyclopediaLocationPageButton[2] );
				if ( IDNewLocation > 0 ) EnableButton( guiEncyclopediaLocationPageButton[0] );
				
				IDimage = 1;
				
				RenderEncyclopediaLocation(FALSE);
				RenderButtonDisabled2();
				
					if ( ENCYCLOPEDIA_PAGE == -1 || ENCYCLOPEDIA_PAGE == 0 )
						RenderButtonDisabled4();
						
					if ( MaxImages == -1 || MaxImages == 0 || MaxImages == 1 )
						DisableButton( guiEncyclopediaPageButton[1] );
					else
						EnableButton( guiEncyclopediaPageButton[1] );
						
					if( uiSoundSampleBR!=NO_SAMPLE )
					{
						SoundStop( uiSoundSampleBR );
					}	
						
				RenderMap();

			}
			else if ( ubRetValue == 0 )
			{
				ID = 0;
				
				IDNewLocation--;
				
				LocationID = IDNewLocation;
				
				if ( IDNewLocation == 0 || IDNewLocation == -1 ) DisableButton( guiEncyclopediaLocationPageButton[0] );
				if ( IDNewLocation < MaxLocation-1) EnableButton( guiEncyclopediaLocationPageButton[2] );
				
				IDimage = 1;
				
				RenderEncyclopediaLocation(FALSE);
				RenderButtonDisabled2();
				
					if ( ENCYCLOPEDIA_PAGE == -1 || ENCYCLOPEDIA_PAGE == 0 )
						RenderButtonDisabled4();
						
					if ( MaxImages == -1 || MaxImages == 0 || MaxImages == 1 )
						DisableButton( guiEncyclopediaPageButton[1] );
					else
						EnableButton( guiEncyclopediaPageButton[1] );
						
					if( uiSoundSampleBR!=NO_SAMPLE )
					{
						SoundStop( uiSoundSampleBR );
					}	
						
				RenderMap();

			}
			else if ( ubRetValue == 1 )
			{
				//ExitEncyclopediaLocation();
				
					if( uiSoundSampleBR!=NO_SAMPLE )
					{
						SoundStop( uiSoundSampleBR );
					}	
				
				if ( bBriefingRoom == TRUE || bBriefingRoomSpecialMission == TRUE )
				{
					if(!fFirstTimeInEncyclopediaLocation) guiCurrentLaptopMode = LAPTOP_MODE_BRIEFING_ROOM_ENTER;
				}
				else
				{
					if(!fFirstTimeInEncyclopediaLocation) guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA;
				}
			}

		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

	}
}

void SelectFiltrButtonsRegionCallBack(GUI_BUTTON * btn, INT32 reason )
{ 
	UINT8	ubRetValue = (UINT8)MSYS_GetBtnUserData( btn, 0 );
	
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			ResetTemp();
			
			if ( bEncyclopediaLocation == TRUE )
			{
				if ( ubRetValue == 0 )
				{
					CopyToTemp ( gEncyclopediaLocationData, TRUE, NO_FILTR , NO_FILTR, TRUE );
					DisableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 1 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_CITIES , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					DisableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 2 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_SAM , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					DisableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 3 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_OTHER , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					DisableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 4 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_MINES , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					DisableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 5 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_MILITARY , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					DisableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 6 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_LABORATORY , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					DisableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 7 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_FACTORY , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					DisableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 8 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_HOSPITAL , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					DisableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 9 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_PRISON , NO_FILTR, TRUE );
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					DisableButton( guiFiltrPageButton[9] );
					EnableButton( guiFiltrPageButton[10] );
				}
				else if ( ubRetValue == 10 )
				{
					CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_AIRPORT , NO_FILTR, TRUE );	
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
					EnableButton( guiFiltrPageButton[8] );
					EnableButton( guiFiltrPageButton[9] );
					DisableButton( guiFiltrPageButton[10] );
				}
			}
			else if ( bEncyclopediaCharacter == TRUE )
			{
				if ( ubRetValue == 0 )
				{
					CopyToTemp ( gEncyclopediaProfilesData, TRUE, NO_FILTR , NO_FILTR, TRUE  ); //0
					DisableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );	
				}
				else if ( ubRetValue == 1 )
				{
					CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_AIM , NO_FILTR, TRUE  ); //1
					EnableButton( guiFiltrPageButton[0] );	
					DisableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
				}
				else if ( ubRetValue == 2 )
				{
					CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_MERC , NO_FILTR, TRUE  ); //2
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					DisableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
				}
				else if ( ubRetValue == 3 ) //NPC - RPC
				{
					//CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_RPC , NO_FILTR, TRUE  ); //3
					  CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_NPC , PROFIL_RPC, TRUE  ); //3
					  EnableButton( guiFiltrPageButton[0] );	
					  EnableButton( guiFiltrPageButton[1] );
					  EnableButton( guiFiltrPageButton[2] );
					  DisableButton( guiFiltrPageButton[3] );
					  EnableButton( guiFiltrPageButton[4] );
					  EnableButton( guiFiltrPageButton[5] );
					  EnableButton( guiFiltrPageButton[6] );
					  EnableButton( guiFiltrPageButton[7] );
				}
				else if ( ubRetValue == 4 ) //NPC
				{
					CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_NPC , NO_FILTR, TRUE  ); //4 
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					DisableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
				}
				else if ( ubRetValue == 5 )
				{
					CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_VEHICLES , NO_FILTR, TRUE  );	//5
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					DisableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
				}
				else if ( ubRetValue == 6 )
				{
					CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_IMP , NO_FILTR, TRUE  );	//5
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					DisableButton( guiFiltrPageButton[6] );
					EnableButton( guiFiltrPageButton[7] );
				}
				else if ( ubRetValue == 7 ) //NPC - EPC
				{
					CopyToTemp ( gEncyclopediaProfilesData, FALSE, PROFIL_NPC , PROFIL_EPC, TRUE  ); //4/1
					EnableButton( guiFiltrPageButton[0] );	
					EnableButton( guiFiltrPageButton[1] );
					EnableButton( guiFiltrPageButton[2] );
					EnableButton( guiFiltrPageButton[3] );
					EnableButton( guiFiltrPageButton[4] );
					EnableButton( guiFiltrPageButton[5] );
					EnableButton( guiFiltrPageButton[6] );
					DisableButton( guiFiltrPageButton[7] );
				}

			}
			else if ( bEncyclopediaInventory == TRUE )
			{
				if ( ubRetValue == 0 )
				{
					CopyToTemp ( gEncyclopediaInventoryData, TRUE, NO_FILTR,NO_FILTR, TRUE  );
					
					
					
					bRenderFiltrBox[0] = TRUE;
					bRenderFiltrBox[1] = TRUE;
					bRenderFiltrBox[2] = TRUE;
					bRenderFiltrBox[3] = TRUE;					
					
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					bRenderFiltrBox[0] = FALSE;
					bRenderFiltrBox[1] = FALSE;
					bRenderFiltrBox[2] = FALSE;
					bRenderFiltrBox[3] = FALSE;
					
					SetFiltrButtons();
					
					//RenderFiltrBox();
					//RenderBoxDisabledButton();
					//InitAddInventoryFiltrButtons( 9,0, bRenderFiltrBox[0] );
			
				}
				else if ( ubRetValue == 1 )
				{
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					bRenderFiltrBox[0] = TRUE;
					bRenderFiltrBox[1] = FALSE;
					bRenderFiltrBox[2] = FALSE;
					bRenderFiltrBox[3] = FALSE;
					
					SetFiltrButtons();
					
					RenderFiltrBox();
					InitAddInventoryFiltrButtons( 9,0, bRenderFiltrBox[0] );
				}
				//Ammo
				else if ( ubRetValue == 2 )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_AMMO, IC_AMMO, TRUE  );

					bRenderFiltrBox[0] = TRUE;
					bRenderFiltrBox[1] = TRUE;
					bRenderFiltrBox[2] = TRUE;
					bRenderFiltrBox[3] = TRUE;	
					
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);	
					
					bRenderFiltrBox[0] = FALSE;
					bRenderFiltrBox[1] = FALSE;
					bRenderFiltrBox[2] = FALSE;
					bRenderFiltrBox[3] = FALSE;
					
					SetFiltrButtons();
					
					/*
					bRenderFiltrBox[0] = FALSE;
					bRenderFiltrBox[1] = TRUE;
					bRenderFiltrBox[2] = FALSE;
					bRenderFiltrBox[3] = FALSE;
					
					RenderFiltrBox();
					InitAddInventoryFiltrButtons(8,9, FALSE);
					*/
				}
				else if ( ubRetValue == 3 )
				{
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					bRenderFiltrBox[0] = FALSE;
					bRenderFiltrBox[1] = FALSE;
					bRenderFiltrBox[2] = TRUE;
					bRenderFiltrBox[3] = FALSE;
					
					SetFiltrButtons();
					
					RenderFiltrBox();
					InitAddInventoryFiltrButtons(4,21, FALSE);
				}
				else if ( ubRetValue == 4 )
				{
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					bRenderFiltrBox[0] = FALSE;
					bRenderFiltrBox[1] = FALSE;
					bRenderFiltrBox[2] = FALSE;
					bRenderFiltrBox[3] = TRUE;
					
					SetFiltrButtons();
					
					RenderFiltrBox();
					InitAddInventoryFiltrButtons(10,25, FALSE);
				}
			}
			else if ( bEncyclopediaQuests == TRUE )
			{
			
			}
			else if ( bBriefingRoom == TRUE )
			{

			}
			else if ( bBriefingRoomSpecialMission == TRUE )
			{
			
			
			}
				
				RenderEncyclopediaLocation(TRUE);
				RenderMap();
				RenderButtonDisabled3();

		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

	}
}

void SelectInventoryFiltrButtonsRegionCallBack(GUI_BUTTON * btn, INT32 reason )
{ 
	UINT8	ubRetValue = (UINT8)MSYS_GetBtnUserData( btn, 0 );
	
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			ResetTemp();
			
			if ( bBriefingRoomSpecialMission == TRUE )
			{
			
			}
			else if ( bBriefingRoom == TRUE )
			{
			
			}
			else if ( bEncyclopediaQuests == TRUE )
			{

			}
			else if ( bEncyclopediaLocation == TRUE )
			{

			}
			else if ( bEncyclopediaCharacter == TRUE )
			{
					
			}
			else if ( bEncyclopediaInventory == TRUE )
			{
				//Guns
				if ( ubRetValue == 0 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, NOT_GUN, IC_GUN, TRUE );
					
					
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}	
				else if ( ubRetValue == 1 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_PISTOL, IC_GUN, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 2 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_M_PISTOL, IC_GUN, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 3 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_SMG, IC_GUN, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 4 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_RIFLE, IC_GUN, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 5 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_SN_RIFLE, IC_GUN, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 6 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_AS_RIFLE, IC_GUN, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 7 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_LMG, IC_GUN, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 8 && bRenderFiltrBox[0] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_SHOTGUN, IC_GUN, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[0] = FALSE;
					SetFiltrButtons();
				}
				//AMMO
				if ( ubRetValue == 0 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_PISTOL, IC_AMMO, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[1] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 1 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_M_PISTOL, IC_AMMO, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 2 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_SMG, IC_AMMO, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[1] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 3 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_RIFLE, IC_AMMO, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 4 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_SN_RIFLE, IC_AMMO, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 5 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_AS_RIFLE, IC_AMMO, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[1] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 6 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_LMG, IC_AMMO, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[0] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				else if ( ubRetValue == 7 && bRenderFiltrBox[1] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, GUN_SHOTGUN, IC_AMMO, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[1] == TRUE ) bRenderFiltrBox[1] = FALSE;
					SetFiltrButtons();
				}
				
				//ARMOUR
			    if ( ubRetValue == 0 && bRenderFiltrBox[2] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, ARMOURCLASS_HELMET, IC_ARMOUR, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[2] == TRUE ) bRenderFiltrBox[2] = FALSE;
					SetFiltrButtons();
				}
			   else if ( ubRetValue == 1 && bRenderFiltrBox[2] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, ARMOURCLASS_VEST, IC_ARMOUR, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[2] == TRUE ) bRenderFiltrBox[2] = FALSE;
					SetFiltrButtons();
				}
			   else if ( ubRetValue == 2 && bRenderFiltrBox[2] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, ARMOURCLASS_LEGGINGS, IC_ARMOUR, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[2] == TRUE ) bRenderFiltrBox[2] = FALSE;
					SetFiltrButtons();
				}	
			   else if ( ubRetValue == 3 && bRenderFiltrBox[2] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, ARMOURCLASS_PLATE, IC_ARMOUR, TRUE  );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[2] == TRUE ) bRenderFiltrBox[2] = FALSE;
					SetFiltrButtons();
				}	
				/*		
			   else if ( ubRetValue == 4 && bRenderFiltrBox[2] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, ARMOURCLASS_MONST, IC_ARMOUR, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[2] == TRUE ) bRenderFiltrBox[2] == FALSE;
					SetFiltrButtons();
				}	
			   else if ( ubRetValue == 5 && bRenderFiltrBox[2] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, ARMOURCLASS_VEHICLE, IC_ARMOUR, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[2] == TRUE ) bRenderFiltrBox[2] == FALSE;
					SetFiltrButtons();
				}	
				*/
				//misc
			    if ( ubRetValue == 0 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_BLADE, IC_BLADE, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}
			    else if ( ubRetValue == 1 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_THROWING_KNIFE, IC_THROWING_KNIFE, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}
			    else if ( ubRetValue == 2 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_PUNCH, IC_PUNCH, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}
			    else if ( ubRetValue == 3 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_GRENADE, IC_GRENADE, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}		
			    else if ( ubRetValue == 4 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_BOMB, IC_BOMB, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}			
			    else if ( ubRetValue == 5 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_MEDKIT, IC_MEDKIT, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}					
			    else if ( ubRetValue == 6 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_KIT, IC_KIT, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}		
			    else if ( ubRetValue == 7 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_FACE, IC_FACE, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}	
			    else if ( ubRetValue == 8 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_LBEGEAR, IC_LBEGEAR, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}	
			    else if ( ubRetValue == 9 && bRenderFiltrBox[3] == TRUE )
				{
					CopyToTemp ( gEncyclopediaInventoryData, FALSE, IC_MISC, IC_MISC, TRUE );
					if ( bRenderFiltrBox[0] == TRUE ) UnloadInventoryFilterBox(9);
					if ( bRenderFiltrBox[1] == TRUE ) UnloadInventoryFilterBox(8);
					if ( bRenderFiltrBox[2] == TRUE ) UnloadInventoryFilterBox(4);
					if ( bRenderFiltrBox[3] == TRUE ) UnloadInventoryFilterBox(10);
					
					if ( bRenderFiltrBox[3] == TRUE ) bRenderFiltrBox[3] = FALSE;
					SetFiltrButtons();
				}					
			}
				RenderEncyclopediaLocation(FALSE);
				RenderMap();
				RenderButtonDisabled3();
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

	}
}


void SelectSoundButtonsRegionCallBack(GUI_BUTTON * btn, INT32 reason )
{ 
	UINT8	ubRetValue = (UINT8)MSYS_GetBtnUserData( btn, 0 );
	CHAR8 str[MAX_ENCYCLOPEDIA_CHARS];
	
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			
			if ( bEncyclopediaLocation == TRUE )
			{
				
				if ( ubRetValue == 0 )
				{
				
				}
				
			}
			else if ( bEncyclopediaCharacter == TRUE )
			{
			
				if ( ubRetValue == 0 )
				{
				
				}

			}
			else if ( bEncyclopediaInventory == TRUE )
			{
				if ( ubRetValue == 0 )
				{
				
				}
			}
			else if ( bEncyclopediaQuests == TRUE )
			{
			
				if ( ubRetValue == 0 )
				{
				
				}
			
			}
			else if ( bBriefingRoom == TRUE )
			{
			
				if ( ubRetValue == 0 )
				{
				
					if( uiSoundSampleBR!=NO_SAMPLE )
					{
						SoundStop( uiSoundSampleBR );
					}			
					
					sprintf(str, gEncyclopediaDataTemp[LocationID].sSounds);
					uiSoundSampleBR = PlayJA2SampleFromFile( str, RATE_11025, HIGHVOLUME, 1, MIDDLE );
					
				}
				
				if ( ubRetValue == 1 )
				{
					if ( gBriefingRoomData[gEncyclopediaDataTemp[LocationID].MissionID].CheckMission == MISSIONNOSTARTED )
					{
					
						gEncyclopediaDataTemp[LocationID].CheckMission = MISSIONSTART;
						gBriefingRoomData[gEncyclopediaDataTemp[LocationID].MissionID].CheckMission = MISSIONSTART;	
						
						DisableButton ( guiSoundButtons[1] );
					}
				}
				
			}
			else if ( bBriefingRoomSpecialMission == TRUE )
			{
			
				if ( ubRetValue == 0 )
				{
					if( uiSoundSampleBR!=NO_SAMPLE )
					{
						SoundStop( uiSoundSampleBR );
					}			
					
					sprintf(str, gEncyclopediaDataTemp[LocationID].sSounds);
					uiSoundSampleBR = PlayJA2SampleFromFile( str, RATE_11025, HIGHVOLUME, 1, MIDDLE );
			
				}
			
			}
				
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

	}
}

//--------------------------------------------------------------


BOOLEAN SaveEncyclopediaToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;
	UINT32 i;


	//Briefing room
	for(i=0; i<NUM_SECTOR; i++)
	{
		saveBriefingRoomData[i].Hidden = gBriefingRoomData[ i ].Hidden;
		saveBriefingRoomData[i].CheckMission = gBriefingRoomData[ i ].CheckMission;
	}

	FileWrite( hFile, &saveBriefingRoomData, sizeof( saveBriefingRoomData), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( saveBriefingRoomData ) )
	{
		return( FALSE );
	}
	
	//Briefing room , special mission 
	for(i=0; i<NUM_SECTOR; i++)
	{
		saveBriefingRoomSpecialMissionData[i].Hidden = gBriefingRoomSpecialMissionData[ i ].Hidden;
		saveBriefingRoomSpecialMissionData[i].CheckMission = gBriefingRoomSpecialMissionData[ i ].CheckMission;
	}

	FileWrite( hFile, &saveBriefingRoomSpecialMissionData, sizeof( saveBriefingRoomSpecialMissionData), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( saveBriefingRoomSpecialMissionData ) )
	{
		return( FALSE );
	}

	//Quests
	for(i=0; i<MAX_QUESTS; i++)
	{
		saveEncyclopediaQuestsData[i] = gEncyclopediaQuestsData[ i ].Hidden;
	}

	FileWrite( hFile, &saveEncyclopediaQuestsData, sizeof( saveEncyclopediaQuestsData), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( saveEncyclopediaQuestsData ) )
	{
		return( FALSE );
	}

	//Locations
	for(i=0; i<NUM_SECTOR; i++)
	{
		saveEncyclopediaLocationData[i] = gEncyclopediaLocationData[ i ].Hidden;
	}

	FileWrite( hFile, &saveEncyclopediaLocationData, sizeof( saveEncyclopediaLocationData), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( saveEncyclopediaLocationData ) )
	{
		return( FALSE );
	}
	
	//profiles
	for(i=0; i<NUM_PROFILES; i++)
	{
		saveEncyclopediaProfilesData[i].Hidden = gEncyclopediaProfilesData[ i ].Hidden;
		saveEncyclopediaProfilesData[i].bvisible = gEncyclopediaProfilesData[ i ].bvisible;
	}

	FileWrite( hFile, &saveEncyclopediaProfilesData, sizeof( saveEncyclopediaProfilesData), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( saveEncyclopediaProfilesData ) )
	{
		return( FALSE );
	}
	
	//inventory
	for(i=0; i<MAXITEMS; i++)
	{
		saveEncyclopediaInventoryData[i] = gEncyclopediaInventoryData[ i ].Hidden;
	}

	FileWrite( hFile, &saveEncyclopediaInventoryData, sizeof( saveEncyclopediaInventoryData), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( saveEncyclopediaInventoryData ) )
	{
		return( FALSE );
	}

	//old profiles
	for(i=0; i<NUM_PROFILES; i++)
	{
		saveEncyclopediaOldProfilesData[i] = gEncyclopediaOldProfilesData[ i ].Hidden;
	}

	FileWrite( hFile, &saveEncyclopediaOldProfilesData, sizeof( saveEncyclopediaOldProfilesData), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( saveEncyclopediaOldProfilesData ) )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN LoadEncyclopediaFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	UINT32 i;

	//Briefing room
	FileRead( hFile, &saveBriefingRoomData, sizeof( saveBriefingRoomData), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( saveBriefingRoomData ) )
	{
		return( FALSE );
	}

	for(i=0; i<NUM_SECTOR; i++)
	{
		gBriefingRoomData[ i ].Hidden = saveBriefingRoomData[i].Hidden;
		gBriefingRoomData[ i ].CheckMission = saveBriefingRoomData[i].CheckMission;
	}
		
	//Briefing room , special mission
	FileRead( hFile, &saveBriefingRoomSpecialMissionData, sizeof( saveBriefingRoomSpecialMissionData), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( saveBriefingRoomSpecialMissionData ) )
	{
		return( FALSE );
	}

	for(i=0; i<NUM_SECTOR; i++)
	{
		gBriefingRoomSpecialMissionData[ i ].Hidden = saveBriefingRoomSpecialMissionData[i].Hidden;
		gBriefingRoomSpecialMissionData[ i ].CheckMission = saveBriefingRoomSpecialMissionData[i].CheckMission;
	}
	
	//Quests
	FileRead( hFile, &saveEncyclopediaQuestsData, sizeof( saveEncyclopediaQuestsData), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( saveEncyclopediaQuestsData ) )
	{
		return( FALSE );
	}	
	
	for(i=0; i<MAX_QUESTS; i++)
	{
		gEncyclopediaQuestsData[ i ].Hidden = saveEncyclopediaQuestsData[i];
	}
	
	//Locations
	FileRead( hFile, &saveEncyclopediaLocationData, sizeof( saveEncyclopediaLocationData), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( saveEncyclopediaLocationData ) )
	{
		return( FALSE );
	}
	
	for(i=0; i<NUM_SECTOR; i++)
	{
		gEncyclopediaLocationData[ i ].Hidden = saveEncyclopediaLocationData[i];
	}
	
	//profiles
	FileRead( hFile, &saveEncyclopediaProfilesData, sizeof( saveEncyclopediaProfilesData), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( saveEncyclopediaProfilesData ) )
	{
		return( FALSE );
	}
	
	for(i=0; i<NUM_PROFILES; i++)
	{
		gEncyclopediaProfilesData[ i ].Hidden = saveEncyclopediaProfilesData[i].Hidden;
		gEncyclopediaProfilesData[ i ].bvisible = saveEncyclopediaProfilesData[i].bvisible;
	}	
	
	//inventory
	FileRead( hFile, &saveEncyclopediaInventoryData, sizeof( saveEncyclopediaInventoryData), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( saveEncyclopediaInventoryData ) )
	{
		return( FALSE );
	}
	
	for(i=0; i<MAXITEMS; i++)
	{
		gEncyclopediaInventoryData[ i ].Hidden = saveEncyclopediaInventoryData[i];
	}
	
	//old profiles
	FileRead( hFile, &saveEncyclopediaOldProfilesData, sizeof( saveEncyclopediaOldProfilesData), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( saveEncyclopediaOldProfilesData ) )
	{
		return( FALSE );
	}
	
	for(i=0; i<NUM_PROFILES; i++)
	{
		gEncyclopediaOldProfilesData[ i ].Hidden = saveEncyclopediaOldProfilesData[i];
	}

	return( TRUE );
}

//--------------------------------------------------------------

//------------ old filtr
/*
void BtnEncyclopediaTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	
	UINT8	ubRetValue = (UINT8)MSYS_GetBtnUserData( btn, 0 );

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			if ( ubRetValue == 0 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_CITIES);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_CITIES, TRUE );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 1 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_SAM);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_SAM, TRUE );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 2 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_OTHER );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 3 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_MINES );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
		
			if ( ubRetValue == 0 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_CITIES);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_CITIES );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 1 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_SAM);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_SAM );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 2 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_OTHER );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 3 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToTrue( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_MINES );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			/*
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			if ( ubRetValue == 0 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_CITIES);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_CITIES );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 1 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_SAM);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_SAM );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 2 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_OTHER );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
	
			if ( ubRetValue == 3 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, TRUE, LOCATION_MINES );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
		
			if ( ubRetValue == 0 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_CITIES);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_CITIES );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 1 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_SAM);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_SAM );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			
			if ( ubRetValue == 2 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_OTHER );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
	
			if ( ubRetValue == 3 && bEncyclopediaLocation == TRUE )
			{
			//	ResetTemp();
				//FiltrToFalse( gEncyclopediaLocationData,LOCATION_OTHER);
				CopyToTemp ( gEncyclopediaLocationData, FALSE, LOCATION_MINES );
				RenderEncyclopediaLocation();
				RenderMap();
				RenderButtonDisabled3();
			}
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
		}
	}
}
*/