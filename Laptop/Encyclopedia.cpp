#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "Laptop All.h"
	#include "timer control.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "email.h"
	#include "Game Clock.h"
	#include "Text.h"
	#include "soldier profile type.h"
#endif

#include "Encyclopedia_Data.h"
#include "Encyclopedia.h"


// Link Images
#define	ENCYCLOPEDIA_BUTTON_SIZE_X						205
#define	ENCYCLOPEDIA_BUTTON_SIZE_Y						19

#define	ENCYCLOPEDIA_COPYRIGHT_X						iScreenWidthOffset + 160
#define	ENCYCLOPEDIA_COPYRIGHT_Y						iScreenHeightOffset + 415 // + LAPTOP_SCREEN_WEB_DELTA_Y
#define	ENCYCLOPEDIA_COPYRIGHT_WIDTH					400
#define	ENCYCLOPEDIA_COPYRIGHT_GAP						9

#define	IMAGE_OFFSET_X						LAPTOP_SCREEN_UL_X
#define	IMAGE_OFFSET_Y						LAPTOP_SCREEN_WEB_UL_Y


//262, 28

// RustBackGround
#define	RUSTBACKGROUND_SIZE_X				125
#define	RUSTBACKGROUND_SIZE_Y				100

#define	RUSTBACKGROUND_1_X					IMAGE_OFFSET_X
#define	RUSTBACKGROUND_1_Y					IMAGE_OFFSET_Y

#define	ENCYCLOPEDIA_FONT12ARIAL			FONT12ARIAL
#define	ENCYCLOPEDIA_FONT_MCOLOR_WHITE		FONT_MCOLOR_WHITE

#define	ENCYCLOPEDIA_BUTTONS_DEF 			4
#define	ENCYCLOPEDIA_SUBTITLE_Y				iScreenHeightOffset + 150 + LAPTOP_SCREEN_WEB_DELTA_Y
#define ENCYCLOPEDIA_CONTENTBUTTON_X		iScreenWidthOffset + 259
#define ENCYCLOPEDIA_CONTENTBUTTON_Y		ENCYCLOPEDIA_SUBTITLE_Y
#define ENCYCLOPEDIA_TOC_X					ENCYCLOPEDIA_CONTENTBUTTON_X
#define ENCYCLOPEDIA_TOC_Y					5
#define	ENCYCLOPEDIA_TOC_GAP_Y				25	

UINT32		guiRustEncyclopediaBackGround;
UINT32		guiRustEncyclopediaLogoAim;

MOUSE_REGION	gSelectedEncyclopediaTocMenuRegion[ 6 ];
UINT32			guiContentButtonEncy;

void SelectEncyclopediaLocationButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectEncyclopediaCharacterButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectEncyclopediaQuestsButton(MOUSE_REGION * pRegion, INT32 iReason );
void SelectEncyclopediaInentoryButton(MOUSE_REGION * pRegion, INT32 iReason );

void		LaptopInitEncyclopedia();
void InitEncyklopediaBool();

BOOLEAN		fFirstTimeInEncyclopedia = TRUE;

BOOLEAN DrawEncyclopediaLogoAim();

#if (defined ENABLE_ENCYCLOPEDIA || defined ENABLE_BRIEFINGROOM) 
ENCYCLOPEDIA_LOCATION gEncyclopediaDataTemp[ NUM_MAX_TEMP ];
#endif

#ifdef ENABLE_ENCYCLOPEDIA
ENCYCLOPEDIA_LOCATION gEncyclopediaLocationData[ NUM_SECTOR ];
ENCYCLOPEDIA_LOCATION gEncyclopediaProfilesData[ NUM_PROFILES ];
ENCYCLOPEDIA_LOCATION gEncyclopediaInventoryData[ MAXITEMS ];
ENCYCLOPEDIA_LOCATION gEncyclopediaOldProfilesData[ NUM_PROFILES ];
ENCYCLOPEDIA_LOCATION gEncyclopediaQuestsData[ MAX_QUESTS ];

ENCYCLOPEDIA_LOCATION gEncyclopediaLocationDataBackup[ NUM_SECTOR ];
ENCYCLOPEDIA_LOCATION gEncyclopediaProfilesDataBackup[ NUM_PROFILES ];
ENCYCLOPEDIA_LOCATION gEncyclopediaInventoryDataBackup[ MAXITEMS ];
ENCYCLOPEDIA_LOCATION gEncyclopediaOldProfilesDataBackup[ NUM_PROFILES ];
ENCYCLOPEDIA_LOCATION gEncyclopediaQuestsDataBackup[ MAX_QUESTS ];

