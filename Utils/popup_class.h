#ifndef POPUP_CLASS
	#define POPUP_CLASS

	#include "popup_callback.h"
	#include "types.h"
	#include "sgp.h"

	#define MAX_POPUPS 32
	#define POPUP_MAX_SUB_POPUPS  24
	#define POPUP_MAX_OPTIONS 96
	#define MAX_REGIONS_IN_INDEX MAX_POPUPS*POPUP_MAX_OPTIONS*POPUP_MAX_SUB_POPUPS

	#define REGION_SUB_POPUP 1
	#define REGION_OPTION 0

#ifdef MSYS_PRIORITY_VERY_HIGH
	#define POPUP_SCREEN_MASK_PRIORITY  (MSYS_PRIORITY_VERY_HIGH + 10) // MSYS priority for popup screen mask
	#define POPUP_OPTION_PRIORITY		(MSYS_PRIORITY_VERY_HIGH + 12) // MSYS priority for popup options
#else	// compatibility with 1.13 versions without the VERY_HIGH priority defined
	#define POPUP_SCREEN_MASK_PRIORITY  MSYS_PRIORITY_HIGHEST 
	#define POPUP_OPTION_PRIORITY		MSYS_PRIORITY_HIGHEST 
#endif

	#define POPUP_CALLBACK_INIT 1	// called at the end of init
	#define POPUP_CALLBACK_END 2	// called after cleanup
	#define POPUP_CALLBACK_SHOW 3	// called before showing
	#define POPUP_CALLBACK_HIDE 4	// called after hiding

	#define POPUP_POSITION_TOP_LEFT 1		// default positioning rule when setting position
	#define POPUP_POSITION_TOP_RIGHT 2		// user defined point (position) is the top right corner of the popup
	#define POPUP_POSITION_BOTTOM_RIGHT 3	// user defined point (position) is the bottom right corner of the popup
	#define POPUP_POSITION_BOTTOM_LEFT 4	// user defined point (position) is the bottom left corner of the popup

	#define POPUP_POSITION_RELATIVE -1		// only used for sub-popups, left corner is relative to parent popup's point of origin

	class POPUP_OPTION;
	class POPUP_SUB_POPUP_OPTION;
	class POPUP;

	typedef void (POPUP::*POPUP_CALLBACK)(MOUSE_REGION *, INT32);

	typedef struct {
		UINT16	regionId;
		UINT32	classId;
	} PopupIndex ;

	extern std::vector<typename PopupIndex>	gPopupRegionIndex;
	extern std::vector<POPUP*>	gPopupIndex;

	extern UINT32			gPopupRegionIndexCounter;
	extern UINT32			gPopupIndexCounter;

	PopupIndex * findMouseRegionInIndex(UINT16 regionId);
	BOOLEAN registerPopupRegion(UINT16 rID, UINT32 cID);
	BOOLEAN unregisterPopupRegion(UINT16 regionId);
	void rebuildPopupRegionIndex(void);
	void popupMaskCallback(MOUSE_REGION *pRegion, INT32 iReason);
	void popupMouseMoveCallback(MOUSE_REGION *pRegion, INT32 iReason);
	void popupMouseClickCallback(MOUSE_REGION *pRegion, INT32 iReason);

	POPUP * findPopupInIndex( UINT32 cID );
	void rebuildPopupIndex(void);
	void reinitPopupIndex(void);
	void initPopupIndex(void);

	class POPUP_OPTION
	{
	public:
		POPUP_OPTION::POPUP_OPTION(void); // default constructor
		POPUP_OPTION(std::wstring* name, popupCallback* newFunction); // constructor
		~POPUP_OPTION();			// destructor
		// setup
		BOOLEAN setName(std::wstring * name);
		BOOLEAN setAction(popupCallback*fun);
		BOOLEAN setAvail(popupCallback *fun);
		BOOLEAN setHover(popupCallback *fun);

		BOOLEAN run();
		BOOLEAN forceRun();

		BOOLEAN runHoverCallback(MOUSE_REGION * pRegion);
		BOOLEAN checkAvailability();

		BOOLEAN HoverFunctionSet(){ return (this->hover != 0); };
		BOOLEAN AvailabilityFunctionSet(){ return (this->avail != 0); };
		
		std::wstring name;
		std::wstring hint;
//	protected:
		popupCallback * action;
		popupCallback * avail;
		popupCallback * hover;

		UINT8 color_foreground;
		UINT8 color_background;
		UINT8 color_highlight;
		UINT8 color_shade;

		UINT32 stringHandle;
	};


	class POPUP_SUB_POPUP_OPTION : public POPUP_OPTION 
	{
	public:
		// constructor/destructor
	POPUP_SUB_POPUP_OPTION(void);
	POPUP_SUB_POPUP_OPTION(std::wstring* name);
	POPUP_SUB_POPUP_OPTION(std::wstring* newName, const POPUP * parent);
	~POPUP_SUB_POPUP_OPTION();

	void showPopup();
	void hidePopup();

	BOOLEAN run();

	void initSubPopup();
	void destroySubPopup();
	void positionSubPopup();

	BOOLEAN setPopupPosition(UINT16 x,	UINT16 y, INT8 positioningRule = POPUP_POSITION_RELATIVE  );

	BOOLEAN customPositionSet;
	UINT32 customX;
	UINT32 customY;
	INT8 customRule;

	POPUP	*	subPopup;
	const POPUP	*	parent;
	};


	class POPUP
	{
		/////////////////////////
		// public functions
	public:
		POPUP(void); // constructor
		POPUP(CHAR* name); // constructor
		~POPUP(void);			// destructor
		// setup
		POPUP_OPTION * addOption(std::wstring * name, popupCallback * action);
		/*INT16 findFreeOptionIndex();*/
		BOOLEAN addOption(POPUP_OPTION &option);
		POPUP_OPTION * getOption(UINT16 n);

		BOOLEAN delOption(CHAR* name);		// Another index to through and clean, aargh
		BOOLEAN delOption(UINT8 optIndex);

		POPUP* addSubMenuOption(std::wstring * name);
		BOOL addSubMenuOption(POPUP_SUB_POPUP_OPTION* sub);
		/*INT16 findFreeSubMenuOptionIndex();*/
		POPUP_SUB_POPUP_OPTION * getSubPopupOption(UINT8 n);

		BOOLEAN setPosition(UINT16 x,	UINT16 y, UINT8 positioningRule = POPUP_POSITION_TOP_LEFT  );
		BOOLEAN setSize(UINT16 x,	UINT16 y );
		// usage
		BOOLEAN show(void);
		BOOLEAN hide(void);
		void hideAfter(void); // called by options, closes popup after running callback
		BOOLEAN toggle(void);
		BOOLEAN refresh(void);
		BOOLEAN forceDraw(void);
		BOOLEAN callOption(int optIndex);
		BOOLEAN Visible(void) { return this->PopupVisible; };

		INT32 getBoxId() const {
			return this->boxId;
		}

		// gets the final dimensions/position of the box, works only after it has been displayed
		SGPRect getBoxDimensions();
		SGPPoint getBoxPosition();

		BOOLEAN setCallback(UINT8 type, popupCallback * callback);
		BOOLEAN isCallbackSet(UINT8 type);
		void recalculateWidth(void);
		INT16 getCurrentWidth(void);	// calculated the width based on the longest option name

		INT16 getCurrentHeight(void);
		/////////////////////////
		// public variables
	public:
		CHAR name[32];
		SGPPoint Position;
		SGPRect Dimensions;
		UINT32 id;
		static UINT32 nextid ;
		UINT8 optionCount;
		UINT8 subPopupOptionCount;

		/////////////////////////
		// private functions
	private:

		void	setInitialValues(void);
		BOOLEAN addToIndex( void );
		BOOLEAN removeFromIndex( void );
		BOOLEAN hideAfterRun;

		// Box init functions
		BOOLEAN CreateDestroyPopUpBoxes(void);
		BOOLEAN CreatePopUpBoxes(void);
		BOOLEAN DestroyPopUpBoxes(void);

		BOOLEAN CreateBox( void );
		void AddOptionStrings();
		void AddSubPopupStrings();

		void RebuildBox( void );

		// Screen mask
		void CreateDestroyScreenMask( void );
		void CreateScreenMask( void );
		void DestroyScreenMask( void );

		// Mouse regions
		void CreateDestroyMouseRegions( void );
		void CreateMouseRegions( void );
		void DestroyMouseRegions( void );
		void AdjustMouseRegions( void );

		void RepositionMouseRegions( void );

	public:
		// MSYS Callbacks
		void MenuMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
		void MenuBtnCallBack( MOUSE_REGION * pRegion, INT32 iReason );
		void ScreenMaskBtnCallback(MOUSE_REGION * pRegion, INT32 iReason );
	private:
		// These are mostly used in the map screen
		void UpdateMapScreenPosition(void);
		void DetermineWhichLMenusCanBeShown( void );

		// Misc utility functions
		void HandleShadingOfLines( void );
		void PositionCursorForBox( void );
		void CheckAndUpdateTacticalPopUpPositions( void );
		void DetermineBoxPositions( void );
		void UpdateTextProperties( void );

		// external flag access functiona
		BOOLEAN MapScreenBottomDirty(void);
		BOOLEAN CharacterInfoPanelDirty(void);
		BOOLEAN TeamPanelDirty(void);
		BOOLEAN RenderPBInterface(void);

		void setMapScreenBottomDirty(BOOLEAN);
		void setCharacterInfoPanelDirty(BOOLEAN);
		void setTeamPanelDirty(BOOLEAN);
		void setRenderPBInterface(BOOLEAN);


		/////////////////////////
		// private variables
	private:
		BOOLEAN PopupVisible;

		popupCallback * initCallback;
		popupCallback * EndCallback;
		popupCallback * ShowCallback;
		popupCallback * HideCallback;

		///////////////////////
		// Popup box vars
		/////////////////

		BOOLEAN FirstClickInScreenMask;
		BOOLEAN BoxPersist;
		INT16 BoxesX, BoxesY;

			// Textures

		static UINT32 uiPOPUPBORDERS;
		static UINT32 uiPOPUPTEX;

			// PopUp Box Handles
		INT32 boxId;

			// which corner of the popup should be aligned with the supplied coordinates ?
		INT8 positioningRule;


			// the x,y position of the pop up in tactical
	public:
		INT16 X, Y;

			// assignment menu mouse regions

		MOUSE_REGION    MenuRegion[ POPUP_MAX_OPTIONS ]; 
		MOUSE_REGION    ScreenMaskRegion;
	private:
			// Box position and size
		SGPPoint	OrigPosition;

		// Options
		std::vector<POPUP_OPTION *> options;

		// Subpopups
		std::vector<POPUP_SUB_POPUP_OPTION *> subPopupOptions;
	
	};
#endif
