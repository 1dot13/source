#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "Types.h"
	#include "ShopKeeper Interface.h"
	#include "Utilities.h"
	#include "Game Clock.h"
	#include "Render Dirty.h"
	#include "Font Control.h"
	#include "WordWrap.h"
	#include "Text Input.h"
	#include "Interface.h"
	#include "Interface Control.h"
	#include "Overhead.h"
	#include "Cursors.h"
	#include "SysUtil.h"
	#include "Interface Panels.h"
	#include "Radar Screen.h"
	#include "Interface Items.h"
	#include "Interface Utils.h"
	#include "VObject_Blitters.h"
	#include "Finances.h"
	#include "Text.h"
	#include "Cursor Control.h"
	#include "Input.h"
	#include "Arms Dealer Init.h"
	#include "english.h"
	#include "Soldier Add.h"
	#include "Faces.h"
	#include "Dialogue Control.h"
	#include "ShopKeeper Quotes.h"
	#include "GameSettings.h"
	#include "MercTextBox.h"
	#include "Random.h"
	#include "Squads.h"
	#include "Soldier Profile.h"
	#include "Message.h"
	#include "Personnel.h"
	#include "LaptopSave.h"
	#include "Quests.h"
	#include "Weapons.h"
	#include "MessageBoxScreen.h"
	#include "LINE.H"
	#include "Drugs and Alcohol.h"
	#include "Map Screen Interface.h"
	#include "Soldier macros.h"
	#include "armsdealerinvinit.h"
	#include "opplist.h"
	#include "los.h"
	#include "NPC.h"
	#include "Soldier Create.h"
	#include "PATHAI.h"
	#include "Points.h"
	#include "InterfaceItemImages.h"
	#include "Encyclopedia_new.h"
	#include "Animation Control.h"	// added by Flugente
	#include "Town Militia.h"		// added by Flugente
#endif

#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "Ja25_Tactical.h"
#include "ub_config.h"
#endif

#include "BuildDefines.h"
#include <algorithm>

////////////////////////////////////////////
//
//	global defines
//
///////////////////////////////////////////

#define		SKI_INTERFACE_WIDTH								536
#define		SKI_INTERFACE_HEIGHT							340

#define		SCREEN_X_OFFSET									(((SCREEN_WIDTH - SKI_INTERFACE_WIDTH) / 2))
#define		SCREEN_Y_OFFSET									((((SCREEN_HEIGHT - INV_INTERFACE_HEIGHT) - (SKI_INTERFACE_HEIGHT)) / 2))

#define		SKI_BUTTON_FONT									MILITARYFONT1//FONT14ARIAL
#define		SKI_BUTTON_COLOR								73

#define		SKI_TITLE_FONT									MILITARYFONT1//FONT14ARIAL
#define		SKI_TITLE_COLOR									169//FONT_MCOLOR_LTYELLOW

#define		SKI_LABEL_FONT									MILITARYFONT1

#define		SKI_ITEM_DESC_FONT								SMALLCOMPFONT
#define		SKI_ITEM_PRICE_COLOR							FONT_MCOLOR_WHITE

#define		SKIT_NUMBER_FONT								BLOCKFONT2

//Number of Inventory slots
#define		SKI_NUM_ARMS_DEALERS_INV_SLOTS					500		// Flugente: simply a resonably huge number
#define		SKI_NUM_TRADING_INV_SLOTS						500		// Flugente: simply a resonably huge number

//Inventory Slots size and offsets
#define		SKI_INV_SLOT_WIDTH								67
#define		SKI_INV_SLOT_HEIGHT								35		// bfant: increased from 31 to fit item name text
#define		SKI_INV_HEIGHT									(SKI_INV_SLOT_HEIGHT - 7)
#define		SKI_INV_WIDTH									60
#define		SKI_INV_OFFSET_X								74
#define		SKI_INV_OFFSET_Y								40		// bfant: increased from 36

#define		SKI_INV_PRICE_OFFSET_X							1
#define		SKI_INV_PRICE_OFFSET_Y							24

// the delay for glow cycling in glow areas, in millisecs
#define		SKI_GLOW_DELAY 70

#define		SKI_TACTICAL_BACKGROUND_START_X				(SCREEN_X_OFFSET + SKI_INTERFACE_WIDTH)
#define		SKI_TACTICAL_BACKGROUND_START_Y				0

#define		SKI_DROP_ITEM_TO_GROUND_START_X				SKI_TACTICAL_BACKGROUND_START_X
#define		SKI_DROP_ITEM_TO_GROUND_START_Y				(SCREEN_Y_OFFSET + 262)
#define		SKI_DROP_ITEM_TO_GROUND_TEXT_START_Y		(SCREEN_Y_OFFSET + 262)

#define		SKI_ITEM_MOVEMENT_AREA_X							(SCREEN_X_OFFSET + 85)
#define		SKI_ITEM_MOVEMENT_AREA_Y							(SCREEN_Y_OFFSET + 263)
#define		SKI_ITEM_MOVEMENT_AREA_WIDTH						(SCREEN_WIDTH - SKI_ITEM_MOVEMENT_AREA_X)
#define		SKI_ITEM_MOVEMENT_AREA_HEIGHT						(SCREEN_Y_OFFSET + 215)//72

#define		SKI_DEALER_OFFER_AREA_Y								(SCREEN_Y_OFFSET + 148)

#define		SKI_ITEM_NUMBER_TEXT_OFFSET_X					50
#define		SKI_ITEM_NUMBER_TEXT_OFFSET_Y					19		// bfant: increased from 15 to fit item name text
#define		SKI_ITEM_NUMBER_TEXT_WIDTH						15

#define		SKI_SUBTITLE_TEXT_SIZE								512

#define		SKI_POSITION_SUBTITLES_Y							(SCREEN_Y_OFFSET + INV_INTERFACE_HEIGHT)//100

#define		SKI_SMALL_FACE_WIDTH									16
#define		SKI_SMALL_FACE_HEIGHT									14
#define		SKI_SMALL_FACE_OFFSET_X								52
#define		SKI_SMALL_FACE_OFFSET_Y								17

#define		SKI_ATTACHMENT_SYMBOL_X_OFFSET				56
#define		SKI_ATTACHMENT_SYMBOL_Y_OFFSET				14

#define		SKI_MAX_AMOUNT_OF_ITEMS_DEALER_CAN_REPAIR_AT_A_TIME			4

// round off reapir times shown to the near quarter-hour
#define		REPAIR_MINUTES_INTERVAL						15

#define		SKI_DEALERS_RANDOM_QUOTE_DELAY				15000
#define		SKI_DEALERS_RANDOM_QUOTE_DELAY_INCREASE_RATE		5000

#define		DELAY_FOR_SHOPKEEPER_IDLE_QUOTE 20000
#define		CHANCE_FOR_SHOPKEEPER_IDLE_QUOTE 40

#define		REALLY_BADLY_DAMAGED_THRESHOLD		30

#define		REPAIR_DELAY_IN_HOURS							6

#define		FLO_DISCOUNT_PERCENTAGE						10

#ifdef JA2UB
#define		SKI_BETTY_MINIMUM_AMOUNT_TO_SET_FACT		10
#define		SKI_RAUL_MINIMUM_AMOUNT_TO_SET_FACT			100
#endif

// Flugente: new defines for shopkeeper interface with a resolution-dependent size

// the main tradescreen is a rectangle, which contains trader inventory, trader offer area and player offer area as sub-rectangles
#define		SKI_TOTAL_END_X						(SCREEN_X_OFFSET + SKI_INTERFACE_WIDTH)
#define		SKI_TOTAL_END_Y						(SCREEN_HEIGHT - INV_INTERFACE_HEIGHT)
#define		SKI_TOTAL_BEGIN_Y					0

// we determine how many item boxes we could display in a row, and then depend where the shopkeeper display should start
// as a result, the margins will always look the same
#define		SKI_TOTAL_BEGIN_X					(SKI_TOTAL_END_X - 166 - ((SKI_TOTAL_END_X - 166) / SKI_INV_OFFSET_X) * SKI_INV_OFFSET_X)

// first: trader inventory
#define		SKI_TRADER_INVENTORY_BEGIN_X		SKI_TOTAL_BEGIN_X
#define		SKI_TRADER_INVENTORY_BEGIN_Y		SKI_TOTAL_BEGIN_Y
#define		SKI_TRADER_INVENTORY_END_X			SKI_TOTAL_END_X
#define		SKI_TRADER_INVENTORY_END_Y			(143 + (SKI_TOTAL_END_Y - SKI_INTERFACE_HEIGHT) / 2)

#define		SKI_MAIN_TITLE_X					(SKI_TRADER_INVENTORY_BEGIN_X + 112)
#define		SKI_MAIN_TITLE_Y					(SKI_TRADER_INVENTORY_BEGIN_Y + 12)
#define		SKI_MAIN_TITLE_WIDTH				420

// merchant face
#define		SKI_FACE_X							(SKI_TRADER_INVENTORY_BEGIN_X + 13)
#define		SKI_FACE_Y							(SKI_TRADER_INVENTORY_BEGIN_Y + 13)
#define		SKI_FACE_WIDTH						90
#define		SKI_FACE_HEIGHT						100

// trader inventory page buttons
#define		SKI_PAGE_UP_ARROWS_X				(SKI_TRADER_INVENTORY_BEGIN_X + 121)
#define		SKI_PAGE_UP_ARROWS_Y				(SKI_TRADER_INVENTORY_BEGIN_Y + 35)

#define		SKI_PAGE_DOWN_ARROWS_X				SKI_PAGE_UP_ARROWS_X
#define		SKI_PAGE_DOWN_ARROWS_Y				(SKI_TRADER_INVENTORY_BEGIN_Y + 102)

#define		SKI_PAGE_X							(SKI_TRADER_INVENTORY_BEGIN_X + 112)
#define		SKI_PAGE_Y							(SKI_TRADER_INVENTORY_BEGIN_Y + 70)
#define		SKI_PAGE_WIDTH						45
#define		SKI_PAGE_HEIGHT						27

#define		SKI_BUDGET_X						(SKI_TRADER_INVENTORY_BEGIN_X + 9)
#define		SKI_BUDGET_Y						(SKI_FACE_Y + SKI_FACE_HEIGHT + 5)
#define		SKI_BUDGET_WIDTH					73
#define		SKI_BUDGET_OFFSET_TO_VALUE			(SKI_BUDGET_Y + 10)

// trader inventory item boxes
#define		SKI_TRADER_INVENTORY_BOXES_BEGIN_X	(SKI_TRADER_INVENTORY_BEGIN_X + 166)
#define		SKI_TRADER_INVENTORY_BOXES_BEGIN_Y	(SKI_TRADER_INVENTORY_BEGIN_Y + 30)
#define		SKI_TRADER_INVENTORY_BOXES_PER_ROW	((SKI_TRADER_INVENTORY_END_X - SKI_TRADER_INVENTORY_BOXES_BEGIN_X) / SKI_INV_OFFSET_X) 
#define		SKI_TRADER_INVENTORY_BOXES_PER_COL	((SKI_TRADER_INVENTORY_END_Y - SKI_TRADER_INVENTORY_BOXES_BEGIN_Y) / SKI_INV_OFFSET_Y) 

// second: trader offer area
#define		SKI_TRADER_OFFER_BEGIN_X			SKI_TOTAL_BEGIN_X
#define		SKI_TRADER_OFFER_BEGIN_Y			SKI_TRADER_INVENTORY_END_Y
#define		SKI_TRADER_OFFER_END_X				SKI_TOTAL_END_X
#define		SKI_TRADER_OFFER_END_Y				(90 + SKI_TRADER_INVENTORY_END_Y)	// bfant: adjusted so 2 rows fit again after item text modification

// trader offer area item boxes
#define		SKI_TRADER_OFFER_BOXES_BEGIN_X		(SKI_TRADER_OFFER_BEGIN_X + 91)
#define		SKI_TRADER_OFFER_BOXES_BEGIN_Y		(SKI_TRADER_OFFER_BEGIN_Y + 8)
#define		SKI_TRADER_OFFER_BOXES_PER_ROW		((SKI_TRADER_OFFER_END_X - SKI_TRADER_OFFER_BOXES_BEGIN_X) / SKI_INV_OFFSET_X) 
#define		SKI_TRADER_OFFER_BOXES_PER_COL		((SKI_TRADER_OFFER_END_Y - SKI_TRADER_OFFER_BOXES_BEGIN_Y) / SKI_INV_OFFSET_Y)

#define		SKI_TOTAL_COST_X					(SKI_TRADER_OFFER_BEGIN_X + 9)
#define		SKI_TOTAL_COST_Y					(SKI_TRADER_OFFER_BEGIN_Y + 35)//159
#define		SKI_TOTAL_COST_WIDTH				73

#define		SKI_ARMS_DEALER_TOTAL_COST_X		(SKI_TRADER_OFFER_BEGIN_X + 16)
#define		SKI_ARMS_DEALER_TOTAL_COST_Y		(SKI_TRADER_OFFER_BEGIN_Y + 51)//191
#define		SKI_ARMS_DEALER_TOTAL_COST_WIDTH	59
#define		SKI_ARMS_DEALER_TOTAL_COST_HEIGHT	20

// third: player offer area
#define		SKI_PLAYER_OFFER_BEGIN_X			SKI_TOTAL_BEGIN_X
#define		SKI_PLAYER_OFFER_BEGIN_Y			SKI_TRADER_OFFER_END_Y
#define		SKI_PLAYER_OFFER_END_X				SKI_TOTAL_END_X
#define		SKI_PLAYER_OFFER_END_Y				SKI_TOTAL_END_Y

// player offer area item boxes
#define		SKI_PLAYER_OFFER_BOXES_BEGIN_X		(SKI_PLAYER_OFFER_BEGIN_X + 91)
#define		SKI_PLAYER_OFFER_BOXES_BEGIN_Y		(SKI_PLAYER_OFFER_BEGIN_Y + 41)
#define		SKI_PLAYER_OFFER_BOXES_PER_ROW		((SKI_PLAYER_OFFER_END_X - SKI_PLAYER_OFFER_BOXES_BEGIN_X) / SKI_INV_OFFSET_X) 
#define		SKI_PLAYER_OFFER_BOXES_PER_COL		((SKI_PLAYER_OFFER_END_Y - SKI_PLAYER_OFFER_BOXES_BEGIN_Y) / SKI_INV_OFFSET_Y) 

#define		SKI_PLAYERS_CURRENT_BALANCE_X						(SKI_PLAYER_OFFER_BEGIN_X + 9)
#define		SKI_PLAYERS_CURRENT_BALANCE_Y						(SKI_PLAYER_OFFER_BEGIN_Y + 35)
#define		SKI_PLAYERS_CURRENT_BALANCE_WIDTH					SKI_TOTAL_COST_WIDTH
#define		SKI_PLAYERS_CURRENT_BALANCE_OFFSET_TO_VALUE			(SKI_PLAYERS_CURRENT_BALANCE_Y + 20)

#define		SKI_TOTAL_VALUE_X									(SKI_PLAYER_OFFER_BEGIN_X + 9)
#define		SKI_TOTAL_VALUE_Y									(SKI_PLAYERS_CURRENT_BALANCE_Y + 50)
#define		SKI_TOTAL_VALUE_WIDTH								SKI_TOTAL_COST_WIDTH
#define		SKI_TOTAL_VALUE_OFFSET_TO_VALUE						(SKI_TOTAL_VALUE_Y + 20)

#define		SKI_TRANSACTION_BUTTON_X			(SKI_PLAYER_OFFER_BEGIN_X + 147)//214
#define		SKI_TRANSACTION_BUTTON_Y			(SKI_PLAYER_OFFER_BEGIN_Y + 8)

#define		SKI_DONE_BUTTON_X					(SKI_PLAYER_OFFER_BEGIN_X + 292)//414
#define		SKI_DONE_BUTTON_Y					SKI_TRANSACTION_BUTTON_Y

// when colouring the tradescreen, we use this margin
#define		SKI_MARGIN							4
////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////
#ifdef JA2UB
//ja25 UB
enum
{
	TRNSMTR_MSG_BOX__NONE,
	TRNSMTR_MSG_BOX__1_FRAME_WAIT,
	TRNSMTR_MSG_BOX__DISPLAY_BOX,
};

void AskUserToAttachTransmitterToLaptop();
void AttachLaptopTransmitterToLaptop( UINT8 ubExitValue );
#endif

UINT32		guiCornerWhereTacticalIsStillSeenImage;		//This image is for where the corner of tactical is still seen through the shop keeper interface

BOOLEAN		gfSKIScreenEntry = TRUE;
BOOLEAN		gfSKIScreenExit	= FALSE;
BOOLEAN		gfUserHasRequestedToLeave = FALSE;

BOOLEAN		gfRenderScreenOnNextLoop = FALSE;

UINT8			gubSkiDirtyLevel = SKI_DIRTY_LEVEL0;
INT32			giSKIMessageBox=-1;

INT8			gbSelectedArmsDealerID = -1;		//Contains the enum value for the currently selected arms dealer

// Flugente: while we're trading, store the ID of non-NPC dealers
UINT8			gusIDOfCivTrader = NOBODY;

//the quote that is in progress, in certain circumstances, we don't want queuing of related but different quotes
INT32			giShopKeepDialogueEventinProgress = - 1;

INVENTORY_IN_SLOT		gMoveingItem;

OBJECTTYPE					*gpHighLightedItemObject=NULL;

BOOLEAN gfResetShopKeepIdleQuote = FALSE;
BOOLEAN gfDoEvaluationAfterOpening = FALSE;

typedef struct
{
	UINT8		ubCurrentPage;
	UINT8		ubNumberOfPages;

	UINT16		ubFirstItemIndexOnPage;

} SELECTED_ARMS_DEALERS_STATS;

SELECTED_ARMS_DEALERS_STATS		gSelectArmsDealerInfo;

//This pointer is used to store the inventory the arms dealer has for sale
std::vector<INVENTORY_IN_SLOT> gpTempDealersInventory;

INVENTORY_IN_SLOT	ArmsDealerOfferArea[ SKI_NUM_TRADING_INV_SLOTS ];
INVENTORY_IN_SLOT	PlayersOfferArea[ SKI_NUM_TRADING_INV_SLOTS ];

std::vector<OBJECTTYPE> subObjects;

BOOLEAN		gfHavePurchasedItemsFromTony = FALSE;

BOOLEAN		gfDealerHasSaidTheEvaluateQuoteOnceThisSession=FALSE;
BOOLEAN		gfAlreadySaidTooMuchToRepair = FALSE;
UINT32		guiRandomQuoteDelayTime=SKI_DEALERS_RANDOM_QUOTE_DELAY;

//Index for the shopkeepers face
INT32			giShopKeeperFaceIndex;

//Id for the popup box
INT32			giPopUpBoxId=-1;

BOOLEAN		gfIsTheShopKeeperTalking;

#ifdef JA2UB
UINT8		gubDisplayMsgBoxAskingUserToAttachTransmitter=0; // Ja25 UB
#endif

// the glow for unwanted items
BOOLEAN fDeltaColorForShopkeepUnwanted = TRUE;
INT8 bShopKeepGlowIndex = 9;
BOOLEAN fDoShopkeepGlow = FALSE;

BOOLEAN		gfRemindedPlayerToPickUpHisStuff = FALSE;

BOOLEAN		gfDoneBusinessThisSession = FALSE;

// this is used within SKI exclusively, to handle small faces
UINT8			gubArrayOfEmployedMercs[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
UINT32		guiSmallSoldiersFace[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
UINT8			gubNumberMercsInArray;

//The subutitled text for what the merc is saying
CHAR16		gsShopKeeperTalkingText[ SKI_SUBTITLE_TEXT_SIZE ];

UINT16		gusPositionOfSubTitlesX=0;

BOOLEAN	gfExitSKIDueToMessageBox=FALSE;

OBJECTTYPE	*pShopKeeperItemDescObject=NULL;

#ifdef JA2UB
BOOLEAN	gfCanSayMakeTransactionQuote; //ja25 UB
#endif

UINT32	guiNextFreeInvSlot;

BOOLEAN gfStartWithRepairsDelayedQuote = FALSE;

BOOLEAN gfPerformTransactionInProgress = FALSE;

BOOLEAN gfCommonQuoteUsedThisSession[ NUM_COMMON_SK_QUOTES ];

extern		SOLDIERTYPE			*gpSMCurrentMerc;
extern		SOLDIERTYPE			*gpItemDescSoldier;
extern		MOUSE_REGION		gItemDescAttachmentRegions[MAX_ATTACHMENTS];
extern		MOUSE_REGION		gInvDesc;
extern		BOOLEAN					gfSMDisableForItems;
extern		OBJECTTYPE			*gpItemDescObject;
extern		OBJECTTYPE			*gpItemDescPrevObject;
extern		OBJECTTYPE			*gpItemDescPrevObject;
extern		void						HandleShortCutExitState( void );
extern		UINT8						gubSelectSMPanelToMerc;
extern		INT32						giItemDescAmmoButton;

extern		BOOLEAN BltVSurfaceUsingDD( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, UINT32 fBltFlags, INT32 iDestX, INT32 iDestY, RECT *SrcRect );

extern		UINT8 gubLastSpecialItemAddedAtElement;

// Enums for possible evaluation results
enum
{
	EVAL_RESULT_NORMAL,
	EVAL_RESULT_OK_BUT_REALLY_DAMAGED,
	EVAL_RESULT_DONT_HANDLE,
	EVAL_RESULT_WORTHLESS,
	EVAL_RESULT_NOT_DAMAGED,
	EVAL_RESULT_NON_REPAIRABLE,
	EVAL_RESULT_ROCKET_RIFLE,

	NUM_EVAL_RESULTS
};

BOOLEAN gfEvalResultQuoteSaid[ NUM_EVAL_RESULTS ];

UINT32	guiLastTimeDealerSaidNormalEvaluationQuote = 0;

BOOLEAN	gfSkiDisplayDropItemToGroundText = FALSE;

class ITEM_TO_ADD_AFTER_SKI_OPEN
{
public:
	ITEM_TO_ADD_AFTER_SKI_OPEN() {initialize();};
	void			initialize() {fActive = FALSE; ItemObject.initialize();};
	BOOLEAN			fActive;
	OBJECTTYPE		ItemObject;
};
ITEM_TO_ADD_AFTER_SKI_OPEN gItemToAdd;

//Page up buttons for the merchants
void		BtnSKI_InvPageUpButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiSKI_InvPageUpButton;
INT32		guiSKI_InvPageUpButtonImage;

//Page down buttons for the merchants
void		BtnSKI_InvPageDownButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiSKI_InvPageDownButton;
INT32		guiSKI_InvPageDownButtonImage;

//Transaction buttons
void		BtnSKI_TransactionButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiSKI_TransactionButton;
INT32		guiSKI_TransactionButtonImage;

//Done buttons
void		BtnSKI_DoneButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiSKI_DoneButton;
INT32		guiSKI_DoneButtonImage;

UINT32 guiItemCrossOut;
UINT32 guiMainTradeScreenImage = -1;

BOOLEAN gfDisplayNoRoomMsg = FALSE;

//Blanket the entire screen
MOUSE_REGION		gSKI_EntireScreenMouseRegions;

MOUSE_REGION		gDealersInventoryMouseRegions[ SKI_NUM_ARMS_DEALERS_INV_SLOTS ];
MOUSE_REGION		gRepairmanInventorySmallFaceMouseRegions[ SKI_NUM_ARMS_DEALERS_INV_SLOTS ];
int					gDealersInventoryActiveRegions = 0;
void SelectDealersInventoryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SelectDealersInventoryMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

MOUSE_REGION		gDealersOfferSlotsMouseRegions[ SKI_NUM_TRADING_INV_SLOTS ];
MOUSE_REGION		gDealersOfferSlotsSmallFaceMouseRegions[SKI_NUM_TRADING_INV_SLOTS];
int					gDealersOfferActiveRegions = 0;
void SelectDealersOfferSlotsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SelectDealersOfferSlotsMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

MOUSE_REGION		gPlayersOfferSlotsMouseRegions[ SKI_NUM_TRADING_INV_SLOTS ];
MOUSE_REGION		gPlayersOfferSlotsSmallFaceMouseRegions[SKI_NUM_TRADING_INV_SLOTS];
int					gPlayersOfferActiveRegions = 0;
void SelectPlayersOfferSlotsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SelectPlayersOfferSlotsMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

MOUSE_REGION		gSkiInventoryMovementAreaMouseRegions;

//Mouse region for the subtitles region when the merc is talking
MOUSE_REGION		gShopKeeperSubTitleMouseRegion;
void ShopKeeperSubTitleRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

MOUSE_REGION		gArmsDealersFaceMouseRegions;
void SelectArmsDealersFaceRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


//Region to allow the user to drop items to the ground
MOUSE_REGION		gArmsDealersDropItemToGroundMouseRegions;
void SelectArmsDealersDropItemToGroundRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SelectArmsDealersDropItemToGroundMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

BOOLEAN		EnterShopKeeperInterface();
BOOLEAN		ExitShopKeeperInterface();
void			HandleShopKeeperInterface();
BOOLEAN		RenderShopKeeperInterface();
void			GetShopKeeperInterfaceUserInput();
void			DestroySkiInventorySlotMouseRegions( );
void			CreateSkiInventorySlotMouseRegions( );
void			InitializeShopKeeper( BOOLEAN fResetPage );
void			CalculateFirstItemIndexOnPage( );
void			DisplayArmsDealerCurrentInventoryPage( );
BOOLEAN		DetermineArmsDealersSellingInventory( );
void			StoreObjectsInNextFreeDealerInvSlot( DEALER_SPECIAL_ITEM* pSpecial, std::vector<INVENTORY_IN_SLOT>& pInventory, UINT8 ubOwner );
BOOLEAN		RepairIsDone(DEALER_SPECIAL_ITEM* pSpecial);

UINT32		DisplayInvSlot( UINT16 ubSlotNum, UINT16 usItemIndex, UINT16 usPosX, UINT16 usPosY, OBJECTTYPE	*ItemObject, BOOLEAN fHatchedOut, UINT8	ubItemArea );
void			DisplayArmsDealerOfferArea();
INT16			AddItemToArmsDealerOfferArea( INVENTORY_IN_SLOT* pInvSlot, INT16	bSlotIdInOtherLocation );
BOOLEAN		RemoveItemFromArmsDealerOfferArea( INT16	bSlotId, BOOLEAN fKeepItem );
void			SetSkiRegionHelpText( INVENTORY_IN_SLOT *pInv, MOUSE_REGION* pRegion, UINT8 ubScreenArea );
void			SetSkiFaceRegionHelpText( INVENTORY_IN_SLOT *pInv, MOUSE_REGION* pRegion, UINT8 ubScreenArea );
void			RestoreTacticalBackGround();
void			DisplayPlayersOfferArea();
UINT32		CalcShopKeeperItemPrice( BOOLEAN fDealerSelling, BOOLEAN fUnitPriceOnly, UINT16 usItemID, FLOAT dModifier, OBJECTTYPE	*pItemObject );
FLOAT			ItemConditionModifier(UINT16 usItemIndex, INT16 bStatus);
BOOLEAN		RemoveItemFromPlayersOfferArea( INT16 bSlotIdInOtherLocation );

UINT32		CalculateTotalArmsDealerCost();
UINT32		CalculateTotalPlayersValue();
void			MoveAllArmsDealersItemsInOfferAreaToPlayersOfferArea( );
void			PerformTransaction( UINT32 uiMoneyFromPlayersAccount );
void			MovePlayerOfferedItemsOfValueToArmsDealersInventory( );
INT16			AddInventoryToSkiLocation( INVENTORY_IN_SLOT *pInv, UINT16 ubSpotLocation, UINT8 ubWhere );
BOOLEAN		RemoveItemFromDealersInventory( INVENTORY_IN_SLOT* pInvSlot, UINT16 ubSlot );
BOOLEAN			InitShopKeepersFace( UINT8 usProfileID );
void			DisplayTalkingArmsDealer();
BOOLEAN		StartShopKeeperTalking( UINT16 usQuoteNum );
void			HandleShopKeeperDialog( UINT8 ubInit );
BOOLEAN		IsGunOrAmmoOfSameTypeSelected( OBJECTTYPE	*pItemObject );
void			RemoveShopKeeperSubTitledText();
BOOLEAN		AreThereItemsInTheArmsDealersOfferArea( );
BOOLEAN		AreThereItemsInThePlayersOfferArea( );
void			ShutUpShopKeeper();
int				CountNumberOfValuelessItemsInThePlayersOfferArea( );
int				CountNumberOfItemsInThePlayersOfferArea( );
int				CountNumberOfItemsInTheArmsDealersOfferArea( );
INT8			GetSlotNumberForMerc( UINT8 ubProfile );
void			HandleCurrentModeText( UINT8 ubMode );
void			EnableDisableDealersInventoryPageButtons();
void			EnableDisableEvaluateAndTransactionButtons();
BOOLEAN		IsMoneyTheOnlyItemInThePlayersOfferArea( );

UINT32		CalculateHowMuchMoneyIsInPlayersOfferArea( );
void			MovePlayersItemsToBeRepairedToArmsDealersInventory();
BOOLEAN		RemoveRepairItemFromDealersOfferArea( INT16	bSlot );

INT8			GetInvSlotOfUnfullMoneyInMercInventory( SOLDIERTYPE *pSoldier );
void			ClearPlayersOfferSlot( INT32 ubSlotToClear );
void			ClearArmsDealerOfferSlot( INT32 ubSlotToClear );

BOOLEAN			DoSkiMessageBox( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT8 ubFlags, MSGBOX_CALLBACK ReturnCallback );

BOOLEAN		WillShopKeeperRejectObjectsFromPlayer( INT8 bDealerId, INT8 bSlotId );
void			CheckAndHandleClearingOfPlayerOfferArea( void );
BOOLEAN		CanShopkeeperOverrideDialogue( void );
INT16			GetNumberOfItemsInPlayerOfferArea( void );
void			HandleCheckIfEnoughOnTheTable( void );
void			InitShopKeeperItemDescBox( OBJECTTYPE *pObject, UINT16 ubPocket, UINT8	ubFromLocation );
void			StartSKIDescriptionBox( void );

BOOLEAN		ShopkeeperAutoPlaceObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj, BOOLEAN fNewItem );
void			ShopkeeperAddItemToPool( INT32 sGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel );

void			IfMercOwnedCopyItemToMercInv( INVENTORY_IN_SLOT *pInv );
void			IfMercOwnedRemoveItemFromMercInv( INVENTORY_IN_SLOT *pInv );
void			IfMercOwnedRemoveItemFromMercInv2( UINT8 ubOwnerProfileId, INT16 bOwnerSlotId );

void			SplitComplexObjectIntoSubObjects( OBJECTTYPE *pComplexObject );
void			CountSubObjectsInObject( OBJECTTYPE *pComplexObject, UINT8 *pubRepairableSubObjects, UINT8 *pubNonRepairableSubObjects );
BOOLEAN		AddObjectForEvaluation(OBJECTTYPE *pObject, UINT8 ubOwnerProfileId, INT16 bOwnerSlotId, BOOLEAN fFirstOne );
BOOLEAN		OfferObjectToDealer( OBJECTTYPE *pComplexObject, UINT8 ubOwnerProfileId, INT16 bOwnerSlotId );

BOOLEAN		SKITryToReturnInvToOwnerOrCurrentMerc( INVENTORY_IN_SLOT *pInv );
BOOLEAN		SKITryToAddInvToMercsInventory( INVENTORY_IN_SLOT *pInv, SOLDIERTYPE *pSoldier );

void			ExitSKIRequested();
void			EvaluateItemAddedToPlayersOfferArea( INT8 bSlotID, BOOLEAN fFirstOne );
void			ResetAllQuoteSaidFlags();

INVENTORY_IN_SLOT	*GetPtrToOfferSlotWhereThisItemIs( UINT8 ubProfileID, INT16 bInvPocket );

void			DealWithItemsStillOnTheTable();
void			ReturnItemToPlayerSomehow( INVENTORY_IN_SLOT *pInvSlot, SOLDIERTYPE *pDropSoldier );
void			GivePlayerSomeChange( UINT32 uiAmount );

void			HandlePossibleRepairDelays();
BOOLEAN		RepairmanFixingAnyItemsThatShouldBeDoneNow( UINT32 *puiHoursSinceOldestItemRepaired );
void			DelayRepairsInProgressBy( UINT32 uiMinutesDelayed );

BOOLEAN		CanTheDropItemToGroundStringBeDisplayed();
void			DisplayTheSkiDropItemToGroundString();

UINT32		EvaluateInvSlot( INVENTORY_IN_SLOT *pInvSlot );

void			BuildItemHelpTextString( CHAR16 sString[], INVENTORY_IN_SLOT *pInv, UINT8 ubScreenArea );
void			BuildRepairTimeString( CHAR16 sString[], UINT32 uiTimeInMinutesToFixItem );
void			BuildDoneWhenTimeString( CHAR16 sString[], UINT8 ubArmsDealer, INVENTORY_IN_SLOT* pObject );

void DisableAllDealersInventorySlots( void );
void EnableAllDealersInventorySlots( void );
void DisableAllDealersOfferSlots( void );
void EnableAllDealersOfferSlots( void );

void HatchOutInvSlot( UINT16 usPosX, UINT16 usPosY );

extern BOOLEAN ItemIsARocketRifle( INT16 sItemIndex );

#ifdef JA2TESTVERSION
BOOLEAN gfTestDisplayDealerCash = FALSE;
void DisplayAllDealersCash();
#endif;

void INVENTORY_IN_SLOT::initialize()
{
	ItemObject.initialize();
	fActive = 0;
	sItemIndex = 0;
	uiFlags = 0;
	ubLocationOfObject = 0;
	bSlotIdInOtherLocation = 0;
	ubIdOfMercWhoOwnsTheItem = 0;
	uiItemPrice = 0;
	uiRepairDoneTime = 0;
}

INVENTORY_IN_SLOT& INVENTORY_IN_SLOT::operator=(OLD_INVENTORY_IN_SLOT_101& src)
{
	this->ItemObject = src.oldItemObject;
	this->fActive = src.fActive;
	this->sItemIndex = src.sItemIndex;
	this->uiFlags = src.uiFlags;
	this->ubLocationOfObject = src.ubLocationOfObject;
	this->bSlotIdInOtherLocation = src.bSlotIdInOtherLocation;
	this->ubIdOfMercWhoOwnsTheItem = src.ubIdOfMercWhoOwnsTheItem;
	this->uiItemPrice = src.uiItemPrice;
	this->uiRepairDoneTime = 0;
	return *this;
}

//ppp

//
// screen handler functions
//

UINT32	ShopKeeperScreenInit()
{
	//Set so next time we come in, we can set up
	gfSKIScreenEntry = TRUE;
	
	return( TRUE );
}

UINT32	ShopKeeperScreenHandle()
{
	StartFrameBufferRender();

	if( gfSKIScreenEntry )
	{
		PauseGame();

		if( !EnterShopKeeperInterface() )
		{
			gfSKIScreenExit = TRUE;
			EnterTacticalScreen( );
			//ADB if we fail, unpause the game
			UnPauseGame();

			return( SHOPKEEPER_SCREEN );
		}
		else
		{
			gfSKIScreenEntry = FALSE;
			gfSKIScreenExit = FALSE;
		}
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
		gfRenderScreenOnNextLoop = TRUE;

		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	if( gfRenderScreenOnNextLoop )
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

	RestoreBackgroundRects();

	GetShopKeeperInterfaceUserInput();
	
	// Check for any newly added items...
	if ( gpSMCurrentMerc->flags.fCheckForNewlyAddedItems )
	{
		// Startup any newly added items....
		CheckForAnyNewlyAddedItems( gpSMCurrentMerc );
		gpSMCurrentMerc->flags.fCheckForNewlyAddedItems = FALSE;
	}

	HandleShopKeeperInterface();
	
	if( gubSkiDirtyLevel == SKI_DIRTY_LEVEL2 )
	{
		RenderShopKeeperInterface();

		fInterfacePanelDirty = DIRTYLEVEL2;

		gubSkiDirtyLevel = SKI_DIRTY_LEVEL0;
	}
	else if( gubSkiDirtyLevel == SKI_DIRTY_LEVEL1 )
	{
		fInterfacePanelDirty = DIRTYLEVEL2;

		gubSkiDirtyLevel = SKI_DIRTY_LEVEL0;
	}

	// render buttons marked dirty
	DisableSMPpanelButtonsWhenInShopKeeperInterface( FALSE );

	RenderButtons( );
	
	//CHRISL: If we put this function call here, we overwrite the bullet graphic.  I don't know why.  I just know it happens.
	//RenderItemDescriptionBox( );
	
	// render help
	SaveBackgroundRects( );
	RenderButtonsFastHelp( );
	
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	if( gfSKIScreenExit )
	{
		ExitShopKeeperInterface();
		gfSKIScreenExit = FALSE;
		gfSKIScreenEntry = TRUE;
		EnterTacticalScreen( );
		UnPauseGame();
	}

	if ( gfDisplayNoRoomMsg )
	{
		// tell player there's not enough room in the player's offer area
		// ARM: message is delayed because we need the mouse restriction to be in place BEFORE it comes up so it gets lifted/restored
		DoSkiMessageBox( MSG_BOX_BASIC_STYLE, SKI_Text[ SKI_TEXT_NO_MORE_ROOM_IN_PLAYER_OFFER_AREA ], SHOPKEEPER_SCREEN, MSG_BOX_FLAG_OK, NULL );

		gfDisplayNoRoomMsg = FALSE;
	}
	
	return( SHOPKEEPER_SCREEN );
}

UINT32	ShopKeeperScreenShutdown()
{
	ShutDownArmsDealers();

	return( TRUE );
}

BOOLEAN EnterShopKeeperInterface()
{
	VOBJECT_DESC		VObjectDesc;
	UINT16				ubCnt;
	CHAR8				zTemp[32];
	VSURFACE_DESC		vs_desc;

	//ADB if we are here, we must be able to talk with an extended ear (CheckIfRadioIsEquipped())
	//but if we are physically too far away, we don't have extended arms!

	SOLDIERTYPE* pSoldier = MercPtrs[ gusSelectedSoldier ];
	SOLDIERTYPE* pShopkeeper = NULL;
	
	if ( gusIDOfCivTrader != NOBODY )
		pShopkeeper = MercPtrs[gusIDOfCivTrader];
	else
		pShopkeeper = FindSoldierByProfileID( armsDealerInfo[gbSelectedArmsDealerID].ubShopKeeperID, FALSE );

	if ( GetRangeFromGridNoDiff( pSoldier->sGridNo, pShopkeeper->sGridNo ) > NPC_TALK_RADIUS )
	{
		//so now we know we are too far away to trade, so instead of just quitting,
		//either post a message or run to the guy like HandleTalkInit does
		if( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"Unable to interact with shopkeeper during combat" );
			return( FALSE );
		}
		else
		{
			// First calculate APs and validate...
			// Check AP cost...
			if ( !EnoughPoints( pSoldier, APBPConstants[AP_TALK], 0, TRUE ) )
			{
				return( FALSE );
			}

			// First get an adjacent gridno....
			UINT8 ubDirection;
			INT32 sActionGridNo =  FindAdjacentGridEx( pSoldier, pShopkeeper->sGridNo, &ubDirection, NULL, FALSE, TRUE );

			if ( sActionGridNo == -1 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
				return( FALSE );
			}

			if ( UIPlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints ) == 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ NO_PATH ] );
				return( FALSE );
			}

			// Walk up and talk to buddy....
			gfNPCCircularDistLimit = TRUE;
			INT32 sGoodGridNo = FindGridNoFromSweetSpotWithStructData( pSoldier, pSoldier->usUIMovementMode, pShopkeeper->sGridNo, (NPC_TALK_RADIUS-1), &ubDirection, TRUE );
			gfNPCCircularDistLimit = FALSE;

			// Now walkup to talk....
			pSoldier->aiData.ubPendingAction = MERC_TALK;
			pSoldier->aiData.uiPendingActionData1 = pShopkeeper->ubID;
			pSoldier->aiData.ubPendingActionAnimCount = 0;

			// WALK UP TO DEST FIRST
			pSoldier->EVENT_InternalGetNewSoldierPath(sGoodGridNo, pSoldier->usUIMovementMode , TRUE , pSoldier->flags.fNoAPToFinishMove );

			return( FALSE );
		}
	}

	// make sure current merc is close enough and eligible to talk to the shopkeeper.
	AssertMsg( CanMercInteractWithSelectedShopkeeper( MercPtrs[ gusSelectedSoldier ] ), "Selected merc can't interact with shopkeeper.  Send save AM-1");

	// Create a video surface to blt corner of the tactical screen that still shines through
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	//Heinz: 22.02.09 BUGFIX: best way - to update whole tactical screen around SKI
	//vs_desc.usWidth = SKI_TACTICAL_BACKGROUND_START_WIDTH;
	//vs_desc.usHeight = SKI_TACTICAL_BACKGROUND_START_HEIGHT;
	vs_desc.usWidth = SCREEN_WIDTH;
	vs_desc.usHeight = SCREEN_HEIGHT;

	vs_desc.ubBitDepth = 16;
	if( !AddVideoSurface( &vs_desc, &guiCornerWhereTacticalIsStillSeenImage) )
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Failed to create Surface where tactical map shows through" );
		#endif

		return( FALSE );
	}
	
	//Heinz: 22.02.09 shadowing of tactical screen for further using as a background 
	ShadowVideoSurfaceRect( guiCornerWhereTacticalIsStillSeenImage, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	
	//Clear out all the save background rects
	EmptyBackgroundRects( );

	ShadowVideoSurfaceRect( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - INV_INTERFACE_HEIGHT );

	if( gfExitSKIDueToMessageBox )
	{
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
		gfExitSKIDueToMessageBox = FALSE;
	}

	// silversurfer: added this to prevent random crashes when the temp vector wasn't initialized properly.
	// This is called in InitializeShopKeeper() a few lines below anyway so it shouldn't hurt here.
	gpTempDealersInventory.clear();

	//Check to make sure the inventory is null ( should always be null if we are just coming in to the SKI )
	Assert( gpTempDealersInventory.empty() == true );

	//Reinitialize the team panel to be the SM panel
	SetCurrentInterfacePanel( SM_PANEL );
	SetCurrentTacticalPanelCurrentMerc( (UINT8)gusSelectedSoldier );
	SetSMPanelCurrentMerc( (UINT8)gusSelectedSoldier );
	
	// load the Main trade screen background image
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("InterFace\\itemcrossout.sti", VObjectDesc.ImageFile);
	if( !AddVideoObject(&VObjectDesc, &guiItemCrossOut ))
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Failed to load itemcrossout.sti" );
#endif
		return( FALSE );
	}
	
	//Create an array of all mercs (anywhere!) currently in the player's employ, and load their small faces
	// This is to support showing of repair item owner's faces even when they're not in the sector, as long as they still work for player
	gubNumberMercsInArray = 0;
	for( ubCnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; ubCnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++ubCnt )
	{
		pSoldier = MercPtrs[ ubCnt ];

		if( pSoldier->bActive && ( pSoldier->ubProfile != NO_PROFILE ) &&
			!(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) )
		{
			// remember whose face is in this slot
			gubArrayOfEmployedMercs[ gubNumberMercsInArray ] = pSoldier->ubProfile;

			//Create the string for the face file name						
			if ( gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_IMP )
			{
				sprintf( zTemp, "IMPFACES\\33FACE\\%02d.sti", gMercProfiles[ pSoldier->ubProfile ].ubFaceIndex );	
			} 
			else
			{			
				sprintf( zTemp, "FACES\\33FACE\\%02d.sti", gMercProfiles[ pSoldier->ubProfile ].ubFaceIndex );
			}
			
			//While we are at it, add their small face
			VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
			FilenameForBPP( zTemp, VObjectDesc.ImageFile);
			if(!AddVideoObject(&VObjectDesc, &guiSmallSoldiersFace[ gubNumberMercsInArray ] ))
			{
				#ifdef JA2BETAVERSION
						ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Failed to load %s", zTemp );
				#endif
				return( FALSE );
			}

			++gubNumberMercsInArray;
		}
	}

	//Load the graphic for the arrow button
	guiSKI_InvPageUpButtonImage = LoadButtonImage("INTERFACE\\TradeScrollArrows.sti", -1,0,-1,1,-1 );
	guiSKI_InvPageDownButtonImage = UseLoadedButtonImage( guiSKI_InvPageUpButtonImage, -1,2,-1,3,-1 );
	
	//Page up button for the merchant inventory
	guiSKI_InvPageUpButton = QuickCreateButton( guiSKI_InvPageUpButtonImage, SKI_PAGE_UP_ARROWS_X, SKI_PAGE_UP_ARROWS_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGHEST,
										DEFAULT_MOVE_CALLBACK, BtnSKI_InvPageUpButtonCallback );
	SpecifyDisabledButtonStyle( guiSKI_InvPageUpButton, DISABLED_STYLE_HATCHED );

	//Page down button for the merchant inventory
	guiSKI_InvPageDownButton = QuickCreateButton( guiSKI_InvPageDownButtonImage, SKI_PAGE_DOWN_ARROWS_X, SKI_PAGE_DOWN_ARROWS_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGHEST,
										DEFAULT_MOVE_CALLBACK, BtnSKI_InvPageDownButtonCallback );
	SpecifyDisabledButtonStyle( guiSKI_InvPageDownButton, DISABLED_STYLE_HATCHED );
	
	EnableDisableShopkeeperButtons(SHOPKEEPER_SCREEN, ACTIVATE_BUTTON);
	
	//Blanket the entire screen
	MSYS_DefineRegion( &gSKI_EntireScreenMouseRegions, 0, 0, (SCREEN_WIDTH - 1), (SCREEN_HEIGHT - INV_INTERFACE_HEIGHT), MSYS_PRIORITY_HIGH-2,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK);
	MSYS_AddRegion( &gSKI_EntireScreenMouseRegions );
	
	//Create the mouse regions for the inventory slot
	CreateSkiInventorySlotMouseRegions( );
	
	//Create the mouse region to limit the movement of the item cursos
	MSYS_DefineRegion( &gSkiInventoryMovementAreaMouseRegions, SKI_ITEM_MOVEMENT_AREA_X, SKI_ITEM_MOVEMENT_AREA_Y, (UINT16)(SKI_ITEM_MOVEMENT_AREA_X+SKI_ITEM_MOVEMENT_AREA_WIDTH), (UINT16)(SKI_ITEM_MOVEMENT_AREA_Y+SKI_ITEM_MOVEMENT_AREA_HEIGHT), MSYS_PRIORITY_HIGH-1,
				CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK ); //SelectSkiInventoryMovementAreaRegionCallBack
	MSYS_AddRegion( &gSkiInventoryMovementAreaMouseRegions );

	//Disable the region that limits the movement of the cursor with the item
	MSYS_DisableRegion( &gSkiInventoryMovementAreaMouseRegions );
	
	//Create the mouse region for the shopkeeper's face
	MSYS_DefineRegion( &gArmsDealersFaceMouseRegions, SKI_FACE_X, SKI_FACE_Y, (UINT16)(SKI_FACE_X+SKI_FACE_WIDTH), (UINT16)(SKI_FACE_Y+SKI_FACE_HEIGHT), MSYS_PRIORITY_HIGH-1,
				CURSOR_NORMAL, MSYS_NO_CALLBACK, SelectArmsDealersFaceRegionCallBack );
	MSYS_AddRegion( &gArmsDealersFaceMouseRegions );
		
	for (int x = 0; x < SKI_NUM_TRADING_INV_SLOTS; ++x)
	{
		ArmsDealerOfferArea[x].initialize();
		PlayersOfferArea[x].initialize();
	}

	if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
	{
		HandlePossibleRepairDelays();
	}

	//Setup the currently selected arms dealer
	InitializeShopKeeper( TRUE );
	
	//Set the flag indicating that we are in the shop keeper interface
	guiTacticalInterfaceFlags |= INTERFACE_SHOPKEEP_INTERFACE;

	gMoveingItem.initialize();

	memset( &gfCommonQuoteUsedThisSession, FALSE, sizeof( gfCommonQuoteUsedThisSession ) );

	//Init the shopkeepers face
	InitShopKeepersFace( armsDealerInfo[ gbSelectedArmsDealerID ].ubShopKeeperID );

	gfDoneBusinessThisSession = FALSE;

	//Call this to set the fact that we just entered the screen
	HandleShopKeeperDialog( 1 );

	ResetAllQuoteSaidFlags();

	//Reset the highlighted item pointer;
	gpHighLightedItemObject = NULL;

	//Reset
	gfRemindedPlayerToPickUpHisStuff = FALSE;
	gfUserHasRequestedToLeave = FALSE;
	gfDisplayNoRoomMsg = FALSE;

	//Disable the map radar region
	MSYS_DisableRegion(&gRadarRegion);

	gfDoEvaluationAfterOpening = FALSE;

	if( gItemToAdd.fActive )
	{
		bool fAddedOK = false;
		//if this is NOT a repair dealer or he is is but there is enough space in the player's offer area
		// (you can't be out of space if it isn't a repairman, only they can fill it up with repaired items!)
		if( ( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS ) ||
			(CountNumberOfItemsInThePlayersOfferArea( ) < (SKI_TRADER_INVENTORY_BOXES_PER_ROW * SKI_TRADER_INVENTORY_BOXES_PER_COL)) )
		{
			if ( OfferObjectToDealer( &(gItemToAdd.ItemObject), gpSMCurrentMerc->ubProfile, NO_SLOT ) )
			{
				fAddedOK = true;
			}
		}

		if ( fAddedOK == true )
		{
			// evaluate it
			gfDoEvaluationAfterOpening = TRUE;
		}
		else
		{
			//add the item back to the current PC into the slot it came from
			//ADB screw slot, slot is (used to be, before it was deleted) only ever, so autoplace it
			AutoPlaceObject(&Menptr[ gpSMCurrentMerc->ubID ], &gItemToAdd.ItemObject, FALSE);
		}

		//Clear the contents of the structure
		gItemToAdd.initialize();
	}

	// Dirty the bottom panel
	fInterfacePanelDirty = DIRTYLEVEL2;

	gfDealerHasSaidTheEvaluateQuoteOnceThisSession = FALSE;
	guiRandomQuoteDelayTime = SKI_DEALERS_RANDOM_QUOTE_DELAY;

	pShopKeeperItemDescObject = NULL;
	