ENCYCLOPEDIA_PROF_SAVE saveEncyclopediaProfilesData[ NUM_PROFILES ];
BOOLEAN saveEncyclopediaLocationData[ NUM_SECTOR ];
BOOLEAN saveEncyclopediaInventoryData[ MAXITEMS ];
BOOLEAN saveEncyclopediaOldProfilesData[ NUM_PROFILES ];
BOOLEAN saveEncyclopediaQuestsData[ MAX_QUESTS ];
#else
ENCYCLOPEDIA_LOCATION gEncyclopediaProfilesData[ 1 ];
#endif // ENABLE_ENCYCLOPEDIA

#ifdef ENABLE_BRIEFINGROOM
ENCYCLOPEDIA_LOCATION gBriefingRoomData[ NUM_SECTOR ];
ENCYCLOPEDIA_LOCATION gBriefingRoomSpecialMissionData[ NUM_SECTOR ];

ENCYCLOPEDIA_LOCATION gBriefingRoomDataBackup[ NUM_SECTOR ];
ENCYCLOPEDIA_LOCATION gBriefingRoomSpecialMissionDataBackup[ NUM_SECTOR ];

MISSION_SAVE saveBriefingRoomData[ NUM_SECTOR ];
MISSION_SAVE saveBriefingRoomSpecialMissionData[ NUM_SECTOR ];
MISSION_SAVE saveMissionData[ NUM_SECTOR ];
#else
ENCYCLOPEDIA_LOCATION gBriefingRoomData[ 1 ];
#endif // ENABLE_BRIEFINGROOM

void GameInitEncyclopedia()
{
	LaptopInitEncyclopedia();
}

