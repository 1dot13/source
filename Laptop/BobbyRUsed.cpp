#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "BobbyRUsed.h"
	#include "BobbyR.h"
	#include "BobbyRGuns.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "WordWrap.h"
	#include "Text.h"
#endif

UINT32		guiUsedBackground;
UINT32		guiUsedGrid;


void GameInitBobbyRUsed()
{

}

BOOLEAN EnterBobbyRUsed()
{
	VOBJECT_DESC	VObjectDesc;

	//gfBigImageMouseRegionCreated = FALSE;

	// load the background graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\usedbackground.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiUsedBackground));

	// load the gunsgrid graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\usedgrid.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiUsedGrid));

	InitBobbyBrTitle();

	guiPrevUsedFilterMode = -1;
	guiCurrentUsedFilterMode = -1;

	SetFirstLastPagesForUsed(guiCurrentUsedFilterMode);

	//Draw menu bar
	InitBobbyMenuBar( );
	InitBobbyRUsedFilterBar();

	RenderBobbyRUsed( );

	return(TRUE);
}

void ExitBobbyRUsed()
{
	DeleteVideoObjectFromIndex(guiUsedBackground);
	DeleteVideoObjectFromIndex(guiUsedGrid);
	DeleteBobbyMenuBar();

	DeleteBobbyRUsedFilter();

	DeleteBobbyBrTitle();
	DeleteMouseRegionForBigImage();

	giCurrentSubPage = gusCurWeaponIndex;
	guiLastBobbyRayPage = LAPTOP_MODE_BOBBY_R_USED;
}

void HandleBobbyRUsed()
{
	HandleBobbyRGuns();
}

void RenderBobbyRUsed()
{
	HVOBJECT hPixHandle;

	WebPageTileBackground(BOBBYR_NUM_HORIZONTAL_TILES, BOBBYR_NUM_VERTICAL_TILES, BOBBYR_BACKGROUND_WIDTH, BOBBYR_BACKGROUND_HEIGHT, guiUsedBackground);

	//Display title at top of page
	//DisplayBobbyRBrTitle();

	// GunForm
	GetVideoObject(&hPixHandle, guiUsedGrid);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, BOBBYR_GRIDLOC_X, BOBBYR_GRIDLOC_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	DisplayItemInfo(BOBBYR_USED_ITEMS, guiCurrentUsedFilterMode);

	UpdateButtonText(guiCurrentLaptopMode);
	UpdateUsedFilterButtons();

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
	//Moa removed below. See comment above LAPTOP_SCREEN_UL_X in laptop.h
	//	fReDrawScreenFlag = TRUE;
	//fPausedReDrawScreenFlag = TRUE;
}










