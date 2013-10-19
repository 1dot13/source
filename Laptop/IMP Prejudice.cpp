#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
	#include "IMP Skill Trait.h"
	#include "_Ja25Englishtext.h"
#else
	#include "IMP Prejudice.h"
	#include "IMP Skill Trait.h"
	#include "Button System.h"
	#include "utilities.h"
	#include "Debug.h"
	#include "Text.h"
	#include "Font Control.h"
	#include "font.h"
	#include "laptop.h"
	#include "cursors.h"
	#include "IMP MainPage.h"
	#include "IMPVideoObjects.h"
	#include "_Ja25EnglishText.h"
	#include "wordwrap.h"
	#include "CharProfile.h"
	#include "soldier profile type.h"
	#include "IMP Compile Character.h"
	#include "GameSettings.h"
	#include "Interface.h"
	#include "DropDown.h"
#endif


//*******************************************************************
//
// Local Defines
//
//*******************************************************************

#define	IMP_PREJUDICE__FONT							FONT12ARIAL
#define	IMP_PREJUDICE__COLOR						FONT_MCOLOR_WHITE

#define	IMP_PREJUDICE__TITLE_X						LAPTOP_SCREEN_UL_X - 111
#define	IMP_PREJUDICE__TITLE_Y						iScreenHeightOffset + 53
#define	IMP_PREJUDICE__TITLE_WIDTH					( LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X )

//*******************************************************************
//
// Local Variables
//
//*******************************************************************

BOOLEAN gfIMPPrejudice_Redraw=FALSE;

// this is the Done	buttons
INT32 giIMPPrejudiceFinishButton;
INT32 giIMPPrejudiceFinishButtonImage;

//*******************************************************************
//
// Function Prototypes
//
//*******************************************************************

void		BtnIMPPrejudiceFinishCallback(GUI_BUTTON *btn,INT32 reason);
void		IMPPrejudiceDisplay();
BOOLEAN		CameBackToPrejudicePageButNotFinished();

//*******************************************************************
//
// Functions
//
//*******************************************************************


//*******************************************************************
//
// DropDown instances
//
//*******************************************************************
#define DROPDOWN_MARKUP_Y	50

//*******************************************************************
//
// DropDown declarations
//
//*******************************************************************
class DropDown_Appearance : public DropDown
{
public:
	static DropDown_Appearance& getInstance()
    {
        static DropDown_Appearance    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_Appearance() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_Appearance(DropDown_Appearance const&); 
    void operator=(DropDown_Appearance const&);
};

class DropDown_AppearanceCare : public DropDown
{
public:
	static DropDown_AppearanceCare& getInstance()
    {
        static DropDown_AppearanceCare    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_AppearanceCare() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_AppearanceCare(DropDown_AppearanceCare const&); 
    void operator=(DropDown_AppearanceCare const&);
};

class DropDown_Refinement : public DropDown
{
public:
	static DropDown_Refinement& getInstance()
    {
        static DropDown_Refinement    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_Refinement() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_Refinement(DropDown_Refinement const&); 
    void operator=(DropDown_Refinement const&);
};

class DropDown_RefinementCare : public DropDown
{
public:
	static DropDown_RefinementCare& getInstance()
    {
        static DropDown_RefinementCare    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_RefinementCare() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_RefinementCare(DropDown_RefinementCare const&); 
    void operator=(DropDown_RefinementCare const&);
};

class DropDown_Nationality : public DropDown
{
public:
	static DropDown_Nationality& getInstance()
    {
        static DropDown_Nationality    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_Nationality() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_Nationality(DropDown_Nationality const&); 
    void operator=(DropDown_Nationality const&);
};

class DropDown_HatedNationality : public DropDown
{
public:
	static DropDown_HatedNationality& getInstance()
    {
        static DropDown_HatedNationality    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_HatedNationality() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_HatedNationality(DropDown_HatedNationality const&); 
    void operator=(DropDown_HatedNationality const&);
};

class DropDown_HatedNationalityCare : public DropDown
{
public:
	static DropDown_HatedNationalityCare& getInstance()
    {
        static DropDown_HatedNationalityCare    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_HatedNationalityCare() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_HatedNationalityCare(DropDown_HatedNationalityCare const&); 
    void operator=(DropDown_HatedNationalityCare const&);
};

class DropDown_Racist : public DropDown
{
public:
	static DropDown_Racist& getInstance()
    {
        static DropDown_Racist    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_Racist() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_Racist(DropDown_Racist const&); 
    void operator=(DropDown_Racist const&);
};

class DropDown_RacistTarget : public DropDown
{
public:
	static DropDown_RacistTarget& getInstance()
    {
        static DropDown_RacistTarget    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_RacistTarget() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_RacistTarget(DropDown_RacistTarget const&); 
    void operator=(DropDown_RacistTarget const&);
};

class DropDown_Sexist : public DropDown
{
public:
	static DropDown_Sexist& getInstance()
    {
        static DropDown_Sexist    instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
	
	virtual MOUSE_CALLBACK CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) );

