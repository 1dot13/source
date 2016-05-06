#ifndef __BASETABLE_H
#define __BASETABLE_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "DropDown.h"

/*
* A base class for tables
*/
class BaseTable : public WidgetBase
{
public:
	BaseTable( );

	void SetInit( BOOLEAN fVal )				{ fInit = fVal ; }
	BOOLEAN IsInit()							{ return fInit ; }

	/*
	* set maximum width and height
	*/
	void SetWidth( UINT16 aVal )				{ musWidth = aVal; }
	virtual UINT16 GetWidth( )					{ return musWidth; }
	void SetHeight( UINT16 aVal )				{ musHeight = aVal; }
	virtual UINT16 GetHeight( )					{ return musHeight; }
	void SetFrameWorkSpace( UINT16 aVal )		{ musFrameWorkSpace = aVal; }
	virtual UINT16 GetFrameWorkSpace( )			{ return musFrameWorkSpace; }

	/*
	* Initialise variables. Called after each creation which allows moving a dropdown
	*/
	virtual void Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

	/*
	* Create a Dropdown with upper left coordinates
	*/
	virtual void Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

	/*
	* Destroy Dropdown, for example once a website isn't displayed anymore
	*/
	virtual void Destroy( );

	/*
	* Display DropDownBase. Use this when refreshing
	*/
	virtual void Display( );

	/*
	* Sometimes a refresh inside the class isn't enough - best example is closing the dropdown area, after which the previous background cannot be restored by this class.
	* In that case, the background and this class has to be redrawn.
	* This function is called internally whenever such a refresh is necessary.
	* It has to be defined in derived classes and should notify your dialogues to redraw at appropriate times
	*
	* This function has to be implemented!
	*/
	virtual void SetRefresh( ) = 0;

	virtual void DestroyMouseRegions();

	void	SetActive( BOOLEAN aVal )			{ mActive = aVal; }
	BOOLEAN IsActive( )							{ return mActive; }

	virtual void	SetMouseRegionActive( BOOLEAN aVal )	{ mMouseRegionActive = aVal; }
	BOOLEAN IsMouseRegionActive( )				{ return mMouseRegionActive; }

	virtual void RegisterMemberThingy( BaseTable * arP )
	{ 
		if ( mRegisteredMembers.empty() )
		{
			mRegisteredMembers.push_back( arP );

			arP->SetActive( TRUE );
		}
		else
		{
			mRegisteredMembers.push_back( arP );
		}
	}

	void	SetName( STR16 aName )				{ mName = aName; }
	STR16	GetName()							{ return mName; }

private:
	// declare but don't define
	BaseTable( BaseTable const& );
	void operator=(BaseTable const&);

private:
	STR16	mName;

	BOOLEAN fInit;

	UINT16	musHeight;
	UINT16	musWidth;

	// as blank square dialogues are ugly, we can a framework on the edges - in this case we need to reserve some space for this
	UINT16 musFrameWorkSpace;

	// determines whether or not this widget will currently be shown
	BOOLEAN mActive;

	// if FALSE, do not display mouse regions
	BOOLEAN mMouseRegionActive;

	std::vector<BaseTable*>				mRegisteredMembers;
};


// a simple panel into which we can embed other widgets
class TestPanel : public BaseTable
{
public:
	TestPanel( );

	/*
	* Display DropDownBase. Use this when refreshing
	*/
	virtual void Display( );

	virtual void SetRefresh( );
};

extern TestPanel gTestPanel3;

// callback types
enum {
	CALLBACK_BUTTON_MOVE = 0,
	CALLBACK_BUTTON_CLICK,
	CALLBACK_REGION_MOVE,
	CALLBACK_REGION_CLICK,
};

class WidgetCallBackInstance
{
public:
	WidgetCallBackInstance( )	{}
	virtual ~WidgetCallBackInstance( )	{}

	virtual void RegionMoveCallBack( MOUSE_REGION * pRegion, INT32 iReason )			{}
	virtual void RegionClickCallBack( MOUSE_REGION * pRegion, INT32 iReason )			{}

	virtual void ButtonMoveCallBack( GUI_BUTTON *btn, INT32 reason )					{}
	virtual void ButtonClickCallBack( GUI_BUTTON *btn, INT32 reason )					{}

