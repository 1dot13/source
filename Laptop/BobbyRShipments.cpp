#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "BobbyRShipments.h"
#else
	#include "laptop.h"
	#include "BobbyRShipments.h"
	#include "bobbyr.h"
	#include "utilities.h"
	#include "WCheck.h"
	#include "Text.h"
	#include "BobbyRGuns.h"
	#include "cursors.h"
	#include "BobbyRMailOrder.h"
	#include "wordwrap.h"
	#include "strategic.h"
	#include "strategicmap.h"
	#include "PostalService.h"
#endif



#define		BOBBYR_SHIPMENT_TITLE_TEXT_FONT			FONT14ARIAL
#define		BOBBYR_SHIPMENT_TITLE_TEXT_COLOR			157

#define		BOBBYR_SHIPMENT_STATIC_TEXT_FONT			FONT12ARIAL
#define		BOBBYR_SHIPMENT_STATIC_TEXT_COLOR			145


#define		BOBBYR_BOBBY_RAY_TITLE_X					LAPTOP_SCREEN_UL_X + 171
#define		BOBBYR_BOBBY_RAY_TITLE_Y					LAPTOP_SCREEN_WEB_UL_Y + 3

#define		BOBBYR_ORDER_FORM_TITLE_X					BOBBYR_BOBBY_RAY_TITLE_X
#define		BOBBYR_ORDER_FORM_TITLE_Y					BOBBYR_BOBBY_RAY_TITLE_Y + 37
#define		BOBBYR_ORDER_FORM_TITLE_WIDTH			159

#define		BOBBYR_SHIPMENT_DELIVERY_GRID_X						LAPTOP_SCREEN_UL_X + 2
#define		BOBBYR_SHIPMENT_DELIVERY_GRID_Y						BOBBYR_SHIPMENT_ORDER_GRID_Y
#define		BOBBYR_SHIPMENT_DELIVERY_GRID_WIDTH				183

#define		BOBBYR_SHIPMENT_ORDER_GRID_X							LAPTOP_SCREEN_UL_X + 223
#define		BOBBYR_SHIPMENT_ORDER_GRID_Y							LAPTOP_SCREEN_WEB_UL_Y + 62


#define		BOBBYR_SHIPMENT_BACK_BUTTON_X							iScreenWidthOffset + 130
#define		BOBBYR_SHIPMENT_BACK_BUTTON_Y							iScreenHeightOffset + 400 + LAPTOP_SCREEN_WEB_DELTA_Y + 4

#define		BOBBYR_SHIPMENT_HOME_BUTTON_X							iScreenWidthOffset + 515
#define		BOBBYR_SHIPMENT_HOME_BUTTON_Y							BOBBYR_SHIPMENT_BACK_BUTTON_Y

#define		BOBBYR_SHIPMENT_NUM_PREVIOUS_SHIPMENTS		13
#define     MAX_SHIPMENTS_THAT_FIT_ON_SCREEN 13



#define		BOBBYR_SHIPMENT_ORDER_NUM_X							iScreenWidthOffset + 116//LAPTOP_SCREEN_UL_X + 9
#define		BOBBYR_SHIPMENT_ORDER_NUM_START_Y					iScreenHeightOffset + 144
#define		BOBBYR_SHIPMENT_ORDER_NUM_WIDTH						64

#define		BOBBYR_SHIPMENT_GAP_BTN_LINES							20


#define		BOBBYR_SHIPMENT_SHIPMENT_ORDER_NUM_X			BOBBYR_SHIPMENT_ORDER_NUM_X
#define		BOBBYR_SHIPMENT_SHIPMENT_ORDER_NUM_Y			iScreenHeightOffset + 117

#define		BOBBYR_SHIPMENT_NUM_ITEMS_X								iScreenWidthOffset + 183//BOBBYR_SHIPMENT_ORDER_NUM_X+BOBBYR_SHIPMENT_ORDER_NUM_WIDTH+2
#define		BOBBYR_SHIPMENT_NUM_ITEMS_Y								BOBBYR_SHIPMENT_SHIPMENT_ORDER_NUM_Y
#define		BOBBYR_SHIPMENT_NUM_ITEMS_WIDTH						116

//#define		BOBBYR_SHIPMENT_


