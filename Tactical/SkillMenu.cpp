/** 
 * @file
 * @author Flugente (bears-pit.com)
 */

#include "SkillMenu.h"
#include "Soldier Profile Type.h"
#include "Overhead.h"
#include "Text.h"
#include "Isometric Utils.h"
#include "DisplayCover.h"
#include "worldman.h"

UINT16 usTraitMenuPosX = 0;
UINT16 usTraitMenuPosY = 0;

INT32	sTraitsMenuTargetGridNo = NOWHERE;

void Hide( void )
{
	// set off display of trait range/target gridno
	ToggleTraitRangeView(FALSE);

	// Dirty!
	fMapPanelDirty = TRUE;
}

// Turns an option off by always returning FALSE
BOOLEAN Popup_OptionOff( void )	{ return FALSE; }

void
SkillMenuItem::SetupPopup(CHAR* name)
{
	// create a popup
	gPopup = new POPUP(name);
	
	// add a callback that lets the keyboard handler know we're done (and ready to pop up again)
	gPopup->setCallback(POPUP_CALLBACK_HIDE, new popupCallbackFunction<void, void>( &Hide ) );
}

//	Different SkillItem classes. We need wrappers for each one, as we cannot pass a pointer to a member function (we need a concrete object)

/////////////////////////////// Trait Selection ////////////////////////////////////////////
TraitSelection	gTraitSelection;

void Wrapper_Setup_TraitSelection( UINT32 aVal)			{	gTraitSelection.Setup(aVal);	}
void Wrapper_Cancel_TraitSelection( UINT32 aVal )		{	gTraitSelection.Cancel();	}
/////////////////////////////// Trait Selection ////////////////////////////////////////////

/////////////////////////////// Skill Selection ////////////////////////////////////////////
SkillSelection	gSkillSelection;

void Wrapper_Function_SkillSelection( UINT32 aVal)		{	gSkillSelection.Functions(aVal);	}
void Wrapper_Setup_SkillSelection( UINT32 aVal)			{	gSkillSelection.Setup(aVal);	}
void Wrapper_Cancel_SkillSelection( UINT32 aVal )		{	gSkillSelection.Cancel();	}
/////////////////////////////// Skill Selection ////////////////////////////////////////////

/////////////////////////////// Artillery Sector Selection ////////////////////////////////////////////
ArtillerySector	gArtillerySector;

//void Wrapper_Function_ArtillerySector( UINT32 aVal)		{	gArtillerySector.Functions(aVal);	}
void Wrapper_Setup_ArtillerySector( UINT32 aVal)		{	gArtillerySector.Setup(aVal);	}
void Wrapper_Cancel_ArtillerySector( UINT32 aVal )		{	gArtillerySector.Cancel();	}
/////////////////////////////// Artillery Sector Selection ////////////////////////////////////////////

/////////////////////////////// Artillery Team Selection ////////////////////////////////////////////
ArtilleryTeam	gArtilleryTeam;

void Wrapper_Function_ArtilleryTeam( UINT32 aVal)		{	gArtilleryTeam.Functions(aVal);	}
void Wrapper_Setup_ArtilleryTeam( UINT32 aVal)			{	gArtilleryTeam.Setup(aVal);	}
void Wrapper_Cancel_ArtilleryTeam( UINT32 aVal )		{	gArtilleryTeam.Cancel();	}
/////////////////////////////// Artillery Team Selection ////////////////////////////////////////////

/////////////////////////////// Reinforcement Sector Selection ////////////////////////////////////////////
ReinforcementSector	gReinforcementSector;

//void Wrapper_Function_ReinforcementSector( UINT32 aVal)		{	gReinforcementSector.Functions(aVal);	}
void Wrapper_Setup_ReinforcementSector( UINT32 aVal)		{	gReinforcementSector.Setup(aVal);	}
void Wrapper_Cancel_ReinforcementSector( UINT32 aVal )		{	gReinforcementSector.Cancel();	}
/////////////////////////////// Reinforcement Sector Selection ////////////////////////////////////////////

/////////////////////////////// Reinforcement Number Selection ////////////////////////////////////////////
ReinforcementNumber	gReinforcementNumber;

void Wrapper_Function_ReinforcementNumber( UINT32 aVal)		{	gReinforcementNumber.Functions(aVal);	}
void Wrapper_Setup_ReinforcementNumber( UINT32 aVal)		{	gReinforcementNumber.Setup(aVal);	}
void Wrapper_Cancel_ReinforcementNumber( UINT32 aVal )		{	gReinforcementNumber.Cancel();	}
/////////////////////////////// Reinforcement Number Selection ////////////////////////////////////////////