	/*
	* This function is implemented again in DropDownTemplate
	*/
	virtual MOUSE_CALLBACK RegionCallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(MOUSE_REGION * pRegion, INT32 iReason) )	{ return RegionDummyfunc; }
	virtual GUI_CALLBACK   ButtonCallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(GUI_BUTTON * btn, INT32 iReason) )		{ return ButtonDummyfunc; }
	
	/*
	* This dummy is needed internally and does nothing when called
	*/
	static void RegionDummyfunc( MOUSE_REGION * pRegion, INT32 iReason )	{}
	static void ButtonDummyfunc( GUI_BUTTON * btn, INT32 iReason )			{}
};


typedef STR16( *cdp_string_func )(UINT32 aNum);
STR16 cdp_string_func_dummy( UINT32 aNum );

typedef void( *cdp_image_func )(UINT32 aNum, UINT32& arImageLib, UINT16& arImage );
void cdp_image_func_dummy( UINT32 aNum, UINT32& arImageLib, UINT16& arImage );

typedef void( *cdp_statusbar_func )(UINT32 aId, UINT16& arCol1, UINT16& arVal1, UINT16& arCol2, UINT16& arVal2, UINT16& arCol3, UINT16& arVal3, UINT16& arCol4, UINT16& arVal4);
void cdp_statusbar_func_dummy( UINT32 aId, UINT16& arCol1, UINT16& arVal1, UINT16& arCol2, UINT16& arVal2, UINT16& arCol3, UINT16& arVal3, UINT16& arCol4, UINT16& arVal4 );

#define COLUMNDATAPROVIDER_MOUSEREGIONS	50

class ColumnDataProvider
{
public:
	ColumnDataProvider( STR16 aName ) : mName( aName ), mNumberOfEntries( 0 ), mRequiredHeigth(20), 
		mType( CDP_STRING ), mCallbackType( CDP_DEFAULT ),
		mFuncString( cdp_string_func_dummy ), mFuncImage( cdp_image_func_dummy )
	{
	}
	
	// general provider functions
	void				SetNumberOfEntries( UINT32 aVal )	{ mNumberOfEntries = aVal; CalcRequiredLength( ); }
	UINT32				GetNumberOfEntries( )				{ return mNumberOfEntries; }

	void				CalcRequiredLength( );
	UINT32				GetRequiredLength( )				{ return mRequiredLength; }
	void				SetRequiredLength( UINT32 aVal )	{ mRequiredLength = aVal; }		// this overrides the calculated value
	UINT32				GetRequiredHeigth( )				{ return mRequiredHeigth; }
	void				SetRequiredHeigth( UINT32 aVal )	{ mRequiredHeigth = aVal; }

	STR16				GetName( )							{ return mName; }

	typedef enum
	{
		CDP_DEFAULT = 0,
		CDP_MILITIA_LIST,
		CDP_MILITIADETAIL,
	} CDP_CALLBACKTYPE;

	typedef enum
	{
		CDP_STRING,
		CDP_IMAGE,
		CDP_STATUSBAR,
	} CDP_TYPE;

	CDP_TYPE			GetProviderType( )					{ return mType ; }
	void				SetProviderType( CDP_TYPE aType )	{ mType = aType; }

	// functions for handling string contents
	void				SetMethodString( cdp_string_func afunc )		{ mFuncString = afunc; SetProviderType( CDP_STRING ); CalcRequiredLength( ); }
	STR16				GetString( UINT32 aNum )			{ return mFuncString( aNum ); }

	// functions for handling images
	void				SetMethodImage( cdp_image_func afunc )	{ mFuncImage = afunc; SetProviderType( CDP_IMAGE ); CalcRequiredLength( ); }
	void				GetImageData( UINT32 aNum, UINT32& arImageLib, UINT16& arImage )	{ return mFuncImage( aNum, arImageLib, arImage ); }

	// functions for handling healthbars
	void				SetMethodStatusBar( cdp_statusbar_func afunc )	{ mFuncStatusBar = afunc; SetProviderType( CDP_STATUSBAR ); CalcRequiredLength( ); }
	void				GetStatusBarData( UINT32 aId, UINT16& arCol1, UINT16& arVal1, UINT16& arCol2, UINT16& arVal2, UINT16& arCol3, UINT16& arVal3, UINT16& arCol4, UINT16& arVal4 )
	{
		return mFuncStatusBar( aId, arCol1, arVal1, arCol2, arVal2, arCol3, arVal3, arCol4, arVal4 );
	}
	