//Region to allow the user to drop items to the ground
	MSYS_DefineRegion( &gArmsDealersDropItemToGroundMouseRegions, SKI_DROP_ITEM_TO_GROUND_START_X, SKI_DROP_ITEM_TO_GROUND_START_Y, SCREEN_WIDTH - 1, SCREEN_HEIGHT - INV_INTERFACE_HEIGHT, MSYS_PRIORITY_HIGH,
						 CURSOR_NORMAL, SelectArmsDealersDropItemToGroundMovementRegionCallBack, SelectArmsDealersDropItemToGroundRegionCallBack );
//						 CURSOR_NORMAL, MSYS_NO_CALLBACK, SelectArmsDealersDropItemToGroundRegionCallBack );
	MSYS_AddRegion( &gArmsDealersDropItemToGroundMouseRegions );

	gfSkiDisplayDropItemToGroundText = FALSE;

	// by default re-enable calls to PerformTransaction()
	gfPerformTransactionInProgress = FALSE;
#ifdef JA2UB	
	//JA25 UB
	//if the dealer is RAUL
	if( gbSelectedArmsDealerID == ARMS_DEALER_RAUL )
	{
		//set the fact the raul refreshed his inventory
		SetFactFalse( FACT_RAULS_INVENTORY_CHANGED_SINCE_LAST_VISIT );
	}
#endif
	return( TRUE );
}

BOOLEAN InitShopKeepersFace( UINT8 usProfileID )
{
	SOLDIERTYPE *pSoldier = NULL;

	// if this is a non-NPC, don't set this up
	if ( gusIDOfCivTrader == NOBODY )
		pSoldier = FindSoldierByProfileID( armsDealerInfo[gbSelectedArmsDealerID].ubShopKeeperID, FALSE );

	if( pSoldier == NULL )
	{
		//Create the facial index
		giShopKeeperFaceIndex = InitFace( usProfileID, NOBODY, FACE_BIGFACE );
	}
	else
	{
		//Create the facial index
		giShopKeeperFaceIndex = InitFace( usProfileID, pSoldier->ubID, FACE_BIGFACE );
	}

	SetAutoFaceActive( FRAME_BUFFER, FACE_AUTO_RESTORE_BUFFER, giShopKeeperFaceIndex, SKI_FACE_X, SKI_FACE_Y );

	//Set it so the face cannot be set InActive
	if ( giShopKeeperFaceIndex > -1 )
		gFacesData[ giShopKeeperFaceIndex ].uiFlags |= FACE_INACTIVE_HANDLED_ELSEWHERE;
	
	RenderAutoFace( giShopKeeperFaceIndex );

	return(TRUE);
}

BOOLEAN ExitShopKeeperInterface()
{
	if( gfExitSKIDueToMessageBox )
	{
		gfSKIScreenExit = FALSE;

//		gfExitSKIDueToMessageBox = FALSE;
	}

	if( InItemDescriptionBox( ) && pShopKeeperItemDescObject != NULL )
	{
		DeleteItemDescriptionBox( );
	}

	FreeMouseCursor( TRUE );

	//Delete the main shopkeep background
	DeleteVideoObjectFromIndex( guiItemCrossOut );
	DeleteVideoSurfaceFromIndex( guiCornerWhereTacticalIsStillSeenImage );

	if ( guiMainTradeScreenImage != -1 )
	{
		DeleteVideoSurfaceFromIndex( guiMainTradeScreenImage );
		guiMainTradeScreenImage = -1;
	}

	ShutUpShopKeeper();
	
	UnloadButtonImage( guiSKI_InvPageUpButtonImage );
	UnloadButtonImage( guiSKI_InvPageDownButtonImage );

	//loop through the area and delete small faces
	for (UINT8 ubCnt=0; ubCnt<gubNumberMercsInArray; ++ubCnt)
	{
		DeleteVideoObjectFromIndex( guiSmallSoldiersFace[ ubCnt ] );
	}

	RemoveButton( guiSKI_InvPageUpButton );
	RemoveButton( guiSKI_InvPageDownButton );

	EnableDisableShopkeeperButtons(SHOPKEEPER_SCREEN, DEACTIVATE_BUTTON);

	MSYS_RemoveRegion( &gSKI_EntireScreenMouseRegions);

	MSYS_RemoveRegion( &gSkiInventoryMovementAreaMouseRegions );
	
	//Remove the region for the face
	MSYS_RemoveRegion( &gArmsDealersFaceMouseRegions );
	
	//Region to allow the user to drop items to the ground
	MSYS_RemoveRegion( &gArmsDealersDropItemToGroundMouseRegions );

	//Destroy the mouse regions for the inventory slots
	DestroySkiInventorySlotMouseRegions( );

	//if there is a temp inventory array, destroy it
	gpTempDealersInventory.clear();

	//Set the flag indicating that we are NOT in the shop keeper interface
	guiTacticalInterfaceFlags &= ~INTERFACE_SHOPKEEP_INTERFACE;

	//if there is a subtitles box up, remove it
	RemoveShopKeeperSubTitledText();

	//Get rid of the ShopKeeper face
	DeleteFace( giShopKeeperFaceIndex );

	//Enable the map region
	MSYS_EnableRegion(&gRadarRegion);

	gfSMDisableForItems = FALSE;
#ifdef JA2UB	
	//JA25 UB
	//Check to see if a merc should say something
	//CheckForValidQuotesWhenLeavingDealer( gTalkPanel.ubCharNum );

	//if the laptop was just fixed
	if( gubQuest[ QUEST_FIX_LAPTOP ] == QUESTDONE && !( gJa25SaveStruct.uiJa25GeneralFlags & JA_GF__PLAYER_SAID_LAPTOP_FIXED_QUOTE ) )
	{		
		//Have a new merc say a quote
		INT16 bSoldierID = RandomSoldierIdFromNewMercsOnPlayerTeam();
		if( bSoldierID > -1 )
		{
			TacticalCharacterDialogue( &Menptr[bSoldierID], QUOTE_JA2UB_LAPTOP_FIXED );
		}

		gJa25SaveStruct.uiJa25GeneralFlags |= JA_GF__PLAYER_SAID_LAPTOP_FIXED_QUOTE;
	}
#endif
	return( TRUE );
}

void HandleShopKeeperInterface()
{
	UINT8 ubStatusOfSkiRenderDirtyFlag = gubSkiDirtyLevel;

	INT32 iCounter = 0;

	//if we are in the item desc panel, disable the buttons
	if( InItemDescriptionBox( ) && pShopKeeperItemDescObject != NULL )
	{
		DisableSMPpanelButtonsWhenInShopKeeperInterface( FALSE );
		DisableButton( guiSKI_InvPageUpButton );
		DisableButton( guiSKI_InvPageDownButton );
		EnableDisableShopkeeperButtons(SHOPKEEPER_SCREEN, DISABLE_BUTTON);
		//DisableButton( guiSKI_TransactionButton );
		//DisableButton( guiSKI_DoneButton );

		//make sure the buttons dont render
//		ButtonList[ guiSKI_InvPageUpButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
//		ButtonList[ guiSKI_InvPageDownButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		//Heinz: 22.02.09 BUGFIX: buttons should render for items in inventory with EDB
		//ButtonList[ guiSKI_TransactionButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		//ButtonList[ guiSKI_DoneButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;


		// make sure the shop keeper doesn't start talking ( reset the timing variable )
		HandleShopKeeperDialog( 2 );

		return;
	}
	
	if( gubSkiDirtyLevel == SKI_DIRTY_LEVEL2 )
	{
		fInterfacePanelDirty = DIRTYLEVEL2;
	}

	RenderTacticalInterface( );

	if ( InItemStackPopup( ) )
	{
		RenderItemStackPopup( ( BOOLEAN )( fInterfacePanelDirty == DIRTYLEVEL2 ) );
	}

	// handle check if enough on the table
	HandleCheckIfEnoughOnTheTable( );

	// Render view window
	fInterfacePanelDirty = DIRTYLEVEL2;
	RenderRadarScreen( );

	if( fInterfacePanelDirty == DIRTYLEVEL2 )
	{
		fInterfacePanelDirty = DIRTYLEVEL0;
	}
		
	DisplayTalkingArmsDealer();

	MarkButtonsDirty( );

	if( gfDoEvaluationAfterOpening )
	{
		BOOLEAN fFirstOne = TRUE;

		gfDoEvaluationAfterOpening = FALSE;

		for ( iCounter = 0; iCounter < gPlayersOfferActiveRegions; ++iCounter )
		{
			if( PlayersOfferArea[ iCounter ].fActive )
			{
				EvaluateItemAddedToPlayersOfferArea( ( INT8 ) iCounter, fFirstOne );
				fFirstOne = FALSE;
			}
		}

		gfAlreadySaidTooMuchToRepair = FALSE;
	}

#ifdef JA2TESTVERSION
	if ( gfTestDisplayDealerCash )
		DisplayAllDealersCash();
#endif

	//if the Ski dirty flag was changed to a lower value, make sure it is set properly
	if( ubStatusOfSkiRenderDirtyFlag > gubSkiDirtyLevel )
		gubSkiDirtyLevel = ubStatusOfSkiRenderDirtyFlag;

	//if the merc is talking and there is an item currently being highlighted
	// ( this gets rid of the item burning through the dealers text box )
	if( gfIsTheShopKeeperTalking )
		//Heinz: 23.02.09 BUGFIX: if shopkeeper is talking we should render popup in all cases
		//if( gpHighLightedItemObject != NULL || gubSkiDirtyLevel != SKI_DIRTY_LEVEL0 )
				RenderMercPopUpBoxFromIndex( giPopUpBoxId, gusPositionOfSubTitlesX, SKI_POSITION_SUBTITLES_Y, FRAME_BUFFER);
	
	//if we are to display the drop item to ground text
	if( gfSkiDisplayDropItemToGroundText )
	{
		DisplayTheSkiDropItemToGroundString();
	}
#ifdef JA2UB
	//ja25 UB
	else if( gubDisplayMsgBoxAskingUserToAttachTransmitter != TRNSMTR_MSG_BOX__NONE )
	{
		if( gubDisplayMsgBoxAskingUserToAttachTransmitter == TRNSMTR_MSG_BOX__1_FRAME_WAIT )
		{
			gubDisplayMsgBoxAskingUserToAttachTransmitter = TRNSMTR_MSG_BOX__DISPLAY_BOX;
		}
		else if( gubDisplayMsgBoxAskingUserToAttachTransmitter == TRNSMTR_MSG_BOX__DISPLAY_BOX )
		{
			//Tell user that they are attaching the transmitter
			AskUserToAttachTransmitterToLaptop();

			//clear the flag
			gubDisplayMsgBoxAskingUserToAttachTransmitter = TRNSMTR_MSG_BOX__NONE;
		}
	}
#endif
}

// Internal function to draw shop keeper inventory boxes
void DrawShopKeeperInventoryBox( INT32 usPosX, INT32 usPosY )
{
	static const UINT16 col_itembox = Get16BPPColor( FROMRGB( 74, 65, 49 ) );
	static const UINT16 col_itemdescription = Get16BPPColor( FROMRGB( 181, 150, 74 ) );
	static const UINT16 col_itemstatusbar = Get16BPPColor( FROMRGB( 57, 56, 41 ) );

	// draw background for status bar
	ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX, usPosY, usPosX + 4, usPosY + 28, col_itemstatusbar );

	// draw background for picture of item, etc.
	ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX + 4, usPosY, usPosX + SKI_INV_SLOT_WIDTH, usPosY + 28, col_itembox );

	// draw background for money area
	ColorFillVideoSurfaceArea( FRAME_BUFFER, usPosX, usPosY + 28, usPosX + SKI_INV_SLOT_WIDTH, usPosY + SKI_INV_SLOT_HEIGHT, col_itemdescription );
}

