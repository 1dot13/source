#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "AimFacialIndex.h"
	#include "WordWrap.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "stdio.h"
	#include "Aim.h"
	#include "Soldier Profile.h"
	#include "email.h"
	#include "Text.h"
	#include "aimsort.h"
	#include "Assignments.h"
	#include "GameSettings.h"
	#include "english.h"
	#include "sysutil.h"
#endif


extern UINT8	gubCurrentSortMode; // symbol already defined in AimSort.cpp (jonathanl)
extern UINT8	gubCurrentListMode; // symbol already declared globally in AimSort.cpp (jonathanl)
extern UINT8	gbCurrentIndex;


UINT32		guiMugShotBorder;
UINT32		guiAimFiFace[ NUM_PROFILES ]; //MAX_NUMBER_MERCS

BOOLEAN		gAimProfiles[ NUM_PROFILES ];

#define		AIM_FI_NUM_MUGSHOTS_X		8
#define		AIM_FI_NUM_MUGSHOTS_Y		5

UINT8 START_MERC =0;
UINT8 END_MERC =0;

#define		AIM_FI_PORTRAIT_WIDTH		52
#define		AIM_FI_PORTRAIT_HEIGHT	48

#define		AIM_FI_FIRST_MUGSHOT_X	IMAGE_OFFSET_X + 6
#define		AIM_FI_FIRST_MUGSHOT_Y	IMAGE_OFFSET_Y + 69//67//70 //68 //65
#define		AIM_FI_MUGSHOT_GAP_X		10
#define		AIM_FI_MUGSHOT_GAP_Y		13
#define		AIM_FI_FACE_OFFSET			2

#define		AIM_FI_NNAME_OFFSET_X		2
#define		AIM_FI_NNAME_OFFSET_Y		AIM_FI_PORTRAIT_HEIGHT+1
#define		AIM_FI_NNAME_WIDTH			AIM_FI_PORTRAIT_WIDTH+4

#define		AIM_FI_MEMBER_TEXT_X		IMAGE_OFFSET_X + 155
#define		AIM_FI_MEMBER_TEXT_Y		AIM_SYMBOL_Y + AIM_SYMBOL_SIZE_Y + 1
#define		AIM_FI_MEMBER_TEXT_WIDTH	190
//tais: nsgi, move member text up
#define		AIM_FI_MEMBER_TEXT_Y_NSGI		AIM_SYMBOL_SMALL_Y + 35


#define		AIM_FI_AWAY_TEXT_OFFSET_X			3
#define		AIM_FI_AWAY_TEXT_OFFSET_Y			23//3//36
#define		AIM_FI_AWAY_TEXT_OFFSET_WIDTH	48


//Mouse Regions

//Face regions
MOUSE_REGION		gMercFaceMouseRegions[ NUM_PROFILES ]; //MAX_NUMBER_MERCS
void SelectMercFaceRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SelectMercFaceMoveRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//Screen region, used to right click to go back to previous page
MOUSE_REGION		gScreenMouseRegions;
void SelectScreenRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


BOOLEAN DrawMercsFaceToScreen(UINT8 ubMercID, UINT16 usPosX, UINT16 usPosY, UINT8 ubImage);

//New profiles
INT32		guiPreviousNewProfilesNextButtonImage;
void BtnNewProfilesButtonCallback(GUI_BUTTON *btn,INT32 reason);

INT32 PAGE_BUTTON;

//Hotkey Assignment
void HandleAimFacialIndexKeyBoardInput();

void GameInitAimFacialIndex()
{

}

/// Calculates the needed string for the page button based on number of mercs
/// and currently displayed page.
static STR16 GetPageButtonText() 
{
	const int MERCS_PER_PAGE = AIM_FI_NUM_MUGSHOTS_X * AIM_FI_NUM_MUGSHOTS_Y;

	int baseTextIndex;
	// Display three pages of mercs (strings from gszAimPages[2] to gszAimPages[4])
	if (MAX_NUMBER_MERCS > MERCS_PER_PAGE * 2)
		baseTextIndex = 2;
	// Display two pages of mercs (strings from gszAimPages[0] to gszAimPages[1])
	else if (MAX_NUMBER_MERCS > MERCS_PER_PAGE)
	{
		Assert(START_MERC < MERCS_PER_PAGE * 2);
		baseTextIndex = 0;
	}
	// Display one page of mercs (string gszAimPages[5])
	else
	{
		Assert(START_MERC < MERCS_PER_PAGE);
		baseTextIndex = 5;
	}

	int currentPage = START_MERC / MERCS_PER_PAGE;
	int stringIndex = baseTextIndex + currentPage;
	return gszAimPages[ stringIndex ];
}

void BtnNewProfilesButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			UINT32 i;
			
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			for(i=0; i<MAX_NUMBER_MERCS; i++)
			{
				gAimProfiles[i] = TRUE;
			}

			if ( START_MERC == 0 )
			{
				START_MERC = 40;
			}	
			else if ( START_MERC == 40 )
			{
				if ( MAX_NUMBER_MERCS > 80 )
					START_MERC = 80;
				else
					START_MERC = 0;
			}
			else
			{
				START_MERC = 0;
			}

			ExitAimFacialIndex();
			EnterAimFacialIndex();

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

BOOLEAN EnterAimFacialIndex()
{
	VOBJECT_DESC	VObjectDesc;
	UINT8	i;
	UINT16		usPosX, usPosY, x,y;
	STR				sFaceLoc = "FACES\\";
	char			sTemp[100];
	
	UINT8 p = 0;

	for(i=0; i<MAX_NUMBER_MERCS; i++)
	{
		gAimProfiles[i] = TRUE;
	}

	// load the Portait graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\MugShotBorder3.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMugShotBorder));
	
	//Page button
	guiPreviousNewProfilesNextButtonImage =	LoadButtonImage("LAPTOP\\BottomButtons2.sti", -1,0,-1,1,-1 ); //New profiles
	
	STR16 buttonText = GetPageButtonText();
	PAGE_BUTTON = CreateIconAndTextButton( guiPreviousNewProfilesNextButtonImage, buttonText, FONT14ARIAL,
														FONT_MCOLOR_DKWHITE, DEFAULT_SHADOW,
														138, DEFAULT_SHADOW,
														TEXT_CJUSTIFIED,
														IMAGE_OFFSET_X + 6, IMAGE_OFFSET_Y + 35, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
														DEFAULT_MOVE_CALLBACK, BtnNewProfilesButtonCallback);
	SetButtonCursor(PAGE_BUTTON, CURSOR_WWW );
	
	usPosX = AIM_FI_FIRST_MUGSHOT_X;
	usPosY = AIM_FI_FIRST_MUGSHOT_Y;
	i=0;
	for(y=0; y<AIM_FI_NUM_MUGSHOTS_Y; y++)
	{
		for(x=0; x<AIM_FI_NUM_MUGSHOTS_X; x++)
		{
		
			if ( gAimProfiles[i+START_MERC] == TRUE ) //new profiles
			{
			
				MSYS_DefineRegion( &gMercFaceMouseRegions[ i ], usPosX, usPosY, (INT16)(usPosX + AIM_FI_PORTRAIT_WIDTH), (INT16)(usPosY + AIM_FI_PORTRAIT_HEIGHT), MSYS_PRIORITY_HIGH,
									CURSOR_WWW, SelectMercFaceMoveRegionCallBack, SelectMercFaceRegionCallBack);
				// Add region
				MSYS_AddRegion( &gMercFaceMouseRegions[ i ] );
				MSYS_SetRegionUserData( &gMercFaceMouseRegions[ i ], 0, i);

			if (gGameExternalOptions.fReadProfileDataFromXML)
			{
				// HEADROCK PROFEX: Do not read direct profile number, instead, look inside the profile for a ubFaceIndex value.
				//sprintf(sTemp, "%s%02d.sti", sFaceLoc, gMercProfiles[AimMercArray[i]].ubFaceIndex);
				sprintf(sTemp, "%s%02d.sti", sFaceLoc, gMercProfiles[gAimAvailability[AimMercArray[i + START_MERC]].ProfilId].ubFaceIndex);
				//sprintf(sTemp, "%s%02d.sti", sFaceLoc, gAimAvailability[AimMercArray[i]].ProfilId);
			}
			else
			{
				//sprintf(sTemp, "%s%02d.sti", sFaceLoc, AimMercArray[i]);
				sprintf(sTemp, "%s%02d.sti", sFaceLoc, gAimAvailability[AimMercArray[i + START_MERC]].ProfilId );
			}
			VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
			FilenameForBPP(sTemp, VObjectDesc.ImageFile);
			if( !AddVideoObject(&VObjectDesc, &guiAimFiFace[i]) )
				return( FALSE );
			}
			
			usPosX += AIM_FI_PORTRAIT_WIDTH + AIM_FI_MUGSHOT_GAP_X;
			i++;
		}
		usPosX = AIM_FI_FIRST_MUGSHOT_X;
		usPosY += AIM_FI_PORTRAIT_HEIGHT + AIM_FI_MUGSHOT_GAP_Y;
	}

	MSYS_DefineRegion( &gScreenMouseRegions, LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y, MSYS_PRIORITY_HIGH-1,
						CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, SelectScreenRegionCallBack);
	// Add region
	MSYS_AddRegion( &gScreenMouseRegions );


	InitAimMenuBar();
	InitAimDefaults();

	RenderAimFacialIndex();

	return( TRUE );
}