	// callbacks for clicking on a column
	void				SetCallBackType( CDP_CALLBACKTYPE aVal )	{ mCallbackType = aVal; }
	CDP_CALLBACKTYPE	GetCallBackType( )							{ return mCallbackType; }
	static void			RegionClickCallBack( MOUSE_REGION * pRegion, INT32 iReason );
		
	void				DestroyMouseRegions();

public:
	MOUSE_REGION		mMouseRegion[COLUMNDATAPROVIDER_MOUSEREGIONS];
	BOOLEAN				mMouseRegion_Defined[COLUMNDATAPROVIDER_MOUSEREGIONS];

private:
	STR16				mName;

	UINT32				mNumberOfEntries;

	// the maximum length required for a column according to its contents
	UINT32				mRequiredLength;
	UINT32				mRequiredHeigth;

	CDP_TYPE			mType;
	CDP_CALLBACKTYPE	mCallbackType;

	cdp_string_func		mFuncString;
	cdp_image_func		mFuncImage;
	cdp_statusbar_func	mFuncStatusBar;
};


// a simple panel into which we can embed other widgets
class TabBox : public BaseTable, public WidgetCallBackInstance
{
public:
	TabBox( );

	/*
	* Initialise variables. Called after each creation which allows moving a dropdown
	*/
	virtual void Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

	/*
	* Create a Dropdown with upper left coordinates
	*/
	virtual void Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

	/*
	* Destroy Dropdown, for example once a website isn't displayed anymore
	*/
	virtual void Destroy( );

	/*
	* Display DropDownBase. Use this when refreshing
	*/
	virtual void Display( );

	virtual void DestroyMouseRegions( );

	struct TestPanelButtonStruct
	{
		BaseTable*		mTestPanel;
		MOUSE_REGION	mMouseRegion;
		BOOLEAN			mMouseRegion_Defined;
	};
	
	virtual void RegisterMemberThingy( BaseTable * arP )
	{ 
		if ( !mRegisteredTables.empty( ) )
			arP->SetActive( FALSE );

		TestPanelButtonStruct tmp;
		tmp.mTestPanel = arP;
		tmp.mMouseRegion_Defined = FALSE;
		mRegisteredTables.push_back( tmp );
		BaseTable::RegisterMemberThingy( arP );
	}

	virtual void RegionMoveCallBack( MOUSE_REGION * pRegion, INT32 iReason )	{}
	virtual void RegionClickCallBack( MOUSE_REGION * pRegion, INT32 iReason );

private:
	std::vector<TestPanelButtonStruct>				mRegisteredTables;
};


class TestTable : public BaseTable, public WidgetCallBackInstance
{
public:
	TestTable( );

	/*
	* Initialise variables. Called after each creation which allows moving a dropdown
	*/
	virtual void Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

	/*
	* Create a Dropdown with upper left coordinates
	*/
	virtual void Create( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

	/*
	* Destroy Dropdown, for example once a website isn't displayed anymore
	*/
	virtual void Destroy( );

	/*
	* Display DropDownBase. Use this when refreshing
	*/
	virtual void Display( );

	virtual void DestroyMouseRegions( )	;

	virtual void RegionMoveCallBack( MOUSE_REGION * pRegion, INT32 iReason );
	virtual void RegionClickCallBack( MOUSE_REGION * pRegion, INT32 iReason );

	virtual void ButtonClickCallBack( GUI_BUTTON *btn, INT32 reason );

	void AddColumnDataProvider( ColumnDataProvider aCP )	{ mColumnDataProviderVector.push_back( aCP ); }
	void ClearColumnDataProvider( )	{ mColumnDataProviderVector.clear(); }

	void CalcRows();

private:
	void	CreateScrollAreaButtons( );

	UINT32	GetNumberOfDataRows( );
	UINT32	GetNumberOfDataRowsShown( );

	UINT32	GetRequiredHeigthPerRow();

private:
	// parts of the scrollbar
	BOOLEAN				mScrollBarDefined;

	INT32				mScrollArrow[2];
	UINT32				mScrollArrowImage[2];

	MOUSE_REGION		mScrollRegion;

	UINT32				mFirstEntryShown;
	UINT32				mLastEntryShown;

	std::vector<ColumnDataProvider>		mColumnDataProviderVector;
};


template<int N>
class TestTableTemplate : public TestTable
{
public:
	static TestTableTemplate<N>& getInstance( )
	{
		static TestTableTemplate<N>    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

	// this function has to be implemented!
	virtual void SetRefresh( );

	virtual void Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

	static void RegionMove( MOUSE_REGION * pRegion, INT32 iReason )					{ return static_cast<TestTable*>(mpSelf)->RegionMoveCallBack( pRegion, iReason ); }
	static void RegionClick( MOUSE_REGION * pRegion, INT32 iReason )				{ return static_cast<TestTable*>(mpSelf)->RegionClickCallBack( pRegion, iReason ); }
	static void ButtonMove( GUI_BUTTON *btn, INT32 iReason )						{ return static_cast<TestTable*>(mpSelf)->ButtonMoveCallBack( btn, iReason ); }
	static void ButtonClick( GUI_BUTTON *btn, INT32 iReason )						{ return static_cast<TestTable*>(mpSelf)->ButtonClickCallBack( btn, iReason ); }

	MOUSE_CALLBACK
	RegionCallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(MOUSE_REGION * pRegion, INT32 iReason) )
	{
		mpSelf = pt2Object;

		switch ( arg )
		{
			case CALLBACK_REGION_MOVE:			return &RegionMove;				break;
			case CALLBACK_REGION_CLICK:			return &RegionClick;			break;
		}

		return *pt2Function;
	}