extern UINT8 gubPurchaseAtTopOfList;
extern BOOLEAN gfDrawGridArrowMouseRegions;
extern BOOLEAN gfDrawGridColumnMouseRegion;
extern MOUSE_REGION	 gSelectedUpDownArrowOnGridScrollAreaRegion[2];
extern void SelectUpDownArrowOnGridScrollAreaRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
extern MOUSE_REGION	*gSelectedGridScrollColumnRegion;
extern INT32 gSelectedGridScrollColumnRegionSize;
extern void SelectGridScrollColumnRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
extern void SelectGridScrollColumnMovementCallBack(MOUSE_REGION * pRegion, INT32 iReason );

UINT32		guiBobbyRShipmentGrid;

BOOLEAN		gfBobbyRShipmentsDirty = FALSE;

INT32			giBobbyRShipmentSelectedShipment = -1;

//Back Button
void BtnBobbyRShipmentBackCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiBobbyRShipmetBack;
INT32		guiBobbyRShipmentBackImage;

//Home Button
void BtnBobbyRShipmentHomeCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiBobbyRShipmentHome;
INT32		giBobbyRShipmentHomeImage;



MOUSE_REGION	gSelectedPreviousShipmentsRegion[BOBBYR_SHIPMENT_NUM_PREVIOUS_SHIPMENTS];
void SelectPreviousShipmentsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


//
// Function Prototypes
//

void DisplayShipmentGrid();
void DisplayPreviousShipments();
void DisplayShipmentTitles();
void RemovePreviousShipmentsMouseRegions();
void CreatePreviousShipmentsMouseRegions();
INT32	CountNumberValidShipmentForTheShipmentsPage();
//ppp
extern CPostalService gPostalService;
extern vector<PShipmentStruct> gShipmentTable;
extern UINT32		guiGoldArrowImages;
extern UINT32		guiBobbyROrderGrid;
//
// Function
//

void GameInitBobbyRShipments()
{

}


BOOLEAN EnterBobbyRShipments()
{
	VOBJECT_DESC	VObjectDesc;

	InitBobbyRWoodBackground();

	// load the Order Grid graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BobbyRay_OnOrder.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBobbyRShipmentGrid));

	// Gold Arrow for the scroll area
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\GoldArrows.sti", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiGoldArrowImages ) );

	guiBobbyRShipmentBackImage =	LoadButtonImage("LAPTOP\\CatalogueButton.sti", -1,0,-1,1,-1 );
	guiBobbyRShipmetBack = CreateIconAndTextButton( guiBobbyRShipmentBackImage, BobbyROrderFormText[BOBBYR_BACK], BOBBYR_GUNS_BUTTON_FONT,
													BOBBYR_GUNS_TEXT_COLOR_ON, BOBBYR_GUNS_SHADOW_COLOR,
													BOBBYR_GUNS_TEXT_COLOR_OFF, BOBBYR_GUNS_SHADOW_COLOR,
													TEXT_CJUSTIFIED,
													BOBBYR_SHIPMENT_BACK_BUTTON_X, BOBBYR_SHIPMENT_BACK_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnBobbyRShipmentBackCallback);
	SetButtonCursor( guiBobbyRShipmetBack, CURSOR_LAPTOP_SCREEN);


	giBobbyRShipmentHomeImage =	UseLoadedButtonImage( guiBobbyRShipmentBackImage, -1,0,-1,1,-1 );
	guiBobbyRShipmentHome = CreateIconAndTextButton( giBobbyRShipmentHomeImage, BobbyROrderFormText[BOBBYR_HOME], BOBBYR_GUNS_BUTTON_FONT,
													BOBBYR_GUNS_TEXT_COLOR_ON, BOBBYR_GUNS_SHADOW_COLOR,
													BOBBYR_GUNS_TEXT_COLOR_OFF, BOBBYR_GUNS_SHADOW_COLOR,
													TEXT_CJUSTIFIED,
													BOBBYR_SHIPMENT_HOME_BUTTON_X, BOBBYR_SHIPMENT_HOME_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnBobbyRShipmentHomeCallback);
	SetButtonCursor( guiBobbyRShipmentHome, CURSOR_LAPTOP_SCREEN);

	CreateBobbyRayOrderTitle();

	giBobbyRShipmentSelectedShipment = -1;
	gubPurchaseAtTopOfList = 0;

	/*
	//if there are shipments
	if( giNumberOfNewBobbyRShipment != 0 )
	{
		INT32 iCnt;

		// WDS - If there are more than 13 shipments only show 13 because
		// that is all that will fit on the screen.  If you show more things
		// get corrupted.
		INT32 max = giNumberOfNewBobbyRShipment;
		if (max > MAX_SHIPMENTS_THAT_FIT_ON_SCREEN)
			max = MAX_SHIPMENTS_THAT_FIT_ON_SCREEN;

		//get the first shipment #
		for( iCnt=0; iCnt<max; iCnt++ )
		{
			if( gpNewBobbyrShipments[iCnt].fActive )
				giBobbyRShipmentSelectedShipment = iCnt;
		}
	}
	*/
	//if there are shipments
	if( gShipmentTable.size() != 0 )
	{
		INT32 iCnt=0;

		//get the first shipment #
		vector<PShipmentStruct>::iterator& psi = gShipmentTable.begin();

		while(psi != gShipmentTable.end())
		{
			if(((PShipmentStruct)*psi)->ShipmentStatus == SHIPMENT_INTRANSIT)
			{
				giBobbyRShipmentSelectedShipment = iCnt;
			}
			psi++;
			iCnt++;
		}
	}
	
	if (gfDrawGridArrowMouseRegions == TRUE)
	{
		MSYS_DisableRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[0]);
		MSYS_DisableRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[1]);
		MSYS_RemoveRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[0]);
		MSYS_RemoveRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[1]);
		gfDrawGridArrowMouseRegions = FALSE;
	}
	if (gfDrawGridColumnMouseRegion == TRUE)
	{
		for(int i=0; i<gSelectedGridScrollColumnRegionSize; i++)
		{
			MSYS_DisableRegion( &gSelectedGridScrollColumnRegion[i] );
			MSYS_RemoveRegion( &gSelectedGridScrollColumnRegion[i] );
		}
		delete [] gSelectedGridScrollColumnRegion;
		gSelectedGridScrollColumnRegion = NULL;
		gSelectedGridScrollColumnRegionSize = 0;
		gfDrawGridColumnMouseRegion = FALSE;
	}
	CreatePreviousShipmentsMouseRegions();

	return( TRUE );
}