BOOLEAN RenderShopKeeperInterface()
{
	CHAR16	zMoney[128];
	HVSURFACE hDestVSurface, hSrcVSurface;
	SGPRect		SrcRect;
	
	if( InItemDescriptionBox( ) && pShopKeeperItemDescObject != NULL )
	{
		return( TRUE );
	}
	
	// wipe everything once and then redraw
	if ( gubSkiDirtyLevel == SKI_DIRTY_LEVEL2 )
	{
		//Restore the area before blitting the new data on
		RestoreExternBackgroundRect( SKI_TOTAL_BEGIN_X, SKI_TOTAL_BEGIN_Y,
									 SKI_TOTAL_END_X - SKI_TOTAL_BEGIN_X,
									 SKI_TOTAL_END_Y - SKI_TOTAL_BEGIN_Y );
	}
		
	UINT16 col_tradescreenmargin	= Get16BPPColor( FROMRGB( 64, 64, 64 ) );
	UINT16 col_tradescreen			= Get16BPPColor( FROMRGB( 255, 0, 255 ) );

	// we need to do the colouring twice to colour the margin differently
	ColorFillVideoSurfaceArea( FRAME_BUFFER, SKI_TOTAL_BEGIN_X, SKI_TOTAL_BEGIN_Y,
							   SKI_TOTAL_END_X, SKI_TOTAL_END_Y,
							   col_tradescreenmargin );
				
	// Flugente: because people find simple plane backgrounds to be offputting, we try for something more fancy
	// we use a very huge background picture and then display parts of it. 
	// Given that the default picture has a size of 3088x2056, you would need a gigantic resolution for this to not work out.
	// Flugente: guiMainTradeScreenImage is unsigned as the functions require UINT32. As it is unlikely we will ever use 2^32 pictures, this works well enough
	if ( guiMainTradeScreenImage == -1 )
	{
		VSURFACE_DESC		vs_desc;
		vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
		strcpy( vs_desc.ImageFile, "LAPTOP\\shopkeeperbackground.pcx" );
		if ( FileExists( vs_desc.ImageFile ) )
		{
			AddVideoSurface( &vs_desc, &guiMainTradeScreenImage );
		}
	}

	if ( guiMainTradeScreenImage != -1 )
	{
		HVSURFACE hSrcVSurface;
		UINT32 uiDestPitchBYTES;
		UINT32 uiSrcPitchBYTES;
		UINT16	*pDestBuf;
		UINT8	*pSrcBuf;
		SGPRect clip;
						
		// get surfaces
		pDestBuf = (UINT16*)LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		GetVideoSurface( &hSrcVSurface, guiMainTradeScreenImage );
		pSrcBuf = LockVideoSurface( guiMainTradeScreenImage, &uiSrcPitchBYTES );
		
		// for our 3 regions we display only parts of the picture
		clip.iLeft = SKI_TRADER_INVENTORY_BEGIN_X + SKI_MARGIN;
		clip.iRight = SKI_TRADER_INVENTORY_END_X - SKI_MARGIN;
		clip.iTop = SKI_TRADER_INVENTORY_BEGIN_Y + SKI_MARGIN;
		clip.iBottom = SKI_TRADER_INVENTORY_END_Y - SKI_MARGIN;

		Blt8BPPDataSubTo16BPPBuffer( pDestBuf, uiDestPitchBYTES, hSrcVSurface, pSrcBuf, uiSrcPitchBYTES, SKI_TRADER_INVENTORY_BEGIN_X + SKI_MARGIN, SKI_TRADER_INVENTORY_BEGIN_Y + SKI_MARGIN, &clip );

		clip.iLeft = SKI_TRADER_OFFER_BEGIN_X + SKI_MARGIN;
		clip.iRight = SKI_TRADER_OFFER_END_X - SKI_MARGIN;
		clip.iTop = SKI_TRADER_OFFER_BEGIN_Y + SKI_MARGIN;
		clip.iBottom = SKI_TRADER_OFFER_END_Y - SKI_MARGIN;

		Blt8BPPDataSubTo16BPPBuffer( pDestBuf, uiDestPitchBYTES, hSrcVSurface, pSrcBuf, uiSrcPitchBYTES, SKI_TRADER_OFFER_BEGIN_X + SKI_MARGIN, SKI_TRADER_OFFER_BEGIN_Y + SKI_MARGIN, &clip );

		clip.iLeft = SKI_PLAYER_OFFER_BEGIN_X + SKI_MARGIN;
		clip.iRight = SKI_PLAYER_OFFER_END_X - SKI_MARGIN;
		clip.iTop = SKI_PLAYER_OFFER_BEGIN_Y + SKI_MARGIN;
		clip.iBottom = SKI_PLAYER_OFFER_END_Y - SKI_MARGIN;

		Blt8BPPDataSubTo16BPPBuffer( pDestBuf, uiDestPitchBYTES, hSrcVSurface, pSrcBuf, uiSrcPitchBYTES, SKI_PLAYER_OFFER_BEGIN_X + SKI_MARGIN, SKI_PLAYER_OFFER_BEGIN_Y + SKI_MARGIN, &clip );
		
		// release surfaces
		UnLockVideoSurface( guiMainTradeScreenImage );
		UnLockVideoSurface( FRAME_BUFFER );		
	}
	// if we cannot find the picture, use a fallback solution
	else
	{
		ColorFillVideoSurfaceArea( FRAME_BUFFER, SKI_TRADER_INVENTORY_BEGIN_X + SKI_MARGIN, SKI_TRADER_INVENTORY_BEGIN_Y + SKI_MARGIN,
								   SKI_TRADER_INVENTORY_END_X - SKI_MARGIN, SKI_TRADER_INVENTORY_END_Y - SKI_MARGIN,
								   col_tradescreen );

		ColorFillVideoSurfaceArea( FRAME_BUFFER, SKI_TRADER_OFFER_BEGIN_X + SKI_MARGIN, SKI_TRADER_OFFER_BEGIN_Y + SKI_MARGIN,
								   SKI_TRADER_OFFER_END_X - SKI_MARGIN, SKI_TRADER_OFFER_END_Y - SKI_MARGIN,
								   col_tradescreen );

		ColorFillVideoSurfaceArea( FRAME_BUFFER, SKI_PLAYER_OFFER_BEGIN_X + SKI_MARGIN, SKI_PLAYER_OFFER_BEGIN_Y + SKI_MARGIN,
								   SKI_PLAYER_OFFER_END_X - SKI_MARGIN, SKI_PLAYER_OFFER_END_Y - SKI_MARGIN,
								   col_tradescreen );
	}
	
	{
		int usPosY = SKI_TRADER_INVENTORY_BOXES_BEGIN_Y;
		for ( int y = 0; y<SKI_TRADER_INVENTORY_BOXES_PER_COL; ++y )
		{
			int usPosX = SKI_TRADER_INVENTORY_BOXES_BEGIN_X;

			for ( int x = 0; x<SKI_TRADER_INVENTORY_BOXES_PER_ROW; ++x )
			{
				DrawShopKeeperInventoryBox( usPosX, usPosY );

				usPosX += SKI_INV_OFFSET_X;
			}
			usPosY += SKI_INV_OFFSET_Y;
		}
	}

	{
		int usPosY = SKI_TRADER_OFFER_BOXES_BEGIN_Y;
		for ( int y = 0; y<SKI_TRADER_OFFER_BOXES_PER_COL; ++y )
		{
			int usPosX = SKI_TRADER_OFFER_BOXES_BEGIN_X;

			for ( int x = 0; x<SKI_TRADER_OFFER_BOXES_PER_ROW; ++x )
			{
				DrawShopKeeperInventoryBox( usPosX, usPosY );

				usPosX += SKI_INV_OFFSET_X;
			}
			usPosY += SKI_INV_OFFSET_Y;
		}
	}

	{
		int usPosY = SKI_PLAYER_OFFER_BOXES_BEGIN_Y;
		for ( int y = 0; y<SKI_PLAYER_OFFER_BOXES_PER_COL; ++y )
		{
			int usPosX = SKI_PLAYER_OFFER_BOXES_BEGIN_X;

			for ( int x = 0; x<SKI_PLAYER_OFFER_BOXES_PER_ROW; ++x )
			{
				DrawShopKeeperInventoryBox( usPosX, usPosY );

				usPosX += SKI_INV_OFFSET_X;
			}
			usPosY += SKI_INV_OFFSET_Y;
		}
	}

	if ( giShopKeeperFaceIndex > -1 )
		RenderAutoFace( giShopKeeperFaceIndex );
	// Flugente: if this is a non-NPC dealer, we instead show their body in their current animation
	else if ( gusIDOfCivTrader != NOBODY )
	{
		SOLDIERTYPE* pSoldier = MercPtrs[gusIDOfCivTrader];

		if ( pSoldier )
		{
			UINT32								uiPitch;

			// Lock video surface
			UINT16* pBuffer = (UINT16*)LockVideoSurface( FRAME_BUFFER, &uiPitch );

			if ( pBuffer != NULL )
			{
				UINT16 usAnimSurface = GetSoldierAnimationSurface( pSoldier, pSoldier->usAnimState );

				HVOBJECT hVObject = gAnimSurfaceDatabase[usAnimSurface].hVideoObject;

				ETRLEObject* pTrav = &(hVObject->pETRLEObject[pSoldier->usAniFrame]);

				INT32 sCenX = SKI_FACE_X + (INT16)(abs( (long)(SKI_FACE_WIDTH - pTrav->usWidth) ) / 2) - pTrav->sOffsetX;
				INT32 sCenY = SKI_FACE_Y + (INT16)(abs( (long)(SKI_FACE_HEIGHT - pTrav->usHeight) ) / 2) - pTrav->sOffsetY;

				SGPRect rect;
				rect.iLeft		= SKI_FACE_X;
				rect.iTop		= SKI_FACE_Y;
				rect.iRight		= SKI_FACE_X + SKI_FACE_WIDTH;
				rect.iBottom	= SKI_FACE_Y + SKI_FACE_HEIGHT;

				Blt8BPPDataTo16BPPBufferTransShadowClip( pBuffer, uiPitch,
														 hVObject,
														 sCenX, sCenY,
														 pSoldier->usAniFrame,
														 &rect,
														 pSoldier->pShades[pSoldier->ubFadeLevel],
														 TRUE
					);
				
				UnLockVideoSurface( FRAME_BUFFER );
			}
		}
	}

	// CHRISL: If in NIV mode, don't display the clock
	if ( UsingNewInventorySystem( ) == false )
		RenderClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );

	// CHRISL: Added X,Y parameters to allow control of TownID string placement.
	RenderTownIDString( LOCATION_NAME_X, LOCATION_NAME_Y );
	
	//Display the Title
	DrawTextToScreen( SKI_Text[ SKI_TEXT_MERCHADISE_IN_STOCK ], SKI_MAIN_TITLE_X, SKI_MAIN_TITLE_Y, SKI_MAIN_TITLE_WIDTH, SKI_TITLE_FONT, SKI_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// display shopkeeper budget
	DisplayWrappedString( SKI_BUDGET_X, SKI_BUDGET_Y, SKI_BUDGET_WIDTH, 2, SKI_LABEL_FONT, SKI_TITLE_COLOR, SKI_Text[SKI_TEXT_BUDGET], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	swprintf( zMoney, L"%d", gArmsDealerStatus[gbSelectedArmsDealerID].uiArmsDealersCash );

	InsertCommasForDollarFigure( zMoney );

	CHAR16			zTemp2[64];
	if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
		swprintf( zTemp2, L"%s Intel", zMoney );
	else
		swprintf( zTemp2, L"$%s", zMoney );

	DrawTextToScreen( zTemp2, SKI_BUDGET_X, SKI_BUDGET_OFFSET_TO_VALUE, SKI_BUDGET_WIDTH, FONT10ARIAL, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, TRUE, CENTER_JUSTIFIED );

	//if the dealer repairs
	if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
	{
		//Display the Repair cost text
		DisplayWrappedString( SKI_TOTAL_COST_X, SKI_TOTAL_COST_Y, SKI_TOTAL_COST_WIDTH, 2, SKI_LABEL_FONT, SKI_TITLE_COLOR, SKI_Text[ SKI_TEXT_REPAIR_COST ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}
	else
	{
		//Display the Total cost text
		DisplayWrappedString( SKI_TOTAL_COST_X, SKI_TOTAL_COST_Y, SKI_TOTAL_COST_WIDTH, 2, SKI_LABEL_FONT, SKI_TITLE_COLOR, SKI_Text[ SKI_TEXT_TOTAL_COST ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	//Display the players current balance text
	DisplayWrappedString( SKI_PLAYERS_CURRENT_BALANCE_X, SKI_PLAYERS_CURRENT_BALANCE_Y, SKI_PLAYERS_CURRENT_BALANCE_WIDTH, 2, SKI_LABEL_FONT, SKI_TITLE_COLOR, SkiMessageBoxText[ SKI_PLAYERS_CURRENT_BALANCE ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the players current balance value
	if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
	{
		swprintf( zMoney, L"%d", (int)(GetIntel()) );
		InsertCommasForDollarFigure( zMoney );
		swprintf( zTemp2, L"%s Intel", zMoney );
	}
	else
	{
		swprintf( zMoney, L"%d", LaptopSaveInfo.iCurrentBalance );
		InsertCommasForDollarFigure( zMoney );
		swprintf( zTemp2, L"$%s", zMoney );
	}

	DrawTextToScreen( zTemp2, SKI_PLAYERS_CURRENT_BALANCE_X, SKI_PLAYERS_CURRENT_BALANCE_OFFSET_TO_VALUE, SKI_PLAYERS_CURRENT_BALANCE_WIDTH, FONT10ARIAL, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, TRUE, CENTER_JUSTIFIED );

	//Display the total value text
	DisplayWrappedString( SKI_TOTAL_VALUE_X, SKI_TOTAL_VALUE_Y, SKI_TOTAL_VALUE_WIDTH, 2, SKI_LABEL_FONT, SKI_TITLE_COLOR, SKI_Text[SKI_TEXT_TOTAL_VALUE], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Heinz: 22.02.09 BUGFIX: best way - to update whole tactical screen around SKI
	//BlitBufferToBuffer( guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SKI_TACTICAL_BACKGROUND_START_X, SKI_TACTICAL_BACKGROUND_START_HEIGHT );
	BlitBufferToBuffer( guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	//At this point the background is pure, copy it to the save buffer
	if( gfRenderScreenOnNextLoop )
	{
	//	BlitBufferToBuffer( guiRENDERBUFFER, guiCornerWhereTacticalIsStillSeenImage, SKI_TACTICAL_BACKGROUND_START_X, SKI_TACTICAL_BACKGROUND_START_Y, SKI_TACTICAL_BACKGROUND_START_WIDTH, SKI_TACTICAL_BACKGROUND_START_HEIGHT );
		GetVideoSurface( &hDestVSurface, guiCornerWhereTacticalIsStillSeenImage );
		GetVideoSurface( &hSrcVSurface, guiSAVEBUFFER );

		//Heinz: 22.02.09 BUGFIX: best way - to update whole tactical screen around SKI
		//SrcRect.iLeft = SKI_TACTICAL_BACKGROUND_START_X;
		//SrcRect.iTop = SKI_TACTICAL_BACKGROUND_START_Y;
		//SrcRect.iRight = SKI_TACTICAL_BACKGROUND_START_X + SKI_TACTICAL_BACKGROUND_START_WIDTH;
		//SrcRect.iBottom = SKI_TACTICAL_BACKGROUND_START_Y + SKI_TACTICAL_BACKGROUND_START_HEIGHT;
		//BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, VO_BLT_SRCTRANSPARENCY, SKI_TACTICAL_BACKGROUND_START_X, SKI_TACTICAL_BACKGROUND_START_Y, (RECT*)&SrcRect );
		SrcRect.iLeft = 0;
		SrcRect.iTop = 0;
		SrcRect.iRight = SCREEN_WIDTH;
		SrcRect.iBottom = SCREEN_HEIGHT;
		BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, VO_BLT_SRCTRANSPARENCY, 0, 0, (RECT*)&SrcRect );

		gfRenderScreenOnNextLoop = FALSE;
	}

	DisplayArmsDealerCurrentInventoryPage( );

	DisplayArmsDealerOfferArea( );

	DisplayPlayersOfferArea( );

	EnableDisableEvaluateAndTransactionButtons( );

	//if the merc is talking and the screen has been dirtied
	if( gfIsTheShopKeeperTalking )
		RenderMercPopUpBoxFromIndex( giPopUpBoxId, gusPositionOfSubTitlesX, SKI_POSITION_SUBTITLES_Y, FRAME_BUFFER);
	
	//Restore the tactical background that is visble behind the SKI panel
	RestoreTacticalBackGround();

	InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	return( TRUE );
}

void RestoreTacticalBackGround()
{
	HVSURFACE hDestVSurface, hSrcVSurface;
	SGPRect		SrcRect;

	GetVideoSurface( &hDestVSurface, guiRENDERBUFFER );
	GetVideoSurface( &hSrcVSurface, guiCornerWhereTacticalIsStillSeenImage );
	
	// Top
	SrcRect.iLeft = 0; 
	SrcRect.iTop = 0; 
	SrcRect.iRight = SCREEN_WIDTH; 
	SrcRect.iBottom = SKI_TOTAL_BEGIN_Y;
	BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, VO_BLT_SRCTRANSPARENCY, SrcRect.iLeft, SrcRect.iTop, (RECT*)&SrcRect );

	// Bottom
	SrcRect.iLeft = 0; 
	SrcRect.iTop = SKI_TOTAL_END_Y;
	SrcRect.iRight = SCREEN_WIDTH; 
	SrcRect.iBottom = SCREEN_HEIGHT - INV_INTERFACE_HEIGHT; 
	BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, VO_BLT_SRCTRANSPARENCY, SrcRect.iLeft, SrcRect.iTop, (RECT*)&SrcRect );

	// Left
	SrcRect.iLeft = 0; 
	SrcRect.iTop = SKI_TOTAL_BEGIN_Y;
	SrcRect.iRight = SKI_TOTAL_BEGIN_X;
	SrcRect.iBottom = SKI_TOTAL_END_Y;
	BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, VO_BLT_SRCTRANSPARENCY, SrcRect.iLeft, SrcRect.iTop, (RECT*)&SrcRect );

	// Right
	SrcRect.iLeft = SKI_TOTAL_END_X;
	SrcRect.iTop = SKI_TOTAL_BEGIN_Y;
	SrcRect.iRight = SCREEN_WIDTH; 
	SrcRect.iBottom = SKI_TOTAL_END_Y;
	BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, VO_BLT_SRCTRANSPARENCY, SrcRect.iLeft, SrcRect.iTop, (RECT*)&SrcRect );
}

void		GetShopKeeperInterfaceUserInput()
{
	InputAtom Event;
	POINT MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	while (DequeueSpecificEvent(&Event, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{
				case ESC:
					// clean exits - does quotes, shuts up shopkeeper, etc.
					ExitSKIRequested();

#ifdef JA2TESTVERSION
					//Instant exit - doesn't clean up much
//					gfSKIScreenExit = TRUE;
#endif
					break;

				case 'x':
					if( Event.usKeyState & ALT_DOWN )
					{
						HandleShortCutExitState( );
					}
					break;

				case SPACE:
					{
						UINT8	ubID;

						DeleteItemDescriptionBox( );

						// skip Robot and EPCs
						ubID = FindNextActiveAndAliveMerc( gpSMCurrentMerc, FALSE, TRUE );

						gubSelectSMPanelToMerc = ubID;

						LocateSoldier( ubID, DONTSETLOCATOR );
						// refresh background for player slots (in case item values change due to Flo's discount)
						gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
					}
					break;

#ifdef JA2TESTVERSION

				case 'r':
					gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
					break;
				case 'i':
					InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
					break;

				case 'd':
					//Test key to toggle the display of the money each dealer has on hand
					gfTestDisplayDealerCash ^= 1;
					break;

#endif
			}
		}
	}
}

#ifdef JA2TESTVERSION
void DisplayAllDealersCash()
{
	UINT16	usPosY=0;
	CHAR16	zTemp[512];
	CHAR16			zTemp2[64];
	UINT8	ubForeColor;

	//loop through all the shopkeeper's and display their money
	for ( INT8 bArmsDealer = 0; bArmsDealer<NUM_ARMS_DEALERS; ++bArmsDealer )
	{
		//Display the shopkeeper's name
		DrawTextToScreen( gMercProfiles[ armsDealerInfo[ bArmsDealer ].ubShopKeeperID ].zNickname, SCREEN_X_OFFSET + 540, SCREEN_Y_OFFSET + usPosY, 0, FONT10ARIAL, SKI_TITLE_COLOR, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED );

		//Display the arms dealer cash on hand
		swprintf( zTemp, L"%d", gArmsDealerStatus[ bArmsDealer ].uiArmsDealersCash );

		InsertCommasForDollarFigure( zTemp );
		
		if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
			swprintf( zTemp2, L"%s Intel", zTemp );
		else
			swprintf( zTemp2, L"$%s", zTemp );

		ubForeColor = ( UINT8 ) ( ( bArmsDealer == gbSelectedArmsDealerID ) ? SKI_BUTTON_COLOR : SKI_TITLE_COLOR );
		DrawTextToScreen( zTemp2, SCREEN_X_OFFSET + 590, SCREEN_Y_OFFSET + usPosY, 0, FONT10ARIAL, ubForeColor, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED );
		usPosY += 17;
	}
}
#endif

void BtnSKI_InvPageUpButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		if( gSelectArmsDealerInfo.ubCurrentPage > 1 )
		{
			gSelectArmsDealerInfo.ubCurrentPage--;
			gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			EnableDisableDealersInventoryPageButtons();
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnSKI_InvPageDownButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		if( gSelectArmsDealerInfo.ubCurrentPage < gSelectArmsDealerInfo.ubNumberOfPages )
		{
			gSelectArmsDealerInfo.ubCurrentPage++;
			gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			EnableDisableDealersInventoryPageButtons();
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnSKI_TransactionButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);

		// if not already doing it (multiple clicks can be queued up while shopkeeper is still talking)
		if ( !gfPerformTransactionInProgress )
		{
			// shut him up
			ShutUpShopKeeper();

			giShopKeepDialogueEventinProgress = -1;

			PerformTransaction( 0 );
		}
	}
}

void BtnSKI_DoneButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		ExitSKIRequested();
	}

	InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
}

void CreateSkiInventorySlotMouseRegions( )
{
	int ubCnt, x,y;
	UINT16	usPosX, usPosY;
	
	// Create the mouse regions for the shopkeeper's inventory
	usPosY = SKI_TRADER_INVENTORY_BOXES_BEGIN_Y;
	ubCnt=0;
	for ( y = 0; y<SKI_TRADER_INVENTORY_BOXES_PER_COL; ++y )
	{
		usPosX = SKI_TRADER_INVENTORY_BOXES_BEGIN_X;

		for ( x = 0; x<SKI_TRADER_INVENTORY_BOXES_PER_ROW; ++x )
		{
			MSYS_DefineRegion( &gDealersInventoryMouseRegions[ ubCnt ], usPosX, usPosY, (INT16)(usPosX + SKI_INV_SLOT_WIDTH), (INT16)(usPosY + SKI_INV_SLOT_HEIGHT), MSYS_PRIORITY_HIGH,
										 CURSOR_NORMAL, SelectDealersInventoryMovementRegionCallBack, SelectDealersInventoryRegionCallBack );
			MSYS_AddRegion( &gDealersInventoryMouseRegions[ubCnt] );
			MSYS_SetRegionUserData( &gDealersInventoryMouseRegions[ ubCnt ], 0, ubCnt);
			MSYS_SetRegionUserData( &gDealersInventoryMouseRegions[ ubCnt ], 1, ARMS_DEALER_INVENTORY );
			
			//if the dealer repairs
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
			{
				//Small Faces
				MSYS_DefineRegion( &gRepairmanInventorySmallFaceMouseRegions[ ubCnt ], (UINT16)(usPosX+SKI_SMALL_FACE_OFFSET_X), (UINT16)(usPosY), (INT16)(usPosX + SKI_SMALL_FACE_OFFSET_X + SKI_SMALL_FACE_WIDTH), (INT16)(usPosY+ SKI_SMALL_FACE_HEIGHT), MSYS_PRIORITY_HIGH+1,
											 CURSOR_NORMAL, NULL, NULL );
				MSYS_AddRegion( &gRepairmanInventorySmallFaceMouseRegions[ ubCnt ] );
				MSYS_SetRegionUserData( &gRepairmanInventorySmallFaceMouseRegions[ ubCnt ], 0, ubCnt );
				MSYS_SetRegionUserData( &gRepairmanInventorySmallFaceMouseRegions[ ubCnt ], 1, ARMS_DEALER_INVENTORY );
			}

			usPosX += SKI_INV_OFFSET_X;
			++ubCnt;
			++gDealersInventoryActiveRegions;
		}
		usPosY += SKI_INV_OFFSET_Y;
	}

	// Create the mouse regions for the shopkeeper's trading slots
	usPosY = SKI_TRADER_OFFER_BOXES_BEGIN_Y;
	ubCnt=0;
	for ( y = 0; y<SKI_TRADER_OFFER_BOXES_PER_COL; ++y )
	{
		usPosX = SKI_TRADER_OFFER_BOXES_BEGIN_X;

		for ( x = 0; x<SKI_TRADER_OFFER_BOXES_PER_ROW; ++x )
		{
			MSYS_DefineRegion( &gDealersOfferSlotsMouseRegions[ ubCnt ], usPosX, usPosY, (INT16)(usPosX + SKI_INV_SLOT_WIDTH), (INT16)(usPosY + SKI_INV_SLOT_HEIGHT), MSYS_PRIORITY_HIGH,
										 CURSOR_NORMAL, SelectDealersOfferSlotsMovementRegionCallBack, SelectDealersOfferSlotsRegionCallBack );
			MSYS_AddRegion( &gDealersOfferSlotsMouseRegions[ubCnt] );
			MSYS_SetRegionUserData( &gDealersOfferSlotsMouseRegions[ ubCnt ], 0, ubCnt);
						
			//if the dealer repairs
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
			{
				//Small Faces
				MSYS_DefineRegion( &gDealersOfferSlotsSmallFaceMouseRegions[ ubCnt ], (UINT16)(usPosX+SKI_SMALL_FACE_OFFSET_X), (UINT16)(usPosY), (INT16)(usPosX + SKI_SMALL_FACE_OFFSET_X + SKI_SMALL_FACE_WIDTH), (INT16)(usPosY+ SKI_SMALL_FACE_HEIGHT), MSYS_PRIORITY_HIGH+1,
											 CURSOR_NORMAL, SelectDealersOfferSlotsMovementRegionCallBack, SelectDealersOfferSlotsRegionCallBack );
				MSYS_AddRegion( &gDealersOfferSlotsSmallFaceMouseRegions[ubCnt] );
				MSYS_SetRegionUserData( &gDealersOfferSlotsSmallFaceMouseRegions[ ubCnt ], 0, ubCnt);
				MSYS_SetRegionUserData( &gDealersOfferSlotsSmallFaceMouseRegions[ ubCnt ], 1, ARMS_DEALER_OFFER_AREA );
			}
			
			usPosX += SKI_INV_OFFSET_X;
			++ubCnt;
			++gDealersOfferActiveRegions;
		}
		usPosY += SKI_INV_OFFSET_Y;
	}

	// Create the mouse regions for the Players trading slots
	usPosY = SKI_PLAYER_OFFER_BOXES_BEGIN_Y;
	ubCnt=0;
	for ( y = 0; y<SKI_PLAYER_OFFER_BOXES_PER_COL; ++y )
	{
		usPosX = SKI_PLAYER_OFFER_BOXES_BEGIN_X;

		for ( x = 0; x<SKI_PLAYER_OFFER_BOXES_PER_ROW; ++x )
		{
			//Trading Slots
			MSYS_DefineRegion( &gPlayersOfferSlotsMouseRegions[ ubCnt ], usPosX, usPosY, (INT16)(usPosX + SKI_INV_SLOT_WIDTH), (INT16)(usPosY + SKI_INV_SLOT_HEIGHT), MSYS_PRIORITY_HIGH,
										 CURSOR_NORMAL, SelectPlayersOfferSlotsMovementRegionCallBack, SelectPlayersOfferSlotsRegionCallBack );
			MSYS_AddRegion( &gPlayersOfferSlotsMouseRegions[ubCnt] );
			MSYS_SetRegionUserData( &gPlayersOfferSlotsMouseRegions[ ubCnt ], 0, ubCnt);
			
			//Small Faces
			MSYS_DefineRegion( &gPlayersOfferSlotsSmallFaceMouseRegions[ ubCnt ], (UINT16)(usPosX+SKI_SMALL_FACE_OFFSET_X), (UINT16)(usPosY), (INT16)(usPosX + SKI_SMALL_FACE_OFFSET_X + SKI_SMALL_FACE_WIDTH), (INT16)(usPosY+ SKI_SMALL_FACE_HEIGHT), MSYS_PRIORITY_HIGH+1,
										 CURSOR_NORMAL, SelectPlayersOfferSlotsMovementRegionCallBack, SelectPlayersOfferSlotsRegionCallBack );
			MSYS_AddRegion( &gPlayersOfferSlotsSmallFaceMouseRegions[ubCnt] );
			MSYS_SetRegionUserData( &gPlayersOfferSlotsSmallFaceMouseRegions[ ubCnt ], 0, ubCnt);
			MSYS_SetRegionUserData( &gPlayersOfferSlotsSmallFaceMouseRegions[ ubCnt ], 1, PLAYERS_OFFER_AREA );

			usPosX += SKI_INV_OFFSET_X;
			++ubCnt;
			++gPlayersOfferActiveRegions;
		}
		usPosY += SKI_INV_OFFSET_Y;
	}
}

void DestroySkiInventorySlotMouseRegions( )
{
	for ( int i = 0; i<gDealersInventoryActiveRegions; ++i )
	{
		MSYS_RemoveRegion( &gDealersInventoryMouseRegions[ i ] );

		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			MSYS_RemoveRegion( &gRepairmanInventorySmallFaceMouseRegions[ i ] );
		}
	}

	gDealersInventoryActiveRegions = 0;

	for ( int i = 0; i<gDealersOfferActiveRegions; ++i )
	{
		MSYS_RemoveRegion( &gDealersOfferSlotsMouseRegions[ i ] );

		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			MSYS_RemoveRegion( &gDealersOfferSlotsSmallFaceMouseRegions[ i ] );
		}
	}

	gDealersOfferActiveRegions = 0;

	for ( int i = 0; i<gPlayersOfferActiveRegions; ++i )
	{
		MSYS_RemoveRegion( &gPlayersOfferSlotsMouseRegions[i] );

		MSYS_RemoveRegion( &gPlayersOfferSlotsSmallFaceMouseRegions[i] );
	}

	gPlayersOfferActiveRegions = 0;
}

//Mouse Call back for the Arms traders inventory slot
void SelectDealersInventoryRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT16	ubSelectedInvSlot = MSYS_GetRegionUserData( pRegion, 0 );
		INT16	ubLocation;

		if( gpTempDealersInventory.empty() == true )
			return;

		ubSelectedInvSlot += gSelectArmsDealerInfo.ubFirstItemIndexOnPage;

		//if the selected slot is above any inventory we have, return
		if( ubSelectedInvSlot >= gpTempDealersInventory.size() )
			return;

		//if there are any items still there
		if( gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject.exists() == true )
		{
			//If the item type has not already been placed
			if( !( gpTempDealersInventory[ ubSelectedInvSlot ].uiFlags & ARMS_INV_ITEM_SELECTED ) )
			{
				//if the dealer repairs
				if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
				{
					// ignore left clicks on items under repair.  Fully repaired items are moved out to player's slots automatically
				}
				else	// not a repairman
				{
					ubLocation = AddItemToArmsDealerOfferArea( &gpTempDealersInventory[ ubSelectedInvSlot ], ubSelectedInvSlot );

					//if the item was added to the Dealer Offer Area correctly
					if( ubLocation != -1 )
					{
						//Set the flag indicating the item has been selected
						gpTempDealersInventory[ ubSelectedInvSlot ].uiFlags |= ARMS_INV_ITEM_SELECTED;

						//Specify the location the items went to
						gpTempDealersInventory[ ubSelectedInvSlot ].ubLocationOfObject = ARMS_DEALER_OFFER_AREA;
						gpTempDealersInventory[ ubSelectedInvSlot ].bSlotIdInOtherLocation = ubLocation;

						//if the shift key is being pressed, remove them all
						if( gfKeyState[ SHIFT ] )
						{
							DeleteObj(&gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject);
						}
						else
						{
							gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject.RemoveObjectsFromStack(1);
						}

						gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
					}
				}
			}
			else	// some of this item are already in dealer's offer area
			{
				UINT8 ubNumToMove;

				//if the shift key is being pressed, remove them all
				if( gfKeyState[ SHIFT ] )
				{
					ubNumToMove = gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject.ubNumberOfObjects;
				}
				else
				{
					ubNumToMove = 1;
				}

				//Reduce the number in dealer's inventory and Increase the number in dealer's offer area
				ArmsDealerOfferArea[ gpTempDealersInventory[ ubSelectedInvSlot ].bSlotIdInOtherLocation ].ItemObject.ForceAddObjectsToStack(
					gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject, ubNumToMove);
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		UINT16	ubSelectedInvSlot = MSYS_GetRegionUserData( pRegion, 0 );

		if( gpTempDealersInventory.empty() == true )
			return;

		ubSelectedInvSlot += gSelectArmsDealerInfo.ubFirstItemIndexOnPage;

		//if the selected slot is above any inventory we have, return
		if( ubSelectedInvSlot >= gpTempDealersInventory.size() )
			return;

		if(gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject.exists() == false)
			return;

		//DEF:
		//bring up the item description box
		if ( !InItemDescriptionBox( ) )
		{
			InitShopKeeperItemDescBox( &gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject, ubSelectedInvSlot, ARMS_DEALER_INVENTORY );
		}
		else
		{
			DeleteItemDescriptionBox( );
		}

		//if the item has been seleceted
		if( gpTempDealersInventory[ ubSelectedInvSlot ].uiFlags & ARMS_INV_ITEM_SELECTED )
		{
			//Check to see it there is more then 1 item in the location
			if( ArmsDealerOfferArea[ gpTempDealersInventory[ ubSelectedInvSlot ].bSlotIdInOtherLocation ].ItemObject.exists() == true )
			{
				//Decrease the number in the dealer offer area
				ArmsDealerOfferArea[ gpTempDealersInventory[ ubSelectedInvSlot ].bSlotIdInOtherLocation ].ItemObject.MoveThisObjectTo(gTempObject, 1);

				//Increase the number in the dealer inventory
				gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject.AddObjectsToStack(gTempObject);

				//if there is nothing left in the arms dealer offer area
				if( ArmsDealerOfferArea[ gpTempDealersInventory[ ubSelectedInvSlot ].bSlotIdInOtherLocation ].ItemObject.exists() == false )
				{
					RemoveItemFromArmsDealerOfferArea( gpTempDealersInventory[ ubSelectedInvSlot ].bSlotIdInOtherLocation, FALSE );
				}

				//redraw the screen
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
		}
	}
}

void SelectDealersInventoryMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT16	ubSelectedInvSlot = MSYS_GetRegionUserData( pRegion, 0 );
	ubSelectedInvSlot += gSelectArmsDealerInfo.ubFirstItemIndexOnPage;

	if( gpTempDealersInventory.empty() == true )
		return;

	if( ubSelectedInvSlot >= gpTempDealersInventory.size() )
		return;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		//if there is nothing in the slot, exit
		if( gpTempDealersInventory[ ubSelectedInvSlot ].fActive == FALSE )
			return;

		gpHighLightedItemObject = &gpTempDealersInventory[ ubSelectedInvSlot ].ItemObject;

		HandleCompatibleAmmoUI( gpSMCurrentMerc, -1, TRUE );
	}
	else if(iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		//if there is nothing in the slot, exit
		if( gpTempDealersInventory[ ubSelectedInvSlot ].fActive == FALSE )
			return;

		gpHighLightedItemObject = NULL;
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;	//ddd
		HandleCompatibleAmmoUI( gpSMCurrentMerc, -1, FALSE );
	}
}

void SelectDealersOfferSlotsMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT16	ubSelectedInvSlot = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		//if there is nothing in the slot, exit
		if( ArmsDealerOfferArea[ ubSelectedInvSlot ].fActive == FALSE )
			return;

		gpHighLightedItemObject = &ArmsDealerOfferArea[ ubSelectedInvSlot ].ItemObject;
		HandleCompatibleAmmoUI( gpSMCurrentMerc, -1, TRUE );
	}
	else if(iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		//if there is nothing in the slot, exit
		if( ArmsDealerOfferArea[ ubSelectedInvSlot ].fActive == FALSE )
			return;

		gpHighLightedItemObject = NULL;
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;//ddd
		HandleCompatibleAmmoUI( gpSMCurrentMerc, -1, FALSE );
	}
}

void SelectPlayersOfferSlotsMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT16	ubSelectedInvSlot = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		//if there is nothing in the slot, exit
		if( PlayersOfferArea[ ubSelectedInvSlot ].fActive == FALSE )
			return;

		gpHighLightedItemObject = &PlayersOfferArea[ ubSelectedInvSlot ].ItemObject;
		HandleCompatibleAmmoUI( gpSMCurrentMerc, -1, TRUE );
	}
	else if(iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		//if there is nothing in the slot, exit
		if( PlayersOfferArea[ ubSelectedInvSlot ].fActive == FALSE )
			return;

		gpHighLightedItemObject = NULL;
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;//ddd
		HandleCompatibleAmmoUI( gpSMCurrentMerc, -1, FALSE );
	}
}

//Mouse Call back for the dealer's OFFER slot
void SelectDealersOfferSlotsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT16	ubSelectedInvSlot = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		//if there is something here
		if( ArmsDealerOfferArea[ ubSelectedInvSlot ].fActive )
		{
			//if the dealer repairs
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
			{
				// return item to player
				RemoveRepairItemFromDealersOfferArea( ubSelectedInvSlot );
			}
			else
			{
				//bring up the item description box
				if ( !InItemDescriptionBox( ) )
				{
					InitShopKeeperItemDescBox( &ArmsDealerOfferArea[ ubSelectedInvSlot ].ItemObject, ubSelectedInvSlot, ARMS_DEALER_OFFER_AREA );
				}
				else
				{
					DeleteItemDescriptionBox( );
				}
			}
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) //MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
/*
		//if the current merc is disabled for whatever reason
		if( gfSMDisableForItems )
			// the
			return;
*/
		//if there is something here
		if( ArmsDealerOfferArea[ ubSelectedInvSlot ].fActive )
		{
			//if this is a repair dealer
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
			{
				//if we don't have an item, pick one up
				if( gMoveingItem.sItemIndex == 0 )
				{
					//if the dealer is a repair dealer, allow the player to pick up the item
					if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
					{
						BeginSkiItemPointer( ARMS_DEALER_OFFER_AREA, ubSelectedInvSlot, FALSE );
					}
				}
				else
				{
					//swap what is in the cursor with what is in the player offer slot
					INVENTORY_IN_SLOT TempSlot;

					// if the slot is overloaded (holds more objects than we have room for valid statuses of)
					if ( ArmsDealerOfferArea[ ubSelectedInvSlot ].ItemObject.ubNumberOfObjects > MAX_OBJECTS_PER_SLOT )
					{
						// then ignore the click - we can't do the swap, or anything very useful, because we can't allow overloaded
						// items into the player's cursor - if he put them into a merc's inventory, the extra statuses are missing!
						// At best, we could do some sort of message here.
						return;
					}

					IfMercOwnedCopyItemToMercInv( &gMoveingItem );

					TempSlot = ArmsDealerOfferArea[ ubSelectedInvSlot ];
					ArmsDealerOfferArea[ ubSelectedInvSlot ] = gMoveingItem;
					gMoveingItem = TempSlot;

					IfMercOwnedRemoveItemFromMercInv( &gMoveingItem );

					// Change mouse cursor
					gpItemPointer = &gMoveingItem.ItemObject;
					SetSkiCursor( EXTERN_CURSOR );

					gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
				}
			}
			else	//non-repairman
			{
				//if the shift key is being pressed, remove them all
				if( gfKeyState[ SHIFT ] || ( ArmsDealerOfferArea[ ubSelectedInvSlot ].ItemObject.ubNumberOfObjects == 1 ) )
				{
					RemoveItemFromArmsDealerOfferArea( ubSelectedInvSlot, TRUE );//ArmsDealerOfferArea[ ubSelectedInvSlot ].bSlotIdInOtherLocation
				}
				else	// multiple items there, SHIFT isn't being pressed
				{
					// remove only one at a time
					gpTempDealersInventory[ ArmsDealerOfferArea[ ubSelectedInvSlot ].bSlotIdInOtherLocation ].ItemObject.ForceAddObjectsToStack(ArmsDealerOfferArea[ ubSelectedInvSlot ].ItemObject, 1);
				}

				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
		}
		else	// empty slot
		{
			//if the cursor has something in it
			if( gMoveingItem.sItemIndex > 0 )
			{
				// we'd better talking to a repairman, cursor is locked out of this area while full for non-repairmen!
				Assert( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS );

				//Drop the item into the current slot
				AddInventoryToSkiLocation( &gMoveingItem, ubSelectedInvSlot, ARMS_DEALER_OFFER_AREA );

				//Reset the cursor
				SetSkiCursor( CURSOR_NORMAL );

				//refresh the screen
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
		}
	}
}

//Mouse Call back for the Players OFFER slot
void SelectPlayersOfferSlotsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT16	ubSelectedInvSlot = MSYS_GetRegionUserData( pRegion, 0 );
	INT16 bAddedToSlotID = -1;
	
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		//if the cursor has no item in it
		if( gMoveingItem.sItemIndex == 0 )
		{
			//if there is nothing here, return
			if( PlayersOfferArea[ ubSelectedInvSlot ].fActive == FALSE )
				return;

			// pick it up into the cursor
			BeginSkiItemPointer( PLAYERS_OFFER_AREA, ubSelectedInvSlot, FALSE );
		}
		else	// we have something in the cursor
		{
			//Drop the item into the current slot
//			AddObjectToSkiLocation( &gpSkiItemPointer, ubSelectedInvSlot, PLAYERS_OFFER_AREA );

			//if there is something already there
			if( PlayersOfferArea[ ubSelectedInvSlot ].fActive )
			{
				//swap what is in the cursor with what is in the player offer slot
				INVENTORY_IN_SLOT TempSlot;

				// if the slot is overloaded (holds more objects than we have room for valid statuses of)
				if ( PlayersOfferArea[ ubSelectedInvSlot ].ItemObject.ubNumberOfObjects > MAX_OBJECTS_PER_SLOT )
				{
					// then ignore the click - we can't do the swap, or anything very useful, because we can't allow overloaded
					// items into the player's cursor - if he put them into a merc's inventory, the extra statuses are missing!
					// At best, we could do some sort of message here.
					return;
				}

				IfMercOwnedCopyItemToMercInv( &gMoveingItem );

				TempSlot = PlayersOfferArea[ ubSelectedInvSlot ];
				PlayersOfferArea[ ubSelectedInvSlot ] = gMoveingItem;
				gMoveingItem = TempSlot;

				IfMercOwnedRemoveItemFromMercInv( &gMoveingItem );

				// Change mouse cursor
				gpItemPointer = &gMoveingItem.ItemObject;
				SetSkiCursor( EXTERN_CURSOR );

				//if the item we are adding is money
				if( Item[ PlayersOfferArea[ ubSelectedInvSlot ].sItemIndex ].usItemClass == IC_MONEY && !( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL ) )
				{
					//Since money is always evaluated
					PlayersOfferArea[ ubSelectedInvSlot ].uiFlags |= ARMS_INV_PLAYERS_ITEM_HAS_VALUE;
					PlayersOfferArea[ ubSelectedInvSlot ].uiItemPrice = PlayersOfferArea[ ubSelectedInvSlot ].ItemObject[0]->data.money.uiMoneyAmount;
				}
			}
			else	// slot is empty
			{
				// if the item has already been evaluated, or has just been purchased
				if ( ( gMoveingItem.uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_BEEN_EVALUATED ) ||
					   ( gMoveingItem.uiFlags & ARMS_INV_JUST_PURCHASED ) )
				{
					//place the item that is in the player's hand into this player offer area slot
					bAddedToSlotID = AddInventoryToSkiLocation( &gMoveingItem, ubSelectedInvSlot, PLAYERS_OFFER_AREA );
					Assert ( bAddedToSlotID != -1 );
				}
				else
				{
					// this splits complex items for repairs.  Also puts things into the first free POA slot
					OfferObjectToDealer( &gMoveingItem.ItemObject, gMoveingItem.ubIdOfMercWhoOwnsTheItem, gMoveingItem.bSlotIdInOtherLocation );
				}

				//Reset the cursor
				SetSkiCursor( CURSOR_NORMAL );
			}

			//Dirty the interface
			gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
		}
	}
	else if(iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) //MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		//if the box is active
		if( PlayersOfferArea[ ubSelectedInvSlot ].fActive )
		{
			RemoveItemFromPlayersOfferArea( ubSelectedInvSlot );
/*
item description
			else
			{
				if ( !InItemDescriptionBox( ) )
				{
					InitItemDescriptionBox( gpSMCurrentMerc, (UINT8)PlayersOfferArea[ ubSelectedInvSlot ].bSlotIdInOtherLocation, 214, 1 + INV_INTERFACE_START_Y, 0 );
				}
				else
				{
					DeleteItemDescriptionBox( );
				}
			}
*/
		}
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
	}
}

void SelectSkiInventoryMovementAreaRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{

	}
}

void EnterShopKeeperInterfaceScreen( UINT8	ubArmsDealer )
{
	gusIDOfCivTrader = NOBODY;

	//Get Dealer ID from from merc Id
	gbSelectedArmsDealerID = GetArmsDealerIDFromMercID( ubArmsDealer );

	if( gbSelectedArmsDealerID <= -1 )
	{
#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Failed to find Arms Dealer ID From Merc ID #%d", ubArmsDealer );
#endif
		gfSKIScreenExit = TRUE;
	}

	LeaveTacticalScreen( SHOPKEEPER_SCREEN );
}

// Flugente: set up shopkeeper with a non-NPC
void EnterShopKeeperInterfaceScreen_NonNPC( INT8 ubArmsDealerID, UINT8 aMercID )
{
	gusIDOfCivTrader = aMercID;

	//Get Dealer ID from from merc Id
	gbSelectedArmsDealerID = ubArmsDealerID;

	if ( gbSelectedArmsDealerID <= -1 )
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Failed to find Arms Dealer ID From Merc ID #%d", ubArmsDealerID );
#endif
		gfSKIScreenExit = TRUE;
	}

	// Flugente: additional dialogue
	if ( gusSelectedSoldier != NOBODY )
		AdditionalTacticalCharacterDialogue_CallsLua( MercPtrs[gusSelectedSoldier], ADE_MERCHANT_CHAT, ubArmsDealerID >= 0 ? (UINT32)ubArmsDealerID : 0 );

	LeaveTacticalScreen( SHOPKEEPER_SCREEN );
}

void InitializeShopKeeper( BOOLEAN fResetPage )
{
	// update time player last dealt with him
	gArmsDealerStatus[ gbSelectedArmsDealerID ].uiTimePlayerLastInSKI = GetWorldTotalMin();
	
	gpTempDealersInventory.clear();

	HandlePossibleArmsDealerIntelRefresh(FALSE);
		
	//Get the number of distinct items in the inventory
	//Create the shopkeeper's temp inventory
	DetermineArmsDealersSellingInventory( );

	gSelectArmsDealerInfo.ubNumberOfPages = (UINT8)(gpTempDealersInventory.size( ) / (SKI_TRADER_INVENTORY_BOXES_PER_ROW * SKI_TRADER_INVENTORY_BOXES_PER_COL));
	if( gpTempDealersInventory.size() % 15 != 0 )
		gSelectArmsDealerInfo.ubNumberOfPages += 1;
	
	//Should we reset the current inventory page being displayed
	if( gpTempDealersInventory.empty() == true )
		gSelectArmsDealerInfo.ubCurrentPage = 0;
	else
	{
		if( fResetPage )
		{
			if( gpTempDealersInventory.empty() == true )
				gSelectArmsDealerInfo.ubCurrentPage = 0;
			else
				gSelectArmsDealerInfo.ubCurrentPage = 1;
		}

		//or if the current page will be an invalid page ( before the first, and after the last )
		else if( gSelectArmsDealerInfo.ubCurrentPage < 0 || gSelectArmsDealerInfo.ubCurrentPage > gSelectArmsDealerInfo.ubNumberOfPages )
		{
			gSelectArmsDealerInfo.ubCurrentPage = 1;
		}

		else if( gpTempDealersInventory.empty() == false )
			gSelectArmsDealerInfo.ubCurrentPage = 1;
	}
	
	//if there is no inventory
	if( gpTempDealersInventory.empty() == true )
	{
		gSelectArmsDealerInfo.ubCurrentPage = 0;
		gSelectArmsDealerInfo.ubNumberOfPages = 0;

		//disable the page up/down buttons
		DisableButton( guiSKI_InvPageUpButton );
		DisableButton( guiSKI_InvPageDownButton );

		return;
	}
	else
	{
		//enable the page up/down buttons
		EnableButton( guiSKI_InvPageUpButton );
		EnableButton( guiSKI_InvPageDownButton );
	}
	
	EnableDisableDealersInventoryPageButtons();
}

void CalculateFirstItemIndexOnPage( )
{
	gSelectArmsDealerInfo.ubFirstItemIndexOnPage = (gSelectArmsDealerInfo.ubCurrentPage - 1) * (SKI_TRADER_INVENTORY_BOXES_PER_ROW * SKI_TRADER_INVENTORY_BOXES_PER_COL);
}