	// this function has to be implemented!
	virtual void SetRefresh()	{ gfIMPPrejudice_Redraw = TRUE; }

private:
	DropDown_Sexist() {};                   // private cosntructor, so we cannot create more instances
    
	// declare but don't define
    DropDown_Sexist(DropDown_Sexist const&); 
    void operator=(DropDown_Sexist const&);
};

//*******************************************************************
//
// DropDown definitions
//
//*******************************************************************
void* gPtrDropDown_Appearance;
void OpenDropDown_DropDown_Appearance(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Appearance)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_Appearance(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Appearance)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_Appearance(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Appearance)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_Appearance(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Appearance)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_Appearance(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Appearance)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_Appearance(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_Appearance)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_Appearance(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_Appearance)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_Appearance::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_Appearance = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_Appearance;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_Appearance;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_Appearance;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_Appearance;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_Appearance;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_Appearance;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_Appearance;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_AppearanceCare;
void OpenDropDown_DropDown_AppearanceCare(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_AppearanceCare)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_AppearanceCare(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_AppearanceCare)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_AppearanceCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_AppearanceCare)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_AppearanceCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_AppearanceCare)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_AppearanceCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_AppearanceCare)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_AppearanceCare(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_AppearanceCare)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_AppearanceCare(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_AppearanceCare)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_AppearanceCare::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_AppearanceCare = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_AppearanceCare;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_AppearanceCare;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_AppearanceCare;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_AppearanceCare;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_AppearanceCare;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_AppearanceCare;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_AppearanceCare;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_Refinement;
void OpenDropDown_DropDown_Refinement(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Refinement)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_Refinement(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Refinement)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_Refinement(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Refinement)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_Refinement(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Refinement)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_Refinement(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Refinement)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_Refinement(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_Refinement)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_Refinement(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_Refinement)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_Refinement::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_Refinement = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_Refinement;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_Refinement;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_Refinement;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_Refinement;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_Refinement;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_Refinement;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_Refinement;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_RefinementCare;
void OpenDropDown_DropDown_RefinementCare(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_RefinementCare)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_RefinementCare(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_RefinementCare)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_RefinementCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_RefinementCare)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_RefinementCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_RefinementCare)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_RefinementCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_RefinementCare)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_RefinementCare(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_RefinementCare)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_RefinementCare(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_RefinementCare)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_RefinementCare::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_RefinementCare = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_RefinementCare;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_RefinementCare;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_RefinementCare;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_RefinementCare;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_RefinementCare;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_RefinementCare;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_RefinementCare;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_Nationality;
void OpenDropDown_DropDown_Nationality(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Nationality)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_Nationality(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Nationality)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_Nationality(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Nationality)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_Nationality(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_Nationality)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_Nationality(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Nationality)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_Nationality(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_Nationality)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_Nationality(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_Nationality)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_Nationality::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_Nationality = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_Nationality;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_Nationality;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_Nationality;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_Nationality;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_Nationality;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_Nationality;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_Nationality;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_HatedNationality;
void OpenDropDown_DropDown_HatedNationality(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_HatedNationality)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_HatedNationality(MOUSE_REGION * pRegion, INT32 iReason )				{ return static_cast<DropDown*>(gPtrDropDown_HatedNationality)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_HatedNationality(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_HatedNationality)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_HatedNationality(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_HatedNationality)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_HatedNationality(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_HatedNationality)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_HatedNationality(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_HatedNationality)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_HatedNationality(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_HatedNationality)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_HatedNationality::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_HatedNationality = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_HatedNationality;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_HatedNationality;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_HatedNationality;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_HatedNationality;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_HatedNationality;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_HatedNationality;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_HatedNationality;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_HatedNationalityCare;
void OpenDropDown_DropDown_HatedNationalityCare(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_HatedNationalityCare)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_HatedNationalityCare(MOUSE_REGION * pRegion, INT32 iReason )				{ return static_cast<DropDown*>(gPtrDropDown_HatedNationalityCare)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_HatedNationalityCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_HatedNationalityCare)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_HatedNationalityCare(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_HatedNationalityCare)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_HatedNationalityCare(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_HatedNationalityCare)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_HatedNationalityCare(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_HatedNationalityCare)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_HatedNationalityCare(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_HatedNationalityCare)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_HatedNationalityCare::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_HatedNationalityCare = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_HatedNationalityCare;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_HatedNationalityCare;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_HatedNationalityCare;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_HatedNationalityCare;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_HatedNationalityCare;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_HatedNationalityCare;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_HatedNationalityCare;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_Racist;
void OpenDropDown_DropDown_Racist(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Racist)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_Racist(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Racist)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_Racist(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Racist)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_Racist(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Racist)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_Racist(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Racist)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_Racist(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_Racist)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_Racist(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_Racist)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_Racist::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_Racist = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_Racist;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_Racist;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_Racist;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_Racist;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_Racist;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_Racist;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_Racist;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_RacistTarget;
void OpenDropDown_DropDown_RacistTarget(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_RacistTarget)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_RacistTarget(MOUSE_REGION * pRegion, INT32 iReason )				{ return static_cast<DropDown*>(gPtrDropDown_RacistTarget)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_RacistTarget(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_RacistTarget)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_RacistTarget(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_RacistTarget)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_RacistTarget(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_RacistTarget)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_RacistTarget(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_RacistTarget)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_RacistTarget(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_RacistTarget)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_RacistTarget::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_RacistTarget = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_RacistTarget;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_RacistTarget;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_RacistTarget;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_RacistTarget;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_RacistTarget;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_RacistTarget;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_RacistTarget;
		break;
	}

	return *pt2Function;
}

void* gPtrDropDown_Sexist;
void OpenDropDown_DropDown_Sexist(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Sexist)->OpenDropDownRegionCallBack(pRegion, iReason); }
void CloseDropDown_DropDown_Sexist(MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<DropDown*>(gPtrDropDown_Sexist)->CloseDropDownRegionCallBack(pRegion, iReason); }
void SelectRegionDropDown_DropDown_Sexist(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Sexist)->SelectDropDownRegionCallBack(pRegion, iReason); }
void SelectMovementDropDown_DropDown_Sexist(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Sexist)->SelectDropDownMovementCallBack(pRegion, iReason); }
void SelectArrowDropDown_DropDown_Sexist(MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<DropDown*>(gPtrDropDown_Sexist)->SelectUpDownArrowOnScrollAreaRegionCallBack(pRegion, iReason); }
void SelectScrollRegionDropDown_DropDown_Sexist(MOUSE_REGION * pRegion, INT32 iReason )		{ return static_cast<DropDown*>(gPtrDropDown_Sexist)->SelectScrollAreaDropDownRegionCallBack(pRegion, iReason); }
void SelectScrollMovementDropDown_DropDown_Sexist(MOUSE_REGION * pRegion, INT32 iReason )	{ return static_cast<DropDown*>(gPtrDropDown_Sexist)->SelectScrollAreaDropDownMovementCallBack(pRegion, iReason); }

