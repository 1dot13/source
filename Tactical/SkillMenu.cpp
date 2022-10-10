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
#include "Queen Command.h"
#include "strategicmap.h"
#include "Map Screen Interface.h"
#include "Rotting Corpses.h"
#include "Dialogue Control.h"
#include "Handle Items.h"

extern void GetEquipmentTemplates( );

std::vector<std::string> gTemplateVector;

extern void ReadEquipmentTable( SOLDIERTYPE* pSoldier, std::string name );

extern INT16 bSelectedInfoChar;

// sevenfm: need this for correct calculation of traits menu position
extern INT16 gsInterfaceLevel;
extern void BeginAutoBandage();

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

void Wrapper_Setup_AutobandageSelection(UINT32 aVal)
{
	gSkillSelection.Cancel();
	gTraitSelection.Cancel();

	BeginAutoBandage();
}
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

/////////////////////////////// Drag Selection ////////////////////////////////////////////
DragSelection	gDragSelection;

void Wrapper_Function_DragSelection( UINT32 aVal)		{ gDragSelection.Functions(aVal);	}
void Wrapper_Function_DragSelection_GridNo( INT32 aVal ){ gDragSelection.FunctionsGridNo( aVal ); }
void Wrapper_Setup_DragSelection( UINT32 aVal )			{ gDragSelection.Setup( aVal ); }
void Wrapper_Cancel_DragSelection( UINT32 aVal )		{ gDragSelection.Cancel( ); }
/////////////////////////////// Drag Selection ////////////////////////////////////////////

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
	const UINT8 num = 5;
	UINT8 traitarray[num];
	traitarray[0] = RADIO_OPERATOR_NT;
	traitarray[1] = INTEL;
	traitarray[2] = DISGUISE;
	traitarray[3] = VARIOUSSKILLS;
	traitarray[4] = AUTOBANDAGESKILLS;

	for ( int i = 0; i < num; ++i)
	{
		swprintf( pStr, gzMercSkillTextNew[traitarray[i]] );

		if (traitarray[i] == AUTOBANDAGESKILLS)
		{
			pOption = new POPUP_OPTION(&std::wstring(pStr), new popupCallbackFunction<void, UINT32>(&Wrapper_Setup_AutobandageSelection, traitarray[i]));
		}
		else
		{
			pOption = new POPUP_OPTION(&std::wstring(pStr), new popupCallbackFunction<void, UINT32>(&Wrapper_Setup_SkillSelection, traitarray[i]));
		}

		// if we cannot perform this skill, grey it out
		if ( !HAS_SKILL_TRAIT(pSoldier, traitarray[i]) )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );
	}

	// cancel option
	swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &Wrapper_Cancel_TraitSelection, 0 ) );
	GetPopup()->addOption( *pOption );
		
	// grab soldier's x,y screen position
	INT16 sX, sY;
	// sevenfm: changed TraitsMenu position from soldier to mouse
	//GetSoldierScreenPos( pSoldier, &sX, &sY );
	GetGridNoScreenPos( sTraitsMenuTargetGridNo, (UINT8)gsInterfaceLevel, &sX, &sY );
		
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

		case INTEL:
			{
				for ( UINT32 uiCounter = SKILLS_INTEL_FIRST; uiCounter <= SKILLS_INTEL_LAST; ++uiCounter )
				{
					swprintf( pStr, pTraitSkillsMenuStrings[uiCounter] );

					pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_SkillSelection, uiCounter ) );

					// if we cannot perform this skill, grey it out
					if ( !( pSoldier->CanUseSkill( uiCounter, TRUE, sTraitsMenuTargetGridNo ) ) )
					{
						// Set this option off.
						pOption->setAvail( new popupCallbackFunction<bool, void*>( &Popup_OptionOff, NULL ) );
					}

					GetPopup()->addOption( *pOption );
				}
			}
			break;

		case DISGUISE:
			{
				for ( UINT32 uiCounter = SKILLS_DISGUISE_FIRST; uiCounter <= SKILLS_DISGUISE_LAST; ++uiCounter )
				{
					swprintf( pStr, pTraitSkillsMenuStrings[uiCounter] );

					pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_SkillSelection, uiCounter ) );

					// if we cannot perform this skill, grey it out
					if ( !( pSoldier->CanUseSkill( uiCounter, TRUE, sTraitsMenuTargetGridNo ) ) )
					{
						// Set this option off.
						pOption->setAvail( new popupCallbackFunction<bool, void*>( &Popup_OptionOff, NULL ) );
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

					if ( uiCounter == SKILLS_DRAG )
						pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Setup_DragSelection, uiCounter ) );
					else
						pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_SkillSelection, uiCounter ) );

					// if we cannot perform this skill, grey it out
					if ( !(pSoldier->CanUseSkill( uiCounter, TRUE, sTraitsMenuTargetGridNo )) )
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
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
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
					SetRegionFastHelpText( &(GetPopup()->MenuRegion[cnt++]), pSoldier->PrintSkillDesc( uiCounter, sTraitsMenuTargetGridNo ) );
				}
			}
			break;

			case INTEL:
			{
				for ( UINT32 uiCounter = SKILLS_INTEL_FIRST; uiCounter <= SKILLS_INTEL_LAST; ++uiCounter )
				{
					SetRegionFastHelpText( &( GetPopup()->MenuRegion[cnt++] ), pSoldier->PrintSkillDesc( uiCounter, sTraitsMenuTargetGridNo ) );
				}
			}
			break;

			case DISGUISE:
				for ( UINT32 uiCounter = SKILLS_DISGUISE_FIRST; uiCounter <= SKILLS_DISGUISE_LAST; ++uiCounter )
				{
					SetRegionFastHelpText( &( GetPopup()->MenuRegion[cnt++] ), pSoldier->PrintSkillDesc( uiCounter, sTraitsMenuTargetGridNo ) );
				}
				break;

			case VARIOUSSKILLS:
			{
				for(UINT32 uiCounter = SKILLS_VARIOUS_FIRST; uiCounter <= SKILLS_VARIOUS_LAST; ++uiCounter)
				{
					SetRegionFastHelpText( &(GetPopup()->MenuRegion[cnt++]), pSoldier->PrintSkillDesc(uiCounter, sTraitsMenuTargetGridNo ) );
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
	
	UINT16 ubID = WhoIsThere2(sTraitsMenuTargetGridNo, 0 );

	BOOLEAN result = pSoldier->UseSkill(aVal, sTraitsMenuTargetGridNo, ubID);

	// additional dialogue
	AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_SKILL_RESULT, aVal, result );
		
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
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
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
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_MILITIA] );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ArtilleryTeam, MILITIA_TEAM ) );

		// grey out if no ArtilleryTeam can be called from this sector
		if ( !IsValidArtilleryOrderSector( sSectorX, sSectorY, pSoldier->bSectorZ, MILITIA_TEAM ) )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}
		GetPopup()->addOption( *pOption );

		// order artillery from our mercs
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_OTHERSQUADS] );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, INT8>( &Wrapper_Function_ArtilleryTeam, pSoldier->bTeam ) );

		// grey out if no ArtilleryTeam can be called from this sector
		if ( !IsValidArtilleryOrderSector( sSectorX, sSectorY, pSoldier->bSectorZ, pSoldier->bTeam ) )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}
		GetPopup()->addOption( *pOption );
		
		// cancel option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
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
	
	BOOLEAN result = pSoldier->OrderArtilleryStrike(usSector, sTraitsMenuTargetGridNo, (UINT8)(aVal));

	// additional dialogue
	AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_SKILL_RESULT, SKILLS_RADIO_ARTILLERY, result );

	Cancel();
	gArtillerySector.Cancel();
	gSkillSelection.Cancel();
	gTraitSelection.Cancel();
}
/////////////////////////////// Artillery Team Selection ////////////////////////////////////////////