void DisplayArmsDealerCurrentInventoryPage( )
{
	CHAR16			zTemp[32];
	UINT16			uiFontHeight;
	UINT16			usCnt=0;
	UINT16			usPosX, usPosY;
	int				sItemCount=0;
	BOOLEAN			fDisplayHatchOnItem=FALSE;
	
	usPosX = SKI_TRADER_INVENTORY_BOXES_BEGIN_X;
	usPosY = SKI_TRADER_INVENTORY_BOXES_BEGIN_Y;

	//if there is any inventory
	if( gpTempDealersInventory.empty() == false )
	{
		if( gubSkiDirtyLevel != SKI_DIRTY_LEVEL0 )
		{
			//Calculate the item that is at the top of the list
			CalculateFirstItemIndexOnPage( );

			//Display the current inventory page
			DrawTextToScreen( SKI_Text[ SKI_TEXT_PAGE ], SKI_PAGE_X, (UINT16)(SKI_PAGE_Y+3), SKI_PAGE_WIDTH, SKI_LABEL_FONT, SKI_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

			//Display the Current Page number
			uiFontHeight = GetFontHeight( SKI_LABEL_FONT );
			swprintf( zTemp, L"%d/%d", gSelectArmsDealerInfo.ubCurrentPage, gSelectArmsDealerInfo.ubNumberOfPages );
			DrawTextToScreen( zTemp, SKI_PAGE_X, (UINT16)(SKI_PAGE_Y+uiFontHeight+6), SKI_PAGE_WIDTH, SKI_LABEL_FONT, SKI_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
		}

		//Display all the items for the current page
		for ( usCnt = gSelectArmsDealerInfo.ubFirstItemIndexOnPage; ((usCnt<gpTempDealersInventory.size( )) && (sItemCount < gDealersInventoryActiveRegions)); ++usCnt )
		{
			//if the item is still in the inventory
			if( gpTempDealersInventory[ usCnt ].sItemIndex != 0 )
			{
				// hatch it out if region is disabled
				if ( !( gDealersInventoryMouseRegions[ sItemCount ].uiFlags & MSYS_REGION_ENABLED ) )
				{
					fDisplayHatchOnItem = TRUE;
				}
				//if the dealer repairs
				else if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
				{
					//if the item is damaged
					if( armsDealerInfo[ gbSelectedArmsDealerID ].uiFlags & ARMS_INV_ITEM_NOT_REPAIRED_YET )
						fDisplayHatchOnItem = TRUE;
					else
						fDisplayHatchOnItem = FALSE;
				}
				else // non-repairman
				{
					// check if none left
					if ( gpTempDealersInventory[ usCnt ].ItemObject.exists() == false )
					{
						fDisplayHatchOnItem = TRUE;
					}
					else
					{
						fDisplayHatchOnItem = FALSE;
					}
				}
				
				// Display the inventory slot
				DisplayInvSlot( usCnt, gpTempDealersInventory[ usCnt ].sItemIndex, usPosX, usPosY,
												&gpTempDealersInventory[ usCnt ].ItemObject,
												fDisplayHatchOnItem,
												ARMS_DEALER_INVENTORY );
				
				if( gubSkiDirtyLevel == SKI_DIRTY_LEVEL2 )
				{
					SetSkiRegionHelpText( &gpTempDealersInventory[ usCnt ], &gDealersInventoryMouseRegions[sItemCount], ARMS_DEALER_INVENTORY );

					//if the dealer repairs
					if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
					{
						SetSkiFaceRegionHelpText( &gpTempDealersInventory[ usCnt ], &gRepairmanInventorySmallFaceMouseRegions[sItemCount], ARMS_DEALER_INVENTORY );
					}
				}

				++sItemCount;

				usPosX += SKI_INV_OFFSET_X;

				//if we are on to the next row
				if ( !(sItemCount % SKI_TRADER_INVENTORY_BOXES_PER_ROW) )
				{
					usPosX = SKI_TRADER_INVENTORY_BOXES_BEGIN_X;
					usPosY += SKI_INV_OFFSET_Y;
				}
			}
		}
	}

	if( gubSkiDirtyLevel == SKI_DIRTY_LEVEL2 )
	{
		//This handles the remaining (empty) slots, resetting Fast Help text, and hatching out disabled ones
		while ( sItemCount < gDealersInventoryActiveRegions )
		{
			SetSkiRegionHelpText( NULL, &gDealersInventoryMouseRegions[ sItemCount ], ARMS_DEALER_INVENTORY );

			//if the dealer repairs
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
			{
				SetSkiFaceRegionHelpText( NULL, &gRepairmanInventorySmallFaceMouseRegions[ sItemCount ], ARMS_DEALER_INVENTORY );
			}

			// hatch it out if region is disabled
			if ( !( gDealersInventoryMouseRegions[ sItemCount ].uiFlags & MSYS_REGION_ENABLED ) )
			{
				HatchOutInvSlot( usPosX, usPosY );
			}

			++sItemCount;

			usPosX += SKI_INV_OFFSET_X;

			//if we are on to the next row
			if ( !(sItemCount % SKI_TRADER_INVENTORY_BOXES_PER_ROW) )
			{
				usPosX = SKI_TRADER_INVENTORY_BOXES_BEGIN_X;
				usPosY += SKI_INV_OFFSET_Y;
			}
		}
	}
}

UINT32 DisplayInvSlot( UINT16 ubSlotNum, UINT16 usItemIndex, UINT16 usPosX, UINT16 usPosY, OBJECTTYPE	*pItemObject, BOOLEAN fHatchedOut, UINT8 ubItemArea )
{
	CHAR16			zTemp[64];
	HVOBJECT		hVObject;
	HVOBJECT		hPixHandle;
	INVTYPE			*pItem;
	ETRLEObject	*pTrav;
	UINT32			usHeight, usWidth;
	INT16				sCenX, sCenY;
	BOOLEAN			fHighlighted = IsGunOrAmmoOfSameTypeSelected( pItemObject );
	BOOLEAN			fDisplayMercFace=FALSE;
	UINT8				ubMercID=0;
	UINT32			uiItemCost=0;
	BOOLEAN			fPrintRepaired = FALSE;

	//if the item is not highlighted, and we are not rerendering the screen
	if( !( fHighlighted || gubSkiDirtyLevel != SKI_DIRTY_LEVEL0 ) )
		return( 0 );
	
	//Display the item graphic, and price
	pItem = &Item[ usItemIndex ];
	GetVideoObject( &hVObject, GetInterfaceGraphicForItem( pItem ) );
	UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;
	pTrav = &(hVObject->pETRLEObject[ usGraphicNum ] );

	usHeight				= (UINT32)pTrav->usHeight;
	usWidth					= (UINT32)pTrav->usWidth;

	sCenX = usPosX + 7 + (INT16)( abs( (long)(SKI_INV_WIDTH - 3 - usWidth) ) / 2 ) - pTrav->sOffsetX;
	sCenY = usPosY + (INT16)( abs( (long) (SKI_INV_HEIGHT - usHeight) ) / 2 ) - pTrav->sOffsetY;
	
	//blt the shadow of the item
	if(gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ]) BltVideoObjectOutlineShadowFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), g_bUsePngItemImages ? 0 : pItem->ubGraphicNum, sCenX-2, sCenY+2);

	//blt the item
	BltVideoObjectOutlineFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), g_bUsePngItemImages ? 0 : pItem->ubGraphicNum, sCenX, sCenY, Get16BPPColor( FROMRGB( 255, 255, 255 ) ), fHighlighted );

#ifdef ENCYCLOPEDIA_WORKS
	//Moa: encyclopedia item visibility
	EncyclopediaSetItemAsVisible( usItemIndex, ENC_ITEM_DISCOVERED_INSPECTABLE );
#endif

	//Display the status of the item
	//bfant: adjusted size of status bar for item text
	DrawItemUIBarEx( pItemObject, 0, (INT16)(usPosX+2), (INT16)(usPosY+2+24), 2, 24,	Get16BPPColor( FROMRGB( 140, 136, 119 ) ), Get16BPPColor( FROMRGB( 140, 136, 119 ) ), TRUE, guiRENDERBUFFER );//guiSAVEBUFFER
	
	//Display the Items Cost
	if( ubItemArea == PLAYERS_OFFER_AREA )
	{
		//if the item has value
		if( PlayersOfferArea[ ubSlotNum ].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_VALUE )
		{
			uiItemCost = PlayersOfferArea[ ubSlotNum ].uiItemPrice;
		}

		//if the item belongs to a merc
		if( PlayersOfferArea[ ubSlotNum ].ubIdOfMercWhoOwnsTheItem != NO_PROFILE )
		{
			//Display the face of the merc
			fDisplayMercFace = TRUE;
			ubMercID = PlayersOfferArea[ ubSlotNum ].ubIdOfMercWhoOwnsTheItem;
		}

		// if the item has just been repaired
		if ( PlayersOfferArea[ ubSlotNum ].uiFlags & ARMS_INV_ITEM_REPAIRED )
		{
			fPrintRepaired = TRUE;
		}
	}
	else if( ubItemArea == ARMS_DEALER_INVENTORY )
	{
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS )
		{
			if( fHatchedOut && pItemObject->exists() == false )
			{
				uiItemCost = 0;
			}
			else
			{
				// show the UNIT price, not the total value of all if stacked
				uiItemCost = CalcShopKeeperItemPrice( DEALER_SELLING, TRUE, usItemIndex, armsDealerInfo[ gbSelectedArmsDealerID ].dSellModifier, pItemObject );
			}
		}
		else // UNDER REPAIR
		{
			//display the length of time needed to repair the item
			uiItemCost = 0;

			//Get the length of time needed to fix the item
			BuildDoneWhenTimeString( zTemp, gbSelectedArmsDealerID, &(gpTempDealersInventory[ ubSlotNum ]) );
			DrawTextToScreen( zTemp, (UINT16)(usPosX+SKI_INV_PRICE_OFFSET_X), (UINT16)(usPosY+SKI_INV_PRICE_OFFSET_Y), SKI_INV_SLOT_WIDTH, SKI_ITEM_DESC_FONT, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

			//if the item belongs to a merc
			if( gpTempDealersInventory[ ubSlotNum ].ubIdOfMercWhoOwnsTheItem != NO_PROFILE )
			{
				//Display the face of the merc
				fDisplayMercFace = TRUE;
				ubMercID = gpTempDealersInventory[ ubSlotNum ].ubIdOfMercWhoOwnsTheItem;
			}
		}
	}
	else		// DEALER'S OFFER AREA
	{
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			//if the dealer repairs, there is an item here, therefore display the items owner's face
			fDisplayMercFace = TRUE;
			ubMercID = ArmsDealerOfferArea[ ubSlotNum ].ubIdOfMercWhoOwnsTheItem;

			uiItemCost = CalculateObjectItemRepairCost( gbSelectedArmsDealerID, pItemObject );
		}
		else
			uiItemCost = CalcShopKeeperItemPrice( DEALER_SELLING, FALSE, usItemIndex, armsDealerInfo[ gbSelectedArmsDealerID ].dSellModifier, pItemObject );
	}

	//if the item has a price, display it
	if( uiItemCost != 0 )
	{
		swprintf( zTemp, L"%d", uiItemCost );
		InsertCommasForDollarFigure( zTemp );

		CHAR16			zTemp2[64];
		if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
			swprintf( zTemp2, L"%s Intel", zTemp );
		else
			swprintf( zTemp2, L"$%s", zTemp );

		DrawTextToScreen( zTemp2, (UINT16)(usPosX+SKI_INV_PRICE_OFFSET_X), (UINT16)(usPosY+SKI_INV_PRICE_OFFSET_Y+4), SKI_INV_SLOT_WIDTH, SKI_ITEM_DESC_FONT, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}

	// bfant: Display item name, limited to displayable characters (space constraints)
	if ( pItem->szItemName != 0 )
	{
		wcsncpy( zTemp, pItem->szItemName, sizeof( zTemp ) / 2 );
		zTemp[63] = '\0';  // make sure it is null-terminated
		ReduceStringLength( zTemp, SKI_INV_SLOT_WIDTH - 2, BLOCKFONTNARROW );
		DrawTextToScreen( zTemp, usPosX, usPosY + 1, SKI_INV_SLOT_WIDTH, BLOCKFONTNARROW, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}

	//if the there is more then 1 or if the item is stackable and some of it has been bought and only 1 remains
	if( ( pItemObject->ubNumberOfObjects > 1 ) ||
			( ( pItemObject->ubNumberOfObjects == 1 ) &&
				( ubItemArea == ARMS_DEALER_INVENTORY ) && ( gpTempDealersInventory[ ubSlotNum ].uiFlags & ARMS_INV_ITEM_SELECTED ) ) )
	{
		swprintf( zTemp, L"x%d", pItemObject->ubNumberOfObjects );
		DrawTextToScreen( zTemp, (UINT16)(usPosX+SKI_ITEM_NUMBER_TEXT_OFFSET_X), (UINT16)(usPosY+SKI_ITEM_NUMBER_TEXT_OFFSET_Y), SKI_ITEM_NUMBER_TEXT_WIDTH, SKIT_NUMBER_FONT, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );
	}

	//if we are to display the face
	if( fDisplayMercFace )
	{
		INT8 iFaceSlot = GetSlotNumberForMerc( ubMercID );

		// if still in player's employ
		if ( iFaceSlot != -1 )
		{
			//Get and blit the face
			GetVideoObject(&hPixHandle, guiSmallSoldiersFace[ iFaceSlot ] );
			BltVideoObject(FRAME_BUFFER, hPixHandle, 0,(UINT16)(usPosX+SKI_SMALL_FACE_OFFSET_X), (UINT16)(usPosY), VO_BLT_SRCTRANSPARENCY,NULL);//SKI_SMALL_FACE_OFFSET_Y
		}
	}

	//if the item has attachments
	if( ItemHasAttachments( pItemObject ) )
	{
		//Display the '*' in the bottom right corner of the square
		swprintf( zTemp, L"*" );
		DrawTextToScreen( zTemp, (UINT16)(usPosX+SKI_ATTACHMENT_SYMBOL_X_OFFSET), (UINT16)(usPosY+SKI_ATTACHMENT_SYMBOL_Y_OFFSET), 0, TINYFONT1, FONT_GREEN, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}

	// CHRISL: if item is LBENODE
	else if( (UsingNewInventorySystem() == true) && pItemObject->HasAnyActiveLBEs() == true)
	{
		//Display the '*' in the bottom right corner of the square
		swprintf( zTemp, L"*" );
		DrawTextToScreen( zTemp, (UINT16)(usPosX+SKI_ATTACHMENT_SYMBOL_X_OFFSET), (UINT16)(usPosY+SKI_ATTACHMENT_SYMBOL_Y_OFFSET), 0, TINYFONT1, FONT_BLUE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );	
	}

	// Display 'JAMMED' if it's jammed
	if ( (*pItemObject)[0]->data.gun.bGunAmmoStatus < 0 )
	{
		swprintf( zTemp, TacticalStr[ JAMMED_ITEM_STR ] );
		VarFindFontCenterCoordinates( usPosX, usPosY, SKI_INV_SLOT_WIDTH, SKI_INV_HEIGHT, TINYFONT1, &sCenX, &sCenY, zTemp );
		DrawTextToScreen( zTemp, sCenX, sCenY, SKI_INV_SLOT_WIDTH, TINYFONT1, FONT_RED, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}
	else if ( fPrintRepaired )
	{
		swprintf( zTemp, SKI_Text[ SKI_TEXT_REPAIRED ] );
		VarFindFontCenterCoordinates( usPosX, usPosY, SKI_INV_SLOT_WIDTH, SKI_INV_HEIGHT, TINYFONT1, &sCenX, &sCenY, zTemp );
		DrawTextToScreen( zTemp, sCenX, sCenY, SKI_INV_SLOT_WIDTH, TINYFONT1, FONT_RED, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}

	if( fHatchedOut )
	{
		HatchOutInvSlot( usPosX, usPosY );
	}

	InvalidateRegion( usPosX, usPosY, usPosX+SKI_INV_SLOT_WIDTH, usPosY+SKI_INV_SLOT_HEIGHT );

	return( uiItemCost );
}

BOOLEAN DetermineArmsDealersSellingInventory( )
{
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("DEF: DetermineArmsDealersSellingInventory") );

	//allocate memory to hold the inventory in memory
	gpTempDealersInventory.clear();
	guiNextFreeInvSlot = 0;

	//loop through the dealer's permanent inventory items, adding them all to the temp inventory list
	for (DealerItemList::iterator iter = gArmsDealersInventory[ gbSelectedArmsDealerID ].begin();
		iter != gArmsDealersInventory[ gbSelectedArmsDealerID ].end();) {

		if ( iter->ItemIsInInventory() == false) {
			++iter;
			continue;
		}

		//shadooow: do not sell any item that is limited to specific system and this system isn't enabled
		if (((Item[iter->object.usItem].usLimitedToSystem & FOOD_SYSTEM_FLAG) && !UsingFoodSystem()) || ((Item[iter->object.usItem].usLimitedToSystem & DISEASE_SYSTEM_FLAG) && !gGameExternalOptions.fDisease))
		{
			++iter;
			continue;
		}

		bool increment = true;
		if (ItemIsSpecial(*iter) == false) {
			StoreObjectsInNextFreeDealerInvSlot( &(*iter), gpTempDealersInventory, gbSelectedArmsDealerID );
		}
		else
		{
			//if the object has attachments, or is damaged, or otherwise special, then it is not stacked
			bool fAddSpecialItem = true;
			bool underRepair = false;
			bool repaired = false;

			if( iter->IsUnderRepair() == true )
			{
				underRepair = true;
				//if the repairs are done
				if( iter->uiRepairDoneTime <= GetWorldTotalMin() )
				{
					if (RepairIsDone( &(*iter) ))
					{
						//ADB moved from RepairIsDone so iterator remains ok
						// Remove the repaired item from the dealer's permanent inventory list
						iter = gArmsDealersInventory[ gbSelectedArmsDealerID ].erase(iter);
						if (iter == gArmsDealersInventory[ gbSelectedArmsDealerID ].end()) {
							break;
						}
						increment = false;

						// don't add it here, it was put in the player's area
						fAddSpecialItem = false;
					}
					else
					{
						repaired = true;
					}
				}
			}

			if ( fAddSpecialItem == true )
			{
				UINT8 ubOwner;
				if ( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS )
				{
					// no merc is the owner
					ubOwner = NO_PROFILE;
				}
				else
				{
					// retain owner so we can display this
					ubOwner = iter->ubOwnerProfileId;
				}

				StoreObjectsInNextFreeDealerInvSlot( &(*iter), gpTempDealersInventory, ubOwner );
				if (underRepair == true) {
					if (repaired == true) {
						gpTempDealersInventory.back().uiFlags |= ARMS_INV_ITEM_REPAIRED;
					}
					else {
						gpTempDealersInventory.back().uiFlags |= ARMS_INV_ITEM_NOT_REPAIRED_YET;
					}
				}
			}
		}
		if (increment == true) {
			++iter;
		}
	}

	// repairmen sort differently from merchants
	// sort this list by object category, and by ascending price within each category
	if ( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
	{
		std::sort(gpTempDealersInventory.begin(), gpTempDealersInventory.end(), RepairmanItemQsortCompare);//RepairmanItemQsortCompare
	}
	else
	{
		std::sort(gpTempDealersInventory.begin(), gpTempDealersInventory.end(), ArmsDealerItemQsortCompare);//ArmsDealerItemQsortCompare
	}
	return( TRUE );
}

bool ArmsDealerItemQsortCompare(INVENTORY_IN_SLOT& pInvSlot1, INVENTORY_IN_SLOT& pInvSlot2)
{
	int retVal = ( CompareItemsForSorting( pInvSlot1.sItemIndex, pInvSlot2.sItemIndex,
		pInvSlot1.ItemObject[0]->data.objectStatus, pInvSlot2.ItemObject[0]->data.objectStatus ) );
	if (retVal == -1) {
		return true;
	}
	return false;
}

bool RepairmanItemQsortCompare(INVENTORY_IN_SLOT& pInvSlot1, INVENTORY_IN_SLOT& pInvSlot2)
{
	// lower repair time first
	if ( pInvSlot1.uiRepairDoneTime < pInvSlot2.uiRepairDoneTime )
	{
		return true;
	}
	return false;
}

void StoreObjectsInNextFreeDealerInvSlot( DEALER_SPECIAL_ITEM *pSpclItemInfo, std::vector<INVENTORY_IN_SLOT>& pInventory, UINT8 ubOwner )
{
	pInventory.push_back(INVENTORY_IN_SLOT());
	INVENTORY_IN_SLOT* pDealerInvSlot = &(pInventory.back());
	pDealerInvSlot->fActive = TRUE;
	pDealerInvSlot->sItemIndex = pSpclItemInfo->object.usItem;
	pDealerInvSlot->uiRepairDoneTime = pSpclItemInfo->uiRepairDoneTime;
	pDealerInvSlot->ubIdOfMercWhoOwnsTheItem = ubOwner;
	pDealerInvSlot->bSlotIdInOtherLocation = -1;

	// Create the item object ( with no more than MAX_OBJECTS_PER_SLOT )
	// can't use the real #, because CreateItems() will blindly set the bStatus for however many we tell it, beyond 8
	MakeObjectOutOfDealerItems( pSpclItemInfo, &(pDealerInvSlot->ItemObject) );

	//ADB no longer necessary
	/*
	if ( ubHowMany > MAX_OBJECTS_PER_SLOT )
	{
		// HACK:  Override ItemObject->ubNumberOfObjects (1-8) with the REAL # of items in this box.
		// Note that this makes it an illegal OBJECTTYPE, since there ubHowMany can easily be more than MAX_OBJECTS_PER_SLOT,
		// so there's no room to store the status of all of them one.  But we only so this for perfect items, so
		// we don't care, it works & saves us a lot hassles here.  Just be careful using the damn things!!!  You can't just
		// pass them off the most functions in Items.C(), use ShopkeeperAutoPlaceObject() and ShopkeeperAddItemToPool() instead.
		pDealerInvSlot->ItemObject.ubNumberOfObjects = ubHowMany;
	}
	*/
}

BOOLEAN RepairIsDone(DEALER_SPECIAL_ITEM* pSpecial)
{

	// make a new shopkeeper invslot item out of it
	INVENTORY_IN_SLOT	RepairItem;

	RepairItem.fActive = TRUE;
	RepairItem.sItemIndex = pSpecial->object.usItem;

	// set the owner of the item.  Slot is always -1 of course.
	RepairItem.ubIdOfMercWhoOwnsTheItem = pSpecial->ubOwnerProfileId;
	RepairItem.bSlotIdInOtherLocation = -1;

	// Create the item object
	MakeObjectOutOfDealerItems( pSpecial, &RepairItem.ItemObject );

	if ( CanDealerRepairItem( gbSelectedArmsDealerID, RepairItem.ItemObject.usItem ) )
	{
		// make its condition 100%
		RepairItem.ItemObject[0]->data.objectStatus = 100;

		// Flugente: shopkeepers will also restore an item's repair threshold
		RepairItem.ItemObject[0]->data.sRepairThreshold = 100;
	}

	// max condition of all permanent attachments on it
	for (attachmentList::iterator iter = RepairItem.ItemObject[0]->attachments.begin(); iter != RepairItem.ItemObject[0]->attachments.end(); ++iter) {
		if ( CanDealerRepairItem( gbSelectedArmsDealerID, iter->usItem ) && iter->exists())
		{
			// fix it up
			(*iter)[0]->data.objectStatus = 100;

			// Flugente: shopkeepers will also restore an item's repair threshold and clean it
			(*iter)[0]->data.sRepairThreshold = 100;
		}
	}

#ifdef JA2UB
	//no UB
#else
	// if the item is imprinted (by anyone, even player's mercs), and it's Fredo repairing it
	if ( ( gbSelectedArmsDealerID == ARMS_DEALER_FREDO ) )
	{
		// then reset the imprinting!
		RepairItem.ItemObject[0]->data.ubImprintID = NO_PROFILE;
	}
#endif
	//try to add the item to the players offer area
	INT8		bSlotNum;
	bSlotNum = AddItemToPlayersOfferArea( RepairItem.ubIdOfMercWhoOwnsTheItem, &RepairItem, -1 );
	// if there wasn't room for it
	if( bSlotNum == -1 )
	{
		// then we have to treat it like it's not done yet!
		return(FALSE);
	}

	PlayersOfferArea[ bSlotNum ].uiFlags |= ARMS_INV_ITEM_REPAIRED;
	// there was room to add it to player's area
	return(TRUE);
}

void DrawHatchOnInventory( UINT32 uiSurface, UINT16 usPosX, UINT16 usPosY, UINT16 usWidth, UINT16 usHeight, UINT16 usColor )
{
	UINT8	 *pDestBuf;
	UINT32 uiDestPitchBYTES;
	SGPRect ClipRect;
	static UINT8 Pattern[8][8] =
	{
		1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,0,
		0,1,0,1,0,1,0,1
	};
	// CHRISL:
	ClipRect.iLeft = usPosX-1;
	ClipRect.iRight = usPosX + usWidth-1;
	ClipRect.iTop = usPosY;
	ClipRect.iBottom = usPosY + usHeight;

	pDestBuf = LockVideoSurface( uiSurface, &uiDestPitchBYTES );
	if(usColor == 0){
		Blt16BPPBufferPixelateRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, Pattern );
	}
	else{
		Blt16BPPBufferPixelateRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, Pattern, usColor );
	}
	UnLockVideoSurface( uiSurface );
}

void DrawHatchOnInventory_MilitiaAccess( UINT32 uiSurface, UINT16 usPosX, UINT16 usPosY, UINT16 usWidth, UINT16 usHeight, UINT16 usColor )
{
	UINT8	 *pDestBuf;
	UINT32 uiDestPitchBYTES;
	SGPRect ClipRect;
	static UINT8 Pattern[8][8] =
	{
		1,0,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,
		0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,1,
		0,0,0,0,1,0,0,0,
		0,1,0,0,0,0,0,0,
		0,0,0,0,0,0,1,0,
		0,0,0,1,0,0,0,0
	};
	// CHRISL:
	ClipRect.iLeft = usPosX-1;
	ClipRect.iRight = usPosX + usWidth-1;
	ClipRect.iTop = usPosY;
	ClipRect.iBottom = usPosY + usHeight;

	pDestBuf = LockVideoSurface( uiSurface, &uiDestPitchBYTES );	
	if(usColor == 0){
		Blt16BPPBufferPixelateRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, Pattern );
	}
	else{
		Blt16BPPBufferPixelateRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, Pattern, usColor );
	}
	UnLockVideoSurface( uiSurface );
}

UINT32 CalcShopKeeperItemPrice( BOOLEAN fDealerSelling, BOOLEAN fUnitPriceOnly, UINT16 usItemID, FLOAT dModifier, OBJECTTYPE *pItemObject )
{
	UINT8		ubCnt;
	UINT32	uiUnitPrice = 0;
	UINT32	uiTotalPrice = 0;
//	UINT32	uiDifFrom10 = 0;
#ifdef JA2UB
UINT8		ubItemsNotCounted = 0; //ja25 UB
#endif
	std::vector<UINT32>	uiItemPrice;

	uiItemPrice.resize(pItemObject->ubNumberOfObjects);

	for (ubCnt = 0; ubCnt < pItemObject->ubNumberOfObjects; ++ubCnt )
	{
		uiItemPrice[ubCnt] = 0;
		// add up value of the main item(s), exact procedure depends on its item class
		switch ( Item [ usItemID ].usItemClass )
		{
			case IC_GUN:
				// add value of the gun
				uiItemPrice[ubCnt] += (UINT32)( CalcValueOfItemToDealer( gbSelectedArmsDealerID, usItemID, fDealerSelling ) *
											 ItemConditionModifier(usItemID, (*pItemObject)[ubCnt]->data.gun.bGunStatus) *
											 dModifier );

				// if any ammo is loaded
				if( (*pItemObject)[ubCnt]->data.gun.usGunAmmoItem != NONE)
				{
					// if it's regular ammo
					if( Item[ (*pItemObject)[ubCnt]->data.gun.usGunAmmoItem ].usItemClass == IC_AMMO )
					{
						// add value of its remaining ammo
						uiItemPrice[ubCnt] += (UINT32)( CalcValueOfItemToDealer( gbSelectedArmsDealerID, (*pItemObject)[ubCnt]->data.gun.usGunAmmoItem, fDealerSelling ) *
																			 ItemConditionModifier((*pItemObject)[ubCnt]->data.gun.usGunAmmoItem, (*pItemObject)[ubCnt]->data.gun.ubGunShotsLeft) *
																			 dModifier );
					}
					else	// assume it's attached ammo (mortar shells, grenades)
					{
						// add its value (uses normal status 0-100)
						uiItemPrice[ubCnt] += (UINT32)( CalcValueOfItemToDealer( gbSelectedArmsDealerID, (*pItemObject)[ubCnt]->data.gun.usGunAmmoItem, fDealerSelling ) *
																			 ItemConditionModifier((*pItemObject)[ubCnt]->data.gun.usGunAmmoItem, (*pItemObject)[ubCnt]->data.gun.bGunAmmoStatus) *
																			 dModifier );
					}
				}
				break;

			case IC_AMMO:
				// add the value of each magazine (multiple mags may have vastly different #bullets left)
					// for bullets, ItemConditionModifier will convert the #ShotsLeft into a percentage
					uiItemPrice[ubCnt] += (UINT32)( CalcValueOfItemToDealer( gbSelectedArmsDealerID, usItemID, fDealerSelling ) *
																		 ItemConditionModifier(usItemID, (*pItemObject)[ubCnt]->data.ubShotsLeft) *
																		 dModifier );

				break;
			default:
				// add the value of each magazine (multiple mags may have vastly different #bullets left)
				// for bullets, ItemConditionModifier will convert the #ShotsLeft into a percentage
				uiItemPrice[ubCnt] += (UINT32)( CalcValueOfItemToDealer( gbSelectedArmsDealerID, usItemID, fDealerSelling ) *
																	 ItemConditionModifier(usItemID, (*pItemObject)[ ubCnt ]->data.objectStatus) *
																	 dModifier );

				break;
		}
		// loop through any attachments and add in their price
		for (attachmentList::iterator iter = (*pItemObject)[ubCnt]->attachments.begin(); iter != (*pItemObject)[ubCnt]->attachments.end(); ++iter) {
			// add value of this particular attachment
			if(iter->exists())
				uiItemPrice[ubCnt] += CalcShopKeeperItemPrice( fDealerSelling, fUnitPriceOnly, iter->usItem, dModifier, &(*iter)) ;
		}

		// Flugente: used in backgrounds now, old code in comments for historical reasons
		/*// if Flo is doing the dealin' and wheelin'
		if ( gpSMCurrentMerc->ubProfile == FLO )
		{
			// if it's a GUN or AMMO (but not Launchers, and all attachments and payload is included)
			switch ( Item [ usItemID ].usItemClass )
			{
				case IC_GUN:
				case IC_AMMO:
					uiDiscountValue = ( uiItemPrice[ubCnt] * FLO_DISCOUNT_PERCENTAGE ) / 100;

					// she gets a discount!  Read her M.E.R.C. profile to understand why
					if ( fDealerSelling )
					{
						// she buys for less...
						uiItemPrice[ubCnt] -= uiDiscountValue;
					}
					else
					{
						// and sells for more!
						uiItemPrice[ubCnt] += uiDiscountValue;
					}
					break;
			}
		}*/

		// Flugente: backgrounds can alter prices		
		INT16 pricepercentage = 100;

		// if it's a GUN or AMMO (but not Launchers, and all attachments and payload is included)
		if ( Item [ usItemID ].usItemClass & (IC_GUN|IC_AMMO) )
		{
			pricepercentage = max( 0, 100 + (fDealerSelling ? -1 : 1) * gpSMCurrentMerc->GetBackgroundValue( BG_PERC_PRICES_GUNS ) );

			// Even without backgrounds, Flo gets a discount. Read her M.E.R.C. profile to understand why
			if ( !UsingBackGroundSystem() && gpSMCurrentMerc->ubProfile == FLO )
				pricepercentage = max( 0, 100 + (fDealerSelling ? -1 : 1) * FLO_DISCOUNT_PERCENTAGE );
		}
		else if ( Item [ usItemID ].usItemClass & (IC_MAPFILTER_MELEE|IC_MAPFILTER_KIT|IC_MAPFILTER_LBE|IC_MAPFILTER_ARMOR|IC_MAPFILTER_MISC) )
		{
			pricepercentage = max( 0, 100 + (fDealerSelling ? -1 : 1) * gpSMCurrentMerc->GetBackgroundValue( BG_PERC_PRICES ) );
		}

		uiItemPrice[ubCnt] = (uiItemPrice[ubCnt] * pricepercentage) / 100;

		// if it's the dealer selling this, make sure the item is worth at least $1
		// if he is buying this from a player, then we allow a value of 0, since that has a special "worthless" quote #18
		if( fDealerSelling && ( uiItemPrice[ubCnt] == 0 ) )
		{
			uiItemPrice[ubCnt] = 1;
		}

/*
		//if the price is not diviseble by 10, make it so
		uiDifFrom10 = 10 - uiItemPrice[ubCnt] % 10;
		if( uiDifFrom10 != 0 && uiDifFrom10 != 10 )
		{
			uiItemPrice[ubCnt] += uiDifFrom10;
		}
*/
	}

	// if we want the unit price, find the highest price in the stack
	for (ubCnt = 0; ubCnt < pItemObject->ubNumberOfObjects; ++ubCnt )
	{
		if(fUnitPriceOnly == TRUE)
		{
			uiUnitPrice = __max(uiUnitPrice, uiItemPrice[ubCnt]);
		}
		else
		{
			uiUnitPrice += uiItemPrice[ubCnt];
		}
	}

	// we're always count the first one
	uiTotalPrice = uiUnitPrice;
	
#ifdef JA2UB	
	//---------------------------JA25 UB---------------
	// if NOT pricing just one
	if ( !fUnitPriceOnly )
	{
		// add value of all that weren't already counted
		uiTotalPrice += ( ubItemsNotCounted * uiUnitPrice );
	}

	//if the dealer is Raul
	if( gbSelectedArmsDealerID == ARMS_DEALER_RAUL )
	{
		//if the item is the antique musket
		if( usItemID == HAND_CANNON ) //4498
		{
			//if the "Find anitque musket" quest is active
			if( gubQuest[ QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL ] == QUESTINPROGRESS )
			{
				//the price should be.... nothin ( reward for the quest )
				uiTotalPrice = 1;

				//if the player hasnt said the quote before
				if( !( gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_48 ) )
				{
					StartShopKeeperTalking( SK_QUOTES_NPC_SPECIFIC_48 );
					gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags |= ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_48;
					gfCanSayMakeTransactionQuote = FALSE;
				}

				//else if 
				if( gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_49 )
				{
					//if the player hasnt said it before
					if( !( gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_50 ) )
					{
						StartShopKeeperTalking( SK_QUOTES_NPC_SPECIFIC_50 );
						gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags |= ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_50;
					}

					//the price should be
					uiTotalPrice = 100;
				}
			}
		}

		//if the item is the barrat
		if( usItemID == BARRETT_UB )
		{
			//and if the player found the antique musket for Raul
			if( IsJa25GeneralFlagSet( JA_GF__BARRETT_IS_HALF_PRICE ) )
			{
				//the barrett is half off
				uiTotalPrice /= 2;
			}
		}
	}

        //JA25UB
	//if the dealer is Raul
	if( gbSelectedArmsDealerID == ARMS_DEALER_BETTY )
	{
		//if the item is the antique musket
		if( usItemID == TEX_MOVIE_ATTACK_CLYDESDALES || usItemID == TEX_MOVIE_WILD_EAST || usItemID == TEX_MOVIE_HAVE_HONDA )
	//	if( usItemID == 1356 || usItemID == 1357 || usItemID == 1358 )
		{
			//Make the item price top be what is listed in the item table ( 20 )
			uiTotalPrice = Item[ usItemID ].usPrice;
		}
	}

#endif	
	return( uiTotalPrice );
}

FLOAT ItemConditionModifier(UINT16 usItemIndex, INT16 bStatus)
{
	FLOAT dConditionModifier = 1.0f;

	//if the item is ammo, the condition modifier is based on how many shots are left
	if( Item[ usItemIndex ].usItemClass == IC_AMMO )
	{
		// # bullets left / max magazine capacity
		dConditionModifier = ( bStatus / (FLOAT) Magazine[ Item[ usItemIndex ].ubClassIndex ].ubMagSize );
	}
	else	// non-ammo
	{
		// handle non-ammo payloads in jammed guns as if they weren't
		if ( bStatus < 0 )
		{
			bStatus *= -1;
		}

		// an item at 100% is worth full price...

//		if ( Item[ usItemIndex ].fFlags & ITEM_REPAIRABLE )
		if ( Item[ usItemIndex ].repairable  )
		{
			// a REPAIRABLE item at 0% is still worth 50% of its full price, not 0%
			dConditionModifier = 0.5f + ( bStatus / (FLOAT)200 );
		}
		else
		{
			// an UNREPAIRABLE item is worth precisely its condition percentage
			dConditionModifier = bStatus / (FLOAT)100;
		}
	}

	return( dConditionModifier );
}

