#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "BobbyR.h"
	#include "BobbyRGuns.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "WordWrap.h"
	#include "Cursors.h"
	#include "interface items.h"
	#include "Weapons.h"
	#include "Store Inventory.h"
	#include "Game Event Hook.h"
	#include "Game Clock.h"
	#include "LaptopSave.h"
	#include "Random.h"
	#include "Text.h"
	#include "Multi Language Graphic Utils.h"
//	#include "Utility.h"
	#include "armsdealerinvinit.h"
	#include "GameSettings.h"
	#include "message.h"
	#include "postalservice.h"
#endif


#ifdef JA2TESTVERSION
	#define BR_INVENTORY_TURNOVER_DEBUG
#endif


#define BOBBIES_SIGN_FONT							FONT14ARIAL
#define BOBBIES_SIGN_COLOR						2
#define BOBBIES_SIGN_BACKCOLOR				FONT_MCOLOR_BLACK
#define BOBBIES_SIGN_BACKGROUNDCOLOR				78

#define BOBBIES_NUMBER_SIGNS					5

#define BOBBIES_SENTENCE_FONT					FONT12ARIAL
#define BOBBIES_SENTENCE_COLOR				FONT_MCOLOR_WHITE
#define BOBBIES_SENTENCE_BACKGROUNDCOLOR			2

#define BOBBY_WOOD_BACKGROUND_X				LAPTOP_SCREEN_UL_X
#define BOBBY_WOOD_BACKGROUND_Y				LAPTOP_SCREEN_WEB_UL_Y
#define BOBBY_WOOD_BACKGROUND_WIDTH		125
#define BOBBY_WOOD_BACKGROUND_HEIGHT	100

#define	BOBBY_RAYS_NAME_X							LAPTOP_SCREEN_UL_X + 77
#define	BOBBY_RAYS_NAME_Y							LAPTOP_SCREEN_WEB_UL_Y + 0
#define	BOBBY_RAYS_NAME_WIDTH					344
#define	BOBBY_RAYS_NAME_HEIGHT				66

#define BOBBYS_PLAQUES_X							LAPTOP_SCREEN_UL_X + 39
#define BOBBYS_PLAQUES_Y							LAPTOP_SCREEN_WEB_UL_Y + 174
#define BOBBYS_PLAQUES_WIDTH					414
#define BOBBYS_PLAQUES_HEIGHT					190

#define BOBBIES_TOPHINGE_X						LAPTOP_SCREEN_UL_X
#define BOBBIES_TOPHINGE_Y						LAPTOP_SCREEN_WEB_UL_Y + 42

#define BOBBIES_BOTTOMHINGE_X					LAPTOP_SCREEN_UL_X
#define BOBBIES_BOTTOMHINGE_Y					LAPTOP_SCREEN_WEB_UL_Y + 338

#define BOBBIES_STORE_PLAQUE_X				LAPTOP_SCREEN_UL_X + 148
#define BOBBIES_STORE_PLAQUE_Y				LAPTOP_SCREEN_WEB_UL_Y + 66
#define BOBBIES_STORE_PLAQUE_HEIGHT		93

#define BOBBIES_HANDLE_X							LAPTOP_SCREEN_UL_X + 457
#define BOBBIES_HANDLE_Y							LAPTOP_SCREEN_WEB_UL_Y + 147

#define BOBBIES_FIRST_SENTENCE_X			LAPTOP_SCREEN_UL_X
#define BOBBIES_FIRST_SENTENCE_Y			BOBBIES_STORE_PLAQUE_Y + BOBBIES_STORE_PLAQUE_HEIGHT - 3
#define BOBBIES_FIRST_SENTENCE_WIDTH	500

#define BOBBIES_2ND_SENTENCE_X				LAPTOP_SCREEN_UL_X
#define BOBBIES_2ND_SENTENCE_Y				BOBBIES_FIRST_SENTENCE_Y + 13
#define BOBBIES_2ND_SENTENCE_WIDTH		500

#define BOBBIES_CENTER_SIGN_OFFSET_Y	23

#define BOBBIES_USED_SIGN_X						BOBBYS_PLAQUES_X + 93
#define BOBBIES_USED_SIGN_Y						BOBBYS_PLAQUES_Y + 32
#define BOBBIES_USED_SIGN_WIDTH				92
#define BOBBIES_USED_SIGN_HEIGHT			50
#define BOBBIES_USED_SIGN_TEXT_OFFSET	BOBBIES_USED_SIGN_Y + 10

#define BOBBIES_MISC_SIGN_X						BOBBYS_PLAQUES_X + 238
#define BOBBIES_MISC_SIGN_Y						BOBBYS_PLAQUES_Y + 27
#define BOBBIES_MISC_SIGN_WIDTH				103
#define BOBBIES_MISC_SIGN_HEIGHT			57
#define BOBBIES_MISC_SIGN_TEXT_OFFSET	BOBBIES_MISC_SIGN_Y + BOBBIES_CENTER_SIGN_OFFSET_Y

#define BOBBIES_GUNS_SIGN_X						BOBBYS_PLAQUES_X + 3
#define BOBBIES_GUNS_SIGN_Y						BOBBYS_PLAQUES_Y + 102
#define BOBBIES_GUNS_SIGN_WIDTH				116
#define BOBBIES_GUNS_SIGN_HEIGHT			75
#define BOBBIES_GUNS_SIGN_TEXT_OFFSET BOBBIES_GUNS_SIGN_Y + BOBBIES_CENTER_SIGN_OFFSET_Y