/////////////////////////////// Soldier Target Selection ////////////////////////////////////////////
SoldierSelection	gSoldierSelection;

void Wrapper_Function_SoldierSelection( UINT32 aVal)	{	gSoldierSelection.Functions(aVal);	}
void Wrapper_Setup_SoldierSelection( UINT32 aVal)		{	gSoldierSelection.Setup(aVal);	}
void Wrapper_Cancel_SoldierSelection( UINT32 aVal )		{	gSoldierSelection.Cancel();	}
/////////////////////////////// Soldier Target Selection ////////////////////////////////////////////


/////////////////////////////// Trait Selection ////////////////////////////////////////////
void
TraitSelection::Setup( UINT32 aVal )
{
	Destroy();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	SetupPopup("TraitSelection");

	POPUP_OPTION *pOption;
	
	CHAR16 pStr[300];

	// create entries for the sub-menus for each trait
	const UINT8 num = 2;
	UINT8 traitarray[num];
	traitarray[0] = RADIO_OPERATOR_NT;
	traitarray[1] = VARIOUSSKILLS;
	for ( int i = 0; i < num; ++i)
	{
		swprintf( pStr, gzMercSkillTextNew[traitarray[i]] );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Setup_SkillSelection, traitarray[i] ) );

		// if we cannot perform this skill, grey it out
		if ( !HAS_SKILL_TRAIT(pSoldier, traitarray[i]) )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );
	}

	// cancel option
	swprintf( pStr, L"Cancel" );				
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Cancel_TraitSelection, 0 ) );
	GetPopup()->addOption( *pOption );
		
	// grab soldier's x,y screen position
	INT16 sX, sY;
	GetSoldierScreenPos( pSoldier, &sX, &sY );
		
	if( sX < 0 ) sX = 0;
	if( sY < 0 ) sY = 0;

	usTraitMenuPosX = sX + 30;		
	usTraitMenuPosY = sY;

	if ( ( usTraitMenuPosX + 400 ) > SCREEN_WIDTH )
		usTraitMenuPosX = SCREEN_WIDTH - 400;

	if ( ( usTraitMenuPosY + 130 ) > SCREEN_HEIGHT )
		usTraitMenuPosY = SCREEN_HEIGHT - 190;

	SetPos(usTraitMenuPosX, usTraitMenuPosY);
}
/////////////////////////////// Trait Selection ////////////////////////////////////////////


/////////////////////////////// Skill Selection ////////////////////////////////////////////
void
SkillSelection::Setup( UINT32 aVal )
{
	Destroy();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	if ( HAS_SKILL_TRAIT(pSoldier, aVal) )
	{
		SetupPopup("SkillSelection");
		
		POPUP_OPTION *pOption;

		CHAR16 pStr[300];

		SetTraitToDisplay(aVal);
		SetGridNoForTraitDisplay(sTraitsMenuTargetGridNo);

		// this switch isn't really necessary. But dividing the skills into menus for each trait gives a better overview, and looks way better than a huge list
		switch ( aVal )
		{
		case RADIO_OPERATOR_NT:
			{
				for(UINT32 uiCounter = SKILLS_RADIO_FIRST; uiCounter <= SKILLS_RADIO_LAST; ++uiCounter)
				{
					swprintf( pStr, pTraitSkillsMenuStrings[uiCounter] );

					if ( uiCounter == SKILLS_RADIO_ARTILLERY)
						pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Setup_ArtillerySector, uiCounter ) );
					else if ( uiCounter == SKILLS_RADIO_CALLREINFORCEMENTS)
						pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Setup_ReinforcementSector, uiCounter ) );
					else
						pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Function_SkillSelection, uiCounter ) );

					// if we cannot perform this skill, grey it out
					if ( !(pSoldier->CanUseSkill(uiCounter, TRUE)) )
					{
						// Set this option off.
						pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
					}
					
					GetPopup()->addOption( *pOption );
				}
			}
			break;

		case VARIOUSSKILLS:
			{
				for(UINT32 uiCounter = SKILLS_VARIOUS_FIRST; uiCounter <= SKILLS_VARIOUS_LAST; ++uiCounter)
				{
					swprintf( pStr, pTraitSkillsMenuStrings[uiCounter] );

					pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Function_SkillSelection, uiCounter ) );

					// if we cannot perform this skill, grey it out
					if ( !(pSoldier->CanUseSkill(uiCounter, TRUE)) )
					{
						// Set this option off.
						pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
					}
					
					GetPopup()->addOption( *pOption );
				}

				SetTraitToDisplay(VARIOUSSKILLS);
				SetGridNoForTraitDisplay(sTraitsMenuTargetGridNo);
				ToggleTraitRangeView(TRUE);
			}
			break;
		
		default:
			break;
		}
				
		// cancel option
		swprintf( pStr, L"Cancel" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Cancel_SkillSelection, 0 ) );
		GetPopup()->addOption( *pOption );
	}

	// same y, different x
	SetPos(gTraitSelection.GetMaxPosX(), usTraitMenuPosY);

	if ( HAS_SKILL_TRAIT(pSoldier, aVal) )
	{
		UINT8 cnt = 0;
		switch ( aVal )
		{
			case RADIO_OPERATOR_NT:
			{
				for(UINT32 uiCounter = SKILLS_RADIO_FIRST; uiCounter <= SKILLS_RADIO_LAST; ++uiCounter)
				{
					SetRegionFastHelpText( &(GetPopup()->MenuRegion[cnt++]), pSoldier->PrintSkillDesc(uiCounter) );
				}
			}
			break;

			case VARIOUSSKILLS:
			{
				for(UINT32 uiCounter = SKILLS_VARIOUS_FIRST; uiCounter <= SKILLS_VARIOUS_LAST; ++uiCounter)
				{
					SetRegionFastHelpText( &(GetPopup()->MenuRegion[cnt++]), pSoldier->PrintSkillDesc(uiCounter) );
				}
			}
			break;
		}
	}
}