void DisplayArmsDealerOfferArea()
{
	INT16		sCnt, sCount;
	CHAR16	zTemp[32];
	UINT32	uiTotalCost;
	UINT16	usPosX, usPosY;
	BOOLEAN fDisplayHatchOnItem;
	
	usPosX = SKI_TRADER_OFFER_BOXES_BEGIN_X;
	usPosY = SKI_TRADER_OFFER_BOXES_BEGIN_Y;

	sCount = 0;

	//Display all the items that are in the Arms traders offered area
	uiTotalCost = 0;
	for ( sCnt = 0; sCnt<gDealersOfferActiveRegions; ++sCnt )
	{
		if( ArmsDealerOfferArea[ sCnt ].fActive )
		{
			// hatch it out if region is disabled
			if ( !( gDealersOfferSlotsMouseRegions[ sCnt ].uiFlags & MSYS_REGION_ENABLED ) )
			{
				fDisplayHatchOnItem = TRUE;
			}
			else if( ArmsDealerOfferArea[ sCnt ].uiFlags & ARMS_INV_ITEM_SELECTED )
			{
				fDisplayHatchOnItem = TRUE;
			}
			else
				fDisplayHatchOnItem = FALSE;

			// Display the inventory slot
			uiTotalCost += DisplayInvSlot( (UINT16)sCnt, ArmsDealerOfferArea[ sCnt ].sItemIndex, usPosX, usPosY,
											&ArmsDealerOfferArea[ sCnt ].ItemObject,
											fDisplayHatchOnItem,
											ARMS_DEALER_OFFER_AREA );
		}
		else	// empty
		{
			// hatch it out if region is disabled
			if ( !( gDealersOfferSlotsMouseRegions[ sCnt ].uiFlags & MSYS_REGION_ENABLED ) )
			{
				HatchOutInvSlot( usPosX, usPosY );
			}
		}

		usPosX += SKI_INV_OFFSET_X;

		++sCount;

		//if we are on to the next row
		if ( !(sCount % SKI_TRADER_OFFER_BOXES_PER_ROW) )
		{
			usPosX = SKI_TRADER_OFFER_BOXES_BEGIN_X;
			usPosY += SKI_INV_OFFSET_Y;
		}
	}

	if( gubSkiDirtyLevel == SKI_DIRTY_LEVEL2 )
	{
		//Display the total cost text
		swprintf( zTemp, L"%d", uiTotalCost );
		InsertCommasForDollarFigure( zTemp );

		CHAR16			zTemp2[64];
		if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
			swprintf( zTemp2, L"%s Intel", zTemp );
		else
			swprintf( zTemp2, L"$%s", zTemp );

		DrawTextToScreen( zTemp2, SKI_ARMS_DEALER_TOTAL_COST_X, (UINT16)(SKI_ARMS_DEALER_TOTAL_COST_Y+5), SKI_INV_SLOT_WIDTH, SKI_LABEL_FONT, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
}

INT16 AddItemToArmsDealerOfferArea( INVENTORY_IN_SLOT* pInvSlot, INT16 bSlotIdInOtherLocation )
{
	for ( INT16 bCnt = 0; bCnt<gDealersOfferActiveRegions; ++bCnt )
	{
		//if there are no items here, copy the data in
		if( ArmsDealerOfferArea[bCnt].fActive == FALSE )
		{
			//Copy the inventory items
			ArmsDealerOfferArea[bCnt] = *pInvSlot;

			//if the shift key is being pressed, add them all
			if( gfKeyState[ SHIFT ] ) {
				//nothing needed
			}
			//If there was more then 1 item, reduce it to only 1 item moved
			else if( pInvSlot->ItemObject.ubNumberOfObjects > 1 ) {
				ArmsDealerOfferArea[bCnt].ItemObject.RemoveObjectsFromStack(ArmsDealerOfferArea[bCnt].ItemObject.ubNumberOfObjects - 1);
			}

			//Remember where the item came from
			ArmsDealerOfferArea[bCnt].bSlotIdInOtherLocation = bSlotIdInOtherLocation;
			ArmsDealerOfferArea[bCnt].ubIdOfMercWhoOwnsTheItem = pInvSlot->ubIdOfMercWhoOwnsTheItem;
			ArmsDealerOfferArea[bCnt].fActive = TRUE;

			SetSkiRegionHelpText( &ArmsDealerOfferArea[bCnt], &gDealersOfferSlotsMouseRegions[ bCnt ], ARMS_DEALER_OFFER_AREA );
			SetSkiFaceRegionHelpText( &ArmsDealerOfferArea[bCnt], &gDealersOfferSlotsSmallFaceMouseRegions[ bCnt ], ARMS_DEALER_OFFER_AREA );

			gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			return( bCnt );
		}
	}

	return( -1 );
}

BOOLEAN RemoveItemFromArmsDealerOfferArea( INT16	bSlotId, BOOLEAN fKeepItem )
{
	//if this is the requested slot
	if( ArmsDealerOfferArea[bSlotId].fActive )
	{
		if( fKeepItem )
		{
			gpTempDealersInventory[ ArmsDealerOfferArea[bSlotId].bSlotIdInOtherLocation ].ItemObject.ForceAddObjectsToStack(ArmsDealerOfferArea[ bSlotId ].ItemObject);
		}

		//Clear the flag that hatches out the item
		gpTempDealersInventory[ ArmsDealerOfferArea[bSlotId].bSlotIdInOtherLocation ].uiFlags &= ~ARMS_INV_ITEM_SELECTED;

		ClearArmsDealerOfferSlot( bSlotId );

		return( TRUE );
	}

	return( FALSE );
}

void SetSkiRegionHelpText( INVENTORY_IN_SLOT *pInv, MOUSE_REGION *pRegion, UINT8 ubScreenArea )
{
	CHAR16 zHelpText[ 512 ];

	Assert( pRegion );

	BuildItemHelpTextString( zHelpText, pInv, ubScreenArea );

	SetRegionFastHelpText( pRegion, zHelpText );
	SetRegionHelpEndCallback( pRegion, SkiHelpTextDoneCallBack );
}

void SetSkiFaceRegionHelpText( INVENTORY_IN_SLOT *pInv, MOUSE_REGION *pRegion, UINT8 ubScreenArea )
{
	CHAR16 zTempText[ 512 ];
	CHAR16 zHelpText[ 512 ];

	Assert( pRegion );

	//if the item isn't NULL, and is owned by a merc
	if( ( pInv != NULL ) && ( pInv->ubIdOfMercWhoOwnsTheItem != NO_PROFILE ) )
	{
		BuildItemHelpTextString( zTempText, pInv, ubScreenArea );
		// add who owns it
		swprintf( zHelpText, L"%s%s %s", gMercProfiles[ pInv->ubIdOfMercWhoOwnsTheItem ].zNickname, pMessageStrings[ MSG_DASH_S ], zTempText );
	}
	else
	{
		zHelpText[ 0 ] = '\0';
	}

	SetRegionFastHelpText( pRegion, zHelpText );
	SetRegionHelpEndCallback( pRegion, SkiHelpTextDoneCallBack );
}

void SkiHelpTextDoneCallBack( void )
{
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}

INT8 AddItemToPlayersOfferArea( UINT8 ubProfileID, INVENTORY_IN_SLOT* pInvSlot, INT16 bSlotIdInOtherLocation )
{
	//if we are to check for a previous slot
	if( bSlotIdInOtherLocation != -1 )
	{
		//if the item has already been added somewhere. don't add it again
		if( GetPtrToOfferSlotWhereThisItemIs( ubProfileID, bSlotIdInOtherLocation ) != NULL )
			return( -1 );
	}

	//look for the first free slot
	for ( int bCnt = 0; bCnt<gPlayersOfferActiveRegions; ++bCnt )
	{
		//if there are no items here, copy the data in
		if( PlayersOfferArea[bCnt].fActive == FALSE )
		{
			PlayersOfferArea[bCnt] = *pInvSlot;

			PlayersOfferArea[bCnt].fActive = TRUE;

			//Specify the owner of the merc
			PlayersOfferArea[bCnt].ubIdOfMercWhoOwnsTheItem = ubProfileID;
			PlayersOfferArea[bCnt].bSlotIdInOtherLocation = bSlotIdInOtherLocation;

			IfMercOwnedCopyItemToMercInv( &(PlayersOfferArea[bCnt]) );

			SetSkiRegionHelpText( &PlayersOfferArea[bCnt], &gPlayersOfferSlotsMouseRegions[ bCnt ], PLAYERS_OFFER_AREA );
			SetSkiFaceRegionHelpText( &PlayersOfferArea[bCnt], &gPlayersOfferSlotsSmallFaceMouseRegions[ bCnt ], PLAYERS_OFFER_AREA );

			//if the item we are adding is money
			if( Item[ PlayersOfferArea[ bCnt ].sItemIndex ].usItemClass == IC_MONEY && !( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL ) )
			{
				//Since money is always evaluated
				PlayersOfferArea[ bCnt ].uiFlags |= ARMS_INV_PLAYERS_ITEM_HAS_VALUE;
				PlayersOfferArea[ bCnt ].uiItemPrice = PlayersOfferArea[ bCnt ].ItemObject[0]->data.money.uiMoneyAmount;
			}
			gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

			return( bCnt );
		}
	}

	return( -1 );
}

BOOLEAN RemoveItemFromPlayersOfferArea( INT16 bSlot )
{
	//if the item doesn't have a duplicate copy in its owner merc's inventory slot
	if( PlayersOfferArea[ bSlot ].bSlotIdInOtherLocation == -1 )
	{
		if ( !SKITryToReturnInvToOwnerOrCurrentMerc( &( PlayersOfferArea[ bSlot ] ) ) )
		{
			//failed to add item, inventory probably filled up or item is unowned and current merc ineligible
			return( FALSE );
		}
	}

	// Clear the contents
	ClearPlayersOfferSlot( bSlot );

	// Dirty
	fInterfacePanelDirty = DIRTYLEVEL2;
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;
	return( TRUE );
}

void DisplayPlayersOfferArea()
{
	INT16	sCount = 0;
	CHAR16	zTemp[32];
	UINT32	uiTotalCost;
	UINT16	usPosX, usPosY;
	BOOLEAN	fDisplayHatchOnItem=FALSE;
	HVOBJECT hHandle;

	// load the "cross out" graphic
	GetVideoObject( &hHandle, guiItemCrossOut );
	
	usPosX = SKI_PLAYER_OFFER_BOXES_BEGIN_X;
	usPosY = SKI_PLAYER_OFFER_BOXES_BEGIN_Y;
		
	//Display all the items that are in the Players offered area
	uiTotalCost = 0;
	for ( int sCnt = 0; sCnt < gPlayersOfferActiveRegions; ++sCnt )
	{
		if( PlayersOfferArea[ sCnt ].fActive )
		{
			//if the item is money
			if( Item[ PlayersOfferArea[ sCnt ].sItemIndex ].usItemClass == IC_MONEY )
			{
				//get an updated status from the amount in the pocket
				if( PlayersOfferArea[ sCnt ].bSlotIdInOtherLocation != -1 && PlayersOfferArea[ sCnt ].ubIdOfMercWhoOwnsTheItem != NO_PROFILE )
				{
					INT16 sSoldierID;

					sSoldierID = GetSoldierIDFromMercID( PlayersOfferArea[ sCnt ].ubIdOfMercWhoOwnsTheItem );
					Assert(sSoldierID != -1);

					PlayersOfferArea[ sCnt ].ItemObject[0]->data.money.uiMoneyAmount = Menptr[ sSoldierID ].inv[ PlayersOfferArea[ sCnt ].bSlotIdInOtherLocation ][0]->data.money.uiMoneyAmount;
					PlayersOfferArea[ sCnt ].uiItemPrice = PlayersOfferArea[ sCnt ].ItemObject[0]->data.money.uiMoneyAmount;
				}
			}
			else	// not money
			{
				//if non-repairman
				if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS )
				{
					// don't evaluate anything he wouldn't buy!
					if( WillShopKeeperRejectObjectsFromPlayer( gbSelectedArmsDealerID, ( INT8 ) sCnt ) == FALSE )
					{
						// skip purchased items!
						if ( !( PlayersOfferArea[ sCnt ].uiFlags & ARMS_INV_JUST_PURCHASED ) )
						{
							// re-evaluate the value of the item (needed for Flo's discount handling)
							EvaluateInvSlot( &( PlayersOfferArea[ sCnt ] ) );
						}
					}
				}
			}

			// Display the inventory slot
			DisplayInvSlot( (UINT16)sCnt, PlayersOfferArea[ sCnt ].sItemIndex, usPosX, usPosY,
											&PlayersOfferArea[ sCnt ].ItemObject,
											fDisplayHatchOnItem,
											PLAYERS_OFFER_AREA );

			if( PlayersOfferArea[ sCnt ].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_VALUE )
				uiTotalCost += PlayersOfferArea[ sCnt ].uiItemPrice;

			// cross out unwanted items
			if ( !(PlayersOfferArea[sCnt].uiFlags & ARMS_INV_JUST_PURCHASED) )
			{
				//If item can't be sold here, or it's completely worthless (very cheap / very broken)
				if ( (WillShopKeeperRejectObjectsFromPlayer( gbSelectedArmsDealerID, sCnt ) == TRUE) || !(PlayersOfferArea[sCnt].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_VALUE) )
				{
					BltVideoObject( FRAME_BUFFER, hHandle, 0, (usPosX + 22), (usPosY), VO_BLT_SRCTRANSPARENCY, NULL );

					// invalidate the region
					InvalidateRegion( usPosX - 1, usPosY - 1, usPosX + SKI_INV_SLOT_WIDTH + 1, usPosY + SKI_INV_SLOT_HEIGHT + 1 );
				}
			}
		}

		usPosX += SKI_INV_OFFSET_X;

		++sCount;

		//if we are on to the next row
		if ( !(sCount % SKI_TRADER_OFFER_BOXES_PER_ROW) )
		{
			usPosX = SKI_PLAYER_OFFER_BOXES_BEGIN_X;
			usPosY += SKI_INV_OFFSET_Y;
		}
	}

	if( gubSkiDirtyLevel == SKI_DIRTY_LEVEL2 )
	{
		//Display the total cost text
		swprintf( zTemp, L"%d", uiTotalCost );
		InsertCommasForDollarFigure( zTemp );
		
		CHAR16			zTemp2[64];
		if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
			swprintf( zTemp2, L"%s Intel", zTemp );
		else
			swprintf( zTemp2, L"$%s", zTemp );

		DrawTextToScreen( zTemp2, SKI_TOTAL_VALUE_X, SKI_TOTAL_VALUE_OFFSET_TO_VALUE, SKI_INV_SLOT_WIDTH, SKI_LABEL_FONT, SKI_ITEM_PRICE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
}

INVENTORY_IN_SLOT	*GetPtrToOfferSlotWhereThisItemIs( UINT8 ubProfileID, INT16 bInvPocket )
{
	for ( int ubCnt = 0; ubCnt < gPlayersOfferActiveRegions; ++ubCnt )
	{
		if( ( PlayersOfferArea[ ubCnt ].bSlotIdInOtherLocation == bInvPocket ) &&
				( PlayersOfferArea[ ubCnt ].ubIdOfMercWhoOwnsTheItem == ubProfileID ) &&
				( PlayersOfferArea[ ubCnt ].ItemObject.exists() == true ) )
		{
			return( &( PlayersOfferArea[ ubCnt ] ) );
		}
	}

	for ( int ubCnt = 0; ubCnt < gDealersOfferActiveRegions; ++ubCnt )
	{
		if ( (ArmsDealerOfferArea[ubCnt].bSlotIdInOtherLocation == bInvPocket) &&
			 (ArmsDealerOfferArea[ubCnt].ubIdOfMercWhoOwnsTheItem == ubProfileID) &&
			 (ArmsDealerOfferArea[ubCnt].ItemObject.exists( ) == true) )
		{
			return(&(ArmsDealerOfferArea[ubCnt]));
		}
	}

	// not found (perfectly valid result)
	return( NULL );
}

BOOLEAN ShouldSoldierDisplayHatchOnItem( UINT8 ubProfileID, INT16 sSlotNum )
{
	INVENTORY_IN_SLOT	*pInvSlot = GetPtrToOfferSlotWhereThisItemIs( ubProfileID, sSlotNum );

	if( pInvSlot == NULL )
		// not found in either offer area
		return( FALSE );

	// found it, display it hatched out
	return( TRUE );
}

UINT32 CalculateTotalArmsDealerCost()
{
	UINT32	uiTotal=0;

	for ( int uiCnt = 0; uiCnt< gDealersOfferActiveRegions; ++uiCnt )
	{
		if( ArmsDealerOfferArea[ uiCnt ].fActive )
		{
			//if the dealer repairs
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
				uiTotal += CalculateObjectItemRepairCost( gbSelectedArmsDealerID, &( ArmsDealerOfferArea[ uiCnt ].ItemObject ) );
			else
				uiTotal += CalcShopKeeperItemPrice( DEALER_SELLING, FALSE, ArmsDealerOfferArea[ uiCnt ].sItemIndex, armsDealerInfo[ gbSelectedArmsDealerID ].dSellModifier, &ArmsDealerOfferArea[uiCnt].ItemObject );
		}
	}
	return( uiTotal );
}

UINT32 CalculateTotalPlayersValue()
{
	// we can't substitute money for intel
	if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
		return 0;

	UINT32 uiTotal = 0;

	for ( int ubCnt = 0; ubCnt < gPlayersOfferActiveRegions; ++ubCnt )
	{
		if( ( PlayersOfferArea[ ubCnt ].fActive ) && ( PlayersOfferArea[ ubCnt ].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_VALUE ) )
		{
			//Calculate a price for the item
			if( Item[ PlayersOfferArea[ ubCnt ].sItemIndex ].usItemClass == IC_MONEY )
				uiTotal += PlayersOfferArea[ ubCnt ].ItemObject[0]->data.money.uiMoneyAmount;
			else
				uiTotal += PlayersOfferArea[ ubCnt ].uiItemPrice;
		}
	}

	return( uiTotal );
}

void PerformTransaction( UINT32 uiMoneyFromPlayersAccount )
{
	UINT32	uiPlayersTotalMoneyValue = CalculateTotalPlayersValue() + uiMoneyFromPlayersAccount;
	UINT32	uiArmsDealersItemsCost = CalculateTotalArmsDealerCost();
	UINT32	uiMoneyInPlayersOfferArea = CalculateHowMuchMoneyIsInPlayersOfferArea( );
	INT32	iChangeToGiveToPlayer = 0;
	int		iAvailablePlayerOfferSlots;
	
	//if the player has already requested to leave, get out
	if( gfUserHasRequestedToLeave )
		return;

	// handle bribing... if the player is giving the dealer money, without buying anything
	if( IsMoneyTheOnlyItemInThePlayersOfferArea( ) && CountNumberOfItemsInTheArmsDealersOfferArea( ) == 0 )
	{
		// accept/refuse money (varies by dealer according to ACCEPTS_GIFTS flag)
		StartShopKeeperTalking( SK_QUOTES_DEALER_OFFERED_MONEY_AS_A_GIFT );

		// if the arms dealer is the kind of person who accepts gifts
		if( armsDealerInfo[ gbSelectedArmsDealerID ].uiFlags & ARMS_DEALER_ACCEPTS_GIFTS )
		{
			//Move all the players evaluated items to the dealer (also adds it to dealer's cash)
			MovePlayerOfferedItemsOfValueToArmsDealersInventory();

			DealerGetsBribed( armsDealerInfo[ gbSelectedArmsDealerID ].ubShopKeeperID, uiMoneyInPlayersOfferArea );
		}
	}
	else	// not a bribe
	{
		//if the dealer is not a repairman
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS )
		{
			iAvailablePlayerOfferSlots = (gPlayersOfferActiveRegions - CountNumberOfValuelessItemsInThePlayersOfferArea( ));

			// always reserve an empty slot for change.  We can't tell for sure whether player is getting change because that
			// could depend on how many items he's trying to buy won't fit - he could only be getting change because of that!
			if ( iAvailablePlayerOfferSlots > 0 )
			{
				--iAvailablePlayerOfferSlots;
			}

			//if there is NOT enough room in the players offer area
			if( CountNumberOfItemsInTheArmsDealersOfferArea( ) > iAvailablePlayerOfferSlots )
			{
				// tell player there's not enough room in the player's offer area
				DoSkiMessageBox( MSG_BOX_BASIC_STYLE, SKI_Text[ SKI_TEXT_NO_MORE_ROOM_IN_PLAYER_OFFER_AREA ], SHOPKEEPER_SCREEN, MSG_BOX_FLAG_OK, NULL );

				return;
			}
		}

		//if the player doesn't have enough money to pay for what he's buying
		if( uiArmsDealersItemsCost > uiPlayersTotalMoneyValue )
		{
			INT32 balance = LaptopSaveInfo.iCurrentBalance;
			if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
				balance = (INT32)GetIntel();

			//if the player doesn't have enough money in his account to pay the rest
			if( uiArmsDealersItemsCost > uiPlayersTotalMoneyValue + balance )
			{
				// tell player he can't possibly afford this
				SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 6,0, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
				SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SKIP_A_FRAME, 0,0, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
				SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 0, ( uiArmsDealersItemsCost - ( balance + uiPlayersTotalMoneyValue ) ), 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
			}
			else
			{
				// player doesn't have enough on the table, but can pay for it from his balance
				/// ask player if wants to subtract the shortfall directly from his balance
				SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SKIP_A_FRAME, 0,0, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
				SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 6,0, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );

				if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
				{
					if ( uiPlayersTotalMoneyValue )
						SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 8, ( uiArmsDealersItemsCost - uiPlayersTotalMoneyValue ), 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
					else
						SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 9, ( uiArmsDealersItemsCost - uiPlayersTotalMoneyValue ), 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
				}
				else
				{
					if ( uiPlayersTotalMoneyValue )
						SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 1, ( uiArmsDealersItemsCost - uiPlayersTotalMoneyValue ), 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
					else
						SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 2, ( uiArmsDealersItemsCost - uiPlayersTotalMoneyValue ), 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
				}
			}

			SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 7,0, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );

			gfResetShopKeepIdleQuote = TRUE;

			// disable further calls to PerformTransaction() until callback calls us again to free this flag
			gfPerformTransactionInProgress = TRUE;

			return;
		}

		// to get this far, player should have the money needed!
		Assert( uiPlayersTotalMoneyValue >= uiArmsDealersItemsCost );
		
		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			//Make sure there is enough money in the Player's offer area to cover the repair charge
			if( ( uiMoneyFromPlayersAccount + uiMoneyInPlayersOfferArea ) >= uiArmsDealersItemsCost )
			{
				//Move all the players evaluated items to the dealer ( For a repairman, this means just: get rid of the money )
				MovePlayerOfferedItemsOfValueToArmsDealersInventory();

				//if the arms dealer is the type of person to give change
				if( armsDealerInfo[ gbSelectedArmsDealerID ].uiFlags & ARMS_DEALER_GIVES_CHANGE )
				{
					//determine the amount of change to give
					iChangeToGiveToPlayer = ( uiMoneyFromPlayersAccount + uiMoneyInPlayersOfferArea ) - uiArmsDealersItemsCost;

					//if there is change
					if( iChangeToGiveToPlayer > 0 )
					{
						GivePlayerSomeChange( iChangeToGiveToPlayer );

						if( uiArmsDealersItemsCost > 0 )
						{
							StartShopKeeperTalking( SK_QUOTES_PLAYER_HAS_TOO_MUCH_MONEY_FOR_TRANSACTION );
						}
					}
					else
						StartShopKeeperTalking( SK_QUOTES_PLAYER_HAS_EXACTLY_ENOUGH_MONEY_FOR_TRANSACTION );
				}
				
				// move the offered items to the repairman's inventory
				MovePlayersItemsToBeRepairedToArmsDealersInventory();
			}
			else
			{
				//the player doesn't have enough money
				return;
			}
		}
		else	// non-repairman
		{
			// DON'T include uiMoneyFromPlayersAccount in change given to player.  The only way this can happen is if he agrees
			// to subtract from his balance, but there isn't enough room.  In that case, the cost of items not transfered is
			// used to reduce his balance deduction instead (not here, in the calling function).
			iChangeToGiveToPlayer = CalculateTotalPlayersValue() - uiArmsDealersItemsCost;
			
			//if the arms dealer buys stuff
			if( ( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_BUYS_ONLY ) ||
					( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_BUYS_SELLS ) )
			{
				// but the dealer can't afford this transaction
				if( iChangeToGiveToPlayer > ( INT32 ) gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash )
				{
					// HACK HACK HACK: We forgot to write/record quote 27 for Jake, so he ALWAYS must have enough money!
					if ( gbSelectedArmsDealerID != ARMS_DEALER_JAKE )
					{
						// no deal - dealer can't afford it
						StartShopKeeperTalking( SK_QUOTES_CANT_AFFORD_TO_BUY_OR_TOO_MUCH_TO_REPAIR );
						gfResetShopKeepIdleQuote = TRUE;

						return;
					}
				}
			}

			//Move all the player's evaluated items to the dealer
			MovePlayerOfferedItemsOfValueToArmsDealersInventory();

			//Move all dealers offered items to the player
			MoveAllArmsDealersItemsInOfferAreaToPlayersOfferArea( );
#ifdef JA2UB			
			//JA25 UB
			//if the arms dealer is Raul
			if( gbSelectedArmsDealerID == ARMS_DEALER_RAUL )
			{
				//if this is the first time that the player purchased somehting form raul
				if( gArmsDealerStatus[ gbSelectedArmsDealerID ].fHasSoldSomethingToPlayer == FALSE )
				{
					//if RAUL doesnt have the BARRETT anymore ie the player already bought it
					//if( gArmsDealersInventory[ ARMS_DEALER_PERKO ][ BARRETT ].ubTotalItems != 0 )
					//{
						if( gubQuest[ QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL ] == QUESTNOTSTARTED )
						{
							//Raul Start the quest and have Raul say somehitng
							StartShopKeeperTalking( SK_QUOTES_NPC_SPECIFIC_46 );
							StartShopKeeperTalking( SK_QUOTES_NPC_SPECIFIC_47 );

							StartQuest( QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL, -1, -1 );
						}
					//}
				}
			}
#endif

			//if the arms dealer is the type of person to give change
			if( armsDealerInfo[ gbSelectedArmsDealerID ].uiFlags & ARMS_DEALER_GIVES_CHANGE )
			{
				if( iChangeToGiveToPlayer > 0 )
				{
					GivePlayerSomeChange( iChangeToGiveToPlayer );

/*
					//Remove the change the arms dealer is returning to the player
					if( ( gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash - uiChangeToGiveToPlayer ) >= 0 )
						gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash -= uiChangeToGiveToPlayer;
					else
						gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash = 0;
*/

					if( uiArmsDealersItemsCost == 0 )
					{
						StartShopKeeperTalking( SK_QUOTES_PLAYER_HAS_EXACTLY_ENOUGH_MONEY_FOR_TRANSACTION );
					}
					else
					{
						StartShopKeeperTalking( SK_QUOTES_PLAYER_HAS_TOO_MUCH_MONEY_FOR_TRANSACTION );
					}
				}
				else if( iChangeToGiveToPlayer == 0 )
					StartShopKeeperTalking( SK_QUOTES_PLAYER_HAS_EXACTLY_ENOUGH_MONEY_FOR_TRANSACTION );
			}
#ifdef JA2UB			
			//Bought from dealer specific stuff

			//if the arms dealer is Betty, set fact 403
			if( gbSelectedArmsDealerID == ARMS_DEALER_BETTY )
			{
				//if the player spent more the X
				if( uiMoneyFromPlayersAccount >= SKI_BETTY_MINIMUM_AMOUNT_TO_SET_FACT )
				{
					//set the fact
					SetFactTrue( FACT_PLAYER_PAID_BETTY_MORE_THEN_X_FOR_ITEM  );
				}
			}

			//if the arms dealer is Raul, set fact 407
			if( gbSelectedArmsDealerID == ARMS_DEALER_RAUL )
			{
				//if the player spent more the X
				if( uiMoneyFromPlayersAccount >= SKI_RAUL_MINIMUM_AMOUNT_TO_SET_FACT )
				{
					//set the fact
					SetFactTrue( FACT_PLAYER_BOUGHT_SOMETHING_FROM_RAUL );
				}
			}

			//Remeber the fact that the player has sold somehting to the player
			gArmsDealerStatus[ gbSelectedArmsDealerID ].fHasSoldSomethingToPlayer = TRUE;
#endif

			//if the arms dealer is tony
			if( gbSelectedArmsDealerID == ARMS_DEALER_TONY )
				gfHavePurchasedItemsFromTony = TRUE;

			//if the arms dealer is Howard( 125), set fact 222
			if( gbSelectedArmsDealerID == ARMS_DEALER_HOWARD )
				SetFactTrue( 222 );
		}

		// add cash transfered from player's account directly to dealer's cash balance
		if( uiMoneyFromPlayersAccount > 0 )
		{
			gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash += uiMoneyFromPlayersAccount;
		}

		//Redraw the screen
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

		gfDoneBusinessThisSession = TRUE;

		//The shopkeeper's inventory has changed, reinitialize
		InitializeShopKeeper( FALSE );
	}
	
	gfResetShopKeepIdleQuote = TRUE;
}

void MoveAllArmsDealersItemsInOfferAreaToPlayersOfferArea( )
{
	//for all items in the dealers items offer area
	INT16		bSlotID=0;
#ifdef JA2UB	
	BOOLEAN	fAddItemToPlayer=TRUE;
#endif
	//loop through all the slots in the shopkeeper's offer area
	for ( int uiCnt = 0; uiCnt<gDealersOfferActiveRegions; ++uiCnt )
	{
		//if there is an item here
		if( ArmsDealerOfferArea[ uiCnt ].fActive )
		{
#ifdef JA2UB
			//JA25 UB
			fAddItemToPlayer = TRUE; 

			//if the dealer is betty
			if( gbSelectedArmsDealerID == ARMS_DEALER_BETTY )
			{
				//if the item is the laptop transmitter
				if( ArmsDealerOfferArea[ uiCnt ].sItemIndex == LAPTOP_TRANSMITTER ) //4500
				{
					//Dont transfer the item
					fAddItemToPlayer = FALSE;

					//Tell the user we are going to attach the transmitter to the laptop
					//( need to wait 1 frame caus ewe are currently in a msg box )
					gubDisplayMsgBoxAskingUserToAttachTransmitter = TRNSMTR_MSG_BOX__1_FRAME_WAIT;
				}

				//if the item is the porno mag
				if( ArmsDealerOfferArea[ uiCnt ].sItemIndex == PORNOS )
				{
					StartShopKeeperTalking( SK_QUOTES_WHEN_PLAYER_BUYS_PORNO );
				}

				//if the item is Tex's videos
				if( ArmsDealerOfferArea[ uiCnt ].sItemIndex == TEX_MOVIE_ATTACK_CLYDESDALES ||//1356  ||
						ArmsDealerOfferArea[ uiCnt ].sItemIndex == TEX_MOVIE_WILD_EAST ||//1357 	||
						ArmsDealerOfferArea[ uiCnt ].sItemIndex == TEX_MOVIE_HAVE_HONDA )//1358  )
				{
					SetFactTrue( FACT_PLAYER_BOUGHT_A_TEX_VIDEO_FROM_BETTY );
				}
			}
			
			if( fAddItemToPlayer ) //JA25 UB
			{		
#endif			
			bSlotID = AddItemToPlayersOfferArea( NO_PROFILE, &ArmsDealerOfferArea[ uiCnt ], -1 );

			if( bSlotID != -1 )
			{
				PlayersOfferArea[ bSlotID ].uiFlags |= ARMS_INV_JUST_PURCHASED;

#ifdef JA2UB				
				
				//if the player has just purchased the BArrett form Raul
				if( ( gbSelectedArmsDealerID == ARMS_DEALER_RAUL ) && ( ArmsDealerOfferArea[ uiCnt ].sItemIndex == BARRETT_UB ) )
				{
					// set a special flag
					gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags |= ARMS_DEALER_FLAG__RAUL_HAS_SOLD_BARRETT_TO_PLAYER;
				}
					
#endif					
				//if the player has just purchased a VIDEO_CAMERA from Franz Hinkle
				if( ( gbSelectedArmsDealerID == ARMS_DEALER_FRANZ ) && ( ArmsDealerOfferArea[ uiCnt ].sItemIndex == VIDEO_CAMERA ) )
				{
					// set a special flag
					gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags |= ARMS_DEALER_FLAG__FRANZ_HAS_SOLD_VIDEO_CAMERA_TO_PLAYER;
				}
			}
#ifdef JA2UB			
			}
#endif

			//Remove the items out of the dealers inventory
			RemoveItemFromDealersInventory( &ArmsDealerOfferArea[ uiCnt ], (UINT16)ArmsDealerOfferArea[ uiCnt ].bSlotIdInOtherLocation );

			//Remove the items from the Shopkeepers Offer area
			if( !RemoveItemFromArmsDealerOfferArea( (UINT16)uiCnt, FALSE ) )//ArmsDealerOfferArea[uiCnt].bSlotIdInOtherLocation
				Assert( 0 );

			if( ArmsDealerOfferArea[uiCnt].fActive )
				Assert(0);
		}
	}

	//loop through all the slots in the shopkeeper's offer area
	for ( int uiCnt = 0; uiCnt < gDealersOfferActiveRegions; ++uiCnt )
	{
		if( ArmsDealerOfferArea[uiCnt].fActive )
			Assert(0);
	}
}

BOOLEAN RemoveItemFromDealersInventory( INVENTORY_IN_SLOT* pInvSlot, UINT16 ubSlot )
{
	//Remove all of this item out of the specified inventory slot
	RemoveItemFromArmsDealerInventory( gbSelectedArmsDealerID, gpTempDealersInventory[ubSlot].sItemIndex, pInvSlot->ItemObject.ubNumberOfObjects, &pInvSlot->ItemObject );

	gfResetShopKeepIdleQuote = TRUE;
	return( TRUE );
}

void MovePlayerOfferedItemsOfValueToArmsDealersInventory()
{
#ifdef JA2UB
	BOOLEAN	fAddItemToDealer=TRUE;
#endif
	//loop through all the slots in the players offer area
	for ( int uiCnt = 0; uiCnt < gPlayersOfferActiveRegions; ++uiCnt )
	{
		//if there is an item here
		if( PlayersOfferArea[ uiCnt ].fActive )
		{
			// and it has value
			if( PlayersOfferArea[ uiCnt ].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_VALUE )
			{
				//Remove the item from the owner merc's inventory
				IfMercOwnedRemoveItemFromMercInv( &(PlayersOfferArea[ uiCnt ]) );

				//if the item is money
				if( Item[ PlayersOfferArea[ uiCnt ].sItemIndex ].usItemClass == IC_MONEY )
				{
					//add the money to the dealers 'cash'
					gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash += PlayersOfferArea[ uiCnt ].ItemObject[0]->data.money.uiMoneyAmount;
				}
#ifdef JA2UB
				else
				{			
					fAddItemToDealer = TRUE;

					//if the dealer is raul
					if( gbSelectedArmsDealerID == ARMS_DEALER_RAUL )
					{
						//if the item is the cannon balls
						if( PlayersOfferArea[ uiCnt ].sItemIndex == CLIP_CANNON_BALL )// 4499 )
						{
							fAddItemToDealer = FALSE;
						}

						//if the item is the antique musket
						if( PlayersOfferArea[ uiCnt ].sItemIndex == HAND_CANNON )// 4498 )
						{
							//Only do this if the player knew about the cannon offer
							if( gubQuest[ QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL ] == QUESTINPROGRESS )
							{
								//Have raul say a quote
								StartShopKeeperTalking( SK_QUOTES_NPC_SPECIFIC_51 );

								//Trigger Rauls action #52
								TriggerNPCRecord( RAUL_UB, 52 );

								//Remeber to discount the barrett
								SetJa25GeneralFlag( JA_GF__BARRETT_IS_HALF_PRICE );
							}

							//Mark the quest done
							EndQuest( QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL, -1, -1 );

							fAddItemToDealer = FALSE;
						}
					}
				
					//if we ARE to transfer the item
					if( fAddItemToDealer )
					{
						//if the dealer doesn't strictly buy items from the player, give the item to the dealer
						if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_BUYS_ONLY )
						{
							// item cease to be merc-owned during this operation
							AddObjectToArmsDealerInventory( gbSelectedArmsDealerID, &( PlayersOfferArea[ uiCnt ].ItemObject ) );
							PlayersOfferArea[ uiCnt ].ItemObject.initialize();
						}
					
					}
				}
#else
				else
				{
					//if the dealer doesn't strictly buy items from the player, give the item to the dealer
					if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_BUYS_ONLY )
					{
						// item cease to be merc-owned during this operation
						AddObjectToArmsDealerInventory( gbSelectedArmsDealerID, &( PlayersOfferArea[ uiCnt ].ItemObject ) );
						PlayersOfferArea[ uiCnt ].ItemObject.initialize();
					}
				}
#endif
				//erase the item from the player's offer area
				ClearPlayersOfferSlot( uiCnt );
			}
		}
	}

	gfResetShopKeepIdleQuote = TRUE;
}

