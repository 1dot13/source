#ifndef __DYNAMICDIALOGUEWIDGET_H
#define __DYNAMICDIALOGUEWIDGET_H

/**
* @file
* @author Flugente (bears-pit.com)
* @brief This file contains declarations of classes and functions used for dynamic dialogue
*/

// Flugente: stuff
#include "Morale.h"
#include "DropDown.h"
#include "DynamicDialogue.h"

#define		MYBOX_TEXT_MAXWIDTH						200
#define		MYBOX_FACE_OFFSET						53

/*
* A class for dynamic dialogue boxes
* These boxes consist of a box with a merc's face in it, and text right next to it
*/
class DDBox : public WidgetBase
{
public:
	DDBox( UINT8 aID );

	/*
	* Create a Dropdown with upper left coordinates
	*/
	void Create( UINT16 sX, UINT16 sY );

	/*
	* Destroy Dropdown, for example once a website isn't displayed anymore
	*/
	void Destroy( );

	/*
	* Display DropDownBase. Use this when refreshing
	*/
	void Display( );

	void Refresh( );

	// we need to redo these functions, as the position can change dynamically
	virtual UINT16 GetY( );
	virtual UINT16 GetY_Text( );

	/*
	* Set help text decribing what can be selected
	*/
	void SetText( STR16 aText )
	{
		swprintf( mText, L"" );
		wcscat( mText, aText );
	}

	/*
	* Get width of entire DropDownBase
	*/
	UINT16	GetTotalWidth( )			{ return musWidth; }

	/*
	* Get right x coordinate
	*/
	UINT16	GetLastX( )					{ return GetX( ) + GetTotalWidth( ); }

	BOOLEAN IsInit( )					{ return mfInit; }

	/*
	* Are we displayed?
	*/
	BOOLEAN	IsDisplayed( )				{ return mfDisplayed; }

	void SetEndTime( UINT32 aVal )		{ musEndTime = aVal; }
	UINT32 GetEndTime( )				{ return musEndTime; }

	void SetStartTime( UINT32 aVal )	{ musCreationTime = aVal; }
	UINT32 GetStartTime( )				{ return musCreationTime; }

	UINT8 GetID( )						{ return musID; }

	void SetEvent( DynamicOpinionSpeechEvent& aEvent ) { mEvent = aEvent; }

	void ResetCreationTime( );

private:
	// declare but don't define
	DDBox( DDBox const& );
	void operator=(DDBox const&);

	/*
	* Initialise variables. Called after each creationm which allows moving a dropdown
	*/
	void Init( UINT16 sX, UINT16 sY );

	void DrawTopEntry( );

private:
	UINT16	musHeight;
	UINT16	musWidth;

	UINT16	musFontHeight;

	BOOLEAN mfInit;
	BOOLEAN mfDisplayed;
	BOOLEAN mfFaceImageExists;

	CHAR16	mText[500];

	UINT32	musCreationTime;
	UINT32  musEndTime;

	UINT8	musID;

	UINT32	musFaceImage;

	DynamicOpinionSpeechEvent mEvent;
};

BOOLEAN InitMyBoxes( );

DDBox* GetDDBox( UINT8 aID );
BOOLEAN RemoveDDBox( UINT8 aID );

void RefreshBoxes( );
void DelayBoxDestruction( UINT32 aVal );
void DelayBoxDestructionBy( INT32 aVal );


class IMPDialogueChooseBox : public WidgetBase
{
public:
	IMPDialogueChooseBox( );

	/*
	* Create a Dropdown with upper left coordinates
	*/
	void Create( UINT16 sX, UINT16 sY );

	/*
	* Destroy Dropdown, for example once a website isn't displayed anymore
	*/
	void Destroy( );

	/*
	* Display DropDownBase. Use this when refreshing
	*/
	void Display( );

	void Refresh( );

	// internal callback types
	enum {
		DROPDOWN_REGION,
	};

	/*
	* Set the content of a dropdown. Each entry consists of an INT16 key, by which you can later identify which entry was selected, and a STR16 that will be displayed.
	* There can be multiple instances of the same key or name.
	*/
	void SetEntries( std::vector<std::pair<INT16, STR16> >& arEntryVec )	{ mEntryVector = arEntryVec; }

	/*
	* Set help text decribing what can be selected
	*/
	void SetText( STR16 aText )
	{
		swprintf( mText, L"" );
		wcscat( mText, aText );
	}

	/*
	* Set help text decribing what can be selected
	*/
	void SetHelpText( STR16 aText )		{ swprintf( mHelpText, L"" ); wcscat( mHelpText, aText ); }