void ExitAimFacialIndex()
{
	UINT8	i;

	RemoveAimDefaults();

	DeleteVideoObjectFromIndex(guiMugShotBorder);


	for(i=0; i<NUM_PROFILES; i++) //MAX_NUMBER_MERCS
	{
		if ( gAimProfiles[i] == TRUE )
		{
			DeleteVideoObjectFromIndex( guiAimFiFace[i]);
			MSYS_RemoveRegion( &gMercFaceMouseRegions[ i ]);
		}
	}
	
	RemoveButton( PAGE_BUTTON );
	
	
	ExitAimMenuBar();

	MSYS_RemoveRegion( &gScreenMouseRegions);
}

void HandleAimFacialIndex()
{
//	if( fShowBookmarkInfo )
//		fPausedReDrawScreenFlag = TRUE;

	HandleAimFacialIndexKeyBoardInput();
}

BOOLEAN RenderAimFacialIndex()
{
	UINT16		usPosX, usPosY, x,y;
	CHAR16		sString[150];
	UINT8			i;
	
	SpecifyButtonText( PAGE_BUTTON, GetPageButtonText() );

	if ( MAX_NUMBER_MERCS > 40 )
	{
		ShowButton( PAGE_BUTTON );
	}
	else
	{
		HideButton( PAGE_BUTTON );
	}

	DrawAimDefaults();

	//Display the 'A.I.M. Members Sorted Ascending By Price' type string
	if( gubCurrentListMode == AIM_ASCEND )
		swprintf(sString, AimFiText[ AIM_FI_AIM_MEMBERS_SORTED_ASCENDING ], AimFiText[gubCurrentSortMode] );
	else
		swprintf(sString, AimFiText[ AIM_FI_AIM_MEMBERS_SORTED_DESCENDING ], AimFiText[gubCurrentSortMode] );

	if(gGameExternalOptions.gfUseNewStartingGearInterface) DrawTextToScreen(sString, AIM_FI_MEMBER_TEXT_X, AIM_FI_MEMBER_TEXT_Y_NSGI, AIM_FI_MEMBER_TEXT_WIDTH, AIM_MAINTITLE_FONT, AIM_MAINTITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	else DrawTextToScreen(sString, AIM_FI_MEMBER_TEXT_X, AIM_FI_MEMBER_TEXT_Y, AIM_FI_MEMBER_TEXT_WIDTH, AIM_MAINTITLE_FONT, AIM_MAINTITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	

	//Draw the mug shot border and face
	usPosX = AIM_FI_FIRST_MUGSHOT_X;
	usPosY = AIM_FI_FIRST_MUGSHOT_Y;

	i=0;
	for(y=0; y<AIM_FI_NUM_MUGSHOTS_Y; y++)
	{
		for(x=0; x<AIM_FI_NUM_MUGSHOTS_X; x++)
		{
			if ( gAimProfiles[i+START_MERC] == TRUE ) //new profiles
			{
				DrawMercsFaceToScreen(i, usPosX, usPosY, 1);
			//	DrawTextToScreen(gMercProfiles[AimMercArray[i]].zNickname, (UINT16)(usPosX - AIM_FI_NNAME_OFFSET_X), (UINT16)(usPosY + AIM_FI_NNAME_OFFSET_Y), AIM_FI_NNAME_WIDTH, AIM_FONT12ARIAL, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
				DrawTextToScreen(gMercProfiles[gAimAvailability[AimMercArray[i+START_MERC]].ProfilId].zNickname, (UINT16)(usPosX - AIM_FI_NNAME_OFFSET_X), (UINT16)(usPosY + AIM_FI_NNAME_OFFSET_Y), AIM_FI_NNAME_WIDTH, AIM_FONT12ARIAL, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
			}
			usPosX += AIM_FI_PORTRAIT_WIDTH + AIM_FI_MUGSHOT_GAP_X;
			i++;
		}
		usPosX = AIM_FI_FIRST_MUGSHOT_X;
		usPosY += AIM_FI_PORTRAIT_HEIGHT + AIM_FI_MUGSHOT_GAP_Y;
	}

	DisableAimButton();

	//display the 'left and right click' onscreen help msg
	if(gGameExternalOptions.gfUseNewStartingGearInterface)
	{
		DrawTextToScreen(AimFiText[AIM_FI_LEFT_CLICK], AIM_FI_LEFT_CLICK_TEXT_X_NSGI, AIM_FI_LEFT_CLICK_TEXT_Y_NSGI, AIM_FI_CLICK_TEXT_WIDTH, AIM_FI_HELP_TITLE_FONT, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		DrawTextToScreen(AimFiText[AIM_FI_TO_SELECT], AIM_FI_LEFT_CLICK_TEXT_X_NSGI, AIM_FI_LEFT_CLICK_TEXT_Y_NSGI+AIM_FI_CLICK_DESC_TEXT_Y_OFFSET, AIM_FI_CLICK_TEXT_WIDTH, AIM_FI_HELP_FONT, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	}
	else
	{
		DrawTextToScreen(AimFiText[AIM_FI_LEFT_CLICK], AIM_FI_LEFT_CLICK_TEXT_X, AIM_FI_LEFT_CLICK_TEXT_Y, AIM_FI_CLICK_TEXT_WIDTH, AIM_FI_HELP_TITLE_FONT, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		DrawTextToScreen(AimFiText[AIM_FI_TO_SELECT], AIM_FI_LEFT_CLICK_TEXT_X, AIM_FI_LEFT_CLICK_TEXT_Y+AIM_FI_CLICK_DESC_TEXT_Y_OFFSET, AIM_FI_CLICK_TEXT_WIDTH, AIM_FI_HELP_FONT, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	}

	DrawTextToScreen(AimFiText[AIM_FI_RIGHT_CLICK], AIM_FI_RIGHT_CLICK_TEXT_X, AIM_FI_LEFT_CLICK_TEXT_Y, AIM_FI_CLICK_TEXT_WIDTH, AIM_FI_HELP_TITLE_FONT, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	DrawTextToScreen(AimFiText[AIM_FI_TO_ENTER_SORT_PAGE], AIM_FI_RIGHT_CLICK_TEXT_X, AIM_FI_LEFT_CLICK_TEXT_Y+AIM_FI_CLICK_DESC_TEXT_Y_OFFSET, AIM_FI_CLICK_TEXT_WIDTH, AIM_FI_HELP_FONT, AIM_FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);

	MarkButtonsDirty( );

	RenderWWWProgramTitleBar( );

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
	return(TRUE);
}

void SelectMercFaceRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_AIM_MEMBERS;
		gbCurrentIndex = (UINT8) MSYS_GetRegionUserData( pRegion, 0 ) + START_MERC;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES;
	}
}


void SelectScreenRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES;
	}
}


void SelectMercFaceMoveRegionCallBack(MOUSE_REGION * pRegion, INT32 reason )
{
	UINT8	ubMercNum;
	UINT16 usPosX, usPosY;
	UINT16 ty1, ty2, tx1, tx2;

	ubMercNum = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	ty1 = AIM_FI_FIRST_MUGSHOT_Y;
	ty2 = (AIM_FI_PORTRAIT_HEIGHT + AIM_FI_MUGSHOT_GAP_Y);

	tx1 = AIM_FI_FIRST_MUGSHOT_X;
	tx2 = (AIM_FI_PORTRAIT_WIDTH + AIM_FI_MUGSHOT_GAP_X);

	usPosY = ubMercNum / AIM_FI_NUM_MUGSHOTS_X;
	usPosY = AIM_FI_FIRST_MUGSHOT_Y + (AIM_FI_PORTRAIT_HEIGHT + AIM_FI_MUGSHOT_GAP_Y) * usPosY;

	usPosX = ubMercNum % AIM_FI_NUM_MUGSHOTS_X;
	usPosX = AIM_FI_FIRST_MUGSHOT_X + (AIM_FI_PORTRAIT_WIDTH + AIM_FI_MUGSHOT_GAP_X) * usPosX;

//	fReDrawNewMailFlag = TRUE;
	if ( gAimProfiles[ubMercNum + START_MERC] == TRUE ) //new  profiles
	{
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		pRegion->uiFlags &= (~BUTTON_CLICKED_ON );
		DrawMercsFaceToScreen(ubMercNum, usPosX, usPosY, 1);
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		pRegion->uiFlags |= BUTTON_CLICKED_ON ;
		DrawMercsFaceToScreen(ubMercNum, usPosX, usPosY, 0);
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	
	}
}

BOOLEAN DrawMercsFaceToScreen(UINT8 ubMercID, UINT16 usPosX, UINT16 usPosY, UINT8 ubImage)
{
	HVOBJECT	hMugShotBorderHandle;
	HVOBJECT	hFaceHandle;
	SOLDIERTYPE	*pSoldier=NULL;

	//pSoldier = FindSoldierByProfileID( AimMercArray[ubMercID], TRUE );
	pSoldier = FindSoldierByProfileID( gAimAvailability[AimMercArray[ubMercID + START_MERC]].ProfilId, TRUE );
	
	


	//Blt the portrait background
	GetVideoObject(&hMugShotBorderHandle, guiMugShotBorder);
	BltVideoObject(FRAME_BUFFER, hMugShotBorderHandle, ubImage,usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

	//Blt face to screen
	GetVideoObject(&hFaceHandle, guiAimFiFace[ubMercID]);
	BltVideoObject(FRAME_BUFFER, hFaceHandle, 0,usPosX+AIM_FI_FACE_OFFSET, usPosY+AIM_FI_FACE_OFFSET, VO_BLT_SRCTRANSPARENCY,NULL);

	//if( IsMercDead( AimMercArray[ubMercID] ) )
	if( IsMercDead( gAimAvailability[AimMercArray[ubMercID + START_MERC]].ProfilId ) )
	{
		//get the face object
		GetVideoObject(&hFaceHandle, guiAimFiFace[ubMercID]);

		//if the merc is dead
		//shade the face red, (to signif that he is dead)
		hFaceHandle->pShades[ 0 ]		= Create16BPPPaletteShaded( hFaceHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );

		//set the red pallete to the face
		SetObjectHandleShade( guiAimFiFace[ubMercID], 0 );

		//Blt face to screen
		BltVideoObject(FRAME_BUFFER, hFaceHandle, 0,usPosX+AIM_FI_FACE_OFFSET, usPosY+AIM_FI_FACE_OFFSET, VO_BLT_SRCTRANSPARENCY,NULL);

		DrawTextToScreen(AimFiText[AIM_FI_DEAD], (UINT16)(usPosX+AIM_FI_AWAY_TEXT_OFFSET_X), (UINT16)(usPosY+AIM_FI_AWAY_TEXT_OFFSET_Y), AIM_FI_AWAY_TEXT_OFFSET_WIDTH, FONT10ARIAL, 145, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	}

	//else if the merc is currently a POW or, the merc was fired as a pow
	//else if( gMercProfiles[ AimMercArray[ubMercID] ].bMercStatus == MERC_FIRED_AS_A_POW	|| ( pSoldier &&	pSoldier->bAssignment == ASSIGNMENT_POW ) )
	else if( gMercProfiles[ gAimAvailability[AimMercArray[ubMercID + START_MERC]].ProfilId ].bMercStatus == MERC_FIRED_AS_A_POW	|| ( pSoldier &&	pSoldier->bAssignment == ASSIGNMENT_POW ) )
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX+AIM_FI_FACE_OFFSET, usPosY+AIM_FI_FACE_OFFSET, usPosX + 48+AIM_FI_FACE_OFFSET, usPosY + 43+AIM_FI_FACE_OFFSET);
		DrawTextToScreen( pPOWStrings[0], (UINT16)(usPosX+AIM_FI_AWAY_TEXT_OFFSET_X), (UINT16)(usPosY+AIM_FI_AWAY_TEXT_OFFSET_Y), AIM_FI_AWAY_TEXT_OFFSET_WIDTH, FONT10ARIAL, 145, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	}

	//if the merc is on our team
	else if( pSoldier != NULL )
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX+AIM_FI_FACE_OFFSET, usPosY+AIM_FI_FACE_OFFSET, usPosX + 48+AIM_FI_FACE_OFFSET, usPosY + 43+AIM_FI_FACE_OFFSET);
		DrawTextToScreen( MercInfo[MERC_FILES_ALREADY_HIRED], (UINT16)(usPosX+AIM_FI_AWAY_TEXT_OFFSET_X), (UINT16)(usPosY+AIM_FI_AWAY_TEXT_OFFSET_Y), AIM_FI_AWAY_TEXT_OFFSET_WIDTH, FONT10ARIAL, 145, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
	}

	//if the merc is away, shadow his/her face and blit 'away' over top
	//else if( !IsMercHireable( AimMercArray[ubMercID] ) )
	else if( !IsMercHireable( gAimAvailability[AimMercArray[ubMercID + START_MERC ]].ProfilId ) )	
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, usPosX+AIM_FI_FACE_OFFSET, usPosY+AIM_FI_FACE_OFFSET, usPosX + 48+AIM_FI_FACE_OFFSET, usPosY + 43+AIM_FI_FACE_OFFSET);
		DrawTextToScreen( AimFiText[AIM_FI_DEAD+1], (UINT16)(usPosX+AIM_FI_AWAY_TEXT_OFFSET_X), (UINT16)(usPosY+AIM_FI_AWAY_TEXT_OFFSET_Y), AIM_FI_AWAY_TEXT_OFFSET_WIDTH, FONT10ARIAL, 145, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		//if not enough room use this..
		//AimFiText[AIM_FI_AWAY]
	}

	return(TRUE);
}



void HandleAimFacialIndexKeyBoardInput()
{
	InputAtom					InputEvent;
	UINT32	i;

	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{//!HandleTextInput( &InputEvent ) &&
		if( InputEvent.usEvent == KEY_DOWN )
		{
			switch (InputEvent.usParam)
			{
				case BACKSPACE:
					// back to AIM sorting screen
					guiCurrentLaptopMode = LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES;
					break;
				case ENTER:
					guiCurrentLaptopMode = LAPTOP_MODE_AIM_MEMBERS;
					break;
				case LEFTARROW:
				case 'a':
					if ( MAX_NUMBER_MERCS > (AIM_FI_NUM_MUGSHOTS_X * AIM_FI_NUM_MUGSHOTS_Y) )
					{
						// previous page
						for(i=0; i<MAX_NUMBER_MERCS; i++)
						{
							gAimProfiles[i] = TRUE;
						}

						if ( START_MERC == 40 )
						{
							START_MERC = 0;
						}	
						else if ( START_MERC == 0 )
						{
							if ( MAX_NUMBER_MERCS > 80 )
								START_MERC = 80;
							else
								START_MERC = 40;
						}
						else
						{
							START_MERC = 40;
						}
					
						ExitAimFacialIndex();
						EnterAimFacialIndex();
					}
					break;
				case RIGHTARROW:
				case 'd':
					if ( MAX_NUMBER_MERCS > (AIM_FI_NUM_MUGSHOTS_X * AIM_FI_NUM_MUGSHOTS_Y) )
					{
						// next page
						for(i=0; i<MAX_NUMBER_MERCS; i++)
						{
							gAimProfiles[i] = TRUE;
						}

						if ( START_MERC == 0 )
						{
							START_MERC = 40;
						}	
						else if ( START_MERC == 40 )
						{
							if ( MAX_NUMBER_MERCS > 80 )
								START_MERC = 80;
							else
								START_MERC = 0;
						}
						else
						{
							START_MERC = 0;
						}
					
						ExitAimFacialIndex();
						EnterAimFacialIndex();
					}
					break;
				default:
					HandleKeyBoardShortCutsForLapTop( InputEvent.usEvent, InputEvent.usParam, InputEvent.usKeyState );
					break;
			}
		}
	}
}