void BeginSkiItemPointer( UINT8 ubSource, INT16 bSlotNum, BOOLEAN fOfferToDealerFirst )
{
	SGPRect			Rect;

/*
	// If we are already moving an item
	if ( gMoveingItem.sItemIndex != 0 )
	{
		return;
	}
*/

	switch( ubSource )
	{
		case ARMS_DEALER_INVENTORY:
			//Should never get in here
			Assert( 0 );
			return;
			break;

		case ARMS_DEALER_OFFER_AREA:
			//Get the item from the slot.
			gMoveingItem = ArmsDealerOfferArea[ bSlotNum ];
			IfMercOwnedRemoveItemFromMercInv( &gMoveingItem );

			//remove the item from the slot
			ClearArmsDealerOfferSlot( bSlotNum );
			
			//Restrict the cursor to the arms dealer offer area, players offer area and the players inventory
			Rect.iLeft = 0;//SKI_ITEM_MOVEMENT_AREA_X;
			Rect.iTop = SKI_DEALER_OFFER_AREA_Y;
			Rect.iRight = SKI_ITEM_MOVEMENT_AREA_X + SKI_ITEM_MOVEMENT_AREA_WIDTH;
			Rect.iBottom = SKI_ITEM_MOVEMENT_AREA_Y + SKI_ITEM_MOVEMENT_AREA_HEIGHT;
			if((UsingNewInventorySystem() == true)){
				Rect.iBottom += 60;
			}

			gpItemPointer = &gMoveingItem.ItemObject;

			//if there is an owner of the item
			if( gMoveingItem.ubIdOfMercWhoOwnsTheItem != -1 )
			{
				gpItemPointerSoldier = FindSoldierByProfileID( gMoveingItem.ubIdOfMercWhoOwnsTheItem, TRUE );
				//make sure the soldier is not null
				if( gpItemPointerSoldier == NULL )
				{
					gpItemPointerSoldier = gpSMCurrentMerc;
				}
			}
			else
				gpItemPointerSoldier = gpSMCurrentMerc;

			break;

		case PLAYERS_OFFER_AREA:
			//Get the item from the slot.
			gMoveingItem = PlayersOfferArea[ bSlotNum ];

			// if the slot is overloaded (holds more objects than we have room for valid statuses of)
			if ( PlayersOfferArea[ bSlotNum ].ItemObject.ubNumberOfObjects > MAX_OBJECTS_PER_SLOT )
			{
				// allow only MAX_OBJECTS_PER_SLOT of those objects to be picked up at a time
				// (sure it kind of sucks, but it's a lot easier than handling overloaded cursor objects in Interface Items!
				PlayersOfferArea[ bSlotNum ].ItemObject.MoveThisObjectTo(gMoveingItem.ItemObject, MAX_OBJECTS_PER_SLOT);
			}
			else	// completely legal object
			{
				// NOTE: a merc-owned object can never be "overloaded", so no problem here
				IfMercOwnedRemoveItemFromMercInv( &gMoveingItem );

				//remove the item from the slot
				ClearPlayersOfferSlot( bSlotNum );
			}

			//Restrict the cursor to the players offer area and the players inventory
			Rect.iLeft = 0;//SKI_ITEM_MOVEMENT_AREA_X;
			Rect.iTop = SKI_ITEM_MOVEMENT_AREA_Y;
			Rect.iRight = SKI_ITEM_MOVEMENT_AREA_X + SKI_ITEM_MOVEMENT_AREA_WIDTH;
			Rect.iBottom = SKI_ITEM_MOVEMENT_AREA_Y + SKI_ITEM_MOVEMENT_AREA_HEIGHT;
			if((UsingNewInventorySystem() == true)){
				Rect.iBottom += 60;
			}

			gpItemPointer = &gMoveingItem.ItemObject;

			//if there is an owner of the item
			if( gMoveingItem.ubIdOfMercWhoOwnsTheItem != -1 )
			{
				gpItemPointerSoldier = FindSoldierByProfileID( gMoveingItem.ubIdOfMercWhoOwnsTheItem, TRUE );
				//make sure the soldier is not null
				if( gpItemPointerSoldier == NULL )
				{
					gpItemPointerSoldier = gpSMCurrentMerc;
				}
			}
			else
				gpItemPointerSoldier = gpSMCurrentMerc;
#ifdef JA2UB				
				//ja25 ub
			//if the dealer is Raul
			if( gbSelectedArmsDealerID == ARMS_DEALER_RAUL )
			{
				//if the item is the antique musket
				if( gMoveingItem.ItemObject.usItem == HAND_CANNON ) //4498  )
				{
					if( gubQuest[ QUEST_FIND_ANTIQUE_MUSKET_FOR_RAUL ] == QUESTINPROGRESS )
					{
						//if the player hasnt said the quote before
						if( !( gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_49 ) )
						{
							StartShopKeeperTalking( SK_QUOTES_NPC_SPECIFIC_49 );
							gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags |= ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_49;
						}
					}
				}
			}
#endif

			break;
			
		case PLAYERS_INVENTORY:
			// better be a valid merc pocket index, or -1
			Assert( ( bSlotNum >= -1 ) && ( bSlotNum < (INT8)gpSMCurrentMerc->inv.size() ) );

			// if we're supposed to store the original pocket #, but that pocket still holds more of these
			if ( ( bSlotNum != -1 ) && ( gpSMCurrentMerc->inv[ bSlotNum ].exists() == true ) )
			{
				// then we can't store the pocket #, because our system can't return stacked objects
				bSlotNum = -1;
			}

			// First try to evaluate the item immediately like any other left-click on a merc's inventory slot would be.  Only
			// if that doesn't work (because there isn't enough room in the player's offer area), the item is picked up into
			// the cursor, and may then get placed into the player's offer area directly, but it will NOT get evaluated that
			// way, and so has no possibility of entering the dealer's inventory (where complex items aren't permitted).
			if ( fOfferToDealerFirst && OfferObjectToDealer( gpItemPointer, gpSMCurrentMerc->ubProfile, bSlotNum ) )
			{
				//Reset the cursor
				SetSkiCursor( CURSOR_NORMAL );
			}
			else	// not supposed to offer it, or dealer has no more room
			{
				// store the current contents of the cursor in a temporary object structure.
				// We have to do this before memsetting gMoveingItem, 'cause during swaps, gpItemPointer == &gMoveingItem.ItemObject!
				gTempObject = *gpItemPointer;

				//ARM: The memset was previously commented out, in order to preserve the owning merc's inv slot # during a swap of
				// items in an inventory slot.  However, that leads to other bugs: if you picked the thing you're swapping in from
				// a restricted inv slot (headgear, vest, etc.), the item swapped out will end up belonging to an illegal slot, and
				// return there with a right click on it in the player's offer area.  So now ALL items picked up here are unowned.
				gMoveingItem.initialize();

				//Get the item from the pointer
				gMoveingItem.ItemObject = gTempObject;

				gMoveingItem.fActive = TRUE;
				gMoveingItem.sItemIndex = gTempObject.usItem;
				gMoveingItem.ubLocationOfObject = ubSource;

				// By necessity, these items don't belong to a slot (so you can't return them via a right click),
				// because it would be too much work to handle attachments, members of a stack, or even items swapped out of slots.
				gMoveingItem.ubIdOfMercWhoOwnsTheItem = gpSMCurrentMerc->ubProfile;
				gMoveingItem.bSlotIdInOtherLocation = bSlotNum;

				//Restrict the cursor to the players offer area and the players inventory
				Rect.iLeft = 0;//SKI_ITEM_MOVEMENT_AREA_X;
				Rect.iTop = SKI_ITEM_MOVEMENT_AREA_Y;
				Rect.iRight = SKI_ITEM_MOVEMENT_AREA_X + SKI_ITEM_MOVEMENT_AREA_WIDTH;
				Rect.iBottom = SKI_ITEM_MOVEMENT_AREA_Y + SKI_ITEM_MOVEMENT_AREA_HEIGHT;
				if((UsingNewInventorySystem() == true)){
					Rect.iBottom += 60;
				}

				gpItemPointer = &gMoveingItem.ItemObject;
				gpItemPointerSoldier = gpSMCurrentMerc;
			}

			break;
	}

	// if we have something in hand
	if ( gpItemPointer != NULL )
	{
		//make sure the soldier is not null
		Assert( gpItemPointerSoldier != NULL );

		// Set mouse
		SetSkiCursor( EXTERN_CURSOR );

		//Enable the region that limits the movement of the cursor with the item
		MSYS_EnableRegion( &gSkiInventoryMovementAreaMouseRegions );

		RestrictMouseCursor( &Rect );

		DisableAllDealersInventorySlots();

		if ( ubSource != ARMS_DEALER_OFFER_AREA )
		{
			DisableAllDealersOfferSlots();
		}

		gfResetShopKeepIdleQuote = TRUE;
	}

	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}

void RestrictSkiMouseCursor()
{
	SGPRect			Rect;

	Rect.iLeft = 0;//SKI_ITEM_MOVEMENT_AREA_X;
	Rect.iTop = SKI_ITEM_MOVEMENT_AREA_Y;
	Rect.iRight = SKI_ITEM_MOVEMENT_AREA_X + SKI_ITEM_MOVEMENT_AREA_WIDTH;
	Rect.iBottom = SKI_ITEM_MOVEMENT_AREA_Y + SKI_ITEM_MOVEMENT_AREA_HEIGHT;

	RestrictMouseCursor( &Rect );
}

void SetSkiCursor( UINT16	usCursor )
{
	int	ubCnt;

	//if we are setting up an item as a cursor
	if( usCursor == EXTERN_CURSOR )
	{
//		EnableSMPanelButtons( FALSE, FALSE );

		// if the current merc is in range
		if( !gfSMDisableForItems )
		{
			// hatch out unavailable merc inventory slots
			ReevaluateItemHatches( gpSMCurrentMerc, FALSE );
		}

		// Set mouse
		guiExternVo = GetInterfaceGraphicForItem( &(Item[ gMoveingItem.sItemIndex ]) );
		gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gMoveingItem.sItemIndex ].ubGraphicNum;
		SetCurrentCursorFromDatabase( EXTERN_CURSOR );

		MSYS_ChangeRegionCursor( &gSMPanelRegion, usCursor );

		MSYS_ChangeRegionCursor( &gSKI_EntireScreenMouseRegions, usCursor );

		MSYS_ChangeRegionCursor( &gArmsDealersDropItemToGroundMouseRegions, usCursor );

		MSYS_SetCurrentCursor( usCursor );

		//if the item desc window is up
		if( gInvDesc.uiFlags & MSYS_REGION_EXISTS )
			MSYS_ChangeRegionCursor( &gInvDesc, usCursor );

		for( ubCnt = 0; ubCnt < gMoveingItem.ItemObject[0]->attachments.size(); ++ubCnt)
		{
			if( gItemDescAttachmentRegions[ubCnt].uiFlags & MSYS_REGION_EXISTS )
				MSYS_ChangeRegionCursor( &gItemDescAttachmentRegions[ubCnt], usCursor );
		}

		for ( ubCnt = 0; ubCnt<gDealersOfferActiveRegions; ++ubCnt )
		{
			MSYS_ChangeRegionCursor( &gPlayersOfferSlotsMouseRegions[ ubCnt], usCursor );
			MSYS_ChangeRegionCursor( &gPlayersOfferSlotsSmallFaceMouseRegions[ ubCnt], usCursor );

			MSYS_ChangeRegionCursor( &gDealersOfferSlotsMouseRegions[ ubCnt], usCursor );

			//if the dealer repairs
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
			{
				MSYS_ChangeRegionCursor( &gDealersOfferSlotsSmallFaceMouseRegions[ ubCnt], usCursor );
			}
		}

		MSYS_ChangeRegionCursor( &gSkiInventoryMovementAreaMouseRegions, usCursor );
	}
	//else we are restoring the old cursor
	else
	{
		gMoveingItem.initialize();

//		gpSkiItemPointer = NULL;
		gpItemPointer = NULL;

		DisableTacticalTeamPanelButtons( FALSE );

//		EnableSMPanelButtons( TRUE, FALSE );
//		CheckForDisabledForGiveItem( );

		// if the current merc is in range
		if( !gfSMDisableForItems )
		{
			// make all merc inventory slots available again
			ReevaluateItemHatches( gpSMCurrentMerc, TRUE );
		}

		MSYS_ChangeRegionCursor( &gSMPanelRegion, usCursor );

		MSYS_ChangeRegionCursor( &gSKI_EntireScreenMouseRegions, usCursor );

		MSYS_ChangeRegionCursor( &gArmsDealersDropItemToGroundMouseRegions, usCursor );

		for ( ubCnt = 0; ubCnt<gDealersOfferActiveRegions; ++ubCnt )
		{
			MSYS_ChangeRegionCursor( &gDealersOfferSlotsMouseRegions[ ubCnt ], usCursor );

			//if the dealer repairs
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
			{
				MSYS_ChangeRegionCursor( &gDealersOfferSlotsSmallFaceMouseRegions[ ubCnt ], usCursor );
			}
		}

		for ( ubCnt = 0; ubCnt<gPlayersOfferActiveRegions; ++ubCnt )
		{
			MSYS_ChangeRegionCursor( &gPlayersOfferSlotsMouseRegions[ubCnt], usCursor );
			MSYS_ChangeRegionCursor( &gPlayersOfferSlotsSmallFaceMouseRegions[ubCnt], usCursor );
		}
		
		//if the item desc window is up
		if( gInvDesc.uiFlags & MSYS_REGION_EXISTS )
			MSYS_ChangeRegionCursor( &gInvDesc, usCursor );

		for( ubCnt = 0; ubCnt < gMoveingItem.ItemObject[0]->attachments.size(); ++ubCnt)
		{
			if( gItemDescAttachmentRegions[ubCnt].uiFlags & MSYS_REGION_EXISTS )
				MSYS_ChangeRegionCursor( &gItemDescAttachmentRegions[ubCnt], usCursor );
		}
		
		MSYS_ChangeRegionCursor( &gSkiInventoryMovementAreaMouseRegions, usCursor );

		MSYS_SetCurrentCursor( usCursor );

		SetCurrentCursorFromDatabase( usCursor );

		FreeMouseCursor( TRUE );

		EnableAllDealersInventorySlots();
		EnableAllDealersOfferSlots();

		//Disable the region that limits the movement of the cursor with the item
		MSYS_DisableRegion( &gSkiInventoryMovementAreaMouseRegions );
	}

	SetCurrentCursorFromDatabase( usCursor );

	// make sure disabled slot hatching gets updated when items picked up / dropped
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}

INT16 AddInventoryToSkiLocation( INVENTORY_IN_SLOT *pInv, UINT16 ubSpotLocation, UINT8 ubWhere )
{
	INT16 bSlotAddedTo = -1;

	switch( ubWhere )
	{
		case ARMS_DEALER_INVENTORY:
		case PLAYERS_INVENTORY:
			// not used this way
			Assert( 0 );
			return( bSlotAddedTo );
			break;

		case ARMS_DEALER_OFFER_AREA:

			//If we can add the item into the slot that was clicked on
			if( ArmsDealerOfferArea[ ubSpotLocation ].fActive == FALSE )
			{
				ArmsDealerOfferArea[ ubSpotLocation ] = *pInv;
				IfMercOwnedCopyItemToMercInv( pInv );

				SetSkiRegionHelpText( &ArmsDealerOfferArea[ ubSpotLocation ], &gDealersOfferSlotsMouseRegions[ ubSpotLocation ], ARMS_DEALER_OFFER_AREA );
				SetSkiFaceRegionHelpText( &ArmsDealerOfferArea[ ubSpotLocation ], &gDealersOfferSlotsSmallFaceMouseRegions[ ubSpotLocation ], ARMS_DEALER_OFFER_AREA );

				bSlotAddedTo = ubSpotLocation;
			}
			else
			{
				bSlotAddedTo = AddItemToArmsDealerOfferArea( pInv, -1 );
			}
			break;

		case PLAYERS_OFFER_AREA:

			//If we can add the item into the slot that was clicked on
			if( PlayersOfferArea[ ubSpotLocation ].fActive == FALSE )
			{
				// put it down in that player offer area slot
				PlayersOfferArea[ ubSpotLocation ] = *pInv;
				IfMercOwnedCopyItemToMercInv( pInv );

				//if the item is money
				if( Item[ PlayersOfferArea[ ubSpotLocation ].sItemIndex ].usItemClass == IC_MONEY && !(armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL) )
				{
					//Since money is always evaluated
					PlayersOfferArea[ ubSpotLocation ].uiFlags |= ARMS_INV_PLAYERS_ITEM_HAS_VALUE;
					PlayersOfferArea[ ubSpotLocation ].uiItemPrice = PlayersOfferArea[ ubSpotLocation ].ItemObject[0]->data.money.uiMoneyAmount;
				}

				SetSkiRegionHelpText( &PlayersOfferArea[ ubSpotLocation ], &gPlayersOfferSlotsMouseRegions[ ubSpotLocation ], PLAYERS_OFFER_AREA );
				SetSkiFaceRegionHelpText( &PlayersOfferArea[ ubSpotLocation ], &gPlayersOfferSlotsSmallFaceMouseRegions[ ubSpotLocation ], PLAYERS_OFFER_AREA );

				bSlotAddedTo = ubSpotLocation;
			}
//			else if( )
//			{
				//check to see the type we are adding is the same as the type that is already there
//			}
			else	// that slot is full
			{
				// add it elsewhere
				bSlotAddedTo = AddItemToPlayersOfferArea( pInv->ubIdOfMercWhoOwnsTheItem, pInv, pInv->bSlotIdInOtherLocation );
			}

		break;
	}

	//Redraw the screen
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;
	gfResetShopKeepIdleQuote = TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;

	return( bSlotAddedTo );
}

void DisplayTalkingArmsDealer()
{
	static BOOLEAN	fWasTheMercTalking= FALSE;
//	static UINT32		uiLastTime=0;
//	UINT32					uiCurTime = GetJA2Clock();
//	static	UINT32	uiMinimumLengthForTalkingText;
	
	//Make sure the Dealers doesn't get disabled
	if ( giShopKeeperFaceIndex > -1 )
		gFacesData[ giShopKeeperFaceIndex ].fDisabled = FALSE;

	HandleDialogue();

	//Gets handled when we render the tactical interface
	HandleAutoFaces( );

	HandleTalkingAutoFaces( );
	HandleShopKeeperDialog( 0 );

	gfIsTheShopKeeperTalking = FALSE;
	if ( giShopKeeperFaceIndex > -1 )
 		gfIsTheShopKeeperTalking = gFacesData[ giShopKeeperFaceIndex ].fTalking;

	//if the merc just started talking
//	if( gfIsTheShopKeeperTalking && !fWasTheMercTalking )
	{

	}

	//if the merc is talking
	if( gfIsTheShopKeeperTalking )
		fWasTheMercTalking = TRUE;

	//if the merc just finished talking
	if( !gfIsTheShopKeeperTalking && fWasTheMercTalking )
	{
		RemoveShopKeeperSubTitledText();
		fWasTheMercTalking = FALSE;
	}
}

void HandleShopKeeperDialog( UINT8 ubInit )
{
	UINT32	uiCurTime = GetJA2Clock();

	static	UINT32	uiLastTime = 0;
	static	UINT32	uiLastTimeSpoke = 0;
	static	INT8		bSpeech = -1;
	
	if( ubInit >= 1 )
	{
		uiLastTime = GetJA2Clock();
	}

	if( ubInit == 1 )
	{
		// special: if it's Arnie, and we have stuff in for repairs, but it's not fixed yet, use a different opening quote!
		if ( ( gbSelectedArmsDealerID == ARMS_DEALER_ARNIE ) && RepairmanIsFixingItemsButNoneAreDoneYet( ARNIE ) )
		{
			bSpeech = ARNIE_QUOTE_NOT_REPAIRED_YET;
		}
		// if repairs were delayed
		if ( gfStartWithRepairsDelayedQuote )
		{
			bSpeech = FREDO_PERKO_SORRY_REPAIR_DELAYED;
			gfStartWithRepairsDelayedQuote = FALSE;

			// treat this as having done business - the player WAS here for a good reason, and it's the dealers fault...
			gfDoneBusinessThisSession = TRUE;
		}
		else
		{
			bSpeech = SK_QUOTES_PLAYER_FIRST_ENTERS_SKI;
		}

		return;
	}

	//if its the first time in
	if( bSpeech != -1 )
	{
		if( ( uiCurTime - uiLastTime ) > 800 )
		{
			uiLastTimeSpoke = GetJA2Clock();
			StartShopKeeperTalking( bSpeech );
			bSpeech = -1;
			uiLastTime = GetJA2Clock();
		}
	}
	
	//Handle Idle converstions
	if( gfIsTheShopKeeperTalking )
	{
		uiLastTime = uiCurTime;
	}
	//if the player has requested to leave
	else if( gfUserHasRequestedToLeave && ( giShopKeepDialogueEventinProgress == -1 ) )
	{
		// to see if the player has finished talking
		if( !gFacesData[ giShopKeeperFaceIndex ].fTalking )
		{
			SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 5, 0, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );
		}

		DealWithItemsStillOnTheTable();
	}
	else
	{
		// Determine if the shopkeeper should say a quote
		if( ( uiCurTime - uiLastTime ) > ( guiRandomQuoteDelayTime + Random( guiRandomQuoteDelayTime ) ) )
		{
			//Only say it occasionally
			if( Chance ( 35 ) )
			{
				INT16 sRandomQuoteToUse = -1;

				// first check if one of the situation warrants one of the more precise quotes

				//if the dealer repairs
				if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
				{
					// if there are items in the arms dealer's offer area (must be awaiting repairs)
					if ( CountNumberOfItemsInTheArmsDealersOfferArea() > 0)
					{
						sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_ITEMS_CHOSEN_TO_SELL_OR_REPAIR;
					}
				}
				else	// non-repairman
				{
					// if there is any items in the player's area with value to this dealer other than just money
					if ( ( CalculateTotalPlayersValue() - CalculateHowMuchMoneyIsInPlayersOfferArea( ) ) > 0 )
					{
						// if the player has also selected some items for purchase
						if ( CountNumberOfItemsInTheArmsDealersOfferArea() > 0 )
						{
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_ITEMS_CHOSEN_TO_TRADE;
						}
						else
						{
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_ITEMS_CHOSEN_TO_SELL_OR_REPAIR;
						}
					}
				}
#ifdef JA2UB				
				//ja25 UB
				// if neither of the more precise quotes fit, or 33 percent of the time anyways
				if ( ( sRandomQuoteToUse == -1 ) || Chance( 33 ) )
				{
					//If the dealer is Betty,
					if( gbSelectedArmsDealerID == ARMS_DEALER_BETTY )
					{
						//she has 4 random quotes
						UINT8	ubRandom = Random( 100 );

						if( ubRandom > 75 )
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_1;
						else if( ubRandom > 50 )
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_2;
						else if( ubRandom > 25 )
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_3;
						else
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_4;

					}
					else
					{
						if( Chance ( 50 ) )
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_1;
						else
							sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_2;
					}
				}

				Assert( sRandomQuoteToUse != -1 );
				Assert( sRandomQuoteToUse < NUM_COMMON_SK_QUOTES );

				if ( !gfCommonQuoteUsedThisSession[ sRandomQuoteToUse ] )
				{
					StartShopKeeperTalking( (UINT16) sRandomQuoteToUse );

					gfCommonQuoteUsedThisSession[ sRandomQuoteToUse ] = TRUE;

					//increase the random quote delay
					guiRandomQuoteDelayTime += SKI_DEALERS_RANDOM_QUOTE_DELAY_INCREASE_RATE; 
				}
				
#else
				// if neither of the more precise quotes fit, or 33 percent of the time anyways
				if ( ( sRandomQuoteToUse == -1 ) || Chance( 33 ) )
				{
					if( Chance ( 50 ) )
						sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_1;
					else
						sRandomQuoteToUse = SK_QUOTES_RANDOM_QUOTE_WHILE_PLAYER_DECIDING_2;
				}

				Assert( sRandomQuoteToUse != -1 );
				Assert( sRandomQuoteToUse < NUM_COMMON_SK_QUOTES );

				if ( !gfCommonQuoteUsedThisSession[ sRandomQuoteToUse ] )
				{
					StartShopKeeperTalking( (UINT16) sRandomQuoteToUse );

					gfCommonQuoteUsedThisSession[ sRandomQuoteToUse ] = TRUE;

					//increase the random quote delay
					guiRandomQuoteDelayTime += SKI_DEALERS_RANDOM_QUOTE_DELAY_INCREASE_RATE;
				}
#endif
			}

			uiLastTime = GetJA2Clock();
		}
	}
}

BOOLEAN StartShopKeeperTalking( UINT16 usQuoteNum )
{
	BOOLEAN fSuccess;

	// if already in the process of leaving, don't start any additional quotes
	if ( gfSKIScreenExit || gfRemindedPlayerToPickUpHisStuff || gfUserHasRequestedToLeave )
	{
		return( FALSE );
	}

	// post event to mark shopkeeper dialogue in progress
	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 3, usQuoteNum, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );

	// post quote dialogue
	fSuccess = CharacterDialogue( armsDealerInfo[ gbSelectedArmsDealerID ].ubShopKeeperID, usQuoteNum, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI, FALSE, FALSE );

	// post event to mark shopkeeper dialogue as ended
	SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_SHOPKEEPER, 4, usQuoteNum, 0, giShopKeeperFaceIndex, DIALOGUE_SHOPKEEPER_UI );

	if ( !fSuccess )
	{
		return( FALSE );
	}

	gfResetShopKeepIdleQuote = TRUE;
	return( TRUE );
}

BOOLEAN IsGunOrAmmoOfSameTypeSelected( OBJECTTYPE	*pItemObject )
{
	//if there is no item selected, return
	if( gpHighLightedItemObject == NULL )
		return( FALSE );

	//if the item is ammo
	if( Item[ gpHighLightedItemObject->usItem ].usItemClass == IC_AMMO )
	{
		//if there is a gun
		if( Item[ pItemObject->usItem ].usItemClass == IC_GUN )
		{
			//of the same caliber
			if( Weapon[ pItemObject->usItem ].ubCalibre == Magazine[ Item[ gpHighLightedItemObject->usItem ].ubClassIndex ].ubCalibre )
			{
				return( TRUE );
			}
		}
	}

	//else if the item is a gun
	else if( Item[ gpHighLightedItemObject->usItem ].usItemClass == IC_GUN )
	{
		//if there is a gun
		if( Item[ pItemObject->usItem ].usItemClass == IC_AMMO )
		{
			//of the same caliber
			if( Weapon[ gpHighLightedItemObject->usItem ].ubCalibre == Magazine[ Item[ pItemObject->usItem ].ubClassIndex ].ubCalibre )
			{
				return( TRUE );
			}
		}
	}
	
	//if the highlighted object is an attachment
//	if( Item[ pItemObject->usItem ].fFlags & ITEM_ATTACHMENT )
	if( Item[ pItemObject->usItem ].attachment  )
	{
		if( ValidAttachment( pItemObject->usItem, gpHighLightedItemObject ) )
			return( TRUE );
	}
	else
	{
		if( ValidAttachment( gpHighLightedItemObject->usItem, pItemObject ) )
			return( TRUE );
	}

	return( FALSE );
}

void			InitShopKeeperSubTitledText( STR16 pString )
{
	//Clear the contents of the subtitle text
	memset(gsShopKeeperTalkingText, 0, sizeof(CHAR16)* SKI_SUBTITLE_TEXT_SIZE);

	swprintf( gsShopKeeperTalkingText, L"\"%s\"", pString );

	//Now setup the popup box
	if( gGameSettings.fOptions[ TOPTION_SUBTITLES ] )
	{
		UINT16 usActualWidth=0;
		UINT16 usActualHeight=0;

		giPopUpBoxId = PrepareMercPopupBox( giPopUpBoxId, BASIC_MERC_POPUP_BACKGROUND, BASIC_MERC_POPUP_BORDER, gsShopKeeperTalkingText, 300, 0, 0, 0, &usActualWidth, &usActualHeight);
		
//		gusPositionOfSubTitlesX = ( 640 - usActualWidth ) / 2 ;
		//position it to start under the guys face

		gusPositionOfSubTitlesX = 13 + SCREEN_X_OFFSET;

		RenderMercPopUpBoxFromIndex( giPopUpBoxId, gusPositionOfSubTitlesX, SKI_POSITION_SUBTITLES_Y, FRAME_BUFFER);

		//check to make sure the region is not already initialized
		if( !( gShopKeeperSubTitleMouseRegion.uiFlags & MSYS_REGION_EXISTS ) )
		{
			MSYS_DefineRegion( &gShopKeeperSubTitleMouseRegion, gusPositionOfSubTitlesX, SKI_POSITION_SUBTITLES_Y, (INT16)(gusPositionOfSubTitlesX + usActualWidth), (INT16)(SKI_POSITION_SUBTITLES_Y + usActualHeight), MSYS_PRIORITY_HIGH,
										 CURSOR_NORMAL, MSYS_NO_CALLBACK, ShopKeeperSubTitleRegionCallBack );
			MSYS_AddRegion( &gShopKeeperSubTitleMouseRegion );
		}

		//redraw the screen
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

	}
}

void RemoveShopKeeperSubTitledText()
{
	if( giPopUpBoxId == -1 )
		return;

	if( RemoveMercPopupBoxFromIndex( giPopUpBoxId ) )
	{
		giPopUpBoxId = -1;
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

		//Get rid of the subtitles region
		if( gGameSettings.fOptions[ TOPTION_SUBTITLES ] )
			MSYS_RemoveRegion( &gShopKeeperSubTitleMouseRegion );
	}
}

BOOLEAN AreThereItemsInTheArmsDealersOfferArea( )
{
	//loop through the players offer area and see if there are any items there
	for ( int ubCnt = 0; ubCnt < gDealersOfferActiveRegions; ++ubCnt )
	{
		//if is an item here
		if( ArmsDealerOfferArea[ ubCnt ].fActive )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

BOOLEAN AreThereItemsInThePlayersOfferArea( )
{
	//loop through the players offer area and see if there are any items there
	for ( int ubCnt = 0; ubCnt < gPlayersOfferActiveRegions; ++ubCnt )
	{
		//if is an item here
		if( PlayersOfferArea[ ubCnt ].fActive )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

//Mouse Call back for the Arms traders inventory slot
void ShopKeeperSubTitleRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP || iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		ShutUpShopKeeper();
	}
}

//Mouse Call back for the Arms delaers face
void SelectArmsDealersFaceRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP || iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		ShutUpShopKeeper();
	}
}

void ShutUpShopKeeper()
{
//	RemoveShopKeeperSubTitledText();

	ShutupaYoFace( giShopKeeperFaceIndex );
	gfIsTheShopKeeperTalking = FALSE;

	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}

int CountNumberOfValuelessItemsInThePlayersOfferArea( )
{
	int	count = 0;

	//loop through the players offer area and see if there are any items there
	for ( int ubCnt = 0; ubCnt<gPlayersOfferActiveRegions; ++ubCnt )
	{
		//if is an item here
		if( PlayersOfferArea[ubCnt].fActive )
		{
			//and if it has no value
			if( !( PlayersOfferArea[ubCnt].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_VALUE ) )
			{
				++count;
			}
		}
	}

	return( count );
}

int CountNumberOfItemsInThePlayersOfferArea( )
{
	int	count = 0;

	//loop through the player's offer area and see if there are any items there
	for ( int ubCnt = 0; ubCnt < gPlayersOfferActiveRegions; ++ubCnt )
	{
		//if is an item here
		if( PlayersOfferArea[ubCnt].fActive )
		{
			++count;
		}
	}

	return( count );
}

int CountNumberOfItemsInTheArmsDealersOfferArea( )
{
	int	count = 0;

	//loop through the dealer's offer area and see if there are any items there
	for ( int ubCnt = 0; ubCnt < gDealersOfferActiveRegions; ++ubCnt )
	{
		//if is an item here
		if( ArmsDealerOfferArea[ubCnt].fActive )
		{
			++count;
		}
	}

	return( count );
}

INT8 GetSlotNumberForMerc( UINT8 ubProfile )
{
	for( INT8 bCnt = 0; bCnt < gubNumberMercsInArray; ++bCnt )
	{
		if( gubArrayOfEmployedMercs[ bCnt ] == ubProfile )
			return( bCnt );
	}

	// not found - not currently working for the player
	return( -1 );
}

void EnableDisableDealersInventoryPageButtons()
{
	//if we are on the first page, disable the page up arrow
	if( gSelectArmsDealerInfo.ubCurrentPage <= 1 )
	{
		DisableButton( guiSKI_InvPageUpButton );
	}
	else
	{
		EnableButton( guiSKI_InvPageUpButton );
	}


	//if we are on the last page, disable the page down button
	if( gSelectArmsDealerInfo.ubCurrentPage == gSelectArmsDealerInfo.ubNumberOfPages )
	{
		DisableButton( guiSKI_InvPageDownButton );
	}
	else
	{
		EnableButton( guiSKI_InvPageDownButton );
	}
}

void EnableDisableShopkeeperButtons(UINT32 uiCurrentItemDescriptionScreen, int bpAction)
{
	if(uiCurrentItemDescriptionScreen != SHOPKEEPER_SCREEN)
		return;

	switch (bpAction)
	{
		case ACTIVATE_BUTTON:
			if(guiSKI_TransactionButton != 0)
				RemoveButton( guiSKI_TransactionButton );
			if(guiSKI_TransactionButtonImage != 0)
				UnloadButtonImage( guiSKI_TransactionButtonImage );
			if(guiSKI_DoneButton != 0)
				RemoveButton( guiSKI_DoneButton );
			if(guiSKI_DoneButtonImage != 0)
				UnloadButtonImage( guiSKI_DoneButtonImage );

			guiSKI_TransactionButtonImage = LoadButtonImage("INTERFACE\\TradeButtons.sti", -1,0,-1,1,-1 );
			guiSKI_DoneButtonImage = UseLoadedButtonImage( guiSKI_TransactionButtonImage, -1,0,-1,1,-1 );

			guiSKI_TransactionButton = CreateIconAndTextButton( guiSKI_TransactionButtonImage, SKI_Text[SKI_TEXT_TRANSACTION], SKI_BUTTON_FONT,
															 SKI_BUTTON_COLOR, DEFAULT_SHADOW, SKI_BUTTON_COLOR, DEFAULT_SHADOW, TEXT_CJUSTIFIED,
															 SKI_TRANSACTION_BUTTON_X, SKI_TRANSACTION_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
															 DEFAULT_MOVE_CALLBACK, BtnSKI_TransactionButtonCallback);
			SpecifyDisabledButtonStyle( guiSKI_TransactionButton, DISABLED_STYLE_HATCHED );
			//if the dealer repairs, use the repair fast help text for the transaction button
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
				SetButtonFastHelpText( guiSKI_TransactionButton, SkiMessageBoxText[ SKI_REPAIR_TRANSACTION_BUTTON_HELP_TEXT ] );
			else
				SetButtonFastHelpText( guiSKI_TransactionButton, SkiMessageBoxText[ SKI_TRANSACTION_BUTTON_HELP_TEXT ] );

			//Done button
			guiSKI_DoneButton = CreateIconAndTextButton( guiSKI_DoneButtonImage, SKI_Text[SKI_TEXT_DONE], SKI_BUTTON_FONT,
															 SKI_BUTTON_COLOR, DEFAULT_SHADOW, SKI_BUTTON_COLOR, DEFAULT_SHADOW, TEXT_CJUSTIFIED,
															 SKI_DONE_BUTTON_X, SKI_DONE_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH+10,
															 DEFAULT_MOVE_CALLBACK, BtnSKI_DoneButtonCallback);
			SpecifyDisabledButtonStyle( guiSKI_DoneButton, DISABLED_STYLE_HATCHED );
			SetButtonFastHelpText( guiSKI_DoneButton, SkiMessageBoxText[ SKI_DONE_BUTTON_HELP_TEXT ] );
			break;
		case DEACTIVATE_BUTTON:
			if(guiSKI_TransactionButton != 0)
				RemoveButton( guiSKI_TransactionButton );
			if(guiSKI_DoneButton != 0)
				RemoveButton( guiSKI_DoneButton );
			EnableDisableShopkeeperButtons(uiCurrentItemDescriptionScreen, UNLOAD_BUTTON);
			guiSKI_TransactionButtonImage	= 0;
			guiSKI_TransactionButton	= 0;
			guiSKI_DoneButtonImage	= 0;
			guiSKI_DoneButton	= 0;
			break;
		case ENABLE_BUTTON:
			if(guiSKI_TransactionButton == 0 || guiSKI_DoneButton == 0)
			{
				EnableDisableShopkeeperButtons(uiCurrentItemDescriptionScreen, DEACTIVATE_BUTTON);
				EnableDisableShopkeeperButtons(uiCurrentItemDescriptionScreen, ACTIVATE_BUTTON);
			}
			if(guiSKI_TransactionButton != 0)
				EnableButton( guiSKI_TransactionButton );
			if(guiSKI_DoneButton != 0)
				EnableButton( guiSKI_DoneButton );
			break;
		case DISABLE_BUTTON:
			if(guiSKI_TransactionButton != 0)
				DisableButton( guiSKI_TransactionButton );
			if(guiSKI_DoneButton != 0)
				DisableButton( guiSKI_DoneButton );
			break;
		case UNLOAD_BUTTON:
			if(guiSKI_TransactionButtonImage != 0)
				UnloadButtonImage( guiSKI_TransactionButtonImage );
			if(guiSKI_DoneButtonImage != 0)
				UnloadButtonImage( guiSKI_DoneButtonImage );
			break;
	}
}

void EnableDisableEvaluateAndTransactionButtons()
{
	BOOLEAN	fItemsHere=FALSE;
	BOOLEAN	fItemEvaluated=FALSE;
	UINT32	uiArmsDealerTotalCost = CalculateTotalArmsDealerCost();
	UINT32	uiPlayersOfferAreaTotalCost = CalculateTotalPlayersValue();
	UINT32	uiPlayersOfferAreaTotalMoney = CalculateHowMuchMoneyIsInPlayersOfferArea( );
	
	//loop through the players offer area
	//loop through all the items in the players offer area and determine if they can be sold here.
	for ( int ubCnt = 0; ubCnt < gPlayersOfferActiveRegions; ++ubCnt )
	{
		//if there is an item here
		if( PlayersOfferArea[ ubCnt ].fActive )
		{
			fItemsHere = TRUE;

			//if the item has value
			if( PlayersOfferArea[ ubCnt ].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_VALUE )
			{
				//if the item isnt money ( which is always evaluated )
				if( Item[ PlayersOfferArea[ ubCnt ].sItemIndex ].usItemClass != IC_MONEY )
					fItemEvaluated = TRUE;

				//else if it is not a repair dealer, and the item is money
				else if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS && 
					Item[ PlayersOfferArea[ ubCnt ].sItemIndex ].usItemClass == IC_MONEY &&
					!( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL ) )
					fItemEvaluated = TRUE;
			}
		}
	}

	for ( int ubCnt = 0; ubCnt < gDealersOfferActiveRegions; ++ubCnt )
	{		
		//if the dealer is a repair dealer
		if ( armsDealerInfo[gbSelectedArmsDealerID].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			//if there is an item here, the item has to have been evaluated
			if ( ArmsDealerOfferArea[ubCnt].fActive )
			{
				fItemEvaluated = TRUE;
			}
		}
	}
	
	//if there are evaluated items here
	if( fItemEvaluated )
	{
//		//and if the players offer area value exceeds the shopkeeper's area
//		if( uiPlayersOfferAreaTotalCost >= uiArmsDealerTotalCost )
			EnableButton( guiSKI_TransactionButton );
//		else
//			DisableButton( guiSKI_TransactionButton );

/*
		//if the only item in the players offer area is money, and there is nothing in the dealers offer area
		if( IsMoneyTheOnlyItemInThePlayersOfferArea( ) && uiArmsDealerTotalCost == 0 )
			DisableButton( guiSKI_TransactionButton );
*/

		//If its a repair dealer, and there is no items in the Dealer Offer area,
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS &&
				CountNumberOfItemsInTheArmsDealersOfferArea( ) == 0 &&
				uiPlayersOfferAreaTotalMoney < uiArmsDealerTotalCost
			)
			DisableButton( guiSKI_TransactionButton );
	}
	//else if there is
	else if( uiArmsDealerTotalCost != 0 )
	{
		EnableButton( guiSKI_TransactionButton );
	}
	else
	{
		DisableButton( guiSKI_TransactionButton );
	}
			
	INT32 balance = LaptopSaveInfo.iCurrentBalance;
	if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
		balance = (INT32)GetIntel();

	if( uiArmsDealerTotalCost > uiPlayersOfferAreaTotalCost + balance )
	{
		DisableButton( guiSKI_TransactionButton );
	}
/* Allow transaction attempt when dealer can't afford to buy that much - he'll reject it with a special quote!
	else if( ( gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash + uiArmsDealerTotalCost ) < ( uiPlayersOfferAreaTotalCost - uiPlayersOfferAreaTotalMoney ) )
	{
		DisableButton( guiSKI_TransactionButton );
	}
*/
	else if( ( uiPlayersOfferAreaTotalCost == uiPlayersOfferAreaTotalMoney ) && ( uiPlayersOfferAreaTotalMoney > 0 ) && ( uiArmsDealerTotalCost == 0 ) )
	{
		DisableButton( guiSKI_TransactionButton );
	}
	
	//if the player is currently moving an item, disable the transaction button
	if( gMoveingItem.sItemIndex != 0 )
	{
		EnableDisableShopkeeperButtons(SHOPKEEPER_SCREEN, DISABLE_BUTTON);
	}
	else
	{
		EnableButton( guiSKI_DoneButton );
	}
	
//ARM: Always permit trying bribes, even if they don't work on a given dealer!
	// if the arms dealer is the kind of person who accepts gifts, and there is stuff to take