void
SkillSelection::Functions( UINT32 aVal )
{
	Cancel();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;
	
	UINT8 ubID = WhoIsThere2(sTraitsMenuTargetGridNo, 0 );

	pSoldier->UseSkill(aVal, sTraitsMenuTargetGridNo, ubID);
		
	Cancel();
	gTraitSelection.Cancel();
}
/////////////////////////////// Skill Selection ////////////////////////////////////////////

/////////////////////////////// Artillery Sector Selection ////////////////////////////////////////////
void
ArtillerySector::Setup( UINT32 aVal )
{
	Destroy();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	if ( pSoldier->CanUseSkill(SKILLS_RADIO_ARTILLERY) )
	{
		SetupPopup("ArtillerySector");
		
		POPUP_OPTION *pOption;
			
		CHAR16 pStr[300];
				
		// check wether we can call artillery from the 4 adjacent sectors
		for (UINT8 i = 0; i < 4; ++i)
		{
			INT16 loopX = pSoldier->sSectorX;
			INT16 loopY = pSoldier->sSectorY;

			if ( i == 0 )		++loopY;
			else if ( i == 1 )	++loopX;
			else if ( i == 2 )	--loopY;
			else if ( i == 3 )	--loopX;

			if ( loopX < 1 || loopX >= MAP_WORLD_X - 1 || loopY < 1 || loopY >= MAP_WORLD_Y - 1 )
				continue;

			UINT32 sectornr = (UINT32)SECTOR( loopX, loopY );
		
			swprintf( pStr, L"%c%d", loopY + 'A' - 1, loopX );

			pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Setup_ArtilleryTeam, sectornr ) );

			// grey out if no artillery can be called from this sector
			if ( !IsValidArtilleryOrderSector( loopX, loopY, pSoldier->bSectorZ, pSoldier->bTeam ) && !IsValidArtilleryOrderSector( loopX, loopY, pSoldier->bSectorZ, MILITIA_TEAM ) )
			{
				// Set this option off.
				pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
			}
			GetPopup()->addOption( *pOption );
		}

		// cancel option
		swprintf( pStr, L"Cancel" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Cancel_ArtillerySector, 0 ) );
		GetPopup()->addOption( *pOption );

		// certain traits have skills whose effects depend on wether someone is near to us (squadleader, commissar). We therefore display our radius of effect while this display is open
		SetTraitToDisplay(RADIO_OPERATOR_NT);
		SetGridNoForTraitDisplay(sTraitsMenuTargetGridNo);
		ToggleTraitRangeView(TRUE);
	}

	// same y, different x
	SetPos(gSkillSelection.GetMaxPosX(), usTraitMenuPosY);
}
/////////////////////////////// Artillery Sector Selection ////////////////////////////////////////////

