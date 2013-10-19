#ifndef __DROPDOWN_H
#define __DROPDOWN_H

/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

#include "LaptopSave.h"

/*
 * helpful draw functions that used to draw the boundaries of the dropdown. The old way of adding sti objects to paint boundaries is... lets say, inefficient
 */
void DisplaySmallLine( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 EndY, UINT16 usColor );
void Display2Line2ShadowVertical( UINT16 usStartX, UINT16 usStartY, UINT16 EndX, UINT16 usColor1, UINT16 usColor2 );
void Display2Line2ShadowHorizontal( UINT16 usStartX, UINT16 usStartY, UINT16 EndY, UINT16 usColor1, UINT16 usColor2 );

// the maximum number of entries that can be shown at once. Increase requires recompilation!
#define DROPDOWN_REGIONS		8

/*
 * As we cannot directly add our callbacks (we need a static callback due to the way MSYS_DefineRegion(...) works, which utterly fails if we have multiple instance of a class) we
 * use a very odd looking workaround.
 * For every DropDown you need, you have to inherit from this class and fill CallBackWrapper(...). You then need a static pointer to an instance of your class, and a wrapper for each callback
 * that will then call the correct callback. The derived class will also have to be a singleton, otherwise callbacks for multiple instances will always refer to the last created one.
 * This seems a lot of work and pretty absurd, but I've found no better workaround without changing MSYS_DefineRegion(), as it expects a MOUSE_CALLBACK and not voi* pointers to objects.
 * On the plus side, you can simply copy this code:

 Definition:
	class DropDown_Example : public DropDown
	{
		public:
		static DropDown_Example& getInstance()
		{
			static DropDown_Example    instance; // Guaranteed to be destroyed.
									// Instantiated on first use.
			return instance;
		}
	
		virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

		//somehow refresh display, otherwise DropDownA will write over itself and other stuff
		// this function has to be implemented!
		virtual void SetRefresh()
		{
			// do stuff here...
		}

		private:
		DropDown_Example() {};                   // private constructor, so we cannot create more instances
    
		// declare but don't define
		DropDown_Example(DropDown_Example const&); 
		void operator=(DropDown_Example const&);
	};

 Declaration:
	void * gPtrDropDown_Example;
	void OpenDropDown_Example(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Example)->OpenDropDownRegionCallBack(pRegion, iReason); }
	void CloseDropDown_Example(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Example)->CloseDropDownRegionCallBack(pRegion, iReason); }
	void SelectRegionDropDown_Example(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Example)->SelectDropDownRegionCallBack(pRegion, iReason); }
	void SelectMovementDropDown_Example(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Example)->SelectDropDownMovementCallBack(pRegion, iReason); }
	void SelectArrowDropDown_Example(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Example)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
	void SelectScrollRegionDropDown_Example(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_Example)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
	void SelectScrollMovementDropDown_Example(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_Example)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

	MOUSE_CALLBACK
	DropDown_Example::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
	{
	gPtrDropDown_Example = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_Example;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_Example;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_Example;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_Example;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_Example;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_Example;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_Example;
		break;
	}

	return *pt2Function;
	}

 Usage:
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Appearance;
	for(UINT8 i = 0; i < NUM_APPEARANCES; ++i)
	entryvecDropDown_Appearance.push_back( std::make_pair<INT16, STR16>(i, szAppearanceText[i]) );
		
	DropDown_Appearance::getInstance().SetEntries(entryvecDropDown_Appearance);
	DropDown_Appearance::getInstance().Create(usX, usY);
	...

 */
class DropDown
{
public:
	DropDown();

	/*
	 * Create a Dropdown with upper left coordinates
	 */
	void Create(UINT16 sX, UINT16 sY);

	/*
	 * Destroy Dropdown, for example once a webiste isn't displayed anymore
	 */
	void Destroy();

	/*
	 * Display DropDown. Use this when refreshing
	 */
	void Display();

	/*
	 * Sometimes a refresh inside the class isn't enough - best example is closing the dropdown area, after whcih te previous background cannot be restored by this class.
	 * In that case, the background and this class has to be redrawn.
	 * This function is called internally whenver such a refreh is necessary.
	 * It has to be defined in derived classes and should notify your dialogues to redraw at apporpiate times
	 *
	 * This function has to be implemented!
	 */
	virtual void SetRefresh() = 0;