#define BOBBIES_AMMO_SIGN_X						BOBBYS_PLAQUES_X + 150
#define BOBBIES_AMMO_SIGN_Y						BOBBYS_PLAQUES_Y + 105
#define BOBBIES_AMMO_SIGN_WIDTH				112
#define BOBBIES_AMMO_SIGN_HEIGHT			71
#define BOBBIES_AMMO_SIGN_TEXT_OFFSET	BOBBIES_AMMO_SIGN_Y + BOBBIES_CENTER_SIGN_OFFSET_Y

#define BOBBIES_ARMOUR_SIGN_X					BOBBYS_PLAQUES_X + 290
#define BOBBIES_ARMOUR_SIGN_Y					BOBBYS_PLAQUES_Y + 108
#define BOBBIES_ARMOUR_SIGN_WIDTH			114
#define BOBBIES_ARMOUR_SIGN_HEIGHT		70
#define BOBBIES_ARMOUR_SIGN_TEXT_OFFSET BOBBIES_ARMOUR_SIGN_Y + BOBBIES_CENTER_SIGN_OFFSET_Y

#define BOBBIES_3RD_SENTENCE_X				LAPTOP_SCREEN_UL_X
#define BOBBIES_3RD_SENTENCE_Y				BOBBIES_BOTTOMHINGE_Y + 40
#define BOBBIES_3RD_SENTENCE_WIDTH		500

#define BOBBY_R_NEW_PURCHASE_ARRIVAL_TIME		(1 * 60 * 24) // minutes in 1 day

#define	BOBBY_R_USED_PURCHASE_OFFSET		MAXITEMS

#define	BOBBYR_UNDERCONSTRUCTION_ANI_DELAY		150
#define	BOBBYR_UNDERCONSTRUCTION_NUM_FRAMES		5

#define BOBBYR_UNDERCONSTRUCTION_X					iScreenWidthOffset + 111 + ( 613 - 111 - BOBBYR_UNDERCONSTRUCTION_WIDTH) / 2
#define BOBBYR_UNDERCONSTRUCTION_Y					iScreenHeightOffset + 175
#define BOBBYR_UNDERCONSTRUCTION1_Y					iScreenHeightOffset + 378

#define	BOBBYR_UNDERCONSTRUCTION_WIDTH				414
#define	BOBBYR_UNDERCONSTRUCTION_HEIGHT				64

#define	BOBBYR_UNDER_CONSTRUCTION_TEXT_X			LAPTOP_SCREEN_UL_X
#define	BOBBYR_UNDER_CONSTRUCTION_TEXT_Y			BOBBYR_UNDERCONSTRUCTION_Y + 62 + 60
#define	BOBBYR_UNDER_CONSTRUCTION_TEXT_WIDTH		613 - 111



UINT32	guiBobbyName;
UINT32	guiPlaque;
UINT32	guiTopHinge;
UINT32	guiBottomHinge;
UINT32	guiStorePlaque;
UINT32	guiHandle;
UINT32	guiWoodBackground;
UINT32	guiUnderConstructionImage;

/*
UINT16	gusFirstGunIndex;
UINT16	gusLastGunIndex;
UINT8		gubNumGunPages;

UINT16	gusFirstAmmoIndex;
UINT16	gusLastAmmoIndex;
UINT8		gubNumAmmoPages;

UINT16	gusFirstMiscIndex;
UINT16	gusLastMiscIndex;
UINT8		gubNumMiscPages;

UINT16	gusFirstArmourIndex;
UINT16	gusLastArmourIndex;
UINT8		gubNumArmourPages;

UINT16	gusFirstUsedIndex;
UINT16	gusLastUsedIndex;
UINT8		gubNumUsedPages;
*/

UINT32	guiLastBobbyRayPage;



UINT8		gubBobbyRPages[]={
						LAPTOP_MODE_BOBBY_R_USED,
						LAPTOP_MODE_BOBBY_R_MISC,
						LAPTOP_MODE_BOBBY_R_GUNS,
						LAPTOP_MODE_BOBBY_R_AMMO,
						LAPTOP_MODE_BOBBY_R_ARMOR};

//Dealtar's Airport Externalization.						
BOOLEAN gfBobbyRInitialized = 0;
extern CPostalService gPostalService;
extern UINT8 guiNumOfDisplayedCities;
extern vector < PDestinationStruct > gDestinationTable;
extern vector < PShipmentStruct > gShipmentTable;
extern MOUSE_REGION *gSelectedDropDownRegion;
extern MOUSE_REGION *gSelectedScrollAreaDropDownRegion;
extern void BobbyRDeliveryCallback(RefToCShipmentManipulator ShipmentManipulator);
//End Dealtar's Airport Externalization.

//Bobby's Sign menu mouse regions
MOUSE_REGION	gSelectedBobbiesSignMenuRegion[ BOBBIES_NUMBER_SIGNS ];
void SelectBobbiesSignMenuRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

BOOLEAN InitBobbiesMouseRegion(UINT8 ubNumerRegions, UINT16 *usMouseRegionPosArray, MOUSE_REGION *MouseRegion);
BOOLEAN RemoveBobbiesMouseRegion(UINT8 ubNumberRegions, MOUSE_REGION *Mouse_Region);
void HandleBobbyRUnderConstructionAni( BOOLEAN fReset );

void SimulateBobbyRayCustomer(STORE_INVENTORY *pInventoryArray, BOOLEAN fUsed);



void GameInitBobbyR()
{
	//Dealtar's Airport Externalization.
	//Originally, this function was empty!
	gPostalService.RegisterDeliveryCallback(0, BobbyRDeliveryCallback);
}