	/*
	* Get key of selected entry
	*/
	INT16	GetSelectedEntryKey( )		{ return mEntryVector[mSelectedEntry].first; }

	/*
	* Get width of entire DropDownBase
	*/
	UINT16	GetTotalWidth( )			{ return musWidth; }

	/*
	* Get right x coordinate
	*/
	UINT16	GetLastX( )					{ return GetX( ) + GetTotalWidth( ); }

	BOOLEAN IsInit( )					{ return mfInit; }

	/*
	* Are we displayed?
	*/
	BOOLEAN	IsDisplayed( )				{ return mfDisplayed; }

	/*
	* Do we have data to display?
	*/
	BOOLEAN HasEntries( )				{ return !mEntryVector.empty( ); }

	void SelectDropDownRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason );

	/*
	* This function is implemented again in DropDownTemplate
	*/
	virtual MOUSE_CALLBACK CallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(MOUSE_REGION * pRegion, INT32 iReason) )	{
		return Dummyfunc;
	}

	void SetEndTime( UINT32 aVal )		{ musEndTime = aVal; }
	UINT32 GetEndTime( )				{ return musEndTime; }

	void SetStartTime( UINT32 aVal )	{ musCreationTime = aVal; }
	UINT32 GetStartTime( )				{ return musCreationTime; }

	void SetEvent( DynamicOpinionSpeechEvent& aEvent ) { mEvent = aEvent; }

	void ResetCreationTime( );

	UINT32 GetTimeLeft( )				{ return musEndTime - musCreationTime; }

private:
	// declare but don't define
	IMPDialogueChooseBox( IMPDialogueChooseBox const& );
	void operator=(IMPDialogueChooseBox const&);

	/*
	* Initialise variables. Called after each creationm which allows moving a dropdown
	*/
	void Init( UINT16 sX, UINT16 sY );

	/*
	* This dummy is needed internally and does nothing when called
	*/
	static void Dummyfunc( MOUSE_REGION * pRegion, INT32 iReason )	{}

	/*
	* Stuff to do once the time is up
	*/
	void TimeOut( );

	void DrawTopEntry( );

private:
	UINT16	musHeight;
	UINT16	musWidth;

	UINT16	musFontHeight;

	BOOLEAN mfInit;
	BOOLEAN mfDisplayed;

	BOOLEAN mfMouseRegionsCreated;

	// a mouse region for every possible choice
	MOUSE_REGION	mChoiceRegion[DOST_CHOICE_MAX];
	BOOLEAN			mChoiceRegionDefined[DOST_CHOICE_MAX];

	std::vector<std::pair<INT16, STR16> > mEntryVector;

	CHAR16	mHelpText[200];

	UINT8	mSelectedEntry;				// keeps track of the currently selected city

	CHAR16	mText[500];

	UINT32	musCreationTime;
	UINT32  musEndTime;

	DynamicOpinionSpeechEvent mEvent;
};

// due to the way callbacks on mouse regions are handled, we need a static object that we can 'hook' on. Thus we use this singleton
class IMPDialogueChooseBox_Static : public IMPDialogueChooseBox
{
public:
	static IMPDialogueChooseBox_Static& getInstance( )
	{
		static IMPDialogueChooseBox_Static    instance; // Guaranteed to be destroyed.

		// Instantiated on first use.
		return instance;
	}

	static void SelectRegionDropDown_DropDown( MOUSE_REGION * pRegion, INT32 iReason )			{ return static_cast<IMPDialogueChooseBox*>(mpSelf)->SelectDropDownRegionCallBack( pRegion, iReason ); }

	MOUSE_CALLBACK
	CallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(MOUSE_REGION * pRegion, INT32 iReason) )
	{
		mpSelf = pt2Object;

		switch ( arg )
		{
		case DROPDOWN_REGION:
			return &SelectRegionDropDown_DropDown;
			break;
		}

		return *pt2Function;
	}

private:
	static void* mpSelf;

private:
	IMPDialogueChooseBox_Static( ) {};                   // private constructor, so we cannot create more instances

	// declare but don't define
	IMPDialogueChooseBox_Static( IMPDialogueChooseBox_Static const& );
	void operator=(IMPDialogueChooseBox_Static const&);
};

// are there still dynamic dialogue boxes active? We use this to check wether we can start new dialogue
BOOLEAN DynamicDialogueBoxesActive( );

// used fo example when we load a game - dialogue has to start anew, so all boxes must be wiped
void DestroyAllDynamicDialogueBoxes( );

#endif