MOUSE_CALLBACK
DropDown_Sexist::CallBackWrapper(void* pt2Object, UINT8 arg, void  (*pt2Function)(MOUSE_REGION * pRegion, INT32 iReason) )
{
	gPtrDropDown_Sexist = pt2Object;

	switch ( arg )
	{
	case DROPDOWN_OPEN:
		return &OpenDropDown_DropDown_Sexist;
		break;
	case DROPDOWN_CLOSE:
		return &CloseDropDown_DropDown_Sexist;
		break;
	case DROPDOWN_REGION:
		return &SelectRegionDropDown_DropDown_Sexist;
		break;
	case DROPDOWN_MOVEMENT:
		return &SelectMovementDropDown_DropDown_Sexist;
		break;
	case DROPDOWN_ARROW:
		return &SelectArrowDropDown_DropDown_Sexist;
		break;
	case DROPDOWN_SCROLL_REGION:
		return &SelectScrollRegionDropDown_DropDown_Sexist;
		break;
	case DROPDOWN_SCROLL_MOVEMENT:
		return &SelectScrollMovementDropDown_DropDown_Sexist;
		break;
	}

	return *pt2Function;
}


void EnterIMPPrejudice( void )
{
	RenderProfileBackGround();
				
	giIMPPrejudiceFinishButtonImage =	LoadButtonImage( "LAPTOP\\button_5.sti" ,-1,0,-1,1,-1 );
	giIMPPrejudiceFinishButton = CreateIconAndTextButton( giIMPPrejudiceFinishButtonImage, pImpButtonText[ 24 ], FONT12ARIAL,
																FONT_WHITE, DEFAULT_SHADOW,
																FONT_WHITE, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																LAPTOP_SCREEN_UL_X +	( 350 ), LAPTOP_SCREEN_WEB_UL_Y + ( 340 ), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnIMPPrejudiceFinishCallback );
	
	SetButtonCursor( giIMPPrejudiceFinishButton, CURSOR_WWW);

	UINT16 usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOULOOK], FONT12ARIAL ) + 10;
	UINT16 usY = LAPTOP_SCREEN_WEB_UL_Y + 43;
	////////////////// DropDown_Appearance //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Appearance;
	for(UINT8 i = 0; i < NUM_APPEARANCES; ++i)
		entryvecDropDown_Appearance.push_back( std::make_pair<INT16, STR16>(i, szAppearanceText[i]) );

	DropDown_Appearance::getInstance().SetEntries(entryvecDropDown_Appearance);
	DropDown_Appearance::getInstance().Create(usX, usY);
	////////////////// DropDown_Appearance //////////////////////////

	usX = DropDown_Appearance::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_ANDAPPEARANCEIS], FONT12ARIAL ) + 20;
	////////////////// DropDown_AppearanceCare //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_AppearanceCare;
	for(UINT8 i = 0; i < NUM_CARELEVELS; ++i)
		entryvecDropDown_AppearanceCare.push_back( std::make_pair<INT16, STR16>(i, szCareLevelText[i]) );

	DropDown_AppearanceCare::getInstance().SetEntries(entryvecDropDown_AppearanceCare);
	DropDown_AppearanceCare::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDown_AppearanceCare::getInstance().Create(usX, usY);
	////////////////// DropDown_AppearanceCare //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUHAVE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDown_Refinement //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Refinement;
	for(UINT8 i = 0; i < NUM_REFINEMENT; ++i)
		entryvecDropDown_Refinement.push_back( std::make_pair<INT16, STR16>(i, szRefinementText[i]) );

	DropDown_Refinement::getInstance().SetEntries(entryvecDropDown_Refinement);
	DropDown_Refinement::getInstance().Create(usX, usY);
	////////////////// DropDown_Refinement //////////////////////////

	usX = DropDown_Refinement::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_ANDCARE], FONT12ARIAL ) + 20;
	////////////////// DropDown_RefinementCare //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_RefinementCare;
	for(UINT8 i = 0; i < NUM_CARELEVELS; ++i)
		entryvecDropDown_RefinementCare.push_back( std::make_pair<INT16, STR16>(i, szCareLevelText[i]) );

	DropDown_RefinementCare::getInstance().SetEntries(entryvecDropDown_RefinementCare);
	DropDown_RefinementCare::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDown_RefinementCare::getInstance().Create(usX, usY);
	////////////////// DropDown_RefinementCare //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDown_Nationality //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Nationality;
	for(UINT8 i = 0; i < NUM_NATIONALITIES; ++i)
		entryvecDropDown_Nationality.push_back( std::make_pair<INT16, STR16>(i, szNationalityText[i]) );

	DropDown_Nationality::getInstance().SetEntries(entryvecDropDown_Nationality);
	DropDown_Nationality::getInstance().Create(usX, usY);
	////////////////// DropDown_Nationality //////////////////////////

	usX = DropDown_Nationality::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_ADHATEEVERYONE], FONT12ARIAL ) + 20;
	////////////////// DropDown_HatedNationality //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_HatedNationality;
	// we can also hate no nation at all...
	entryvecDropDown_HatedNationality.push_back( std::make_pair<INT16, STR16>(-1, szNationalityText_Special[1]) );

	for(UINT8 i = 0; i < NUM_NATIONALITIES; ++i)
		entryvecDropDown_HatedNationality.push_back( std::make_pair<INT16, STR16>(i, szNationalityText[i]) );

	DropDown_HatedNationality::getInstance().SetEntries(entryvecDropDown_HatedNationality);
	DropDown_HatedNationality::getInstance().Create(usX, usY);
	////////////////// DropDown_HatedNationality //////////////////////////

	usX = DropDown_HatedNationality::getInstance().GetLastX() + 5;
	////////////////// DropDown_HatedNationalityCare //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_HatedNationalityCare;
	for(UINT8 i = 0; i < NUM_CARELEVELS; ++i)
		entryvecDropDown_HatedNationalityCare.push_back( std::make_pair<INT16, STR16>(i, szCareLevelText[i]) );

	DropDown_HatedNationalityCare::getInstance().SetEntries(entryvecDropDown_HatedNationalityCare);
	DropDown_HatedNationalityCare::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDown_HatedNationalityCare::getInstance().Create(usX, usY);
	////////////////// DropDown_HatedNationalityCare //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDown_Racist //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Racist;
	for(UINT8 i = 0; i < NUM_RACIST; ++i)
		entryvecDropDown_Racist.push_back( std::make_pair<INT16, STR16>(i, szRacistText[i]) );

	DropDown_Racist::getInstance().SetEntries(entryvecDropDown_Racist);
	DropDown_Racist::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDown_Racist::getInstance().Create(usX, usY);
	////////////////// DropDown_Racist //////////////////////////

	usX = DropDown_Racist::getInstance().GetLastX() + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_RACISTAGAINSTNON], FONT12ARIAL ) + 10;
	////////////////// DropDown_RacistTarget //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_RacistTarget;
	for(UINT8 i = 0; i < NUM_RACES; ++i)
		entryvecDropDown_RacistTarget.push_back( std::make_pair<INT16, STR16>(i, szRaceText[i]) );

	DropDown_RacistTarget::getInstance().SetEntries(entryvecDropDown_RacistTarget);
	DropDown_RacistTarget::getInstance().Create(usX, usY);
	////////////////// DropDown_RacistTarget //////////////////////////

	usX = LAPTOP_SCREEN_UL_X + 5 + StringPixLength ( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], FONT12ARIAL ) + 10;
	usY += DROPDOWN_MARKUP_Y;
	////////////////// DropDown_Sexist //////////////////////////
	std::vector<std::pair<INT16, STR16> > entryvecDropDown_Sexist;
	for(UINT8 i = 0; i < NUM_SEXIST; ++i)
		entryvecDropDown_Sexist.push_back( std::make_pair<INT16, STR16>(i, szSexistText[i]) );

	DropDown_Sexist::getInstance().SetEntries(entryvecDropDown_Sexist);
	DropDown_Sexist::getInstance().SetColorLine( Get16BPPColor( FROMRGB( 38, 191, 60 ) ) );
	DropDown_Sexist::getInstance().Create(usX, usY);
	////////////////// DropDown_Sexist //////////////////////////
}