BOOLEAN EnterBobbyR()
{
	VOBJECT_DESC	VObjectDesc;
	UINT8 i;

	//Dealtar's Airport Externalization.
	// Update the destination and shipment tables upon entering Bobby R
	if (!gDestinationTable.empty())
	{
		gDestinationTable.erase(gDestinationTable.begin(), gDestinationTable.end());
	}

	if (!gShipmentTable.empty())
	{
		gShipmentTable.erase(gShipmentTable.begin(), gShipmentTable.end());
	}

	RefToDestinationListIterator dli = gPostalService.LookupDestinationList().begin();

	while (dli != gPostalService.LookupDestinationList().end())
	{
		gDestinationTable.push_back(&DESTINATION(dli));
		dli++;

	}

	RefToShipmentListIterator sli = gPostalService.LookupShipmentList().begin();

	while (sli != gPostalService.LookupShipmentList().end())
	{
		gShipmentTable.push_back(&SHIPMENT(sli));
		sli++;
	}

	// Dealtar: The following code had to be put here, because GameInitBobbyR() is called
	// before XML data is read into gPostalService due to the screen order in
	// screens.cpp -> GameScreens[]
	if (!gfBobbyRInitialized)
	{
		gfBobbyRInitialized = TRUE;
		gSelectedDropDownRegion = new MOUSE_REGION[gPostalService.LookupDestinationList().size()];
		gSelectedScrollAreaDropDownRegion = new MOUSE_REGION[gPostalService.LookupDestinationList().size()];
		int x = gPostalService.LookupDestinationList().size();
		guiNumOfDisplayedCities = (x < 10) ? x : 10;
		//gDestinationTable.resize(gPostalService.LookupDestinationList().size());
	}
	//End Dealtar's Airport Externalization.
	// an array of mouse regions for the bobbies signs.	Top Left corner, bottom right corner
	UINT16	usMouseRegionPosArray[] = {BOBBIES_USED_SIGN_X, BOBBIES_USED_SIGN_Y, BOBBIES_USED_SIGN_X+BOBBIES_USED_SIGN_WIDTH, BOBBIES_USED_SIGN_Y+BOBBIES_USED_SIGN_HEIGHT,
																	BOBBIES_MISC_SIGN_X, BOBBIES_MISC_SIGN_Y, BOBBIES_MISC_SIGN_X+BOBBIES_MISC_SIGN_WIDTH, BOBBIES_MISC_SIGN_Y+BOBBIES_MISC_SIGN_HEIGHT,
																	BOBBIES_GUNS_SIGN_X, BOBBIES_GUNS_SIGN_Y, BOBBIES_GUNS_SIGN_X+BOBBIES_GUNS_SIGN_WIDTH, BOBBIES_GUNS_SIGN_Y+BOBBIES_GUNS_SIGN_HEIGHT,
																	BOBBIES_AMMO_SIGN_X, BOBBIES_AMMO_SIGN_Y, BOBBIES_AMMO_SIGN_X+BOBBIES_AMMO_SIGN_WIDTH, BOBBIES_AMMO_SIGN_Y+BOBBIES_AMMO_SIGN_HEIGHT,
																	BOBBIES_ARMOUR_SIGN_X, BOBBIES_ARMOUR_SIGN_Y, BOBBIES_ARMOUR_SIGN_X+BOBBIES_ARMOUR_SIGN_WIDTH, BOBBIES_ARMOUR_SIGN_Y+BOBBIES_ARMOUR_SIGN_HEIGHT};

	InitBobbyRWoodBackground();

	// load the Bobbyname graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_BOBBYNAME );
	CHECKF(AddVideoObject(&VObjectDesc, &guiBobbyName));

	// load the plaque graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BobbyPlaques.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiPlaque));

	// load the TopHinge graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BobbyTopHinge.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiTopHinge));

	// load the BottomHinge graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BobbyBottomHinge.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBottomHinge));

	// load the Store Plaque graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_STOREPLAQUE );
	CHECKF(AddVideoObject(&VObjectDesc, &guiStorePlaque));

	// load the Handle graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BobbyHandle.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiHandle));


	InitBobbiesMouseRegion(BOBBIES_NUMBER_SIGNS, usMouseRegionPosArray, gSelectedBobbiesSignMenuRegion);


	if( !LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
	{
		// load the Handle graphic and add it
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("LAPTOP\\UnderConstruction.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiUnderConstructionImage));

		for(i=0; i<BOBBIES_NUMBER_SIGNS; i++)
		{
			MSYS_DisableRegion( &gSelectedBobbiesSignMenuRegion[i] );
		}

		LaptopSaveInfo.ubHaveBeenToBobbyRaysAtLeastOnceWhileUnderConstruction = BOBBYR_BEEN_TO_SITE_ONCE;
	}


	SetBookMark(BOBBYR_BOOKMARK);
	HandleBobbyRUnderConstructionAni( TRUE );

	RenderBobbyR();

	return( TRUE );
}

void ExitBobbyR()
{

	DeleteVideoObjectFromIndex(guiBobbyName);
	DeleteVideoObjectFromIndex(guiPlaque);
	DeleteVideoObjectFromIndex(guiTopHinge);
	DeleteVideoObjectFromIndex(guiBottomHinge);
	DeleteVideoObjectFromIndex(guiStorePlaque);
	DeleteVideoObjectFromIndex(guiHandle);

	if( !LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
	{
		DeleteVideoObjectFromIndex(guiUnderConstructionImage);
	}


	DeleteBobbyRWoodBackground();

	RemoveBobbiesMouseRegion(BOBBIES_NUMBER_SIGNS, gSelectedBobbiesSignMenuRegion);

	guiLastBobbyRayPage = LAPTOP_MODE_BOBBY_R;
}

void HandleBobbyR()
{
	HandleBobbyRUnderConstructionAni( FALSE );
}

void RenderBobbyR()
{
	HVOBJECT hPixHandle;
	HVOBJECT hStorePlaqueHandle;

	DrawBobbyRWoodBackground();

	// Bobby's Name
	GetVideoObject(&hPixHandle, guiBobbyName);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,BOBBY_RAYS_NAME_X, BOBBY_RAYS_NAME_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Plaque
	GetVideoObject(&hPixHandle, guiPlaque);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,BOBBYS_PLAQUES_X, BOBBYS_PLAQUES_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Top Hinge
	GetVideoObject(&hPixHandle, guiTopHinge);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,BOBBIES_TOPHINGE_X, BOBBIES_TOPHINGE_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Bottom Hinge
	GetVideoObject(&hPixHandle, guiBottomHinge);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,BOBBIES_BOTTOMHINGE_X, BOBBIES_BOTTOMHINGE_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// StorePlaque
	GetVideoObject(&hStorePlaqueHandle, guiStorePlaque);
	BltVideoObject(FRAME_BUFFER, hStorePlaqueHandle, 0,BOBBIES_STORE_PLAQUE_X, BOBBIES_STORE_PLAQUE_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Handle
	GetVideoObject(&hPixHandle, guiHandle);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,BOBBIES_HANDLE_X, BOBBIES_HANDLE_Y, VO_BLT_SRCTRANSPARENCY,NULL);

/*
	if( !LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
	{
		// The undercontsruction graphic
		GetVideoObject(&hPixHandle, guiUnderConstructionImage );
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,BOBBIES_FIRST_SENTENCE_X, BOBBIES_FIRST_SENTENCE_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,BOBBIES_3RD_SENTENCE_X, BOBBIES_3RD_SENTENCE_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	}
*/

	SetFontShadow(BOBBIES_SENTENCE_BACKGROUNDCOLOR);


	if( LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
	{
		//Bobbys first sentence
	//	ShadowText( FRAME_BUFFER, BobbyRaysFrontText[BOBBYR_ADVERTISMENT_1], BOBBIES_SENTENCE_FONT, BOBBIES_FIRST_SENTENCE_X, BOBBIES_FIRST_SENTENCE_Y );
		DrawTextToScreen(BobbyRaysFrontText[BOBBYR_ADVERTISMENT_1], BOBBIES_FIRST_SENTENCE_X, BOBBIES_FIRST_SENTENCE_Y, BOBBIES_FIRST_SENTENCE_WIDTH, BOBBIES_SENTENCE_FONT, BOBBIES_SENTENCE_COLOR, BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED | TEXT_SHADOWED );

		//Bobbys second sentence
		DrawTextToScreen(BobbyRaysFrontText[BOBBYR_ADVERTISMENT_2], BOBBIES_2ND_SENTENCE_X, BOBBIES_2ND_SENTENCE_Y, BOBBIES_2ND_SENTENCE_WIDTH, BOBBIES_SENTENCE_FONT, BOBBIES_SENTENCE_COLOR, BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED | TEXT_SHADOWED );
		SetFontShadow(DEFAULT_SHADOW);
	}


	SetFontShadow(BOBBIES_SIGN_BACKGROUNDCOLOR);
	//Text on the Used Sign
	DisplayWrappedString(BOBBIES_USED_SIGN_X, BOBBIES_USED_SIGN_TEXT_OFFSET, BOBBIES_USED_SIGN_WIDTH-5, 2, BOBBIES_SIGN_FONT, BOBBIES_SIGN_COLOR, BobbyRaysFrontText[BOBBYR_USED], BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED);
	//Text on the Misc Sign
	DisplayWrappedString(BOBBIES_MISC_SIGN_X, BOBBIES_MISC_SIGN_TEXT_OFFSET, BOBBIES_MISC_SIGN_WIDTH, 2, BOBBIES_SIGN_FONT, BOBBIES_SIGN_COLOR, BobbyRaysFrontText[BOBBYR_MISC], BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED);
	//Text on the Guns Sign
	DisplayWrappedString(BOBBIES_GUNS_SIGN_X, BOBBIES_GUNS_SIGN_TEXT_OFFSET, BOBBIES_GUNS_SIGN_WIDTH, 2, BOBBIES_SIGN_FONT, BOBBIES_SIGN_COLOR, BobbyRaysFrontText[BOBBYR_GUNS], BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED);
	//Text on the Ammo Sign
	DisplayWrappedString(BOBBIES_AMMO_SIGN_X, BOBBIES_AMMO_SIGN_TEXT_OFFSET, BOBBIES_AMMO_SIGN_WIDTH, 2, BOBBIES_SIGN_FONT, BOBBIES_SIGN_COLOR, BobbyRaysFrontText[BOBBYR_AMMO], BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED);
	//Text on the Armour Sign
	DisplayWrappedString(BOBBIES_ARMOUR_SIGN_X, BOBBIES_ARMOUR_SIGN_TEXT_OFFSET, BOBBIES_ARMOUR_SIGN_WIDTH, 2, BOBBIES_SIGN_FONT, BOBBIES_SIGN_COLOR, BobbyRaysFrontText[BOBBYR_ARMOR], BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED);
	SetFontShadow(DEFAULT_SHADOW);


	if( LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
	{
		//Bobbys Third sentence
		SetFontShadow(BOBBIES_SENTENCE_BACKGROUNDCOLOR);
		DrawTextToScreen(BobbyRaysFrontText[BOBBYR_ADVERTISMENT_3], BOBBIES_3RD_SENTENCE_X, BOBBIES_3RD_SENTENCE_Y, BOBBIES_3RD_SENTENCE_WIDTH, BOBBIES_SENTENCE_FONT, BOBBIES_SENTENCE_COLOR, BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED | TEXT_SHADOWED );
		SetFontShadow(DEFAULT_SHADOW);
	}

	//if we cant go to any sub pages, darken the page out
	if( !LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y );
	}

	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}




BOOLEAN InitBobbyRWoodBackground()
{
	VOBJECT_DESC	VObjectDesc;

	// load the Wood bacground graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BobbyWood.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiWoodBackground));

	return(TRUE);
}

BOOLEAN DeleteBobbyRWoodBackground()
{
	DeleteVideoObjectFromIndex(guiWoodBackground);
	return(TRUE);
}


BOOLEAN DrawBobbyRWoodBackground()
{
	HVOBJECT hWoodBackGroundHandle;
	UINT16	x,y, uiPosX, uiPosY;

	// Blt the Wood background
	GetVideoObject(&hWoodBackGroundHandle, guiWoodBackground);

	uiPosY = BOBBY_WOOD_BACKGROUND_Y;
	for(y=0; y<4; y++)
	{
		uiPosX = BOBBY_WOOD_BACKGROUND_X;
		for(x=0; x<4; x++)
		{
		BltVideoObject(FRAME_BUFFER, hWoodBackGroundHandle, 0,uiPosX, uiPosY, VO_BLT_SRCTRANSPARENCY,NULL);
			uiPosX += BOBBY_WOOD_BACKGROUND_WIDTH;
		}
		uiPosY += BOBBY_WOOD_BACKGROUND_HEIGHT;
	}

	return(TRUE);
}


BOOLEAN InitBobbiesMouseRegion(UINT8 ubNumerRegions, UINT16 *usMouseRegionPosArray, MOUSE_REGION *MouseRegion)
{
	UINT8 i,ubCount=0;

	for(i=0; i<ubNumerRegions; i++)
	{
		//Mouse region for the toc buttons
		MSYS_DefineRegion( &MouseRegion[i], usMouseRegionPosArray[ubCount], usMouseRegionPosArray[ubCount+1], usMouseRegionPosArray[ubCount+2], usMouseRegionPosArray[ubCount+3], MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectBobbiesSignMenuRegionCallBack);
		MSYS_AddRegion(&MouseRegion[i]);
		MSYS_SetRegionUserData( &MouseRegion[i], 0, gubBobbyRPages[i]);

		ubCount +=4;
	}


	return(TRUE);
}


BOOLEAN RemoveBobbiesMouseRegion(UINT8 ubNumberRegions, MOUSE_REGION *Mouse_Region)
{
	UINT8 i;

	for(i=0; i<ubNumberRegions; i++)
		MSYS_RemoveRegion( &Mouse_Region[i]);

	return(TRUE);
}




void SelectBobbiesSignMenuRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT8	ubNewPage = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );
		guiCurrentLaptopMode = ubNewPage;
//		FindLastItemIndex(ubNewPage);

	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}


/*
BOOLEAN WebPageTileBackground(UINT8 ubNumX, UINT8 ubNumY, UINT16 usWidth, UINT16 usHeight, UINT32 uiBackground)
{
	HVOBJECT hBackGroundHandle;
	UINT16	x,y, uiPosX, uiPosY;

	// Blt the Wood background
	GetVideoObject(&hBackGroundHandle, uiBackground);

	uiPosY = LAPTOP_SCREEN_WEB_UL_Y;
	for(y=0; y<ubNumY; y++)
	{
		uiPosX = LAPTOP_SCREEN_UL_X;
		for(x=0; x<ubNumX; x++)
		{
		BltVideoObject(FRAME_BUFFER, hBackGroundHandle, 0,uiPosX, uiPosY, VO_BLT_SRCTRANSPARENCY,NULL);
			uiPosX += usWidth;
		}
		uiPosY += usHeight;
	}
	return(TRUE);
}
*/


void HandleBobbyRUnderConstructionAni( BOOLEAN fReset )
{
	HVOBJECT hPixHandle;
	static UINT32	uiLastTime=1;
	static UINT16	usCount=0;
	UINT32	uiCurTime=GetJA2Clock();


	if( LaptopSaveInfo.fBobbyRSiteCanBeAccessed )
		return;

	if( fReset )
		usCount =1;

	if( fShowBookmarkInfo )
	{
		fReDrawBookMarkInfo = TRUE;
	}

	if( ( ( uiCurTime - uiLastTime ) > BOBBYR_UNDERCONSTRUCTION_ANI_DELAY )||( fReDrawScreenFlag ) )
	{
		// The undercontsruction graphic
		GetVideoObject(&hPixHandle, guiUnderConstructionImage );
		BltVideoObject(FRAME_BUFFER, hPixHandle, usCount, BOBBYR_UNDERCONSTRUCTION_X, BOBBYR_UNDERCONSTRUCTION_Y, VO_BLT_SRCTRANSPARENCY,NULL);

		BltVideoObject( FRAME_BUFFER, hPixHandle, usCount, BOBBYR_UNDERCONSTRUCTION_X, BOBBYR_UNDERCONSTRUCTION1_Y, VO_BLT_SRCTRANSPARENCY,NULL );

		DrawTextToScreen( BobbyRaysFrontText[ BOBBYR_UNDER_CONSTRUCTION ], BOBBYR_UNDER_CONSTRUCTION_TEXT_X, BOBBYR_UNDER_CONSTRUCTION_TEXT_Y, BOBBYR_UNDER_CONSTRUCTION_TEXT_WIDTH, FONT16ARIAL, BOBBIES_SENTENCE_COLOR, BOBBIES_SIGN_BACKCOLOR, FALSE, CENTER_JUSTIFIED | INVALIDATE_TEXT );

	InvalidateRegion( BOBBYR_UNDERCONSTRUCTION_X, BOBBYR_UNDERCONSTRUCTION_Y, BOBBYR_UNDERCONSTRUCTION_X+BOBBYR_UNDERCONSTRUCTION_WIDTH, BOBBYR_UNDERCONSTRUCTION_Y+BOBBYR_UNDERCONSTRUCTION_HEIGHT );
	InvalidateRegion( BOBBYR_UNDERCONSTRUCTION_X, BOBBYR_UNDERCONSTRUCTION1_Y, BOBBYR_UNDERCONSTRUCTION_X+BOBBYR_UNDERCONSTRUCTION_WIDTH, BOBBYR_UNDERCONSTRUCTION1_Y+BOBBYR_UNDERCONSTRUCTION_HEIGHT );

		uiLastTime = GetJA2Clock();

		usCount++;

		if( usCount >= BOBBYR_UNDERCONSTRUCTION_NUM_FRAMES )
			usCount = 0;
	}
}



void InitBobbyRayInventory()
{
	//Initializes which NEW items can be bought at Bobby Rays
	InitBobbyRayNewInventory();

	//Initializes the starting values for Bobby Rays NEW Inventory
	SetupStoreInventory( LaptopSaveInfo.BobbyRayInventory, FALSE );

	//Initializes which USED items can be bought at Bobby Rays
	InitBobbyRayUsedInventory();

	//Initializes the starting values for Bobby Rays USED Inventory
	SetupStoreInventory( LaptopSaveInfo.BobbyRayUsedInventory, TRUE);
}


BOOLEAN InitBobbyRayNewInventory()
{
	UINT16	usBobbyrIndex = 0;

	memset( LaptopSaveInfo.BobbyRayInventory, 0, sizeof(STORE_INVENTORY) * MAXITEMS);

	// add all the NEW items he can ever sell into his possible inventory list, for now in order by item #
	for( UINT16 i = 0; i < MAXITEMS; ++i )
	{
		//if Bobby Ray sells this, it can be sold, and it's allowed into this game (some depend on e.g. gun-nut option)
//		if( ( StoreInventory[ i ][ BOBBY_RAY_NEW ] != 0) && !( Item[ i ].fFlags & ITEM_NOT_BUYABLE ) && ItemIsLegal( i ) )
		LaptopSaveInfo.BobbyRayInventory[ usBobbyrIndex ].usItemIndex = i;
		++usBobbyrIndex;
	}

	if ( usBobbyrIndex > 1 )
	{
		// sort this list by object category, and by ascending price within each category
		qsort( LaptopSaveInfo.BobbyRayInventory, usBobbyrIndex, sizeof( STORE_INVENTORY ), BobbyRayItemQsortCompare );
	}

	// remember how many entries in the list are valid
	LaptopSaveInfo.usInventoryListLength[ BOBBY_RAY_NEW ] = usBobbyrIndex;
	// also mark the end of the list of valid item entries
	LaptopSaveInfo.BobbyRayInventory[ usBobbyrIndex ].usItemIndex = BOBBYR_NO_ITEMS;

	return(TRUE);
}


BOOLEAN InitBobbyRayUsedInventory()
{
	UINT16	usBobbyrIndex = 0;


	memset( LaptopSaveInfo.BobbyRayUsedInventory, 0, sizeof(STORE_INVENTORY) * MAXITEMS);

	// add all the NEW items he can ever sell into his possible inventory list, for now in order by item #
	for( UINT16 i = 0; i < MAXITEMS; ++i )
	{
		//if Bobby Ray sells this, it can be sold, and it's allowed into this game (some depend on e.g. gun-nut option)
//		if( ( StoreInventory[ i ][ BOBBY_RAY_USED ] != 0) && !( Item[ i ].fFlags & ITEM_NOT_BUYABLE ) && ItemIsLegal( i ) )
		// in case his store inventory list is wrong, make sure this category of item can be sold used
		if ( CanDealerItemBeSoldUsed( i ) )
		{
			LaptopSaveInfo.BobbyRayUsedInventory[ usBobbyrIndex ].usItemIndex = i;
			++usBobbyrIndex;
		}
	}

	if ( usBobbyrIndex > 1 )
	{
		// sort this list by object category, and by ascending price within each category
		qsort( LaptopSaveInfo.BobbyRayUsedInventory, usBobbyrIndex, sizeof( STORE_INVENTORY ), BobbyRayItemQsortCompare );
	}

	// remember how many entries in the list are valid
	LaptopSaveInfo.usInventoryListLength[BOBBY_RAY_USED] = usBobbyrIndex;
	// also mark the end of the list of valid item entries
	LaptopSaveInfo.BobbyRayUsedInventory[ usBobbyrIndex ].usItemIndex = BOBBYR_NO_ITEMS;

	return(TRUE);
}



void DailyUpdateOfBobbyRaysNewInventory()
{
	INT32 i;
	UINT16 usItemIndex;
	BOOLEAN fPrevElig;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("DailyUpdateOfBobbyRaysNewInventory: list length = %d", LaptopSaveInfo.usInventoryListLength[BOBBY_RAY_NEW]));

	//simulate other buyers by reducing the current quantity on hand
	SimulateBobbyRayCustomer(LaptopSaveInfo.BobbyRayInventory, BOBBY_RAY_NEW);

	//loop through all items BR can stock to see what needs reordering
	for(i = 0; i < LaptopSaveInfo.usInventoryListLength[BOBBY_RAY_NEW]; ++i)
	{
		// the index is NOT the item #, get that from the table
		usItemIndex = LaptopSaveInfo.BobbyRayInventory[ i ].usItemIndex;

		Assert(usItemIndex < MAXITEMS);

		DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("DailyUpdateOfBobbyRaysNewInventory: checking item = %d, qty on order = %d",usItemIndex,LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnOrder));
		// make sure this item is still sellable in the latest version of the store inventory
		if ( StoreInventory[ usItemIndex ][ BOBBY_RAY_NEW ] == 0 )
		{
			DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("DailyUpdateOfBobbyRaysNewInventory: skipping item = %d",usItemIndex));
			continue;
		}

		//if the item isn't already on order
		if( LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnOrder == 0 || gGameOptions.ubBobbyRayQuality == BR_AWESOME )
		{
			DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("DailyUpdateOfBobbyRaysNewInventory: item = %d, qty on hand = %d, half desired amount = %d",usItemIndex,LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnHand,(StoreInventory[ usItemIndex ][ BOBBY_RAY_NEW ] * gGameOptions.ubBobbyRayQuantity )/2));
			//if the qty on hand is half the desired amount or fewer
			if( LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnHand <= (StoreInventory[ usItemIndex ][ BOBBY_RAY_NEW ] * gGameOptions.ubBobbyRayQuantity )/2 )
			{
				// remember value of the "previously eligible" flag
				fPrevElig = LaptopSaveInfo.BobbyRayInventory[ i ].fPreviouslyEligible;

				//determine if any can/should be ordered, and how many
				LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnOrder = HowManyBRItemsToOrder( usItemIndex, LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnHand, BOBBY_RAY_NEW);

				//if he found some to buy
				if( LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnOrder > 0 )
				{
					// if this is the first day the player is eligible to have access to this thing
					if ( !fPrevElig || gGameOptions.ubBobbyRayQuality == BR_AWESOME )
					{
						DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("DailyUpdateOfBobbyRaysNewInventory: item = %d, add fresh inventory",usItemIndex));
						// eliminate the ordering delay and stock the items instantly!
						// This is just a way to reward the player right away for making progress without the reordering lag...
						AddFreshBobbyRayInventory( usItemIndex );
					}
					else
					{
						OrderBobbyRItem(usItemIndex);

#ifdef BR_INVENTORY_TURNOVER_DEBUG
						if ( usItemIndex == ROCKET_LAUNCHER )
							MapScreenMessage( 0, MSG_DEBUG, L"%s: BR Ordered %d, Has %d", WORLDTIMESTR, LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnOrder, LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnHand );
#endif
					}
				}
			}
		}
	}
}