	// internal callback types
	enum {
		DROPDOWN_OPEN = 0,
		DROPDOWN_CLOSE,
		DROPDOWN_REGION,
		DROPDOWN_MOVEMENT,
		DROPDOWN_ARROW,
		DROPDOWN_SCROLL_REGION,
		DROPDOWN_SCROLL_MOVEMENT,
	};
	
	// call to open/close the drop down
	void OpenDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
	void CloseDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

	void SelectDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
	void SelectDropDownMovementCallBack(MOUSE_REGION * pRegion, INT32 iReason );
	void SelectUpDownArrowOnScrollAreaRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

	void SelectScrollAreaDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
	void SelectScrollAreaDropDownMovementCallBack(MOUSE_REGION * pRegion, INT32 iReason );

	/*
	 * This function has to be implemented, see documetnation as to why and how!
	 */
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) ) = 0;
	
	/*
	 * Set the content of a dropdown. Each entry consists of an INT16 key, by which you can later identify which entry was selected, and a STR16 that will be displayed.
	 * There can be multiple instances of the same key or name.
	 */
	void SetEntries( std::vector<std::pair<INT16, STR16> >& arEntryVec )	{ mEntryVector = arEntryVec; }

	/*
	 * Set Color of boundary line
	 */
	void SetColorLine( UINT16 aCol )		{ mColorLine = aCol; }

	/*
	 * Set Color of boundary line shadow
	 */
	void SetColorLineShadow( UINT16 aCol )	{ mColorLineShadow = aCol; }

	/*
	 * Set background color of selected entry
	 */
	void SetColorMarked( UINT16 aCol )		{ mColorMarked = aCol; }

	/*
	 * Set Color of highlight of the scrollbar mover
	 */
	void SetColorHighLight( UINT16 aCol )	{ mColorHighLight = aCol; }

	/*
	 * Get key of selected entry
	 */
	INT16	GetSelectedEntryKey()		{ return mEntryVector[mSelectedEntry].first; }

	/*
	 * Get width of entire DropDown
	 */
	UINT16	GetTotalWidth()				{ return musWidth + musArrowWidth; }

	/*
	 * Get right x coordinate
	 */
	UINT16	GetLastX()					{ return musStartX + GetTotalWidth(); }

private:
	// declare but don't define
    DropDown(DropDown const&); 
    void operator=(DropDown const&);

	/*
	 * Initialise variables. Called after each creationm which allows moving a dropdown 
	 */
	void Init(UINT16 sX, UINT16 sY);

	/*
	 * Functions exclusively for the dropdown region, which is only shown after user interaction
	 */
	void Create_Drop();
	void Init_Drop();
	void Display_Drop();
	void Destroy_Drop();

	/*
	 * This dummy is needed internally and does nothing when called
	 */
	static void Dummyfunc(MOUSE_REGION * pRegion, INT32 iReason )	{}

	void DrawTopEntry();
	void DrawSelectedCity();
	void DrawGoldRectangle();

private:
	UINT16	musHeight;
	UINT16	musWidth;
	
	UINT16	musStartX;
	UINT16	musStartY;
	UINT16	musStartX_Drop;
	UINT16	musStartY_Drop;
	UINT16	musScrollAreaX;

	UINT16	musUpArrowX;
	UINT16	musUpArrowY;
	UINT16	musDownArrowX;
	UINT16	musDownArrowY;
	UINT16	musArrowWidth;
	UINT16	musAreaHeight;

	UINT16	musFontHeight;

	UINT16	mColorLine;			// color of boundary lines
	UINT16	mColorLineShadow;	// color of boundary line shadows
	UINT16	mColorMarked;		// color of marked entries
	UINT16	mColorHighLight;	// color of bar highlighting
	
	BOOLEAN mfMouseRegionsCreated;
	BOOLEAN mfMouseRegionsCreated_Drop;

	MOUSE_REGION	mSelectedOpenDropDownRegion;
	MOUSE_REGION	mSelectedCloseDropDownRegion;

	MOUSE_REGION	mDropDownRegion[DROPDOWN_REGIONS];
	MOUSE_REGION	mgSelectedUpDownArrowOnScrollAreaRegion[2];
	MOUSE_REGION	mSelectedScrollAreaDropDownRegion[DROPDOWN_REGIONS];

	UINT32	muiGoldArrowImages;

	std::vector<std::pair<INT16, STR16> > mEntryVector;

	UINT8	mSelectedEntry;				// keeps track of the currently selected city
	UINT8	mFirstShownEntry;			// top entry of the dropped region
	UINT8	mNumDisplayedEntries;		// number of entries displayed. Calculated internally, no need to change by user
};

#endif