/////////////////////////////// Reinforcement Sector Selection ////////////////////////////////////////////
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
			if ( !CanRequestMilitiaReinforcements( pSoldier->sSectorX, pSoldier->sSectorY, loopX, loopY ) )
			{
				// Set this option off.
				pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
			}
			GetPopup()->addOption( *pOption );
		}

		// cancel option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
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
		
		UINT8 numberofmilitia = NumNonPlayerTeamMembersInSector( SECTORX( usSector ), SECTORY( usSector ), MILITIA_TEAM );
		
		// 5 militia option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_X_MILITIA], 5 );
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 5 ) );
		
		if ( numberofmilitia < 5 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );

		// 10 militia option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_X_MILITIA], 10 );
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 10 ) );

		if ( numberofmilitia < 10 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );

		// 15 militia option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_X_MILITIA], 15 );
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 15 ) );

		if ( numberofmilitia < 15 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		GetPopup()->addOption( *pOption );

		// 20 militia option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_X_MILITIA], 20 );
		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 20 ) );

		if ( numberofmilitia < 20 )
		{
			// Set this option off.
			pOption->setAvail(new popupCallbackFunction<bool,void*>( &Popup_OptionOff, NULL ));
		}

		// 30 militia option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_X_MILITIA], 30 );
		pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 30 ) );

		if ( numberofmilitia < 30 )
		{
			// Set this option off.
			pOption->setAvail( new popupCallbackFunction<bool, void*>( &Popup_OptionOff, NULL ) );
		}

		// 40 militia option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_X_MILITIA], 40 );
		pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, 40 ) );

		if ( numberofmilitia < 40 )
		{
			// Set this option off.
			pOption->setAvail( new popupCallbackFunction<bool, void*>( &Popup_OptionOff, NULL ) );
		}

		GetPopup()->addOption( *pOption );

		// all militia option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_ALL_MILITIA] );

		pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_ReinforcementNumber, numberofmilitia ) );
		GetPopup()->addOption( *pOption );

		// cancel option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
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
	
	BOOLEAN result = pSoldier->RadioCallReinforcements(usSector, aVal);

	// additional dialogue
	AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_SKILL_RESULT, SKILLS_RADIO_CALLREINFORCEMENTS, result );

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
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
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

	BOOLEAN result = pSoldier->UseSkill(usSkill, sTraitsMenuTargetGridNo, aVal);

	// additional dialogue
	AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_SKILL_RESULT, usSkill, result );

	Cancel();
	gSkillSelection.Cancel();
	gTraitSelection.Cancel();
}
/////////////////////////////// Soldier Target Selection ////////////////////////////////////////////

