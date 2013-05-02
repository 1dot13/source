#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "AimLinks.h"
	#include "aim.h"
	#include "WCheck.h"
	#include "WordWrap.h"
	#include "Text.h"
	#include "Multi Language Graphic Utils.h"
#endif

#ifdef JA2UB
#include "ub_config.h"
#endif

#define		AIM_LINK_TITLE_FONT						FONT14ARIAL
#define		AIM_LINK_TITLE_COLOR					AIM_GREEN

#define	AIM_LINK_FONT									FONT12ARIAL
#define	AIM_LINK_COLOR								AIM_FONT_GOLD

#define		AIM_LINK_NUM_LINKS						3

#define		AIM_LINK_LINK_OFFSET_Y				94//90

#define		AIM_LINK_LINK_WIDTH						420
#define		AIM_LINK_LINK_HEIGHT					70

#define		AIM_LINK_BOBBY_LINK_X					LAPTOP_SCREEN_UL_X + 40
#define		AIM_LINK_BOBBY_LINK_Y					LAPTOP_SCREEN_WEB_UL_Y + 91

#define		AIM_LINK_FUNERAL_LINK_X				AIM_LINK_BOBBY_LINK_X
#define		AIM_LINK_FUNERAL_LINK_Y				AIM_LINK_BOBBY_LINK_Y + AIM_LINK_LINK_OFFSET_Y

#define		AIM_LINK_INSURANCE_LINK_X			AIM_LINK_BOBBY_LINK_X
#define		AIM_LINK_INSURANCE_LINK_Y			AIM_LINK_FUNERAL_LINK_Y + AIM_LINK_LINK_OFFSET_Y

#define		AIM_LINK_TITLE_X							IMAGE_OFFSET_X + 149
#define		AIM_LINK_TITLE_Y							AIM_SYMBOL_Y + AIM_SYMBOL_SIZE_Y + 10
#define		AIM_LINK_TITLE_WIDTH					AIM_SYMBOL_WIDTH

#define		AIM_LINK_LINK_TEXT_1_Y				AIM_LINK_BOBBY_LINK_Y + 71
#define		AIM_LINK_LINK_TEXT_2_Y				AIM_LINK_FUNERAL_LINK_Y + 36
#define		AIM_LINK_LINK_TEXT_3_Y				AIM_LINK_INSURANCE_LINK_Y + 45


	#ifdef JA2UB
	INT16 LinkID = 3;
	#endif



UINT32		guiBobbyLink;
UINT32		guiFuneralLink;
UINT32		guiInsuranceLink;
UINT8			gubLinkPages[]={
							BOBBYR_BOOKMARK,
							FUNERAL_BOOKMARK,
							INSURANCE_BOOKMARK};

//Clicking on guys Face
MOUSE_REGION	gSelectedLinkRegion[ AIM_LINK_NUM_LINKS ] ;
void SelectLinkRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


void GameInitAimLinks()
{

}

BOOLEAN EnterAimLinks()
{
	VOBJECT_DESC	VObjectDesc;
	UINT16					usPosY;	
	
	InitAimDefaults();
	InitAimMenuBar();

	#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkBobby == TRUE )
	{
	#endif
	// load the Bobby link graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_BOBBYRAYLINK );
	CHECKF(AddVideoObject(&VObjectDesc, &guiBobbyLink));
	#ifdef JA2UB
	}
	#endif
	
	#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkFuneral == TRUE )
	{
	#endif
	// load the Funeral graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_MORTUARYLINK );
	CHECKF(AddVideoObject(&VObjectDesc, &guiFuneralLink));
	#ifdef JA2UB
	}
	#endif
	
	#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
	{
	#endif
	// load the Insurance graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_INSURANCELINK );
	CHECKF(AddVideoObject(&VObjectDesc, &guiInsuranceLink));
	#ifdef JA2UB
	}
	#endif
	
	#ifdef JA2UB
		usPosY = AIM_LINK_BOBBY_LINK_Y;
		if (gGameUBOptions.LaptopLinkBobby == TRUE )
		{
		MSYS_DefineRegion( &gSelectedLinkRegion[0], AIM_LINK_BOBBY_LINK_X, usPosY , AIM_LINK_BOBBY_LINK_X + AIM_LINK_LINK_WIDTH, (UINT16)(usPosY + AIM_LINK_LINK_HEIGHT), MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinkRegionCallBack );
		MSYS_AddRegion(&gSelectedLinkRegion[0]);
		MSYS_SetRegionUserData( &gSelectedLinkRegion[0], 0, gubLinkPages[0]);
		//usPosY += AIM_LINK_LINK_OFFSET_Y;	
		}
		
		usPosY = AIM_LINK_FUNERAL_LINK_Y;	
		if (gGameUBOptions.LaptopLinkFuneral == TRUE )
		{
		MSYS_DefineRegion( &gSelectedLinkRegion[1], AIM_LINK_FUNERAL_LINK_X, usPosY , AIM_LINK_FUNERAL_LINK_X + AIM_LINK_LINK_WIDTH, (UINT16)(usPosY + AIM_LINK_LINK_HEIGHT), MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinkRegionCallBack );
		MSYS_AddRegion(&gSelectedLinkRegion[1]);
		MSYS_SetRegionUserData( &gSelectedLinkRegion[1], 0, gubLinkPages[1]);
		//usPosY += AIM_LINK_LINK_OFFSET_Y;	
		}
		
		usPosY = AIM_LINK_INSURANCE_LINK_Y;
		if (gGameUBOptions.LaptopLinkInsurance== TRUE )
		{
		MSYS_DefineRegion( &gSelectedLinkRegion[2], AIM_LINK_INSURANCE_LINK_X, usPosY , AIM_LINK_INSURANCE_LINK_X + AIM_LINK_LINK_WIDTH, (UINT16)(usPosY + AIM_LINK_LINK_HEIGHT), MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinkRegionCallBack );
		MSYS_AddRegion(&gSelectedLinkRegion[2]);
		MSYS_SetRegionUserData( &gSelectedLinkRegion[2], 0, gubLinkPages[2]);
		usPosY += AIM_LINK_LINK_OFFSET_Y;	
		}
	
	#else	
	usPosY = AIM_LINK_BOBBY_LINK_Y;
	for(UINT8 i=0; i<AIM_LINK_NUM_LINKS; i++)
	{
		MSYS_DefineRegion( &gSelectedLinkRegion[i], AIM_LINK_BOBBY_LINK_X, usPosY , AIM_LINK_BOBBY_LINK_X + AIM_LINK_LINK_WIDTH, (UINT16)(usPosY + AIM_LINK_LINK_HEIGHT), MSYS_PRIORITY_HIGH,
								CURSOR_WWW, MSYS_NO_CALLBACK, SelectLinkRegionCallBack );
		MSYS_AddRegion(&gSelectedLinkRegion[i]);
		MSYS_SetRegionUserData( &gSelectedLinkRegion[i], 0, gubLinkPages[i]);
		usPosY += AIM_LINK_LINK_OFFSET_Y;
	}
	#endif

	RenderAimLinks();
	return(TRUE);
}