void ExitBobbyRShipments()
{
	DeleteBobbyRWoodBackground();
	DestroyBobbyROrderTitle();

	DeleteVideoObjectFromIndex(guiBobbyRShipmentGrid);
	DeleteVideoObjectFromIndex(guiGoldArrowImages);

	UnloadButtonImage( guiBobbyRShipmentBackImage );
	UnloadButtonImage( giBobbyRShipmentHomeImage );
	RemoveButton( guiBobbyRShipmetBack );
	RemoveButton( guiBobbyRShipmentHome );

	RemovePreviousShipmentsMouseRegions();
	if (gfDrawGridArrowMouseRegions == TRUE)
	{
		MSYS_DisableRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[0]);
		MSYS_DisableRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[1]);
		MSYS_RemoveRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[0]);
		MSYS_RemoveRegion( &gSelectedUpDownArrowOnGridScrollAreaRegion[1]);
		gfDrawGridArrowMouseRegions = FALSE;
}
	if (gfDrawGridColumnMouseRegion == TRUE)
	{
		for(int i=0; i<gSelectedGridScrollColumnRegionSize; i++)
		{
			MSYS_DisableRegion( &gSelectedGridScrollColumnRegion[i] );
			MSYS_RemoveRegion( &gSelectedGridScrollColumnRegion[i] );
		}
		delete [] gSelectedGridScrollColumnRegion;
		gSelectedGridScrollColumnRegion = NULL;
		gSelectedGridScrollColumnRegionSize = 0;
		gfDrawGridColumnMouseRegion = FALSE;
	}
}

void HandleBobbyRShipments()
{
	if( gfBobbyRShipmentsDirty )
	{
		gfBobbyRShipmentsDirty = FALSE;

		RenderBobbyRShipments();
	}
}