void DailyUpdateOfBobbyRaysUsedInventory()
{
	INT16 i;
	UINT16 usItemIndex;
	BOOLEAN fPrevElig;

	//simulate other buyers by reducing the current quantity on hand
	SimulateBobbyRayCustomer(LaptopSaveInfo.BobbyRayUsedInventory, BOBBY_RAY_USED);

	for(i = 0; i < LaptopSaveInfo.usInventoryListLength[BOBBY_RAY_USED]; i++)
	{
		//if the used item isn't already on order
		if( LaptopSaveInfo.BobbyRayUsedInventory[ i ].ubQtyOnOrder == 0 || gGameOptions.ubBobbyRayQuality == BR_AWESOME )
		{
			//if we don't have ANY
			if( LaptopSaveInfo.BobbyRayUsedInventory[ i ].ubQtyOnHand == 0 )
			{
				// the index is NOT the item #, get that from the table
				usItemIndex = LaptopSaveInfo.BobbyRayUsedInventory[ i ].usItemIndex;
				Assert(usItemIndex < MAXITEMS);

				// make sure this item is still sellable in the latest version of the store inventory
				if ( StoreInventory[ usItemIndex ][ BOBBY_RAY_USED ] == 0 )
				{
					continue;
				}

				// remember value of the "previously eligible" flag
				fPrevElig = LaptopSaveInfo.BobbyRayUsedInventory[ i ].fPreviouslyEligible;

				//determine if any can/should be ordered, and how many
				LaptopSaveInfo.BobbyRayUsedInventory[ i ].ubQtyOnOrder = HowManyBRItemsToOrder(usItemIndex, LaptopSaveInfo.BobbyRayUsedInventory[ i ].ubQtyOnHand, BOBBY_RAY_USED);

				//if he found some to buy
				if( LaptopSaveInfo.BobbyRayUsedInventory[ i ].ubQtyOnOrder > 0 )
				{
					// if this is the first day the player is eligible to have access to this thing
					if ( !fPrevElig || gGameOptions.ubBobbyRayQuality == BR_AWESOME )
					{
						// eliminate the ordering delay and stock the items instantly!
						// This is just a way to reward the player right away for making progress without the reordering lag...
						AddFreshBobbyRayInventory( usItemIndex );
					}
					else
					{
						OrderBobbyRItem((INT16) (usItemIndex + BOBBY_R_USED_PURCHASE_OFFSET));
					}
				}
			}
		}
	}
}


