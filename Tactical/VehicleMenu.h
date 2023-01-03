#ifndef _VEHICLE_MENU_
#define _VEHICLE_MENU_

#include "popup_class.h"
#include "Soldier Profile.h"

class VehicleMenuItem
{
public:
	VehicleMenuItem()	:
	  fInitialized(FALSE), usPosX(0), gPopup(NULL)
	{
	}
	  	
	virtual void Cancel()
	{
		// Hide the Filter Menu pop-up.
		if (gPopup != NULL && fInitialized )
		{
			gPopup->hide();
		}
	}

	virtual void Destroy()
	{
		// If we already have a popup, destroy it first. This ensures we get a fresh menu each time.
		if ( fInitialized )
		{
			delete(gPopup);
			gPopup = NULL;
			fInitialized = FALSE;
		}
	}

	UINT16 GetMaxPosX()
	{
		if ( gPopup )
			return (usPosX + gPopup->getBoxDimensions().iRight);

		return usPosX;
	}

	virtual void SetPos(UINT16 usX, UINT16 usY)
	{
		// same y, different x
		usPosX = usX;
		gPopup->setPosition( usPosX, usY );

		fInitialized = TRUE;
		gPopup->show();
	}

	void SetupPopup(CHAR* name);

	POPUP* GetPopup()
	{
		return gPopup;
	}

	virtual void Setup( UINT32 aVal ) {}
	virtual void Functions( UINT32 aVal ) {}

private:
	BOOLEAN fInitialized;
	UINT16  usPosX;
	POPUP*  gPopup;
};

// select a vehicle seat (and possibly other options)
class VehicleSelection : public VehicleMenuItem
{
public:
	VehicleSelection()	:	usSector(0)	{}

	void Setup( UINT32 aVal );
	void Functions( UINT32 aVal );

private:
	UINT32 usSector;
};


void VehicleMenu( INT32 usMapPos, SOLDIERTYPE *pSoldier, SOLDIERTYPE *pVehicle );

#endif