/////////////////////////////// Drag Selection ////////////////////////////////////////////
void
DragSelection::Setup( UINT32 aVal )
{
	Destroy( );

	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier == NULL )
		return;

	if ( pSoldier->CanUseSkill( aVal ) )
	{
		usSkill = aVal;

		SetupPopup( "DragSelection" );

		POPUP_OPTION *pOption;

		CHAR16 pStr[300];

		// pretty simple: we find every soldier in a radius around the target position and add him to the list
		// loop through all soldiers around
		for ( UINT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[CIV_TEAM].bLastID; ++cnt )
		{
			if ( cnt != pSoldier->ubID && pSoldier->CanDragPerson(cnt) )
			{
				swprintf( pStr, L"%s", MercPtrs[cnt]->GetName( ) );

				pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_DragSelection, cnt ) );
				
				GetPopup( )->addOption( *pOption );
			}
		}

		// corpses
		std::vector<INT16> corpseids = GetCorpseIDsNearGridNo( pSoldier->sGridNo, pSoldier->pathing.bLevel, 1 );

		for ( std::vector<INT16>::iterator it = corpseids.begin(); it != corpseids.end(); ++it )
		{
			ROTTING_CORPSE* pCorpse = GetRottingCorpse( (*it) );

			swprintf( pStr, pSkillMenuStrings[SKILLMENU_CORPSES], pCorpse->name );

			// we have to use an offset of NOBODY in order to differentiate between person and corpse
			pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_DragSelection, (*it) + NOBODY ) );
			
			GetPopup( )->addOption( *pOption );
		}

		// gridno
		for ( int ubDirection = 0; ubDirection < NUM_WORLD_DIRECTIONS; ++ubDirection )
		{
			INT32 sTempGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( ubDirection ) );

			UINT32 tiletype;
			UINT16 structurenumber;
			UINT8 hitpoints;
			UINT8 decalflag;
			if ( pSoldier->CanDragStructure( sTempGridNo )
				&& IsDragStructurePresent( sTempGridNo, pSoldier->pathing.bLevel, tiletype, structurenumber, hitpoints, decalflag ) )
			{
				int xmlentry;
				GetDragStructureXmlEntry( tiletype, structurenumber, xmlentry );

				if ( xmlentry >= 0 )
				{
					swprintf( pStr, L"%hs (%s)", gStructureMovePossible[xmlentry].szTileSetDisplayName, FaceDirs[gOneCDirection[ubDirection]] );

					// we have to use an offset of NOBODY in order to differentiate between person and corpse
					pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_DragSelection_GridNo, sTempGridNo ) );

					GetPopup()->addOption( *pOption );
				}
			}
		}

		// cancel option
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_CANCEL] );
		pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Cancel_DragSelection, 0 ) );
		GetPopup( )->addOption( *pOption );
	}

	// same y, different x
	SetPos( gSkillSelection.GetMaxPosX( ), usTraitMenuPosY );
}