void RenderBobbyRShipments()
{
//	HVOBJECT hPixHandle;

	// Dealtar: this must be static as this is accessed after this function has returned
	static BobbyRayPurchaseStruct brps[100];
	for(int i = 0; i < 100; i++) //JMich
	{
		memset(&brps[i], 0, sizeof(BobbyRayPurchaseStruct));
	}

	DrawBobbyRWoodBackground();

	DrawBobbyROrderTitle();

	//Output the title
	DrawTextToScreen(gzBobbyRShipmentText[ BOBBYR_SHIPMENT__TITLE ], BOBBYR_ORDER_FORM_TITLE_X, BOBBYR_ORDER_FORM_TITLE_Y, BOBBYR_ORDER_FORM_TITLE_WIDTH, BOBBYR_SHIPMENT_TITLE_TEXT_FONT, BOBBYR_SHIPMENT_TITLE_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

	DisplayShipmentGrid();

	if(giBobbyRShipmentSelectedShipment != -1) 
	{
		RefToShipmentPackageListIterator spli = gShipmentTable[giBobbyRShipmentSelectedShipment]->ShipmentPackages.begin();
		int j;
		for(unsigned i = 0; i < gShipmentTable[giBobbyRShipmentSelectedShipment]->ShipmentPackages.size(); i++, spli++)
		{
			brps[i].bItemQuality = ((ShipmentPackageStruct)*spli).bItemQuality;
			brps[i].ubNumberPurchased = ((ShipmentPackageStruct)*spli).ubNumber;
			brps[i].usItemIndex = ((ShipmentPackageStruct)*spli).usItemIndex;
			brps[i].fUsed = (((ShipmentPackageStruct)*spli).bItemQuality < 100);

			if(brps[i].fUsed)
			{
				for(j=0; j < MAXITEMS; j++)
				{
					if(LaptopSaveInfo.BobbyRayUsedInventory[j].usItemIndex == brps[i].usItemIndex)
					{
						break;
					}
				}
				brps[i].usBobbyItemIndex = j;
			}
			else
			{
				for(j=0; j < MAXITEMS; j++)
				{
					if(LaptopSaveInfo.BobbyRayInventory[j].usItemIndex == brps[i].usItemIndex)
					{
						break;
					}
				}
				brps[i].usBobbyItemIndex = j;
			}
		}
	}
	
	/*
	if( giBobbyRShipmentSelectedShipment != -1 &&
			gpNewBobbyrShipments[ giBobbyRShipmentSelectedShipment ].fActive &&
			gpNewBobbyrShipments[ giBobbyRShipmentSelectedShipment ].fDisplayedInShipmentPage )
	{
//		DisplayPurchasedItems( FALSE, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y, &LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray[giBobbyRShipmentSelectedShipment].BobbyRayPurchase[0], FALSE );
		DisplayPurchasedItems( FALSE, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y, &gpNewBobbyrShipments[giBobbyRShipmentSelectedShipment].BobbyRayPurchase[0], FALSE, giBobbyRShipmentSelectedShipment );
	}
	else
	{
//		DisplayPurchasedItems( FALSE, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y, &LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray[giBobbyRShipmentSelectedShipment].BobbyRayPurchase[0], TRUE );
		DisplayPurchasedItems( FALSE, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y, NULL, TRUE, giBobbyRShipmentSelectedShipment );
	}
	*/

	if( giBobbyRShipmentSelectedShipment != -1 &&
		gShipmentTable[ giBobbyRShipmentSelectedShipment ]->ShipmentStatus == SHIPMENT_INTRANSIT) // &&
	{
		DisplayPurchasedItems( FALSE, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y,&brps[0], FALSE, giBobbyRShipmentSelectedShipment );
	}
	else
	{
		DisplayPurchasedItems( FALSE, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y, NULL, TRUE, giBobbyRShipmentSelectedShipment );
	}
	

	DisplayShipmentTitles();
	DisplayPreviousShipments();

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void BtnBobbyRShipmentBackCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		guiCurrentLaptopMode = LAPTOP_MODE_BOBBY_R_MAILORDER;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


void BtnBobbyRShipmentHomeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		guiCurrentLaptopMode	= LAPTOP_MODE_BOBBY_R;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


void DisplayShipmentGrid()
{
	HVOBJECT hPixHandle, hPixGrid;
	VOBJECT_DESC	VObjectDesc;
	GetVideoObject(&hPixHandle, guiBobbyRShipmentGrid);
	// Shipment Order Grid
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, BOBBYR_SHIPMENT_DELIVERY_GRID_X, BOBBYR_SHIPMENT_DELIVERY_GRID_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BobbyOrderGrid.sti", VObjectDesc.ImageFile);
	AddVideoObject(&VObjectDesc, &guiBobbyROrderGrid);
	GetVideoObject(&hPixGrid, guiBobbyROrderGrid);
	BltVideoObject(FRAME_BUFFER, hPixGrid, 0, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Order Grid
	//BltVideoObject(FRAME_BUFFER, hPixGrid, 0, BOBBYR_SHIPMENT_ORDER_GRID_X, BOBBYR_SHIPMENT_ORDER_GRID_Y, VO_BLT_SRCTRANSPARENCY,NULL);
}



void DisplayShipmentTitles()
{
	//output the order #
	DrawTextToScreen( gzBobbyRShipmentText[BOBBYR_SHIPMENT__ORDERED_ON], BOBBYR_SHIPMENT_SHIPMENT_ORDER_NUM_X, BOBBYR_SHIPMENT_SHIPMENT_ORDER_NUM_Y, BOBBYR_SHIPMENT_ORDER_NUM_WIDTH, BOBBYR_SHIPMENT_STATIC_TEXT_FONT, BOBBYR_SHIPMENT_STATIC_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Output the # of items
	DrawTextToScreen( gzBobbyRShipmentText[BOBBYR_SHIPMENT__NUM_ITEMS], BOBBYR_SHIPMENT_NUM_ITEMS_X, BOBBYR_SHIPMENT_NUM_ITEMS_Y, BOBBYR_SHIPMENT_NUM_ITEMS_WIDTH, BOBBYR_SHIPMENT_STATIC_TEXT_FONT, BOBBYR_SHIPMENT_STATIC_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
}

void DisplayPreviousShipments()
{
	UINT32 uiCnt;
	CHAR16	zText[512];
	UINT16	usPosY = BOBBYR_SHIPMENT_ORDER_NUM_START_Y;
	UINT32	uiNumItems; // = CountNumberValidShipmentForTheShipmentsPage();
	UINT32	uiNumberItemsInShipments = 0;
	UINT32	uiItemCnt;
	UINT8		ubFontColor = BOBBYR_SHIPMENT_STATIC_TEXT_COLOR;
	
	RefToShipmentListIterator sli = gPostalService.LookupShipmentList().begin();

	uiNumItems = (UINT32)gPostalService.LookupShipmentList().size();
	if(uiNumItems > BOBBYR_SHIPMENT_NUM_PREVIOUS_SHIPMENTS)
		uiNumItems = BOBBYR_SHIPMENT_NUM_PREVIOUS_SHIPMENTS;

	//loop through all the shipments
	for( uiCnt=0; uiCnt<uiNumItems; uiCnt++ )
	{
		/*
		//if it is a valid shipment, and can be displayed at bobby r
		if( gpNewBobbyrShipments[ uiCnt ].fActive &&
				gpNewBobbyrShipments[ giBobbyRShipmentSelectedShipment ].fDisplayedInShipmentPage )
		*/
		// if it is a shipment that is active (= in transit)
		if( gShipmentTable[ uiCnt ]->ShipmentStatus == SHIPMENT_INTRANSIT)
		{
			if( uiCnt == (UINT32)giBobbyRShipmentSelectedShipment )
			{
				ubFontColor = FONT_MCOLOR_WHITE;
			}
			else
			{
				ubFontColor = BOBBYR_SHIPMENT_STATIC_TEXT_COLOR;
			}

			//Display the "ordered on day num"
			//swprintf( zText, L"%s %d", gpGameClockString[0], gpNewBobbyrShipments[ uiCnt ].uiOrderedOnDayNum );
			swprintf( zText, L"%s %d", gpGameClockString[0], gShipmentTable[ uiCnt ]->uiOrderDate );
			DrawTextToScreen( zText, BOBBYR_SHIPMENT_ORDER_NUM_X, usPosY, BOBBYR_SHIPMENT_ORDER_NUM_WIDTH, BOBBYR_SHIPMENT_STATIC_TEXT_FONT, ubFontColor, 0, FALSE, CENTER_JUSTIFIED );

			uiNumberItemsInShipments = 0;

			/*
	//		for( uiItemCnt=0; uiItemCnt<LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray[ uiCnt ].ubNumberPurchases; uiItemCnt++ )
			for( uiItemCnt=0; uiItemCnt<gpNewBobbyrShipments[ uiCnt ].ubNumberPurchases; uiItemCnt++ )
			{
	//			uiNumberItemsInShipments += LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray[ uiCnt ].BobbyRayPurchase[uiItemCnt].ubNumberPurchased;
				uiNumberItemsInShipments += gpNewBobbyrShipments[ uiCnt ].BobbyRayPurchase[uiItemCnt].ubNumberPurchased;
			}
			*/
			for( uiItemCnt=0; uiItemCnt<gShipmentTable[ uiCnt ]->ShipmentPackages.size(); uiItemCnt++ )
			{
				uiNumberItemsInShipments += gShipmentTable[ uiCnt ]->ShipmentPackages[uiItemCnt].ubNumber;
			}


			//Display the # of items
			swprintf( zText, L"%d", uiNumberItemsInShipments );
			DrawTextToScreen( zText, BOBBYR_SHIPMENT_NUM_ITEMS_X, usPosY, BOBBYR_SHIPMENT_NUM_ITEMS_WIDTH, BOBBYR_SHIPMENT_STATIC_TEXT_FONT, ubFontColor, 0, FALSE, CENTER_JUSTIFIED );
			usPosY += BOBBYR_SHIPMENT_GAP_BTN_LINES;
		}
	}
}

void CreatePreviousShipmentsMouseRegions()
{
	UINT32 uiCnt;
	UINT16	usPosY = BOBBYR_SHIPMENT_ORDER_NUM_START_Y;
	UINT16	usWidth = BOBBYR_SHIPMENT_DELIVERY_GRID_WIDTH;
	UINT16	usHeight = GetFontHeight( BOBBYR_SHIPMENT_STATIC_TEXT_FONT );
	//UINT32	uiNumItems = CountNumberOfBobbyPurchasesThatAreInTransit();
	UINT32	uiNumItems = gPostalService.GetShipmentCount(SHIPMENT_INTRANSIT);

		// WDS - If there are more than 13 shipments only show 13 because
		// that is all that will fit on the screen.  If you show more things
		// get corrupted.
		UINT32 max = uiNumItems;
		if (max > MAX_SHIPMENTS_THAT_FIT_ON_SCREEN)
			max = MAX_SHIPMENTS_THAT_FIT_ON_SCREEN;

	for( uiCnt=0; uiCnt<max; uiCnt++ )
	{
		MSYS_DefineRegion( &gSelectedPreviousShipmentsRegion[uiCnt], BOBBYR_SHIPMENT_ORDER_NUM_X, usPosY, (UINT16)(BOBBYR_SHIPMENT_ORDER_NUM_X+usWidth), (UINT16)(usPosY+usHeight), MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectPreviousShipmentsRegionCallBack );
		MSYS_AddRegion(&gSelectedPreviousShipmentsRegion[uiCnt]);
		MSYS_SetRegionUserData( &gSelectedPreviousShipmentsRegion[uiCnt], 0, uiCnt);

		usPosY += BOBBYR_SHIPMENT_GAP_BTN_LINES;
	}
}

void RemovePreviousShipmentsMouseRegions()
{
	UINT32 uiCnt;
	//UINT32	uiNumItems = CountNumberOfBobbyPurchasesThatAreInTransit();
	UINT32	uiNumItems = gPostalService.GetShipmentCount(SHIPMENT_INTRANSIT);


	for( uiCnt=0; uiCnt<uiNumItems; uiCnt++ )
	{
	MSYS_RemoveRegion( &gSelectedPreviousShipmentsRegion[uiCnt] );
	}
}

void SelectPreviousShipmentsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		gubPurchaseAtTopOfList = 0;
		INT32 iSlotID = MSYS_GetRegionUserData( pRegion, 0 );


//		if( CountNumberOfBobbyPurchasesThatAreInTransit() > iSlotID )
		if( gPostalService.GetShipmentCount(SHIPMENT_INTRANSIT) > iSlotID )
		{
			INT32 iCnt;
			INT32	iValidShipmentCounter=0;

			giBobbyRShipmentSelectedShipment = -1;

			//loop through and get the "x" iSlotID shipment
//			for( iCnt=0; iCnt<giNumberOfNewBobbyRShipment; iCnt++ )
			for( iCnt=0; iCnt<gPostalService.GetShipmentCount(SHIPMENT_INTRANSIT); iCnt++ )
			{
//				if( gpNewBobbyrShipments[iCnt].fActive )
				if( gShipmentTable[iCnt]->ShipmentStatus == SHIPMENT_INTRANSIT )
				{
					if( iValidShipmentCounter == iSlotID )
					{
						giBobbyRShipmentSelectedShipment = iCnt;
					}

					iValidShipmentCounter++;
				}
			}
		}

		gfBobbyRShipmentsDirty = TRUE;
	}
}

//Dealtar's Airport Externalization
/*
* Function no longer used.
INT32	CountNumberValidShipmentForTheShipmentsPage()
{
	if( giNumberOfNewBobbyRShipment > BOBBYR_SHIPMENT_NUM_PREVIOUS_SHIPMENTS )
		return( BOBBYR_SHIPMENT_NUM_PREVIOUS_SHIPMENTS );
	else
		return( giNumberOfNewBobbyRShipment );
}
*/

