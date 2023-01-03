#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "BobbyRArmour.h"
	#include "BobbyRGuns.h"
	#include "BobbyR.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "WordWrap.h"
	#include "Text.h"
#endif


UINT32		guiArmourBackground;
UINT32		guiArmourGrid;



void GameInitBobbyRArmour()
{

}

BOOLEAN EnterBobbyRArmour()
{
	VOBJECT_DESC	VObjectDesc;

	// load the background graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\Armourbackground.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiArmourBackground));

	// load the gunsgrid graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\Armourgrid.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiArmourGrid));

	InitBobbyBrTitle();

	guiPrevArmourFilterMode = -1;
	guiCurrentArmourFilterMode = -1;

	SetFirstLastPagesForNew( IC_ARMOUR, guiCurrentArmourFilterMode );

	//Draw menu bar
	InitBobbyMenuBar( );

	InitBobbyRArmourFilterBar();

	RenderBobbyRArmour( );

	return(TRUE);
}

void ExitBobbyRArmour()
{
	DeleteVideoObjectFromIndex(guiArmourBackground);
	DeleteVideoObjectFromIndex(guiArmourGrid);
	DeleteBobbyMenuBar();

	DeleteBobbyRArmourFilter();

	DeleteBobbyBrTitle();
	DeleteMouseRegionForBigImage();

	giCurrentSubPage = gusCurWeaponIndex;
	guiLastBobbyRayPage = LAPTOP_MODE_BOBBY_R_ARMOR;
}

void HandleBobbyRArmour()
{
	HandleBobbyRGuns();
}

void RenderBobbyRArmour()
{

	HVOBJECT hPixHandle;

	WebPageTileBackground(BOBBYR_NUM_HORIZONTAL_TILES, BOBBYR_NUM_VERTICAL_TILES, BOBBYR_BACKGROUND_WIDTH, BOBBYR_BACKGROUND_HEIGHT, guiArmourBackground);

	//Display title at top of page
	//DisplayBobbyRBrTitle();

	// GunForm
	GetVideoObject(&hPixHandle, guiArmourGrid);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, BOBBYR_GRIDLOC_X, BOBBYR_GRIDLOC_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	DisplayItemInfo(IC_ARMOUR, guiCurrentArmourFilterMode);

	UpdateButtonText(guiCurrentLaptopMode);
	// TODO

	UpdateArmourFilterButtons();

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
	//Moa removed below. See comment above LAPTOP_SCREEN_UL_X in laptop.h
	//	fReDrawScreenFlag = TRUE;
	//fPausedReDrawScreenFlag = TRUE;
}











