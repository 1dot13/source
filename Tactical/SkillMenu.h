#ifndef _SKILL_MENU_
#define _SKILL_MENU_

/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

#include "popup_class.h"

/**
 * @brief	A basic class for popup menus
 *			Use this to create mensu calling other menus, each having various sub-options
 *			I tried to make this as orderly as possible. It's ugly, but much more clean than the first solution, which was a hardcoded mess.
 *			You will, however, still need wrapper functions (we use callbacks on member functions), see Implementation.
 */
class SkillMenuItem
{
public:
	SkillMenuItem()	:
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

// select a trait
class TraitSelection : public SkillMenuItem
{
public:
	TraitSelection()	{}

	void Setup( UINT32 aVal );
};

// select a skill
class SkillSelection : public SkillMenuItem
{
public:
	SkillSelection()	:	usSector(0)	{}

	void Setup( UINT32 aVal );
	void Functions( UINT32 aVal );

private:
	UINT32 usSector;
};

// select sector to call artillery from
class ArtillerySector : public SkillMenuItem
{
public:
	ArtillerySector()	{}

	void Setup( UINT32 aVal );
};

// select team we request artillery from
class ArtilleryTeam : public SkillMenuItem
{
public:
	ArtilleryTeam()	:	usSector(0)	{}

	void Setup( UINT32 aVal );
	void Functions( UINT32 aVal );

private:
	UINT32 usSector;
};

// select the sector we call reinforcements from
class ReinforcementSector : public SkillMenuItem
{
public:
	ReinforcementSector()	{}

	void Setup( UINT32 aVal );
};

// select the number of milita that should reinforce
class ReinforcementNumber : public SkillMenuItem
{
public:
	ReinforcementNumber()	:	usSector(0)	{}

	void Setup( UINT32 aVal );
	void Functions( UINT32 aVal );

private:
	UINT32 usSector;
};

// select an individual soldier as target of a skill
class SoldierSelection : public SkillMenuItem
{
public:
	SoldierSelection()		:	usSkill(0)	{}

	void Setup( UINT32 aVal );
	void Functions( UINT32 aVal );

private:
	UINT32 usSkill;
};


/**
 *	Flugente: traits menu popup box
 *  This will create other boxes when needed
 */
void TraitsMenu( INT32 usMapPos );

#endif