#ifndef __MILITIAINTERFACE_H
#define __MILITIAINTERFACE_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "BaseTable.h"
#include "MilitiaIndividual.h"

extern UINT32 gusCurrentMilitia;

// the data of an individual militia is displayed on its own widget. There are several parts:
// - face, name, traits and gun are displayed on top. This data is also displayed for each militia in a list
// - Below that there is a tabbox with 3 entries:
//   - On the service record, we see their current body, stats & health and their history - which battles they took part in, and where and when they were recruited
//     Clicking on a history item links us to to campaign history. The history items are in a table.
//   - The skilltree is a separate tab. Here we can choose traits and promote militia, provided they have sufficient experience (currently not used)
//   - The inventory is a separate tab as well.

class MilitiaServiceRecordTable : public BaseTable
{
public:
	MilitiaServiceRecordTable( );
		
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
	virtual void SetRefresh( );

	void SetId( UINT32 usId )	{ mMilitiaId = usId; }
	
private:
	// declare but don't define
	MilitiaServiceRecordTable( MilitiaServiceRecordTable const& );
	void operator=(MilitiaServiceRecordTable const&);

private:
	UINT32		mMilitiaId;				// the id of the militia whose data we display
};

class MilitiaPersonalDataTable : public TestPanel
{
public:
	MilitiaPersonalDataTable( );

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
	virtual void SetRefresh( );

	virtual void DestroyMouseRegions( );

	void SetId( UINT32 usId )	{ mMilitiaId = usId; }

private:
	// declare but don't define
	MilitiaPersonalDataTable( MilitiaPersonalDataTable const& );
	void operator=(MilitiaPersonalDataTable const&);

private:
	UINT32		mMilitiaId;				// the id of the militia whose data we display

	INT32		mButtonFire;
	BOOLEAN		mButtonFireCreated;
};

class MilitiaPersonalInventoryTable : public TestPanel
{
public:
	MilitiaPersonalInventoryTable( );

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
	virtual void SetRefresh( );

	virtual void DestroyMouseRegions( );

	void SetId( UINT32 usId )	{ mMilitiaId = usId; }

private:
	// declare but don't define
	MilitiaPersonalInventoryTable( MilitiaPersonalInventoryTable const& );
	void operator=(MilitiaPersonalInventoryTable const&);

private:
	UINT32		mMilitiaId;				// the id of the militia whose data we display
};

class MilitiaInidividualMainWidget : public TestPanel
{
public:
	MilitiaInidividualMainWidget( );

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
	virtual void SetRefresh( );

	virtual void DestroyMouseRegions( );

	virtual void SetMouseRegionActive( BOOLEAN aVal )	{ TabBoxTemplate<1>::getInstance( ).SetMouseRegionActive( aVal ); }

	void SetId( UINT32 usId )	{ mMilitiaId = usId; gusCurrentMilitia = usId; mPersonalData.SetId( usId ); mServiceRecord.SetId( usId ); mItemsTable.SetId( usId ); }

	static void CloseButtonCallback( GUI_BUTTON * btn, INT32 iReason );

private:
	// declare but don't define
	MilitiaInidividualMainWidget( MilitiaInidividualMainWidget const& );
	void operator=(MilitiaInidividualMainWidget const&);

private:
	UINT32							mMilitiaId;					// the id of the militia whose data we display

	TestPanel						mPersonalDataPanel;
	MilitiaPersonalDataTable		mPersonalData;
	TestPanel						mServiceRecordPanel;
	MilitiaServiceRecordTable		mServiceRecord;
	TestPanel						mItemPanel;
	MilitiaPersonalInventoryTable	mItemsTable;

	// close buttons
	BOOLEAN					mCloseButtonDefined;
	INT32					mCloseButton;
	UINT32					mCloseImage;
};

extern MilitiaInidividualMainWidget militiaindividualmainwidget;

#endif