BOOLEAN EnterEncyclopedia()
{
#ifdef ENABLE_ENCYCLOPEDIA
  VOBJECT_DESC    VObjectDesc;
  UINT16		i, usPosY;

	LaptopInitEncyclopedia();

	InitEncyclopediaDefaults();
	
	// load the Rust bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("LAPTOP\\rustbackground.sti", VObjectDesc.ImageFile);
	FilenameForBPP("ENCYCLOPEDIA\\encyclopedialogoaim.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiRustEncyclopediaLogoAim));

	// load the MemberShipcard graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("Encyclopedia\\CONTENTBUTTON.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiContentButtonEncy));

	//** Mouse Regions **
	
	usPosY = ENCYCLOPEDIA_CONTENTBUTTON_Y + 120;
	for(i=0; i<ENCYCLOPEDIA_BUTTONS_DEF; i++)
	{
			if ( i == 0 )
		MSYS_DefineRegion( &gSelectedEncyclopediaTocMenuRegion[i], ENCYCLOPEDIA_TOC_X, usPosY, (UINT16)(ENCYCLOPEDIA_TOC_X + ENCYCLOPEDIA_BUTTON_SIZE_X), (UINT16)(usPosY + ENCYCLOPEDIA_BUTTON_SIZE_Y), MSYS_PRIORITY_HIGH,
									CURSOR_WWW, MSYS_NO_CALLBACK, SelectEncyclopediaLocationButton);
		else if ( i == 1 )
		MSYS_DefineRegion( &gSelectedEncyclopediaTocMenuRegion[i], ENCYCLOPEDIA_TOC_X, usPosY, (UINT16)(ENCYCLOPEDIA_TOC_X + ENCYCLOPEDIA_BUTTON_SIZE_X), (UINT16)(usPosY + ENCYCLOPEDIA_BUTTON_SIZE_Y), MSYS_PRIORITY_HIGH,
							CURSOR_WWW, MSYS_NO_CALLBACK, SelectEncyclopediaCharacterButton);							
		else if ( i == 2 )
		MSYS_DefineRegion( &gSelectedEncyclopediaTocMenuRegion[i], ENCYCLOPEDIA_TOC_X, usPosY, (UINT16)(ENCYCLOPEDIA_TOC_X + ENCYCLOPEDIA_BUTTON_SIZE_X), (UINT16)(usPosY + ENCYCLOPEDIA_BUTTON_SIZE_Y), MSYS_PRIORITY_HIGH,
									CURSOR_WWW, MSYS_NO_CALLBACK, SelectEncyclopediaInentoryButton);								
		else if ( i == 3 )
		MSYS_DefineRegion( &gSelectedEncyclopediaTocMenuRegion[i], ENCYCLOPEDIA_TOC_X, usPosY, (UINT16)(ENCYCLOPEDIA_TOC_X + ENCYCLOPEDIA_BUTTON_SIZE_X), (UINT16)(usPosY + ENCYCLOPEDIA_BUTTON_SIZE_Y), MSYS_PRIORITY_HIGH,
									CURSOR_WWW, MSYS_NO_CALLBACK, SelectEncyclopediaQuestsButton);													
									
		MSYS_AddRegion(&gSelectedEncyclopediaTocMenuRegion[i]);
		
		usPosY += ENCYCLOPEDIA_TOC_GAP_Y;
	}
  
	fFirstTimeInEncyclopedia = FALSE;
	
	RenderEncyclopedia();
#endif // ENABLE_ENCYCLOPEDIA
	return( TRUE );
}

void LaptopInitEncyclopedia()
{

}

void InitEncyklopediaBool()
{
	bEncyclopediaLocation = FALSE;
	bEncyclopediaCharacter = FALSE;
	bEncyclopediaInventory = FALSE;
	bEncyclopediaQuests  = FALSE;
	bBriefingRoom  = FALSE;
	bBriefingRoomSpecialMission = FALSE;
}

void ExitEncyclopedia()
{
#ifdef ENABLE_ENCYCLOPEDIA
	UINT16 i;
	
	//InitEncyklopediaBool();
	
	RemoveEncyclopediaDefaults();

	DeleteVideoObjectFromIndex(guiContentButtonEncy);
	DeleteVideoObjectFromIndex(guiRustEncyclopediaLogoAim);

	// Remove Mouse Regions
	for(i=0; i<ENCYCLOPEDIA_BUTTONS_DEF; i++)
		MSYS_RemoveRegion( &gSelectedEncyclopediaTocMenuRegion[i]);
		
	//guiCurrentLaptopMode = LAPTOP_MODE_NONE;
#endif // ENABLE_ENCYCLOPEDIA
}

void HandleEncyclopedia()
{

}

void RenderEncyclopedia()
{
#ifdef ENABLE_ENCYCLOPEDIA
    UINT16		i, usPosY;
	UINT16		usHeight;
	HVOBJECT	hContentButtonHandle;  
	UINT16		usWidth=0;
	
	DrawEncyclopediaDefaults();
	
	DrawEncyclopediaLogoAim();
	
	GetVideoObject(&hContentButtonHandle, guiContentButtonEncy);

	usHeight = GetFontHeight(ENCYCLOPEDIA_FONT12ARIAL);
	usPosY = ENCYCLOPEDIA_CONTENTBUTTON_Y + 120;
	for(i=0; i<ENCYCLOPEDIA_BUTTONS_DEF; i++)
	{
		BltVideoObject(FRAME_BUFFER, hContentButtonHandle, 0,ENCYCLOPEDIA_TOC_X, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

		usWidth = StringPixLength(pMenuStrings[i], ENCYCLOPEDIA_FONT12ARIAL);
		DrawTextToScreen(pMenuStrings[i], ENCYCLOPEDIA_TOC_X, (UINT16)(usPosY + ENCYCLOPEDIA_TOC_Y), ENCYCLOPEDIA_BUTTON_SIZE_X, ENCYCLOPEDIA_FONT12ARIAL, ENCYCLOPEDIA_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
		
		usPosY += ENCYCLOPEDIA_TOC_GAP_Y;
	}
	
	DisplayEncyclopediaSlogan();

	DisplayEncyclopediaCopyright();

	RenderWWWProgramTitleBar( );
	
	//InitEncyklopediaBool();

  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
#endif // ENABLE_ENCYCLOPEDIA
}

void SelectEncyclopediaLocationButton(MOUSE_REGION * pRegion, INT32 iReason )
{
#ifdef ENABLE_ENCYCLOPEDIA
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		bEncyclopediaLocation = TRUE;
		bEncyclopediaCharacter = FALSE;
		bEncyclopediaInventory = FALSE;
		bEncyclopediaQuests  = FALSE;
		bBriefingRoom  = FALSE;
		bBriefingRoomSpecialMission = FALSE;
		
		ResetVal = TRUE;
		
		IDPageEncyData = PAGEENCYCLOPEDIALOCATION; 
		
		if(!fFirstTimeInEncyclopedia) guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA_LOCATION;	
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
#endif // ENABLE_ENCYCLOPEDIA
}

void SelectEncyclopediaCharacterButton(MOUSE_REGION * pRegion, INT32 iReason )
{
#ifdef ENABLE_ENCYCLOPEDIA
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		bEncyclopediaLocation = FALSE;
		bEncyclopediaCharacter = TRUE;
		bEncyclopediaInventory = FALSE;
		bEncyclopediaQuests  = FALSE;
		bBriefingRoom  = FALSE;
		bBriefingRoomSpecialMission = FALSE;
		
		ResetVal = TRUE;
		
		IDPageEncyData = PAGEENCYCLOPEDIACHARACTER; 
		
		if(!fFirstTimeInEncyclopedia) guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA_LOCATION;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
#endif // ENABLE_ENCYCLOPEDIA
}

void SelectEncyclopediaQuestsButton(MOUSE_REGION * pRegion, INT32 iReason )
{
#ifdef ENABLE_ENCYCLOPEDIA
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		bEncyclopediaLocation = FALSE;
		bEncyclopediaCharacter = FALSE;
		bEncyclopediaInventory = FALSE;
		bEncyclopediaQuests  = TRUE;
		bBriefingRoom  = FALSE;
		bBriefingRoomSpecialMission = FALSE;
		
		ResetVal = FALSE;
		
		IDPageEncyData = PAGEENCYCLOPEDIAQUESTS; 
		
		if(!fFirstTimeInEncyclopedia) guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA_LOCATION; 
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
#endif // ENABLE_ENCYCLOPEDIA
}

void SelectEncyclopediaInentoryButton(MOUSE_REGION * pRegion, INT32 iReason )
{
#ifdef ENABLE_ENCYCLOPEDIA
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		bEncyclopediaLocation = FALSE;
		bEncyclopediaCharacter = FALSE;
		bEncyclopediaInventory = TRUE;
		bEncyclopediaQuests  = FALSE;
		bBriefingRoom  = FALSE;
		bBriefingRoomSpecialMission = FALSE;
		
		ResetVal = FALSE;
		
		IDPageEncyData = PAGEENCYCLOPEDIAINVENTORY; 
		
		if(!fFirstTimeInEncyclopedia) guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA_LOCATION;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
#endif // ENABLE_ENCYCLOPEDIA
}

BOOLEAN InitEncyclopediaDefaults()
{
//#ifdef ENABLE_ENCYCLOPEDIA
#if (defined ENABLE_ENCYCLOPEDIA || defined ENABLE_BRIEFINGROOM) 
  VOBJECT_DESC    VObjectDesc;

	// load the Rust bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	//FilenameForBPP("LAPTOP\\rustbackground.sti", VObjectDesc.ImageFile);
	FilenameForBPP("ENCYCLOPEDIA\\encyclopediabackground.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiRustEncyclopediaBackGround));

#endif // ENABLE_ENCYCLOPEDIA
	return(TRUE);
}

BOOLEAN RemoveEncyclopediaDefaults()
{
//#ifdef ENABLE_ENCYCLOPEDIA
#if (defined ENABLE_ENCYCLOPEDIA || defined ENABLE_BRIEFINGROOM) 
	DeleteVideoObjectFromIndex(guiRustEncyclopediaBackGround);
#endif // ENABLE_ENCYCLOPEDIA
	return(TRUE);
}


BOOLEAN DrawEncyclopediaLogoAim()
{
#ifdef ENABLE_ENCYCLOPEDIA
  HVOBJECT hLogoEncyklopedia;
//	UINT16	x,y, uiPosX, uiPosY;

	GetVideoObject(&hLogoEncyklopedia, guiRustEncyclopediaLogoAim);
	BltVideoObject(FRAME_BUFFER, hLogoEncyklopedia, 0,ENCYCLOPEDIA_TOC_X, RUSTBACKGROUND_1_Y + 40, VO_BLT_SRCTRANSPARENCY,NULL);

#endif // ENABLE_ENCYCLOPEDIA
	return(TRUE);
}

BOOLEAN DrawEncyclopediaDefaults()
{
//#ifdef ENABLE_ENCYCLOPEDIA
#if (defined ENABLE_ENCYCLOPEDIA || defined ENABLE_BRIEFINGROOM) 
  HVOBJECT hRustBackGroundHandle;
// UINT16	x,y, uiPosX, uiPosY;

	// Blt the rust background
	GetVideoObject(&hRustBackGroundHandle, guiRustEncyclopediaBackGround);
	BltVideoObject(FRAME_BUFFER, hRustBackGroundHandle, 0,RUSTBACKGROUND_1_X, RUSTBACKGROUND_1_Y, VO_BLT_SRCTRANSPARENCY,NULL);
/*
	uiPosY = RUSTBACKGROUND_1_Y;
	for(y=0; y<4; y++)
	{
		uiPosX = RUSTBACKGROUND_1_X;
		for(x=0; x<4; x++)
		{
		  BltVideoObject(FRAME_BUFFER, hRustBackGroundHandle, 0,uiPosX, uiPosY, VO_BLT_SRCTRANSPARENCY,NULL);
			uiPosX += RUSTBACKGROUND_SIZE_X;
		}
		uiPosY += RUSTBACKGROUND_SIZE_Y;
	}
	*/
#endif // ENABLE_ENCYCLOPEDIA
	return(TRUE);
}

BOOLEAN DisplayEncyclopediaSlogan()
{

	return(TRUE);
}

BOOLEAN DisplayEncyclopediaCopyright()
{


	return(TRUE);
}

void ShowNPCEncyclopediaEntry(UINT8 ubNPC, BOOLEAN hidden)
{
#ifdef ENABLE_ENCYCLOPEDIA
	gEncyclopediaProfilesData[ubNPC].Hidden = TRUE; //encyclopedia
#endif
}