void
DragSelection::Functions( UINT32 aVal )
{
	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier )
	{
		BOOLEAN result = pSoldier->UseSkill( usSkill, NOWHERE, aVal );

		// additional dialogue
		AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_SKILL_RESULT, usSkill, result );
	}

	Cancel( );
	gSkillSelection.Cancel();
	gTraitSelection.Cancel( );
}

void
DragSelection::FunctionsGridNo( INT32 aVal )
{
	SOLDIERTYPE * pSoldier = NULL;

	GetSoldier( &pSoldier, gusSelectedSoldier );

	if ( pSoldier )
	{
		BOOLEAN result = pSoldier->UseSkill( usSkill, aVal, 0 );

		// additional dialogue
		AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_SKILL_RESULT, usSkill, result );
	}

	Cancel();
	gSkillSelection.Cancel();
	gTraitSelection.Cancel();
}
/////////////////////////////// Drag Selection ////////////////////////////////////////////

/**
 *	Flugente: traits menu popup box
 *  This will create other boxes when needed
 */
void TraitsMenu( INT32 usMapPos )
{
	sTraitsMenuTargetGridNo = usMapPos;

	gTraitSelection.Setup(0);
}

/////////////////////////////// Trait Selection ////////////////////////////////////////////
EquipmentSelection	gEquipmentSelection;

void Wrapper_Function_EquipmentSelection(UINT32 aVal) { gEquipmentSelection.Functions(aVal); }
void Wrapper_Setup_EquipmentSelection(UINT32 aVal) { gEquipmentSelection.Setup(aVal); }
void Wrapper_Cancel_EquipmentSelection(UINT32 aVal) { gEquipmentSelection.Cancel(); }
/////////////////////////////// Trait Selection ////////////////////////////////////////////

/////////////////////////////// Trait Selection ////////////////////////////////////////////
UINT32 gEquipmentSelectionFirstEntryShown = 0;
UINT32 gEquipmentSelectionLastEntryShown = 0;
UINT32 gEquipmentSelectionMaxEntriesShown = 0;