//	if( armsDealerInfo[ gbSelectedArmsDealerID ].uiFlags & ARMS_DEALER_ACCEPTS_GIFTS )
	{
		//if the player is giving the dealer money, without buying anything
		if( IsMoneyTheOnlyItemInThePlayersOfferArea( ) && CountNumberOfItemsInTheArmsDealersOfferArea( ) == 0 )
		{
			EnableButton( guiSKI_TransactionButton );
		}
	}

	if( gfUserHasRequestedToLeave )
		DisableButton( guiSKI_TransactionButton );
	
	//if the player is in the item desc for the arms dealer items
	if( InItemDescriptionBox( ) && pShopKeeperItemDescObject != NULL )
	{
		ButtonList[ guiSKI_TransactionButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ guiSKI_DoneButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
	}
	else
	{
		ButtonList[ guiSKI_TransactionButton ]->uiFlags &= ~BUTTON_FORCE_UNDIRTY;
		ButtonList[ guiSKI_DoneButton ]->uiFlags &= ~BUTTON_FORCE_UNDIRTY;
	}
}

void AddItemToPlayersOfferAreaAfterShopKeeperOpen( OBJECTTYPE *pItemObject)
{
	gItemToAdd.fActive						= TRUE;
	gItemToAdd.ItemObject = *pItemObject;
}

BOOLEAN IsMoneyTheOnlyItemInThePlayersOfferArea( )
{
	// we can't substitute money for intel
	if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
		return FALSE;

	BOOLEAN fFoundMoney = FALSE;

	for ( int ubCnt = 0; ubCnt < gPlayersOfferActiveRegions; ++ubCnt )
	{
		//if there is an item here
		if( PlayersOfferArea[ ubCnt ].fActive )
		{
			if( Item[ PlayersOfferArea[ ubCnt ].sItemIndex ].usItemClass != IC_MONEY )
				return( FALSE );
			else
				fFoundMoney = TRUE;
		}
	}

	if ( fFoundMoney )
		Assert(	CalculateHowMuchMoneyIsInPlayersOfferArea() > 0 );

	// only return TRUE if there IS money in the POA
	return( fFoundMoney );
}

UINT32 CalculateHowMuchMoneyIsInPlayersOfferArea( )
{
	// we can't substitute money for intel
	if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
		return 0;

	UINT32	uiTotalMoneyValue=0;

	for ( int ubCnt = 0; ubCnt < gPlayersOfferActiveRegions; ++ubCnt )
	{
		//if there is an item here
		if( PlayersOfferArea[ ubCnt ].fActive )
		{
			if( Item[ PlayersOfferArea[ ubCnt ].sItemIndex ].usItemClass == IC_MONEY )
			{
				uiTotalMoneyValue += PlayersOfferArea[ ubCnt ].ItemObject[0]->data.money.uiMoneyAmount;
			}
		}
	}

	return( uiTotalMoneyValue );
}

void MovePlayersItemsToBeRepairedToArmsDealersInventory()
{
	//loop through all the slots in the shopkeeper's offer area
	for ( int uiCnt = 0; uiCnt<gDealersOfferActiveRegions; ++uiCnt )
	{
		//if there is a item here
		if( ArmsDealerOfferArea[ uiCnt ].fActive )
		{
			// NOTE:  Any items that make it into a repairman's dealer offer area are guaranteed to be:
			//				a) Repairable
			//				b) Actually damaged
			//				c) Already stripped of all attachments
			//				d) If a gun, stripped of any non-ammo-class GunAmmoItems, and bullets

			// add it to the arms dealer's inventory
			GiveObjectToArmsDealerForRepair( gbSelectedArmsDealerID, &( ArmsDealerOfferArea[ uiCnt ].ItemObject ), ArmsDealerOfferArea[ uiCnt ].ubIdOfMercWhoOwnsTheItem );

			//Remove the item from the owner merc's inventory
			IfMercOwnedRemoveItemFromMercInv( &( ArmsDealerOfferArea[ uiCnt ]) );

			//erase the item from the dealer's offer area
			ClearArmsDealerOfferSlot( uiCnt );
		}
	}

	gfResetShopKeepIdleQuote = TRUE;
}

BOOLEAN RemoveRepairItemFromDealersOfferArea( INT16	bSlot )
{
	//if the item doesn't have a duplicate copy in its owner merc's inventory slot
	if( ArmsDealerOfferArea[ bSlot ].bSlotIdInOtherLocation == -1 )
	{
		if ( !SKITryToReturnInvToOwnerOrCurrentMerc( &( ArmsDealerOfferArea[ bSlot ] ) ) )
		{
			//failed to add item, inventory probably filled up or item is unowned and current merc ineligible
			return( FALSE );
		}
	}

	// Clear the contents of the dealer's offer slot and its help text
	ClearArmsDealerOfferSlot( bSlot );

	// Dirty
	fInterfacePanelDirty = DIRTYLEVEL2;
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL1;
	return( TRUE );
}

INT8	GetInvSlotOfUnfullMoneyInMercInventory( SOLDIERTYPE *pSoldier )
{
	//loop through the soldier's inventory
	INT8 invsize = (INT8)pSoldier->inv.size();
	for( INT8 bCnt=0; bCnt < invsize; ++bCnt)
	{
		// Look for MONEY only, not Gold or Silver!!!  And look for a slot not already full
		if( ( pSoldier->inv[ bCnt ].usItem == MONEY ) && ( pSoldier->inv[ bCnt ][0]->data.money.uiMoneyAmount < MoneySlotLimit( bCnt ) ) )
		{
			return( bCnt );
		}
	}

	return( NO_SLOT );
}

void ClearArmsDealerOfferSlot( INT32 ubSlotToClear )
{
	// Clear the contents
	ArmsDealerOfferArea[ ubSlotToClear ].initialize();

	//Remove the mouse help text from the region
	SetRegionFastHelpText( &gDealersOfferSlotsMouseRegions[ ubSlotToClear ], L"" );

	//if the dealer repairs
	if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
	{
		SetRegionFastHelpText( &gDealersOfferSlotsSmallFaceMouseRegions[ ubSlotToClear ], L"" );
	}
}

void ClearPlayersOfferSlot( INT32 ubSlotToClear )
{
	// Clear the contents
	PlayersOfferArea[ ubSlotToClear ].initialize();

	//Clear the text for the item
	SetRegionFastHelpText( &gPlayersOfferSlotsMouseRegions[ ubSlotToClear ], L"" );
	SetRegionFastHelpText( &gPlayersOfferSlotsSmallFaceMouseRegions[ ubSlotToClear ], L"" );

	// if the player offer area is clear, reset flags for transaction
	CheckAndHandleClearingOfPlayerOfferArea( );
}

void EvaluateItemAddedToPlayersOfferArea( INT8 bSlotID, BOOLEAN fFirstOne )
{
	UINT32	uiEvalResult = EVAL_RESULT_NORMAL;
	BOOLEAN	fRocketRifleWasEvaluated = FALSE;
	UINT8		ubNumberOfItemsAddedToRepairDuringThisEvaluation=0;
	
	// there better be an item there
	Assert ( PlayersOfferArea[ bSlotID ].fActive );

	//if money is the item being evaluated, leave
	if( Item[ PlayersOfferArea[ bSlotID ].sItemIndex ].usItemClass == IC_MONEY )
		return;

	// if already evaluated, don't do it again
	if( PlayersOfferArea[ bSlotID ].uiFlags & ARMS_INV_PLAYERS_ITEM_HAS_BEEN_EVALUATED )
	{
		return;
	}

	// say "Hmm... Let's see" once per trading session to start evaluation
	// SPECIAL: Devin doesn't have this quote (he's the only one)

#ifdef JA2UB
//no UB
#else
	if( !gfDealerHasSaidTheEvaluateQuoteOnceThisSession && ( gbSelectedArmsDealerID != ARMS_DEALER_DEVIN ) )
	{
		gfDealerHasSaidTheEvaluateQuoteOnceThisSession = TRUE;
		StartShopKeeperTalking( SK_QUOTES_PLAYER_REQUESTED_EVALUATION );
	}
#endif

	//Can this particular kind of item be sold/repaired here
	if( WillShopKeeperRejectObjectsFromPlayer( gbSelectedArmsDealerID, bSlotID ) == FALSE )
	{
		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			int	uiNumberOfItemsInForRepairs = CountTotalItemsRepairDealerHasInForRepairs( gbSelectedArmsDealerID );
			int	uiNumberOfItemsAlreadyEvaluated = CountNumberOfItemsInTheArmsDealersOfferArea();

			//Get the number of items being evaluated
			ubNumberOfItemsAddedToRepairDuringThisEvaluation = PlayersOfferArea[ bSlotID ].ItemObject.ubNumberOfObjects;

			//if there is already enough items in for repairs, complain about it and DON'T accept the item for repairs
			if( ( uiNumberOfItemsAlreadyEvaluated + ubNumberOfItemsAddedToRepairDuringThisEvaluation + uiNumberOfItemsInForRepairs ) > SKI_MAX_AMOUNT_OF_ITEMS_DEALER_CAN_REPAIR_AT_A_TIME )
			{
				if ( !gfAlreadySaidTooMuchToRepair )
				{
					//Start the dealer talking
					StartShopKeeperTalking( SK_QUOTES_CANT_AFFORD_TO_BUY_OR_TOO_MUCH_TO_REPAIR );
					gfAlreadySaidTooMuchToRepair = TRUE;
				}

				return;
			}

			//if the item is a rocket rifle
			if( ItemIsARocketRifle( PlayersOfferArea[ bSlotID ].sItemIndex ) )
			{
				fRocketRifleWasEvaluated = TRUE;
			}
			
			//if the item is damaged, or is a rocket rifle (which always "need repairing" even at 100%, to reset imprinting)
			if( ( PlayersOfferArea[ bSlotID ].ItemObject[0]->data.objectStatus < 100 ) || fRocketRifleWasEvaluated )
			{
				// Move the item to the Dealer's Offer Area
				INT16 bSlotAddedTo = AddItemToArmsDealerOfferArea( &PlayersOfferArea[ bSlotID ], PlayersOfferArea[ bSlotID ].bSlotIdInOtherLocation );

				if( bSlotAddedTo != -1 )
				{
					// Clear the contents
					ClearPlayersOfferSlot( bSlotID );

/*	ARM: Leave it there, until transaction occurs it should be recallable
					//Remove the item from the owner merc's inventory
					IfMercOwnedRemoveItemFromMercInv( &(ArmsDealerOfferArea[ bSlotAddedTo ]) );
*/

					//Mark the item as unselected, signifying that it can be moved
					ArmsDealerOfferArea[ bSlotAddedTo ].uiFlags &= ~ARMS_INV_ITEM_SELECTED;

					//increment the number of items being added
					++ubNumberOfItemsAddedToRepairDuringThisEvaluation;

					// check if the item is really badly damaged
					if( Item[ ArmsDealerOfferArea[ bSlotAddedTo ].sItemIndex ].usItemClass != IC_AMMO )
					{
						if( ArmsDealerOfferArea[ bSlotAddedTo ].ItemObject[0]->data.objectStatus < REALLY_BADLY_DAMAGED_THRESHOLD )
						{
							uiEvalResult = EVAL_RESULT_OK_BUT_REALLY_DAMAGED;
						}
					}

					// check if it's the first time a rocket rifle is being submitted to Fredo
					if( fRocketRifleWasEvaluated && ( gbSelectedArmsDealerID == ARMS_DEALER_FREDO ) )
					{
						//if he hasn't yet said his quote
						if( !( gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__FREDO_HAS_SAID_ROCKET_RIFLE_QUOTE ) )
						{
							// use this special evaluation result instead (has a unique quote)
							uiEvalResult = EVAL_RESULT_ROCKET_RIFLE;
						}
					}
				}
				else
				{
					#ifdef JA2BETAVERSION
						ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Failed to add repair item to ArmsDealerOfferArea.  AM-0");
					#endif
					return;
				}
			}
			else
			{
				uiEvalResult = EVAL_RESULT_NOT_DAMAGED;
			}
		}
		else	// not a repairman
		{
			uiEvalResult = EvaluateInvSlot( & ( PlayersOfferArea[ bSlotID ] ) );
		}
	}
	else	// dealer doesn't handle this type of object
	{
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			// only otherwise repairable items count as actual rejections
//			if ( Item[ PlayersOfferArea[ bSlotID ].sItemIndex ].fFlags & ITEM_REPAIRABLE )
			if ( Item[ PlayersOfferArea[ bSlotID ].sItemIndex ].repairable  )
			{
				uiEvalResult = EVAL_RESULT_DONT_HANDLE;
			}
			else
			{
				uiEvalResult = EVAL_RESULT_NON_REPAIRABLE;
			}
		}
		else
		{
			uiEvalResult = EVAL_RESULT_DONT_HANDLE;
		}
	}
	
	// mark this item as having been evaluated
	PlayersOfferArea[ bSlotID ].uiFlags |= ARMS_INV_PLAYERS_ITEM_HAS_BEEN_EVALUATED;
	
	// when evaluating complex items that get split into multiple subobjects, dealer will talk only about the first one!
	// don't bother with any of this if shopkeeper can't talk right now
	if ( fFirstOne && CanShopkeeperOverrideDialogue( ) )
	{
		INT16 sQuoteNum = -1;
		UINT8 ubChanceToSayNormalQuote;
		
		switch ( uiEvalResult )
		{
			case EVAL_RESULT_DONT_HANDLE:
#ifdef JA2UB
			//JA25 UB
			if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_SELLS_ONLY )
				{
					// then he doesn't have quotes 17, 19, or 20, always use 4.  Devin doesn't have 18 either,
					// while the text of 18 seems wrong for Sam & Howard if offered something they should consider valuable.
					sQuoteNum = SK_QUOTES_NOT_INTERESTED_IN_THIS_ITEM;			
				}
				else
				{
					//if the dealer is BETTY
					if( gbSelectedArmsDealerID == ARMS_DEALER_BETTY && 
							( Item[ PlayersOfferArea[ bSlotID ].sItemIndex ].usItemClass == IC_AMMO ||
								Item[ PlayersOfferArea[ bSlotID ].sItemIndex ].usItemClass == IC_GUN 
							)
						)
					{
						// she accepts items, but not this one
						sQuoteNum = SK_QUOTES_NPC_SPECIFIC_40;
					}
					else
					{
						// he accepts items, but not this one
						sQuoteNum = SK_QUOTES_DURING_EVALUATION_STUFF_REJECTED;
					}
				}
#endif
				if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_SELLS_ONLY )
				{
					// then he doesn't have quotes 17, 19, or 20, always use 4.  Devin doesn't have 18 either,
					// while the text of 18 seems wrong for Sam & Howard if offered something they should consider valuable.
					sQuoteNum = SK_QUOTES_NOT_INTERESTED_IN_THIS_ITEM;
				}
				else
				{
					// he accepts items, but not this one
					sQuoteNum = SK_QUOTES_DURING_EVALUATION_STUFF_REJECTED;
				}
				break;

			case EVAL_RESULT_NON_REPAIRABLE:
				// use quote 4 for this situation...  It's a bit weird but better than "that looks fine / ain't broke"
				sQuoteNum = SK_QUOTES_NOT_INTERESTED_IN_THIS_ITEM;
				break;

			case EVAL_RESULT_NOT_DAMAGED:
			case EVAL_RESULT_WORTHLESS:
				sQuoteNum = SK_QUOTES_EVALUATION_RESULT_VALUE_OF_ZERO;
				break;

			case EVAL_RESULT_OK_BUT_REALLY_DAMAGED:
				sQuoteNum = SK_QUOTES_EVALUATION_RESULT_SOME_REALLY_DAMAGED_ITEMS;
				break;

			case EVAL_RESULT_ROCKET_RIFLE:
				sQuoteNum = FREDO_QUOTE_CAN_RESET_IMPRINTING;
				break;

			case EVAL_RESULT_NORMAL:
				//if it has been a long time since saying the last quote
				if( guiLastTimeDealerSaidNormalEvaluationQuote == 0 )
					ubChanceToSayNormalQuote = 100;
				else if( ( GetJA2Clock() - guiLastTimeDealerSaidNormalEvaluationQuote ) > 25000 )
					ubChanceToSayNormalQuote = 60;
				else if( ( GetJA2Clock() - guiLastTimeDealerSaidNormalEvaluationQuote ) > 12000 )
					ubChanceToSayNormalQuote = 25;
				else if( ( GetJA2Clock() - guiLastTimeDealerSaidNormalEvaluationQuote ) > 7000 )
					ubChanceToSayNormalQuote = 10;
				else
					ubChanceToSayNormalQuote = 0;

				if( Chance( ubChanceToSayNormalQuote ) )
				{
					sQuoteNum = SK_QUOTES_EVALUATION_RESULT_NORMAL;
				}
				break;

			default:
				#ifdef JA2BETAVERSION
					ScreenMsg( FONT_MCOLOR_WHITE, MSG_BETAVERSION, L"Invalid evaluation result of %d.  AM-0", uiEvalResult );
				#endif
				break;
		}

		// if a valid quote was selected
		if ( sQuoteNum != -1 )
		{
			// if the appropriate quote hasn't already been used during this evaluation cycle
			if ( !gfEvalResultQuoteSaid[ uiEvalResult ] )
			{
				// try to say it
				if ( StartShopKeeperTalking( ( UINT16 ) sQuoteNum ) )
				{
					// set flag to keep from repeating it
					gfEvalResultQuoteSaid[ uiEvalResult ] = TRUE;

					// if it's the normal result
					if ( uiEvalResult == EVAL_RESULT_NORMAL )
					{
						// set new time of last usage
						guiLastTimeDealerSaidNormalEvaluationQuote = GetJA2Clock();
					}

					if ( sQuoteNum == FREDO_QUOTE_CAN_RESET_IMPRINTING )
					{
						//Set the fact that we have said this (to prevent that quote from coming up again)
						gArmsDealerStatus[ gbSelectedArmsDealerID ].ubSpecificDealerFlags |= ARMS_DEALER_FLAG__FREDO_HAS_SAID_ROCKET_RIFLE_QUOTE;
					}
				}
			}
		}
	}
}

BOOLEAN DoSkiMessageBox( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT8 ubFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect pCenteringRect= {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - INV_INTERFACE_HEIGHT };

	// reset exit mode
	gfExitSKIDueToMessageBox = TRUE;

	// do message box and return
	giSKIMessageBox = DoMessageBox( ubStyle, zString, uiExitScreen, ( UINT8 ) ( ubFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ), ReturnCallback, &pCenteringRect );

	// send back return state
	return( ( giSKIMessageBox != -1 ) );
}

void ConfirmDontHaveEnoughForTheDealerMessageBoxCallBack( UINT8 bExitValue )
{
	// simply redraw the panel
	if( bExitValue == MSG_BOX_RETURN_OK )
	{
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
	}

	// done, re-enable calls to PerformTransaction()
	gfPerformTransactionInProgress = FALSE;
}

void ConfirmToDeductMoneyFromPlayersAccountMessageBoxCallBack( UINT8 bExitValue )
{
	// yes, deduct the money
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		UINT32	uiPlayersOfferAreaValue = CalculateTotalPlayersValue();
		UINT32	uiArmsDealersItemsCost = CalculateTotalArmsDealerCost();
		INT32		iMoneyToDeduct = (INT32)( uiArmsDealersItemsCost - uiPlayersOfferAreaValue );

		//Perform the transaction with the extra money from the players account
		PerformTransaction( iMoneyToDeduct );

		AddTransactionToPlayersBook( PURCHASED_ITEM_FROM_DEALER, armsDealerInfo[ gbSelectedArmsDealerID ].ubShopKeeperID, GetWorldTotalMin(), -iMoneyToDeduct );
	}

	// done, re-enable calls to PerformTransaction()
	gfPerformTransactionInProgress = FALSE;

	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}

void ConfirmToDeductIntelFromPlayersAccountMessageBoxCallBack( UINT8 bExitValue )
{
	// yes, deduct the money
	if ( bExitValue == MSG_BOX_RETURN_YES )
	{
		UINT32	uiPlayersOfferAreaValue = CalculateTotalPlayersValue();
		UINT32	uiArmsDealersItemsCost = CalculateTotalArmsDealerCost();
		INT32		iMoneyToDeduct = (INT32)( uiArmsDealersItemsCost - uiPlayersOfferAreaValue );

		//Perform the transaction with the extra money from the players account
		PerformTransaction( iMoneyToDeduct );

		AddIntel( (FLOAT)-iMoneyToDeduct, TRUE );
	}

	// done, re-enable calls to PerformTransaction()
	gfPerformTransactionInProgress = FALSE;

	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}

// run through what the player has on the table and see if the shop keep will aceept it or not
BOOLEAN WillShopKeeperRejectItemFromPlayer( INT8 bDealerId, UINT16 usItem )
{
	BOOLEAN fRejected = TRUE;

	if ( Item[usItem].usItemClass == IC_MONEY )
	{
		// we can't substitute money for intel
		if ( armsDealerInfo[gbSelectedArmsDealerID].uiFlags & ARMS_DEALER_DEALWITHINTEL )
			fRejected = TRUE;
		else
			fRejected = FALSE;
	}
	else if ( CanDealerTransactItem( gbSelectedArmsDealerID, usItem, TRUE ) )
	{
		fRejected = FALSE;
	}
	else
	{
		fRejected = TRUE;
	}

	return fRejected;
}