//returns the number of items to order
UINT8 HowManyBRItemsToOrder(UINT16 usItemIndex, UINT8 ubCurrentlyOnHand, UINT8 ubBobbyRayNewUsed )
{
	UINT8	ubItemsOrdered = 0;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("HowManyBRItemsToOrder: item = %d",usItemIndex));

	Assert(usItemIndex < MAXITEMS);
	// formulas below will fail if there are more items already in stock than optimal
	//Assert(ubCurrentlyOnHand <= StoreInventory[ usItemIndex ][ ubBobbyRayNewUsed ] * gGameOptions.ubBobbyRayQuantity) ;
	Assert(ubBobbyRayNewUsed < BOBBY_RAY_LISTS);


	// decide if he can get stock for this item (items are reordered an entire batch at a time)
	if (ItemTransactionOccurs( -1, usItemIndex, DEALER_BUYING, ubBobbyRayNewUsed ))
	{
		if (ubBobbyRayNewUsed == BOBBY_RAY_NEW)
		{
			ubItemsOrdered = HowManyItemsToReorder(StoreInventory[ usItemIndex ][ ubBobbyRayNewUsed ] * gGameOptions.ubBobbyRayQuantity, ubCurrentlyOnHand);
		}
		else
		{
			//Since these are used items we only should order 1 of each type
			ubItemsOrdered = 1;
		}
	}
	else
	{
		// can't obtain this item from suppliers
		ubItemsOrdered = 0;
	}


	return(ubItemsOrdered);
}