void RenderIMPPrejudice( void )
{
	//render the metal background graphic
	RenderProfileBackGround();

	IMPPrejudiceDisplay();

	DropDown_Appearance::getInstance().Display();
	DropDown_AppearanceCare::getInstance().Display();
	DropDown_Refinement::getInstance().Display();
	DropDown_RefinementCare::getInstance().Display();
	DropDown_Nationality::getInstance().Display();
	DropDown_HatedNationality::getInstance().Display();
	DropDown_HatedNationalityCare::getInstance().Display();
	DropDown_Racist::getInstance().Display();
	DropDown_RacistTarget::getInstance().Display();
	DropDown_Sexist::getInstance().Display();
}


void ExitIMPPrejudice( void )
{
	DropDown_Appearance::getInstance().Destroy();
	DropDown_AppearanceCare::getInstance().Destroy();
	DropDown_Refinement::getInstance().Destroy();
	DropDown_RefinementCare::getInstance().Destroy();
	DropDown_Nationality::getInstance().Destroy();
	DropDown_HatedNationality::getInstance().Destroy();
	DropDown_HatedNationalityCare::getInstance().Destroy();
	DropDown_Racist::getInstance().Destroy();
	DropDown_RacistTarget::getInstance().Destroy();
	DropDown_Sexist::getInstance().Destroy();

	RemoveButton( giIMPPrejudiceFinishButton );
	UnloadButtonImage( giIMPPrejudiceFinishButtonImage );
}