	GUI_CALLBACK
	ButtonCallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(GUI_BUTTON *btn, INT32 iReason) )
	{
		mpSelf = pt2Object;

		switch ( arg )
		{
			case CALLBACK_BUTTON_MOVE:			return &ButtonMove;				break;
			case CALLBACK_BUTTON_CLICK:			return &ButtonClick;			break;
		}

		return *pt2Function;
	}

private:
	static void* mpSelf;

private:
	TestTableTemplate<N>( ) {};                   // private constructor, so we cannot create more instances

	// declare but don't define
	TestTableTemplate( TestTableTemplate const& );
	void operator=(TestTableTemplate const&);
};

template <int N>
void TestTableTemplate<N>::SetRefresh();

template <int N>
void TestTableTemplate<N>::Init( UINT16 sX, UINT16 sY, UINT16 sX_End, UINT16 sY_End );

template <int N>
void* TestTableTemplate<N>::mpSelf = NULL;


template<int N>
class TabBoxTemplate : public TabBox
{
public:
	static TabBoxTemplate<N>& getInstance( )
	{
		static TabBoxTemplate<N>    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

	static TabBox* GetPSelf( )	{ return static_cast<TabBox*>(mpSelf); }

	// this function has to be implemented!
	virtual void SetRefresh( );

	static void RegionMove( MOUSE_REGION * pRegion, INT32 iReason )					{ return GetPSelf()->RegionMoveCallBack( pRegion, iReason ); }
	static void RegionClick( MOUSE_REGION * pRegion, INT32 iReason )				{ return GetPSelf()->RegionClickCallBack( pRegion, iReason ); }
	static void ButtonMove( GUI_BUTTON *btn, INT32 iReason )						{ return GetPSelf()->ButtonMoveCallBack( btn, iReason ); }
	static void ButtonClick( GUI_BUTTON *btn, INT32 iReason )						{ return GetPSelf()->ButtonClickCallBack( btn, iReason ); }

	void SetPSelf( void* pt2Object )	{ mpSelf = pt2Object; }

	MOUSE_CALLBACK
	RegionCallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(MOUSE_REGION * pRegion, INT32 iReason) )
	{
		SetPSelf ( pt2Object );

		switch ( arg )
		{
			case CALLBACK_REGION_MOVE:			return &RegionMove;				break;
			case CALLBACK_REGION_CLICK:			return &RegionClick;			break;
		}

		return *pt2Function;
	}

	GUI_CALLBACK
	ButtonCallBackWrapper( void* pt2Object, UINT8 arg, void( *pt2Function )(GUI_BUTTON *btn, INT32 iReason) )
	{
		SetPSelf( pt2Object );

		switch ( arg )
		{
			case CALLBACK_BUTTON_MOVE:			return &ButtonMove;				break;
			case CALLBACK_BUTTON_CLICK:			return &ButtonClick;			break;
		}

		return *pt2Function;
	}

private:
	static void* mpSelf;

private:
	TabBoxTemplate<N>( ) {};                   // private constructor, so we cannot create more instances

	// declare but don't define
	TabBoxTemplate( TabBoxTemplate const& );
	void operator=(TabBoxTemplate const&);
};

template <int N>
void TabBoxTemplate<N>::SetRefresh( );

template <int N>
void* TabBoxTemplate<N>::mpSelf = NULL;

#endif