void OrderBobbyRItem(UINT16 usItemIndex)
{
	UINT32 uiArrivalTime;

	//add the new item to the queue.	The new item will arrive in 'uiArrivalTime' minutes.
	uiArrivalTime = BOBBY_R_NEW_PURCHASE_ARRIVAL_TIME + Random( BOBBY_R_NEW_PURCHASE_ARRIVAL_TIME / 2 );
	uiArrivalTime += GetWorldTotalMin();
	AddStrategicEvent( EVENT_UPDATE_BOBBY_RAY_INVENTORY, uiArrivalTime, usItemIndex);
}


void AddFreshBobbyRayInventory( UINT16 usItemIndex )
{
	INT16 sInventorySlot;
	STORE_INVENTORY *pInventoryArray;
	BOOLEAN fUsed;
	UINT8 ubItemQuality;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("AddFreshBobbyRayInventory: item = %d", usItemIndex ));

	if( usItemIndex >= BOBBY_R_USED_PURCHASE_OFFSET )
	{
		usItemIndex -= BOBBY_R_USED_PURCHASE_OFFSET;
		pInventoryArray = LaptopSaveInfo.BobbyRayUsedInventory;
		fUsed = BOBBY_RAY_USED;
		ubItemQuality = 20 + (UINT8) Random( 60 );
	}
	else
	{
		pInventoryArray = LaptopSaveInfo.BobbyRayInventory;
		fUsed = BOBBY_RAY_NEW;
		ubItemQuality = 100;
	}


	// find out which inventory slot that item is stored in
	sInventorySlot = GetInventorySlotForItem(pInventoryArray, usItemIndex, fUsed);
	if (sInventorySlot == -1)
	{
		DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("AddFreshBobbyRayInventory: item not found! = %d", usItemIndex ));
		AssertMsg( FALSE, String( "AddFreshBobbyRayInventory(), Item %d not found.	AM-0.", usItemIndex ) );
		return;
	}

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3,String("AddFreshBobbyRayInventory: item = %d, qty on hand = %d, qty on order = %d", usItemIndex,pInventoryArray[ sInventorySlot ].ubQtyOnHand, pInventoryArray[ sInventorySlot ].ubQtyOnOrder ));

	pInventoryArray[ sInventorySlot ].ubQtyOnHand += pInventoryArray[ sInventorySlot ].ubQtyOnOrder;
	pInventoryArray[ sInventorySlot ].ubItemQuality = ubItemQuality;