/////////////////////////////// Artillery Team Selection ////////////////////////////////////////////
void
ArtilleryTeam::Setup( UINT32 aVal )
{
	Destroy();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	if ( pSoldier->CanUseSkill(SKILLS_RADIO_ARTILLERY) && pSoldier->bSectorZ == 0)
	{		
		usSector = aVal;
		
		SetupPopup("ArtilleryTeam");

		POPUP_OPTION *pOption;
			
		CHAR16 pStr[300];

		// determine sector coordinates
		INT16 sSectorX = SECTORX(usSector);
		INT16 sSectorY = SECTORY(usSector);
				
		// order artillery from militia
		swprintf( pStr, L"Militia" );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ArtilleryTeam, MILITIA_TEAM ) );

		// grey out if no ArtilleryTeam can be called from this sector
		if ( !IsValidArtilleryOrderSector( sSectorX, sSectorY, pSoldier->bSectorZ, MILITIA_TEAM ) )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}
		GetPopup()->addOption( *pOption );

		// order artillery from our mercs
		swprintf( pStr, L"Other Squads" );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, INT8>( &Wrapper_Function_ArtilleryTeam, pSoldier->bTeam ) );

		// grey out if no ArtilleryTeam can be called from this sector
		if ( !IsValidArtilleryOrderSector( sSectorX, sSectorY, pSoldier->bSectorZ, pSoldier->bTeam ) )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}
		GetPopup()->addOption( *pOption );
		
		// cancel option
		swprintf( pStr, L"Cancel" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Cancel_ArtilleryTeam, 0 ) );
		GetPopup()->addOption( *pOption );

		// certain traits have skills whose effects depend on wether someone is near to us (squadleader, commissar). We therefore display our radius of effect while this display is open
		SetTraitToDisplay(RADIO_OPERATOR_NT);
		SetGridNoForTraitDisplay(sTraitsMenuTargetGridNo);
		ToggleTraitRangeView(TRUE);
	}

	// same y, different x
	SetPos(gArtillerySector.GetMaxPosX(), usTraitMenuPosY);
}

void
ArtilleryTeam::Functions( UINT32 aVal )
{
	Cancel();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL || !pSoldier->CanUseSkill(SKILLS_RADIO_ARTILLERY) )
		return;
	
	pSoldier->OrderArtilleryStrike(usSector, sTraitsMenuTargetGridNo, (UINT8)(aVal));

	Cancel();
	gArtillerySector.Cancel();
	gSkillSelection.Cancel();
	gTraitSelection.Cancel();
}
/////////////////////////////// Artillery Team Selection ////////////////////////////////////////////

/////////////////////////////// Reinforcement Sector Selection ////////////////////////////////////////////
extern UINT8 CountAllMilitiaInSector(INT16 sMapX, INT16 sMapY);

void
ReinforcementSector::Setup( UINT32 aVal )
{
	Destroy();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	if ( pSoldier->CanUseSkill(aVal) )
	{
		SetupPopup("Reinforcements sector");
		
		POPUP_OPTION *pOption;

		CHAR16 pStr[300];
				
		// check wether we can call artillery from the 4 adjacent sectors
		for (UINT8 i = 0; i < 4; ++i)
		{
			INT16 loopX = pSoldier->sSectorX;
			INT16 loopY = pSoldier->sSectorY;

			if ( i == 0 )		++loopY;
			else if ( i == 1 )	++loopX;
			else if ( i == 2 )	--loopY;
			else if ( i == 3 )	--loopX;

			if ( loopX < 1 || loopX >= MAP_WORLD_X - 1 || loopY < 1 || loopY >= MAP_WORLD_Y - 1 )
				continue;

			UINT32 sectornr = (UINT32)SECTOR( loopX, loopY );
		
			swprintf( pStr, L"%c%d", loopY + 'A' - 1, loopX );

			pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Setup_ReinforcementNumber, sectornr ) );

			// grey out if no reinforcements can be called from this sector
			if( !CountAllMilitiaInSector( loopX, loopY ) )
			{
				// Set this option off.
				pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
			}
			GetPopup()->addOption( *pOption );
		}

		// cancel option
		swprintf( pStr, L"Cancel" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Cancel_ReinforcementSector, 0 ) );
		GetPopup()->addOption( *pOption );
	}

	// same y, different x
	SetPos(gSkillSelection.GetMaxPosX(), usTraitMenuPosY);
}
/////////////////////////////// Reinforcement Sector Selection ////////////////////////////////////////////