void ExitAimLinks()
{
	INT16						i;

	RemoveAimDefaults();
	
	#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkBobby == TRUE )
		DeleteVideoObjectFromIndex(guiBobbyLink);
	
	if (gGameUBOptions.LaptopLinkFuneral == TRUE )
		DeleteVideoObjectFromIndex(guiFuneralLink);
	
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		DeleteVideoObjectFromIndex(guiInsuranceLink);
	
	if (gGameUBOptions.LaptopLinkBobby == TRUE )
		MSYS_RemoveRegion( &gSelectedLinkRegion[0]);
	
	if (gGameUBOptions.LaptopLinkFuneral == TRUE )
		MSYS_RemoveRegion( &gSelectedLinkRegion[1]);
	
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		MSYS_RemoveRegion( &gSelectedLinkRegion[2]);
		
	#else
	
	DeleteVideoObjectFromIndex(guiBobbyLink);
	DeleteVideoObjectFromIndex(guiFuneralLink);
	DeleteVideoObjectFromIndex(guiInsuranceLink);
	
	for(i=0; i<AIM_LINK_NUM_LINKS; i++)
		MSYS_RemoveRegion( &gSelectedLinkRegion[i]);
	#endif
	
	
	ExitAimMenuBar();

}

void HandleAimLinks()
{

}

void RenderAimLinks()
{
	HVOBJECT hPixHandle;

	DrawAimDefaults();
	DisableAimButton();

	#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkBobby == TRUE )
	{
	#endif
	GetVideoObject(&hPixHandle, guiBobbyLink);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, AIM_LINK_BOBBY_LINK_X, AIM_LINK_BOBBY_LINK_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	#ifdef JA2UB
	}
	#endif
	
	#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkFuneral == TRUE )
	{
	#endif
	GetVideoObject(&hPixHandle, guiFuneralLink);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, AIM_LINK_FUNERAL_LINK_X, AIM_LINK_FUNERAL_LINK_Y, VO_BLT_SRCTRANSPARENCY,NULL);
//	DrawTextToScreen(AimLinkText[AIM_LINK_FUNERAL], AIM_LINK_BOBBY_LINK_X, AIM_LINK_LINK_TEXT_2_Y, AIM_LINK_LINK_WIDTH, AIM_LINK_FONT, AIM_LINK_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	#ifdef JA2UB
	}
	#endif
	
	#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
	{
	#endif
	GetVideoObject(&hPixHandle, guiInsuranceLink);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, AIM_LINK_INSURANCE_LINK_X, AIM_LINK_INSURANCE_LINK_Y, VO_BLT_SRCTRANSPARENCY,NULL);
//	DrawTextToScreen(AimLinkText[AIM_LINK_LISTENING], AIM_LINK_BOBBY_LINK_X, AIM_LINK_LINK_TEXT_3_Y, AIM_LINK_LINK_WIDTH, AIM_LINK_FONT, AIM_LINK_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	#ifdef JA2UB
	}
	#endif
	
	//Draw Link Title
	DrawTextToScreen(AimLinkText[AIM_LINK_TITLE], AIM_LINK_TITLE_X, AIM_LINK_TITLE_Y, AIM_LINK_TITLE_WIDTH, AIM_LINK_TITLE_FONT, AIM_LINK_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void SelectLinkRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT32	gNextLaptopPage;

		gNextLaptopPage = MSYS_GetRegionUserData( pRegion, 0 );

		GoToWebPage( gNextLaptopPage );
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}