#ifdef BR_INVENTORY_TURNOVER_DEBUG
	if ( usItemIndex == ROCKET_LAUNCHER && !fUsed )
		MapScreenMessage( 0, MSG_DEBUG, L"%s: BR Bought %d, Has %d", WORLDTIMESTR, pInventoryArray[ sInventorySlot ].ubQtyOnOrder, pInventoryArray[ sInventorySlot ].ubQtyOnHand );
#endif

	// cancel order
	pInventoryArray[ sInventorySlot ].ubQtyOnOrder = 0;
}


INT16 GetInventorySlotForItem(STORE_INVENTORY *pInventoryArray, UINT16 usItemIndex, BOOLEAN fUsed)
{
	INT16 i;

	for(i = 0; i < LaptopSaveInfo.usInventoryListLength[fUsed]; i++)
	{
		//if we have some of this item in stock
		if( pInventoryArray[ i ].usItemIndex == usItemIndex)
		{
			return(i);
		}
	}

	// not found!
	return(-1);
}


void SimulateBobbyRayCustomer(STORE_INVENTORY *pInventoryArray, BOOLEAN fUsed)
{
	INT16 i;
	UINT8 ubItemsSold;

	//loop through all items BR can stock to see what gets sold
	for(i = 0; i < LaptopSaveInfo.usInventoryListLength[fUsed]; i++)
	{
		//if we have some of this item in stock
		if( pInventoryArray[ i ].ubQtyOnHand > 0)
		{
			ubItemsSold = HowManyItemsAreSold( -1, pInventoryArray[ i ].usItemIndex, pInventoryArray[ i ].ubQtyOnHand, fUsed);
			pInventoryArray[ i ].ubQtyOnHand -= ubItemsSold;

#ifdef BR_INVENTORY_TURNOVER_DEBUG
			if (ubItemsSold > 0 )
			{
				if ( i == ROCKET_LAUNCHER && !fUsed )
					MapScreenMessage( 0, MSG_DEBUG, L"%s: BR Sold %d, Has %d", WORLDTIMESTR, ubItemsSold, pInventoryArray[ i ].ubQtyOnHand );
			}
#endif
		}
	}
}


void CancelAllPendingBRPurchaseOrders(void)
{
	// remove all the BR-Order events off the event queue
	DeleteAllStrategicEventsOfType( EVENT_UPDATE_BOBBY_RAY_INVENTORY );

	// zero out all the quantities on order
	for( UINT16 i = 0; i < MAXITEMS; ++i)
	{
		LaptopSaveInfo.BobbyRayInventory[ i ].ubQtyOnOrder = 0;
		LaptopSaveInfo.BobbyRayUsedInventory[ i ].ubQtyOnOrder = 0;
	}

	// do an extra daily update immediately to create new reorders ASAP
	DailyUpdateOfBobbyRaysNewInventory();
	DailyUpdateOfBobbyRaysUsedInventory();
}