void HandleIMPPrejudice( void )
{
	if( gfIMPPrejudice_Redraw )
	{
		RenderIMPPrejudice( );
		gfIMPPrejudice_Redraw = FALSE;
	}

	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}

void IMPPrejudiceDisplay()
{
	UINT32 uiCnt;
	UINT16 usPosX, usPosY;

	//Display the title
	DrawTextToScreen( L"I.M.P. Prejudices", IMP_PREJUDICE__TITLE_X, IMP_PREJUDICE__TITLE_Y, IMP_PREJUDICE__TITLE_WIDTH, FONT14ARIAL, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	ColorFillVideoSurfaceArea( FRAME_BUFFER, LAPTOP_SCREEN_UL_X + 2, LAPTOP_SCREEN_WEB_UL_Y + 40, LAPTOP_SCREEN_LR_X - 2,	LAPTOP_SCREEN_WEB_UL_Y + 20 + 5 * DROPDOWN_MARKUP_Y, Get16BPPColor( FROMRGB( 100, 100, 100 ) ) );

	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY = LAPTOP_SCREEN_WEB_UL_Y + 50;
		
	// Appearance
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOULOOK], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_Appearance::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ANDAPPEARANCEIS], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_AppearanceCare::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_IMPORTANTTOYOU], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
				
	// Refinement
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUHAVE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_Refinement::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ANDCARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_RefinementCare::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ABOUTTHAT], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Hated Nations
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_Nationality::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_ADHATEEVERYONE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Racism
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_Racist::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_RACISTAGAINSTNON], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_RacistTarget::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_PEOPLE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Sexism
	usPosX = LAPTOP_SCREEN_UL_X + 5;
	usPosY += DROPDOWN_MARKUP_Y;
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_YOUARE], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosX = 10 + DropDown_Sexist::getInstance().GetLastX();
	DrawTextToScreen( szPersonalityDisplayText[PERSONALITYTEXT_DOT], usPosX, usPosY, 0, IMP_PREJUDICE__FONT, IMP_PREJUDICE__COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
}