/////////////////////////////// Reinforcement Number Selection ////////////////////////////////////////////
void
ReinforcementNumber::Setup( UINT32 aVal )
{
	Destroy();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	if ( pSoldier->CanUseSkill(SKILLS_RADIO_CALLREINFORCEMENTS) )
	{
		usSector = aVal;

		SetupPopup("Reinforcements number submenu");
	
		POPUP_OPTION *pOption;

		CHAR16 pStr[300];
		
		UINT8 numberofmilitia = CountAllMilitiaInSector( SECTORX(usSector), SECTORY(usSector) );

		// 5 militia option
		swprintf( pStr, L"5 Militia" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 5 ) );
		
		if ( numberofmilitia < 5 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );

		// 10 militia option
		swprintf( pStr, L"10 Militia" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 10 ) );

		if ( numberofmilitia < 10 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );

		// 15 militia option
		swprintf( pStr, L"15 Militia" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 15 ) );

		if ( numberofmilitia < 15 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );

		// 20 militia option
		swprintf( pStr, L"20 Militia" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 20 ) );

		if ( numberofmilitia < 20 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );

		// all militia option
		swprintf( pStr, L"All Militia" );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, numberofmilitia ) );
		GetPopup()->addOption( *pOption );

		// cancel option
		swprintf( pStr, L"Cancel" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Cancel_ReinforcementNumber, 0 ) );
		GetPopup()->addOption( *pOption );
	}

	// same y, different x
	SetPos(gReinforcementSector.GetMaxPosX(), usTraitMenuPosY);
}

void
ReinforcementNumber::Functions( UINT32 aVal )
{
	Cancel();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );
		
	if ( pSoldier == NULL || !pSoldier->CanUseSkill(SKILLS_RADIO_CALLREINFORCEMENTS) )
		return;
	
	pSoldier->RadioCallReinforcements(usSector, aVal);

	Cancel();
	gReinforcementSector.Cancel();
	gSkillSelection.Cancel();
	gTraitSelection.Cancel();
}
/////////////////////////////// Reinforcement Number Selection ////////////////////////////////////////////

/////////////////////////////// Soldier Target Selection ////////////////////////////////////////////
void
SoldierSelection::Setup( UINT32 aVal )
{
	Destroy();
	
	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	if ( pSoldier->CanUseSkill(aVal) )
	{
		usSkill = aVal;

		SetupPopup("SoldierSelection");
		
		POPUP_OPTION *pOption;
	
		CHAR16 pStr[300];

		// pretty simple: we find every soldier in a radius around the target position and add him to the list
		// loop through all soldiers around
		for ( UINT32 cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID ; cnt <= gTacticalStatus.Team[ CIV_TEAM ].bLastID ; ++cnt )
		{
			INT32 iRange = GetRangeInCellCoordsFromGridNoDiff( sTraitsMenuTargetGridNo, MercPtrs[ cnt ]->sGridNo );

			if ( iRange < 100 )
			{
				if ( cnt != pSoldier->ubID )
				{
					swprintf( pStr, L"%s", MercPtrs[ cnt ]->GetName() );

					pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT8>( &Wrapper_Function_SoldierSelection, cnt ) );

					// grey out if no artillery can be called from this sector
					if ( 0 )
					{
						// Set this option off.
						pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
					}
					GetPopup()->addOption( *pOption );
				}
			}
		}

		// cancel option
		swprintf( pStr, L"Cancel" );				
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Cancel_SoldierSelection, 0 ) );
		GetPopup()->addOption( *pOption );
	}

	// same y, different x
	SetPos(gSkillSelection.GetMaxPosX(), usTraitMenuPosY);
}

void
SoldierSelection::Functions( UINT32 aVal )
{
	Cancel();

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	pSoldier->UseSkill(usSkill, sTraitsMenuTargetGridNo, aVal);

	Cancel();
	gSkillSelection.Cancel();
	gTraitSelection.Cancel();
}
/////////////////////////////// Soldier Target Selection ////////////////////////////////////////////


/**
 *	Flugente: traits menu popup box
 *  This will create other boxes when needed
 */
void TraitsMenu( INT32 usMapPos )
{
	sTraitsMenuTargetGridNo = usMapPos;

	gTraitSelection.Setup(0);
}