BOOLEAN DoesCurrentDealerRefuseToTradeItem( UINT16 usItem )
{
	//if non-repairman
	if ( armsDealerInfo[gbSelectedArmsDealerID].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS )
	{
		// don't evaluate anything he wouldn't buy!
		if ( WillShopKeeperRejectItemFromPlayer( gbSelectedArmsDealerID, usItem ) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// run through what the player has on the table and see if the shop keep will aceept it or not
BOOLEAN WillShopKeeperRejectObjectsFromPlayer( INT8 bDealerId, INT8 bSlotId )
{
	return WillShopKeeperRejectItemFromPlayer( bDealerId, PlayersOfferArea[bSlotId].sItemIndex );
}

void CheckAndHandleClearingOfPlayerOfferArea( void )
{
	BOOLEAN fActiveSlot = FALSE;

	// find out if all the player trade slots/offer area is empty
	for ( int iCounter = 0; iCounter<gPlayersOfferActiveRegions; ++iCounter )
	{
		if( PlayersOfferArea[ iCounter ].fActive == TRUE )
		{
			// nope, there is an active slot
			fActiveSlot = TRUE;

			break;
		}
	}

	// if all player's offer area slots are empty
	if( fActiveSlot == FALSE )
	{
		ResetAllQuoteSaidFlags();
		gfRemindedPlayerToPickUpHisStuff = FALSE;
	}
}

BOOLEAN CanShopkeeperOverrideDialogue( void )
{
	// if he's not currently saying anything
	if( !gfIsTheShopKeeperTalking && ( giShopKeepDialogueEventinProgress == -1 ) )
	{
		return( TRUE );
	}

	// if the quote he is currently saying is unimportant (fluff),
	// then shut him up & allow the override
	if( giShopKeepDialogueEventinProgress < SK_QUOTES_PLAYER_REQUESTED_EVALUATION )
	{
		// shutup the shop keep and do this quote
		ShutUpShopKeeper( );
		return( TRUE );
	}

	// he's currently saying something important
	return( FALSE );
}

INT16 GetNumberOfItemsInPlayerOfferArea( void )
{
	INT16 sCounter = 0;

	// find number of active slot in player offer area
	for ( int iCounter = 0; iCounter < gPlayersOfferActiveRegions; ++iCounter )
	{
		if( PlayersOfferArea[ iCounter ].fActive )
		{
			++sCounter;
		}
	}

	return( sCounter );
}

void HandleCheckIfEnoughOnTheTable( void )
{
	static INT32 iLastTime = 0;
	INT32 iDifference = 0, iRand = 0;
	UINT32	uiPlayersOfferAreaValue = CalculateTotalPlayersValue();
	UINT32	uiArmsDealersItemsCost = CalculateTotalArmsDealerCost();

	if( ( iLastTime == 0 ) || gfResetShopKeepIdleQuote )
	{
		iLastTime = GetJA2Clock();
		gfResetShopKeepIdleQuote = FALSE;
	}

	iDifference = GetJA2Clock() - iLastTime;

	iRand = Random( 100 );

	// delay for shopkeeper passed?
	if( iDifference > DELAY_FOR_SHOPKEEPER_IDLE_QUOTE )
	{
		// random chance enough?
		if( iRand > CHANCE_FOR_SHOPKEEPER_IDLE_QUOTE )
		{
			// is there enough on the table
			if( ( uiArmsDealersItemsCost > uiPlayersOfferAreaValue ) && ( uiPlayersOfferAreaValue ) )
			{
				StartShopKeeperTalking( SK_QUOTES_EVAULATION_PLAYER_DOESNT_HAVE_ENOUGH_VALUE );
			}
		}

		gfResetShopKeepIdleQuote = TRUE;
	}
}

void InitShopKeeperItemDescBox( OBJECTTYPE *pObject, UINT16 ubPocket, UINT8 ubFromLocation )
{
	INT16	sPosX, sPosY;
			
	bool showItemDescriptionBox = false;
	switch( ubFromLocation )
	{
		case ARMS_DEALER_INVENTORY:
		{
			showItemDescriptionBox = true;

			UINT16	ubSelectedInvSlot = ubPocket - gSelectArmsDealerInfo.ubFirstItemIndexOnPage;

			if( gGameSettings.fOptions[ TOPTION_ENHANCED_DESC_BOX ] == FALSE)
			{
				sPosX = SKI_TRADER_INVENTORY_BOXES_BEGIN_X;
				sPosY = SKI_TRADER_INVENTORY_BOXES_BEGIN_Y;
			}
			else
			{
				sPosX = SCREEN_X_OFFSET + 2;
				sPosY = SCREEN_Y_OFFSET + 146;
			}

			//if the start position + the height of the box is off the screen, reposition
			if( sPosY < (0 + SCREEN_Y_OFFSET) )
				sPosY = 0 + SCREEN_Y_OFFSET;

			//if the start position + the width of the box is off the screen, reposition
			if( ( sPosX + 358 ) > SCREEN_WIDTH )
				sPosX = sPosX - ( ( sPosX + 358 ) - SCREEN_WIDTH ) - 5;

			//if it is starting to far to the left
			else if( sPosX < (0 + SCREEN_X_OFFSET) )
				sPosX = 0 + SCREEN_X_OFFSET;

			//if the box will appear over the mercs face, move the box over so it doesn't obstruct the face
			if( sPosY < SKI_FACE_Y + SKI_FACE_HEIGHT + 20 )
				if( sPosX < (SCREEN_X_OFFSET + 160) )
					sPosX = SCREEN_X_OFFSET + 160;
		}
		break;

		case ARMS_DEALER_OFFER_AREA:
		{
			showItemDescriptionBox = false;

			sPosX = SKI_TRADER_OFFER_BOXES_BEGIN_X;
			sPosY = SKI_TRADER_OFFER_BOXES_BEGIN_Y;

			//if the start position + the height of the box is off the screen, reposition
			if( sPosY < (SCREEN_Y_OFFSET + 0) )
				sPosY = 0 + SCREEN_Y_OFFSET;

			//if the start position + the width of the box is off the screen, reposition
			if( ( sPosX + 358 ) > SCREEN_WIDTH )
				sPosX = sPosX - ( ( sPosX + 358 ) - SCREEN_WIDTH ) - 5;

			//if it is starting to far to the left
			else if( sPosX < (0 + SCREEN_X_OFFSET) )
				sPosX = 10 + SCREEN_X_OFFSET;

			//if the box will appear over the mercs face, move the box over so it doesn't obstruct the face
			if( sPosY < SKI_FACE_Y + SKI_FACE_HEIGHT + 20 )
				if( sPosX < (160 + SCREEN_X_OFFSET) )
					sPosY = SCREEN_Y_OFFSET + INV_INTERFACE_HEIGHT;
		}
		break;

		default:
			Assert( 0 );
			return;
	}
	
	// WANNE: Only allow right click (item desription box) in the arms dealer inventory
	if (showItemDescriptionBox)
	{
		pShopKeeperItemDescObject = pObject;

		InitItemDescriptionBox( gpSMCurrentMerc, 255, sPosX, sPosY, 0 );

		StartSKIDescriptionBox( );
	}
}

void StartSKIDescriptionBox( void )
{
	//shadow the entire screen
	DrawHatchOnInventory( FRAME_BUFFER, SKI_TOTAL_BEGIN_X, SKI_TOTAL_BEGIN_Y, SKI_TOTAL_END_X - SKI_TOTAL_BEGIN_X, SKI_TOTAL_END_Y - SKI_TOTAL_BEGIN_Y );

	InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	// disable almost everything!

	gfSMDisableForItems = TRUE;
	DisableInvRegions( gfSMDisableForItems );

	DisableButton( guiSKI_InvPageUpButton );
	DisableButton( guiSKI_InvPageDownButton );
	EnableDisableShopkeeperButtons(SHOPKEEPER_SCREEN, DISABLE_BUTTON);

	DisableAllDealersInventorySlots();
	DisableAllDealersOfferSlots();

	for ( int iCnt = 0; iCnt < gPlayersOfferActiveRegions; ++iCnt )
	{
		MSYS_DisableRegion( &gPlayersOfferSlotsMouseRegions[ iCnt ] );
		MSYS_DisableRegion( &gPlayersOfferSlotsSmallFaceMouseRegions[ iCnt ] );
	}

	if( gShopKeeperSubTitleMouseRegion.uiFlags & MSYS_REGION_EXISTS )
	{
		MSYS_DisableRegion( &gShopKeeperSubTitleMouseRegion );
	}

	RenderItemDescriptionBox( );
}

void DeleteShopKeeperItemDescBox()
{
	pShopKeeperItemDescObject = NULL;
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

	//Redraw the face
	if ( giShopKeeperFaceIndex > -1 )
		gFacesData[ giShopKeeperFaceIndex ].uiFlags |= FACE_REDRAW_WHOLE_FACE_NEXT_FRAME;
	
	// enable almost everything!

	CheckForDisabledForGiveItem( );
	DisableInvRegions( gfSMDisableForItems );	//actually does an enable if current merc is in range

	EnableButton( guiSKI_InvPageUpButton );
	EnableButton( guiSKI_InvPageDownButton );
	EnableDisableShopkeeperButtons(SHOPKEEPER_SCREEN, ENABLE_BUTTON);

	EnableDisableDealersInventoryPageButtons();
	EnableDisableEvaluateAndTransactionButtons();
	
	EnableAllDealersInventorySlots();
	EnableAllDealersOfferSlots();

	for ( int iCnt = 0; iCnt < gPlayersOfferActiveRegions; ++iCnt )
	{
		MSYS_EnableRegion( &gPlayersOfferSlotsMouseRegions[ iCnt ] );
		MSYS_EnableRegion( &gPlayersOfferSlotsSmallFaceMouseRegions[ iCnt ] );
	}

	if( gShopKeeperSubTitleMouseRegion.uiFlags & MSYS_REGION_EXISTS )
	{
		MSYS_EnableRegion( &gShopKeeperSubTitleMouseRegion );
	}
}

BOOLEAN OfferObjectToDealer( OBJECTTYPE *pComplexObject, UINT8 ubOwnerProfileId, INT16 bOwnerSlotId )
{
	UINT8		ubRepairableSubObjects;
	UINT8		ubNonRepairableSubObjects;
	int			ubDealerOfferAreaSlotsNeeded;
	int			ubPlayerOfferAreaSlotsNeeded;
	int			ubDiff;
	int			ubHowManyMoreItemsCanDealerTake;
	BOOLEAN	fFirstOne = TRUE;
	BOOLEAN fSuccess = FALSE;

	if ( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer != ARMS_DEALER_REPAIRS )
	{
		// if not actually doing repairs, there's no need to split objects up at all
		if ( !AddObjectForEvaluation( pComplexObject, ubOwnerProfileId, bOwnerSlotId, TRUE ) )
		{
			gfDisplayNoRoomMsg = TRUE;
			return(FALSE);	// no room
		}
	}
	else	// dealing with a repairman
	{
		// split the object into its components
		SplitComplexObjectIntoSubObjects( pComplexObject );

		// determine how many subobjects of each category this complex object will have to be broken up into
		CountSubObjectsInObject( pComplexObject, &ubRepairableSubObjects, &ubNonRepairableSubObjects );

		// in the simplest situation, the # subobjects of each type gives us how many slots of each type are needed
		ubDealerOfferAreaSlotsNeeded = ubRepairableSubObjects;
		ubPlayerOfferAreaSlotsNeeded = ubNonRepairableSubObjects;

		// consider that if dealer is at or will reach his max # of items repaired limit, not everything repairable will move
		ubHowManyMoreItemsCanDealerTake = SKI_MAX_AMOUNT_OF_ITEMS_DEALER_CAN_REPAIR_AT_A_TIME -
										CountTotalItemsRepairDealerHasInForRepairs( gbSelectedArmsDealerID ) -
										CountNumberOfItemsInTheArmsDealersOfferArea();

		// if he can't repair everything repairable that we're about to submit, the space we'll need changes
		if ( ubDealerOfferAreaSlotsNeeded > ubHowManyMoreItemsCanDealerTake )
		{
			ubDiff = ubDealerOfferAreaSlotsNeeded - ubHowManyMoreItemsCanDealerTake;
			ubDealerOfferAreaSlotsNeeded -= ubDiff;
			ubPlayerOfferAreaSlotsNeeded += ubDiff;
		}

		// if there is anything repairable that dealer will accept
		if ( ubDealerOfferAreaSlotsNeeded > 0 )
		{
			// we need at least one EXTRA empty slot in player's area to pass repairable objects through for their evaluation
			++ubPlayerOfferAreaSlotsNeeded;
		}

		//this is a silly assert, was it meant to be something else?
		Assert( SKI_MAX_AMOUNT_OF_ITEMS_DEALER_CAN_REPAIR_AT_A_TIME < (SKI_PLAYER_OFFER_BOXES_PER_ROW * SKI_PLAYER_OFFER_BOXES_PER_COL) );
/*
		This code is commented out because a repair dealer will never be allowed to repair over more then
		SKI_MAX_AMOUNT_OF_ITEMS_DEALER_CAN_REPAIR_AT_A_TIME ( 4 ) items at a time, therefore, cant fill up the dealer offer area

		// check if we have room for all of each type
		if ( ( ubDealerOfferAreaSlotsNeeded + CountNumberOfItemsInTheArmsDealersOfferArea() ) > SKI_NUM_TRADING_INV_SLOTS )
		{
			// tell player there's not enough room in the dealer's offer area
			DoSkiMessageBox( MSG_BOX_BASIC_STYLE, L"There is not enough room in the dealers repair area.", SHOPKEEPER_SCREEN, MSG_BOX_FLAG_OK, NULL );
			return(FALSE);	// no room
		}
*/

		if ( (ubPlayerOfferAreaSlotsNeeded + CountNumberOfItemsInThePlayersOfferArea( )) > (SKI_PLAYER_OFFER_BOXES_PER_ROW * SKI_PLAYER_OFFER_BOXES_PER_COL) )
		{
			gfDisplayNoRoomMsg = TRUE;
			return(FALSE);	// no room
		}

		// we have room, so move them all to the appropriate slots
		for ( size_t ubSubObject = 0; ubSubObject < subObjects.size(); ++ubSubObject )
		{
			// if it's the main item itself (always in the very first subobject), and it has no other subobjects
			if ( ( ubSubObject == 0 ) && ( subObjects.size() == 1) )
			{
				// store its owner merc as the owner, and store the correct slot
				fSuccess = AddObjectForEvaluation( &subObjects[ ubSubObject ], ubOwnerProfileId, bOwnerSlotId, fFirstOne );
			}
			else	// attachments, bullets/payload
			{
				// store it with a valid owner, but an invalid slot, so it still shows who owns it, but can't return to its slot
// ARM: New code will be needed here if we add parent/child item support & interface
				fSuccess = AddObjectForEvaluation( &subObjects[ ubSubObject ], ubOwnerProfileId, -1, fFirstOne );
			}

			// it has to succeed, or we have a bug in our earlier check for sufficient room
			Assert( fSuccess );

			fFirstOne = FALSE;
		}

		gfAlreadySaidTooMuchToRepair = FALSE;
	}

//ARM: This comment isn't true unless parent/child support is added.  Right now repairmen don't do this!
	// NOTE that, either way, if owned, the item remains in the merc's inventory (hatched out) until the transaction is completed

	// Dirty
	fInterfacePanelDirty = DIRTYLEVEL2;

	return(TRUE);		// it worked
}

void SplitComplexObjectIntoSubObjects( OBJECTTYPE *pComplexObject )
{
	Assert( pComplexObject );
	Assert( pComplexObject->exists() == true );

	subObjects.clear();
	int usItem = pComplexObject->usItem;
	//CHRISL: This won't work.  As we his MoveThisObjectTo, we reduce ubNumberOfObjects.  If we started with ubNumberOfObjects=4 & x=0,
	//	our first iteration would be ubNumberOfObjects=3 & x=1.  Then ubNumberOfObjects=2 & x=2, which ends the loop.  We want to 
	//	run the loop ubNumberOfObjects number of times regardless of whether ubNumberOfObjects is reduced.  Use a while loop.
	//for (int x = 0; x < pComplexObject->ubNumberOfObjects; ++x) {
	while(pComplexObject->ubNumberOfObjects > 0) {
		//we need not worry about attachments!!!!

		if (pComplexObject->MoveThisObjectTo(gTempObject, 1) == 0) {
			subObjects.push_back(gTempObject);
			StackedObjectData* pData = subObjects.back()[0];

			// if it's a gun
			if ( Item [ usItem ].usItemClass == IC_GUN )
			{
				int usGunAmmoItem = pData->data.gun.usGunAmmoItem;
				int ubGunShotsLeft = pData->data.gun.ubGunShotsLeft;
				int bGunAmmoStatus = pData->data.gun.bGunAmmoStatus;

				// strip off any loaded ammo/payload
				// Exception: don't do this with rocket launchers, their "shots left" are fake and this screws 'em up!
				if ( !Item[usItem].singleshotrocketlauncher ) // Madd rpg - still do this
				{
					pData->data.gun.usGunAmmoItem = NONE;
					pData->data.gun.ubGunShotsLeft = 0;
					pData->data.gun.ubGunState = 0;
					pData->data.gun.ubGunAmmoType = 0;
					pData->data.gun.bGunAmmoStatus = 0;
				}

				// and it has ammo/payload
				if ( usGunAmmoItem != NONE )
				{
					// if it's bullets
					if ( Item[ usGunAmmoItem ].usItemClass == IC_AMMO )
					{
						// and there are some left
						if ( ubGunShotsLeft > 0 )
						{
							// make the bullets into another subobject
							CreateAmmo( usGunAmmoItem, &gTempObject, ubGunShotsLeft );
							subObjects.push_back(gTempObject);
						}
						// ignore this if it's out of bullets
					}
					else	// non-ammo payload
					{
						// if the gun was jammed, fix up the payload's status
						if ( bGunAmmoStatus < 0 )
						{
							bGunAmmoStatus *= -1;
						}

						// make the payload into another subobject
						CreateItem( usGunAmmoItem, bGunAmmoStatus, &gTempObject );
						subObjects.push_back(gTempObject);
					}
				}
			}
		}
	}
}

void CountSubObjectsInObject( OBJECTTYPE *pComplexObject, UINT8 *pubRepairableSubObjects, UINT8 *pubNonRepairableSubObjects )
{
	UINT8 ubSubObject;

	*pubRepairableSubObjects		= 0;
	*pubNonRepairableSubObjects = 0;

	// check every subobject and count it as either repairable or non-
	for ( ubSubObject = 0; ubSubObject < subObjects.size(); ubSubObject++ )
	{
		// is it in need of fixing, and also repairable by this dealer?
		// A jammed gun with a 100% status is NOT repairable - shouldn't ever happen
		if ( ( subObjects[ ubSubObject ][0]->data.objectStatus != 100 ) &&
			CanDealerRepairItem( gbSelectedArmsDealerID, subObjects[ ubSubObject ].usItem ) )
		{
			( *pubRepairableSubObjects )++;
		}
		else
		{
			( *pubNonRepairableSubObjects )++;
		}
	}
}

BOOLEAN AddObjectForEvaluation(OBJECTTYPE *pObject, UINT8 ubOwnerProfileId, INT16 bOwnerSlotId, BOOLEAN fFirstOne )
{
	INVENTORY_IN_SLOT InvSlot;
	INT8	bAddedToSlotID;

	// Make a new inv slot out of the subobject
	InvSlot.ItemObject = *pObject;

	InvSlot.sItemIndex = pObject->usItem;
	InvSlot.ubLocationOfObject = PLAYERS_INVENTORY;
	InvSlot.ubIdOfMercWhoOwnsTheItem = ubOwnerProfileId;

	//Add the item to the Players Offer Area
	bAddedToSlotID = AddItemToPlayersOfferArea( ubOwnerProfileId, &InvSlot, bOwnerSlotId );

	//Do the evaluation for the item if it was added correctly
	if( bAddedToSlotID != -1 )
	{
		// This will move any repairable (sub)objects into the dealer's offer area immediately
		EvaluateItemAddedToPlayersOfferArea( bAddedToSlotID, fFirstOne );
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

// The Shopkeeper interface *MUST* use this intermediary function instead of calling AutoPlaceObject() directly!
// This is because the OBJECTTYPEs used within Shopkeeper may contain an illegal ubNumberOfObjects
BOOLEAN ShopkeeperAutoPlaceObject( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObject, BOOLEAN fNewItem )
{
	// the entire pObj will get memset to 0 by RemoveObjs() if all the items are successfully placed,
	// so we have to keep a copy to retrieve with every iteration of the loop
	OBJECTTYPE movingObject;
	
	while ( pObject->exists() == true )
	{
		// figure out how many to place during this loop iteration.  Can't do more than MAX_OBJECTS_PER_SLOT at a time
		int numToMove = min( MAX_OBJECTS_PER_SLOT, pObject->ubNumberOfObjects);

		pObject->MoveThisObjectTo(movingObject, numToMove);
		if (!AutoPlaceObject( pSoldier, &movingObject, fNewItem ))
		//if (!AutoPlaceObject( pSoldier, pObject, fNewItem ))
		{
			//CHRISL: When we call this, we don't care about cap limits so stack up to max
			// no more room, didn't all fit - add back in any that we didn't even get to yet
			pObject->AddObjectsToStack(movingObject, -1, 0, NUM_INV_SLOTS, MAX_OBJECTS_PER_SLOT);
			return( FALSE );
		}
	}

	return( TRUE );
}

// The Shopkeeper interface *MUST* use this intermediary function instead of calling AddItemToPool() directly!
// This is because the OBJECTTYPEs used within Shopkeeper may contain an illegal ubNumberOfObjects
void ShopkeeperAddItemToPool( INT32 sGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel )
{
	// the entire pObj will get memset to 0 by RemoveObjs() if all the items are successfully placed,
	// so we have to keep a copy to retrieve with every iteration of the loop
	OBJECTTYPE movingObject;

	while ( pObject->exists() == true )
	{
		// figure out how many to place during this loop iteration.  Can't do more than MAX_OBJECTS_PER_SLOT at a time
		int numToMove = min( MAX_OBJECTS_PER_SLOT, pObject->ubNumberOfObjects);

		pObject->MoveThisObjectTo(movingObject, numToMove);
		AddItemToPool( sGridNo, &movingObject, bVisible, ubLevel, usFlags, bRenderZHeightAboveLevel );
	}
}

void IfMercOwnedCopyItemToMercInv( INVENTORY_IN_SLOT *pInv )
{
	INT16 sSoldierID;

	//if the item picked up was in a previous location, and that location is on a merc's inventory
	if ( ( pInv->bSlotIdInOtherLocation != -1 ) && ( pInv->ubIdOfMercWhoOwnsTheItem != NO_PROFILE ) )
	{
		// get soldier
		sSoldierID = GetSoldierIDFromMercID( pInv->ubIdOfMercWhoOwnsTheItem );
		Assert( sSoldierID != -1 );
		Assert( CanMercInteractWithSelectedShopkeeper( MercPtrs[ sSoldierID ] ) );

		// then it better be a valid slot #
		Assert( pInv->bSlotIdInOtherLocation < (INT8)Menptr[ sSoldierID ].inv.size() );
		// and it better have a valid merc who owned it
		Assert( pInv->ubIdOfMercWhoOwnsTheItem != NO_PROFILE );
		
		//Copy the object back into that merc's original inventory slot
		Menptr[ sSoldierID ].inv[ pInv->bSlotIdInOtherLocation ] = pInv->ItemObject;
	}
}

void IfMercOwnedRemoveItemFromMercInv( INVENTORY_IN_SLOT *pInv )
{
	IfMercOwnedRemoveItemFromMercInv2( pInv->ubIdOfMercWhoOwnsTheItem, pInv->bSlotIdInOtherLocation );
}

void IfMercOwnedRemoveItemFromMercInv2( UINT8 ubOwnerProfileId, INT16 bOwnerSlotId )
{
	//if this item was in a previous location, and that location is on a merc's inventory
	if ( ( bOwnerSlotId != -1 ) && ( ubOwnerProfileId != NO_PROFILE ) )
	{
		// and it better have a valid merc who owned it
		Assert( ubOwnerProfileId != NO_PROFILE );
		INT16 sSoldierID = GetSoldierIDFromMercID( ubOwnerProfileId );
		Assert( sSoldierID != -1 );
		// then it better be a valid slot #
		Assert( bOwnerSlotId < (INT8)Menptr[ sSoldierID ].inv.size() );

		Assert( CanMercInteractWithSelectedShopkeeper( MercPtrs[ sSoldierID ] ) );

		//remove the object from that merc's original inventory slot
		DeleteObj(&(Menptr[ sSoldierID ].inv[bOwnerSlotId]));
	}
}

BOOLEAN SKITryToReturnInvToOwnerOrCurrentMerc( INVENTORY_IN_SLOT *pInv )
{
	// don't use this if the item has a copy in merc's inventory!!  It would create a duplicate!!
	Assert( pInv->bSlotIdInOtherLocation == -1 );

	// if it does have an owner
	if( pInv->ubIdOfMercWhoOwnsTheItem != NO_PROFILE )
	{
		INT16 sSoldierID;

		sSoldierID = GetSoldierIDFromMercID( pInv->ubIdOfMercWhoOwnsTheItem );
		// if that soldier is not in player's hire any longer
		if ( sSoldierID == -1 )
		{
			return(FALSE);
		}

		// For owners of repaired items, this checks that owner is still hired, in sector,
		// on current squad, close enough to the shopkeeper, etc.
		if ( !CanMercInteractWithSelectedShopkeeper( MercPtrs[ sSoldierID ] ) )
		{
			return(FALSE);
		}

		// Try to find a place to put in its owner's inventory (regardless of which merc is currently displayed!)
		if ( SKITryToAddInvToMercsInventory( pInv, MercPtrs[ sSoldierID ] ) )
		{
			return( TRUE );
		}

		// owner's inventory is full, so we'll try to give it to the current merc instead
	}

	//if the current merc is not disabled
	if( !gfSMDisableForItems )
	{
		// Try to find a place to put in current merc's inventory
		if ( SKITryToAddInvToMercsInventory( pInv, gpSMCurrentMerc ) )
		{
			return( TRUE );
		}
	}

	//failed to add item, either inventory was filled up, or nobody was even eligible to receive it
	return( FALSE );
}

BOOLEAN SKITryToAddInvToMercsInventory( INVENTORY_IN_SLOT *pInv, SOLDIERTYPE *pSoldier )
{
	INT8	bMoneyInvPos;
	BOOLEAN fNewItem = FALSE;
	
	//if the item is money
	if( Item[ pInv->sItemIndex ].usItemClass == IC_MONEY )
	{
		// search through the merc's inventory for a pocket of money which isn't full already
		bMoneyInvPos = GetInvSlotOfUnfullMoneyInMercInventory( pSoldier );

		// if he has a money pocket going
		if ( bMoneyInvPos != -1 )
		{
			// try to add to it.  If successful, it will delete the object...  It returns TRUE even if not all would fit!
			PlaceObject( pSoldier, bMoneyInvPos, &( pInv->ItemObject ) );

			// if the money is all gone
			if ( pInv->ItemObject[0]->data.money.uiMoneyAmount == 0 )
			{
				// we've been succesful!
				return(TRUE);
			}
			// otherwise we'll try to place the rest seperately
		}
	}

  // If it's just been purchased or repaired, mark it as a "new item"
	fNewItem = ( BOOLEAN ) ( pInv->uiFlags & ( ARMS_INV_JUST_PURCHASED | ARMS_INV_ITEM_REPAIRED ) );

	//try autoplacing the item in this soldier's inventory.
	if( !ShopkeeperAutoPlaceObject( pSoldier, &pInv->ItemObject, fNewItem ) )
	{
		//failed to add item, inventory probably filled up
		return( FALSE );
	}

	return(TRUE);
}

BOOLEAN CanMercInteractWithSelectedShopkeeper( SOLDIERTYPE *pSoldier )
{
	SOLDIERTYPE *pShopkeeper;
	INT32 sDestGridNo;
	INT8			bDestLevel;
	UINT32		uiRange;
	
	Assert( pSoldier!= NULL );
	Assert( gbSelectedArmsDealerID != -1 );

	if ( gusIDOfCivTrader != NOBODY )
		pShopkeeper = MercPtrs[gusIDOfCivTrader];
	else
		pShopkeeper = FindSoldierByProfileID( armsDealerInfo[gbSelectedArmsDealerID].ubShopKeeperID, FALSE );

	Assert( pShopkeeper != NULL );
	Assert( pShopkeeper->bActive );
	Assert( pShopkeeper->bInSector );

	if ( pShopkeeper->stats.bLife < OKLIFE )
	{
		return( FALSE );
	}

	if ( pSoldier->bActive && pSoldier->bInSector && IsMercOnCurrentSquad( pSoldier ) && ( pSoldier->stats.bLife >= OKLIFE ) &&
		!( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) )
	{
		sDestGridNo = pShopkeeper->sGridNo;
		bDestLevel	= pShopkeeper->pathing.bLevel;
		// If he has LOS...
		if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sDestGridNo, bDestLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
		{
			// Get range to shopkeeper
			uiRange = GetRangeFromGridNoDiff( pSoldier->sGridNo, sDestGridNo );

			// and is close enough to talk to the shopkeeper (use this define INSTEAD of PASSING_ITEM_DISTANCE_OKLIFE!)
			if ( uiRange <= NPC_TALK_RADIUS )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

#ifdef JA2TESTVERSION

void AddShopkeeperToGridNo( UINT8 ubProfile, INT32 sGridNo )
{
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	INT16										sSectorX, sSectorY;
	UINT8									ubID;

	GetCurrentWorldSector( &sSectorX, &sSectorY );

	MercCreateStruct.initialize();
	MercCreateStruct.bTeam				= CIV_TEAM;
	MercCreateStruct.ubProfile		= ubProfile;
	MercCreateStruct.sSectorX			= sSectorX;
	MercCreateStruct.sSectorY			= sSectorY;
	MercCreateStruct.bSectorZ			= gbWorldSectorZ;
	MercCreateStruct.sInsertionGridNo		= sGridNo;

	if ( TacticalCreateSoldier( &MercCreateStruct, &ubID ) )
	{
		AddSoldierToSector( ubID );

		// So we can see them!
		AllTeamsLookForAll(NO_INTERRUPTS);
	}
}

#endif


void ExitSKIRequested()
{
	BOOLEAN	fPlayerOwnedStuffOnTable = FALSE;
	
	ShutUpShopKeeper();

	if( !gfRemindedPlayerToPickUpHisStuff )
	{
		if( AreThereItemsInThePlayersOfferArea( ) )
		{
			fPlayerOwnedStuffOnTable = TRUE;
		}

		if( ( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS ) &&
				AreThereItemsInTheArmsDealersOfferArea( ) )
		{
			fPlayerOwnedStuffOnTable = TRUE;
		}

		//if there are any items belonging to player still on the table
		if( fPlayerOwnedStuffOnTable )
		{
			// remind player to pick them up
			ShutUpShopKeeper( );
			StartShopKeeperTalking( SK_QUOTES_PRESSED_DONE_STILL_HAS_STUFF_IN_OFFER_AREA );
			gfRemindedPlayerToPickUpHisStuff = TRUE;
			return;
		}
		else
		{
			//if the player has already requested to leave
			if( gfUserHasRequestedToLeave )
			{
				// has already asked to leave, shut the dealer up and allow the player to leave
				ShutUpShopKeeper();
				return;
			}
		}
	}
	
	// if the player hasn't already requested to leave
	if( !gfUserHasRequestedToLeave )
	{
		UINT16 usQuoteNum;

		if( gfDoneBusinessThisSession )
			usQuoteNum = SK_QUOTES_PRESSES_DONE_HAS_AT_LEAST_1_TRANSACTION;
		else
			usQuoteNum = SK_QUOTES_PRESSED_DONE_HASNT_MADE_TRANSACTION;

		StartShopKeeperTalking( usQuoteNum );

		//Set the fact that the player wants to leave
		gfUserHasRequestedToLeave = TRUE;
	}
}

void ResetAllQuoteSaidFlags()
{
	// reset flags for quotes said
	for ( UINT8 uiCnt = 0; uiCnt < NUM_EVAL_RESULTS; ++uiCnt )
	{
		gfEvalResultQuoteSaid[ uiCnt ] = FALSE;
	}

	guiLastTimeDealerSaidNormalEvaluationQuote = 0;
}

void DealWithItemsStillOnTheTable()
{
	SOLDIERTYPE *pDropSoldier;
	
	// in case we have have to drop stuff off at someone's feet, figure out where it's all gonna go

	// use the current merc, unless he's ineligible, then use the selected merc instead.
	if( !gfSMDisableForItems )
	{
		pDropSoldier = gpSMCurrentMerc;
	}
	else
	{
		pDropSoldier = MercPtrs[ gusSelectedSoldier ];
	}

	// this guy HAS to be valid!
	Assert( CanMercInteractWithSelectedShopkeeper( pDropSoldier ) );
	
	//loop through the players offer area and add return any items there to the player
	for ( int ubCnt = 0; ubCnt < (SKI_PLAYER_OFFER_BOXES_PER_ROW * SKI_PLAYER_OFFER_BOXES_PER_COL); ++ubCnt )
	{
		//if there is an item here, give it back somehow
		if( PlayersOfferArea[ ubCnt ].fActive )
		{
			ReturnItemToPlayerSomehow( &( PlayersOfferArea[ ubCnt ] ), pDropSoldier );
			ClearPlayersOfferSlot( ubCnt );
		}
	}
	
	//if the dealer repairs
	if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
	{
		//loop through the arms dealers' offer area and return any items there to the player
		for ( int ubCnt = 0; ubCnt < (SKI_TRADER_OFFER_BOXES_PER_ROW * SKI_TRADER_OFFER_BOXES_PER_COL); ++ubCnt)
		{
			//if there is an item here, give it back somehow
			if( ArmsDealerOfferArea[ ubCnt ].fActive )
			{
				ReturnItemToPlayerSomehow( &( ArmsDealerOfferArea[ ubCnt ] ), pDropSoldier );
				ClearArmsDealerOfferSlot( ubCnt );
			}
		}
	}
}

void ReturnItemToPlayerSomehow( INVENTORY_IN_SLOT *pInvSlot, SOLDIERTYPE *pDropSoldier )
{
	//if the item doesn't have a duplicate copy in its owner merc's inventory slot
	if( pInvSlot->bSlotIdInOtherLocation == -1 )
	{
		// first try to give it to its owner, or if he's unavailable, the current merc
		if ( !SKITryToReturnInvToOwnerOrCurrentMerc( pInvSlot ) )
		{
			// failed to add item, inventory probably filled up or item is unowned and current merc ineligible.
			// drop it at the specified guy's feet instead
			ShopkeeperAddItemToPool( pDropSoldier->sGridNo, &pInvSlot->ItemObject, VISIBLE, pDropSoldier->pathing.bLevel, 0, 0 );
		}
	}

	//CHRISL: We need to handle LBENODE items a little differently so we don't lose anything stored in them.  Instead of
	// trying to return the item, simply drop the item to the ground.
	if(UsingNewInventorySystem() == true && pInvSlot->ItemObject.IsActiveLBE(0) == true)
	{
		pDropSoldier->inv[pInvSlot->bSlotIdInOtherLocation].initialize();
		ShopkeeperAddItemToPool( pDropSoldier->sGridNo, &pInvSlot->ItemObject, VISIBLE, pDropSoldier->pathing.bLevel, 0, 0 );
	}
}

void GivePlayerSomeChange( UINT32 uiAmount )
{
	INVENTORY_IN_SLOT	MoneyInvSlot;

	CreateMoney( uiAmount, &MoneyInvSlot.ItemObject );
	MoneyInvSlot.sItemIndex = MoneyInvSlot.ItemObject.usItem;
	MoneyInvSlot.ubLocationOfObject = PLAYERS_INVENTORY;
	MoneyInvSlot.ubIdOfMercWhoOwnsTheItem = NO_PROFILE;

	AddItemToPlayersOfferArea( NO_PROFILE, &MoneyInvSlot, -1 );

	if ( ( gbSelectedArmsDealerID == ARMS_DEALER_JAKE ) &&
		 ( gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash < uiAmount ) )
	{
		// HACK HACK HACK: We forgot to write/record quote 27 for Jake, so he ALWAYS must have enough money!
		gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash += ( uiAmount - gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash );
	}

	// he must have enough cash left to give change
	Assert( gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash >= uiAmount );
	// subtract change from dealer's cash
	gArmsDealerStatus[ gbSelectedArmsDealerID ].uiArmsDealersCash -= uiAmount;
}

void DealerGetsBribed( UINT8 ubProfileId, UINT32 uiMoneyAmount )
{
	BOOLEAN fBribable = FALSE;
	UINT32 uiMinBribe = 0;
	UINT16 usFact = 0;

	// this is only for dealers
	if (!IsMercADealer( ubProfileId ) )
	{
		return;
	}

	switch ( ubProfileId )
	{
		case FRANK:
			fBribable = TRUE;
			uiMinBribe = 10;		// This should match the handling of a bribe submitted outside the SKI, using the GIVE system
			usFact = FACT_FRANK_HAS_BEEN_BRIBED;
			break;
	}

	// if this guy can be bribed, and we gave enough to bribe him
	if ( fBribable && ( uiMoneyAmount >= uiMinBribe ) )
	{
		// set the appropriate fact
		SetFactTrue( usFact );
	}
}

void HandlePossibleRepairDelays()
{
	UINT32 uiHoursSinceAnyItemsShouldHaveBeenRepaired = 0;
	
	// assume there won't be a delay
	gfStartWithRepairsDelayedQuote = FALSE;

	// if it's Fredo or Perko
#ifdef JA2UB
	if ( ( gbSelectedArmsDealerID == ARMS_DEALER_FREDO ) || ( gbSelectedArmsDealerID == ARMS_DEALER_RAUL ) )
#else
	if ( ( gbSelectedArmsDealerID == ARMS_DEALER_FREDO ) || ( gbSelectedArmsDealerID == ARMS_DEALER_PERKO ) )
#endif

	{
		// because the quotes are so specific, we'll only use them once per game per repairman
		if( !gArmsDealerStatus[ gbSelectedArmsDealerID ].fRepairDelayBeenUsed )
		{
			// and it's been a while since the player last dealt with this repairman (within SKI that is)
			// this serves 2 purposes:
			// a) reduces delays being much more likely if player checks time remaining very frequently, AND
			// b) gives time for the events described in the text of the dealers' excuses to happen (e.g. scouting trip)
			if ( ( GetWorldTotalMin() - gArmsDealerStatus[ gbSelectedArmsDealerID ].uiTimePlayerLastInSKI ) >= ( 3 * 60 ) )
			{
				// if he should have been finished, but it's only been a few hours since then (not days!)
				if ( RepairmanFixingAnyItemsThatShouldBeDoneNow( &uiHoursSinceAnyItemsShouldHaveBeenRepaired ) &&
						 ( uiHoursSinceAnyItemsShouldHaveBeenRepaired < REPAIR_DELAY_IN_HOURS ) )
				{
					// then there's a fair chance he'll be delayed.  Use pre-chance to hopefully preserve across reloads
					if ( PreChance( 50 ) )
					{
						DelayRepairsInProgressBy( ( REPAIR_DELAY_IN_HOURS + Random( REPAIR_DELAY_IN_HOURS ) ) * 60 );

						// this triggers a different opening quote
						gfStartWithRepairsDelayedQuote = TRUE;

						// set flag so it doesn't happen again
						gArmsDealerStatus[ gbSelectedArmsDealerID ].fRepairDelayBeenUsed = TRUE;
					}
				}
			}
		}
	}
}

BOOLEAN RepairmanFixingAnyItemsThatShouldBeDoneNow( UINT32 *puiHoursSinceOldestItemRepaired )
{
	//if the dealer is not a repair dealer, return
	if( !DoesDealerDoRepairs( gbSelectedArmsDealerID ) )
		return( FALSE );

	BOOLEAN fFoundOne = FALSE;
	UINT32 uiMinutesSinceItWasDone;
	UINT32 uiMinutesShopClosedSinceItWasDone;
	UINT32 uiWorkingHoursSinceThisItemRepaired;

	*puiHoursSinceOldestItemRepaired = 0;

	UINT32 currentTime = GetWorldTotalMin();
	//loop through the dealers inventory and check if there are only unrepaired items
	for (DealerItemList::iterator iter = gArmsDealersInventory[gbSelectedArmsDealerID].begin();
		iter != gArmsDealersInventory[gbSelectedArmsDealerID].end(); ++iter) {
		if ( iter->ItemIsInInventory() == true )
		{
			if( iter->IsUnderRepair() == true )
			{
				//if the repairs are done
				if( iter->uiRepairDoneTime <= currentTime )
				{
					// at least one item is supposed to be done by now
					fFoundOne = TRUE;

					uiMinutesSinceItWasDone = currentTime - iter->uiRepairDoneTime;
					uiMinutesShopClosedSinceItWasDone = CalculateMinutesClosedBetween( gbSelectedArmsDealerID, iter->uiRepairDoneTime, currentTime );

					// calculate how many WORKING hours since this item's been repaired
					uiWorkingHoursSinceThisItemRepaired = ( uiMinutesSinceItWasDone - uiMinutesShopClosedSinceItWasDone ) / 60;

					// we need to determine how long it's been since the item that's been repaired for the longest time was done
					if ( uiWorkingHoursSinceThisItemRepaired > *puiHoursSinceOldestItemRepaired )
					{
						*puiHoursSinceOldestItemRepaired = uiWorkingHoursSinceThisItemRepaired;
					}
				}
			}
		}
	}

	// if FALSE returned here, he's either not repairing anything, or none of it's done yet
	return( fFoundOne );
}

void DelayRepairsInProgressBy( UINT32 uiMinutesDelayed )
{
	UINT32 uiMinutesShopClosedBeforeItsDone;
	
	//if the dealer is not a repair dealer, return
	if( !DoesDealerDoRepairs( gbSelectedArmsDealerID ) )
		return;

	//loop through the dealers inventory and check if there are only unrepaired items
	for (DealerItemList::iterator iter = gArmsDealersInventory[gbSelectedArmsDealerID].begin();
		iter != gArmsDealersInventory[gbSelectedArmsDealerID].end(); ++iter) {
		if ( iter->ItemIsInInventory() == true )
		{
			if( iter->IsUnderRepair() == true )
			{
				uiMinutesShopClosedBeforeItsDone = CalculateOvernightRepairDelay( gbSelectedArmsDealerID, iter->uiRepairDoneTime, uiMinutesDelayed );
				// add this many minutes to the repair time estimate
				iter->uiRepairDoneTime += ( uiMinutesShopClosedBeforeItsDone + uiMinutesDelayed );
			}
		}
	}
}

//Mouse Call back for the Arms delaers face
void SelectArmsDealersDropItemToGroundRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	// WANNE 2: not needed to drop the item to the ground->on the right side of the shopkeeper screen!
	/*
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		SOLDIERTYPE *pDropSoldier;

		// use the current merc, unless he's ineligible, then use the selected merc instead.
		if( !gfSMDisableForItems )
		{
			pDropSoldier = gpSMCurrentMerc;
		}
		else
		{
			pDropSoldier = MercPtrs[ gusSelectedSoldier ];
		}

		//if we don't have an item, pick one up
		if( gMoveingItem.sItemIndex != 0 )
		{

			//add the item to the ground
			ShopkeeperAddItemToPool( pDropSoldier->sGridNo, &gMoveingItem.ItemObject, VISIBLE, pDropSoldier->pathing.bLevel, 0, 0 );

			//Reset the cursor
			SetSkiCursor( CURSOR_NORMAL );

			//refresh the screen
			gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
		}
	}
	*/
}

void SelectArmsDealersDropItemToGroundMovementRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		gfSkiDisplayDropItemToGroundText = FALSE;
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
	}
	else if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( CanTheDropItemToGroundStringBeDisplayed() )
		{
			gfSkiDisplayDropItemToGroundText = TRUE;
		}
		else
		{
			gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			gfSkiDisplayDropItemToGroundText = FALSE;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_MOVE )
	{
	}
}

BOOLEAN	CanTheDropItemToGroundStringBeDisplayed()
{
	//if we don't have an item, pick one up
	if( gMoveingItem.sItemIndex != 0 )
		return( TRUE );
	else
		return( FALSE );
}

void DisplayTheSkiDropItemToGroundString()
{
	DisplayWrappedString( SKI_DROP_ITEM_TO_GROUND_START_X, SKI_DROP_ITEM_TO_GROUND_TEXT_START_Y, (UINT16)(SCREEN_WIDTH-SKI_DROP_ITEM_TO_GROUND_START_X), 2, SKI_LABEL_FONT, SKI_TITLE_COLOR, SKI_Text[ SKI_TEXT_DROP_ITEM_TO_GROUND ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED | DONT_DISPLAY_TEXT );
}

UINT32 EvaluateInvSlot( INVENTORY_IN_SLOT *pInvSlot )
{
	UINT32	uiEvalResult = EVAL_RESULT_NORMAL;
	FLOAT		dPriceModifier;
	UINT32	uiBuyingPrice;
	
	//if the dealer is Micky
	if( gbSelectedArmsDealerID == ARMS_DEALER_MICKY )
	{
		INT16	sSoldierID;
		sSoldierID = GetSoldierIDFromMercID( armsDealerInfo[ gbSelectedArmsDealerID ].ubShopKeeperID );
		if( ( sSoldierID != -1 ) && ( GetDrunkLevel( &Menptr[ sSoldierID ] ) == DRUNK ) )
		{
			//Micky is DRUNK, pays more!
			dPriceModifier = armsDealerInfo[ gbSelectedArmsDealerID ].dSellModifier;
		}
		else
		{
			// Micky isn't drunk, charge regular price
			dPriceModifier = armsDealerInfo[ gbSelectedArmsDealerID ].dBuyModifier;
		}
	}
	else
	{
		dPriceModifier = armsDealerInfo[ gbSelectedArmsDealerID ].dBuyModifier;
	}

	// Calculate dealer's buying price for the item
	uiBuyingPrice = CalcShopKeeperItemPrice( DEALER_BUYING, FALSE, pInvSlot->sItemIndex, dPriceModifier, &( pInvSlot->ItemObject ) );

	pInvSlot->uiItemPrice = uiBuyingPrice;

	if ( uiBuyingPrice > 0 )
	{
		// check if the item is really badly damaged
		if( Item[ pInvSlot->sItemIndex ].usItemClass != IC_AMMO )
		{
			if( pInvSlot->ItemObject[0]->data.objectStatus < REALLY_BADLY_DAMAGED_THRESHOLD )
			{
				uiEvalResult = EVAL_RESULT_OK_BUT_REALLY_DAMAGED;
			}
		}

		//the item can be sold here, so mark it as such
		pInvSlot->uiFlags |= ARMS_INV_PLAYERS_ITEM_HAS_VALUE;
	}
	else
	{
		// he normally buys this, but it's in such bad shape that he won't, it's worthless (different quote)
		uiEvalResult = EVAL_RESULT_WORTHLESS;

		pInvSlot->uiFlags &= ~ARMS_INV_PLAYERS_ITEM_HAS_VALUE;
	}

	return( uiEvalResult );
}

void BuildRepairTimeString( CHAR16 sString[], UINT32 uiTimeInMinutesToFixItem )
{
	UINT16	usNumberOfHoursToFixItem = 0;
	
	// if it's 0, it shouldn't be up here any more!
	Assert ( uiTimeInMinutesToFixItem > 0 );
	
	// round off to next higher 15 minutes
	if ( ( uiTimeInMinutesToFixItem % REPAIR_MINUTES_INTERVAL ) > 0 )
	{
		uiTimeInMinutesToFixItem += REPAIR_MINUTES_INTERVAL - ( uiTimeInMinutesToFixItem % REPAIR_MINUTES_INTERVAL );
	}

	if ( uiTimeInMinutesToFixItem < REPAIR_MINUTES_INTERVAL )
	{
		uiTimeInMinutesToFixItem = REPAIR_MINUTES_INTERVAL;
	}

	// show up to 1.5 hrs in minutes
	if ( uiTimeInMinutesToFixItem <= 90 )
	{
		// show minutes
		swprintf( sString, SKI_Text[ SKI_TEXT_MINUTES ], uiTimeInMinutesToFixItem );
	}
	else
	{
		// show hours

		// round fractions of 15+ minutes up to next full hour
		usNumberOfHoursToFixItem = (UINT16) ( ( uiTimeInMinutesToFixItem + 45 ) / 60 );

		if ( usNumberOfHoursToFixItem > 1 )
		{
			swprintf( sString, SKI_Text[ SKI_TEXT_PLURAL_HOURS ], usNumberOfHoursToFixItem );
		}
		else
		{
			wcscpy( sString, SKI_Text[ SKI_TEXT_ONE_HOUR ] );
		}
	}
}

void BuildDoneWhenTimeString( CHAR16 sString[], UINT8 ubArmsDealer, INVENTORY_IN_SLOT* pObject )
{
	UINT32 uiDoneTime;
	UINT32 uiDay, uiHour, uiMin;
	
	//dealer must be a repair dealer
	Assert( DoesDealerDoRepairs( ubArmsDealer ) );

	DEALER_SPECIAL_ITEM* pSpecial = 0;
	for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
		iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {
		if (iter->ItemIsInInventory() == true &&  iter->IsUnderRepair() == true) {
			if (pObject->ItemObject == iter->object && pObject->uiRepairDoneTime == iter->uiRepairDoneTime) {
				pSpecial = &(*iter);
				break;
			}
		}
	}

	Assert(pSpecial);
	// that item must be in repair
	Assert( pSpecial->IsUnderRepair() == true );

	//if the item has already been repaired
	uiDoneTime = pSpecial->uiRepairDoneTime;
	if( uiDoneTime <= GetWorldTotalMin() )
	{
		wcscpy( sString, L"" );
		return;
	}
	
	// round off to next higher 15 minutes
	if ( ( uiDoneTime % REPAIR_MINUTES_INTERVAL ) > 0 )
	{
		uiDoneTime += REPAIR_MINUTES_INTERVAL - ( uiDoneTime % REPAIR_MINUTES_INTERVAL );
	}

	// uiDoneTime is in minutes...
	uiDay  = ( uiDoneTime / NUM_MIN_IN_DAY );
	uiHour = ( uiDoneTime - ( uiDay * NUM_MIN_IN_DAY ) ) / NUM_MIN_IN_HOUR;
	uiMin	 = uiDoneTime - ( ( uiDay * NUM_MIN_IN_DAY ) + ( uiHour * NUM_MIN_IN_HOUR ) );

	// only show day if it's gonna take overnight
	if ( GetWorldDay() != uiDay )
	{
		swprintf( sString, L"%s %d %02d:%02d", pDayStrings[ 0 ], uiDay, uiHour, uiMin );
	}
	else
	{
		swprintf( sString, L"%02d:%02d", uiHour, uiMin );
	}
}

void BuildItemHelpTextString( CHAR16 sString[], INVENTORY_IN_SLOT *pInv, UINT8 ubScreenArea )
{
	CHAR16 zHelpText[ 512 ];
	CHAR16 zRepairTime[ 64 ];

	if( pInv != NULL )
	{
		GetHelpTextForItem( zHelpText, &( pInv->ItemObject ), NULL );

		// add repair time for items in a repairman's offer area
		if ( ( ubScreenArea == ARMS_DEALER_OFFER_AREA ) &&
				 ( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS ) )
		{
			BuildRepairTimeString( zRepairTime, CalculateObjectItemRepairTime( gbSelectedArmsDealerID, &( pInv->ItemObject ) ) );
			swprintf( sString, L"%s\n(%s: %s)", zHelpText, gzLateLocalizedString[ 44 ], zRepairTime );
		}
		else
		{
			wcscpy( sString, zHelpText );
		}
	}
	else
	{
		sString[ 0 ] = '\0';
	}
}

void DisableAllDealersInventorySlots( void )
{
	for ( int iCnt = 0; iCnt < gDealersInventoryActiveRegions; ++iCnt )
	{
		MSYS_DisableRegion( &gDealersInventoryMouseRegions[ iCnt ] );

		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			MSYS_DisableRegion( &gRepairmanInventorySmallFaceMouseRegions[ iCnt ] );
		}
	}
}

void EnableAllDealersInventorySlots( void )
{
	for ( int iCnt = 0; iCnt < gDealersInventoryActiveRegions; ++iCnt )
	{
		MSYS_EnableRegion( &gDealersInventoryMouseRegions[ iCnt ] );

		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			MSYS_EnableRegion( &gRepairmanInventorySmallFaceMouseRegions[ iCnt ] );
		}
	}
}

void DisableAllDealersOfferSlots( void )
{
	for ( int iCnt = 0; iCnt < gDealersOfferActiveRegions; ++iCnt )
	{
		MSYS_DisableRegion( &gDealersOfferSlotsMouseRegions[ iCnt ] );

		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			MSYS_DisableRegion( &gDealersOfferSlotsSmallFaceMouseRegions[ iCnt ] );
		}
	}
}

void EnableAllDealersOfferSlots( void )
{
	for ( int iCnt = 0; iCnt < gDealersOfferActiveRegions; ++iCnt )
	{
		MSYS_EnableRegion( &gDealersOfferSlotsMouseRegions[ iCnt ] );

		//if the dealer repairs
		if( armsDealerInfo[ gbSelectedArmsDealerID ].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
		{
			MSYS_EnableRegion( &gDealersOfferSlotsSmallFaceMouseRegions[ iCnt ] );
		}
	}
}

void HatchOutInvSlot( UINT16 usPosX, UINT16 usPosY )
{
	UINT16 usSlotX, usSlotY;
	UINT16 usSlotWidth, usSlotHeight;

	//if we are in the item desc panel
	if( InItemDescriptionBox( ) && pShopKeeperItemDescObject != NULL )
	{
		// do nothing, don't wanna shade things twice!
		return;
	}

	usSlotX = ( UINT16 ) ( usPosX - 1 );
	usSlotY = ( UINT16 ) ( usPosY - 1 );
	usSlotWidth = SKI_INV_SLOT_WIDTH;
	usSlotHeight = SKI_INV_SLOT_HEIGHT;

	//Hatch it out
	DrawHatchOnInventory( guiRENDERBUFFER, usSlotX, usSlotY, usSlotWidth, usSlotHeight );
	InvalidateRegion( usSlotX, usSlotY, usSlotX + usSlotWidth, usSlotY + usSlotHeight );
}
#ifdef JA2UB
//ja25 UB
void AskUserToAttachTransmitterToLaptop()
{
	DoSkiMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__ATTACH_TRANSMITTER_TO_LAPTOP ], SHOPKEEPER_SCREEN, MSG_BOX_FLAG_OK, AttachLaptopTransmitterToLaptop );
}

void AttachLaptopTransmitterToLaptop( UINT8 ubExitValue )
{
	//
	// Get the laptop working again
	//

	//Mark the quest done
	if ( gGameUBOptions.LaptopQuestEnabled == TRUE )
	EndQuest( QUEST_FIX_LAPTOP, -1, -1 );
}
#endif
