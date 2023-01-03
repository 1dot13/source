#ifndef __BOBBYRGUNS_H
#define __BOBBYRGUNS_H

#define		BOBBYR_ITEM_DESC_NAME_SIZE				160
#define		BOBBYR_ITEM_DESC_INFO_SIZE				640
#define		BOBBYR_ITEM_DESC_FILE_SIZE				800

#define		BOBBYR_USED_ITEMS			0xFFFFFFFF


#define		BOBBYR_GUNS_BUTTON_FONT						FONT10ARIAL
#define		BOBBYR_GUNS_TEXT_COLOR_ON					FONT_NEARBLACK // FONT_DKGREEN
#define		BOBBYR_GUNS_TEXT_COLOR_OFF				FONT_NEARBLACK
//#define		BOBBYR_GUNS_TEXT_COLOR_ON					FONT_MCOLOR_DKWHITE
//#define		BOBBYR_GUNS_TEXT_COLOR_OFF				FONT_MCOLOR_WHITE

#define		BOBBYR_GUNS_SHADOW_COLOR					169

#define		BOBBYR_NO_ITEMS										65535





extern UINT16			gusCurWeaponIndex;
extern UINT8			gubLastGunIndex;



void GameInitBobbyRGuns();
BOOLEAN EnterBobbyRGuns();
void ExitBobbyRGuns();
void HandleBobbyRGuns();
void RenderBobbyRGuns();

BOOLEAN DeleteBobbyRGunsFilter();
BOOLEAN DeleteBobbyRAmmoFilter();
BOOLEAN DeleteBobbyRUsedFilter();
BOOLEAN DeleteBobbyRArmourFilter();
BOOLEAN DeleteBobbyRMiscFilter();
BOOLEAN InitBobbyRGunsFilterBar();
BOOLEAN InitBobbyRAmmoFilterBar();
BOOLEAN InitBobbyRUsedFilterBar();
BOOLEAN InitBobbyRArmourFilterBar();
BOOLEAN InitBobbyRMiscFilterBar();

BOOLEAN DisplayBobbyRBrTitle();
BOOLEAN DeleteBobbyBrTitle();
BOOLEAN InitBobbyBrTitle();
BOOLEAN InitBobbyMenuBar( );
BOOLEAN DeleteBobbyMenuBar();



//BOOLEAN DisplayWeaponInfo();
BOOLEAN DisplayItemInfo(UINT32 uiItemClass, INT32 iFilter, INT32 iSubFilter = -1);  // Madd: added iSubFilter for new BR filter options
void DeleteMouseRegionForBigImage();
void UpdateButtonText(UINT32	uiCurPage);

void UpdateAmmoFilterButtons();
void UpdateGunFilterButtons();
void UpdateUsedFilterButtons();
void UpdateArmourFilterButtons();
void UpdateMiscFilterButtons();

void EnterInitBobbyRGuns();
void DailyUpdateOfBobbyRaysUsedInventory();
UINT16 CalcBobbyRayCost( UINT16 usIndex, UINT16 usBobbyIndex, BOOLEAN fUsed);
//void CalculateFirstAndLastIndexs();
void SetFirstLastPagesForUsed(INT32 iFilter);
void SetFirstLastPagesForNew( UINT32 uiClass, INT32 iFilter , INT32 iSubFilter = -1); // Madd: added iSubFilter for new BR filter options

#endif



 