void BtnIMPPrejudiceFinishCallback(GUI_BUTTON *btn,INT32 reason)
{
	// btn callback for IMP personality quiz answer button
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		//if we are just reviewing the page
		if( iCurrentProfileMode == IMP__FINISH )
		{
			//go back tot he done screen
			iCurrentImpPage = IMP_FINISH;
		}
		// Flugente: setting determines wether we see the background page
		else
		{
			// we finished this -> write down our choices
			bRace						= ( INT8)DropDown_RacistTarget::getInstance().GetSelectedEntryKey();
			bNationality				= ( INT8)DropDown_Nationality::getInstance().GetSelectedEntryKey();
			bAppearance					= ( INT8)DropDown_Appearance::getInstance().GetSelectedEntryKey();
			bAppearanceCareLevel		= ( INT8)DropDown_AppearanceCare::getInstance().GetSelectedEntryKey();
			bRefinement					= ( INT8)DropDown_Refinement::getInstance().GetSelectedEntryKey();
			bRefinementCareLevel		= ( INT8)DropDown_RefinementCare::getInstance().GetSelectedEntryKey();
			bHatedNationality			= ( INT8)DropDown_HatedNationality::getInstance().GetSelectedEntryKey();
			bHatedNationalityCareLevel	= ( INT8)DropDown_HatedNationalityCare::getInstance().GetSelectedEntryKey();
			bRacist						= ( INT8)DropDown_Racist::getInstance().GetSelectedEntryKey();
			bSexist						= (UINT8)DropDown_Sexist::getInstance().GetSelectedEntryKey();

			if ( gGameOptions.fBackGround )
			{
				iCurrentImpPage = IMP_BACKGROUND;

				fButtonPendingFlag = TRUE;
			}
			else
			{
				iCurrentImpPage = IMP_MAIN_PAGE;

				if( CameBackToPrejudicePageButNotFinished() )
				{
				}
				else
				{
					//We are finished on this page
					iCurrentProfileMode = IMP__ATTRIBUTES;
				}
			}
		}
	}
}

BOOLEAN CameBackToPrejudicePageButNotFinished()
{
	//if we are in a page that comes after this one
	if( iCurrentProfileMode == IMP__ATTRIBUTES )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}