void
EquipmentSelection::Setup(UINT32 aVal)
{
	Destroy();

	SetupPopup("EquipmentSelection");
	
	INT16 sX = 261 + 406 + xResOffset + 30;
	INT16 sY = 0 + 10 + yResOffset + 30;

	if ( sX < 0 ) sX = 0;
	if ( sY < 0 ) sY = 0;

	usTraitMenuPosX = sX + 30;
	usTraitMenuPosY = sY;

	if ( (usTraitMenuPosX + 400) > SCREEN_WIDTH )
		usTraitMenuPosX = SCREEN_WIDTH - 400;

	if ( (usTraitMenuPosY + 130) > SCREEN_HEIGHT )
		usTraitMenuPosY = SCREEN_HEIGHT - 190;

	POPUP_OPTION *pOption;

	CHAR16 pStr[300];

	// create entries for the sub-menus for each trait
	GetEquipmentTemplates();

	// we limit the maximum number of entries shown at once - too many entries could theoretically cause the button system to freak out
	gEquipmentSelectionMaxEntriesShown = min(40, max( 0, (SCREEN_HEIGHT - 150 - usTraitMenuPosY) / 12 ) );

	gEquipmentSelectionLastEntryShown = min( gEquipmentSelectionFirstEntryShown + gEquipmentSelectionMaxEntriesShown, gTemplateVector.size( ) );

	if ( gEquipmentSelectionLastEntryShown < gEquipmentSelectionMaxEntriesShown )
		gEquipmentSelectionFirstEntryShown = 0;
	else
		gEquipmentSelectionFirstEntryShown = gEquipmentSelectionLastEntryShown - gEquipmentSelectionMaxEntriesShown;
	
	UINT32 cnt = gEquipmentSelectionFirstEntryShown;
	for ( UINT32 i = gEquipmentSelectionFirstEntryShown; i < gEquipmentSelectionLastEntryShown; ++i )
	{
		//std::string str8 = (*it);
		std::string str8 = gTemplateVector[i];

		std::size_t found = str8.find_last_of( "." );

		std::string str82 = str8.substr( 0, found );
		
		const char* coca = str82.c_str( );

		CHAR16 tmpchar[256];

		int nChars = MultiByteToWideChar( CP_ACP, 0, coca, -1, NULL, 0 );
		MultiByteToWideChar( CP_UTF8, 0, coca, -1, tmpchar, nChars );

		pOption = new POPUP_OPTION( &std::wstring( tmpchar ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_EquipmentSelection, i ) );

		pOption->color_foreground = FONT_GREEN;

		GetPopup()->addOption(*pOption);
	}

	// 'more' option (moves down list)
	if ( gEquipmentSelectionMaxEntriesShown < gTemplateVector.size( ) )
	{
		swprintf( pStr, pSkillMenuStrings[SKILLMENU_MORE] );
		pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_EquipmentSelection, 99999 ) );
		GetPopup( )->addOption( *pOption );
	}
	
	// cancel option
	swprintf(pStr, pSkillMenuStrings[SKILLMENU_CANCEL]);
	pOption = new POPUP_OPTION( &std::wstring( pStr ), new popupCallbackFunction<void, UINT32>( &Wrapper_Function_EquipmentSelection, 99998 ) );
	GetPopup()->addOption(*pOption);
	
	SetPos(usTraitMenuPosX, usTraitMenuPosY);
}

void
EquipmentSelection::Functions(UINT32 aVal)
{
	// this special value allows to 'scroll down'
	if ( aVal == 99999 )
	{
		gEquipmentSelectionFirstEntryShown += gEquipmentSelectionMaxEntriesShown;

		if ( gEquipmentSelectionFirstEntryShown >= gTemplateVector.size( ) )
			gEquipmentSelectionFirstEntryShown = 0;

		Cancel( );

		EquipmentListMenu();
	}
	else if ( aVal == 99998 )
	{
		gEquipmentSelectionFirstEntryShown = 0;

		Cancel( );
	}
	else if ( aVal < gTemplateVector.size( ) )
	{
		SOLDIERTYPE* pSoldier = &Menptr[gCharactersList[bSelectedInfoChar].usSolID];
		if ( pSoldier )
		{
			std::string name = gTemplateVector[aVal];

			ReadEquipmentTable( pSoldier, name );
		}

		// Note that we don't close menu - this way we don't have to open it every time we want to apply a template
	}
}
/////////////////////////////// Trait Selection ////////////////////////////////////////////

void EquipmentListMenu()
{
	gEquipmentSelection.Setup(0);
}

void EquipmentListMenuCancel()
{
	gEquipmentSelection.Cancel();
}
